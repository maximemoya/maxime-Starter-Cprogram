# Gestion des Fichiers et Mémoire

SDL2 propose une couche d'abstraction pour manipuler les données de manière portable : **SDL_RWops**.

## 1. Pourquoi utiliser RWops ?
Normalement, `fopen` fonctionne bien sous Windows/Linux, mais peut poser problème sur Android (fichiers dans des APK) ou sur certaines consoles. `SDL_RWops` unifie l'accès.

## 2. Fonctions principales
- `SDL_RWFromFile(const char* file, const char* mode)` : Ouvre un fichier.
- `SDL_RWread(rw, ptr, size, maxnum)` : Lit des données.
- `SDL_RWclose(rw)` : Ferme la ressource.

## 3. Utilité pour les librairies satellites
La plupart des fonctions de `SDL_image` ou `SDL_ttf` acceptent des `SDL_RWops`, ce qui permet de charger des assets depuis la mémoire vive ou des fichiers compressés.
