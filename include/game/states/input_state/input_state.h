#ifndef INPUT_STATE_H
#define INPUT_STATE_H

#include <stdbool.h>
#include <SDL.h>

// Resolved directional state synthesised from all input sources
// (keyboard arrows + D-pad hat + both analog sticks).
// Read after calling global_input_state_update_pressed_keys().
typedef struct
{
    bool up;
    bool down;
    bool left;
    bool right;
} InputState;

extern InputState global_input_state;

/**
 * Zero-initialise the global input state.
 */
void global_input_state_initInputState(void);

/**
 * Process a single SDL event and update the internal raw tracking state.
 * Update control pressed keys arrow / hat / joysticks
 * Also recomputes the resolved up/down/left/right booleans.
 *
 * Call this from your state's event handler for every event polled.
 */
void global_input_state_update_pressed_keys(const SDL_Event *e);

/**
 * Reset the internal raw tracking state AND the resolved directional booleans.
 *
 * Call this when changing app state to prevent stale directional input
 * from leaking across state transitions.
 */
void global_input_state_resetInputState(void);

#endif // INPUT_STATE_H
