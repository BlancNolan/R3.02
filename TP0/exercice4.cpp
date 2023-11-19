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
void swap(vector<T> &v, const int first, const int second) {
    // {v.size() >0} => {permutation des valeurs de v aux indices first et second}

    int temp = v[first];
    v[first] = v[second];
    v[second] = temp;

}

template<class T>
T partage(vector<T> &v, const int inf, const int sup, int &nbComp) {
// {v.size()≥ 1} => {résultat = valeur du médian tel que défini dans l’étape 1)
//                   et mise en place du plus petit et du plus grand dans v telles que définies dans l’étape 2)}

    int m = (inf+sup)/2;
    int valPartage;
    int petit;
    int grand;
    nbComp++;
    if(v[inf] < v[m]){
        nbComp++;
        if (v[m]<v[sup]) valPartage = v[m], petit = v[inf], grand = v[sup];
        else{
            std::tie(valPartage, petit, grand) = (v[sup] < v[inf]) ? std::make_tuple(v[inf], v[sup], v[m]): std::make_tuple(v[sup], v[inf], v[m]);
            nbComp++;
        }
    }else{
        nbComp++;
        if (v[inf] < v[sup]) valPartage = v[inf], petit = v[m], grand = v[sup];
        else{
            std::tie(valPartage, petit, grand) = (v[m]<v[sup]) ? std::make_tuple(v[sup], v[m], v[inf]) : std::make_tuple(v[m], v[sup], v[inf]);
            nbComp++;
        }
    }

    v[inf] = petit;
    v[sup] = grand;
    int temp = v[sup-1];
    v[sup-1] = valPartage;
    v[m] = temp;

    return valPartage;
}

template<class T>
void insertionSort(vector<T> &a, const int inf, const int sup, int &nbComp) {

    for (int i = inf+1; i <= sup; ++i) {
        T key = a[i];
        int j = i - 1;

        // Déplacement des éléments du vecteur pour faire de la place pour la clé

        while (j >= 0 && a[j] > key) {
            nbComp++;
            a[j + 1] = a[j];
            --j;
        }
        if(j >= 0) nbComp++;

        // Placement de la clé dans la position correcte
        a[j + 1] = key;
    }
}


template<class T>
void triR302(vector<T> &v, const int inf, const int sup, int &nbComp) {


    if( inf+6 > sup){
        insertionSort(v, inf, sup, nbComp);
    }else{
        //1&2)
        int valpartage = partage(v, inf, sup, nbComp);
        //3)
        int indiceSep = inf+1;
        for (int i = inf+1; i <= (sup-2) ; ++i) {
            nbComp++;
            if(v[i] < valpartage){
                swap(v, i, indiceSep);
                indiceSep++;
            }
        }

        //4)
        swap(v, indiceSep, sup-1);

        //5)
        triR302(v, inf, indiceSep-1, nbComp);
        triR302(v, indiceSep+1, sup, nbComp);
    }

}

void testTriR302() {
    cout << "***************************" << endl;
    cout << "*          triR302        *" << endl;
    cout << "***************************" << endl;
    vector<int> v = {10, 3, 6, 8, 67, 2, 9, 1};
    int nbComp = 0;
    cout << "vecteur initial : ";
    afficheVecteur(v);
    triR302(v, 0, (int) v.size() - 1, nbComp);
    cout << "vecteur trié : ";
    afficheVecteur(v);
    cout << "comparaison : "<< nbComp << endl;


    v.clear();
    srand((unsigned) time(NULL));
    for (int i = 0; i < 128; i++) {
        int b = rand() % 128 + 1;
        v.push_back(b);
    }
    nbComp = 0;
    cout << "vecteur initial : ";
    afficheVecteur(v);
    triR302(v, 0, (int) v.size() - 1, nbComp);
    cout << "vecteur trié : ";
    afficheVecteur(v);
    cout << "comparaison : "<< nbComp << endl;
}

int main(int argc, char **argv) {
    testTriR302();
    return 0;
}