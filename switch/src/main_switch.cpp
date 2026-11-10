#include <switch.h>
#include <cstdio>
#include <cstdlib>

namespace SwitchFS {
    bool Initialize();
    void Shutdown();
}

namespace SwitchInput {
    void Initialize();
    void ShowControllerApplet(int minPlayers, int maxPlayers);
}

extern "C" int recomp_main(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    // 1. Initialize Console & System
    consoleInit(NULL);
    printf("========================================\n");
    printf("  Soulcalibur II (Xbox) - Switch Port  \n");
    printf("  Static Recompilation via Deko3D       \n");
    printf("========================================\n");

    // 2. Initialize Switch Filesystem (RomFS)
    if (!SwitchFS::Initialize()) {
        printf("[Main] Warning: Could not initialize RomFS. Using SDMC fallback.\n");
    }

    // 3. Initialize Controller Subsystem
    SwitchInput::Initialize();

    // 4. Run Game Loop
    printf("[Main] Starting recomp runner...\n");
    
    // In full build, calls runner.cpp / recomp_main()
    while (appletMainLoop()) {
        padUpdate(nullptr);
        u64 kDown = padGetButtonsDown(nullptr);

        // Plus Button: In-game options / controller applet
        if (kDown & HidNpadButton_Plus) {
            SwitchInput::ShowControllerApplet(1, 2);
        }

        // Minus Button: Exit homebrew
        if (kDown & HidNpadButton_Minus) {
            break;
        }

        consoleUpdate(NULL);
    }

    // 5. Cleanup
    SwitchFS::Shutdown();
    consoleExit(NULL);
    return 0;
}
