#include "kmeans.h"
#include <thread>
#include <future>
#include <mutex>
#include <cmath>
#include <limits>
#include <vector>
#include <stdexcept>

vector<Centroide> criarCentroidesAleatorios(int numeroK, vector<Instancia>& instancias){
   vector<Centroide> centroides;
   vector<future<Centroide>> futures;
   mutex mutex;

   for (int i = 0; i < numeroK; ++i) {
      futures.push_back(async(launch::async, [i, &instancias]() {
         return Centroide::criarCentroideAleatorio(i, instancias);
      }));
   }

   for (auto& fut : futures) {
      Centroide centroide = fut.get();
      lock_guard<std::mutex> lock(mutex);
      centroides.push_back(move(centroide));
   }

   return centroides;
}

double calcularDistanciaEuclidiana(vector<double> vetorInstancia, vector<double> vetorCentroide){
   double distancia = 0.0;

   for(int i = 0; i < vetorInstancia.size(); i++){
      distancia += pow((vetorCentroide[i] - vetorInstancia[i]),2);
   }

   return sqrt(distancia);
}

void calcularCentroidesProximos(vector<Centroide>& centroides, const vector<Instancia>& instancias) {
   vector<future<void>> futures;
    mutex mtx;  // Mutex para sincronizar o acesso aos centroides

    auto calcularCentroideProximo = [&](const Instancia& instancia) {
        int indiceCentroideProximo = -1;
        double menorDistancia = numeric_limits<double>::max();

        for (size_t i = 0; i < centroides.size(); ++i) {
            double distancia = calcularDistanciaEuclidiana(instancia.getAtributos(), centroides[i].getAtributos());
            if (distancia < menorDistancia) {
                menorDistancia = distancia;
                indiceCentroideProximo = i;
            }
        }

        if (indiceCentroideProximo != -1) {
            lock_guard<mutex> lock(mtx);  // Protege a operação de adição
            centroides[indiceCentroideProximo].adicionarInstancia(instancia);
        }
    };

    for (const auto& instancia : instancias) {
        futures.push_back(async(launch::async, calcularCentroideProximo, cref(instancia)));
    }

    for (auto& fut : futures) {
        fut.get();
    }
}

/*

1. Inicialize k centróides (μ1, μ2, ..., μk) aleatoriamente.
2. Para cada ponto de dados xi:
   a. Calcule a distância de xi para cada centróide μj.
   b. Atribua xi ao cluster cj cujo centróide μj é o mais próximo.
3. Para cada cluster cj:
   a. Recalcule a posição do centróide μj como a média dos pontos de dados atribuídos a cj.
4. Repita os passos 2 e 3 até que os centróides não mudem significativamente ou um número máximo de iterações seja atingido.

*/