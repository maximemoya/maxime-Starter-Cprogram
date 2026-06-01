#ifndef GLOBAL_SPRITES_H
#define GLOBAL_SPRITES_H

#include <stdint.h>

// 8-bit grayscale sprites (0 = transparent), consumed by pix_add_sprite8_scale.
// Defined in src/variables/globalSprites.c (RULES.md §3 / RULES.md sprite convention).

// 16x16 player silhouette (256 elements).
extern const uint8_t spritePlayer_256[256];

// 160x120 menu background (19200 elements), filled at startup by globalSprites_init().
extern uint8_t spriteBackground01_19200[19200];

/**
 * Populate the runtime-generated background sprite. Call once at init.
 */
void globalSprites_init(void);

#endif // GLOBAL_SPRITES_H
