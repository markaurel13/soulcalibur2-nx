#include "save_transaction.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <filesystem>
#include <sys/stat.h>

namespace fs = std::filesystem;

static bool s_ready = false;
static uint32_t s_active_owner = 0;
static uint32_t s_depth = 0;
static bool s_failed = false;
static fs::path s_storage;
static fs::path s_live;

extern "C" {

bool recomp_save_initialize(const char *disc_root) {
    if (!disc_root || !*disc_root) return false;
    std::string path = std::string(disc_root) + "/save";
    return recomp_save_initialize_storage(path.c_str());
}

bool recomp_save_initialize_storage(const char *storage_root) {
    if (!storage_root || !*storage_root) return false;
    printf("[SaveTransaction] Initializing save storage at '%s'...\n", storage_root);

    std::error_code ec;
    s_storage = storage_root;
    s_live = s_storage / "partition1";

    fs::create_directories(s_storage, ec);
    fs::create_directories(s_live, ec);

    s_ready = true;
    s_depth = 0;
    s_failed = false;
    printf("[SaveTransaction] [+] Save system initialized successfully at: %s\n", s_live.c_str());
    return true;
}

bool recomp_save_begin(uint32_t owner) {
    if (!s_ready) return false;
    if (s_depth != 0) {
        if (owner != s_active_owner) return false;
        ++s_depth;
        return true;
    }
    s_active_owner = owner;
    s_depth = 1;
    s_failed = false;
    return true;
}

bool recomp_save_end(uint32_t owner, bool success) {
    if (!s_ready || s_depth == 0 || owner != s_active_owner) return false;
    s_failed = s_failed || !success;
    if (--s_depth != 0) return !s_failed;
    return !s_failed;
}

bool recomp_save_end_recovers(uint32_t owner, bool success) {
    return s_ready && s_depth == 1 && owner == s_active_owner && (s_failed || !success);
}

void recomp_save_note_failure(uint32_t owner) {
    if (s_depth != 0 && owner == s_active_owner) s_failed = true;
}

void recomp_save_note_pending_failure(void) {
    if (s_depth != 0) s_failed = true;
}

bool recomp_save_active(uint32_t owner) {
    return s_ready && s_depth != 0 && owner == s_active_owner;
}

bool recomp_save_pending(void) {
    return s_depth != 0;
}

bool recomp_save_ready(void) {
    return s_ready;
}

}
