#include <iostream>
#include "pilha.h"
#include "pnode.h"

int main()
{
    pilha<char> pil;

    for (int i = 0; i < 10;i++)
    {
        pnode<char>*p = new pnode<char>(97+i);
        pil.stack(p);
    }

    for (int i = 0; i < 10;i++)
        std::cout << pil.unstack()->getVal() << " ";
}