#ifndef PIXTOOLS_H
#define PIXTOOLS_H

#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>

// SCREEN SIZE
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_H_LINE 263

// Common Colors (ARGB format: 0xAARRGGBB)
#define PIX_WHITE 0xFFFFFFFF
#define PIX_BLACK 0xFF000000
#define PIX_GRAY 0xFF808080

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Joystick *joystick;
    uint32_t *pixels;
    int width;
    int height;
    bool quit;
    bool blend_enabled;
} PixContext;

/**
 * Enables or disables alpha blending for subsequent draw calls.
 */
void pix_set_blend(PixContext *ctx, bool enabled);

/**
 * Initializes the library, creates a window and a renderer.
 * Returns a pointer to a PixContext on success, NULL on failure.
 */
PixContext *pix_init(const char *title, int width, int height, int scale);

/**
 * Clears the pixel buffer with a specific color (ARGB).
 */
void pix_clear(PixContext *ctx, uint32_t color);

/**
 * Add a single pixel to the buffer.
 */
void pix_add_one(PixContext *ctx, int x, int y, uint32_t color);

/**
 * Add a single letter with pixel size 8x8 to the buffer.
 */
void pix_add_letter(PixContext *ctx, int x, int y, char c, uint32_t text_color, uint32_t bg_color);

/**
 * Add a single letter with pixel size 8x8 scaled to the buffer.
 */
void pix_add_letter_scale(PixContext *ctx, int x, int y, char c, float scale, uint32_t text_color, uint32_t bg_color);

/**
 * Add a string with pixel size 8x8 scaled to the buffer.
 */
void pix_add_string_scale(PixContext *ctx, int x, int y, const char *s, float scale, uint32_t text_color, uint32_t bg_color, int padding);

/**
 * Add a horizontal line to the buffer.
 */
void pix_add_hline(PixContext *ctx, int x, int y, int w, uint32_t color);

/**
 * Add a vertical line to the buffer.
 */
void pix_add_vline(PixContext *ctx, int x, int y, int h, uint32_t color);

/**
 * Add a rectangle full color to the buffer.
 */
void pix_add_rect_full(PixContext *ctx, int x, int y, int w, int h, uint32_t color);

/**
 * Add a rectangle border to the buffer.
 */
void pix_add_rect_border(PixContext *ctx, int x, int y, int w, int h, uint32_t color);

/**
 * Saves a pixel buffer to a raw binary file.
 */
bool pix_save_raw(const char *path, const uint32_t *pixels, int w, int h);

/**
 * Loads a pixel buffer from a raw binary file.
 * NOTE: User is responsible for freeing the returned pointer.
 */
uint32_t *pix_load_raw(const char *path, int *w, int *h);

/**
 * Draws a raw pixel buffer (a sprite) onto the context at (x, y).
 */
void pix_add_sprite(PixContext *ctx, int x, int y, int w, int h, const uint32_t *sprite_pixels);

/**
 * Draws a grayscale 8-bit sprite onto the context at (x, y).
 * The `sprite_pixels` array contains values from 0 to 255.
 * The value 0 is completely transparent.
 * Values 1-255 are mapped to grayscale colors.
 */
void pix_add_sprite8(PixContext *ctx, int x, int y, int w, int h, const uint8_t *sprite_pixels);

/**
 * Draws a grayscale 8-bit sprite onto the context at (x, y) with integer scaling.
 */
void pix_add_sprite8_scale(PixContext *ctx, int x, int y, int w, int h, const uint8_t *sprite_pixels, int scale);

/**
 * Updates the window by drawing the current pixel buffer on screen.
 */
void pix_draw(PixContext *ctx);

/**
 * Cleans up resources and frees the context.
 */
void pix_close(PixContext *ctx);

/**
 * Displays a full-screen message before closing the program.
 */
void pix_show_exit_message(PixContext *ctx, const char *message);

#endif // PIXTOOLS_H
