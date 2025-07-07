//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <queue>
#include <list>
#include <utility> 
#include <limits.h>
#include <limits>
#include <unordered_map>
#include <unordered_set>

#include "Hash.h"
#include "par.h"

using namespace std;

class Grafo {
public:
    Grafo();
    ~Grafo();

    vector<char> fecho_transitivo_direto(char id_no);               // a
    vector<char> fecho_transitivo_indireto(char id_no);             // b
    vector<char> caminho_minimo_dijkstra(char id_no_a, char id_no_b); // c
    vector<char> caminho_minimo_floyd(char id_no, char id_no_b); // d
    Grafo* arvore_geradora_minima_prim(vector<char> ids_nos); // e
    Grafo* arvore_geradora_minima_kruskal(vector<char> ids_nos); // f
    Grafo* arvore_caminhamento_profundidade(char id_no); // g
    int raio(); // h 1
    int diametro(); // h 2
    vector<char> centro(); // h 3
    vector<char> periferia(); // h 4
   
    
    //criado pelo grupo
    void montar_Grafo_por_arquivo(const string &nome_arquivo);
    vector<char> caminho_minimo_floyd_sem_print(char id_no, char id_no_b); // d
    int indice_no(char id);
    void imprimir_grafo();
    int excentricidade(char id_no_a);
    std::ofstream grafoParaArquivo(std::vector<par<std::string, int>> &listaArestas, std::string nomeArq);
    std::ofstream grafoParaArquivo(const Grafo &grafo, std::string nomeArq);
    std::ofstream vetorParaArquivo(const std::vector<char>& vetor, std::string nomeArq);
    std::ofstream h_paraArquivo(par<int,int> raio_diametro,par<std::vector<char>*, std::vector<char>*> vetores, std::string nomeArq);

    HASH<No *, bool> *Hash_n;
    void PROF(No *NoAt, std::vector<par<std::string, int>> *listaAdjRet);

    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No*> lista_adj;
    int distancia_no_atual;
};



#endif //GRAFO_H
