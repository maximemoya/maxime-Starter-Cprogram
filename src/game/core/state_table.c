#include "game/core/state_table.h"

#include "game/states/state_menu.h"
#include "game/states/state_game.h"
#include "game/states/state_pause.h"
#include "game/states/state_mapping.h"

// STATICS

// One row per playable AppState. Adding a state = one line here (plus its enum
// value); you cannot omit a phase without leaving a visible NULL. STATE_QUIT is
// terminal and intentionally absent — app_state_vtable returns NULL for it.
static const PhaseVTable STATES[] = {
    [STATE_MENU]    = {menu_event_handler,    menu_action_per_tick,    menu_draw},
    [STATE_GAME]    = {game_event_handler,    game_action_per_tick,    game_draw},
    [STATE_PAUSE]   = {pause_event_handler,   pause_action_per_tick,   pause_draw},
    [STATE_MAPPING] = {mapping_event_handler, mapping_action_per_tick, mapping_draw},
};

// FUNCTIONS

const PhaseVTable *app_state_vtable(AppState state)
{
    if (state < 0 || state >= (int)(sizeof(STATES) / sizeof(STATES[0])))
        return NULL;
    return &STATES[state];
}
