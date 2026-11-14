/**
 * LTO-enabled extraction of sub_0004B630 and sub_0004D090 from recomp_0001.c.
 * Allows cross-TU inlining of NEON math (switch_math_neon.c) and memory helpers.
 */
#define RECOMP_GENERATED_CODE 1
#include "recomp_funcs.h"
#include "sc2_fast_mem.h"
#include "x87_math.h"

void sc2_fast_sub_0004B630(void)
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

loc_0004B630: ;
    _fb = (uint32_t)(0xA4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0xA4));
    esp = esp - 0xA4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0x86EA9A7Eu; /* flag setter tag */
    eax = MEM32(0x3706E4);
    PUSH32(esp, ebx);
    MEM32(esp + 0xA4) = eax;
    eax = MEM32(0x38E588);
    ebx = MEM32(eax + 0x50);
    PUSH32(esp, ebp);
    ebp = MEM32(eax + 0x54);
    eax = MEM32(ebp + 0x74);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    PUSH32(esp, edi);
    edi = MEM32(ebp + 8);
    MEM32(esp + 0x48) = ebx;
    MEM32(esp + 0x2C) = edi;
    if (CMP_EQ(_fa, _fb)) goto loc_0004C7F0; /* je: equal / zero */

loc_0004B666: ;
    eax = MEM32(0x398B28);
    PUSH32(esp, esi);
    esi = 4;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, esi (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xA991CA4Du; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C7EF; /* je: equal / zero */

loc_0004B679: ;
    PUSH32(esp, 1);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004B681u); RECOMP_ABI_CALL(0x00041120u, sub_00041120); /* call 0x00041120 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004B681: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C7EF; /* je: equal / zero */

loc_0004B68C: ;
    eax = MEM32(ebx * 4 + 0x39813C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C7EF; /* je: equal / zero */

loc_0004B69B: ;
    PUSH32(esp, 0);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004B6A3u); RECOMP_ABI_CALL(0x00046E20u, sub_00046E20); /* call 0x00046E20 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004B6A3: ;
    fp_pop(); /* fstp st(0) */
    eax = MEM32(0x398968);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, esi (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xA991CA4Du; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C7EF; /* je: equal / zero */

loc_0004B6B5: ;
    _fa = (uint32_t)(MEM16(0x398950)) & 0xFFFFu; _fb = (uint32_t)(0) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(0x398950), 0 (16-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x79D6943Cu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C6F7; /* je: equal / zero */

loc_0004B6C3: ;
    SET_LO8(eax, MEM8(ebp + 4));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004C6F7; /* jne: not equal / not zero */

loc_0004B6CE: ;
    eax = MEM32(ebp + 0x368);
    ecx = MEM32(ebp + 8);
    edx = MEM32(ecx + 0x368);
    MEM32(esp + 0x3C) = eax;
    SET_LO8(eax, MEM8(ebp + 0xD3));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    MEM32(esp + 0x44) = edx;
    if (CMP_EQ(_fa, _fb)) goto loc_0004B6FA; /* je: equal / zero */

loc_0004B6EF: ;
    PUSH32(esp, 3);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0)) ? 0x80u : 0u) | (((((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004B6F7u); RECOMP_ABI_CALL(0x00045550u, sub_00045550); /* call 0x00045550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004B6F7: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */

loc_0004B6FA: ;
    SET_LO8(eax, MEM8(ebp + 0xC7));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004B718; /* je: equal / zero */

loc_0004B704: ;
    _fa = (uint32_t)(MEM16(ebp + 0x26C)) & 0xFFFFu; _fb = (uint32_t)(2) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(ebp + 0x26C), 2 (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xDB41995Fu; /* flag setter tag */
    if (CMP_B(_fa, _fb)) goto loc_0004B718; /* jb: below (unsigned <) */

loc_0004B70E: ;
    MEM32(ebp + 0x15F8) = 0xE;

loc_0004B718: ;
    eax = MEM32(ebp + 0x15F8);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004B79A; /* jne: not equal / not zero */

loc_0004B722: ;
    SET_LO8(eax, MEM8(ebp + 0xC9));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004B79A; /* jne: not equal / not zero */

loc_0004B72C: ;
    SET_LO8(eax, MEM8(ebp + 0xD2));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004B79A; /* je: equal / zero */

loc_0004B736: ;
    SET_LO8(eax, MEM8(ebp + 0xB4));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004B754; /* jne: not equal / not zero */

loc_0004B740: ;
    SET_LO8(eax, MEM8(ebp + 0xB2));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004B754; /* jne: not equal / not zero */

loc_0004B74A: ;
    SET_LO8(eax, MEM8(ebp + 0xA4));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004B79A; /* je: equal / zero */

loc_0004B754: ;
    SET_LO8(eax, MEM8(ebp + 0xAE));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004B766; /* je: equal / zero */

loc_0004B75E: ;
    fp_push(MEMF(ebp + 0x1FB0)); /* fld float */
    goto loc_0004B76C;

loc_0004B766: ;
    fp_push(MEMF(ebp + 0x1C30)); /* fld float */

loc_0004B76C: ;
    fp_push(MEMF(ebp + 0x2828)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_0004B798; /* jne: not equal / not zero */

loc_0004B77F: ;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(ebp + 0x2828)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [ebp + 0x2828] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x42694A75u; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_0004B79A; /* jne: not equal / not zero */

loc_0004B78C: ;
    MEM32(ebp + 0x15F8) = 0xD;
    goto loc_0004B79A;

loc_0004B798: ;
    fp_pop(); /* fstp st(0) */

loc_0004B79A: ;
    eax = MEM32(ebp + 0x15F8);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xB) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xB (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x6799B7ECu; /* flag setter tag */
    MEM32(ebp + 0x15FC) = eax;
    if (CMP_NE(_fa, _fb)) goto loc_0004B7D2; /* jne: not equal / not zero */

loc_0004B7AB: ;
    eax = MEM32(edi + 0x15F8);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004B7D2; /* je: equal / zero */

loc_0004B7B5: ;
    eax = MEM32(ebp + 0x1690);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(8)) >> 32) & 1);
    eax = eax + 8;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x525D6E6Du; /* flag setter tag */
    PUSH32(esp, eax);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004B7C5u); RECOMP_ABI_CALL(0x00042160u, sub_00042160); /* call 0x00042160 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004B7C5: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    MEM32(ebp + 0x15FC) = 0;

loc_0004B7D2: ;
    eax = MEM32(ebp + 0x15F8);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004B7FE; /* je: equal / zero */

loc_0004B7DC: ;
    eax = MEM32(edi + 0x15F8);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004B7FE; /* je: equal / zero */

loc_0004B7E6: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(8) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 8 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x1F215C34u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004B7FE; /* jne: not equal / not zero */

loc_0004B7EB: ;
    PUSH32(esp, ebp);
    MEM32(ebp + 0x15FC) = 0;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004B7FBu); RECOMP_ABI_CALL(0x00041CA0u, sub_00041CA0); /* call 0x00041CA0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004B7FB: ;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */

loc_0004B7FE: ;
    eax = MEM32(ebp + 0x15FC);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C6DB; /* je: equal / zero */

loc_0004B80C: ;
    esi = MEM32(ebp + 0x1684);
    _cf = 0; /* xor clears CF */
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x61F5A291u; /* flag setter tag */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, ebx (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xD91CF3FFu; /* flag setter tag */
    MEM32(esp + 0x10) = esi;
    MEM32(esp + 0x48) = ebx;
    if (CMP_NE(_fa, _fb)) goto loc_0004B840; /* jne: not equal / not zero */

loc_0004B820: ;
    esi = esp + 0x50;
    MEM32(esp + 0x10) = esi;
    MEM16(esp + 0x9A) = LO16(ebx);
    MEM16(esp + 0x98) = LO16(ebx);
    MEM16(esp + 0x8A) = LO16(ebx);

loc_0004B840: ;
    SET_LO8(eax, MEM8(ebp + 0xB4));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004B858; /* je: equal / zero */

loc_0004B84A: ;
    PUSH32(esp, 4);
    PUSH32(esp, ebx);
    PUSH32(esp, 0x1C);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0)) ? 0x80u : 0u) | (((((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004B855u); RECOMP_ABI_CALL(0x0003D9A0u, sub_0003D9A0); /* call 0x0003D9A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004B855: ;
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x10)) >> 32) & 1);
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7E08D19Du; /* flag setter tag */

loc_0004B858: ;
    ecx = MEM32(ebp + 0x15FC);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0xE) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0xE (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xF8F5DDECu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004B889; /* je: equal / zero */

loc_0004B863: ;
    edx = (uint32_t)(int32_t)SMEM16(esi + 0x3A);
    MEM32(esp + 0x24) = edx;
    MEM32(ebp + 0x1580) = ebx;
    MEM32(ebp + 0x157C) = ebx;
    fp_push((double)SMEM32(esp + 0x24)); /* fild */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 0x1560)); /* fmul dword ptr [ebp + 0x1560] */
    MEMF(ebp + 0x1578) = (float)fp_top(); /* fst */
    goto loc_0004B8A3;

loc_0004B889: ;
    fp_push(MEMF(ebp + 0x1578)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x178174)); /* fmul dword ptr [0x178174] */
    fp_push(MEMF(ebp + 0x1580)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 0x1578)); /* fmul dword ptr [ebp + 0x1578] */
    fp_st1() = RECOMP_FP_PC(fp_st1() - fp_top()); fp_pop(); /* fsubp st(1) */

loc_0004B8A3: ;
    eax = MEM32(ebp + 0x15B0);
    MEMF(ebp + 0x1570) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(ebp + 0x15B8);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edx (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x3F7F8199u; /* flag setter tag */
    MEM32(ebp + 0x15B4) = eax;
    if (CMP_GE(_fas, _fbs)) goto loc_0004B8C1; /* jge: greater or equal (signed >=) */

loc_0004B8BF: ;
    eax = edx;

loc_0004B8C1: ;
    MEM32(ebp + 0x15B8) = eax;
    eax = ecx + -1;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xE) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xE (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xFA6E46EDu; /* flag setter tag */
    ebx = 0x13;
    if (CMP_A(_fa, _fb)) goto loc_0004B901; /* ja: above (unsigned >) */

loc_0004B8D4: ;
    eax = ZX8(MEM8(eax + 0x4C824));
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    { uint32_t _jt = MEM32(eax * 4 + 0x4C808); /* switch: 7 entries, 7 targets */
    if (_jt == 0x0004B8E2u) goto loc_0004B8E2;
    if (_jt == 0x0004B901u) goto loc_0004B901;
    if (_jt == 0x0004B92Bu) goto loc_0004B92B;
    if (_jt == 0x0004BB11u) goto loc_0004BB11;
    if (_jt == 0x0004BB69u) goto loc_0004BB69;
    if (_jt == 0x0004BB84u) goto loc_0004BB84;
    if (_jt == 0x0004BBEAu) goto loc_0004BBEA;
    g_seh_ebp = ebp; RECOMP_ITAIL(_jt); return; }

loc_0004B8E2: ;
    edx = MEM32(ebp + 0x15A8);
    edx++;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x9C5AB014u; /* flag setter tag */
    eax = edx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xA) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xA (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xFD2D8586u; /* flag setter tag */
    MEM32(ebp + 0x15A8) = edx;
    if (CMP_L(_fas, _fbs)) goto loc_0004B8FB; /* jl: less (signed <) */

loc_0004B8F6: ;
    eax = 0xA;

loc_0004B8FB: ;
    MEM32(ebp + 0x15A8) = eax;

loc_0004B901: ;
    ecx = MEM32(ebp + 0x15A4);
    PUSH32(esp, 4);
    PUSH32(esp, 1);
    ecx++;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x53735CF0u; /* flag setter tag */
    PUSH32(esp, 0x22);
    PUSH32(esp, ebp);
    MEM32(ebp + 0x15A4) = ecx;
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((_fb != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004B91Au); RECOMP_ABI_CALL(0x0003D9A0u, sub_0003D9A0); /* call 0x0003D9A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004B91A: ;
    PUSH32(esp, 1);
    PUSH32(esp, edi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004B922u); RECOMP_ABI_CALL(0x00046EA0u, sub_00046EA0); /* call 0x00046EA0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004B922: ;
    PUSH32(esp, edi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004B928u); RECOMP_ABI_CALL(0x00053650u, sub_00053650); /* call 0x00053650 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004B928: ;
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x1C)) >> 32) & 1);
    esp = esp + 0x1C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xB4A3B468u; /* flag setter tag */

loc_0004B92B: ;
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0xD9300E98u ? (_cf) : (_fk == 0xBA2432BEu ? (_cf) : (_fk == 0x6565E3FAu ? (CMP_B(_fa, _fb)) : (_fk == 0xB4A3B468u ? (_cf) : (_fk == 0xC77E4F9Du ? (CMP_B(_fa & _fb, 0)) : (_fk == 0xA97432F7u ? (CMP_B(_fa, _fb)) : (_fk == 0xFA6E46EDu ? (CMP_B(_fa, _fb)) : _cf)))))))) ? 0x1u : 0u) | (((_fk == 0xD9300E98u ? (RECOMP_PARITY8(_fa)) : (_fk == 0xBA2432BEu ? (RECOMP_PARITY8(_fa)) : (_fk == 0x6565E3FAu ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0xB4A3B468u ? (RECOMP_PARITY8(_fa)) : (_fk == 0xC77E4F9Du ? (RECOMP_PARITY8((_fa) & (_fb))) : (_fk == 0xA97432F7u ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0xFA6E46EDu ? (RECOMP_PARITY8((_fa) - (_fb))) : RECOMP_PARITY8(_fa))))))))) ? 0x4u : 0u) | (((_fk == 0xD9300E98u ? ((_fa == 0)) : (_fk == 0xBA2432BEu ? ((_fa == 0)) : (_fk == 0x6565E3FAu ? (CMP_EQ(_fa, _fb)) : (_fk == 0xB4A3B468u ? ((_fa == 0)) : (_fk == 0xC77E4F9Du ? (TEST_Z(_fa, _fb)) : (_fk == 0xA97432F7u ? (CMP_EQ(_fa, _fb)) : (_fk == 0xFA6E46EDu ? (CMP_EQ(_fa, _fb)) : (_fa == 0))))))))) ? 0x40u : 0u) | (((_fk == 0xD9300E98u ? ((_fas < 0)) : (_fk == 0xBA2432BEu ? ((_fas < 0)) : (_fk == 0x6565E3FAu ? ((((uint16_t)((uint16_t)(_fas) - (uint16_t)(_fbs)) >> 15) != 0)) : (_fk == 0xB4A3B468u ? ((_fas < 0)) : (_fk == 0xC77E4F9Du ? (TEST_S(_fas, _fbs)) : (_fk == 0xA97432F7u ? ((((uint16_t)((uint16_t)(_fas) - (uint16_t)(_fbs)) >> 15) != 0)) : (_fk == 0xFA6E46EDu ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fas < 0))))))))) ? 0x80u : 0u) | (((_fk == 0xD9300E98u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (_fk == 0xBA2432BEu ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (_fk == 0x6565E3FAu ? ((((uint16_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 15) != 0)) : (_fk == 0xB4A3B468u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (_fk == 0xC77E4F9Du ? (0) : (_fk == 0xA97432F7u ? ((((uint16_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 15) != 0)) : (_fk == 0xFA6E46EDu ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0))))))))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004B931u); RECOMP_ABI_CALL(0x00042090u, sub_00042090); /* call 0x00042090 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004B931: ;
    MEMF(esp + 0x28) = (float)fp_top(); /* fst */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    MEMF(ebp + 0x15BC) = (float)fp_top(); fp_pop(); /* fstp */
    ecx = ebp;
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004B945u); RECOMP_ABI_CALL(0x00046FF0u, sub_00046FF0); /* call 0x00046FF0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004B945: ;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x24)); /* fmul dword ptr [esp + 0x24] */
    MEMF(esp + 0x24) = (float)fp_top(); /* fst */
    MEMF(ebp + 0x15BC) = (float)fp_top(); fp_pop(); /* fstp */
    _fa = (uint32_t)(MEM16(0x39894A)) & 0xFFFFu; _fb = (uint32_t)(0) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(0x39894A), 0 (16-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x6B431BAFu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004B970; /* je: equal / zero */

loc_0004B95D: ;
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint16_t)((uint16_t)(_fas) - (uint16_t)(_fbs)) >> 15) != 0)) ? 0x80u : 0u) | (((((uint16_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 15) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004B963u); RECOMP_ABI_CALL(0x00040DD0u, sub_00040DD0); /* call 0x00040DD0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004B963: ;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x28)); /* fmul dword ptr [esp + 0x28] */
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    MEMF(ebp + 0x15BC) = (float)fp_top(); fp_pop(); /* fstp */

loc_0004B970: ;
    _fa = (uint32_t)(MEM16(ebp + 0x58)) & 0xFFFFu; _fb = (uint32_t)(LO16(ebx)) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(ebp + 0x58), LO16(ebx) (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x847F3E77u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004B988; /* jne: not equal / not zero */

loc_0004B976: ;
    fp_push(MEMF(0x398A28)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 0x15BC)); /* fmul dword ptr [ebp + 0x15bc] */
    MEMF(ebp + 0x15BC) = (float)fp_top(); fp_pop(); /* fstp */

loc_0004B988: ;
    _fa = (uint32_t)(MEM16(edi + 0x58)) & 0xFFFFu; _fb = (uint32_t)(LO16(ebx)) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(edi + 0x58), LO16(ebx) (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x83FE98DAu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004B9D9; /* jne: not equal / not zero */

loc_0004B98E: ;
    fp_push(MEMF(0x398A24)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0004B9CD; /* jp: parity */

loc_0004B9A1: ;
    edx = (uint32_t)(int32_t)SMEM16(0x398A2E);
    fp_pop(); /* fstp st(0) */
    eax = (uint32_t)(int32_t)SMEM16(0x398A30);
    _fb = (uint32_t)(edx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(eax) < (uint32_t)(edx));
    eax = eax - edx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0x1970D594u; /* flag setter tag */
    MEM32(esp + 0x24) = eax;
    fp_push((double)SMEM32(esp + 0x24)); /* fild */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x176C04)); /* fmul dword ptr [0x176c04] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x176190)); /* fadd dword ptr [0x176190] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x176BEC)); /* fmul dword ptr [0x176bec] */

loc_0004B9CD: ;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 0x15BC)); /* fmul dword ptr [ebp + 0x15bc] */
    MEMF(ebp + 0x15BC) = (float)fp_top(); fp_pop(); /* fstp */

loc_0004B9D9: ;
    ecx = MEM32(esp + 0x44);
    fp_push(MEMF(ecx + 8)); /* fld float */
    edx = MEM32(esp + 0x3C);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 0x15BC)); /* fmul dword ptr [ebp + 0x15bc] */
    MEMF(ebp + 0x15BC) = (float)fp_top(); /* fst */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edx + 0xC)); /* fmul dword ptr [edx + 0xc] */
    MEMF(ebp + 0x15BC) = (float)fp_top(); /* fst */
    _fa = (uint32_t)(MEM16(0x39895E)) & 0xFFFFu; _fb = (uint32_t)(7) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(0x39895E), 7 (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x9DEC4874u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004BC4A; /* je: equal / zero */

loc_0004BA07: ;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 0x1570)); /* fmul dword ptr [ebp + 0x1570] */
    fp_top() = RECOMP_FP_PC(MEMF(ebp + 0x155C) - fp_top()); /* fsubr dword ptr [ebp + 0x155c] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(ebp + 0x1564)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [ebp + 0x1564] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0004BC4C; /* jp: parity */

