#ifndef CDS_guloso_H
#define CDS_guloso_H

#include "par.h"
#include "Grafo.h"
#include "No.h"
#include "Hash_unica.h"

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
        std::vector<No *> CDS_teste(Grafo *grafo);
    private:
        par<bool, int> *adjDominante(No *node);
        par<bool, int> *adjDominante_teste(No *node);

        HASH_unica *Hash_nodes;
};

#endif // CDS_guloso_H
