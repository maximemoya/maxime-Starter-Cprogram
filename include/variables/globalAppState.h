#ifndef GLOBAL_APP_STATE_H
#define GLOBAL_APP_STATE_H

// Top-level application state machine (RULES.md §2: one responsibility per module).
// The global instance lives in src/variables/globalAppState.c (RULES.md §3).
typedef enum
{
    STATE_MENU,
    STATE_GAME,
    STATE_PAUSE,
    STATE_MAPPING
} AppState;

extern AppState app_state;

#endif // GLOBAL_APP_STATE_H
