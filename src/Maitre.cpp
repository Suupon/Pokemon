#include "Maitre.hpp"

Maitre::Maitre(const std::string& nom, const std::vector<Pokemon*>& pokemons,
               const std::string& titre, int nbVictoires, bool vaincu)
    : Entraineur(nom), titre(titre), nbVictoires(nbVictoires), vaincu(vaincu) {
    for (const auto& pokemon : pokemons) {
        ajouterPokemon(pokemon);
    }
}

std::string Maitre::interagir() const {
    if (vaincu) {
        return "Je suis " + getNom() + ", " + titre + "! Tu as prouvé ta valeur en me battant. Peu de personnes peuvent en dire autant. Continue de t'entraîner, car le chemin d'un Maître Pokémon n'a pas de fin.";
    } else {
        return "Je suis " + getNom() + ", " + titre + "! Avec mes " + std::to_string(nbVictoires) + " victoires, je représente un défi de taille. Es-tu prêt à m'affronter ?";
    }
}

void Maitre::incrementerVictoires() {
    nbVictoires++;
} 