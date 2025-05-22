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
using namespace std;

// Codes couleurs
const string RESET = "\033[0m";
const string ROUGE = "\033[31m";
const string VERT = "\033[32m";
const string JAUNE = "\033[33m";
const string BLEU = "\033[34m";
const string MAGENTA = "\033[35m";
const string CYAN = "\033[36m";
const string BLANC = "\033[37m";
const string GRAS = "\033[1m";

void afficherMenu() {
    cout << "\n" << GRAS << MAGENTA << "=== Menu Principal ===" << RESET << endl;
    cout << CYAN << "1. Afficher mes Pokémons" << RESET << endl;
    cout << CYAN << "2. Soigner mes Pokémons" << RESET << endl;
    cout << CYAN << "3. Afficher mes stats" << RESET << endl;
    cout << VERT << "4. Défier un Leader" << RESET << endl;
    cout << VERT << "5. Défier un Maître" << RESET << endl;
    cout << VERT << "6. Défier un autre dresseur" << RESET << endl;
    cout << CYAN << "7. Interagir avec les Pokémons/Entraîneurs" << RESET << endl;
    cout << CYAN << "8. Gérer l'ordre des Pokémons" << RESET << endl;
    cout << CYAN << "9. Changer de dresseur" << RESET << endl;
    cout << ROUGE << "0. Quitter" << RESET << endl;
    cout << GRAS << "Votre choix : " << RESET;
}

void viderBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int lireChoix() {
    int choix;
    while (!(cin >> choix)) {
        cout << "Entrée invalide. Veuillez entrer un nombre." << endl;
        viderBuffer();
        cout << "Votre choix : ";
    }
    viderBuffer();
    return choix;
}

void pause(int milliseconds = 1000) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

