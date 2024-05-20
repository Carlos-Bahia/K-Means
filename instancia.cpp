#include "Library/instancia.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

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

vector<vector<double>> lerArquivo(const string& caminho) {
    vector<vector<double>> linhas;
    ifstream arquivo(caminho);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << caminho << endl;
        return linhas;
    }

    string linha;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string item;
        vector<double> atributos;
        while (ss >> item) {
            atributos.push_back(stod(item));
        }
        linhas.push_back(move(atributos));
    }

    arquivo.close();
    return linhas;
}

vector<Instancia> Instancia::lerIris() {
    vector<Instancia> instancias;

    ifstream arquivo("Iris/iris.data");

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo" << endl;
        return instancias;
    }

    string linha;
    int id = 0;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        string item;
        vector<double> atributos;

        // Lê os primeiros quatro atributos numéricos
        for (int i = 0; i < 4; ++i) {
            if (getline(ss, item, ',')) {
                atributos.push_back(stod(item));
            }
        }

        // Ignora o rótulo da classe
        if (getline(ss, item, ',')) {
            // Aqui o item contém o rótulo da classe, se necessário
        }

        Instancia instancia(id, atributos);
        instancias.push_back(move(instancia));
        ++id;
    }

    arquivo.close();

    return instancias;
}

vector<Instancia> Instancia::lerMFeat() {
    vector<Instancia> instancias;

    // Vetores para armazenar os dados lidos de cada arquivo
    vector<vector<double>> fou = lerArquivo("Mfeat/mfeat-fou");
    vector<vector<double>> fac = lerArquivo("Mfeat/mfeat-fac");
    vector<vector<double>> kar = lerArquivo("Mfeat/mfeat-kar");
    vector<vector<double>> pix = lerArquivo("Mfeat/mfeat-pix");
    vector<vector<double>> zer = lerArquivo("Mfeat/mfeat-zer");
    vector<vector<double>> mor = lerArquivo("Mfeat/mfeat-mor");

    // Verifica se todos os arquivos têm o mesmo número de linhas
    size_t numInstancias = fou.size();
    if (fac.size() != numInstancias || kar.size() != numInstancias || pix.size() != numInstancias || zer.size() != numInstancias || mor.size() != numInstancias) {
        cerr << "Erro: os arquivos não possuem o mesmo número de linhas." << endl;
        return instancias;
    }

    // Combina os atributos de cada linha de todos os arquivos em uma única instância
    for (size_t i = 0; i < numInstancias; ++i) {
        vector<double> atributos;
        atributos.insert(atributos.end(), fou[i].begin(), fou[i].end());
        atributos.insert(atributos.end(), fac[i].begin(), fac[i].end());
        atributos.insert(atributos.end(), kar[i].begin(), kar[i].end());
        atributos.insert(atributos.end(), pix[i].begin(), pix[i].end());
        atributos.insert(atributos.end(), zer[i].begin(), zer[i].end());
        atributos.insert(atributos.end(), mor[i].begin(), mor[i].end());

        Instancia instancia(i, atributos);
        instancias.push_back(move(instancia));
    }

    return instancias;
}

void Instancia::escreverInstancias(const vector<Instancia>& instancias, const string& nome_arquivo) {
    string pasta = "OutputTeste";
    fs::path directory = pasta;

    // Cria a pasta se ela não existir
    if (!fs::exists(directory)) {
        if (!fs::create_directories(directory)) {
            cerr << "Erro ao criar a pasta: " << directory << endl;
            return;
        }
    }

    fs::path caminho_arquivo = directory / nome_arquivo;

    ofstream arquivo(caminho_arquivo);

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo para escrita: " << caminho_arquivo << endl;
        return;
    }

    for (const Instancia& instancia : instancias) {
        arquivo << "Instancia: ";
        for (double atributo : instancia.getAtributos()) {
            arquivo << fixed << setprecision(2) << atributo << " ";
        }
        arquivo << endl;
    }

    arquivo.close();
}
