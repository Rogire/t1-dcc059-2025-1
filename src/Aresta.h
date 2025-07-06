#ifndef ARESTA_H
#define ARESTA_H

using namespace std;
class Aresta {
public:
    Aresta(char id_no_alvo , int peso = 0);
    ~Aresta();

    char id_no_alvo;
    int peso;
    bool retorno;
};



#endif //ARESTA_H
