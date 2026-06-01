#include <stdio.h>
#include <stdbool.h>

#include "game/states/game/levels/level_03.h"

#include "game/entities/player/player.h"

#include "libpixtools/world_scrolling_system/world_scroll.h"

// STATICS

#define PLAYER_W 32
#define PLAYER_H 32
#define PLAYER_SCREEN_X 120
#define JUMP_VY (-20)
#define GRAVITY 2
#define MAX_VY 24

#define SPEED_START_X100 400 // 4 px/tick
#define SPEED_MAX_X100 1600  // 16 px/tick
#define SPEED_ACCEL_EVERY 8  // ticks between accel ticks
#define SPEED_ACCEL_DELTA 6

static bool initialised = false;
static int camera_x = 0;
static int scroll_speed_x100 = SPEED_START_X100;
static Player player;
static int tick_counter = 0;
static char bufString[32];

static void ensureInit(void)
{
    if (initialised)
        return;
    world_scroll_init(0x1E5E1103u);
    camera_x = 0;
    scroll_speed_x100 = SPEED_START_X100;
    player_init(&player, PLAYER_SCREEN_X, 200);
    tick_counter = 0;
    initialised = true;
}

// FUNCTIONS

void level_03_event_handler(const SDL_Event *e)
{
    // Jump: keyboard SPACE or mapped A (parity + remappable, via player module).
    if (player_jump_pressed(e))
        player_jump(&player, JUMP_VY);
}

void level_03_action_per_tick(PixContext *ctx)
{
    (void)ctx;
    ensureInit();

    camera_x += scroll_speed_x100 / 100;

    tick_counter++;
    if (tick_counter % SPEED_ACCEL_EVERY == 0 && scroll_speed_x100 < SPEED_MAX_X100)
        scroll_speed_x100 += SPEED_ACCEL_DELTA;

    int world_px = camera_x + PLAYER_SCREEN_X + PLAYER_W / 2;
    int ground_top = world_scroll_ground_top_y(world_px);
    player_apply_gravity(&player, GRAVITY, MAX_VY, ground_top, PLAYER_H);
}

void level_03_draw(PixContext *ctx)
{
    ensureInit();

    world_scroll_draw(ctx, camera_x,
                      0xFF2A1840u,  // twilight purple
                      0xFFFF6A40u,  // sunset
                      0xFF20221Au); // shadow ground

    pix_add_sprite8_scale(ctx, PLAYER_SCREEN_X, player.y, 16, 16, spritePlayer_256, 2);

    pix_add_string_scale(ctx, 8, 8, "LV03 RUNNER", 2.0f, PIX_WHITE, PIX_BLACK, 2);
    snprintf(bufString, sizeof(bufString), "SCORE:%d", camera_x / 10);
    pix_add_string_scale(ctx, 8, 36, bufString, 2.0f, PIX_WHITE, PIX_BLACK, 2);
    snprintf(bufString, sizeof(bufString), "SPD:%d", scroll_speed_x100 / 100);
    pix_add_string_scale(ctx, 8, 64, bufString, 2.0f, PIX_WHITE, PIX_BLACK, 2);
}
