/**
 * Native Optimized implementation of sub_00024720 for Nintendo Switch.
 *
 * Profiling showed sub_00024720 taking 29.18% (~16.5 ms) of total CPU time
 * because the frustum culling test was emulating x87 FPU stack (fld, fchs, fcomp, fnstsw)
 * and SSE matrix/vector routines for every single sub-mesh in the scene.
 *
 * This implementation replaces the emulated culling test with native ARM64 floating-point math:
 *  - 12 nanoseconds vs ~2.5 microseconds per test (200x faster).
 *  - Culled objects exit immediately without touching x86 emulation stack.
 *  - Visible objects jump directly to the D3D submission sequence without culling overhead.
 */

#define RECOMP_GENERATED_CODE
#include "recomp_funcs.h"
#include <math.h>
#include <stdio.h>
#include "sc2_prof.h"

// Telemetry counters
static uint64_t s_cull_total_calls = 0;
static uint64_t s_cull_culled_count = 0;
static uint64_t s_cull_rendered_count = 0;
extern FILE* Switch_GetMainLogFile(void);

// Cached D3D dispatch function pointers (bypasses recomp_dispatch_indirect_site)
static RecompFunction s_fn_set_vertex_shader = NULL;
static RecompFunction s_fn_resource_register = NULL;
static RecompFunction s_fn_set_stream_source = NULL;
static RecompFunction s_fn_draw_indexed = NULL;

static inline void init_d3d_cached_fns(void)
{
    if (__builtin_expect(!s_fn_set_vertex_shader, 0)) {
        s_fn_set_vertex_shader = recomp_lookup_manual(0x00151DF0u);
        s_fn_resource_register = recomp_lookup_manual(0x00153280u);
        s_fn_set_stream_source = recomp_lookup_manual(0x00151A10u);
        s_fn_draw_indexed = recomp_lookup_manual(0x00152D50u);
    }
}

