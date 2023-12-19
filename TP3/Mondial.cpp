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
#include <math.h>
#include <stdio.h>
#include <algorithm>

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
/**
 * Retourne le nombre d'aéroports présents dans le fichier XML
 * @return valeur du nombre d'aéroports
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
/**
 * Affiche les codes des pays présents dans le fichier XML avec leur rang
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


/**
 * Retourne le nombre de déserts présents dans le fichier XML
 * @return valeur du nombre de dessert
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

/**
 * Retourne le nombre d'élément de la catégorie <categoryName> présents dans le fichier XML
 * @param categoryName
 * @return nombre d'élément de la catégorie <categoryName>
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

/**
 * Retourne le pointeur sur l'élément <country> d'un pays identifié par son nom (countryName)
 * @param countryName
 * @return pointeur sur l'élément <country> dont la valeur du fils <name> est égal à countryName, nullprt sinon
 */
XMLElement* Mondial::getCountryXmlelementFromNameRec(string countryName) const {

    return getCountryXmlelementFromNameRecWorker(racineMondial->FirstChildElement("countriescategory")->FirstChildElement(), countryName);
}

/**
 * worker de la methode getCountryXmlelementFromNameRec
 * @param currentCountryElement pointeur sur l'élément <country> actuel
 * @param countryName valeur du fils <name> de l'élément recherché
 * @return nullptr si l'élément n'est pas trouvé, sinon le pointeur sur l'élément recherché
 */
XMLElement* Mondial::getCountryXmlelementFromNameRecWorker(XMLElement* currentCountryElement, string countryName) const {

    if (currentCountryElement == nullptr) return nullptr;
    else if (currentCountryElement->FirstChildElement("name")->GetText() == countryName) return currentCountryElement;
    else return getCountryXmlelementFromNameRecWorker(currentCountryElement->NextSiblingElement(), countryName);
}

/**
 * Retourne le code du pays identifié par son nom (countryName)
 * @throw PrecondVioleeExcep si le pays n'existe pas
 * @param countryName
 * @return code du pays, lève un execption de type PrecondVioleeExcep si le pays n'existe pas
 */
string Mondial::getCountryCodeFromName(string countryName) const throw (PrecondVioleeExcep) {

    //réccuperation du pointeur pour countryName
    XMLElement* resultat = getCountryXmlelementFromNameRec(countryName);

    //verification que countryName existe
    if (!resultat) throw PrecondVioleeExcep(countryName +" n'existe pas !");
    else return resultat->Attribute("car_code");
}


/**
 * Retourne l'élément <country> d'un pays identifié par son nom (countryName)
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
 * retourne le pointeur sur l'élément <country> d'un pays identifié par son code (countryCode) avec l'attribut "car_code"
 * @param countryCode
 * @return pointeur sur l'élément <country> dont la valeur de l'attribut "car_code" est égal à countryCode, nullprt sinon
 */
XMLElement* Mondial::getCountryXmlelementFromCode(string countryCode) const {

    XMLElement* currentCountryElementPtr = racineMondial->FirstChildElement("countriescategory")->FirstChildElement();
    while(currentCountryElementPtr && currentCountryElementPtr->Attribute("car_code") != countryCode){
        currentCountryElementPtr = currentCountryElementPtr->NextSiblingElement();
    }
    return currentCountryElementPtr;
}

/**
 * Affiche les pays frontaliers du pays <countryName> avec la longueur de la frontière
 * précise si le pays n'existe pas ou n'a pas de pays frontalier
 * @param countryName
 */
void Mondial::printCountryBorders(string countryName) const {

    //reccuperation du pointeur de l'élément pays ayant pour nom <countryName>
    XMLElement* countryPtr = getCountryXmlelementFromNameRec(countryName);

    if(!countryPtr){ // le pays n'existe pas
        cout << "Le pays : " << countryName << ", n'existe pas !" << endl;
    }else{
        if(!countryPtr->FirstChildElement("border")){ // le pays n'a pas de pays frontalier car pas de balise <border>
            cout << "Le pays : "<<countryName <<", n'a pas de pays frontalier !" << endl;
        }else{
            cout << "Le pays : "<< countryName << endl;
            XMLElement* currentBorderPtr = countryPtr->FirstChildElement("border");
            while(currentBorderPtr){ // parcours de toutes les balises <border> du pays currentCountryPtr
                cout << "  est frontalier avec : "
                    << getCountryXmlelementFromCode(currentBorderPtr->Attribute("country"))->FirstChildElement("name")->GetText()
                    <<", la longueur de sa frontière avec celui-ci est : "<<currentBorderPtr->Attribute("length") << endl;

                currentBorderPtr= currentBorderPtr->NextSiblingElement("border");
            }
        }
    }
}

