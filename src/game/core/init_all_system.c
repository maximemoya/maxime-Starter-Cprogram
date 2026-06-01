#include "game/core/init_all_system.h"

#include "libpixtools/pixtools.h"

#include "game/core/event_handler/input_mapping/input_mapping.h"
#include "game/states/input_state/input_state.h"
#include "game/states/state_menu.h"
#include "variables/globalSprites.h"

PixContext *init_all_system(void)
{
    SDL_Log("Initializing PixContext...");
    // DO NOT CHANGE THIS ! Initialize a 640x480 virtual screen scaled up 1x
    PixContext *ctx = pix_init("Pixel Art Library Test", SCREEN_WIDTH, SCREEN_HEIGHT, 1);
    if (!ctx)
    {
        SDL_Log("Failed to initialize PixContext!");
        return NULL;
    }

    // Input bindings: load the user's saved mapping, else factory defaults.
    if (!input_mapping_load(CONSOLE_MAPPING_LOG_PATH))
        input_mapping_set_defaults();

    // Resolved directional state + menu + generated sprites.
    global_input_state_initInputState();
    menu_init_menuState();
    globalSprites_init();

    return ctx;
}
