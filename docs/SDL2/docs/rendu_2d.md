# Rendu 2D (Render API)

SDL2 propose une API de rendu performante qui utilise l'accélération matérielle (GPU) via OpenGL, DirectX ou Metal.

## 1. Le Renderer
Le `SDL_Renderer` est le "pinceau" qui dessine dans la fenêtre.
```c
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
```
- `-1` : Utilise le premier driver supportant les flags demandés.
- `SDL_RENDERER_ACCELERATED` : Force l'utilisation du GPU.
- `SDL_RENDERER_PRESENTVSYNC` : Synchronise l'affichage avec le rafraîchissement de l'écran (évite le tearing).

## 2. Surfaces vs Textures
- **SDL_Surface** : Image stockée dans la RAM (CPU). Ancienne méthode, utile pour charger des fichiers bruts.
- **SDL_Texture** : Image stockée dans la VRAM (GPU). Beaucoup plus rapide pour le rendu.

## 3. Cycle de rendu classique
```c
// 1. Effacer l'écran
SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir
SDL_RenderClear(renderer);

// 2. Dessiner des choses (textures, lignes, etc.)
SDL_RenderCopy(renderer, texture, &srcRect, &destRect);

// 3. Afficher le résultat
SDL_RenderPresent(renderer);
```

## 4. Primitives de dessin
- `SDL_RenderDrawPoint(renderer, x, y)`
- `SDL_RenderDrawLine(renderer, x1, y1, x2, y2)`
- `SDL_RenderFillRect(renderer, &rect)`
