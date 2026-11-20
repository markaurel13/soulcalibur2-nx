/**
 * LTO-enabled extraction of hot pre-draw callees from recomp_0001.c:
 * sub_00040580, sub_00040670, sub_00067D70, sub_000688E0.
 */
#define RECOMP_GENERATED_CODE 1
#include "recomp_funcs.h"
#include "sc2_fast_mem.h"
#include "x87_math.h"

void sc2_fast_sub_00040580(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fk = 0; /* tag of the last flag setter */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00040580: ;
    PUSH32(esp, edi);
    edi = MEM32(esp + 0xC);
    _fa = (uint32_t)(MEM16(edi + 0x52)) & 0xFFFFu; _fb = (uint32_t)(0xFFFFFFFFu) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(edi + 0x52), 0xFFFFFFFFu (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x3B133DDBu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004065F; /* je: equal / zero */

loc_00040590: ;
    SET_LO16(eax, MEM16(edi + 0x54));
    _fa = (uint32_t)(LO16(eax)) & 0xFFFFu; _fb = (uint32_t)(0xFFFF) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp LO16(eax), 0xFFFF (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x68D865ECu; /* flag setter tag */
    PUSH32(esp, esi);
    esi = MEM32(esp + 0xC);
    if (CMP_EQ(_fa, _fb)) goto loc_000405AD; /* je: equal / zero */

loc_0004059F: ;
    SET_LO8(ecx, MEM8(esi + 0xD9));
    _fa = (uint32_t)(LO8(ecx)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(ecx), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xF68F40CDu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_000405AD; /* je: equal / zero */

loc_000405A9: ;
    MEM16(edi + 0x52) = LO16(eax);

loc_000405AD: ;
    SET_LO16(eax, MEM16(edi + 0x56));
    _fa = (uint32_t)(LO16(eax)) & 0xFFFFu; _fb = (uint32_t)(0xFFFF) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp LO16(eax), 0xFFFF (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x68D865ECu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_000405C5; /* je: equal / zero */

loc_000405B7: ;
    SET_LO8(ecx, MEM8(esi + 0xC2));
    _fa = (uint32_t)(LO8(ecx)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(ecx), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xF68F40CDu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_000405C5; /* je: equal / zero */

loc_000405C1: ;
    MEM16(edi + 0x52) = LO16(eax);

loc_000405C5: ;
    SET_LO16(eax, MEM16(edi + 0x58));
    _fa = (uint32_t)(LO16(eax)) & 0xFFFFu; _fb = (uint32_t)(0xFFFF) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp LO16(eax), 0xFFFF (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x68D865ECu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_000405E9; /* je: equal / zero */

loc_000405CF: ;
    SET_LO8(ecx, MEM8(esi + 0xC2));
    _fa = (uint32_t)(LO8(ecx)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(ecx), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xF68F40CDu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_000405E9; /* je: equal / zero */

loc_000405D9: ;
    ecx = MEM32(esi + 8);
    _fa = (uint32_t)(MEM32(ecx + 0x15B4)) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ecx + 0x15B4), 4 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xFC7EF02Eu; /* flag setter tag */
    if (CMP_L(_fas, _fbs)) goto loc_000405E9; /* jl: less (signed <) */

loc_000405E5: ;
    MEM16(edi + 0x52) = LO16(eax);

loc_000405E9: ;
    edx = (uint32_t)(int32_t)SMEM16(edi + 0x4E);
    SET_LO16(ecx, MEM16(edi));
    _fa = ((int64_t)(int32_t)(edx) * (int64_t)(int32_t)(0x380) != (int64_t)(int32_t)((uint32_t)(edx) * (uint32_t)(0x380)));
    edx = (uint32_t)((int32_t)edx * (int32_t)0x380);
    _fk = 0x7BC637AEu; /* flag setter tag */
    _fa = (uint32_t)(LO16(ecx)) & 0xFFFFu; _fb = (uint32_t)(1) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp LO16(ecx), 1 (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x5C2A9023u; /* flag setter tag */
    eax = edx + esi + 0x1C28;
    if (CMP_NE(_fa, _fb)) goto loc_0004060E; /* jne: not equal / not zero */

loc_00040603: ;
    _fa = (uint32_t)(MEM16(eax)) & 0xFFFFu; _fb = (uint32_t)(0) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(eax), 0 (16-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xAFD249BFu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004060E; /* jne: not equal / not zero */

loc_00040609: ;
    fp_push(MEMF(eax + 0x20)); /* fld float */
    goto loc_00040611;

loc_0004060E: ;
    fp_push(MEMF(eax + 8)); /* fld float */

loc_00040611: ;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(edi + 0x60)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [edi + 0x60] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x42694A75u; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_0004065E; /* jne: not equal / not zero */

loc_0004061B: ;
    SET_LO16(eax, MEM16(edi + 0x52));
    _fa = (uint32_t)(LO16(eax)) & 0xFFFFu; _fb = (uint32_t)(0xFFFD) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp LO16(eax), 0xFFFD (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x8182C412u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00040659; /* je: equal / zero */

loc_00040625: ;
    edx = MEM32(edi + 0x5C);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, edx);
    eax = SX16(LO16(eax));
    ecx = SX16(LO16(ecx));
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004063Fu); sub_0003FCF0(); /* call 0x0003FCF0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004063F: ;
    edx = (uint32_t)(int32_t)SMEM16(edi);
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x20)) >> 32) & 1);
    esp = esp + 0x20;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xF9C06C5Fu; /* flag setter tag */
    _fa = ((int64_t)(int32_t)(edx) * (int64_t)(int32_t)(0x380) != (int64_t)(int32_t)((uint32_t)(edx) * (uint32_t)(0x380)));
    edx = (uint32_t)((int32_t)edx * (int32_t)0x380);
    _fk = 0x7BC637AEu; /* flag setter tag */
    MEM32(edx + esi + 0x1C54) = 1;
    POP32(esp, esi);
    POP32(esp, edi);
    g_eflags = (g_eflags & ~1u) | (((_fa != 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

loc_00040659: ;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004065Eu); sub_0003FB80(); /* call 0x0003FB80 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004065E: ;
    POP32(esp, esi);

loc_0004065F: ;
    POP32(esp, edi);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x3B133DDBu ? (CMP_B(_fa, _fb)) : (_fk == 0x00000000u ? ((g_eflags & 1u) /* eflags */) : CMP_B(_fa & _fb, 0)))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

void sc2_fast_sub_00040670(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fk = 0; /* tag of the last flag setter */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_00040670: ;
    PUSH32(esp, ebx);
    ebx = MEM32(esp + 0xC);
    eax = ebx;
    _fa = ((int64_t)(int32_t)(eax) * (int64_t)(int32_t)(0x380) != (int64_t)(int32_t)((uint32_t)(eax) * (uint32_t)(0x380)));
    eax = (uint32_t)((int32_t)eax * (int32_t)0x380);
    _fk = 0xC299E0E7u; /* flag setter tag */
    PUSH32(esp, esi);
    esi = MEM32(esp + 0xC);
    MEM16(eax + esi + 0x1C74) = MEM16(eax + esi + 0x1C74) + 1;
    _fa = (uint32_t)(MEM16(eax + esi + 0x1C74)) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fb = (_fa == 0x8000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x3C1775D2u; /* flag setter tag */
    PUSH32(esp, edi);
    edi = eax + esi + 0x1C28;
    SET_LO16(eax, MEM16(edi + 0x4C));
    _fa = (uint32_t)(LO16(eax)) & 0xFFFFu; _fb = (uint32_t)(0x10) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp LO16(eax), 0x10 (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x573FFD61u; /* flag setter tag */
    if (CMP_G(_fas, _fbs)) goto loc_0004075D; /* jg: greater (signed >) */

loc_000406A0: ;
    PUSH32(esp, edi);
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000406A7u); sub_0003D4A0(); /* call 0x0003D4A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000406A7: ;
    PUSH32(esp, edi);
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000406AEu); sc2_fast_sub_00040580(); /* direct in-TU LTO call */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000406AE: ;
    PUSH32(esp, edi);
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000406B5u); sub_0003D870(); /* call 0x0003D870 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000406B5: ;
    eax = (uint32_t)(int32_t)SMEM16(edi + 2);
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x18)) >> 32) & 1);
    esp = esp + 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xBD67F1D5u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xFFFFFFFEu) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xFFFFFFFEu (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x2C757611u; /* flag setter tag */
    if (CMP_L(_fas, _fbs)) goto loc_000406C5; /* jl: less (signed <) */

loc_000406C1: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_L(_fas, _fbs)) goto loc_000406FF; /* jl: less (signed <) */

loc_000406C5: ;
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x504457D4u; /* flag setter tag */
    MEM16(edi + 0x48) = 1;
    MEM32(0x3980C4) = eax;
    MEM32(0x3980B8) = eax;
    ecx = (uint32_t)(int32_t)SMEM16(edi + 2);
    PUSH32(esp, ecx);
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~1u) | ((0) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000406E2u); sub_0003E310(); /* call 0x0003E310 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000406E2: ;
    PUSH32(esp, eax);
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000406E9u); sub_0009A900(); /* call 0x0009A900 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000406E9: ;
    eax = MEM32(0x3980B8);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x10)) >> 32) & 1);
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7E08D19Du; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_000406FF; /* je: equal / zero */

loc_000406F5: ;
    PUSH32(esp, edi);
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000406FCu); sc2_fast_sub_00040580(); /* direct in-TU LTO call */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000406FC: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */

loc_000406FF: ;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x11A6DAE2u ? (CMP_B(_fa, _fb)) : _cf)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00040706u); sub_000436E0(); /* call 0x000436E0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00040706: ;
    edx = MEM32(esi + 0x1664);
    PUSH32(esp, edx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00040712u); sub_00033E40(); /* call 0x00033E40 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00040712: ;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x177898)); /* fmul dword ptr [0x177898] */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004071Du); sub_00145A64(); /* call 0x00145A64 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004071D: ;
    PUSH32(esp, eax);
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00040725u); sub_0003E670(); /* call 0x0003E670 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00040725: ;
    eax = MEM32(edi + 0x2C);
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x18)) >> 32) & 1);
    esp = esp + 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xBD67F1D5u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00040740; /* je: equal / zero */

loc_0004072F: ;
    _fa = (uint32_t)(MEM16(edi + 0x48)) & 0xFFFFu; _fb = (uint32_t)(0) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(edi + 0x48), 0 (16-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x864A09FDu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_00040740; /* jne: not equal / not zero */

loc_00040736: ;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004073Du); sub_0003F230(); /* call 0x0003F230 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004073D: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */

loc_00040740: ;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0xD0F44B25u ? (_cf) : (_fk == 0x11A6DAE2u ? (CMP_B(_fa, _fb)) : CMP_B(_fa, _fb)))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00040747u); sub_0003F520(); /* call 0x0003F520 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00040747: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004077C; /* je: equal / zero */

