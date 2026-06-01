#ifndef LEVEL_03_H
#define LEVEL_03_H

#include <SDL.h>

#include "libpixtools/pixtools.h"
#include "variables/globalSprites.h"

void level_03_event_handler(const SDL_Event *e);
void level_03_action_per_tick(PixContext *ctx);
void level_03_draw(PixContext *ctx);

#endif // LEVEL_03_H
