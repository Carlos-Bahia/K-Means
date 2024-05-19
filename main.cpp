#include "instancia.h"
#include "centroide.h"
#include "kmeans.h"
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

int main() {

    auto start = chrono::high_resolution_clock::now();

    vector<Instancia> instanciasIris = Instancia::lerIris();
    vector<Instancia> instanciasMFeat = Instancia::lerMFeat();
    vector<Centroide> centroidesIris = criarCentroidesAleatorios(3,instanciasIris);
    vector<Centroide> centroidesMfeat = criarCentroidesAleatorios(10, instanciasMFeat);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);

    start = chrono::high_resolution_clock::now();

    calcularCentroidesProximos(centroidesIris, instanciasIris);
    calcularCentroidesProximos(centroidesMfeat, instanciasMFeat);

    end = chrono::high_resolution_clock::now();

    auto durationCalculoInicial = chrono::duration_cast<chrono::milliseconds>(end-start);

    start = chrono::high_resolution_clock::now();

    Instancia::escreverInstancias(instanciasIris, "instancias_iris.txt");
    Instancia::escreverInstancias(instanciasMFeat, "instancias_mfeat.txt");
    Centroide::escreverCentroide(centroidesIris, "centroides_iris.txt");
    Centroide::escreverCentroide(centroidesMfeat, "centroides_mfeat.txt");
    Centroide::escreverCentroidesComInstancias(centroidesIris, "centroides_iris_instancias.txt");
    Centroide::escreverCentroidesComInstancias(centroidesMfeat, "centroides_mfeat_instancias.txt");

    end = chrono::high_resolution_clock::now();
    auto durationEscrita = chrono::duration_cast<chrono::milliseconds>(end-start);

    cout << "K-Means Finalizado." << endl;
    cout << "Informações importantes:" << endl;
    cout << "Tempo total de execução: " << duration.count() << " milissegundos." << endl;
    cout << "Tempo total de calculo de Instancias Proximas: " << durationCalculoInicial.count() << " milissegundos." << endl;
    cout << "Tempo total de escrita de arquivos: " << durationEscrita.count() << " milissegundos." << endl;
    return 0;
}