loc_0004BA24: ;
    fp_push(MEMF(ebp + 0x1570)); /* fld float */
    MEM32(esp + 0x28) = 1;
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x17628C)); /* fadd dword ptr [0x17628c] */
    MEMF(ebp + 0x1570) = (float)fp_top(); fp_pop(); /* fstp */

loc_0004BA3E: ;
    eax = MEM32(edi + 0x1698);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004BA69; /* je: equal / zero */

loc_0004BA48: ;
    _fa = (uint32_t)(MEM32(ebp + 0x15FC)) & 0xFFFFFFFFu; _fb = (uint32_t)(8) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ebp + 0x15FC), 8 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x58C764D5u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004BA69; /* je: equal / zero */

loc_0004BA51: ;
    eax = MEM32(eax + 8);
    PUSH32(esp, 4);
    PUSH32(esp, 0);
    PUSH32(esp, 0x21);
    PUSH32(esp, ebp);
    MEM32(edi + 0x16D4) = eax;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BA66u); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BA66: ;
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x10)) >> 32) & 1);
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7E08D19Du; /* flag setter tag */

loc_0004BA69: ;
    edx = MEM32(ebp + 0x15B4);
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x504457D4u; /* flag setter tag */
    SET_LO16(eax, MEM16(esi + 0x48));
    _cf = 0; /* logical op clears CF */
    ebx = ebx | 0xFFFFFFFFu;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    _fk = 0xFD9388F9u; /* flag setter tag */
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x3E0F5DA7u; /* flag setter tag */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edx, 4 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xB7388E8Fu; /* flag setter tag */
    MEM32(esp + 0x18) = ebx;
    MEM32(esp + 0x20) = 1;
    MEM32(esp + 0x14) = edi;
    MEM32(esp + 0x34) = 0x3F800000;
    MEM32(esp + 0x24) = 0x3F800000;
    MEM32(esp + 0x38) = ebx;
    MEM32(esp + 0x1C) = ebx;
    ecx = eax;
    if (CMP_L(_fas, _fbs)) goto loc_0004BAAD; /* jl: less (signed <) */

loc_0004BAA9: ;
    SET_LO16(ecx, MEM16(esi + 0x4A));

loc_0004BAAD: ;
    esi = MEM32(ebp + 0x1708);
    edx = SX16(LO16(eax));
    _fa = ((int64_t)(int32_t)(edx) * (int64_t)(int32_t)(0x34) != (int64_t)(int32_t)((uint32_t)(edx) * (uint32_t)(0x34)));
    edx = (uint32_t)((int32_t)edx * (int32_t)0x34);
    _fk = 0xE8783E77u; /* flag setter tag */
    _fb = (uint32_t)(esi) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edx) + (uint64_t)(esi)) >> 32) & 1);
    edx = edx + esi;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xFBEC69C9u; /* flag setter tag */
    esi = SX16(LO16(ecx));
    ecx = MEM32(ebp + 0x1708);
    _fa = ((int64_t)(int32_t)(esi) * (int64_t)(int32_t)(0x34) != (int64_t)(int32_t)((uint32_t)(esi) * (uint32_t)(0x34)));
    esi = (uint32_t)((int32_t)esi * (int32_t)0x34);
    _fk = 0x18496712u; /* flag setter tag */
    PUSH32(esp, ebp);
    _fb = (uint32_t)(ecx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(ecx)) >> 32) & 1);
    esi = esi + ecx;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xA9D41CA9u; /* flag setter tag */
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BACFu); RECOMP_ABI_CALL(0x00041FB0u, sub_00041FB0); /* call 0x00041FB0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BACF: ;
    PUSH32(esp, ebp);
    MEM32(esp + 0x48) = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BAD9u); RECOMP_ABI_CALL(0x00041FB0u, sub_00041FB0); /* call 0x00041FB0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BAD9: ;
    MEM32(esp + 0x34) = eax;
    eax = MEM32(ebp + 0x16AC);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xFFFFFFFFu) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xFFFFFFFFu (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xB6C1447Bu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004BAFA; /* je: equal / zero */

loc_0004BAEB: ;
    ecx = MEM32(ebp + 0x1708);
    _fa = ((int64_t)(int32_t)(eax) * (int64_t)(int32_t)(0x34) != (int64_t)(int32_t)((uint32_t)(eax) * (uint32_t)(0x34)));
    eax = (uint32_t)((int32_t)eax * (int32_t)0x34);
    _fk = 0xD17B7E01u; /* flag setter tag */
    _fb = (uint32_t)(ecx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(ecx)) >> 32) & 1);
    eax = eax + ecx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xEA5B177Cu; /* flag setter tag */
    esi = eax;
    edx = esi;

loc_0004BAFA: ;
    eax = MEM32(ebp + 0x15FC);
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xE) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xE (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xFA6E46EDu; /* flag setter tag */
    if (CMP_A(_fa, _fb)) goto loc_0004C1B2; /* ja: above (unsigned >) */

loc_0004BB0A: ;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    { uint32_t _jt = MEM32(eax * 4 + 0x4C834); /* switch: 29 entries, 24 targets */
    if (_jt == 0x0004BC59u) goto loc_0004BC59;
    if (_jt == 0x0004BC81u) goto loc_0004BC81;
    if (_jt == 0x0004BD63u) goto loc_0004BD63;
    if (_jt == 0x0004BDBCu) goto loc_0004BDBC;
    if (_jt == 0x0004BDDBu) goto loc_0004BDDB;
    if (_jt == 0x0004BE36u) goto loc_0004BE36;
    if (_jt == 0x0004BE46u) goto loc_0004BE46;
    if (_jt == 0x0004BE56u) goto loc_0004BE56;
    if (_jt == 0x0004BE69u) goto loc_0004BE69;
    if (_jt == 0x0004BF1Du) goto loc_0004BF1D;
    if (_jt == 0x0004C057u) goto loc_0004C057;
    if (_jt == 0x0004C0A3u) goto loc_0004C0A3;
    if (_jt == 0x0004C0D5u) goto loc_0004C0D5;
    if (_jt == 0x0004C109u) goto loc_0004C109;
    if (_jt == 0x0004C166u) goto loc_0004C166;
    if (_jt == 0x0004C1B2u) goto loc_0004C1B2;
    if (_jt == 0x0004C5AAu) goto loc_0004C5AA;
    if (_jt == 0x0004C5E3u) goto loc_0004C5E3;
    if (_jt == 0x0004C5EDu) goto loc_0004C5ED;
    if (_jt == 0x0004C607u) goto loc_0004C607;
    if (_jt == 0x0004C620u) goto loc_0004C620;
    if (_jt == 0x0004C649u) goto loc_0004C649;
    if (_jt == 0x0004C65Au) goto loc_0004C65A;
    if (_jt == 0x0004C6CDu) goto loc_0004C6CD;
    g_seh_ebp = ebp; RECOMP_ITAIL(_jt); return; }

loc_0004BB11: ;
    PUSH32(esp, 4);
    PUSH32(esp, 1);
    PUSH32(esp, 0x39);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BB1Du); RECOMP_ABI_CALL(0x0003D9A0u, sub_0003D9A0); /* call 0x0003D9A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BB1D: ;
    PUSH32(esp, 1);
    PUSH32(esp, edi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BB25u); RECOMP_ABI_CALL(0x00046EA0u, sub_00046EA0); /* call 0x00046EA0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BB25: ;
    PUSH32(esp, edi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BB2Bu); RECOMP_ABI_CALL(0x00053680u, sub_00053680); /* call 0x00053680 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BB2B: ;
    ecx = (uint32_t)(int32_t)SMEM8(ebp);
    _fa = ((int64_t)(int32_t)(ecx) * (int64_t)(int32_t)(0x2520) != (int64_t)(int32_t)((uint32_t)(ecx) * (uint32_t)(0x2520)));
    ecx = (uint32_t)((int32_t)ecx * (int32_t)0x2520);
    _fk = 0xFEB8DE31u; /* flag setter tag */
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x1C)) >> 32) & 1);
    esp = esp + 0x1C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xB4A3B468u; /* flag setter tag */
    PUSH32(esp, ebp);
    _fb = (uint32_t)(0x3D40C8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0x3D40C8)) >> 32) & 1);
    ecx = ecx + 0x3D40C8;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xBFD57952u; /* flag setter tag */
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BB44u); RECOMP_ABI_CALL(0x000ADA70u, sub_000ADA70); /* call 0x000ADA70 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BB44: ;
    _fa = (uint32_t)(MEM16(edi + 0x16BA)) & 0xFFFFu; _fb = (uint32_t)(8) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(edi + 0x16BA), 8 (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x6565E3FAu; /* flag setter tag */
    if (CMP_GE(_fas, _fbs)) goto loc_0004BB5C; /* jge: greater or equal (signed >=) */

loc_0004BB4E: ;
    _fa = (uint32_t)(MEM16(edi + 0x16BC)) & 0xFFFFu; _fb = (uint32_t)(8) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(edi + 0x16BC), 8 (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xA97432F7u; /* flag setter tag */
    if (CMP_L(_fas, _fbs)) goto loc_0004B92B; /* jl: less (signed <) */

loc_0004BB5C: ;
    MEM32(esp + 0x48) = 1;
    goto loc_0004B92B;

loc_0004BB69: ;
    PUSH32(esp, 4);
    PUSH32(esp, ebx);
    PUSH32(esp, 9);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BB74u); RECOMP_ABI_CALL(0x0003D9F0u, sub_0003D9F0); /* call 0x0003D9F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BB74: ;
    PUSH32(esp, 1);
    PUSH32(esp, edi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BB7Cu); RECOMP_ABI_CALL(0x00046EA0u, sub_00046EA0); /* call 0x00046EA0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BB7C: ;
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x18)) >> 32) & 1);
    esp = esp + 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xBD67F1D5u; /* flag setter tag */
    goto loc_0004B92B;

loc_0004BB84: ;
    PUSH32(esp, 4);
    PUSH32(esp, 0x14);
    PUSH32(esp, 9);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BB90u); RECOMP_ABI_CALL(0x0003D9F0u, sub_0003D9F0); /* call 0x0003D9F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BB90: ;
    ecx = MEM32(0x38E520);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x10)) >> 32) & 1);
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7E08D19Du; /* flag setter tag */
    PUSH32(esp, 1);
    _fb = (uint32_t)(0x6C0) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0x6C0)) >> 32) & 1);
    ecx = ecx + 0x6C0;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xEC2CD2CBu; /* flag setter tag */
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BBA6u); RECOMP_ABI_CALL(0x0001F600u, sub_0001F600); /* call 0x0001F600 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BBA6: ;
    fp_push(MEMF(0x1761A0)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), fp_st1()); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); fp_pop(); /* fucompp  */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xC77E4F9Du; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0004BBDC; /* jp: parity */

loc_0004BBB5: ;
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) & (_fb))) ? 0x4u : 0u) | ((TEST_Z(_fa, _fb)) ? 0x40u : 0u) | ((TEST_S(_fas, _fbs)) ? 0x80u : 0u) | ((0) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BBBBu); RECOMP_ABI_CALL(0x000A29E0u, sub_000A29E0); /* call 0x000A29E0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BBBB: ;
    PUSH32(esp, 0x3E4CCCCD);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BBC5u); RECOMP_ABI_CALL(0x000A2A20u, sub_000A2A20); /* call 0x000A2A20 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BBC5: ;
    PUSH32(esp, 0x41800000);
    PUSH32(esp, 0x41400000);
    PUSH32(esp, 0x41600000);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BBD9u); RECOMP_ABI_CALL(0x000A28A0u, sub_000A28A0); /* call 0x000A28A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BBD9: ;
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x14)) >> 32) & 1);
    esp = esp + 0x14;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xBA2432BEu; /* flag setter tag */

