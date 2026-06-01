# Portée et Durée de Vie des Variables en C

Comprendre où une variable est accessible et combien de temps elle existe en mémoire est fondamental pour éviter les bugs et sécuriser son code.

---

## 1. La Portée (Scope)

La portée définit la région du code où une variable est visible et utilisable.

### Portée Locale (Bloc)
Une variable déclarée à l'intérieur d'une paire d'accolades `{ }` n'existe qu'à l'intérieur de ce bloc.
```c
void maFonction() {
    int locale = 5; // Existe uniquement ici
    if (locale == 5) {
        int i = 0;  // Existe uniquement dans ce if
    }
}
```

### Portée Globale (Fichier)
Une variable déclarée en dehors de toute fonction est accessible par toutes les fonctions situées après sa déclaration.
```c
int score_global = 100; // Accessible partout

void afficher() {
    printf("%d", score_global);
}
```
*Note : L'utilisation de variables globales est généralement déconseillée car elle rend le code difficile à maintenir.*

---

## 2. La Durée de Vie (Lifetime)

La durée de vie indique combien de temps la donnée reste en mémoire.

### Automatique (Pile)
Les variables locales ont une durée de vie automatique. Elles sont créées à l'entrée du bloc et détruites à la sortie.

### Dynamique (Tas)
Les variables créées via `malloc` vivent jusqu'à ce que `free` soit appelé (ou à la fin du programme).

### Statique
Les variables statiques vivent pendant toute l'exécution du programme, de son lancement à sa fin.

---

## 3. Les Modificateurs Spéciaux

### Le mot-clé `static`
Son effet dépend d'où il est utilisé :
1. **Sur une variable locale :** Elle garde sa valeur entre deux appels de la fonction (elle est stockée dans la zone statique de la mémoire, pas sur la pile).
   ```c
   void compteur() {
       static int appels = 0; // Initialisé une seule fois
       appels++;
       printf("Appel #%d\n", appels);
   }
   ```
2. **Sur une fonction ou une variable globale :** Elle restreint la portée de cette variable/fonction au **fichier actuel uniquement**. C'est crucial pour l'encapsulation (l'équivalent de "private" en C++).

### Le mot-clé `extern`
Il permet de déclarer une variable globale qui est **définie dans un autre fichier**.
```c
// Dans fichier_b.c
extern int score_global; // Indique que score_global existe ailleurs

void utiliserScore() {
    score_global += 10;
}
```
*C'est le pattern utilisé dans le projet pour l'objet `player`.*
