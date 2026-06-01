#include "game/states/state_mapping.h"

#include "game/core/event_handler/input_mapping/input_mapping.h"

#include "libpixtools/joystick_rg35xxh/joystick_rg35xxh_button_enum.h"

// STATICS

static void onBackToMenu(void)
{
    app_state = STATE_MENU;
}

// FUNCTIONS

void mapping_init(void)
{
    // Prefer the user's saved bindings; fall back to factory defaults.
    if (!input_mapping_load(CONSOLE_MAPPING_LOG_PATH))
        input_mapping_set_defaults();
}

void mapping_event_handler(const SDL_Event *e)
{
    // KEYBOARD — back to menu
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_BACKSPACE)
        onBackToMenu();

    // JOYSTICK RG35XXH — touche mappée pour B
    if (input_mapping_is_press(e, MAPPING_LABEL_B))
        onBackToMenu();
}

void mapping_action_per_tick(PixContext *ctx)
{
    (void)ctx;
}

void mapping_draw(PixContext *ctx)
{
    pix_add_string_scale(ctx, 8, 8, "CONFIG TOUCHES", 2.5f, PIX_WHITE, PIX_BLACK, 4);

    // Two columns listing every label and its current binding.
    char buf[32];
    for (int i = 0; i < MAPPING_LABEL_COUNT; i++)
    {
        int col = i / 11;
        int row = i % 11;
        int x = 16 + col * 300;
        int y = 70 + row * 34;

        input_mapping_to_string(global_console_mapping[i], buf, sizeof(buf));
        pix_add_string_scale(ctx, x, y, input_mapping_label_name((MappingLabel)i), 1.5f, PIX_WHITE, PIX_BLACK, 1);
        pix_add_string_scale(ctx, x + 150, y, buf, 1.5f, PIX_GRAY, PIX_BLACK, 1);
    }

    pix_add_string_scale(ctx, 8, 448, "B / RETOUR : MENU", 2.0f, PIX_GRAY, PIX_BLACK, 2);
}
