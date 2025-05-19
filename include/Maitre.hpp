#ifndef MAITRE_HPP
#define MAITRE_HPP

#include "Entraineur.hpp"
#include <string>
#include <vector>

class Maitre : public Entraineur {
private:
    int nbVictoires;
    std::string titre;

public:
    Maitre(const std::string& nom, const std::vector<Pokemon*>& pokemons,
           const std::string& titre, int nbVictoires = 0);
    ~Maitre() override = default;
    bool estMaitre() const override { return true; }


    // Getters spécifiques
    int getNbVictoires() const { return nbVictoires; }
    std::string getTitre() const { return titre; }

    // Implémentation de la méthode interagir de l'interface
    std::string interagir() const override;

    // Méthodes spécifiques au Maître
    void incrementerVictoires();
};

#endif // MAITRE_HPP 