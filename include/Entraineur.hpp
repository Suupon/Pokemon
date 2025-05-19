#ifndef ENTRAINEUR_HPP
#define ENTRAINEUR_HPP

#include "Pokemon.hpp"
#include "Interagir.hpp"
#include <string>
#include <vector>

class Entraineur : public Interagir {
protected:
    std::string nom;
    std::vector<Pokemon*> pokemons;
    static const size_t MAX_POKEMONS = 6;

public:
    Entraineur(const std::string& nom);
    virtual ~Entraineur();
    virtual bool estMaitre() const { return false; }


    // Getters
    std::string getNom() const { return nom; }
    const std::vector<Pokemon*>& getPokemons() const { return pokemons; }

    // Gestion des Pokémon
    bool ajouterPokemon(Pokemon* pokemon);
    Pokemon* getPokemonActif() const;
    void soignerEquipe();
    bool aPerdu() const;

    // Méthode virtuelle pure pour l'interaction
    virtual std::string interagir() const = 0;
};

#endif 