loc_0004074E: ;
    MEM16(edi + 0x4C) = MEM16(edi + 0x4C) + 1;
    _fa = (uint32_t)(MEM16(edi + 0x4C)) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fb = (_fa == 0x8000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x76D6704Eu; /* flag setter tag */
    _fa = (uint32_t)(MEM16(edi + 0x4C)) & 0xFFFFu; _fb = (uint32_t)(0x10) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(edi + 0x4C), 0x10 (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x935B4FA1u; /* flag setter tag */
    if (CMP_LE(_fas, _fbs)) goto loc_000406A0; /* jle: less or equal (signed <=) */

loc_0004075D: ;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x711257E0u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_00040777; /* jne: not equal / not zero */

loc_00040761: ;
    PUSH32(esp, ebx);
    PUSH32(esp, ebx);
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, ebx);
    PUSH32(esp, ebx);
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00040770u); sub_0003FCF0(); /* call 0x0003FCF0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00040770: ;
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x20)) >> 32) & 1);
    esp = esp + 0x20;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xF9C06C5Fu; /* flag setter tag */
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

loc_00040777: ;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004077Cu); sub_0003FB80(); /* call 0x0003FB80 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004077C: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x00000000u ? ((g_eflags & 1u) /* eflags */) : CMP_B(_fa, _fb))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

void sc2_fast_sub_00067D70(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* prologue saves caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_00067D70: ;
    PUSH32(esp, ebp);
    ebp = esp;
    g_ebp = ebp; /* publish frame for frameless callees */
    g_seh_ebp = ebp;
    _cf = 0; /* logical op clears CF */
    esp = esp & 0xFFFFFFF0u;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fb = (uint32_t)(0x74) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0x74));
    esp = esp - 0x74;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    SET_LO8(eax, MEM8(ebp + 0xC));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    edi = ecx;
    ebx = MEM32(edi + 0x44);
    if (CMP_EQ(_fa, _fb)) goto loc_00067E42; /* je: equal / zero */

loc_00067D8C: ;
    PUSH32(esp, edi);
    ecx = esp + 0x44;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00067D96u); sub_00011130(); /* call 0x00011130 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00067D96: ;
    ecx = (uint32_t)(int32_t)SMEM16(ebx + 0x22);
    esi = MEM32(ebp + 8);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(ecx) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    ecx = (uint32_t)(ecx) << ((6) & 31u);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    eax = esp + 0x40;
    _fb = (uint32_t)(esi) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(esi)) >> 32) & 1);
    ecx = ecx + esi;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = esp + 0x48;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00067DB1u); sub_00011390(); /* call 0x00011390 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00067DB1: ;
    edx = esp + 0x40;
    PUSH32(esp, edx);
    ecx = esp + 0x14;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00067DBFu); sub_000120E0(); /* call 0x000120E0 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00067DBF: ;
    fp_push(MEMF(esp + 0x1C)); /* fld float */
    eax = (uint32_t)(int32_t)SMEM16(ebx + 0x22);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x1C)); /* fmul dword ptr [esp + 0x1c] */
    if (((6) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((6) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    ecx = MEM32(eax + esi + 0x30);
    edx = MEM32(eax + esi + 0x34);
    fp_top() = RECOMP_FP_PC(MEMF(0x176190) - fp_top()); /* fsubr dword ptr [0x176190] */
    _fb = (uint32_t)(esi) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(esi)) >> 32) & 1);
    eax = eax + esi;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    eax = MEM32(eax + 0x38);
    fp_top() = fabs(fp_top()); /* fabs */
    fp_top() = RECOMP_FP_PC(sqrt(fp_top())); /* fsqrt */
    MEM32(esp + 0x30) = ecx;
    ecx = MEM32(esp + 0x10);
    PUSH32(esp, ecx);
    MEM32(esp + 0x38) = edx;
    edx = MEM32(esp + 0x18);
    MEM32(esp + 0x24) = ecx;
    ecx = esp + 0x24;
    MEM32(esp + 0x28) = edx;
    MEM32(esp + 0x3C) = eax;
    eax = MEM32(esp + 0x1C);
    edx = esp + 0x34;
    MEM32(esp + 0x2C) = eax;
    fp_push(MEMF(esp + 0x20)); /* fld float */
    { fp_st1() = atan2(fp_st1(), fp_top()); fp_pop(); } /* fpatan */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x176194)); /* fmul dword ptr [0x176194] */
    fp_top() = RECOMP_FP_PC(fp_top() + fp_top()); /* fadd st(0), st(0) */
    fp_top() = fabs(fp_top()); /* fabs */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00067E2Cu); sub_00067CE0(); /* call 0x00067CE0 */
    _cf = g_eflags & 1u; /* callee carry */

loc_00067E2C: ;
    esi = (uint32_t)(int32_t)SMEM16(ebx + 0x22);
    eax = MEM32(ebp + 8);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(esi) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    esi = (uint32_t)(esi) << ((6) & 31u);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(eax)) >> 32) & 1);
    esi = esi + eax;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ecx = 0x10;
    if (!g_df) { uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi); uint32_t _n = ecx * 4;
      if (_d + _n <= _s || _s + _n <= _d) memcpy(_d, _s, _n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4) = MEM32(esi + _i*4); }
      esi += ecx * 4; edi += ecx * 4; }
    else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi - _i*4) = MEM32(esi - _i*4); esi -= ecx * 4; edi -= ecx * 4; }
    ecx = 0; /* rep movsd */

loc_00067E42: ;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_EQ(_fa, _fb)) goto loc_00067E5A; /* je: equal / zero */

loc_00067E46: ;
    ecx = MEM32(ebp + 8);
    eax = MEM32(ebx);
    esi = MEM32(ebx + 0x24);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    ecx = ebx;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; { uint32_t _icall_target = MEM32(eax + 4); PUSH32(esp, 0x00067E54u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x00067E51u); } /* indirect call */
    }
    _cf = g_eflags & 1u; /* callee carry */

