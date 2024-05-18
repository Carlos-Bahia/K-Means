#ifndef K_MEANS_INSTANCIA_H
#define K_MEANS_INSTANCIA_H

#include <iostream>
#include <vector>

using namespace std;

class Instancia {
    private:
        int id;
        vector<double> atributos;

    public:

    // Construtores
    Instancia(int id, vector<double> atributos);

    // Getters
    int getId() const;
    vector<double> getAtributos() const;

    // Setters
    void setId(int id);
    void setAtributos(const vector<double>& atributos);

    // Manipulação de Atributos
    void adicionarAtributo(double atributo);
    void imprimirAtributos() const;

    // Manipulação de Arquivos
    static void lerIris();
    static void lerMFeat();
};

#endif