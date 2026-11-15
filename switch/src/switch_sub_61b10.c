/* LTO-enabled extraction of sub_00061B10 from recomp_0001.c for NEON cross-TU inlining. */
#define RECOMP_GENERATED_CODE 1
#include "recomp_funcs.h"
#include "sc2_fast_mem.h"
#include "x87_math.h"

void __wrap_sub_00061B10(void)
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

loc_00061B10: ;
    PUSH32(esp, ebp);
    ebp = esp;
    g_ebp = ebp; /* publish frame for frameless callees */
    g_seh_ebp = ebp;
    _cf = 0; /* logical op clears CF */
    esp = esp & 0xFFFFFFF0u;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0xDA254061u; /* flag setter tag */
    _fb = (uint32_t)(0x1B4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0x1B4));
    esp = esp - 0x1B4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0xCCC851B0u; /* flag setter tag */
    SET_LO8(ecx, MEM8(0x3BCB53));
    _fa = (uint32_t)(LO8(ecx)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(ecx), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xF68F40CDu; /* flag setter tag */
    PUSH32(esp, ebx);
    ebx = MEM32(0x38E594);
    PUSH32(esp, esi);
    PUSH32(esp, edi);
    edi = MEM32(ebx + 0x94);
    if (CMP_EQ(_fa, _fb)) goto loc_00062581; /* je: equal / zero */

loc_00061B39: ;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x8C6ED7C1u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00062581; /* je: equal / zero */

loc_00061B41: ;
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x504457D4u; /* flag setter tag */
    SET_LO8(eax, MEM8(edi + 0x10));
    _cf = 0; /* logical op clears CF */
    eax = eax & 0x1F;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0x7E9CF5F0u; /* flag setter tag */
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0xFF) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0xFF (8-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x7D0277B2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00062581; /* je: equal / zero */

loc_00061B51: ;
    edx = MEM32(edi + 0x2C);
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fb = (uint32_t)(MEM32(ebx + 0xA0)) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edx, MEM32(ebx + 0xA0) (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x69CA169Fu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_00062581; /* jne: not equal / not zero */

loc_00061B60: ;
    eax = MEM32(ebx + 0x98);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_00061B72; /* jne: not equal / not zero */

loc_00061B6A: ;
    SET_LO8(eax, MEM8(edi + 0x67));
    MEM8(ebx + 0x1E) = LO8(eax);
    goto loc_00061B78;

loc_00061B72: ;
    SET_LO8(edx, MEM8(eax + 0x67));
    MEM8(ebx + 0x1E) = LO8(edx);

loc_00061B78: ;
    _fa = (uint32_t)(LO8(ecx)) & 0xFFu; _fb = (uint32_t)(4) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test LO8(ecx), 4 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x90BC875Cu; /* flag setter tag */
    edx = MEM32(0x38E588);
    eax = SX8(LO8(ecx));
    MEM32(edx + 0x44) = eax;
    if (TEST_NZ(_fa, _fb)) goto loc_0006231E; /* jne: not equal / not zero */

loc_00061B8D: ;
    eax = MEM32(edi + 0x5C);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00061C4C; /* je: equal / zero */

loc_00061B98: ;
    eax = MEM32(ebx + 0xA4);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_00061BD4; /* jne: not equal / not zero */

loc_00061BA2: ;
    eax = MEM32(edi + 0x6C);
    ecx = MEM32(0x1916D4);
    PUSH32(esp, eax);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061BB1u); sub_00018130(); /* call 0x00018130 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061BB1: ;
    ecx = MEM32(eax);
    MEM32(0x22CBF0) = ecx;
    edx = MEM32(eax + 4);
    MEM32(0x22CBF4) = edx;
    ecx = MEM32(eax + 8);
    MEM32(0x22CBF8) = ecx;
    edx = MEM32(eax + 0xC);
    MEM32(0x22CBFC) = edx;

loc_00061BD4: ;
    eax = MEM32(edi + 0x5C);
    esi = MEM32(ebx + 0xA4);
    if (((6) & 31u)) _cf = (int)(((uint32_t)(esi) >> ((32 - (((6) & 31u))) & 31u)) & 1u);
    esi = (uint32_t)(esi) << ((6) & 31u);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0xC84A92CFu; /* flag setter tag */
    _fb = (uint32_t)(eax) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(eax)) >> 32) & 1);
    esi = esi + eax;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x7C5F3607u; /* flag setter tag */
    eax = ebx + 0x30;
    edi = eax;
    ecx = 0x10;
    MEM32(esp + 0x14) = esi;
    if (!g_df) { uint8_t *_d = (uint8_t*)XBOX_PTR(edi), *_s = (uint8_t*)XBOX_PTR(esi); uint32_t _n = ecx * 4;
      if (_d + _n <= _s || _s + _n <= _d) memcpy(_d, _s, _n);
      else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*4) = MEM32(esi + _i*4); }
      esi += ecx * 4; edi += ecx * 4; }
    else { uint32_t _i; for (_i = 0; _i < ecx; _i++) MEM32(edi - _i*4) = MEM32(esi - _i*4); esi -= ecx * 4; edi -= ecx * 4; }
    ecx = 0; /* rep movsd */
    ecx = ebx + 0x80;
    PUSH32(esp, ecx);
    PUSH32(esp, eax);
    ecx = esp + 0x48;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061C03u); sub_00012720(); /* call 0x00012720 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061C03: ;
    ecx = MEM32(esp + 0x48);
    edx = MEM32(esp + 0x40);
    eax = MEM32(esp + 0x44);
    MEM32(ebx + 0x68) = ecx;
    ecx = ebx + 0x30;
    MEM32(ebx + 0x60) = edx;
    edx = MEM32(ebx + 0x70);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    MEM32(ebx + 0x64) = eax;
    MEM32(ebx + 0x6C) = 0x3F800000;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061C2Cu); sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061C2C: ;
    eax = MEM32(ebx + 0x74);
    ecx = ebx + 0x40;
    PUSH32(esp, ecx);
    PUSH32(esp, eax);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061C39u); sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061C39: ;
    edx = MEM32(ebx + 0x78);
    ecx = ebx + 0x50;
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061C46u); sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061C46: ;
    edi = MEM32(esp + 0x14);
    goto loc_00061C51;

loc_00061C4C: ;
    eax = ebx + 0x30;
    edi = eax;

loc_00061C51: ;
    SET_LO8(eax, MEM8(ebx + 0x1E));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0006257A; /* je: equal / zero */

