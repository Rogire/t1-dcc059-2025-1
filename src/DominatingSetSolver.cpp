#include "DominatingSetSolver.h"
#include <queue>
#include <algorithm>
#include <iostream>

DominatingSetSolver::DominatingSetSolver(Grafo& grafo) : grafo(grafo) {}

std::vector<char> DominatingSetSolver::obterVizinhos(char id) {
    for (No* no : grafo.lista_adj) {
        if (no->id == id) {
            return no->getAdjacentes();
        }
    }
    return {};
}

bool DominatingSetSolver::ehDominado(char no, const std::set<char>& dominantes) {
    if (dominantes.count(no)) return true;
    for (char vizinho : obterVizinhos(no)) {
        if (dominantes.count(vizinho)) return true;
    }
    return false;
}

bool DominatingSetSolver::subgrafoEhConexo(const std::set<char>& subconjunto) {
    if (subconjunto.empty()) return true;

    std::set<char> visitados;
    std::queue<char> fila;

    char inicial = *subconjunto.begin();
    fila.push(inicial);
    visitados.insert(inicial);

    while (!fila.empty()) {
        char atual = fila.front(); fila.pop();
        for (char vizinho : obterVizinhos(atual)) {
            if (subconjunto.count(vizinho) && visitados.count(vizinho) == 0) {
                visitados.insert(vizinho);
                fila.push(vizinho);
            }
        }
    }

    return visitados.size() == subconjunto.size();
}

std::set<char> DominatingSetSolver::guloso() {
    std::set<char> dominantes;
    std::set<char> naoDominados;

    for (No* no : grafo.lista_adj) {
        naoDominados.insert(no->id);
    }

    while (!naoDominados.empty()) {
        char melhorNo = '\0';
        int melhorCobranca = -1;

        for (No* no : grafo.lista_adj) {
            char id = no->id;
            if (dominantes.count(id)) continue;

            int cobranca = naoDominados.count(id);
            for (char viz : no->getAdjacentes()) {
                if (naoDominados.count(viz)) cobranca++;
            }

            if (cobranca > melhorCobranca) {
                melhorCobranca = cobranca;
                melhorNo = id;
            }
        }

        if (melhorNo == '\0') break;

        dominantes.insert(melhorNo);
        naoDominados.erase(melhorNo);
        for (char viz : obterVizinhos(melhorNo)) {
            naoDominados.erase(viz);
        }

        if (!subgrafoEhConexo(dominantes)) {
            for (No* no : grafo.lista_adj) {
                char id = no->id;
                if (dominantes.count(id)) continue;

                for (char viz : no->getAdjacentes()) {
                    if (dominantes.count(viz)) {
                        dominantes.insert(id);
                        goto fim_conexao;
                    }
                }
            }
        fim_conexao:;
        }
    }

    return dominantes;
}
