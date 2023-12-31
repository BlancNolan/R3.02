#ifndef _LISTE_CHAINEE_TRIEE
#define _LISTE_CHAINEE_TRIEE

#include "Shared/ListeTrieeInterface.h"
#include "Shared/Cellule.h"
#include "Shared/PrecondVioleeExcep.h"

using namespace std;

template<class TypeInfo>
class ListeTrieeChainee : public ListeTrieeInterface<TypeInfo> {
private:
    Cellule<TypeInfo>* ptrTete; // Pointeur sur la première cellule de la liste
    int nbCellules; // nombre courant de cellules dans la liste

    // Returne un pointeur sur une copie de la listeChaineeOriginale
    Cellule<TypeInfo>* copieListe(const Cellule<TypeInfo>* laListeOriginale);

    /**
     * Worker récursif d'insertion de nouvelleInfo dans ptrCetteListe
     * @pre ptrCetteListe est triée avant insertion
     * @post ptrCetteListe est triée après insertion
     * @param ptrCetteListe : pointeur (donnée-référence) sur la "liste" de cellules dans laquelle insérer
     * @param nouvelleInfo : valeur à insérer
     */
    void insereRecWorker(Cellule<TypeInfo>*& ptrCetteListe, const TypeInfo& nouvelleInfo);

    /**
     * worker récursif de suppression de la première occurrence d'une uneInfo
     * @param ptrCetteListe : pointeur (donnée-résultat) sur la "liste" de Cellules
     * @param uneInfo : information à supprimer
     * @return : true si suppression possible (uneInfo présente) ; False sinon
     */
    bool supprimePremOccInfoRecWorker(Cellule<TypeInfo>*& ptrCetteListe, const TypeInfo& uneInfo);

    void afficheCroissantRecWorker(const Cellule<TypeInfo>* ptrCetteListe) const;

    void supprimeTeteWorker(Cellule<TypeInfo> *&ptrCetteListe);

    void videRecWorker(Cellule<TypeInfo> *ptrCetteListe);


public:
    // Constructeur par défaut
    ListeTrieeChainee();
    // Constructeur à partir d'une liste chaînée exixtante
    ListeTrieeChainee(const ListeTrieeChainee<TypeInfo>& uneListe);
    // Destructeur
    virtual ~ListeTrieeChainee();

    /**
     *     /!\ ATTENTION /!\
     * 
     *  VOIR LA DOCUMENTATION DANS LA CLASSE ListeTrieeInterface 
     *
     */
    void insereRec(const TypeInfo& nouvelleInfo);

    bool supprimePremOccInfoIter(const TypeInfo& uneInfo);
    bool supprimePremOccInfoRec(const TypeInfo& uneInfo);
    int getPositIter(const TypeInfo& uneInfo) const;

    void supprimeToutesDuplications();

    // Méthodes déjà founies dans ListeTrieeInterface :
    bool estVide() const;
    int getLongueur() const;
    void videRec();

    // Méthodes pour l'affichage
    void afficheCroissantRec() const;

    // Méthodes ensemblistes
    /**
     * Vérifie que cette liste est un ensemble : elle est croissante et il n'y a pas de duplication
     * @return true si cette liste est un ensemble
     */
    bool estEnsemble() const;

    /**
     * Intersection de cet ensemble avec ensembleB
     * @pre  cette liste triée est un ensemble, ensembleB est un ensemble
     * @post  le résultat est un ensemble
     * @param ensembleB
     * @return une liste chaînée triée qui est un ensemble intersection de cet ensemble et ensembleB
     */
    ListeTrieeChainee<TypeInfo>* insersectionAvec(const ListeTrieeChainee<TypeInfo>* ensembleB) const;

    /**
     * Union de cet ensemble avec ensembleB
     * @pre  cette liste triée est un ensemble, ensembleB est un ensemble
     * @post  le résultat est un ensemble
     * @param ensembleB
     * @return une liste chaînée triée qui est un ensemble union de cet ensemble et ensembleB
     */
    ListeTrieeChainee<TypeInfo>* unionAvec(const ListeTrieeChainee<TypeInfo>* ensembleB) const;

}; // end ListeChaineeTriee

