#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include <SDL.h>
#include <stdbool.h>

// Shared player entity for every level. `world_x` is the horizontal position in
// whatever coordinate system the level uses (world-space for scrolling levels,
// screen-space for the auto-scroller); `y` is always screen-space vertical.
typedef struct
{
    int  world_x;
    int  y;
    int  vy;
    bool on_ground;
} Player;

/**
 * Initialise position; velocity is zeroed and on_ground cleared.
 */
void player_init(Player *p, int world_x, int y);

/**
 * True when `e` is the "jump" edge — keyboard SPACE or the mapped A button.
 * Goes through input_mapping so it respects the user's remapping (RULES.md DRY).
 */
bool player_jump_pressed(const SDL_Event *e);

/**
 * Apply an upward jump impulse, but only when grounded.
 */
void player_jump(Player *p, int jump_vy);

/**
 * Integrate gravity for one tick: accelerate vy (clamped to max_vy), advance y,
 * then resolve collision against `ground_top` for a sprite of height player_h,
 * updating on_ground accordingly.
 */
void player_apply_gravity(Player *p, int gravity, int max_vy, int ground_top, int player_h);

#endif // GAME_PLAYER_H
