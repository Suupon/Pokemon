#include "Leader.hpp"

Leader::Leader(const std::string& nom, const std::vector<Pokemon*>& pokemons,
               const std::string& badge, const std::string& gymnase, Type specialite)
    : Entraineur(nom), badge(badge), gymnase(gymnase), specialite(specialite), vaincu(false) {
    for (const auto& pokemon : pokemons) {
        ajouterPokemon(pokemon);
    }
}

std::string Leader::interagir() const {
    return getNom() + " : \"Tu m'as prouvé ta valeur en remportant le " + badge + ". Continue à t'entraîner !\"";
}

std::string Leader::messageDefi() const {
    return "Je suis " + getNom() + ", le Champion de l'" + gymnase + " ! Remporte le " + badge + " !";
}

void Leader::donnerBadge(Entraineur& challenger) {
    vaincu = true;
    std::string message = "Félicitations ! Tu as gagné le " + badge + " !";
    
} 