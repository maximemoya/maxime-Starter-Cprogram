# Les Macros en C

Les macros sont des morceaux de code définis par `#define` qui peuvent prendre des paramètres. Elles sont remplacées par leur définition partout où elles sont appelées.

---

## 1. Déclaration d'une Macro
```c
#define CARRE(x) (x * x)
```
*Si vous écrivez `CARRE(5)`, le préprocesseur le remplacera par `(5 * 5)`.*

---

## 2. Macros vs Fonctions

| Point | Macro | Fonction |
| :--- | :--- | :--- |
| **Exécution** | Temps de pré-compilation (remplacement) | Temps d'exécution (appel) |
| **Vitesse** | Très rapide (pas d'appel de fonction) | Légèrement plus lente |
| **Type** | Aucun contrôle de type (générique) | Typage strict |
| **Taille** | Augmente la taille du binaire si utilisée souvent | Code partagé (plus compact) |

---

## 3. Les Pièges Classiques (⚠️ Danger)

### Le Piège des Parenthèses
Sans parenthèses, l'ordre des opérations peut être faussé.
```c
#define MULTIPLIER(a, b) a * b

int res = MULTIPLIER(2 + 3, 4); 
// Devient : 2 + 3 * 4 = 14 (au lieu de 5 * 4 = 20 !)
```
**Solution :** Toujours entourer les paramètres ET la macro de parenthèses.
```c
#define MULTIPLIER(a, b) ((a) * (b))
```

### Le Piège des Effets de Bord
Une macro évalue son argument autant de fois qu'il apparaît dans sa définition.
```c
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int x = 5, y = 10;
int m = MAX(++x, y); 
// x sera incrémenté une ou deux fois selon la condition !
```

---

## 4. Macros Multilignes
On utilise le backslash `\` pour continuer une macro sur la ligne suivante.
```c
#define LOG_ERROR(msg) do { \
    fprintf(stderr, "ERREUR: %s\n", msg); \
    exit(EXIT_FAILURE); \
} while(0)
```
*(L'astuce du `do { ... } while(0)` permet de s'assurer que la macro se comporte comme une seule instruction.)*

---

## 5. Quand les utiliser ?
- Pour de toutes petites opérations mathématiques simples.
- Pour des outils de débogage.
- Pour créer des abstractions de bas niveau là où une fonction serait trop coûteuse.
