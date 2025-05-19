#include "Pokemon.hpp"
#include <algorithm>
#include <random>
#include <iostream>
#include <cctype>
#include <map>
#include <string>

// Initialisation des variables statiques
std::random_device Pokemon::rd;
std::mt19937 Pokemon::gen(rd());
std::uniform_real_distribution<> Pokemon::dis(0.0, 1.0);
std::map<Type, std::map<Type, double>> Pokemon::tableEfficacite;

// Fonction utilitaire pour convertir une chaîne en minuscules et sans accents
std::string toLowerNoAccent(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    static const std::map<std::string, char> accentMap = {
        {"é", 'e'}, {"è", 'e'}, {"ê", 'e'}, {"ë", 'e'},
        {"à", 'a'}, {"â", 'a'}, {"ä", 'a'},
        {"î", 'i'}, {"ï", 'i'},
        {"ô", 'o'}, {"ö", 'o'},
        {"ù", 'u'}, {"û", 'u'}, {"ü", 'u'},
        {"ÿ", 'y'}
    };
    
    std::string result;
    for (size_t i = 0; i < str.length();) {
        bool found = false;
        for (const auto& [accent, replacement] : accentMap) {
            if (str.compare(i, accent.length(), accent) == 0) {
                result += replacement;
                i += accent.length();
                found = true;
                break;
            }
        }
        if (!found) {
            result += str[i];
            ++i;
        }
    }
    return result;
}

Type Pokemon::stringToType(const std::string& typeStr) {
    std::string normalizedType = toLowerNoAccent(typeStr);
    
    if (normalizedType == "normal") return Type::NORMAL;
    if (normalizedType == "feu") return Type::FEU;
    if (normalizedType == "eau") return Type::EAU;
    if (normalizedType == "plante") return Type::PLANTE;
    if (normalizedType == "electrik") return Type::ELECTRIK;
    if (normalizedType == "glace") return Type::GLACE;
    if (normalizedType == "combat") return Type::COMBAT;
    if (normalizedType == "poison") return Type::POISON;
    if (normalizedType == "sol") return Type::SOL;
    if (normalizedType == "vol") return Type::VOL;
    if (normalizedType == "psy") return Type::PSY;
    if (normalizedType == "insecte") return Type::INSECTE;
    if (normalizedType == "roche") return Type::ROCHE;
    if (normalizedType == "spectre") return Type::SPECTRE;
    if (normalizedType == "dragon") return Type::DRAGON;
    if (normalizedType == "acier") return Type::ACIER;
    if (normalizedType == "fee") return Type::FEE;
    
    throw std::invalid_argument("Type inconnu: " + typeStr);
}

void Pokemon::initTableEfficacite() {
    if (!tableEfficacite.empty()) return;

    // Initialisation des efficacités normales (1.0) par défaut
    for (int i = 0; i <= static_cast<int>(Type::FEE); ++i) {
        for (int j = 0; j <= static_cast<int>(Type::FEE); ++j) {
            tableEfficacite[static_cast<Type>(i)][static_cast<Type>(j)] = 1.0;
        }
    }

    // Définition des efficacités spéciales
    // Feu
    tableEfficacite[Type::FEU][Type::PLANTE] = 2.0;
    tableEfficacite[Type::FEU][Type::ACIER] = 2.0;
    tableEfficacite[Type::FEU][Type::EAU] = 0.5;
    tableEfficacite[Type::FEU][Type::ROCHE] = 0.5;

    // Eau
    tableEfficacite[Type::EAU][Type::FEU] = 2.0;
    tableEfficacite[Type::EAU][Type::ROCHE] = 2.0;
    tableEfficacite[Type::EAU][Type::PLANTE] = 0.5;
    tableEfficacite[Type::EAU][Type::DRAGON] = 0.5;

    // Plante
    tableEfficacite[Type::PLANTE][Type::EAU] = 2.0;
    tableEfficacite[Type::PLANTE][Type::ROCHE] = 2.0;
    tableEfficacite[Type::PLANTE][Type::FEU] = 0.5;
    tableEfficacite[Type::PLANTE][Type::INSECTE] = 0.5;

    // Électrik
    tableEfficacite[Type::ELECTRIK][Type::EAU] = 2.0;
    tableEfficacite[Type::ELECTRIK][Type::VOL] = 2.0;
    tableEfficacite[Type::ELECTRIK][Type::SOL] = 0.0;
    tableEfficacite[Type::ELECTRIK][Type::PLANTE] = 0.5;
}

