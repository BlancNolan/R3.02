/* 
 * File:   Mondial.cpp
 * Author: hb
 * 
 * Created on 22 novembre 2018, 16:05
 */

#include "Mondial.h"


#include <iostream>     // pour cout
#include <iomanip>      // pour setw()
#include <sstream>
#include <iterator>

Mondial::Mondial(const char* filename) {
    // Chargement du fichier XML en mémoire
    imageMondial.LoadFile(filename);
    // Initialisation de l'attribut racineMondial avec la racine (élément <mondial>)
    racineMondial = imageMondial.FirstChildElement();
}

void Mondial::Print() {
    imageMondial.Print();
}

/*
 * FOURNIE
 */
int Mondial::getNbAirports() const {
    // initialisation du nombre d’aéroports
    int nb = 0;
    // accéder à <airportscategory>, c’est un fils de l'élément <racineMondial>
    XMLElement* airportsCategory = racineMondial->FirstChildElement("airportscategory");
    // parcours complet des fils de <airportscategory> en les comptants
    // 1) accéder au premier fils <airport> de <airportscategory>
    XMLElement* currentAirport = airportsCategory->FirstChildElement();
    // 2) parcourir tous les <airport> qui sont des frères
    while (currentAirport != nullptr) {
        // un aéroport supplémentaire
        nb = nb + 1;
        // avancer au frère <airport> suivant de currentAirport
        currentAirport = currentAirport->NextSiblingElement();
    }
    // currentAirport n’a plus de frère {currentAirport == nullptr}, c’est le dernier
    return nb;
}

/*
 * FOURNIE
 */
void Mondial::printCountriesCode() const {
    int rank = 1; // rang du pays
    string carcodeValue; // valeur de l'attribut "car_cod" du pays courant
    // accéder à <countriescategory>, c’est un fils de l'élément <racineMondial>)
    XMLElement* countriesCategory = racineMondial->FirstChildElement("countriescategory");
    // parcours complet des fils de <countriescategory> en affichant le rang et le code
    // 1) accéder au premier fils <country> de <countriescategory>
    XMLElement* currentCountry = countriesCategory->FirstChildElement();
    // 2) parcourir tous les <country> qui sont des frères
    while (currentCountry != nullptr) {
        // traiter le pays courant
        //      1) récupérer la valeur de l’attribut "car_code"
        carcodeValue = currentCountry->Attribute("car_code");
        //      2) faire l’affichage
        cout << setw(5) << rank << " : " << carcodeValue << endl;
        // avancer au frère <country> suivant de currentCountry
        currentCountry = currentCountry->NextSiblingElement();
        // mettre à jour le rang
        rank = rank + 1;
    }
    // currentCountry n’a pas de frère {currentCountry == nullptr}, c’est fini
}


/*
 * A COMPLETER
 */
int Mondial::getNbDeserts() const {

    //initialisaton du nombre de dessert
    int nbDessert = 0;
    // accéder à <desertscategory>, c’est un fils de l'élément <racineMondial>
    XMLElement* dessertsCategory = racineMondial->FirstChildElement("desertscategory");
    // parcours complet des fils de <desertscategory> en les comptants
    // 1) accéder au premier fils <dessert> de <desertscategory>
    XMLElement* currentDessert = dessertsCategory->FirstChildElement();
    // 2) parcourir tous les <dessert> qui sont des frères
    while (currentDessert != nullptr) {
        // un dessert supplémentaire
        nbDessert += 1;
        // avancer au frère <dessert> suivant de currentDessert
        currentDessert = currentDessert->NextSiblingElement();
    }
    // currentDessert n’a plus de frère {currentDessert == nullptr}, c’est le dernier
    return nbDessert;

}

/*
 * A COMPLETER
 */
int Mondial::getNbElemCat(const string categoryName) {

    //association du nom de la categorie avec la catégorie
    auto catName = decod_category.at(categoryName).c_str();

    //initialisaton du nombre de d'élément
    int nbelement = 0;
    // accéder à <categoryName>, c’est un fils de l'élément <racineMondial>
    XMLElement* elementCategory = racineMondial->FirstChildElement(catName);
    // parcours complet des fils de <categoryName> en les comptants
    // 1) accéder au premier fils <Name> de <categoryName>
    XMLElement* currentElement = elementCategory->FirstChildElement();
    // 2) parcourir tous les <Name> qui sont des frères
    while (currentElement != nullptr) {
        // un element supplémentaire
        nbelement += 1;
        // avancer au frère <name> suivant de currentElement
        currentElement = currentElement->NextSiblingElement();
    }
    // currentElement n’a plus de frère {currentElement == nullptr}, c’est le dernier
    return nbelement;
}

