# Séparation du Code et Include Guards

Pour les projets importants, il est impératif de séparer les déclarations de l'implémentation pour garder un code propre et éviter les erreurs de compilation.

---

## 1. Fichier d'en-tête (`.h`) vs Fichier Source (`.c`)

### Le Fichier d'en-tête (`.h`)
C'est le "manuel d'utilisation" du module. Il contient ce qui est **public**.
- Prototypes de fonctions.
- Déclarations de structures (`struct`).
- Constantes (`#define`) et `enum`.
- Variables globales partagées (via `extern`).

### Le Fichier Source (`.c`)
C'est la "boîte noire". Il contient la **logique interne**.
- Implémentation des fonctions.
- Variables et fonctions privées au module (via `static`).

---

## 2. Pourquoi séparer ?
1. **Compilation plus rapide :** Si vous modifiez un `.c`, seul ce fichier est recompilé.
2. **Clarté :** On voit tout de suite les fonctions disponibles sans lire des milliers de lignes de logique.
3. **Prévention des erreurs :** Évite les définitions multiples.

---

## 3. Les "Include Guards"
Lorsqu'un projet grandit, un fichier `.h` peut être inclus plusieurs fois indirectement. Sans protection, le compilateur verra deux fois les mêmes structures, ce qui provoquera une erreur.

**Solution :** Utiliser des directives de préprocesseur pour n'inclure le fichier qu'une seule fois.

```c
#ifndef MON_MODULE_H
#define MON_MODULE_H

// Tout le contenu du fichier .h ici

#endif
```

---

## 4. Organisation Type (utilisée ici)
```text
/include/  -> Contient les .h (les contrats)
/src/      -> Contient les .c (l'implémentation)
```
Dans chaque `.c`, on commence par inclure son `.h` correspondant :
`#include "mon_module.h"`

---

## 5. Erreurs à éviter
- **Ne jamais inclure de `.c`** dans un autre fichier. On n'inclut que des `.h`.
- **Ne pas définir de corps de fonction** dans un `.h` (sauf cas très particuliers comme les fonctions `inline`), car cela créerait des doublons au moment du lien (Linker).