loc_00061C5C: ;
    fp_push(MEMF(0x1761A0)); /* fld float */
    fp_push(MEMF(ebx + 0x44)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), fp_st1()); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); fp_pop(); /* fucompp  */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x44) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x44 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0xC77E4F9Du; /* flag setter tag */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_0006257A; /* jnp: not parity */

loc_00061C72: ;
    ecx = MEM32(ebx + 0xA4);
    edx = MEM32(ebx + 0x9C);
    fp_push(MEMF(edx + ecx * 4 + 0x50)); /* fld float */
    eax = MEM32(ebx + 0x90);
    esi = MEM32(ebx + 0x10);
    MEMF(esp + 0x34) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(ebx + 0x64)); /* fld float */
    MEM32(esp + 0x6C) = eax;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x176A88)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x176a88] */
    MEM32(esp + 0x3C) = esi;
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_00061CBC; /* jne: not equal / not zero */

loc_00061CA7: ;
    fp_push(MEMF(esp + 0x34)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A8)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a8] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_0006231E; /* jnp: not parity */

loc_00061CBC: ;
    fp_push(MEMF(esi + 0x34)); /* fld float */
    PUSH32(esp, 0x43000000);
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x38)); /* fsub dword ptr [esp + 0x38] */
    PUSH32(esp, 0);
    PUSH32(esp, ecx);
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061CD3u); sub_000200B0(); /* call 0x000200B0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061CD3: ;
    eax = MEM32(ebx + 0xAC);
    MEMF(esp + 0x28) = (float)fp_top(); fp_pop(); /* fstp */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00061D1B; /* je: equal / zero */

loc_00061CE4: ;
    fp_push(MEMF(esp + 0x1C)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x17A0A8)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x17a0a8] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00061D1B; /* jp: parity */

loc_00061CF5: ;
    fp_push(MEMF(esp + 0x1C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x17A0A4)); /* fmul dword ptr [0x17a0a4] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x176294)); /* fadd dword ptr [0x176294] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x176188)); /* fmul dword ptr [0x176188] */
    if (recomp_fp_trig_in_range(fp_top())) fp_top() = sin(fp_top()); /* fsin */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x176190)); /* fadd dword ptr [0x176190] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x17A0A8)); /* fmul dword ptr [0x17a0a8] */
    goto loc_00061D1F;

loc_00061D1B: ;
    fp_push(MEMF(esp + 0x1C)); /* fld float */

loc_00061D1F: ;
    _fa = (uint32_t)(MEM32(ebx + 0xA4)) & 0xFFFFFFFFu; _fb = (uint32_t)(0x15) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ebx + 0xA4), 0x15 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x49227E1Au; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00061D32; /* je: equal / zero */

loc_00061D28: ;
    eax = MEM32(esp + 0x1C);
    MEM32(esp + 0x10) = eax;
    goto loc_00061D82;

