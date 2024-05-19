#ifndef K_MEANS_CENTROIDE_H
#define K_MEANS_CENTROIDE_H

#include "instancia.h"
#include <vector>

class Centroide{
    private:
        int id;
        vector<double> atributos;
        vector<Instancia> instancias_proximas;

    public:
    // Construtores
    Centroide() = default;
    Centroide(int id, vector<double> atributos, vector<Instancia> instacias_proximas);

    // Getters
    int getId() const;
    vector<double> getAtributos() const;
    vector<Instancia> getProximos() const;

    // Setters
    void setId(int id);
    void setAtributos(const vector<double>& atributos);
    void setProximos(const vector<Instancia>& proximos);
    void adicionarInstancia(const Instancia& instancia);

    // Função para criar centroide aleatorio
    static Centroide criarCentroideAleatorio(int id, vector<Instancia> instancias);

    //Função para escrever arquivo com os centroides
    static void escreverCentroide(const vector<Centroide>& centroides, const string& nome_arquivo);
    static void escreverCentroidesComInstancias(const vector<Centroide>& centroides, const string& nome_arquivo);
};

#endif