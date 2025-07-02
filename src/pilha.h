#ifndef PILHA_H
#define PILHA_H

#include "pnode.h"
// CRIADO PELO GRUPO - Igor Correa Trifilio Campos 202365092AB

template <typename V>
class pilha
{
public:
    pilha() : topo(nullptr) {};

    ~pilha()
    {
        while(!empty())
        {
            pnode<V>* temp = unstack();
            delete temp;
        }
    }
    
    pnode<V>* getTopo() { return topo; };

    bool empty()
    {
        return this->topo == nullptr;
    }

    pnode<V>* unstack()
    {
        if(empty())
            return nullptr;

        pnode<V> *aux = this->topo;
        this->topo = this->topo->getProx();
        aux->setProx(nullptr);

        return aux;
    }

    void stack(pnode<V>* p)
    {
        if (p != nullptr)
        {
            p->setProx(topo);
            topo = p;
        }        
    }

private:
    pnode<V>* topo;
};

#endif // PILHA_H
