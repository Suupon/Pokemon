#include "Entraineur.hpp"
using namespace std;

Entraineur::Entraineur(const string& nom) : nom(nom) {}

Entraineur::~Entraineur() {
    for (auto pokemon : pokemons) {
        delete pokemon;
    }
}

bool Entraineur::ajouterPokemon(Pokemon* pokemon) {
    if (pokemons.size() >= MAX_POKEMONS) return false;
    pokemons.push_back(pokemon);
    return true;
}

Pokemon* Entraineur::getPokemonActif() const {
    for (const auto& pokemon : pokemons) {
        if (!pokemon->estKO()) return pokemon;
    }
    return nullptr;
}

void Entraineur::soignerEquipe() {
    for (auto pokemon : pokemons) {
        pokemon->soigner();
    }
}

bool Entraineur::aPerdu() const {
    for (const auto& pokemon : pokemons) {
        if (!pokemon->estKO()) return false;
    }
    return true;
} 