#ifndef STATE_GAME_H
#define STATE_GAME_H

#include <SDL.h>

#include "libpixtools/pixtools.h"
#include "variables/globalAppState.h"

// Playable levels. Order matches the level switches in state_game.c.
typedef enum
{
    LEVEL_01,
    LEVEL_02,
    LEVEL_03,
    LEVEL_COUNT
} LevelId;

void game_reset_current_level(void);
void game_event_handler(const SDL_Event *e);
void game_action_per_tick(PixContext *ctx);
void game_draw(PixContext *ctx);

#endif // STATE_GAME_H
