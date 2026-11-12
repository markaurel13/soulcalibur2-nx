/**
 * Optimized implementation of sub_000240C0 for Nintendo Switch.
 * Caches D3D state calls, eliminates RECOMP_ICALL_SAFE, and strips dead g_eflags.
 */

#define RECOMP_GENERATED_CODE
#include "recomp_funcs.h"
#include <stdint.h>
#include "sc2_prof.h"

static RecompFunction s_fn_set_light = NULL;
static RecompFunction s_fn_light_enable = NULL;
static RecompFunction s_fn_set_material = NULL;
static RecompFunction s_fn_cull_mode = NULL;
static RecompFunction s_fn_set_texture_stage_state = NULL;
static RecompFunction s_fn_set_transform = NULL;
static RecompFunction s_fn_001508e0 = NULL;
static RecompFunction s_fn_set_pixel_shader_program = NULL;

static inline void init_240c0_fns(void)
{
    if (__builtin_expect(!s_fn_set_light, 0)) {
        s_fn_set_light = recomp_lookup_manual(0x001504C0u);
        s_fn_light_enable = recomp_lookup_manual(0x00150780u);
        s_fn_set_material = recomp_lookup_manual(0x0014F9B0u);
        s_fn_cull_mode = recomp_lookup_manual(0x0014E500u);
        s_fn_set_texture_stage_state = recomp_lookup_manual(0x0014E9A0u);
        s_fn_set_transform = recomp_lookup_manual(0x0014F890u);
        s_fn_001508e0 = recomp_lookup_manual(0x001508E0u);
        s_fn_set_pixel_shader_program = recomp_lookup_manual(0x00151F60u);
    }
}

void __wrap_sub_000240C0(void)
{
    SC2_PROF_SCOPE(0x000240C0u);
    init_240c0_fns();
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

loc_000240C0: ;
    PUSH32(esp, ebp);
    ebp = esp;
    g_ebp = ebp; /* publish frame for frameless callees */
    g_seh_ebp = ebp;
    _cf = 0; /* logical op clears CF */
    esp = esp & 0xFFFFFFF0u;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fb = (uint32_t)(0xC4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0xC4));
    esp = esp - 0xC4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    PUSH32(esp, ebx);
    ebx = MEM32(ebp + 8);
    eax = MEM32(ebx);
    PUSH32(esp, esi);
    esi = MEM32(ebx + 4);
    _cf = 0; /* logical op clears CF */
    esi = esi & 0xF;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0x10000000) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* test eax, 0x10000000 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    eax = MEM32(0x15E188);
    PUSH32(esp, edi);
    if (TEST_Z(_fa, _fb)) goto loc_000240FC; /* je: equal / zero */

loc_000240E6: ;
    _cf = 0; /* logical op clears CF */
    eax = eax | 0x2000;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    MEM32(0x15E188) = eax;
    MEM32(0x15E500) = 0;
    goto loc_00024112;

loc_000240FC: ;
    _cf = 0; /* logical op clears CF */
    eax = eax | 0x2000;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    MEM32(0x15E188) = eax;
    eax = ZX8(MEM8(0x38C1F0));
    MEM32(0x15E500) = eax;

loc_00024112: ;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(2) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 2 (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_EQ(_fa, _fb)) goto loc_0002412B; /* je: equal / zero */

loc_00024117: ;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(3) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 3 (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_EQ(_fa, _fb)) goto loc_0002412B; /* je: equal / zero */

loc_0002411C: ;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 4 (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_EQ(_fa, _fb)) goto loc_0002412B; /* je: equal / zero */

loc_00024121: ;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0xB) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0xB (32-bit) */
    _cf = (int)(_fa < _fb);
    MEM8(esp + 0xF) = 1;
    if (CMP_NE(_fa, _fb)) goto loc_00024130; /* jne: not equal / not zero */

loc_0002412B: ;
    MEM8(esp + 0xF) = 0;

