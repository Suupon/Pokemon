#include "Pokemon.hpp"
#include <algorithm>
#include <random>
#include <iostream>
#include <cctype>
#include <map>
#include <string>
using namespace std;


random_device Pokemon::rd;
mt19937 Pokemon::gen(rd());
uniform_real_distribution<> Pokemon::dis(0.0, 1.0);
map<Type, map<Type, double>> Pokemon::tableEfficacite;


string toLowerNoAccent(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    static const map<string, char> accentMap = {
        {"é", 'e'}, {"è", 'e'}, {"ê", 'e'}, {"ë", 'e'},
        {"à", 'a'}, {"â", 'a'}, {"ä", 'a'},
        {"î", 'i'}, {"ï", 'i'},
        {"ô", 'o'}, {"ö", 'o'},
        {"ù", 'u'}, {"û", 'u'}, {"ü", 'u'},
        {"ÿ", 'y'}
    };
    
    string result;
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

Type Pokemon::stringToType(const string& typeStr) {
    string normalizedType = toLowerNoAccent(typeStr);
    
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
    if (normalizedType == "tenebres") return Type::TENEBRES;
    if (normalizedType == "fee") return Type::FEE;
    
    throw invalid_argument("Type inconnu: " + typeStr);
}

void Pokemon::initTableEfficacite() {
    if (!tableEfficacite.empty()) return;

    // Efficacités par défaut
    for (int i = 0; i <= static_cast<int>(Type::FEE); ++i) {
        for (int j = 0; j <= static_cast<int>(Type::FEE); ++j) {
            tableEfficacite[static_cast<Type>(i)][static_cast<Type>(j)] = 1.0;
        }
    }

    // Définition des efficacités
    // Acier
    tableEfficacite[Type::NORMAL][Type::ACIER] = 0.5;
    tableEfficacite[Type::FEU][Type::ACIER] = 2.0;
    tableEfficacite[Type::EAU][Type::ACIER] = 0.5;
    tableEfficacite[Type::PLANTE][Type::ACIER] = 0.5;
    tableEfficacite[Type::ELECTRIK][Type::ACIER] = 0.5;
    tableEfficacite[Type::GLACE][Type::ACIER] = 0.5;
    tableEfficacite[Type::COMBAT][Type::ACIER] = 2.0;
    tableEfficacite[Type::POISON][Type::ACIER] = 0.0;
    tableEfficacite[Type::SOL][Type::ACIER] = 2.0;
    tableEfficacite[Type::VOL][Type::ACIER] = 0.5;
    tableEfficacite[Type::PSY][Type::ACIER] = 0.5;
    tableEfficacite[Type::INSECTE][Type::ACIER] = 0.5;
    tableEfficacite[Type::ROCHE][Type::ACIER] = 0.5;
    tableEfficacite[Type::DRAGON][Type::ACIER] = 0.5;
    tableEfficacite[Type::FEE][Type::ACIER] = 0.5;

    // Combat
    tableEfficacite[Type::NORMAL][Type::COMBAT] = 1.0;
    tableEfficacite[Type::FEU][Type::COMBAT] = 1.0;
    tableEfficacite[Type::EAU][Type::COMBAT] = 1.0;
    tableEfficacite[Type::PLANTE][Type::COMBAT] = 1.0;
    tableEfficacite[Type::ELECTRIK][Type::COMBAT] = 1.0;
    tableEfficacite[Type::GLACE][Type::COMBAT] = 2.0;
    tableEfficacite[Type::COMBAT][Type::COMBAT] = 1.0;
    tableEfficacite[Type::POISON][Type::COMBAT] = 0.5;
    tableEfficacite[Type::SOL][Type::COMBAT] = 1.0;
    tableEfficacite[Type::VOL][Type::COMBAT] = 2.0;
    tableEfficacite[Type::PSY][Type::COMBAT] = 2.0;
    tableEfficacite[Type::INSECTE][Type::COMBAT] = 0.5;
    tableEfficacite[Type::ROCHE][Type::COMBAT] = 0.5;
    tableEfficacite[Type::SPECTRE][Type::COMBAT] = 1.0;
    tableEfficacite[Type::FEE][Type::COMBAT] = 2.0;

    // Dragon
    tableEfficacite[Type::FEU][Type::DRAGON] = 0.5;
    tableEfficacite[Type::EAU][Type::DRAGON] = 0.5;
    tableEfficacite[Type::PLANTE][Type::DRAGON] = 0.5;
    tableEfficacite[Type::ELECTRIK][Type::DRAGON] = 0.5;
    tableEfficacite[Type::GLACE][Type::DRAGON] = 2.0;
    tableEfficacite[Type::DRAGON][Type::DRAGON] = 2.0;
    tableEfficacite[Type::FEE][Type::DRAGON] = 0.0;

    // Eau
    tableEfficacite[Type::FEU][Type::EAU] = 0.5;
    tableEfficacite[Type::EAU][Type::EAU] = 0.5;
    tableEfficacite[Type::PLANTE][Type::EAU] = 2.0;
    tableEfficacite[Type::ELECTRIK][Type::EAU] = 2.0;
    tableEfficacite[Type::GLACE][Type::EAU] = 0.5;
    tableEfficacite[Type::ACIER][Type::EAU] = 0.5;

    // Electrik
    tableEfficacite[Type::ELECTRIK][Type::ELECTRIK] = 0.5;
    tableEfficacite[Type::SOL][Type::ELECTRIK] = 2.0;
    tableEfficacite[Type::VOL][Type::ELECTRIK] = 0.5;
    tableEfficacite[Type::ACIER][Type::ELECTRIK] = 0.5;

    // Fee
    tableEfficacite[Type::COMBAT][Type::FEE] = 0.5;
    tableEfficacite[Type::POISON][Type::FEE] = 2.0;
    tableEfficacite[Type::INSECTE][Type::FEE] = 0.5;
    tableEfficacite[Type::DRAGON][Type::FEE] = 0.0;
    tableEfficacite[Type::ACIER][Type::FEE] = 2.0;

    // Feu 
    tableEfficacite[Type::FEU][Type::FEU] = 0.5;
    tableEfficacite[Type::EAU][Type::FEU] = 2.0;
    tableEfficacite[Type::PLANTE][Type::FEU] = 0.5;
    tableEfficacite[Type::GLACE][Type::FEU] = 0.5;
    tableEfficacite[Type::COMBAT][Type::FEU] = 1.0;
    tableEfficacite[Type::SOL][Type::FEU] = 2.0;
    tableEfficacite[Type::INSECTE][Type::FEU] = 0.5;
    tableEfficacite[Type::ROCHE][Type::FEU] = 2.0;
    tableEfficacite[Type::ACIER][Type::FEU] = 0.5;
    tableEfficacite[Type::FEE][Type::FEU] = 0.5;

    // Glace 
    tableEfficacite[Type::FEU][Type::GLACE] = 2.0;
    tableEfficacite[Type::GLACE][Type::GLACE] = 0.5;
    tableEfficacite[Type::COMBAT][Type::GLACE] = 2.0;
    tableEfficacite[Type::ROCHE][Type::GLACE] = 2.0;
    tableEfficacite[Type::ACIER][Type::GLACE] = 2.0;

    // Insecte
    tableEfficacite[Type::FEU][Type::INSECTE] = 2.0;
    tableEfficacite[Type::PLANTE][Type::INSECTE] = 0.5;
    tableEfficacite[Type::COMBAT][Type::INSECTE] = 0.5;
    tableEfficacite[Type::SOL][Type::INSECTE] = 0.5;
    tableEfficacite[Type::VOL][Type::INSECTE] = 2.0;
    tableEfficacite[Type::ROCHE][Type::INSECTE] = 2.0;

    // Normal
    tableEfficacite[Type::COMBAT][Type::NORMAL] = 2.0;
    tableEfficacite[Type::SPECTRE][Type::NORMAL] = 0.0;

    // Plante
    tableEfficacite[Type::FEU][Type::PLANTE] = 2.0;
    tableEfficacite[Type::EAU][Type::PLANTE] = 0.5;
    tableEfficacite[Type::PLANTE][Type::PLANTE] = 0.5;
    tableEfficacite[Type::ELECTRIK][Type::PLANTE] = 0.5;
    tableEfficacite[Type::GLACE][Type::PLANTE] = 2.0;
    tableEfficacite[Type::POISON][Type::PLANTE] = 2.0;
    tableEfficacite[Type::SOL][Type::PLANTE] = 0.5;
    tableEfficacite[Type::VOL][Type::PLANTE] = 2.0;
    tableEfficacite[Type::INSECTE][Type::PLANTE] = 2.0;

    // Poison
    tableEfficacite[Type::PLANTE][Type::POISON] = 0.5;
    tableEfficacite[Type::COMBAT][Type::POISON] = 0.5;
    tableEfficacite[Type::POISON][Type::POISON] = 0.5;
    tableEfficacite[Type::SOL][Type::POISON] = 2.0;
    tableEfficacite[Type::PSY][Type::POISON] = 2.0;
    tableEfficacite[Type::INSECTE][Type::POISON] = 0.5;
    tableEfficacite[Type::FEE][Type::POISON] = 0.5;

    // Psy
    tableEfficacite[Type::COMBAT][Type::PSY] = 0.5;
    tableEfficacite[Type::PSY][Type::PSY] = 0.5;
    tableEfficacite[Type::INSECTE][Type::PSY] = 2.0;
    tableEfficacite[Type::SPECTRE][Type::PSY] = 2.0;
    tableEfficacite[Type::TENEBRES][Type::PSY] = 2.0;

    // Roche
    tableEfficacite[Type::NORMAL][Type::ROCHE] = 0.5;
    tableEfficacite[Type::FEU][Type::ROCHE] = 0.5;
    tableEfficacite[Type::EAU][Type::ROCHE] = 2.0;
    tableEfficacite[Type::PLANTE][Type::ROCHE] = 2.0;
    tableEfficacite[Type::COMBAT][Type::ROCHE] = 2.0;
    tableEfficacite[Type::POISON][Type::ROCHE] = 0.5;
    tableEfficacite[Type::SOL][Type::ROCHE] = 2.0;
    tableEfficacite[Type::VOL][Type::ROCHE] = 0.5;
    tableEfficacite[Type::ACIER][Type::ROCHE] = 2.0;

    // Sol 
    tableEfficacite[Type::EAU][Type::SOL] = 2.0;
    tableEfficacite[Type::PLANTE][Type::SOL] = 2.0;
    tableEfficacite[Type::ELECTRIK][Type::SOL] = 0.0;
    tableEfficacite[Type::GLACE][Type::SOL] = 2.0;
    tableEfficacite[Type::POISON][Type::SOL] = 0.5;
    tableEfficacite[Type::ROCHE][Type::SOL] = 0.5;

    // Spectre
    tableEfficacite[Type::NORMAL][Type::SPECTRE] = 0.0;
    tableEfficacite[Type::COMBAT][Type::SPECTRE] = 0.0;
    tableEfficacite[Type::POISON][Type::SPECTRE] = 0.5;
    tableEfficacite[Type::INSECTE][Type::SPECTRE] = 0.5;
    tableEfficacite[Type::SPECTRE][Type::SPECTRE] = 2.0;
    tableEfficacite[Type::TENEBRES][Type::SPECTRE] = 2.0;

    // Vol
    tableEfficacite[Type::PLANTE][Type::VOL] = 0.5;
    tableEfficacite[Type::ELECTRIK][Type::VOL] = 2.0;
    tableEfficacite[Type::GLACE][Type::VOL] = 2.0;
    tableEfficacite[Type::COMBAT][Type::VOL] = 0.5;
    tableEfficacite[Type::SOL][Type::VOL] = 0.0;
    tableEfficacite[Type::INSECTE][Type::VOL] = 0.5;
    tableEfficacite[Type::ROCHE][Type::VOL] = 2.0;
}

Pokemon::Pokemon(const string& nom, const vector<Type>& types, int hp, 
                const string& nomAttaque, int degatsAttaque)
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
    
    // Limiter l'efficacité maximale à 2.0 et minimale à 0.25
    if (efficacite > 2.0) {
        efficacite = 2.0;
    } else if (efficacite < 0.25 && efficacite > 0.0) {
        efficacite = 0.25;
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
    
    return min(0.20, max(0.05, chanceCritique));
}

string Pokemon::interagir() const {

    static map<string, string> messagesInteraction = {
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
    
    // Message par défaut
    return nom + " vous regarde avec curiosité !";
} 