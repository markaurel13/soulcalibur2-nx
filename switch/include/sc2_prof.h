/* Perfilador de funciones guest (muestreo). Solo capa de plataforma.
 * Cada funcion recompilada publica su direccion al entrar y restaura la
 * anterior al salir (cleanup), de modo que un hilo muestreador obtiene el
 * tiempo "propio" (self) por funcion guest sin bloquear el pipeline. */
#ifndef SC2_PROF_H
#define SC2_PROF_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
extern volatile uint32_t sc2_prof_cur;
#ifdef __cplusplus
}
#endif

#ifdef SC2_FUNC_PROFILE
static inline uint32_t sc2_prof_enter(uint32_t a) {
    uint32_t p = sc2_prof_cur;
    sc2_prof_cur = a;
    return p;
}
static inline void sc2_prof_leave(uint32_t *p) { sc2_prof_cur = *p; }
#define SC2_PROF_SCOPE(a) \
    uint32_t _sc2_pp __attribute__((cleanup(sc2_prof_leave))) = sc2_prof_enter(a)
#else
#define SC2_PROF_SCOPE(a) ((void)0)
#endif

#endif
