classDiagram
direction LR
    class Interagir {
	    +interagir() string
    }

    class Pokemon {
	    -nom string
	    -types vector~Type~
	    -maxHp int
	    -currentHp int
	    -nomAttaque string
	    -degatsAttaque int
	    +Pokemon(nom, types, hp, nomAttaque, degatsAttaque)
	    +getNom() string
	    +getTypes() vector~Type~
	    +getMaxHp() int
	    +getCurrentHp() int
	    +subirDegats(degats)
	    +estKO() bool
	    +soigner()
	    +interagir() string
    }

    class Entraineur {
	    #nom string
	    #pokemons vector~Pokemon*~
	    +Entraineur(nom)
	    +getNom() string
	    +getPokemons() vector~Pokemon*~
	    +ajouterPokemon(pokemon) bool
	    +getPokemonActif() Pokemon*
	    +soignerEquipe()
	    +aPerdu() bool
	    +estMaitre() bool
	    +interagir() string*
    }

    class Joueur {
	    -badges int
	    -victoires int
	    -defaites int
	    -badgesGagnes set~string~
	    -vaincu bool
	    +Joueur(nom)
	    +getBadges() int
	    +getVictoires() int
	    +getDefaites() int
	    +gagnerBadge(nb)
	    +gagnerCombat(nb)
	    +perdreCombat(nb)
	    +donnerBadge(challenger)
	    +interagir() string
    }

    class Leader {
	    -badge string
	    -gymnase string
	    -specialite Type
	    -vaincu bool
	    +Leader(nom, pokemons, badge, gymnase, specialite)
	    +getBadge() string
	    +getGymnase() string
	    +aEteVaincu() bool
	    +setVaincu(etat)
	    +messageDefi() string
	    +donnerBadge(challenger)
	    +interagir() string
    }

    class Maitre {
	    -nbVictoires int
	    -titre string
	    -vaincu bool
	    +Maitre(nom, pokemons, titre, nbVictoires, vaincu)
	    +getNbVictoires() int
	    +getTitre() string
	    +incrementerVictoires()
	    +interagir() string
    }

    class Combat {
	    +BONUS_DEGATS_MAITRE double
	    +calculerDegats(attaquant, cible, estMaitre)
	    +afficherAttaque(nomAttaquant, nomAttaque, degatsBase, estMaitre)
	    +afficherResultat(resultat, cible)
	    +afficherEtatPokemon(pokemon)
    }

    class DataLoader {
	    -pokemonCache map~string, Pokemon*~
	    +chargerPokemons(fichier) vector~Pokemon*~
	    +chargerJoueur(fichier, nomJoueur) Joueur*
	    +chargerLeaders(fichier) vector~Leader*~
	    +chargerMaitres(fichier) vector~Maitre*~
	    +chargerStatistiques(fichier, joueur)
	    +sauvegarderStatistiques(fichier, joueur)
	    +nettoyerCache()
    }

    class Type {
	    NORMAL
	    FEU
	    EAU
	    PLANTE
	    ELECTRIK
	    GLACE
	    COMBAT
	    POISON
	    SOL
	    VOL
	    PSY
	    INSECTE
	    ROCHE
	    SPECTRE
	    DRAGON
	    ACIER
	    TENEBRES
	    FEE
    }

    class UntitledClass {
    }

	<<interface>> Interagir
	<<utility>> Combat
	<<utility>> DataLoader
	<<enumeration>> Type

    Pokemon ..|> Interagir : implémente
    Entraineur ..|> Interagir : implémente
    Joueur --|> Entraineur : hérite
    Leader --|> Entraineur : hérite
    Maitre --|> Entraineur : hérite
    Entraineur o-- "0..*" Pokemon : possède >
    Pokemon ..> Type : utilise
    Leader ..> Type : a spécialité
    Combat ..> Pokemon : utilise
    DataLoader ..> Pokemon : crée
    DataLoader ..> Joueur : crée
    DataLoader ..> Leader : crée
    DataLoader ..> Maitre : crée

