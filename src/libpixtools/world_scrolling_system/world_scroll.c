#include "world_scroll/world_scroll.h"

#include <math.h>
#include <stdlib.h>

// STATICS

#define GROUND_BASE_Y 340
#define GROUND_MIN_Y  240
#define GROUND_MAX_Y  448

static uint32_t s_seed = 0x9E3779B9u;

static uint32_t hash32(uint32_t x)
{
    x ^= x >> 16;
    x *= 0x7feb352dU;
    x ^= x >> 15;
    x *= 0x846ca68bU;
    x ^= x >> 16;
    return x;
}

static uint32_t lerp_argb(uint32_t a, uint32_t b, float t)
{
    int ar = (int)((a >> 16) & 0xFF);
    int ag = (int)((a >> 8) & 0xFF);
    int ab = (int)(a & 0xFF);
    int br = (int)((b >> 16) & 0xFF);
    int bg = (int)((b >> 8) & 0xFF);
    int bb = (int)(b & 0xFF);
    int rr = ar + (int)((float)(br - ar) * t);
    int rg = ag + (int)((float)(bg - ag) * t);
    int rb = ab + (int)((float)(bb - ab) * t);
    return 0xFF000000u
         | ((uint32_t)(rr & 0xFF) << 16)
         | ((uint32_t)(rg & 0xFF) << 8)
         | ((uint32_t)(rb & 0xFF));
}

// FUNCTIONS

void world_scroll_init(uint32_t seed)
{
    s_seed = seed ? seed : 0x9E3779B9u;
    srand(s_seed);
}

int world_scroll_ground_top_y(int world_col)
{
    // Negative world columns mirror positive ones so left-of-spawn still has ground.
    int c = world_col < 0 ? -world_col : world_col;

    double a = sin((double)c * 0.027) * 34.0;
    double b = sin((double)c * 0.0131 + 1.7) * 20.0;
    double d = sin((double)c * 0.083 + 0.4) * 7.0;

    int perturb = (int)(hash32((uint32_t)c ^ s_seed) % 7) - 3;

    int y = GROUND_BASE_Y + (int)(a + b + d) + perturb;
    if (y < GROUND_MIN_Y) y = GROUND_MIN_Y;
    if (y > GROUND_MAX_Y) y = GROUND_MAX_Y;
    return y;
}

void world_scroll_draw(PixContext *ctx, int camera_x,
                       uint32_t sky_top, uint32_t sky_bottom,
                       uint32_t ground_color)
{
    // Sky gradient: 12 horizontal bands.
    const int bands = 12;
    int band_h = SCREEN_HEIGHT / bands;
    for (int i = 0; i < bands; i++)
    {
        float t = (float)i / (float)(bands - 1);
        uint32_t col = lerp_argb(sky_top, sky_bottom, t);
        pix_add_rect_full(ctx, 0, i * band_h, SCREEN_WIDTH, band_h, col);
    }

    // Ground: per-column vline from terrain top to screen bottom.
    // Top 3 px slightly brighter to read as a grass/edge line.
    uint32_t edge_color = lerp_argb(ground_color, 0xFFFFFFFF, 0.18f);
    for (int sx = 0; sx < SCREEN_WIDTH; sx++)
    {
        int wc = camera_x + sx;
        int gy = world_scroll_ground_top_y(wc);
        if (gy >= SCREEN_HEIGHT) continue;
        pix_add_vline(ctx, sx, gy, 3, edge_color);
        int rest = SCREEN_HEIGHT - (gy + 3);
        if (rest > 0)
            pix_add_vline(ctx, sx, gy + 3, rest, ground_color);
    }
}
