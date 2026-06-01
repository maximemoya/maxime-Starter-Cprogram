# Les Directives du Préprocesseur en C

Le préprocesseur est un programme qui traite votre code source **avant** que le compilateur ne le voie. Ses commandes commencent toujours par un dièse `#`.

---

## 1. `#include` : Inclusion de fichiers
Elle demande au préprocesseur de copier-coller le contenu d'un fichier à l'endroit de la directive.

- `#include <stdio.h>` : Recherche dans les répertoires système standards.
- `#include "mon_fichier.h"` : Recherche d'abord dans le répertoire local du projet.

---

## 2. `#define` : Définition de constantes
Permet de créer un alias pour une valeur ou un morceau de code.

```c
#define PI 3.14159
#define MAX_BUFFER 1024
```
*Le préprocesseur remplace toutes les occurrences de `PI` par `3.14159` dans le code avant la compilation.*

---

## 3. Compilation Conditionnelle
Permet d'inclure ou d'exclure des parties de code selon certaines conditions.

### `#ifdef` / `#ifndef` (If Defined / If Not Defined)
```c
#define DEBUG

#ifdef DEBUG
    printf("Mode Debug activé\n");
#endif
```

### `#if`, `#elif`, `#else`
Permet des tests plus complexes.
```c
#define VERSION 2

#if VERSION == 1
    // Code version 1
#elif VERSION == 2
    // Code version 2
#endif
```

---

## 4. `#undef` : Annulation
Supprime une définition précédemment créée avec `#define`.

---

## 5. Pourquoi utiliser ces directives ?
1. **Lisibilité :** Utiliser des noms explicites au lieu de nombres magiques.
2. **Portabilité :** Adapter le code selon l'OS (ex: `#ifdef _WIN32`).
3. **Sécurité :** Éviter les inclusions multiples grâce aux "Include Guards".