/**
 * reccupère le XMLElement <river> ayant son nom <name> valant "riverName"
 * @param riverName
 * @return XmlElement* du fichier xml si trouver sinon nullptr
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

/**
 * Affiche les pays traversés par le fleuve (riverName)
 * precise si le fleuve n'existe pas
 * @param riverName
 */
void Mondial::printAllCountriesCrossedByRiver(string riverName) const {

    // reccupération de l'élément river qui possède le nom riverName
    XMLElement *riverPtr = getRiverXmlelementFromNameIter(riverName);

    if (!riverPtr) // le fleuve n'existe pas
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

/**
 * Affiche les informations sur les pays traversés par le fleuve (riverName)
 * precise si le fleuve n'existe pas
 * @param riverName
 */
void Mondial::printCountriesWithProvincesCrossedByRiver(string riverName) const {

    // reccupération de l'élément river qui possède le nom riverName
    XMLElement *riverPtr = getRiverXmlelementFromNameIter(riverName);

    if (!riverPtr) // le fleuve n'existe pas
        cout << "Le fleuve : "<<riverName<<", n'existe pas !" <<endl;
    else {
        cout << "Le fleuve : " << riverName << "\n  traverse les pays suivants : ";
        XMLElement *currentLocatedRiverPtr = riverPtr->FirstChildElement("located");
        while(currentLocatedRiverPtr){ // parcours de toutes les balises <located> du fleuve currentRiverPtr
            cout << getCountryXmlelementFromCode(currentLocatedRiverPtr->Attribute("country"))->FirstChildElement("name")->GetText() << ", ";
            currentLocatedRiverPtr = currentLocatedRiverPtr->NextSiblingElement("located");
        }
        cout <<"il a la longueur suivante : "<<riverPtr->FirstChildElement("length")->GetText()<< endl;
    }
}

/**
 * Affiche les informations sur les pays et les provinces traversés par le fleuve (riverName)
 * @param riverName
 */
void Mondial::printCountriesAndProvincesCrossedByRiver(string riverName) const {
    // reccupération de l'élément river qui possède le nom riverName
    XMLElement *riverPtr = getRiverXmlelementFromNameIter(riverName);

    if (!riverPtr) // le fleuve n'existe pas
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
        //hashmap des élément located avec leur code de pays
        map<string, XMLElement*> locatedElements;
        while(currentLocatedElementPtr){
            locatedElements[currentLocatedElementPtr->Attribute("country")] = currentLocatedElementPtr; // mise dans la map des element located de l'île
            currentLocatedElementPtr = currentLocatedElementPtr->NextSiblingElement("located");
        }
        for(string codePays : listePays){ // pour chaque pays présent dans l'attribut "country" de <river>
            XMLElement *currentCountryPtr = getCountryXmlelementFromCode(codePays);

            if (locatedElements.find(codePays) != locatedElements.end()){   // verification que le pays est dans une balise <located> et à donc des provinces
                cout << "  - " << currentCountryPtr->FirstChildElement("name")->GetText()
                    <<", où il traverse les divisions administratives suivantes : "<<endl;

                //reccupération du bon element <located>
                currentLocatedElementPtr = locatedElements.at(codePays);
                //reccupérer la chaîne des provinces traversées :
                string provinces = currentLocatedElementPtr->Attribute("province");
                //séparation des provinces dans un vecteur :
                vector<string> listeProvince = split(provinces, ' ');
                //affichage des provinces
                printListeProvinceFromCountry(currentCountryPtr, listeProvince);
            }else{
                cout << "  - " << currentCountryPtr->FirstChildElement("name")->GetText() << endl;
            }
        }
    }
}

