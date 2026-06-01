#include "libpixtools/pixtools.h"
#include "libpixtools/pixletters/pixletters.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// STATICS

// File-scope log handle, shared by log_output_function and log_close
static FILE *pix_log_file = NULL;

static void log_close(void)
{
    if (pix_log_file)
    {
        fclose(pix_log_file);
        pix_log_file = NULL;
    }
}

/**
 * Custom log function to write everything to a file with timestamps.
 * This ensures we can see logs on handheld consoles without a terminal.
 */
static void log_output_function(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
    (void)userdata;
    (void)category;
    (void)priority;

    // Get current time
    time_t now = time(NULL);
    const struct tm *t = localtime(&now);
    char timestamp[25];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

    if (!pix_log_file)
        pix_log_file = fopen("pixel_art.log", "a");

    if (pix_log_file)
    {
        fprintf(pix_log_file, "[%s] %s\n", timestamp, message);
        fflush(pix_log_file);
    }
    // Also print to standard output (useful for Mac terminal)
    printf("[%s] %s\n", timestamp, message);
}

// Alpha-blend src over dst (both 0xAARRGGBB), result is opaque. `a` is the src
// alpha. Single source of truth for the blend math (RULES.md DRY) — uses >>8
// (÷256), the fast/standard variant shared by every primitive below.
static uint32_t blend_argb(uint32_t src, uint32_t dst, uint8_t a)
{
    uint32_t sr = (src >> 16) & 0xFF;
    uint32_t sg = (src >> 8) & 0xFF;
    uint32_t sb = src & 0xFF;
    uint32_t dr = (dst >> 16) & 0xFF;
    uint32_t dg = (dst >> 8) & 0xFF;
    uint32_t db = dst & 0xFF;

    uint32_t r = ((sr * a) + (dr * (255 - a))) >> 8;
    uint32_t g = ((sg * a) + (dg * (255 - a))) >> 8;
    uint32_t b = ((sb * a) + (db * (255 - a))) >> 8;

    return 0xFF000000u | (r << 16) | (g << 8) | b;
}

// FUNCTIONS

PixContext *pix_init(const char *title, int width, int height, int scale)
{
    // Register custom log function first
    SDL_LogSetOutputFunction(log_output_function, NULL);

    SDL_Log("--- NEW SESSION ---");
    SDL_Log("pix_init: Initializing SDL...");
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    // Open first available joystick if any (for RG35XXH). Keep the handle so
    // pix_close can close exactly this device (SDL_JoystickFromInstanceID takes
    // an instance id, not a device index — confusing the two leaks the handle).
    SDL_Log("pix_init: Checking for joysticks...");
    SDL_Joystick *joystick = NULL;
    if (SDL_NumJoysticks() > 0)
    {
        SDL_Log("pix_init: Opening joystick 0");
        joystick = SDL_JoystickOpen(0);
    }

    SDL_Log("pix_init: Creating Window...");
    PixContext *ctx = malloc(sizeof(PixContext));
    if (!ctx)
        return NULL;

    ctx->width = width;
    ctx->height = height;
    ctx->quit = false;
    ctx->joystick = joystick;

    ctx->window = SDL_CreateWindow(title,
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   width * scale,
                                   height * scale,
                                   SDL_WINDOW_SHOWN);

    if (!ctx->window)
    {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        free(ctx);
        return NULL;
    }

    // Nearest-neighbour scaling for pixel art (no blur). Must be set BEFORE the
    // renderer/texture are created — the hint is read at texture-creation time.
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    SDL_Log("pix_init: Creating Renderer...");
    // Enable VSync for smooth movement and eliminate tearing
    ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ctx->renderer)
    {
        // Headless fallback (SDL_VIDEODRIVER=dummy in CI has no GPU): retry with the
        // software renderer so self-test / sanitizer runs can still init and present.
        SDL_Log("pix_init: accelerated renderer unavailable (%s); falling back to software", SDL_GetError());
        ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_SOFTWARE);
    }
    if (!ctx->renderer)
    {
        SDL_DestroyWindow(ctx->window);
        free(ctx);
        return NULL;
    }

    SDL_Log("pix_init: Creating Texture...");
    ctx->texture = SDL_CreateTexture(ctx->renderer,
                                     SDL_PIXELFORMAT_ARGB8888,
                                     SDL_TEXTUREACCESS_STREAMING,
                                     width, height);
    if (!ctx->texture)
    {
        fprintf(stderr, "Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(ctx->renderer);
        SDL_DestroyWindow(ctx->window);
        free(ctx);
        return NULL;
    }

    SDL_Log("pix_init: Allocating pixel buffer...");
    ctx->pixels = malloc((size_t)width * (size_t)height * sizeof(uint32_t));
    if (!ctx->pixels)
    {
        fprintf(stderr, "Pixel buffer allocation failed (width=%d, height=%d)\n", width, height);
        SDL_DestroyTexture(ctx->texture);
        SDL_DestroyRenderer(ctx->renderer);
        SDL_DestroyWindow(ctx->window);
        free(ctx);
        return NULL;
    }
    memset(ctx->pixels, 0, (size_t)width * (size_t)height * sizeof(uint32_t));

    ctx->blend_enabled = false;

    SDL_Log("pix_init: Success!");
    return ctx;
}

