#ifndef DOMINATING_SET_SOLVER_H
#define DOMINATING_SET_SOLVER_H

#include "Grafo.h"
#include <vector>
#include <set>
#include <random>

class DominatingSetSolver {
public:
    DominatingSetSolver(Grafo& grafo);

    std::set<char> guloso(); // algoritmo guloso
    std::set<char> gulosoRandomizado(double alpha);
    void gulosoRandomizadoReativo(int iteracoes, const std::vector<double>& alphas, int bloco = 50);
    std::set<char>corrigirConectividade(std::set<char> dominantes);


private:
    Grafo& grafo;

    std::vector<char> obterVizinhos(char id);
    bool ehDominado(char no, const std::set<char>& dominantes);
    bool subgrafoEhConexo(const std::set<char>& subconjunto);
};

#endif
