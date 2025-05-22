#ifndef JOUEUR_HPP
#define JOUEUR_HPP

#include "Entraineur.hpp"
#include <set>
using namespace std;

class Joueur : public Entraineur {
private:
    int badges;
    int victoires;
    int defaites;
    set<string> badgesGagnes; 
    bool vaincu; 

public:
    Joueur(const string& nom);
    ~Joueur() override = default;

    // Getters
    int getBadges() const { return badges; }
    int getVictoires() const { return victoires; }
    int getDefaites() const { return defaites; }
    const set<string>& getBadgesGagnes() const { return badgesGagnes; }
    bool aEteVaincu() const { return vaincu; }

    // Setters
    void gagnerBadge(int nb = 1) { badges += nb; }
    void gagnerCombat(int nb = 1) { victoires += nb; }
    void perdreCombat(int nb = 1) { defaites += nb; }
    void ajouterBadge(const string& badge) { badgesGagnes.insert(badge); }
    void setBadgesGagnes(const set<string>& badges) { badgesGagnes = badges; }
    void setVaincu(bool etat) { vaincu = etat; }
    
    // Other methods
    bool possedeBadge(const string& badge) const { return badgesGagnes.find(badge) != badgesGagnes.end(); }
    string getBadgesGagnesString() const;
    void donnerBadge(Entraineur& challenger);
    void echangerPokemon(int pos1, int pos2);
    string interagir() const override;
};

#endif 