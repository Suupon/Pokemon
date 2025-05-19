#ifndef COMBAT_HPP
#define COMBAT_HPP

#include "Pokemon.hpp"
#include <string>

class Combat {
public:
    // Constante pour le bonus de dégâts du Maître
    static constexpr double BONUS_DEGATS_MAITRE = 1.25; // +25% de dégâts

    // Structure pour stocker le résultat d'une attaque
    struct ResultatAttaque {
        int degatsBase;
        int degatsFinaux;
        double multiplicateur;
        bool estCritique;
        bool estImmunise;
    };

    // Méthodes pour gérer le combat
    static ResultatAttaque calculerDegats(const Pokemon& attaquant, Pokemon& cible, bool estMaitre = false);
    static void afficherAttaque(const std::string& nomAttaquant, const std::string& nomAttaque, int degatsBase, bool estMaitre = false);
    static void afficherResultat(const ResultatAttaque& resultat, const Pokemon& cible);
    static void afficherEfficacite(double multiplicateur);
    static void afficherEtatPokemon(const Pokemon& pokemon);
};

#endif 