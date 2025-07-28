#ifndef DOMINATING_SET_SOLVER_H
#define DOMINATING_SET_SOLVER_H

#include "Grafo.h"
#include <vector>
#include <set>
#include <random>

class DominatingSetSolver {
public:
    DominatingSetSolver(Grafo& grafo);

    std::set<char> guloso();
    /*Função GULOSO(Grafo G):
    dominantes ← ∅
    naoDominados ← todos os vértices de G

    // Escolhe vértice inicial com maior cobertura (ele + vizinhos)
    para cada vértice v em G:
        cobertura ← 1 + grau de v
        se cobertura > maxCobertura:
            maxCobertura ← cobertura
            primeiro ← v

    Adiciona primeiro a dominantes
    Remove primeiro e seus vizinhos de naoDominados

    Enquanto naoDominados não está vazio:
        melhor ← nulo
        melhorCobranca ← -1

        para cada vértice v não dominante:
            se v é adjacente a algum dominante:
                cobranca ← nº de vértices em {v ∪ vizinhos de v} que estão em naoDominados
                se cobranca > melhorCobranca:
                    melhorCobranca ← cobranca
                    melhor ← v

        se nenhum "melhor" foi encontrado:
            escolhe qualquer vértice ainda não dominante

        Adiciona melhor a dominantes
        Remove melhor e seus vizinhos de naoDominados

    Corrige conectividade de dominantes (caso subgrafo não seja conexo)
    Retorna dominantes
*/
    std::set<char> gulosoRandomizado(double alpha);
    /*Função GULOSO_RANDOMIZADO(Grafo G, α):
    dominantes ← ∅
    naoDominados ← todos os vértices de G

    Escolhe vértice inicial com maior cobertura
    Adiciona primeiro a dominantes
    Remove primeiro e seus vizinhos de naoDominados

    Enquanto naoDominados não está vazio:
        candidatos ← []

        para cada vértice v não dominante:
            se v é adjacente a algum dominante:
                cobranca ← nº de vértices em {v ∪ vizinhos de v} que estão em naoDominados
                se cobranca > 0:
                    adiciona (v, cobranca) a candidatos

        se candidatos está vazio:
            adiciona qualquer vértice não dominante como candidato

        Ordena candidatos por cobranca decrescente
        rcl_size ← max(1, ceil(α * tamanho(candidatos)))
        RCL ← os rcl_size melhores candidatos

        Escolhe aleatoriamente um vértice de RCL
        Adiciona escolhido a dominantes
        Remove escolhido e seus vizinhos de naoDominados

    Corrige conectividade de dominantes
    Retorna dominantes
*/
    
    
    void gulosoRandomizadoReativo(int iteracoes, const std::vector<double>& alphas, int bloco = 50);
    /*Função GULOSO_RANDOMIZADO_REATIVO(Grafo G, vetor de α, iterações, bloco):
    Inicializa probabilidades iguais para cada α
    Inicializa contadores de uso, somas de tamanhos e tempos

    melhorSolucao ← ∅
    melhorTamanho ← ∞
    melhorAlpha ← α[0]

    Para iter = 1 até iterações:
        Sorteia α com base nas probabilidades
        Executa GULOSO_RANDOMIZADO(G, α)
        Mede tempo e tamanho da solução

        Atualiza contadores e estatísticas

        Se solução é melhor que melhorTamanho:
            Atualiza melhorSolucao, melhorTamanho, melhorAlpha

        Se iter % bloco == 0:
            Para cada α:
                Se foi usado:
                    qualidade ← inverso da média de tamanho das soluções com esse α
            Atualiza probabilidades proporcionalmente à qualidade

    Exibe melhor solução, α correspondente e médias por α
*/
    
    
    std::set<char>corrigirConectividade(std::set<char> dominantes);


private:
    Grafo& grafo;

    std::vector<char> obterVizinhos(char id);
    bool ehDominado(char no, const std::set<char>& dominantes);
    bool subgrafoEhConexo(const std::set<char>& subconjunto);
};

#endif