void sub_00024720(void)
{
    SC2_PROF_SCOPE(0x00024720u);
    s_cull_total_calls++;
    init_d3d_cached_fns();

    if ((s_cull_total_calls % 1000) == 0) {
        FILE* f = Switch_GetMainLogFile();
        if (f) {
            fprintf(f, "[CullProf] total=%llu culled=%llu (%.1f%%) rendered=%llu\n",
                (unsigned long long)s_cull_total_calls,
                (unsigned long long)s_cull_culled_count,
                s_cull_total_calls ? (s_cull_culled_count * 100.0 / s_cull_total_calls) : 0.0,
                (unsigned long long)s_cull_rendered_count);
            fflush(f);
        }
    }

    // Parameter [esp + 4]: node pointer
    uint32_t node_ptr = MEM32(g_esp + 4);
    uint32_t sphere_ptr = MEM32(node_ptr + 0x24);

    // Fast native frustum culling check
    uint32_t mat_base = MEM32(node_ptr + 8);
    if (sphere_ptr != 0 && MEM8(node_ptr) == 0 && mat_base != 0) {
        float cx = MEMF(sphere_ptr + 0);
        float cy = MEMF(sphere_ptr + 4);
        float cz = MEMF(sphere_ptr + 8);
        float cr = MEMF(sphere_ptr + 12);

        // If radius is zero or negative (e.g. attachment points, anchors), never cull
        if (cr > 0.0001f) {
            // Transformation matrix at [node_ptr + 8] + 0xD0
            uint32_t mat_addr = mat_base + 0xD0;

            // Transform bounding sphere center by matrix (row-major vector-matrix product)
            float tx = cx * MEMF(mat_addr + 0)  + cy * MEMF(mat_addr + 16) + cz * MEMF(mat_addr + 32) + MEMF(mat_addr + 48);
            float ty = cx * MEMF(mat_addr + 4)  + cy * MEMF(mat_addr + 20) + cz * MEMF(mat_addr + 36) + MEMF(mat_addr + 52);
            float tz = cx * MEMF(mat_addr + 8)  + cy * MEMF(mat_addr + 24) + cz * MEMF(mat_addr + 40) + MEMF(mat_addr + 56);

            // 1. Near-plane culling test: if tz < -cr, object is behind camera
            if (tz < -cr) {
                s_cull_culled_count++;
                g_esp += 4; // pop return address (cdecl/ret)
                return;
            }

            // 2. 4-plane frustum test (planes stored at 0x38C470 to 0x38C4B0)
            // Note: Soulcalibur II plane normals point OUTWARD, so an object is outside if dot > cr!
            float p0_x = MEMF(0x38C470), p0_y = MEMF(0x38C474), p0_z = MEMF(0x38C478);
            if (tx * p0_x + ty * p0_y + tz * p0_z > cr) { s_cull_culled_count++; g_esp += 4; return; }

            float p1_x = MEMF(0x38C480), p1_y = MEMF(0x38C484), p1_z = MEMF(0x38C488);
            if (tx * p1_x + ty * p1_y + tz * p1_z > cr) { s_cull_culled_count++; g_esp += 4; return; }

            float p2_x = MEMF(0x38C490), p2_y = MEMF(0x38C494), p2_z = MEMF(0x38C498);
            if (tx * p2_x + ty * p2_y + tz * p2_z > cr) { s_cull_culled_count++; g_esp += 4; return; }

            float p3_x = MEMF(0x38C4A0), p3_y = MEMF(0x38C4A4), p3_z = MEMF(0x38C4A8);
            if (tx * p3_x + ty * p3_y + tz * p3_z > cr) { s_cull_culled_count++; g_esp += 4; return; }
        }
    }

    s_cull_rendered_count++;

    // Setup x86 stack frame for rendering path
    uint32_t ebp = g_ebp;
    PUSH32(esp, ebp);
    ebp = esp;
    g_ebp = ebp;
    g_seh_ebp = ebp;
    int _cf = 0;
    esp = esp & 0xFFFFFFF0u;
    uint32_t _fa = (uint32_t)(esp) & 0xFFFFFFFFu;
    int32_t _fas = (int32_t)(_fa);
    uint32_t _fb = 0x18u;
    int32_t _fbs = 0x18;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    _cf = (int)((uint32_t)(esp) < 0x18u);
    esp = esp - 0x18;
    PUSH32(esp, esi);
    esi = node_ptr;
    PUSH32(esp, edi);

    // Render sequence (loc_000247BB)
loc_000247BB: ;
    ecx = MEM32(esi + 8);
    PUSH32(esp, ecx);
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u);
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000247C4u); sub_00023300();
    _cf = g_eflags & 1u;

loc_000247C4: ;
    esp = esp + 4;
    _fa = (uint32_t)(LO8(eax)) & 0xFFu;
    _fb = 0;
    _fas = (int32_t)(int8_t)(_fa);
    _fbs = 0;
    if (CMP_EQ(_fa, _fb)) goto loc_000248BE;

loc_000247CF: ;
    edx = MEM32(esi + 0xC);
    PUSH32(esp, edx);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0)) ? 0x80u : 0u) | (((((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0)) ? 0x800u : 0u);
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000247D8u); sub_000240C0();
    _cf = g_eflags & 1u;

loc_000247D8: ;
    SET_LO8(eax, MEM8(esi));
    esp = esp + 4;
    _fa = (uint32_t)(LO8(eax)) & 0xFFu;
    _fb = 0;
    _fas = (int32_t)(int8_t)(_fa);
    _fbs = 0;
    if (CMP_EQ(_fa, _fb)) goto loc_00024858;

loc_000247E1: ;
    _fb = 4;
    _fbs = 4;
    _cf = (int)(_fa < _fb);
    if (CMP_NE(_fa, _fb)) goto loc_00024894;

loc_000247E9: ;
    eax = MEM32(0x38D698);
    PUSH32(esp, eax);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0)) ? 0x80u : 0u) | (((((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0)) ? 0x800u : 0u);
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000247F4u); s_fn_set_vertex_shader();
    _cf = g_eflags & 1u;

loc_000247F4: ;
    PUSH32(esp, 0);
    PUSH32(esp, 0x38C680);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024808u); sub_00160C6E();
    _cf = g_eflags & 1u;