void pix_set_blend(PixContext *ctx, bool enabled)
{
    ctx->blend_enabled = enabled;
}

void pix_clear(PixContext *ctx, uint32_t color)
{
    for (int i = 0; i < ctx->width * ctx->height; i++)
    {
        ctx->pixels[i] = color;
    }
}

void pix_add_one(PixContext *ctx, int x, int y, uint32_t color)
{
    if (x < 0 || x >= ctx->width || y < 0 || y >= ctx->height)
        return;

    int index = y * ctx->width + x;

    if (ctx->blend_enabled)
    {
        uint8_t src_a = (color >> 24) & 0xFF;
        if (src_a == 0)
            return;
        if (src_a == 255)
        {
            ctx->pixels[index] = color;
            return;
        }
        ctx->pixels[index] = blend_argb(color, ctx->pixels[index], src_a);
    }
    else
    {
        ctx->pixels[index] = color;
    }
}

void pix_add_letter(PixContext *ctx, int x, int y, char c, uint32_t text_color, uint32_t bg_color)
{
    const char *letter = mappingCharToPixelLetter(c);
    for (int row = 0; row < PIX_LETTER_SIZE_HEIGHT; row++)
    {
        unsigned char bits = (unsigned char)letter[row];
        for (int col = 0; col < 8; col++)
        {
            uint32_t color = (bits & (1 << (7 - col))) ? text_color : bg_color;
            if ((color >> 24) != 0) // Only draw if not fully transparent
                pix_add_one(ctx, x + col, y + row, color);
        }
    }
}

void pix_add_letter_scale(PixContext *ctx, int x, int y, char c, float scale, uint32_t text_color, uint32_t bg_color)
{
    const char *letter = mappingCharToPixelLetter(c);
    for (int row = 0; row < PIX_LETTER_SIZE_HEIGHT; row++)
    {
        unsigned char bits = (unsigned char)letter[row];
        for (int col = 0; col < 8; col++)
        {
            uint32_t color = (bits & (1 << (7 - col))) ? text_color : bg_color;
            if ((color >> 24) != 0) // Only draw if not fully transparent
            {
                int x0 = (int)(col * scale);
                int x1 = (int)((col + 1) * scale);
                int y0 = (int)(row * scale);
                int y1 = (int)((row + 1) * scale);
                pix_add_rect_full(ctx, x + x0, y + y0, x1 - x0, y1 - y0, color);
            }
        }
    }
}

