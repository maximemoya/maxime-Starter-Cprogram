#ifndef PHASE_VTABLE_H
#define PHASE_VTABLE_H

#include <SDL.h>

#include "libpixtools/pixtools.h"

// The universal per-frame triad. Every dispatchable unit (app state, game level,
// future sub-entities) exposes these three phases, in this fixed order. A table
// of these makes the triad self-enforcing: you cannot register a unit without
// providing all three phases, and adding one is a single row, not three switches.
typedef struct
{
    void (*event_handler)(const SDL_Event *e);
    void (*action_per_tick)(PixContext *ctx);
    void (*draw)(PixContext *ctx);
} PhaseVTable;

#endif // PHASE_VTABLE_H
