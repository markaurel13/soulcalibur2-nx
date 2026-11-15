/* LTO-enabled extraction of sub_0007CA00 from recomp_0002.c for cross-TU inlining of sub_00079CD0/79A30/123C0. */
#define RECOMP_GENERATED_CODE 1
#include "recomp_funcs.h"
#include "sc2_fast_mem.h"
#include "x87_math.h"

void __wrap_sub_0007CA00(void)
{
    uint32_t ebp = 0;
    ebp = g_ebp;  /* prologue saves caller's frame */
    int _flags = 0; /* fallback flag var */
    uint32_t _fk = 0; /* tag of the last flag setter */
    uint32_t _fa = 0, _fb = 0;
    int32_t _fas = 0, _fbs = 0;
    (void)_fa; (void)_fb; (void)_fas; (void)_fbs;
    int _cf = g_eflags & 1u; /* incoming carry flag */

loc_0007CA00: ;
    PUSH32(esp, ebp);
    ebp = esp;
    g_ebp = ebp; /* publish frame for frameless callees */
    g_seh_ebp = ebp;
    _fb = (uint32_t)(0x42C) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esp) < (uint32_t)(0x42C));
    esp = esp - 0x42C;
    _fa = (uint32_t)(esp) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0x0C1C64B5u; /* flag setter tag */
    eax = MEM32(ebp + 0xC);
    PUSH32(esp, ebx);
    PUSH32(esp, esi);
    esi = MEM32(ebp + 8);
    PUSH32(esp, edi);
    PUSH32(esp, 0xFFFFFFFFu);
    MEM32(ebp + -36) = ecx;
    MEM32(ebp + -32) = esi;
    MEM32(ebp + -8) = eax;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0007CA22u); sub_00079D40(); /* call 0x00079D40 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0007CA22: ;
    ecx = eax;
    ebx = ZX16(MEM16(ecx + 2));
    eax = esi;
    edx = ((int32_t)eax < 0) ? 0xFFFFFFFF : 0; /* cdq */
    _cf = 0; /* logical op clears CF */
    edx = edx & 7;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0x44F8D173u; /* flag setter tag */
    _fb = (uint32_t)(edx) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(edx)) >> 32) & 1);
    eax = eax + edx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x2572FB98u; /* flag setter tag */
    if (((1) & 31u)) _cf = (int)(((uint32_t)(ebx) >> (((((1) & 31u)) - 1) & 31u)) & 1u);
    ebx = (uint32_t)(ebx) >> ((1) & 31u);
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0xEBF82DF1u; /* flag setter tag */
    edi = ecx + 0x1C;
    if (((3) & 31u)) _cf = (int)(((uint32_t)((int32_t)(int32_t)(eax)) >> (((((3) & 31u)) - 1) & 31u)) & 1);
    eax = (uint32_t)(((int32_t)(int32_t)(eax)) >> ((3) & 31u));
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sar result */
    _fk = 0x3A938898u; /* flag setter tag */
    _cf = 0; /* xor clears CF */
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x0296481Bu; /* flag setter tag */
    _cf = 0; /* logical op clears CF */
    esi = esi & 0x80000007u;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0xCF493B4Fu; /* flag setter tag */
    MEM32(ebp + -4) = edi;
    MEM32(ebp + -28) = ebx;
    if ((_fas >= 0)) goto loc_0007CA4D; /* jns: not sign (positive) */

loc_0007CA48: ;
    esi--;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0xC523AE8Du; /* flag setter tag */
    _cf = 0; /* logical op clears CF */
    esi = esi | 0xFFFFFFF8u;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* or result */
    _fk = 0x5E31286Du; /* flag setter tag */
    esi++;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x0AB4FBC0u; /* flag setter tag */

loc_0007CA4D: ;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    MEM32(ebp + 8) = esi;
    if (CMP_L(_fas, _fbs)) goto loc_0007CA66; /* jl: less (signed <) */

loc_0007CA54: ;
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x86F8765Eu; /* flag setter tag */

