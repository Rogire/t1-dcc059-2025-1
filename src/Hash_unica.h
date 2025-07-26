#ifndef HASH_UNICA_H
#define HASH_UNICA_H

#include <iostream>
#include <vector>
#include "par.h"

// CRIADO PELO GRUPO - Igor Correa Trifilio Campos 202365092AB
/*
Alunos:
Igor Correa Trifilio Campos
Gustavo Duarte Fernandes de Jesus
Enzo Araújo Pinheiro
Gabriel Toledo Gonçalves Barreto

Repositório: https://github.com/Rogire/t1-dcc059-2025-1
*/



class HASH_unica
{
public:
    HASH_unica() {};

    HASH_unica(const std::vector<No*> &lista_adj)
    {
        if (lista_adj.empty())
        {
            std::cerr << "Erro: lista_adj está vazia!" << std::endl;
            return;
        }

        int maior = lista_adj.at(0)->id;

        for (auto No : lista_adj)
            if (No->id > maior)
                maior = No->id;

        this->tam = maior + 1;
        this->Hash_unique = new No *[this->tam];

        for (int i = 0; i < this->tam; i++)
            this->Hash_unique[i] = nullptr;
    };

    void InitHash(std::vector<No*> &lista_adj)
    {
        for (auto node : lista_adj)
        {
            if (node->id >= 0 && node->id < this->tam)
            {
                if(this->Hash_unique[node->id] != nullptr)
                    delete this->Hash_unique[node->id];

                this->Hash_unique[node->id] = node;
            }
        }
    }

    void printHash(std::vector<No*> &lista_adj)
    {
        std::cout << " HASH MONTADA: \n";

        for (auto node : lista_adj)
        {
            std::cout << this->Hash_unique[node->id]->id << " \n";
        }
    }

    ~HASH_unica()
    {
        for (int i{0}; i < tam; i++)
            if (this->Hash_unique[i])
                delete this->Hash_unique[i];

        delete[] this->Hash_unique;
    }

    No* get(int id)
    {
        if (id >= 0 && id < tam)
            return this->Hash_unique[id];

        return nullptr;
    }

private:
    No **Hash_unique;

    int tam{};
};

#endif // HASH_UNICA_H
