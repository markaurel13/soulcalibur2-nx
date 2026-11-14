#include <switch.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cinttypes>
#include <vector>
#include <string>
#include <sys/iosupport.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <dirent.h>
#include <atomic>
#include "switch_watchdog.h"

namespace SwitchFS {
    bool Initialize();
    void Shutdown();
}

namespace SwitchInput {
    void Initialize();
    void ShowControllerApplet(int minPlayers, int maxPlayers);
}

extern "C" {
void recomp_dsound_api_shutdown(void);
void recomp_d3d8_shutdown(void);

// Override libnx weak exception stack with 64KB so stdio/fprintf in the exception handler never overflows
u64 __nx_exception_stack_size = 0x10000;
alignas(16) u8 __nx_exception_stack[0x10000];
}

int sc2_runner_main(int argc, char **argv);
extern "C" int recomp_dispatch_init(void);

static FILE* s_sd_log = nullptr;
static int s_nxlink_sock = -1;
static bool s_socket_initialized = false;
static bool s_cleaned_up = false;
static const devoptab_t* s_orig_stdout = nullptr;
static const devoptab_t* s_orig_stderr = nullptr;

static Mutex s_log_mutex;

struct ScopedLogMutex {
    ScopedLogMutex() { mutexLock(&s_log_mutex); }
    ~ScopedLogMutex() { mutexUnlock(&s_log_mutex); }
};

static char s_line_buf[2048];
static size_t s_line_len = 0;

extern "C" FILE* Switch_GetMainLogFile(void) {
    return s_sd_log;
}

static void flush_log_line(const char *line, size_t len) {
    if (len == 0) return;

    // Filter high-frequency per-frame runtime spam before touching SD card or nxlink
    if (memmem(line, len, "recomp d3d8: Clear", 18) != nullptr ||
        memmem(line, len, "recomp d3d8: Swap frame=", 24) != nullptr ||
        memmem(line, len, "recomp kernel: NtReadFile", 25) != nullptr) {
        return;
    }

    if (s_sd_log) {
        fwrite(line, 1, len, s_sd_log);
    }
    if (s_nxlink_sock >= 0) {
        send(s_nxlink_sock, line, len, MSG_DONTWAIT);
    }
}

extern "C" {
extern char __nx_mod0;
extern uint32_t sc2_scene;
}

