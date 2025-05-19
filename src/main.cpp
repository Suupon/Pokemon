#include "Joueur.hpp"
#include "Leader.hpp"
#include "Maitre.hpp"
#include "DataLoader.hpp"
#include "Combat.hpp"
#include <iostream>
#include <limits>
#include <thread>
#include <chrono>
#include <sstream>
#include <set>

void afficherMenu() {
    std::cout << "\n=== Menu Principal ===" << std::endl;
    std::cout << "1. Afficher mes Pokémon" << std::endl;
    std::cout << "2. Soigner mes Pokémon" << std::endl;
    std::cout << "3. Afficher mes stats" << std::endl;
    std::cout << "4. Défier un Leader" << std::endl;
    std::cout << "5. Défier le Maître" << std::endl;
    std::cout << "6. Interagir avec les Pokémon/Entraîneurs" << std::endl;
    std::cout << "7. Gérer l'ordre des Pokémon" << std::endl;
    std::cout << "8. Quitter" << std::endl;
    std::cout << "Votre choix : ";
}

void viderBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int lireChoix() {
    int choix;
    while (!(std::cin >> choix)) {
        std::cout << "Entrée invalide. Veuillez entrer un nombre." << std::endl;
        viderBuffer();
        std::cout << "Votre choix : ";
    }
    viderBuffer();
    return choix;
}

void pause(int milliseconds = 1000) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

class EquipeCombat {
private:
    std::vector<Pokemon*> pokemons;
    size_t indexActif;
    std::string nomDresseur;

public:
    EquipeCombat(const std::string& nom, const std::vector<Pokemon*>& equipe) 
        : pokemons(equipe), indexActif(0), nomDresseur(nom) {}

    Pokemon* getPokemonActif() const {
        if (indexActif < pokemons.size()) {
            return pokemons[indexActif];
        }
        return nullptr;
    }

    void pokemonKO() {
        indexActif++;
    }

    const std::vector<Pokemon*>& getPokemons() const {
        return pokemons;
    }

    std::string getNom() const {
        return nomDresseur;
    }

    bool aPerdu() const {
        return indexActif >= pokemons.size();
    }
};

