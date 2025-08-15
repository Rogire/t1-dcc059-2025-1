#include <iostream>
#include <chrono>
#include <vector>
#include "Gerenciador.h"
#include "No.h"

/*
Alunos:
Igor Correa Trifilio Campos
Gustavo Duarte Fernandes de Jesus
Enzo Araújo Pinheiro
Gabriel Toledo Gonçalves Barreto

Repositório: https://github.com/Rogire/t1-dcc059-2025-1
*/
using namespace std;

void imprimir_valores(std::vector<std::pair<double,std::vector<No*>>>& Valores_media_guloso,std::vector<std::pair<double,std::vector<No*>>>& Valores_media_guloso_rand,std::vector<std::pair<double,std::vector<No*>>>& Valores_reativo, std::vector<float>& alphas)
{
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

    for(std::pair<double,std::vector<No*>> v : Valores_reativo)
    {
        std::cout<<"Tempo: "<<v.first<<" Tamanho: "<<v.second.size();
        for(auto n : v.second)
            std::cout<<" "<<n->id;
        std::cout<<"\n";
    }
}

double calcular_tempo(CDS_guloso solver, Grafo* grafo, vector<No*>* resG, char tipo,float alpha=-1,std::vector<float> alphas={})
{
    auto ini1 = std::chrono::high_resolution_clock::now();
    if(tipo == 'a')
        *resG = solver.CDS(grafo);
    else if(tipo=='b')
        *resG = solver.CDS_randomizado(grafo,alpha);
    else if(tipo=='c')
        *resG = solver.CDS_randomizado_reativo(grafo,alphas,300,30,false).solucao;
    auto fim1 = std::chrono::high_resolution_clock::now();
    
    return std::chrono::duration<double>(fim1 - ini1).count();
}

std::pair<double,double> calcular_media(std::vector<std::pair<double,std::vector<No*>>> valores)
{
    double media_tempo = 0;
    double media_tam = 0;

    for(std::pair<double,std::vector<No*>> v : valores)
    {
        media_tempo += v.first;
        media_tam += v.second.size();
    }

    return {(media_tempo/valores.size()),(media_tam/valores.size())};
}

void testar_algoritmos(int num_Iter, Grafo* grafo, std::vector<float> alphas, bool printar=false)
{
    std::vector<std::pair<double,std::vector<No*>>> Valores_media_guloso;
    std::vector<std::pair<double,std::vector<No*>>> Valores_media_guloso_rand;
    std::vector<std::pair<double,std::vector<No*>>> Valores_reativo;
    
    std::pair<double,double> media_guloso, media_guloso_rand, media_reativo;

    for(int i{0};i<num_Iter;i++)
    {
        CDS_guloso solver;
        srand(static_cast<unsigned int>(time(NULL)));

        vector<No*> resG;
        double tempo1 = calcular_tempo(solver,grafo,&resG,'a');

        Valores_media_guloso.push_back(std::make_pair(tempo1, resG));

        for(float v : alphas)
        {
            vector<No *> resG_Rnd;
            double tempo2 = calcular_tempo(solver,grafo,&resG_Rnd,'b',v);
            Valores_media_guloso_rand.push_back(std::make_pair(tempo2, resG_Rnd));
        }

        vector<No*> resg_G_rnd_reat;
        double tempo3 = calcular_tempo(solver, grafo, &resg_G_rnd_reat, 'c',-1,alphas);
        Valores_reativo.push_back(std::make_pair(tempo3, resg_G_rnd_reat));
    }

    if(printar)
        imprimir_valores(Valores_media_guloso, Valores_media_guloso_rand, Valores_reativo, alphas);

    media_guloso = calcular_media(Valores_media_guloso);
    media_guloso_rand = calcular_media(Valores_media_guloso_rand);
    media_reativo = calcular_media(Valores_reativo);

    std::cout<<"Media Guloso:\nmédia tempo: "<<media_guloso.first<<" média tamanho: "<<media_guloso.second<<"\n";
    std::cout<<"Media Guloso Randomizado:\nmédia tempo: "<<media_guloso_rand.first<<" média tamanho: "<<media_guloso_rand.second<<"\n";
    std::cout<<"Media Guloso Randomizado Reativo: \nmédia tempo: "<<media_reativo.first<<" média tamanho: "<<media_reativo.second<<"\n";
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