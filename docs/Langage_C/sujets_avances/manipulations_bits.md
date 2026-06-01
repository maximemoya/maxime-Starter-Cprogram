# Les Manipulations de Bits (Bitwise Operations) en C

Le langage C permet de manipuler les données à leur niveau le plus fondamental : le bit (0 ou 1). C'est très utile pour l'optimisation, l'embarqué, ou les drapeaux (flags).

---

## 1. Les Opérateurs Bit à Bit

| Opérateur | Nom | Description | Exemple (`A = 1100`, `B = 1010`) |
| :---: | :--- | :--- | :--- |
| `&` | ET (AND) | 1 si les deux bits sont 1. | `A & B` = `1000` |
| `\|` | OU (OR) | 1 si au moins un bit est 1. | `A \| B` = `1110` |
| `^` | OU Exclusif (XOR) | 1 si les bits sont différents. | `A ^ B` = `0110` |
| `~` | NON (NOT) | Inverse tous les bits. | `~A` = `0011` |
| `<<` | Décalage Gauche | Décale les bits vers la gauche. | `A << 1` = `1000` (x2) |
| `>>` | Décalage Droite | Décale les bits vers la droite. | `A >> 1` = `0110` (/2) |

---

## 2. Utilisation : Les Flags (Drapeaux)
Plutôt que d'avoir plusieurs variables booléennes, on utilise un seul entier où chaque bit représente une option.

```c
#define FLAG_VISIBLE   0x01  // 0000 0001 (1)
#define FLAG_INVINCIBLE 0x02 // 0000 0010 (2)
#define FLAG_POISONED  0x04  // 0000 0100 (4)

unsigned char status = 0; // Aucun flag actif (0000 0000)
```

### Activer un bit (OR `|`)
```c
status |= FLAG_VISIBLE; // status devient 0000 0001
```

### Désactiver un bit (AND NOT `& ~`)
```c
status &= ~FLAG_VISIBLE; // Force le bit à 0
```

### Inverser un bit (XOR `^`)
```c
status ^= FLAG_INVINCIBLE; // Bascule l'invincibilité
```

### Vérifier un bit (AND `&`)
```c
if (status & FLAG_POISONED) {
    // Le joueur est empoisonné
}
```

---

## 3. Astuces et Pièges
- **Masques (Masks) :** Les constantes hexadécimales (`0x01`, `0x0F`, `0xFF`) sont très souvent utilisées pour créer des "masques" qui isolent certains bits.
- **Décalages rapides :** `x << 1` est généralement plus rapide que `x * 2`. `x >> 1` est plus rapide que `x / 2`.
- **Parenthèses :** Les opérateurs bit à bit ont une priorité très basse en C. Mettez **toujours** des parenthèses autour de vos opérations !
  - Mauvais : `if (status & FLAG == FLAG)`
  - Bon : `if ((status & FLAG) == FLAG)`