bool combat(Joueur* joueur, Entraineur* adversaire) {
    std::cout << "\n=== PRÉPARATION DU COMBAT ===" << std::endl;
    
    // Vérifier l'état des 3 premiers Pokémon du joueur
    const auto& tousLesPokemonJoueur = joueur->getPokemons();
    for (size_t i = 0; i < 3 && i < tousLesPokemonJoueur.size(); ++i) {
        if (tousLesPokemonJoueur[i]->getCurrentHp() <= 0) {
            std::cout << "Attention ! " << tousLesPokemonJoueur[i]->getNom() 
                     << " est K.O. et ne peut pas combattre !" << std::endl;
            std::cout << "Veuillez soigner vos Pokémon ou changer leur ordre avant le combat." << std::endl;
            return false;
        }
    }
    
    // Utiliser automatiquement les 3 premiers Pokémon du joueur
    std::vector<Pokemon*> equipeJoueur;
    for (size_t i = 0; i < 3 && i < tousLesPokemonJoueur.size(); ++i) {
        equipeJoueur.push_back(tousLesPokemonJoueur[i]);
    }
    
    std::vector<Pokemon*> equipeAdversaire;
    
    // Sélectionner les 3 premiers Pokémon de l'adversaire
    const std::vector<Pokemon*>& tousLesPokemonAdversaire = adversaire->getPokemons();
    for (size_t i = 0; i < 3 && i < tousLesPokemonAdversaire.size(); ++i) {
        equipeAdversaire.push_back(tousLesPokemonAdversaire[i]);
    }

    EquipeCombat equipeJ(joueur->getNom(), equipeJoueur);
    EquipeCombat equipeA(adversaire->getNom(), equipeAdversaire);

    std::cout << "\n=== DÉBUT DU COMBAT ===" << std::endl;
    std::cout << equipeJ.getNom() << " VS " << equipeA.getNom() << std::endl;
    
    // Afficher les équipes au début du combat
    std::cout << "\nÉquipe de " << equipeJ.getNom() << " :" << std::endl;
    for (const auto& pokemon : equipeJ.getPokemons()) {
        std::cout << "- " << pokemon->getNom() 
                  << " (PV: " << pokemon->getCurrentHp() << "/" << pokemon->getMaxHp() << ")" 
                  << std::endl;
    }
    
    std::cout << "\nÉquipe de " << equipeA.getNom() << " :" << std::endl;
    for (const auto& pokemon : equipeA.getPokemons()) {
        std::cout << "- " << pokemon->getNom() 
                  << " (PV: " << pokemon->getCurrentHp() << "/" << pokemon->getMaxHp() << ")" 
                  << std::endl;
    }
    pause(2000);

    while (true) {
        Pokemon* pokemonJoueur = equipeJ.getPokemonActif();
        Pokemon* pokemonAdversaire = equipeA.getPokemonActif();
        
        if (!pokemonJoueur) {
            std::cout << equipeJ.getNom() << " n'a plus de Pokémon en état de combattre !" << std::endl;
            return false;
        }
        if (!pokemonAdversaire) {
            std::cout << equipeA.getNom() << " n'a plus de Pokémon en état de combattre !" << std::endl;
            return true;
        }

        // Afficher l'état des Pokémon au début du tour
        std::cout << "\n=== TOUR DE COMBAT ===" << std::endl;
        std::cout << equipeJ.getNom() << " : ";
        Combat::afficherEtatPokemon(*pokemonJoueur);
        std::cout << equipeA.getNom() << " : ";
        Combat::afficherEtatPokemon(*pokemonAdversaire);
        pause();

        // Attaque du joueur
        Combat::afficherAttaque(pokemonJoueur->getNom(), pokemonJoueur->getNomAttaque(), pokemonJoueur->getDegatsAttaque());
        auto resultat = Combat::calculerDegats(*pokemonJoueur, *pokemonAdversaire);
        Combat::afficherResultat(resultat, *pokemonAdversaire);
        pause();

        if (pokemonAdversaire->estKO()) {
            std::cout << pokemonAdversaire->getNom() << " est K.O. !" << std::endl;
            equipeA.pokemonKO();
            pause();
            continue;
        }

        // Attaque de l'adversaire
        Combat::afficherAttaque(pokemonAdversaire->getNom(), pokemonAdversaire->getNomAttaque(), pokemonAdversaire->getDegatsAttaque());
        resultat = Combat::calculerDegats(*pokemonAdversaire, *pokemonJoueur);
        Combat::afficherResultat(resultat, *pokemonJoueur);
        pause();

        if (pokemonJoueur->estKO()) {
            std::cout << pokemonJoueur->getNom() << " est K.O. !" << std::endl;
            equipeJ.pokemonKO();
            pause();
        }
    }
}

void afficherDetailsPokemon(const Pokemon* pokemon) {
    std::cout << "\n=== Détails de " << pokemon->getNom() << " ===" << std::endl;
    std::cout << "Types : ";
    for (const auto& type : pokemon->getTypes()) {
        switch(type) {
            case Type::NORMAL: std::cout << "Normal"; break;
            case Type::FEU: std::cout << "Feu"; break;
            case Type::EAU: std::cout << "Eau"; break;
            case Type::PLANTE: std::cout << "Plante"; break;
            case Type::ELECTRIK: std::cout << "Electrik"; break;
            case Type::GLACE: std::cout << "Glace"; break;
            case Type::COMBAT: std::cout << "Combat"; break;
            case Type::POISON: std::cout << "Poison"; break;
            case Type::SOL: std::cout << "Sol"; break;
            case Type::VOL: std::cout << "Vol"; break;
            case Type::PSY: std::cout << "Psy"; break;
            case Type::INSECTE: std::cout << "Insecte"; break;
            case Type::ROCHE: std::cout << "Roche"; break;
            case Type::SPECTRE: std::cout << "Spectre"; break;
            case Type::DRAGON: std::cout << "Dragon"; break;
            case Type::ACIER: std::cout << "Acier"; break;
            case Type::FEE: std::cout << "Fée"; break;
        }
        std::cout << " ";
    }
    std::cout << std::endl;
    std::cout << "Points de vie : " << pokemon->getCurrentHp() << "/" << pokemon->getMaxHp() << std::endl;
    std::cout << "Attaque : " << pokemon->getNomAttaque() << " (Dégâts : " << pokemon->getDegatsAttaque() << ")" << std::endl;
}

