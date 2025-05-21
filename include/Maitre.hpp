#ifndef MAITRE_HPP
#define MAITRE_HPP

#include "Entraineur.hpp"
#include <string>
#include <vector>
using namespace std;

class Maitre : public Entraineur {
private:
    int nbVictoires;
    string titre;
    bool vaincu;  // Indique si le Maître a été vaincu par le joueur

public:
    Maitre(const string& nom, const vector<Pokemon*>& pokemons,
           const string& titre, int nbVictoires = 0, bool vaincu = false);
    ~Maitre() override = default;
    bool estMaitre() const override { return true; }

    // Getters spécifiques
    int getNbVictoires() const { return nbVictoires; }
    string getTitre() const { return titre; }
    bool aEteVaincu() const { return vaincu; }
    
    // Setters
    void setVaincu(bool etat) { vaincu = etat; }

    // Implémentation de la méthode interagir de l'interface
    string interagir() const override;

    // Méthodes spécifiques au Maître
    void incrementerVictoires();
};

#endif // MAITRE_HPP 