#include "CDS_guloso.h"


// Menor conjunto dominante possível
// Deve ser conexo
// todo vértice do grafo está ou no conjunto ou é adjacente a pelo menos um vértice do conjunto
// O(V² + 2VE)

Grafo* CDS_guloso::CDS(Grafo* grafo)
{
    this->Hash_nodes = new HASH_unica(grafo->lista_adj);
    Hash_nodes->InitHash(grafo->lista_adj);

    std::vector<std::string> listaArestas;
    std::vector<No *> Candidatos = grafo->lista_adj, Dominantes;
    int nos_dominados{0}, total_nos = grafo->lista_adj.size();

    par<bool, int> *info_vertice_atual;
    int i{0}, c{0}, it_aresta{0}, totA{0};

    while (nos_dominados < total_nos)
    {
        std::cout << "While Iterou " << i << " vezes\n";
        i++;
        it_aresta = 0;

        No *prox{};
        int adj_ND_at{INT_MIN}, i_prox{};

        // monta o vetor de candidatos
        for (int i{0}; i < Candidatos.size(); i++)
        {
            std::cout << "For iterou " << c << "vezes\n";
            c++;
            No *node = Candidatos.at(i);
            info_vertice_atual = adjDominante(node);

            std::cout << "AdjDominante iterou: " << node->arestas.size() << " vezes\n";
            it_aresta += node->arestas.size();

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
            std::cout << "for arestas Iterou " << it_aresta << " vezes\n";
            it_aresta++;

            No *vizinho = Hash_nodes->get(a->id_no_alvo);

            if (vizinho->dominante)
                listaArestas.push_back(std::string(1, vizinho->id) + " " + std::string(1, prox->id));

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

    std::cout << "Vértices Dominantes:\n";
    for (No *c : Dominantes)
        std::cout << c->id << " ";
    std::cout << "\n";

    std::ofstream arq("MCDC.txt");
    // cabeçalho do txt
    arq << grafo->in_direcionado << " " << grafo->in_ponderado_aresta << " " << grafo->in_ponderado_vertice << std::endl;
    arq << Dominantes.size() << std::endl;

    for (No *node : Dominantes)
        arq << node->id << "\n";

    for (std::string s : listaArestas)
        arq << s << "\n";

    arq.close();

    Grafo *ret = new Grafo();
    ret->montar_Grafo_por_arquivo("MCDC.txt");
    system("rm MCDC.txt");

    return ret;
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