void pix_add_string_scale(PixContext *ctx, int x, int y, const char *s, float scale, uint32_t text_color, uint32_t bg_color, int padding)
{
    int len = strlen(s);
    if (len == 0)
        return;

    // 1. Draw background box if not transparent
    if ((bg_color >> 24) != 0)
    {
        int text_w = (int)(len * 8 * scale);
        int text_h = (int)(8 * scale);
        pix_add_rect_full(ctx, x - padding, y - padding, text_w + 2 * padding, text_h + 2 * padding, bg_color);
    }

    // 2. Draw letters (with transparent background since we already drew the box)
    for (int i = 0; s[i] != '\0'; i++)
    {
        pix_add_letter_scale(ctx, x + (int)(i * 8 * scale), y, s[i], scale, text_color, 0x00000000);
    }
}

void pix_add_hline(PixContext *ctx, int x, int y, int w, uint32_t color)
{
    if (y < 0 || y >= ctx->height)
        return;
    int x1 = x, x2 = x + w;
    if (x1 < 0)
        x1 = 0;
    if (x2 > ctx->width)
        x2 = ctx->width;
    if (x1 >= x2)
        return;

    uint32_t *row = &ctx->pixels[y * ctx->width + x1];
    int length = x2 - x1;

    uint8_t a = (color >> 24) & 0xFF;
    if (ctx->blend_enabled && a < 255)
    {
        if (a == 0)
            return;
        for (int i = 0; i < length; i++)
            row[i] = blend_argb(color, row[i], a);
    }
    else
    {
        for (int i = 0; i < length; i++)
            row[i] = color;
    }
}

void pix_add_vline(PixContext *ctx, int x, int y, int h, uint32_t color)
{
    if (x < 0 || x >= ctx->width)
        return;
    int y1 = y, y2 = y + h;
    if (y1 < 0)
        y1 = 0;
    if (y2 > ctx->height)
        y2 = ctx->height;
    if (y1 >= y2)
        return;

    uint32_t *pixel = &ctx->pixels[y1 * ctx->width + x];
    int length = y2 - y1;

    uint8_t a = (color >> 24) & 0xFF;
    if (ctx->blend_enabled && a < 255)
    {
        if (a == 0)
            return;
        for (int i = 0; i < length; i++)
        {
            *pixel = blend_argb(color, *pixel, a);
            pixel += ctx->width;
        }
    }
    else
    {
        for (int i = 0; i < length; i++)
        {
            *pixel = color;
            pixel += ctx->width;
        }
    }
}

void pix_add_rect_full(PixContext *ctx, int x, int y, int w, int h, uint32_t color)
{
    int x1 = x, y1 = y, x2 = x + w, y2 = y + h;
    if (x1 < 0)
        x1 = 0;
    if (y1 < 0)
        y1 = 0;
    if (x2 > ctx->width)
        x2 = ctx->width;
    if (y2 > ctx->height)
        y2 = ctx->height;
    if (x1 >= x2 || y1 >= y2)
        return;

    uint8_t a = (color >> 24) & 0xFF;
    if (ctx->blend_enabled && a < 255)
    {
        if (a == 0)
            return;
        for (int i = y1; i < y2; i++)
        {
            uint32_t *row = &ctx->pixels[i * ctx->width + x1];
            int row_width = x2 - x1;
            for (int j = 0; j < row_width; j++)
                row[j] = blend_argb(color, row[j], a);
        }
    }
    else
    {
        for (int i = y1; i < y2; i++)
        {
            uint32_t *row = &ctx->pixels[i * ctx->width + x1];
            int row_width = x2 - x1;
            for (int j = 0; j < row_width; j++)
                row[j] = color;
        }
    }
}

void pix_add_rect_border(PixContext *ctx, int x, int y, int w, int h, uint32_t color)
{
    if (w <= 0 || h <= 0)
        return;

    // Use optimized line drawing functions
    pix_add_hline(ctx, x, y, w, color);         // Top
    pix_add_hline(ctx, x, y + h - 1, w, color); // Bottom
    pix_add_vline(ctx, x, y, h, color);         // Left
    pix_add_vline(ctx, x + w - 1, y, h, color); // Right
}

