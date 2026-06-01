#include "game/states/state_game.h"
#include "game/states/game/levels/level_01.h"
#include "game/states/game/levels/level_02.h"
#include "game/states/game/levels/level_03.h"

#include "game/core/phase_vtable.h"
#include "game/core/event_handler/input_mapping/input_mapping.h"

#include "game/states/input_state/input_state.h"

// STATICS

// Same triad, one tier down: levels are dispatched through a table exactly like
// app states. Adding a level = one row here (plus its LevelId enum value).
static const PhaseVTable LEVELS[LEVEL_COUNT] = {
    [LEVEL_01] = {level_01_event_handler, level_01_action_per_tick, level_01_draw},
    [LEVEL_02] = {level_02_event_handler, level_02_action_per_tick, level_02_draw},
    [LEVEL_03] = {level_03_event_handler, level_03_action_per_tick, level_03_draw},
};

// Per-level reset, parallel to LEVELS. Reset is a lifecycle hook, not a per-frame
// phase, so it lives beside the triad table rather than inside PhaseVTable.
// Adding a level = one more row here too.
static void (*const LEVEL_RESETS[LEVEL_COUNT])(void) = {
    [LEVEL_01] = level_01_reset,
    [LEVEL_02] = level_02_reset,
    [LEVEL_03] = level_03_reset,
};

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
    LEVEL_RESETS[current_level](); // enter the level fresh, not where we left it
    global_input_state_resetInputState();
}

// FUNCTIONS

void game_reset_current_level(void)
{
    current_level = LEVEL_01;
    LEVEL_RESETS[current_level](); // "NOUVELLE PARTIE" starts level 01 from scratch
}

void game_event_handler(const SDL_Event *e)
{
    // global EVENT HANDLER — raw input is pumped once by global_main_event_handler.

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

    // specific EVENT HANDLER — current level
    LEVELS[current_level].event_handler(e);
}

void game_action_per_tick(PixContext *ctx)
{
    LEVELS[current_level].action_per_tick(ctx);
}

void game_draw(PixContext *ctx)
{
    LEVELS[current_level].draw(ctx);
}
