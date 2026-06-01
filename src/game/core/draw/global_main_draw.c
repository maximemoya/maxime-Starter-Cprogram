#include "game/core/draw/global_main_draw.h"

#include "game/states/state_menu.h"
#include "game/states/state_game.h"
#include "game/states/state_pause.h"

void global_main_draw(PixContext *ctx)
{
    // specific DRAW start

    switch (app_state)
    {
    case STATE_MENU:
        menu_draw(ctx);
        break;
    case STATE_GAME:
        game_draw(ctx);
        break;
    case STATE_PAUSE:
        pause_draw(ctx);
        break;
    }

    // global DRAW end

    pix_draw(ctx);
}
