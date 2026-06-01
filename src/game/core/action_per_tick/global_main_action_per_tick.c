#include "game/core/action_per_tick/global_main_action_per_tick.h"

#include "game/states/state_menu.h"
#include "game/states/state_game.h"
#include "game/states/state_pause.h"
#include "game/states/state_mapping.h"

#define ACTION_TICK_INTERVAL_MS 32

static Uint32 lastActionTime = 0;

void global_main_action_per_tick(PixContext *ctx)
{

    // global ACTION PER TICK
    Uint32 now = SDL_GetTicks();
    if ((now - lastActionTime) < (Uint32)ACTION_TICK_INTERVAL_MS)
        return;
    lastActionTime = now;

    // specific ACTION PER TICK
    switch (app_state)
    {
    case STATE_MENU:
        menu_action_per_tick(ctx);
        break;
    case STATE_GAME:
        game_action_per_tick(ctx);
        break;
    case STATE_PAUSE:
        pause_action_per_tick(ctx);
        break;
    case STATE_MAPPING:
        mapping_action_per_tick(ctx);
        break;
    default:
        break;
    }
}