loc_0004BBDC: ;
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0xBA2432BEu ? (_cf) : CMP_B(_fa & _fb, 0))) ? 0x1u : 0u) | (((_fk == 0xBA2432BEu ? (RECOMP_PARITY8(_fa)) : RECOMP_PARITY8((_fa) & (_fb)))) ? 0x4u : 0u) | (((_fk == 0xBA2432BEu ? ((_fa == 0)) : TEST_Z(_fa, _fb))) ? 0x40u : 0u) | (((_fk == 0xBA2432BEu ? ((_fas < 0)) : TEST_S(_fas, _fbs))) ? 0x80u : 0u) | (((_fk == 0xBA2432BEu ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BBE2u); RECOMP_ABI_CALL(0x00053700u, sub_00053700); /* call 0x00053700 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BBE2: ;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    goto loc_0004B92B;

loc_0004BBEA: ;
    PUSH32(esp, 1);
    PUSH32(esp, edi);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BBF2u); RECOMP_ABI_CALL(0x00046EA0u, sub_00046EA0); /* call 0x00046EA0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BBF2: ;
    ecx = MEM32(0x38E520);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    PUSH32(esp, 1);
    _fb = (uint32_t)(0x6C0) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0x6C0)) >> 32) & 1);
    ecx = ecx + 0x6C0;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xEC2CD2CBu; /* flag setter tag */
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BC08u); RECOMP_ABI_CALL(0x0001F600u, sub_0001F600); /* call 0x0001F600 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BC08: ;
    fp_push(MEMF(0x1761A0)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), fp_st1()); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); fp_pop(); /* fucompp  */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xC77E4F9Du; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0004B92B; /* jp: parity */

loc_0004BC1B: ;
    ecx = MEM32(ebp + 8);
    PUSH32(esp, ecx);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) & (_fb))) ? 0x4u : 0u) | ((TEST_Z(_fa, _fb)) ? 0x40u : 0u) | ((TEST_S(_fas, _fbs)) ? 0x80u : 0u) | ((0) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BC24u); RECOMP_ABI_CALL(0x000A29E0u, sub_000A29E0); /* call 0x000A29E0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BC24: ;
    PUSH32(esp, 0x3DCCCCCD);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BC2Eu); RECOMP_ABI_CALL(0x000A2A20u, sub_000A2A20); /* call 0x000A2A20 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BC2E: ;
    PUSH32(esp, 0x41A00000);
    PUSH32(esp, 0x41A00000);
    PUSH32(esp, 0x41400000);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BC42u); RECOMP_ABI_CALL(0x000A28A0u, sub_000A28A0); /* call 0x000A28A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BC42: ;
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x14)) >> 32) & 1);
    esp = esp + 0x14;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xBA2432BEu; /* flag setter tag */
    goto loc_0004B92B;

loc_0004BC4A: ;
    fp_pop(); /* fstp st(0) */

loc_0004BC4C: ;
    MEM32(esp + 0x28) = 0;
    goto loc_0004BA3E;

loc_0004BC59: ;
    ecx = (uint32_t)(int32_t)SMEM16(ebp + 0x1CAC);
    ebx = (uint32_t)(int32_t)SMEM16(ebp + 0x202C);
    _cf = 0; /* logical op clears CF */
    eax = eax | 0xFFFFFFFFu;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    _fk = 0xC795549Du; /* flag setter tag */
    MEM32(esp + 0x18) = ecx;
    MEM16(ebp + 0x202C) = LO16(eax);
    MEM16(ebp + 0x1CAC) = LO16(eax);
    goto loc_0004C1B2;

loc_0004BC81: ;
    eax = MEM32(esp + 0x40);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004BC95; /* jne: not equal / not zero */

loc_0004BC89: ;
    MEM32(esp + 0x18) = 0;
    PUSH32(esp, 4);
    goto loc_0004BC9F;

loc_0004BC95: ;
    MEM32(esp + 0x18) = 1;
    PUSH32(esp, 5);

loc_0004BC9F: ;
    PUSH32(esp, edx);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BCA6u); RECOMP_ABI_CALL(0x000423B0u, sub_000423B0); /* call 0x000423B0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BCA6: ;
    edx = MEM32(esp + 0x48);
    fp_push(MEMF(edx + 0x1C)); /* fld float */
    ebx = ZX16(LO16(eax));
    eax = MEM32(esp + 0x50);
    MEMF(ebp + 0x15BC) = (float)fp_top(); /* fst */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */
    PUSH32(esp, ecx);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x18)); /* fadd dword ptr [eax + 0x18] */
    MEM32(esp + 0x3C) = 0xFFFFFFFEu;
    edi = 5;
    MEM32(esp + 0x18) = 0x3F800000;
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, 0x3F800000);
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BCE3u); RECOMP_ABI_CALL(0x000EC580u, sub_000EC580); /* call 0x000EC580 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BCE3: ;
    ecx = MEM32(esp + 0x18);
    eax = MEM32(esp + 0x38);
    MEMF(ebp + 0x15BC) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ebp + 0x15BC)); /* fld float */
    edx = (uint32_t)(int32_t)SMEM16(ecx + 0x40);
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 0x1570)); /* fmul dword ptr [ebp + 0x1570] */
    SET_LO16(ecx, MEM16(eax + 0x16BA));
    MEM32(esp + 0x24) = edx;
    fp_top() = RECOMP_FP_PC(MEMF(ebp + 0x155C) - fp_top()); /* fsubr dword ptr [ebp + 0x155c] */
    MEM16(ebp + 0x1590) = LO16(ecx);
    SET_LO16(edx, MEM16(eax + 0x16BC));
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(ebp + 0x1564)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [ebp + 0x1564] */
    MEM16(ebp + 0x1592) = LO16(edx);
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0004BD56; /* jp: parity */

loc_0004BD37: ;
    fp_push(MEMF(ebp + 0x1570)); /* fld float */
    MEM32(esp + 0x28) = 1;
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x17628C)); /* fadd dword ptr [0x17628c] */
    MEMF(ebp + 0x1570) = (float)fp_top(); fp_pop(); /* fstp */
    goto loc_0004C1AD;

loc_0004BD56: ;
    MEM32(esp + 0x28) = 0;
    goto loc_0004C1B2;

loc_0004BD63: ;
    esi = MEM32(esp + 0x30);
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x504457D4u; /* flag setter tag */
    MEM32(ebp + 0x15BC) = eax;
    MEM32(esp + 0x14) = eax;
    MEM32(esp + 0x1C) = eax;
    eax = ZX16(MEM16(ebp + 0x262));
    PUSH32(esp, 4);
    PUSH32(esp, eax);
    PUSH32(esp, 0x10);
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~0x8C5u) | ((0) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((0 != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BD89u); RECOMP_ABI_CALL(0x0003D9F0u, sub_0003D9F0); /* call 0x0003D9F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BD89: ;
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BD9Du); RECOMP_ABI_CALL(0x0003FCF0u, sub_0003FCF0); /* call 0x0003FCF0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BD9D: ;
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0);
    PUSH32(esp, 0x2000);
    PUSH32(esp, 1);
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BDB4u); RECOMP_ABI_CALL(0x0003FCF0u, sub_0003FCF0); /* call 0x0003FCF0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BDB4: ;
    _fb = (uint32_t)(0x50) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x50)) >> 32) & 1);
    esp = esp + 0x50;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x5511BC70u; /* flag setter tag */
    goto loc_0004C1B2;

loc_0004BDBC: ;
    MEM32(ebp + 0x15BC) = 0x3F000000;
    MEM32(esp + 0x14) = 0;
    MEM32(esp + 0x1C) = 0;
    goto loc_0004C1B2;

loc_0004BDDB: ;
    PUSH32(esp, 4);
    PUSH32(esp, 1);
    _cf = 0; /* xor clears CF */
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x9F87D19Fu; /* flag setter tag */
    PUSH32(esp, 0x24);
    PUSH32(esp, ebp);
    MEM32(ebp + 0x15BC) = esi;
    MEM32(esp + 0x24) = esi;
    MEM32(esp + 0x2C) = esi;
    g_eflags = (g_eflags & ~0x8C5u) | ((0) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((0 != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BDF7u); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BDF7: ;
    PUSH32(esp, 4);
    PUSH32(esp, esi);
    PUSH32(esp, 0x21);
    PUSH32(esp, ebp);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BE02u); RECOMP_ABI_CALL(0x0003D9A0u, sub_0003D9A0); /* call 0x0003D9A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BE02: ;
    PUSH32(esp, 4);
    PUSH32(esp, esi);
    PUSH32(esp, 0x1B);
    PUSH32(esp, ebp);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BE0Du); RECOMP_ABI_CALL(0x0003D9A0u, sub_0003D9A0); /* call 0x0003D9A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BE0D: ;
    PUSH32(esp, 4);
    PUSH32(esp, 1);
    PUSH32(esp, 0x23);
    PUSH32(esp, ebp);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BE19u); RECOMP_ABI_CALL(0x0003D9A0u, sub_0003D9A0); /* call 0x0003D9A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BE19: ;
    eax = ZX16(MEM16(ebp + 0x25E));
    _fb = (uint32_t)(0xFFFFFFFEu) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0xFFFFFFFEu)) >> 32) & 1);
    eax = eax + 0xFFFFFFFEu;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xAD6276D1u; /* flag setter tag */
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x40)) >> 32) & 1);
    esp = esp + 0x40;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x67000AC6u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(5) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 5 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x8C512BE3u; /* flag setter tag */
    if (CMP_A(_fa, _fb)) goto loc_0004C1B2; /* ja: above (unsigned >) */

loc_0004BE2F: ;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    { uint32_t _jt = MEM32(eax * 4 + 0x4C870); /* switch: 14 entries, 10 targets */
    if (_jt == 0x0004BE36u) goto loc_0004BE36;
    if (_jt == 0x0004BE46u) goto loc_0004BE46;
    if (_jt == 0x0004C5AAu) goto loc_0004C5AA;
    if (_jt == 0x0004C5E3u) goto loc_0004C5E3;
    if (_jt == 0x0004C5EDu) goto loc_0004C5ED;
    if (_jt == 0x0004C607u) goto loc_0004C607;
    if (_jt == 0x0004C620u) goto loc_0004C620;
    if (_jt == 0x0004C649u) goto loc_0004C649;
    if (_jt == 0x0004C65Au) goto loc_0004C65A;
    if (_jt == 0x0004C6CDu) goto loc_0004C6CD;
    g_seh_ebp = ebp; RECOMP_ITAIL(_jt); return; }

loc_0004BE36: ;
    PUSH32(esp, 2);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BE3Eu); RECOMP_ABI_CALL(0x000419C0u, sub_000419C0); /* call 0x000419C0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BE3E: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    goto loc_0004C1B2;

loc_0004BE46: ;
    PUSH32(esp, 1);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BE4Eu); RECOMP_ABI_CALL(0x000419C0u, sub_000419C0); /* call 0x000419C0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BE4E: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    goto loc_0004C1B2;

loc_0004BE56: ;
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x3E0F5DA7u; /* flag setter tag */
    MEM32(esp + 0x18) = edi;
    MEM32(esp + 0x20) = 0xF;
    goto loc_0004C185;

loc_0004BE69: ;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edx, esi (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xE1E113C8u; /* flag setter tag */
    eax = MEM32(esp + 0x2C);
    if (CMP_EQ(_fa, _fb)) goto loc_0004BE7F; /* je: equal / zero */

loc_0004BE71: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004BE7A; /* jne: not equal / not zero */

loc_0004BE75: ;
    PUSH32(esp, 4);
    PUSH32(esp, esi);
    goto loc_0004BE8A;

loc_0004BE7A: ;
    PUSH32(esp, 5);
    PUSH32(esp, esi);
    goto loc_0004BE8A;

loc_0004BE7F: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004BE87; /* jne: not equal / not zero */

loc_0004BE83: ;
    PUSH32(esp, 0);
    goto loc_0004BE89;

loc_0004BE87: ;
    PUSH32(esp, 1);

loc_0004BE89: ;
    PUSH32(esp, edx);

loc_0004BE8A: ;
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BE90u); RECOMP_ABI_CALL(0x000423B0u, sub_000423B0); /* call 0x000423B0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BE90: ;
    ebx = ZX16(LO16(eax));
    eax = MEM32(esp + 0x1C);
    ecx = (uint32_t)(int32_t)SMEM16(eax + 0x3E);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */
    _fa = (uint32_t)(HI8(ebx)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp HI8(ebx), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xD919F1E7u; /* flag setter tag */
    MEM32(esp + 0x14) = 0x3F800000;
    MEM32(esp + 0x1C) = ecx;
    if ((((uint8_t)((uint8_t)(_fa) - (uint8_t)(_fb)) >> 7) == 0)) goto loc_0004BEE1; /* jns: not sign (positive) */

loc_0004BEAE: ;
    PUSH32(esp, ebp);
    MEM32(esp + 0x1C) = 0;
    _cf = 0; /* logical op clears CF */
    ebx = ebx & 0xFFFF7FFFu;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0x20F9C18Bu; /* flag setter tag */
    MEM32(esp + 0x24) = 7;
    MEM32(ebp + 0x15FC) = 5;
    edi = 3;
    g_eflags = (g_eflags & ~0x8C5u) | ((0) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((0 != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BED9u); RECOMP_ABI_CALL(0x00042A80u, sub_00042A80); /* call 0x00042A80 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BED9: ;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    goto loc_0004C1B2;

loc_0004BEE1: ;
    eax = MEM32(esp + 0x2C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004BF03; /* jne: not equal / not zero */

loc_0004BEE9: ;
    MEM32(esp + 0x18) = 0;
    MEM32(esp + 0x20) = 2;
    edi = 2;
    goto loc_0004C1B2;

loc_0004BF03: ;
    MEM32(esp + 0x18) = 1;
    MEM32(esp + 0x20) = 3;
    edi = 2;
    goto loc_0004C1B2;

loc_0004BF1D: ;
    eax = MEM32(esp + 0x2C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004BF29; /* jne: not equal / not zero */

loc_0004BF25: ;
    PUSH32(esp, 0);
    goto loc_0004BF2B;

loc_0004BF29: ;
    PUSH32(esp, 1);

loc_0004BF2B: ;
    PUSH32(esp, esi);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BF32u); RECOMP_ABI_CALL(0x000423B0u, sub_000423B0); /* call 0x000423B0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BF32: ;
    ebx = ZX16(LO16(eax));
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */
    _fa = (uint32_t)(HI8(ebx)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp HI8(ebx), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xD919F1E7u; /* flag setter tag */
    MEM32(esp + 0x14) = 0x3F800000;
    if ((((uint8_t)((uint8_t)(_fa) - (uint8_t)(_fb)) >> 7) == 0)) goto loc_0004BFFB; /* jns: not sign (positive) */

loc_0004BF48: ;
    eax = MEM32(esp + 0x28);
    _cf = 0; /* logical op clears CF */
    ebx = ebx & 0xFFFF7FFFu;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0x20F9C18Bu; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    MEM32(esp + 0x18) = 0;
    MEM32(esp + 0x20) = 7;
    MEM32(ebp + 0x15FC) = 5;
    MEM32(esp + 0x14) = 0x3F800000;
    if (CMP_EQ(_fa, _fb)) goto loc_0004BF88; /* je: equal / zero */

loc_0004BF78: ;
    MEM32(esp + 0x24) = 0x3F866666;
    MEM32(esp + 0x34) = 0x3FACCCCD;

loc_0004BF88: ;
    _fa = (uint32_t)(MEM16(0x39894A)) & 0xFFFFu; _fb = (uint32_t)(0) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(0x39894A), 0 (16-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x6B431BAFu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004BFB4; /* je: equal / zero */

loc_0004BF92: ;
    PUSH32(esp, 0x11);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint16_t)((uint16_t)(_fas) - (uint16_t)(_fbs)) >> 15) != 0)) ? 0x80u : 0u) | (((((uint16_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 15) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BF9Au); RECOMP_ABI_CALL(0x00034E50u, sub_00034E50); /* call 0x00034E50 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BF9A: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004BFB4; /* je: equal / zero */

loc_0004BFA1: ;
    PUSH32(esp, 0x11);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BFA9u); RECOMP_ABI_CALL(0x0003D5D0u, sub_0003D5D0); /* call 0x0003D5D0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BFA9: ;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x3C)); /* fmul dword ptr [esp + 0x3c] */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    MEMF(esp + 0x34) = (float)fp_top(); fp_pop(); /* fstp */

loc_0004BFB4: ;
    _fa = (uint32_t)(MEM32(ebp + 0x15B4)) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ebp + 0x15B4), 4 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x160DB6C1u; /* flag setter tag */
    if (CMP_GE(_fas, _fbs)) goto loc_0004BFDC; /* jge: greater or equal (signed >=) */

loc_0004BFBD: ;
    edx = MEM32(esp + 0x10);
    eax = (uint32_t)(int32_t)SMEM16(edx + 0x3C);
    PUSH32(esp, ebp);
    edi = 1;
    MEM32(esp + 0x20) = eax;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BFD4u); RECOMP_ABI_CALL(0x00042A80u, sub_00042A80); /* call 0x00042A80 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BFD4: ;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    goto loc_0004C1B2;

loc_0004BFDC: ;
    ecx = MEM32(esp + 0x10);
    edx = (uint32_t)(int32_t)SMEM16(ecx + 0x42);
    PUSH32(esp, ebp);
    edi = 3;
    MEM32(esp + 0x20) = edx;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004BFF3u); RECOMP_ABI_CALL(0x00042A80u, sub_00042A80); /* call 0x00042A80 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004BFF3: ;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    goto loc_0004C1B2;

loc_0004BFFB: ;
    eax = MEM32(esp + 0x2C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004C015; /* jne: not equal / not zero */

