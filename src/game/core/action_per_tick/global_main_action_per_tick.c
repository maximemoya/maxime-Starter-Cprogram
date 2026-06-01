#include "game/core/action_per_tick/global_main_action_per_tick.h"

#include "game/core/state_table.h"

#define ACTION_TICK_INTERVAL_MS 32

static Uint32 lastActionTime = 0;

void global_main_action_per_tick(PixContext *ctx)
{
    // global ACTION PER TICK — fixed-step gate shared by every state's tick.
    Uint32 now = SDL_GetTicks();
    if ((now - lastActionTime) < (Uint32)ACTION_TICK_INTERVAL_MS)
        return;
    lastActionTime = now;

    // specific ACTION PER TICK — vtable dispatch (NULL for terminal states).
    const PhaseVTable *vt = app_state_vtable(app_state);
    if (vt)
        vt->action_per_tick(ctx);
}
