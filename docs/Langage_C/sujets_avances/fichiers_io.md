# Manipulation de Fichiers et Entrées/Sorties en C

Le langage C permet de lire et d'écrire dans des fichiers sur le disque dur via la bibliothèque `<stdio.h>`.

---

## 1. Le pointeur `FILE *`
Pour manipuler un fichier, on utilise un pointeur spécial de type `FILE`.

```c
FILE *fichier = NULL;
```

---

## 2. Ouvrir et Fermer (`fopen`, `fclose`)

### Ouverture
La fonction `fopen` prend le chemin du fichier et le "mode" d'ouverture.
```c
fichier = fopen("sauvegarde.txt", "w");
if (fichier == NULL) {
    printf("Erreur : Impossible d'ouvrir le fichier.\n");
    return;
}
```

**Les modes principaux :**
- `"r"` (Read) : Lecture seule (le fichier doit exister).
- `"w"` (Write) : Écriture (crée le fichier ou l'écrase s'il existe).
- `"a"` (Append) : Ajout à la fin du fichier.
- Rajoutez `"b"` pour le mode binaire (`"rb"`, `"wb"`).

### Fermeture
Vous **devez** fermer chaque fichier ouvert avec `fclose` pour libérer la ressource système.
```c
fclose(fichier);
```

---

## 3. Fichiers Textes

### Écrire (`fprintf`)
Fonctionne exactement comme `printf`, mais écrit dans le fichier spécifié.
```c
fprintf(fichier, "Score du joueur %s : %d\n", nom, score);
```

### Lire (`fscanf`, `fgets`)
- `fscanf` lit des données formatées (attention aux espaces).
- `fgets` lit une ligne entière de texte (beaucoup plus sûr).
```c
char ligne[100];
if (fgets(ligne, 100, fichier) != NULL) {
    printf("Ligne lue : %s", ligne);
}
```

---

## 4. Fichiers Binaires (`fread`, `fwrite`)
Pour sauvegarder des structures de données entières de manière rapide et compacte (sans les transformer en texte), on utilise le mode binaire.

**Écrire une structure :**
```c
struct Personnage heros = {"Alan", 100};
FILE *f = fopen("save.bin", "wb");
if (f) {
    fwrite(&heros, sizeof(struct Personnage), 1, f);
    fclose(f);
}
```

**Lire une structure :**
```c
struct Personnage charge;
FILE *f = fopen("save.bin", "rb");
if (f) {
    fread(&charge, sizeof(struct Personnage), 1, f);
    fclose(f);
}
```
*C'est la méthode idéale pour gérer les sauvegardes de jeux !*
