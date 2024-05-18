#include "instancia.h"

// Construtores
Instancia::Instancia(int id, vector<double> atributos) : id(id), atributos(atributos) {}

// Getters
int Instancia::getId() const {
    return id;
}

vector<double> Instancia::getAtributos() const {
    return atributos;
}

// Setters
void Instancia::setId(int id) {
    this->id = id;
}

void Instancia::setAtributos(const vector<double>& atributos) {
    this->atributos = atributos;
}

// Manipulação de Atributos
void Instancia::adicionarAtributo(double atributo) {
    atributos.push_back(atributo);
}

void Instancia::imprimirAtributos() const {
    cout << "ID: " << id << endl;
    cout << "Atributos: ";
    for (double atributo : atributos) {
        cout << atributo << " ";
    }
    cout << endl;
}