void Mondial::getXmlelementCityAndProvinceFromAttribut(XMLElement*& currentCountryPtr, XMLElement*& currentCityPtr, XMLElement*& currentProvincePtr, const char* attribut, string value) const{

    bool flag = false; // booléen de sortie de boucle si la ville est trouvé
    while (currentCountryPtr && !flag) {
        currentProvincePtr = currentCountryPtr->FirstChildElement("province");

        while (currentProvincePtr && !flag) {
            currentCityPtr = currentProvincePtr->FirstChildElement("city");
            while (currentCityPtr && currentCityPtr->Attribute(attribut) != value) {
                currentCityPtr = currentCityPtr->NextSiblingElement("city");
            }
            if (currentCityPtr) flag = true;
            if (!flag)
                currentProvincePtr = currentProvincePtr->NextSiblingElement("province"); // si la ville n'est pas trouvé on passe à la province suivante
        }

        if (!flag) {    // si la ville n'est pas trouvé dans une balise <provinces> on la cherche dans le pays
            currentCityPtr = currentCountryPtr->FirstChildElement("city");
            while (currentCityPtr && currentCityPtr->Attribute(attribut) != value) {
                currentCityPtr = currentCityPtr->NextSiblingElement("city");
            }
            if (currentCityPtr) flag = true;
            if (!flag) currentCountryPtr = currentCountryPtr->NextSiblingElement(); // si la ville n'est pas trouvé on passe au pays suivant
        }
    }

}


/**
 * retourne un pointeur sur l'élément <city> si son <name> vaut value
 * @param attribut
 * @param value
 * @return pointeur sur l'élément <city> si triuvé sinon nullptr
 */
void Mondial::getXmlelementCityAndProvinceFromName(XMLElement *&currentCountryPtr, XMLElement *&currentCityPtr, XMLElement *&currentProvincePtr,
                                                       std::string cityName) const {

    //recherche de l'élémnet de <city> possèdant le fils <name> valant cityName
    bool flag = false; // booléen de sortie de boucle si la ville est trouvé
    while(currentCountryPtr && !flag){
        currentProvincePtr = currentCountryPtr->FirstChildElement("province");

        while(currentProvincePtr && !flag){
            currentCityPtr = currentProvincePtr->FirstChildElement("city");
            while(currentCityPtr && currentCityPtr->FirstChildElement("name")->GetText() != cityName){
                currentCityPtr = currentCityPtr->NextSiblingElement("city");
            }
            if (currentCityPtr) flag = true;
            if (!flag) currentProvincePtr = currentProvincePtr->NextSiblingElement("province"); // si la ville n'est pas trouvé on passe à la province suivante
        }

        if (!flag) {    // si la ville n'est pas trouvé dans une balise <provinces> on la cherche dans le pays
            currentCityPtr = currentCountryPtr->FirstChildElement("city");
            while (currentCityPtr && currentCityPtr->FirstChildElement("name")->GetText() != cityName) {
                currentCityPtr = currentCityPtr->NextSiblingElement("city");
            }
            if (currentCityPtr) flag = true;
            if (!flag) currentCountryPtr = currentCountryPtr->NextSiblingElement(); // si la ville n'est pas trouvé on passe au pays suivant
        }
    }

}

/**
 * Affiche les informations sur la ville <cityName> (nom, pays, province, latitude, longitude, altitude, dernière population connue)
 * @param cityName
 */