void afficherEquipePokemonInteractif(const Entraineur* entraineur) {
    bool continuer = true;
    while (continuer) {
        const auto& pokemons = entraineur->getPokemons();
        std::cout << "\nÉquipe de " << entraineur->getNom() << " :" << std::endl;
        for (size_t i = 0; i < pokemons.size(); ++i) {
            std::cout << i + 1 << ". " << pokemons[i]->getNom() 
                      << " (PV: " << pokemons[i]->getCurrentHp() << "/" << pokemons[i]->getMaxHp() << ")" 
                      << std::endl;
        }
        
        std::cout << "\nChoisissez un Pokémon pour voir ses détails (1-" << pokemons.size() << ") ou 0 pour revenir : ";
        int choix = lireChoix();
        
        if (choix == 0) {
            continuer = false;
        } else if (choix > 0 && choix <= static_cast<int>(pokemons.size())) {
            afficherDetailsPokemon(pokemons[choix - 1]);
            std::cout << "\nAppuyez sur Entrée pour revenir à la liste...";
            std::cin.get();
        } else {
            std::cout << "Choix invalide !" << std::endl;
            pause();
        }
    }
}

void soignerPokemonSpecifique(Joueur* joueur) {
    const auto& pokemons = joueur->getPokemons();
    
    // Vérifier d'abord si des Pokémon sont blessés
    bool pokemonBlesses = false;
    for (const auto& pokemon : pokemons) {
        if (pokemon->getCurrentHp() < pokemon->getMaxHp()) {
            pokemonBlesses = true;
            break;
        }
    }
    
    if (!pokemonBlesses) {
        std::cout << "Tous vos Pokémon sont en pleine forme !" << std::endl;
        pause();
        return;
    }

    std::cout << "\nChoisissez un Pokémon à soigner :" << std::endl;
    
    // Afficher uniquement les Pokémon blessés
    for (size_t i = 0; i < pokemons.size(); ++i) {
        if (pokemons[i]->getCurrentHp() < pokemons[i]->getMaxHp()) {
            std::cout << i + 1 << ". " << pokemons[i]->getNom() 
                      << " (PV: " << pokemons[i]->getCurrentHp() << "/" << pokemons[i]->getMaxHp() << ")" 
                      << std::endl;
        }
    }
    
    std::cout << "0. Annuler" << std::endl;
    std::cout << "\nVotre choix : ";
    int choix = lireChoix();
    
    if (choix > 0 && choix <= static_cast<int>(pokemons.size())) {
        Pokemon* pokemon = pokemons[choix - 1];
        if (pokemon->getCurrentHp() < pokemon->getMaxHp()) {
            pokemon->soigner();
            std::cout << pokemon->getNom() << " a été soigné !" << std::endl;
        } else {
            std::cout << pokemon->getNom() << " est déjà en pleine forme !" << std::endl;
        }
        pause();
    }
}

void interagirAvecPokemonEtEntraineurs(Joueur* joueur, const Maitre* maitre, const std::vector<Leader*>& leaders) {
    bool continuerInteraction = true;
    
    while (continuerInteraction) {
        std::cout << "\n=== Menu d'Interaction ===" << std::endl;
        std::cout << "1. Interagir avec mes Pokémon" << std::endl;
        std::cout << "2. Interagir avec les Leaders vaincus" << std::endl;
        if (joueur->getBadges() >= leaders.size()) {
            std::cout << "3. Interagir avec le Maître" << std::endl;
        }
        std::cout << "0. Retour" << std::endl;
        std::cout << "Votre choix : ";

        int choix = lireChoix();
        switch (choix) {
            case 0:
                continuerInteraction = false;
                break;
                
            case 1: {
                const auto& pokemons = joueur->getPokemons();
                std::cout << "\nChoisissez un Pokémon pour interagir (1-" << pokemons.size() << ") ou 0 pour revenir : " << std::endl;
                for (size_t i = 0; i < pokemons.size(); ++i) {
                    std::cout << i + 1 << ". " << pokemons[i]->getNom() << std::endl;
                }
                
                int choixPokemon = lireChoix();
                if (choixPokemon > 0 && choixPokemon <= static_cast<int>(pokemons.size())) {
                    std::cout << pokemons[choixPokemon - 1]->interagir() << std::endl;
                    pause(2000);
                }
                break;
            }
            case 2: {
                // Créer une liste des Leaders vaincus
                std::vector<Leader*> leadersVaincus;
                for (Leader* leader : leaders) {
                    if (leader->aEteVaincu()) {
                        leadersVaincus.push_back(leader);
                    }
                }
                
                if (leadersVaincus.empty()) {
                    std::cout << "Vous n'avez pas encore vaincu de Leader." << std::endl;
                    pause();
                } else {
                    bool choisirLeader = true;
                    while (choisirLeader) {
                        std::cout << "\nChoisissez un Leader vaincu (0 pour revenir) :" << std::endl;
                        for (size_t i = 0; i < leadersVaincus.size(); ++i) {
                            std::cout << i+1 << ". " << leadersVaincus[i]->getNom() << std::endl;
                        }
                        
                        int choixLeader = lireChoix();
                        if (choixLeader == 0) {
                            choisirLeader = false;
                        } else if (choixLeader >= 1 && choixLeader <= static_cast<int>(leadersVaincus.size())) {
                            Leader* leader = leadersVaincus[choixLeader-1];
                            std::cout << leader->interagir() << std::endl;
                            pause();
                            choisirLeader = false;
                        } else {
                            std::cout << "Choix invalide !" << std::endl;
                            pause();
                        }
                    }
                }
                break;
            }
            case 3: {
                if (joueur->getBadges() >= leaders.size() && maitre != nullptr) {
                    std::cout << maitre->interagir() << std::endl;
                    pause(2000);
                }
                break;
            }
            default:
                std::cout << "Choix invalide !" << std::endl;
                pause();
                break;
        }
    }
}

