#ifndef COMBAT_HPP
#define COMBAT_HPP

#include "Pokemon.hpp"
#include <string>
using namespace std;

class Combat {
public:
    // Constante pour le bonus de dégâts du Maître
    static constexpr double BONUS_DEGATS_MAITRE = 1.25; // +25% de dégâts

    // Structure pour stocker le résultat d'une attaque
    struct ResultatAttaque {
        int degatsBase;
        int degatsFinaux;
        double multiplicateur;      // Multiplicateur final (incluant type, critique, bonus maître)
        double multiplicateurType;  // Multiplicateur de type uniquement
        bool estCritique;
        bool estImmunise;
        bool estMaitre;            // Indique si le bonus maître a été appliqué
    };

    // Méthodes pour gérer le combat
    static ResultatAttaque calculerDegats(const Pokemon& attaquant, Pokemon& cible, bool estMaitre = false);
    static void afficherAttaque(const string& nomAttaquant, const string& nomAttaque, int degatsBase, bool estMaitre = false);
    static void afficherResultat(const ResultatAttaque& resultat, const Pokemon& cible);
    static void afficherEfficacite(double multiplicateur);
    static void afficherEtatPokemon(const Pokemon& pokemon);
};

#endif 