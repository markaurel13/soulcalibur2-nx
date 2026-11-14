/**
 * Optimized implementation of sub_0002B670 (shadow volume push-buffer submitter)
 * for Nintendo Switch.
 * - Inlines sub_0002B470.
 * - Caches BeginPush / EndPush / SetRenderState / SetCullMode targets.
 * - Uses direct recomp_fast_ram pointers for the 3-float stride vertex copy loop.
 */

#define RECOMP_GENERATED_CODE
#include "recomp_funcs.h"
#include "sc2_fast_mem.h"
#include <stdint.h>

static RecompFunction s_fn_set_render_state = NULL;
static RecompFunction s_fn_cull_mode = NULL;
static RecompFunction s_fn_begin_push = NULL;
static RecompFunction s_fn_end_push = NULL;

static inline void init_2b670_fns(void)
{
    if (__builtin_expect(!s_fn_set_render_state, 0)) {
        s_fn_set_render_state = recomp_lookup_manual(0x0014E130u);
        s_fn_cull_mode = recomp_lookup_manual(0x0014E500u);
        s_fn_begin_push = recomp_lookup_manual(0x00152540u);
        s_fn_end_push = recomp_lookup_manual(0x00152560u);
    }
}

static inline void fast_copy_shadow_verts(uint32_t *esi_io, uint32_t *ecx_io, uint32_t count)
{
    uint32_t esi_val = *esi_io;
    uint32_t ecx_val = *ecx_io;
    const uint32_t src_norm = (esi_val + 0x10u) & 0x7fffffffu;
    const uint32_t dst_norm = ecx_val & 0x7fffffffu;
    const uint32_t src_bytes = (count > 0u) ? ((count - 1u) * 0x40u + 12u) : 0u;
    const uint32_t dst_bytes = count * 12u;

    if (__builtin_expect(recomp_fast_ram != NULL &&
                         src_norm + src_bytes <= RECOMP_XBOX_RAM_SIZE &&
                         dst_norm + dst_bytes <= RECOMP_XBOX_RAM_SIZE, 1)) {
        const uint8_t *src = recomp_fast_ram + src_norm;
        uint32_t *dst = (uint32_t *)(void *)(recomp_fast_ram + dst_norm);
        for (uint32_t i = 0u; i < count; ++i) {
            const uint32_t *s = (const uint32_t *)(const void *)(src + i * 0x40u);
            dst[0] = s[0];
            dst[1] = s[1];
            dst[2] = s[2];
            dst += 3;
        }
        *esi_io = esi_val + count * 0x40u;
        *ecx_io = ecx_val + count * 12u;
    } else {
        for (uint32_t i = 0u; i < count; ++i) {
            MEM32(ecx_val) = MEM32(esi_val + 0x10u);
            MEM32(ecx_val + 4u) = MEM32(esi_val + 0x14u);
            MEM32(ecx_val + 8u) = MEM32(esi_val + 0x18u);
            esi_val += 0x40u;
            ecx_val += 12u;
        }
        *esi_io = esi_val;
        *ecx_io = ecx_val;
    }
}

static inline void fast_sub_0002B470(uint32_t ebp)
{
    uint32_t a = MEM32(0x38E4ECu);
    if (a > 1u) {
        a = (a - 1u) & 0xFFu;
        MEM32(0x38E4ECu) = a;
    }
    PUSH32(esp, esi);
    PUSH32(esp, 2u);
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B48Du);
    sub_000317C0();

    uint32_t d = MEM32(0x38E4ECu);
    esp += 4u;
    ecx = 0x40368u;
    edx = d;
    esi = d;
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B4A2u);
    s_fn_set_render_state();

    edx = 0x1E01u;
    ecx = 0x40378u;
    MEM32(0x15E4ACu) = esi;
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B4B7u);
    s_fn_set_render_state();

    edx = 0x204u;
    ecx = 0x40364u;
    MEM32(0x15E4A4u) = 0x1E01u;
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B4D0u);
    s_fn_set_render_state();

    PUSH32(esp, 0x900u);
    MEM32(0x15E4A8u) = 0x204u;
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B4E4u);
    s_fn_cull_mode();

    POP32(esp, esi);
}

