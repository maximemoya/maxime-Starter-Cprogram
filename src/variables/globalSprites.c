#include "variables/globalSprites.h"

// 16x16 placeholder character (0 = transparent, 255 = opaque white).
// Grid below mirrors the array row-for-row.
//
//   . . . . . . . . . . . . . . . .
//   . . . . . . # # # # . . . . . .
//   . . . . . # # # # # # . . . . .
//   . . . . # # # # # # # # . . . .
//   . . . # # # # # # # # # # . . .
//   . . . # # . # # # # . # # . . .
//   . . . # # # # # # # # # # . . .
//   . . . # # # # # # # # # # . . .
//   . . . . # # . . . . # # . . . .
//   . . . # # . . . . . . # # . . .
//   . . # # . . . . . . . . # # . .
//   . . . . . . . . . . . . . . . .   (x6)
//
#define O 0
#define W 255
const uint8_t spritePlayer_256[256] = {
    O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
    O, O, O, O, O, O, W, W, W, W, O, O, O, O, O, O,
    O, O, O, O, O, W, W, W, W, W, W, O, O, O, O, O,
    O, O, O, O, W, W, W, W, W, W, W, W, O, O, O, O,
    O, O, O, W, W, W, W, W, W, W, W, W, W, O, O, O,
    O, O, O, W, W, O, W, W, W, W, O, W, W, O, O, O,
    O, O, O, W, W, W, W, W, W, W, W, W, W, O, O, O,
    O, O, O, W, W, W, W, W, W, W, W, W, W, O, O, O,
    O, O, O, O, W, W, O, O, O, O, W, W, O, O, O, O,
    O, O, O, W, W, O, O, O, O, O, O, W, W, O, O, O,
    O, O, W, W, O, O, O, O, O, O, O, O, W, W, O, O,
    O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
    O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
    O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
    O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O,
    O, O, O, O, O, O, O, O, O, O, O, O, O, O, O, O};
#undef O
#undef W

// 160x120 menu backdrop, generated at startup so we don't ship 19200 literals.
uint8_t spriteBackground01_19200[19200];

#define BG_W 160
#define BG_H 120

void globalSprites_init(void)
{
    for (int y = 0; y < BG_H; y++)
    {
        for (int x = 0; x < BG_W; x++)
        {
            // Vertical gradient (dark at top, lighter at the bottom).
            int v = 16 + (y * 180) / (BG_H - 1);
            // Sparse bright specks for a starfield-ish texture.
            if (((x ^ y) & 31) == 0)
                v = 255;
            if (v < 1)
                v = 1;
            if (v > 255)
                v = 255;
            spriteBackground01_19200[y * BG_W + x] = (uint8_t)v;
        }
    }
}