loc_00061D32: ;
    ecx = esp + 0x50;
    fp_pop(); /* fstp st(0) */
    PUSH32(esp, ecx);
    PUSH32(esp, esi);
    ecx = esp + 0x48;
    MEM32(esp + 0x58) = 0x3F0CCCCD;
    MEM32(esp + 0x5C) = 0;
    MEM32(esp + 0x60) = 0;
    MEM32(esp + 0x64) = 0x3F800000;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061D63u); sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061D63: ;
    fp_push(MEMF(esp + 0x44)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x34)); /* fsub dword ptr [esp + 0x34] */
    PUSH32(esp, 0x43000000);
    PUSH32(esp, 0);
    PUSH32(esp, ecx);
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061D7Bu); sub_000200B0(); /* call 0x000200B0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061D7B: ;
    MEMF(esp + 0x1C) = (float)fp_top(); /* fst */
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */

loc_00061D82: ;
    fp_push(MEMF(0x1761A8)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(0x22CBF4)); /* fdiv dword ptr [0x22cbf4] */
    MEMF(esp + 0x14) = (float)fp_top(); /* fst */
    fp_top() = RECOMP_FP_PC(fp_top() * fp_st1()); /* fmul st(1) */
    MEMF(esp + 0x38) = (float)fp_top(); fp_pop(); /* fstp */
    fp_pop(); /* fstp st(0) */
    fp_push(MEMF(esp + 0x14)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x1C)); /* fmul dword ptr [esp + 0x1c] */
    MEMF(esp + 0x50) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esi + 0x34)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x34)); /* fsub dword ptr [esp + 0x34] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x17A0A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x17a0a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if ((!RECOMP_PARITY8((_fa) & (_fb)))) goto loc_0006231E; /* jnp: not parity */

loc_00061DBE: ;
    fp_push(MEMF(esp + 0x38)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x176280)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x176280] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_00061DDD; /* jne: not equal / not zero */

loc_00061DCF: ;
    edx = MEM32(0x1916D0);
    fp_push(MEMF(edx + 0x164)); /* fld float */
    goto loc_00061DF8;

loc_00061DDD: ;
    eax = MEM32(0x1916D0);
    fp_push(MEMF(eax + 0x168)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1761AC)); /* fmul dword ptr [0x1761ac] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x38)); /* fmul dword ptr [esp + 0x38] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(eax + 0x160)); /* fadd dword ptr [eax + 0x160] */

loc_00061DF8: ;
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esp + 0x50)); /* fdiv dword ptr [esp + 0x50] */
    fp_top() = RECOMP_FP_PC(MEMF(0x176190) - fp_top()); /* fsubr dword ptr [0x176190] */
    MEMF(esp + 0x38) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x38)); /* fsub dword ptr [esp + 0x38] */
    MEMF(esp + 0x14) = (float)fp_top(); fp_pop(); /* fstp */
    fp_push(MEMF(esp + 0x10)); /* fld float */
    eax = MEM32(esp + 0x14);
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x176190)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x176190] */
    MEM32(esp + 0x58) = eax;
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00061E60; /* jp: parity */

loc_00061E2D: ;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761AC)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761ac] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00061E48; /* jp: parity */

loc_00061E3E: ;
    MEM32(esp + 0x5C) = 0x40000000;
    goto loc_00061E6E;

loc_00061E48: ;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(0x1761AC)); /* fsub dword ptr [0x1761ac] */
    fp_top() = RECOMP_FP_PC(fp_top() + fp_top()); /* fadd st(0), st(0) */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x14)); /* fmul dword ptr [esp + 0x14] */
    fp_top() = RECOMP_FP_PC(MEMF(0x176280) - fp_top()); /* fsubr dword ptr [0x176280] */
    goto loc_00061E6A;

loc_00061E60: ;
    fp_push(MEMF(esp + 0x38)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x176190)); /* fadd dword ptr [0x176190] */

loc_00061E6A: ;
    MEMF(esp + 0x5C) = (float)fp_top(); fp_pop(); /* fstp */

loc_00061E6E: ;
    ecx = MEM32(ebx + 0xB4);
    PUSH32(esp, ecx);
    ecx = MEM32(0x1916D4);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061E80u); sub_00060EE0(); /* call 0x00060EE0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061E80: ;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_00061F32; /* jne: not equal / not zero */