loc_00067E54: ;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    ebx = esi;
    if (CMP_NE(_fa, _fb)) goto loc_00067E46; /* jne: not equal / not zero */

loc_00067E5A: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 12; return; /* ret 8 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

void sc2_fast_sub_000688E0(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* prologue saves caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fk = 0; /* tag of the last flag setter */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_000688E0: ;
    PUSH32(esp, ebp);
    ebp = esp;
    g_ebp = ebp; /* publish frame for frameless callees */
    g_seh_ebp = ebp;
    _cf = 0; /* logical op clears CF */
    esp = esp & 0xFFFFFFF0u;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0xDA254061u; /* flag setter tag */
    _fb = (uint32_t)(0xA54) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0xA54));
    esp = esp - 0xA54;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0x02F95F2Eu; /* flag setter tag */
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    MEM32(esp + 0x48) = ecx;
    PUSH32(esp, edi);
    eax = esp + 0x5A8;
    ecx = 0x1A;
    /* nop */

loc_00068900: ;
    edx = 0x3F800000;
    MEM32(eax + 4) = edx;
    MEM32(eax) = edx;
    MEM32(eax + -4) = edx;
    MEM32(eax + -8) = edx;
    _fb = (uint32_t)(0x30) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0x30)) >> 32) & 1);
    eax = eax + 0x30;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x96CA5155u; /* flag setter tag */
    ecx--;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x9CE409BDu; /* flag setter tag */
    if ((_fa != 0)) goto loc_00068900; /* jne: not equal / not zero */

loc_00068916: ;
    eax = esp + 0xC8;
    ecx = 0x1A;

loc_00068922: ;
    edx = 0x3F800000;
    MEM32(eax + 4) = edx;
    MEM32(eax) = edx;
    MEM32(eax + -4) = edx;
    MEM32(eax + -8) = edx;
    _fb = (uint32_t)(0x30) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0x30)) >> 32) & 1);
    eax = eax + 0x30;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x96CA5155u; /* flag setter tag */
    ecx--;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x9CE409BDu; /* flag setter tag */
    if ((_fa != 0)) goto loc_00068922; /* jne: not equal / not zero */

loc_00068938: ;
    ebx = MEM32(ebp + 8);
    _fb = (uint32_t)(0xF80) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ebx) + (uint64_t)(0xF80)) >> 32) & 1);
    ebx = ebx + 0xF80;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x3907325Fu; /* flag setter tag */
    MEM32(esp + 0x18) = 0;
    MEM32(esp + 0x1C) = 0x1A;

loc_00068951: ;
    eax = MEM32(esp + 0x18);
    ecx = MEM32(ebx + 0x30);
    edx = MEM32(ebx + 0x34);
    eax = esp + eax + 0x598;
    MEM32(eax + -8) = ecx;
    ecx = MEM32(ebx + 0x38);
    esi = eax + 0x10;
    edi = eax + 0xC;
    PUSH32(esp, esi);
    MEM32(eax) = ecx;
    MEM32(eax + -4) = edx;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(8)) >> 32) & 1);
    eax = eax + 8;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x525D6E6Du; /* flag setter tag */
    PUSH32(esp, edi);
    PUSH32(esp, eax);
    ecx = ebx;
    MEM32(esp + 0x34) = eax;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068984u); sub_00011C20(); /* call 0x00011C20 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068984: ;
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esi)); /* fdiv dword ptr [esi] */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0xC));
    esp = esp - 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0xD4620F2Du; /* flag setter tag */
    edx = MEM32(esp + 0x34);
    ecx = esp + 0x6C;
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(edi)); /* fdiv dword ptr [edi] */
    MEMF(esp + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(edx)); /* fdiv dword ptr [edx] */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, ebx);
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000689B8u); sub_00011B60(); /* call 0x00011B60 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000689B8: ;
    edi = MEM32(esp + 0x18);
    eax = esp + 0x60;
    esi = esp + edi + 0x580;
    PUSH32(esp, eax);
    ecx = esi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000689CFu); sub_000120E0(); /* call 0x000120E0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000689CF: ;
    eax = MEM32(esp + 0x1C);
    edi = esp + edi + 0xA0;
    ecx = 0xC;
    if (!g_df) { uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi); uint32_t _n = ecx * 4;
      if (_d + _n <= _s || _s + _n <= _d) memcpy(_d, _s, _n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4) = MEM32(esi + _i*4); }
      esi += ecx * 4; edi += ecx * 4; }
    else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi - _i*4) = MEM32(esi - _i*4); esi -= ecx * 4; edi -= ecx * 4; }
    ecx = 0; /* rep movsd */
    ecx = MEM32(esp + 0x18);
    _fb = (uint32_t)(0x30) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0x30)) >> 32) & 1);
    ecx = ecx + 0x30;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x9451CA54u; /* flag setter tag */
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ebx) + (uint64_t)(0x40)) >> 32) & 1);
    ebx = ebx + 0x40;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xF1EBBFE0u; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x18) = ecx;
    MEM32(esp + 0x1C) = eax;
    if ((_fa != 0)) goto loc_00068951; /* jne: not equal / not zero */

loc_000689FA: ;
    eax = MEM32(esp + 0x4C);
    esi = eax + 0x288;
    MEM32(esp + 0x24) = esi;
    MEM32(esp + 0x2C) = 5;

loc_00068A10: ;
    ecx = MEM32(esi);
    _cf = 0; /* logical op clears CF */
    ecx = ecx | MEM32(esi + 4);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    _fk = 0xFAD4E1F5u; /* flag setter tag */
    if ((_fa == 0)) goto loc_00068B15; /* je: equal / zero */

loc_00068A1B: ;
    ecx = MEM32(esi + -12);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xE7AED121u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00068B15; /* je: equal / zero */

loc_00068A26: ;
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0);
    PUSH32(esp, 0x17A2A8);
    PUSH32(esp, eax);
    edx = esp + 0xBC;
    PUSH32(esp, edx);
    ecx = esi + -16;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068A46u); sub_0007ED30(); /* call 0x0007ED30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068A46: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(8)) >> 32) & 1);
    esi = esi + 8;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x45C4B897u; /* flag setter tag */
    MEM32(esp + 0x20) = esi;
    MEM32(esp + 0x50) = 1;
    MEM32(esp + 0x54) = 0;
    edi = esp + 0x590;
    ebx = esp + 0xC0;
    _cf = 0; /* xor clears CF */
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x9F87D19Fu; /* flag setter tag */
    MEM32(esp + 0x1C) = 0x1A;

loc_00068A75: ;
    ecx = MEM32(esp + 0x24);
    eax = MEM32(ecx);
    edx = MEM32(esp + 0x50);
    ecx = MEM32(ecx + 4);
    _cf = 0; /* logical op clears CF */
    eax = eax & edx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0x85196A23u; /* flag setter tag */
    _cf = 0; /* logical op clears CF */
    ecx = ecx & MEM32(esp + 0x54);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0xFC5AE811u; /* flag setter tag */
    _cf = 0; /* logical op clears CF */
    eax = eax | ecx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    _fk = 0xDADFDF7Eu; /* flag setter tag */
    if ((_fa == 0)) goto loc_00068AD4; /* je: equal / zero */

