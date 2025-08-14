#include "par.h"
#include "Grafo.h"
#include "No.h"
#include "Hash_unica.h"
#include <utility>
#include <chrono>
#include <set>
#include <random>
#include <vector>
// CRIADO PELO GRUPO - Igor Correa Trifilio Campos 202365092AB
/*
Alunos:
Igor Correa Trifilio Campos
Gustavo Duarte Fernandes de Jesus
Enzo Araújo Pinheiro
Gabriel Toledo Gonçalves Barreto

Repositório: https://github.com/Rogire/t1-dcc059-2025-1
*/
class elemento{
    public:
        std::vector<No*> solucao;
        double tempo;
        float alpha;

        elemento(){}
        ~elemento(){}
        elemento(std::vector<No*> soluc, double tempo, float alp){
            this->solucao = soluc;
            this->tempo = tempo;
            this->alpha=alp;
        }
};

#ifndef CDS_guloso_H
#define CDS_guloso_H
class CDS_guloso
{
    public:
        CDS_guloso() {};
        ~CDS_guloso() {};

        std::vector<No*>CDS(Grafo* grafo);
        std::vector<No *> CDS_randomizado(Grafo *grafo,float alpha);
        std::vector<No*> Construtivo_randomizado(Grafo *grafo,float alpha);
        elemento CDS_randomizado_reativo(Grafo *grafo, std::vector<float> alphas, int numIter, int bloco,bool printar=true);
        void adjDominante(No *node);

    private:
        HASH_unica *Hash_nodes;
};

#endif // CDS_guloso_H