bool pix_save_raw(const char *path, const uint32_t *pixels, int w, int h)
{
    FILE *f = fopen(path, "wb");
    if (!f)
        return false;

    const size_t pixel_count = (size_t)w * (size_t)h;
    // Save dimensions first, then pixel data — check every write (disk full, SD ejected, ...).
    if (fwrite(&w, sizeof(int), 1, f) != 1 || fwrite(&h, sizeof(int), 1, f) != 1 || fwrite(pixels, sizeof(uint32_t), pixel_count, f) != pixel_count)
    {
        fclose(f);
        return false;
    }

    fclose(f);
    return true;
}

uint32_t *pix_load_raw(const char *path, int *w, int *h)
{
    FILE *f = fopen(path, "rb");
    if (!f)
        return NULL;

    if (fread(w, sizeof(int), 1, f) != 1)
    {
        fclose(f);
        return NULL;
    }
    if (fread(h, sizeof(int), 1, f) != 1)
    {
        fclose(f);
        return NULL;
    }

    // Bounds check: reject malformed/malicious files (untrusted input from disk).
    // Cap at 4096x4096 = 64 MB max allocation.
    if (*w <= 0 || *h <= 0 || *w > 4096 || *h > 4096)
    {
        fclose(f);
        return NULL;
    }

    const size_t pixel_count = (size_t)*w * (size_t)*h;
    // Bound check above (4096x4096) ensures pixel_count fits safely; analyzer can't trace it.
    uint32_t *pixels = malloc(pixel_count * sizeof(uint32_t)); // NOLINT(clang-analyzer-optin.taint.TaintedAlloc)
    if (!pixels)
    {
        fclose(f);
        return NULL;
    }

    if (fread(pixels, sizeof(uint32_t), pixel_count, f) != pixel_count)
    {
        free(pixels);
        fclose(f);
        return NULL;
    }

    fclose(f);
    return pixels;
}

void pix_add_sprite(PixContext *ctx, int x, int y, int w, int h, const uint32_t *sprite_pixels)
{
    // 1. Clipping: Calculate the visible portion of the sprite
    int x1 = x, y1 = y;
    int x2 = x + w, y2 = y + h;

    if (x1 < 0)
        x1 = 0;
    if (y1 < 0)
        y1 = 0;
    if (x2 > ctx->width)
        x2 = ctx->width;
    if (y2 > ctx->height)
        y2 = ctx->height;

    // Early exit if sprite is entirely off-screen
    if (x1 >= x2 || y1 >= y2)
        return;

    int sprite_x_offset = x1 - x;
    int sprite_y_offset = y1 - y;
    int visible_w = x2 - x1;
    int visible_h = y2 - y1;

    // 2. Optimized Row-by-Row loop
    for (int i = 0; i < visible_h; i++)
    {
        uint32_t *dst_row = &ctx->pixels[(y1 + i) * ctx->width + x1];
        const uint32_t *src_row = &sprite_pixels[(sprite_y_offset + i) * w + sprite_x_offset];

        for (int j = 0; j < visible_w; j++)
        {
            uint32_t color = src_row[j];
            uint32_t a = (color >> 24) & 0xFF;

            if (a == 0)
                continue; // Skip fully transparent pixels

            if (ctx->blend_enabled && a < 255)
            {
                dst_row[j] = blend_argb(color, dst_row[j], (uint8_t)a);
            }
            else
            {
                // Opaque copy
                dst_row[j] = color;
            }
        }
    }
}

