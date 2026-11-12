/**
 * Safe guarded implementation of sub_00023C30 for Nintendo Switch.
 * Prevents runaway loops on character bone hierarchy transformations.
 */

#define RECOMP_GENERATED_CODE
#include "recomp_funcs.h"
#include <math.h>
#include <stdio.h>
#include "sc2_prof.h"

extern FILE* Switch_GetMainLogFile(void);

void sub_00023C30(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* prologue saves caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    int _sc2_outer_iters = 0;
    SC2_PROF_SCOPE(0x00023C30u);
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_00023C30: ;
    PUSH32(esp, ebp);
    ebp = esp;
    g_ebp = ebp; /* publish frame for frameless callees */
    g_seh_ebp = ebp;
    _cf = 0; /* logical op clears CF */
    esp = esp & 0xFFFFFFF0u;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fb = (uint32_t)(0x44) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0x44));
    esp = esp - 0x44;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    eax = MEM32(0x38C4B4);
    PUSH32(esp, ebx);
    ebx = MEM32(0x38C668);
    PUSH32(esp, esi);
    esi = MEM32(ebp + 8);
    ecx = MEM32(esi + eax * 4 + 0x14);
    _fa = (uint32_t)(MEM32(esi)) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(esi), 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    PUSH32(esp, edi);
    edi = MEM32(esi + 0x10);
    MEM32(esp + 0xC) = ecx;
    MEM32(esp + 0x10) = 0;
    if (CMP_BE(_fa, _fb)) goto loc_00023CA2; /* jbe: below or equal (unsigned <=) */

loc_00023C62: ;
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    edx = MEM32(esp + 0xC);
    eax = edi + 0x20;
    MEM32(esp + 0x14) = eax;
    eax = MEM32(edi + 0x1C);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, edx);
    ecx = edi;
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023C81u); RECOMP_ABI_CALL(0x000239D0u, sub_000239D0); /* call 0x000239D0 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023C81: ;
    edi = MEM32(esp + 0x10);
    eax = MEM32(esp + 0x14);
    ecx = MEM32(esi);
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(0x20)) >> 32) & 1);
    edi = edi + 0x20;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ecx (32-bit) */
    _cf = (int)(_fa < _fb);
    MEM32(esp + 0xC) = edi;
    edi = MEM32(esp + 0x14);
    MEM32(esp + 0x10) = eax;
    if (CMP_B(_fa, _fb)) goto loc_00023C62; /* jb: below (unsigned <) */

loc_00023CA2: ;
    eax = MEM32(esi + 4);
    ecx = eax;
    _cf = 0; /* logical op clears CF */
    SET_LO8(ecx, LO8(ecx) & 3);
    _fa = (uint32_t)(LO8(ecx)) & 0xFFu; _fas = (int32_t)(int8_t)(_fa); /* and result */
    _cf = (int)((LO8(ecx)) != 0);
    SET_LO8(ecx, (uint32_t)(-(int32_t)LO8(ecx)));
    _fa = (uint32_t)(LO8(ecx)) & 0xFFu; _fas = (int32_t)(int8_t)(_fa); /* neg result */
    ecx = _cf ? 0xFFFFFFFF : 0; /* sbb self (CF extend) */ _fb = 0;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sbb result */
    if (((2) & 31u)) _cf = (int)(((uint32_t)(eax) >> (((((2) & 31u)) - 1) & 31u)) & 1u);
    eax = (uint32_t)(eax) >> ((2) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _cf = (int)((ecx) != 0);
    ecx = (uint32_t)(-(int32_t)ecx);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* neg result */
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(eax)) >> 32) & 1);
    ecx = ecx + eax;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax = ecx;
    if ((_fa == 0)) goto loc_00023E2C; /* je: equal / zero */

loc_00023CBD: ;
    MEM32(esp + 0x10) = eax;

