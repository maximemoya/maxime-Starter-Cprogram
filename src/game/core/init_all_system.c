#include "game/core/init_all_system.h"

#include "libpixtools/pixtools.h"

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
    return ctx;
}
