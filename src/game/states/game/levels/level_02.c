#include <stdio.h>
#include <stdbool.h>

#include "game/states/game/levels/level_02.h"

#include "game/states/input_state/input_state.h"
#include "game/entities/player/player.h"

#include "libpixtools/world_scrolling_system/world_scroll.h"

// STATICS

#define PLAYER_W 32
#define PLAYER_H 32
#define WALK_SPEED 5
#define JUMP_VY (-18)
#define GRAVITY 2
#define MAX_VY 22

static bool initialised = false;
static int camera_x = 0;
static Player player;
static char bufString[32];

static void ensureInit(void)
{
    if (initialised)
        return;
    world_scroll_init(0x1E5E1102u);
    camera_x = 0;
    player_init(&player, 80, 200);
    initialised = true;
}

// FUNCTIONS

void level_02_event_handler(const SDL_Event *e)
{
    // Jump: keyboard SPACE or mapped A (parity + remappable, via player module).
    if (player_jump_pressed(e))
        player_jump(&player, JUMP_VY);
}

void level_02_action_per_tick(PixContext *ctx)
{
    (void)ctx;
    ensureInit();

    if (global_input_state.left)
        player.world_x -= WALK_SPEED;
    if (global_input_state.right)
        player.world_x += WALK_SPEED;
    if (player.world_x < 0)
        player.world_x = 0;

    int ground_top = world_scroll_ground_top_y(player.world_x + PLAYER_W / 2);
    player_apply_gravity(&player, GRAVITY, MAX_VY, ground_top, PLAYER_H);

    camera_x = player.world_x - SCREEN_WIDTH / 3;
    if (camera_x < 0)
        camera_x = 0;
}

void level_02_draw(PixContext *ctx)
{
    ensureInit();

    world_scroll_draw(ctx, camera_x,
                      0xFF87CEEBu,  // sky blue
                      0xFFE6F2FFu,  // pale horizon
                      0xFF3A7A28u); // grass green

    int screen_px = player.world_x - camera_x;
    pix_add_sprite8_scale(ctx, screen_px, player.y, 16, 16, spritePlayer_256, 2);

    pix_add_string_scale(ctx, 8, 8, "LV02 PLATFORMER", 2.0f, PIX_WHITE, PIX_BLACK, 2);
    snprintf(bufString, sizeof(bufString), "X:%d", player.world_x);
    pix_add_string_scale(ctx, 8, 36, bufString, 2.0f, PIX_WHITE, PIX_BLACK, 2);
}