Pokemon::Pokemon(const std::string& nom, const std::vector<Type>& types, int hp, 
                const std::string& nomAttaque, int degatsAttaque)
    : nom(nom), types(types), maxHp(hp), currentHp(hp), 
      nomAttaque(nomAttaque), degatsAttaque(degatsAttaque) {
    initTableEfficacite();
}

double Pokemon::getEfficaciteType(Type typeAttaque) const {
    double efficacite = 1.0;
    // Pour chaque type du Pokémon défenseur
    for (Type typeDefenseur : types) {
        efficacite *= tableEfficacite[typeAttaque][typeDefenseur];
    }
    return efficacite;
}

bool Pokemon::estCoupCritique(double efficacite) const {
    double chance = getChanceCritique(efficacite);
    return dis(gen) < chance;
}

double Pokemon::getChanceCritique(double efficacite) const {
    double chanceCritique = CHANCE_CRITIQUE_BASE;
    
    if (efficacite > 1.0) {
        chanceCritique *= (1.0 + (efficacite - 1.0) * 0.5);
    } else if (efficacite < 1.0) {
        chanceCritique *= efficacite;
    }
    
    return std::min(0.20, std::max(0.05, chanceCritique));
}

std::string Pokemon::interagir() const {

    static std::map<std::string, std::string> messagesInteraction = {
        {"Bulbizarre", "Bulbizarre agite son bulbe avec enthousiasme !"},
        {"Herbizarre", "Herbizarre fait briller ses feuilles au soleil."},
        {"Florizarre", "Florizarre libère une agréable senteur de fleur."},
        {"Salamèche", "Salamèche agite sa queue et la flamme s'intensifie !"},
        {"Reptincel", "Reptincel gratte le sol avec ses griffes acérées."},
        {"Dracaufeu", "Dracaufeu rugit et fait trembler le sol !"},
        {"Carapuce", "Carapuce rentre dans sa carapace puis ressort avec un grand sourire."},
        {"Carabaffe", "Carabaffe fait un tour sur lui-même et ses oreilles brillent."},
        {"Tortank", "Tortank pointe ses canons à eau et les fait pivoter avec précision."},
        {"Pikachu", "Pikachu fait des étincelles sur ses joues et semble très joyeux !"},
        {"Raichu", "Raichu agite sa longue queue en forme d'éclair et fait des étincelles."},
        {"Racaillou", "Racaillou roule sur lui-même et fait trembler le sol."},
        {"Onix", "Onix rugit et se dresse de toute sa hauteur impressionnante."},
        {"Grolem", "Grolem frappe ses poings l'un contre l'autre, créant une petite onde de choc."},
        {"Voltorbe", "Voltorbe tourne rapidement sur lui-même avec excitation."},
        {"Electrode", "Electrode vibre dangereusement et semble sur le point d'exploser !"},
        {"Stari", "Stari fait tournoyer son joyau central qui s'illumine de mille feux."},
        {"Staross", "Staross déploie ses branches et son joyau central brille intensément."},
        {"Hypocean", "Hypocean crache un jet d'eau en l'air puis l'attrape avec grâce."}
    };


    auto it = messagesInteraction.find(nom);
    if (it != messagesInteraction.end()) {
        return it->second;
    }
    
    // Message par défaut si l'espèce n'a pas de message spécifique
    return nom + " vous regarde avec curiosité !";
} 