loc_00061E91: ;
    edx = esp + 0x20;
    PUSH32(esp, edx);
    PUSH32(esp, esi);
    ecx = edx;
    MEM32(esp + 0x28) = 0x3F800000;
    MEM32(esp + 0x2C) = 0;
    MEM32(esp + 0x30) = 0;
    MEM32(esp + 0x34) = 0x3F800000;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061EBEu); sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061EBE: ;
    fp_push(MEMF(esp + 0x50)); /* fld float */
    PUSH32(esp, 0x22CBF0);
    fp_top() = RECOMP_FP_PC(fp_top() + fp_top()); /* fadd st(0), st(0) */
    PUSH32(esp, ecx);
    ecx = esp + 0x48;
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    MEM32(esp + 0x34) = 0x3F800000;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061EDEu); sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061EDE: ;
    eax = esp + 0x40;
    PUSH32(esp, eax);
    ecx = esp + 0x24;
    PUSH32(esp, ecx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061EEDu); sub_00012550(); /* call 0x00012550 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061EED: ;
    eax = MEM32(0x38E520);
    edx = esp + 0x20;
    MEM32(esp + 0x2C) = 0x3F800000;
    ecx = MEM32(eax + 0x69C);
    PUSH32(esp, edx);
    PUSH32(esp, ecx);
    ecx = edx;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061F0Du); sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061F0D: ;
    fp_push(MEMF(esp + 0x2C)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x17A0A8)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x17a0a8] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00061F32; /* jp: parity */

loc_00061F1E: ;
    fp_push(MEMF(esp + 0x38)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x176198)); /* fmul dword ptr [0x176198] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x176190)); /* fadd dword ptr [0x176190] */
    MEMF(esp + 0x5C) = (float)fp_top(); fp_pop(); /* fstp */

loc_00061F32: ;
    eax = ZX16(MEM16(ebx + 8));
    edx = MEM32(ebx + 0xC);
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(2)) >> 32) & 1);
    eax = eax + 2;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xB2C38A2Eu; /* flag setter tag */
    PUSH32(esp, edi);
    ecx = esp + 0x114;
    MEM32(esp + 0x3C) = edx;
    MEM32(esp + 0x18) = eax;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061F51u); sub_000110D0(); /* call 0x000110D0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061F51: ;
    eax = MEM32(esp + 0x50);
    PUSH32(esp, 0x22CBF0);
    PUSH32(esp, eax);
    ecx = esp + 0xF8;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061F67u); sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061F67: ;
    PUSH32(esp, 0x22CBF0);
    ecx = esp + 0x114;
    PUSH32(esp, ecx);
    ecx = esp + 0x108;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00061F80u); sub_000127F0(); /* call 0x000127F0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00061F80: ;
    _fa = (uint32_t)(MEM8(0x38F909)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp MEM8(0x38F909), 1 (8-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x0675F32Eu; /* flag setter tag */
    if (CMP_A(_fa, _fb)) goto loc_00061F96; /* ja: above (unsigned >) */

loc_00061F89: ;
    SET_LO8(eax, MEM8(0x38F90B));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00062341; /* je: equal / zero */