loc_00068A8C: ;
    eax = MEM32(esp + 0x20);
    ecx = MEM32(eax);
    MEM32(esp + 0x18) = ecx;
    edx = MEM32(esp + 0x18);
    PUSH32(esp, edx);
    eax = esp + esi + 0xA4;
    ecx = esp + esi + 0x584;
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    g_eflags = (g_eflags & ~1u) | ((0) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068AB0u); sub_000123C0(); /* call 0x000123C0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068AB0: ;
    ecx = MEM32(esp + 0x18);
    PUSH32(esp, ecx);
    edx = ebx + -16;
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    ecx = edi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068AC1u); sub_000129F0(); /* call 0x000129F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068AC1: ;
    eax = MEM32(esp + 0x18);
    PUSH32(esp, eax);
    ecx = esp + esi + 0x5A4;
    PUSH32(esp, ebx);
    PUSH32(esp, ecx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068AD4u); sub_000129F0(); /* call 0x000129F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068AD4: ;
    ecx = MEM32(esp + 0x20);
    edx = MEM32(esp + 0x54);
    eax = MEM32(esp + 0x1C);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(4)) >> 32) & 1);
    ecx = ecx + 4;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xAD912013u; /* flag setter tag */
    MEM32(esp + 0x20) = ecx;
    ecx = MEM32(esp + 0x50);
    { uint32_t _c = (uint32_t)(1) & 31u; if (_c && _c < 32u) { edx = (edx << _c) | (ecx >> (32 - _c)); _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shld result */ } }  /* shld */
    _fk = 0xCF60DEC1u; /* flag setter tag */
    if (((1) & 31u)) _cf = (int)(((uint32_t)(ecx) >> ((32 - (((1) & 31u))) & 31u)) & 1u);
    ecx = (uint32_t)(ecx) << ((1) & 31u);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x5A35DC62u; /* flag setter tag */
    _fb = (uint32_t)(0x30) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(0x30)) >> 32) & 1);
    edi = edi + 0x30;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x3430E82Au; /* flag setter tag */
    _fb = (uint32_t)(0x30) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x30)) >> 32) & 1);
    esi = esi + 0x30;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xCBB5D381u; /* flag setter tag */
    _fb = (uint32_t)(0x30) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ebx) + (uint64_t)(0x30)) >> 32) & 1);
    ebx = ebx + 0x30;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x78A484F4u; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x50) = ecx;
    MEM32(esp + 0x54) = edx;
    MEM32(esp + 0x1C) = eax;
    if ((_fa != 0)) goto loc_00068A75; /* jne: not equal / not zero */

loc_00068B0D: ;
    eax = MEM32(esp + 0x4C);
    esi = MEM32(esp + 0x24);

loc_00068B15: ;
    ecx = MEM32(esp + 0x2C);
    _fb = (uint32_t)(0x80) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x80)) >> 32) & 1);
    esi = esi + 0x80;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xB6A2FC3Du; /* flag setter tag */
    ecx--;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x9CE409BDu; /* flag setter tag */
    MEM32(esp + 0x24) = esi;
    MEM32(esp + 0x2C) = ecx;
    if ((_fa != 0)) goto loc_00068A10; /* jne: not equal / not zero */

loc_00068B2E: ;
    ecx = MEM32(eax + 0x4F8);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xE7AED121u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00068E4E; /* je: equal / zero */

loc_00068B3C: ;
    _fb = (uint32_t)(0xF80) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0xF80)) >> 32) & 1);
    ecx = ecx + 0xF80;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x06640ED4u; /* flag setter tag */
    edx = esp + 0x5A0;
    MEM32(esp + 0x2C) = edx;
    edx = MEM32(esp + 0x4C);
    MEM32(esp + 0x1C) = ecx;
    ecx = eax + 0x1A0;
    eax = esp + 0xC8;
    _fb = (uint32_t)(ecx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(edx) < (uint32_t)(ecx));
    edx = edx - ecx;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0x9E29E0F5u; /* flag setter tag */
    MEM32(esp + 0x20) = eax;
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x3E0F5DA7u; /* flag setter tag */
    eax = esp + 0x590;
    _fb = (uint32_t)(0x208) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edx) + (uint64_t)(0x208)) >> 32) & 1);
    edx = edx + 0x208;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x8593353Du; /* flag setter tag */
    MEM32(esp + 0x5C) = ecx;
    MEM32(esp + 0x18) = eax;
    ebx = esp + 0xC4;
    MEM32(esp + 0x50) = edx;
    MEM32(esp + 0x28) = 0x1A;

loc_00068B92: ;
    eax = MEM32(esp + 0x50);
    fp_push(MEMF(eax + ecx)); /* fld float */
    MEMF(esp + 0x24) = (float)fp_top(); /* fst */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A4)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a4] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_00068E02; /* jne: not equal / not zero */

loc_00068BAE: ;
    esi = MEM32(ecx);
    ecx = MEM32(esp + 0x4C);
    edx = MEM32(ecx + 0x4F8);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(esi) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    esi = (uint32_t)(esi) << ((6) & 31u);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0xC84A92CFu; /* flag setter tag */
    _fb = (uint32_t)(edx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(edx)) >> 32) & 1);
    esi = esi + edx;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x66FDF04Du; /* flag setter tag */
    edx = MEM32(esi);
    MEM32(esp + 0x30) = edx;
    eax = MEM32(esi + 4);
    MEM32(esp + 0x34) = eax;
    ecx = MEM32(esi + 8);
    MEM32(esp + 0x38) = ecx;
    ecx = esp + 0x30;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068BDCu); sub_00012B00(); /* call 0x00012B00 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068BDC: ;
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / fp_st1()); /* fdiv st(1) */
    edx = esp + 0x30;
    PUSH32(esp, edx);
    PUSH32(esp, ecx);
    ecx = edx;
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068BF6u); sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068BF6: ;
    ecx = MEM32(eax);
    MEM32(esp + 0x60) = ecx;
    edx = MEM32(eax + 4);
    MEM32(esp + 0x70) = edx;
    ecx = MEM32(eax + 8);
    MEM32(esp + 0x80) = ecx;
    edx = MEM32(eax + 0xC);
    MEM32(esp + 0x90) = edx;
    eax = MEM32(esi + 0x10);
    MEM32(esp + 0x30) = eax;
    ecx = MEM32(esi + 0x14);
    MEM32(esp + 0x34) = ecx;
    edx = MEM32(esi + 0x18);
    ecx = esp + 0x30;
    MEM32(esp + 0x38) = edx;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068C35u); sub_00012B00(); /* call 0x00012B00 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068C35: ;
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / fp_st1()); /* fdiv st(1) */
    eax = esp + 0x30;
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = eax;
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068C4Fu); sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068C4F: ;
    ecx = MEM32(eax);
    MEM32(esp + 0x64) = ecx;
    edx = MEM32(eax + 4);
    MEM32(esp + 0x74) = edx;
    ecx = MEM32(eax + 8);
    MEM32(esp + 0x84) = ecx;
    edx = MEM32(eax + 0xC);
    MEM32(esp + 0x94) = edx;
    eax = MEM32(esi + 0x20);
    MEM32(esp + 0x30) = eax;
    ecx = MEM32(esi + 0x24);
    MEM32(esp + 0x34) = ecx;
    edx = MEM32(esi + 0x28);
    ecx = esp + 0x30;
    MEM32(esp + 0x38) = edx;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068C8Eu); sub_00012B00(); /* call 0x00012B00 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068C8E: ;
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / fp_st1()); /* fdiv st(1) */
    eax = esp + 0x30;
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = eax;
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068CA8u); sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068CA8: ;
    ecx = MEM32(eax);
    MEM32(esp + 0x68) = ecx;
    edx = MEM32(eax + 4);
    MEM32(esp + 0x78) = edx;
    ecx = MEM32(eax + 8);
    MEM32(esp + 0x88) = ecx;
    edx = MEM32(eax + 0xC);
    MEM32(esp + 0x98) = edx;
    eax = MEM32(esi + 0x30);
    MEM32(esp + 0x30) = eax;
    ecx = MEM32(esi + 0x34);
    MEM32(esp + 0x34) = ecx;
    edx = MEM32(esi + 0x38);
    eax = esp + 0x30;
    PUSH32(esp, eax);
    ecx = esp + 0x64;
    PUSH32(esp, ecx);
    ecx = eax;
    MEM32(esp + 0x40) = edx;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068CEFu); sub_000127F0(); /* call 0x000127F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068CEF: ;
    PUSH32(esp, eax);
    PUSH32(esp, 0xBF800000u);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068CFCu); sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068CFC: ;
    edx = MEM32(esp + 0x30);
    eax = MEM32(esp + 0x34);
    ecx = MEM32(esp + 0x38);
    MEM32(esp + 0x90) = edx;
    edx = MEM32(esp + 0x1C);
    MEM32(esp + 0x94) = eax;
    eax = esp + 0x60;
    PUSH32(esp, edx);
    MEM32(esp + 0x9C) = ecx;
    PUSH32(esp, eax);
    ecx = eax;
    MEM32(esp + 0xA4) = 0x3F800000;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068D39u); sub_00011250(); /* call 0x00011250 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068D39: ;
    ecx = ebx + 4;
    PUSH32(esp, ecx);
    PUSH32(esp, ebx);
    esi = esp + edi + 0xC8;
    PUSH32(esp, esi);
    ecx = esp + 0x6C;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068D4Fu); sub_00011C20(); /* call 0x00011C20 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068D4F: ;
    fp_push(MEMF(0x176190)); /* fld float */
    edx = MEM32(esp + 0x20);
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(edx)); /* fdiv dword ptr [edx] */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0xC));
    esp = esp - 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0xD4620F2Du; /* flag setter tag */
    eax = esp + 0x6C;
    ecx = eax;
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esp + edi + 0xD0)); /* fdiv dword ptr [esp + edi + 0xd0] */
    MEMF(esp + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esi)); /* fdiv dword ptr [esi] */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, eax);
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068D8Au); sub_00011B60(); /* call 0x00011B60 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068D8A: ;
    edx = esp + 0x60;
    ecx = esp + edi + 0xA0;
    PUSH32(esp, edx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068D9Bu); sub_000120E0(); /* call 0x000120E0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068D9B: ;
    eax = MEM32(esp + 0x90);
    ecx = MEM32(esp + 0x94);
    edx = MEM32(esp + 0x98);
    MEM32(esp + edi + 0xB0) = eax;
    eax = MEM32(esp + 0x24);
    PUSH32(esp, eax);
    eax = esp + edi + 0xA4;
    MEM32(ebx + -16) = ecx;
    ecx = esp + edi + 0x584;
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    MEM32(ebx + -12) = edx;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068DD7u); sub_000123C0(); /* call 0x000123C0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068DD7: ;
    ecx = MEM32(esp + 0x24);
    PUSH32(esp, ecx);
    ecx = MEM32(esp + 0x1C);
    eax = esp + edi + 0xB4;
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068DEEu); sub_000129F0(); /* call 0x000129F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068DEE: ;
    edx = MEM32(esp + 0x24);
    ecx = MEM32(esp + 0x2C);
    PUSH32(esp, edx);
    PUSH32(esp, esi);
    PUSH32(esp, ecx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068DFEu); sub_000129F0(); /* call 0x000129F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068DFE: ;
    ecx = MEM32(esp + 0x5C);

loc_00068E02: ;
    edx = MEM32(esp + 0x20);
    esi = MEM32(esp + 0x2C);
    eax = 0x30;
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edx) + (uint64_t)(eax)) >> 32) & 1);
    edx = edx + eax;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x77A0E457u; /* flag setter tag */
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(eax)) >> 32) & 1);
    esi = esi + eax;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7C5F3607u; /* flag setter tag */
    MEM32(esp + 0x20) = edx;
    edx = MEM32(esp + 0x18);
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edx) + (uint64_t)(eax)) >> 32) & 1);
    edx = edx + eax;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x77A0E457u; /* flag setter tag */
    MEM32(esp + 0x2C) = esi;
    esi = MEM32(esp + 0x1C);
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(eax)) >> 32) & 1);
    edi = edi + eax;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x08121CEEu; /* flag setter tag */
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ebx) + (uint64_t)(eax)) >> 32) & 1);
    ebx = ebx + eax;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xB12F776Eu; /* flag setter tag */
    eax = MEM32(esp + 0x28);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x40)) >> 32) & 1);
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x42FAE895u; /* flag setter tag */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(4)) >> 32) & 1);
    ecx = ecx + 4;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xAD912013u; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x18) = edx;
    MEM32(esp + 0x1C) = esi;
    MEM32(esp + 0x5C) = ecx;
    MEM32(esp + 0x28) = eax;
    if ((_fa != 0)) goto loc_00068B92; /* jne: not equal / not zero */