loc_00023CC1: ;
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    eax = edi + 0x100;
    MEM32(esp + 0x2C) = eax;
    eax = MEM32(edi + 0x5C);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x14) = eax;
    eax = MEM32(edi + 0x7C);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x18) = eax;
    eax = MEM32(edi + 0x9C);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x1C) = eax;
    eax = MEM32(edi + 0xBC);
    esi = MEM32(edi + 0x3C);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x20) = eax;
    eax = MEM32(edi + 0xDC);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x24) = eax;
    eax = MEM32(edi + 0xFC);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x28) = eax;
    eax = MEM32(edi + 0x1C);
    edx = esp + 0x30;
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    if (((6) & 31u)) _cf = (int)(((uint32_t)(esi) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    esi = (uint32_t)(esi) << ((6) & 31u);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, edx);
    ecx = edi;
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(ebx)) >> 32) & 1);
    esi = esi + ebx;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023D73u); RECOMP_ABI_CALL(0x00023A60u, sub_00023A60); /* call 0x00023A60 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023D73: ;
    eax = esp + 0x34;
    PUSH32(esp, eax);
    eax = esi;
    esi = MEM32(esp + 0x14);
    ecx = edi + 0x20;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023D86u); RECOMP_ABI_CALL(0x00023AF0u, sub_00023AF0); /* call 0x00023AF0 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023D86: ;
    eax = MEM32(esp + 0x1C);
    edx = esp + 0x38;
    ecx = edi + 0x40;
    PUSH32(esp, edx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023D97u); RECOMP_ABI_CALL(0x00023A60u, sub_00023A60); /* call 0x00023A60 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023D97: ;
    eax = esp + 0x3C;
    PUSH32(esp, eax);
    eax = MEM32(esp + 0x28);
    ecx = edi + 0x60;
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x20)) >> 32) & 1);
    esi = esi + 0x20;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023DABu); RECOMP_ABI_CALL(0x00023AF0u, sub_00023AF0); /* call 0x00023AF0 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023DAB: ;
    eax = MEM32(esp + 0x2C);
    edx = esp + 0x40;
    ecx = edi + 0x80;
    PUSH32(esp, edx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023DBFu); RECOMP_ABI_CALL(0x00023A60u, sub_00023A60); /* call 0x00023A60 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023DBF: ;
    esi = MEM32(esp + 0x20);
    eax = esp + 0x44;
    PUSH32(esp, eax);
    eax = MEM32(esp + 0x38);
    ecx = edi + 0xA0;
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x40)) >> 32) & 1);
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023DDAu); RECOMP_ABI_CALL(0x00023AF0u, sub_00023AF0); /* call 0x00023AF0 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023DDA: ;
    eax = MEM32(esp + 0x3C);
    edx = esp + 0x48;
    ecx = edi + 0xC0;
    PUSH32(esp, edx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023DEEu); RECOMP_ABI_CALL(0x00023A60u, sub_00023A60); /* call 0x00023A60 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023DEE: ;
    eax = esp + 0x4C;
    ecx = edi + 0xE0;
    PUSH32(esp, eax);
    edi = MEM32(esp + 0x2C);
    eax = MEM32(esp + 0x48);
    esi = edi + 0x60;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023E09u); RECOMP_ABI_CALL(0x00023AF0u, sub_00023AF0); /* call 0x00023AF0 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023E09: ;
    eax = MEM32(esp + 0x30);
    _fb = (uint32_t)(0x80) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(0x80)) >> 32) & 1);
    edi = edi + 0x80;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x20)) >> 32) & 1);
    esp = esp + 0x20;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    MEM32(esp + 0xC) = edi;
    edi = MEM32(esp + 0x2C);
    MEM32(esp + 0x10) = eax;
    if ((_fa != 0)) goto loc_00023CC1; /* jne: not equal / not zero */

loc_00023E29: ;
    esi = MEM32(ebp + 8);

