```mermaid
classDiagram
    class Interagir {
        <<interface>>
        +~Interagir()
        +interagir() string
    }

    class Pokemon {
        -string nom
        -vector~Type~ types
        -int maxHp
        -int currentHp
        -string nomAttaque
        -int degatsAttaque
        +Pokemon(nom, types, hp, nomAttaque, degatsAttaque)
        +getNom() string
        +getTypes() const vector~Type~&
        +getMaxHp() int
        +getCurrentHp() int
        +getNomAttaque() string
        +getDegatsAttaque() int
        +subirDegats(degats)
        +estKO() bool
        +soigner()
        +getEfficaciteType(typeAttaque) double
        +estCoupCritique(efficacite) bool
        +getChanceCritique(efficacite) double
        +interagir() string
    }

    class Entraineur {
        #string nom
        #vector~Pokemon*~ pokemons
        +Entraineur(nom)
        +~Entraineur()
        +estMaitre() bool
        +getNom() string
        +getPokemons() const vector~Pokemon*~&
        +ajouterPokemon(pokemon) bool
        +getPokemonActif() Pokemon*
        +soignerEquipe()
        +aPerdu() bool
        +interagir() string*
    }

    class Joueur {
        -int badges
        -int victoires
        -int defaites
        -set~string~ badgesGagnes
        -bool vaincu
        +Joueur(nom)
        +~Joueur()
        +getBadges() int
        +getVictoires() int
        +getDefaites() int
        +getBadgesGagnes() const set~string~&
        +aEteVaincu() bool
        +gagnerBadge(nb)
        +gagnerCombat(nb)
        +perdreCombat(nb)
        +ajouterBadge(badge)
        +setBadgesGagnes(badges)
        +setVaincu(etat)
        +possedeBadge(badge) bool
        +getBadgesGagnesString() string
        +donnerBadge(challenger)
        +echangerPokemon(pos1, pos2)
        +interagir() string
    }

    class Leader {
        -string badge
        -string gymnase
        -Type specialite
        -bool vaincu
        +Leader(nom, pokemons, badge, gymnase, specialite)
        +getBadge() string
        +getGymnase() string
        +getSpecialite() Type
        +aEteVaincu() bool
        +setVaincu(etat)
        +interagir() string
        +messageDefi() string
        +donnerBadge(challenger)
    }

    class Maitre {
        -int nbVictoires
        -string titre
        -bool vaincu
        +Maitre(nom, pokemons, titre, nbVictoires, vaincu)
        +estMaitre() bool
        +getNbVictoires() int
        +getTitre() string
        +aEteVaincu() bool
        +setVaincu(etat)
        +interagir() string
        +incrementerVictoires()
    }

    class Combat {
        +BONUS_DEGATS_MAITRE : double
        +calculerDegats(attaquant, cible, estMaitre) ResultatAttaque
        +afficherAttaque(nomAttaquant, nomAttaque, degatsBase, estMaitre)
        +afficherResultat(resultat, cible)
        +afficherEfficacite(multiplicateur)
        +afficherEtatPokemon(pokemon)
    }

    class Combat::ResultatAttaque {
        +int degatsBase
        +int degatsFinaux
        +double multiplicateur
        +double multiplicateurType
        +bool estCritique
        +bool estImmunise
        +bool estMaitre
    }

    class Combat::EquipeCombat {
        -vector~Pokemon*~ pokemons
        -size_t indexActif
        -string nomDresseur
        +EquipeCombat(nom, equipe)
        +getPokemonActif() Pokemon*
        +pokemonKO()
        +getPokemons() const vector~Pokemon*~&
        +getNom() string
        +aPerdu() bool
    }

    class DataLoader {
        +chargerPokemons(fichier) vector~Pokemon*~
        +chargerJoueur(fichier, nomJoueur) Joueur*
        +chargerTousJoueurs(fichier) vector~Joueur*~
        +chargerLeaders(fichier) vector~Leader*~
        +chargerMaitres(fichier) vector~Maitre*~
        +chargerStatistiques(fichier, joueur)
        +sauvegarderStatistiques(fichier, joueur)
        +nettoyerCache()
    }

    Interagir <|-- Pokemon
    Interagir <|-- Entraineur
    Entraineur <|-- Joueur
    Entraineur <|-- Leader
    Entraineur <|-- Maitre
    Combat +-- "ResultatAttaque" Combat::ResultatAttaque
    Combat +-- "EquipeCombat" Combat::EquipeCombat
    Combat::EquipeCombat o-- Pokemon
    DataLoader ..> Pokemon
    DataLoader ..> Joueur
    DataLoader ..> Leader
    DataLoader ..> Maitre
``` 