template<class TypeInfo>
ListeTrieeChainee<TypeInfo>::ListeTrieeChainee() : ptrTete(nullptr), nbCellules(0) {
} // end constructeur par défaut (une liste videRec)

template<class TypeInfo>
ListeTrieeChainee<TypeInfo>::ListeTrieeChainee(const ListeTrieeChainee<TypeInfo>& uneListe) {
    ptrTete = copieListe(uneListe.ptrTete);
    nbCellules = uneListe.nbCellules;
} // end constructeur par copie

template<class TypeInfo>
Cellule<TypeInfo>* ListeTrieeChainee<TypeInfo>::copieListe(const Cellule<TypeInfo>* laListeOriginale) {
    // création d'une liste videRec pour y ranger la copie
    Cellule<TypeInfo>* laListeCopiee = nullptr;
    if (laListeOriginale != nullptr) { // La liste originale n'est pas videRec
        // construction de la première Cellule de la copie
        // avec l'information portée par la première Cellule de la laListeOriginale
        laListeCopiee = new Cellule<TypeInfo>(laListeOriginale->getInfo());
        // avec comme suivante le résutlat de la copie de la suite de la laListeOriginale
        laListeCopiee->setSuivante(copieListe(laListeOriginale->getSuivante()));
    } // end if
    // rendre la copie
    return laListeCopiee;
} // end copieListe

template<class TypeInfo>
ListeTrieeChainee<TypeInfo>::~ListeTrieeChainee() {
    videRec();
} // end destructeur

template<class TypeInfo>
void ListeTrieeChainee<TypeInfo>::insereRec(const TypeInfo& nouvelleInfo) {
    insereRecWorker(ptrTete, nouvelleInfo);
}

/**
 * Transforme cette Liste en un ensemble en supprimant toutes les valeurs dupliquées
 * @tparam TypeInfo
 */
template<class TypeInfo>
void ListeTrieeChainee<TypeInfo>::supprimeToutesDuplications() {
    if (ptrTete){
        Cellule<TypeInfo> *ptrCurrentCell = ptrTete->getRefSuivante();
        Cellule<TypeInfo> *ptrPrecedCellue = ptrTete;

        while(ptrCurrentCell){
            if (ptrPrecedCellue->getInfo() == ptrCurrentCell->getInfo()){
                ptrPrecedCellue->setSuivante(ptrCurrentCell->getRefSuivante());
                ptrCurrentCell->setSuivante(nullptr);
                delete ptrCurrentCell;
                nbCellules--;
                ptrCurrentCell = ptrPrecedCellue->getRefSuivante();
            }else{
                ptrCurrentCell = ptrCurrentCell->getRefSuivante();
                ptrPrecedCellue = ptrPrecedCellue->getRefSuivante();
            }
        }
    }

}

/**
 * position occupée si présente, -position si absente
 * @tparam TypeInfo
 * @param uneInfo information dont on cherche la position
 * @return position de uneInfo si présente, -position que uneInfo aurait dû occuper si absente
 */
template<class TypeInfo>
int ListeTrieeChainee<TypeInfo>::getPositIter(const TypeInfo& uneInfo) const {
    Cellule<TypeInfo>* ptrCurrentCell = ptrTete;
    int pos = 1;
    while(ptrCurrentCell != NULL && uneInfo > ptrCurrentCell->getInfo()){
        pos++;
        ptrCurrentCell = ptrCurrentCell->getRefSuivante();
    }
    if (ptrCurrentCell == NULL) return -pos;
    return uneInfo == ptrCurrentCell->getInfo() ? pos : -pos;

} // end getPositIter

template<class TypeInfo>
void ListeTrieeChainee<TypeInfo>::afficheCroissantRec() const {
    afficheCroissantRecWorker(ptrTete);
} // end afficheCroissantRec