loc_00068E4A: ;
    eax = MEM32(esp + 0x4C);

loc_00068E4E: ;
    esi = MEM32(ebp + 8);
    _fb = (uint32_t)(0xF80) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0xF80)) >> 32) & 1);
    esi = esi + 0xF80;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x9E40C8AAu; /* flag setter tag */
    ebx = eax + 0x1A0;
    edi = esp + 0x598;
    MEM32(esp + 0x28) = 0x1A;
    /* nop */

loc_00068E70: ;
    eax = edi + -24;
    PUSH32(esp, eax);
    ecx = esi;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068E7Bu); sub_00011710(); /* call 0x00011710 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068E7B: ;
    ecx = MEM32(edi + -8);
    edx = MEM32(edi + -4);
    eax = MEM32(edi);
    MEM32(esi + 0x30) = ecx;
    ecx = MEM32(edi + 0x10);
    PUSH32(esp, ecx);
    MEM32(esi + 0x34) = edx;
    edx = MEM32(edi + 0xC);
    MEM32(esi + 0x38) = eax;
    eax = MEM32(edi + 8);
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    PUSH32(esp, esi);
    ecx = esi;
    MEM32(esi + 0x3C) = 0x3F800000;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068EA7u); sub_00011B60(); /* call 0x00011B60 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068EA7: ;
    ecx = MEM32(ebx);
    eax = MEM32(ebp + 8);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(ecx) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    ecx = (uint32_t)(ecx) << ((6) & 31u);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x29DB4FF6u; /* flag setter tag */
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(eax)) >> 32) & 1);
    ecx = ecx + eax;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xCB7A4EFAu; /* flag setter tag */
    PUSH32(esp, esi);
    PUSH32(esp, ecx);
    ecx = esi;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00068EBAu); sub_00011250(); /* call 0x00011250 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00068EBA: ;
    eax = MEM32(esp + 0x28);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x40)) >> 32) & 1);
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x42FAE895u; /* flag setter tag */
    _fb = (uint32_t)(0x30) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(0x30)) >> 32) & 1);
    edi = edi + 0x30;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x3430E82Au; /* flag setter tag */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ebx) + (uint64_t)(4)) >> 32) & 1);
    ebx = ebx + 4;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x3B2DA6D2u; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x28) = eax;
    if ((_fa != 0)) goto loc_00068E70; /* jne: not equal / not zero */

loc_00068ECE: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 8; return; /* ret 4 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

void __wrap_sub_00040580(void) { sc2_fast_sub_00040580(); }

void sc2_fast_sub_000477A0(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* frameless: caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fk = 0; /* tag of the last flag setter */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)
    ebp = g_seh_ebp; /* fpo_leaf: inherit caller's frame */

loc_000477A0: ;
    _fb = (uint32_t)(0x2C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0x2C));
    esp = esp - 0x2C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0x4AA269B4u; /* flag setter tag */
    eax = MEM32(0x3706E4);
    PUSH32(esp, ebp);
    ebp = MEM32(esp + 0x34);
    PUSH32(esp, esi);
    MEM32(esp + 0x30) = eax;
    eax = MEM32(ebp + 0xC);
    PUSH32(esp, edi);
    PUSH32(esp, ebp);
    MEM32(esp + 0x14) = eax;
    esi = ebp + 0x3050;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000477C6u); sub_00047750(); /* call 0x00047750 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000477C6: ;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    _fa = (uint32_t)(MEM16(ebp + 0x58)) & 0xFFFFu; _fb = (uint32_t)(0x19) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(ebp + 0x58), 0x19 (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x20F13A87u; /* flag setter tag */
    MEM32(esp + 0xC) = eax;
    if (CMP_NE(_fa, _fb)) goto loc_000477E3; /* jne: not equal / not zero */

loc_000477D4: ;
    edi = eax + 0x5C0;
    ecx = 0x90;
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x504457D4u; /* flag setter tag */
    { uint32_t _i; int32_t _st = RECOMP_DF_STEP(4); for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*_st) = eax; edi += ecx * _st; }
    ecx = 0; /* rep stosd */

loc_000477E3: ;
    PUSH32(esp, ebx);
    ebx = ebp + 0x2870;
    ecx = ebx;
    edx = MEM32(ecx);
    eax = esi + 0x4C0;
    MEM32(eax) = edx;
    edx = MEM32(ecx + 4);
    MEM32(eax + 4) = edx;
    edx = MEM32(ecx + 8);
    MEM32(eax + 8) = edx;
    ecx = MEM32(ecx + 0xC);
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x3E0F5DA7u; /* flag setter tag */
    MEM32(eax + 0xC) = ecx;
    MEM32(esp + 0x40) = edi;
    edi = edi;

loc_00047810: ;
    PUSH32(esp, edi);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x3E0F5DA7u ? (0) : CMP_B(_fa, _fb))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00047817u); sub_00043120(); /* call 0x00043120 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00047817: ;
    edx = MEM32(esp + 0x48);
    _cf = 0; /* logical op clears CF */
    edx = edx | eax;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    _fk = 0x47242C55u; /* flag setter tag */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0xDF715D11u; /* flag setter tag */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(5) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 5 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x119926C0u; /* flag setter tag */
    MEM32(esp + 0x40) = edx;
    if (CMP_L(_fas, _fbs)) goto loc_00047810; /* jl: less (signed <) */

