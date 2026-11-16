#include "audio_output.h"
#include "switch_core_manager.h"
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
    bool was_playing = false;
    bool has_prev_sample = false;
    uint32_t guest_slot = 0xFFFFFFFFu;
    uint32_t base_sample_rate = 0;
    uint32_t sample_rate = 0;
    uint32_t channels = 0;
    uint64_t last_tick = 0;
    uint64_t last_submit_tick = 0;
    float dry_left = 1.0f;
    float dry_right = 1.0f;
    int32_t last_vol_db = 0;
    int16_t prev_sample_l = 0;
    int16_t prev_sample_r = 0;
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
static float s_final_mix_vol = 0.65f;

// ---- Diagnostic counters (read by the Deko3D presenter's periodic perf report) ----
static uint64_t s_perf_update_ticks = 0, s_perf_submit_ticks = 0, s_perf_flush_ticks = 0;
static uint32_t s_perf_updates = 0, s_perf_submits = 0;

struct AudioDiagStats {
    uint32_t commits = 0;
    uint32_t wb_full_fail = 0;
    uint32_t wb_no_free = 0;
    uint32_t wb_too_big = 0;
    uint32_t pcm_drop_frames = 0;
    uint32_t underruns = 0;
    uint32_t flush_underruns = 0;
    uint32_t steals = 0;
    uint32_t voice_inits = 0;
    uint32_t init_fail = 0;
    uint32_t resets = 0;
    uint32_t resets_playing = 0;
    uint32_t rate_changes = 0;
    uint32_t max_active_voices = 0;
    uint32_t min_wb_queued = 99u;
    uint32_t max_wb_queued = 0;
    uint32_t audrv_rc_fail = 0;
    uint64_t last_audrv_tick = 0;
    double max_audrv_gap_ms = 0.0;
    uint64_t last_submit_tick = 0;
    double max_submit_gap_ms = 0.0;
    uint64_t last_flush_tick = 0;
    double max_flush_period_ms = 0.0;
    double max_mutex_wait_ms = 0.0;
    int32_t peak_pcm = 0;
    uint32_t clipped_pcm = 0;
    uint32_t float_over_1 = 0;
    uint32_t pcm_jumps = 0;
    int32_t max_pcm_jump = 0;
    int64_t pcm_sum = 0;
    uint64_t pcm_samples = 0;
    uint64_t total_frames_sub = 0;
    uint64_t total_frames_com = 0;
    int32_t min_vol_db = 0;
    int32_t max_vol_db = -99999;
    float max_gain = 0.0f;
    float max_sum_gain = 0.0f;
    float peak_sum_gain = 0.0f;
    float peak_rms_gain = 0.0f;
    float min_final_mix = 99.0f;
    float max_final_mix = 0.0f;
    float max_dry_routing = 0.0f;
    uint32_t routing_gt1 = 0;
    float min_pitch = 99.0f;
    float max_pitch = 0.0f;
    uint32_t min_rate = 999999u;
    uint32_t max_rate = 0u;
    uint32_t ch1 = 0, ch2 = 0, ch_gt2 = 0;
    uint32_t bits8 = 0, bits16 = 0, bits32 = 0, bits_other = 0;
    uint32_t warn_printed = 0;
    uint32_t detail_printed = 0;
};
static AudioDiagStats s_adiag;