/*
 * A COMPLETER
 */
XMLElement* Mondial::getCountryXmlelementFromNameRec(string countryName) const {

    return getCountryXmlelementFromNameRecWorker(racineMondial->FirstChildElement("countriescategory")->FirstChildElement(), countryName);
}

/*
 * A COMPLETER
 */
XMLElement* Mondial::getCountryXmlelementFromNameRecWorker(XMLElement* currentCountryElement, string countryName) const {

    if (currentCountryElement == nullptr) return nullptr;
    else if (currentCountryElement->FirstChildElement("name")->GetText() == countryName) return currentCountryElement;
    else return getCountryXmlelementFromNameRecWorker(currentCountryElement->NextSiblingElement(), countryName);
}

/*
 * A COMPLETER
 */
string Mondial::getCountryCodeFromName(string countryName) const throw (PrecondVioleeExcep) {

    //réccuperation du pointeur pour countryName
    XMLElement* resultat = getCountryXmlelementFromNameRec(countryName);

    //verification que countryName existe
    if (!resultat) throw PrecondVioleeExcep(countryName +" n'existe pas !");
    else return resultat->Attribute("car_code");
}

/*
 * A COMPLETER
 */
/**
 * élément <country> d'un pays identifié par son nom countryName
 * @param countryName
 * @return pointeur sur l'élément <country> dont la valeur du fils <name> est égal à countryName, nullprt sinon
 */
XMLElement* Mondial::getCountryXmlelementFromNameIter(string countryName) const {
    /*
     * A COMPLETER
     */
    // supprimer à partir d'ici après complétion
    return nullptr;
}

/**
 *retourne la valeur de la dernière mesure de population d’un pays identifié par son nom (countryName)
 * @param countryName
 * @return -1 si le pays n'existe pas, 0 si aucune population enregistrer, la dernière mesure de population sinon
 */
int Mondial::getCountryPopulationFromName(string countryName) const {

    //réccuperation du pointeur du pays <countryName>
    XMLElement *country = getCountryXmlelementFromNameRec(countryName);

    if (!country)return -1;
    else{
        if(country->FirstChildElement("population")) return static_cast<unsigned int>(country->LastChildElement("population")->GetText());
        else return 0;
    }

}

/**
 *
 * @param countryCode
 * @return
 */
XMLElement* Mondial::getCountryXmlelementFromCode(string countryCode) const {
    /*
     * A COMPLETER
     */
    // supprimer à partir d'ici après complétion
    return nullptr;
}

/*
 * A COMPLETER
 */
void Mondial::printCountryBorders(string countryName) const {
    /*
     * A COMPLETER
     */
    // supprimer à partir d'ici après complétion
}

/*
 * A COMPLETER
 */
XMLElement* Mondial::getRiverXmlelementFromNameIter(string riverName) const {
    /*
     * A COMPLETER
     */
    // supprimer à partir d'ici après complétion
    return nullptr;
}

/*
 * A COMPLETER
 */
void Mondial::printAllCountriesCrossedByRiver(string riverName) const {
    /*
     * A COMPLETER
     */
}

/*
 * A COMPLETER
 */
void Mondial::printCountriesWithProvincesCrossedByRiver(string riverName) const {
    /*
     * A COMPLETER
     */
}

/*
 * A COMPLETER
 */
void Mondial::printCountriesAndProvincesCrossedByRiver(string riverName) const {
    /*
     * A COMPLETER
     */
}

/*
 * A COMPLETER
 */
void Mondial::printCityInformation(string cityName) const {
    /*
     * A COMPLETER
     */
}

/**
 * Exemple de question additionnelle pour l'exercice 9 afficher toutes les informations disponibles
 * dans Mondial concernant toutes les îles.
 * On peut commencer par une île en particulier à partir de son nom
 */
void Mondial::printIslandsInformations() const {
    /*
     * A COMPLETER
     */
}

/*
 * Méthodes de service fournies
 */

template<typename Out>
void Mondial::split(string& s, char delim, Out result) const {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        *(result++) = item;
    }
}

/**
 * Méthode à utiliser pour découper les mots d'une chaîne dans un vecteur
 * Utile pour la gestion des attributs à valeurs multiples
 * @param s chaîne à découper
 * @param delim délimiteur des mots de la chaîne à découper
 * @return vecteur contenant les mots de la chaîne découpée
 */
vector<std::string> Mondial::split(string& s, char delim) const {
    vector<std::string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

Mondial::~Mondial() {
}