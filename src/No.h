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
    bool marca;
    vector<Aresta*> arestas;

    std::vector<char> getAdjacentes() const; // ✅ NOVO
};

#endif //NO_H
