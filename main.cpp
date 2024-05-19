#include "instancia.h"
#include "centroide.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<Instancia> instanciasIris = Instancia::lerIris();

    Instancia::escreverInstancias(instanciasIris, "instancias_iris.txt");

    vector<Instancia> instanciasMFeat = Instancia::lerMFeat();

    Instancia::escreverInstancias(instanciasMFeat, "instancias_mfeat.txt");

    vector<Centroide> centroidesIris;
    vector<Centroide> centroidesMfeat;

    Centroide centroideIris = Centroide::criarCentroideAleatorio(0, instanciasIris);
    centroidesIris.push_back(centroideIris);

    Centroide::escreverCentroide(centroidesIris, "centroides_iris.txt");

    Centroide centroideMfeat = Centroide::criarCentroideAleatorio(0, instanciasMFeat);
    centroidesMfeat.push_back(centroideMfeat);

    Centroide::escreverCentroide(centroidesMfeat, "centroides_mfeat.txt");

    return 0;
}