void gererOrdrePokemon(Joueur* joueur) {
    bool continuer = true;
    while (continuer) {
        std::cout << "\n=== Gestion de l'ordre des Pokémon ===" << std::endl;
        std::cout << "Ordre actuel de vos Pokémon :" << std::endl;
        const auto& pokemons = joueur->getPokemons();
        for (size_t i = 0; i < pokemons.size(); ++i) {
            std::cout << i+1 << ". " << pokemons[i]->getNom() 
                     << " (PV: " << pokemons[i]->getCurrentHp() << "/" << pokemons[i]->getMaxHp() << ")"
                     << (i < 3 ? " [Combat]" : "") << std::endl;
        }
        
        std::cout << "\nQue souhaitez-vous faire ?" << std::endl;
        std::cout << "1. Échanger deux Pokémon" << std::endl;
        std::cout << "0. Retour" << std::endl;
        std::cout << "Votre choix : ";
        
        int choix = lireChoix();
        if (choix == 0) {
            continuer = false;
        } else if (choix == 1) {
            std::cout << "Choisissez le premier Pokémon (1-" << pokemons.size() << ") : ";
            int pos1 = lireChoix() - 1;
            std::cout << "Choisissez le deuxième Pokémon (1-" << pokemons.size() << ") : ";
            int pos2 = lireChoix() - 1;
            
            if (pos1 >= 0 && pos1 < static_cast<int>(pokemons.size()) &&
                pos2 >= 0 && pos2 < static_cast<int>(pokemons.size()) &&
                pos1 != pos2) {
                joueur->echangerPokemon(pos1, pos2);
                std::cout << "Pokémon échangés avec succès !" << std::endl;
            } else {
                std::cout << "Positions invalides ! Veuillez choisir des numéros entre 1 et " 
                         << pokemons.size() << "." << std::endl;
            }
            pause();
        } else {
            std::cout << "Choix invalide ! Veuillez choisir 1 pour échanger ou 0 pour revenir." << std::endl;
            pause();
        }
    }
}

