/**
 * LTO-enabled extraction of sub_00069370 from recomp_0001.c.
 * Allows cross-TU inlining of NEON sub_00011B60 inside its 3 matrix/cloth loops.
 */
#define RECOMP_GENERATED_CODE 1
#include "recomp_funcs.h"
#include "sc2_fast_mem.h"
#include "x87_math.h"

void sc2_fast_sub_00069370(void)
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

loc_00069370: ;
    PUSH32(esp, ebp);
    ebp = esp;
    g_ebp = ebp; /* publish frame for frameless callees */
    g_seh_ebp = ebp;
    _cf = 0; /* logical op clears CF */
    esp = esp & 0xFFFFFFF0u;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0xDA254061u; /* flag setter tag */
    _fb = (uint32_t)(0x6A4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0x6A4));
    esp = esp - 0x6A4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0x245C99D7u; /* flag setter tag */
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = ecx;
    eax = esi + 0x30;
    PUSH32(esp, edi);
    PUSH32(esp, eax);
    MEM32(esp + 0x28) = esi;
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa + _fb) ^ _fb) & ((_fa + _fb) ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0006938Eu); RECOMP_ABI_CALL(0x00063100u, sub_00063100); /* call 0x00063100 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0006938E: ;
    ecx = MEM32(ebp + 0xC);
    edi = MEM32(ebp + 8);
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    MEM32(0x3CA0F0) = ecx;
    edx = MEM32(edi + 0x78);
    eax = MEM32(edi + 0x70);
    PUSH32(esp, 0);
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000693ADu); RECOMP_ABI_CALL(0x000E9290u, sub_000E9290); /* call 0x000E9290 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000693AD: ;
    MEMF(0x3CA0F4) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esi + 0x18);
    ecx = eax;
    if (((6) & 31u)) _cf = (int)(((uint32_t)(ecx) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    ecx = (uint32_t)(ecx) << ((6) & 31u);
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x29DB4FF6u; /* flag setter tag */
    MEM32(0x3CA000) = eax;
    eax = MEM32(esi + 0x20);
    _fb = (uint32_t)(edi) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(edi)) >> 32) & 1);
    ecx = ecx + edi;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x92F365B5u; /* flag setter tag */
    _fb = (uint32_t)(0x18) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0x18)) >> 32) & 1);
    esp = esp + 0x18;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xBD67F1D5u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    ebx = edi + 0x5C0;
    MEM32(esp + 0x28) = ebx;
    MEM32(esp + 0x20) = ecx;
    MEM32(esp + 0x18) = 0;
    if (CMP_LE(_fas, _fbs)) goto loc_0006947E; /* jle: less or equal (signed <=) */

loc_000693E6: ;
    esi = 0x3C9F74;
    MEM32(esp + 0x1C) = esi;
    goto loc_000693F5;

loc_000693F1: ;
    esi = MEM32(esp + 0x1C);

