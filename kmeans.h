#ifndef KMEANS_H
#define KMEANS_H

#include "centroide.h"
#include <vector>

vector<Centroide> criarCentroidesAleatorios(int numeroK, vector<Instancia>& instancias);
double calcularDistanciaEuclidiana(Centroide centroide, Instancia instancia);
void calcularCentroidesProximos(vector<Centroide>& centroides, const vector<Instancia>& instancias);

#endif