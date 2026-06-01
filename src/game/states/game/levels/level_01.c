#include <stdio.h>

#include "game/states/game/levels/level_01.h"

#include "game/states/input_state/input_state.h"
#include "game/entities/player/player.h"

#include "libpixtools/world_scrolling_system/world_scroll.h"

// STATICS

#define PLAYER_W 32 // 16 sprite x scale 2
#define PLAYER_H 32
#define PLAYER_SPEED 6
#define CAMERA_STEP 4

static bool initialised = false;
static int camera_x = 0;
static Player player; // free-flight: world_x holds the screen-space x here
static char bufString[32];

static void ensureInit(void)
{
    if (initialised)
        return;
    world_scroll_init(0x1E5E1101u);
    camera_x = 0;
    player_init(&player, 80, 220);
    initialised = true;
}

// FUNCTIONS

void level_01_reset(void)
{
    // Re-arm lazy init; ensureInit() rebuilds camera/player on the next tick/draw.
    initialised = false;
}

void level_01_event_handler(const SDL_Event *e)
{
    (void)e;
}

void level_01_action_per_tick(PixContext *ctx)
{
    (void)ctx;
    ensureInit();

    camera_x += CAMERA_STEP;

    if (global_input_state.up)
        player.y -= PLAYER_SPEED;
    if (global_input_state.down)
        player.y += PLAYER_SPEED;
    if (global_input_state.left)
        player.world_x -= PLAYER_SPEED;
    if (global_input_state.right)
        player.world_x += PLAYER_SPEED;

    if (player.world_x < 0)
        player.world_x = 0;
    if (player.world_x > SCREEN_WIDTH - PLAYER_W)
        player.world_x = SCREEN_WIDTH - PLAYER_W;
    if (player.y < 0)
        player.y = 0;

    int ground_top = world_scroll_ground_top_y(camera_x + player.world_x + PLAYER_W / 2);
    if (player.y + PLAYER_H > ground_top)
        player.y = ground_top - PLAYER_H;
}

void level_01_draw(PixContext *ctx)
{
    ensureInit();

    world_scroll_draw(ctx, camera_x,
                      0xFF1A1A4Fu,  // deep night sky
                      0xFFFFA060u,  // orange horizon
                      0xFF3E2A1Au); // dark soil

    pix_add_sprite8_scale(ctx, player.world_x, player.y, 16, 16, spritePlayer_256, 2);

    pix_add_string_scale(ctx, 8, 8, "LV01 SHOOTER", 2.0f, PIX_WHITE, PIX_BLACK, 2);
    snprintf(bufString, sizeof(bufString), "X:%d", camera_x);
    pix_add_string_scale(ctx, 8, 36, bufString, 2.0f, PIX_WHITE, PIX_BLACK, 2);
}
