#include "audio_output.h"
#include <switch.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <malloc.h>
#include <algorithm>
#include <atomic>

static constexpr int MAX_HW_VOICES = 24;
static constexpr int WAVEBUFS_PER_VOICE = 8;
static constexpr size_t PER_WAVEBUF_BYTES = 20480; // 5120 stereo 16-bit samples (~106 ms at 48 kHz)
static constexpr size_t WAVEBUF_POOL_SIZE = MAX_HW_VOICES * WAVEBUFS_PER_VOICE * PER_WAVEBUF_BYTES; // ~3.9 MB
static constexpr uint32_t STAGING_CAP_FRAMES = 4096; // 4096 frames * 4 bytes = 16384 bytes <= PER_WAVEBUF_BYTES

struct HwVoiceState {
    bool active = false;
    uint32_t guest_slot = 0xFFFFFFFFu;
    uint32_t base_sample_rate = 0;
    uint32_t sample_rate = 0;
    uint32_t channels = 0;
    uint64_t last_tick = 0;
    float dry_left = 1.0f;
    float dry_right = 1.0f;
    int32_t last_vol_db = 0;
    uint32_t staging_frames = 0;
    int16_t staging_pcm[STAGING_CAP_FRAMES * 2] = {};
    AudioDriverWaveBuf wavebufs[WAVEBUFS_PER_VOICE] = {};
};

struct GuestSlotRouting {
    bool configured = false;
    float left = 1.0f;
    float right = 1.0f;
};

static RMutex s_audio_mutex;

struct ScopedAudioMutex {
    ScopedAudioMutex() { rmutexLock(&s_audio_mutex); }
    ~ScopedAudioMutex() { rmutexUnlock(&s_audio_mutex); }
};

static bool s_audio_initialized = false;
static bool s_audio_init_failed = false;
static AudioDriver s_drv = {};
static int s_mempool_id = -1;
static uint8_t *s_pool_ptr = nullptr;
static HwVoiceState s_hw_voices[MAX_HW_VOICES] = {};
static GuestSlotRouting s_guest_routing[256] = {};
static std::atomic<int> s_audio_muted(0);
static std::atomic<int> s_audio_paused(0);
static uint64_t s_submit_counter = 0;
static uint64_t s_wavebuf_counter = 0;

// ---- Diagnostic counters (read by the Deko3D presenter's periodic perf report) ----
static uint64_t s_perf_update_ticks = 0, s_perf_submit_ticks = 0, s_perf_flush_ticks = 0;
static uint32_t s_perf_updates = 0, s_perf_submits = 0;

static Result perfAudrvUpdate() {
    uint64_t t0 = armGetSystemTick();
    Result rc = audrvUpdate(&s_drv);
    s_perf_update_ticks += armGetSystemTick() - t0;
    ++s_perf_updates;
    return rc;
}

struct PerfScope {
    uint64_t *acc; uint64_t t0;
    explicit PerfScope(uint64_t *a) : acc(a), t0(armGetSystemTick()) {}
    ~PerfScope() { *acc += armGetSystemTick() - t0; }
};

extern "C" void SwitchAudio_TakePerf(uint64_t *update_ticks, uint64_t *submit_ticks, uint64_t *flush_ticks,
                                     uint32_t *updates, uint32_t *submits) {
    *update_ticks = s_perf_update_ticks; *submit_ticks = s_perf_submit_ticks; *flush_ticks = s_perf_flush_ticks;
    *updates = s_perf_updates; *submits = s_perf_submits;
    s_perf_update_ticks = s_perf_submit_ticks = s_perf_flush_ticks = 0;
    s_perf_updates = s_perf_submits = 0;
}

static int countActiveWavebufs(const HwVoiceState &v) {
    int count = 0;
    for (int w = 0; w < WAVEBUFS_PER_VOICE; ++w) {
        auto st = v.wavebufs[w].state;
        if (st == AudioDriverWaveBufState_Waiting ||
            st == AudioDriverWaveBufState_Queued ||
            st == AudioDriverWaveBufState_Playing) {
            count++;
        }
    }
    return count;
}