loc_000693F5: ;
    edi = esi + 4;
    PUSH32(esp, edi);
    eax = esi + -4;
    PUSH32(esp, esi);
    PUSH32(esp, eax);
    ecx = ebx;
    MEM32(esp + 0x38) = eax;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00069409u); RECOMP_ABI_CALL(0x00011C20u, sub_00011C20); /* call 0x00011C20 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00069409: ;
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(edi)); /* fdiv dword ptr [edi] */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0xC));
    esp = esp - 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0xD4620F2Du; /* flag setter tag */
    ecx = MEM32(esp + 0x38);
    MEMF(esp + 8) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esi)); /* fdiv dword ptr [esi] */
    MEMF(esp + 4) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(ecx)); /* fdiv dword ptr [ecx] */
    ecx = ebx;
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    PUSH32(esp, ebx);
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa + _fb) ^ _fb) & ((_fa + _fb) ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0006943Bu); RECOMP_ABI_CALL(0x00011B60u, sub_00011B60); /* call 0x00011B60 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0006943B: ;
    eax = MEM32(esp + 0x20);
    edx = MEM32(esp + 0x1C);
    edi = MEM32(esp + 0x20);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0x40)) >> 32) & 1);
    eax = eax + 0x40;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x1F856A41u; /* flag setter tag */
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edx) + (uint64_t)(0x10)) >> 32) & 1);
    edx = edx + 0x10;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xEF4FC9B8u; /* flag setter tag */
    esi = ebx;
    MEM32(esp + 0x20) = eax;
    eax = MEM32(esp + 0x18);
    ecx = 0x10;
    MEM32(esp + 0x1C) = edx;
    edx = MEM32(esp + 0x24);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ebx) + (uint64_t)(0x40)) >> 32) & 1);
    ebx = ebx + 0x40;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xF1EBBFE0u; /* flag setter tag */
    if (!g_df) { uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi); uint32_t _n = ecx * 4;
      if (_d + _n <= _s || _s + _n <= _d) memcpy(_d, _s, _n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4) = MEM32(esi + _i*4); }
      esi += ecx * 4; edi += ecx * 4; }
    else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi - _i*4) = MEM32(esi - _i*4); esi -= ecx * 4; edi -= ecx * 4; }
    ecx = 0; /* rep movsd */
    ecx = MEM32(edx + 0x20);
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x86F8765Eu; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, ecx (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xF0566D7Du; /* flag setter tag */
    MEM32(esp + 0x18) = eax;
    if (CMP_L(_fas, _fbs)) goto loc_000693F1; /* jl: less (signed <) */

loc_00069479: ;
    edi = MEM32(ebp + 8);
    esi = edx;

loc_0006947E: ;
    eax = MEM32(ebp + 0x10);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0006949A; /* je: equal / zero */

loc_00069485: ;
    esi = edi + 0xF80;
    ecx = 0x1A0;
    edi = esp + 0x30;
    if (!g_df) { uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi); uint32_t _n = ecx * 4;
      if (_d + _n <= _s || _s + _n <= _d) memcpy(_d, _s, _n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4) = MEM32(esi + _i*4); }
      esi += ecx * 4; edi += ecx * 4; }
    else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi - _i*4) = MEM32(esi - _i*4); esi -= ecx * 4; edi -= ecx * 4; }
    ecx = 0; /* rep movsd */
    esi = MEM32(esp + 0x24);

loc_0006949A: ;
    edi = MEM32(esi + 8);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x8C6ED7C1u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_000694BE; /* je: equal / zero */

loc_000694A1: ;
    ebx = MEM32(ebp + 8);
    _fb = (uint32_t)(0x80) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ebx) + (uint64_t)(0x80)) >> 32) & 1);
    ebx = ebx + 0x80;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x967E68D9u; /* flag setter tag */
    /* nop */

loc_000694B0: ;
    eax = MEM32(edi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebx);
    ecx = edi;
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0x967E68D9u ? (_cf) : CMP_B(_fa, _fb))) ? 0x1u : 0u) | (((_fk == 0x967E68D9u ? (RECOMP_PARITY8(_fa)) : RECOMP_PARITY8((_fa) - (_fb)))) ? 0x4u : 0u) | (((_fk == 0x967E68D9u ? ((_fa == 0)) : CMP_EQ(_fa, _fb))) ? 0x40u : 0u) | (((_fk == 0x967E68D9u ? ((_fas < 0)) : (((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0))) ? 0x80u : 0u) | (((_fk == 0x967E68D9u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; g_seh_ebp = ebp; { uint32_t _icall_target = MEM32(eax); PUSH32(esp, 0x000694B7u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000694B5u); } /* indirect call */
    }
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000694B7: ;
    edi = MEM32(edi + 0x24);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x8C6ED7C1u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_000694B0; /* jne: not equal / not zero */

loc_000694BE: ;
    edi = MEM32(esi + 0x10);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x8C6ED7C1u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_000694DE; /* je: equal / zero */

loc_000694C5: ;
    ebx = MEM32(ebp + 8);
    _fb = (uint32_t)(0x80) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ebx) + (uint64_t)(0x80)) >> 32) & 1);
    ebx = ebx + 0x80;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x967E68D9u; /* flag setter tag */
    edi = edi;

