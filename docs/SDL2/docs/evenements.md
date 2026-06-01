# Gestion des Événements

SDL2 gère les interactions utilisateur via une file d'attente d'événements.

## 1. La boucle d'événements
Il faut vider la file à chaque frame pour que l'application reste réactive.
```c
SDL_Event event;
while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
        running = false;
    }
}
```

## 2. Le Clavier
- `event.key.keysym.sym` : **Keycode** (ex: `SDLK_a`). Dépend de la lettre imprimée sur la touche (AZERTY vs QWERTY).
- `event.key.keysym.scancode` : **Scancode** (ex: `SDL_SCANCODE_A`). Dépend de la position physique de la touche (recommandé pour les jeux).

## 3. La Souris
- `SDL_MOUSEMOTION` : Déplacement.
- `SDL_MOUSEBUTTONDOWN` : Clic.
- `SDL_MOUSEWHEEL` : Molette.

## 4. Événements de fenêtre
`event.type == SDL_WINDOWEVENT` permet de détecter si la fenêtre a été minimisée, restaurée ou si elle a perdu le focus.