loc_0004C003: ;
    MEM32(esp + 0x18) = 0;
    MEM32(esp + 0x20) = 2;
    goto loc_0004C025;

loc_0004C015: ;
    MEM32(esp + 0x18) = 1;
    MEM32(esp + 0x20) = 3;

loc_0004C025: ;
    _fa = (uint32_t)(MEM32(ebp + 0x15B4)) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ebp + 0x15B4), 4 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x160DB6C1u; /* flag setter tag */
    if (CMP_GE(_fas, _fbs)) goto loc_0004C041; /* jge: greater or equal (signed >=) */

loc_0004C02E: ;
    eax = MEM32(esp + 0x10);
    ecx = (uint32_t)(int32_t)SMEM16(eax + 0x3E);
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x3E0F5DA7u; /* flag setter tag */
    MEM32(esp + 0x1C) = ecx;
    goto loc_0004C1B2;

loc_0004C041: ;
    edx = MEM32(esp + 0x10);
    eax = (uint32_t)(int32_t)SMEM16(edx + 0x44);
    edi = 2;
    MEM32(esp + 0x1C) = eax;
    goto loc_0004C1B2;

loc_0004C057: ;
    PUSH32(esp, 4);
    PUSH32(esp, 1);
    _cf = 0; /* xor clears CF */
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x9F87D19Fu; /* flag setter tag */
    PUSH32(esp, 0x29);
    PUSH32(esp, ebp);
    MEM32(ebp + 0x15BC) = esi;
    MEM32(esp + 0x24) = 0xBF800000u;
    MEM32(esp + 0x2C) = esi;
    ebx = 0x2000;
    MEM32(esp + 0x30) = 0xE;
    g_eflags = (g_eflags & ~0x8C5u) | ((0) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((0 != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C084u); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C084: ;
    PUSH32(esp, 4);
    PUSH32(esp, 1);
    PUSH32(esp, 0x12);
    PUSH32(esp, ebp);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C090u); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C090: ;
    PUSH32(esp, 4);
    PUSH32(esp, esi);
    PUSH32(esp, 0x14);
    PUSH32(esp, ebp);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C09Bu); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C09B: ;
    _fb = (uint32_t)(0x30) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x30)) >> 32) & 1);
    esp = esp + 0x30;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xEE4F31D2u; /* flag setter tag */
    goto loc_0004C1B2;

loc_0004C0A3: ;
    ecx = MEM32(ebp + 0x15A8);
    PUSH32(esp, ebp);
    ebx = 0x2000;
    edi = 4;
    MEM32(esp + 0x18) = 0x3F800000;
    MEM32(esp + 0x3C) = ecx;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C0C5u); RECOMP_ABI_CALL(0x00042A80u, sub_00042A80); /* call 0x00042A80 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C0C5: ;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    MEM32(esp + 0x20) = 0xE;
    goto loc_0004C1B2;

loc_0004C0D5: ;
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x504457D4u; /* flag setter tag */
    PUSH32(esp, 4);
    PUSH32(esp, eax);
    PUSH32(esp, 0x27);
    PUSH32(esp, ebp);
    MEM32(ebp + 0x15BC) = eax;
    MEM32(esp + 0x24) = 0xBF800000u;
    MEM32(esp + 0x2C) = eax;
    ebx = 0x2000;
    MEM32(esp + 0x30) = 0x11;
    g_eflags = (g_eflags & ~0x8C5u) | ((0) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((0 != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C101u); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C101: ;
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x10)) >> 32) & 1);
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7E08D19Du; /* flag setter tag */
    goto loc_0004C1B2;

loc_0004C109: ;
    SET_LO8(eax, MEM8(ebp + 0xA7));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004C11F; /* jne: not equal / not zero */

loc_0004C113: ;
    MEM32(esp + 0x18) = 0x2001;
    PUSH32(esp, 2);
    goto loc_0004C129;

loc_0004C11F: ;
    MEM32(esp + 0x18) = 0x2002;
    PUSH32(esp, 3);

loc_0004C129: ;
    PUSH32(esp, edx);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0)) ? 0x80u : 0u) | (((((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C130u); RECOMP_ABI_CALL(0x000423B0u, sub_000423B0); /* call 0x000423B0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C130: ;
    ebx = ZX16(LO16(eax));
    eax = MEM32(esp + 0x34);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C143; /* je: equal / zero */

loc_0004C13E: ;
    ebx = 0x2000;

loc_0004C143: ;
    edx = MEM32(esp + 0x10);
    eax = (uint32_t)(int32_t)SMEM16(edx + 0x3C);
    edi = 6;
    MEM32(esp + 0x14) = 0x3F800000;
    MEM32(esp + 0x1C) = eax;
    MEM32(esp + 0x20) = 0xA;
    goto loc_0004C1B2;

loc_0004C166: ;
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x504457D4u; /* flag setter tag */
    MEM32(esp + 0x18) = eax;
    MEM32(esp + 0x20) = 0x10;
    edi = 5;
    MEM32(ebp + 0x1594) = eax;
    MEM32(ebp + 0x15BC) = eax;

loc_0004C185: ;
    eax = MEM32(esp + 0x30);
    SET_LO16(ecx, MEM16(eax + 0x16BA));
    MEM16(ebp + 0x1590) = LO16(ecx);
    SET_LO16(edx, MEM16(eax + 0x16BC));
    MEM32(esp + 0x14) = 0x3DCCCCCD;
    MEM16(ebp + 0x1592) = LO16(edx);

loc_0004C1AD: ;
    ebx = 0x2000;

loc_0004C1B2: ;
    eax = MEM32(esp + 0x28);
    PUSH32(esp, 4);
    PUSH32(esp, eax);
    PUSH32(esp, 0x46);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0x7E08D19Du ? (_cf) : (_fk == 0x11A6DAE2u ? (CMP_B(_fa, _fb)) : (_fk == 0xFA6E46EDu ? (CMP_B(_fa, _fb)) : (_fk == 0x8C512BE3u ? (CMP_B(_fa, _fb)) : (_fk == 0xEE4F31D2u ? (_cf) : (_fk == 0x504457D4u ? (0) : (_fk == 0x3E0F5DA7u ? (0) : (_fk == 0x452A891Eu ? (CMP_B(_fa & _fb, 0)) : (_fk == 0x5511BC70u ? (_cf) : (_fk == 0xD0F44B25u ? (_cf) : (_fk == 0x160DB6C1u ? (CMP_B(_fa, _fb)) : (_fk == 0xD9300E98u ? (_cf) : 0))))))))))))) ? 0x1u : 0u) | (((_fk == 0x7E08D19Du ? (RECOMP_PARITY8(_fa)) : (_fk == 0x11A6DAE2u ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0xFA6E46EDu ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0x8C512BE3u ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0xEE4F31D2u ? (RECOMP_PARITY8(_fa)) : (_fk == 0x504457D4u ? (RECOMP_PARITY8(_fa)) : (_fk == 0x3E0F5DA7u ? (RECOMP_PARITY8(_fa)) : (_fk == 0x452A891Eu ? (RECOMP_PARITY8((_fa) & (_fb))) : (_fk == 0x5511BC70u ? (RECOMP_PARITY8(_fa)) : (_fk == 0xD0F44B25u ? (RECOMP_PARITY8(_fa)) : (_fk == 0x160DB6C1u ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0xD9300E98u ? (RECOMP_PARITY8(_fa)) : RECOMP_PARITY8(_fa)))))))))))))) ? 0x4u : 0u) | (((_fk == 0x7E08D19Du ? ((_fa == 0)) : (_fk == 0x11A6DAE2u ? (CMP_EQ(_fa, _fb)) : (_fk == 0xFA6E46EDu ? (CMP_EQ(_fa, _fb)) : (_fk == 0x8C512BE3u ? (CMP_EQ(_fa, _fb)) : (_fk == 0xEE4F31D2u ? ((_fa == 0)) : (_fk == 0x504457D4u ? ((_fa == 0)) : (_fk == 0x3E0F5DA7u ? ((_fa == 0)) : (_fk == 0x452A891Eu ? (TEST_Z(_fa, _fb)) : (_fk == 0x5511BC70u ? ((_fa == 0)) : (_fk == 0xD0F44B25u ? ((_fa == 0)) : (_fk == 0x160DB6C1u ? (CMP_EQ(_fa, _fb)) : (_fk == 0xD9300E98u ? ((_fa == 0)) : (_fa == 0)))))))))))))) ? 0x40u : 0u) | (((_fk == 0x7E08D19Du ? ((_fas < 0)) : (_fk == 0x11A6DAE2u ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fk == 0xFA6E46EDu ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fk == 0x8C512BE3u ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fk == 0xEE4F31D2u ? ((_fas < 0)) : (_fk == 0x504457D4u ? ((_fas < 0)) : (_fk == 0x3E0F5DA7u ? ((_fas < 0)) : (_fk == 0x452A891Eu ? (TEST_S(_fas, _fbs)) : (_fk == 0x5511BC70u ? ((_fas < 0)) : (_fk == 0xD0F44B25u ? ((_fas < 0)) : (_fk == 0x160DB6C1u ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fk == 0xD9300E98u ? ((_fas < 0)) : (_fas < 0)))))))))))))) ? 0x80u : 0u) | (((_fk == 0x7E08D19Du ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (_fk == 0x11A6DAE2u ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (_fk == 0xFA6E46EDu ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (_fk == 0x8C512BE3u ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (_fk == 0xEE4F31D2u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (_fk == 0x504457D4u ? ((0 != 0)) : (_fk == 0x3E0F5DA7u ? ((0 != 0)) : (_fk == 0x452A891Eu ? (0) : (_fk == 0x5511BC70u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (_fk == 0xD0F44B25u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (_fk == 0x160DB6C1u ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (_fk == 0xD9300E98u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (0 != 0)))))))))))))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C1C1u); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C1C1: ;
    edx = MEM32(esp + 0x20);
    ecx = edi + edi * 2;
    eax = edx + ecx * 2 + 8;
    ecx = MEM32(ebp + 0x16AC);
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x10)) >> 32) & 1);
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7E08D19Du; /* flag setter tag */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(0xFFFFFFFFu) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, 0xFFFFFFFFu (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x9AC5D3C3u; /* flag setter tag */
    MEM32(esp + 0x2C) = eax;
    if (CMP_EQ(_fa, _fb)) goto loc_0004C1E6; /* je: equal / zero */

loc_0004C1DE: ;
    MEM32(esp + 0x14) = 0;

loc_0004C1E6: ;
    esi = MEM32(esp + 0x30);
    MEM32(ebp + 0x1600) = edi;
    MEM32(esi + 0x16A4) = edi;
    MEM32(ebp + 0x1604) = eax;
    MEM32(esi + 0x16B0) = eax;
    _fa = (uint32_t)(MEM16(0x39895C)) & 0xFFFFu; _fb = (uint32_t)(0) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(0x39895C), 0 (16-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xB98CD874u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C229; /* je: equal / zero */

loc_0004C20C: ;
    fp_push(MEMF(ebp + 0x155C)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1776F8)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1776f8] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x42694A75u; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_0004C229; /* jne: not equal / not zero */

loc_0004C21F: ;
    MEM32(ebp + 0x15BC) = 0;

loc_0004C229: ;
    edi = MEM32(ebp + 0x15FC);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0xE) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0xE (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x5894FF92u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C35C; /* je: equal / zero */

loc_0004C238: ;
    fp_push(MEMF(ebp + 0x15BC)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 0x1570)); /* fmul dword ptr [ebp + 0x1570] */
    MEMF(esp + 0x40) = (float)fp_top(); /* fst */
    eax = MEM32(esp + 0x40);
    MEMF(ebp + 0x1570) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, eax);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C259u); RECOMP_ABI_CALL(0x0003D560u, sub_0003D560); /* call 0x0003D560 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C259: ;
    MEMF(ebp + 0x1570) = (float)fp_top(); /* fst */
    eax = edi;
    MEMF(ebp + 0x1574) = (float)fp_top(); fp_pop(); /* fstp */
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    if ((_fa != 0)) goto loc_0004C27A; /* jne: not equal / not zero */

loc_0004C26D: ;
    PUSH32(esp, 1);
    PUSH32(esp, 0x64);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((_fb != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C277u); RECOMP_ABI_CALL(0x00048780u, sub_00048780); /* call 0x00048780 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C277: ;
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */

loc_0004C27A: ;
    eax = MEM32(esp + 0x28);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004C35C; /* jne: not equal / not zero */

loc_0004C286: ;
    ecx = MEM32(esp + 0x3C);
    fp_push(MEMF(ecx + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 0x1574)); /* fmul dword ptr [ebp + 0x1574] */
    fp_push(MEMF(0x1761A0)); /* fld float */
    { double _t = fp_st1(); fp_push(_t); } /* fld st(1) */
    g_fp_cmp = RECOMP_FCMP(fp_top(), fp_st1()); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); fp_pop(); /* fucompp  */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xC77E4F9Du; /* flag setter tag */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_0004C2EE; /* jnp: not parity */

loc_0004C2A4: ;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x1560)); /* fmul dword ptr [esi + 0x1560] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_0004C2E0; /* jne: not equal / not zero */

loc_0004C2B7: ;
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    PUSH32(esp, 0);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x1570)); /* fadd dword ptr [esi + 0x1570] */
    PUSH32(esp, 0x64);
    PUSH32(esp, esi);
    MEMF(esi + 0x1570) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x1574)); /* fadd dword ptr [esi + 0x1574] */
    MEMF(esi + 0x1574) = (float)fp_top(); fp_pop(); /* fstp */
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) & (_fb))) ? 0x4u : 0u) | ((TEST_Z(_fa, _fb)) ? 0x40u : 0u) | ((TEST_S(_fas, _fbs)) ? 0x80u : 0u) | ((0) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C2DBu); RECOMP_ABI_CALL(0x00048780u, sub_00048780); /* call 0x00048780 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C2DB: ;
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */
    goto loc_0004C2F0;

loc_0004C2E0: ;
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x155C)); /* fadd dword ptr [esi + 0x155c] */
    MEMF(esi + 0x155C) = (float)fp_top(); fp_pop(); /* fstp */
    goto loc_0004C2F0;

loc_0004C2EE: ;
    fp_pop(); /* fstp st(0) */

loc_0004C2F0: ;
    edx = MEM32(esp + 0x44);
    fp_push(MEMF(edx + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 0x1570)); /* fmul dword ptr [ebp + 0x1570] */
    fp_push(MEMF(0x1761A0)); /* fld float */
    { double _t = fp_st1(); fp_push(_t); } /* fld st(1) */
    g_fp_cmp = RECOMP_FCMP(fp_top(), fp_st1()); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); fp_pop(); /* fucompp  */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xC77E4F9Du; /* flag setter tag */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_0004C35A; /* jnp: not parity */

loc_0004C30E: ;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esi + 0x1560)); /* fmul dword ptr [esi + 0x1560] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_0004C32F; /* jne: not equal / not zero */

loc_0004C321: ;
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x155C)); /* fadd dword ptr [esi + 0x155c] */
    MEMF(esi + 0x155C) = (float)fp_top(); fp_pop(); /* fstp */
    goto loc_0004C35C;

