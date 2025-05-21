#include "Maitre.hpp"
using namespace std;

Maitre::Maitre(const string& nom, const vector<Pokemon*>& pokemons,
               const string& titre, int nbVictoires, bool vaincu)
    : Entraineur(nom), titre(titre), nbVictoires(nbVictoires), vaincu(vaincu) {
    for (const auto& pokemon : pokemons) {
        ajouterPokemon(pokemon);
    }
}

string Maitre::interagir() const {
    if (vaincu) {
        return "Je suis " + getNom() + ", " + titre + "! Tu as prouvé ta valeur en me battant. Peu de personnes peuvent en dire autant. Continue de t'entraîner, car le chemin d'un Maître Pokémon n'a pas de fin.";
    } else {
        return "Je suis " + getNom() + ", " + titre + "! Avec mes " + to_string(nbVictoires) + " victoires, je représente un défi de taille. Es-tu prêt à m'affronter ?";
    }
}

void Maitre::incrementerVictoires() {
    nbVictoires++;
} 