#include <stdio.h>
#include <string.h>

#include "game/core/event_handler/input_mapping/input_mapping.h"

#include "libpixtools/joystick_rg35xxh/joystick_rg35xxh_button_enum.h"
#include "libpixtools/joystick_rg35xxh/joystick_rg35xxh_hat_enum.h"
#include "libpixtools/joystick_rg35xxh/joystick_rg35xxh_axis_enum.h"

// STATICS

static const char *label_names[MAPPING_LABEL_COUNT] = {
    "A", "B", "X", "Y", "L1", "R1", "L2", "R2", "SELECT", "START",
    "HAUT", "BAS", "GAUCHE", "DROITE",
    "STICK_G HAUT", "STICK_G BAS", "STICK_G GAUCHE", "STICK_G DROITE",
    "STICK_D HAUT", "STICK_D BAS", "STICK_D GAUCHE", "STICK_D DROITE"};

static const char *kind_token(MapInputKind k)
{
    switch (k)
    {
    case MAP_INPUT_BUTTON:
        return "BUTTON";
    case MAP_INPUT_HAT:
        return "HAT";
    case MAP_INPUT_AXIS:
        return "AXIS";
    case MAP_INPUT_KEY:
        return "KEY";
    case MAP_INPUT_NONE:
    default:
        return "NONE";
    }
}

static MapInputKind kind_from_token(const char *t)
{
    if (strcmp(t, "BUTTON") == 0)
        return MAP_INPUT_BUTTON;
    if (strcmp(t, "HAT") == 0)
        return MAP_INPUT_HAT;
    if (strcmp(t, "AXIS") == 0)
        return MAP_INPUT_AXIS;
    if (strcmp(t, "KEY") == 0)
        return MAP_INPUT_KEY;
    return MAP_INPUT_NONE;
}

// FUNCTIONS

void input_mapping_set_defaults(void)
{
    global_console_mapping[MAPPING_LABEL_A] = (MapInput){MAP_INPUT_BUTTON, JOYSTICK_RG35XXH_A, 0};
    global_console_mapping[MAPPING_LABEL_B] = (MapInput){MAP_INPUT_BUTTON, JOYSTICK_RG35XXH_B, 0};
    global_console_mapping[MAPPING_LABEL_X] = (MapInput){MAP_INPUT_BUTTON, JOYSTICK_RG35XXH_X, 0};
    global_console_mapping[MAPPING_LABEL_Y] = (MapInput){MAP_INPUT_BUTTON, JOYSTICK_RG35XXH_Y, 0};
    global_console_mapping[MAPPING_LABEL_L1] = (MapInput){MAP_INPUT_BUTTON, JOYSTICK_RG35XXH_L1, 0};
    global_console_mapping[MAPPING_LABEL_R1] = (MapInput){MAP_INPUT_BUTTON, JOYSTICK_RG35XXH_R1, 0};
    global_console_mapping[MAPPING_LABEL_L2] = (MapInput){MAP_INPUT_BUTTON, JOYSTICK_RG35XXH_L2, 0};
    global_console_mapping[MAPPING_LABEL_R2] = (MapInput){MAP_INPUT_BUTTON, JOYSTICK_RG35XXH_R2, 0};
    global_console_mapping[MAPPING_LABEL_SELECT] = (MapInput){MAP_INPUT_BUTTON, JOYSTICK_RG35XXH_SELECT, 0};
    global_console_mapping[MAPPING_LABEL_START] = (MapInput){MAP_INPUT_BUTTON, JOYSTICK_RG35XXH_START, 0};

    global_console_mapping[MAPPING_LABEL_HAUT] = (MapInput){MAP_INPUT_HAT, JOYSTICK_RG35XXH_HAT_UP, 0};
    global_console_mapping[MAPPING_LABEL_BAS] = (MapInput){MAP_INPUT_HAT, JOYSTICK_RG35XXH_HAT_DOWN, 0};
    global_console_mapping[MAPPING_LABEL_GAUCHE] = (MapInput){MAP_INPUT_HAT, JOYSTICK_RG35XXH_HAT_LEFT, 0};
    global_console_mapping[MAPPING_LABEL_DROITE] = (MapInput){MAP_INPUT_HAT, JOYSTICK_RG35XXH_HAT_RIGHT, 0};

    global_console_mapping[MAPPING_LABEL_STICK_G_HAUT] = (MapInput){MAP_INPUT_AXIS, JOYSTICK_RG35XXH_AXIS_LEFT_Y, -1};
    global_console_mapping[MAPPING_LABEL_STICK_G_BAS] = (MapInput){MAP_INPUT_AXIS, JOYSTICK_RG35XXH_AXIS_LEFT_Y, 1};
    global_console_mapping[MAPPING_LABEL_STICK_G_GAUCHE] = (MapInput){MAP_INPUT_AXIS, JOYSTICK_RG35XXH_AXIS_LEFT_X, -1};
    global_console_mapping[MAPPING_LABEL_STICK_G_DROITE] = (MapInput){MAP_INPUT_AXIS, JOYSTICK_RG35XXH_AXIS_LEFT_X, 1};

    global_console_mapping[MAPPING_LABEL_STICK_D_HAUT] = (MapInput){MAP_INPUT_AXIS, JOYSTICK_RG35XXH_AXIS_RIGHT_Y, -1};
    global_console_mapping[MAPPING_LABEL_STICK_D_BAS] = (MapInput){MAP_INPUT_AXIS, JOYSTICK_RG35XXH_AXIS_RIGHT_Y, 1};
    global_console_mapping[MAPPING_LABEL_STICK_D_GAUCHE] = (MapInput){MAP_INPUT_AXIS, JOYSTICK_RG35XXH_AXIS_RIGHT_X, -1};
    global_console_mapping[MAPPING_LABEL_STICK_D_DROITE] = (MapInput){MAP_INPUT_AXIS, JOYSTICK_RG35XXH_AXIS_RIGHT_X, 1};
}