void Mondial::printCityInformation(string cityName) const {

    //recherche de l'élémnet de <city> possèdant le fils <name> valant cityName
    XMLElement *currentCountryPtr = racineMondial->FirstChildElement("countriescategory")->FirstChildElement();
    XMLElement* currentCityPtr;
    XMLElement* currentProvincePtr;
    getXmlelementCityAndProvinceFromName(currentCountryPtr, currentCityPtr, currentProvincePtr, cityName);

    if (!currentCityPtr){ // la ville n'existe pas
        cout << "La ville "<<cityName<<", n'existe pas !" << endl;
    }else{
        cout << "La ville "<<cityName
            << "\n - se trouve dans le pays : "<<currentCountryPtr->FirstChildElement("name")->GetText()<<endl;
        if (currentProvincePtr){ // si la ville est dans une balise <province>
            cout << " - dans la division adminstrative : "<<currentProvincePtr->FirstChildElement("name")->GetText()<< endl;
        }
        cout <<" - sa latitude est : "<<currentCityPtr->FirstChildElement("latitude")->GetText()
        <<"\n - sa longitude est : "<<currentCityPtr->FirstChildElement("longitude")->GetText()
        <<"\n - son altitude est : "<<currentCityPtr->FirstChildElement("elevation")->GetText()
        <<"\n - sa population est : "<<currentCityPtr->LastChildElement("population")->GetText()<<endl;
    }
}

/*---------------------------------  METHODES DE L'EXERCICE 9   ---------------------------------*/

/**
 * reccupère le XMLElement fils de <categorie> ayant son identifiant valant "id"
 * @throw PrecondVioleeExcep si la catégorie n'existe pas
 * @param id
 * @param categorie
 * @return XmlElement* du fichier xml si trouver sinon nullptr
 */
XMLElement *Mondial::getXmlelementFromIdAndCatRec(string id, const char* categorie) const throw (PrecondVioleeExcep){
    const char *attribut;
    if (categorie == "countriescategory") attribut = "car_code";
    else if (categorie == "airportscategory") attribut = "iatacode";
    else attribut = "id"; // pour les autres catégories

    XMLElement* currentElement = racineMondial->FirstChildElement(categorie);
    if (!currentElement) throw PrecondVioleeExcep((string)categorie+" n'existe pas !"); // la catégorie n'existe pas on lève une exception
    else return getXmlelementFromIdAndCatRecWorker(currentElement->FirstChildElement(), id, attribut);
}

/**
 * worker de la methode getXmlelementFromIdAndCatRec
 * @param currentElement pointeur sur l'élément actuel
 * @param id valeur de l'attribut de l'élément recherché
 * @param attribut nom de l'attribut de l'élément recherché
 * @return nullptr si l'élément n'est pas trouvé, sinon le pointeur sur l'élément recherché
 */
XMLElement *
Mondial::getXmlelementFromIdAndCatRecWorker(XMLElement *currentElement, string &id, const char* attribut) const {

    if (!currentElement) return nullptr;
    else if (currentElement->Attribute(attribut) == id) return currentElement;
    else return getXmlelementFromIdAndCatRecWorker(currentElement->NextSiblingElement(), id, attribut);
}

/**
 * Affiche les provinces de currentCountryPtr si leur id est dans listeProvince
 * @param currentCountryPtr
 * @param listeProvince
 */
