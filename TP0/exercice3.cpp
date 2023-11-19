#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

template<class T>
void afficheVecteur(const vector<T> v) {
    // {v.size() > 0} => {pretty print de v}
    cout << "[" << v[0];
    int i = 1;
    while (i < v.size()) {
        cout << ", " << v[i];
        i++;
    }
    cout << "]" << endl;
}

template<class T>
void minMaxIter(const vector<T> v, T &min, T &max, int &nbComp) {
    // {v.size()≥ 1} => {min = plus petite valeur de v,
    //                   max = plus grande valeur de v,
    //                   nbcomp = nombre de comparaisons impliquant au moins un élément de v}

    min = v[0];
    max = v[0];
    for (int i = 1; i < v.size(); ++i) {
        if (min > v[i])
            min = v[i];
        if(max < v[i])
            max = v[i];
        nbComp+=2;
    }

}

template<class T>
void minMaxRec(const vector<T>& v, int inf, int sup, T &min, T &max, int &nbComp) {
    // {v.size()≥ 1} => {min = plus petite valeur de v sur l'intervalle [inf..sup],
    //                   max = plus grande valeur de v sur l'intervalle [inf..sup],
    //                   nbcomp = nombre de comparaisons impliquant au moins un élément de v}

    if (sup - inf <= 1) { // vecteur de taille 1 ou 2
        nbComp += 1;
        if (v[inf] < v[sup]) {
            min = v[inf];
            max = v[sup];
        } else {
            min = v[sup];
            max = v[inf];
        }
    } else {
        int m = (sup + inf) / 2;
        T min1, max1, min2, max2;
        minMaxRec(v, inf, m, min1, max1, nbComp);
        minMaxRec(v, m + 1, sup, min2, max2, nbComp);

        nbComp += 2;
        min = (min1 < min2) ? min1 : min2;
        max = (max1 > max2) ? max1 : max2;
    }
}


void testMinMaxIter() {
    cout << "***************************" << endl;
    cout << "*        min max iter     *" << endl;
    cout << "***************************" << endl;

    int min;
    int max;
    int nbComp = 0;

    vector<int> v = {10};
    minMaxIter(v, min, max, nbComp);
    cout << "v : ";
    afficheVecteur(v);
    cout << "Taille du vecteur = " << v.size() << " -> " << " min = " << min << ", max = " << max << ", nbComp = " << nbComp << endl << endl;

    nbComp = 0;
    v = {10, 3, 6, 8, 67, 2, 9, 1};
    minMaxIter(v, min, max, nbComp);
    cout << "v : ";
    afficheVecteur(v);
    cout << "Taille du vecteur = " << v.size() << " -> " << " min = " << min << ", max = " << max << ", nbComp = " << nbComp << endl << endl;

    nbComp = 0;
    v.clear();
    srand((unsigned)time(NULL));
    for (int i =0; i < 128; i++){
        int b = rand() % 128 + 1;
        v.push_back(b);
    }
    minMaxIter(v, min, max, nbComp);
    cout << "v : ";
    afficheVecteur(v);
    cout << "Taille du vecteur = " << v.size() << " -> " << " min = " << min << ", max = " << max << ", nbComp = " << nbComp << endl << endl;
}

void testMinMaxDR() {
    cout << "***************************" << endl;
    cout << "*         min max DR      *" << endl;
    cout << "***************************" << endl;

    int min;
    int max;
    int inf;
    int sup;
    int nbComp = 0;

    vector<int> v = {10};
    inf = 0;
    sup = v.size()-1;
    minMaxRec(v, inf, sup, min, max, nbComp);
    cout << "v : ";
    afficheVecteur(v);
    cout << "Taille du vecteur = " << v.size() <<  " -> min = " << min << ", max = " << max << ", nbComp = " << nbComp << endl << endl;

    nbComp = 0;
    v = {10, 3, 6, 8, 67, 2, 9, 1};
    inf = 0;
    sup = v.size()-1;
    minMaxRec(v, inf, sup, min, max, nbComp);
    cout << "v : ";
    afficheVecteur(v);
    cout << "Taille du vecteur = " << v.size() <<  " -> min = " << min << ", max = " << max << ", nbComp = " << nbComp << endl << endl;

    nbComp = 0;
    v.clear();
    srand((unsigned)time(NULL));
    for (int i =0; i < 128; i++){
        int b = rand() % 128 + 1;
        v.push_back(b);
    }
    inf = 0;
    sup = v.size()-1;
    minMaxRec(v, inf, sup, min, max, nbComp);
    cout << "v : ";
    afficheVecteur(v);
    cout << "Taille du vecteur = " << v.size() << " -> min = " << min << ", max = " << max << ", nbComp = " << nbComp << endl << endl;
}

int main(int argc, char **argv) {

    testMinMaxIter();
    testMinMaxDR();

    return 0;
}
