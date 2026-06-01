#include <stdio.h>
#include <stdbool.h>

#include "game/states/game/levels/level_03.h"

#include "game/states/input_state/input_state.h"

#include "libpixtools/joystick_rg35xxh/joystick_rg35xxh_button_enum.h"

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
static int player_y = 200;
static int player_vy = 0;
static bool on_ground = false;
static int tick_counter = 0;
static char bufString[32];

static void ensureInit(void)
{
    if (initialised)
        return;
    world_scroll_init(0x1E5E1103u);
    camera_x = 0;
    scroll_speed_x100 = SPEED_START_X100;
    player_y = 200;
    player_vy = 0;
    on_ground = false;
    tick_counter = 0;
    initialised = true;
}

static void onJump(void)
{
    if (on_ground)
    {
        player_vy = JUMP_VY;
        on_ground = false;
    }
}

// FUNCTIONS

void level_03_event_handler(const SDL_Event *e)
{
    // KEYBOARD — actions
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_SPACE)
        onJump();

    // JOYSTICK RG35XXH — actions
    if (e->type == SDL_JOYBUTTONDOWN && e->jbutton.button == JOYSTICK_RG35XXH_A)
        onJump();
}

void level_03_action_per_tick(PixContext *ctx)
{
    (void)ctx;
    ensureInit();

    camera_x += scroll_speed_x100 / 100;

    tick_counter++;
    if (tick_counter % SPEED_ACCEL_EVERY == 0 && scroll_speed_x100 < SPEED_MAX_X100)
        scroll_speed_x100 += SPEED_ACCEL_DELTA;

    player_vy += GRAVITY;
    if (player_vy > MAX_VY)
        player_vy = MAX_VY;
    player_y += player_vy;

    int world_px = camera_x + PLAYER_SCREEN_X + PLAYER_W / 2;
    int ground_top = world_scroll_ground_top_y(world_px);
    if (player_y + PLAYER_H >= ground_top)
    {
        player_y = ground_top - PLAYER_H;
        player_vy = 0;
        on_ground = true;
    }
    else
    {
        on_ground = false;
    }
}

void level_03_draw(PixContext *ctx)
{
    ensureInit();

    world_scroll_draw(ctx, camera_x,
                      0xFF2A1840u,  // twilight purple
                      0xFFFF6A40u,  // sunset
                      0xFF20221Au); // shadow ground

    pix_add_sprite8_scale(ctx, PLAYER_SCREEN_X, player_y, 16, 16, spritePlayer_256, 2);

    pix_add_string_scale(ctx, 8, 8, "LV03 RUNNER", 2.0f, PIX_WHITE, PIX_BLACK, 2);
    snprintf(bufString, sizeof(bufString), "SCORE:%d", camera_x / 10);
    pix_add_string_scale(ctx, 8, 36, bufString, 2.0f, PIX_WHITE, PIX_BLACK, 2);
    snprintf(bufString, sizeof(bufString), "SPD:%d", scroll_speed_x100 / 100);
    pix_add_string_scale(ctx, 8, 64, bufString, 2.0f, PIX_WHITE, PIX_BLACK, 2);
}