int main() {
    Joueur* joueur = nullptr;
    std::vector<Leader*> leaders;
    Maitre* maitre = nullptr;

    try {
        std::cout << "Chargement des Pokémon..." << std::endl;
        // Charger d'abord tous les Pokémon disponibles
        DataLoader::chargerPokemons("data/pokemon.csv");
        
        std::cout << "Chargement du joueur..." << std::endl;
        // Charger le joueur avec ses Pokémon
        joueur = DataLoader::chargerJoueur("data/joueur.csv");
        
        std::cout << "Chargement des Leaders..." << std::endl;
        // Charger les Leaders
        leaders = DataLoader::chargerLeaders("data/leaders.csv");
        
        std::cout << "Chargement du Maître..." << std::endl;
        // Charger le Maître
        maitre = DataLoader::chargerMaitre("data/maitres.csv");

        std::cout << "Chargement terminé !" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement des données : " << e.what() << std::endl;
        return 1;
    }

    bool continuer = true;
    while (continuer) {
        afficherMenu();
        int choix = lireChoix();

        switch (choix) {
            case 1: // Afficher Pokémon
                afficherEquipePokemonInteractif(joueur);
                break;

            case 2: // Soigner
                soignerPokemonSpecifique(joueur);
                break;

            case 3: // Stats
                std::cout << "\nStatistiques de " << joueur->getNom() << std::endl;
                std::cout << "Badges : " << joueur->getBadges() << std::endl;
                std::cout << "Victoires : " << joueur->getVictoires() << std::endl;
                std::cout << "Défaites : " << joueur->getDefaites() << std::endl;
                break;

            case 4: // Défier Leader
                if (leaders.empty()) {
                    std::cout << "Aucun Leader disponible." << std::endl;
                    pause();
                } else {
                    bool choisirLeader = true;
                    while (choisirLeader) {
                        std::cout << "\nChoisissez un Leader à défier (0 pour revenir) :" << std::endl;
                        for (size_t i = 0; i < leaders.size(); ++i) {
                            std::string statut = leaders[i]->aEteVaincu() ? " (Vaincu)" : "";
                            std::cout << i+1 << ". " << leaders[i]->messageDefi() << statut << std::endl;
                        }
                        
                        int choixLeader = lireChoix();
                        if (choixLeader == 0) {
                            choisirLeader = false;
                        } else if (choixLeader >= 1 && choixLeader <= static_cast<int>(leaders.size())) {
                            Leader* leader = leaders[choixLeader-1];
                            
                            bool resultatCombat = combat(joueur, leader);
                            if (resultatCombat) {
                                std::cout << "Félicitations ! Vous avez battu " << leader->getNom() << " !" << std::endl;
                                if (!leader->aEteVaincu()) {
                                    joueur->gagnerBadge();
                                    leader->setVaincu(true);
                                }
                                joueur->gagnerCombat();
                                // Soigner l'équipe du Leader après le combat
                                leader->soignerEquipe();
                            } else if (resultatCombat == false) {
                                // Si le combat n'a pas pu commencer à cause d'un Pokémon K.O.
                                pause(2000);
                            } else {
                                std::cout << "Vous avez perdu contre " << leader->getNom() << "." << std::endl;
                                joueur->perdreCombat();
                                // Soigner l'équipe du Leader après le combat
                                leader->soignerEquipe();
                            }
                            choisirLeader = false;
                        } else {
                            std::cout << "Choix invalide !" << std::endl;
                            pause();
                        }
                    }
                }
                break;

            case 5: // Défier Maître
                if (joueur->getBadges() < leaders.size()) {
                    std::cout << "Vous devez avoir vaincu tous les Leaders (" 
                             << leaders.size() << " badges) pour défier le Maître !" << std::endl;
                    pause();
                } else {
                    std::cout << maitre->interagir() << std::endl;
                    pause();
                    
                    if (combat(joueur, maitre)) {
                        std::cout << "Félicitations ! Vous avez battu le Maître Pokémon !" << std::endl;
                        joueur->gagnerCombat();
                        maitre->incrementerVictoires();
                    } else {
                        std::cout << "Vous avez perdu contre le Maître." << std::endl;
                        joueur->perdreCombat();
                    }
                }
                break;

            case 6: // Interagir
                interagirAvecPokemonEtEntraineurs(joueur, maitre, leaders);
                break;

            case 7: // Gérer l'ordre des Pokémon
                gererOrdrePokemon(joueur);
                break;

            case 8: // Quitter
                std::cout << "Au revoir !" << std::endl;
                continuer = false;
                break;

            default:
                std::cout << "Choix invalide ! Veuillez choisir entre 1 et 8." << std::endl;
        }
    }

    // Nettoyage de la mémoire dans le bon ordre
    // D'abord les entraîneurs qui utilisent les Pokémon
    if (joueur != nullptr) {
        delete joueur;
        joueur = nullptr;
    }
    
    if (maitre != nullptr) {
        delete maitre;
        maitre = nullptr;
    }
    
    for (Leader* leader : leaders) {
        if (leader != nullptr) {
            delete leader;
        }
    }
    leaders.clear();

    // Enfin, nettoyer le cache des Pokémon
    DataLoader::nettoyerCache();

    return 0;
} 