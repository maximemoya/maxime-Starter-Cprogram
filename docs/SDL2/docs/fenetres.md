# Gestion des Fenêtres (Windowing)

Une fenêtre (`SDL_Window`) est le conteneur principal de votre application graphique.

## 1. Création de la fenêtre
On utilise `SDL_CreateWindow()` :
```c
SDL_Window* window = SDL_CreateWindow(
    "Mon Jeu Pixel Art",    // Titre
    SDL_WINDOWPOS_CENTERED, // Position X
    SDL_WINDOWPOS_CENTERED, // Position Y
    640,                    // Largeur
    480,                    // Hauteur
    SDL_WINDOW_SHOWN        // Flags
);
```

## 2. Flags courants
- `SDL_WINDOW_FULLSCREEN` : Plein écran.
- `SDL_WINDOW_FULLSCREEN_DESKTOP` : Plein écran à la résolution actuelle du bureau.
- `SDL_WINDOW_RESIZABLE` : Permet à l'utilisateur de redimensionner la fenêtre.
- `SDL_WINDOW_BORDERLESS` : Pas de bordures ni de barre de titre.
- `SDL_WINDOW_HIDDEN` : Fenêtre invisible à la création.

## 3. Manipulation
- `SDL_SetWindowTitle(window, "Nouveau Titre")` : Changer le titre.
- `SDL_SetWindowSize(window, 1280, 720)` : Changer la taille dynamiquement.
- `SDL_DestroyWindow(window)` : Détruire la fenêtre avant de quitter.