loc_0007CA55: ;
    ecx = MEM32(ebp + -4);
    esi = (uint32_t)(int32_t)SMEM16(ecx);
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(2)) >> 32) & 1);
    ecx = ecx + 2;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x44CB81EDu; /* flag setter tag */
    _fb = (uint32_t)(esi) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edx) + (uint64_t)(esi)) >> 32) & 1);
    edx = edx + esi;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xFBEC69C9u; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(ebp + -4) = ecx;
    if ((_fa != 0)) goto loc_0007CA55; /* jne: not equal / not zero */

loc_0007CA66: ;
    eax = ebx + ebx;
    _fb = (uint32_t)(3) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(3)) >> 32) & 1);
    eax = eax + 3;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x2877B844u; /* flag setter tag */
    _cf = 0; /* logical op clears CF */
    eax = eax & 0xFFFFFFFCu;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* and result */
    _fk = 0xCC56F0C3u; /* flag setter tag */
    esi = edx + edi;
    g_eflags = (g_eflags & ~1u) | ((0) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0007CA77u); sub_00145C40(); /* call 0x00145C40 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0007CA77: ;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x711257E0u; /* flag setter tag */
    eax = esp;
    MEM32(ebp + -12) = eax;
    if (CMP_LE(_fas, _fbs)) goto loc_0007CA9F; /* jle: less or equal (signed <=) */

loc_0007CA80: ;
    edx = ebx;

loc_0007CA82: ;
    SET_LO16(edi, MEM16(esi));
    ecx = MEM32(ebp + 0xC);
    MEM16(ecx) = LO16(edi);
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(2)) >> 32) & 1);
    ecx = ecx + 2;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x44CB81EDu; /* flag setter tag */
    MEM16(eax) = 0;
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(2)) >> 32) & 1);
    esi = esi + 2;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xA55A5CD4u; /* flag setter tag */
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(2)) >> 32) & 1);
    eax = eax + 2;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xB2C38A2Eu; /* flag setter tag */
    edx--;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x53CDE559u; /* flag setter tag */
    MEM32(ebp + 0xC) = ecx;
    if ((_fa != 0)) goto loc_0007CA82; /* jne: not equal / not zero */

loc_0007CA9F: ;
    eax = MEM32(ebp + 8);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    edi = MEM32(ebp + 0x10);
    if (CMP_NE(_fa, _fb)) goto loc_0007CAB1; /* jne: not equal / not zero */

loc_0007CAA9: ;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x8C6ED7C1u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0007CBEC; /* je: equal / zero */

loc_0007CAB1: ;
    PUSH32(esp, 0x12);
    PUSH32(esp, 4);
    ecx = ebp + -24;
    PUSH32(esp, ecx);
    ecx = ebp + -1068;
    MEM32(ebp + -24) = 0;
    MEM32(ebp + -20) = esi;
    MEM32(ebp + -1068) = 0x17CA68;
    g_eflags = (g_eflags & ~1u) | ((CMP_B(_fa, _fb)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0007CAD8u); sub_00079B80(); /* call 0x00079B80 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0007CAD8: ;
    eax = MEM32(ebp + 8);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x11A6DAE2u; /* flag setter tag */
    if (CMP_LE(_fas, _fbs)) goto loc_0007CB4E; /* jle: less or equal (signed <=) */

loc_0007CADF: ;
    MEM32(ebp + -4) = eax;

loc_0007CAE2: ;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x711257E0u; /* flag setter tag */
    edi = MEM32(ebp + -8);
    esi = MEM32(ebp + -12);
    if (CMP_LE(_fas, _fbs)) goto loc_0007CB43; /* jle: less or equal (signed <=) */

loc_0007CAEC: ;
    MEM32(ebp + 0xC) = ebx;
    /* nop */

loc_0007CAF0: ;
    edx = ebp + -24;
    PUSH32(esp, edx);
    ecx = ebp + -1068;
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x496F2313u ? (_cf) : CMP_B(_fa, _fb))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0007CAFFu); sub_00079CD0(); /* call 0x00079CD0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0007CAFF: ;
    ebx = eax;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(3) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 3 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x05BF071Fu; /* flag setter tag */
    if (CMP_G(_fas, _fbs)) goto loc_0007CB0E; /* jg: greater (signed >) */