loc_00061F96: ;
    edx = esp + 0xF0;
    PUSH32(esp, edx);
    _cf = 0; /* xor clears CF */
    esi = 0; /* xor self */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x9F87D19Fu; /* flag setter tag */
    PUSH32(esp, 0x3F666666);
    ecx = esp + 0xE8;
    MEM32(esp + 0x78) = 0;
    MEM32(esp + 0x7C) = 0;
    MEM32(esp + 0x80) = 0xBF800000u;
    MEM32(esp + 0x84) = 0x3F800000;
    MEM32(esp + 0x88) = 0xBF800000u;
    MEM32(esp + 0x8C) = 0;
    MEM32(esp + 0x90) = 0;
    MEM32(esp + 0x94) = 0x3F800000;
    MEM32(esp + 0x98) = 0x3F800000;
    MEM32(esp + 0x9C) = 0;
    MEM32(esp + 0xA0) = 0;
    MEM32(esp + 0xA4) = 0x3F800000;
    MEM32(esp + 0xA8) = 0;
    MEM32(esp + 0xAC) = 0;
    MEM32(esp + 0xB0) = 0x3F800000;
    MEM32(esp + 0xB4) = 0x3F800000;
    MEM32(esp + 0xB8) = 0;
    MEM32(esp + 0xBC) = 0;
    MEM32(esp + 0xC0) = 0xBF800000u;
    MEM32(esp + 0xC4) = 0x3F800000;
    MEM32(esp + 0xC8) = 0;
    MEM32(esp + 0xCC) = 0x3F800000;
    MEM32(esp + 0xD0) = 0;
    MEM32(esp + 0xD4) = 0x3F800000;
    MEM32(esp + 0xD8) = 0;
    MEM32(esp + 0xDC) = 0xBF800000u;
    MEM32(esp + 0xE0) = 0;
    MEM32(esp + 0xE4) = 0x3F800000;
    MEM32(esp + 0x18) = esi;
    g_eflags = (g_eflags & ~1u) | ((0) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000620E3u); sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000620E3: ;
    SET_LO16(eax, MEM16(0x39895E));
    _fa = (uint32_t)(LO16(eax)) & 0xFFFFu; _fb = (uint32_t)(9) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp LO16(eax), 9 (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xA4A51D36u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_000620FA; /* je: equal / zero */

loc_000620EF: ;
    _fa = (uint32_t)(LO16(eax)) & 0xFFFFu; _fb = (uint32_t)(0xF) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp LO16(eax), 0xF (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x2392AD9Eu; /* flag setter tag */
    MEM8(esp + 0x1B) = 0;
    if (CMP_NE(_fa, _fb)) goto loc_00062100; /* jne: not equal / not zero */

loc_000620FA: ;
    MEM8(esp + 0x1B) = 1;
    /* nop */

loc_00062100: ;
    ecx = MEM32(esp + 0x3C);
    eax = esp + esi + 0x80;
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = esp + 0x28;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00062116u); sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00062116: ;
    edx = esp + 0xE0;
    PUSH32(esp, edx);
    eax = esp + 0x24;
    edi = esp + esi + 0x164;
    PUSH32(esp, eax);
    ecx = edi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00062131u); sub_00012570(); /* call 0x00012570 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00062131: ;
    ecx = MEM32(0x38E520);
    MEM32(esp + esi + 0x16C) = 0x3F800000;
    edx = MEM32(ecx + 0x680);
    PUSH32(esp, edi);
    PUSH32(esp, edx);
    ecx = esp + 0x28;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00062153u); sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00062153: ;
    eax = esp + 0x20;
    PUSH32(esp, eax);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0006215Fu); sub_00012950(); /* call 0x00012950 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0006215F: ;
    ecx = esp + 0x70;
    PUSH32(esp, ecx);
    ecx = esp + 0x24;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0006216Du); sub_00012600(); /* call 0x00012600 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0006216D: ;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x17A09C)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x17a09c] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_00062183; /* jne: not equal / not zero */

loc_0006217A: ;
    MEM32(esp + 0x10) = MEM32(esp + 0x10) + 1;
    _fa = (uint32_t)(MEM32(esp + 0x10)) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x70A66404u; /* flag setter tag */
    goto loc_00062221;

loc_00062183: ;
    edx = MEM32(0x38E520);
    eax = MEM32(edx + 0x69C);
    PUSH32(esp, edi);
    PUSH32(esp, eax);
    ecx = esp + 0x28;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0006219Au); sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0006219A: ;
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esp + 0x2C)); /* fdiv dword ptr [esp + 0x2c] */
    ecx = esp + 0x20;
    PUSH32(esp, ecx);
    PUSH32(esp, ecx);
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000621B2u); sub_000126D0(); /* call 0x000126D0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000621B2: ;
    fp_push(MEMF(0x3BCB30)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 0x20)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 0x20] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_000621CF; /* jne: not equal / not zero */

loc_000621C3: ;
    edx = MEM32(esp + 0x20);
    MEM32(0x3BCB30) = edx;
    goto loc_000621E9;

loc_000621CF: ;
    fp_push(MEMF(0x3BCB40)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 0x20)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 0x20] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000621E9; /* jp: parity */

loc_000621E0: ;
    eax = MEM32(esp + 0x20);
    MEM32(0x3BCB40) = eax;

loc_000621E9: ;
    fp_push(MEMF(0x3BCB34)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 0x24)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 0x24] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_00062206; /* jne: not equal / not zero */

loc_000621FA: ;
    ecx = MEM32(esp + 0x24);
    MEM32(0x3BCB34) = ecx;
    goto loc_00062221;

loc_00062206: ;
    fp_push(MEMF(0x3BCB44)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 0x24)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 0x24] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00062221; /* jp: parity */

loc_00062217: ;
    edx = MEM32(esp + 0x24);
    MEM32(0x3BCB44) = edx;

loc_00062221: ;
    _fa = (uint32_t)(MEM32(ebx + 0xA4)) & 0xFFFFFFFFu; _fb = (uint32_t)(2) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(ebx + 0xA4), 2 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x84D69FF5u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_00062303; /* jne: not equal / not zero */