static bool commitVoiceStaging(int hw_idx) {
    HwVoiceState &v = s_hw_voices[hw_idx];
    if (!v.active || v.staging_frames == 0 || v.channels == 0) return false;

    // audren hardware voices accept at most 4 active wavebufs
    if (countActiveWavebufs(v) >= 4) {
        return false;
    }

    int wb_idx = -1;
    for (int w = 0; w < WAVEBUFS_PER_VOICE; ++w) {
        if (v.wavebufs[w].state == AudioDriverWaveBufState_Free ||
            v.wavebufs[w].state == AudioDriverWaveBufState_Done) {
            wb_idx = w;
            break;
        }
    }
    if (wb_idx < 0) return false;

    uint32_t num_frames = v.staging_frames;
    uint32_t dst_ch = v.channels;
    size_t dst_bytes = (size_t)num_frames * dst_ch * sizeof(int16_t);
    size_t aligned_bytes = (dst_bytes + 63u) & ~size_t(63u);
    if (aligned_bytes > PER_WAVEBUF_BYTES) {
        v.staging_frames = 0;
        return false;
    }

    size_t offset = (size_t)(hw_idx * WAVEBUFS_PER_VOICE + wb_idx) * PER_WAVEBUF_BYTES;
    uint8_t *dst_raw = s_pool_ptr + offset;
    memcpy(dst_raw, v.staging_pcm, dst_bytes);
    if (aligned_bytes > dst_bytes) {
        memset(dst_raw + dst_bytes, 0, aligned_bytes - dst_bytes);
    }
    armDCacheFlush(dst_raw, aligned_bytes);

    AudioDriverWaveBuf *wb = &v.wavebufs[wb_idx];
    memset(wb, 0, sizeof(*wb));
    wb->data_pcm16 = reinterpret_cast<s16*>(dst_raw);
    wb->size = dst_bytes;
    wb->start_sample_offset = 0;
    wb->end_sample_offset = num_frames;
    wb->is_looping = false;

    audrvVoiceAddWaveBuf(&s_drv, hw_idx, wb);
    if (!audrvVoiceIsPlaying(&s_drv, hw_idx)) {
        audrvVoiceStart(&s_drv, hw_idx);
    }

    v.staging_frames = 0;
    s_wavebuf_counter++;
    return true;
}

static void applyHwVoiceGains(int hw_idx, int32_t volume_hundredth_db) {
    HwVoiceState &v = s_hw_voices[hw_idx];
    v.last_vol_db = volume_hundredth_db;
    if (s_audio_muted.load() != 0 || s_audio_paused.load() != 0) {
        audrvVoiceSetVolume(&s_drv, hw_idx, 0.0f);
        return;
    }

    float db_gain = 1.0f;
    if (volume_hundredth_db <= -9600) {
        db_gain = 0.0f;
    } else if (volume_hundredth_db < 0) {
        db_gain = std::pow(10.0f, (float)volume_hundredth_db / 2000.0f);
    }
    db_gain = std::clamp(db_gain, 0.0f, 1.0f);
    audrvVoiceSetVolume(&s_drv, hw_idx, db_gain);

    float l = v.dry_left;
    float r = v.dry_right;
    if (v.channels == 1) {
        audrvVoiceSetMixFactor(&s_drv, hw_idx, l, 0, 0);
        audrvVoiceSetMixFactor(&s_drv, hw_idx, r, 0, 1);
    } else {
        audrvVoiceSetMixFactor(&s_drv, hw_idx, l, 0, 0);
        audrvVoiceSetMixFactor(&s_drv, hw_idx, 0.0f, 0, 1);
        audrvVoiceSetMixFactor(&s_drv, hw_idx, 0.0f, 1, 0);
        audrvVoiceSetMixFactor(&s_drv, hw_idx, r, 1, 1);
    }
}

