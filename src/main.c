#include <stdlib.h>

#include <SDL.h>

#include "game/core/init_all_system.h"
#include "game/core/event_handler/global_main_event_handler.h"
#include "game/core/action_per_tick/global_main_action_per_tick.h"
#include "game/core/draw/global_main_draw.h"
#include "libpixtools/pixtools.h"

int main(void)
{
    // ----------------------------------
    // --- INIT -------------------------
    // ----------------------------------

    SDL_Log("--- Program Start ---");
    PixContext *ctx = init_all_system();
    if (!ctx)
        return EXIT_FAILURE;

    // Self-test mode (RULES.md §3 boundary: env var = external input). When SELFTEST
    // is set, run a fixed number of frames then exit cleanly instead of looping until
    // quit — gives ASan/UBSan a full init→loop→teardown path headlessly in CI
    // (pair with SDL_VIDEODRIVER=dummy). 0 keeps normal interactive mode.
    const Uint64 SELFTEST_FRAME_BUDGET = 60;
    const Uint64 selftest_frames = (SDL_getenv("SELFTEST") != NULL) ? SELFTEST_FRAME_BUDGET : 0;
    Uint64 frame_count = 0;

    // ----------------------------------
    // --- MAIN -------------------------
    // ----------------------------------

    SDL_Log("Entering Main Loop...");
    while (!ctx->quit)
    {
        global_main_event_handler(ctx);
        global_main_action_per_tick(ctx);
        global_main_draw(ctx);

        if (selftest_frames != 0 && ++frame_count >= selftest_frames)
            ctx->quit = true;
    }

    // ----------------------------------
    // --- END --------------------------
    // ----------------------------------

    pix_show_exit_message(ctx, "FERMETURE DU PROGRAMME");
    pix_close(ctx);
    return EXIT_SUCCESS;
}
