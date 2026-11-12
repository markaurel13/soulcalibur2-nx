/**
 * Optimized implementation of sub_0002B670 for Nintendo Switch.
 * Direct dispatch to D3D push-buffer and state functions.
 * Eliminates RECOMP_ICALL_SAFE overhead and dead x86 parity calculations.
 */

#define RECOMP_GENERATED_CODE
#include "recomp_funcs.h"
#include <stdint.h>
#include "sc2_prof.h"

static RecompFunction s_fn_begin_push = NULL;
static RecompFunction s_fn_end_push = NULL;
static RecompFunction s_fn_set_render_state = NULL;
static RecompFunction s_fn_cull_mode = NULL;

static inline void init_2b670_fns(void)
{
    if (__builtin_expect(!s_fn_begin_push, 0)) {
        s_fn_begin_push = recomp_lookup_manual(0x00152540u);
        s_fn_end_push = recomp_lookup_manual(0x00152560u);
        s_fn_set_render_state = recomp_lookup_manual(0x0014E130u);
        s_fn_cull_mode = recomp_lookup_manual(0x0014E500u);
    }
}

void __wrap_sub_0002B670(void)
{
    SC2_PROF_SCOPE(0x0002B670u);
    init_2b670_fns();

    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fk = 0; /* tag of the last flag setter */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_flags; (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_0002B670: ;
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0xC));
    esp = esp - 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = MEM32(esp + 0x10);
    PUSH32(esp, ebx);
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    edi = MEM32(eax);
    ebp = ZX16(MEM16(edi + 0x58));
    esi = MEM32(edi + 0x5C);
    MEM32(esp + 0x18) = edi;
    MEM32(esp + 0x14) = esi;
    MEM32(esp + 0x20) = ebp;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B695u); sub_0002B470(); /* call 0x0002B470 */

loc_0002B695: ;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebp, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_BE(_fa, _fb)) goto loc_0002B7CF; /* jbe: below or equal (unsigned <=) */

loc_0002B69D: ;
    /* nop */

loc_0002B6A0: ;
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebp, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_BE(_fa, _fb)) goto loc_0002B6C2; /* jbe: below or equal (unsigned <=) */

loc_0002B6A6: ;
    eax = esi + 0xC;
    /* nop */

loc_0002B6B0: ;
    _fa = (uint32_t)(MEM32(eax)) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(eax), 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_EQ(_fa, _fb)) goto loc_0002B6BA; /* je: equal / zero */

loc_0002B6B5: ;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(2) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 2 (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_GE(_fas, _fbs)) goto loc_0002B6C2; /* jge: greater or equal (signed >=) */

loc_0002B6BA: ;
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0x40)) >> 32) & 1);
    eax = eax + 0x40;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, ebp (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_B(_fa, _fb)) goto loc_0002B6B0; /* jb: below (unsigned <) */

loc_0002B6C2: ;
    ebx = edi + edi * 2;
    if (((2) & 31u)) _cf = (int)(((uint32_t)(ebx) >> ((32 - (((2) & 31u))) & 31u)) & 1u);
    ebx = (uint32_t)(ebx) << ((2) & 31u);
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    if (((2) & 31u)) _cf = (int)(((uint32_t)(ebx) >> (((((2) & 31u)) - 1) & 31u)) & 1u);
    ebx = (uint32_t)(ebx) >> ((2) & 31u);
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    ecx = ebx + 5;
    PUSH32(esp, ecx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B6D4u); s_fn_begin_push(); /* direct call begin_push (0x00152540) */

loc_0002B6D4: ;
    edx = ebx;
    if (((0x12) & 31u)) _cf = (int)(((uint32_t)(edx) >> ((32 - (((0x12) & 31u))) & 31u)) & 1u);
    edx = (uint32_t)(edx) << ((0x12) & 31u);
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(0x40001818) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edx) + (uint64_t)(0x40001818)) >> 32) & 1);
    edx = edx + 0x40001818;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(eax + 8) = edx;
    MEM32(eax) = 0x417FC;
    MEM32(eax + 4) = 6;
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0xC)) >> 32) & 1);
    eax = eax + 0xC;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 4 (32-bit) */
    _cf = (int)(_fa < _fb);
    ecx = eax;
    edx = edi;
    if (CMP_L(_fas, _fbs)) goto loc_0002B775; /* jl: less (signed <) */

loc_0002B6FB: ;
    edx = edi + -4;
    if (((2) & 31u)) _cf = (int)(((uint32_t)(edx) >> (((((2) & 31u)) - 1) & 31u)) & 1u);
    edx = (uint32_t)(edx) >> ((2) & 31u);
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    edx++;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    MEM32(esp + 0x10) = edx;
    if (((2) & 31u)) _cf = (int)(((uint32_t)(edx) >> ((32 - (((2) & 31u))) & 31u)) & 1u);
    edx = (uint32_t)(edx) << ((2) & 31u);
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    ebp = edx;
    edx = edi;
    _fb = (uint32_t)(ebp) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(edx) < (uint32_t)(ebp));
    edx = edx - ebp;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */

