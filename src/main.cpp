#include <iostream>
#include <chrono>
#include <vector>
#include "Gerenciador.h"
#include "No.h"
#include "testador.h"

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
    int selecao = -1, selP2=-1;

    while(selecao == -1)
    {
        std::cout << "Selecione o que deseja fazer:\n(1) Parte 1\n(2) Parte 2\n";
        cin >> selecao;

        if(selecao == 1 || selecao == 2)
        {}
        else
            selecao = -1;
    }

    if(selecao == 1)
         Gerenciador::comandos(grafo);
    else
    {
        while(selP2 == -1)
        {
            std::cout << "Selecione o que deseja fazer:\n(1) rodar algortimos\n(2) testar algoritmos\n";
            cin >> selP2;

            if(selP2 == 1 || selP2 == 2)
            {}
            else
                selP2 = -1;
        }
        if(selP2 == 1)
        {
            CDS_guloso solver;

            srand(static_cast<unsigned int>(time(NULL)));

            cout << "\n=== Guloso Puro ===\n";
            auto ini1 = std::chrono::high_resolution_clock::now();
            vector<No*> resG = solver.CDS(grafo);
            auto fim1 = std::chrono::high_resolution_clock::now();
            double tempo1 = std::chrono::duration<double>(fim1 - ini1).count();

            cout << "Conjunto Dominante Conexo (Guloso): { ";
            for (No* c : resG)
                cout << c->id << " ";
            cout << "}\nTamanho: " << resG.size()
                << " | Tempo (s): " << tempo1 << endl;

            cout << "\n=== Guloso Randomizado ===\n";
            auto ini2 = std::chrono::high_resolution_clock::now();
            vector<No *> resG_Rnd = solver.CDS_randomizado(grafo, 0.3);
            auto fim2 = std::chrono::high_resolution_clock::now();
            double tempo2 = std::chrono::duration<double>(fim2 - ini2).count();
            
            cout << "Conjunto Dominante Conexo (Guloso Aleatório): { ";
            for (No *c : resG_Rnd)
                cout << c->id << " ";
            cout << "}\nTamanho: " << resG_Rnd.size()
                << " | Tempo (s): " << tempo2 << endl;

            std::vector<float> alphas = { 0.8, 0.5, 0.2};

            cout << "\n=== Guloso Randomizado Reativo ===\n";
            solver.CDS_randomizado_reativo(grafo,alphas, 300,30,{});
        }
        else {
            Testador::testar_algoritmos(30,  grafo,  {0.8, 0.5, 0.2});
        }
    }
    return 0;
}