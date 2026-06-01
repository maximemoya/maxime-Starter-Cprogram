#include "game/states/input_state/input_state.h"
#include "game/core/event_handler/input_mapping/input_mapping.h"
#include "variables/globalConsoleMapping.h"
#include "libpixtools/joystick_rg35xxh/joystick_rg35xxh_axis_enum.h"

// --- RAW STATE (file-scope, updated by events) -----------------------------

// Flèches clavier : conservées pour la parité macOS (toujours actives, en plus
// du mapping manette).
static bool keyUp = false;
static bool keyDown = false;
static bool keyLeft = false;
static bool keyRight = false;

// État brut généralisé : permet d'évaluer n'importe quelle entrée mappée.
static bool buttonHeld[256];
static Uint8 hatState = SDL_HAT_CENTERED;
static Sint16 axisVal[8];

// Touches clavier maintenues (pour une direction mappée sur une touche).
#define HELD_KEYS_CAP 16
static SDL_Keycode heldKeys[HELD_KEYS_CAP];
static int heldKeysCount = 0;

// PUBLIC INSTANCE: definition moved to src/variables/globalInputState.c (RULES.md §3).
// `extern` declaration kept in input_state.h.

// --- HELPERS ---------------------------------------------------------------

static bool key_held(int code)
{
    for (int i = 0; i < heldKeysCount; i++)
        if (heldKeys[i] == (SDL_Keycode)code)
            return true;
    return false;
}

static void key_held_add(SDL_Keycode code)
{
    if (key_held((int)code) || heldKeysCount >= HELD_KEYS_CAP)
        return;
    heldKeys[heldKeysCount++] = code;
}

static void key_held_remove(SDL_Keycode code)
{
    for (int i = 0; i < heldKeysCount; i++)
    {
        if (heldKeys[i] == code)
        {
            heldKeys[i] = heldKeys[heldKeysCount - 1];
            heldKeysCount--;
            return;
        }
    }
}

// true si l'entrée physique mappée est actuellement maintenue.
static bool map_input_active(MapInput in)
{
    switch (in.kind)
    {
    case MAP_INPUT_BUTTON:
        return in.code >= 0 && in.code < 256 && buttonHeld[in.code];
    case MAP_INPUT_KEY:
        return key_held(in.code);
    case MAP_INPUT_HAT:
        return in.code != 0 && (hatState & in.code) == in.code;
    case MAP_INPUT_AXIS:
        if (in.code < 0 || in.code >= 8)
            return false;
        return in.sign < 0 ? axisVal[in.code] < -JOYSTICK_RG35XXH_AXIS_DEADZONE
                           : axisVal[in.code] > JOYSTICK_RG35XXH_AXIS_DEADZONE;
    case MAP_INPUT_NONE:
    default:
        return false;
    }
}

static void resolve_input_state(InputState *is)
{
    // Chaque direction = flèche clavier (parité) OU l'un des 3 libellés
    // directionnels mappés (D-pad, stick gauche, stick droit).
    is->up = keyUp ||
             map_input_active(global_console_mapping[MAPPING_LABEL_HAUT]) ||
             map_input_active(global_console_mapping[MAPPING_LABEL_STICK_G_HAUT]) ||
             map_input_active(global_console_mapping[MAPPING_LABEL_STICK_D_HAUT]);

    is->down = keyDown ||
               map_input_active(global_console_mapping[MAPPING_LABEL_BAS]) ||
               map_input_active(global_console_mapping[MAPPING_LABEL_STICK_G_BAS]) ||
               map_input_active(global_console_mapping[MAPPING_LABEL_STICK_D_BAS]);

    is->left = keyLeft ||
               map_input_active(global_console_mapping[MAPPING_LABEL_GAUCHE]) ||
               map_input_active(global_console_mapping[MAPPING_LABEL_STICK_G_GAUCHE]) ||
               map_input_active(global_console_mapping[MAPPING_LABEL_STICK_D_GAUCHE]);

    is->right = keyRight ||
                map_input_active(global_console_mapping[MAPPING_LABEL_DROITE]) ||
                map_input_active(global_console_mapping[MAPPING_LABEL_STICK_G_DROITE]) ||
                map_input_active(global_console_mapping[MAPPING_LABEL_STICK_D_DROITE]);
}

// --- PUBLIC API ------------------------------------------------------------

void global_input_state_initInputState(void)
{
    global_input_state.up = false;
    global_input_state.down = false;
    global_input_state.left = false;
    global_input_state.right = false;
}

void global_input_state_resetInputState(void)
{
    keyUp = false;
    keyDown = false;
    keyLeft = false;
    keyRight = false;

    for (int i = 0; i < 256; i++)
        buttonHeld[i] = false;
    hatState = SDL_HAT_CENTERED;
    for (int i = 0; i < 8; i++)
        axisVal[i] = 0;
    heldKeysCount = 0;

    global_input_state.up = false;
    global_input_state.down = false;
    global_input_state.left = false;
    global_input_state.right = false;
}

void global_input_state_update_pressed_keys(const SDL_Event *e)
{
    // KEYBOARD — flèches (parité) + suivi générique des touches maintenues
    if (e->type == SDL_KEYDOWN)
    {
        if (e->key.keysym.sym == SDLK_UP)
            keyUp = true;
        if (e->key.keysym.sym == SDLK_DOWN)
            keyDown = true;
        if (e->key.keysym.sym == SDLK_LEFT)
            keyLeft = true;
        if (e->key.keysym.sym == SDLK_RIGHT)
            keyRight = true;
        if (e->key.repeat == 0)
            key_held_add(e->key.keysym.sym);
    }
    if (e->type == SDL_KEYUP)
    {
        if (e->key.keysym.sym == SDLK_UP)
            keyUp = false;
        if (e->key.keysym.sym == SDLK_DOWN)
            keyDown = false;
        if (e->key.keysym.sym == SDLK_LEFT)
            keyLeft = false;
        if (e->key.keysym.sym == SDLK_RIGHT)
            keyRight = false;
        key_held_remove(e->key.keysym.sym);
    }

    // JOYSTICK RG35XXH — boutons maintenus
    if (e->type == SDL_JOYBUTTONDOWN)
        buttonHeld[e->jbutton.button] = true;
    if (e->type == SDL_JOYBUTTONUP)
        buttonHeld[e->jbutton.button] = false;

    // JOYSTICK RG35XXH — AXIS
    if (e->type == SDL_JOYAXISMOTION && e->jaxis.axis < 8)
        axisVal[e->jaxis.axis] = e->jaxis.value;

    // JOYSTICK RG35XXH — HAT
    if (e->type == SDL_JOYHATMOTION)
        hatState = e->jhat.value;

    // Recompute the resolved directional booleans
    resolve_input_state(&global_input_state);
}
