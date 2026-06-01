#ifndef STATE_TABLE_H
#define STATE_TABLE_H

#include "game/core/phase_vtable.h"
#include "variables/globalAppState.h"

/**
 * Dispatch table for an AppState, or NULL for states with no per-frame behaviour
 * (e.g. STATE_QUIT) or an out-of-range value. The global loop consults this
 * instead of switching on app_state, so the menu/game/pause/mapping triads are
 * registered in exactly one place.
 */
const PhaseVTable *app_state_vtable(AppState state);

#endif // STATE_TABLE_H
