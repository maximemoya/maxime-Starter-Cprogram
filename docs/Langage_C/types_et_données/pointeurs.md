# Les Pointeurs en C

Les pointeurs sont souvent considérés comme la partie la plus difficile du C, mais ils sont aussi sa plus grande force. Ils permettent de manipuler directement la mémoire.

---

## 1. Concept de base : L'Adresse
Chaque variable est stockée à une adresse précise dans la RAM (un numéro unique).
- `&variable` : Donne l'**adresse** de la variable.
- `*pointeur` : Accède à la **valeur** stockée à l'adresse pointée.

---

## 2. Déclaration et Utilisation
Un pointeur est une variable qui stocke une adresse mémoire.

```c
int age = 25;
int *p_age = &age; // p_age stocke l'adresse de age

printf("Adresse de age : %p\n", (void*)p_age);
printf("Valeur de age via le pointeur : %d\n", *p_age);
```

---

## 3. Pourquoi utiliser des pointeurs ?
1. **Passage par référence :** Modifier une variable à l'intérieur d'une fonction.
2. **Allocation dynamique :** Réserver de la mémoire pendant l'exécution (voir `malloc`).
3. **Tableaux :** En C, le nom d'un tableau est un pointeur vers son premier élément.

---

## 4. L'Arithmétique des pointeurs
Si vous ajoutez 1 à un pointeur, il ne pointe pas vers l'octet suivant, mais vers l'**élément suivant** du même type.

```c
int tab[3] = {10, 20, 30};
int *p = tab; // Pointe sur tab[0]
p++;          // Pointe maintenant sur tab[1] (avance de 4 octets car int=4)
```

---

## 5. Le pointeur `NULL`
Un pointeur qui ne pointe vers rien doit toujours être mis à `NULL`. Cela permet d'éviter d'accéder à des zones mémoire interdites.

```c
int *p = NULL;
if (p != NULL) {
    // Utilisation sûre
}
```

---

## 6. Pièges Classiques
- **Pointeur sauvage :** Utiliser un pointeur non initialisé.
- **Déréférencement de NULL :** Tenter de lire `*p` quand `p` est `NULL` (provoque un Segmentation Fault).
