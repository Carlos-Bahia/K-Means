#ifndef KMEANS_H
#define KMEANS_H

#include "centroide.h"
#include <vector>
#include <map>

vector<Centroide> criarCentroidesAleatorios(int numeroK, vector<Instancia>& instancias);
double calcularDistanciaEuclidiana(vector<double> vetorInstancia, vector<double> vetorCentroide);
void calcularCentroidesProximos(vector<Centroide>& centroides, const vector<Instancia>& instancias, int estado);
Centroide calcularCentroideMaisProximo(vector<Centroide>& centroides, const Instancia& instancia);
void atualizarCentroides(vector<Centroide>& centroides);
bool verificarConvergencia(const vector<Centroide>& centroides, const vector<Centroide>& centroidesAntigos, double tolerancia);
void kmeans(int baseDeDados,int K);
map<int, int> mapearMatrizEsperada(const vector<Centroide>& centroides, int baseDados);
map<int,int> mapearMatrizReal(const vector<Centroide>& centroides, int baseDados);
void imprimirMap(const map<int, int>& mapa);
double fmeasure(vector<Centroide>& centroides, int baseDados,const vector<Instancia>& instancias);
double daviesBouldin(const vector<Centroide>& centroides);
double distanciaIntraClusterDaviesBouldin(Centroide centroide);
double calinskiHarabasz(vector<Centroide> centroides, vector<Instancia> instancias);
vector<double> calcularCentroideGlobal(const vector<Instancia>& instancias);
vector<vector<int>> calcularMatrizDeContingencia(const map<int, int>& esperado, const map<int, int>& real,int baseDados);
void exibirMatrizDeContingencia(const std::vector<std::vector<int>>& matriz);
double adjustedRandIndex(const vector<Centroide>& centroides, int baseDados, const vector<Instancia>& instancias);

#endif