loc_0004C32F: ;
    fp_top() = -fp_top(); /* fchs */
    PUSH32(esp, 0);
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    PUSH32(esp, 0x64);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x1570)); /* fadd dword ptr [esi + 0x1570] */
    PUSH32(esp, esi);
    MEMF(esi + 0x1570) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x1574)); /* fadd dword ptr [esi + 0x1574] */
    MEMF(esi + 0x1574) = (float)fp_top(); fp_pop(); /* fstp */
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) & (_fb))) ? 0x4u : 0u) | ((TEST_Z(_fa, _fb)) ? 0x40u : 0u) | ((TEST_S(_fas, _fbs)) ? 0x80u : 0u) | ((0) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C355u); RECOMP_ABI_CALL(0x00048780u, sub_00048780); /* call 0x00048780 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C355: ;
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */
    goto loc_0004C35C;

loc_0004C35A: ;
    fp_pop(); /* fstp st(0) */

loc_0004C35C: ;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0xFFFFFFFFu) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0xFFFFFFFFu (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x8CC7981Fu; /* flag setter tag */
    esi = MEM32(esp + 0x18);
    if (CMP_NE(_fa, _fb)) goto loc_0004C36D; /* jne: not equal / not zero */

loc_0004C365: ;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, ebx (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xD91CF3FFu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C6CD; /* je: equal / zero */

loc_0004C36D: ;
    SET_LO8(ecx, MEM8(ebp + 0xAE));
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x504457D4u; /* flag setter tag */
    _fa = (uint32_t)(LO8(ecx)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(ecx), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xF68F40CDu; /* flag setter tag */
    MEM32(ebp + 0x15F0) = eax;
    if (CMP_NE(_fa, _fb)) goto loc_0004C385; /* jne: not equal / not zero */

loc_0004C37F: ;
    MEM32(ebp + 0x15F4) = eax;

loc_0004C385: ;
    edi = MEM32(esp + 0x20);
    PUSH32(esp, 4);
    PUSH32(esp, edi);
    PUSH32(esp, 9);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0)) ? 0x80u : 0u) | (((((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C394u); RECOMP_ABI_CALL(0x0003D9F0u, sub_0003D9F0); /* call 0x0003D9F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C394: ;
    PUSH32(esp, 4);
    PUSH32(esp, 1);
    PUSH32(esp, 0xE);
    PUSH32(esp, ebp);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C3A0u); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C3A0: ;
    eax = ebp + 0x1C28;
    PUSH32(esp, eax);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C3ACu); RECOMP_ABI_CALL(0x0003D6F0u, sub_0003D6F0); /* call 0x0003D6F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C3AC: ;
    ecx = ebp + 0x1FA8;
    PUSH32(esp, ecx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C3B8u); RECOMP_ABI_CALL(0x0003D6F0u, sub_0003D6F0); /* call 0x0003D6F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C3B8: ;
    _fb = (uint32_t)(0x28) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x28)) >> 32) & 1);
    esp = esp + 0x28;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xE0C8B704u; /* flag setter tag */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0xFFFFFFFFu) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0xFFFFFFFFu (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x11E87328u; /* flag setter tag */
    MEM16(ebp + 0x3BD2) = 1;
    if (CMP_EQ(_fa, _fb)) goto loc_0004C3E9; /* je: equal / zero */

loc_0004C3C9: ;
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0);
    PUSH32(esp, esi);
    PUSH32(esp, 0);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C3DCu); RECOMP_ABI_CALL(0x0003FCF0u, sub_0003FCF0); /* call 0x0003FCF0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C3DC: ;
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x20)) >> 32) & 1);
    esp = esp + 0x20;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xF9C06C5Fu; /* flag setter tag */
    MEM32(ebp + 0x1C54) = 1;

loc_0004C3E9: ;
    PUSH32(esp, 4);
    PUSH32(esp, edi);
    PUSH32(esp, 9);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0x11E87328u ? (CMP_B(_fa, _fb)) : _cf)) ? 0x1u : 0u) | (((_fk == 0x11E87328u ? (RECOMP_PARITY8((_fa) - (_fb))) : RECOMP_PARITY8(_fa))) ? 0x4u : 0u) | (((_fk == 0x11E87328u ? (CMP_EQ(_fa, _fb)) : (_fa == 0))) ? 0x40u : 0u) | (((_fk == 0x11E87328u ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fas < 0))) ? 0x80u : 0u) | (((_fk == 0x11E87328u ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C3F4u); RECOMP_ABI_CALL(0x0003D9F0u, sub_0003D9F0); /* call 0x0003D9F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C3F4: ;
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x10)) >> 32) & 1);
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7E08D19Du; /* flag setter tag */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0xFFFFFFFFu) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0xFFFFFFFFu (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x8CC7981Fu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C41C; /* je: equal / zero */

loc_0004C3FC: ;
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0xFFFFFFFFu);
    PUSH32(esp, 0);
    PUSH32(esp, ebx);
    PUSH32(esp, 1);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C40Fu); RECOMP_ABI_CALL(0x0003FCF0u, sub_0003FCF0); /* call 0x0003FCF0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C40F: ;
    _fb = (uint32_t)(0x20) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x20)) >> 32) & 1);
    esp = esp + 0x20;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xF9C06C5Fu; /* flag setter tag */
    MEM32(ebp + 0x1FD4) = 1;

loc_0004C41C: ;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0xFFFFFFFFu) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0xFFFFFFFFu (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x11E87328u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004C425; /* jne: not equal / not zero */

loc_0004C421: ;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, esi (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x276E80F1u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C452; /* je: equal / zero */

loc_0004C425: ;
    fp_push(MEMF(ebp + 0x2848)); /* fld float */
    edx = MEM32(ebp + 0x2874);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(ebp + 0x2874)); /* fsub dword ptr [ebp + 0x2874] */
    PUSH32(esp, ecx);
    MEM32(ebp + 0x2850) = edx;
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C446u); RECOMP_ABI_CALL(0x00033E40u, sub_00033E40); /* call 0x00033E40 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C446: ;
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, ebp);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C44Fu); RECOMP_ABI_CALL(0x000482D0u, sub_000482D0); /* call 0x000482D0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C44F: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */

loc_0004C452: ;
    fp_push(MEMF(esp + 0x14)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x42694A75u; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_0004C532; /* jne: not equal / not zero */

loc_0004C467: ;
    eax = MEM32(esp + 0x2C);
    ecx = MEM32(esp + 0x10);
    edx = 0x8000;
    _fa = (uint32_t)(MEM16(eax)) & 0xFFFFu; _fb = (uint32_t)(LO16(edx)) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(eax), LO16(edx) (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x75D30C41u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004C480; /* jne: not equal / not zero */

loc_0004C479: ;
    SET_LO16(esi, MEM16(ecx + 8));
    MEM16(eax) = LO16(esi);

loc_0004C480: ;
    _fa = (uint32_t)(MEM16(eax + 4)) & 0xFFFFu; _fb = (uint32_t)(LO16(edx)) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(eax + 4), LO16(edx) (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xEB9A91BEu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004C48E; /* jne: not equal / not zero */

loc_0004C486: ;
    SET_LO16(esi, MEM16(ecx + 0xC));
    MEM16(eax + 4) = LO16(esi);

loc_0004C48E: ;
    _fa = (uint32_t)(MEM16(eax + 2)) & 0xFFFFu; _fb = (uint32_t)(LO16(edx)) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(eax + 2), LO16(edx) (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xD74F419Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004C49C; /* jne: not equal / not zero */

loc_0004C494: ;
    SET_LO16(ecx, MEM16(ecx + 0xA));
    MEM16(eax + 2) = LO16(ecx);

loc_0004C49C: ;
    edx = MEM32(esp + 0x38);
    eax = MEM32(esp + 0x14);
    ecx = MEM32(esp + 0x2C);
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint16_t)((uint16_t)(_fas) - (uint16_t)(_fbs)) >> 15) != 0)) ? 0x80u : 0u) | (((((uint16_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 15) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C4B1u); RECOMP_ABI_CALL(0x00044B00u, sub_00044B00); /* call 0x00044B00 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C4B1: ;
    fp_push(MEMF(esp + 0x44)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 0x2970)); /* fmul dword ptr [ebp + 0x2970] */
    eax = MEM32(esp + 0x38);
    edi = ebp + 0x2970;
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x10)) >> 32) & 1);
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7E08D19Du; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    MEMF(edi) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x34)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 0x2978)); /* fmul dword ptr [ebp + 0x2978] */
    MEMF(ebp + 0x2978) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x24)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 0x2974)); /* fmul dword ptr [ebp + 0x2974] */
    MEMF(ebp + 0x2974) = (float)fp_top(); /* fst */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C530; /* je: equal / zero */

loc_0004C4EE: ;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_0004C532; /* jne: not equal / not zero */

loc_0004C4FB: ;
    ecx = edi;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) & (_fb))) ? 0x4u : 0u) | ((TEST_Z(_fa, _fb)) ? 0x40u : 0u) | ((TEST_S(_fas, _fbs)) ? 0x80u : 0u) | ((0) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C502u); RECOMP_ABI_CALL(0x00012B40u, sub_00012B40); /* call 0x00012B40 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C502: ;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x178170)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x178170] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_0004C532; /* jne: not equal / not zero */

loc_0004C50F: ;
    ecx = edi;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) & (_fb))) ? 0x4u : 0u) | ((TEST_Z(_fa, _fb)) ? 0x40u : 0u) | ((TEST_S(_fas, _fbs)) ? 0x80u : 0u) | ((0) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C516u); RECOMP_ABI_CALL(0x00012B40u, sub_00012B40); /* call 0x00012B40 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C516: ;
    fp_top() = RECOMP_FP_PC(MEMF(0x178170) / fp_top()); /* fdivr dword ptr [0x178170] */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(edi)); /* fmul dword ptr [edi] */
    MEMF(edi) = (float)fp_top(); fp_pop(); /* fstp */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ebp + 0x2978)); /* fmul dword ptr [ebp + 0x2978] */
    MEMF(ebp + 0x2978) = (float)fp_top(); fp_pop(); /* fstp */
    goto loc_0004C532;

loc_0004C530: ;
    fp_pop(); /* fstp st(0) */

loc_0004C532: ;
    eax = MEM32(esp + 0x1C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xFFFFFFFFu) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xFFFFFFFFu (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xB6C1447Bu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C541; /* je: equal / zero */

loc_0004C53B: ;
    MEM32(ebp + 0x1594) = eax;

loc_0004C541: ;
    _cf = 0; /* xor clears CF */
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x9F87D19Fu; /* flag setter tag */
    _fa = (uint32_t)(MEM16(0x39894A)) & 0xFFFFu; _fb = (uint32_t)(LO16(esi)) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(0x39894A), LO16(esi) (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xF09E7825u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C58C; /* je: equal / zero */

loc_0004C54C: ;
    edx = MEM32(esp + 0x4C);
    PUSH32(esp, edx);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint16_t)((uint16_t)(_fas) - (uint16_t)(_fbs)) >> 15) != 0)) ? 0x80u : 0u) | (((((uint16_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 15) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C556u); RECOMP_ABI_CALL(0x000F62D0u, sub_000F62D0); /* call 0x000F62D0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C556: ;
    eax = MEM32(esp + 0x34);
    PUSH32(esp, 0x1C);
    PUSH32(esp, eax);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C562u); RECOMP_ABI_CALL(0x00034E50u, sub_00034E50); /* call 0x00034E50 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C562: ;
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C58C; /* je: equal / zero */

loc_0004C569: ;
    _fa = (uint32_t)(MEM32(0x398968)) & 0xFFFFFFFFu; _fb = (uint32_t)(2) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x398968), 2 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x5DFFC374u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004C58C; /* jne: not equal / not zero */

loc_0004C572: ;
    fp_push(MEMF(ebp + 0x1574)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_0004C58C; /* jne: not equal / not zero */

loc_0004C585: ;
    MEM16(0x398128) = MEM16(0x398128) + 1;
    _fa = (uint32_t)(MEM16(0x398128)) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fb = (_fa == 0x8000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x9280A441u; /* flag setter tag */

loc_0004C58C: ;
    eax = MEM32(ebp + 0x15FC);
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xD) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xD (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x8980D579u; /* flag setter tag */
    if (CMP_A(_fa, _fb)) goto loc_0004C6CD; /* ja: above (unsigned >) */

loc_0004C59C: ;
    ecx = ZX8(MEM8(eax + 0x4C8A8));
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    { uint32_t _jt = MEM32(ecx * 4 + 0x4C888); /* switch: 8 entries, 8 targets */
    if (_jt == 0x0004C5AAu) goto loc_0004C5AA;
    if (_jt == 0x0004C5E3u) goto loc_0004C5E3;
    if (_jt == 0x0004C5EDu) goto loc_0004C5ED;
    if (_jt == 0x0004C607u) goto loc_0004C607;
    if (_jt == 0x0004C620u) goto loc_0004C620;
    if (_jt == 0x0004C649u) goto loc_0004C649;
    if (_jt == 0x0004C65Au) goto loc_0004C65A;
    if (_jt == 0x0004C6CDu) goto loc_0004C6CD;
    g_seh_ebp = ebp; RECOMP_ITAIL(_jt); return; }

loc_0004C5AA: ;
    edx = MEM32(esp + 0x10);
    SET_LO16(eax, MEM16(edx + 0x4C));
    MEM16(ebp + 0x15A0) = LO16(eax);
    ecx = 2;
    eax = ebp;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C5C5u); RECOMP_ABI_CALL(0x00046020u, sub_00046020); /* call 0x00046020 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C5C5: ;
    _fa = (uint32_t)(MEM32(esp + 0x48)) & 0xFFFFFFFFu; _fb = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(esp + 0x48), esi (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x33A13B1Du; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C6CD; /* je: equal / zero */

loc_0004C5CF: ;
    PUSH32(esp, 4);
    PUSH32(esp, 0x16);
    PUSH32(esp, 9);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C5DBu); RECOMP_ABI_CALL(0x0003D9F0u, sub_0003D9F0); /* call 0x0003D9F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C5DB: ;
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x10)) >> 32) & 1);
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7E08D19Du; /* flag setter tag */
    goto loc_0004C6CD;

loc_0004C5E3: ;
    ecx = 3;
    goto loc_0004C6C6;

loc_0004C5ED: ;
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C5F3u); RECOMP_ABI_CALL(0x00048C00u, sub_00048C00); /* call 0x00048C00 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C5F3: ;
    ecx = MEM32(ebp + 0x29A4);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    MEM32(ebp + 0x2974) = ecx;
    goto loc_0004C6CD;

loc_0004C607: ;
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C60Du); RECOMP_ABI_CALL(0x00048C00u, sub_00048C00); /* call 0x00048C00 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C60D: ;
    edx = MEM32(esp + 0x14);
    PUSH32(esp, edx);
    PUSH32(esp, ebp);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C618u); RECOMP_ABI_CALL(0x00046D30u, sub_00046D30); /* call 0x00046D30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C618: ;
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */
    goto loc_0004C6BB;

loc_0004C620: ;
    eax = MEM32(esp + 0x10);
    SET_LO16(ecx, MEM16(eax + 0x4C));
    PUSH32(esp, ebp);
    MEM16(ebp + 0x15A0) = LO16(ecx);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C635u); RECOMP_ABI_CALL(0x00048C00u, sub_00048C00); /* call 0x00048C00 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C635: ;
    PUSH32(esp, ebp);
    MEM32(ebp + 0x282C) = esi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C641u); RECOMP_ABI_CALL(0x000443E0u, sub_000443E0); /* call 0x000443E0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C641: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    goto loc_0004C6CD;

loc_0004C649: ;
    edx = MEM32(esp + 0x10);
    SET_LO16(eax, MEM16(edx + 0x4C));
    MEM16(ebp + 0x15A0) = LO16(eax);
    goto loc_0004C6CD;

loc_0004C65A: ;
    eax = (uint32_t)(int32_t)SMEM16(ebp + 0x1FF2);
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    ecx = MEM32(esp + 0x10);
    SET_LO16(edx, MEM16(ecx + 0x4C));
    MEM16(ebp + 0x15A0) = LO16(edx);
    if ((_fa != 0)) goto loc_0004C6BB; /* jne: not equal / not zero */

