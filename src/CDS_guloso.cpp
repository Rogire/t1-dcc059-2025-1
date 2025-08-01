#include "CDS_guloso.h"


// Menor conjunto dominante possível
// Deve ser conexo
// todo vértice do grafo está ou no conjunto ou é adjacente a pelo menos um vértice do conjunto
// O(V² + VE)

vector<No*> CDS_guloso::CDS(Grafo* grafo)
{
    this->Hash_nodes = new HASH_unica(grafo->lista_adj);
    Hash_nodes->InitHash(grafo->lista_adj);

    std::vector<No *> Candidatos = grafo->lista_adj, Dominantes;
    int nos_dominados{0}, total_nos = grafo->lista_adj.size();

    par<bool, int> *info_vertice_atual;

    int i{0}, c{0}, it_aresta{0}, totA{0};

    while (nos_dominados < total_nos)
    {
        i++;
        it_aresta = 0;

        No *prox{};
        int adj_ND_at{INT_MIN}, i_prox{};

        // monta o vetor de candidatos
        for (int i{0}; i < Candidatos.size(); i++)
        {
            c++;
            No *node = Candidatos.at(i);

            info_vertice_atual = adjDominante(node);
            it_aresta += node->arestas.size();

            if(info_vertice_atual->getValue() == 0)
            {
                Candidatos.erase(Candidatos.begin() + i);
                continue;
            }

            if (info_vertice_atual->getKey() || nos_dominados == 0)
            {
                if (info_vertice_atual->getValue() > adj_ND_at)
                {
                    prox = node;
                    i_prox = i;
                    adj_ND_at = info_vertice_atual->getValue();
                }
            }
        }

        prox->dominante = true;

        if (!prox->dominado)
        {
            prox->dominado = true;
            nos_dominados++;
        }

        Dominantes.push_back(prox);

        for (Aresta *a : prox->arestas)
        {
            it_aresta++;

            No *vizinho = Hash_nodes->get(a->id_no_alvo);

            if (!vizinho->dominado)
            {
                vizinho->dominado = true;
                nos_dominados++;
            }
        }

        Candidatos.erase(Candidatos.begin() + i_prox);
        totA += it_aresta;
    }

    std::cout << "Pra pegar os vértices dominantes iterou " << i + c + totA << " vezes\n";

    return Dominantes;
}

par<bool, int>* CDS_guloso::adjDominante(No *node)
{
    bool adj_Dom{false};
    int n_adj_nD{0};

    for (Aresta *a : node->arestas)
    {
        No *vizinho = this->Hash_nodes->get(a->id_no_alvo);

        if (vizinho->dominante)
            adj_Dom = true;
        else if (!vizinho->dominado)
            n_adj_nD++;
    }

    return new par(adj_Dom, n_adj_nD);
}


//===========================================================================================================

vector<No *> CDS_guloso::CDS_teste(Grafo *grafo)
{
    this->Hash_nodes = new HASH_unica(grafo->lista_adj);
    Hash_nodes->InitHash(grafo->lista_adj);

    std::vector<No *> Candidatos = grafo->lista_adj, Dominantes;
    int nos_dominados{0}, total_nos = grafo->lista_adj.size();

    par<bool, int> *info_vertice_atual;

    int i{0}, c{0}, it_aresta{0}, totA{0};

    while (nos_dominados < total_nos)
    {
        i++;
        it_aresta = 0;

        No *prox{};
        int adj_ND_at{INT_MIN}, i_prox{};

        // monta o vetor de candidatos
        for (int i{0}; i < Candidatos.size(); i++)
        {
            c++;
            No *node = Candidatos.at(i);

            it_aresta += node->NaoDom.size();
            info_vertice_atual = adjDominante_teste(node);

            if (info_vertice_atual->getValue() == 0)
            {
                Candidatos.erase(Candidatos.begin() + i);
                continue;
            }

            if (info_vertice_atual->getKey() || nos_dominados == 0)
            {
                if (info_vertice_atual->getValue() > adj_ND_at)
                {
                    prox = node;
                    i_prox = i;
                    adj_ND_at = info_vertice_atual->getValue();
                }
            }
        }

        prox->dominante = true;

        if (!prox->dominado)
        {
            prox->dominado = true;
            nos_dominados++;
        }

        Dominantes.push_back(prox);

        for (Aresta *a : prox->arestas)
        {
            it_aresta++;

            No *vizinho = Hash_nodes->get(a->id_no_alvo);

            if (!vizinho->dominado)
            {
                vizinho->dominado = true;
                nos_dominados++;
            }
        }

        Candidatos.erase(Candidatos.begin() + i_prox);
        totA += it_aresta;
    }

    std::cout << "Pra pegar os vértices dominantes iterou " << i + c + totA << " vezes\n";

    return Dominantes;
}

par<bool, int> *CDS_guloso::adjDominante_teste(No *node)
{
    bool adj_Dom{false};
    int n_adj_nD{0};

    for (int i = node->NaoDom.size() - 1; i >= 0; i--)
    {
        Aresta *a = node->arestas[i];
        No *vizinho = this->Hash_nodes->get(a->id_no_alvo);

        if (vizinho->dominante)
            adj_Dom = true;
        else if (!vizinho->dominado)
            n_adj_nD++;
        else
            node->NaoDom.erase(node->NaoDom.begin() + i);
    }

    return new par(adj_Dom, n_adj_nD);
}