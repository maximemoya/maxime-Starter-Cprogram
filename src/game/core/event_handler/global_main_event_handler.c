#include "game/core/event_handler/global_main_event_handler.h"

#include "game/core/event_handler/input_mapping/input_mapping.h"
#include "game/core/state_table.h"
#include "game/states/input_state/input_state.h"

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

        // Raw input tracking, once for every state (no per-state duplication).
        global_input_state_update_pressed_keys(&e);

        // specific EVENT HANDLER — vtable dispatch (NULL for terminal states).
        const PhaseVTable *vt = app_state_vtable(app_state);
        if (vt)
            vt->event_handler(&e);
    }

    // A sub-handler can request app exit by switching to STATE_QUIT.
    if (app_state == STATE_QUIT)
        ctx->quit = true;
}
