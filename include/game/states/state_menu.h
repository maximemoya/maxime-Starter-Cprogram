#ifndef STATE_MENU_H
#define STATE_MENU_H

#include <SDL.h>
#include <stdbool.h>

#include "libpixtools/pixtools.h"
#include "variables/globalAppState.h"
#include "variables/globalSprites.h"

// Menu entries. Order matches menu_items[] in state_menu.c.
typedef enum
{
    MENU_ITEM_NEW_GAME,
    MENU_ITEM_MAPPING,
    MENU_ITEM_QUIT,
    MENU_ITEM_COUNT
} MenuItem;

typedef struct
{
    int  selected;       // current MenuItem index
    bool quit_requested; // set when the user picks QUITTER
    bool up_prev;        // edge-detection latches
    bool down_prev;
} MenuState;

void menu_init_menuState(void);
bool menu_should_quit(void);
void menu_event_handler(const SDL_Event *e);
void menu_action_per_tick(PixContext *ctx);
void menu_draw(PixContext *ctx);

#endif // STATE_MENU_H
