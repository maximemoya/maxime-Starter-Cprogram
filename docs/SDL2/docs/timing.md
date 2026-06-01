# Gestion du Temps (Timing)

Le contrôle du temps est crucial pour garantir que le jeu tourne à la même vitesse sur tous les ordinateurs.

## 1. Mesure du temps
- `SDL_GetTicks()` : Retourne le nombre de millisecondes depuis le lancement de SDL. Simple et efficace.
- `SDL_GetPerformanceCounter()` : Plus précis, utile pour calculer des deltas très fins.

## 2. Limiter les FPS
On utilise souvent `SDL_Delay()` pour "reposer" le processeur si une frame a été calculée trop vite.
```c
uint32_t frameStart = SDL_GetTicks();
// ... faire le rendu ...
uint32_t frameTime = SDL_GetTicks() - frameStart;

if (frameTime < (1000 / 60)) { // Si < 60 FPS
    SDL_Delay((1000 / 60) - frameTime);
}
```

## 3. Delta Time
Pour les déplacements fluides, on calcule le `dt` (temps écoulé entre deux frames) et on multiplie les vitesses par ce facteur.
