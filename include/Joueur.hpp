#ifndef JOUEUR_HPP
#define JOUEUR_HPP

#include "Entraineur.hpp"

class Joueur : public Entraineur {
private:
    int badges;
    int victoires;
    int defaites;

public:
    Joueur(const std::string& nom);
    ~Joueur() override = default;

    // Getters
    int getBadges() const { return badges; }
    int getVictoires() const { return victoires; }
    int getDefaites() const { return defaites; }

    // Setters
    void gagnerBadge() { badges++; }
    void gagnerCombat() { victoires++; }
    void perdreCombat() { defaites++; }

    // Gestion de l'ordre des Pokémon
    void echangerPokemon(int pos1, int pos2);

    // Implémentation de la méthode interagir
    std::string interagir() const override;
};

#endif 