loc_00024130: ;
    _cf = 0; /* xor clears CF */
    eax = 0; /* xor self */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    ecx = 0x11;
    edi = esp + 0x88;
    { uint32_t _i; int32_t _st = RECOMP_DF_STEP(4); for (_i = 0; _i < ecx; _i++) MEM32(edi + _i*_st) = eax; edi += ecx * _st; }
    ecx = 0; /* rep stosd */
    _fa = (uint32_t)(MEM32(ebx)) & 0xFFFFFFFFu; _fb = (uint32_t)(0x20000000) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* test MEM32(ebx), 0x20000000 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    ecx = ebx + 0x20;
    if (TEST_Z(_fa, _fb)) goto loc_00024168; /* je: equal / zero */

loc_0002414B: ;
    edx = MEM32(ecx);
    eax = MEM32(ecx + 4);
    MEM32(esp + 0x20) = edx;
    edx = MEM32(ecx + 8);
    MEM32(esp + 0x24) = eax;
    eax = MEM32(ecx + 0xC);
    MEM32(esp + 0x28) = edx;
    MEM32(esp + 0x2C) = eax;
    goto loc_00024177;

loc_00024168: ;
    PUSH32(esp, 0x38C560);
    PUSH32(esp, ecx);
    ecx = esp + 0x28;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024177u); sub_00012550(); /* call 0x00012550 */

loc_00024177: ;
    edx = esp + 0x20;
    PUSH32(esp, 0x3F800000);
    PUSH32(esp, edx);
    ecx = edx;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024188u); sub_00012D00(); /* call 0x00012D00 */

loc_00024188: ;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(4) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 4 (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_EQ(_fa, _fb)) goto loc_000241A5; /* je: equal / zero */

loc_0002418D: ;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0xB) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0xB (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_EQ(_fa, _fb)) goto loc_000241A5; /* je: equal / zero */

loc_00024192: ;
    ecx = MEM32(0x38C258);
    eax = esp + 0x20;
    PUSH32(esp, eax);
    PUSH32(esp, ecx);
    ecx = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000241A5u); sub_000126A0(); /* call 0x000126A0 */

loc_000241A5: ;
    SET_LO8(eax, MEM8(esp + 0xF));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_NE(_fa, _fb)) goto loc_000241BF; /* jne: not equal / not zero */

loc_000241AD: ;
    eax = MEM32(0x38C1F4);
    edx = esp + 0x20;
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    ecx = edx;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000241BFu); sub_000126A0(); /* call 0x000126A0 */

loc_000241BF: ;
    _fa = (uint32_t)(MEM8(ebx)) & 0xFFu; _fb = (uint32_t)(0xF) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test MEM8(ebx), 0xF (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (TEST_NZ(_fa, _fb)) goto loc_00024280; /* jne: not equal / not zero */

loc_000241C8: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000241D1u); s_fn_light_enable(); /* manual call 0x00150780 */
    }

loc_000241D1: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    PUSH32(esp, 1);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000241DAu); s_fn_light_enable(); /* manual call 0x00150780 */
    }

loc_000241DA: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    PUSH32(esp, 2);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000241E3u); s_fn_light_enable(); /* manual call 0x00150780 */
    }

loc_000241E3: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    PUSH32(esp, 3);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000241ECu); s_fn_light_enable(); /* manual call 0x00150780 */
    }

loc_000241EC: ;
    ecx = esp + 0x20;
    PUSH32(esp, ecx);
    PUSH32(esp, 0x43000000);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000241FBu); sub_000126A0(); /* call 0x000126A0 */

loc_000241FB: ;
    PUSH32(esp, 0);
    edx = esp + 0x24;
    PUSH32(esp, 0x437F0000);
    PUSH32(esp, edx);
    ecx = edx;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002420Eu); sub_00012D30(); /* call 0x00012D30 */

loc_0002420E: ;
    fp_push(MEMF(esp + 0x20)); /* fld float */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024217u); sub_00145A64(); /* call 0x00145A64 */

