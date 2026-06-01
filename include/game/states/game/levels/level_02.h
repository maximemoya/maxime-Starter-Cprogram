#ifndef LEVEL_02_H
#define LEVEL_02_H

#include <SDL.h>

#include "libpixtools/pixtools.h"
#include "variables/globalSprites.h"

void level_02_event_handler(const SDL_Event *e);
void level_02_action_per_tick(PixContext *ctx);
void level_02_draw(PixContext *ctx);

#endif // LEVEL_02_H
