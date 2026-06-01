# Mapping des Touches - RG35XXH (MustardOS / muOS)

Ce document détaille le mapping matériel des contrôles de la console Anbernic RG35XXH pour une utilisation avec SDL2.

## 1. Boutons (SDL_JOYBUTTONDOWN / UP)

Le joystick matériel expose les boutons suivants via l'index `e.jbutton.button`.

| Bouton Physique | Index SDL | Utilisation Suggérée |
| :--- | :--- | :--- |
| `VOLUME_DOWN` | 1 | Volume / Debug |
| `VOLUME_UP` | 2 | Volume / Debug |
| `A` | 3 | Action principale / Valider |
| `B` | 4 | Retour / Annuler |
| `Y` | 5 | Action secondaire |
| `X` | 6 | Menu |
| `L1` | 7 | Gâchette gauche |
| `R1` | 8 | Gâchette droite |
| `SELECT` | 9 | Option |
| `START` | 10 | Pause / Quitter (utilisé par `pix_poll_events`) |
| `FUNCTION` | 11 | Hotkey système |
| `L3` (Clic Gauche) | 12 | Joystick gauche pressé |
| `L2` | 13 | Gâchette arrière gauche |
| `R2` | 14 | Gâchette arrière droite |
| `R3` (Clic Droit) | 15 | Joystick droit pressé |
| `FONCTION` (Bis) | 16 | Bouton frontal central |

### Exemple de code (Boutons) :
```c
if (e.type == SDL_JOYBUTTONDOWN) {
    switch(e.jbutton.button) {
        case 10: ctx->quit = true; break; // START
        case 3:  /* Action A */ break;
    }
}
```

---

## 2. Croix Directionnelle (SDL_JOYHATMOTION)

Les flèches (`ARROWS`) ne sont **pas** des boutons, mais un **Hat** (Chapeau chinois). Elles sont accessibles via l'index `e.jhat.hat` (généralement `0`).

Utiliser les constantes nommées de `include/tool/joystick_rg35xxh/joystick_rg35xxh_hat_enum.h` (alias directs de `SDL_HAT_*`).

La valeur `e.jhat.value` est un masque de bits :

| Direction | Constante | Valeur Hexa |
| :--- | :--- | :--- |
| **Haut** | `JOYSTICK_RG35XXH_HAT_UP` | `0x01` |
| **Bas** | `JOYSTICK_RG35XXH_HAT_DOWN` | `0x04` |
| **Gauche** | `JOYSTICK_RG35XXH_HAT_LEFT` | `0x08` |
| **Droite** | `JOYSTICK_RG35XXH_HAT_RIGHT` | `0x02` |
| **Centré** | `SDL_HAT_CENTERED` | `0x00` |

### Exemple de code (Flèches) :
```c
#include "tool/joystick_rg35xxh/joystick_rg35xxh_hat_enum.h"

if (e.type == SDL_JOYHATMOTION) {
    if (e.jhat.value & JOYSTICK_RG35XXH_HAT_UP)    { /* Monter */ }
    if (e.jhat.value & JOYSTICK_RG35XXH_HAT_DOWN)  { /* Descendre */ }
    if (e.jhat.value & JOYSTICK_RG35XXH_HAT_LEFT)  { /* Gauche */ }
    if (e.jhat.value & JOYSTICK_RG35XXH_HAT_RIGHT) { /* Droite */ }
}
```

---

## 3. Joysticks Analogiques (SDL_JOYAXISMOTION)

La console possède deux sticks analogiques. Les mouvements sont capturés via l'index `e.jaxis.axis`.

La valeur `e.jaxis.value` varie de `-32768` à `32767`. Utiliser les constantes nommées de `include/tool/joystick_rg35xxh/joystick_rg35xxh_axis_enum.h` plutôt que les index bruts. Deadzone : `JOYSTICK_RG35XXH_AXIS_DEADZONE` (8000).

| Stick | Axe | Constante | Index SDL | Direction |
| :--- | :--- | :--- | :--- | :--- |
| **Gauche** | Horizontal | `JOYSTICK_RG35XXH_AXIS_LEFT_X`  | 0 | Négatif: Gauche / Positif: Droite |
| **Gauche** | Vertical   | `JOYSTICK_RG35XXH_AXIS_LEFT_Y`  | 1 | Négatif: Haut / Positif: Bas |
| **Droit**  | Horizontal | `JOYSTICK_RG35XXH_AXIS_RIGHT_X` | 2 | Négatif: Gauche / Positif: Droite |
| **Droit**  | Vertical   | `JOYSTICK_RG35XXH_AXIS_RIGHT_Y` | 3 | Négatif: Haut / Positif: Bas |

### Exemple de code (Analogique) :
```c
#include "tool/joystick_rg35xxh/joystick_rg35xxh_axis_enum.h"

if (e.type == SDL_JOYAXISMOTION) {
    if (e.jaxis.value < -JOYSTICK_RG35XXH_AXIS_DEADZONE
     || e.jaxis.value >  JOYSTICK_RG35XXH_AXIS_DEADZONE) {
        switch (e.jaxis.axis) {
            case JOYSTICK_RG35XXH_AXIS_LEFT_X:  /* Stick Gauche X */ break;
            case JOYSTICK_RG35XXH_AXIS_LEFT_Y:  /* Stick Gauche Y */ break;
            case JOYSTICK_RG35XXH_AXIS_RIGHT_X: /* Stick Droit X */  break;
            case JOYSTICK_RG35XXH_AXIS_RIGHT_Y: /* Stick Droit Y */  break;
        }
    }
}
```

---

## 4. Raccourcis sur Mac (Clavier)

Pour la parité de développement, les contrôles clavier suivants sont mappés :

- `ESC` : Quitter le programme.
- `Fermeture Fenêtre` (Bouton rouge) : Quitter le programme.
- `Souris` : Utilisée pour simuler des interactions tactiles ou de sélection.
