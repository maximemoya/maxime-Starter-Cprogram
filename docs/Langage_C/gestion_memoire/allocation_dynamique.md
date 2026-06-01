# L'Allocation Dynamique en C

L'allocation dynamique permet de réserver de la mémoire sur le Tas (Heap) pendant l'exécution du programme.

---

## 1. Les fonctions de base (`<stdlib.h>`)

### `malloc` (Memory Allocation)
Réserve un bloc de mémoire de la taille spécifiée (en octets). La mémoire n'est **pas initialisée** (elle contient n'importe quoi).
```c
int *p = malloc(sizeof(int) * 10); // Alloue un tableau de 10 entiers
```

### `calloc` (Contiguous Allocation)
Similaire à `malloc`, mais initialise tous les octets à **zéro**.
```c
int *p = calloc(10, sizeof(int));
```

### `realloc` (Re-Allocation)
Modifie la taille d'un bloc déjà alloué.
```c
p = realloc(p, sizeof(int) * 20); // Agrandit le tableau à 20 entiers
```

### `free` (Libération)
Libère la mémoire précédemment allouée pour qu'elle puisse être réutilisée. **Obligatoire pour éviter les fuites.**
```c
free(p);
```

---

## 2. Le rituel de l'allocation
Chaque allocation doit suivre ces étapes :
1. **Calcul de la taille :** Utilisez toujours `sizeof`.
2. **Vérification du retour :** `malloc` renvoie `NULL` si la mémoire est pleine.
3. **Utilisation.**
4. **Libération :** Appelez `free`.
5. **Nettoyage :** Mettez le pointeur à `NULL` après le `free`.

```c
int *p = malloc(sizeof(int));
if (p == NULL) {
    fprintf(stderr, "Erreur d'allocation !\n");
    return EXIT_FAILURE;
}

// ... utilisation ...

free(p);
p = NULL; 
```

---

## 3. La "Règle d'Or" (Rappel)
Dans ce projet, l'allocation dynamique est vue comme un dernier recours. 
- **Pourquoi ?** Parce qu'elle introduit des risques de fuites, de fragmentation et de pointeurs nuls.
- **Alternative :** Utilisez des tableaux statiques avec une taille maximale définie par une macro (`#define MAX_ITEMS 100`).

---

## 4. Quand `malloc` est-il indispensable ?
1. La taille exacte de la donnée est inconnue à la compilation (ex: charger un fichier de taille variable).
2. La structure de données est récursive (listes chaînées, arbres).
3. L'objet est trop gros pour la pile (plusieurs Mo).
