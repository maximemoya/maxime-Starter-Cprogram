#include "game/states/state_menu.h"
#include "game/states/state_game.h"
#include "game/states/state_mapping.h"
#include "game/states/input_state/input_state.h"

#include "game/core/event_handler/input_mapping/input_mapping.h"

// STATICS

static MenuState menu_state = {0};

static const char *menu_items[MENU_ITEM_COUNT] = {
    "NOUVELLE PARTIE",
    "CONFIG TOUCHES",
    "QUITTER"};

static void onMenuConfirmAction(void)
{
    if (menu_state.selected == MENU_ITEM_NEW_GAME)
    {
        game_reset_current_level();
        global_input_state_resetInputState();
        app_state = STATE_GAME;
        return;
    }
    if (menu_state.selected == MENU_ITEM_MAPPING)
    {
        global_input_state_resetInputState();
        mapping_init();
        app_state = STATE_MAPPING;
        return;
    }
    if (menu_state.selected == MENU_ITEM_QUIT)
    {
        global_input_state_resetInputState();
        menu_state.quit_requested = true;
        return;
    }
    return;
}

// FUNCTIONS

void menu_init_menuState(void)
{
    menu_state.selected = MENU_ITEM_NEW_GAME;
    menu_state.quit_requested = false;
    menu_state.up_prev = false;
    menu_state.down_prev = false;
}

bool menu_should_quit(void)
{
    return menu_state.quit_requested;
}

void menu_event_handler(const SDL_Event *e)
{
    // Delegate input tracking to the shared module
    global_input_state_update_pressed_keys(e);

    // KEYBOARD — state transitions
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_RETURN)
    {
        onMenuConfirmAction();
    }
    // JOYSTICK RG35XXH — state transitions (touche mappée pour A)
    if (input_mapping_is_press(e, MAPPING_LABEL_A))
    {
        onMenuConfirmAction();
    }
}

void menu_action_per_tick(PixContext *ctx)
{
    (void)ctx;

    // Edge-detected directional navigation (one move per press)
    if (global_input_state.up && !menu_state.up_prev)
        menu_state.selected = (menu_state.selected - 1 + MENU_ITEM_COUNT) % MENU_ITEM_COUNT;
    if (global_input_state.down && !menu_state.down_prev)
        menu_state.selected = (menu_state.selected + 1) % MENU_ITEM_COUNT;

    menu_state.up_prev = global_input_state.up;
    menu_state.down_prev = global_input_state.down;
}

void menu_draw(PixContext *ctx)
{
    pix_add_sprite8_scale(ctx, 0, 0, 160, 120, spriteBackground01_19200, 4);

    // Titre
    pix_add_string_scale(ctx, 194, 100, "PIXEL ART GAME", 2.5f, PIX_WHITE, PIX_BLACK, 6);

    // Items
    for (int i = 0; i < MENU_ITEM_COUNT; i++)
    {
        int y = 280 + i * 70;
        if (i == menu_state.selected)
            pix_add_string_scale(ctx, 220, y, menu_items[i], 2.0f, PIX_WHITE, PIX_BLACK, 8);
        else
            pix_add_string_scale(ctx, 220, y, menu_items[i], 2.0f, PIX_GRAY, PIX_BLACK, 8);
    }
}