loc_0004782A: ;
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x3E0F5DA7u; /* flag setter tag */
    eax = edx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edi (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x7C546C9Cu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00047846; /* je: equal / zero */

loc_00047832: ;
    eax = 1;
    MEM32(esi + 0xB50) = eax;
    MEM16(ebp + 0x23A4) = LO16(eax);
    goto loc_00047853;

loc_00047846: ;
    MEM32(esi + 0xB50) = edi;
    MEM16(ebp + 0x23A4) = LO16(edi);

loc_00047853: ;
    PUSH32(esp, edi);
    edx = esp + 0x1C;
    PUSH32(esp, 0x17803C);
    PUSH32(esp, edx);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00047863u); sub_00145BE2(); /* call 0x00145BE2 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00047863: ;
    PUSH32(esp, edi);
    PUSH32(esp, ebp);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004786Au); sub_0003E970(); /* call 0x0003E970 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004786A: ;
    PUSH32(esp, edi);
    PUSH32(esp, ebp);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00047871u); sub_000432E0(); /* call 0x000432E0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00047871: ;
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x1C)) >> 32) & 1);
    esp = esp + 0x1C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xB4A3B468u; /* flag setter tag */
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0xDF715D11u; /* flag setter tag */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(5) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 5 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x119926C0u; /* flag setter tag */
    if (CMP_L(_fas, _fbs)) goto loc_00047853; /* jl: less (signed <) */

loc_0004787A: ;
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00047880u); sub_00044030(); /* call 0x00044030 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00047880: ;
    eax = MEM32(ebx);
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x3E0F5DA7u; /* flag setter tag */
    MEM32(ebp + 0x1FD4) = edi;
    MEM32(ebp + 0x1C54) = edi;
    MEM32(esi + 0x4C0) = eax;
    ecx = MEM32(ebx + 4);
    MEM32(esi + 0x4C4) = ecx;
    edx = MEM32(ebx + 8);
    MEM32(esi + 0x4C8) = edx;
    eax = MEM32(ebx + 0xC);
    MEM32(esi + 0x4CC) = eax;
    ecx = (uint32_t)(int32_t)SMEM16(ebp + 0x23A4);
    MEM32(esi + 0xB50) = ecx;
    edx = ZX8(MEM8(ebp + 0xE0));
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    _fa = (uint32_t)(MEM16(ebp + 0x58)) & 0xFFFFu; _fb = (uint32_t)(0x19) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(ebp + 0x58), 0x19 (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x20F13A87u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_000478D5; /* je: equal / zero */

loc_000478CF: ;
    eax = (uint32_t)(int32_t)SMEM16(ebp + 0x5A);
    goto loc_000478D9;

loc_000478D5: ;
    eax = (uint32_t)(int32_t)SMEM16(ebp + 0x5C);

loc_000478D9: ;
    _fb = (uint32_t)(0xFFFFFFFDu) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0xFFFFFFFDu)) >> 32) & 1);
    eax = eax + 0xFFFFFFFDu;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xDE8CE545u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0x19) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0x19 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x4DAC365Au; /* flag setter tag */
    ebx = 0xD;
    SET_LO8(ecx, 9);
    if (CMP_A(_fa, _fb)) goto loc_000479B7; /* ja: above (unsigned >) */

loc_000478EC: ;
    eax = ZX8(MEM8(eax + 0x47BEC));
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    { uint32_t _jt = MEM32(eax * 4 + 0x47BC8); /* switch: 9 entries, 9 targets */
    if (_jt == 0x000478FAu) goto loc_000478FA;
    if (_jt == 0x0004790Du) goto loc_0004790D;
    if (_jt == 0x0004791Fu) goto loc_0004791F;
    if (_jt == 0x0004795Fu) goto loc_0004795F;
    if (_jt == 0x0004796Du) goto loc_0004796D;
    if (_jt == 0x0004797Bu) goto loc_0004797B;
    if (_jt == 0x0004798Bu) goto loc_0004798B;
    if (_jt == 0x0004799Au) goto loc_0004799A;
    if (_jt == 0x000479B7u) goto loc_000479B7;
    g_seh_ebp = ebp; RECOMP_ITAIL(_jt); return; }

loc_000478FA: ;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fb = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edx, edi (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x3424B519u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_000479B7; /* jne: not equal / not zero */

loc_00047902: ;
    MEM8(esi + 0xB54) = LO8(ebx);
    goto loc_000479BD;

loc_0004790D: ;
    MEM8(esi + 0xB54) = LO8(ecx);
    MEM8(esi + 0xB55) = 3;
    goto loc_000479C4;

loc_0004791F: ;
    MEM8(esi + 0xB54) = LO8(ecx);
    MEM8(esi + 0xB55) = 0x17;
    MEM8(esi + 0xB56) = 0x18;
    MEM8(esi + 0xB57) = 0x19;
    MEM8(esi + 0xB58) = 0x1A;
    MEM8(esi + 0xB59) = 0x1B;
    MEM8(esi + 0xB5A) = 0x1C;
    MEM8(esi + 0xB5B) = 0x1D;
    MEM8(esi + 0xB5C) = 0x1E;
    goto loc_000479C4;

loc_0004795F: ;
    MEM8(esi + 0xB54) = LO8(ebx);
    MEM8(esi + 0xB55) = LO8(ecx);
    goto loc_000479C4;

loc_0004796D: ;
    MEM8(esi + 0xB54) = LO8(ecx);
    MEM8(esi + 0xB55) = LO8(ebx);
    goto loc_000479C4;

loc_0004797B: ;
    MEM8(esi + 0xB54) = 8;
    MEM8(esi + 0xB55) = 0xC;
    goto loc_000479C4;

loc_0004798B: ;
    MEM8(esi + 0xB54) = LO8(ecx);
    MEM8(esi + 0xB55) = 0x17;
    goto loc_000479C4;

loc_0004799A: ;
    SET_LO8(eax, 0xE);
    MEM8(esi + 0xB54) = LO8(ecx);
    MEM8(esi + 0xB55) = LO8(eax);
    MEM8(esi + 0xB56) = 3;
    MEM8(esi + 0xB57) = LO8(eax);
    goto loc_000479C4;

loc_000479B7: ;
    MEM8(esi + 0xB54) = LO8(ecx);

loc_000479BD: ;
    MEM8(esi + 0xB55) = 0xE;

loc_000479C4: ;
    eax = MEM32(0x398968);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebx (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x9A93F82Au; /* flag setter tag */
    if (CMP_L(_fas, _fbs)) goto loc_000479E6; /* jl: less (signed <) */

loc_000479CD: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xF) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xF (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x60DA7487u; /* flag setter tag */
    if (CMP_G(_fas, _fbs)) goto loc_000479E6; /* jg: greater (signed >) */

loc_000479D2: ;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fb = (uint32_t)(MEM32(0x398A04)) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebp, MEM32(0x398A04) (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x3D541DD1u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_000479E6; /* jne: not equal / not zero */

loc_000479DA: ;
    MEM8(esi + 0xB54) = LO8(ecx);
    MEM8(esi + 0xB55) = LO8(ebx);

loc_000479E6: ;
    ecx = ebp + 0x2920;
    eax = MEM32(ecx);
    edx = esi + 0x4D0;
    MEM32(edx) = eax;
    eax = MEM32(ecx + 4);
    MEM32(edx + 4) = eax;
    eax = MEM32(ecx + 8);
    MEM32(edx + 8) = eax;
    ecx = MEM32(ecx + 0xC);
    PUSH32(esp, ebp);
    MEM32(edx + 0xC) = ecx;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00047A0Eu); RECOMP_ABI_CALL(0x00044360u, sub_00044360); /* call 0x00044360 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00047A0E: ;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    _fa = (uint32_t)(MEM16(0x39894A)) & 0xFFFFu; _fb = (uint32_t)(LO16(edi)) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(0x39894A), LO16(edi) (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x255BDEF4u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00047AAC; /* je: equal / zero */

loc_00047A1E: ;
    eax = (uint32_t)(int32_t)SMEM8(ebp);
    edx = MEM32(0x6050E8);
    ecx = eax;
    _fa = ((int64_t)(int32_t)(eax) * (int64_t)(int32_t)(0x11C) != (int64_t)(int32_t)((uint32_t)(eax) * (uint32_t)(0x11C)));
    eax = (uint32_t)((int32_t)eax * (int32_t)0x11C);
    _fk = 0xFD4E643Fu; /* flag setter tag */
    if (((4) & 31u)) _cf = (int)(((uint32_t)(ecx) >> ((32 - (((4) & 31u))) & 31u)) & 1u);
    ecx = (uint32_t)(ecx) << ((4) & 31u);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0xC7C22D63u; /* flag setter tag */
    if (((LO8(ecx)) & 31u)) _cf = (int)(((uint32_t)(edx) >> (((((LO8(ecx)) & 31u)) - 1) & 31u)) & 1u);
    edx = (uint32_t)(edx) >> ((LO8(ecx)) & 31u);
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0xC69B1BFDu; /* flag setter tag */
    _fa = (uint32_t)(MEM32(eax + 0x60511C)) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* test MEM32(eax + 0x60511C), edx (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xF0E56AE8u; /* flag setter tag */
    if (TEST_Z(_fa, _fb)) goto loc_00047AAC; /* je: equal / zero */

loc_00047A3D: ;
    SET_LO8(eax, MEM8(0x1A07E0));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    MEM32(esp + 0x40) = 1;
    edi = 0x1A07E0;
    if (CMP_EQ(_fa, _fb)) goto loc_00047A89; /* je: equal / zero */

loc_00047A53: ;
    ecx = SX8(LO8(eax));
    eax = 1;
    _cf = 0; /* xor clears CF */
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x0296481Bu; /* flag setter tag */
    g_eflags = (g_eflags & ~1u) | ((0) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00047A62u); sub_00145D40(); /* call 0x00145D40 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00047A62: ;
    ecx = MEM32(esi + 0xE88);
    ebx = MEM32(esi + 0xE8C);
    eax = ~eax;
    _cf = 0; /* logical op clears CF */
    ecx = ecx & eax;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0x6B11DF41u; /* flag setter tag */
    edx = ~edx;
    _cf = 0; /* logical op clears CF */
    ebx = ebx & edx;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0x0BE6209Fu; /* flag setter tag */
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0xDF715D11u; /* flag setter tag */
    MEM32(esi + 0xE88) = ecx;
    MEM32(esi + 0xE8C) = ebx;
    SET_LO8(eax, MEM8(edi));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_00047A53; /* jne: not equal / not zero */

loc_00047A89: ;
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x00874B0Eu ? (CMP_B(_fa, _fb)) : (_fk == 0x255BDEF4u ? (CMP_B(_fa, _fb)) : CMP_B(_fa & _fb, 0)))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00047A8Fu); sub_00045170(); /* call 0x00045170 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00047A8F: ;
    SET_LO8(eax, MEM8(ebp + 0xA8));
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    ecx = esi;
    if (CMP_EQ(_fa, _fb)) goto loc_00047AB2; /* je: equal / zero */

loc_00047A9E: ;
    ebx = MEM32(esp + 0x10);
    PUSH32(esp, 0);
    PUSH32(esp, ebx);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00047AAAu); sub_0007F370(); /* call 0x0007F370 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00047AAA: ;
    goto loc_00047AC2;

loc_00047AAC: ;
    MEM32(esp + 0x40) = edi;
    goto loc_00047A89;

loc_00047AB2: ;
    eax = MEM32(esp + 0x10);
    PUSH32(esp, 1);
    PUSH32(esp, eax);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00047ABEu); sub_0007F370(); /* call 0x0007F370 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00047ABE: ;
    ebx = MEM32(esp + 0x10);

loc_00047AC2: ;
    ecx = MEM32(ebp + 0x2848);
    PUSH32(esp, ebp);
    MEM32(esp + 0x14) = ecx;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00047AD2u); sub_00043FF0(); /* call 0x00043FF0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00047AD2: ;
    fp_push(MEMF(ebp + 0x2848)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x14)); /* fsub dword ptr [esp + 0x14] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebp + 0x1668)); /* fadd dword ptr [ebp + 0x1668] */
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00047AEAu); sub_00033E40(); /* call 0x00033E40 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00047AEA: ;
    eax = MEM32(esp + 0x18);
    MEMF(ebp + 0x1668) = (float)fp_top(); fp_pop(); /* fstp */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00047B19; /* je: equal / zero */