template<class TypeInfo>
bool ListeTrieeChainee<TypeInfo>::estVide() const {
    return nbCellules == 0;
} // end estVide

template<class TypeInfo>
int ListeTrieeChainee<TypeInfo>::getLongueur() const {
    return nbCellules;
} // end getLongueur

/*
 * Insertion toujours possible (à moins qu'il n'y ait plus de place dans le tas)
 */
template<class TypeInfo>
void ListeTrieeChainee<TypeInfo>::insereRecWorker(Cellule<TypeInfo>*& ptrCetteListe, const TypeInfo& nouvelleInfo) {

    if (!ptrCetteListe){
        ptrCetteListe = new Cellule<TypeInfo>(nouvelleInfo);
        nbCellules++;
    }else if(nouvelleInfo < ptrCetteListe->getInfo()) {
        Cellule<TypeInfo> *newCell = new Cellule<TypeInfo>();
        newCell->setSuivante(ptrCetteListe->getRefSuivante());
        newCell->setInfo(ptrCetteListe->getInfo());
        ptrCetteListe->setInfo(nouvelleInfo);
        ptrCetteListe->setSuivante(newCell);
        nbCellules++;
    }else{
        insereRecWorker(ptrCetteListe->getRefSuivante(), nouvelleInfo);
    }
}

/**
 * Suppression de la première occurrence de uneInfo si présente et retour true, si absente retour false
 * @tparam TypeInfo
 * @param ptrCetteListe liste de Cellules à traiter
 * @param uneInfo information à supprimer si présente
 * @return true si uneInfo présente, false sinon
 */
template<class TypeInfo>
bool ListeTrieeChainee<TypeInfo>::supprimePremOccInfoRecWorker(Cellule<TypeInfo>*& ptrCetteListe, const TypeInfo & uneInfo) {

    if (!ptrCetteListe){
        return false;
    }else if(ptrCetteListe->getInfo() == uneInfo){
        supprimeTeteWorker(ptrCetteListe);
        return true;
    }else
        supprimePremOccInfoRecWorker(ptrCetteListe->getRefSuivante(), uneInfo);
}

template<class TypeInfo>
bool ListeTrieeChainee<TypeInfo>::supprimePremOccInfoRec(const TypeInfo & uneInfo) {
    return supprimePremOccInfoRecWorker(ptrTete, uneInfo);
}

/**
 * Suppression de la première occurrence de uneInfo si présente et retour true, si absente retour false
 * @tparam TypeInfo
 * @param uneInfo information à supprimer
 * @return true si uneInfo présente, false sinon
 */
template<class TypeInfo>
bool ListeTrieeChainee<TypeInfo>::supprimePremOccInfoIter(const TypeInfo& uneInfo) {

    Cellule<TypeInfo> *ptrCurrentCell = ptrTete->getRefSuivante();
    Cellule<TypeInfo> *ptrPrecedCellue = ptrTete;

    if(ptrPrecedCellue->getInfo() == uneInfo){
        ptrTete = ptrCurrentCell;
        ptrPrecedCellue->setSuivante(nullptr);
        delete ptrPrecedCellue;
        nbCellules--;
        return true;
    }

    while(ptrCurrentCell && ptrCurrentCell->getInfo() != uneInfo){
        ptrCurrentCell = ptrCurrentCell->getRefSuivante();
        ptrPrecedCellue = ptrPrecedCellue->getRefSuivante();
    }
    if (ptrCurrentCell){
        ptrPrecedCellue->setSuivante(ptrCurrentCell->getRefSuivante());
        ptrCurrentCell->setSuivante(nullptr);
        delete ptrCurrentCell;
        nbCellules--;
        return true;
    }
    return false;

}

