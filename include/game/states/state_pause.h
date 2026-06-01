#ifndef STATE_PAUSE_H
#define STATE_PAUSE_H

#include <SDL.h>

#include "libpixtools/pixtools.h"
#include "variables/globalAppState.h"

void pause_event_handler(const SDL_Event *e);
void pause_action_per_tick(PixContext *ctx);
void pause_draw(PixContext *ctx);

#endif // STATE_PAUSE_H