loc_00023E2C: ;
    ecx = MEM32(esi);
    eax = MEM32(esi + 4);
    edx = MEM32(esi + 0x10);
    edi = ecx + eax * 2;
    _fb = (uint32_t)(ecx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ecx)) >> 32) & 1);
    eax = eax + ecx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx = MEM32(0x38C4B4);
    if (((5) & 31u)) _cf = (int)(((uint32_t)(edi) >> ((32 - (((5) & 31u))) & 31u)) & 1u);
    edi = (uint32_t)(edi) << ((5) & 31u);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    if (((5) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((5) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((5) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(edx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(edx)) >> 32) & 1);
    edi = edi + edx;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(MEM32(esi + ecx * 4 + 0x14)) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(MEM32(esi + ecx * 4 + 0x14))) >> 32) & 1);
    eax = eax + MEM32(esi + ecx * 4 + 0x14);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0xC) = eax;
    eax = MEM32(esi + 8);
    ecx = eax;
    if (((1) & 31u)) _cf = (int)(((uint32_t)(ecx) >> (((((1) & 31u)) - 1) & 31u)) & 1u);
    ecx = (uint32_t)(ecx) >> ((1) & 31u);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _cf = 0; /* logical op clears CF */
    eax = eax & 1;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(eax)) >> 32) & 1);
    ecx = ecx + eax;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    if ((_fa == 0)) goto loc_00023F7C; /* je: equal / zero */

loc_00023E61: ;
    MEM32(esp + 0x10) = ecx;
    goto loc_00023E70;

loc_00023E70: ;
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    eax = edi + 0xC0;
    MEM32(esp + 0x1C) = eax;
    eax = MEM32(edi + 0x5C);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x2C) = eax;
    eax = MEM32(edi + 0x7C);
    esi = MEM32(edi + 0x3C);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x28) = eax;
    eax = MEM32(edi + 0x9C);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x24) = eax;
    eax = MEM32(edi + 0xBC);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x20) = eax;
    eax = MEM32(edi + 0x1C);
    edx = esp + 0x30;
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    if (((6) & 31u)) _cf = (int)(((uint32_t)(esi) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    esi = (uint32_t)(esi) << ((6) & 31u);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, edx);
    ecx = edi;
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(ebx)) >> 32) & 1);
    esi = esi + ebx;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023EF6u); RECOMP_ABI_CALL(0x00023A60u, sub_00023A60); /* call 0x00023A60 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023EF6: ;
    eax = esp + 0x34;
    PUSH32(esp, eax);
    eax = esi;
    ecx = edi + 0x20;
    esi = esp + 0x38;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023F09u); RECOMP_ABI_CALL(0x00023B90u, sub_00023B90); /* call 0x00023B90 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023F09: ;
    eax = MEM32(esp + 0x34);
    edx = esi;
    esi = MEM32(esp + 0x14);
    ecx = edi + 0x40;
    PUSH32(esp, edx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023F1Cu); RECOMP_ABI_CALL(0x00023AF0u, sub_00023AF0); /* call 0x00023AF0 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023F1C: ;
    eax = esp + 0x3C;
    PUSH32(esp, eax);
    eax = MEM32(esp + 0x38);
    ecx = edi + 0x60;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023F2Du); RECOMP_ABI_CALL(0x00023A60u, sub_00023A60); /* call 0x00023A60 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023F2D: ;
    eax = MEM32(esp + 0x34);
    edx = esp + 0x40;
    ecx = edi + 0x80;
    PUSH32(esp, edx);
    esi = edx;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023F43u); RECOMP_ABI_CALL(0x00023B90u, sub_00023B90); /* call 0x00023B90 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023F43: ;
    eax = esi;
    ecx = edi + 0xA0;
    edi = MEM32(esp + 0x20);
    PUSH32(esp, eax);
    eax = MEM32(esp + 0x38);
    esi = edi + 0x20;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023F5Cu); RECOMP_ABI_CALL(0x00023AF0u, sub_00023AF0); /* call 0x00023AF0 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023F5C: ;
    eax = MEM32(esp + 0x28);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(0x40)) >> 32) & 1);
    edi = edi + 0x40;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x18)) >> 32) & 1);
    esp = esp + 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    MEM32(esp + 0xC) = edi;
    edi = MEM32(esp + 0x1C);
    MEM32(esp + 0x10) = eax;
    if ((_fa != 0)) goto loc_00023E70; /* jne: not equal / not zero */

