# Extensions Officielles

SDL2 est souvent accompagné de trois bibliothèques "satellites" presque indispensables.

## 1. SDL_image
Permet de charger des formats d'images plus complexes que le BMP (format natif de SDL).
```c
#include <SDL_image.h>
IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
SDL_Texture* tex = IMG_LoadTexture(renderer, "sprite.png");
```

## 2. SDL_ttf
Indispensable pour afficher du texte à partir de polices vectorielles (.ttf ou .otf).
```c
#include <SDL_ttf.h>
TTF_Init();
TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
SDL_Surface* surf = TTF_RenderText_Solid(font, "Hello!", color);
```

## 3. SDL_mixer
Gestion avancée du son (musique de fond, effets sonores simultanés).
- Supporte le MP3, OGG, FLAC, MOD.
- Gère des "canaux" pour jouer plusieurs sons en même temps sans se soucier des détails techniques de mixage.