void Mondial::printListeProvinceFromCountry(tinyxml2::XMLElement *currentCountryPtr,
                                            vector<std::string> listeProvince) const {

    XMLElement* currentProvincePtr = currentCountryPtr->FirstChildElement("province");
    while(currentProvincePtr){
        if (count(listeProvince.begin(), listeProvince.end(), currentProvincePtr->Attribute("id")))
            cout << "\t\t* "<<currentProvincePtr->FirstChildElement("name")->GetText() <<endl;       // affiche du nom de la province si elle est dans l'attribut "province de l'élément <located> actuel
        currentProvincePtr = currentProvincePtr->NextSiblingElement("province");
    }
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
        cout << "L'île " << currentIslandPtr->FirstChildElement("name")->GetText();
        //affichage de la surface si elle est presente
        if (currentIslandPtr->FirstChildElement("area"))cout<< " fait "<< currentIslandPtr->FirstChildElement("area")->GetText() << " km carrés et ";
        cout << "appartient à :" << endl;

         /************************* Affichage du(des) pays ***************************/

        //mise dans un vecteur et affichage de l'ensemble des pays qui possèdent l'île avec leur province si nécessaire
        string countries = currentIslandPtr->Attribute("country");
        vector<string> listeCountry = split(countries,' ');     // séparation des mot de l'attribut country
        XMLElement *currentLocatedElementPtr = currentIslandPtr->FirstChildElement("located");
        //hashmap des élément located avec leur code de pays
        map<string, XMLElement*> locatedElements;
        while(currentLocatedElementPtr){
            locatedElements[currentLocatedElementPtr->Attribute("country")] = currentLocatedElementPtr; // mise dans la map des element located de l'île
            currentLocatedElementPtr = currentLocatedElementPtr->NextSiblingElement("located");
        }
        for(string code : listeCountry){ // pour chaque pays présent dans l'attribut "country" de <island>
            XMLElement* currentCountryPtr = getCountryXmlelementFromCode(code);
            if (locatedElements.find(code) != locatedElements.end()){  // verification que le pays est dans une balise <located> et a donc des provinces
                cout << "\t - " << currentCountryPtr->FirstChildElement("name")->GetText()
                     <<" dans la division administrative de : "<<endl;

                //reccupération du bon element <located>
                currentLocatedElementPtr = locatedElements.at(code);
                //reccupérer la chaîne des provinces traversées :
                string provinces = currentLocatedElementPtr->Attribute("province");
                //séparation des provinces dans un vecteur :
                vector<string> listeProvince = split(provinces, ' ');
                //affichage des provinces
                printListeProvinceFromCountry(currentCountryPtr, listeProvince);
            }else {
                cout << "\t* " << currentCountryPtr->FirstChildElement("name")->GetText() << endl;
            }
        }

        /************************* Affichage du(des) mer(s) ***************************/

        //mise dans un vecteur et affichage des mer et océan entourant l'île si l'attribut sea est présent
        if (currentIslandPtr->Attribute("sea")) {
            cout << "  - l'île est entourée par :" << endl;
            string seas = currentIslandPtr->Attribute("sea");   // ensemble des mers qui entoure l'île
            vector<string> listeSea = split(seas, ' ');
            for (string idSea: listeSea) {
                XMLElement *currentSeaPtr = getXmlelementFromIdAndCatRec(idSea, "seascategory"); // reccupération de l'élément <sea> ayant l'id "idseas"
                cout << "\t* " << currentSeaPtr->FirstChildElement("name")->GetText() << endl;  //affichage du nom de <sea>
            }
        }

        /************************* Affichage des rivières  ***************************/
        if(currentIslandPtr->Attribute("river")){
            cout << "  - Elle est entourée par la ou les rivière(s) : "<<endl;
            string rivers = currentIslandPtr->Attribute("river"); // reccupération de(s) rivière
            vector<string> listeRivers = split(rivers, ' '); //sépartion des rivières dans un vecteur
            XMLElement* currentRiver;
            for(string river : listeRivers){ //pour chaque rivière de listeRiver
                try{
                    currentRiver = getXmlelementFromIdAndCatRec(river, "riverscategory");
                    cout << "\t * " << currentRiver->FirstChildElement("name")->GetText();
                }catch (PrecondVioleeExcep &e){
                    cout << e.what() << endl;
                }

            }
        }

        /************************* Affichage du lac  ***************************/
        if(currentIslandPtr->Attribute("lake")) {
            cout << "  - Elle est entourée par le lac : " << endl;
            string lake = currentIslandPtr->Attribute("lake"); // reccupération de l'identifiant du lake
            try {
                XMLElement *currentLake = getXmlelementFromIdAndCatRec(lake, "lakescategory");
                cout << "\t * " << currentLake->FirstChildElement("name")->GetText();   // affichage du nom du lac
            } catch (PrecondVioleeExcep &e) {
                cout << e.what() << endl;
            }
        }

        /************************* Affichage du type ***************************/

        // affichage du type de l'île s'il existe
        if (currentIslandPtr->Attribute("type")) cout << "  - C'est une île de type "<< currentIslandPtr->Attribute("type") << endl;

        /************************* Affichage de l'archipel ***************************/

        // affichage de l'achipel s'il existe
        if (currentIslandPtr->FirstChildElement("islands")) cout << "  - Elle fait partie de l'archipel de(s) " <<currentIslandPtr->FirstChildElement("islands")->GetText()<< endl;

        /************************* Affichage de la longitude / l'atitude / altitude ***************************/

        int lattitude = stoi(currentIslandPtr->FirstChildElement("latitude")->GetText());
        int longitude = stoi(currentIslandPtr->FirstChildElement("longitude")->GetText());


        cout << "  - Les coordonnées de l'île sont : ";
        if (lattitude < 0) cout << abs(lattitude) << "°S";
        else cout << lattitude << "°N";
        if (longitude < 0) cout << ", "<< abs(longitude) << "°O";
        else cout << ", "<< longitude << "°E";
        if(currentIslandPtr->FirstChildElement("elevation"))
            cout << " et elle culmine à "<< currentIslandPtr->FirstChildElement("elevation")->GetText() << " mètres d'altitude ";
        cout << endl << endl;

        currentIslandPtr = currentIslandPtr->NextSiblingElement();
    }
}

