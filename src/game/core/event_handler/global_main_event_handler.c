#include "game/core/event_handler/global_main_event_handler.h"

#include "game/core/event_handler/input_mapping/input_mapping.h"

#include "game/states/state_menu.h"
#include "game/states/state_game.h"
#include "game/states/state_pause.h"
#include "game/states/state_mapping.h"

void global_main_event_handler(PixContext *ctx)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {

        // global EVENT HANDLER

        if (e.type == SDL_QUIT)
            ctx->quit = true;
        if (app_state != STATE_MAPPING)
        {
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                ctx->quit = true;
            if (input_mapping_is_press(&e, MAPPING_LABEL_START))
                ctx->quit = true;
        }

        // specific EVENT HANDLER

        switch (app_state)
        {
        case STATE_MENU:
            menu_event_handler(&e);
            break;
        case STATE_GAME:
            game_event_handler(&e);
            break;
        case STATE_PAUSE:
            pause_event_handler(&e);
            break;
        case STATE_MAPPING:
            mapping_event_handler(&e);
            break;
        default:
            break;
        }
    }

    if (menu_should_quit())
        ctx->quit = true;
}
