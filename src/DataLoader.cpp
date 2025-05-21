#include "DataLoader.hpp"
#include "Pokemon.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <map>
using namespace std;

// Initialisation du cache statique
map<string, Pokemon*> DataLoader::pokemonCache;

// Fonction utilitaire pour normaliser les types
string normaliserType(const string& type) {
    static const map<string, string> conversions = {
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

Type DataLoader::stringToType(const string& typeStr) {
    if (typeStr.empty()) return Type::NORMAL;
    
    // Normaliser le type avant la conversion
    string typeNormalise = normaliserType(typeStr);
    
    try {
        return Pokemon::stringToType(typeNormalise);
    } catch (const invalid_argument& e) {
        throw invalid_argument("Type de pokemon invalide: " + typeStr);
    }
}

vector<Pokemon*> DataLoader::chargerPokemons(const string& fichier) {
    vector<Pokemon*> pokemons;
    ifstream file(fichier);
    
    if (!file.is_open()) {
        throw runtime_error("Impossible d'ouvrir le fichier: " + fichier);
    }

    string ligne;
    // Ignorer la première ligne (en-têtes)
    getline(file, ligne);
    
    while (getline(file, ligne)) {
        if (!ligne.empty()) {
            auto donnees = splitLine(ligne, ',');
            try {
                if (donnees.size() >= 6) {
                    string nom = donnees[0];
                    vector<Type> types;
                    types.push_back(stringToType(donnees[1]));
                    if (!donnees[2].empty()) {
                        types.push_back(stringToType(donnees[2]));
                    }
                    int hp = stoi(donnees[3]);
                    string nomAttaque = donnees[4];
                    int degatsAttaque = stoi(donnees[5]);

                    Pokemon* pokemon = new Pokemon(nom, types, hp, nomAttaque, degatsAttaque);
                    if (pokemon) {
                        pokemons.push_back(pokemon);
                        addPokemonToCache(pokemon->getNom(), pokemon);
                    }
                }
            } catch (const exception& e) {
                // Log l'erreur mais continue le chargement
                cerr << "Erreur lors du chargement d'un pokemon: " << e.what() << endl;
            }
        }
    }

    return pokemons;
}

Joueur* DataLoader::chargerJoueur(const string& fichier, const string& nomJoueur) {
    ifstream file(fichier);
    if (!file.is_open()) {
        throw runtime_error("Impossible d'ouvrir le fichier: " + fichier);
    }

    string ligne;
    // Ignorer l'en-tête
    getline(file, ligne);
    
    while (getline(file, ligne)) {
        auto donnees = splitLine(ligne, ',');
        if (donnees.size() >= 2) { // Au moins le nom et un Pokémon
            // Si un nom de joueur spécifique est demandé, vérifier que c'est le bon
            if (!nomJoueur.empty() && donnees[0] != nomJoueur) {
                continue; // Ce n'est pas le joueur recherché, continuer la recherche
            }
            
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
    throw runtime_error("Format de fichier joueur invalide ou joueur non trouvé");
}

vector<Joueur*> DataLoader::chargerTousJoueurs(const string& fichier) {
    vector<Joueur*> joueurs;
    ifstream file(fichier);
    if (!file.is_open()) {
        throw runtime_error("Impossible d'ouvrir le fichier: " + fichier);
    }

    string ligne;
    // Ignorer l'en-tête
    getline(file, ligne);
    
    while (getline(file, ligne)) {
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
            joueurs.push_back(joueur);
        }
    }
    
    if (joueurs.empty()) {
        throw runtime_error("Format de fichier joueur invalide ou fichier vide");
    }
    
    return joueurs;
}

vector<Leader*> DataLoader::chargerLeaders(const string& fichier) {
    vector<Leader*> leaders;
    ifstream file(fichier);
    if (!file.is_open()) {
        throw runtime_error("Impossible d'ouvrir le fichier: " + fichier);
    }
    
    string ligne;
    // Ignorer l'en-tête
    getline(file, ligne);
    
    while (getline(file, ligne)) {
        if (!ligne.empty()) {
            // Supprimer les caractères de fin de ligne indésirables
            ligne.erase(remove(ligne.begin(), ligne.end(), '\r'), ligne.end());
            ligne.erase(remove(ligne.begin(), ligne.end(), '%'), ligne.end());
            
            auto donnees = splitLine(ligne, ',');
            
            try {
                if (donnees.size() >= 4) {
                    vector<Pokemon*> pokemons;
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
            } catch (const exception& e) {
                cerr << "Erreur lors du chargement d'un leader : " << e.what() << endl;
            }
        }
    }
    
    return leaders;
}

vector<Maitre*> DataLoader::chargerMaitres(const string& fichier) {
    vector<Maitre*> maitres;
    ifstream file(fichier);
    if (!file.is_open()) {
        throw runtime_error("Impossible d'ouvrir le fichier: " + fichier);
    }

    string ligne;
    // Ignorer l'en-tête
    getline(file, ligne);
    
    while (getline(file, ligne)) {
        auto donnees = splitLine(ligne, ',');
        if (donnees.size() >= 2) { // Au moins le nom et un Pokémon
            vector<Pokemon*> pokemons;
            for (size_t i = 1; i < donnees.size() && !donnees[i].empty(); ++i) {
                Pokemon* pokemon = getPokemonFromCache(donnees[i]);
                if (pokemon) {
                    pokemons.push_back(pokemon);
                }
            }
            maitres.push_back(new Maitre(donnees[0], pokemons, "Maître Pokémon", 0, false));
        }
    }
    
    if (maitres.empty()) {
        throw runtime_error("Format de fichier maître invalide ou fichier vide");
    }
    
    return maitres;
}

vector<string> DataLoader::splitLine(const string& ligne, char delimiteur) {
    vector<string> tokens;
    stringstream ss(ligne);
    string token;
    
    while (getline(ss, token, delimiteur)) {
        // Supprimer les espaces au début et à la fin
        token.erase(0, token.find_first_not_of(" \t"));
        token.erase(token.find_last_not_of(" \t") + 1);
        tokens.push_back(token);
    }
    
    return tokens;
}

Pokemon* DataLoader::getPokemonFromCache(const string& nom) {
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

void DataLoader::addPokemonToCache(const string& nom, Pokemon* pokemon) {
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

void DataLoader::chargerStatistiques(const string& fichier, Joueur* joueur) {
    if (!joueur) return;
    
    try {
        ifstream file(fichier);
        if (!file.is_open()) {
            cerr << "Attention: Impossible d'ouvrir le fichier de statistiques: " << fichier << endl;
            return;
        }
        
        string ligne;
        // Ignorer l'en-tête
        getline(file, ligne);
        
        while (getline(file, ligne)) {
            auto donnees = splitLine(ligne, ',');
            if (donnees.size() >= 4 && donnees[0] == joueur->getNom()) {
                // Charger les statistiques
                // Badges
                int badges = stoi(donnees[1]);
                joueur->gagnerBadge(badges);
                
                // Victoires
                int victoires = stoi(donnees[2]);
                joueur->gagnerCombat(victoires);
                
                // Défaites
                int defaites = stoi(donnees[3]);
                joueur->perdreCombat(defaites);
                
                // Badges gagnés
                if (donnees.size() >= 5 && !donnees[4].empty()) {
                    auto badgesStr = splitLine(donnees[4], ';');
                    for (const auto& badge : badgesStr) {
                        joueur->ajouterBadge(badge);
                    }
                }
                
                break;
            }
        }
    } catch (const exception& e) {
        cerr << "Erreur lors du chargement des statistiques: " << e.what() << endl;
    }
}

void DataLoader::sauvegarderStatistiques(const string& fichier, const Joueur* joueur) {
    if (!joueur) return;
    
    // Lire toutes les lignes du fichier
    vector<string> lignes;
    bool joueurTrouve = false;
    
    // Essayer d'ouvrir le fichier existant
    ifstream fileIn(fichier);
    if (fileIn.is_open()) {
        string ligne;
        // Lire la première ligne (en-tête)
        getline(fileIn, ligne);
        lignes.push_back(ligne); // Ajouter l'en-tête
        
        // Lire le reste du fichier
        while (getline(fileIn, ligne)) {
            auto donnees = splitLine(ligne, ',');
            if (!donnees.empty() && donnees[0] == joueur->getNom()) {
                // Mettre à jour les statistiques de ce joueur
                joueurTrouve = true;
                ligne = joueur->getNom() + "," + 
                       to_string(joueur->getBadges()) + "," + 
                       to_string(joueur->getVictoires()) + "," + 
                       to_string(joueur->getDefaites()) + "," +
                       joueur->getBadgesGagnesString();
            }
            lignes.push_back(ligne);
        }
        fileIn.close();
    } else {
        // Si le fichier n'existe pas, créer un en-tête
        lignes.push_back("Nom,Badges,Victoires,Defaites,BadgesGagnes");
    }
    
    // Si le joueur n'a pas été trouvé, ajouter une nouvelle ligne
    if (!joueurTrouve) {
        string nouvelleLigne = joueur->getNom() + "," + 
                              to_string(joueur->getBadges()) + "," + 
                              to_string(joueur->getVictoires()) + "," + 
                              to_string(joueur->getDefaites()) + "," +
                              joueur->getBadgesGagnesString();
        lignes.push_back(nouvelleLigne);
    }
    
    // Écrire toutes les lignes dans le fichier
    ofstream fileOut(fichier);
    if (fileOut.is_open()) {
        for (const auto& ligne : lignes) {
            fileOut << ligne << endl;
        }
        fileOut.close();
    }
} 