loc_00024217: ;
    fp_push(MEMF(esp + 0x24)); /* fld float */
    esi = ZX8(LO8(eax));
    _cf = 0; /* logical op clears CF */
    esi = esi | 0xFFFFFF00u;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    if (((8) & 31u)) _cf = (int)(((uint32_t)(esi) >> ((32 - (((8) & 31u))) & 31u)) & 1u);
    esi = (uint32_t)(esi) << ((8) & 31u);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    g_eflags = (g_eflags & ~0xC5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002422Cu); sub_00145A64(); /* call 0x00145A64 */

loc_0002422C: ;
    eax = ZX8(LO8(eax));
    fp_push(MEMF(esp + 0x28)); /* fld float */
    _cf = 0; /* logical op clears CF */
    esi = esi | eax;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    if (((8) & 31u)) _cf = (int)(((uint32_t)(esi) >> ((32 - (((8) & 31u))) & 31u)) & 1u);
    esi = (uint32_t)(esi) << ((8) & 31u);
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    g_eflags = (g_eflags & ~0xC5u) | ((_cf) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002423Du); sub_00145A64(); /* call 0x00145A64 */

loc_0002423D: ;
    ecx = ZX8(LO8(eax));
    _cf = 0; /* logical op clears CF */
    esi = esi | ecx;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    ecx = MEM32(0x15E188);
    edx = esp + 0x88;
    _cf = 0; /* logical op clears CF */
    ecx = ecx | 0x3000;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edx);
    MEM32(0x15E55C) = esi;
    MEM32(0x15E550) = 1;
    MEM32(0x15E188) = ecx;
    MEM32(0x15E52C) = 0;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002427Bu); s_fn_set_material(); /* manual call 0x0014F9B0 */
    }

loc_0002427B: ;
    goto loc_0002450A;

loc_00024280: ;
    edx = MEM32(0x15E188);
    eax = esp + 0x20;
    PUSH32(esp, eax);
    _cf = 0; /* xor clears CF */
    ebx = 0; /* xor self */
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _cf = 0; /* logical op clears CF */
    edx = edx | 0x1000;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    PUSH32(esp, 0x3F000000);
    ecx = eax;
    MEM32(0x15E55C) = ebx;
    MEM32(0x15E188) = edx;
    MEM32(0x15E550) = 1;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000242B5u); sub_000126A0(); /* call 0x000126A0 */

loc_000242B5: ;
    ecx = MEM32(esp + 0x20);
    edx = MEM32(esp + 0x24);
    eax = MEM32(esp + 0x28);
    MEM32(0x38C284) = ecx;
    ecx = MEM32(esp + 0x2C);
    MEM32(0x38C288) = edx;
    MEM32(0x38C28C) = eax;
    MEM32(0x38C290) = ecx;
    edi = 0x38C260;
    esi = 0x38C520;

loc_000242E6: ;
    eax = MEM32(ebp + 8);
    PUSH32(esp, esi);
    _fb = (uint32_t)(0x30) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(0x30)) >> 32) & 1);
    eax = eax + 0x30;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    PUSH32(esp, eax);
    ecx = esp + 0x18;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000242F7u); sub_00012860(); /* call 0x00012860 */

loc_000242F7: ;
    SET_LO8(eax, MEM8(esp + 0xF));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_NE(_fa, _fb)) goto loc_00024316; /* jne: not equal / not zero */

loc_000242FF: ;
    fp_push(MEMF(0x38C1F4)); /* fld float */
    edx = esp + 0x10;
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x1761AC)); /* fmul dword ptr [0x1761ac] */
    PUSH32(esp, edx);
    PUSH32(esp, ecx);
    MEMF(esp) = (float)fp_top(); fp_pop(); /* fstp */
    goto loc_00024320;

loc_00024316: ;
    eax = esp + 0x10;
    PUSH32(esp, eax);
    PUSH32(esp, 0x3F000000);

loc_00024320: ;
    ecx = esp + 0x18;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024329u); sub_000126A0(); /* call 0x000126A0 */