loc_0002B710: ;
    ebp = MEM32(esi + 0x10);
    MEM32(ecx) = ebp;
    ebp = MEM32(esi + 0x14);
    MEM32(ecx + 4) = ebp;
    ebp = MEM32(esi + 0x18);
    MEM32(ecx + 8) = ebp;
    ebp = MEM32(esi + 0x50);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x40)) >> 32) & 1);
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(ecx + 0xC) = ebp;
    ebp = MEM32(esi + 0x14);
    MEM32(ecx + 0x10) = ebp;
    ebp = MEM32(esi + 0x18);
    MEM32(ecx + 0x14) = ebp;
    ebp = MEM32(esi + 0x50);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x40)) >> 32) & 1);
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(ecx + 0x18) = ebp;
    ebp = MEM32(esi + 0x14);
    MEM32(ecx + 0x1C) = ebp;
    ebp = MEM32(esi + 0x18);
    MEM32(ecx + 0x20) = ebp;
    ebp = MEM32(esi + 0x50);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x40)) >> 32) & 1);
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(ecx + 0x24) = ebp;
    ebp = MEM32(esi + 0x14);
    MEM32(ecx + 0x28) = ebp;
    ebp = MEM32(esi + 0x18);
    MEM32(ecx + 0x2C) = ebp;
    ebp = MEM32(esp + 0x10);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x40)) >> 32) & 1);
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(0x30) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0x30)) >> 32) & 1);
    ecx = ecx + 0x30;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ebp--;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    MEM32(esp + 0x10) = ebp;
    if ((_fa != 0)) goto loc_0002B710; /* jne: not equal / not zero */

loc_0002B771: ;
    ebp = MEM32(esp + 0x20);

loc_0002B775: ;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_LE(_fas, _fbs)) goto loc_0002B7A2; /* jle: less or equal (signed <=) */

loc_0002B779: ;
    MEM32(esp + 0x20) = edx;

loc_0002B780: ;
    edx = MEM32(esi + 0x10);
    MEM32(ecx) = edx;
    edx = MEM32(esi + 0x14);
    MEM32(ecx + 4) = edx;
    edx = MEM32(esi + 0x18);
    MEM32(ecx + 8) = edx;
    edx = MEM32(esp + 0x20);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x40)) >> 32) & 1);
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0xC)) >> 32) & 1);
    ecx = ecx + 0xC;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    edx--;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    MEM32(esp + 0x20) = edx;
    if ((_fa != 0)) goto loc_0002B780; /* jne: not equal / not zero */

loc_0002B7A2: ;
    MEM32(eax + ebx * 4) = 0x417FC;
    MEM32(eax + ebx * 4 + 4) = 0;
    eax = eax + ebx * 4 + 8;
    PUSH32(esp, eax);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B7BBu); s_fn_end_push(); /* direct call end_push (0x00152560) */

loc_0002B7BB: ;
    _fb = (uint32_t)(edi) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(ebp) < (uint32_t)(edi));
    ebp = ebp - edi;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    MEM32(esp + 0x20) = ebp;
    if ((_fa != 0)) goto loc_0002B6A0; /* jne: not equal / not zero */

loc_0002B7C7: ;
    esi = MEM32(esp + 0x14);
    edi = MEM32(esp + 0x18);

loc_0002B7CF: ;
    _cf = 0; /* xor clears CF */
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x40378;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B7DBu); s_fn_set_render_state(); /* direct call set_render_state_simple (0x0014E130) */

loc_0002B7DB: ;
    edx = 0x202;
    ecx = 0x40364;
    MEM32(0x15E4A4) = 0;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B7F4u); s_fn_set_render_state(); /* direct call set_render_state_simple (0x0014E130) */

loc_0002B7F4: ;
    PUSH32(esp, 0x901);
    MEM32(0x15E4A8) = 0x202;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B808u); s_fn_cull_mode(); /* direct call cull_mode (0x0014E500) */

loc_0002B808: ;
    ebp = ZX16(MEM16(edi + 0x58));
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebp, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    MEM32(esp + 0x20) = ebp;
    if (CMP_BE(_fa, _fb)) goto loc_0002B933; /* jbe: below or equal (unsigned <=) */

loc_0002B818: ;
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebp, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_BE(_fa, _fb)) goto loc_0002B833; /* jbe: below or equal (unsigned <=) */

loc_0002B81E: ;
    eax = esi + 0xC;

loc_0002B821: ;
    _fa = (uint32_t)(MEM32(eax)) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(eax), 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_EQ(_fa, _fb)) goto loc_0002B82B; /* je: equal / zero */

loc_0002B826: ;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(2) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 2 (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_GE(_fas, _fbs)) goto loc_0002B833; /* jge: greater or equal (signed >=) */

loc_0002B82B: ;
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0x40)) >> 32) & 1);
    eax = eax + 0x40;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, ebp (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_B(_fa, _fb)) goto loc_0002B821; /* jb: below (unsigned <) */