static int acquireHwVoice(uint32_t guest_slot, uint32_t sample_rate, uint32_t dst_channels) {
    uint32_t rate = std::clamp(sample_rate, 4000u, 96000u);

    int chosen = -1;
    for (int i = 0; i < MAX_HW_VOICES; ++i) {
        if (s_hw_voices[i].active && s_hw_voices[i].guest_slot == guest_slot) {
            chosen = i;
            break;
        }
    }
    if (chosen < 0) {
        for (int i = 0; i < MAX_HW_VOICES; ++i) {
            if (!s_hw_voices[i].active) {
                chosen = i;
                break;
            }
        }
    }
    if (chosen < 0) {
        uint64_t oldest = UINT64_MAX;
        for (int i = 0; i < MAX_HW_VOICES; ++i) {
            if (s_hw_voices[i].last_tick < oldest) {
                oldest = s_hw_voices[i].last_tick;
                chosen = i;
            }
        }
        if (chosen < 0) chosen = 0;
        if (s_hw_voices[chosen].active) {
            audrvVoiceStop(&s_drv, chosen);
            audrvVoiceDrop(&s_drv, chosen);
            s_hw_voices[chosen].active = false;
        }
    }

    HwVoiceState &v = s_hw_voices[chosen];
    // IMPORTANT: Do NOT re-init or drop the voice when only sample_rate changes!
    // Soulcalibur II sweeps voice frequencies every frame (weapon swings, etc.).
    if (!v.active || v.guest_slot != guest_slot || v.channels != dst_channels) {
        if (v.active) {
            audrvVoiceStop(&s_drv, chosen);
            audrvVoiceDrop(&s_drv, chosen);
            v.active = false;
        }
        if (!audrvVoiceInit(&s_drv, chosen, (int)dst_channels, PcmFormat_Int16, (float)rate)) {
            return -1;
        }
        audrvVoiceSetDestinationMix(&s_drv, chosen, AUDREN_FINAL_MIX_ID);
        audrvVoiceSetPitch(&s_drv, chosen, 1.0f);
        v.active = true;
        v.guest_slot = guest_slot;
        v.base_sample_rate = rate;
        v.sample_rate = rate;
        v.channels = dst_channels;
        v.staging_frames = 0;
        if (guest_slot < 256 && s_guest_routing[guest_slot].configured) {
            v.dry_left = s_guest_routing[guest_slot].left;
            v.dry_right = s_guest_routing[guest_slot].right;
        } else {
            v.dry_left = 1.0f;
            v.dry_right = 1.0f;
        }
        for (int w = 0; w < WAVEBUFS_PER_VOICE; ++w) {
            v.wavebufs[w] = {};
            v.wavebufs[w].state = AudioDriverWaveBufState_Free;
        }
        audrvVoiceStart(&s_drv, chosen);
    } else if (v.sample_rate != rate) {
        v.sample_rate = rate;
        float pitch = (v.base_sample_rate > 0) ? ((float)rate / (float)v.base_sample_rate) : 1.0f;
        audrvVoiceSetPitch(&s_drv, chosen, std::clamp(pitch, 0.05f, 4.0f));
    }

    v.last_tick = armGetSystemTick();
    return chosen;
}

