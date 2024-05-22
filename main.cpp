#include "Library/instancia.h"
#include "Library/centroide.h"
#include "Library/kmeans.h"
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

int main() {

    //Parametros: Kmeans(Base de Dados, Numero de Clusters)
    //1 - Iris, 2 - MFeat
    kmeans(1,3);
    
    return 0;
}