/**
 * Affiche les informations sur les aéroports du pays <countryName> (nom, ville, latitude, longitude, altitude, fuseau horaire)
 */
void Mondial::printAirportFromOfCountry(string countryName) const {
    // recuperation du code du pays
    string code = "";
    try{
        code = getCountryCodeFromName(countryName);
    }catch (PrecondVioleeExcep &e){
        cout << e.what() << endl;
    }

    if(!code.empty()){
        // création d'un vecteur contenant les elements <airport> du pays
        vector<XMLElement *> listeAirPort;

        // récuperation du premier <airport>
        XMLElement *currentAirport = racineMondial->FirstChildElement("airportscategory")->FirstChildElement();
        //parcour des airport
        while (currentAirport) {
            // si le code du pays correspond à celui de l'airport
            if (currentAirport->Attribute("country") == code) {
                // on l'ajoute au vecteur
                listeAirPort.push_back(currentAirport);
            }
            // on passe à l'airport suivant
            currentAirport = currentAirport->NextSiblingElement();
        }

        if (listeAirPort.empty()){
            cout << "Le pays : "<<countryName <<", n'a pas d'aéroport !" << endl;
        }else{
            cout << "Les aéroport de "<<countryName << " :"<<endl;

            for (XMLElement* airportPtr : listeAirPort){
                cout << "L'aéroport : "<< airportPtr->FirstChildElement("name")->GetText();
                if (airportPtr->Attribute("city")) {
                    //recherche de l'élémnet de <city> possèdant l'attribut id valant airport->Attribute("city")
                    XMLElement *currentCountryPtr = racineMondial->FirstChildElement(
                            "countriescategory")->FirstChildElement();
                    XMLElement *currentProvincePtr;
                    XMLElement *currentCityPtr;
                    string cityId = airportPtr->Attribute("city");

                    getXmlelementCityAndProvinceFromAttribut(currentCountryPtr, currentCityPtr, currentProvincePtr, "id", cityId);

                    cout<< " se trouve à " << currentCityPtr->FirstChildElement("name")->GetText()<<endl;
                }else cout << " se trouve dans une zone non habitée"<<endl;


                int lattitude = stoi(airportPtr->FirstChildElement("latitude")->GetText());
                int longitude = stoi(airportPtr->FirstChildElement("longitude")->GetText());
                cout << "  - Les coordonnées de l'aéroport sont : ";
                if (lattitude < 0) cout << abs(lattitude) << "°S";
                else cout << lattitude << "°N";
                if (longitude < 0) cout << ", "<< abs(longitude) << "°O";
                else cout << ", "<< longitude << "°E";
                    cout << " et il culmine à "<< airportPtr->FirstChildElement("elevation")->GetText() << " mètres d'altitude ";
                cout << endl;

                //affichage de l'heure à l'aide du fuseau horaire
                time_t rawtime;
                struct tm * ptm;
                time ( &rawtime );
                ptm = gmtime ( &rawtime );
                cout << "  - À ces coordonnée il est actuemment : "<< ptm->tm_hour+stoi(airportPtr->FirstChildElement("gmtOffset")->GetText()) <<"H et "<< ptm->tm_min << "min"<< endl;

                if (airportPtr->FirstChildElement("located_on"))
                    cout << "  - Il est situé sur l'île : "
                    << getXmlelementFromIdAndCatRec(airportPtr->FirstChildElement("located_on")->Attribute("island"), "islandscategory")->FirstChildElement("name")->GetText()<< endl;
                cout << endl;

            }
        }
    }
}


