#include "No.h"

No::No(char id, int peso) {
    this->id = id;
    this->peso = peso;
    this->marca = 0;
}

No::~No() {}

std::vector<char> No::getAdjacentes() const {
    std::vector<char> vizinhos;
    for (Aresta* a : arestas) {
        vizinhos.push_back(a->id_no_alvo); // ⚠ depende do seu Aresta
    }
    return vizinhos;
}
