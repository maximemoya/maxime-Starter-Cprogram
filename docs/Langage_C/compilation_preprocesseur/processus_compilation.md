# Le Processus de Compilation en C

Transformer un fichier texte `.c` en un programme exécutable est un processus complexe composé de 4 étapes majeures.

---

## 1. Le Préprocesseur (`gcc -E`)
Le préprocesseur nettoie le code et traite les directives (`#`).
- **Inclusion :** Remplace les `#include` par le contenu des fichiers.
- **Substitution :** Remplace les macros et constantes par leurs valeurs.
- **Nettoyage :** Supprime les commentaires.
- **Sortie :** Un fichier temporaire `.i` (code source pur).

---

## 2. Le Compilateur (`gcc -S`)
Il traduit le code C en **Langage Assembleur**, un langage de bas niveau propre à l'architecture de votre processeur (x86, ARM, etc.).
- **Vérification :** Analyse la syntaxe et la cohérence des types.
- **Optimisation :** Améliore le code pour le rendre plus rapide.
- **Sortie :** Un fichier `.s`.

---

## 3. L'Assembleur (`gcc -c`)
Il traduit le langage assembleur en **Code Objet** (langage machine binaire que le CPU comprend).
- **Relocalisation :** Les adresses mémoires ne sont pas encore définitives.
- **Sortie :** Un fichier `.o` (ou `.obj` sur Windows). C'est du binaire, mais pas encore exécutable.

---

## 4. L'Éditeur de Liens / Linker (`gcc`)
Il rassemble tous les fichiers objets (`.o`) du projet et les bibliothèques externes (ex: SDL2, math.h) pour créer l'exécutable final.
- **Résolution :** Il lie les appels de fonctions à leurs définitions réelles dans d'autres fichiers.
- **Sortie :** Le fichier **Exécutable** (ex: `a.out`, `main.exe`).

---

## Schéma Récapitulatif
```text
[ .c ] -> Préprocesseur -> [ .i ]
[ .i ] -> Compilateur   -> [ .s ]
[ .s ] -> Assembleur    -> [ .o ]
[ .o ] + [ Biblios ] -> Linker -> [ EXÉCUTABLE ]
```

---

## 5. Commandes Utiles
- `gcc main.c` : Compilation complète.
- `gcc -c main.c` : S'arrête après l'assemblage (produit `main.o`).
- `gcc -o mon_programme main.c` : Nomme l'exécutable final.
