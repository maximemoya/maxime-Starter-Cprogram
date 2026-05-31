#include <stdbool.h>
#include <stdlib.h>

#include <SDL.h>

int main(void)
{
    // Boundary check: SDL is an external API (RULES.md §3) — validate its returns.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow(
        "myApplication",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Boundary check (RULES.md §3): we asked for PRESENTVSYNC, but the driver may
    // grant a renderer without it (software fallback, some drivers). RULES.md §7
    // forbids SDL_Delay in the loop *because* VSync paces it — that premise only
    // holds if VSync is actually active. Confirm it; if it isn't, we must pace the
    // loop ourselves or it busy-spins at 100% CPU (worst case on the RG35XXH).
    SDL_RendererInfo info;
    // On query failure we can't know — assume no VSync so the fallback cap is on
    // (degraded path is safe; a false busy-loop is not).
    bool has_vsync = (SDL_GetRendererInfo(renderer, &info) == 0) &&
                     ((info.flags & SDL_RENDERER_PRESENTVSYNC) != 0);
    if (has_vsync)
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "VSync active: loop paced by the display");
    else
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "VSync unavailable: pacing the loop with a frame cap");

    // ~60 FPS target for the fallback path only (RULES.md §7 exception).
    const Uint32 FRAME_TIME_MS = 16;

    bool running = true;
    SDL_Event event;

    while (running)
    {
        Uint64 frame_start = SDL_GetTicks64();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        // Pace the loop only when VSync is absent — never when it does the job (RULES.md §7).
        if (!has_vsync)
        {
            Uint64 elapsed = SDL_GetTicks64() - frame_start;
            if (elapsed < FRAME_TIME_MS)
                SDL_Delay(FRAME_TIME_MS - (Uint32)elapsed);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
