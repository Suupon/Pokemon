#include "Combat.hpp"
#include <iostream>
using namespace std;

// Implementation of EquipeCombat class
Combat::EquipeCombat::EquipeCombat(const string& nom, const vector<Pokemon*>& equipe) 
    : pokemons(equipe), indexActif(0), nomDresseur(nom) {}

Pokemon* Combat::EquipeCombat::getPokemonActif() const {
    if (indexActif < pokemons.size()) {
        return pokemons[indexActif];
    }
    return nullptr;
}

void Combat::EquipeCombat::pokemonKO() {
    indexActif++;
}

const vector<Pokemon*>& Combat::EquipeCombat::getPokemons() const {
    return pokemons;
}

string Combat::EquipeCombat::getNom() const {
    return nomDresseur;
}

bool Combat::EquipeCombat::aPerdu() const {
    return indexActif >= pokemons.size();
}

Combat::ResultatAttaque Combat::calculerDegats(const Pokemon& attaquant, Pokemon& cible, bool estMaitre) {
    ResultatAttaque resultat;
    resultat.degatsBase = attaquant.getDegatsAttaque();
    
    // Calcul du multiplicateur de type
    Type typeAttaque = attaquant.getTypes()[0];
    resultat.multiplicateur = cible.getEfficaciteType(typeAttaque);
    
    
    if (resultat.multiplicateur == 0.0) {
        resultat.estImmunise = true;
        resultat.degatsFinaux = 0;
        return resultat;
    }
    
    resultat.estImmunise = false;
    
    
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

void Combat::afficherAttaque(const string& nomAttaquant, const string& nomAttaque, int degatsBase, bool estMaitre) {
    cout << "\n" << nomAttaquant << " utilise " << nomAttaque << " ! (Dégâts de base : " << degatsBase << ")";
    if (estMaitre) {
        cout << " [BONUS MAÎTRE +" << static_cast<int>((BONUS_DEGATS_MAITRE - 1.0) * 100) << "%]";
    }
    cout << endl;
}

void Combat::afficherResultat(const ResultatAttaque& resultat, const Pokemon& cible) {
    if (resultat.estImmunise) {
        cout << "Ça n'affecte pas " << cible.getNom() << "..." << endl;
        return;
    }
    
    // Afficher le message de coup critique si applicable
    if (resultat.estCritique) {
        cout << "Coup critique !" << endl;
    }
    
   
    if (resultat.multiplicateurType > 1.0 || resultat.multiplicateurType < 1.0) {
        afficherEfficacite(resultat.multiplicateurType);
    }
    
    cout << "\n→ L'attaque inflige " << resultat.degatsFinaux << " dégâts" << endl;
    cout << "→ " << cible.getNom() << " a maintenant " 
              << cible.getCurrentHp() << "/" << cible.getMaxHp() << " PV\n" << endl;
}

void Combat::afficherEfficacite(double multiplicateur) {
    if (multiplicateur > 1.0) {
        cout << "C'est super efficace !" << endl;
    } else if (multiplicateur < 1.0 && multiplicateur > 0.0) {
        cout << "Ce n'est pas très efficace..." << endl;
    }
}

void Combat::afficherEtatPokemon(const Pokemon& pokemon) {
    cout << pokemon.getNom() << " (PV: " << pokemon.getCurrentHp() 
              << "/" << pokemon.getMaxHp() << ")" << endl;
} 