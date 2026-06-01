#include <stdio.h>
#include <stdbool.h>

#include "game/states/game/levels/level_02.h"

#include "game/states/input_state/input_state.h"

#include "libpixtools/joystick_rg35xxh/joystick_rg35xxh_button_enum.h"

#include "world_scrolling_system/world_scroll.h"

// STATICS

#define PLAYER_W 32
#define PLAYER_H 32
#define WALK_SPEED 5
#define JUMP_VY (-18)
#define GRAVITY 2
#define MAX_VY 22

static bool initialised = false;
static int camera_x = 0;
static int player_world_x = 80;
static int player_y = 200;
static int player_vy = 0;
static bool on_ground = false;
static char bufString[32];

static void ensureInit(void)
{
    if (initialised)
        return;
    world_scroll_init(0x1E5E1102u);
    camera_x = 0;
    player_world_x = 80;
    player_y = 200;
    player_vy = 0;
    on_ground = false;
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

void level_02_event_handler(const SDL_Event *e)
{
    // KEYBOARD — actions
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_SPACE)
        onJump();

    // JOYSTICK RG35XXH — actions
    if (e->type == SDL_JOYBUTTONDOWN && e->jbutton.button == JOYSTICK_RG35XXH_A)
        onJump();
}

void level_02_action_per_tick(PixContext *ctx)
{
    (void)ctx;
    ensureInit();

    if (global_input_state.left)
        player_world_x -= WALK_SPEED;
    if (global_input_state.right)
        player_world_x += WALK_SPEED;

    player_vy += GRAVITY;
    if (player_vy > MAX_VY)
        player_vy = MAX_VY;
    player_y += player_vy;

    int ground_top = world_scroll_ground_top_y(player_world_x + PLAYER_W / 2);
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

    camera_x = player_world_x - SCREEN_WIDTH / 3;
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

    int screen_px = player_world_x - camera_x;
    pix_add_sprite8_scale(ctx, screen_px, player_y, 16, 16, spritePlayer_256, 2);

    pix_add_string_scale(ctx, 8, 8, "LV02 PLATFORMER", 2.0f, PIX_WHITE, PIX_BLACK, 2);
    snprintf(bufString, sizeof(bufString), "X:%d", player_world_x);
    pix_add_string_scale(ctx, 8, 36, bufString, 2.0f, PIX_WHITE, PIX_BLACK, 2);
}
