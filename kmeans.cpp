#include "Library/kmeans.h"
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

void calcularCentroidesProximos(vector<Centroide>& centroides, const vector<Instancia>& instancias, int estado) {
    bool needsRecalculation;

    do {
        needsRecalculation = false;
        vector<future<void>> futures;
        mutex mtx;  // Mutex para sincronizar o acesso aos centroides

        // Limpar instâncias anteriores em todos os centroides
        for (auto& centroide : centroides) {
            centroide.limparInstanciasProximas();
        }

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

        if(estado == 1){
            // Reinicializar centróides sem instâncias e marcar que precisamos recalcular
            for (auto& centroide : centroides) {
                if (centroide.getProximos().size() == 0) {
                    centroide = Centroide::criarCentroideAleatorio(centroide.getId(), instancias);
                    needsRecalculation = true;
                }
            }
        }
    } while (needsRecalculation);
}

void atualizarCentroides(vector<Centroide>& centroides) {
    vector<future<void>> futures;
    mutex mtx;

    auto atualizarCentroide = [&](Centroide& centroide) {
        vector<double> novaMedia(centroide.getAtributos().size(), 0.0);
        const auto& instancias = centroide.getProximos();

        if (instancias.empty()) return;

        for (const auto& instancia : instancias) {
            const auto& atributos = instancia.getAtributos();
            for (size_t i = 0; i < atributos.size(); ++i) {
                novaMedia[i] += atributos[i];
            }
        }

        for (double& valor : novaMedia) {
            valor /= instancias.size();
        }

        lock_guard<mutex> lock(mtx);
        centroide.setAtributos(move(novaMedia));
        centroide.limparInstanciasProximas();
    };

    for (auto& centroide : centroides) {
        futures.push_back(async(launch::async, atualizarCentroide, ref(centroide)));
    }

    for (auto& fut : futures) {
        fut.get();
    }
}

bool verificarConvergencia(const vector<Centroide>& centroides, const vector<Centroide>& centroidesAntigos, double tolerancia = 1e-6) {
    if (centroides.size() != centroidesAntigos.size()) {
        throw invalid_argument("Os vetores de centroides atuais e antigos devem ter o mesmo tamanho.");
    }

    auto verificarCentroide = [&](size_t start, size_t end) {
        for (size_t i = start; i < end; ++i) {
            const vector<double>& atributosAntigos = centroidesAntigos[i].getAtributos();
            const vector<double>& atributos = centroides[i].getAtributos();

            if (atributos.size() != atributosAntigos.size()) {
                throw invalid_argument("Os vetores de atributos dos centroides devem ter o mesmo tamanho.");
            }

            for (size_t j = 0; j < atributos.size(); ++j) {
                if (abs(atributos[j] - atributosAntigos[j]) > tolerancia) {
                    return false;
                }
            }
        }
        return true;
    };

    size_t numThreads = thread::hardware_concurrency();
    size_t tamanho = centroides.size();
    size_t chunkSize = (tamanho + numThreads - 1) / numThreads;  // Divisão arredondada para cima

    vector<future<bool>> futures;

    for (size_t i = 0; i < numThreads; ++i) {
        size_t start = i * chunkSize;
        size_t end = min(start + chunkSize, tamanho);
        if (start < end) {
            futures.push_back(async(launch::async, verificarCentroide, start, end));
        }
    }

    for (auto& fut : futures) {
        if (!fut.get()) {
            return false;
        }
    }

    return true;
}


void kmeans(int baseDeDados, int K){

    auto start = chrono::high_resolution_clock::now();

    vector<Instancia> instancias;
    if(baseDeDados == 1){
        instancias = Instancia::lerIris();
    } else if (baseDeDados == 2){
        instancias = Instancia::lerMFeat();
    } else{
        cout << "Opção inválida!" << endl;
        cout << "Finalizando Programa." << endl;
        return;
    }


    auto endInstancias = chrono::high_resolution_clock::now();

    vector<Centroide> centroides = criarCentroidesAleatorios(K, instancias);
    vector<Centroide> centroidesAntigo;
    
    calcularCentroidesProximos(centroides, instancias, 1);
    atualizarCentroides(centroides);

    do{
        centroidesAntigo = centroides;
        calcularCentroidesProximos(centroides, instancias, 0);
        atualizarCentroides(centroides);
    }while(!verificarConvergencia(centroides, centroidesAntigo, 0.001));
        calcularCentroidesProximos(centroides,instancias, 0);


    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);
    auto durationInstancias = chrono::duration_cast<chrono::milliseconds>(endInstancias-start);
    auto durationCentroides = chrono::duration_cast<chrono::milliseconds>(end-endInstancias);

    vector<chrono::milliseconds> durations;
    durations.push_back(duration);
    durations.push_back(durationInstancias);
    durations.push_back(durationCentroides);

    Centroide::escreverCentroidesComInstancias(centroides, "centroides.txt", durations);
}