template<class TypeInfo>
void ListeTrieeChainee<TypeInfo>::supprimeTeteWorker(Cellule<TypeInfo> *&ptrCetteListe) {
    //  pointeur temporaire sur la première cellule
    Cellule<TypeInfo> *ptrCelluleASupprimer = ptrCetteListe;
    // si la liste n'est pas videRec
    if (ptrCetteListe) {
        // la cellule suivante de la première cellule devient la nouvelle première cellule
        // MODIFICATION du paramètre résultat
        ptrCetteListe = ptrCetteListe->getSuivante();
        // rendre au tas la cellule à supprimer
        ptrCelluleASupprimer->setSuivante(nullptr);
        delete ptrCelluleASupprimer;
        ptrCelluleASupprimer = nullptr;
        nbCellules--; // une cellule en moins
    }
    // si la liste est déjà videRec, ne rien faire
} // end supprimeTeteWorker

template<class TypeInfo>
void ListeTrieeChainee<TypeInfo>::videRecWorker(Cellule<TypeInfo> *ptrCetteListe) {
    // > ptrCetteListe == nullptr => rien {la liste est videRec} *
    // > ptrCetteListe != nullptr => supprimeTete(ptrCetteListe); nbcellules--; videRecWorker(ptrCetteListe);

    if (ptrCetteListe != nullptr) {
        // supprimer en tête
        supprimeTeteWorker(ptrCetteListe);
        nbCellules--;
        // vider la liste qui reste
        videRecWorker(ptrCetteListe);
    } // end if
} // end videRec

template<class TypeInfo>
void ListeTrieeChainee<TypeInfo>::videRec() {
    videRecWorker(ptrTete);
    ptrTete = nullptr;
} // end videRec

template<class TypeInfo>
void ListeTrieeChainee<TypeInfo>::afficheCroissantRecWorker(const Cellule<TypeInfo>* ptrCetteListe) const {
    if (ptrCetteListe) {
        cout << ptrCetteListe->getInfo() << " ";
        afficheCroissantRecWorker(ptrCetteListe->getSuivante());
    }
} // end afficheCroissantRec

/**
 * vérifie que cette liste est un ensemble : elle est triée croissante sans duplication (ie triée au sens strict)
 * @tparam TypeInfo
 * @return true si ensemble, false sinon
 */
template<class TypeInfo>
bool ListeTrieeChainee<TypeInfo>::estEnsemble() const {
    if (!ptrTete){
        return true;
    }
    Cellule<TypeInfo> *ptrCurrentCell = ptrTete->getRefSuivante();
    Cellule<TypeInfo> *ptrPrecedCellue = ptrTete;

    while(ptrCurrentCell && ptrPrecedCellue->getInfo() != ptrCurrentCell->getInfo()){
        ptrCurrentCell = ptrCurrentCell->getRefSuivante();
        ptrPrecedCellue = ptrPrecedCellue->getRefSuivante();
    }

    return !ptrCurrentCell;
}

/**
 * Intersection de cet ensemble avec ensembleB
 * @pre  cette liste triée est un ensemble, ensembleB est un ensemble
 * @post  le résultat est un ensemble
 * @param ensembleB
 * @return une liste chaînée triée qui est un ensemble intersection de cet ensemble et ensembleB
 */