extern "C" {

void SwitchAudio_FlushPending(void) {
    if (!s_audio_initialized) return;
    PerfScope perf_scope(&s_perf_flush_ticks);
    ScopedAudioMutex lock;
    for (int i = 0; i < MAX_HW_VOICES; ++i) {
        if (s_hw_voices[i].active && s_hw_voices[i].staging_frames > 0) {
            commitVoiceStaging(i);
        }
    }
    perfAudrvUpdate();
}

uint32_t recomp_audio_output_speakers(void) {
    return 2; // Stereo for Switch
}

void recomp_audio_output_initialize(void) {
    if (s_audio_initialized || s_audio_init_failed) return;
    ScopedAudioMutex lock;
    if (s_audio_initialized || s_audio_init_failed) return;

    printf("[SwitchAudio] Initializing native audren subsystem...\n");

    // In libnx, config.num_voices is the number of voice channels (stereo voice = 2 channels),
    // and audrvCreate's 3rd parameter is num_final_mix_channels (2 for stereo!).
    static const AudioRendererConfig config = {
        .output_rate = AudioRendererOutputRate_48kHz,
        .num_voices = MAX_HW_VOICES,
        .num_effects = 0,
        .num_sinks = 1,
        .num_mix_objs = 1,
        .num_mix_buffers = 2,
    };

    Result rc = audrenInitialize(&config);
    if (R_FAILED(rc)) {
        printf("[SwitchAudio] audrenInitialize failed (0x%x)\n", rc);
        s_audio_init_failed = true;
        return;
    }

    rc = audrvCreate(&s_drv, &config, 2);
    if (R_FAILED(rc)) {
        printf("[SwitchAudio] audrvCreate failed (0x%x)\n", rc);
        audrenExit();
        s_audio_init_failed = true;
        return;
    }

    s_pool_ptr = static_cast<uint8_t*>(memalign(AUDREN_MEMPOOL_ALIGNMENT, WAVEBUF_POOL_SIZE));
    if (!s_pool_ptr) {
        printf("[SwitchAudio] Failed to allocate 8MB wavebuf pool\n");
        audrvClose(&s_drv);
        audrenExit();
        s_audio_init_failed = true;
        return;
    }
    memset(s_pool_ptr, 0, WAVEBUF_POOL_SIZE);
    armDCacheFlush(s_pool_ptr, WAVEBUF_POOL_SIZE);

    s_mempool_id = audrvMemPoolAdd(&s_drv, s_pool_ptr, WAVEBUF_POOL_SIZE);
    audrvMemPoolAttach(&s_drv, s_mempool_id);

    static const uint8_t sink_channels[2] = { 0, 1 };
    audrvDeviceSinkAdd(&s_drv, AUDREN_DEFAULT_DEVICE_NAME, 2, sink_channels);

    rc = perfAudrvUpdate();
    if (R_FAILED(rc)) {
        printf("[SwitchAudio] initial audrvUpdate failed (0x%x)\n", rc);
    }
    rc = audrenStartAudioRenderer();
    if (R_FAILED(rc)) {
        printf("[SwitchAudio] audrenStartAudioRenderer failed (0x%x)\n", rc);
    }

    for (int i = 0; i < MAX_HW_VOICES; ++i) {
        s_hw_voices[i].active = false;
        s_hw_voices[i].staging_frames = 0;
        for (int w = 0; w < WAVEBUFS_PER_VOICE; ++w) {
            s_hw_voices[i].wavebufs[w].state = AudioDriverWaveBufState_Free;
        }
    }

    s_audio_initialized = true;
    printf("[SwitchAudio] Native audio renderer online (48 kHz stereo, %d HW voices, 8MB pool).\n", MAX_HW_VOICES);
}

void recomp_audio_output_shutdown(void) {
    if (!s_audio_initialized) return;
    ScopedAudioMutex lock;
    if (!s_audio_initialized) return;

    for (int i = 0; i < MAX_HW_VOICES; ++i) {
        if (s_hw_voices[i].active) {
            audrvVoiceStop(&s_drv, i);
            audrvVoiceDrop(&s_drv, i);
            s_hw_voices[i].active = false;
            s_hw_voices[i].staging_frames = 0;
        }
    }

    if (s_mempool_id >= 0) {
        audrvMemPoolDetach(&s_drv, s_mempool_id);
        perfAudrvUpdate();
        audrenWaitFrame();
        audrvMemPoolRemove(&s_drv, s_mempool_id);
        perfAudrvUpdate();
        s_mempool_id = -1;
    }

    audrenStopAudioRenderer();
    audrvClose(&s_drv);
    audrenExit();

    if (s_pool_ptr) {
        free(s_pool_ptr);
        s_pool_ptr = nullptr;
    }
    s_audio_initialized = false;
    printf("[SwitchAudio] Audio renderer shut down cleanly.\n");
}

void recomp_audio_output_reset_voice(uint32_t slot) {
    ScopedAudioMutex lock;
    if (slot < 256) {
        s_guest_routing[slot] = {};
    }
    if (!s_audio_initialized) return;
    for (int i = 0; i < MAX_HW_VOICES; ++i) {
        if (s_hw_voices[i].active && s_hw_voices[i].guest_slot == slot) {
            audrvVoiceStop(&s_drv, i);
            audrvVoiceDrop(&s_drv, i);
            s_hw_voices[i].active = false;
            s_hw_voices[i].staging_frames = 0;
            perfAudrvUpdate();
            break;
        }
    }
}

int recomp_audio_output_enabled(void) {
    return s_audio_initialized ? 1 : 0;
}

int recomp_audio_output_ready(void) {
    return s_audio_initialized ? 1 : 0;
}

int recomp_audio_output_on_owner_thread(void) {
    return 1;
}

void recomp_audio_output_set_paused(int paused) {
    s_audio_paused.store(paused);
}

unsigned long long recomp_audio_output_underruns(void) {
    return 0;
}

void recomp_audio_output_set_muted(int muted) {
    s_audio_muted.store(muted);
}

int recomp_audio_output_muted(void) {
    return s_audio_muted.load();
}

int recomp_audio_output_set_reverb(const RecompAudioReverb *parameters) {
    (void)parameters;
    return 1;
}

int recomp_audio_output_set_voice_routing(uint32_t slot, uint32_t channels,
    const float *dry, const float *wet) {
    (void)wet;
    if (slot >= 256 || channels == 0) return 0;

    ScopedAudioMutex lock;
    float l = 1.0f, r = 1.0f;
    if (dry) {
        // Matrix layout: dry[source + destination * channels]
        l = 0.0f;
        r = 0.0f;
        for (uint32_t c = 0; c < channels; ++c) {
            l += dry[c + 0 * channels];
            r += dry[c + 1 * channels];
        }
        l = std::clamp(l, 0.0f, 2.0f);
        r = std::clamp(r, 0.0f, 2.0f);
    }

    s_guest_routing[slot].configured = true;
    s_guest_routing[slot].left = l;
    s_guest_routing[slot].right = r;

    if (s_audio_initialized) {
        for (int i = 0; i < MAX_HW_VOICES; ++i) {
            if (s_hw_voices[i].active && s_hw_voices[i].guest_slot == slot) {
                s_hw_voices[i].dry_left = l;
                s_hw_voices[i].dry_right = r;
                break;
            }
        }
    }
    return 1;
}

void recomp_audio_output_submit(
    uint32_t slot, const uint8_t *pcm, uint32_t bytes,
    uint32_t sample_rate, uint32_t channels, uint32_t bits_per_sample,
    int32_t volume_hundredth_db) {
    if (!pcm || bytes == 0 || channels == 0) return;

    PerfScope perf_scope(&s_perf_submit_ticks);
    ScopedAudioMutex lock;
    ++s_perf_submits;
    if (!s_audio_initialized) {
        recomp_audio_output_initialize();
        if (!s_audio_initialized) return;
    }

    uint32_t bytes_per_sample = bits_per_sample / 8u;
    if (bytes_per_sample == 0) return;
    uint32_t frame_bytes = bytes_per_sample * channels;
    uint32_t num_frames = bytes / frame_bytes;
    if (num_frames == 0) return;

    uint32_t dst_ch = (channels >= 2u) ? 2u : 1u;
    int hw_idx = acquireHwVoice(slot, sample_rate, dst_ch);
    if (hw_idx < 0) return;

    static uint32_t s_log_submits = 0;
    if (++s_log_submits <= 30) {
        printf("[SwitchAudio] submit #%u: slot=%u bytes=%u rate=%u ch=%u bits=%u vol=%d (hw=%d)\n",
               s_log_submits, slot, bytes, sample_rate, channels, bits_per_sample, volume_hundredth_db, hw_idx);
    }

    applyHwVoiceGains(hw_idx, volume_hundredth_db);

    HwVoiceState &v = s_hw_voices[hw_idx];
    uint32_t frame_cursor = 0;

    while (frame_cursor < num_frames) {
        uint32_t space = STAGING_CAP_FRAMES - v.staging_frames;
        if (space == 0) {
            if (!commitVoiceStaging(hw_idx)) {
                // Staging buffer and wavebufs are full; skip remaining frames to avoid blocking
                break;
            }
            space = STAGING_CAP_FRAMES - v.staging_frames;
        }

        uint32_t chunk_frames = std::min(num_frames - frame_cursor, space);
        int16_t *dst = v.staging_pcm + (size_t)v.staging_frames * dst_ch;
        const uint8_t *src_chunk = pcm + (size_t)frame_cursor * frame_bytes;

        if (bits_per_sample == 16 && channels == dst_ch) {
            memcpy(dst, src_chunk, (size_t)chunk_frames * dst_ch * sizeof(int16_t));
        } else if (bits_per_sample == 16 && channels > 2) {
            const int16_t *src16 = reinterpret_cast<const int16_t*>(src_chunk);
            for (uint32_t f = 0; f < chunk_frames; ++f) {
                dst[f * 2 + 0] = src16[f * channels + 0];
                dst[f * 2 + 1] = src16[f * channels + 1];
            }
        } else if (bits_per_sample == 8) {
            for (uint32_t f = 0; f < chunk_frames; ++f) {
                for (uint32_t c = 0; c < dst_ch; ++c) {
                    int32_t val = ((int32_t)src_chunk[f * channels + c] - 128) * 256;
                    dst[f * dst_ch + c] = (int16_t)std::clamp(val, -32768, 32767);
                }
            }
        } else if (bits_per_sample == 32) {
            const float *srcf = reinterpret_cast<const float*>(src_chunk);
            for (uint32_t f = 0; f < chunk_frames; ++f) {
                for (uint32_t c = 0; c < dst_ch; ++c) {
                    float s = std::clamp(srcf[f * channels + c], -1.0f, 1.0f);
                    dst[f * dst_ch + c] = (int16_t)(s * 32767.0f);
                }
            }
        } else {
            memset(dst, 0, (size_t)chunk_frames * dst_ch * sizeof(int16_t));
        }

        v.staging_frames += chunk_frames;
        frame_cursor += chunk_frames;

        int active_wbs = countActiveWavebufs(v);
        uint32_t min_coalesce_frames = std::max(256u, v.sample_rate / 50u); // ~20ms
        if (active_wbs < 2 || v.staging_frames >= min_coalesce_frames) {
            commitVoiceStaging(hw_idx);
        }
    }

    s_submit_counter++;
    if ((s_submit_counter % 500ULL) == 1ULL) {
        printf("[SwitchAudio] Submit #%llu (wb=%llu): slot=%u hw=%d rate=%u ch=%u frames=%u vol_db=%d active_wb=%d\n",
               (unsigned long long)s_submit_counter,
               (unsigned long long)s_wavebuf_counter,
               slot, hw_idx, sample_rate, channels,
               num_frames, volume_hundredth_db,
               countActiveWavebufs(v));
    }
}

} // extern "C"