bool combat(Joueur* joueur, Entraineur* adversaire) {
    cout << "\n=== PRÉPARATION DU COMBAT ===" << endl;
    
   
    bool estMaitre = adversaire->estMaitre();
    
    
    int nombrePokemonCombat = estMaitre ? 6 : 3;
    
    
    const auto& tousLesPokemonJoueur = joueur->getPokemons();
    for (size_t i = 0; i < nombrePokemonCombat && i < tousLesPokemonJoueur.size(); ++i) {
        if (tousLesPokemonJoueur[i]->getCurrentHp() <= 0) {
            cout << "Attention ! " << tousLesPokemonJoueur[i]->getNom() 
                     << " est K.O. et ne peut pas combattre !" << endl;
            cout << "Veuillez soigner vos Pokémon ou changer leur ordre avant le combat." << endl;
            return false;
        }
    }
    

    if (estMaitre && tousLesPokemonJoueur.size() < 6) {
        cout << "Pour défier un Maître, vous devez avoir au moins 6 Pokémon dans votre équipe !" << endl;
        cout << "Vous n'avez que " << tousLesPokemonJoueur.size() << " Pokémon." << endl;
        return false;
    }
    
    //Pokémons du joueur
    vector<Pokemon*> equipeJoueur;
    for (size_t i = 0; i < nombrePokemonCombat && i < tousLesPokemonJoueur.size(); ++i) {
        equipeJoueur.push_back(tousLesPokemonJoueur[i]);
    }
    
    //Pokémons de l'adversaire
    vector<Pokemon*> equipeAdversaire;
    const vector<Pokemon*>& tousLesPokemonAdversaire = adversaire->getPokemons();
    for (size_t i = 0; i < nombrePokemonCombat && i < tousLesPokemonAdversaire.size(); ++i) {
        equipeAdversaire.push_back(tousLesPokemonAdversaire[i]);
    }

    Combat::EquipeCombat equipeJ(joueur->getNom(), equipeJoueur);
    Combat::EquipeCombat equipeA(adversaire->getNom(), equipeAdversaire);

    cout << "\n=== DÉBUT DU COMBAT ===" << endl;
    cout << equipeJ.getNom() << " VS " << equipeA.getNom() << endl;
    
  
    cout << "\nÉquipe de " << equipeJ.getNom() << " :" << endl;
    for (const auto& pokemon : equipeJ.getPokemons()) {
        cout << "- " << pokemon->getNom() 
                  << " (PV: " << pokemon->getCurrentHp() << "/" << pokemon->getMaxHp() << ")" 
                  << endl;
    }
    
    cout << "\nÉquipe de " << equipeA.getNom() << " :" << endl;
    for (const auto& pokemon : equipeA.getPokemons()) {
        cout << "- " << pokemon->getNom() 
                  << " (PV: " << pokemon->getCurrentHp() << "/" << pokemon->getMaxHp() << ")" 
                  << endl;
    }
    pause(2000);

    while (true) {
        Pokemon* pokemonJoueur = equipeJ.getPokemonActif();
        Pokemon* pokemonAdversaire = equipeA.getPokemonActif();
        
        if (!pokemonJoueur) {
            cout << equipeJ.getNom() << " n'a plus de Pokémon en état de combattre !" << endl;
            return false;
        }
        if (!pokemonAdversaire) {
            cout << equipeA.getNom() << " n'a plus de Pokémon en état de combattre !" << endl;
            return true;
        }

       
        cout << "\n=== TOUR DE COMBAT ===" << endl;
        cout << equipeJ.getNom() << " : ";
        Combat::afficherEtatPokemon(*pokemonJoueur);
        cout << equipeA.getNom() << " : ";
        Combat::afficherEtatPokemon(*pokemonAdversaire);
        pause();

        // Attaque du joueur
        Combat::afficherAttaque(pokemonJoueur->getNom(), pokemonJoueur->getNomAttaque(), pokemonJoueur->getDegatsAttaque(), false);
        auto resultat = Combat::calculerDegats(*pokemonJoueur, *pokemonAdversaire, false);
        Combat::afficherResultat(resultat, *pokemonAdversaire);
        pause();

        if (pokemonAdversaire->estKO()) {
            cout << pokemonAdversaire->getNom() << " est K.O. !" << endl;
            equipeA.pokemonKO();
            pause();
            continue;
        }

        // Attaque de l'adversaire 
        Combat::afficherAttaque(pokemonAdversaire->getNom(), pokemonAdversaire->getNomAttaque(), pokemonAdversaire->getDegatsAttaque(), estMaitre);
        resultat = Combat::calculerDegats(*pokemonAdversaire, *pokemonJoueur, estMaitre);
        Combat::afficherResultat(resultat, *pokemonJoueur);
        pause();

        if (pokemonJoueur->estKO()) {
            cout << pokemonJoueur->getNom() << " est K.O. !" << endl;
            equipeJ.pokemonKO();
            pause();
        }
    }
}