loc_0006222E: ;
    ecx = MEM32(esp + 0x3C);
    eax = esp + 0x40;
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = esp + 0x28;
    MEM32(esp + 0x48) = 0x3F000000;
    MEM32(esp + 0x4C) = 0;
    MEM32(esp + 0x50) = 0;
    MEM32(esp + 0x54) = 0x3F800000;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00062261u); sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00062261: ;
    eax = MEM32(0x38E520);
    ecx = MEM32(eax + 0x69C);
    edx = esp + 0x20;
    PUSH32(esp, edx);
    PUSH32(esp, ecx);
    ecx = esp + 0x28;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0006227Bu); sub_00012790(); /* call 0x00012790 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0006227B: ;
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() / MEMF(esp + 0x2C)); /* fdiv dword ptr [esp + 0x2c] */
    edx = esp + 0x20;
    PUSH32(esp, edx);
    PUSH32(esp, ecx);
    ecx = edx;
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00062295u); sub_000126D0(); /* call 0x000126D0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00062295: ;
    fp_push(MEMF(0x3BCB30)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 0x20)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 0x20] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_000622B1; /* jne: not equal / not zero */

loc_000622A6: ;
    eax = MEM32(esp + 0x20);
    MEM32(0x3BCB30) = eax;
    goto loc_000622CC;

loc_000622B1: ;
    fp_push(MEMF(0x3BCB40)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 0x20)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 0x20] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000622CC; /* jp: parity */

loc_000622C2: ;
    ecx = MEM32(esp + 0x20);
    MEM32(0x3BCB40) = ecx;

loc_000622CC: ;
    fp_push(MEMF(0x3BCB34)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 0x24)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 0x24] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_000622E9; /* jne: not equal / not zero */

loc_000622DD: ;
    edx = MEM32(esp + 0x24);
    MEM32(0x3BCB34) = edx;
    goto loc_00062303;

loc_000622E9: ;
    fp_push(MEMF(0x3BCB44)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 0x24)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 0x24] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00062303; /* jp: parity */

loc_000622FA: ;
    eax = MEM32(esp + 0x24);
    MEM32(0x3BCB44) = eax;

loc_00062303: ;
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x10)) >> 32) & 1);
    esi = esi + 0x10;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x5BF233CEu; /* flag setter tag */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0x60) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0x60 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x26B656CBu; /* flag setter tag */
    if (CMP_L(_fas, _fbs)) goto loc_00062100; /* jl: less (signed <) */

loc_0006230F: ;
    _fa = (uint32_t)(MEM32(esp + 0x10)) & 0xFFFFFFFFu; _fb = (uint32_t)(6) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp MEM32(esp + 0x10), 6 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xFCA6B6B9u; /* flag setter tag */
    SET_LO8(eax, MEM8(esp + 0x1B));
    if (CMP_NE(_fa, _fb)) goto loc_00062329; /* jne: not equal / not zero */

loc_0006231A: ;
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_00062341; /* jne: not equal / not zero */

loc_0006231E: ;
    MEM8(ebx + 0x1E) = 0;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x90BC875Cu ? (CMP_B(_fa & _fb, 0)) : (_fk == 0x452A891Eu ? (CMP_B(_fa & _fb, 0)) : CMP_B(_fa, _fb)))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

loc_00062329: ;
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x00874B0Eu; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_00062341; /* jne: not equal / not zero */

loc_0006232D: ;
    fp_push(MEMF(esp + 0x5C)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x176190)); /* fadd dword ptr [0x176190] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1761AC)); /* fmul dword ptr [0x1761ac] */
    MEMF(esp + 0x5C) = (float)fp_top(); fp_pop(); /* fstp */

loc_00062341: ;
    eax = MEM32(esp + 0x14);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_LE(_fas, _fbs)) goto loc_000623EF; /* jle: less or equal (signed <=) */

loc_0006234D: ;
    esi = MEM32(esp + 0x38);
    edi = MEM32(esp + 0x6C);
    ecx = eax;
    _fb = (uint32_t)(0x1C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x1C)) >> 32) & 1);
    esi = esi + 0x1C;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x9159563Bu; /* flag setter tag */
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(0x10)) >> 32) & 1);
    edi = edi + 0x10;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xA4770865u; /* flag setter tag */
    MEM32(esp + 0x14) = ecx;

