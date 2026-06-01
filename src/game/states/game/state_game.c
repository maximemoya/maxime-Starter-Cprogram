#include <stdbool.h>

#include "game/states/state_game.h"
#include "game/states/game/levels/level_01.h"
#include "game/states/game/levels/level_02.h"
#include "game/states/game/levels/level_03.h"

#include "game/states/input_state/input_state.h"

#include "game/core/event_handler/input_mapping/input_mapping.h"

// STATICS

static LevelId current_level = LEVEL_01;

static void onBackToMenu(void)
{
    global_input_state_resetInputState();
    app_state = STATE_MENU;
}

static void onPauseGame(void)
{
    global_input_state_resetInputState();
    app_state = STATE_PAUSE;
}

static void onCycleLevel(void)
{
    current_level = (current_level + 1) % LEVEL_COUNT;
    global_input_state_resetInputState();
}

// FUNCTIONS

void game_reset_current_level(void)
{
    current_level = LEVEL_01;
}

void game_event_handler(const SDL_Event *e)
{

    // global EVENT HANDLER

    // Delegate input tracking to the shared module
    global_input_state_update_pressed_keys(e);

    // KEYBOARD — state transitions
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_BACKSPACE)
        onBackToMenu();
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_p)
        onPauseGame();
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_n)
        onCycleLevel();

    // JOYSTICK RG35XXH — state transitions (touches mappées)
    if (input_mapping_is_press(e, MAPPING_LABEL_Y))
        onBackToMenu();
    if (input_mapping_is_press(e, MAPPING_LABEL_SELECT))
        onPauseGame();
    if (input_mapping_is_press(e, MAPPING_LABEL_R1))
        onCycleLevel();

    // specific EVENT HANDLER
    switch (current_level)
    {
    case LEVEL_01:
        level_01_event_handler(e);
        break;
    case LEVEL_02:
        level_02_event_handler(e);
        break;
    case LEVEL_03:
        level_03_event_handler(e);
        break;
    default:
        break;
    }
}

void game_action_per_tick(PixContext *ctx)
{
    switch (current_level)
    {
    case LEVEL_01:
        level_01_action_per_tick(ctx);
        break;
    case LEVEL_02:
        level_02_action_per_tick(ctx);
        break;
    case LEVEL_03:
        level_03_action_per_tick(ctx);
        break;
    default:
        break;
    }
}

void game_draw(PixContext *ctx)
{
    switch (current_level)
    {
    case LEVEL_01:
        level_01_draw(ctx);
        break;
    case LEVEL_02:
        level_02_draw(ctx);
        break;
    case LEVEL_03:
        level_03_draw(ctx);
        break;
    default:
        break;
    }
}