loc_00047AFB: ;
    edx = ebx + 0x80;
    MEM32(eax + 0x5C) = edx;
    eax = MEM32(ebp + 0x10);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00047B19; /* je: equal / zero */

loc_00047B0B: ;
    ecx = MEM32(ebp + 0x27CC);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(ecx) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    ecx = (uint32_t)(ecx) << ((6) & 31u);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x29DB4FF6u; /* flag setter tag */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(ebx)) >> 32) & 1);
    ecx = ecx + ebx;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7434F103u; /* flag setter tag */
    MEM32(eax + 0x5C) = ecx;

loc_00047B19: ;
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x7434F103u ? (_cf) : CMP_B(_fa, _fb))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00047B1Fu); sub_000442C0(); /* call 0x000442C0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00047B1F: ;
    eax = MEM32(esp + 0x44);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00047B82; /* je: equal / zero */

loc_00047B2A: ;
    SET_LO8(eax, MEM8(0x1A07E0));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    edi = 0x1A07E0;
    if (CMP_EQ(_fa, _fb)) goto loc_00047B82; /* je: equal / zero */

loc_00047B38: ;
    ecx = SX8(LO8(eax));
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(ecx) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    ecx = (uint32_t)(ecx) << ((6) & 31u);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x29DB4FF6u; /* flag setter tag */
    _fb = (uint32_t)(ebx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(ebx)) >> 32) & 1);
    ecx = ecx + ebx;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7434F103u; /* flag setter tag */
    PUSH32(esp, 0);
    PUSH32(esp, ecx);
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00047B4Cu); sub_00011B60(); /* call 0x00011B60 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00047B4C: ;
    ecx = (uint32_t)(int32_t)SMEM8(edi);
    eax = 1;
    _cf = 0; /* xor clears CF */
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x0296481Bu; /* flag setter tag */
    g_eflags = (g_eflags & ~1u) | ((0) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00047B5Bu); sub_00145D40(); /* call 0x00145D40 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00047B5B: ;
    ecx = MEM32(esi + 0xE88);
    eax = ~eax;
    _cf = 0; /* logical op clears CF */
    ecx = ecx & eax;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0x6B11DF41u; /* flag setter tag */
    eax = MEM32(esi + 0xE8C);
    edx = ~edx;
    _cf = 0; /* logical op clears CF */
    eax = eax & edx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0x85196A23u; /* flag setter tag */
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0xDF715D11u; /* flag setter tag */
    MEM32(esi + 0xE88) = ecx;
    MEM32(esi + 0xE8C) = eax;
    SET_LO8(eax, MEM8(edi));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_00047B38; /* jne: not equal / not zero */

loc_00047B82: ;
    edx = (uint32_t)(int32_t)SMEM8(ebp);
    eax = MEM32(edx * 4 + 0x3981E0);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    POP32(esp, ebx);
    if (CMP_EQ(_fa, _fb)) goto loc_00047BB6; /* je: equal / zero */

loc_00047B92: ;
    fp_push(MEMF(ebp + 0x2A00)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebp + 0x2990)); /* fadd dword ptr [ebp + 0x2990] */
    MEMF(ebp + 0x2990) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ebp + 0x2A08)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(ebp + 0x2998)); /* fadd dword ptr [ebp + 0x2998] */
    MEMF(ebp + 0x2998) = (float)fp_top(); fp_pop(); /* fstp */

loc_00047BB6: ;
    ecx = MEM32(esp + 0x34);
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00047BC2u); sub_001456E4(); /* call 0x001456E4 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00047BC2: ;
    _fb = (uint32_t)(0x2C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x2C)) >> 32) & 1);
    esp = esp + 0x2C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xE78B746Fu; /* flag setter tag */
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

/**
 * sub_00047C10
 * Original: 0x00047C10 - 0x00047CFC (236 bytes, 76 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */

void sc2_fast_sub_000200B0(void)
{
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_000200B0: ;
    fp_push(MEMF(esp + 4)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 8)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 8] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000200C4; /* jp: parity */

loc_000200BF: ;
    fp_push(MEMF(esp + 8)); /* fld float */
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    esp += 4; return; /* ret */

loc_000200C4: ;
    fp_push(MEMF(esp + 4)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 0xC)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 0xc] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (TEST_NZ(_fa, _fb)) goto loc_000200D8; /* jne: not equal / not zero */