loc_000694D0: ;
    edx = MEM32(edi);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ebx);
    ecx = edi;
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0x967E68D9u ? (_cf) : CMP_B(_fa, _fb))) ? 0x1u : 0u) | (((_fk == 0x967E68D9u ? (RECOMP_PARITY8(_fa)) : RECOMP_PARITY8((_fa) - (_fb)))) ? 0x4u : 0u) | (((_fk == 0x967E68D9u ? ((_fa == 0)) : CMP_EQ(_fa, _fb))) ? 0x40u : 0u) | (((_fk == 0x967E68D9u ? ((_fas < 0)) : (((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0))) ? 0x80u : 0u) | (((_fk == 0x967E68D9u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; g_seh_ebp = ebp; { uint32_t _icall_target = MEM32(edx); PUSH32(esp, 0x000694D7u); RECOMP_ICALL_SAFE_AT(_icall_target, _icall_esp, 0x000694D5u); } /* indirect call */
    }
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000694D7: ;
    edi = MEM32(edi + 0x24);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x8C6ED7C1u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_000694D0; /* jne: not equal / not zero */

loc_000694DE: ;
    eax = MEM32(ebp + 0x10);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_000694FD; /* je: equal / zero */

loc_000694E5: ;
    edi = MEM32(ebp + 8);
    _fb = (uint32_t)(0xF80) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(0xF80)) >> 32) & 1);
    edi = edi + 0xF80;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x62B8BB29u; /* flag setter tag */
    ecx = 0x1A0;
    esi = esp + 0x30;
    if (!g_df) { uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi); uint32_t _n = ecx * 4;
      if (_d + _n <= _s || _s + _n <= _d) memcpy(_d, _s, _n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4) = MEM32(esi + _i*4); }
      esi += ecx * 4; edi += ecx * 4; }
    else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi - _i*4) = MEM32(esi - _i*4); esi -= ecx * 4; edi -= ecx * 4; }
    ecx = 0; /* rep movsd */
    esi = MEM32(esp + 0x24);

loc_000694FD: ;
    edi = MEM32(esi + 0xC);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x8C6ED7C1u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0006952F; /* je: equal / zero */

loc_00069504: ;
    eax = MEM32(ebp + 8);
    ebx = MEM32(edi + 0x40);
    PUSH32(esp, 1);
    _fb = (uint32_t)(0x80) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0x80)) >> 32) & 1);
    eax = eax + 0x80;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x37419572u; /* flag setter tag */
    PUSH32(esp, eax);
    ecx = edi;
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00069519u); RECOMP_ABI_CALL(0x00067D70u, sub_00067D70); /* call 0x00067D70 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00069519: ;
    eax = MEM32(ebp + 8);
    _fb = (uint32_t)(0x80) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0x80)) >> 32) & 1);
    eax = eax + 0x80;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x37419572u; /* flag setter tag */
    PUSH32(esp, eax);
    ecx = edi;
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00069529u); RECOMP_ABI_CALL(0x000843D0u, sub_000843D0); /* call 0x000843D0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00069529: ;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x711257E0u; /* flag setter tag */
    edi = ebx;
    if (CMP_NE(_fa, _fb)) goto loc_00069504; /* jne: not equal / not zero */

loc_0006952F: ;
    edi = MEM32(esi + 0x14);
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x8C6ED7C1u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00069565; /* je: equal / zero */

loc_00069536: ;
    eax = MEM32(ebp + 8);
    _fb = (uint32_t)(0x80) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0x80)) >> 32) & 1);
    eax = eax + 0x80;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x37419572u; /* flag setter tag */
    MEM32(esp + 0x18) = eax;

