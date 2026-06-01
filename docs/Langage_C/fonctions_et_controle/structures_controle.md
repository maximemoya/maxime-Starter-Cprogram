# Les Structures de Contrôle en C

Les structures de contrôle permettent de diriger le flux d'exécution du programme, de faire des choix et de répéter des actions.

---

## 1. Les Conditions (if / else)

Elles permettent d'exécuter du code seulement si une condition est vraie (non nulle en C).

```c
int hp = 50;

if (hp <= 0) {
    printf("Game Over!\n");
} else if (hp < 20) {
    printf("Attention, PV critiques !\n");
} else {
    printf("Tout va bien.\n");
}
```

*Note : En C, il n'y a pas de type booléen natif avant C99 (`<stdbool.h>`). Tout entier valant `0` est faux, tout entier différent de `0` est vrai.*

---

## 2. Les Choix Multiples (switch / case)

Idéal pour vérifier la valeur d'une seule variable entière ou d'une énumération (`enum`). Beaucoup plus propre qu'une longue série de `if / else if`.

```c
int direction = 2; // 1: Haut, 2: Droite, 3: Bas, 4: Gauche

switch(direction) {
    case 1:
        y -= 1;
        break; // N'oubliez pas le break, sinon les cases suivantes s'exécutent aussi !
    case 2:
        x += 1;
        break;
    default:
        printf("Direction inconnue.\n");
        break;
}
```

---

## 3. Les Boucles de Répétition

### La boucle `while` (Tant que)
Exécute le bloc tant que la condition est vraie. Utile quand on ne connaît pas le nombre d'itérations à l'avance (ex: la boucle principale du jeu).

```c
bool jeu_en_cours = true;

while(jeu_en_cours) {
    // Gérer événements, mise à jour, affichage
    // ...
}
```

### La boucle `do ... while`
Similaire à `while`, mais le bloc est **toujours exécuté au moins une fois** avant que la condition ne soit testée.

```c
int choix;
do {
    printf("Choisissez (1 ou 2) : ");
    scanf("%d", &choix);
} while (choix != 1 && choix != 2);
```

### La boucle `for` (Pour)
Idéale quand le nombre d'itérations est connu (comme pour parcourir un tableau). Elle regroupe l'initialisation, la condition et l'incrémentation sur la même ligne.

```c
for (int i = 0; i < 10; i++) {
    printf("Itération %d\n", i);
}
```

---

## 4. Les Instructions de Rupture
- `break` : Sort immédiatement de la boucle (ou du `switch`) la plus proche.
- `continue` : Saute le reste du code de l'itération actuelle et passe à la suivante dans la boucle.
