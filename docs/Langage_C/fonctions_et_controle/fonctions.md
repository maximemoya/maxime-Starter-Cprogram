# Les Fonctions en C

Les fonctions sont des blocs de code réutilisables qui effectuent une tâche spécifique. Elles permettent de structurer et d'organiser un programme.

---

## 1. Déclaration vs Définition

En C, il est crucial de différencier ces deux concepts, surtout lorsque le code est réparti sur plusieurs fichiers.

### La Déclaration (Prototype)
Elle indique au compilateur que la fonction existe, son nom, ce qu'elle prend en paramètres et ce qu'elle retourne. Elle se place généralement dans les fichiers d'en-tête (`.h`).

```c
int additionner(int a, int b);
```

### La Définition (Implémentation)
C'est le code réel de la fonction. Elle se place dans les fichiers source (`.c`).

```c
int additionner(int a, int b) {
    return a + b;
}
```

---

## 2. Le type de retour
Une fonction peut renvoyer une valeur via le mot-clé `return`.
- Si elle ne renvoie rien, son type de retour doit être `void`.

```c
void afficherMessage(const char *msg) {
    printf("%s\n", msg);
    // Pas de return
}
```

---

## 3. Passage des paramètres

### Passage par Valeur
Par défaut, le C copie la valeur des arguments passés à la fonction. La fonction travaille sur cette copie. Les modifications n'affectent pas la variable d'origine.

```c
void incrementer(int x) {
    x = x + 1; // x est une copie, la variable d'origine ne change pas
}
```

### Passage par Pointeur (Référence)
Pour modifier une variable de l'extérieur depuis l'intérieur d'une fonction, il faut passer son **adresse mémoire** (un pointeur). C'est la méthode à privilégier pour les grosses structures de données afin d'éviter des copies coûteuses.

```c
void incrementer_vrai(int *x) {
    *x = *x + 1; // On modifie la valeur à l'adresse mémoire donnée
}

// Utilisation :
// int score = 10;
// incrementer_vrai(&score); // score vaut maintenant 11
```

---

## 4. La fonction `main`
C'est le point d'entrée de tout programme C.

```c
int main(int argc, char *argv[]) {
    // ...
    return 0; // 0 signifie que le programme s'est terminé sans erreur
}
```
- `argc` : Le nombre d'arguments passés en ligne de commande.
- `argv` : Un tableau contenant ces arguments sous forme de chaînes de caractères.
