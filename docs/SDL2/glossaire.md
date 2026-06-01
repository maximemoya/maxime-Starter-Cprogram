# Glossaire des fonctionnalités SDL2

Ce document répertorie les principales capacités de la bibliothèque SDL2 (Simple DirectMedia Layer 2), utilisée dans ce projet pour la gestion de l'affichage pixel art et des entrées utilisateur.

## 1. Introduction
SDL2 est une bibliothèque de développement multiplateforme conçue pour fournir un accès de bas niveau à l'audio, au clavier, à la souris, au joystick et au matériel graphique via OpenGL et Direct3D.

## 2. Initialisation et Sous-systèmes
SDL2 est modulaire. On initialise les composants nécessaires via `SDL_Init()`.
- `SDL_INIT_VIDEO` : Gestion des fenêtres et des drivers graphiques.
- `SDL_INIT_AUDIO` : Sortie et entrée audio.
- `SDL_INIT_EVENTS` : Gestion de la file d'événements.
- `SDL_INIT_JOYSTICK` / `SDL_INIT_GAMECONTROLLER` : Support des manettes.
- `SDL_INIT_TIMER` : Gestion précise du temps.

## 3. Gestion des Fenêtres (Windowing)
Permet de créer et manipuler des fenêtres système.
- **Création** : `SDL_CreateWindow()`.
- **États** : Support du plein écran (`SDL_WINDOW_FULLSCREEN`), fenêtres sans bordures, redimensionnables ou cachées.
- **Multi-écrans** : Gestion de plusieurs moniteurs.

## 4. Rendu 2D Accéléré (Render API)
Le moteur de rendu 2D moderne de SDL2 utilise l'accélération matérielle (GPU).
- **Renderer** : `SDL_CreateRenderer()` crée le contexte de dessin lié à une fenêtre.
- **Textures** : `SDL_Texture` représente des images stockées en mémoire vidéo (plus performant que les anciennes `SDL_Surface`).
- **Primitives** : Fonctions natives pour dessiner des points (`SDL_RenderDrawPoint`), des lignes (`SDL_RenderDrawLine`) et des rectangles (`SDL_RenderFillRect`).
- **Copie** : `SDL_RenderCopy()` permet d'afficher une partie d'une texture sur une partie de l'écran (idéal pour les sprites et le pixel art).

## 5. Gestion des Événements
SDL2 capture tous les signaux système dans une file d'attente.
- **Boucle principale** : Utilisation de `SDL_PollEvent()` pour traiter les messages sans bloquer le programme.
- **Clavier** : Distingue les `Keycodes` (la lettre physique) des `Scancodes` (la position de la touche sur le clavier, idéal pour les configurations AZERTY/QWERTY).
- **Souris** : Position relative/absolue, état des boutons et molette.
- **Fenêtre** : Détection de la fermeture (croix rouge), du redimensionnement ou de la perte de focus.

## 6. Gestion du Temps (Timing)
Indispensable pour maintenir un framerate constant.
- `SDL_GetTicks()` : Nombre de millisecondes écoulées depuis l'initialisation.
- `SDL_GetPerformanceCounter()` : Pour une précision encore plus élevée (microsecondes).
- `SDL_Delay()` : Met le programme en pause pour économiser les ressources CPU.

## 7. Gestion de la Mémoire et des Fichiers
- **RWops** : Une interface d'abstraction (`SDL_RWops`) permettant de lire ou écrire des données depuis des fichiers, la mémoire ou même des ressources compressées de manière identique sur Windows, Linux, macOS ou Android.

## 8. Extensions Officielles (Satellites)
Bien que non incluses dans le cœur de SDL2, ces bibliothèques sont le standard pour compléter ses fonctionnalités :
- **SDL_image** : Support de formats comme PNG, JPG, TIF, WebP.
- **SDL_ttf** : Rendu de texte de haute qualité à partir de polices TrueType (.ttf).
- **SDL_mixer** : Gestion multi-canal du son, support du MP3, OGG, et des musiques de fond.
