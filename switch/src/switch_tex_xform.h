#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SwitchTexXform {
    bool active;
    float mv[16];
    float tex_mat[2][16];
    uint32_t stage_ctrl[2]; // bits 0..1: uv_src, bit 2: has_matrix, bits 4..5: gen_kind (0..3)
    uint32_t normalize_normals;
} SwitchTexXform;

void Switch_SetPendingTexXform(const SwitchTexXform *xf);
bool Switch_ConsumePendingTexXform(SwitchTexXform *out);

#ifdef __cplusplus
}
#endif
