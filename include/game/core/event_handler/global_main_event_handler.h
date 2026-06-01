#ifndef GLOBAL_MAIN_EVENT_HANDLER_H
#define GLOBAL_MAIN_EVENT_HANDLER_H

#include "libpixtools/pixtools.h"

/**
 * Poll and dispatch all pending SDL events for the current app state.
 */
void global_main_event_handler(PixContext *ctx);

#endif // GLOBAL_MAIN_EVENT_HANDLER_H
