# Les Arguments du Programme (`argc`, `argv`) en C

Lorsque vous lancez un programme en ligne de commande, vous pouvez lui passer des paramètres. Le C les récupère via les paramètres spéciaux de la fonction `main`.

---

## 1. La signature complète de `main`

Pour gérer les arguments, la fonction `main` s'écrit ainsi :
```c
int main(int argc, char *argv[]) {
    // ...
    return 0;
}
```
- `argc` (**Arg**ument **C**ount) : Un entier qui contient le nombre total de paramètres passés au programme.
- `argv` (**Arg**ument **V**ector) : Un tableau de chaînes de caractères (pointeurs sur `char`) contenant les paramètres eux-mêmes.

---

## 2. Comment ça marche ?

Imaginons que vous lanciez votre programme ainsi dans le terminal :
```bash
./mon_jeu --fullscreen map_level_1.txt
```

Dans ce cas :
- `argc` vaudra **3**.
- `argv[0]` = `"./mon_jeu"` *(Le premier argument est **toujours** le nom du programme lui-même)*.
- `argv[1]` = `"--fullscreen"`
- `argv[2]` = `"map_level_1.txt"`
- `argv[3]` = `NULL` *(Le tableau se termine toujours par NULL)*.

---

## 3. Exemple d'utilisation

Voici un exemple simple qui parcourt et affiche tous les arguments :

```c
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    printf("Nombre d'arguments : %d\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("Argument %d : %s\n", i, argv[i]);
        
        // Comparer un argument
        if (strcmp(argv[i], "--debug") == 0) {
            printf(">>> Mode Debug Activé !\n");
        }
    }
    
    return 0;
}
```

---

## 4. Conversion des Arguments
Tous les arguments reçus via `argv` sont du **texte** (des chaînes de caractères). 
Si vous passez un nombre (ex: `./mon_programme 42`), `argv[1]` vaudra la chaîne `"42"`. 

Pour l'utiliser comme un vrai nombre en C, il faut le convertir en entier via la fonction `atoi` ou `strtol` (bibliothèque `<stdlib.h>`).

```c
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc > 1) {
        int valeur = atoi(argv[1]);
        printf("Valeur multipliée par 2 : %d\n", valeur * 2);
    }
    return 0;
}
```
