#ifndef KMEANS_H
#define KMEANS_H

#include "centroide.h"
#include <vector>

vector<Centroide> criarCentroidesAleatorios(int numeroK, vector<Instancia>& instancias);
double calcularDistanciaEuclidiana(Centroide centroide, Instancia instancia);
void calcularCentroidesProximos(vector<Centroide>& centroides, const vector<Instancia>& instancias, int estado);
void atualizarCentroides(vector<Centroide>& centroides);
bool verificarConvergencia(const vector<Centroide>& centroides, const vector<Centroide>& centroidesAntigos, double tolerancia);
void kmeans(int baseDeDados,int K);

#endif