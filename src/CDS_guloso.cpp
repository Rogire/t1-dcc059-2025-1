#include "CDS_guloso.h"
// Menor conjunto dominante possível
// Deve ser conexo
// todo vértice do grafo está ou no conjunto ou é adjacente a pelo menos um vértice do conjunto
// O(V² + VE)

void CDS_guloso::adjDominante(No *node)
{
    for (int i = node->NaoDom.size() - 1; i >= 0; i--)
    {
        Aresta *a = node->NaoDom[i];
        No *vizinho = this->Hash_nodes->get(a->id_no_alvo);

        if (vizinho->dominante)
            node->adj_Dominante = true;
        
        if (vizinho->dominado)
            node->NaoDom.erase(node->NaoDom.begin() + i);
    }
}

//===========================================================================================================

vector<No *> CDS_guloso::CDS(Grafo *grafo)
{
    this->Hash_nodes = new HASH_unica(grafo->lista_adj);
    Hash_nodes->InitHash(grafo->lista_adj);

    std::vector<No *> Candidatos = grafo->lista_adj, Dominantes;
    int nos_dominados{0}, total_nos = grafo->lista_adj.size();

    //par<bool, int> *info_vertice_atual;
    //std::pair<bool, int> *info_vertice_atual;
    int i{0}, c{0}, it_aresta{0}, totA{0};
    
    for (No *n : Candidatos)
    {
        i++;
        n->NaoDom = n->arestas;
        n->dominante = false;
        n->dominado = false;
        n->adj_Dominante = false;
    }

    while (nos_dominados < total_nos)
    {
        i++;
        it_aresta = 0;

        No *prox{};
        int adj_ND_at{INT_MIN}, i_prox{-1};

        // monta o vetor de candidatos
        for (int j{0}; j < Candidatos.size(); j++)
        {
            c++;
            No *node = Candidatos.at(j);

            if (node->NaoDom.empty())
            {
                Candidatos.erase(Candidatos.begin() + j);
                j--;
                continue;
            }

            it_aresta += node->NaoDom.size();
            adjDominante(node);

            if(node->adj_Dominante || nos_dominados == 0)
            {
                if (static_cast<int>(node->NaoDom.size()) > adj_ND_at)
                {
                    prox = node;
                    i_prox = j;
                    adj_ND_at = static_cast<int>(node->NaoDom.size());
                }
            }
        }

        if(prox == nullptr || i_prox == -1)
        {
            std::cerr << "Nenhum candidato.\n";
            break;
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
        i--;
        totA += it_aresta;
    }

    std::cout << "Pra pegar os vértices dominantes iterou " << i + c + totA << " vezes\n";

    return Dominantes;
}

vector<No *> CDS_guloso::CDS_teste_randomizado(Grafo *grafo, float alpha)
{
    this->Hash_nodes = new HASH_unica(grafo->lista_adj);
    Hash_nodes->InitHash(grafo->lista_adj);

    std::vector<No *> Candidatos = grafo->lista_adj, Dominantes;
    int nos_dominados{0}, total_nos = grafo->lista_adj.size();
    int i{0}, c{0}, it_aresta{0}, totA{0};

    srand(static_cast<unsigned int>(time(NULL)));

    //reinicia os paramêtros dos nós para caso outros algoritmos já tenham sido rodados antes
    for (No *n : Candidatos)
    {
        i++;
        n->NaoDom = n->arestas;
        n->dominante = false;
        n->dominado = false;
        n->adj_Dominante = false;
    }

    while (nos_dominados < total_nos)
    {
        i++;
        it_aresta = 0;

        int Maior_n_adj_nd = INT_MIN;
        int Menor_n_adj_nd = INT_MAX;

        // encontra qual é o limite para o cálculo do limite
        for (int j = 0; j < Candidatos.size(); j++)
        {
            c++;
            No *node = Candidatos[j];

            if (node->NaoDom.empty())
            {
                std::cout << "Node: " << node->id << " removido\n";
                Candidatos.erase(Candidatos.begin() + j);
                j--;
                continue;
            }
            it_aresta += node->NaoDom.size();
            
            adjDominante(node);

            if (node->adj_Dominante || nos_dominados == 0)
            {
                int valor = node->NaoDom.size();

                Maior_n_adj_nd = std::max(Maior_n_adj_nd, valor);
                Menor_n_adj_nd = std::min(Menor_n_adj_nd, valor);
            }
        }

        int limite_inferior = Maior_n_adj_nd - static_cast<int>(alpha * (Maior_n_adj_nd - Menor_n_adj_nd));
        std::vector<No *> RCL;

        // o vetor RCL é montado com uma parcela do vetor de candidatos original, de acordo com os elementos que possuirem
        // seu num adj nd maior que o limite mínimo estabelecido
        for (auto &candidato : Candidatos)
        {
            c++;
            if (candidato->NaoDom.size() >= limite_inferior)
                RCL.push_back(candidato);
        }
            
        
        if (RCL.empty())
            break;

        int indice_aleatorio = rand() % RCL.size();

        //Pega um aleatório dos melhores e adiciona aos dominantes, repete isso até acabar os elementos
        No *prox = RCL[indice_aleatorio];
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

        for (int j = 0; j < Candidatos.size(); j++)
        {
            c++;
            if (Candidatos[j] == prox)
            {
                Candidatos.erase(Candidatos.begin() + j);
                break;
            }
        }

        totA += it_aresta;
    }

    std::cout << "Algoritmo randomizado iterou " << i + c + totA << " vezes\n";
    return Dominantes;
}

std::vector<No *> CDS_guloso::CDS_teste_randomizado_reativo(Grafo *grafo, float alpha)
{
    return {};
}
    