loc_000200D3: ;
    fp_push(MEMF(esp + 0xC)); /* fld float */
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    esp += 4; return; /* ret */

loc_000200D8: ;
    fp_push(MEMF(esp + 4)); /* fld float */
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

/**
 * sub_000200B4
 * Original: 0x000200B4 - 0x000200DD (41 bytes, 15 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */

void sc2_fast_sub_00023840(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* prologue saves caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */
    #define fp_push(v) do { double _fp_value = (v); \
        g_fp_top = (g_fp_top + 7u) & 7u; \
        g_fp_stack[g_fp_top] = _fp_value; } while (0)
    #define fp_pop() (g_fp_top = (g_fp_top + 1u) & 7u)
    #define fp_top() g_fp_stack[g_fp_top]
    #define fp_st(i) g_fp_stack[(g_fp_top + (i)) & 7u]
    #define fp_st1() fp_st(1)

loc_00023840: ;
    PUSH32(esp, ebp);
    ebp = esp;
    g_ebp = ebp; /* publish frame for frameless callees */
    g_seh_ebp = ebp;
    _cf = 0; /* logical op clears CF */
    esp = esp & 0xFFFFFFF0u;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fb = (uint32_t)(0x48) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0x48));
    esp = esp - 0x48;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, esi);
    esi = MEM32(ebp + 0xC);
    PUSH32(esp, edi);
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x10;
    edi = esp + 0x10;
    { uint32_t _i; int32_t _st = RECOMP_DF_STEP(4); for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*_st) = eax; edi += ecx * _st; }
    ecx = 0; /* rep stosd */
    eax = ZX16(MEM16(esi));
    edi = MEM32(ebp + 8);
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 4 (32-bit) */
    _cf = (int)(_fa < _fb);
    MEM32(esp + 0x4C) = 0x3F800000;
    if (CMP_A(_fa, _fb)) goto loc_000239A0; /* ja: above (unsigned >) */

loc_00023873: ;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    { uint32_t _jt = MEM32(eax * 4 + 0x239B4); /* switch: 5 entries, 5 targets */
    if (_jt == 0x0002387Au) goto loc_0002387A;
    if (_jt == 0x000238EFu) goto loc_000238EF;
    if (_jt == 0x00023925u) goto loc_00023925;
    if (_jt == 0x00023973u) goto loc_00023973;
    if (_jt == 0x00023992u) goto loc_00023992;
    g_seh_ebp = ebp; RECOMP_ITAIL(_jt); return; }

loc_0002387A: ;
    eax = MEM32(esi + 0xC);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    MEM32(esp + 0x48) = 0x3F000000;
    MEM32(esp + 0x44) = 0x3F000000;
    MEM32(esp + 0x40) = 0x3F000000;
    MEM32(esp + 0x38) = 0x3F000000;
    MEM32(esp + 0x24) = 0x3F000000;
    MEM32(esp + 0x10) = 0x3F000000;
    if (CMP_EQ(_fa, _fb)) goto loc_000238D0; /* je: equal / zero */

loc_000238B1: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x30000);
    PUSH32(esp, edi);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000238BCu); RECOMP_ICALL_SAFE(0x0014E9A0u, _icall_esp); /* manual call 0x0014E9A0 */
    }
    _cf = g_eflags & 1u; /* callee carry */

loc_000238BC: ;
    edx = esp + 0x10;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edx);
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(2)) >> 32) & 1);
    edi = edi + 2;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, edi);
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000238CAu); RECOMP_ICALL_SAFE(0x0014F890u, _icall_esp); /* manual call 0x0014F890 */
    }
    _cf = g_eflags & 1u; /* callee carry */

loc_000238CA: ;
    POP32(esp, edi);
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp);
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

loc_000238D0: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x10000);
    PUSH32(esp, edi);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000238DBu); RECOMP_ICALL_SAFE(0x0014E9A0u, _icall_esp); /* manual call 0x0014E9A0 */
    }
    _cf = g_eflags & 1u; /* callee carry */

loc_000238DB: ;
    edx = esp + 0x10;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edx);
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(2)) >> 32) & 1);
    edi = edi + 2;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, edi);
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000238E9u); RECOMP_ICALL_SAFE(0x0014F890u, _icall_esp); /* manual call 0x0014F890 */
    }
    _cf = g_eflags & 1u; /* callee carry */

loc_000238E9: ;
    POP32(esp, edi);
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp);
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

loc_000238EF: ;
    fp_push(MEMF(esi + 0x14)); /* fld float */
    edx = MEM32(esi + 4);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 4)); /* fmul dword ptr [esi + 4] */
    eax = MEM32(esi + 8);
    MEM32(esp + 0x10) = edx;
    edx = esp + 0x10;
    MEMF(esp + 0x30) = (float)fp_top(); fp_pop(); /* fstp */
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edx);
    fp_push(MEMF(esi + 0x18)); /* fld float */
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(2)) >> 32) & 1);
    edi = edi + 2;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 8)); /* fmul dword ptr [esi + 8] */
    PUSH32(esp, edi);
    MEM32(esp + 0x2C) = eax;
    MEMF(esp + 0x3C) = (float)fp_top(); fp_pop(); /* fstp */
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002391Fu); RECOMP_ICALL_SAFE(0x0014F890u, _icall_esp); /* manual call 0x0014F890 */
    }
    _cf = g_eflags & 1u; /* callee carry */

loc_0002391F: ;
    POP32(esp, edi);
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp);
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

loc_00023925: ;
    fp_push(MEMF(esi + 0x14)); /* fld float */
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002392Du); sub_00145A64(); /* call 0x00145A64 */
    _cf = g_eflags & 1u; /* callee carry */

loc_0002392D: ;
    edx = MEM32(esi + 8);
    ecx = MEM32(esi + 4);
    MEM32(esp + 0x24) = edx;
    MEM32(esp + 0x10) = ecx;
    ecx = MEM32(esi + 0xC);
    edx = ((int32_t)eax < 0) ? 0xFFFFFFFF : 0; /* cdq */
    { int64_t _dividend = ((int64_t)(int32_t)edx << 32) | eax;
      eax = (uint32_t)((int32_t)(_dividend / (int32_t)ecx));
      edx = (uint32_t)((int32_t)(_dividend % (int32_t)ecx)); }
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(2)) >> 32) & 1);
    edi = edi + 2;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    MEM32(esp + 8) = edx;
    fp_push((double)SMEM32(esp + 8)); /* fild */
    MEM32(esp + 0xC) = eax;
    edx = esp + 0x10;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edx);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 4)); /* fmul dword ptr [esi + 4] */
    PUSH32(esp, edi);
    MEMF(esp + 0x38) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push((double)SMEM32(esp + 0x14)); /* fild */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 8)); /* fmul dword ptr [esi + 8] */
    MEMF(esp + 0x3C) = (float)fp_top(); fp_pop(); /* fstp */
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002396Du); RECOMP_ICALL_SAFE(0x0014F890u, _icall_esp); /* manual call 0x0014F890 */
    }
    _cf = g_eflags & 1u; /* callee carry */

loc_0002396D: ;
    POP32(esp, edi);
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp);
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

loc_00023973: ;
    fp_push(MEMF(esi + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x176188)); /* fmul dword ptr [0x176188] */
    if (recomp_fp_trig_in_range(fp_top())) fp_top() = sin(fp_top()); /* fsin */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0xC)); /* fmul dword ptr [esi + 0xc] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 4)); /* fmul dword ptr [esi + 4] */
    MEMF(esp + 0x30) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esi + 0x18)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 8)); /* fmul dword ptr [esi + 8] */
    MEMF(esp + 0x34) = (float)fp_top(); fp_pop(); /* fstp */

loc_00023992: ;
    eax = MEM32(esi + 4);
    ecx = MEM32(esi + 8);
    MEM32(esp + 0x10) = eax;
    MEM32(esp + 0x24) = ecx;

loc_000239A0: ;
    edx = esp + 0x10;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edx);
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(2)) >> 32) & 1);
    edi = edi + 2;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, edi);
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000239AEu); RECOMP_ICALL_SAFE(0x0014F890u, _icall_esp); /* manual call 0x0014F890 */
    }
    _cf = g_eflags & 1u; /* callee carry */

loc_000239AE: ;
    POP32(esp, edi);
    POP32(esp, esi);
    esp = ebp;
    POP32(esp, ebp);
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

/**
 * sub_000239D0
 * Original: 0x000239D0 - 0x00023A5A (138 bytes, 44 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
