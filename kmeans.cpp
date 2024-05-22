#include "Library/kmeans.h"
#include <thread>
#include <future>
#include <mutex>
#include <map>
#include <cmath>
#include <numeric>
#include <limits>
#include <vector>
#include <set>
#include <stdexcept>
#include <algorithm>

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

Centroide calcularCentroideMaisProximo(vector<Centroide>& centroides, const Instancia& instancia) {
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
        return centroides[indiceCentroideProximo];
    } else {
        throw runtime_error("Nenhum centroide encontrado.");
    }
}

//Implementando indice da Silhueta
//silhouette Measure

double silhouetteMeasure(const vector<Centroide>& centroides) {
    vector<double> silhouettesA;

    for (const Centroide& centroide : centroides) {
        vector<Instancia> instancias = centroide.getProximos();

        for (size_t i = 0; i < instancias.size(); ++i) {
            double distanciaTotal = 0.0;

            for (size_t j = 0; j < instancias.size(); ++j) {
                if (i != j) {
                    double distanciaTemp = calcularDistanciaEuclidiana(instancias[i].getAtributos(), instancias[j].getAtributos());
                    distanciaTotal += distanciaTemp;
                }
            }
            silhouettesA.push_back(distanciaTotal / (instancias.size() - 1));
        }
    }

    vector<double> silhouettesB;

    for (size_t i = 0; i < centroides.size(); ++i) {
        vector<Centroide> centroidesTemp;

        for (size_t j = 0; j < centroides.size(); ++j) {
            if (centroides[j].getAtributos() != centroides[i].getAtributos()) {
                centroidesTemp.push_back(centroides[j]);
            }
        }

        vector<Instancia> instancias = centroides[i].getProximos();

        for (size_t j = 0; j < instancias.size(); ++j) {
            Centroide centroideProximo = calcularCentroideMaisProximo(centroidesTemp, instancias[j]);
            vector<Instancia> instanciasProximas = centroideProximo.getProximos();
            double distanciaTotal = 0.0;

            for (size_t k = 0; k < instanciasProximas.size(); ++k) {
                double temp = calcularDistanciaEuclidiana(instancias[j].getAtributos(), instanciasProximas[k].getAtributos());
                distanciaTotal += temp;
            }
            silhouettesB.push_back(distanciaTotal / instanciasProximas.size());
        }
    }

    vector<double> silhouette;

    for (size_t i = 0; i < silhouettesA.size(); ++i) {
        double temp = (silhouettesB[i] - silhouettesA[i]) / max(silhouettesA[i], silhouettesB[i]);
        silhouette.push_back(temp);
    }

    double media = accumulate(silhouette.begin(), silhouette.end(), 0.0);

    return media / silhouette.size();
}

map<int, int> mapearMatrizEsperada(const int baseDados) {
    map<int, int> resultado;

    if (baseDados == 1) {
        for (int i = 0; i < 2000; i++) {
            resultado[i] = i / 200;
        }
    } else if (baseDados == 2) {
        for (int i = 0; i < 150; i++) {
            resultado[i] = i / 50;
        }
    }

    return resultado;
}

map<int,int> mapearMatrizReal(const vector<Centroide>& centroides, int baseDados){
    map<int,int> resultado;
    int numInteracoes;

    if(baseDados == 1){
        numInteracoes = 150;
    } else{
        numInteracoes = 2000;
    }

    for(int j = 0; j < centroides.size(); j++){
        vector<Instancia> instancias = centroides[j].getProximos();
        for(int i = 0; i < instancias.size(); i++){
            resultado[instancias[i].getId()] = j;
        }
    }
    
    return resultado;
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

    double silhouette = silhouetteMeasure(centroides);
    double medidaF = fmeasure(centroides, baseDeDados);

    vector<double> indices;
    indices.push_back(move(silhouette));
    indices.push_back(move(medidaF));

    Centroide::escreverCentroidesComInstancias(centroides, durations, indices);
}

double fmeasure(const vector<Centroide>& centroides, int baseDados){
    map<int,int> matrizEsperada = mapearMatrizEsperada(baseDados);
    map<int,int> matrizReal = mapearMatrizReal(centroides, baseDados);

    int TP = 0;
    int FP = 0;
    int FN = 0;

    for(const auto& par : matrizEsperada){
        int id = par.first;
        int classEsperada = par.second;
        int classReal = matrizReal[id];
    

        if(classReal == classEsperada){
            TP++;
        } else{
            FP++;
            FN++;
        }
    }

    double precision = TP / double(TP+FP);
    double recall = TP / double(TP+FN);
    double f1 = 2 * (precision * recall) / (precision + recall);

    return f1;
}

void imprimirMap(const map<int, int>& mapa) {
    for (const auto& par : mapa) {
        cout << "Chave: " << par.first << " - Valor: " << par.second << endl;
    }
}
