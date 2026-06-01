# Les Variables et Types de Base en C

Ce document présente les briques fondamentales du langage C : les variables et leurs types primitifs.

---

## 1. Qu'est-ce qu'une variable ?
Une variable est une zone de la mémoire vive (RAM) à laquelle on donne un nom. Elle permet de stocker une valeur que l'on peut modifier au cours de l'exécution du programme.

```c
int score = 100;
```

---

## 2. Les Types Entiers
Ils servent à stocker des nombres sans virgule.

| Type | Taille (octets) | Valeurs (approx.) |
| :--- | :--- | :--- |
| `char` | 1 | -128 à 127 (ou 0 à 255) |
| `short` | 2 | -32 768 à 32 767 |
| `int` | 4 | -2 milliards à 2 milliards |
| `long` | 4 ou 8 | Très grand |
| `long long` | 8 | Astronomique |

**Note :** On peut utiliser `unsigned` devant ces types (ex: `unsigned int`) pour ne stocker que des valeurs positives, doublant ainsi la limite haute.

---

## 3. Les Types Flottants
Ils servent à stocker des nombres avec une virgule (nombres réels).

- `float` : Précision simple (4 octets).
- `double` : Précision double (8 octets). C'est le type par défaut pour les nombres à virgule en C.

---

## 4. Le type `char` et les caractères
En C, un `char` est un entier de 1 octet. On l'utilise pour stocker des caractères via le code ASCII.

```c
char lettre = 'A'; // En mémoire, c'est la valeur 65
```

---

## 5. L'opérateur `sizeof`
Il permet de connaître la taille exacte d'un type ou d'une variable sur votre machine (car cela peut varier selon le processeur).

```c
printf("Taille d'un int : %lu octets\n", sizeof(int));
```

---

## 6. Bonnes Pratiques
1. **Initialisation :** Toujours donner une valeur de départ à vos variables. Une variable non initialisée contient n'importe quoi ("valeur poubelle").
2. **Nommage :** Utilisez des noms explicites (`pv_joueur` plutôt que `p`).
3. **Types Fixes (`<stdint.h>`) :** Pour être sûr de la taille (ex: 32 bits partout), utilisez `int32_t`, `uint8_t`, etc.
