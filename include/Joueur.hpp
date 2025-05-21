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
    set<string> badgesGagnes; // Pour stocker les noms des badges gagnés
    bool vaincu;  // Pour indiquer si le joueur a été vaincu (utilisé lors des combats contre d'autres joueurs)

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
    
    // Méthode pour vérifier si un badge spécifique a été gagné
    bool possedeBadge(const string& badge) const { return badgesGagnes.find(badge) != badgesGagnes.end(); }

    // Retourne les badges gagnés sous forme de chaîne CSV
    string getBadgesGagnesString() const;

    // Méthode pour donner un badge (utilisée lors des combats entre joueurs)
    void donnerBadge(Entraineur& challenger);

    // Gestion de l'ordre des Pokémon
    void echangerPokemon(int pos1, int pos2);

    // Implémentation de la méthode interagir
    string interagir() const override;
};

#endif 