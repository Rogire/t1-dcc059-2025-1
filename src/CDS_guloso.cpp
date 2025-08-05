#include "CDS_guloso.h"
// Menor conjunto dominante possível
// Deve ser conexo
// todo vértice do grafo está ou no conjunto ou é adjacente a pelo menos um vértice do conjunto
// O(V² + VE)

std::pair<bool,int>*CDS_guloso::adjDominante(No *node)
{
    bool adj_Dom{false};
    int n_adj_nD{0};

    for (int i = node->NaoDom.size() - 1; i >= 0; i--)
    {
        Aresta *a = node->arestas[i];
        No *vizinho = this->Hash_nodes->get(a->id_no_alvo);

        if (vizinho->dominante)
        {
            adj_Dom = true;
            node->adj_Dominante = true;
        }
        else if (!vizinho->dominado)
            n_adj_nD++;
        else
            node->NaoDom.erase(node->NaoDom.begin() + i);
        
        }

    return new std::pair(adj_Dom, n_adj_nD);
}

//===========================================================================================================

vector<No *> CDS_guloso::CDS(Grafo *grafo)
{
    this->Hash_nodes = new HASH_unica(grafo->lista_adj);
    Hash_nodes->InitHash(grafo->lista_adj);

    std::vector<No *> Candidatos = grafo->lista_adj, Dominantes;
    int nos_dominados{0}, total_nos = grafo->lista_adj.size();

    //par<bool, int> *info_vertice_atual;
    std::pair<bool, int> *info_vertice_atual;
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
        int adj_ND_at{INT_MIN}, i_prox{};

        // monta o vetor de candidatos
        for (int i{0}; i < Candidatos.size(); i++)
        {
            c++;
            No *node = Candidatos.at(i);

            if(nos_dominados == 0 && node->NaoDom.empty())
                node->NaoDom = node->arestas;

            it_aresta += node->NaoDom.size();
            info_vertice_atual = adjDominante(node);

            if (info_vertice_atual->second == 0)
            {
                Candidatos.erase(Candidatos.begin() + i);
                i--;
                continue;
            }


            if(info_vertice_atual->first || nos_dominados == 0)
            {
                if (info_vertice_atual->second > adj_ND_at)
                {
                    prox = node;
                    i_prox = i;
                    adj_ND_at = info_vertice_atual->second;
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
    std::pair<bool, int> *info_vertice_atual;
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

        std::vector<std::pair<No *, int>> candidatos_com_valor;

        // monta o vetor com os melhores candidatos (adjacentes a dominante e com maior num adj nd)
        for (int j = 0; j < Candidatos.size(); j++)
        {
            No *node = Candidatos[j];
            it_aresta += node->NaoDom.size();
            info_vertice_atual = adjDominante(node);

            int num_adj_nd_atual = info_vertice_atual->second;
            bool atual_eh_adj_dom = info_vertice_atual->first;

            if (num_adj_nd_atual == 0)
            {
                Candidatos.erase(Candidatos.begin() + j);
                j--;
                continue;
            }

            if (atual_eh_adj_dom || nos_dominados == 0)
            {
                int valor = num_adj_nd_atual;
                candidatos_com_valor.push_back({node, valor});

                Maior_n_adj_nd = std::max(Maior_n_adj_nd, valor);
                Menor_n_adj_nd = std::min(Menor_n_adj_nd, valor);
            }
        }

        int limite_inferior = Maior_n_adj_nd - static_cast<int>(alpha * (Maior_n_adj_nd - Menor_n_adj_nd));
        std::vector<std::pair<No *, int>> RCL;

        //o vetor RCL é montado com uma parcela do vetor de candidatos original, de acordo com os elementos que possuirem
        //seu num adj nd maior que o limite mínimo estabelecido 
        for (auto &candidato : candidatos_com_valor)
            if (candidato.second >= limite_inferior)
                RCL.push_back(candidato);
        
        if (RCL.empty())
            break;

        int indice_aleatorio = rand() % RCL.size();

        //Pega um aleatório dos melhores e adiciona aos dominantes, repete isso até acabar os elementos
        No *prox = RCL[indice_aleatorio].first;
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
            i++;
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
    