/**
 * Affiche les informations sur la montagne la plus proche de la ville (cityName) ()
 */
void Mondial::printMountainClosestToCity(string cityName) const {


    //recherche de l'élémnet de <city> possèdant le fils <name> valant cityName
    XMLElement *currentCountryPtr = racineMondial->FirstChildElement("countriescategory")->FirstChildElement();
    XMLElement* currentCityPtr;
    XMLElement* currentProvincePtr;
    getXmlelementCityAndProvinceFromName(currentCountryPtr, currentCityPtr, currentProvincePtr, cityName);

    if (!currentCityPtr){ // la ville n'existe pas
        cout << "La ville "<<cityName<<", n'existe pas !" << endl;
    }else {
        XMLElement *closestMountainPtr = racineMondial->FirstChildElement("mountainscategory")->FirstChildElement(); // element le plus proche de ville
        XMLElement *currentMountainPtr = closestMountainPtr->NextSiblingElement();
        // calucle d'une distance entre 2 points a et b =  racine((a.x - b.x)² + (a.y - b.y)²)
        double distance = sqrt(pow(stoi(currentCityPtr->FirstChildElement("latitude")->GetText()) - stoi(closestMountainPtr->FirstChildElement("latitude")->GetText()), 2)
                            + pow(stoi(currentCityPtr->FirstChildElement("longitude")->GetText()) - stoi(closestMountainPtr->FirstChildElement("longitude")->GetText()), 2));
        double distance2;
        while(currentMountainPtr){
            distance2 = sqrt(pow(stoi(currentCityPtr->FirstChildElement("latitude")->GetText())-stoi(currentMountainPtr->FirstChildElement("latitude")->GetText()),2)
                    + pow(stoi(currentCityPtr->FirstChildElement("longitude")->GetText())-stoi(currentMountainPtr->FirstChildElement("longitude")->GetText()),2));
            if (distance2 < distance){
                distance = distance2;
                closestMountainPtr = currentMountainPtr;
            }
            currentMountainPtr = currentMountainPtr->NextSiblingElement();
        }

        // TODO : préciser la valeur de la distance (elle est tronqué)
        cout << "La montagne la plus proche de " << cityName << " est " << closestMountainPtr->FirstChildElement("name")->GetText() << ", à une distance de "<<distance*111.3 <<" kilomètre."<<endl;
        cout << "  - Elle se situe dans le(s) pays :"<<endl;
        string pays = closestMountainPtr->Attribute("country");
        vector<string> listePays = split(pays, ' ');
        for(string paysCode : listePays){
            cout << "\t* "<< getCountryXmlelementFromCode(pays)->FirstChildElement("name")->GetText()<<endl;
        }

        cout << "  - Elle se situe dans la chaîne de : " << closestMountainPtr->FirstChildElement("mountains")->GetText()<<endl;
        int lattitude = stoi(closestMountainPtr->FirstChildElement("latitude")->GetText());
        int longitude = stoi(closestMountainPtr->FirstChildElement("longitude")->GetText());
        cout << "  - Les coordonnées de l'aéroport sont : ";
        if (lattitude < 0) cout << abs(lattitude) << "°S";
        else cout << lattitude << "°N";
        if (longitude < 0) cout << ", "<< abs(longitude) << "°O";
        else cout << ", "<< longitude << "°E";
        cout << " et il culmine à "<< closestMountainPtr->FirstChildElement("elevation")->GetText() << " mètres d'altitude "<<endl;

        if (closestMountainPtr->Attribute("type")){
            cout << "  - C'est une montagne de type "<< closestMountainPtr->Attribute("type") << endl;
        }
        if(closestMountainPtr->Attribute("last_eruption")){
            string date = closestMountainPtr->Attribute("last_eruption");
            cout << "  - Elle a eu sa dernière éruption en "<< date << endl;
        }
        if (closestMountainPtr->Attribute("island"))
            cout << "  - Elle est situé sur l'île : "
                 << getXmlelementFromIdAndCatRec(closestMountainPtr->Attribute("island"), "islandscategory")->FirstChildElement("name")->GetText()<< endl;
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