loc_00024329: ;
    edx = MEM32(esp + 0x10);
    eax = MEM32(esp + 0x14);
    ecx = edi + 4;
    MEM32(ecx) = edx;
    edx = MEM32(esp + 0x18);
    MEM32(ecx + 4) = eax;
    eax = MEM32(esp + 0x1C);
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edi);
    MEM32(ecx + 8) = edx;
    PUSH32(esp, ebx);
    MEM32(ecx + 0xC) = eax;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002434Eu); s_fn_set_light(); /* manual call 0x001504C0 */
    }

loc_0002434E: ;
    _fb = (uint32_t)(0x10) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(0x10)) >> 32) & 1);
    esi = esi + 0x10;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    ebx++;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fb = (uint32_t)(0x68) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(0x68)) >> 32) & 1);
    edi = edi + 0x68;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(0x38C550) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 0x38C550 (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_L(_fas, _fbs)) goto loc_000242E6; /* jl: less (signed <) */

loc_0002435D: ;
    esi = MEM32(ebp + 8);
    fp_push(MEMF(esi + 0x4C)); /* fld float */
    g_fp_cmp = RECOMP_FCMP(fp_top(), MEMF(0x176264)); g_fp_cc = RECOMP_FCMP_CC(g_fp_cmp); fp_pop(); /* fcomp dword ptr [0x176264] */
    eax = (eax & 0xFFFF0000u) | (uint32_t)(uint16_t)(((g_fp_top & 7u) << 11) | g_fp_cc); /* fnstsw ax <- fpu status */
    _fa = (uint32_t)(HI8(eax)) & 0xFFu; _fb = (uint32_t)(1) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* test HI8(eax), 1 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (TEST_NZ(_fa, _fb)) goto loc_00024424; /* jne: not equal / not zero */

loc_00024374: ;
    ebx = MEM32(0x15E188);
    PUSH32(esp, 0x38C510);
    ecx = esi + 0x40;
    PUSH32(esp, ecx);
    _cf = 0; /* logical op clears CF */
    ebx = ebx | 0x3000;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    ecx = esp + 0x18;
    MEM32(0x15E188) = ebx;
    MEM32(0x15E52C) = 1;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000243A2u); sub_00012860(); /* call 0x00012860 */

loc_000243A2: ;
    SET_LO8(eax, MEM8(esp + 0xF));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    ecx = esp + 0x10;
    if (CMP_NE(_fa, _fb)) goto loc_000243B9; /* jne: not equal / not zero */

loc_000243AE: ;
    eax = MEM32(0x38C1F4);
    edx = ecx;
    PUSH32(esp, edx);
    PUSH32(esp, eax);
    goto loc_000243C3;

loc_000243B9: ;
    PUSH32(esp, ecx);
    PUSH32(esp, 0x3F000000);
    ecx = esp + 0x18;

loc_000243C3: ;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000243C8u); sub_000126A0(); /* call 0x000126A0 */

loc_000243C8: ;
    edx = MEM32(esp + 0x10);
    eax = MEM32(esp + 0x14);
    ecx = MEM32(esp + 0x18);
    MEM32(0x38C414) = edx;
    edx = MEM32(esp + 0x1C);
    MEM32(0x38C418) = eax;
    MEM32(0x38C41C) = ecx;
    MEM32(0x38C420) = edx;
    eax = MEM32(esi + 0x4C);
    MEM32(esp + 0xC8) = eax;
    MEM32(esp + 0xB0) = 0x3F800000;
    MEM32(esp + 0xAC) = 0x3F800000;
    MEM32(esp + 0xA8) = 0x3F800000;
    PUSH32(esp, 0x38C400);
    goto loc_000244CF;

loc_00024424: ;
    edi = MEM32(0x15E188);
    PUSH32(esp, 0x38C550);
    eax = esi + 0x30;
    _cf = 0; /* logical op clears CF */
    edi = edi | 0x3000;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    PUSH32(esp, eax);
    ecx = esp + 0x18;
    MEM32(0x15E188) = edi;
    MEM32(0x15E52C) = 0;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024452u); sub_00012860(); /* call 0x00012860 */

