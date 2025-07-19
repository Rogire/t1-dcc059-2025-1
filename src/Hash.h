#ifndef HASH_H
#define HASH_H

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

template <typename K, typename V>
class HASH
{
public:
    HASH() {};
    HASH(std::vector<K> &lista_adj, bool unique = false)
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

        if(unique)
        {
            this->Hash_unique = new V *[this->tam];
            for (int i = 0; i < this->tam; i++)
                this->Hash_unique[i] = nullptr;
        }
        else
        {
            this->Hash = new par<K, V> *[this->tam];

            for (int i = 0; i < this->tam; i++)
                this->Hash[i] = nullptr;
        }

    };

    void InitHash(std::vector<K> &lista_adj, V valInicial)
    {
        for (auto node : lista_adj)
        {
            if (node->id >= 0 && node->id < this->tam)
            {
                if (this->Hash[node->id] != nullptr)
                    delete this->Hash[node->id];

                this->Hash[node->id] = new par<K, V>(node, valInicial);
            }
        }
    }

    void printHash(std::vector<K> &lista_adj)
    {
        std::cout << " HASH MONTADA: \n";

        for (auto node : lista_adj)
        {
            std::cout << this->Hash[node->id]->getKey()->id << " \n";
        }
    }

    ~HASH()
    {
        if (this->Hash)
        {
            for (int i{0}; i < tam; i++)
                if (this->Hash[i])
                    delete this->Hash[i];

            delete[] this->Hash;
        }
        
        if (this->Hash_unique)
        {
            for (int i{0}; i < tam; i++)
                if (this->Hash_unique[i])
                    delete this->Hash_unique[i];

            delete[] this->Hash_unique;
        }
    }

    par<K, V> *get(int id)
    {
        if (id >= 0 && id < tam)
            return this->Hash[id];

        return nullptr;
    }

private:
    par<K, V> **Hash;
    V **Hash_unique;

    int tam{};
};

#endif // HASH_H
