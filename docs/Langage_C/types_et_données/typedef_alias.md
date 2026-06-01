# Typedef et Alias de Types en C

L'instruction `typedef` permet de créer un nouveau nom (un alias) pour un type existant. Cela sert à rendre le code plus court et plus lisible.

---

## 1. Utilisation simple
```c
typedef unsigned int uint;
uint score = 10; // Équivalent à unsigned int score = 10;
```

---

## 2. Simplifier les structures
C'est l'usage le plus courant. Sans `typedef`, il faut écrire `struct` à chaque déclaration.

```c
// Sans typedef
struct Point { int x, y; };
struct Point p1;

// Avec typedef
typedef struct {
    int x;
    int y;
} Point;

Point p1; // Plus besoin d'écrire 'struct'
```

---

## 3. Alias pour les pointeurs
On peut aussi créer des alias pour les types pointeurs complexes.

```c
typedef int* IntPtr;
IntPtr p; // p est un pointeur sur int
```

---

## 4. Pourquoi utiliser `typedef` ?
1. **Lisibilité :** Rendre les types complexes (comme les pointeurs de fonctions) plus faciles à lire.
2. **Portabilité :** Si vous changez de type (ex: passer de `float` à `double` pour la précision), vous n'avez qu'à changer le `typedef`.
3. **Abstraction :** Cacher le fait qu'une variable est une structure ou un simple entier.

---

## 5. Convention de nommage
Il est fréquent de rajouter un suffixe `_t` ou de mettre une majuscule pour distinguer les types créés des variables.
- Exemples : `uint32_t`, `Player`, `Context_t`.

---

## 6. Attention : Ne pas en abuser
Trop de `typedef` peut parfois rendre le code "mystérieux" car on ne sait plus quel est le type réel derrière l'alias. Il faut rester raisonnable !