loc_00024452: ;
    SET_LO8(eax, MEM8(esp + 0xF));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    ecx = esp + 0x10;
    if (CMP_NE(_fa, _fb)) goto loc_00024468; /* jne: not equal / not zero */

loc_0002445E: ;
    edx = MEM32(0x38C1F4);
    PUSH32(esp, ecx);
    PUSH32(esp, edx);
    goto loc_00024472;

loc_00024468: ;
    eax = esp + 0x10;
    PUSH32(esp, eax);
    PUSH32(esp, 0x3F000000);

loc_00024472: ;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024477u); sub_000126A0(); /* call 0x000126A0 */

loc_00024477: ;
    ecx = MEM32(esp + 0x10);
    edx = MEM32(esp + 0x14);
    eax = MEM32(esp + 0x18);
    MEM32(0x38C39C) = ecx;
    ecx = MEM32(esp + 0x1C);
    MEM32(0x38C3A0) = edx;
    MEM32(0x38C3A4) = eax;
    MEM32(0x38C3A8) = ecx;
    MEM32(esp + 0xC8) = 0;
    MEM32(esp + 0xB0) = 0;
    MEM32(esp + 0xAC) = 0;
    MEM32(esp + 0xA8) = 0;
    PUSH32(esp, 0x38C398);

loc_000244CF: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 3);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000244D6u); s_fn_set_light(); /* manual call 0x001504C0 */
    }

loc_000244D6: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 1);
    PUSH32(esp, 0);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000244DFu); s_fn_light_enable(); /* manual call 0x00150780 */
    }

loc_000244DF: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 1);
    PUSH32(esp, 1);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000244E8u); s_fn_light_enable(); /* manual call 0x00150780 */
    }

loc_000244E8: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 1);
    PUSH32(esp, 2);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000244F1u); s_fn_light_enable(); /* manual call 0x00150780 */
    }

loc_000244F1: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 1);
    PUSH32(esp, 3);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000244FAu); s_fn_light_enable(); /* manual call 0x00150780 */
    }

loc_000244FA: ;
    edx = esp + 0x88;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, edx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024507u); s_fn_set_material(); /* manual call 0x0014F9B0 */
    }

loc_00024507: ;
    ebx = MEM32(ebp + 8);

loc_0002450A: ;
    _fa = (uint32_t)(MEM32(ebx)) & 0xFFFFFFFFu; _fb = (uint32_t)(0x8000000) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* test MEM32(ebx), 0x8000000 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (TEST_Z(_fa, _fb)) goto loc_00024519; /* je: equal / zero */

loc_00024512: ;
    PUSH32(esp, 0x900);
    goto loc_0002451B;

loc_00024519: ;
    PUSH32(esp, 0);

loc_0002451B: ;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x00024520u); s_fn_cull_mode(); /* manual call 0x0014E500 */
    }

loc_00024520: ;
    eax = MEM32(ebx + 8);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_EQ(_fa, _fb)) goto loc_000245E0; /* je: equal / zero */

loc_0002452B: ;
    PUSH32(esp, eax);
    PUSH32(esp, 0);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024533u); sub_00025460(); /* call 0x00025460 */

loc_00024533: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002453Fu); s_fn_set_texture_stage_state(); /* manual call 0x0014E9A0 */
    }

loc_0002453F: ;
    eax = MEM32(ebx + 0x14);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_EQ(_fa, _fb)) goto loc_00024551; /* je: equal / zero */

loc_00024546: ;
    PUSH32(esp, eax);
    PUSH32(esp, 0);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002454Eu); sub_00023840(); /* call 0x00023840 */

loc_0002454E: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */

loc_00024551: ;
    eax = MEM32(ebx + 0xC);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_EQ(_fa, _fb)) goto loc_0002466E; /* je: equal / zero */

loc_0002455C: ;
    PUSH32(esp, eax);
    PUSH32(esp, 1);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024564u); sub_00025460(); /* call 0x00025460 */