loc_0007CB06: ;
    _fb = (uint32_t)(0xFFFFFFFEu) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ebx) + (uint64_t)(0xFFFFFFFEu)) >> 32) & 1);
    ebx = ebx + 0xFFFFFFFEu;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x9764AAB5u; /* flag setter tag */
    _fb = (uint32_t)(LO16(ebx)) & 0xFFFFu; _fbs = (int32_t)(int16_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(MEM16(esi)) + (uint64_t)(LO16(ebx))) >> 16) & 1);
    MEM16(esi) = MEM16(esi) + LO16(ebx);
    _fa = (uint32_t)(MEM16(esi)) & 0xFFFFu; _fas = (int32_t)(int16_t)(_fa); /* add result */
    _fk = 0xEB1460C7u; /* flag setter tag */
    goto loc_0007CB2B;

loc_0007CB0E: ;
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(ebx) < (uint32_t)(2));
    ebx = ebx - 2;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0x9CE29A46u; /* flag setter tag */
    PUSH32(esp, ebx);
    ecx = ebp + -24;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0007CB1Au); sub_00079A30(); /* call 0x00079A30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0007CB1A: ;
    edx = 1;
    ecx = ebx;
    if (((LO8(ecx)) & 31u)) _cf = (int)(((uint32_t)(edx) >> ((32 - (((LO8(ecx)) & 31u))) & 31u)) & 1u);
    edx = (uint32_t)(edx) << ((LO8(ecx)) & 31u);
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0xEEDF4F9Fu; /* flag setter tag */
    edx--;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x53CDE559u; /* flag setter tag */
    edx = ~edx;
    _cf = 0; /* logical op clears CF */
    eax = eax ^ edx;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x4AE6919Eu; /* flag setter tag */
    _fb = (uint32_t)(LO16(eax)) & 0xFFFFu; _fbs = (int32_t)(int16_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(MEM16(esi)) + (uint64_t)(LO16(eax))) >> 16) & 1);
    MEM16(esi) = MEM16(esi) + LO16(eax);
    _fa = (uint32_t)(MEM16(esi)) & 0xFFFFu; _fas = (int32_t)(int16_t)(_fa); /* add result */
    _fk = 0x545ADF3Eu; /* flag setter tag */

loc_0007CB2B: ;
    SET_LO16(eax, MEM16(esi));
    _fb = (uint32_t)(LO16(eax)) & 0xFFFFu; _fbs = (int32_t)(int16_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(MEM16(edi)) + (uint64_t)(LO16(eax))) >> 16) & 1);
    MEM16(edi) = MEM16(edi) + LO16(eax);
    _fa = (uint32_t)(MEM16(edi)) & 0xFFFFu; _fas = (int32_t)(int16_t)(_fa); /* add result */
    _fk = 0x71216C18u; /* flag setter tag */
    eax = MEM32(ebp + 0xC);
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(esi) + (uint64_t)(2)) >> 32) & 1);
    esi = esi + 2;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xA55A5CD4u; /* flag setter tag */
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(2)) >> 32) & 1);
    edi = edi + 2;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x2F0B870Du; /* flag setter tag */
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    MEM32(ebp + 0xC) = eax;
    if ((_fa != 0)) goto loc_0007CAF0; /* jne: not equal / not zero */

loc_0007CB40: ;
    ebx = MEM32(ebp + -28);

loc_0007CB43: ;
    MEM32(ebp + -4) = MEM32(ebp + -4) - 1;
    _fa = (uint32_t)(MEM32(ebp + -4)) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0xEA22A653u; /* flag setter tag */
    if ((_fa != 0)) goto loc_0007CAE2; /* jne: not equal / not zero */

