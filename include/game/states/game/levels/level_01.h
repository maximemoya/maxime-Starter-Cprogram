#ifndef LEVEL_01_H
#define LEVEL_01_H

#include <SDL.h>

#include "libpixtools/pixtools.h"
#include "variables/globalSprites.h"

void level_01_reset(void);
void level_01_event_handler(const SDL_Event *e);
void level_01_action_per_tick(PixContext *ctx);
void level_01_draw(PixContext *ctx);

#endif // LEVEL_01_H