loc_0004C673: ;
    eax = MEM32(ebp + 0x27C8);
    MEM32(ebp + 0x15FC) = 6;
    if (((0xE) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((0xE) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((0xE) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0xB8FBF1BEu; /* flag setter tag */
    ecx = MEM32(eax + ebp + 0x4ED0);
    eax = eax + ebp + 0x4EA0;
    PUSH32(esp, 4);
    MEM32(ebp + 0x2920) = ecx;
    edx = MEM32(eax + 0x34);
    PUSH32(esp, 1);
    MEM32(ebp + 0x2924) = edx;
    eax = MEM32(eax + 0x38);
    PUSH32(esp, 0x15);
    PUSH32(esp, ebp);
    MEM32(ebp + 0x2928) = eax;
    g_eflags = (g_eflags & ~0xC5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C6B8u); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C6B8: ;
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x10)) >> 32) & 1);
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7E08D19Du; /* flag setter tag */

loc_0004C6BB: ;
    MEM32(ebp + 0x282C) = esi;
    ecx = 1;

loc_0004C6C6: ;
    eax = ebp;
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0xFA6E46EDu ? (CMP_B(_fa, _fb)) : (_fk == 0x794B12F6u ? (_cf) : (_fk == 0x496F2313u ? (_cf) : _cf)))) ? 0x1u : 0u) | (((_fk == 0xFA6E46EDu ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0x794B12F6u ? (RECOMP_PARITY8(_fa)) : (_fk == 0x496F2313u ? (RECOMP_PARITY8(_fa)) : RECOMP_PARITY8(_fa))))) ? 0x4u : 0u) | (((_fk == 0xFA6E46EDu ? (CMP_EQ(_fa, _fb)) : (_fk == 0x794B12F6u ? ((_fa == 0)) : (_fk == 0x496F2313u ? ((_fa == 0)) : (_fa == 0))))) ? 0x40u : 0u) | (((_fk == 0xFA6E46EDu ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fk == 0x794B12F6u ? ((_fas < 0)) : (_fk == 0x496F2313u ? ((_fas < 0)) : (_fas < 0))))) ? 0x80u : 0u) | (((_fk == 0xFA6E46EDu ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (_fk == 0x794B12F6u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (_fk == 0x496F2313u ? ((_fb != 0)) : (((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0))))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C6CDu); RECOMP_ABI_CALL(0x00046020u, sub_00046020); /* call 0x00046020 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C6CD: ;
    ecx = MEM32(esp + 0x2C);
    PUSH32(esp, ecx);
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0xD0F44B25u ? (_cf) : (_fk == 0xD91CF3FFu ? (CMP_B(_fa, _fb)) : (_fk == 0x33A13B1Du ? (CMP_B(_fa, _fb)) : (_fk == 0x00000000u ? ((g_eflags & 1u) /* eflags */) : (_fk == 0xFA6E46EDu ? (CMP_B(_fa, _fb)) : (_fk == 0x8980D579u ? (CMP_B(_fa, _fb)) : (_fk == 0x8C512BE3u ? (CMP_B(_fa, _fb)) : (_fk == 0xD9300E98u ? (_cf) : _cf))))))))) ? 0x1u : 0u) | (((_fk == 0xD0F44B25u ? (RECOMP_PARITY8(_fa)) : (_fk == 0xD91CF3FFu ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0x33A13B1Du ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0x00000000u ? ((g_eflags & 4u) /* eflags */) : (_fk == 0xFA6E46EDu ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0x8980D579u ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0x8C512BE3u ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0xD9300E98u ? (RECOMP_PARITY8(_fa)) : RECOMP_PARITY8(_fa)))))))))) ? 0x4u : 0u) | (((_fk == 0xD0F44B25u ? ((_fa == 0)) : (_fk == 0xD91CF3FFu ? (CMP_EQ(_fa, _fb)) : (_fk == 0x33A13B1Du ? (CMP_EQ(_fa, _fb)) : (_fk == 0x00000000u ? ((g_eflags & 0x40u) /* eflags */) : (_fk == 0xFA6E46EDu ? (CMP_EQ(_fa, _fb)) : (_fk == 0x8980D579u ? (CMP_EQ(_fa, _fb)) : (_fk == 0x8C512BE3u ? (CMP_EQ(_fa, _fb)) : (_fk == 0xD9300E98u ? ((_fa == 0)) : (_fa == 0)))))))))) ? 0x40u : 0u) | (((_fk == 0xD0F44B25u ? ((_fas < 0)) : (_fk == 0xD91CF3FFu ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fk == 0x33A13B1Du ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fk == 0x00000000u ? (((g_eflags >> 7) & 1u) /* eflags */) : (_fk == 0xFA6E46EDu ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fk == 0x8980D579u ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fk == 0x8C512BE3u ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fk == 0xD9300E98u ? ((_fas < 0)) : (_fas < 0)))))))))) ? 0x80u : 0u) | (((_fk == 0xD0F44B25u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (_fk == 0xD91CF3FFu ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (_fk == 0x33A13B1Du ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (_fk == 0x00000000u ? (((g_eflags >> 11) & 1u) /* eflags */) : (_fk == 0xFA6E46EDu ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (_fk == 0x8980D579u ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (_fk == 0x8C512BE3u ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (_fk == 0xD9300E98u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)))))))))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C6D8u); RECOMP_ABI_CALL(0x00042160u, sub_00042160); /* call 0x00042160 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C6D8: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */

loc_0004C6DB: ;
    SET_LO8(eax, MEM8(ebp + 0xAE));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C6ED; /* je: equal / zero */

loc_0004C6E5: ;
    MEM32(ebp + 0x34C) = MEM32(ebp + 0x34C) + 1;
    _fa = (uint32_t)(MEM32(ebp + 0x34C)) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0xA6FB6228u; /* flag setter tag */
    goto loc_0004C6F7;

loc_0004C6ED: ;
    MEM32(ebp + 0x34C) = 0xFFFFFFFFu;

loc_0004C6F7: ;
    PUSH32(esp, 1);
    PUSH32(esp, 0xFF00FF00u);
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0xA6FB6228u ? (_cf) : (_fk == 0x00874B0Eu ? (CMP_B(_fa, _fb)) : CMP_B(_fa, _fb)))) ? 0x1u : 0u) | (((_fk == 0xA6FB6228u ? (RECOMP_PARITY8(_fa)) : (_fk == 0x00874B0Eu ? (RECOMP_PARITY8((_fa) - (_fb))) : RECOMP_PARITY8((_fa) - (_fb))))) ? 0x4u : 0u) | (((_fk == 0xA6FB6228u ? ((_fa == 0)) : (_fk == 0x00874B0Eu ? (CMP_EQ(_fa, _fb)) : CMP_EQ(_fa, _fb)))) ? 0x40u : 0u) | (((_fk == 0xA6FB6228u ? ((_fas < 0)) : (_fk == 0x00874B0Eu ? ((((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0)) : (((uint16_t)((uint16_t)(_fas) - (uint16_t)(_fbs)) >> 15) != 0)))) ? 0x80u : 0u) | (((_fk == 0xA6FB6228u ? ((_fb != 0)) : (_fk == 0x00874B0Eu ? ((((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0)) : (((uint16_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 15) != 0)))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C703u); RECOMP_ABI_CALL(0x000343A0u, sub_000343A0); /* call 0x000343A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C703: ;
    edi = ebp + 0xE7;
    esi = ebp + 0xA0;
    ecx = 0x47;
    PUSH32(esp, ebp);
    if (!g_df) { uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi); uint32_t _n = ecx;
      if (_d + _n <= _s || _s + _n <= _d) memcpy(_d, _s, _n);
      else { uint32_t _i; for (_i = 0; _i < _n; _i++) _d[_i] = _s[_i]; }
      esi += ecx; edi += ecx; }
    else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM8(edi - _i) = MEM8(esi - _i); esi -= ecx; edi -= ecx; }
    ecx = 0; /* rep movsb */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C71Cu); RECOMP_ABI_CALL(0x00041AF0u, sub_00041AF0); /* call 0x00041AF0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C71C: ;
    PUSH32(esp, ebp);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C722u); RECOMP_ABI_CALL(0x00041A00u, sub_00041A00); /* call 0x00041A00 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C722: ;
    SET_LO8(eax, MEM8(ebp + 4));
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x10)) >> 32) & 1);
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7E08D19Du; /* flag setter tag */
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004C747; /* jne: not equal / not zero */

loc_0004C72C: ;
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0)) ? 0x80u : 0u) | (((((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C732u); RECOMP_ABI_CALL(0x00046A50u, sub_00046A50); /* call 0x00046A50 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C732: ;
    PUSH32(esp, ebp);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C738u); RECOMP_ABI_CALL(0x00048A40u, sub_00048A40); /* call 0x00048A40 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C738: ;
    PUSH32(esp, ebp);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C73Eu); RECOMP_ABI_CALL(0x00048510u, sub_00048510); /* call 0x00048510 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C73E: ;
    PUSH32(esp, ebp);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C744u); RECOMP_ABI_CALL(0x00053740u, sub_00053740); /* call 0x00053740 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C744: ;
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x10)) >> 32) & 1);
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7E08D19Du; /* flag setter tag */

loc_0004C747: ;
    esi = ebp + 0x40E0;
    ecx = esi;
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0x00874B0Eu ? (CMP_B(_fa, _fb)) : _cf)) ? 0x1u : 0u) | (((_fk == 0x00874B0Eu ? (RECOMP_PARITY8((_fa) - (_fb))) : RECOMP_PARITY8(_fa))) ? 0x4u : 0u) | (((_fk == 0x00874B0Eu ? (CMP_EQ(_fa, _fb)) : (_fa == 0))) ? 0x40u : 0u) | (((_fk == 0x00874B0Eu ? ((((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0)) : (_fas < 0))) ? 0x80u : 0u) | (((_fk == 0x00874B0Eu ? ((((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0)) : (((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C754u); RECOMP_ABI_CALL(0x00089BA0u, sub_00089BA0); /* call 0x00089BA0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C754: ;
    PUSH32(esp, 4);
    PUSH32(esp, 0xFF00FFFFu);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C760u); RECOMP_ABI_CALL(0x000343A0u, sub_000343A0); /* call 0x000343A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C760: ;
    edx = (uint32_t)(int32_t)SMEM8(ebp + 4);
    eax = MEM32(ebp + 0x27D0);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    PUSH32(esp, 0);
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    ecx = ebp + 0x2B10;
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C77Cu); RECOMP_ABI_CALL(0x00069370u, sub_00069370); /* call 0x00069370 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C77C: ;
    _fa = (uint32_t)(MEM16(0x39894A)) & 0xFFFFu; _fb = (uint32_t)(0) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(0x39894A), 0 (16-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x6B431BAFu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004C7BD; /* je: equal / zero */

loc_0004C786: ;
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
    if (TEST_Z(_fa, _fb)) goto loc_0004C7BD; /* je: equal / zero */

loc_0004C7A5: ;
    ecx = MEM32(ebp + 0x27D0);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    _fb = (uint32_t)(0x1880) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0x1880)) >> 32) & 1);
    ecx = ecx + 0x1880;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x4F4D992Au; /* flag setter tag */
    PUSH32(esp, 0);
    PUSH32(esp, ecx);
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C7BDu); RECOMP_ABI_CALL(0x00011B60u, sub_00011B60); /* call 0x00011B60 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C7BD: ;
    PUSH32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0x6B431BAFu ? (CMP_B(_fa, _fb)) : (_fk == 0x00000000u ? ((g_eflags & 1u) /* eflags */) : CMP_B(_fa & _fb, 0)))) ? 0x1u : 0u) | (((_fk == 0x6B431BAFu ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0x00000000u ? ((g_eflags & 4u) /* eflags */) : RECOMP_PARITY8((_fa) & (_fb))))) ? 0x4u : 0u) | (((_fk == 0x6B431BAFu ? (CMP_EQ(_fa, _fb)) : (_fk == 0x00000000u ? ((g_eflags & 0x40u) /* eflags */) : TEST_Z(_fa, _fb)))) ? 0x40u : 0u) | (((_fk == 0x6B431BAFu ? ((((uint16_t)((uint16_t)(_fas) - (uint16_t)(_fbs)) >> 15) != 0)) : (_fk == 0x00000000u ? (((g_eflags >> 7) & 1u) /* eflags */) : TEST_S(_fas, _fbs)))) ? 0x80u : 0u) | (((_fk == 0x6B431BAFu ? ((((uint16_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 15) != 0)) : (_fk == 0x00000000u ? (((g_eflags >> 11) & 1u) /* eflags */) : 0))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C7C3u); RECOMP_ABI_CALL(0x0004B420u, sub_0004B420); /* call 0x0004B420 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C7C3: ;
    PUSH32(esp, 3);
    PUSH32(esp, 0xFF000000u);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C7CFu); RECOMP_ABI_CALL(0x000343A0u, sub_000343A0); /* call 0x000343A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C7CF: ;
    SET_LO8(eax, MEM8(ebp + 4));
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004C7E0; /* jne: not equal / not zero */

loc_0004C7D9: ;
    ecx = esi;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0)) ? 0x80u : 0u) | (((((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C7E0u); RECOMP_ABI_CALL(0x00089850u, sub_00089850); /* call 0x00089850 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C7E0: ;
    PUSH32(esp, 3);
    PUSH32(esp, 0xFFFFFF00u);
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0x00000000u ? ((g_eflags & 1u) /* eflags */) : CMP_B(_fa, _fb))) ? 0x1u : 0u) | (((_fk == 0x00000000u ? ((g_eflags & 4u) /* eflags */) : RECOMP_PARITY8((_fa) - (_fb)))) ? 0x4u : 0u) | (((_fk == 0x00000000u ? ((g_eflags & 0x40u) /* eflags */) : CMP_EQ(_fa, _fb))) ? 0x40u : 0u) | (((_fk == 0x00000000u ? (((g_eflags >> 7) & 1u) /* eflags */) : (((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0))) ? 0x80u : 0u) | (((_fk == 0x00000000u ? (((g_eflags >> 11) & 1u) /* eflags */) : (((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C7ECu); RECOMP_ABI_CALL(0x000343A0u, sub_000343A0); /* call 0x000343A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C7EC: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */

loc_0004C7EF: ;
    POP32(esp, esi);

loc_0004C7F0: ;
    ecx = MEM32(esp + 0xAC);
    POP32(esp, edi);
    POP32(esp, ebp);
    POP32(esp, ebx);
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0x11A6DAE2u ? (CMP_B(_fa, _fb)) : (_fk == 0xA991CA4Du ? (CMP_B(_fa, _fb)) : _cf))) ? 0x1u : 0u) | (((_fk == 0x11A6DAE2u ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0xA991CA4Du ? (RECOMP_PARITY8((_fa) - (_fb))) : RECOMP_PARITY8(_fa)))) ? 0x4u : 0u) | (((_fk == 0x11A6DAE2u ? (CMP_EQ(_fa, _fb)) : (_fk == 0xA991CA4Du ? (CMP_EQ(_fa, _fb)) : (_fa == 0)))) ? 0x40u : 0u) | (((_fk == 0x11A6DAE2u ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fk == 0xA991CA4Du ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fas < 0)))) ? 0x80u : 0u) | (((_fk == 0x11A6DAE2u ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (_fk == 0xA991CA4Du ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004C7FFu); RECOMP_ABI_CALL(0x001456E4u, sub_001456E4); /* call 0x001456E4 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004C7FF: ;
    _fb = (uint32_t)(0xA4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xA4)) >> 32) & 1);
    esp = esp + 0xA4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x9746B83Fu; /* flag setter tag */
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

/**
 * sub_0004C8C0
 * Original: 0x0004C8C0 - 0x0004D065 (1957 bytes, 560 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */

void sc2_fast_sub_0004D090(void)
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

loc_0004D090: ;
    PUSH32(esp, ebx);
    ebx = MEM32(0x38E588);
    PUSH32(esp, esi);
    esi = MEM32(ebx + 0x54);
    eax = MEM32(esi + 0x74);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    PUSH32(esp, edi);
    edi = MEM32(ebx + 0x50);
    if (CMP_EQ(_fa, _fb)) goto loc_0004D5E7; /* je: equal / zero */

loc_0004D0AA: ;
    _fa = (uint32_t)(MEM32(0x398B28)) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x398B28), 4 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x1A929F05u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D12C; /* jne: not equal / not zero */

loc_0004D0B3: ;
    eax = (uint32_t)(int32_t)SMEM8(esi + 4);
    ecx = MEM32(esi + 0x27D0);
    PUSH32(esp, 1);
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = esi + 0x2B10;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D0CCu); RECOMP_ABI_CALL(0x00069370u, sub_00069370); /* call 0x00069370 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D0CC: ;
    eax = MEM32(esi + 0xC);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D0FD; /* je: equal / zero */

loc_0004D0D3: ;
    edx = MEM32(esi + 0x27D0);
    _fb = (uint32_t)(0x80) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edx) + (uint64_t)(0x80)) >> 32) & 1);
    edx = edx + 0x80;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x0F7095CEu; /* flag setter tag */
    MEM32(eax + 0x5C) = edx;
    eax = MEM32(esi + 0x10);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D0FD; /* je: equal / zero */

