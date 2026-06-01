#ifndef STATE_MAPPING_H
#define STATE_MAPPING_H

#include <SDL.h>

#include "libpixtools/pixtools.h"
#include "variables/globalAppState.h"

/**
 * Enter the key-mapping screen: load persisted bindings or fall back to defaults.
 */
void mapping_init(void);

void mapping_event_handler(const SDL_Event *e);
void mapping_action_per_tick(PixContext *ctx);
void mapping_draw(PixContext *ctx);

#endif // STATE_MAPPING_H