bool input_mapping_log_exists(void)
{
    FILE *f = fopen(CONSOLE_MAPPING_LOG_PATH, "r");
    if (f)
    {
        fclose(f);
        return true;
    }
    return false;
}

bool input_mapping_load(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
        return false;

    char line[128];
    for (int i = 0; i < MAPPING_LABEL_COUNT; i++)
    {
        if (!fgets(line, sizeof(line), f))
            break;
        char tok[16];
        int code = 0;
        int sign = 0;
        // Format : "KIND CODE SIGN  # LABEL" — le commentaire est ignoré.
        if (sscanf(line, "%15s %d %d", tok, &code, &sign) == 3)
            global_console_mapping[i] = (MapInput){kind_from_token(tok), code, sign};
        // Ligne illisible : on garde la valeur courante (défaut).
    }

    fclose(f);
    return true;
}

bool input_mapping_save(const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f)
    {
        SDL_Log("input_mapping: impossible d'ouvrir %s en écriture", path);
        return false;
    }

    for (int i = 0; i < MAPPING_LABEL_COUNT; i++)
    {
        MapInput m = global_console_mapping[i];
        fprintf(f, "%s %d %d  # %s\n", kind_token(m.kind), m.code, m.sign, label_names[i]);
    }

    fclose(f);
    return true;
}

bool input_mapping_is_press(const SDL_Event *e, MappingLabel label)
{
    if (label < 0 || label >= MAPPING_LABEL_COUNT)
        return false;

    MapInput m = global_console_mapping[label];
    switch (m.kind)
    {
    case MAP_INPUT_BUTTON:
        return e->type == SDL_JOYBUTTONDOWN && e->jbutton.button == m.code;
    case MAP_INPUT_KEY:
        return e->type == SDL_KEYDOWN && e->key.repeat == 0 && (int)e->key.keysym.sym == m.code;
    case MAP_INPUT_HAT:
        return e->type == SDL_JOYHATMOTION && m.code != 0 && (e->jhat.value & m.code) == m.code;
    case MAP_INPUT_AXIS:
        if (e->type != SDL_JOYAXISMOTION || e->jaxis.axis != m.code)
            return false;
        return m.sign < 0 ? e->jaxis.value < -JOYSTICK_RG35XXH_AXIS_DEADZONE
                          : e->jaxis.value > JOYSTICK_RG35XXH_AXIS_DEADZONE;
    case MAP_INPUT_NONE:
    default:
        return false;
    }
}

const char *input_mapping_label_name(MappingLabel label)
{
    if (label < 0 || label >= MAPPING_LABEL_COUNT)
        return "?";
    return label_names[label];
}

void input_mapping_to_string(MapInput in, char *buf, size_t n)
{
    switch (in.kind)
    {
    case MAP_INPUT_BUTTON:
        snprintf(buf, n, "BOUTON %d", in.code);
        break;
    case MAP_INPUT_HAT:
        snprintf(buf, n, "DPAD %d", in.code);
        break;
    case MAP_INPUT_AXIS:
        snprintf(buf, n, "AXE %d %c", in.code, in.sign < 0 ? '-' : '+');
        break;
    case MAP_INPUT_KEY:
        snprintf(buf, n, "TOUCHE %d", in.code);
        break;
    case MAP_INPUT_NONE:
    default:
        snprintf(buf, n, "-");
        break;
    }
}
