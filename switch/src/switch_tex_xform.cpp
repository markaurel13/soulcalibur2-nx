#include "switch_tex_xform.h"
#include <string.h>

static _Thread_local SwitchTexXform s_pending_xf;

void Switch_SetPendingTexXform(const SwitchTexXform *xf) {
    if (xf && xf->active) {
        s_pending_xf = *xf;
    } else {
        s_pending_xf.active = false;
    }
}

bool Switch_ConsumePendingTexXform(SwitchTexXform *out) {
    if (!s_pending_xf.active) {
        return false;
    }
    if (out) {
        *out = s_pending_xf;
    }
    s_pending_xf.active = false;
    return true;
}
