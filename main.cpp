#include "instancia.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<Instancia> instanciasIris = Instancia::lerIris();

    Instancia::escreverInstancias(instanciasIris, "instancias_iris.txt");

    vector<Instancia> instanciasMFeat = Instancia::lerMFeat();

    Instancia::escreverInstancias(instanciasMFeat, "instancias_mfeat.txt");

    return 0;
}