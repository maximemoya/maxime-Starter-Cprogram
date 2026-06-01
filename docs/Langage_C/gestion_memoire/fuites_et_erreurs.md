# Fuites de Mémoire et Erreurs Courantes en C

La gestion manuelle de la mémoire est source de nombreux bugs. Voici comment les identifier et les éviter.

---

## 1. La Fuite de Mémoire (Memory Leak)
Une fuite se produit lorsque vous allouez de la mémoire avec `malloc` mais que vous perdez le pointeur sans avoir appelé `free`.

### Exemple de fuite
```c
void maFonction() {
    int *p = malloc(sizeof(int));
    *p = 42;
    // On quitte la fonction sans free(p). 
    // L'adresse est perdue, la mémoire reste occupée pour rien.
}
```

**Conséquence :** Le programme consomme de plus en plus de RAM jusqu'au plantage (surtout critique dans une boucle infinie ou un jeu).

---

## 2. Le Pointeur Sauvage (Dangling Pointer)
C'est un pointeur qui pointe vers une zone mémoire qui a déjà été libérée.

### Exemple
```c
int *p = malloc(sizeof(int));
free(p);
// p contient toujours l'ancienne adresse !
*p = 10; // ERREUR : Accès à une zone libérée (Use-after-free)
```

**Solution :** Toujours mettre le pointeur à `NULL` immédiatement après le `free`.

---

## 3. L'Erreur de Segmentation (Segfault)
C'est la tentative d'accès à une zone mémoire que le système d'exploitation ne vous a pas autorisée.

**Causes fréquentes :**
- Déréférencer un pointeur `NULL`.
- Débordement de tableau (écrire après la fin).
- Accéder à un pointeur sauvage.
- Débordement de la pile (*Stack Overflow*).

---

## 4. Outils de Diagnostic
- **Valgrind :** L'outil ultime sur Linux pour détecter les fuites de mémoire et les accès invalides.
- **AddressSanitizer (ASan) :** Un outil intégré à `gcc` et `clang` qui surveille la mémoire à l'exécution.
  - Compilation : `gcc -fsanitize=address -g main.c`

---

## 5. Comment éviter ces erreurs ?
1. **La Règle d'Or :** Moins vous utilisez `malloc`, moins vous avez de chances de vous tromper.
2. **Propriété Claire :** Définissez toujours qui est responsable de libérer la mémoire (la fonction qui alloue doit souvent être celle qui libère, ou le module propriétaire).
3. **Pointeurs NULL :** Initialisez à `NULL` et remettez à `NULL` après libération.