void __wrap_sub_0002B670(void)
{
    init_2b670_fns();
    uint32_t ebp = g_seh_ebp;

    esp -= 0xCu;
    eax = MEM32(esp + 0x10u);
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    PUSH32(esp, edi);

    edi = MEM32(eax);
    uint32_t count_rem = ZX16(MEM16(edi + 0x58u));
    esi = MEM32(edi + 0x5Cu);
    uint32_t saved_edi = edi;
    uint32_t saved_esi = esi;

    ebp = count_rem;
    fast_sub_0002B470(ebp);

    /* Pass 1: front-face / initial stencil pass */
    while (count_rem > 0u) {
        uint32_t chunk = 0u;
        uint32_t ptr = esi + 0xCu;
        while (chunk < count_rem) {
            if (MEM32(ptr) != 0u && (int32_t)chunk >= 2) {
                break;
            }
            ptr += 0x40u;
            chunk++;
        }

        ebx = chunk * 3u;
        ecx = ebx + 5u;
        PUSH32(esp, ecx);
        g_ebp = ebp;
        g_seh_ebp = ebp;
        PUSH32(esp, 0x0002B6D4u);
        s_fn_begin_push();

        uint32_t push_ptr = eax;
        MEM32(push_ptr + 8u) = (ebx << 18) + 0x40001818u;
        MEM32(push_ptr) = 0x417FCu;
        MEM32(push_ptr + 4u) = 6u;
        push_ptr += 0xCu;

        if (chunk > 0u) {
            fast_copy_shadow_verts(&esi, &push_ptr, chunk);
        }

        uint32_t end_ptr = push_ptr + 8u;
        MEM32(push_ptr) = 0x417FCu;
        MEM32(push_ptr + 4u) = 0u;
        PUSH32(esp, end_ptr);
        g_ebp = ebp;
        g_seh_ebp = ebp;
        PUSH32(esp, 0x0002B7BBu);
        s_fn_end_push();

        count_rem -= chunk;
    }

    esi = saved_esi;
    edi = saved_edi;

    /* Switch stencil/cull for back-face pass */
    edx = 0u;
    ecx = 0x40378u;
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B7DBu);
    s_fn_set_render_state();

    edx = 0x202u;
    ecx = 0x40364u;
    MEM32(0x15E4A4u) = 0u;
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B7F4u);
    s_fn_set_render_state();

    PUSH32(esp, 0x901u);
    MEM32(0x15E4A8u) = 0x202u;
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B808u);
    s_fn_cull_mode();

    /* Pass 2 */
    count_rem = ZX16(MEM16(edi + 0x58u));
    while (count_rem > 0u) {
        uint32_t chunk = 0u;
        uint32_t ptr = esi + 0xCu;
        while (chunk < count_rem) {
            if (MEM32(ptr) != 0u && (int32_t)chunk >= 2) {
                break;
            }
            ptr += 0x40u;
            chunk++;
        }

        ebx = chunk * 3u;
        ecx = ebx + 5u;
        PUSH32(esp, ecx);
        g_ebp = ebp;
        g_seh_ebp = ebp;
        PUSH32(esp, 0x0002B845u);
        s_fn_begin_push();

        uint32_t push_ptr = eax;
        MEM32(push_ptr + 8u) = (ebx << 18) + 0x40001818u;
        MEM32(push_ptr) = 0x417FCu;
        MEM32(push_ptr + 4u) = 6u;
        push_ptr += 0xCu;

        if (chunk > 0u) {
            fast_copy_shadow_verts(&esi, &push_ptr, chunk);
        }

        uint32_t end_ptr = push_ptr + 8u;
        MEM32(push_ptr) = 0x417FCu;
        MEM32(push_ptr + 4u) = 0u;
        PUSH32(esp, end_ptr);
        g_ebp = ebp;
        g_seh_ebp = ebp;
        PUSH32(esp, 0x0002B927u);
        s_fn_end_push();

        count_rem -= chunk;
    }

    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    g_ebp = ebp;
    g_seh_ebp = ebp;
    esp += 0x10u; /* 0xC local + 4 ret */
}
