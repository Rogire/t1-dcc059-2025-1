#ifndef NO_H
#define NO_H

#include <string>
#include <vector>
#include "Aresta.h"

using namespace std;
class No {
public:
    No(char id, int peso = 0);
    ~No();

    char id;
    int peso;
    bool marca; // usada pra indicar que o no ja foi analisado
    
    vector<Aresta*> arestas;

    //========================= Parte 2 =========================================
    bool dominante = false;
    bool dominado = false;
    vector<Aresta *> NaoDom;

    bool adj_Dominante = false;
    int adj_ND = 0;
};

#endif //NO_H
