#include <stdio.h>

#include "game/states/game/levels/level_01.h"

#include "game/states/input_state/input_state.h"

#include "libpixtools/world_scrolling_system/world_scroll.h"

// STATICS

#define PLAYER_W 32 // 16 sprite x scale 2
#define PLAYER_H 32
#define PLAYER_SPEED 6
#define CAMERA_STEP 4

static bool initialised = false;
static int camera_x = 0;
static int player_x = 80;
static int player_y = 220;
static char bufString[32];

static void ensureInit(void)
{
    if (initialised)
        return;
    world_scroll_init(0x1E5E1101u);
    camera_x = 0;
    player_x = 80;
    player_y = 220;
    initialised = true;
}

// FUNCTIONS

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
        player_y -= PLAYER_SPEED;
    if (global_input_state.down)
        player_y += PLAYER_SPEED;
    if (global_input_state.left)
        player_x -= PLAYER_SPEED;
    if (global_input_state.right)
        player_x += PLAYER_SPEED;

    if (player_x < 0)
        player_x = 0;
    if (player_x > SCREEN_WIDTH - PLAYER_W)
        player_x = SCREEN_WIDTH - PLAYER_W;
    if (player_y < 0)
        player_y = 0;

    int ground_top = world_scroll_ground_top_y(camera_x + player_x + PLAYER_W / 2);
    if (player_y + PLAYER_H > ground_top)
        player_y = ground_top - PLAYER_H;
}

void level_01_draw(PixContext *ctx)
{
    ensureInit();

    world_scroll_draw(ctx, camera_x,
                      0xFF1A1A4Fu,  // deep night sky
                      0xFFFFA060u,  // orange horizon
                      0xFF3E2A1Au); // dark soil

    pix_add_sprite8_scale(ctx, player_x, player_y, 16, 16, spritePlayer_256, 2);

    pix_add_string_scale(ctx, 8, 8, "LV01 SHOOTER", 2.0f, PIX_WHITE, PIX_BLACK, 2);
    snprintf(bufString, sizeof(bufString), "X:%d", camera_x);
    pix_add_string_scale(ctx, 8, 36, bufString, 2.0f, PIX_WHITE, PIX_BLACK, 2);
}