loc_00023F79: ;
    esi = MEM32(ebp + 8);

loc_00023F7C: ;
    eax = MEM32(esi + 8);
    edx = MEM32(esi);
    ecx = MEM32(esi + 4);
    edi = eax + edx;
    edi = edi + eax * 2;
    edi = edi + ecx * 2;
    _fb = (uint32_t)(ecx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ecx)) >> 32) & 1);
    eax = eax + ecx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx = MEM32(0x38C4B4);
    if (((5) & 31u)) _cf = (int)(((uint32_t)(edi) >> ((32 - (((5) & 31u))) & 31u)) & 1u);
    edi = (uint32_t)(edi) << ((5) & 31u);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(MEM32(esi + 0x10)) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(MEM32(esi + 0x10))) >> 32) & 1);
    edi = edi + MEM32(esi + 0x10);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(edx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(edx)) >> 32) & 1);
    eax = eax + edx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_push(MEMF(edi + 0xC)); /* fld float */
    edx = MEM32(esi + ecx * 4 + 0x14);
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    if (((5) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((5) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((5) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(edx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(edx)) >> 32) & 1);
    eax = eax + edx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0xC) = eax;
    MEM32(esp + 0x10) = 0;
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (TEST_NZ(_fa, _fb)) goto loc_000240B0; /* jne: not equal / not zero */

loc_00023FC6: ;
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    esi = MEM32(edi + 0x3C);
    eax = edi + 0x60;
    MEM32(esp + 0x28) = eax;
    eax = MEM32(edi + 0x5C);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 0x2C) = eax;
    eax = MEM32(edi + 0x1C);
    edx = esp + 0x30;
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    if (((6) & 31u)) _cf = (int)(((uint32_t)(esi) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    esi = (uint32_t)(esi) << ((6) & 31u);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, edx);
    ecx = edi;
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(ebx)) >> 32) & 1);
    esi = esi + ebx;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00023FF9u); RECOMP_ABI_CALL(0x00023A60u, sub_00023A60); /* call 0x00023A60 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00023FF9: ;
    eax = esp + 0x34;
    PUSH32(esp, eax);
    eax = esi;
    ecx = edi + 0x20;
    esi = esp + 0x38;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002400Cu); RECOMP_ABI_CALL(0x00023B90u, sub_00023B90); /* call 0x00023B90 */
    _cf = g_eflags & 1u; /* callee carry */

loc_0002400C: ;
    eax = MEM32(esp + 0x34);
    edx = esi;
    ecx = edi + 0x40;
    PUSH32(esp, edx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002401Bu); RECOMP_ABI_CALL(0x00023B90u, sub_00023B90); /* call 0x00023B90 */
    _cf = g_eflags & 1u; /* callee carry */

loc_0002401B: ;
    eax = MEM32(esp + 0x1C);
    ecx = MEM32(esp + 0x34);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_LE(_fas, _fbs)) goto loc_0002405C; /* jle: less or equal (signed <=) */
    if ((uint32_t)eax > 32u) {
        FILE *f = Switch_GetMainLogFile();
        if (f) { fprintf(f, "[sub_00023C30 Safe] Clamping bogus child count: 0x%x -> 0\n", eax); fflush(f); }
        goto loc_0002405C;
    }

loc_0002402A: ;
    MEM32(esp + 0x14) = eax;
    edi = edi;

