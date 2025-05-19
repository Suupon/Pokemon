#include "DataLoader.hpp"
#include "Pokemon.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <map>

// Initialisation du cache statique
std::map<std::string, Pokemon*> DataLoader::pokemonCache;

// Fonction utilitaire pour normaliser les types
std::string normaliserType(const std::string& type) {
    static const std::map<std::string, std::string> conversions = {
        {"Électrik", "Electrik"},
        {"ÉLECTRIK", "Electrik"},
        {"électrik", "Electrik"},
        {"Fée", "Fee"},
        {"FÉE", "Fee"},
        {"fée", "Fee"}
    };

    // Vérifier si une conversion existe
    auto it = conversions.find(type);
    if (it != conversions.end()) {
        return it->second;
    }

    return type;
}

Type DataLoader::stringToType(const std::string& typeStr) {
    if (typeStr.empty()) return Type::NORMAL;
    
    // Normaliser le type avant la conversion
    std::string typeNormalise = normaliserType(typeStr);
    
    try {
        return Pokemon::stringToType(typeNormalise);
    } catch (const std::invalid_argument& e) {
        throw std::invalid_argument("Type de pokemon invalide: " + typeStr);
    }
}

std::vector<Pokemon*> DataLoader::chargerPokemons(const std::string& fichier) {
    std::vector<Pokemon*> pokemons;
    std::ifstream file(fichier);
    
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier: " + fichier);
    }

    std::string ligne;
    // Ignorer la première ligne (en-têtes)
    std::getline(file, ligne);
    
    while (std::getline(file, ligne)) {
        if (!ligne.empty()) {
            auto donnees = splitLine(ligne, ',');
            try {
                if (donnees.size() >= 6) {
                    std::string nom = donnees[0];
                    std::vector<Type> types;
                    types.push_back(stringToType(donnees[1]));
                    if (!donnees[2].empty()) {
                        types.push_back(stringToType(donnees[2]));
                    }
                    int hp = std::stoi(donnees[3]);
                    std::string nomAttaque = donnees[4];
                    int degatsAttaque = std::stoi(donnees[5]);

                    Pokemon* pokemon = new Pokemon(nom, types, hp, nomAttaque, degatsAttaque);
                    if (pokemon) {
                        pokemons.push_back(pokemon);
                        addPokemonToCache(pokemon->getNom(), pokemon);
                    }
                }
            } catch (const std::exception& e) {
                // Log l'erreur mais continue le chargement
                std::cerr << "Erreur lors du chargement d'un pokemon: " << e.what() << std::endl;
            }
        }
    }

    return pokemons;
}

Joueur* DataLoader::chargerJoueur(const std::string& fichier) {
    std::ifstream file(fichier);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier: " + fichier);
    }

    std::string ligne;
    // Ignorer l'en-tête
    std::getline(file, ligne);
    
    if (std::getline(file, ligne)) {
        auto donnees = splitLine(ligne, ',');
        if (donnees.size() >= 2) { // Au moins le nom et un Pokémon
            Joueur* joueur = new Joueur(donnees[0]);
            
            // Ajouter les Pokémon
            for (size_t i = 1; i < donnees.size() && !donnees[i].empty(); ++i) {
                Pokemon* pokemon = getPokemonFromCache(donnees[i]);
                if (pokemon) {
                    joueur->ajouterPokemon(pokemon);
                }
            }
            return joueur;
        }
    }
    throw std::runtime_error("Format de fichier joueur invalide");
}

std::vector<Leader*> DataLoader::chargerLeaders(const std::string& fichier) {
    std::vector<Leader*> leaders;
    std::ifstream file(fichier);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier: " + fichier);
    }
    
    std::string ligne;
    // Ignorer l'en-tête
    std::getline(file, ligne);
    
    while (std::getline(file, ligne)) {
        if (!ligne.empty()) {
            // Supprimer les caractères de fin de ligne indésirables
            ligne.erase(std::remove(ligne.begin(), ligne.end(), '\r'), ligne.end());
            ligne.erase(std::remove(ligne.begin(), ligne.end(), '%'), ligne.end());
            
            auto donnees = splitLine(ligne, ',');
            
            try {
                if (donnees.size() >= 4) {
                    std::vector<Pokemon*> pokemons;
                    for (size_t i = 3; i < donnees.size() && !donnees[i].empty(); ++i) {
                        if (donnees[i] != "null") {
                            Pokemon* pokemon = getPokemonFromCache(donnees[i]);
                            if (pokemon) {
                                pokemons.push_back(pokemon);
                            }
                        }
                    }
                    
                    // Déterminer le type du leader basé sur son premier Pokémon
                    Type specialite = Type::FEU; // Par défaut
                    if (!pokemons.empty()) {
                        specialite = pokemons[0]->getTypes()[0];
                    }
                    
                    Leader* leader = new Leader(donnees[0], pokemons, donnees[2], donnees[1], specialite);
                    leaders.push_back(leader);
                }
            } catch (const std::exception& e) {
                std::cerr << "Erreur lors du chargement d'un leader : " << e.what() << std::endl;
            }
        }
    }
    
    return leaders;
}

std::vector<Maitre*> DataLoader::chargerMaitres(const std::string& fichier) {
    std::vector<Maitre*> maitres;
    std::ifstream file(fichier);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier: " + fichier);
    }

    std::string ligne;
    // Ignorer l'en-tête
    std::getline(file, ligne);
    
    while (std::getline(file, ligne)) {
        auto donnees = splitLine(ligne, ',');
        if (donnees.size() >= 2) { // Au moins le nom et un Pokémon
            std::vector<Pokemon*> pokemons;
            for (size_t i = 1; i < donnees.size() && !donnees[i].empty(); ++i) {
                Pokemon* pokemon = getPokemonFromCache(donnees[i]);
                if (pokemon) {
                    pokemons.push_back(pokemon);
                }
            }
            maitres.push_back(new Maitre(donnees[0], pokemons, "Maître Pokémon"));
        }
    }
    
    if (maitres.empty()) {
        throw std::runtime_error("Format de fichier maître invalide ou fichier vide");
    }
    
    return maitres;
}

std::vector<std::string> DataLoader::splitLine(const std::string& ligne, char delimiteur) {
    std::vector<std::string> tokens;
    std::stringstream ss(ligne);
    std::string token;
    
    while (std::getline(ss, token, delimiteur)) {
        // Supprimer les espaces au début et à la fin
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        tokens.push_back(token);
    }
    
    return tokens;
}

Pokemon* DataLoader::getPokemonFromCache(const std::string& nom) {
    auto it = pokemonCache.find(nom);
    if (it != pokemonCache.end()) {
        // Créer une copie du Pokémon pour éviter les problèmes de double libération
        Pokemon* original = it->second;
        return new Pokemon(original->getNom(), original->getTypes(), 
                         original->getMaxHp(), original->getNomAttaque(), 
                         original->getDegatsAttaque());
    }
    return nullptr;
}

void DataLoader::addPokemonToCache(const std::string& nom, Pokemon* pokemon) {
    if (!pokemonCache.count(nom)) {
        pokemonCache[nom] = pokemon;
    }
}

void DataLoader::nettoyerCache() {
    // Libérer la mémoire des Pokémon dans le cache
    for (auto& [nom, pokemon] : pokemonCache) {
        delete pokemon;
    }
    pokemonCache.clear();
} 