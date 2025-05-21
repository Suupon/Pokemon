#ifndef DATALOADER_HPP
#define DATALOADER_HPP

#include "Pokemon.hpp"
#include "Joueur.hpp"
#include "Leader.hpp"
#include "Maitre.hpp"
#include <string>
#include <vector>
#include <map>
using namespace std;

class DataLoader {
private:
    static map<string, Pokemon*> pokemonCache;
    
    static vector<string> splitLine(const string& ligne, char delimiteur = ',');
    static Type stringToType(const string& typeStr);
    static Pokemon* creerPokemon(const string& nom, const vector<Type>& types, 
                               int hp, const string& nomAttaque, int degatsAttaque);
    static Pokemon* getPokemonFromCache(const string& nom);
    static void addPokemonToCache(const string& nom, Pokemon* pokemon);

public:
    // Chargement des données
    static vector<Pokemon*> chargerPokemons(const string& fichier);
    static Joueur* chargerJoueur(const string& fichier, const string& nomJoueur = "");
    static vector<Joueur*> chargerTousJoueurs(const string& fichier);
    static vector<Leader*> chargerLeaders(const string& fichier);
    static vector<Maitre*> chargerMaitres(const string& fichier);
    
    // Gestion des statistiques
    static void chargerStatistiques(const string& fichier, Joueur* joueur);
    static void sauvegarderStatistiques(const string& fichier, const Joueur* joueur);
    
    // Nettoyage du cache
    static void nettoyerCache();
};

#endif // DATALOADER_HPP 