loc_00062361: ;
    edx = MEM32(esp + 0x3C);
    PUSH32(esp, edi);
    PUSH32(esp, edx);
    ecx = esp + 0x48;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00062370u); sub_00012720(); /* call 0x00012720 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_00062370: ;
    eax = esp + 0x100;
    PUSH32(esp, eax);
    ecx = edi;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0006237Fu); sub_00012600(); /* call 0x00012600 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0006237F: ;
    ecx = MEM32(esp + 0x5C);
    edx = MEM32(esp + 0x58);
    fp_top() = RECOMP_FP_PC(fp_top() + fp_top()); /* fadd st(0), st(0) */
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x176190)); /* fadd dword ptr [0x176190] */
    MEMF(esp + 0x18) = (float)fp_top(); fp_pop(); /* fstp */
    eax = MEM32(esp + 0x18);
    PUSH32(esp, eax);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0006239Fu); sub_000200B0(); /* call 0x000200B0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0006239F: ;
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(esp + 0x1C);
    _fb = (uint32_t)(0xC) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(0xC)) >> 32) & 1);
    esp = esp + 0xC;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x794B12F6u; /* flag setter tag */
    ecx = esp + 0xF0;
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    ecx = esp + 0xE8;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000623BFu); sub_000126A0(); /* call 0x000126A0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000623BF: ;
    eax = esp + 0xE0;
    PUSH32(esp, eax);
    ecx = esp + 0x44;
    PUSH32(esp, ecx);
    ecx = esi + -12;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000623D4u); sub_00012570(); /* call 0x00012570 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_000623D4: ;
    eax = MEM32(esp + 0x14);
    MEM32(esi) = 0x3F800000;
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x40)) >> 32) & 1);
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x42FAE895u; /* flag setter tag */
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(0x40)) >> 32) & 1);
    edi = edi + 0x40;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xBD7FD33Eu; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(esp + 0x14) = eax;
    if ((_fa != 0)) goto loc_00062361; /* jne: not equal / not zero */

loc_000623EF: ;
    eax = MEM32(ebx + 0xAC);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_00062501; /* je: equal / zero */

loc_000623FD: ;
    fp_push(MEMF(ebx + 0x64)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x34)); /* fsub dword ptr [esp + 0x34] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x17A098)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x17a098] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_00062501; /* jp: parity */

loc_00062415: ;
    edx = MEM32(ebx + 0x60);
    fp_push(MEMF(esp + 0x34)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x17A094)); /* fadd dword ptr [0x17a094] */
    ecx = MEM32(ebx + 0x64);
    eax = MEM32(ebx + 0x90);
    esi = MEM32(ebx + 0xC);
    MEM32(esp + 0x40) = edx;
    MEMF(esp + 0x1C) = (float)fp_top(); fp_pop(); /* fstp */
    edx = MEM32(ebx + 0x68);
    _cf = 0; /* xor clears CF */
    edi = 0; /* xor self */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x3E0F5DA7u; /* flag setter tag */
    _fa = (uint32_t)(MEM16(ebx + 8)) & 0xFFFFu; _fb = (uint32_t)(LO16(edi)) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(ebx + 8), LO16(edi) (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x79026061u; /* flag setter tag */
    MEM32(esp + 0x44) = ecx;
    MEM32(esp + 0x48) = edx;
    if (CMP_BE(_fa, _fb)) goto loc_0006257A; /* jbe: below or equal (unsigned <=) */

loc_0006244D: ;
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x14)) >> 32) & 1);
    esi = esi + 0x14;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x9FDED0EDu; /* flag setter tag */
    _fb = (uint32_t)(0x34) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0x34)) >> 32) & 1);
    eax = eax + 0x34;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xDBA98962u; /* flag setter tag */
    MEM32(esp + 0x3C) = eax;
    goto loc_00062460;

loc_00062460: ;
    eax = MEM32(esi);
    MEM32(esp + 0x10) = eax;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 0x34)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 0x34] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_000624E5; /* jne: not equal / not zero */

loc_00062475: ;
    ecx = MEM32(esp + 0x10);
    edx = esi + -4;
    MEM32(esp + 0x44) = ecx;
    PUSH32(esp, edx);
    ecx = esp + 0x44;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa & _fb, 0)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0006248Au); sub_00012BC0(); /* call 0x00012BC0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0006248A: ;
    MEMF(esp + 0x14) = (float)fp_top(); /* fst */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x17A090)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x17a090] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000624E5; /* jp: parity */

loc_0006249B: ;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(esp + 0x1C)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [esp + 0x1c] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_000624C4; /* jne: not equal / not zero */

loc_000624AA: ;
    fp_push(MEMF(esp + 0x10)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - MEMF(esp + 0x1C)); /* fsub dword ptr [esp + 0x1c] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x14)); /* fmul dword ptr [esp + 0x14] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x17A08C)); /* fmul dword ptr [0x17a08c] */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0x1C)); /* fadd dword ptr [esp + 0x1c] */
    MEMF(esi) = (float)fp_top(); fp_pop(); /* fstp */
    goto loc_000624E5;

loc_000624C4: ;
    eax = MEM32(esp + 0x3C);
    fp_push(MEMF(eax)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x1761A0)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x1761a0] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(5) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 5 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x452A891Eu; /* flag setter tag */
    if (RECOMP_PARITY8((_fa) & (_fb))) goto loc_000624DF; /* jp: parity */

