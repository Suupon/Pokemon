#ifndef LEADER_HPP
#define LEADER_HPP

#include "Entraineur.hpp"
#include <string>
#include <vector>

class Leader : public Entraineur {
private:
    std::string badge;
    std::string gymnase;
    Type specialite;
    bool vaincu;

public:
    Leader(const std::string& nom, const std::vector<Pokemon*>& pokemons, 
           const std::string& badge, const std::string& gymnase, Type specialite);
    ~Leader() override = default;

    // Getters spécifiques
    std::string getBadge() const { return badge; }
    std::string getGymnase() const { return gymnase; }
    Type getSpecialite() const { return specialite; }
    bool aEteVaincu() const { return vaincu; }
    void setVaincu(bool etat) { vaincu = etat; }

    // Implémentation de la méthode interagir de l'interface
    std::string interagir() const override;
    
    // Message de défi (pour le menu 4)
    std::string messageDefi() const;

    // Méthode spécifique au Leader
    void donnerBadge(Entraineur& challenger);
};

#endif // LEADER_HPP 