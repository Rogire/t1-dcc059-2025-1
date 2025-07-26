#include "DominatingSetSolver.h"
#include <queue>
#include <set>
#include <vector>

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

    // Inicializa conjunto de vértices não dominados
    for (No* no : grafo.lista_adj)
        naoDominados.insert(no->id);

    // Começa com nó que domina mais (ele + vizinhos)
    char primeiro = '\0';
    int maxCobertura = -1;
    for (No* no : grafo.lista_adj) {
        int cobertura = 1 + (int)no->getAdjacentes().size();
        if (cobertura > maxCobertura) {
            maxCobertura = cobertura;
            primeiro = no->id;
        }
    }

    dominantes.insert(primeiro);
    naoDominados.erase(primeiro);
    for (char viz : obterVizinhos(primeiro))
        naoDominados.erase(viz);

    // Loop principal
    while (!naoDominados.empty()) {
        char melhorNo = '\0';
        int melhorCobranca = -1;

        // Busca nó adjacente ao conjunto dominante que maximize cobertura
        for (No* no : grafo.lista_adj) {
            char id = no->id;
            if (dominantes.count(id)) continue;

            bool adjacenteDominantes = false;
            for (char viz : no->getAdjacentes()) {
                if (dominantes.count(viz)) {
                    adjacenteDominantes = true;
                    break;
                }
            }
            if (!adjacenteDominantes) continue;

            int cobranca = 0;
            if (naoDominados.count(id)) cobranca++;
            for (char viz : no->getAdjacentes()) {
                if (naoDominados.count(viz)) cobranca++;
            }

            if (cobranca > melhorCobranca) {
                melhorCobranca = cobranca;
                melhorNo = id;
            }
        }

        // Se não achar nó adjacente, pega qualquer nó (para não travar)
        if (melhorNo == '\0') {
            for (No* no : grafo.lista_adj) {
                char id = no->id;
                if (!dominantes.count(id)) {
                    melhorNo = id;
                    break;
                }
            }
        }

        if (melhorNo == '\0') break;

        dominantes.insert(melhorNo);
        naoDominados.erase(melhorNo);
        for (char viz : obterVizinhos(melhorNo))
            naoDominados.erase(viz);
    }

    return dominantes;
}