void pix_add_sprite8(PixContext *ctx, int x, int y, int w, int h, const uint8_t *sprite_pixels)
{
    // 1. Clipping: Calculate the visible portion of the sprite
    int x1 = x, y1 = y;
    int x2 = x + w, y2 = y + h;

    if (x1 < 0)
        x1 = 0;
    if (y1 < 0)
        y1 = 0;
    if (x2 > ctx->width)
        x2 = ctx->width;
    if (y2 > ctx->height)
        y2 = ctx->height;

    // Early exit if sprite is entirely off-screen
    if (x1 >= x2 || y1 >= y2)
        return;

    int sprite_x_offset = x1 - x;
    int sprite_y_offset = y1 - y;
    int visible_w = x2 - x1;
    int visible_h = y2 - y1;

    // 2. Optimized Row-by-Row loop
    for (int i = 0; i < visible_h; i++)
    {
        uint32_t *dst_row = &ctx->pixels[(y1 + i) * ctx->width + x1];
        const uint8_t *src_row = &sprite_pixels[(sprite_y_offset + i) * w + sprite_x_offset];

        for (int j = 0; j < visible_w; j++)
        {
            uint8_t v = src_row[j];

            if (v == 0)
                continue; // Skip transparent pixels

            // Convert grayscale value v to ARGB: (Alpha 255) | (R: v) | (G: v) | (B: v)
            uint32_t color = (0xFF000000) | (v << 16) | (v << 8) | v;

            // We use simple opaque copy for now as per user request (0 = transparent, others = opaque grayscale)
            // If blending is needed later, we can add it here.
            dst_row[j] = color;
        }
    }
}

void pix_add_sprite8_scale(PixContext *ctx, int x, int y, int w, int h, const uint8_t *sprite_pixels, int scale)
{
    if (scale <= 0)
        return;

    for (int sy = 0; sy < h; sy++)
    {
        for (int sx = 0; sx < w; sx++)
        {
            uint8_t v = sprite_pixels[sy * w + sx];
            if (v == 0)
                continue; // Transparent

            uint32_t color = (0xFF000000) | (v << 16) | (v << 8) | v;
            pix_add_rect_full(ctx, x + (sx * scale), y + (sy * scale), scale, scale, color);
        }
    }
}

void pix_draw(PixContext *ctx)
{
    static bool first_draw = true;
    if (first_draw)
    {
        SDL_Log("pix_draw: First draw call successful");
        first_draw = false;
    }
    SDL_UpdateTexture(ctx->texture, NULL, ctx->pixels, ctx->width * sizeof(uint32_t));
    SDL_RenderClear(ctx->renderer);
    SDL_RenderCopy(ctx->renderer, ctx->texture, NULL, NULL);
    SDL_RenderPresent(ctx->renderer);
}

void pix_close(PixContext *ctx)
{
    if (!ctx)
        return;

    log_close();

    // Close exactly the joystick we opened in pix_init (if any).
    if (ctx->joystick && SDL_JoystickGetAttached(ctx->joystick))
        SDL_JoystickClose(ctx->joystick);

    free(ctx->pixels);
    SDL_DestroyTexture(ctx->texture);
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
    SDL_Quit();
    free(ctx);
}

void pix_show_exit_message(PixContext *ctx, const char *message)
{
    // ctx is an internal invariant here (main already validated it). assert, no silent if.
    assert(ctx);

    SDL_Log("Closing application: %s", message);

    // Draw 3 times to ensure all hardware buffers (double/triple buffering) are updated
    for (int i = 0; i < 3; i++)
    {
        // 1. Clear screen to black
        pix_clear(ctx, PIX_BLACK);
        const float scale = 2.0f;

        // 2. Add message centered
        int text_width = strlen(message) * 8 * scale;
        int x = (ctx->width - text_width) / 2;
        int y = (ctx->height - 12) / 2;

        pix_add_string_scale(ctx, x, y, message, scale, PIX_WHITE, PIX_BLACK, 0);

        // 3. Draw and Present
        pix_draw(ctx);
    }

    // 4. Short delay to let the user see the message (reduced for optimization)
    SDL_Delay(200);
}