loc_0007CB48: ;
    edi = MEM32(ebp + 0x10);
    eax = MEM32(ebp + 8);

loc_0007CB4E: ;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp edi, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x8C6ED7C1u; /* flag setter tag */
    if (CMP_EQ(_fa, _fb)) goto loc_0007CBEC; /* je: equal / zero */

loc_0007CB56: ;
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x86F8765Eu; /* flag setter tag */
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fb = (uint32_t)(8) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp eax, 8 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x1F215C34u; /* flag setter tag */
    if (CMP_NE(_fa, _fb)) goto loc_0007CB7B; /* jne: not equal / not zero */

loc_0007CB5C: ;
    eax = MEM32(ebp + -32);
    ecx = MEM32(ebp + -36);
    PUSH32(esp, 0);
    PUSH32(esp, edi);
    eax++;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x80000000u); /* inc result/SF/OF; CF unchanged */
    _fk = 0x86F8765Eu; /* flag setter tag */
    PUSH32(esp, eax);
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0007CB6Cu); sub_0007CA00(); /* call 0x0007CA00 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0007CB6C: ;
    esp = ebp + -1080;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_ebp = ebp; g_seh_ebp = ebp; esp += 16; return; /* ret 12 */

loc_0007CB7B: ;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu; _fb = (uint32_t)(0) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp ebx, 0 (32-bit) */
    _cf = 0; /* nothing borrows from zero */
    _fk = 0x711257E0u; /* flag setter tag */
    ecx = MEM32(ebp + -12);
    MEM32(ebp + 8) = ecx;
    if (CMP_LE(_fas, _fbs)) goto loc_0007CBEC; /* jle: less or equal (signed <=) */

loc_0007CB85: ;
    edx = ebp + -24;
    PUSH32(esp, edx);
    ecx = ebp + -1068;
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x711257E0u ? (CMP_B(_fa, _fb)) : _cf)) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0007CB94u); sub_00079CD0(); /* call 0x00079CD0 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0007CB94: ;
    esi = eax;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fb = (uint32_t)(3) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fbs = (int32_t)(int32_t)(_fb); /* cmp esi, 3 (32-bit) */
    _cf = (int)(_fa < _fb);
    _fk = 0x72925CE7u; /* flag setter tag */
    if (CMP_G(_fas, _fbs)) goto loc_0007CBAF; /* jg: greater (signed >) */

loc_0007CB9B: ;
    ecx = MEM32(ebp + -8);
    eax = MEM32(ebp + 8);
    _cf = 0; /* xor clears CF */
    edx = 0; /* xor self */
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x0296481Bu; /* flag setter tag */
    SET_LO16(edx, MEM16(ecx));
    _fb = (uint32_t)(MEM16(eax)) & 0xFFFFu; _fbs = (int32_t)(int16_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(LO16(edx)) + (uint64_t)(MEM16(eax))) >> 16) & 1);
    SET_LO16(edx, LO16(edx) + MEM16(eax));
    _fa = (uint32_t)(LO16(edx)) & 0xFFFFu; _fas = (int32_t)(int16_t)(_fa); /* add result */
    _fk = 0x79199D0Cu; /* flag setter tag */
    edx = edx + esi + -2;
    goto loc_0007CBD7;