extern "C" void __libnx_exception_handler(ThreadExceptionDump *dump) {
    static std::atomic<bool> s_in_handler(false);
    if (s_in_handler.exchange(true)) {
        svcExitProcess();
        return;
    }

    if (s_sd_log) {
        uintptr_t mod_base = (uintptr_t)&__nx_mod0 - 0x00000000000000f0;

        fprintf(s_sd_log, "\n========================================\n");
        fprintf(s_sd_log, "  ATMOSPHERE / LIBNX EXCEPTION CAUGHT!  \n");
        fprintf(s_sd_log, "========================================\n");
        fprintf(s_sd_log, "Module Base: 0x%016" PRIxPTR "\n", mod_base);
        fprintf(s_sd_log, "Error Desc:  0x%08" PRIx32 "\n", dump->error_desc);
        fprintf(s_sd_log, "PC:          0x%016" PRIx64 " (rel: +0x%08" PRIx64 ")\n",
                (uint64_t)dump->pc.x, (uint64_t)(dump->pc.x - mod_base));
        fprintf(s_sd_log, "LR:          0x%016" PRIx64 " (rel: +0x%08" PRIx64 ")\n",
                (uint64_t)dump->lr.x, (uint64_t)(dump->lr.x - mod_base));
        fprintf(s_sd_log, "SP:          0x%016" PRIx64 "\n", (uint64_t)dump->sp.x);
        fprintf(s_sd_log, "FP:          0x%016" PRIx64 "\n", (uint64_t)dump->fp.x);
        fprintf(s_sd_log, "FAR:         0x%016" PRIx64 "\n", (uint64_t)dump->far.x);
        fprintf(s_sd_log, "ESR:         0x%08" PRIx32 "\n", dump->esr);
        fprintf(s_sd_log, "PSTATE:      0x%08" PRIx32 "\n", dump->pstate);
        for (int i = 0; i < 28; i += 2) {
            fprintf(s_sd_log, "X%02d: 0x%016" PRIx64 "   X%02d: 0x%016" PRIx64 "\n",
                    i, (uint64_t)dump->cpu_gprs[i].x,
                    i + 1, (uint64_t)dump->cpu_gprs[i + 1].x);
        }
        fprintf(s_sd_log, "X28: 0x%016" PRIx64 "\n", (uint64_t)dump->cpu_gprs[28].x);

        if (dump->pc.x >= mod_base && dump->pc.x < mod_base + 0x1200000 && (dump->pc.x & 3) == 0) {
            fprintf(s_sd_log, "\nInstructions around PC:\n");
            const uint32_t *code = (const uint32_t *)(uintptr_t)dump->pc.x;
            for (int i = -3; i <= 3; ++i) {
                fprintf(s_sd_log, "  %s %p (rel +0x%08" PRIxPTR "): 0x%08" PRIx32 "\n",
                        (i == 0) ? "=>" : "  ",
                        (const void*)(code + i),
                        (uintptr_t)(code + i) - mod_base,
                        code[i]);
            }
        }

        if (dump->lr.x >= mod_base && dump->lr.x < mod_base + 0x1200000 && (dump->lr.x & 3) == 0) {
            fprintf(s_sd_log, "\nInstructions around LR:\n");
            const uint32_t *code = (const uint32_t *)(uintptr_t)dump->lr.x;
            for (int i = -3; i <= 3; ++i) {
                fprintf(s_sd_log, "  %s %p (rel +0x%08" PRIxPTR "): 0x%08" PRIx32 "\n",
                        (i == 0) ? "=>" : "  ",
                        (const void*)(code + i),
                        (uintptr_t)(code + i) - mod_base,
                        code[i]);
            }
        }

        fprintf(s_sd_log, "\nGuest Scene: %u\n", sc2_scene);
        fprintf(s_sd_log, "========================================\n");
        fflush(s_sd_log);
        fsync(fileno(s_sd_log));
    }

    // Exit immediately so Atmosphere kernel doesn't trap into an unhandled break loop
    svcExitProcess();
}

static ssize_t log_devoptab_write(struct _reent *r, void *fd, const char *ptr, size_t len) {
    (void)r; (void)fd;
    if (!ptr || len == 0) return 0;

    ScopedLogMutex lock;
    for (size_t i = 0; i < len; ++i) {
        char c = ptr[i];
        s_line_buf[s_line_len++] = c;
        if (c == '\n' || s_line_len >= sizeof(s_line_buf) - 1) {
            flush_log_line(s_line_buf, s_line_len);
            s_line_len = 0;
        }
    }
    return (ssize_t)len;
}

static devoptab_t s_log_devoptab = {
    "logtee",
    0,
    NULL, NULL,
    log_devoptab_write,
    NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    0,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL
};

static void cleanup_switch_services(void) {
    if (s_cleaned_up) return;
    s_cleaned_up = true;

    printf("[Main] Ejecutando limpieza ordenada de subsistemas (atexit)...\n");

    // 1. Shut down audio and 3D presenter while Deko3D / audren resources are still valid
    recomp_dsound_api_shutdown();
    recomp_d3d8_shutdown();

    // 2. Flush any remaining partial line in log buffer
    if (s_line_len > 0) {
        flush_log_line(s_line_buf, s_line_len);
        s_line_len = 0;
    }

    // 3. Restore default stdout/stderr devoptabs
    if (s_orig_stdout) devoptab_list[STD_OUT] = s_orig_stdout;
    if (s_orig_stderr) devoptab_list[STD_ERR] = s_orig_stderr;

    // 4. Close SD log file
    if (s_sd_log) {
        fflush(s_sd_log);
        fsync(fileno(s_sd_log));
        fclose(s_sd_log);
        s_sd_log = nullptr;
    }

    // 5. Close nxlink and release BSD socket TransferMemory before hbloader frees the heap!
    if (s_nxlink_sock >= 0) {
        close(s_nxlink_sock);
        s_nxlink_sock = -1;
    }
    if (s_socket_initialized) {
        socketExit();
        s_socket_initialized = false;
    }

    // 6. Unmount RomFS if mounted
    SwitchFS::Shutdown();
}

