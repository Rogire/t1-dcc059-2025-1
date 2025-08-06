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

        if (vizinho->dominado)
            node->NaoDom.erase(node->NaoDom.begin() + i);
    }
}

void reiniciar_variaveis(std::vector<No*>* Candidatos)
{
    for (No *n : *Candidatos)
    {
        n->NaoDom = n->arestas;
        n->dominante = false;
        n->dominado = false;
        n->adj_Dominante = false;
    }
}

void marcar_como_dominante(No *node, int &nos_dominados, std::vector<No *> *Dominantes, std::vector<No *> *Candidatos, HASH_unica *Hash_nodes)
{
    node->dominante = true;

    if (!node->dominado)
    {
        node->dominado = true;
        nos_dominados++;
    }

    Dominantes->push_back(node);

    for (Aresta *a : node->arestas)
    {
        No *vizinho = Hash_nodes->get(a->id_no_alvo);

        if (!vizinho->dominado)
        {
            vizinho->dominado = true;
            vizinho->adj_Dominante = true;
            nos_dominados++;
        }
    }
}

//===========================================================================================================

vector<No *> CDS_guloso::CDS(Grafo *grafo)
{
    this->Hash_nodes = new HASH_unica(grafo->lista_adj);
    Hash_nodes->InitHash(grafo->lista_adj);

    std::vector<No *> Candidatos = grafo->lista_adj, Dominantes;
    int nos_dominados{0}, total_nos = grafo->lista_adj.size();

    // reinicia os paramêtros dos nós para caso outros algoritmos já tenham sido rodados antes
    reiniciar_variaveis(&Candidatos);

    while (nos_dominados < total_nos)
    {
        No *prox{};
        int adj_ND_at{INT_MIN}, i_prox{-1};

        // monta o vetor de candidatos
        for (int j{0}; j < Candidatos.size(); j++)
        {
            No *node = Candidatos.at(j);

            if (node->NaoDom.empty())
            {
                Candidatos.erase(Candidatos.begin() + j);
                j--;
                continue;
            }

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
        marcar_como_dominante(prox, nos_dominados, &Dominantes, &Candidatos, Hash_nodes);

        Candidatos.erase(Candidatos.begin() + i_prox);

    }

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

    srand(static_cast<unsigned int>(time(NULL)));

    // reinicia os paramêtros dos nós para caso outros algoritmos já tenham sido rodados antes
    reiniciar_variaveis(&Candidatos);

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

    marcar_como_dominante(maior_ord, nos_dominados, &Dominantes, &Candidatos, Hash_nodes);

    while (nos_dominados < total_nos)
    {

        int Maior_n_adj_nd = INT_MIN;
        int Menor_n_adj_nd = INT_MAX;

        // encontra qual é o limite para o cálculo do limite
        for (int j = 0; j < Candidatos.size(); j++)
        {
            No *node = Candidatos[j];

            adjDominante(node);

            if ((nos_dominados == 0 && node->NaoDom.size() == 1) || node->NaoDom.empty())
            {
                Candidatos.erase(Candidatos.begin() + j);
                j--;
                continue;
            }

            if (node->adj_Dominante || nos_dominados == 0)
            {
                int valor = node->NaoDom.size();

                Maior_n_adj_nd = std::max(Maior_n_adj_nd, valor);
                Menor_n_adj_nd = std::min(Menor_n_adj_nd, valor);
            }
        }

        int limite_inferior = Maior_n_adj_nd - static_cast<int>(alpha * (Maior_n_adj_nd - Menor_n_adj_nd));
        std::vector<No*> RCL;

        // o vetor RCL é montado com uma parcela do vetor de candidatos original, de acordo com os elementos que possuirem
        // seu num adj nd maior que o limite mínimo estabelecido
        for (auto& candidato : Candidatos)
        {
            if(candidato->adj_Dominante || nos_dominados == 0)
                if (candidato->NaoDom.size() >= limite_inferior)
                    RCL.push_back(candidato);
        }

        if (RCL.empty())
        {
            for (auto &candidato : Candidatos)
            {
                if (candidato->adj_Dominante || nos_dominados == 0)
                    RCL.push_back(candidato);
            }
        }
     
        int indice_aleatorio = rand() % RCL.size();

        // Pega um aleatório dos melhores e adiciona aos dominantes, repete isso até acabar os elementos
        No *prox = RCL[indice_aleatorio];
        marcar_como_dominante(prox, nos_dominados, &Dominantes, &Candidatos, Hash_nodes);

        for (int j = 0; j < Candidatos.size(); j++)
        {
            if (Candidatos.at(j) == prox)
            {
                Candidatos.erase(Candidatos.begin() + j);
                break;
            }
        }

    }

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
