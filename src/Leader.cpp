#include "Leader.hpp"
#include "Joueur.hpp"
using namespace std;

Leader::Leader(const string& nom, const vector<Pokemon*>& pokemons,
               const string& badge, const string& gymnase, Type specialite)
    : Entraineur(nom), badge(badge), gymnase(gymnase), specialite(specialite), vaincu(false) {
    for (const auto& pokemon : pokemons) {
        ajouterPokemon(pokemon);
    }
}

string Leader::interagir() const {
    return getNom() + " : \"Tu m'as prouvé ta valeur en remportant le " + badge + ". Continue à t'entraîner !\"";
}

string Leader::messageDefi() const {
    return "Je suis " + getNom() + ", le Champion de l'" + gymnase + " ! Remporte le " + badge + " !";
}

void Leader::donnerBadge(Entraineur& challenger) {
    vaincu = true;
    
    // Si le challenger est un Joueur, lui ajouter le badge spécifique
    Joueur* joueur = dynamic_cast<Joueur*>(&challenger);
    if (joueur) {
        joueur->ajouterBadge(badge);
        joueur->gagnerBadge();
    }
    
    string message = "Félicitations ! Tu as gagné le " + badge + " !";
} 