static bool file_exists(const std::string& path, size_t* out_size = nullptr) {
    struct stat st;
    if (stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode)) {
        if (out_size) *out_size = st.st_size;
        return true;
    }
    return false;
}

#ifdef main
#undef main
#endif

int main(int argc, char *argv[]) {
    // 1. Crear directorios base en la microSD
    mkdir("sdmc:/switch", 0777);
    mkdir("sdmc:/switch/soulcalibur2", 0777);
    mkdir("sdmc:/switch/soulcalibur2/disc", 0777);
    mkdir("sdmc:/switch/soulcalibur2/save", 0777);

    // 2. Abrir archivo de log persistente en microSD (buffer RAM de 64 KB sin fsync por linea)
    static char s_sd_log_buf[65536];
    s_sd_log = fopen("sdmc:/switch/soulcalibur2/soulcalibur2.log", "w");
    if (s_sd_log) {
        setvbuf(s_sd_log, s_sd_log_buf, _IOFBF, sizeof(s_sd_log_buf));
    }

    // 3. Inicializar soporte de sockets y conectar con nxlink en la PC
    Result sock_rc = socketInitializeDefault();
    if (R_SUCCEEDED(sock_rc)) {
        s_socket_initialized = true;
        s_nxlink_sock = nxlinkConnectToHost(false, false);
    }

    // 4. Instalar devoptab para redirigir stdout y stderr a SD + nxlink
    s_orig_stdout = devoptab_list[STD_OUT];
    s_orig_stderr = devoptab_list[STD_ERR];
    devoptab_list[STD_OUT] = &s_log_devoptab;
    devoptab_list[STD_ERR] = &s_log_devoptab;
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    // Registrar manejador atexit para liberar TransferMemory / CodeMemory incluso si recomp_stop() llama a exit()
    std::atexit(cleanup_switch_services);

    printf("\n========================================\n");
    printf("  Soulcalibur II (Xbox) - Switch Port  \n");
    printf("  Static Recompilation via Deko3D       \n");
    printf("  Build: %s %s\n", __DATE__, __TIME__);
    printf("========================================\n\n");

    printf("[Main] Red/Socket rc=0x%08X, nxlink_sock=%d\n", sock_rc, s_nxlink_sock);
    if (s_nxlink_sock >= 0) {
        printf("[Main] [+] nxlink conectado exitosamente a la PC!\n");
    } else {
        printf("[Main] [*] nxlink no conectado (modo directo desde tarjeta SD).\n");
    }
    if (s_sd_log) {
        printf("[Main] [+] Log persistente activo en 'sdmc:/switch/soulcalibur2/soulcalibur2.log'\n");
    } else {
        printf("[Main] [-] Error: no se pudo abrir log en SD!\n");
    }

    // 5. Montar RomFS si está disponible
    SwitchFS::Initialize();

    // 6. Inicializar subsistema de mandos
    SwitchInput::Initialize();

    // 7. Búsqueda automática de los archivos del juego
    std::string disc_root = "";
    std::string xbe_path = "";

    const std::vector<std::string> search_roots = {
        "sdmc:/switch/soulcalibur2",
        "sdmc:/switch/soulcalibur2/disc",
        "romfs:",
        "romfs:/disc"
    };

    const std::vector<std::string> xbe_names = {
        "Default.xbe", "default.xbe", "DEFAULT.XBE"
    };

    printf("[Main] Buscando archivos de juego de Xbox...\n");

    for (const auto& root : search_roots) {
        for (const auto& name : xbe_names) {
            std::string cand = root + "/" + name;
            size_t size = 0;
            if (file_exists(cand, &size)) {
                disc_root = root;
                xbe_path = cand;
                printf("[Main] [+] Encontrado XBE: %s (%zu KB)\n", xbe_path.c_str(), size / 1024);
                break;
            }
        }
        if (!disc_root.empty()) break;
    }

    if (disc_root.empty()) {
        printf("[Main] [ERROR FATAL] No se encontro 'Default.xbe'!\n");
        printf("[Main] Por favor coloca los archivos en 'sdmc:/switch/soulcalibur2/':\n");
        printf("       sdmc:/switch/soulcalibur2/Default.xbe\n");
        printf("       sdmc:/switch/soulcalibur2/root.olk\n");
        printf("       sdmc:/switch/soulcalibur2/movie.afs\n");
        printf("       sdmc:/switch/soulcalibur2/bgm.afs\n");
        cleanup_switch_services();
        return 1;
    }

    // Verificar root.olk y archivo de invitados (--guests)
    // Nota: El root.olk retail de Xbox mide ~661 MB. Un root.olk parcheado con Link y Heihachi
    // mide >700 MB (o se llama explícitamente guests.olk).
    size_t olk_size = 0;
    std::string guests_olk_path = "";
    if (file_exists(disc_root + "/guests.olk", &olk_size)) {
        guests_olk_path = disc_root + "/guests.olk";
        printf("[Main] [+] Encontrado guests.olk (%zu MB) para Link y Heihachi\n", olk_size / (1024 * 1024));
    } else if (file_exists(disc_root + "/root.olk", &olk_size)) {
        printf("[Main] [+] Encontrado root.olk (%zu MB)\n", olk_size / (1024 * 1024));
        if (olk_size > 700ULL * 1024ULL * 1024ULL) {
            guests_olk_path = disc_root + "/root.olk";
            printf("[Main] [+] root.olk detectado como paquete extendido con invitados (>700 MB)\n");
        }
    } else if (file_exists(disc_root + "/ROOT.OLK", &olk_size)) {
        printf("[Main] [+] Encontrado ROOT.OLK (%zu MB)\n", olk_size / (1024 * 1024));
        if (olk_size > 700ULL * 1024ULL * 1024ULL) {
            guests_olk_path = disc_root + "/ROOT.OLK";
            printf("[Main] [+] ROOT.OLK detectado como paquete extendido con invitados (>700 MB)\n");
        }
    } else {
        printf("[Main] [!] Advertencia: root.olk no encontrado en '%s'.\n", disc_root.c_str());
    }

    std::string guests_arg = guests_olk_path;
    if (guests_arg.rfind("sdmc:/", 0) == 0) {
        guests_arg = guests_arg.substr(5); // "/switch/soulcalibur2/guests.olk"
    }

    // 8. Preparar argumentos requeridos por runner.cpp
    std::string nro_arg = (argc > 0 && argv && argv[0]) ? argv[0] : "sdmc:/switch/soulcalibur2/soulcalibur2.nro";
    std::string storage_arg = "sdmc:/switch/soulcalibur2/save";

    std::vector<std::string> arg_strings;
    arg_strings.push_back(nro_arg);
    arg_strings.push_back("--xbe");
    arg_strings.push_back(xbe_path);
    arg_strings.push_back("--disc-root");
    arg_strings.push_back(disc_root);
    arg_strings.push_back("--storage");
    arg_strings.push_back(storage_arg);
    if (!guests_arg.empty()) {
        arg_strings.push_back("--guests");
        arg_strings.push_back(guests_arg);
    }
    arg_strings.push_back("--widescreen");
    arg_strings.push_back("--skip-intro");

    std::vector<char*> runner_argv;
    for (auto& s : arg_strings) {
        runner_argv.push_back(s.data());
    }

    printf("[Main] Iniciando motor de Soulcalibur II...\n");
    for (size_t i = 0; i < runner_argv.size(); i++) {
        printf("       arg[%zu]: %s\n", i, runner_argv[i]);
    }

    if (recomp_dispatch_init()) {
        printf("[Main] [+] Flat dispatch table initialized successfully.\n");
    } else {
        printf("[Main] [!] Flat dispatch unavailable; fallback to binary search.\n");
    }

    Switch_StartWatchdog();
    std::atexit(Switch_StopWatchdog);

    int result = sc2_runner_main((int)runner_argv.size(), runner_argv.data());

    printf("[Main] Runner finalizado con codigo: %d.\n", result);
    cleanup_switch_services();
    return result;
}