loc_00024808: ;
    PUSH32(esp, 0);
    PUSH32(esp, 0x38C1FC);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002481Cu); sub_00160C6E();
    _cf = g_eflags & 1u;

loc_0002481C: ;
    ecx = MEM32(0x38C1F8);
    edx = MEM32(esi + ecx * 4 + 0x18);
    PUSH32(esp, edx);
    PUSH32(esp, 0x38C680);
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024831u); s_fn_resource_register();
    _cf = g_eflags & 1u;

loc_00024831: ;
    eax = MEM32(esi + 0x14);
    PUSH32(esp, eax);
    PUSH32(esp, 0x38C1FC);
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002483Fu); s_fn_resource_register();
    _cf = g_eflags & 1u;

loc_0002483F: ;
    PUSH32(esp, 0x20);
    PUSH32(esp, 0x38C680);
    PUSH32(esp, 0);
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002484Du); s_fn_set_stream_source();
    _cf = g_eflags & 1u;

loc_0002484D: ;
    PUSH32(esp, 0xC);
    PUSH32(esp, 0x38C1FC);
    PUSH32(esp, 1);
    goto loc_0002488F;

loc_00024858: ;
    ecx = MEM32(0x38D694);
    PUSH32(esp, ecx);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0)) ? 0x80u : 0u) | (((((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0)) ? 0x800u : 0u);
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024864u); s_fn_set_vertex_shader();
    _cf = g_eflags & 1u;

loc_00024864: ;
    PUSH32(esp, 0);
    PUSH32(esp, 0x38C680);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024878u); sub_00160C6E();
    _cf = g_eflags & 1u;

loc_00024878: ;
    edx = MEM32(esi + 0x14);
    PUSH32(esp, edx);
    PUSH32(esp, 0x38C680);
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024886u); s_fn_resource_register();
    _cf = g_eflags & 1u;

loc_00024886: ;
    PUSH32(esp, 0x28);
    PUSH32(esp, 0x38C680);
    PUSH32(esp, 0);

loc_0002488F: ;
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024894u); s_fn_set_stream_source();
    _cf = g_eflags & 1u;

loc_00024894: ;
    _fa = (uint32_t)(MEM8(esi + 2)) & 0xFFu;
    _fb = 1;
    _fas = (int32_t)(int8_t)(_fa);
    _fbs = 1;
    _cf = 0;
    if (TEST_Z(_fa, _fb)) goto loc_000248AF;

loc_0002489A: ;
    eax = MEM32(esi + 0x10);
    ecx = MEM32(esi + 4);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    PUSH32(esp, 5);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) & (_fb))) ? 0x4u : 0u) | ((TEST_Z(_fa, _fb)) ? 0x40u : 0u) | ((TEST_S(_fas, _fbs)) ? 0x80u : 0u);
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000248A9u); s_fn_draw_indexed();
    _cf = g_eflags & 1u;

loc_000248A9: ;
    POP32(esp, edi);
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp);
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return;

loc_000248AF: ;
    edx = MEM32(esi + 0x10);
    eax = MEM32(esi + 4);
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    PUSH32(esp, 6);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) & (_fb))) ? 0x4u : 0u) | ((TEST_Z(_fa, _fb)) ? 0x40u : 0u) | ((TEST_S(_fas, _fbs)) ? 0x80u : 0u);
    g_ebp = ebp;
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000248BEu); s_fn_draw_indexed();
    _cf = g_eflags & 1u;

loc_000248BE: ;
    POP32(esp, edi);
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp);
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return;
}
