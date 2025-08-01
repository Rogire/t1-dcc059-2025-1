#include <iostream>
#include <chrono>
#include <set>
#include "Gerenciador.h"

/*
Alunos:
Igor Correa Trifilio Campos
Gustavo Duarte Fernandes de Jesus
Enzo Araújo Pinheiro
Gabriel Toledo Gonçalves Barreto

Repositório: https://github.com/Rogire/t1-dcc059-2025-1
*/
using namespace std;
int main(int argc, char *argv[])
{

    Grafo* grafo = new Grafo();
    grafo->montar_Grafo_por_arquivo(argv[1]);

    //grafo->CDS_guloso();

    //Gerenciador::comandos(grafo);

    CDS_guloso solver;

    // =============================
    // 1. Guloso Puro
    // =============================

    cout << "\n=== Guloso Puro ===\n";
    auto ini1 = std::chrono::high_resolution_clock::now();
    vector<No*> resG = solver.CDS_teste(grafo);
    auto fim1 = std::chrono::high_resolution_clock::now();
    double tempo1 = std::chrono::duration<double>(fim1 - ini1).count();

    cout << "Conjunto Dominante Conexo (Guloso): { ";
    for (No* c : resG)
        cout << c->id << " ";
    cout << "}\nTamanho: " << resG.size()
         << " | Tempo (s): " << tempo1 << endl;

    return 0;
}
