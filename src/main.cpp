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
#include <ctime>
#include <random>

// Codes ANSI pour les couleurs
const std::string RESET = "\033[0m";
const std::string ROUGE = "\033[31m";
const std::string VERT = "\033[32m";
const std::string JAUNE = "\033[33m";
const std::string BLEU = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";
const std::string BLANC = "\033[37m";
const std::string GRAS = "\033[1m";

void afficherMenu() {
    std::cout << "\n" << GRAS << MAGENTA << "=== Menu Principal ===" << RESET << std::endl;
    std::cout << CYAN << "1. Afficher mes Pokémon" << RESET << std::endl;
    std::cout << CYAN << "2. Soigner mes Pokémon" << RESET << std::endl;
    std::cout << CYAN << "3. Afficher mes stats" << RESET << std::endl;
    std::cout << VERT << "4. Défier un Leader" << RESET << std::endl;
    std::cout << VERT << "5. Défier un Maître" << RESET << std::endl;
    std::cout << VERT << "6. Défier un autre dresseur" << RESET << std::endl;
    std::cout << CYAN << "7. Interagir avec les Pokémon/Entraîneurs" << RESET << std::endl;
    std::cout << CYAN << "8. Gérer l'ordre des Pokémon" << RESET << std::endl;
    std::cout << CYAN << "9. Changer de dresseur" << RESET << std::endl;
    std::cout << ROUGE << "0. Quitter" << RESET << std::endl;
    std::cout << GRAS << "Votre choix : " << RESET;
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
    
    // Déterminer si l'adversaire est un Maître
    bool estMaitre = adversaire->estMaitre();
    
    // Nombre de Pokémon pour le combat
    int nombrePokemonCombat = estMaitre ? 6 : 3;
    
    // Vérifier l'état des Pokémon du joueur pour le combat
    const auto& tousLesPokemonJoueur = joueur->getPokemons();
    for (size_t i = 0; i < nombrePokemonCombat && i < tousLesPokemonJoueur.size(); ++i) {
        if (tousLesPokemonJoueur[i]->getCurrentHp() <= 0) {
            std::cout << "Attention ! " << tousLesPokemonJoueur[i]->getNom() 
                     << " est K.O. et ne peut pas combattre !" << std::endl;
            std::cout << "Veuillez soigner vos Pokémon ou changer leur ordre avant le combat." << std::endl;
            return false;
        }
    }
    
    // Si c'est un combat contre un Maître, vérifier que le joueur a au moins 6 Pokémon
    if (estMaitre && tousLesPokemonJoueur.size() < 6) {
        std::cout << "Pour défier un Maître, vous devez avoir au moins 6 Pokémon dans votre équipe !" << std::endl;
        std::cout << "Vous n'avez que " << tousLesPokemonJoueur.size() << " Pokémon." << std::endl;
        return false;
    }
    
    // Sélectionner les Pokémon du joueur pour le combat
    std::vector<Pokemon*> equipeJoueur;
    for (size_t i = 0; i < nombrePokemonCombat && i < tousLesPokemonJoueur.size(); ++i) {
        equipeJoueur.push_back(tousLesPokemonJoueur[i]);
    }
    
    // Sélectionner les Pokémon de l'adversaire pour le combat
    std::vector<Pokemon*> equipeAdversaire;
    const std::vector<Pokemon*>& tousLesPokemonAdversaire = adversaire->getPokemons();
    for (size_t i = 0; i < nombrePokemonCombat && i < tousLesPokemonAdversaire.size(); ++i) {
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
        Combat::afficherAttaque(pokemonJoueur->getNom(), pokemonJoueur->getNomAttaque(), pokemonJoueur->getDegatsAttaque(), false);
        auto resultat = Combat::calculerDegats(*pokemonJoueur, *pokemonAdversaire, false);
        Combat::afficherResultat(resultat, *pokemonAdversaire);
        pause();

        if (pokemonAdversaire->estKO()) {
            std::cout << pokemonAdversaire->getNom() << " est K.O. !" << std::endl;
            equipeA.pokemonKO();
            pause();
            continue;
        }

        // Attaque de l'adversaire (avec bonus si c'est un Maître)
        Combat::afficherAttaque(pokemonAdversaire->getNom(), pokemonAdversaire->getNomAttaque(), pokemonAdversaire->getDegatsAttaque(), estMaitre);
        resultat = Combat::calculerDegats(*pokemonAdversaire, *pokemonJoueur, estMaitre);
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
    std::cout << "\n" << GRAS << BLEU << "=== Détails de " << pokemon->getNom() << " ===" << RESET << std::endl;
    std::cout << JAUNE << "Types : " << RESET;
    for (const auto& type : pokemon->getTypes()) {
        switch(type) {
            case Type::NORMAL: std::cout << BLANC << "Normal" << RESET; break;
            case Type::FEU: std::cout << ROUGE << "Feu" << RESET; break;
            case Type::EAU: std::cout << BLEU << "Eau" << RESET; break;
            case Type::PLANTE: std::cout << VERT << "Plante" << RESET; break;
            case Type::ELECTRIK: std::cout << JAUNE << "Electrik" << RESET; break;
            case Type::GLACE: std::cout << CYAN << "Glace" << RESET; break;
            case Type::COMBAT: std::cout << ROUGE << "Combat" << RESET; break;
            case Type::POISON: std::cout << MAGENTA << "Poison" << RESET; break;
            case Type::SOL: std::cout << JAUNE << "Sol" << RESET; break;
            case Type::VOL: std::cout << CYAN << "Vol" << RESET; break;
            case Type::PSY: std::cout << MAGENTA << "Psy" << RESET; break;
            case Type::INSECTE: std::cout << VERT << "Insecte" << RESET; break;
            case Type::ROCHE: std::cout << JAUNE << "Roche" << RESET; break;
            case Type::SPECTRE: std::cout << MAGENTA << "Spectre" << RESET; break;
            case Type::DRAGON: std::cout << BLEU << "Dragon" << RESET; break;
            case Type::ACIER: std::cout << BLANC << "Acier" << RESET; break;
            case Type::TENEBRES: std::cout << MAGENTA << "Ténèbres" << RESET; break;
            case Type::FEE: std::cout << MAGENTA << "Fée" << RESET; break;
        }
        std::cout << " ";
    }
    std::cout << std::endl;
    
    // Barre de vie colorée
    std::cout << CYAN << "Points de vie : " << RESET;
    int pourcentageVie = (pokemon->getCurrentHp() * 100) / pokemon->getMaxHp();
    std::string couleurVie = ROUGE;  // Rouge par défaut
    
    if (pourcentageVie > 70) {
        couleurVie = VERT;  // Bonne santé
    } else if (pourcentageVie > 30) {
        couleurVie = JAUNE;  // Santé moyenne
    }
    
    std::cout << couleurVie << pokemon->getCurrentHp() << RESET << "/" << GRAS << pokemon->getMaxHp() << RESET << " ";
    
    // Afficher une barre de progression
    std::cout << "[";
    int barresPleines = (pourcentageVie * 20) / 100;
    for (int i = 0; i < 20; i++) {
        if (i < barresPleines) {
            std::cout << couleurVie << "█" << RESET;
        } else {
            std::cout << "░";
        }
    }
    std::cout << "]" << std::endl;
    
    std::cout << MAGENTA << "Attaque : " << GRAS << pokemon->getNomAttaque() << RESET 
              << " (" << ROUGE << "Dégâts : " << GRAS << pokemon->getDegatsAttaque() << RESET << ")" << std::endl;
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

    // Afficher les Pokémon blessés
    std::cout << "\nVos Pokémon blessés :" << std::endl;
    int compteurBlesses = 0;
    for (size_t i = 0; i < pokemons.size(); ++i) {
        if (pokemons[i]->getCurrentHp() < pokemons[i]->getMaxHp()) {
            std::cout << i + 1 << ". " << pokemons[i]->getNom() 
                      << " (PV: " << pokemons[i]->getCurrentHp() << "/" << pokemons[i]->getMaxHp() << ")" 
                      << std::endl;
            compteurBlesses++;
        }
    }
    
    std::cout << "\nQue souhaitez-vous faire ?" << std::endl;
    std::cout << "1. Soigner un Pokémon spécifique" << std::endl;
    std::cout << "2. Soigner tous les Pokémon" << std::endl;
    std::cout << "0. Annuler" << std::endl;
    std::cout << "\nVotre choix : ";
    
    int choixAction = lireChoix();
    
    if (choixAction == 1) {
        // Soigner un Pokémon spécifique
        std::cout << "\nChoisissez un Pokémon à soigner (1-" << pokemons.size() << ") ou 0 pour annuler : ";
        int choix = lireChoix();
        
        if (choix > 0 && choix <= static_cast<int>(pokemons.size())) {
            Pokemon* pokemon = pokemons[choix - 1];
            if (pokemon->getCurrentHp() < pokemon->getMaxHp()) {
                pokemon->soigner();
                std::cout << pokemon->getNom() << " a été soigné !" << std::endl;
            } else {
                std::cout << pokemon->getNom() << " est déjà en pleine forme !" << std::endl;
            }
        }
    } else if (choixAction == 2) {
        // Soigner tous les Pokémon
        int nombrePokemonsSoignes = 0;
        for (auto& pokemon : pokemons) {
            if (pokemon->getCurrentHp() < pokemon->getMaxHp()) {
                pokemon->soigner();
                nombrePokemonsSoignes++;
            }
        }
        
        if (nombrePokemonsSoignes > 0) {
            std::cout << "Tous vos Pokémon blessés (" << nombrePokemonsSoignes << ") ont été soignés !" << std::endl;
        } else {
            std::cout << "Aucun Pokémon n'avait besoin de soins !" << std::endl;
        }
    }
    
    pause();
}

void interagirAvecPokemonEtEntraineurs(Joueur* joueur, const Maitre* maitreCourant, const std::vector<Leader*>& leaders, const std::vector<Maitre*>& maitres) {
    bool continuerInteraction = true;
    
    while (continuerInteraction) {
        std::cout << "\n" << GRAS << MAGENTA << "=== Menu d'Interaction ===" << RESET << std::endl;
        std::cout << "1. Interagir avec mes Pokémon" << std::endl;
        std::cout << "2. Interagir avec les Leaders vaincus" << std::endl;
        
        // Vérifier si au moins un maître a été vaincu
        bool maitreVaincu = false;
        for (const Maitre* maitre : maitres) {
            if (maitre->aEteVaincu()) {
                maitreVaincu = true;
                break;
            }
        }
        
        if (maitreVaincu) {
                         std::cout << "3. Interagir avec les Maîtres vaincus" << std::endl;
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
                // Créer une liste des Maîtres vaincus
                std::vector<Maitre*> maitresVaincus;
                for (Maitre* maitre : maitres) {
                    if (maitre->aEteVaincu()) {
                        maitresVaincus.push_back(maitre);
                    }
                }
                
                if (maitresVaincus.empty()) {
                    std::cout << "Vous n'avez pas encore vaincu de Maître." << std::endl;
                    pause();
                } else {
                    bool choisirMaitre = true;
                    while (choisirMaitre) {
                        std::cout << "\nChoisissez un Maître vaincu pour interagir (0 pour revenir) :" << std::endl;
                        for (size_t i = 0; i < maitresVaincus.size(); ++i) {
                            std::cout << i+1 << ". " << maitresVaincus[i]->getNom() << ", " << maitresVaincus[i]->getTitre() << std::endl;
                        }
                        
                        int choixMaitre = lireChoix();
                        if (choixMaitre == 0) {
                            choisirMaitre = false;
                        } else if (choixMaitre >= 1 && choixMaitre <= static_cast<int>(maitresVaincus.size())) {
                            Maitre* maitre = maitresVaincus[choixMaitre-1];
                            std::cout << maitre->interagir() << std::endl;
                            pause(2000);
                            choisirMaitre = false;
                        } else {
                            std::cout << "Choix invalide !" << std::endl;
                            pause();
                        }
                    }
                }
                break;
            }
            default:
                std::cout << "Choix invalide ! Veuillez choisir entre 0 et 9." << std::endl;
                pause();
                break;
        }
    }
}

