#include <cstdio>

extern "C" {

void recomp_install_host_diagnostics(void) {
    // Keep standard streams unbuffered so every single print/error is flushed to disk instantly
    setvbuf(stdout, nullptr, _IONBF, 0);
    setvbuf(stderr, nullptr, _IONBF, 0);
}

}
