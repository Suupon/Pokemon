#ifndef POKEMON_HPP
#define POKEMON_HPP

#include "Interagir.hpp"
#include <string>
#include <vector>
#include <random>
#include <map>
using namespace std;

enum class Type {
    NORMAL,
    FEU,
    EAU,
    PLANTE,
    ELECTRIK,
    GLACE,
    COMBAT,
    POISON,
    SOL,
    VOL,
    PSY,
    INSECTE,
    ROCHE,
    SPECTRE,
    DRAGON,
    ACIER,
    TENEBRES,
    FEE
};

class Pokemon : public Interagir {
public:
    // Constantes pour les coups critiques
    static constexpr double CHANCE_CRITIQUE_BASE = 0.1; // 10% de chance de base
    static constexpr double MULTIPLICATEUR_CRITIQUE = 2.0;

    friend class Combat;  // Permet à Combat d'accéder aux membres privés

    // Fonction de conversion des chaînes en types
    static Type stringToType(const string& typeStr);

    Pokemon(const string& nom, const vector<Type>& types, int hp, 
            const string& nomAttaque, int degatsAttaque);
    virtual ~Pokemon() = default;

    // Getters
    string getNom() const { return nom; }
    const vector<Type>& getTypes() const { return types; }
    int getMaxHp() const { return maxHp; }
    int getCurrentHp() const { return currentHp; }
    string getNomAttaque() const { return nomAttaque; }
    int getDegatsAttaque() const { return degatsAttaque; }

    // Méthodes de base
    void subirDegats(int degats) { currentHp = max(0, currentHp - degats); }
    bool estKO() const { return currentHp <= 0; }
    void soigner() { currentHp = maxHp; }

    // Calcul d'efficacité des types
    double getEfficaciteType(Type typeAttaque) const;
    
    // Méthodes pour les coups critiques
    bool estCoupCritique(double efficacite) const;
    double getChanceCritique(double efficacite) const;
    
    // Implémentation de l'interface Interagir
    string interagir() const override;

private:
    string nom;
    vector<Type> types;
    int maxHp;
    int currentHp;
    string nomAttaque;
    int degatsAttaque;
    
    // Générateur de nombres aléatoires
    static random_device rd;
    static mt19937 gen;
    static uniform_real_distribution<> dis;

    // Table des efficacités des types
    static map<Type, map<Type, double>> tableEfficacite;
    static void initTableEfficacite();
};

#endif 