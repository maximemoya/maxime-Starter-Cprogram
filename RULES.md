# Règles d'Architecture du Projet (SDL2 Pixel Art)

## 1. Gestion de la Mémoire (La Règle d'Or)
> "N'utilise `malloc` que si tu ne peux pas faire autrement."
- **Privilégier l'allocation statique :** Utiliser des objets statiques (variables instanciées au début du programme) afin d'éviter de stresser la mémoire avec des `malloc` et `calloc`.
- **Cas justifiant `malloc` :** Uniquement si la donnée doit survivre à la portée de la fonction et qu'il est impossible de faire autrement, ou si la structure est véritablement trop grosse pour tenir sur la pile.
- **Libération obligatoire :** Tout pointeur alloué dynamiquement doit systématiquement avoir sa contrepartie `free()` pour éviter les fuites de mémoire.

## 2. Structure et Organisation du Code
Le projet suit une séparation stricte entre les en-têtes et l'implémentation, avec une organisation modulaire :

### Organisation des Dossiers
- `include/` : Exclusivement les fichiers d'en-tête (`.h`).
- `src/` : Exclusivement les fichiers d'implémentation (`.c`).
- **Symétrie Obligatoire :** Tout sous-dossier dans `include/` doit avoir son équivalent dans `src/`.

## 3. Pratiques de Codage
- **Encapsulation :** Les modules complexes doivent utiliser des structures de contexte.
- **Include Guards :** Toujours protéger les `.h` avec `#ifndef FILENAME_H`.
- **Préfixes :** Utiliser des préfixes cohérents par module pour éviter les collisions de noms.
- **Initialisation Statique :** Préférer `initXyz(&monObjet, ...)` sur un objet déjà alloué statiquement plutôt que `monObjet = createXyz(...)` qui ferait un malloc caché.
- **`const` par défaut sur les pointeurs en lecture seule :** Si une fonction ne mute pas la cible d'un pointeur passé en paramètre, sa signature doit utiliser `const`. Exemple : `void printfPlayer(const Player *player)` (lecture seule) vs `void initPlayer(Player *player, ...)` (mutation). Le compilateur vérifie ainsi le contrat ; toute tentative de mutation à travers un `const` devient une erreur de compilation. Header et `.c` doivent toujours matcher.
- **Pas de check `NULL` défensif sur pointeurs internes :** Les fonctions opérant sur des pointeurs issus d'invariants internes (ex: `&globalPlayer`) ne doivent pas vérifier `if (!ptr) return;`. Ce bruit cache les vrais bugs. Vérifier uniquement aux frontières (input utilisateur, API externe, retour de `malloc`/`fopen`). Si une assertion paranoïaque est souhaitée en debug, utiliser `assert(ptr)` (compilé out en release), jamais un `if` silencieux.

### Mise en Page Standard d'un Module

#### Header `.h` — ordre obligatoire

```c
#ifndef STATE_GAME_H                    // 1. Include guard
#define STATE_GAME_H

#include "libpixtools/pixtools.h"       // 2. Includes minimaux (uniquement ce que
#include <SDL.h>                        //    les déclarations du header utilisent)

typedef enum                            // 3. Types publics (enums, structs, typedefs)
{
    LEVEL_01,
    LEVEL_02,
    LEVEL_COUNT                         //    Sentinelle pour énumérations comptables
} LevelId;

void game_event_handler(SDL_Event *e);  // 4. Prototypes publics
void game_action_per_tick(PixContext *ctx);
void game_draw(PixContext *ctx);
void game_reset_current_level(void);

#endif // STATE_GAME_H                  // 5. Fin garde + commentaire de fermeture
```

Règles :
- Le commentaire `// STATE_GAME_H` après `#endif` aide à la lecture des fichiers longs.

#### Source `.c` — ordre obligatoire

