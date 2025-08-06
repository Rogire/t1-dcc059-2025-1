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
    if(alpha > 1)
    {
        std::cerr << "ERRO: alpha deve ser um número entre 0 e 1.\n";
        return {};
    }
    this->Hash_nodes = new HASH_unica(grafo->lista_adj);
    Hash_nodes->InitHash(grafo->lista_adj);

    std::vector<No *> Candidatos = grafo->lista_adj, Dominantes;
    int nos_dominados{0}, total_nos = grafo->lista_adj.size();
    int i{0}, c{0}, it_aresta{0}, totA{0};

    srand(static_cast<unsigned int>(time(NULL)));

    // reinicia os paramêtros dos nós para caso outros algoritmos já tenham sido rodados antes
    for (No *n : Candidatos)
    {
        i++;
        n->NaoDom = n->arestas;
        n->dominante = false;
        n->dominado = false;
        n->adj_Dominante = false;
    }

    int maior_primeiro{INT_MIN}, i_primeiro{};

    No *maior_ord{};
    for (int j = 0; j < Candidatos.size(); j++)
    {
        No *node = Candidatos.at(j);
        
        if(static_cast<int>(node->NaoDom.size()) > maior_primeiro)
        {
            maior_primeiro = node->NaoDom.size();
            i_primeiro = j;
            maior_ord = node;
        }
    }

    maior_ord->dominante = true;

    if (!maior_ord->dominado)
    {
        maior_ord->dominado = true;
        nos_dominados++;
    }

    Dominantes.push_back(maior_ord);

    for (Aresta *a : maior_ord->arestas)
    {
        it_aresta++;
        No *vizinho = Hash_nodes->get(a->id_no_alvo);

        if (!vizinho->dominado)
        {
            vizinho->dominado = true;
            vizinho->adj_Dominante = true;
            nos_dominados++;
        }
    }

    for (int j = 0; j < Candidatos.size(); j++)
    {
        c++;
        if (Candidatos[j] == maior_ord)
        {
            std::cout << "Nó removido: " << maior_ord->id << '\n';
            Candidatos.erase(Candidatos.begin() + j);
            break;
        }
    }
    totA += it_aresta;

    while (nos_dominados < total_nos)
    {
        i++;
        it_aresta = 0;

        int Maior_n_adj_nd = INT_MIN;
        int Menor_n_adj_nd = INT_MAX;

        std::cout << "Candidatos:\n";
        for (auto c : Candidatos)
            std::cout << c->id << " ";
        std::cout << "\n";

        // encontra qual é o limite para o cálculo do limite
        for (int j = 0; j < Candidatos.size(); j++)
        {
            c++;
            No *node = Candidatos[j];

            it_aresta += node->NaoDom.size();

            for (int i = node->NaoDom.size() - 1; i >= 0; i--)
            {
                Aresta *a = node->NaoDom[i];
                No *vizinho = this->Hash_nodes->get(a->id_no_alvo);

                if (vizinho->dominado)
                    node->NaoDom.erase(node->NaoDom.begin() + i);
            }

            if((nos_dominados == 0 && node->NaoDom.size() == 1) || node->NaoDom.empty())
            {
                std::cout << "Node: " << node->id << " removido\n";
                Candidatos.erase(Candidatos.begin() + j);
                j--;
                continue;
            }
            std::cout << "node " << node->id << ((node->adj_Dominante) ? (" é adj dominante\n") : (" não é adj dominante\n"));
            if (node->adj_Dominante || nos_dominados == 0)
            {
                int valor = node->NaoDom.size();

                Maior_n_adj_nd = std::max(Maior_n_adj_nd, valor);
                Menor_n_adj_nd = std::min(Menor_n_adj_nd, valor);
            }
        }

        std::cout << "acabou for candidatos\n";

        std::cout << Maior_n_adj_nd<<", ";
        std::cout << Menor_n_adj_nd<<"\n";
        int limite_inferior = Maior_n_adj_nd - static_cast<int>(alpha * (Maior_n_adj_nd - Menor_n_adj_nd));
        std::vector<No*> RCL;

        // o vetor RCL é montado com uma parcela do vetor de candidatos original, de acordo com os elementos que possuirem
        // seu num adj nd maior que o limite mínimo estabelecido
        for (auto& candidato : Candidatos)
        {
            std::cout << "node " << candidato->id << ((candidato->adj_Dominante)?(" é adj dominante\n"):(" não é adj dominante\n"));
            std::cout << candidato->NaoDom.size() << ", " << limite_inferior <<", "<<(candidato->NaoDom.size() >= limite_inferior ? "sim" : "não")<<"\n";
            std::cout<< "nos dominados: " << nos_dominados << "\n";

            c++;
            if(candidato->adj_Dominante || nos_dominados == 0)
                if (candidato->NaoDom.size() >= limite_inferior)
                    RCL.push_back(candidato);
        }

        if (RCL.empty())
        {
            std::cout << "Entrou pq ngm bateu o limite\n";
            for (auto &candidato : Candidatos)
            {
                c++;
                if (candidato->adj_Dominante || nos_dominados == 0)
                    RCL.push_back(candidato);
            }
        }
     
        int indice_aleatorio = rand() % RCL.size();

        std::cout << "RCL:\n";
        for(auto c : RCL)
            std::cout << c->id << " ";
        std::cout << "\n";

        // Pega um aleatório dos melhores e adiciona aos dominantes, repete isso até acabar os elementos
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
                vizinho->adj_Dominante = true;
                nos_dominados++;
            }
        }

        for (int j = 0; j < Candidatos.size(); j++)
        {
            c++;
            if (Candidatos[j] == prox)
            {
                std::cout << "Nó removido: " << prox->id << '\n';
                Candidatos.erase(Candidatos.begin() + j);
                break;
            }
        }
        totA += it_aresta;
    }

    std::cout << "Algoritmo randomizado iterou " << i + c + totA << " vezes\n";
    return Dominantes;
}

class APQM
{
public:
    APQM(float alpha, float probI)
    {
        this->alp = alpha;
        this->prob = probI;
    };
    ~APQM() {};

private:
    float alp;
    float med = -1;
    float q = -1;
    float prob;
};

std::vector<No *> CDS_guloso::CDS_teste_randomizado_reativo(Grafo *grafo, std::vector<float> alphas)
{
    float probInicial = 1 / static_cast<int>(alphas.size());

    std::vector<APQM> valoresCalc;

    for(float a : alphas)
        valoresCalc.push_back(APQM(a,probInicial));

    

    //std::vector<No*> Dominantes = CDS_teste_randomizado(grafo, alpha);

    return {}; // Dominantes;
}
