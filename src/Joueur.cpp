#include "Joueur.hpp"

Joueur::Joueur(const std::string& nom)
    : Entraineur(nom), badges(0), victoires(0), defaites(0), vaincu(false) {}

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

void Joueur::donnerBadge(Entraineur& challenger) {
    vaincu = true;
    
    // Si le challenger est un joueur, lui donner un badge fictif
    Joueur* joueur = dynamic_cast<Joueur*>(&challenger);
    if (joueur) {
        joueur->ajouterBadge(getNom() + " Badge");
        joueur->gagnerBadge();
    }
}

std::string Joueur::getBadgesGagnesString() const {
    std::string result;
    bool first = true;
    for (const auto& badge : badgesGagnes) {
        if (!first) {
            result += ",";
        }
        result += badge;
        first = false;
    }
    return result;
} 