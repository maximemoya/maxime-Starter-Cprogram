# Les Pointeurs de Fonctions en C

Tout comme une variable a une adresse en mémoire, **le code d'une fonction a aussi une adresse**. Un pointeur de fonction stocke cette adresse et permet d'appeler la fonction dynamiquement.

---

## 1. Déclaration
La syntaxe est un peu particulière. Il faut spécifier le type de retour et les paramètres attendus.

```c
// Déclaration d'un pointeur nommé 'ptr_fonction' 
// vers une fonction prenant deux 'int' et renvoyant un 'int'.
int (*ptr_fonction)(int, int);
```
*(Attention aux parenthèses autour de `*ptr_fonction`, sans elles, le compilateur croirait que la fonction renvoie un `int*`).*

---

## 2. Assignation et Appel
On assigne l'adresse d'une fonction simplement en utilisant son nom (sans les parenthèses d'appel).

```c
int additionner(int a, int b) { return a + b; }
int multiplier(int a, int b) { return a * b; }

int main() {
    int (*operation)(int, int); // Pointeur
    
    operation = additionner; // Assigne la fonction d'addition
    printf("Résultat : %d\n", operation(5, 3)); // Affiche 8
    
    operation = multiplier; // Change dynamiquement
    printf("Résultat : %d\n", operation(5, 3)); // Affiche 15
}
```

---

## 3. Pourquoi les utiliser ?

### A. Les Callbacks (Rappels)
Passer une fonction en paramètre d'une autre fonction. C'est très utilisé dans les interfaces graphiques (ex: SDL2) ou pour des tris personnalisés.
```c
// qsort utilise un pointeur de fonction pour savoir comment comparer deux éléments
qsort(tableau, taille, sizeof(int), maFonctionDeComparaison);
```

### B. Les Tableaux de Fonctions
Très utiles pour remplacer des gros `switch` (Machines à états, interpréteurs de commandes).
```c
void attaquer() { /* ... */ }
void defendre() { /* ... */ }
void fuir() { /* ... */ }

// Tableau de pointeurs de fonctions
void (*actions[3])() = {attaquer, defendre, fuir};

int choix = 1; // Le joueur choisit "Défendre"
actions[choix](); // Appelle defendre() directement !
```

---

## 4. Simplifier avec `typedef`
La syntaxe peut devenir lourde. Un `typedef` rend les choses beaucoup plus lisibles :
```c
typedef int (*OperationMath)(int, int);

OperationMath op = additionner;
```
