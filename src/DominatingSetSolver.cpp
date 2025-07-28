#include "DominatingSetSolver.h"
#include <queue>
#include <set>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <numeric>
#include <chrono>
#include <iomanip>

DominatingSetSolver::DominatingSetSolver(Grafo& grafo) : grafo(grafo) {}

std::vector<char> DominatingSetSolver::obterVizinhos(char id) {
    for (No* no : grafo.lista_adj)
        if (no->id == id)
            return no->getAdjacentes();
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

    for (No* no : grafo.lista_adj)
        naoDominados.insert(no->id);

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

    while (!naoDominados.empty()) {
        char melhorNo = '\0';
        int melhorCobranca = -1;

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

    return corrigirConectividade(dominantes);
}

std::set<char> DominatingSetSolver::gulosoRandomizado(double alpha) {
    std::set<char> dominantes;
    std::set<char> naoDominados;
    for (No* no : grafo.lista_adj)
        naoDominados.insert(no->id);

    std::random_device rd;
    std::mt19937 rng(rd());

    // Primeiro nó: escolher o de maior cobertura (igual ao guloso)
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

    while (!naoDominados.empty()) {
        std::vector<std::pair<char, int>> candidatos;

        // Montar lista de candidatos que são adjacentes a dominantes
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

            int cobranca = naoDominados.count(id);
            for (char viz : no->getAdjacentes())
                cobranca += naoDominados.count(viz);

            if (cobranca > 0)
                candidatos.emplace_back(id, cobranca);
        }

        if (candidatos.empty()) {
            // Se não há candidatos adjacentes (ex: componentes desconexos), adicionar qualquer nó restante
            for (No* no : grafo.lista_adj) {
                char id = no->id;
                if (!dominantes.count(id)) {
                    candidatos.emplace_back(id, 0);
                }
            }
            if (candidatos.empty()) break;
        }

        std::sort(candidatos.begin(), candidatos.end(), [](auto& a, auto& b) {
            return a.second > b.second;
        });

        int rcl_size = std::max(1, (int)std::ceil(alpha * candidatos.size()));
        std::uniform_int_distribution<int> escolha(0, rcl_size - 1);
        char escolhido = candidatos[escolha(rng)].first;

        dominantes.insert(escolhido);
        naoDominados.erase(escolhido);
        for (char viz : obterVizinhos(escolhido))
            naoDominados.erase(viz);
    }

    return corrigirConectividade(dominantes);
}

std::set<char> DominatingSetSolver::corrigirConectividade(std::set<char> dominantes) {
    if (subgrafoEhConexo(dominantes)) return dominantes;

    std::set<char> visitados;
    std::queue<char> fila;
    char inicial = *dominantes.begin();
    fila.push(inicial);
    visitados.insert(inicial);

    while (!fila.empty()) {
        char atual = fila.front(); fila.pop();
        for (char vizinho : obterVizinhos(atual)) {
            if (dominantes.count(vizinho) && visitados.count(vizinho) == 0) {
                visitados.insert(vizinho);
                fila.push(vizinho);
            } else if (!dominantes.count(vizinho)) {
                dominantes.insert(vizinho);
                visitados.insert(vizinho);
                fila.push(vizinho);
            }
        }
    }

    return dominantes;
}

void DominatingSetSolver::gulosoRandomizadoReativo(int iteracoes, const std::vector<double>& alphas, int bloco) {
    int numAlphas = alphas.size();
    std::vector<double> probabilidades(numAlphas, 1.0 / numAlphas);
    std::vector<int> usos(numAlphas, 0);
    std::vector<double> somasTamanhos(numAlphas, 0.0);
    std::vector<double> somasTempos(numAlphas, 0.0);

    int melhorTamanho = std::numeric_limits<int>::max();
    std::set<char> melhorSolucao;
    double melhorAlpha = alphas[0];

    std::random_device rd;
    std::mt19937 rng(rd());

    for (int iter = 1; iter <= iteracoes; iter++) {
        std::discrete_distribution<int> dist(probabilidades.begin(), probabilidades.end());
        int idxAlpha = dist(rng);
        double alpha = alphas[idxAlpha];

        auto inicio = std::chrono::high_resolution_clock::now();
        std::set<char> solucao = gulosoRandomizado(alpha);
        auto fim = std::chrono::high_resolution_clock::now();

        double duracao = std::chrono::duration<double>(fim - inicio).count();
        int tamanho = solucao.size();

        usos[idxAlpha]++;
        somasTamanhos[idxAlpha] += tamanho;
        somasTempos[idxAlpha] += duracao;

        if (tamanho < melhorTamanho) {
            melhorTamanho = tamanho;
            melhorSolucao = solucao;
            melhorAlpha = alpha;
        }

        if (iter % bloco == 0) {
            std::vector<double> qualidades(numAlphas, 0.0);
            for (int i = 0; i < numAlphas; i++) {
                if (usos[i] > 0)
                    qualidades[i] = 1.0 / (somasTamanhos[i] / usos[i]);
            }
            double somaQualidades = std::accumulate(qualidades.begin(), qualidades.end(), 0.0);
            if (somaQualidades > 0) {
                for (int i = 0; i < numAlphas; i++)
                    probabilidades[i] = qualidades[i] / somaQualidades;
            }
        }
    }

    std::cout << "\n=== Resultado Final ===\n";
    std::cout << "Melhor solução encontrada: { ";
    for (char no : melhorSolucao) std::cout << no << " ";
    std::cout << "}\nTamanho: " << melhorTamanho << "\nAlpha correspondente: " << melhorAlpha << "\n\n";
    std::cout << "Médias por α:\n";
    for (int i = 0; i < numAlphas; i++) {
        double mediaTam = usos[i] ? somasTamanhos[i] / usos[i] : 0;
        double mediaTempo = usos[i] ? somasTempos[i] / usos[i] : 0;
        std::cout << "α = " << alphas[i] << " | Média tamanho: " << mediaTam << " | Média tempo (s): " << mediaTempo << "\n";
    }
}
