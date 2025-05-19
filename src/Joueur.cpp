#include "Joueur.hpp"

Joueur::Joueur(const std::string& nom)
    : Entraineur(nom), badges(0), victoires(0), defaites(0) {}

std::string Joueur::interagir() const {
    return "Je suis " + nom + ", et je deviendrai le meilleur dresseur !";
}

void Joueur::echangerPokemon(int pos1, int pos2) {
    if (pos1 >= 0 && pos1 < static_cast<int>(pokemons.size()) &&
        pos2 >= 0 && pos2 < static_cast<int>(pokemons.size()) &&
        pos1 != pos2) {
        std::swap(pokemons[pos1], pokemons[pos2]);
    }
} 