loc_0002B833: ;
    ebx = edi + edi * 2;
    if (((2) & 31u)) _cf = (int)(((uint32_t)(ebx) >> ((32 - (((2) & 31u))) & 31u)) & 1u);
    ebx = (uint32_t)(ebx) << ((2) & 31u);
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    if (((2) & 31u)) _cf = (int)(((uint32_t)(ebx) >> (((((2) & 31u)) - 1) & 31u)) & 1u);
    ebx = (uint32_t)(ebx) >> ((2) & 31u);
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    ecx = ebx + 5;
    PUSH32(esp, ecx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B845u); s_fn_begin_push(); /* direct call begin_push (0x00152540) */

loc_0002B845: ;
    edx = ebx;
    if (((0x12) & 31u)) _cf = (int)(((uint32_t)(edx) >> ((32 - (((0x12) & 31u))) & 31u)) & 1u);
    edx = (uint32_t)(edx) << ((0x12) & 31u);
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(0x40001818) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edx) + (uint64_t)(0x40001818)) >> 32) & 1);
    edx = edx + 0x40001818;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(eax + 8) = edx;
    MEM32(eax) = 0x417FC;
    MEM32(eax + 4) = 6;
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0xC)) >> 32) & 1);
    eax = eax + 0xC;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 4 (32-bit) */
    _cf = (int)(_fa < _fb);
    ecx = eax;
    edx = edi;
    if (CMP_L(_fas, _fbs)) goto loc_0002B8E9; /* jl: less (signed <) */

loc_0002B870: ;
    edx = edi + -4;
    if (((2) & 31u)) _cf = (int)(((uint32_t)(edx) >> (((((2) & 31u)) - 1) & 31u)) & 1u);
    edx = (uint32_t)(edx) >> ((2) & 31u);
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    edx++;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    MEM32(esp + 0x10) = edx;
    if (((2) & 31u)) _cf = (int)(((uint32_t)(edx) >> ((32 - (((2) & 31u))) & 31u)) & 1u);
    edx = (uint32_t)(edx) << ((2) & 31u);
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    ebp = edx;
    edx = edi;
    _fb = (uint32_t)(ebp) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(edx) < (uint32_t)(ebp));
    edx = edx - ebp;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */

loc_0002B884: ;
    ebp = MEM32(esi + 0x10);
    MEM32(ecx) = ebp;
    ebp = MEM32(esi + 0x14);
    MEM32(ecx + 4) = ebp;
    ebp = MEM32(esi + 0x18);
    MEM32(ecx + 8) = ebp;
    ebp = MEM32(esi + 0x50);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x40)) >> 32) & 1);
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(ecx + 0xC) = ebp;
    ebp = MEM32(esi + 0x14);
    MEM32(ecx + 0x10) = ebp;
    ebp = MEM32(esi + 0x18);
    MEM32(ecx + 0x14) = ebp;
    ebp = MEM32(esi + 0x50);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x40)) >> 32) & 1);
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(ecx + 0x18) = ebp;
    ebp = MEM32(esi + 0x14);
    MEM32(ecx + 0x1C) = ebp;
    ebp = MEM32(esi + 0x18);
    MEM32(ecx + 0x20) = ebp;
    ebp = MEM32(esi + 0x50);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x40)) >> 32) & 1);
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(ecx + 0x24) = ebp;
    ebp = MEM32(esi + 0x14);
    MEM32(ecx + 0x28) = ebp;
    ebp = MEM32(esi + 0x18);
    MEM32(ecx + 0x2C) = ebp;
    ebp = MEM32(esp + 0x10);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x40)) >> 32) & 1);
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(0x30) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0x30)) >> 32) & 1);
    ecx = ecx + 0x30;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ebp--;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    MEM32(esp + 0x10) = ebp;
    if ((_fa != 0)) goto loc_0002B884; /* jne: not equal / not zero */

loc_0002B8E5: ;
    ebp = MEM32(esp + 0x20);

loc_0002B8E9: ;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_LE(_fas, _fbs)) goto loc_0002B90E; /* jle: less or equal (signed <=) */

loc_0002B8ED: ;
    /* nop */

loc_0002B8F0: ;
    ebp = MEM32(esi + 0x10);
    MEM32(ecx) = ebp;
    ebp = MEM32(esi + 0x14);
    MEM32(ecx + 4) = ebp;
    ebp = MEM32(esi + 0x18);
    MEM32(ecx + 8) = ebp;
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x40)) >> 32) & 1);
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0xC)) >> 32) & 1);
    ecx = ecx + 0xC;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    edx--;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    if ((_fa != 0)) goto loc_0002B8F0; /* jne: not equal / not zero */

loc_0002B90A: ;
    ebp = MEM32(esp + 0x20);

loc_0002B90E: ;
    MEM32(eax + ebx * 4) = 0x417FC;
    MEM32(eax + ebx * 4 + 4) = 0;
    eax = eax + ebx * 4 + 8;
    PUSH32(esp, eax);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002B927u); s_fn_end_push(); /* direct call end_push (0x00152560) */

loc_0002B927: ;
    _fb = (uint32_t)(edi) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(ebp) < (uint32_t)(edi));
    ebp = ebp - edi;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    MEM32(esp + 0x20) = ebp;
    if ((_fa != 0)) goto loc_0002B818; /* jne: not equal / not zero */

loc_0002B933: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */
}