loc_000624D7: ;
    ecx = MEM32(esp + 0x34);
    MEM32(esi) = ecx;
    goto loc_000624E5;

loc_000624DF: ;
    edx = MEM32(esp + 0x1C);
    MEM32(esi) = edx;

loc_000624E5: ;
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(MEM32(esp + 0x3C)) + (uint64_t)(0x40)) >> 32) & 1);
    MEM32(esp + 0x3C) = MEM32(esp + 0x3C) + 0x40;
    _fa = (uint32_t)(MEM32(esp + 0x3C)) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xAC29E05Du; /* flag setter tag */
    eax = ZX16(MEM16(ebx + 8));
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x40)) >> 32) & 1);
    esi = esi + 0x40;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x42FAE895u; /* flag setter tag */
    edi++;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0xDF715D11u; /* flag setter tag */
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, eax (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x121F66EFu; /* flag setter tag */
    if (CMP_L(_fas, _fbs)) goto loc_00062460; /* jl: less (signed <) */

loc_000624FA: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

loc_00062501: ;
    fp_push(MEMF(esp + 0x1C)); /* fld float */
    ecx = MEM32(0x1916D0);
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(0x176238)); /* fadd dword ptr [0x176238] */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(ecx + 0x16C)); /* fmul dword ptr [ecx + 0x16c] */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x176190)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x176190] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_00062541; /* jne: not equal / not zero */

loc_00062524: ;
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x177114)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); /* fcom dword ptr [0x177114] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(0x41) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 0x41 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x540E384Au; /* flag setter tag */
    if (TEST_NZ(_fa, _fb)) goto loc_00062539; /* jne: not equal / not zero */

loc_00062531: ;
    fp_pop(); /* fstp st(0) */
    fp_push(MEMF(0x177114)); /* fld float */

loc_00062539: ;
    fp_push(MEMF(0x1761A0)); /* fld float */
    goto loc_00062549;

loc_00062541: ;
    fp_push(MEMF(0x176190)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() - fp_st1()); /* fsub st(1) */

loc_00062549: ;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(esp + 0x34)); /* fmul dword ptr [esp + 0x34] */
    eax = MEM32(ebx + 0xC);
    _cf = 0; /* xor clears CF */
    ecx = 0; /* xor self */
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x71650E52u; /* flag setter tag */
    _fa = (uint32_t)(MEM16(ebx + 8)) & 0xFFFFu; _fb = (uint32_t)(LO16(ecx)) & 0xFFFFu;
    _fas = (int32_t)(int16_t)(_fa); _fbs = (int32_t)(int16_t)(_fb); /* cmp MEM16(ebx + 8), LO16(ecx) (16-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xF5006180u; /* flag setter tag */
    MEMF(esp + 0x6C) = (float)fp_top(); fp_pop(); /* fstp */
    if (CMP_BE(_fa, _fb)) goto loc_00062578; /* jbe: below or equal (unsigned <=) */

loc_0006255C: ;
    _fb = (uint32_t)(0x14) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0x14)) >> 32) & 1);
    eax = eax + 0x14;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xC2A15239u; /* flag setter tag */
    /* nop */

loc_00062560: ;
    { double _t = fp_top(); fp_push(_t); } /* fld st(0) */
    _fb = (uint32_t)(0x40) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0x40)) >> 32) & 1);
    eax = eax + 0x40;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x1F856A41u; /* flag setter tag */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(eax + -64)); /* fmul dword ptr [eax - 0x40] */
    ecx++;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x53735CF0u; /* flag setter tag */
    fp_top() = RECOMP_FP_PC(fp_top() + MEMF(esp + 0x6C)); /* fadd dword ptr [esp + 0x6c] */
    MEMF(eax + -64) = (float)fp_top(); fp_pop(); /* fstp */
    edx = ZX16(MEM16(ebx + 8));
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fb = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ecx, edx (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0xCBD5F2B1u; /* flag setter tag */
    if (CMP_L(_fas, _fbs)) goto loc_00062560; /* jl: less (signed <) */

loc_00062578: ;
    fp_pop(); /* fstp st(0) */

loc_0006257A: ;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0xCBD5F2B1u ? (CMP_B(_fa, _fb)) : (_fk == 0xF5006180u ? (CMP_B(_fa, _fb)) : (_fk == 0x00874B0Eu ? (CMP_B(_fa, _fb)) : (_fk == 0xC77E4F9Du ? (CMP_B(_fa & _fb, 0)) : CMP_B(_fa, _fb)))))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

loc_00062581: ;
    eax = MEM32(0x38E588);
    POP32(esp, edi);
    POP32(esp, esi);
    MEM16(eax + 0x28) = 1;
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}
