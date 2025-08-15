#ifndef TESTADOR_H
#define TESTADOR_H

#include "Gerenciador.h"
#include <string>
#include <utility>
#include <vector>

using namespace std;
class Testador {
public:
    Testador(char id, int peso = 0);
    ~Testador();
    void static imprimir_valores(std::vector<std::pair<double,std::vector<No*>>>& Valores_media_guloso, std::vector<std::pair<double,std::vector<No*>>>& Valores_reativo, std::vector<float>& alphas)
    {
        std::cout<<"Valores media Guloso: \n";
        for(std::pair<double,std::vector<No*>> v : Valores_media_guloso)
        {
            std::cout<<"Tempo: "<<v.first<<" Tamanho: "<<v.second.size();
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

    double static calcular_tempo(CDS_guloso solver, Grafo* grafo, vector<No*>* resG, char tipo,float alpha=-1,std::vector<float> alphas={})
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

    std::pair<double,double> static calcular_media(std::vector<std::pair<double,std::vector<No*>>> valores)
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

    void static testar_algoritmos(int num_Iter, Grafo* grafo, std::vector<float> alphas, bool printar=false)
    {
        std::vector<std::pair<double,std::vector<No*>>> Valores_media_guloso;
        std::vector<std::pair<double,std::vector<No*>>> Valores_reativo;

        vector<vector<pair<double,vector<No*>>>> Valores_media_guloso_rand_alphas(static_cast<int>(alphas.size()));

        std::pair<double,double> media_guloso, media_guloso_rand, media_reativo;
        vector<pair<double,double>> media_guloso_rand_alphas(static_cast<int>(alphas.size()));

        for(int i{0};i<num_Iter;i++)
        {
            CDS_guloso solver;
            srand(static_cast<unsigned int>(time(NULL)));

            vector<No*> resG;
            double tempo1 = calcular_tempo(solver,grafo,&resG,'a');

            Valores_media_guloso.push_back(std::make_pair(tempo1, resG));

            for(int j{0};j<alphas.size();j++)
            {
                float v = alphas[j];
                vector<No *> resG_Rnd;
                double tempo2 = calcular_tempo(solver,grafo,&resG_Rnd,'b',v);
                
                Valores_media_guloso_rand_alphas[j].push_back(std::make_pair(tempo2, resG_Rnd));
            }

            vector<No*> resg_G_rnd_reat;
            double tempo3 = calcular_tempo(solver, grafo, &resg_G_rnd_reat, 'c',-1,alphas);
            Valores_reativo.push_back(std::make_pair(tempo3, resg_G_rnd_reat));
        }

        if(printar)
            imprimir_valores(Valores_media_guloso,Valores_reativo, alphas);

        media_guloso = calcular_media(Valores_media_guloso);
        media_reativo = calcular_media(Valores_reativo);

        for(int i{0};i<alphas.size();i++)
            media_guloso_rand_alphas[i] = calcular_media(Valores_media_guloso_rand_alphas[i]);

        std::cout<<"Media Guloso:\nmédia tempo: "<<media_guloso.first<<" média tamanho: "<<media_guloso.second<<"\n";
        std::cout<<"Media Guloso Randomizado Alphas:\n";
        for(int i{0};i<alphas.size();i++)
            std::cout<<"Alpha: "<<alphas[i]<<" média tempo: "<<media_guloso_rand_alphas[i].first<<" média tamanho: "<<media_guloso_rand_alphas[i].second<<"\n";

        std::cout<<"Media Guloso Randomizado Reativo: \nmédia tempo: "<<media_reativo.first<<" média tamanho: "<<media_reativo.second<<"\n";
    }
};
#endif //TESTADOR_H
