# Pokémon Battle Simulator

Un simulateur de combat Pokémon en C++ offrant une expérience de jeu complète en ligne de commande.

## 📋 Description

Ce simulateur reproduit l'expérience de combat de la franchise Pokémon, permettant aux joueurs de capturer des Pokémon, de les entraîner, et de défier des dresseurs, des Leaders d'arène et des Maîtres.

## 🎮 Fonctionnalités

- **Gestion d'équipe** : Gérez une équipe de jusqu'à 6 Pokémon
- **Combats Pokémon** : Affrontez des Leaders d'arène, des Maîtres et d'autres dresseurs
- **Système de badges** : Gagnez des badges en battant les Leaders d'arène
- **Interactions** : Interagissez avec vos Pokémon et les dresseurs vaincus
- **Statistiques** : Suivez vos performances avec un système de statistiques
- **Interface colorée** : Une interface en ligne de commande avec codes ANSI pour une meilleure expérience

## 🛠️ Compilation et exécution

### Prérequis
- Compilateur C++ avec support C++17
- Make (pour la méthode de compilation avec make)

### Compilation et Exécution avec G++

**Méthode 1 - directement avec g++**
```bash
g++ -std=c++17 src/*.cpp -o pokemon -Iinclude
```
**Méthode 1 - exécution directe**
```bash
./pokemon
```

### Compilation et Exécution avec Make

**Méthode 2 - avec make**
```bash
make
```


**Méthode 2 - avec make**
```bash
make run
```

## 📁 Structure du projet

- **src/** : Contient les fichiers source C++
- **include/** : Contient les fichiers d'en-tête
- **data/** : Contient les fichiers de données (Pokémon, dresseurs, etc.)

## 🕹️ Guide de jeu

1. À partir du menu principal, choisissez votre action.
2. Pour commencer, explorez vos Pokémon (option 1).
3. Défiez des Leaders (option 4) pour gagner des badges.
4. Une fois tous les badges collectés, défiez les Maîtres (option 5).
5. Vous pouvez soigner vos Pokémon à tout moment (option 2).
6. Consultez vos statistiques (option 3) pour suivre votre progression.

## 🧩 Système de combat

- Les combats contre les Leaders se font avec 3 Pokémon.
- Les combats contre les Maîtres se font avec 6 Pokémon.
- Les Maîtres disposent d'un bonus de dégâts.
- Votre équipe doit être en forme avant un combat.

## 👨‍💻 Développement

Le jeu a été développé en C++ avec une architecture orientée objet. Il utilise une séparation claire entre les responsabilités (modèle, chargement de données, interface).

## 📝 Notes

- Les Pokémon, Leaders et Maîtres sont chargés depuis des fichiers CSV dans le dossier data/.
- Les statistiques sont sauvegardées automatiquement après chaque combat.