void gererOrdrePokemon(Joueur* joueur) {
    bool continuer = true;
    while (continuer) {
        std::cout << "\n" << GRAS << MAGENTA << "=== Gestion de l'ordre des Pokémon ===" << RESET << std::endl;
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

// Fonction pour défier un autre dresseur
void defierAutreDresseur(Joueur* joueurPrincipal, const std::string& fichierJoueurs) {
    // Charger tous les joueurs du fichier
    std::vector<Joueur*> autreDresseurs;
    try {
        autreDresseurs = DataLoader::chargerTousJoueurs(fichierJoueurs);
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement des dresseurs : " << e.what() << std::endl;
        pause();
        return;
    }
    
    // Filtrer pour ne pas inclure le joueur principal
    std::vector<Joueur*> dresseursFiltres;
    for (Joueur* dresseur : autreDresseurs) {
        if (dresseur->getNom() != joueurPrincipal->getNom()) {
            dresseursFiltres.push_back(dresseur);
        } else {
            delete dresseur; // On supprime la copie du joueur principal
        }
    }
    
    if (dresseursFiltres.empty()) {
        std::cout << "Aucun autre dresseur disponible pour le combat." << std::endl;
        pause();
        return;
    }
    
    // Afficher la liste des dresseurs disponibles
    bool continuer = true;
    while (continuer) {
        std::cout << "\n" << GRAS << MAGENTA << "=== Dresseurs disponibles ===" << RESET << std::endl;
        for (size_t i = 0; i < dresseursFiltres.size(); ++i) {
            std::cout << i + 1 << ". " << dresseursFiltres[i]->getNom() << std::endl;
        }
        std::cout << "0. Retour" << std::endl;
        std::cout << "Choisissez un dresseur à défier : ";
        
        int choix = lireChoix();
        if (choix == 0) {
            continuer = false;
        } else if (choix > 0 && choix <= static_cast<int>(dresseursFiltres.size())) {
            Joueur* adversaire = dresseursFiltres[choix - 1];
            
            std::cout << "\nVous allez affronter " << adversaire->getNom() << " !" << std::endl;
            pause();
            
            bool resultatCombat = combat(joueurPrincipal, adversaire);
            if (resultatCombat) {
                std::cout << "Félicitations ! Vous avez battu " << adversaire->getNom() << " !" << std::endl;
                joueurPrincipal->gagnerCombat();
                if (!adversaire->aEteVaincu()) {
                    adversaire->setVaincu(true);
                    // Donner le badge au joueur (cette méthode appelle déjà gagnerBadge)
                    adversaire->donnerBadge(*joueurPrincipal);
                }
            } else if (resultatCombat == false) {
                // Si le combat n'a pas pu commencer à cause d'un Pokémon K.O.
                // Aucune action supplémentaire requise, le message est déjà affiché
                // dans la fonction combat
            } else {
                std::cout << "Vous avez perdu contre " << adversaire->getNom() << "." << std::endl;
                joueurPrincipal->perdreCombat();
                // Sauvegarder les statistiques après la défaite
                DataLoader::sauvegarderStatistiques("data/statistiques.csv", joueurPrincipal);
                // Soigner l'équipe du Leader après le combat
                adversaire->soignerEquipe();
            }
            
            // Soigner l'équipe de l'adversaire après le combat
            adversaire->soignerEquipe();
            
            continuer = false;
        } else {
            std::cout << "Choix invalide !" << std::endl;
            pause();
        }
    }
    
    // Nettoyer la mémoire
    for (Joueur* dresseur : dresseursFiltres) {
        delete dresseur;
    }
    dresseursFiltres.clear();
}

// Fonction pour changer de dresseur
Joueur* changerDresseur(Joueur* joueurActuel, const std::string& fichierJoueurs) {
    // Charger tous les joueurs du fichier
    std::vector<Joueur*> tousJoueurs;
    try {
        tousJoueurs = DataLoader::chargerTousJoueurs(fichierJoueurs);
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement des dresseurs : " << e.what() << std::endl;
        pause();
        return joueurActuel;
    }
    
    if (tousJoueurs.empty()) {
        std::cout << "Aucun dresseur disponible." << std::endl;
        pause();
        return joueurActuel;
    }
    
    // Afficher la liste des dresseurs disponibles
    std::cout << "\n" << GRAS << MAGENTA << "=== Sélection du dresseur ===" << RESET << std::endl;
    std::cout << "Dresseur actuel : " << joueurActuel->getNom() << std::endl;
    std::cout << "\nDresseurs disponibles :" << std::endl;
    
    for (size_t i = 0; i < tousJoueurs.size(); ++i) {
        std::string marqueur = (tousJoueurs[i]->getNom() == joueurActuel->getNom()) ? " [Actuel]" : "";
        std::cout << i + 1 << ". " << tousJoueurs[i]->getNom() << marqueur << std::endl;
    }
    
    std::cout << "0. Annuler" << std::endl;
    std::cout << "Choisissez un dresseur : ";
    
    int choix = lireChoix();
    if (choix == 0) {
        // Libérer la mémoire des autres joueurs
        for (Joueur* j : tousJoueurs) {
            if (j->getNom() != joueurActuel->getNom()) {
                delete j;
            }
        }
        return joueurActuel;
    } else if (choix > 0 && choix <= static_cast<int>(tousJoueurs.size())) {
        Joueur* nouveauJoueur = tousJoueurs[choix - 1];
        
        // Si le joueur a choisi un dresseur différent
        if (nouveauJoueur->getNom() != joueurActuel->getNom()) {
            std::cout << "Vous jouez maintenant avec " << nouveauJoueur->getNom() << " !" << std::endl;
            
            // Supprimer l'ancien joueur mais garder le nouveau
            for (Joueur* j : tousJoueurs) {
                if (j->getNom() != nouveauJoueur->getNom()) {
                    delete j;
                }
            }
            
            // Supprimer l'ancien joueur
            delete joueurActuel;
            
            pause();
            return nouveauJoueur;
        } else {
            std::cout << "Vous jouez déjà avec " << joueurActuel->getNom() << "." << std::endl;
            
            // Libérer la mémoire des autres joueurs
            for (Joueur* j : tousJoueurs) {
                if (j != joueurActuel) {
                    delete j;
                }
            }
            
            pause();
            return joueurActuel;
        }
    } else {
        std::cout << "Choix invalide !" << std::endl;
        
        // Libérer la mémoire de tous les joueurs sauf l'actuel
        for (Joueur* j : tousJoueurs) {
            if (j->getNom() != joueurActuel->getNom()) {
                delete j;
            }
        }
        
        pause();
        return joueurActuel;
    }
}

int main() {
    Joueur* joueur = nullptr;
    std::vector<Leader*> leaders;
    std::vector<Maitre*> maitres;
    Maitre* maitreCourant = nullptr; // Le maître actuellement sélectionné pour le combat

    try {
        std::cout << "Chargement des Pokémon..." << std::endl;
        // Charger d'abord tous les Pokémon disponibles
        DataLoader::chargerPokemons("data/pokemon.csv");
        
        std::cout << "Chargement du joueur..." << std::endl;
        // Charger le joueur avec ses Pokémon
        joueur = DataLoader::chargerJoueur("data/joueur.csv");
        
        // Charger les statistiques du joueur
        std::cout << "Chargement des statistiques..." << std::endl;
        DataLoader::chargerStatistiques("data/statistiques.csv", joueur);
        
        std::cout << "Chargement des Leaders..." << std::endl;
        // Charger les Leaders
        leaders = DataLoader::chargerLeaders("data/leaders.csv");
        
        std::cout << "Chargement des Maîtres..." << std::endl;
        // Charger les Maîtres
        maitres = DataLoader::chargerMaitres("data/maitres.csv");

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
                {
                bool dansStats = true;
                while (dansStats) {
                    std::cout << "\n" << GRAS << MAGENTA << "=== Statistiques de " << joueur->getNom() << " ===" << RESET << std::endl;
                    std::cout << "Badges : " << joueur->getBadges() << std::endl;
                    
                    // Afficher les badges spécifiques gagnés
                    const auto& badgesGagnes = joueur->getBadgesGagnes();
                    if (!badgesGagnes.empty()) {
                        std::cout << "Badges gagnés : ";
                        bool premier = true;
                        for (const auto& badge : badgesGagnes) {
                            if (!premier) {
                                std::cout << ", ";
                            }
                            std::cout << badge;
                            premier = false;
                        }
                        std::cout << std::endl;
                    } else {
                        std::cout << "Aucun badge gagné pour l'instant" << std::endl;
                    }
                    
                    std::cout << "Victoires : " << joueur->getVictoires() << std::endl;
                    std::cout << "Défaites : " << joueur->getDefaites() << std::endl;
                    
                    if (joueur->getVictoires() > 0 || joueur->getDefaites() > 0) {
                        double ratio = joueur->getVictoires() / static_cast<double>(joueur->getVictoires() + joueur->getDefaites());
                        std::cout << "Ratio de victoires : " << static_cast<int>(ratio * 100) << "%" << std::endl;
                    }
                    
                    std::cout << "\n0. Retour au menu principal" << std::endl;
                    std::cout << "Votre choix : ";
                    
                    int choixStat = lireChoix();
                    if (choixStat == 0) {
                        dansStats = false;
                    }
                }
                break;
                }

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
                                    leader->setVaincu(true);
                                    // Donner le badge au joueur (cette méthode appelle déjà gagnerBadge)
                                    leader->donnerBadge(*joueur);
                                }
                                joueur->gagnerCombat();
                                // Sauvegarder les statistiques après la victoire
                                DataLoader::sauvegarderStatistiques("data/statistiques.csv", joueur);
                                // Soigner l'équipe du Leader après le combat
                                leader->soignerEquipe();
                            } else if (resultatCombat == false) {
                                // Si le combat n'a pas pu commencer à cause d'un Pokémon K.O.
                                // Aucune action supplémentaire requise, le message est déjà affiché
                                // dans la fonction combat
                            } else {
                                std::cout << "Vous avez perdu contre " << leader->getNom() << "." << std::endl;
                                joueur->perdreCombat();
                                // Sauvegarder les statistiques après la défaite
                                DataLoader::sauvegarderStatistiques("data/statistiques.csv", joueur);
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
                             << leaders.size() << " badges) pour défier un Maître !" << std::endl;
                    pause();
                } else if (maitres.empty()) {
                    std::cout << "Aucun Maître disponible." << std::endl;
                    pause();
                } else {
                    // Sélectionner un Maître aléatoirement à chaque défi
                    // Initialiser le générateur avec une nouvelle seed à chaque appel
                    std::random_device rd;  // Seed plus robuste qu'un time(nullptr)
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<> dist(0, maitres.size() - 1);
                    int indexMaitre = dist(gen);
                    maitreCourant = maitres[indexMaitre];
                    
                    std::cout << "Vous allez défier " << maitreCourant->getNom() << ", " 
                             << maitreCourant->getTitre() << " !" << std::endl;
                    std::cout << "\nATTENTION : Les Maîtres bénéficient d'un bonus de dégâts de " 
                             << static_cast<int>((Combat::BONUS_DEGATS_MAITRE - 1.0) * 100) << "% !" << std::endl;
                    std::cout << "De plus, ce combat se fera avec 6 Pokémon contre 6 !" << std::endl;
                    pause();
                    
                    bool resultatCombat = combat(joueur, maitreCourant);
                    if (resultatCombat) {
                        std::cout << "Félicitations ! Vous avez battu " << maitreCourant->getNom() 
                                 << ", le " << maitreCourant->getTitre() << " !" << std::endl;
                        joueur->gagnerCombat();
                        maitreCourant->incrementerVictoires();
                        maitreCourant->setVaincu(true); // Marquer le maître comme vaincu
                        // Sauvegarder les statistiques après la victoire
                        DataLoader::sauvegarderStatistiques("data/statistiques.csv", joueur);
                    } else if (resultatCombat == false) {
                        // Si le combat n'a pas pu commencer à cause d'un Pokémon K.O.
                        // Aucune action supplémentaire requise, le message est déjà affiché
                        // dans la fonction combat
                    } else {
                        std::cout << "Vous avez perdu contre " << maitreCourant->getNom() << "." << std::endl;
                        joueur->perdreCombat();
                        // Sauvegarder les statistiques après la défaite
                        DataLoader::sauvegarderStatistiques("data/statistiques.csv", joueur);
                    }
                    
                    // Soigner l'équipe du Maître après le combat
                    maitreCourant->soignerEquipe();
                }
                break;

            case 6: // Défier un autre dresseur
                defierAutreDresseur(joueur, "data/joueur.csv");
                break;

            case 7: // Interagir
                interagirAvecPokemonEtEntraineurs(joueur, maitreCourant, leaders, maitres);
                break;

            case 8: // Gérer l'ordre des Pokémon
                gererOrdrePokemon(joueur);
                break;

            case 9: // Changer de dresseur
                joueur = changerDresseur(joueur, "data/joueur.csv");
                break;

            case 0: // Quitter
                std::cout << "Sauvegarde des statistiques..." << std::endl;
                DataLoader::sauvegarderStatistiques("data/statistiques.csv", joueur);
                std::cout << "Au revoir !" << std::endl;
                continuer = false;
                break;

            default:
                std::cout << "Choix invalide ! Veuillez choisir entre 0 et 9." << std::endl;
        }
    }

    // Nettoyage de la mémoire dans le bon ordre
    // D'abord les entraîneurs qui utilisent les Pokémon
    if (joueur != nullptr) {
        delete joueur;
        joueur = nullptr;
    }
    
    // Nettoyer les maîtres
    for (Maitre* m : maitres) {
        if (m != nullptr) {
            delete m;
        }
    }
    maitres.clear();
    
    // Nettoyer les leaders
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