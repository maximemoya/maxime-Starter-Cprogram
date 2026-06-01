#ifndef GLOBAL_MAIN_ACTION_PER_TICK_H
#define GLOBAL_MAIN_ACTION_PER_TICK_H

#include "libpixtools/pixtools.h"

/**
 * Advance the current app state's simulation by one fixed tick (rate-limited).
 */
void global_main_action_per_tick(PixContext *ctx);

#endif // GLOBAL_MAIN_ACTION_PER_TICK_H