loc_00024564: ;
    eax = MEM32(ebx + 0x18);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_EQ(_fa, _fb)) goto loc_0002457E; /* je: equal / zero */

loc_0002456E: ;
    PUSH32(esp, eax);
    PUSH32(esp, 1);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024576u); sub_00023840(); /* call 0x00023840 */

loc_00024576: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    goto loc_00024677;

loc_0002457E: ;
    ecx = esp + 0x40;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024587u); sub_00011050(); /* call 0x00011050 */

loc_00024587: ;
    PUSH32(esp, 0x3F000000);
    PUSH32(esp, 0x3F000000);
    eax = esp + 0x48;
    PUSH32(esp, 0x3F000000);
    PUSH32(esp, eax);
    ecx = eax;
    MEM32(esp + 0x88) = 0x3F000000;
    MEM32(esp + 0x84) = 0x3F000000;
    MEM32(esp + 0x80) = 0x3F000000;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000245C3u); sub_00011B60(); /* call 0x00011B60 */

loc_000245C3: ;
    ecx = esp + 0x40;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, ecx);
    PUSH32(esp, 3);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000245CFu); s_fn_set_transform(); /* manual call 0x0014F890 */
    }

loc_000245CF: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x10000);
    PUSH32(esp, 1);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000245DBu); s_fn_set_texture_stage_state(); /* manual call 0x0014E9A0 */
    }

loc_000245DB: ;
    goto loc_00024677;

loc_000245E0: ;
    eax = MEM32(ebx + 0xC);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_EQ(_fa, _fb)) goto loc_00024665; /* je: equal / zero */

loc_000245E7: ;
    PUSH32(esp, eax);
    PUSH32(esp, 0);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000245EFu); sub_00025460(); /* call 0x00025460 */

loc_000245EF: ;
    eax = MEM32(ebx + 0x18);
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_EQ(_fa, _fb)) goto loc_00024606; /* je: equal / zero */

loc_000245F9: ;
    PUSH32(esp, eax);
    PUSH32(esp, 0);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024601u); sub_00023840(); /* call 0x00023840 */

loc_00024601: ;
    _fb = (uint32_t)(8) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(8)) >> 32) & 1);
    esp = esp + 8;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    goto loc_00024677;

loc_00024606: ;
    ecx = esp + 0x40;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002460Fu); sub_00011050(); /* call 0x00011050 */

loc_0002460F: ;
    PUSH32(esp, 0x3F000000);
    PUSH32(esp, 0x3F000000);
    edx = esp + 0x48;
    PUSH32(esp, 0x3F000000);
    PUSH32(esp, edx);
    ecx = edx;
    MEM32(esp + 0x88) = 0x3F000000;
    MEM32(esp + 0x84) = 0x3F000000;
    MEM32(esp + 0x80) = 0x3F000000;
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002464Bu); sub_00011B60(); /* call 0x00011B60 */

loc_0002464B: ;
    eax = esp + 0x40;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, eax);
    PUSH32(esp, 2);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024657u); s_fn_set_transform(); /* manual call 0x0014F890 */
    }

loc_00024657: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0x10000);
    PUSH32(esp, 0);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024663u); s_fn_set_texture_stage_state(); /* manual call 0x0014E9A0 */
    }

loc_00024663: ;
    goto loc_00024677;

loc_00024665: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    PUSH32(esp, 0);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0002466Eu); s_fn_001508e0(); /* manual call 0x001508E0 */
    }

loc_0002466E: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, 0);
    PUSH32(esp, 1);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024677u); s_fn_001508e0(); /* manual call 0x001508E0 */
    }

loc_00024677: ;
    eax = MEM32(ebx + 8);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    eax = MEM32(ebx + 0xC);
    if (CMP_EQ(_fa, _fb)) goto loc_000246B0; /* je: equal / zero */

loc_00024681: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_EQ(_fa, _fb)) goto loc_000246B4; /* je: equal / zero */

