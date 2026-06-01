#include "game/states/state_pause.h"
#include "game/states/state_game.h"

#include "game/core/event_handler/input_mapping/input_mapping.h"

// STATICS

static void onResumeGame(void)
{
    app_state = STATE_GAME;
}

// FUNCTIONS

void pause_event_handler(const SDL_Event *e)
{
    // KEYBOARD
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_p)
        onResumeGame();

    // JOYSTICK RG35XXH (touche mappée pour SELECT)
    if (input_mapping_is_press(e, MAPPING_LABEL_SELECT))
        onResumeGame();
}

void pause_action_per_tick(PixContext *ctx)
{
    (void)ctx;
}

void pause_draw(PixContext *ctx)
{
    // Pause is an overlay: explicitly redraw the frozen game underneath (its tick
    // is not running, so the frame is static), then the banner on top. No longer
    // relies on stale pixel-buffer persistence.
    game_draw(ctx);
    pix_add_string_scale(ctx, 250, 220, "PAUSE", 4.0f, PIX_WHITE, PIX_BLACK, 8);
}