loc_00024030: ;
    eax = ZX8(MEM8(ecx + 0x1C));
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    edx = esp + 0x30;
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    edi = ecx + 0x20;
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, edx);
    esi = edx;
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002404Cu); RECOMP_ABI_CALL(0x00023B90u, sub_00023B90); /* call 0x00023B90 */
    _cf = g_eflags & 1u; /* callee carry */

loc_0002404C: ;
    eax = MEM32(esp + 0x18);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    ecx = edi;
    MEM32(esp + 0x14) = eax;
    if ((_fa != 0)) goto loc_00024030; /* jne: not equal / not zero */

loc_0002405C: ;
    eax = ZX8(MEM8(ecx + 0x1C));
    esi = MEM32(esp + 0xC);
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    (void)0; /* prefetchnta: cache/ordering hint, nothing to model */
    edx = esp + 0x30;
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    edi = ecx + 0x20;
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ebx)) >> 32) & 1);
    eax = eax + ebx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, edx);
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024082u); RECOMP_ABI_CALL(0x00023AF0u, sub_00023AF0); /* call 0x00023AF0 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00024082: ;
    fp_push(MEMF(edi + 0xC)); /* fld float */
    eax = MEM32(edi + -4);
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    ecx = MEM32(esp + 0x14);
    if (((8) & 31u)) _cf = (int)(((uint32_t)(eax) >> (((((8) & 31u)) - 1) & 31u)) & 1u);
    eax = (uint32_t)(eax) >> ((8) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(eax)) >> 32) & 1);
    ecx = ecx + eax;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x20)) >> 32) & 1);
    esi = esi + 0x20;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    MEM32(esp + 0xC) = esi;
    MEM32(esp + 0x10) = ecx;
    _sc2_outer_iters++;
    {
        static int s_dump_count = 0;
        if (s_dump_count < 100) {
            FILE *f = Switch_GetMainLogFile();
            if (f) {
                float fval = MEMF(edi + 0xC);
                uint32_t uval = MEM32(edi + 0xC);
                uint32_t m4 = MEM32(edi - 4);
                float f_ref = MEMF(0x1761A0);
                uint32_t u_ref = MEM32(0x1761A0);
                fprintf(f, "[NodeLog #%d] iter=%d edi=0x%x f_0c=%f (0x%08x) ref=%f (0x%08x) g_fp_cc=0x%04x test_z=%d m4=0x%08x (>>8=%u)\n",
                    s_dump_count, _sc2_outer_iters, edi, (double)fval, uval, (double)f_ref, u_ref, g_fp_cc, TEST_Z(_fa, _fb), m4, m4 >> 8);
                fflush(f);
                s_dump_count++;
            }
        }
    }
    if (_sc2_outer_iters > 1024) {
        FILE *f = Switch_GetMainLogFile();
        if (f) { fprintf(f, "[sub_00023C30 Safe] Outer loop reached 1024 iters! Terminating bone hierarchy cleanly.\n"); fflush(f); }
        goto loc_000240B0;
    }
    if (TEST_Z(_fa, _fb)) goto loc_00023FC6; /* je: equal / zero */

    {
        static int s_nat_count = 0;
        if (s_nat_count < 10) {
            FILE *f = Switch_GetMainLogFile();
            if (f) {
                fprintf(f, "[sub_00023C30] NATURAL TERMINATION #%d: total_iters=%d f_0c=%f (0x%08x)\n",
                    s_nat_count, _sc2_outer_iters, (double)MEMF(edi + 0xC), MEM32(edi + 0xC));
                fflush(f);
                s_nat_count++;
            }
        }
    }

loc_000240B0: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) & (_fb))) ? 0x4u : 0u) | ((TEST_Z(_fa, _fb)) ? 0x40u : 0u) | ((TEST_S(_fas, _fbs)) ? 0x80u : 0u) | ((0) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

