#ifndef INPUT_MAPPING_H
#define INPUT_MAPPING_H

#include <SDL.h>
#include <stdbool.h>
#include <stddef.h>

// Where the user's remapped console bindings are persisted (RULES.md §3 boundary: file I/O).
#define CONSOLE_MAPPING_LOG_PATH "console_mapping.log"

// Kind of physical input a logical label is bound to.
typedef enum
{
    MAP_INPUT_NONE = 0,
    MAP_INPUT_BUTTON,
    MAP_INPUT_HAT,
    MAP_INPUT_AXIS,
    MAP_INPUT_KEY
} MapInputKind;

// A single physical binding. `code` is button id / hat mask / axis index / keycode.
// `sign` selects axis direction (-1 or +1); unused for other kinds.
typedef struct
{
    MapInputKind kind;
    int          code;
    int          sign;
} MapInput;

// Logical labels the game reacts to. Order MUST match label_names[] and the
// defaults table in input_mapping.c.
typedef enum
{
    MAPPING_LABEL_A,
    MAPPING_LABEL_B,
    MAPPING_LABEL_X,
    MAPPING_LABEL_Y,
    MAPPING_LABEL_L1,
    MAPPING_LABEL_R1,
    MAPPING_LABEL_L2,
    MAPPING_LABEL_R2,
    MAPPING_LABEL_SELECT,
    MAPPING_LABEL_START,
    MAPPING_LABEL_HAUT,
    MAPPING_LABEL_BAS,
    MAPPING_LABEL_GAUCHE,
    MAPPING_LABEL_DROITE,
    MAPPING_LABEL_STICK_G_HAUT,
    MAPPING_LABEL_STICK_G_BAS,
    MAPPING_LABEL_STICK_G_GAUCHE,
    MAPPING_LABEL_STICK_G_DROITE,
    MAPPING_LABEL_STICK_D_HAUT,
    MAPPING_LABEL_STICK_D_BAS,
    MAPPING_LABEL_STICK_D_GAUCHE,
    MAPPING_LABEL_STICK_D_DROITE,
    MAPPING_LABEL_COUNT
} MappingLabel;

// Resolved bindings, indexed by MappingLabel. Defined in src/variables/globalConsoleMapping.c.
extern MapInput global_console_mapping[MAPPING_LABEL_COUNT];

/**
 * Reset every label to its RG35XXH factory default.
 */
void input_mapping_set_defaults(void);

/**
 * True if a persisted mapping file exists at CONSOLE_MAPPING_LOG_PATH.
 */
bool input_mapping_log_exists(void);

/**
 * Apply factory defaults, then overlay any bindings saved at `path`. Afterwards
 * every label is bound (defaults for anything missing/unreadable), so there is
 * never an unbound state. Returns true iff a saved file was actually read.
 */
bool input_mapping_load(const char *path);

/**
 * Save the current bindings to `path`. Returns false on write failure.
 */
bool input_mapping_save(const char *path);

/**
 * True when the SDL event `e` is the "just pressed" edge of `label`.
 */
bool input_mapping_is_press(const SDL_Event *e, MappingLabel label);

/**
 * Human-readable name of a label (e.g. "A", "STICK_G HAUT").
 */
const char *input_mapping_label_name(MappingLabel label);

/**
 * Format a binding into `buf` (size `n`), e.g. "BOUTON 3".
 */
void input_mapping_to_string(MapInput in, char *buf, size_t n);

#endif // INPUT_MAPPING_H
