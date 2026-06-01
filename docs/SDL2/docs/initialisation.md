# Initialisation de SDL2

L'initialisation est la première étape de tout programme utilisant SDL2. Elle permet de préparer les sous-systèmes matériels.

## 1. La fonction SDL_Init
On utilise `SDL_Init()` pour charger les drivers nécessaires.

```c
if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
    printf("Erreur d'initialisation : %s\n", SDL_GetError());
    return 1;
}
```

## 2. Les sous-systèmes (Flags)
Vous pouvez combiner plusieurs flags avec l'opérateur OR (`|`) :
- `SDL_INIT_VIDEO` : Vidéo (indispensable pour les fenêtres).
- `SDL_INIT_AUDIO` : Audio.
- `SDL_INIT_EVENTS` : Événements (souvent inclus avec VIDEO).
- `SDL_INIT_JOYSTICK` / `SDL_INIT_GAMECONTROLLER` : Périphériques d'entrée.
- `SDL_INIT_TIMER` : Gestion du temps.
- `SDL_INIT_EVERYTHING` : Initialise tous les systèmes ci-dessus.

## 3. Gestion des erreurs
SDL2 fournit `SDL_GetError()` qui retourne une chaîne de caractères décrivant le dernier problème rencontré. C'est essentiel pour le debug.

## 4. Nettoyage
À la fin du programme, il est impératif d'appeler `SDL_Quit()` pour libérer proprement la mémoire et les ressources système.
