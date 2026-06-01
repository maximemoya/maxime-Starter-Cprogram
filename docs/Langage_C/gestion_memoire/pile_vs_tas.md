# La Pile (Stack) vs Le Tas (Heap) en C

Comprendre la différence entre la Pile et le Tas est essentiel pour gérer efficacement la mémoire de vos programmes.

---

## 1. La Pile (Stack)
La pile est une zone mémoire gérée automatiquement par le processeur. Elle fonctionne sur le principe LIFO (*Last In, First Out*).

### Caractéristiques
- **Automatique :** La mémoire est allouée quand vous déclarez une variable locale et libérée à la fin de la fonction (`}`).
- **Rapide :** L'accès est extrêmement performant car géré directement par le CPU.
- **Limitée :** La taille de la pile est fixe et relativement petite (quelques Mo). Si vous allouez trop de données, vous risquez un *Stack Overflow*.

```c
void maFonction() {
    int x = 10; // Alloué sur la Pile
} // x est libéré ici automatiquement
```

---

## 2. Le Tas (Heap)
Le tas est une zone mémoire beaucoup plus vaste destinée à l'allocation dynamique. C'est vous qui en avez le contrôle.

### Caractéristiques
- **Manuelle :** Vous demandez de la mémoire avec `malloc` et vous **devez** la libérer avec `free`.
- **Flexible :** La taille n'est limitée que par la RAM disponible. Les données survivent à la fin de la fonction qui les a créées.
- **Plus lente :** L'allocation et la libération prennent plus de temps que sur la pile.

```c
int *p = malloc(sizeof(int)); // Alloué sur le Tas
*p = 20;
free(p); // Libéré manuellement
```

---

## 3. Tableau Comparatif

| Point | Pile (Stack) | Tas (Heap) |
| :--- | :--- | :--- |
| **Gestion** | Automatique (CPU) | Manuelle (Programmeur) |
| **Vitesse** | Très rapide | Plus lente |
| **Taille** | Petite, limitée | Très grande |
| **Durée de vie** | Portée de la fonction | Jusqu'au `free()` ou fin du programme |
| **Fragmentation** | Impossible | Possible |

---

## 4. Quand utiliser quoi ?
- **Utilisez la Pile par défaut** pour les variables locales, les petites structures et les tableaux de taille fixe.
- **Utilisez le Tas** si la donnée est trop volumineuse pour la pile ou si elle doit être accessible par plusieurs fonctions successives sans être copiée.

---

## 5. La "Règle d'Or" du Projet
> "N'utilise `malloc` que si tu ne peux pas faire autrement."
Privilégiez les objets statiques ou les variables sur la pile pour éviter la complexité et les risques liés à la gestion manuelle du Tas.
