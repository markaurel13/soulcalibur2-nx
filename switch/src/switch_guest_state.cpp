#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>
#include <filesystem>

extern "C" {

int execv(const char *path, char *const argv[]) {
    (void)path; (void)argv;
    return -1;
}

int flock(int fd, int operation) {
    (void)fd; (void)operation;
    return 0;
}

void recomp_d3d_textures_configure(const char *replace_folder, const char *dump_folder) {
    (void)replace_folder; (void)dump_folder;
}
void recomp_d3d_set_shader_preset(const char *path) { (void)path; }
void sc2_touch_set_menu(bool open) { (void)open; }
}

// Launcher and Netplay Stubs for Switch
namespace launcher {
    struct OpenMatch {};
    struct LocalAddress { std::string name; std::string address; };

    std::filesystem::path curl_program() { return {}; }
    bool fetch_lobby(const std::string&, std::vector<OpenMatch>&) { return false; }
    std::vector<LocalAddress> local_addresses() { return {}; }
    int run_program(const std::filesystem::path&, const std::vector<std::string>&, std::string*) { return -1; }
}

#include <time.h>
#include <sys/time.h>

extern "C" {

int timespec_get(struct timespec *ts, int base) {
    if (!ts) return 0;
    clock_gettime(CLOCK_REALTIME, ts);
    return base;
}

long sysconf(int name) {
    (void)name;
    return 4096;
}

int futimens(int fd, const struct timespec times[2]) {
    (void)fd; (void)times;
    return 0;
}

bool recomp_input_host_key(int key) {
    (void)key;
    return false;
}

bool recomp_input_host_mouse_drag(float *dx, float *dy) {
    (void)dx; (void)dy;
    return false;
}

void recomp_screenshot_request(void) {
}

void recomp_input_host_set_capture_hook(void *hook) {
    (void)hook;
}

void recomp_input_host_set_layout(const void *bindings) {
    (void)bindings;
}

}