```c
#include <stdbool.h>                    // 1. Includes système (entre <>)
                                        //    (ligne vide)
#include "input_state/input_state.h"    // 2. Includes projet (entre "")
#include "states/state_game.h"          //    Inclure son propre header en haut.
#include "states/game/levels/level_01.h"
#include "states/game/levels/level_02.h"
#include "libpixtools/joystick_rg35xxh/joystick_rg35xxh_button_enum.h"
#include "variables/globalVariables.h"

// STATICS                              // 3. Marqueur de section

static LevelId current_level = LEVEL_01; // 4. État statique (file-local) — `static` obligatoire

static void onBackToMenu(void)           // 5. Fonctions helper privées — `static` obligatoire
{                                        //    Nommage : `on<Action>` (verbe d'action)
    input_state_resetInputState();
    app_state = STATE_MENU;
}

static void onPauseGame(void) { /* … */ }
static void onCycleLevel(void) { /* … */ }

// FUNCTIONS                             // 6. Marqueur de section

void game_reset_current_level(void)      // 7. Fonctions publiques
{                                        //    Préfixe = nom du module (`game_`)
    current_level = LEVEL_01;
}

void game_event_handler(SDL_Event *e)
{
    // Delegate input tracking to the shared module
    input_state_process_event(e);        // 8. Délégation aux modules partagés en premier

    // KEYBOARD — state transitions      // 9. Sous-sections commentées par source d'entrée
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_BACKSPACE)
        onBackToMenu();                  //    DRY : appel à la même fonction static
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_p)
        onPauseGame();

    // JOYSTICK RG35XXH — state transitions
    if (e->type == SDL_JOYBUTTONDOWN && e->jbutton.button == JOYSTICK_RG35XXH_Y)
        onBackToMenu();                  //    Même helper que la branche clavier
    if (e->type == SDL_JOYBUTTONDOWN && e->jbutton.button == JOYSTICK_RG35XXH_SELECT)
        onPauseGame();

    // Level dispatch                    // 10. Dispatch en fin de fonction
    switch (current_level)
    {
    case LEVEL_01:
        level_01_event_handler(e);
        break;
    case LEVEL_02:
        level_02_event_handler(e);
        break;
    default:                             //     Toujours un `default: break;` même si exhaustif
        break;
    }
}
```

#### Récapitulatif des règles de mise en page

| Élément | Règle |
|---|---|
| Includes système | En premier, ligne vide avant les includes projet |
| Own header | Inclus en haut du bloc projet (force la compilation isolée du `.h`) |
| Section `// STATICS` | Marqueur obligatoire avant tout `static` (variables + helpers) |
| Section `// FUNCTIONS` | Marqueur obligatoire avant les fonctions publiques |
| Helpers privés | `static`, nommés `on<Action>` (verbe), appelés depuis chaque branche d'entrée |
| Fonctions publiques | Préfixe = nom du module (`game_*`, `level_01_*`, `pix_*`, `input_state_*`) |
| Event handler | Délégation → branche KEYBOARD → branche JOYSTICK → dispatch switch |
| Switch | `default: break;` toujours présent |
| Commentaires de section | `// KEYBOARD`, `// JOYSTICK RG35XXH`, `// Level dispatch` |
| `extern` | Sur variables uniquement, jamais sur prototypes de fonctions |

## 4. Gestion des Sprites
- **Format Standard :** Les sprites globaux doivent être définis dans des tableaux `uint8_t` de 256 éléments (16x16).
- **Définition Lisible :** Pour la création, utiliser systématiquement une matrice intermédiaire `uint8_t[16][16]` dans la fonction `initGlobalSprites()` afin de visualiser la forme du sprite dans le code.
- **Transparence :** La valeur `0` est strictement réservée à la transparence. Aucun pixel visible ne doit utiliser cette valeur.
- **Localisation :** Tous les sprites partagés doivent être centralisés dans `src/variables/globalSprites.c` et déclarés dans `include/variables/globalSprites.h`.

