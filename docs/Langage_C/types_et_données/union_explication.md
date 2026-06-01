# Comprendre les Structs et les Unions en C (Le cas SDL_Event)

Ce document explique la différence entre les `struct` et les `union`, ainsi que la manière dont la mémoire est gérée, en prenant pour exemple le système d'événements de SDL2.

---

## 1. La Structure (`struct`) : Accumulation
Une `struct` est une collection de variables où chaque membre possède sa propre place en mémoire.

### Mémoire
Les membres sont placés les uns après les autres. La taille totale est la somme des membres (plus d'éventuels octets de "padding" pour l'alignement).

```c
struct Personnage {
    int id;      // 4 octets
    float hp;    // 4 octets
    char lvl;    // 1 octet
}; 
// Taille totale ≈ 9 à 12 octets
```

**Visualisation :**
`[ ID (4) ] [ HP (4) ] [ LVL (1) ]`

---

## 2. L'Union (`union`) : Chevauchement
Une `union` permet de stocker différents types de données au **même endroit** en mémoire. On ne peut utiliser qu'un seul membre à la fois.

### Mémoire
Tous les membres commencent à l'**adresse zéro**. La taille de l'union est égale à la taille de son plus grand membre.

```c
union Donnees {
    int i;       // 4 octets
    float f;     // 4 octets
    char c;      // 1 octet
};
// Taille totale = 4 octets
```

**Visualisation :**
`[ i / f / c (4 octets partagés) ]`
*Si vous modifiez `i`, vous écrasez la valeur de `f`.*

---

## 3. Le secret de `SDL_Event` (Tagged Union)
`SDL_Event` est une union de plusieurs structures (clavier, souris, fenêtre). Comment SDL sait-il quel membre lire ? Grâce au **"Common Initial Sequence"** (Séquence Initiale Commune).

### La règle d'or
En C, si plusieurs structures commencent par le même type (ici `Uint32 type`), ce champ se retrouve exactement au même endroit en mémoire dans l'union.

```c
typedef union SDL_Event {
    Uint32 type;                    // Le "Tag" (étiquette)
    SDL_KeyboardEvent key;          // Structure clavier
    SDL_MouseButtonEvent button;    // Structure souris
} SDL_Event;
```

### Ce qu'il se passe en mémoire :
| Octets | 0 - 3 | 4 - 7 | 8 - 11 | ... |
| :--- | :--- | :--- | :--- | :--- |
| **SDL_Event.type** | `type` | (vide) | (vide) | |
| **SDL_Event.key** | `type` | `timestamp` | `windowID` | ... |
| **SDL_Event.button** | `type` | `timestamp` | `windowID` | ... |

**Résultat :** Quand vous lisez `e.type`, vous accédez aux 4 premiers octets, peu importe que l'événement soit un clic ou une touche pressée. C'est ce qui permet de faire le `switch(e.type)`.

---

## 4. Ordre et Alignement
1. **Ordre :** Le C garantit que l'ordre des membres dans le code est respecté en mémoire. Le premier membre déclaré est toujours à l'adresse de départ.
2. **Padding :** Le compilateur peut ajouter des octets vides entre les membres d'une `struct` pour que les données soient alignées sur 4 ou 8 octets (pour la performance du CPU).
3. **Casting :** Passer de `SDL_Event*` à `SDL_MouseButtonEvent*` est sûr car ils partagent le même début de mémoire.

---

## 5. Versions de C (C11/C17)
Depuis la norme **C11**, on peut utiliser des **Unions Anonymes**. Cela permet d'inclure une union dans une struct sans lui donner de nom, rendant les membres directement accessibles.

```c
struct {
    int type;
    union { // Anonyme
        int val_int;
        float val_float;
    };
} mon_objet;

mon_objet.val_int = 10; // Accès direct (au lieu de mon_objet.data.val_int)
```