loc_00024685: ;
    eax = MEM32(ebx + 4);
    if (((4) & 31u)) _cf = (int)(((uint32_t)(eax) >> (((((4) & 31u)) - 1) & 31u)) & 1u);
    eax = (uint32_t)(eax) >> ((4) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _cf = 0; /* logical op clears CF */
    eax = eax & 0xF;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(3) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 3 (32-bit) */
    _cf = (int)(_fa < _fb);
    if (CMP_A(_fa, _fb)) goto loc_000246C2; /* ja: above (unsigned >) */

loc_00024694: ;
    { uint32_t _jt = MEM32(eax * 4 + 0x2470C); /* switch: 4 entries, 3 targets */
    if (_jt == 0x0002469Bu) goto loc_0002469B;
    if (_jt == 0x000246A2u) goto loc_000246A2;
    if (_jt == 0x000246A9u) goto loc_000246A9;
    g_seh_ebp = ebp; RECOMP_ITAIL(_jt); return; }

loc_0002469B: ;
    esi = 0x194140;
    goto loc_000246C6;

loc_000246A2: ;
    esi = 0x194320;
    goto loc_000246C6;

loc_000246A9: ;
    esi = 0x194230;
    goto loc_000246C6;

loc_000246B0: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_EQ(_fa, _fb)) goto loc_000246BB; /* je: equal / zero */

loc_000246B4: ;
    esi = 0x194050;
    goto loc_000246C6;

loc_000246BB: ;
    esi = 0x193F60;
    goto loc_000246C6;

loc_000246C2: ;
    esi = MEM32(esp + 0x3C);

loc_000246C6: ;
    SET_LO8(eax, MEM8(esp + 0xF));
    _fa = (uint32_t)(LO8(eax)) & 0xFFu; _fb = (uint32_t)(0) & 0xFFu;
    _fas = (int32_t)(int8_t)(_fa); _fbs = (int32_t)(int8_t)(_fb); /* cmp LO8(eax), 0 (8-bit) */
    _cf = 0; /* nothing borrows from zero */
    if (CMP_EQ(_fa, _fb)) goto loc_000246E7; /* je: equal / zero */

loc_000246CE: ;
    fp_push(MEMF(0x38C1F4)); /* fld float */
    fp_top() = RECOMP_FP_PC(fp_top() * MEMF(0x17625C)); /* fmul dword ptr [0x17625c] */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000246DFu); sub_00145A64(); /* call 0x00145A64 */

loc_000246DF: ;
    eax = ZX8(LO8(eax));
    if (((0x18) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((0x18) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((0x18) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    goto loc_000246EA;

loc_000246E7: ;
    _cf = 0; /* logical op clears CF */
    eax = eax | 0xFFFFFFFFu;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */

loc_000246EA: ;
    { uint32_t _icall_esp = g_esp;
    PUSH32(esp, esi);
    MEM32(esi + 0x2C) = eax;
    MEM32(esi + 0x28) = eax;
    g_eflags = (g_eflags & ~0xC5u) | (((_fk == 0xC795549Du ? (0) : _cf)) ? 0x1u : 0u) | ((RECOMP_PARITY8(_fa)) ? 0x4u : 0u) | (((_fa == 0)) ? 0x40u : 0u) | (((_fas < 0)) ? 0x80u : 0u); /* flags across call boundary */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x000246F6u); s_fn_set_pixel_shader_program(); /* manual call 0x00151F60 */
    }

loc_000246F6: ;
    ecx = MEM32(ebx + 4);
    _cf = 0; /* logical op clears CF */
    ecx = ecx & 0xF;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    PUSH32(esp, ecx);
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x00024702u); sub_00022FE0(); /* call 0x00022FE0 */

loc_00024702: ;
    _fb = (uint32_t)(4) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esp) + (uint64_t)(4)) >> 32) & 1);
    esp = esp + 4;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_ebp = ebp; g_seh_ebp = ebp; esp += 4; return; /* ret */

    #undef fp_push
    #undef fp_pop
    #undef fp_top
    #undef fp_st
    #undef fp_st1
}
