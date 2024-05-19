#include "instancia.h"
#include "centroide.h"
#include "kmeans.h"
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

int main() {

    //Parametros: Kmeans(Base de Dados, Numero de Clusters)
    //1 - Iris, 2 - MFeat
    kmeans(2,10);

    return 0;
}