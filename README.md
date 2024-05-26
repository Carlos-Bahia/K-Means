# Projeto de Pesquisa em Agrupamento de Dados

Este repositório faz parte de um projeto de pesquisa em Agrupamento de Dados orientado pelo professor Renê Gusmão. O projeto implementa o algoritmo de clustering K-means em C++.

## Estrutura do Projeto

O projeto é composto pelos seguintes arquivos:

- `centroide.cpp` e `centroide.h`: Implementação da classe centroide, que lida com os centróides no processo de clustering.
- `instancia.cpp` e `instancia.h`: Implementação da classe instância, representando os pontos de dados a serem agrupados.
- `kmeans.cpp` e `kmeans.h`: Implementação do algoritmo K-means.
- `main.cpp`: O arquivo principal para executar o algoritmo K-means em um conjunto de dados.

## Instruções de Compilação

Para compilar o projeto, siga estes passos:

1. Certifique-se de ter um compilador C++ instalado (por exemplo, g++).
2. Abra um terminal e navegue até o diretório do projeto.
3. Execute o seguinte comando para compilar o projeto:

   ```sh
   g++ *.cpp -o kmeans

4. Execute o programa compilado:
   ```sh
   ./kmeans

## Uso

Para executar o programa nas bases de dados já disponíveis. Altere diretamente no arquivo main.cpp e então re-compile o programa.

## Resultados

Uma vez que o programa seja compilado e executado, será criado uma pasta chamada Output, contendo um arquivo .txt com os resultados da execução, incluindo tempos de execuções e informações dos centroides criados.


## Indices de Validação

Ao fim da execução do código, 5 indices de validação são executadas e adicionados ao arquivo de resultado.
Incluindo : Sillhouette, Davies-Bouldin, Calinski-Harabasz, F-measure e Adjusted Rand Index