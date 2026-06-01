# Les Structures (`struct`) en C

Une structure permet de regrouper plusieurs variables de types différents sous un même nom. C'est l'ancêtre des "objets" en programmation.

---

## 1. Déclaration et Définition
On définit d'abord le "modèle" de la structure, puis on crée des instances.

```c
struct Personnage {
    char nom[20];
    int pv;
    float vitesse;
};

struct Personnage heros = {"Alan", 100, 5.5f};
```

---

## 2. Accès aux membres
On utilise l'opérateur point `.` pour accéder aux variables internes.

```c
heros.pv -= 10;
printf("PV de %s : %d\n", heros.nom, heros.pv);
```

---

## 3. Pointeurs et Structures (L'opérateur `->`)
C'est un cas très fréquent en C. Pour accéder à un membre via un pointeur, on utilise la flèche `->`.

```c
struct Personnage *p = &heros;
p->pv = 50; // Équivalent à (*p).pv = 50;
```

---

## 4. Alignement et Padding
Le compilateur peut ajouter des octets "vides" (padding) entre les membres pour que les données soient alignées sur des adresses multiples de 4 ou 8 (pour la vitesse du processeur).

```c
struct Exemple {
    char c;    // 1 octet
    // 3 octets de padding ici
    int i;     // 4 octets
};
// Taille totale = 8 octets (et non 5 !)
```

---

## 5. Passage en paramètre
- **Par valeur :** `void maFonction(struct Personnage p);` (Copie toute la structure, lent si elle est grosse).
- **Par pointeur :** `void maFonction(struct Personnage *p);` (Passe juste l'adresse, rapide et permet la modification). **C'est la méthode recommandée.**

---

## 6. Différence avec l'Union
- `struct` : Les membres sont les uns **après** les autres en mémoire.
- `union` : Les membres sont au **même endroit** en mémoire.
