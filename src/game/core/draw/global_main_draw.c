#include "game/core/draw/global_main_draw.h"

#include "game/core/state_table.h"

void global_main_draw(PixContext *ctx)
{
    // specific DRAW — vtable dispatch (NULL for terminal states).
    const PhaseVTable *vt = app_state_vtable(app_state);
    if (vt)
        vt->draw(ctx);

    // global DRAW end — single present for the whole frame.
    pix_draw(ctx);
}