loc_0004D0E9: ;
    ecx = MEM32(esi + 0x27CC);
    edx = MEM32(esi + 0x27D0);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(ecx) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    ecx = (uint32_t)(ecx) << ((6) & 31u);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x29DB4FF6u; /* flag setter tag */
    _fb = (uint32_t)(edx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(edx)) >> 32) & 1);
    ecx = ecx + edx;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD1D888B0u; /* flag setter tag */
    MEM32(eax + 0x5C) = ecx;

loc_0004D0FD: ;
    PUSH32(esp, esi);
    ecx = 0x3E51E0;
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0xD1D888B0u ? (_cf) : CMP_B(_fa, _fb))) ? 0x1u : 0u) | (((_fk == 0xD1D888B0u ? (RECOMP_PARITY8(_fa)) : RECOMP_PARITY8((_fa) - (_fb)))) ? 0x4u : 0u) | (((_fk == 0xD1D888B0u ? ((_fa == 0)) : CMP_EQ(_fa, _fb))) ? 0x40u : 0u) | (((_fk == 0xD1D888B0u ? ((_fas < 0)) : (((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0))) ? 0x80u : 0u) | (((_fk == 0xD1D888B0u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D108u); RECOMP_ABI_CALL(0x000E94F0u, sub_000E94F0); /* call 0x000E94F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D108: ;
    PUSH32(esp, esi);
    ecx = 0x3E51E0;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D113u); RECOMP_ABI_CALL(0x000E9610u, sub_000E9610); /* call 0x000E9610 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D113: ;
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D119u); RECOMP_ABI_CALL(0x000E6410u, sub_000E6410); /* call 0x000E6410 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D119: ;
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D11Fu); RECOMP_ABI_CALL(0x00047750u, sub_00047750); /* call 0x00047750 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D11F: ;
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D125u); RECOMP_ABI_CALL(0x0004B420u, sub_0004B420); /* call 0x0004B420 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D125: ;
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

loc_0004D12C: ;
    PUSH32(esp, ebp);
    ebp = 7;
    _fa = (uint32_t)(MEM16(0x39895E)) & 0xFFFFu; _fb = (uint32_t)(LO16(ebp)) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(0x39895E), LO16(ebp) (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x69340278u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D144; /* jne: not equal / not zero */

loc_0004D13B: ;
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint16_t)((uint16_t)(_fas) - (uint16_t)(_fbs)) >> 15) != 0)) ? 0x80u : 0u) | (((((uint16_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 15) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D141u); RECOMP_ABI_CALL(0x00053C00u, sub_00053C00); /* call 0x00053C00 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D141: ;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */

loc_0004D144: ;
    PUSH32(esp, esi);
    MEM32(edi * 4 + 0x39813C) = 0;
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0xD9300E98u ? (_cf) : CMP_B(_fa, _fb))) ? 0x1u : 0u) | (((_fk == 0xD9300E98u ? (RECOMP_PARITY8(_fa)) : RECOMP_PARITY8((_fa) - (_fb)))) ? 0x4u : 0u) | (((_fk == 0xD9300E98u ? ((_fa == 0)) : CMP_EQ(_fa, _fb))) ? 0x40u : 0u) | (((_fk == 0xD9300E98u ? ((_fas < 0)) : (((uint16_t)((uint16_t)(_fas) - (uint16_t)(_fbs)) >> 15) != 0))) ? 0x80u : 0u) | (((_fk == 0xD9300E98u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (((uint16_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 15) != 0))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D155u); RECOMP_ABI_CALL(0x000440E0u, sub_000440E0); /* call 0x000440E0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D155: ;
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D15Bu); RECOMP_ABI_CALL(0x000441C0u, sub_000441C0); /* call 0x000441C0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D15B: ;
    SET_LO8(eax, MEM8(esi + 0xAE));
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D188; /* je: equal / zero */

loc_0004D168: ;
    SET_LO8(eax, MEM8(esi + 0xAF));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D188; /* jne: not equal / not zero */

loc_0004D172: ;
    SET_LO8(eax, MEM8(esi + 0xB4));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D1AD; /* jne: not equal / not zero */

loc_0004D17C: ;
    MEM32(esi + 0x15AC) = 0;
    goto loc_0004D1AD;

loc_0004D188: ;
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x504457D4u; /* flag setter tag */
    MEM32(esi + 0x15AC) = eax;
    MEM32(esi + 0x15A8) = eax;
    MEM32(esi + 0x15A4) = eax;
    eax = 1;
    MEM32(esi + 0x15B8) = eax;
    MEM32(esi + 0x15B4) = eax;

loc_0004D1AD: ;
    eax = MEM32(0x398700);
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x3E0F5DA7u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x81DCDC20u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D1C2; /* je: equal / zero */

loc_0004D1B8: ;
    edi = 1;
    goto loc_0004D2AD;

loc_0004D1C2: ;
    eax = (uint32_t)(int32_t)SMEM16(0x398942);
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    if ((_fa == 0)) goto loc_0004D206; /* je: equal / zero */

loc_0004D1CC: ;
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    if ((_fa != 0)) goto loc_0004D2A2; /* jne: not equal / not zero */

loc_0004D1D3: ;
    PUSH32(esp, 1);
    PUSH32(esp, 0x399220);
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((_fb != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D1DFu); RECOMP_ABI_CALL(0x00041120u, sub_00041120); /* call 0x00041120 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D1DF: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D2A2; /* je: equal / zero */

loc_0004D1EA: ;
    PUSH32(esp, 1);
    PUSH32(esp, 0x3AA080);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D1F6u); RECOMP_ABI_CALL(0x00041120u, sub_00041120); /* call 0x00041120 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D1F6: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D2A2; /* je: equal / zero */

loc_0004D201: ;
    goto loc_0004D29D;

loc_0004D206: ;
    eax = MEM32(0x398968);
    _fb = (uint32_t)(0xFFFFFFFEu) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0xFFFFFFFEu)) >> 32) & 1);
    eax = eax + 0xFFFFFFFEu;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xAD6276D1u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xE) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xE (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xFA6E46EDu; /* flag setter tag */
    if (CMP_A(_fa, _fb)) goto loc_0004D2A2; /* ja: above (unsigned >) */

loc_0004D217: ;
    edx = ZX8(MEM8(eax + 0x4D5FC));
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    { uint32_t _jt = MEM32(edx * 4 + 0x4D5EC); /* switch: 4 entries, 4 targets */
    if (_jt == 0x0004D225u) goto loc_0004D225;
    if (_jt == 0x0004D234u) goto loc_0004D234;
    if (_jt == 0x0004D294u) goto loc_0004D294;
    if (_jt == 0x0004D2A2u) goto loc_0004D2A2;
    g_seh_ebp = ebp; RECOMP_ITAIL(_jt); return; }

loc_0004D225: ;
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D22Bu); RECOMP_ABI_CALL(0x000356D0u, sub_000356D0); /* call 0x000356D0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D22B: ;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D2A2; /* je: equal / zero */

loc_0004D232: ;
    goto loc_0004D29D;

loc_0004D234: ;
    eax = MEM32(esi + 0x1558);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D262; /* jne: not equal / not zero */

loc_0004D23E: ;
    SET_LO8(eax, MEM8(esi + 0xD2));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D262; /* jne: not equal / not zero */

loc_0004D248: ;
    fp_push(MEMF(esi + 0x155C)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_0004D262; /* jne: not equal / not zero */

loc_0004D25B: ;
    edi = 1;
    goto loc_0004D264;

loc_0004D262: ;
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x3E0F5DA7u; /* flag setter tag */

loc_0004D264: ;
    eax = (uint32_t)(int32_t)SMEM16(0x39898A);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 4 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x16E51989u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D275; /* je: equal / zero */

loc_0004D270: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xA) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xA (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xFD2D8586u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D277; /* jne: not equal / not zero */

loc_0004D275: ;
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x3E0F5DA7u; /* flag setter tag */

loc_0004D277: ;
    _fa = (uint32_t)(MEM32(0x398994)) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x398994), 4 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xC3741360u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D2A2; /* jne: not equal / not zero */

loc_0004D280: ;
    eax = MEM32(0x3E0528);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_BE(_fa, _fb)) goto loc_0004D2A2; /* jbe: below or equal (unsigned <=) */

loc_0004D289: ;
    SET_LO8(eax, MEM8(esi + 6));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D2A2; /* jne: not equal / not zero */

loc_0004D290: ;
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x3E0F5DA7u; /* flag setter tag */
    goto loc_0004D2A2;

loc_0004D294: ;
    SET_LO8(eax, MEM8(0x39895A));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(MEM8(esi)) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), MEM8(esi) (8-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xB7D9D44Fu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D2A2; /* jne: not equal / not zero */

loc_0004D29D: ;
    edi = 1;

loc_0004D2A2: ;
    eax = MEM32(0x39898C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D2AD; /* je: equal / zero */

loc_0004D2AB: ;
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x3E0F5DA7u; /* flag setter tag */

loc_0004D2AD: ;
    PUSH32(esp, 4);
    PUSH32(esp, edi);
    PUSH32(esp, 0x1E);
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0x81DCDC20u ? (CMP_B(_fa, _fb)) : (_fk == 0x11A6DAE2u ? (CMP_B(_fa, _fb)) : 0))) ? 0x1u : 0u) | (((_fk == 0x81DCDC20u ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0x11A6DAE2u ? (RECOMP_PARITY8((_fa) - (_fb))) : RECOMP_PARITY8(_fa)))) ? 0x4u : 0u) | (((_fk == 0x81DCDC20u ? (CMP_EQ(_fa, _fb)) : (_fk == 0x11A6DAE2u ? (CMP_EQ(_fa, _fb)) : (_fa == 0)))) ? 0x40u : 0u) | (((_fk == 0x81DCDC20u ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fk == 0x11A6DAE2u ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fas < 0)))) ? 0x80u : 0u) | (((_fk == 0x81DCDC20u ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (_fk == 0x11A6DAE2u ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (0 != 0)))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D2B8u); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D2B8: ;
    SET_LO8(eax, MEM8(esi + 4));
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x10)) >> 32) & 1);
    esp = esp + 0x10;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7E08D19Du; /* flag setter tag */
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D5E6; /* jne: not equal / not zero */

loc_0004D2C6: ;
    _fa = (uint32_t)(MEM32(0x398968)) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x398968), 4 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xB4A5628Au; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D5E6; /* je: equal / zero */

loc_0004D2D3: ;
    eax = MEM32(esi + 0x50);
    _cf = 0; /* xor clears CF */
    ebp = 0; /* xor self */
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x5AD26482u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ebp (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x81DCDC20u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D2E5; /* je: equal / zero */

loc_0004D2DC: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(1) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 1 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x8B12E888u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D2E5; /* je: equal / zero */

loc_0004D2E1: ;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, ebp (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xB61EFD1Cu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D2F4; /* jne: not equal / not zero */

loc_0004D2E5: ;
    PUSH32(esp, esi);
    MEM32(esi + 0x18) = ebp;
    MEM32(esi + 0x14) = ebp;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D2F1u); RECOMP_ABI_CALL(0x000412F0u, sub_000412F0); /* call 0x000412F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D2F1: ;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */

loc_0004D2F4: ;
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0xB61EFD1Cu ? (CMP_B(_fa, _fb)) : _cf)) ? 0x1u : 0u) | (((_fk == 0xB61EFD1Cu ? (RECOMP_PARITY8((_fa) - (_fb))) : RECOMP_PARITY8(_fa))) ? 0x4u : 0u) | (((_fk == 0xB61EFD1Cu ? (CMP_EQ(_fa, _fb)) : (_fa == 0))) ? 0x40u : 0u) | (((_fk == 0xB61EFD1Cu ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fas < 0))) ? 0x80u : 0u) | (((_fk == 0xB61EFD1Cu ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D2FAu); RECOMP_ABI_CALL(0x000415F0u, sub_000415F0); /* call 0x000415F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D2FA: ;
    SET_LO8(eax, MEM8(esi + 0xBE));
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D319; /* je: equal / zero */

loc_0004D307: ;
    PUSH32(esp, ebp);
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0)) ? 0x80u : 0u) | (((((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D30Eu); RECOMP_ABI_CALL(0x00041330u, sub_00041330); /* call 0x00041330 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D30E: ;
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D314u); RECOMP_ABI_CALL(0x00041550u, sub_00041550); /* call 0x00041550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D314: ;
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */
    goto loc_0004D32B;

loc_0004D319: ;
    _fa = (uint32_t)(MEM32(0x398994)) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x398994), 4 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xC3741360u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D32B; /* jne: not equal / not zero */

loc_0004D322: ;
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D328u); RECOMP_ABI_CALL(0x00041550u, sub_00041550); /* call 0x00041550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D328: ;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */

loc_0004D32B: ;
    _fa = (uint32_t)(MEM16(0x398942)) & 0xFFFFu; _fb = (uint32_t)(2) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(0x398942), 2 (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x3862CE67u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D393; /* jne: not equal / not zero */

loc_0004D335: ;
    _fa = (uint32_t)(MEM16(0x39894A)) & 0xFFFFu; _fb = (uint32_t)(LO16(ebp)) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(0x39894A), LO16(ebp) (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xD8D36E48u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D393; /* je: equal / zero */

loc_0004D33E: ;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint16_t)((uint16_t)(_fas) - (uint16_t)(_fbs)) >> 15) != 0)) ? 0x80u : 0u) | (((((uint16_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 15) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D343u); RECOMP_ABI_CALL(0x000F67D0u, sub_000F67D0); /* call 0x000F67D0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D343: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D350; /* je: equal / zero */

loc_0004D347: ;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D34Cu); RECOMP_ABI_CALL(0x000F6A10u, sub_000F6A10); /* call 0x000F6A10 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D34C: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D35A; /* je: equal / zero */

loc_0004D350: ;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D355u); RECOMP_ABI_CALL(0x00050D20u, sub_00050D20); /* call 0x00050D20 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D355: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(2) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 2 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xFFBFB877u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D360; /* jne: not equal / not zero */

loc_0004D35A: ;
    MEM32(esi + 0x20) = ebp;
    MEM32(esi + 0x24) = ebp;

loc_0004D360: ;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D365u); RECOMP_ABI_CALL(0x000F7170u, sub_000F7170); /* call 0x000F7170 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D365: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D393; /* je: equal / zero */

loc_0004D369: ;
    _fa = (uint32_t)(MEM8(esi + 6)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp MEM8(esi + 6), 1 (8-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x1D60E4C9u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D393; /* jne: not equal / not zero */

loc_0004D36F: ;
    _fa = (uint32_t)(MEM32(0x398944)) & 0xFFFFFFFFu; _fb = (uint32_t)(0x5A) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x398944), 0x5A (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x7B4252D2u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D393; /* jne: not equal / not zero */

loc_0004D378: ;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D37Du); RECOMP_ABI_CALL(0x0001BF30u, sub_0001BF30); /* call 0x0001BF30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D37D: ;
    _cf = 0; /* xor clears CF */
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x0296481Bu; /* flag setter tag */
    ecx = 3;
    { uint64_t _dividend = ((uint64_t)edx << 32) | eax;
      eax = (uint32_t)(_dividend / (uint32_t)ecx);
      edx = (uint32_t)(_dividend % (uint32_t)ecx); }
    edx = MEM32(edx * 4 + 0x1A0B9C);
    MEM32(esi + 0x24) = edx;
    MEM32(esi + 0x20) = edx;

loc_0004D393: ;
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D399u); RECOMP_ABI_CALL(0x0003E7A0u, sub_0003E7A0); /* call 0x0003E7A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D399: ;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D3A0u); RECOMP_ABI_CALL(0x00048170u, sub_00048170); /* call 0x00048170 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D3A0: ;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D3A7u); RECOMP_ABI_CALL(0x0004C8C0u, sub_0004C8C0); /* call 0x0004C8C0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D3A7: ;
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D3AEu); RECOMP_ABI_CALL(0x0004B030u, sub_0004B030); /* call 0x0004B030 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D3AE: ;
    PUSH32(esp, 1);
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D3B6u); RECOMP_ABI_CALL(0x00041120u, sub_00041120); /* call 0x00041120 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D3B6: ;
    _fb = (uint32_t)(0x24) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x24)) >> 32) & 1);
    esp = esp + 0x24;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xFE83AF34u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D5E6; /* je: equal / zero */