template<class TypeInfo>
ListeTrieeChainee<TypeInfo>* ListeTrieeChainee<TypeInfo>::insersectionAvec(const ListeTrieeChainee<TypeInfo>* ensembleB) const {
    // l'idée est de mettre, si possible une première Cellule dans EnsembleResultat
    // puis on insérera successivment derière la dernière Cellule de EnsembleResultat

    // intitialisation de l'ensemble résultat, il ne contient aucune Cellule son ptrTete == nullptr
    ListeTrieeChainee<TypeInfo>* ptrEnsembleResultat = new ListeTrieeChainee<TypeInfo>();

    //pointeur sur la première Cellule de chacun des deux ensembles
    Cellule<TypeInfo>* ptrListeEnsembleA = this->ptrTete;
    Cellule<TypeInfo>* ptrListeEnsembleB = ensembleB->ptrTete;
    Cellule<TypeInfo>* currentResultCell = nullptr;

    while (ptrListeEnsembleA && ptrListeEnsembleB) {
        TypeInfo infoA = ptrListeEnsembleA->getInfo();
        TypeInfo infoB = ptrListeEnsembleB->getInfo();

        if(infoA == infoB) {
            Cellule<TypeInfo> *newCell = new Cellule<TypeInfo>(infoA);
            if (!currentResultCell) {
                ptrEnsembleResultat->ptrTete = newCell;
                currentResultCell = ptrEnsembleResultat->ptrTete;
            } else {
                currentResultCell->setSuivante(newCell);
                currentResultCell = currentResultCell->getRefSuivante();
            }
            ptrEnsembleResultat->nbCellules += 1;
        }
        if (infoA < infoB) {
            ptrListeEnsembleA = ptrListeEnsembleA->getRefSuivante();
        }else if(infoA == infoB){
            ptrListeEnsembleB = ptrListeEnsembleB->getRefSuivante();
            ptrListeEnsembleA = ptrListeEnsembleA->getRefSuivante();
        }else{
            ptrListeEnsembleB = ptrListeEnsembleB->getRefSuivante();
        }
    }

    ptrEnsembleResultat->supprimeToutesDuplications();

    return ptrEnsembleResultat;
} // end intersectionAvec

/**
  * Union de cet ensemble avec ensembleB
  * @pre  cette liste triée est un ensemble, ensembleB est un ensemble
  * @post  le résultat est un ensemble
  * @param ensembleB
  * @return une liste chaînée triée qui est un ensemble union de cet ensemble et ensembleB
  */
template<class TypeInfo>
ListeTrieeChainee<TypeInfo>* ListeTrieeChainee<TypeInfo>::unionAvec(const ListeTrieeChainee<TypeInfo>* ensembleB) const {
    // l'idée est de mettre, si possible une première Cellule dans EnsembleResultat
    // puis on insérera successivment derière la dernière Cellule de EnsembleResultat

    // intitialisation de l'ensemble résultat, il ne contient aucune Cellule son ptrTete == nullptr
    ListeTrieeChainee<TypeInfo>* ptrEnsembleResultat = new ListeTrieeChainee<TypeInfo>();

    //pointeur sur la première Cellule de chacun des deux ensembles
    Cellule<TypeInfo>* ptrListeEnsembleA = this->ptrTete;
    Cellule<TypeInfo>* ptrListeEnsembleB = ensembleB->ptrTete;
    Cellule<TypeInfo>* currentResultCell = nullptr;


    while (ptrListeEnsembleA || ptrListeEnsembleB) {
        TypeInfo infoA = (ptrListeEnsembleA ? ptrListeEnsembleA->getInfo() : NULL);
        TypeInfo infoB = (ptrListeEnsembleB ? ptrListeEnsembleB->getInfo() : NULL);
        Cellule<TypeInfo>* newCell;

        if(infoA == NULL){
            newCell = new Cellule<TypeInfo>(infoB);
        }else if(infoB == NULL){
            newCell = new Cellule<TypeInfo>(infoA);
        } else{
            newCell = new Cellule<TypeInfo>(infoA < infoB ? infoA : infoB);
        }
        if (!currentResultCell) {
            ptrEnsembleResultat->ptrTete = newCell;
            currentResultCell = ptrEnsembleResultat->ptrTete;
        } else {
            currentResultCell->setSuivante(newCell);
            currentResultCell = currentResultCell->getRefSuivante();
        }

        ptrEnsembleResultat->nbCellules += 1;

        if (ptrListeEnsembleA && (infoB == NULL || infoA <= infoB)) {
            ptrListeEnsembleA = ptrListeEnsembleA->getRefSuivante();
        }
        if (ptrListeEnsembleB && ( infoA == NULL || infoB <= infoA)) {
            ptrListeEnsembleB = ptrListeEnsembleB->getRefSuivante();
        }
    }

    ptrEnsembleResultat->supprimeToutesDuplications();


    //on a terminé
    return ptrEnsembleResultat;
} // end unionAvec

//  Fin de l'implémentation.

#endif