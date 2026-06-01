# Modular main.c — Design Spec

## Context

`src/main.c` currently holds all SDL plumbing inline. The goal is to make it a minimal caller that delegates to named modules, matching the pseudocode skeleton already present as comments. This follows RULES.md §2 (one module / one responsibility) and §3 (PixContext as encapsulation unit).

---

## Architecture

### New files

| Path | Purpose |
|------|---------|
| `include/pix/pix.h` | `PixContext` typedef + `pix_init` / `pix_quit` prototypes |
| `src/pix/pix.c` | SDL lifecycle — init window + renderer, cleanup |
| `include/game/game_main.h` | `game_event_handler` / `game_on_tick_action` / `game_draw` prototypes |
| `src/game/game_main.c` | Game loop stubs (black clear, SDL_QUIT handling) |

### Modified file

`src/main.c` — reduced to ~20 lines.

---

## PixContext struct (`include/pix/pix.h`)

```c
typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    bool          quit;
} PixContext;

bool pix_init(PixContext *ctx);
void pix_quit(PixContext *ctx);
```

---

## pix module (`src/pix/pix.c`)

`pix_init` validates every SDL call (boundary per RULES.md §3):
- `SDL_Init(SDL_INIT_VIDEO)`
- `SDL_CreateWindow` (640×480, centered, shown)
- `SDL_CreateRenderer` (ACCELERATED | PRESENTVSYNC)

Returns `false` and logs on any failure, cleaning up what was already allocated. Sets `ctx->quit = false` on success.

`pix_quit` calls `SDL_DestroyRenderer`, `SDL_DestroyWindow`, `SDL_Quit`.

---

## game module (`src/game/game_main.c`)

Stubs — enough to compile and run cleanly:

- `game_main_event_handler(PixContext *ctx, SDL_Event *e)`: polls `SDL_PollEvent` in a loop; sets `ctx->quit = true` on `SDL_QUIT`.
- `game_main_on_tick_action(PixContext *ctx)`: empty for now.
- `game_main_draw(PixContext *ctx)`: `SDL_SetRenderDrawColor` black + `SDL_RenderClear` + `SDL_RenderPresent`.

---

## main.c after refactor

```c
#include <stdlib.h>

#include "libpixtools/pixtools.h"

#include "game/core/init_all_system.h"
#include "game/core/event_handler/global_main_event_handler.h"
#include "game/core/action_per_tick/global_main_action_per_tick.h"
#include "game/core/draw/global_main_draw.h"

int main(void)
{

    // ----------------------------------
    // --- INIT -------------------------
    // ----------------------------------

    SDL_Log("--- Program Start ---");
    PixContext *ctx = init_all_system();
    if (!ctx)
        return EXIT_FAILURE;

    // ----------------------------------
    // --- MAIN -------------------------
    // ----------------------------------

    SDL_Log("Entering Main Loop...");
    while (!ctx->quit)
    {
        global_main_event_handler(ctx);
        global_main_action_per_tick(ctx);
        global_main_draw(ctx);
    }

    // ----------------------------------
    // --- END --------------------------
    // ----------------------------------

    pix_show_exit_message(ctx, "FERMETURE DU PROGRAMME");
    pix_close(ctx);
    return EXIT_SUCCESS;
}
```

---

## Verification

```sh
make -f makefile.mac run    # window opens, black screen, closes on quit
make -f makefile.mac lint   # cppcheck + clang-tidy must stay clean
make -f makefile.mac asan   # sanitizer run (close window manually)
```
