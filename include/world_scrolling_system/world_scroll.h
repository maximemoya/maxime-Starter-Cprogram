#ifndef WORLD_SCROLL_H
#define WORLD_SCROLL_H

#include <stdint.h>

#include "libpixtools/pixtools.h"

/**
 * Seed the procedural terrain (0 selects a built-in default seed).
 */
void world_scroll_init(uint32_t seed);

/**
 * Top y of the ground column at absolute world column `world_col`.
 */
int world_scroll_ground_top_y(int world_col);

/**
 * Draw the sky gradient and procedural ground for the given camera offset.
 */
void world_scroll_draw(PixContext *ctx, int camera_x,
                       uint32_t sky_top, uint32_t sky_bottom,
                       uint32_t ground_color);

#endif // WORLD_SCROLL_H
