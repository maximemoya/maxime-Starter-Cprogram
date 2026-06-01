#include "game/states/input_state/input_state.h"

// Resolved directional state, recomputed every event by global_input_state_update_pressed_keys().
InputState global_input_state = {0};
