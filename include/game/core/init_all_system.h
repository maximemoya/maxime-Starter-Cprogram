#ifndef INIT_ALL_SYSTEM_H
#define INIT_ALL_SYSTEM_H

#include "libpixtools/pixtools.h"

/**
 * Initialise every subsystem (SDL/window via pix_init, input mapping, input state,
 * menu, sprites). Returns the owning PixContext, or NULL on failure.
 */
PixContext *init_all_system(void);

#endif // INIT_ALL_SYSTEM_H