loc_00069542: ;
    ecx = MEM32(esp + 0x18);
    ebx = MEM32(edi + 0x40);
    PUSH32(esp, 1);
    PUSH32(esp, ecx);
    ecx = edi;
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0x37419572u ? (_cf) : CMP_B(_fa, _fb))) ? 0x1u : 0u) | (((_fk == 0x37419572u ? (RECOMP_PARITY8(_fa)) : RECOMP_PARITY8((_fa) - (_fb)))) ? 0x4u : 0u) | (((_fk == 0x37419572u ? ((_fa == 0)) : CMP_EQ(_fa, _fb))) ? 0x40u : 0u) | (((_fk == 0x37419572u ? ((_fas < 0)) : (((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0))) ? 0x80u : 0u) | (((_fk == 0x37419572u ? ((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) : (((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00069553u); RECOMP_ABI_CALL(0x00067D70u, sub_00067D70); /* call 0x00067D70 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00069553: ;
    edx = MEM32(esp + 0x18);
    PUSH32(esp, edx);
    ecx = edi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0006955Fu); RECOMP_ABI_CALL(0x000843D0u, sub_000843D0); /* call 0x000843D0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0006955F: ;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x711257E0u; /* flag setter tag */
    edi = ebx;
    if (CMP_NE(_fa, _fb)) goto loc_00069542; /* jne: not equal / not zero */

loc_00069565: ;
    ebx = MEM32(esi + 0x18);
    edx = MEM32(ebp + 8);
    ecx = MEM32(ebp + 0x10);
    edi = MEM32(esp + 0x28);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(ebx) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    ebx = (uint32_t)(ebx) << ((6) & 31u);
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0xBF67C937u; /* flag setter tag */
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x504457D4u; /* flag setter tag */
    _fb = (uint32_t)(edx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ebx) + (uint64_t)(edx)) >> 32) & 1);
    ebx = ebx + edx;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xAB8DB124u; /* flag setter tag */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, eax (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xD17734FBu; /* flag setter tag */
    MEM32(esp + 0x20) = edi;
    MEM32(esp + 0x18) = eax;
    if (CMP_NE(_fa, _fb)) goto loc_00069609; /* jne: not equal / not zero */

loc_00069589: ;
    ecx = MEM32(esp + 0x24);
    _fa = (uint32_t)(MEM32(ecx + 0x20)) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ecx + 0x20), eax (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xE76AF583u; /* flag setter tag */
    if (CMP_LE(_fas, _fbs)) goto loc_000695F0; /* jle: less or equal (signed <=) */

loc_00069592: ;
    eax = 0x3C9F74;
    MEM32(esp + 0x1C) = eax;
    goto loc_000695A5;

loc_0006959D: ;
    edi = MEM32(esp + 0x20);
    eax = MEM32(esp + 0x1C);

loc_000695A5: ;
    edx = MEM32(eax + 4);
    ecx = MEM32(eax);
    PUSH32(esp, edx);
    edx = MEM32(eax + -4);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    PUSH32(esp, edi);
    ecx = edi;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000695B8u); RECOMP_ABI_CALL(0x00011B60u, sub_00011B60); /* call 0x00011B60 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000695B8: ;
    eax = MEM32(esp + 0x20);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0x40)) >> 32) & 1);
    eax = eax + 0x40;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x1F856A41u; /* flag setter tag */
    esi = edi;
    edi = ebx;
    ecx = 0x10;
    if (!g_df) { uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi); uint32_t _n = ecx * 4;
      if (_d + _n <= _s || _s + _n <= _d) memcpy(_d, _s, _n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4) = MEM32(esi + _i*4); }
      esi += ecx * 4; edi += ecx * 4; }
    else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi - _i*4) = MEM32(esi - _i*4); esi -= ecx * 4; edi -= ecx * 4; }
    ecx = 0; /* rep movsd */
    esi = MEM32(esp + 0x1C);
    ecx = MEM32(esp + 0x24);
    edx = MEM32(ecx + 0x20);
    MEM32(esp + 0x20) = eax;
    eax = MEM32(esp + 0x18);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ebx) + (uint64_t)(0x40)) >> 32) & 1);
    ebx = ebx + 0x40;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xF1EBBFE0u; /* flag setter tag */
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x86F8765Eu; /* flag setter tag */
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x10)) >> 32) & 1);
    esi = esi + 0x10;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x5BF233CEu; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edx (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x3F7F8199u; /* flag setter tag */
    MEM32(esp + 0x18) = eax;
    MEM32(esp + 0x1C) = esi;
    if (CMP_L(_fas, _fbs)) goto loc_0006959D; /* jl: less (signed <) */

loc_000695F0: ;
    edx = MEM32(ebp + 8);
    ecx = MEM32(esp + 0x24);
    PUSH32(esp, edx);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0x40)) >> 32) & 1);
    ecx = ecx + 0x40;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x1D1EF140u; /* flag setter tag */
    g_eflags = (g_eflags & ~0x8C5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa - _fb) ^ _fa) & (_fb ^ _fa)) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00069600u); RECOMP_ABI_CALL(0x000688E0u, sub_000688E0); /* call 0x000688E0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00069600: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | (((_fk == 0x00000000u ? ((g_eflags & 1u) /* eflags */) : CMP_B(_fa, _fb))) ? 0x1u : 0u) | (((_fk == 0x00000000u ? ((g_eflags & 4u) /* eflags */) : RECOMP_PARITY8((_fa) - (_fb)))) ? 0x4u : 0u) | (((_fk == 0x00000000u ? ((g_eflags & 0x40u) /* eflags */) : CMP_EQ(_fa, _fb))) ? 0x40u : 0u) | (((_fk == 0x00000000u ? (((g_eflags >> 7) & 1u) /* eflags */) : (((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0))) ? 0x80u : 0u) | (((_fk == 0x00000000u ? (((g_eflags >> 11) & 1u) /* eflags */) : (((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0))) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 16; return; /* ret 12 */

loc_00069609: ;
    edx = MEM32(esp + 0x24);
    _fa = (uint32_t)(MEM32(edx + 0x20)) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(edx + 0x20), eax (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xFB9F7107u; /* flag setter tag */
    if (CMP_LE(_fas, _fbs)) goto loc_00069600; /* jle: less or equal (signed <=) */

loc_00069612: ;
    eax = 0x3C9F74;
    MEM32(esp + 0x1C) = eax;
    goto loc_00069625;

loc_0006961D: ;
    edi = MEM32(esp + 0x20);
    eax = MEM32(esp + 0x1C);

loc_00069625: ;
    ecx = MEM32(eax + 4);
    edx = MEM32(eax);
    eax = MEM32(eax + -4);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    PUSH32(esp, ebx);
    ecx = ebx;
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00069638u); RECOMP_ABI_CALL(0x00011B60u, sub_00011B60); /* call 0x00011B60 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00069638: ;
    eax = MEM32(esp + 0x18);
    esi = ebx;
    ecx = 0x10;
    if (!g_df) { uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi); uint32_t _n = ecx * 4;
      if (_d + _n <= _s || _s + _n <= _d) memcpy(_d, _s, _n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4) = MEM32(esi + _i*4); }
      esi += ecx * 4; edi += ecx * 4; }
    else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi - _i*4) = MEM32(esi - _i*4); esi -= ecx * 4; edi -= ecx * 4; }
    ecx = 0; /* rep movsd */
    ecx = MEM32(esp + 0x20);
    esi = MEM32(esp + 0x1C);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(0x40)) >> 32) & 1);
    ecx = ecx + 0x40;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x1D1EF140u; /* flag setter tag */
    MEM32(esp + 0x20) = ecx;
    ecx = MEM32(esp + 0x24);
    edx = MEM32(ecx + 0x20);
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ebx) + (uint64_t)(0x40)) >> 32) & 1);
    ebx = ebx + 0x40;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xF1EBBFE0u; /* flag setter tag */
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x86F8765Eu; /* flag setter tag */
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x10)) >> 32) & 1);
    esi = esi + 0x10;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x5BF233CEu; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, edx (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x3F7F8199u; /* flag setter tag */
    MEM32(esp + 0x18) = eax;
    MEM32(esp + 0x1C) = esi;
    if (CMP_L(_fas, _fbs)) goto loc_0006961D; /* jl: less (signed <) */

loc_0006966E: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~0x8C5u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u) | ((RECOMP_PARITY8((_fa) - (_fb))) ? 0x4u : 0u) | ((CMP_EQ(_fa, _fb)) ? 0x40u : 0u) | (((((uint32_t)((uint32_t)(_fas) - (uint32_t)(_fbs)) >> 31) != 0)) ? 0x80u : 0u) | (((((uint32_t)(((_fa) ^ (_fb)) & ((_fa) ^ ((_fa) - (_fb)))) >> 31) != 0)) ? 0x800u : 0u); /* flags across call boundary */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 16; return; /* ret 12 */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}

/**
 * sub_00069680
 * Original: 0x00069680 - 0x000696DA (90 bytes, 39 insns)
 * CC: cdecl, 0 params, returns int_or_void
 * Frame: fpo_leaf
 */
