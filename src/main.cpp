#include <iostream>
#include <chrono>
#include <vector>
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


void testar_algoritmos(int num_Iter, Grafo* grafo, std::vector<float> alphas)
{
    std::vector<std::pair<double,std::vector<No*>>> Valores_media_guloso;
    std::vector<std::pair<double,std::vector<No*>>> Valores_media_guloso_rand;
    
    //std::vector<std::pair<double,std::vector<No*>>> Valores_media_guloso_rand_reat;
    std::vector<elemento> Valores_media_guloso_rand_reat;

    for(int i{0};i<num_Iter;i++)
    {
        CDS_guloso solver;
        srand(static_cast<unsigned int>(time(NULL)));

        auto ini1 = std::chrono::high_resolution_clock::now();
        vector<No*> resG = solver.CDS(grafo);
        auto fim1 = std::chrono::high_resolution_clock::now();
        double tempo1 = std::chrono::duration<double>(fim1 - ini1).count();

        Valores_media_guloso.push_back(std::make_pair(tempo1, resG));

        for(float v : alphas)
        {
            auto ini2 = std::chrono::high_resolution_clock::now();
            vector<No *> resG_Rnd = solver.CDS_randomizado(grafo, v);
            auto fim2 = std::chrono::high_resolution_clock::now();
            double tempo2 = std::chrono::duration<double>(fim2 - ini2).count();

            Valores_media_guloso_rand.push_back(std::make_pair(tempo2, resG_Rnd));
        }


        auto resg_G_rnd_reat = solver.CDS_randomizado_reativo(grafo,alphas, 300, 30,false);

        Valores_media_guloso_rand_reat.push_back(resg_G_rnd_reat);
    }

    std::cout<<"Valores media Guloso: \n";
    for(std::pair<double,std::vector<No*>> v : Valores_media_guloso)
    {
        std::cout<<"Tempo: "<<v.first<<" Tamanho: "<<v.second.size();
        for(auto n : v.second)
            std::cout<<" "<<n->id;
        std::cout<<"\n";
    }
    

    std::cout<<"Valores media Guloso Randomizado: \n";
    for(int i{0};i<alphas.size();i++)
    {
        auto v = Valores_media_guloso_rand[i];
        std::cout<<"Tempo: "<<v.first<<" Tamanho: "<<v.second.size() << " Alpha: "<<alphas[i];
        for(auto n : v.second)
            std::cout<<" "<<n->id;
        std::cout<<"\n";
    }
    

    std::cout<<"Valores media Guloso Randomizado Reativo: \n";

    int c{0};

    for(auto v : Valores_media_guloso_rand_reat)
    {
        std::cout<<"Tempo: "<<v.tempo<<" Tamanho: "<<v.solucao.size()<<" Alpha: "<<v.alpha<<"\n";

        for(auto n : v.solucao)
            std::cout<<" "<<n->id;

        std::cout<<"\n";
        
    }
    
}
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
            testar_algoritmos(30,  grafo,  {0.8, 0.5, 0.2});
        }

    }
    return 0;
}


