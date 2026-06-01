# Tableaux et Chaînes de Caractères en C

En C, les tableaux et les chaînes de caractères sont intimement liés aux pointeurs.

---

## 1. Les Tableaux (Arrays)
Un tableau est une suite d'éléments de même type stockés de manière contiguë en mémoire.

```c
int scores[5] = {10, 20, 30, 40, 50};
printf("%d", scores[0]); // Accès au premier élément
```

### Lien avec les pointeurs
Le nom du tableau (`scores`) est en réalité l'adresse de son premier élément.
`scores[2]` est strictement équivalent à `*(scores + 2)`.

---

## 2. Les Chaînes de Caractères (Strings)
Le C n'a pas de type "String" natif. Une chaîne est simplement un **tableau de `char`** qui se termine par un caractère spécial : `\0` (le caractère nul).

```c
char nom[] = "Alan"; 
// En mémoire : ['A', 'l', 'a', 'n', '\0']
```

### Pourquoi le `\0` ?
Sans lui, les fonctions comme `printf` ou `strlen` ne sauraient pas où s'arrêter en mémoire et liraient ce qui se trouve après la chaîne (provoquant des bugs).

---

## 3. Manipulation de Chaînes (`<string.h>`)
Pour manipuler les chaînes, on utilise la bibliothèque standard :
- `strlen(s)` : Donne la longueur (sans compter le `\0`).
- `strcpy(dest, src)` : Copie une chaîne.
- `strcmp(s1, s2)` : Compare deux chaînes.

---

## 4. Tableaux 2D (Matrices)
On peut déclarer des tableaux à plusieurs dimensions.

```c
int grille[3][3]; // Une grille de 3x3
grille[0][0] = 1;
```

En mémoire, un tableau 2D est stocké "à plat", ligne après ligne.

---

## 5. Points d'attention
1. **Débordement (Buffer Overflow) :** Le C ne vérifie pas si vous accédez à un index en dehors du tableau. Si vous écrivez dans `scores[10]` alors que la taille est 5, vous corrompez la mémoire.
2. **Taille Fixe :** La taille d'un tableau statique doit être connue à la compilation. Pour une taille variable, il faut utiliser l'allocation dynamique.
3. **Initialisation :** `char s[10] = "";` initialise toute la chaîne à zéro.
