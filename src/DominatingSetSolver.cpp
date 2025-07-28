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

// Construtor que recebe um grafo para trabalhar com ele
DominatingSetSolver::DominatingSetSolver(Grafo& grafo) : grafo(grafo) {}

// Método que retorna os vizinhos de um nó identificado por 'id'
std::vector<char> DominatingSetSolver::obterVizinhos(char id) {
    for (No* no : grafo.lista_adj)
        if (no->id == id)
            return no->getAdjacentes();  // Retorna lista de adjacentes do nó
    return {};  // Retorna vetor vazio se nó não encontrado
}

// Verifica se um nó 'no' está dominado pelo conjunto 'dominantes'
// Um nó está dominado se ele próprio ou algum vizinho pertence ao conjunto dominantes
bool DominatingSetSolver::ehDominado(char no, const std::set<char>& dominantes) {
    if (dominantes.count(no)) return true;  // Nó está no conjunto dominante
    for (char vizinho : obterVizinhos(no)) {
        if (dominantes.count(vizinho)) return true;  // Algum vizinho está no conjunto dominante
    }
    return false;  // Nó não está dominado
}

// Verifica se o subgrafo induzido pelo subconjunto de nós 'subconjunto' é conexo
bool DominatingSetSolver::subgrafoEhConexo(const std::set<char>& subconjunto) {
    if (subconjunto.empty()) return true;  // Subconjunto vazio é considerado conexo

    std::set<char> visitados;  // Nós visitados na busca
    std::queue<char> fila;
    char inicial = *subconjunto.begin();  // Pega um nó inicial do subconjunto
    fila.push(inicial);
    visitados.insert(inicial);

    // Busca em largura para verificar conectividade dentro do subconjunto
    while (!fila.empty()) {
        char atual = fila.front(); fila.pop();
        for (char vizinho : obterVizinhos(atual)) {
            // Só considera vizinhos que estão no subconjunto e ainda não foram visitados
            if (subconjunto.count(vizinho) && visitados.count(vizinho) == 0) {
                visitados.insert(vizinho);
                fila.push(vizinho);
            }
        }
    }

    // Se todos os nós do subconjunto foram visitados, é conexo
    return visitados.size() == subconjunto.size();
}

// Algoritmo guloso para encontrar conjunto dominante
std::set<char> DominatingSetSolver::guloso() {
    std::set<char> dominantes;     // Nós que formam o conjunto dominante
    std::set<char> naoDominados;   // Nós ainda não dominados

    // Inicializa conjunto de não dominados com todos os nós do grafo
    for (No* no : grafo.lista_adj)
        naoDominados.insert(no->id);

    // Seleciona o nó com maior cobertura (ele + vizinhos)
    char primeiro = '\0';
    int maxCobertura = -1;
    for (No* no : grafo.lista_adj) {
        int cobertura = 1 + (int)no->getAdjacentes().size();
        if (cobertura > maxCobertura) {
            maxCobertura = cobertura;
            primeiro = no->id;
        }
    }

    // Adiciona o nó inicial ao conjunto dominante
    dominantes.insert(primeiro);

    // Remove o nó inicial e seus vizinhos do conjunto de não dominados
    naoDominados.erase(primeiro);
    for (char viz : obterVizinhos(primeiro))
        naoDominados.erase(viz);

    // Enquanto ainda houver nós não dominados
    while (!naoDominados.empty()) {
        char melhorNo = '\0';
        int melhorCobranca = -1;

        // Procura o próximo nó que seja adjacente a algum dominante e que maximize a cobertura
        for (No* no : grafo.lista_adj) {
            char id = no->id;
            if (dominantes.count(id)) continue;  // Ignora nós já dominantes

            // Verifica se o nó é adjacente a algum dominante
            bool adjacenteDominantes = false;
            for (char viz : no->getAdjacentes()) {
                if (dominantes.count(viz)) {
                    adjacenteDominantes = true;
                    break;
                }
            }
            if (!adjacenteDominantes) continue;  // Pula se não é adjacente

            // Calcula quantos nós não dominados o nó pode dominar (ele mesmo + vizinhos)
            int cobranca = 0;
            if (naoDominados.count(id)) cobranca++;
            for (char viz : no->getAdjacentes()) {
                if (naoDominados.count(viz)) cobranca++;
            }

            // Atualiza o melhor nó se cobertura maior
            if (cobranca > melhorCobranca) {
                melhorCobranca = cobranca;
                melhorNo = id;
            }
        }

        // Se não encontrou nó adjacente que ajude, escolhe qualquer nó restante
        if (melhorNo == '\0') {
            for (No* no : grafo.lista_adj) {
                char id = no->id;
                if (!dominantes.count(id)) {
                    melhorNo = id;
                    break;
                }
            }
        }
        if (melhorNo == '\0') break;  // Se não tem mais nós, termina

        // Adiciona o melhor nó encontrado ao conjunto dominante e remove dele e vizinhos do não dominados
        dominantes.insert(melhorNo);
        naoDominados.erase(melhorNo);
        for (char viz : obterVizinhos(melhorNo))
            naoDominados.erase(viz);
    }

    // Corrige a conectividade do conjunto dominante antes de retornar
    return corrigirConectividade(dominantes);
}

