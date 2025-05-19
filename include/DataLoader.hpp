#ifndef DATALOADER_HPP
#define DATALOADER_HPP

#include "Pokemon.hpp"
#include "Joueur.hpp"
#include "Leader.hpp"
#include "Maitre.hpp"
#include <string>
#include <vector>
#include <map>

class DataLoader {
private:
    static std::map<std::string, Pokemon*> pokemonCache;
    
    static std::vector<std::string> splitLine(const std::string& ligne, char delimiteur = ',');
    static Type stringToType(const std::string& typeStr);
    static Pokemon* creerPokemon(const std::string& nom, const std::vector<Type>& types, 
                               int hp, const std::string& nomAttaque, int degatsAttaque);
    static Pokemon* getPokemonFromCache(const std::string& nom);
    static void addPokemonToCache(const std::string& nom, Pokemon* pokemon);

public:
    // Chargement des données
    static std::vector<Pokemon*> chargerPokemons(const std::string& fichier);
    static Joueur* chargerJoueur(const std::string& fichier, const std::string& nomJoueur = "");
    static std::vector<Joueur*> chargerTousJoueurs(const std::string& fichier);
    static std::vector<Leader*> chargerLeaders(const std::string& fichier);
    static std::vector<Maitre*> chargerMaitres(const std::string& fichier);
    
    // Gestion des statistiques
    static void chargerStatistiques(const std::string& fichier, Joueur* joueur);
    static void sauvegarderStatistiques(const std::string& fichier, const Joueur* joueur);
    
    // Nettoyage du cache
    static void nettoyerCache();
};

#endif // DATALOADER_HPP 