## 5. Cross-Platform et Synchronisation (Parité Totale)
- **Parité de Comportement :** Le programme doit se comporter de manière strictement identique sur macOS et sur RG35XXH. 
- **Adoption de la Contrainte Maximale :** Comme la version RG35XXH (MustardOS/GCC) est plus stricte que macOS (Clang), on adoptera systématiquement les contraintes les plus fortes sur toutes les plateformes (ex: gestion rigoureuse des `extern`, interdiction des définitions multiples, etc.).
- **Maintenance des Makefiles :** Les makefiles sont désormais conçus pour découvrir automatiquement les fichiers sources `src/**/*.c` et générer les objets correspondants dans `dist/...` via une règle de motif. Les fichiers d'en-tête sous `include/**` sont suivis automatiquement grâce aux options `-MMD -MP` et les fichiers de dépendances `.d` sont inclus par le Makefile.

  Conséquences pratiques :
  - Ajouter `src/x/x.c` et `include/x/x.h` suffit pour que le fichier soit compilé et pris en compte (s'il est `#include` par un .c existant). Il n'est plus nécessaire d'éditer manuellement les Makefiles pour les cas standards.
  - Pour forcer qu'une source soit compilée dans la bibliothèque statique `libpixtools.a`, placez-la sous `src/libpixtools/` ; sinon elle sera liée dans l'exécutable applicatif.
  - Les cibles de type `run` effectuent désormais un `clean` limité à la plateforme avant la construction pour éviter les artéfacts obsolètes (ex : `make -f makefile.mac run` nettoie seulement `dist/macos`). Utilisez `make clean` si un nettoyage plus large est nécessaire.
  - Le parallélisme par défaut est configuré (JOBS = nombre de coeurs - 1) mais peut être surchargé par `make JOBS=4` ou `make -j8`.

  Remarque : Si vous avez des cas spéciaux (fichiers générés, extensions non standard, ou flux de compilation particuliers), conservez la pratique de déclarer explicitement ces règles dans le Makefile et documentez pourquoi dans un commentaire à proximité.

  En résumé : évitez de maintenir manuellement une liste de `.c` sauf besoin très spécifique — la découverte automatique réduit les risques d'oubli et facilite l'ajout de nouveaux modules.

## 6. DRY (Don't Repeat Yourself) & Code Modulaire

> "Zéro copier-coller. Si une logique apparaît deux fois, extraire."

### Pas de duplication entre sources d'entrée
- **Interdiction de dupliquer la logique d'action entre les branches clavier et joystick.** Le handler d'événement ne doit faire que la distinction de la source d'entrée ; les deux chemins appellent ensuite la même fonction `static`.
- Exemple de violation : écrire `if (selected == MENU_NEW_GAME) game_state = STATE_GAME;` à la fois dans le bloc `if (e->type == SDL_KEYDOWN)` et dans le bloc `if (e->type == SDL_JOYBUTTONDOWN)`.
- Exemple conforme : extraire `static void onMenuConfirmAction(void)` et l'appeler depuis les deux branches.

### Pas de duplication entre states
- Si plusieurs states partagent un comportement (lecture d'entrée, validation, transition), extraire dans un **module dédié** sous `src/<nom>/`.
- Exemple existant : `src/input_state/` centralise la lecture clavier + D-pad + sticks analogiques. Les states (`state_game`, `state_menu`, `state_pause`) ne lisent jamais directement les events d'entrée — ils interrogent `input_state.up/down/left/right`.

### Un module, une responsabilité
- Chaque module (`src/x/x.c` + `include/x/x.h`) résout **un** problème clair, nommable en une phrase.
- Si un module commence à mélanger des responsabilités hétérogènes, le scinder en deux.
- Les noms de modules doivent refléter ce qu'ils gèrent (`input_state`, `state_menu`, `global_main_event_handler`), pas ce qu'ils contiennent (`utils`, `helpers`, `misc` sont à proscrire).

### Quand une troisième occurrence apparaît
- Deux occurrences similaires sont tolérables (parfois un faux ami).
- Trois occurrences = extraction obligatoire. La règle « rule of three » s'applique pour éviter les abstractions prématurées tout en garantissant qu'aucune triplette ne reste figée.

---

## 7. Gestion du Temps (Timing)
Le projet utilise la synchronisation verticale (**VSync**) pour caler la boucle principale sur l'affichage, garantissant une fluidité maximale sans déchirure d'écran.

### Synchronisation et Fluidité
- **Interdiction de `SDL_Delay` :** Ne jamais utiliser `SDL_Delay` à l'intérieur de la boucle `while (!ctx->quit)`. C'est le VSync activé dans `pix_init` qui gère naturellement le temps d'attente entre les frames.
- **Rendu Sync :** Le rendu doit être la dernière étape de la boucle via `pix_draw(ctx)`.

### Logique Temporelle (Timers Non-Bloquants)
Pour gérer des événements indépendants de la vitesse d'affichage (ex: animation, cadence de tir) :
- **Utilisation de `SDL_GetTicks()` :** Utiliser systématiquement `SDL_GetTicks()` pour obtenir le temps actuel en millisecondes.
- **Pattern du Timer :** Toujours utiliser une variable de suivi (`lastActionTime`) pour comparer le temps écoulé sans bloquer l'exécution du reste du programme.
- **Indépendance des FPS :** Pour les calculs complexes, privilégier le concept de **Delta Time** (temps écoulé entre deux frames) afin que la vitesse du jeu reste constante peu importe la fréquence de rafraîchissement de l'écran.