// Variante gulosa randomizada com parâmetro alpha (entre 0 e 1)
std::set<char> DominatingSetSolver::gulosoRandomizado(double alpha) {
    std::set<char> dominantes;
    std::set<char> naoDominados;
    for (No* no : grafo.lista_adj)
        naoDominados.insert(no->id);

    std::random_device rd;
    std::mt19937 rng(rd());

    // Primeiro nó: escolhe o de maior cobertura (igual ao guloso)
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

    // Enquanto ainda existirem nós não dominados
    while (!naoDominados.empty()) {
        std::vector<std::pair<char, int>> candidatos;

        // Monta lista de candidatos que são adjacentes a dominantes
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

            // Conta quantos nós não dominados o candidato pode dominar (ele mesmo + vizinhos)
            int cobranca = naoDominados.count(id);
            for (char viz : no->getAdjacentes())
                cobranca += naoDominados.count(viz);

            if (cobranca > 0)
                candidatos.emplace_back(id, cobranca);
        }

        // Caso não haja candidatos adjacentes (ex. em componentes desconexos), adiciona qualquer nó restante
        if (candidatos.empty()) {
            for (No* no : grafo.lista_adj) {
                char id = no->id;
                if (!dominantes.count(id)) {
                    candidatos.emplace_back(id, 0);
                }
            }
            if (candidatos.empty()) break;
        }

        // Ordena candidatos por cobertura decrescente
        std::sort(candidatos.begin(), candidatos.end(), [](auto& a, auto& b) {
            return a.second > b.second;
        });

        // Define tamanho da lista restrita de candidatos (RCL) baseado no alpha
        int rcl_size = std::max(1, (int)std::ceil(alpha * candidatos.size()));

        // Escolhe aleatoriamente um candidato dentro da RCL
        std::uniform_int_distribution<int> escolha(0, rcl_size - 1);
        char escolhido = candidatos[escolha(rng)].first;

        // Atualiza conjuntos
        dominantes.insert(escolhido);
        naoDominados.erase(escolhido);
        for (char viz : obterVizinhos(escolhido))
            naoDominados.erase(viz);
    }

    // Corrige conectividade e retorna
    return corrigirConectividade(dominantes);
}

// Método que garante que o conjunto dominante seja conexo,
// inserindo nós intermediários se necessário para conectar componentes desconexas
std::set<char> DominatingSetSolver::corrigirConectividade(std::set<char> dominantes) {
    // Se já é conexo, retorna logo
    if (subgrafoEhConexo(dominantes)) return dominantes;

    std::set<char> visitados;
    std::queue<char> fila;
    char inicial = *dominantes.begin();
    fila.push(inicial);
    visitados.insert(inicial);

    // Busca em largura para conectar componentes desconexas
    while (!fila.empty()) {
        char atual = fila.front(); fila.pop();
        for (char vizinho : obterVizinhos(atual)) {
            if (dominantes.count(vizinho) && visitados.count(vizinho) == 0) {
                // Vizinho já no conjunto, apenas visita
                visitados.insert(vizinho);
                fila.push(vizinho);
            } else if (!dominantes.count(vizinho)) {
                // Vizinho fora do conjunto, insere para conectar
                dominantes.insert(vizinho);
                visitados.insert(vizinho);
                fila.push(vizinho);
            }
        }
    }

    return dominantes;
}

// Algoritmo guloso randomizado reativo que ajusta probabilidades dos alphas baseado no desempenho
void DominatingSetSolver::gulosoRandomizadoReativo(int iteracoes, const std::vector<double>& alphas, int bloco) {
    int numAlphas = alphas.size();

    // Inicializa probabilidades iguais para cada alpha
    std::vector<double> probabilidades(numAlphas, 1.0 / numAlphas);

    // Contadores para número de usos, somatório de tamanhos e tempos de solução por alpha
    std::vector<int> usos(numAlphas, 0);
    std::vector<double> somasTamanhos(numAlphas, 0.0);
    std::vector<double> somasTempos(numAlphas, 0.0);

    int melhorTamanho = std::numeric_limits<int>::max();  // Melhor tamanho encontrado
    std::set<char> melhorSolucao;                         // Melhor solução encontrada
    double melhorAlpha = alphas[0];                       // Alpha associado à melhor solução

    std::random_device rd;
    std::mt19937 rng(rd());

    // Loop principal do algoritmo reativo
    for (int iter = 1; iter <= iteracoes; iter++) {
        // Escolhe um alpha baseado na distribuição de probabilidades atual
        std::discrete_distribution<int> dist(probabilidades.begin(), probabilidades.end());
        int idxAlpha = dist(rng);
        double alpha = alphas[idxAlpha];

        // Mede tempo para gerar solução com alpha escolhido
        auto inicio = std::chrono::high_resolution_clock::now();
        std::set<char> solucao = gulosoRandomizado(alpha);
        auto fim = std::chrono::high_resolution_clock::now();

        double duracao = std::chrono::duration<double>(fim - inicio).count();
        int tamanho = (int)solucao.size();

        // Atualiza estatísticas para o alpha utilizado
        usos[idxAlpha]++;
        somasTamanhos[idxAlpha] += tamanho;
        somasTempos[idxAlpha] += duracao;

        // Atualiza melhor solução se encontrou uma melhor
        if (tamanho < melhorTamanho) {
            melhorTamanho = tamanho;
            melhorSolucao = solucao;
            melhorAlpha = alpha;
        }

        // A cada 'bloco' iterações, atualiza probabilidades com base na qualidade das soluções
        if (iter % bloco == 0) {
            std::vector<double> qualidades(numAlphas, 0.0);
            for (int i = 0; i < numAlphas; i++) {
                if (usos[i] > 0)
                    // Qualidade é inversamente proporcional ao tamanho médio da solução
                    qualidades[i] = 1.0 / (somasTamanhos[i] / usos[i]);
            }
            double somaQualidades = std::accumulate(qualidades.begin(), qualidades.end(), 0.0);
            if (somaQualidades > 0) {
                for (int i = 0; i < numAlphas; i++)
                    probabilidades[i] = qualidades[i] / somaQualidades;  // Normaliza para somar 1
            }
        }
    }

    // Imprime o resultado final do algoritmo
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
