#include "Combat.hpp"
#include <iostream>

Combat::ResultatAttaque Combat::calculerDegats(const Pokemon& attaquant, Pokemon& cible, bool estMaitre) {
    ResultatAttaque resultat;
    resultat.degatsBase = attaquant.getDegatsAttaque();
    
    // Calcul du multiplicateur de type
    Type typeAttaque = attaquant.getTypes()[0];
    resultat.multiplicateur = cible.getEfficaciteType(typeAttaque);
    
    // Vérification de l'immunité
    if (resultat.multiplicateur == 0.0) {
        resultat.estImmunise = true;
        resultat.degatsFinaux = 0;
        return resultat;
    }
    
    resultat.estImmunise = false;
    
    // Vérification du coup critique
    resultat.estCritique = attaquant.estCoupCritique(resultat.multiplicateur);
    if (resultat.estCritique) {
        resultat.multiplicateur *= Pokemon::MULTIPLICATEUR_CRITIQUE;
    }
    
    // Appliquer le bonus de dégâts pour les Maîtres
    if (estMaitre) {
        // Stocker le multiplicateur original avant d'appliquer le bonus du Maître
        resultat.multiplicateurType = resultat.multiplicateur;
        resultat.multiplicateur *= BONUS_DEGATS_MAITRE;
        resultat.estMaitre = true;
    } else {
        resultat.multiplicateurType = resultat.multiplicateur;
        resultat.estMaitre = false;
    }
    
    // Calcul des dégâts finaux
    resultat.degatsFinaux = static_cast<int>(resultat.degatsBase * resultat.multiplicateur);
    
    // Application des dégâts
    cible.subirDegats(resultat.degatsFinaux);
    
    return resultat;
}

void Combat::afficherAttaque(const std::string& nomAttaquant, const std::string& nomAttaque, int degatsBase, bool estMaitre) {
    std::cout << "\n" << nomAttaquant << " utilise " << nomAttaque << " ! (Dégâts de base : " << degatsBase << ")";
    if (estMaitre) {
        std::cout << " [BONUS MAÎTRE +" << static_cast<int>((BONUS_DEGATS_MAITRE - 1.0) * 100) << "%]";
    }
    std::cout << std::endl;
}

void Combat::afficherResultat(const ResultatAttaque& resultat, const Pokemon& cible) {
    if (resultat.estImmunise) {
        std::cout << "Ça n'affecte pas " << cible.getNom() << "..." << std::endl;
        return;
    }
    
    if (resultat.estCritique) {
        std::cout << "Coup critique !" << std::endl;
    } else {
        // Afficher le message d'efficacité uniquement s'il n'y a pas de bonus Maître ou si l'efficacité n'est pas neutre
        if (!resultat.estMaitre || (resultat.multiplicateurType != 1.0)) {
            afficherEfficacite(resultat.multiplicateurType);
        }
    }
    
    std::cout << "\n→ L'attaque inflige " << resultat.degatsFinaux << " dégâts" << std::endl;
    std::cout << "→ " << cible.getNom() << " a maintenant " 
              << cible.getCurrentHp() << "/" << cible.getMaxHp() << " PV\n" << std::endl;
}

void Combat::afficherEfficacite(double multiplicateur) {
    if (multiplicateur > 1.0) {
        std::cout << "C'est super efficace !" << std::endl;
    } else if (multiplicateur < 1.0 && multiplicateur > 0.0) {
        std::cout << "Ce n'est pas très efficace..." << std::endl;
    }
}

void Combat::afficherEtatPokemon(const Pokemon& pokemon) {
    std::cout << pokemon.getNom() << " (PV: " << pokemon.getCurrentHp() 
              << "/" << pokemon.getMaxHp() << ")" << std::endl;
} 