void afficherDetailsPokemon(const Pokemon* pokemon) {
    cout << "\n" << GRAS << BLEU << "=== Détails de " << pokemon->getNom() << " ===" << RESET << endl;
    cout << JAUNE << "Types : " << RESET;
    for (const auto& type : pokemon->getTypes()) {
        switch(type) {
            case Type::NORMAL: cout << BLANC << "Normal" << RESET; break;
            case Type::FEU: cout << ROUGE << "Feu" << RESET; break;
            case Type::EAU: cout << BLEU << "Eau" << RESET; break;
            case Type::PLANTE: cout << VERT << "Plante" << RESET; break;
            case Type::ELECTRIK: cout << JAUNE << "Electrik" << RESET; break;
            case Type::GLACE: cout << CYAN << "Glace" << RESET; break;
            case Type::COMBAT: cout << ROUGE << "Combat" << RESET; break;
            case Type::POISON: cout << MAGENTA << "Poison" << RESET; break;
            case Type::SOL: cout << JAUNE << "Sol" << RESET; break;
            case Type::VOL: cout << CYAN << "Vol" << RESET; break;
            case Type::PSY: cout << MAGENTA << "Psy" << RESET; break;
            case Type::INSECTE: cout << VERT << "Insecte" << RESET; break;
            case Type::ROCHE: cout << JAUNE << "Roche" << RESET; break;
            case Type::SPECTRE: cout << MAGENTA << "Spectre" << RESET; break;
            case Type::DRAGON: cout << BLEU << "Dragon" << RESET; break;
            case Type::ACIER: cout << BLANC << "Acier" << RESET; break;
            case Type::TENEBRES: cout << MAGENTA << "Ténèbres" << RESET; break;
            case Type::FEE: cout << MAGENTA << "Fée" << RESET; break;
        }
        cout << " ";
    }
    cout << endl;
    
    // Barre de vie
    cout << CYAN << "Points de vie : " << RESET;
    int pourcentageVie = (pokemon->getCurrentHp() * 100) / pokemon->getMaxHp();
    string couleurVie = ROUGE;  
    
    if (pourcentageVie > 70) {
        couleurVie = VERT;  
    } else if (pourcentageVie > 30) {
        couleurVie = JAUNE;  
    }
    
    cout << couleurVie << pokemon->getCurrentHp() << RESET << "/" << GRAS << pokemon->getMaxHp() << RESET << " ";
    
    // Barre de vie
    cout << "[";
    int barresPleines = (pourcentageVie * 20) / 100;
    for (int i = 0; i < 20; i++) {
        if (i < barresPleines) {
            cout << couleurVie << "█" << RESET;
        } else {
            cout << "░";
        }
    }
    cout << "]" << endl;
    
    cout << MAGENTA << "Attaque : " << GRAS << pokemon->getNomAttaque() << RESET 
              << " (" << ROUGE << "Dégâts : " << GRAS << pokemon->getDegatsAttaque() << RESET << ")" << endl;
}

void afficherEquipePokemonInteractif(const Entraineur* entraineur) {
    bool continuer = true;
    while (continuer) {
        const auto& pokemons = entraineur->getPokemons();
        cout << "\nÉquipe de " << entraineur->getNom() << " :" << endl;
        for (size_t i = 0; i < pokemons.size(); ++i) {
            cout << i + 1 << ". " << pokemons[i]->getNom() 
                  << " (PV: " << pokemons[i]->getCurrentHp() << "/" << pokemons[i]->getMaxHp() << ")" 
                  << endl;
        }
        
        cout << "\nChoisissez un Pokémon pour voir ses détails (1-" << pokemons.size() << ") ou 0 pour revenir : ";
        int choix = lireChoix();
        
        if (choix == 0) {
            continuer = false;
        } else if (choix > 0 && choix <= static_cast<int>(pokemons.size())) {
            afficherDetailsPokemon(pokemons[choix - 1]);
            cout << "\nAppuyez sur Entrée pour revenir à la liste...";
            cin.get();
        } else {
            cout << "Choix invalide !" << endl;
            pause();
        }
    }
}