loc_0007CBAF: ;
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* sub source, before the write */
    _cf = (int)((uint32_t)(esi) < (uint32_t)(2));
    esi = esi - 2;
    _fa = (uint32_t)(esi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* sub result */
    _fk = 0xEBCFC1BEu; /* flag setter tag */
    PUSH32(esp, esi);
    ecx = ebp + -24;
    g_eflags = (g_eflags & ~1u) | ((_cf) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; /* frame stays current across calls */
    g_seh_ebp = ebp;
    PUSH32(esp, 0x0007CBBBu); sub_00079A30(); /* call 0x00079A30 */
    _cf = g_eflags & 1u; /* callee carry */
    _fk = 0x00000000u; /* flag setter tag */

loc_0007CBBB: ;
    edx = eax;
    ecx = esi;
    eax = 1;
    if (((LO8(ecx)) & 31u)) _cf = (int)(((uint32_t)(eax) >> ((32 - (((LO8(ecx)) & 31u))) & 31u)) & 1u);
    eax = (uint32_t)(eax) << ((LO8(ecx)) & 31u);
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* shift result */
    _fk = 0x2C308138u; /* flag setter tag */
    ecx = MEM32(ebp + -8);
    eax--;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0x496F2313u; /* flag setter tag */
    eax = ~eax;
    _cf = 0; /* logical op clears CF */
    edx = edx ^ eax;
    _fa = (uint32_t)(edx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* xor result */
    _fk = 0x18348E51u; /* flag setter tag */
    _fb = (uint32_t)(MEM16(ecx)) & 0xFFFFu; _fbs = (int32_t)(int16_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(LO16(edx)) + (uint64_t)(MEM16(ecx))) >> 16) & 1);
    SET_LO16(edx, LO16(edx) + MEM16(ecx));
    _fa = (uint32_t)(LO16(edx)) & 0xFFFFu; _fas = (int32_t)(int16_t)(_fa); /* add result */
    _fk = 0x4DF02351u; /* flag setter tag */
    eax = MEM32(ebp + 8);
    _fb = (uint32_t)(MEM16(eax)) & 0xFFFFu; _fbs = (int32_t)(int16_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(LO16(edx)) + (uint64_t)(MEM16(eax))) >> 16) & 1);
    SET_LO16(edx, LO16(edx) + MEM16(eax));
    _fa = (uint32_t)(LO16(edx)) & 0xFFFFu; _fas = (int32_t)(int16_t)(_fa); /* add result */
    _fk = 0x79199D0Cu; /* flag setter tag */

loc_0007CBD7: ;
    MEM16(edi) = LO16(edx);
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(eax) + (uint64_t)(2)) >> 32) & 1);
    eax = eax + 2;
    _fa = (uint32_t)(eax) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0xB2C38A2Eu; /* flag setter tag */
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(ecx) + (uint64_t)(2)) >> 32) & 1);
    ecx = ecx + 2;
    _fa = (uint32_t)(ecx) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x44CB81EDu; /* flag setter tag */
    _fb = (uint32_t)(2) & 0xFFFFFFFFu; _fbs = (int32_t)(int32_t)(_fb); /* add source, before the write */
    _cf = (int)((((uint64_t)(edi) + (uint64_t)(2)) >> 32) & 1);
    edi = edi + 2;
    _fa = (uint32_t)(edi) & 0xFFFFFFFFu; _fas = (int32_t)(int32_t)(_fa); /* add result */
    _fk = 0x2F0B870Du; /* flag setter tag */
    ebx--;
    _fa = (uint32_t)(ebx) & 0xFFFFFFFFu;
    _fas = (int32_t)(int32_t)(_fa); _fb = (_fa == 0x7FFFFFFFu); /* dec result/SF/OF; CF unchanged */
    _fk = 0xF6219CEAu; /* flag setter tag */
    MEM32(ebp + 8) = eax;
    MEM32(ebp + -8) = ecx;
    if ((_fa != 0)) goto loc_0007CB85; /* jne: not equal / not zero */

loc_0007CBEC: ;
    esp = ebp + -1080;
    POP32(esp, edi);
    POP32(esp, esi);
    POP32(esp, ebx);
    esp = ebp;
    POP32(esp, ebp);
    g_eflags = (g_eflags & ~1u) | (((_fk == 0x8C6ED7C1u ? (CMP_B(_fa, _fb)) : (_fk == 0x711257E0u ? (CMP_B(_fa, _fb)) : _cf))) ? 0x1u : 0u); /* flags across call boundary (carry-only opt) */
    g_ebp = ebp; g_seh_ebp = ebp; esp += 16; return; /* ret 12 */

}