loc_0004D3C1: ;
    SET_LO8(eax, MEM8(esi + 0xB5));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D478; /* je: equal / zero */

loc_0004D3CF: ;
    fp_push(MEMF(esi + 0x2984)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x2974)); /* fadd dword ptr [esi + 0x2974] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0004D478; /* jp: parity */

loc_0004D3EC: ;
    _fa = (uint32_t)(MEM32(0x1A0B88)) & 0xFFFFFFFFu; _fb = (uint32_t)(0xFFFFFFFFu) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(0x1A0B88), 0xFFFFFFFFu (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x78DD5527u; /* flag setter tag */
    edi = 0x1A0B88;
    if (CMP_EQ(_fa, _fb)) goto loc_0004D478; /* je: equal / zero */

loc_0004D3FA: ;
    ebp = esi + 0x3D70;

loc_0004D400: ;
    eax = MEM32(edi);
    PUSH32(esp, eax);
    ecx = ebp;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D40Au); RECOMP_ABI_CALL(0x0007BD50u, sub_0007BD50); /* call 0x0007BD50 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D40A: ;
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D41B; /* jne: not equal / not zero */

loc_0004D40E: ;
    eax = MEM32(edi + 4);
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(4)) >> 32) & 1);
    edi = edi + 4;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xC65126F3u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0xFFFFFFFFu) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0xFFFFFFFFu (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xB6C1447Bu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D400; /* jne: not equal / not zero */

loc_0004D419: ;
    goto loc_0004D478;

loc_0004D41B: ;
    _fa = (uint32_t)(MEM32(edi)) & 0xFFFFFFFFu; _fb = (uint32_t)(0xFFFFFFFFu) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(edi), 0xFFFFFFFFu (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x192A0639u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D478; /* je: equal / zero */

loc_0004D420: ;
    fp_push(MEMF(esi + 0x2820)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esi + 0x2924)); /* fsub dword ptr [esi + 0x2924] */
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    fp_top() = fabs(fp_top()); /* fabs */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x176280)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x176280] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_0004D44B; /* jp: parity */

loc_0004D43D: ;
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esi + 0x2924)); /* fadd dword ptr [esi + 0x2924] */
    MEMF(esi + 0x2924) = (float)fp_top(); fp_pop(); /* fstp */
    goto loc_0004D44D;

loc_0004D44B: ;
    fp_pop(); /* fstp st(0) */

loc_0004D44D: ;
    PUSH32(esp, 4);
    PUSH32(esp, 0);
    PUSH32(esp, 0x12);
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) & (_fb))) ? 0x4u : 0u) | ((TEST_Z(_fa, _fb)) ? 0x40u : 0u) | ((TEST_S(_fas, _fbs)) ? 0x80u : 0u) | ((0) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D459u); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D459: ;
    PUSH32(esp, 4);
    PUSH32(esp, 0);
    PUSH32(esp, 0x15);
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D465u); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D465: ;
    PUSH32(esp, esi);
    MEM32(esi + 0x3EE4) = 0;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D475u); RECOMP_ABI_CALL(0x00043690u, sub_00043690); /* call 0x00043690 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D475: ;
    _fb = (uint32_t)(0x24) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x24)) >> 32) & 1);
    esp = esp + 0x24;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xFE83AF34u; /* flag setter tag */

loc_0004D478: ;
    edi = esi;
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0x00874B0Eu ? (CMP_B(_fa, _fb)) : (_fk == 0x78DD5527u ? (CMP_B(_fa, _fb)) : (_fk == 0x540E384Au ? (CMP_B(_fa & _fb, 0)) : (_fk == 0xFE83AF34u ? (_cf) : (_fk == 0xB6C1447Bu ? (CMP_B(_fa, _fb)) : CMP_B(_fa, _fb))))))) ? 0x1u : 0u) | (((_fk == 0x00874B0Eu ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0x78DD5527u ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0x540E384Au ? (RECOMP_PARITY8((_fa) & (_fb))) : (_fk == 0xFE83AF34u ? (RECOMP_PARITY8(_fa)) : (_fk == 0xB6C1447Bu ? (RECOMP_PARITY8((_fa) - (_fb))) : RECOMP_PARITY8((_fa) - (_fb)))))))) ? 0x4u : 0u) | (((_fk == 0x00874B0Eu ? (CMP_EQ(_fa, _fb)) : (_fk == 0x78DD5527u ? (CMP_EQ(_fa, _fb)) : (_fk == 0x540E384Au ? (TEST_Z(_fa, _fb)) : (_fk == 0xFE83AF34u ? ((_fa == 0)) : (_fk == 0xB6C1447Bu ? (CMP_EQ(_fa, _fb)) : CMP_EQ(_fa, _fb))))))) ? 0x40u : 0u) | (((_fk == 0x00874B0Eu ? ((((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0)) : (_fk == 0x78DD5527u ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fk == 0x540E384Au ? (TEST_S(_fas, _fbs)) : (_fk == 0xFE83AF34u ? ((_fas < 0)) : (_fk == 0xB6C1447Bu ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0))))))) ? 0x80u : 0u) | (((_fk == 0x00874B0Eu ? ((((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0)) : (_fk == 0x78DD5527u ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (_fk == 0x540E384Au ? (0) : (_fk == 0xFE83AF34u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (_fk == 0xB6C1447Bu ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0))))))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D47Fu); RECOMP_ABI_CALL(0x00046870u, sub_00046870); /* call 0x00046870 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D47F: ;
    _fa = (uint32_t)(MEM8(esi)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp MEM8(esi), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x6D62626Au; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0004D4B3; /* jne: not equal / not zero */

loc_0004D484: ;
    eax = MEM32(esi + 0x74);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    edi = MEM32(esi + 8);
    if (CMP_EQ(_fa, _fb)) goto loc_0004D4B3; /* je: equal / zero */

loc_0004D48E: ;
    eax = MEM32(edi + 0x74);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D4B3; /* je: equal / zero */

loc_0004D495: ;
    PUSH32(esp, esi);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D49Bu); RECOMP_ABI_CALL(0x000496E0u, sub_000496E0); /* call 0x000496E0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D49B: ;
    PUSH32(esp, 1);
    PUSH32(esp, edi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D4A3u); RECOMP_ABI_CALL(0x00041120u, sub_00041120); /* call 0x00041120 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D4A3: ;
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D4B3; /* je: equal / zero */

loc_0004D4AA: ;
    PUSH32(esp, edi);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D4B0u); RECOMP_ABI_CALL(0x000496E0u, sub_000496E0); /* call 0x000496E0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D4B0: ;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD9300E98u; /* flag setter tag */

loc_0004D4B3: ;
    edi = 1;
    PUSH32(esp, edi);
    PUSH32(esp, 0xFF00FF00u);
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0x11A6DAE2u ? (CMP_B(_fa, _fb)) : (_fk == 0xD9300E98u ? (_cf) : CMP_B(_fa, _fb)))) ? 0x1u : 0u) | (((_fk == 0x11A6DAE2u ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0xD9300E98u ? (RECOMP_PARITY8(_fa)) : RECOMP_PARITY8((_fa) - (_fb))))) ? 0x4u : 0u) | (((_fk == 0x11A6DAE2u ? (CMP_EQ(_fa, _fb)) : (_fk == 0xD9300E98u ? ((_fa == 0)) : CMP_EQ(_fa, _fb)))) ? 0x40u : 0u) | (((_fk == 0x11A6DAE2u ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fk == 0xD9300E98u ? ((_fas < 0)) : (((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0)))) ? 0x80u : 0u) | (((_fk == 0x11A6DAE2u ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (_fk == 0xD9300E98u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0)))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D4C3u); RECOMP_ABI_CALL(0x000343A0u, sub_000343A0); /* call 0x000343A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D4C3: ;
    PUSH32(esp, 4);
    PUSH32(esp, 0);
    PUSH32(esp, 0x35);
    PUSH32(esp, esi);
    MEM16(esi + 0x1FF0) = LO16(edi);
    MEM16(esi + 0x1C70) = LO16(edi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D4DDu); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D4DD: ;
    PUSH32(esp, 4);
    PUSH32(esp, 0);
    PUSH32(esp, 0x3C);
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D4E9u); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D4E9: ;
    SET_LO8(edx, MEM8(esi + 0xD6));
    SET_LO8(ecx, MEM8(esi + 0xA1));
    SET_LO8(eax, MEM8(esi + 0xBC));
    PUSH32(esp, 4);
    PUSH32(esp, 0);
    _cf = 0; /* logical op clears CF */
    SET_LO8(edx, LO8(edx) | LO8(ecx));
    _fa = (uint32_t)(LO8(edx)) & 0xFFu; _fas = (int32_t)(int8_t)(_fa); /* or result */
    _fk = 0x544C5D2Du; /* flag setter tag */
    PUSH32(esp, edi);
    PUSH32(esp, esi);
    MEM8(esi + 0xE8) = LO8(edx);
    MEM8(esi + 0x103) = LO8(eax);
    g_eflags = (g_eflags & ~0x8C5u) | ((0) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((0 != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D514u); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D514: ;
    PUSH32(esp, 4);
    PUSH32(esp, 0);
    PUSH32(esp, 0x36);
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D520u); RECOMP_ABI_CALL(0x0003EE30u, sub_0003EE30); /* call 0x0003EE30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D520: ;
    edi = esi + 0x12E;
    eax = esi + 0xA0;
    _fb = (uint32_t)(0x48) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x48)) >> 32) & 1);
    esp = esp + 0x48;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x23A7974Cu; /* flag setter tag */
    ecx = esi + 0x175;
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(edi) < (uint32_t)(eax));
    edi = edi - eax;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0x341032E2u; /* flag setter tag */
    ebp = 0x47;
    /* nop */

loc_0004D540: ;
    SET_LO8(edx, MEM8(eax));
    _fa = (uint32_t)(MEM8(edi + eax)) & 0xFFu; _fb = (uint32_t)(LO8(edx)) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp MEM8(edi + eax), LO8(edx) (8-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xA4CC1ECAu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D555; /* je: equal / zero */

loc_0004D547: ;
    _fa = (uint32_t)(LO8(edx)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(edx), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xA15ADC08u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0004D550; /* je: equal / zero */

loc_0004D54B: ;
    MEM8(ecx) = 1;
    goto loc_0004D558;

loc_0004D550: ;
    MEM8(ecx) = 0xFF;
    goto loc_0004D558;

loc_0004D555: ;
    MEM8(ecx) = 0;

loc_0004D558: ;
    SET_LO8(edx, MEM8(eax));
    ecx++;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x53735CF0u; /* flag setter tag */
    MEM8(edi + eax) = LO8(edx);
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x86F8765Eu; /* flag setter tag */
    ebp--;
    _fa = (uint32_t)(ebp) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0xED6EB8E0u; /* flag setter tag */
    if ((_fa != 0)) goto loc_0004D540; /* jne: not equal / not zero */

loc_0004D562: ;
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x3E0F5DA7u; /* flag setter tag */
    PUSH32(esp, edi);
    PUSH32(esp, esi);
    MEM16(esi + 0x1C74) = LO16(edi);
    g_eflags = (g_eflags & ~0x8C5u) | ((0) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((0 != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D572u); RECOMP_ABI_CALL(0x00040670u, sub_00040670); /* call 0x00040670 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D572: ;
    PUSH32(esp, 1);
    PUSH32(esp, esi);
    MEM16(esi + 0x1FF4) = LO16(edi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D581u); RECOMP_ABI_CALL(0x00040670u, sub_00040670); /* call 0x00040670 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D581: ;
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D587u); RECOMP_ABI_CALL(0x0003DAB0u, sub_0003DAB0); /* call 0x0003DAB0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D587: ;
    PUSH32(esp, 4);
    PUSH32(esp, 0xFFFF0000u);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D593u); RECOMP_ABI_CALL(0x000343A0u, sub_000343A0); /* call 0x000343A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D593: ;
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D599u); RECOMP_ABI_CALL(0x00044CC0u, sub_00044CC0); /* call 0x00044CC0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D599: ;
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D59Fu); RECOMP_ABI_CALL(0x00045BA0u, sub_00045BA0); /* call 0x00045BA0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D59F: ;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D5A4u); RECOMP_ABI_CALL(0x00046280u, sub_00046280); /* call 0x00046280 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D5A4: ;
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D5AAu); RECOMP_ABI_CALL(0x000444F0u, sub_000444F0); /* call 0x000444F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D5AA: ;
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D5B0u); RECOMP_ABI_CALL(0x00048E60u, sub_00048E60); /* call 0x00048E60 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D5B0: ;
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D5B6u); RECOMP_ABI_CALL(0x00041FD0u, sub_00041FD0); /* call 0x00041FD0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D5B6: ;
    PUSH32(esp, esi);
    MEM32(esi + 0x15B0) = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D5C2u); RECOMP_ABI_CALL(0x00047140u, sub_00047140); /* call 0x00047140 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D5C2: ;
    PUSH32(esp, 1);
    PUSH32(esp, 0xFF00FF00u);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D5CEu); RECOMP_ABI_CALL(0x000343A0u, sub_000343A0); /* call 0x000343A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D5CE: ;
    PUSH32(esp, esi);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D5D4u); RECOMP_ABI_CALL(0x000477A0u, sub_000477A0); /* call 0x000477A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D5D4: ;
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x40)) >> 32) & 1);
    esp = esp + 0x40;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x67000AC6u; /* flag setter tag */
    PUSH32(esp, 3);
    PUSH32(esp, 0xFFFFFF00u);
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0004D5E3u); RECOMP_ABI_CALL(0x000343A0u, sub_000343A0); /* call 0x000343A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0004D5E3: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xD0F44B25u; /* flag setter tag */

loc_0004D5E6: ;
    POP32(esp, ebp);

loc_0004D5E7: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0x11A6DAE2u ? (CMP_B(_fa, _fb)) : (_fk == 0x00874B0Eu ? (CMP_B(_fa, _fb)) : (_fk == 0xD0F44B25u ? (_cf) : CMP_B(_fa, _fb))))) ? 0x1u : 0u) | (((_fk == 0x11A6DAE2u ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0x00874B0Eu ? (RECOMP_PARITY8((_fa) - (_fb))) : (_fk == 0xD0F44B25u ? (RECOMP_PARITY8(_fa)) : RECOMP_PARITY8((_fa) - (_fb)))))) ? 0x4u : 0u) | (((_fk == 0x11A6DAE2u ? (CMP_EQ(_fa, _fb)) : (_fk == 0x00874B0Eu ? (CMP_EQ(_fa, _fb)) : (_fk == 0xD0F44B25u ? ((_fa == 0)) : CMP_EQ(_fa, _fb))))) ? 0x40u : 0u) | (((_fk == 0x11A6DAE2u ? ((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) : (_fk == 0x00874B0Eu ? ((((uint8_t)((uint8_t)(_fas) - (uint8_t)(_fbs)) >> 7) != 0)) : (_fk == 0xD0F44B25u ? ((_fas < 0)) : (((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0))))) ? 0x80u : 0u) | (((_fk == 0x11A6DAE2u ? ((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) : (_fk == 0x00874B0Eu ? ((((uint8_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 7) != 0)) : (_fk == 0xD0F44B25u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0))))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

/**
 * sub_0004D610
 * Original: 0x0004D610 - 0x0004D901 (753 bytes, 221 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