void soignerPokemonSpecifique(Joueur* joueur) {
    const auto& pokemons = joueur->getPokemons();
    
    bool pokemonBlesses = false;
    for (const auto& pokemon : pokemons) {
        if (pokemon->getCurrentHp() < pokemon->getMaxHp()) {
            pokemonBlesses = true;
            break;
        }
    }
    
    if (!pokemonBlesses) {
        cout << "Tous vos Pokémon sont en pleine forme !" << endl;
        pause();
        return;
    }


    cout << "\nVos Pokémon blessés :" << endl;
    int compteurBlesses = 0;
    for (size_t i = 0; i < pokemons.size(); ++i) {
        if (pokemons[i]->getCurrentHp() < pokemons[i]->getMaxHp()) {
            cout << i + 1 << ". " << pokemons[i]->getNom() 
                  << " (PV: " << pokemons[i]->getCurrentHp() << "/" << pokemons[i]->getMaxHp() << ")" 
                  << endl;
            compteurBlesses++;
        }
    }
    
    cout << "\nQue souhaitez-vous faire ?" << endl;
    cout << "1. Soigner un Pokémon spécifique" << endl;
    cout << "2. Soigner tous les Pokémon" << endl;
    cout << "0. Annuler" << endl;
    cout << "\nVotre choix : ";
    
    int choixAction = lireChoix();
    
    if (choixAction == 1) {
        
        cout << "\nChoisissez un Pokémon à soigner (1-" << pokemons.size() << ") ou 0 pour annuler : ";
        int choix = lireChoix();
        
        if (choix > 0 && choix <= static_cast<int>(pokemons.size())) {
            Pokemon* pokemon = pokemons[choix - 1];
            if (pokemon->getCurrentHp() < pokemon->getMaxHp()) {
                pokemon->soigner();
                cout << pokemon->getNom() << " a été soigné !" << endl;
            } else {
                cout << pokemon->getNom() << " est déjà en pleine forme !" << endl;
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
            cout << "Tous vos Pokémon blessés (" << nombrePokemonsSoignes << ") ont été soignés !" << endl;
        } else {
            cout << "Aucun Pokémon n'avait besoin de soins !" << endl;
        }
    }
    
    pause();
}

void interagirAvecPokemonEtEntraineurs(Joueur* joueur, const Maitre* maitreCourant, const vector<Leader*>& leaders, const vector<Maitre*>& maitres) {
    bool continuerInteraction = true;
    
    while (continuerInteraction) {
        cout << "\n" << GRAS << MAGENTA << "=== Menu d'Interaction ===" << RESET << endl;
        cout << "1. Interagir avec mes Pokémon" << endl;
        cout << "2. Interagir avec les Leaders vaincus" << endl;
        
        // Vérifier si au moins un maître a été vaincu
        bool maitreVaincu = false;
        for (const Maitre* maitre : maitres) {
            if (maitre->aEteVaincu()) {
                maitreVaincu = true;
                break;
            }
        }
        
        if (maitreVaincu) {
            cout << "3. Interagir avec les Maîtres vaincus" << endl;
        }
        
        cout << "0. Retour" << endl;
        cout << "Votre choix : ";

        int choix = lireChoix();
        switch (choix) {
            case 0:
                continuerInteraction = false;
                break;
                
            case 1: {
                const auto& pokemons = joueur->getPokemons();
                cout << "\nChoisissez un Pokémon pour interagir (1-" << pokemons.size() << ") ou 0 pour revenir : " << endl;
                for (size_t i = 0; i < pokemons.size(); ++i) {
                    cout << i + 1 << ". " << pokemons[i]->getNom() << endl;
                }
                
                int choixPokemon = lireChoix();
                if (choixPokemon > 0 && choixPokemon <= static_cast<int>(pokemons.size())) {
                    cout << pokemons[choixPokemon - 1]->interagir() << endl;
                    pause(2000);
                }
                break;
            }
            case 2: {
                // Créer une liste des Leaders vaincus
                vector<Leader*> leadersVaincus;
                for (Leader* leader : leaders) {
                    if (leader->aEteVaincu()) {
                        leadersVaincus.push_back(leader);
                    }
                }
                
                if (leadersVaincus.empty()) {
                    cout << "Vous n'avez pas encore vaincu de Leader." << endl;
                    pause();
                } else {
                    bool choisirLeader = true;
                    while (choisirLeader) {
                        cout << "\nChoisissez un Leader vaincu (0 pour revenir) :" << endl;
                        for (size_t i = 0; i < leadersVaincus.size(); ++i) {
                            cout << i+1 << ". " << leadersVaincus[i]->getNom() << endl;
                        }
                        
                        int choixLeader = lireChoix();
                        if (choixLeader == 0) {
                            choisirLeader = false;
                        } else if (choixLeader >= 1 && choixLeader <= static_cast<int>(leadersVaincus.size())) {
                            Leader* leader = leadersVaincus[choixLeader-1];
                            cout << leader->interagir() << endl;
                            pause();
                            choisirLeader = false;
                        } else {
                            cout << "Choix invalide !" << endl;
                            pause();
                        }
                    }
                }
                break;
            }
            case 3: {
                // Créer une liste des Maîtres vaincus
                vector<Maitre*> maitresVaincus;
                for (Maitre* maitre : maitres) {
                    if (maitre->aEteVaincu()) {
                        maitresVaincus.push_back(maitre);
                    }
                }
                
                if (maitresVaincus.empty()) {
                    cout << "Vous n'avez pas encore vaincu de Maître." << endl;
                    pause();
                } else {
                    bool choisirMaitre = true;
                    while (choisirMaitre) {
                        cout << "\nChoisissez un Maître vaincu pour interagir (0 pour revenir) :" << endl;
                        for (size_t i = 0; i < maitresVaincus.size(); ++i) {
                            cout << i+1 << ". " << maitresVaincus[i]->getNom() << ", " << maitresVaincus[i]->getTitre() << endl;
                        }
                        
                        int choixMaitre = lireChoix();
                        if (choixMaitre == 0) {
                            choisirMaitre = false;
                        } else if (choixMaitre >= 1 && choixMaitre <= static_cast<int>(maitresVaincus.size())) {
                            Maitre* maitre = maitresVaincus[choixMaitre-1];
                            cout << maitre->interagir() << endl;
                            pause();
                            choisirMaitre = false;
                        } else {
                            cout << "Choix invalide !" << endl;
                            pause();
                        }
                    }
                }
                break;
            }
            default:
                cout << "Choix invalide ! Veuillez choisir entre 0 et 9." << endl;
                pause();
                break;
        }
    }
}

void gererOrdrePokemon(Joueur* joueur) {
    bool continuer = true;
    while (continuer) {
        cout << "\n" << GRAS << MAGENTA << "=== Gestion de l'ordre des Pokémon ===" << RESET << endl;
        cout << "Ordre actuel de vos Pokémon :" << endl;
        const auto& pokemons = joueur->getPokemons();
        for (size_t i = 0; i < pokemons.size(); ++i) {
            cout << i+1 << ". " << pokemons[i]->getNom() 
                     << " (PV: " << pokemons[i]->getCurrentHp() << "/" << pokemons[i]->getMaxHp() << ")"
                     << (i < 3 ? " [Combat]" : "") << endl;
        }
        
        cout << "\nQue souhaitez-vous faire ?" << endl;
        cout << "1. Échanger deux Pokémon" << endl;
        cout << "0. Retour" << endl;
        cout << "Votre choix : ";
        
        int choix = lireChoix();
        if (choix == 0) {
            continuer = false;
        } else if (choix == 1) {
            cout << "Choisissez le premier Pokémon (1-" << pokemons.size() << ") : ";
            int pos1 = lireChoix() - 1;
            cout << "Choisissez le deuxième Pokémon (1-" << pokemons.size() << ") : ";
            int pos2 = lireChoix() - 1;
            
            if (pos1 >= 0 && pos1 < static_cast<int>(pokemons.size()) &&
                pos2 >= 0 && pos2 < static_cast<int>(pokemons.size()) &&
                pos1 != pos2) {
                joueur->echangerPokemon(pos1, pos2);
                cout << "Pokémon échangés avec succès !" << endl;
            } else {
                cout << "Positions invalides ! Veuillez choisir des numéros entre 1 et " 
                         << pokemons.size() << "." << endl;
            }
            pause();
        } else {
            cout << "Choix invalide ! Veuillez choisir 1 pour échanger ou 0 pour revenir." << endl;
            pause();
        }
    }
}

void defierAutreDresseur(Joueur* joueurPrincipal, const string& fichierJoueurs) {
    // Charger tous les joueurs du fichier
    vector<Joueur*> autreDresseurs;
    try {
        autreDresseurs = DataLoader::chargerTousJoueurs(fichierJoueurs);
    } catch (const exception& e) {
        cerr << "Erreur lors du chargement des dresseurs : " << e.what() << endl;
        pause();
        return;
    }
    
    // Filtrer pour ne pas inclure le joueur principal
    vector<Joueur*> dresseursFiltres;
    for (Joueur* dresseur : autreDresseurs) {
        if (dresseur->getNom() != joueurPrincipal->getNom()) {
            dresseursFiltres.push_back(dresseur);
        } else {
            delete dresseur;
        }
    }
    
    if (dresseursFiltres.empty()) {
        cout << "Aucun autre dresseur disponible pour le combat." << endl;
        pause();
        return;
    }
    
    // Afficher la liste des dresseurs disponibles
    bool continuer = true;
    while (continuer) {
        cout << "\n" << GRAS << MAGENTA << "=== Dresseurs disponibles ===" << RESET << endl;
        for (size_t i = 0; i < dresseursFiltres.size(); ++i) {
            cout << i + 1 << ". " << dresseursFiltres[i]->getNom() << endl;
        }
        cout << "0. Retour" << endl;
        cout << "Choisissez un dresseur à défier : ";
        
        int choix = lireChoix();
        if (choix == 0) {
            continuer = false;
        } else if (choix > 0 && choix <= static_cast<int>(dresseursFiltres.size())) {
            Joueur* adversaire = dresseursFiltres[choix - 1];
            
            cout << "\nVous allez affronter " << adversaire->getNom() << " !" << endl;
            pause();
            
            bool resultatCombat = combat(joueurPrincipal, adversaire);
            if (resultatCombat) {
                cout << "Félicitations ! Vous avez battu " << adversaire->getNom() << " !" << endl;
                joueurPrincipal->gagnerCombat();
                if (!adversaire->aEteVaincu()) {
                    adversaire->setVaincu(true);
                    // Donner le badge au joueur (appelle déjà gagnerBadge)
                    adversaire->donnerBadge(*joueurPrincipal);
                }
            } else if (resultatCombat == false) {
                // Si le combat n'a pas pu commencer à cause d'un Pokémon K.O.
                // Aucune action supplémentaire requise, le message est déjà affiché
                // dans la fonction combat
            } else {
                cout << "Vous avez perdu contre " << adversaire->getNom() << "." << endl;
                joueurPrincipal->perdreCombat();
                // Sauvegarder les statistiques après la défaite
                DataLoader::sauvegarderStatistiques("data/statistiques.csv", joueurPrincipal);
                // Soigner l'équipe du Leader après le combat
                adversaire->soignerEquipe();
            }
            
            continuer = false;
        } else {
            cout << "Choix invalide !" << endl;
            pause();
        }
    }
    
    // Nettoyer la mémoire
    for (Joueur* dresseur : dresseursFiltres) {
        delete dresseur;
    }
    dresseursFiltres.clear();
}

Joueur* changerDresseur(Joueur* joueurActuel, const string& fichierJoueurs) {
    // Charger tous les joueurs du fichier
    vector<Joueur*> tousJoueurs;
    try {
        tousJoueurs = DataLoader::chargerTousJoueurs(fichierJoueurs);
    } catch (const exception& e) {
        cerr << "Erreur lors du chargement des dresseurs : " << e.what() << endl;
        pause();
        return joueurActuel;
    }
    
    if (tousJoueurs.empty()) {
        cout << "Aucun dresseur disponible." << endl;
        pause();
        return joueurActuel;
    }
    
    // Afficher la liste des dresseurs disponibles
    cout << "\n" << GRAS << MAGENTA << "=== Sélection du dresseur ===" << RESET << endl;
    cout << "Dresseur actuel : " << joueurActuel->getNom() << endl;
    cout << "\nDresseurs disponibles :" << endl;
    
    for (size_t i = 0; i < tousJoueurs.size(); ++i) {
        string marqueur = (tousJoueurs[i]->getNom() == joueurActuel->getNom()) ? " [Actuel]" : "";
        cout << i + 1 << ". " << tousJoueurs[i]->getNom() << marqueur << endl;
    }
    
    cout << "0. Annuler" << endl;
    cout << "Choisissez un dresseur : ";
    
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
            cout << "Vous jouez maintenant avec " << nouveauJoueur->getNom() << " !" << endl;
            
            // Charger les statistiques du nouveau joueur
            DataLoader::chargerStatistiques("data/statistiques.csv", nouveauJoueur);
            
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
            cout << "Vous jouez déjà avec " << joueurActuel->getNom() << "." << endl;
            
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
        cout << "Choix invalide !" << endl;
        
        // Libérer la mémoire des autres joueurs
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
    vector<Leader*> leaders;
    vector<Maitre*> maitres;
    Maitre* maitreCourant = nullptr;

    try {
        cout << "Chargement des Pokémons.." << endl;
        // Charger d'abord tous les Pokémon disponibles
        DataLoader::chargerPokemons("data/pokemon.csv");
        
        // Charger les joueurs disponibles
        cout << "Chargement des joueurs..." << endl;
        vector<Joueur*> joueursDisponibles = DataLoader::chargerTousJoueurs("data/joueur.csv");
        
        // Permettre à l'utilisateur de choisir un joueur
        cout << "\n" << GRAS << MAGENTA << "=== Sélection du dresseur ===" << RESET << endl;
        cout << "Choisissez votre dresseur :" << endl;
        
        for (size_t i = 0; i < joueursDisponibles.size(); ++i) {
            cout << i + 1 << ". " << joueursDisponibles[i]->getNom() << endl;
        }
        
        cout << "Votre choix : ";
        int choixJoueur = lireChoix();
        
        if (choixJoueur < 1 || choixJoueur > static_cast<int>(joueursDisponibles.size())) {
            cout << "Choix invalide ! Utilisation du premier dresseur par défaut." << endl;
            choixJoueur = 1;
        }
        
        // Sélectionner le joueur choisi
        joueur = joueursDisponibles[choixJoueur - 1];
        
        // Libérer la mémoire des autres joueurs
        for (size_t i = 0; i < joueursDisponibles.size(); ++i) {
            if (i != choixJoueur - 1) {
                delete joueursDisponibles[i];
            }
        }
        
        // Charger les statistiques du joueur
        cout << "Chargement des statistiques..." << endl;
        DataLoader::chargerStatistiques("data/statistiques.csv", joueur);
        
        cout << "Chargement des Leaders..." << endl;
        // Charger les Leaders
        leaders = DataLoader::chargerLeaders("data/leaders.csv");
        
        cout << "Chargement des Maîtres..." << endl;
        // Charger les Maîtres
        maitres = DataLoader::chargerMaitres("data/maitres.csv");

        cout << "Chargement terminé !" << endl;

    } catch (const exception& e) {
        cerr << "Erreur lors du chargement des données : " << e.what() << endl;
        return 1;
    }

    bool continuer = true;
    while (continuer) {
        afficherMenu();
        int choix = lireChoix();

        switch (choix) {
            case 1: // Liste Pokémons
                afficherEquipePokemonInteractif(joueur);
                break;

            case 2: // Soigner
                soignerPokemonSpecifique(joueur);
                break;

            case 3: // Stats
                {
                bool dansStats = true;
                while (dansStats) {
                    cout << "\n" << GRAS << MAGENTA << "=== Statistiques de " << joueur->getNom() << " ===" << RESET << endl;
                    cout << "Badges : " << joueur->getBadges() << endl;
                    
                   
                    const auto& badgesGagnes = joueur->getBadgesGagnes();
                    if (!badgesGagnes.empty()) {
                        cout << "Badges gagnés : ";
                        bool premier = true;
                        for (const auto& badge : badgesGagnes) {
                            if (!premier) {
                                cout << ", ";
                            }
                            cout << badge;
                            premier = false;
                        }
                        cout << endl;
                    } else {
                        cout << "Aucun badge gagné pour l'instant" << endl;
                    }
                    
                    cout << "Victoires : " << joueur->getVictoires() << endl;
                    cout << "Défaites : " << joueur->getDefaites() << endl;
                    
                    if (joueur->getVictoires() > 0 || joueur->getDefaites() > 0) {
                        double ratio = joueur->getVictoires() / static_cast<double>(joueur->getVictoires() + joueur->getDefaites());
                        cout << "Ratio de victoires : " << static_cast<int>(ratio * 100) << "%" << endl;
                    }
                    
                    cout << "\n0. Retour au menu principal" << endl;
                    cout << "Votre choix : ";
                    
                    int choixStat = lireChoix();
                    if (choixStat == 0) {
                        dansStats = false;
                    }
                }
                break;
                }

            case 4: // Leaders
                if (leaders.empty()) {
                    cout << "Aucun Leader disponible." << endl;
                    pause();
                } else {
                    bool choisirLeader = true;
                    while (choisirLeader) {
                        cout << "\nChoisissez un Leader à défier (0 pour revenir) :" << endl;
                        for (size_t i = 0; i < leaders.size(); ++i) {
                            string statut = leaders[i]->aEteVaincu() ? " (Vaincu)" : "";
                            cout << i+1 << ". " << leaders[i]->messageDefi() << statut << endl;
                        }
                        
                        int choixLeader = lireChoix();
                        if (choixLeader == 0) {
                            choisirLeader = false;
                        } else if (choixLeader >= 1 && choixLeader <= static_cast<int>(leaders.size())) {
                            Leader* leader = leaders[choixLeader-1];
                            
                            bool resultatCombat = combat(joueur, leader);
                            if (resultatCombat) {
                                cout << "Félicitations ! Vous avez battu " << leader->getNom() << " !" << endl;
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
                                cout << "Vous avez perdu contre " << leader->getNom() << "." << endl;
                                joueur->perdreCombat();
                                // Sauvegarder les statistiques après la défaite
                                DataLoader::sauvegarderStatistiques("data/statistiques.csv", joueur);
                                // Soigner l'équipe du Leader après le combat
                                leader->soignerEquipe();
                            }
                            choisirLeader = false;
                        } else {
                            cout << "Choix invalide !" << endl;
                            pause();
                        }
                    }
                }
                break;

            case 5: // Maitres
                if (joueur->getBadges() < leaders.size()) {
                    cout << "Vous devez avoir vaincu tous les Leaders (" 
                             << leaders.size() << " badges) pour défier un Maître !" << endl;
                    pause();
                } else if (maitres.empty()) {
                    cout << "Aucun Maître disponible." << endl;
                    pause();
                } else {
                    // Sélectionner un Maître aléatoirement à chaque défi
                    // Initialiser le générateur avec une nouvelle seed à chaque appel
                    random_device rd;  // Seed plus robuste qu'un time(nullptr)
                    mt19937 gen(rd());
                    uniform_int_distribution<> dist(0, maitres.size() - 1);
                    int indexMaitre = dist(gen);
                    maitreCourant = maitres[indexMaitre];
                    
                    cout << "Vous allez défier " << maitreCourant->getNom() << ", " 
                             << maitreCourant->getTitre() << " !" << endl;
                    cout << "\nATTENTION : Les Maîtres bénéficient d'un bonus de dégâts de " 
                             << static_cast<int>((Combat::BONUS_DEGATS_MAITRE - 1.0) * 100) << "% !" << endl;
                    cout << "De plus, ce combat se fera avec 6 Pokémon contre 6 !" << endl;
                    pause();
                    
                    bool resultatCombat = combat(joueur, maitreCourant);
                    if (resultatCombat) {
                        cout << "Félicitations ! Vous avez battu " << maitreCourant->getNom() 
                                 << ", le " << maitreCourant->getTitre() << " !" << endl;
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
                        cout << "Vous avez perdu contre " << maitreCourant->getNom() << "." << endl;
                        joueur->perdreCombat();
                        // Sauvegarder les statistiques après la défaite
                        DataLoader::sauvegarderStatistiques("data/statistiques.csv", joueur);
                    }
                    
                    // Soigner l'équipe du Maître après le combat
                    maitreCourant->soignerEquipe();
                }
                break;

            case 6: // Dresseurs
                defierAutreDresseur(joueur, "data/joueur.csv");
                break;

            case 7: // Interagir
                interagirAvecPokemonEtEntraineurs(joueur, maitreCourant, leaders, maitres);
                break;

            case 8: // Gérer l'ordre des Pokémons
                gererOrdrePokemon(joueur);
                break;

            case 9: // Changer de dresseurs
                joueur = changerDresseur(joueur, "data/joueur.csv");
                break;

            case 0: // Quitter
                cout << "Sauvegarde des statistiques..." << endl;
                DataLoader::sauvegarderStatistiques("data/statistiques.csv", joueur);
                cout << "Au revoir !" << endl;
                continuer = false;
                break;

            default:
                cout << "Choix invalide ! Veuillez choisir entre 0 et 9." << endl;
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