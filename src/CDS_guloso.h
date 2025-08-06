#ifndef CDS_guloso_H
#define CDS_guloso_H

#include "par.h"
#include "Grafo.h"
#include "No.h"
#include "Hash_unica.h"
#include <utility>
#include <chrono>
#include <set>
#include <random>
// CRIADO PELO GRUPO - Igor Correa Trifilio Campos 202365092AB
/*
Alunos:
Igor Correa Trifilio Campos
Gustavo Duarte Fernandes de Jesus
Enzo Araújo Pinheiro
Gabriel Toledo Gonçalves Barreto

Repositório: https://github.com/Rogire/t1-dcc059-2025-1
*/

class CDS_guloso
{
    public:
        CDS_guloso() {};
        ~CDS_guloso() {};

        std::vector<No*>CDS(Grafo* grafo);
        std::vector<No *> CDS_teste_randomizado(Grafo *grafo,float alpha);
        void CDS_teste_randomizado_reativo(Grafo *grafo, std::vector<float> alphas, int numIter, int bloco);
        void adjDominante(No *node);

    private:
        HASH_unica *Hash_nodes;
};

#endif // CDS_guloso_H
