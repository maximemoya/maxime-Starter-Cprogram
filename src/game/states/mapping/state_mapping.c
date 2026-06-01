#include "game/states/state_mapping.h"

#include "game/core/event_handler/input_mapping/input_mapping.h"

#include "libpixtools/joystick_rg35xxh/joystick_rg35xxh_button_enum.h"
#include "libpixtools/joystick_rg35xxh/joystick_rg35xxh_axis_enum.h"

// STATICS

#define MAPPING_ROWS_PER_COL 11

// Navigation / confirm / cancel here use PHYSICAL hardware (raw keyboard arrows,
// raw D-pad hat, and the fixed RG35XXH A/B button ids) — never the remappable
// layer — so a broken mapping can still be navigated and repaired. The physical
// B button is reserved as cancel/back and therefore cannot be (re)bound here.
static int  selected = 0;
static bool capturing = false;

static void onSaveAndBack(void)
{
    input_mapping_save(CONSOLE_MAPPING_LOG_PATH);
    selected = 0;
    capturing = false;
    app_state = STATE_MENU;
}

static void onSelectPrev(void)
{
    selected = (selected - 1 + MAPPING_LABEL_COUNT) % MAPPING_LABEL_COUNT;
}

static void onSelectNext(void)
{
    selected = (selected + 1) % MAPPING_LABEL_COUNT;
}

// Bind a captured physical input to the selected label and leave capture mode.
static void onCapture(MapInput in)
{
    global_console_mapping[selected] = in;
    capturing = false;
}

// Interpret `e` while in capture mode: cancel (ESC / physical B), or bind the
// first button / hat / axis / key seen. Same kind/sign encoding as input_mapping.
static void onCaptureEvent(const SDL_Event *e)
{
    if (e->type == SDL_KEYDOWN && e->key.repeat == 0 && e->key.keysym.sym == SDLK_ESCAPE)
    {
        capturing = false;
        return;
    }
    if (e->type == SDL_JOYBUTTONDOWN && e->jbutton.button == JOYSTICK_RG35XXH_B)
    {
        capturing = false;
        return;
    }

    if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
        onCapture((MapInput){MAP_INPUT_KEY, (int)e->key.keysym.sym, 0});
    else if (e->type == SDL_JOYBUTTONDOWN)
        onCapture((MapInput){MAP_INPUT_BUTTON, e->jbutton.button, 0});
    else if (e->type == SDL_JOYHATMOTION && e->jhat.value != SDL_HAT_CENTERED)
        onCapture((MapInput){MAP_INPUT_HAT, e->jhat.value, 0});
    else if (e->type == SDL_JOYAXISMOTION && e->jaxis.value < -JOYSTICK_RG35XXH_AXIS_DEADZONE)
        onCapture((MapInput){MAP_INPUT_AXIS, e->jaxis.axis, -1});
    else if (e->type == SDL_JOYAXISMOTION && e->jaxis.value > JOYSTICK_RG35XXH_AXIS_DEADZONE)
        onCapture((MapInput){MAP_INPUT_AXIS, e->jaxis.axis, 1});
}

// FUNCTIONS

void mapping_init(void)
{
    // load() establishes the factory-default baseline internally, then overlays
    // any saved bindings — so we are never left unbound.
    input_mapping_load(CONSOLE_MAPPING_LOG_PATH);
    selected = 0;
    capturing = false;
}

void mapping_event_handler(const SDL_Event *e)
{
    if (capturing)
    {
        onCaptureEvent(e);
        return;
    }

    // NAVIGATION — raw keyboard (parity / desktop dev)
    if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_UP:
            onSelectPrev();
            break;
        case SDLK_DOWN:
            onSelectNext();
            break;
        case SDLK_RETURN:
            capturing = true;
            break;
        case SDLK_BACKSPACE:
        case SDLK_ESCAPE:
            onSaveAndBack();
            break;
        default:
            break;
        }
    }

    // NAVIGATION — raw D-pad hat (physical, independent of the mapping)
    if (e->type == SDL_JOYHATMOTION)
    {
        if (e->jhat.value & SDL_HAT_UP)
            onSelectPrev();
        if (e->jhat.value & SDL_HAT_DOWN)
            onSelectNext();
    }

    // CONFIRM / BACK — fixed physical RG35XXH buttons (A = edit, B = save + menu)
    if (e->type == SDL_JOYBUTTONDOWN)
    {
        if (e->jbutton.button == JOYSTICK_RG35XXH_A)
            capturing = true;
        if (e->jbutton.button == JOYSTICK_RG35XXH_B)
            onSaveAndBack();
    }
}

void mapping_action_per_tick(PixContext *ctx)
{
    (void)ctx;
}

void mapping_draw(PixContext *ctx)
{
    pix_add_string_scale(ctx, 8, 8, "CONFIG TOUCHES", 2.5f, PIX_WHITE, PIX_BLACK, 4);

    // Two columns listing every label and its current binding; selected row in white.
    char buf[32];
    for (int i = 0; i < MAPPING_LABEL_COUNT; i++)
    {
        int col = i / MAPPING_ROWS_PER_COL;
        int row = i % MAPPING_ROWS_PER_COL;
        int x = 16 + col * 300;
        int y = 70 + row * 34;

        uint32_t row_color = (i == selected) ? PIX_WHITE : PIX_GRAY;
        input_mapping_to_string(global_console_mapping[i], buf, sizeof(buf));
        pix_add_string_scale(ctx, x, y, input_mapping_label_name((MappingLabel)i), 1.5f, row_color, PIX_BLACK, 1);
        pix_add_string_scale(ctx, x + 150, y, buf, 1.5f, row_color, PIX_BLACK, 1);
    }

    if (capturing)
        pix_add_string_scale(ctx, 8, 448, "APPUYEZ SUR UNE ENTREE   (B/ESC: ANNULER)", 1.5f, PIX_WHITE, PIX_BLACK, 2);
    else
        pix_add_string_scale(ctx, 8, 448, "FLECHES/DPAD: NAVIGUER   A: MODIFIER   B: SAUVER+MENU", 1.5f, PIX_GRAY, PIX_BLACK, 2);
}
