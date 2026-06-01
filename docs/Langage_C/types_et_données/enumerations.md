# Les Énumérations (`enum`) en C

Les énumérations permettent de définir des noms symboliques pour des valeurs entières, rendant le code beaucoup plus lisible.

---

## 1. Pourquoi utiliser `enum` ?
Au lieu d'utiliser des "nombres magiques" difficiles à comprendre :

```c
// Moins lisible
if (etat == 0) { ... }

// Plus lisible
if (etat == ETAT_MARCHE) { ... }
```

---

## 2. Déclaration
```c
enum Couleur {
    ROUGE,   // vaut 0 par défaut
    VERT,    // vaut 1
    BLEU     // vaut 2
};

enum Couleur mon_fond = BLEU;
```

---

## 3. Forcer les valeurs
On peut attribuer des valeurs spécifiques à chaque membre.

```c
enum Erreur {
    OK = 0,
    ERR_PAS_DE_MEMOIRE = -1,
    ERR_FICHIER_INTROUVABLE = -2
};
```

---

## 4. Taille en mémoire
En C, une variable de type `enum` est généralement traitée comme un **`int`** (souvent 4 octets). Cela dépend toutefois de la plage de valeurs et de l'optimisation du compilateur.

---

## 5. Utilisation avec `switch`
Les `enum` sont parfaits pour être utilisés avec l'instruction `switch`.

```c
switch(etat_jeu) {
    case START: 
        afficher_menu(); 
        break;
    case PLAYING: 
        mettre_a_jour(); 
        break;
    case GAME_OVER: 
        fin(); 
        break;
}
```

---

## 6. Points Clés
- Les noms (`ROUGE`, `VERT`...) sont des constantes globales ou de portée locale.
- Une `enum` n'offre pas de sécurité de type stricte en C : on peut techniquement assigner `5` à une variable `enum Couleur` même si 5 n'est pas défini.
