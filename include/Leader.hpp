#ifndef LEADER_HPP
#define LEADER_HPP

#include "Entraineur.hpp"
#include <string>
#include <vector>
using namespace std;

class Leader : public Entraineur {
private:
    string badge;
    string gymnase;
    Type specialite;
    bool vaincu;

public:
    Leader(const string& nom, const vector<Pokemon*>& pokemons, 
           const string& badge, const string& gymnase, Type specialite);
    ~Leader() override = default;

    // Getters spécifiques
    string getBadge() const { return badge; }
    string getGymnase() const { return gymnase; }
    Type getSpecialite() const { return specialite; }
    bool aEteVaincu() const { return vaincu; }
    void setVaincu(bool etat) { vaincu = etat; }

    // Implémentation de la méthode interagir de l'interface
    string interagir() const override;
    
    // Message de défi (pour le menu 4)
    string messageDefi() const;

    // Méthode spécifique au Leader
    void donnerBadge(Entraineur& challenger);
};

#endif // LEADER_HPP 