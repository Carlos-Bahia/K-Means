#ifndef KMEANS_H
#define KMEANS_H

#include "centroide.h"
#include <vector>

vector<Centroide> criarCentroidesAleatorios(int numeroK, vector<Instancia>& instancias);
double calcularDistanciaEuclidiana(vector<double> vetorInstancia, vector<double> vetorCentroide);
void calcularCentroidesProximos(vector<Centroide>& centroides, const vector<Instancia>& instancias, int estado);
Centroide calcularCentroideMaisProximo(vector<Centroide>& centroides, const Instancia& instancia);
void atualizarCentroides(vector<Centroide>& centroides);
bool verificarConvergencia(const vector<Centroide>& centroides, const vector<Centroide>& centroidesAntigos, double tolerancia);
void kmeans(int baseDeDados,int K);

#endif