#define AUDIO_WARN_THROTTLED(fmt, ...) \
    do { \
        if (s_adiag.warn_printed < 8u) { \
            ++s_adiag.warn_printed; \
            printf("[AudioWarn] " fmt "\n", ##__VA_ARGS__); \
        } \
    } while (0)

static Result perfAudrvUpdate() {
    uint64_t t0 = armGetSystemTick();
    if (s_adiag.last_audrv_tick != 0) {
        double gap = (double)(t0 - s_adiag.last_audrv_tick) / 19200.0;
        if (gap > s_adiag.max_audrv_gap_ms) s_adiag.max_audrv_gap_ms = gap;
    }
    s_adiag.last_audrv_tick = t0;
    Result rc = audrvUpdate(&s_drv);
    if (R_FAILED(rc)) ++s_adiag.audrv_rc_fail;
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

extern "C" void SwitchAudio_PrintAndResetDiag(void) {
    ScopedAudioMutex lock;
    uint32_t cur_active = 0;
    float sum_gain = 0.0f;
    for (int i = 0; i < MAX_HW_VOICES; ++i) {
        if (s_hw_voices[i].active) {
            ++cur_active;
            if (audrvVoiceIsPlaying(&s_drv, i)) {
                float dg = (s_hw_voices[i].last_vol_db <= -9600) ? 0.0f :
                           (s_hw_voices[i].last_vol_db < 0 ? std::pow(10.0f, (float)s_hw_voices[i].last_vol_db / 2000.0f) : 1.0f);
                sum_gain += dg * std::max(s_hw_voices[i].dry_left, s_hw_voices[i].dry_right);
            }
        }
    }
    if (sum_gain > s_adiag.max_sum_gain) s_adiag.max_sum_gain = sum_gain;
    if (cur_active > s_adiag.max_active_voices) s_adiag.max_active_voices = cur_active;

    double dc_avg = s_adiag.pcm_samples ? (double)s_adiag.pcm_sum / (double)s_adiag.pcm_samples : 0.0;
    printf("[AudioDiag] v=%u(max=%u,init=%u,fail=%u,steal=%u,rst=%u/%u) wb(c=%u,full=%u,nofree=%u,big=%u,drop=%u,under=%u/%u,q=%u..%u) frames(sub=%llu,com=%llu) gap(upd=%.1fms,sub=%.1fms,fl=%.1fms,mtx=%.2fms) err=%u\n",
           cur_active, s_adiag.max_active_voices, s_adiag.voice_inits, s_adiag.init_fail, s_adiag.steals,
           s_adiag.resets, s_adiag.resets_playing,
           s_adiag.commits, s_adiag.wb_full_fail, s_adiag.wb_no_free, s_adiag.wb_too_big, s_adiag.pcm_drop_frames,
           s_adiag.underruns, s_adiag.flush_underruns,
           s_adiag.min_wb_queued == 99u ? 0u : s_adiag.min_wb_queued, s_adiag.max_wb_queued,
           (unsigned long long)s_adiag.total_frames_sub, (unsigned long long)s_adiag.total_frames_com,
           s_adiag.max_audrv_gap_ms, s_adiag.max_submit_gap_ms, s_adiag.max_flush_period_ms, s_adiag.max_mutex_wait_ms,
           s_adiag.audrv_rc_fail);
    printf("[AudioQual] pcm(peak=%d,clip=%u,jmp=%u,maxjmp=%d,dc=%.1f,fovf=%u) gain(max=%.2f,sum=%.2f/%.2f,rms=%.2f,dry=%.2f,gt1=%u,mix=%.2f..%.2f,vol=%d..%d) fmt(r=%u..%u,rc=%u,pitch=%.2f..%.2f,ch=%u/%u/%u,b=%u/%u/%u/%u)\n",
           s_adiag.peak_pcm, s_adiag.clipped_pcm, s_adiag.pcm_jumps, s_adiag.max_pcm_jump, dc_avg, s_adiag.float_over_1,
           s_adiag.max_gain, s_adiag.max_sum_gain, s_adiag.peak_sum_gain, s_adiag.peak_rms_gain, s_adiag.max_dry_routing, s_adiag.routing_gt1,
           s_adiag.min_final_mix > 10.0f ? s_final_mix_vol : s_adiag.min_final_mix,
           s_adiag.max_final_mix < 0.01f ? s_final_mix_vol : s_adiag.max_final_mix,
           s_adiag.min_vol_db, s_adiag.max_vol_db,
           s_adiag.min_rate == 999999u ? 0u : s_adiag.min_rate, s_adiag.max_rate, s_adiag.rate_changes,
           s_adiag.min_pitch > 10.0f ? 1.0f : s_adiag.min_pitch,
           s_adiag.max_pitch < 0.01f ? 1.0f : s_adiag.max_pitch,
           s_adiag.ch1, s_adiag.ch2, s_adiag.ch_gt2,
           s_adiag.bits8, s_adiag.bits16, s_adiag.bits32, s_adiag.bits_other);
    uint64_t keep_audrv = s_adiag.last_audrv_tick;
    uint64_t keep_sub = s_adiag.last_submit_tick;
    uint64_t keep_flush = s_adiag.last_flush_tick;
    s_adiag = AudioDiagStats{};
    s_adiag.last_audrv_tick = keep_audrv;
    s_adiag.last_submit_tick = keep_sub;
    s_adiag.last_flush_tick = keep_flush;
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
    int active_wbs = countActiveWavebufs(v);
    if (active_wbs >= 4) {
        ++s_adiag.wb_full_fail;
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
    if (wb_idx < 0) {
        ++s_adiag.wb_no_free;
        AUDIO_WARN_THROTTLED("wb_no_free hw=%d slot=%u active_wbs=%d", hw_idx, v.guest_slot, active_wbs);
        return false;
    }

    uint32_t num_frames = v.staging_frames;
    uint32_t dst_ch = v.channels;
    size_t dst_bytes = (size_t)num_frames * dst_ch * sizeof(int16_t);
    size_t aligned_bytes = (dst_bytes + 63u) & ~size_t(63u);
    if (aligned_bytes > PER_WAVEBUF_BYTES) {
        ++s_adiag.wb_too_big;
        AUDIO_WARN_THROTTLED("wb_too_big hw=%d slot=%u bytes=%zu", hw_idx, v.guest_slot, aligned_bytes);
        v.staging_frames = 0;
        return false;
    }

    // When a voice starts (or restarts after an underrun) with an empty queue, prepend a small
    // ~10 ms silent pre-roll WaveBuf so 16-22 ms frame jitter never starves Audren before the next submit.
    uint32_t queued_added = 0;
    if (active_wbs == 0 && !audrvVoiceIsPlaying(&s_drv, hw_idx)) {
        int pre_idx = -1;
        for (int w = 0; w < WAVEBUFS_PER_VOICE; ++w) {
            if (w == wb_idx) continue;
            if (v.wavebufs[w].state == AudioDriverWaveBufState_Free ||
                v.wavebufs[w].state == AudioDriverWaveBufState_Done) {
                pre_idx = w;
                break;
            }
        }
        if (pre_idx >= 0) {
            uint32_t pre_frames = std::clamp(v.sample_rate / 100u, 48u, 512u);
            size_t pre_bytes = (size_t)pre_frames * dst_ch * sizeof(int16_t);
            size_t pre_aligned = (pre_bytes + 63u) & ~size_t(63u);
            if (pre_aligned <= PER_WAVEBUF_BYTES) {
                size_t pre_off = (size_t)(hw_idx * WAVEBUFS_PER_VOICE + pre_idx) * PER_WAVEBUF_BYTES;
                uint8_t *pre_raw = s_pool_ptr + pre_off;
                memset(pre_raw, 0, pre_aligned);
                armDCacheFlush(pre_raw, pre_aligned);
                AudioDriverWaveBuf *wb_pre = &v.wavebufs[pre_idx];
                memset(wb_pre, 0, sizeof(*wb_pre));
                wb_pre->data_pcm16 = reinterpret_cast<s16*>(pre_raw);
                wb_pre->size = pre_bytes;
                wb_pre->start_sample_offset = 0;
                wb_pre->end_sample_offset = pre_frames;
                wb_pre->is_looping = false;
                audrvVoiceAddWaveBuf(&s_drv, hw_idx, wb_pre);
                ++queued_added;
            }
        }
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
    ++queued_added;
    if (!audrvVoiceIsPlaying(&s_drv, hw_idx)) {
        audrvVoiceStart(&s_drv, hw_idx);
    }

    v.staging_frames = 0;
    s_wavebuf_counter++;
    ++s_adiag.commits;
    s_adiag.total_frames_com += num_frames;
    uint32_t q_after = (uint32_t)active_wbs + queued_added;
    if (q_after < s_adiag.min_wb_queued) s_adiag.min_wb_queued = q_after;
    if (q_after > s_adiag.max_wb_queued) s_adiag.max_wb_queued = q_after;
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

    if (volume_hundredth_db < s_adiag.min_vol_db) s_adiag.min_vol_db = volume_hundredth_db;
    if (volume_hundredth_db > s_adiag.max_vol_db) s_adiag.max_vol_db = volume_hundredth_db;
    float mg = db_gain * std::max(v.dry_left, v.dry_right);
    if (mg > s_adiag.max_gain) s_adiag.max_gain = mg;

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
            ++s_adiag.steals;
            AUDIO_WARN_THROTTLED("steal hw=%d old_slot=%u -> new_slot=%u", chosen, s_hw_voices[chosen].guest_slot, guest_slot);
            audrvVoiceStop(&s_drv, chosen);
            audrvVoiceDrop(&s_drv, chosen);
            s_hw_voices[chosen].active = false;
            s_hw_voices[chosen].was_playing = false;
            s_hw_voices[chosen].has_prev_sample = false;
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
            ++s_adiag.init_fail;
            AUDIO_WARN_THROTTLED("voiceInit fail hw=%d slot=%u ch=%u rate=%u", chosen, guest_slot, dst_channels, rate);
            return -1;
        }
        audrvVoiceSetDestinationMix(&s_drv, chosen, AUDREN_FINAL_MIX_ID);
        audrvVoiceSetPitch(&s_drv, chosen, 1.0f);
        v.active = true;
        v.was_playing = true;
        v.has_prev_sample = false;
        v.last_submit_tick = 0;
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
        ++s_adiag.voice_inits;
    } else {
        if (countActiveWavebufs(v) == 0 && v.staging_frames == 0) {
            ++s_adiag.underruns;
            if (v.last_submit_tick != 0 && s_adiag.detail_printed < 10u) {
                ++s_adiag.detail_printed;
                double vgap = (double)(armGetSystemTick() - v.last_submit_tick) / 19200.0;
                printf("[AudioUnder] slot=%u hw=%d gap=%.1fms rate=%u ch=%u\n",
                       guest_slot, chosen, vgap, rate, dst_channels);
            }
        }
        if (v.sample_rate != rate) {
            v.sample_rate = rate;
            ++s_adiag.rate_changes;
            float raw_pitch = (v.base_sample_rate > 0) ? ((float)rate / (float)v.base_sample_rate) : 1.0f;
            float pitch = std::clamp(raw_pitch, 0.05f, 4.0f);
            if (pitch < s_adiag.min_pitch) s_adiag.min_pitch = pitch;
            if (pitch > s_adiag.max_pitch) s_adiag.max_pitch = pitch;
            audrvVoiceSetPitch(&s_drv, chosen, pitch);
        }
    }

    v.last_tick = armGetSystemTick();
    return chosen;
}

static void flushAudioInternal(void) {
    if (!s_audio_initialized) return;
    PerfScope perf_scope(&s_perf_flush_ticks);
    uint64_t t_wait0 = armGetSystemTick();
    ScopedAudioMutex lock;
    uint64_t t_enter = armGetSystemTick();
    double wait_ms = (double)(t_enter - t_wait0) / 19200.0;
    if (wait_ms > s_adiag.max_mutex_wait_ms) s_adiag.max_mutex_wait_ms = wait_ms;
    if (s_adiag.last_flush_tick != 0) {
        double fgap = (double)(t_enter - s_adiag.last_flush_tick) / 19200.0;
        if (fgap > s_adiag.max_flush_period_ms) s_adiag.max_flush_period_ms = fgap;
    }
    s_adiag.last_flush_tick = t_enter;

    float sum_sq = 0.0f;
    float inst_sum_gain = 0.0f;
    for (int i = 0; i < MAX_HW_VOICES; ++i) {
        if (!s_hw_voices[i].active) continue;
        int wbs_before = countActiveWavebufs(s_hw_voices[i]);
        if (s_hw_voices[i].staging_frames > 0 && wbs_before < 4) {
            commitVoiceStaging(i);
            wbs_before = countActiveWavebufs(s_hw_voices[i]);
        }
        bool playing = audrvVoiceIsPlaying(&s_drv, i);
        if (!playing && s_hw_voices[i].was_playing && wbs_before == 0 && s_hw_voices[i].staging_frames == 0) {
            ++s_adiag.flush_underruns;
        }
        s_hw_voices[i].was_playing = playing;
        if (playing) {
            float dg = (s_hw_voices[i].last_vol_db <= -9600) ? 0.0f :
                       (s_hw_voices[i].last_vol_db < 0 ? std::pow(10.0f, (float)s_hw_voices[i].last_vol_db / 2000.0f) : 1.0f);
            float g = dg * std::max(s_hw_voices[i].dry_left, s_hw_voices[i].dry_right);
            sum_sq += g * g;
            inst_sum_gain += g;
        }
    }
    if (inst_sum_gain > s_adiag.peak_sum_gain) s_adiag.peak_sum_gain = inst_sum_gain;
    float rms_gain = std::sqrt(sum_sq);
    if (rms_gain > s_adiag.peak_rms_gain) s_adiag.peak_rms_gain = rms_gain;
    // Volumen maestro fijo a 0.65f (sin ducking dinamico) para evitar que picos
    // de suma >1.0 disparen el limitador/DRC de salida y mantenga volumen constante.
    s_final_mix_vol = 0.65f;
    if (s_final_mix_vol < s_adiag.min_final_mix) s_adiag.min_final_mix = s_final_mix_vol;
    if (s_final_mix_vol > s_adiag.max_final_mix) s_adiag.max_final_mix = s_final_mix_vol;
    audrvMixSetVolume(&s_drv, AUDREN_FINAL_MIX_ID, s_final_mix_vol);

    static uint64_t s_last_audrv_tick = 0;
    uint64_t now = armGetSystemTick();
    if ((now - s_last_audrv_tick) >= (5ULL * 19200ULL)) {
        s_last_audrv_tick = now;
        perfAudrvUpdate();
    }
}

static Thread s_audio_flush_thread;
alignas(0x1000) static uint8_t s_audio_flush_stack[0x10000];
static std::atomic<bool> s_audio_flush_running{false};
static std::atomic<bool> s_audio_flush_thread_active{false};

static void audioFlushWorker(void*) {
    while (s_audio_flush_running.load(std::memory_order_acquire)) {
        svcSleepThread(5000000ULL); // 5 ms
        if (!s_audio_flush_running.load(std::memory_order_acquire)) break;
        flushAudioInternal();
    }
}

extern "C" {

void SwitchAudio_FlushPending(void) {
    if (s_audio_flush_thread_active.load(std::memory_order_acquire)) return;
    flushAudioInternal();
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
        .num_voices = MAX_HW_VOICES * 2, // 48 voice channels so 24 mono/stereo HW voices never exhaust channels
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

    const int sys_core = SwitchCore_GetSystemCore();
    if (sys_core >= 0) {
        s_audio_flush_running.store(true, std::memory_order_release);
        Result trc = threadCreate(&s_audio_flush_thread, audioFlushWorker, nullptr,
                                  s_audio_flush_stack, sizeof(s_audio_flush_stack), 0x2A, sys_core);
        if (R_SUCCEEDED(trc)) {
            threadStart(&s_audio_flush_thread);
            s_audio_flush_thread_active.store(true, std::memory_order_release);
            printf("[SwitchAudio] Dedicated audio flush thread started on Core %d (Prio 0x2A).\n", sys_core);
        } else {
            s_audio_flush_running.store(false, std::memory_order_release);
            printf("[SwitchAudio] Failed to start Core %d audio thread (0x%x); using presenter flush.\n", sys_core, trc);
        }
    }
}

void recomp_audio_output_shutdown(void) {
    if (s_audio_flush_thread_active.exchange(false, std::memory_order_acq_rel)) {
        s_audio_flush_running.store(false, std::memory_order_release);
        threadWaitForExit(&s_audio_flush_thread);
        threadClose(&s_audio_flush_thread);
    }
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
    ++s_adiag.resets;
    if (slot < 256) {
        s_guest_routing[slot] = {};
    }
    if (!s_audio_initialized) return;
    for (int i = 0; i < MAX_HW_VOICES; ++i) {
        if (s_hw_voices[i].active && s_hw_voices[i].guest_slot == slot) {
            if (countActiveWavebufs(s_hw_voices[i]) > 0 || s_hw_voices[i].staging_frames > 0) {
                ++s_adiag.resets_playing;
            }
            audrvVoiceStop(&s_drv, i);
            audrvVoiceDrop(&s_drv, i);
            s_hw_voices[i].active = false;
            s_hw_voices[i].was_playing = false;
            s_hw_voices[i].has_prev_sample = false;
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
    float dry_max = std::max(l, r);
    if (dry_max > s_adiag.max_dry_routing) s_adiag.max_dry_routing = dry_max;
    if (dry_max > 1.01f) ++s_adiag.routing_gt1;

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

    {
        uint64_t now_sub = armGetSystemTick();
        if (s_adiag.last_submit_tick != 0) {
            double sgap = (double)(now_sub - s_adiag.last_submit_tick) / 19200.0;
            if (sgap > s_adiag.max_submit_gap_ms) s_adiag.max_submit_gap_ms = sgap;
        }
        s_adiag.last_submit_tick = now_sub;
        if (sample_rate < s_adiag.min_rate) s_adiag.min_rate = sample_rate;
        if (sample_rate > s_adiag.max_rate) s_adiag.max_rate = sample_rate;
        if (channels == 1u) ++s_adiag.ch1;
        else if (channels == 2u) ++s_adiag.ch2;
        else ++s_adiag.ch_gt2;
        if (bits_per_sample == 8u) ++s_adiag.bits8;
        else if (bits_per_sample == 16u) ++s_adiag.bits16;
        else if (bits_per_sample == 32u) ++s_adiag.bits32;
        else ++s_adiag.bits_other;
        s_adiag.total_frames_sub += num_frames;
    }

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
                uint32_t drop_f = num_frames - frame_cursor;
                s_adiag.pcm_drop_frames += drop_f;
                AUDIO_WARN_THROTTLED("pcm_drop slot=%u hw=%d dropped=%u rate=%u", slot, hw_idx, drop_f, sample_rate);
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
                    float raw_s = srcf[f * channels + c];
                    if (raw_s > 1.0f || raw_s < -1.0f) ++s_adiag.float_over_1;
                    float s = std::clamp(raw_s, -1.0f, 1.0f);
                    dst[f * dst_ch + c] = (int16_t)(s * 32767.0f);
                }
            }
        } else {
            memset(dst, 0, (size_t)chunk_frames * dst_ch * sizeof(int16_t));
        }

        if (chunk_frames > 0) {
            int16_t first_l = dst[0];
            int16_t first_r = (dst_ch >= 2) ? dst[1] : dst[0];
            if (v.has_prev_sample) {
                int32_t jl = std::abs((int32_t)first_l - (int32_t)v.prev_sample_l);
                int32_t jr = std::abs((int32_t)first_r - (int32_t)v.prev_sample_r);
                int32_t j = std::max(jl, jr);
                if (j > s_adiag.max_pcm_jump) s_adiag.max_pcm_jump = j;
                if (j > 12000) {
                    ++s_adiag.pcm_jumps;
                    if (s_adiag.detail_printed < 10u) {
                        ++s_adiag.detail_printed;
                        double vgap = v.last_submit_tick ? (double)(armGetSystemTick() - v.last_submit_tick) / 19200.0 : 0.0;
                        printf("[AudioJump] slot=%u hw=%d jmp=%d (%d->%d) gap=%.1fms wbs=%d stg=%u rate=%u\n",
                               slot, hw_idx, j, (int)v.prev_sample_l, (int)first_l, vgap,
                               countActiveWavebufs(v), v.staging_frames, sample_rate);
                    }
                }
            }
            uint32_t last_off = (chunk_frames - 1u) * dst_ch;
            v.prev_sample_l = dst[last_off];
            v.prev_sample_r = (dst_ch >= 2) ? dst[last_off + 1] : dst[last_off];
            v.has_prev_sample = true;
        }

        {
            uint32_t total_s = chunk_frames * dst_ch;
            for (uint32_t si = 0; si < total_s; ++si) {
                int32_t sv = dst[si];
                int32_t av = sv < 0 ? -sv : sv;
                if (av > s_adiag.peak_pcm) s_adiag.peak_pcm = av;
                if (av >= 32760) ++s_adiag.clipped_pcm;
                s_adiag.pcm_sum += sv;
            }
            s_adiag.pcm_samples += total_s;
        }

        v.staging_frames += chunk_frames;
        frame_cursor += chunk_frames;

        int active_wbs = countActiveWavebufs(v);
        uint32_t min_coalesce_frames = std::max(256u, v.sample_rate / 50u); // ~20ms
        uint32_t low_q_frames = std::max(64u, v.sample_rate / 250u);       // ~4ms
        if (active_wbs == 0 ||
            (active_wbs == 1 && v.staging_frames >= low_q_frames) ||
            v.staging_frames >= min_coalesce_frames) {
            commitVoiceStaging(hw_idx);
        }
    }

    v.last_submit_tick = armGetSystemTick();
    s_submit_counter++;
}

} // extern "C"
