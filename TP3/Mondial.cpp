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
    while (currentAirport) {
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
    while (currentCountry) {
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
    while (currentDessert) {
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
    while (currentElement) {
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

    XMLElement* currentCountryElementPtr = racineMondial->FirstChildElement("countriescategory")->FirstChildElement();
    while( currentCountryElementPtr && currentCountryElementPtr->FirstChildElement("name")->GetText() != countryName){
        currentCountryElementPtr = currentCountryElementPtr->NextSiblingElement();
    }

    return currentCountryElementPtr;
}

/**
 *retourne la valeur de la dernière mesure de population d’un pays identifié par son nom (countryName)
 * @param countryName
 * @return -1 si le pays n'existe pas, 0 si aucune population enregistrer, la dernière mesure de population sinon
 */
int Mondial::getCountryPopulationFromName(string countryName) const {

    //réccuperation du pointeur du pays <countryName>
    XMLElement *country = getCountryXmlelementFromNameIter(countryName);

    if (!country) return -1;
    else{
        if(country->FirstChildElement("population")) return atoi(country->LastChildElement("population")->GetText());
        else return 0;
    }

}

/**
 *
 * @param countryCode
 * @return
 */
XMLElement* Mondial::getCountryXmlelementFromCode(string countryCode) const {

    XMLElement* currentCountryElementPtr = racineMondial->FirstChildElement("countriescategory")->FirstChildElement();
    while(currentCountryElementPtr && currentCountryElementPtr->Attribute("car_code") != countryCode){
        currentCountryElementPtr = currentCountryElementPtr->NextSiblingElement();
    }
    return currentCountryElementPtr;
}

/*
 * A COMPLETER
 */
void Mondial::printCountryBorders(string countryName) const {

    //reccuperation du pointeur de l'élément pays ayant pour nom <countryName>
    XMLElement* countryPtr = getCountryXmlelementFromNameRec(countryName);

    if(!countryPtr){
        cout << "Le pays : " << countryName << ", n'existe pas !" << endl;
    }else{
        if(!countryPtr->FirstChildElement("border")){
            cout << "Le pays : "<<countryName <<", n'a pas de pays frontalier !" << endl;
        }else{
            cout << "Le pays : "<< countryName << endl;
            XMLElement* currentBorderPtr = countryPtr->FirstChildElement("border");
            while(currentBorderPtr){
                cout << "  est frontalier avec : "
                    << getCountryXmlelementFromCode(currentBorderPtr->Attribute("country"))->FirstChildElement("name")->GetText()
                    <<", la longueur de sa frontière avec celui-ci est : "<<currentBorderPtr->Attribute("length") << endl;

                currentBorderPtr= currentBorderPtr->NextSiblingElement("border");
            }
        }
    }
}

/*
 * A COMPLETER
 */
XMLElement* Mondial::getRiverXmlelementFromNameIter(string riverName) const {

    // premier fils de la catégorie <riverscategory>
    XMLElement* currentRiverElementPtr = racineMondial->FirstChildElement("riverscategory")->FirstChildElement();

    //condition de sorti du while : soit on a parcouru toute les rivers, soit on a trouver la river où <name> vaut riverName
    while(currentRiverElementPtr && currentRiverElementPtr->FirstChildElement("name")->GetText() != riverName){
        currentRiverElementPtr = currentRiverElementPtr->NextSiblingElement();
    }
    return currentRiverElementPtr;
}

/*
 * A COMPLETER
 */
void Mondial::printAllCountriesCrossedByRiver(string riverName) const {

    // reccupération de l'élément river qui possède le nom riverName
    XMLElement *riverPtr = getRiverXmlelementFromNameIter(riverName);

    if (!riverPtr)
        cout << "Le fleuve : "<<riverName<<", n'existe pas !" <<endl;
    else{
        cout << "Le fleuve : "<< riverName << "\n  traverse les pays suivants : ";

        //reccupération de la liste de codes de pays traversés par la rivière
        string s = riverPtr->Attribute("country");
        //séparation de la liste avec le charactère " "
        vector<string> listeCountry = split(s, ' ');
        //affichage de la liste des pays
        for(string ss : listeCountry){
            cout << getCountryXmlelementFromCode(ss)->FirstChildElement("name")->GetText() << ", ";
        }
        cout <<"il a la longueur suivante : "<<riverPtr->FirstChildElement("length")->GetText()<< endl;
    }

}

/*
 * A COMPLETER
 */
void Mondial::printCountriesWithProvincesCrossedByRiver(string riverName) const {

    // reccupération de l'élément river qui possède le nom riverName
    XMLElement *riverPtr = getRiverXmlelementFromNameIter(riverName);

    if (!riverPtr)
        cout << "Le fleuve : "<<riverName<<", n'existe pas !" <<endl;
    else {
        cout << "Le fleuve : " << riverName << "\n  traverse les pays suivants : ";
        XMLElement *currentLocatedRiverPtr = riverPtr->FirstChildElement("located");
        while(currentLocatedRiverPtr){
            cout << getCountryXmlelementFromCode(currentLocatedRiverPtr->Attribute("country"))->FirstChildElement("name")->GetText() << ", ";
            currentLocatedRiverPtr = currentLocatedRiverPtr->NextSiblingElement("located");
        }
        cout <<"il a la longueur suivante : "<<riverPtr->FirstChildElement("length")->GetText()<< endl;
    }
}

/*
 * A COMPLETER
 */
void Mondial::printCountriesAndProvincesCrossedByRiver(string riverName) const {
    // reccupération de l'élément river qui possède le nom riverName
    XMLElement *riverPtr = getRiverXmlelementFromNameIter(riverName);

    if (!riverPtr)
        cout << "Le fleuve : "<<riverName<<", n'existe pas !" <<endl;
    else {
        cout << "Le fleuve : " << riverName << " de longueur "<<riverPtr->FirstChildElement("length")->GetText()
            <<" traverse les pays suivants :"<< endl;
        //reccupérer la chaîne des pays traversé :
        string s = riverPtr->Attribute("country");
        //séparation des pays dans un vecteur :
        vector<string> listePays = split(s, ' ');
        //reccupération de l'élément <located>
        XMLElement *currentLocatedElementPtr = riverPtr->FirstChildElement("located");
        //liste des élements <located>
        vector<string> locateds;
        while(currentLocatedElementPtr){
            locateds.emplace_back(currentLocatedElementPtr->Attribute("country"));
            currentLocatedElementPtr = currentLocatedElementPtr->NextSiblingElement("located");
        }
        currentLocatedElementPtr = riverPtr->FirstChildElement("located");
        for(string ss : listePays){
            XMLElement *currentCountryPtr = getCountryXmlelementFromCode(ss);

            if (count(locateds.begin(), locateds.end(), ss)){
                cout << "  - " << currentCountryPtr->FirstChildElement("name")->GetText()
                    <<", où il traverse les divisions administratives suivantes : "<<endl;
                //reccupération du bon element <located>
                while(currentLocatedElementPtr->Attribute("country") != ss){
                    currentLocatedElementPtr = currentLocatedElementPtr->NextSiblingElement("located");
                }

                //reccupérer la chaîne des provinces traversées :
                string provinces = currentLocatedElementPtr->Attribute("province");
                //séparation des provinces dans un vecteur :
                vector<string> listeProvince = split(provinces, ' ');
                //reccupération de la premiere province de currentCountryPtr
                XMLElement *currentProvincePtr = currentCountryPtr->FirstChildElement("province");
                while(currentProvincePtr){
                    if (count(listeProvince.begin(), listeProvince.end(), currentProvincePtr->Attribute("id")))
                        cout << "\t * "<<currentProvincePtr->FirstChildElement("name")->GetText() <<endl;
                    currentProvincePtr = currentProvincePtr->NextSiblingElement("province");
                }

                currentLocatedElementPtr = riverPtr->FirstChildElement("located");
            }else{
                cout << "  - " << currentCountryPtr->FirstChildElement("name")->GetText() << endl;
            }
        }
    }
}

/*
 * A COMPLETER
 */
void Mondial::printCityInformation(string cityName) const {

    //recherche de l'élémnet de <city> possèdant le fils <name> valant cityName
    XMLElement *currentCountryPtr = racineMondial->FirstChildElement("countriescategory")->FirstChildElement();
    XMLElement *currentProvincePtr = nullptr;
    XMLElement *currentCityPtr = nullptr;
    bool flag = false;
    while(currentCountryPtr && !flag){
        currentProvincePtr = currentCountryPtr->FirstChildElement("province");
        while(currentProvincePtr && !flag){
            currentCityPtr = currentProvincePtr->FirstChildElement("city");
            while(currentCityPtr && currentCityPtr->FirstChildElement("name")->GetText() != cityName){
                currentCityPtr = currentCityPtr->NextSiblingElement("city");
            }
            if (currentCityPtr) flag = true;
            if (!flag) currentProvincePtr = currentProvincePtr->NextSiblingElement("province");
        }
        if (!flag) {
            currentCityPtr = currentCountryPtr->FirstChildElement("city");
            while (currentCityPtr && currentCityPtr->FirstChildElement("name")->GetText() != cityName) {
                currentCityPtr = currentCityPtr->NextSiblingElement("city");
            }
            if (currentCityPtr) flag = true;
            if (!flag) currentCountryPtr = currentCountryPtr->NextSiblingElement();
        }
    }

    if (!currentCityPtr){
        cout << "La ville "<<cityName<<", n'existe pas !" << endl;
    }else{
        cout << "La ville "<<cityName
            << "\n - se trouve dans le pays : "<<currentCountryPtr->FirstChildElement("name")->GetText()<<endl;
        if (currentProvincePtr){
            cout << " - dans la division adminstrative : "<<currentProvincePtr->FirstChildElement("name")->GetText()<< endl;
        }
        cout <<" - sa latitude est : "<<currentCityPtr->FirstChildElement("latitude")->GetText()
        <<"\n - sa longitude est : "<<currentCityPtr->FirstChildElement("longitude")->GetText()
        <<"\n - son altitude est : "<<currentCityPtr->FirstChildElement("elevation")->GetText()
        <<"\n - sa population est : "<<currentCityPtr->LastChildElement("population")->GetText()<<endl;
    }
}

XMLElement *Mondial::getSeaXmlelementFromIdRec(string idSea) const {
    return getSeaXmlelementFromIdRecWorker(racineMondial->FirstChildElement("seascategory")->FirstChildElement(), idSea);
}

XMLElement *
Mondial::getSeaXmlelementFromIdRecWorker(XMLElement *currentIslandElement, string idSea) const {

    if (!currentIslandElement) return nullptr;
    else if(currentIslandElement->Attribute("id") == idSea) return currentIslandElement;
    else return getSeaXmlelementFromIdRecWorker(currentIslandElement->NextSiblingElement(), idSea);
}

/**
 * Exemple de question additionnelle pour l'exercice 9 afficher toutes les informations disponibles
 * dans Mondial concernant toutes les îles.
 * On peut commencer par une île en particulier à partir de son nom
 */
void Mondial::printIslandsInformations() const {

    //reccupération du premier element <island> de <islandscategory>
    XMLElement* currentIslandPtr =  racineMondial->FirstChildElement("islandscategory")->FirstChildElement();

    //boucle d'affichage des informations pour toutes les îles
    while(currentIslandPtr){
        //introduction sur l'île
        cout << "L'île " << currentIslandPtr->FirstChildElement("name")->GetText() << " fait "
        << currentIslandPtr->FirstChildElement("area") << " km carrés et appartient à :" << endl;

        //mise dans un vecteur et affichage de l'ensemble des pays qui possèdent l'île
        string countries = currentIslandPtr->Attribute("country");
        vector<string> listeCountry = split(countries,' ');
        for(string code : listeCountry){
            XMLElement* currentCountriePtr = getCountryXmlelementFromCode(code);
            cout << "\t* "<< currentCountriePtr->FirstChildElement("name")->GetText() <<endl;
        }

        cout << "- l'île est entourée par :" << endl;
        //mise dans un vecteur et affichage des mer et océan entourant l'île
        string seas = currentIslandPtr->Attribute("sea");
        vector<string> listeSea = split(seas, ' ');
        for(string idSea : listeSea){
            XMLElement* currentSeaPtr = getSeaXmlelementFromIdRec(idSea);
            cout << "\t* "<< currentSeaPtr->FirstChildElement("name")->GetText() <<endl;
        }

        //


    }
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