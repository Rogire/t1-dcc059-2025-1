#include "CDS_guloso.h"
#include "No.h"
#include <vector>
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

            if ((nos_dominados == 0 && node->NaoDom.size() == 1) || node->NaoDom.empty())
            {
                Candidatos.erase(Candidatos.begin() + j);
                j--;
                continue;
            }

            if(nos_dominados != 0)
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

int get_alpha_index(std::vector<std::vector<elemento>>*& valores_rand_alphas, float alpha)
{
    int i=0;
    for(auto el : *valores_rand_alphas)
    {
        if(!el.empty())
            if(el[0].alpha == alpha)
                return i;
        i++;
    }

    return -1;
}

std::vector<No*> CDS_guloso::Construtivo_randomizado(Grafo *grafo,float alpha)
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

    // reinicia os paramêtros dos nós para caso outros algoritmos já tenham sido rodados antes
    reiniciar_variaveis(&Candidatos);

    int maior_primeiro{INT_MIN}, i_primeiro{};

    //pega o primeiro nó de forma gulosa para otimizar o algoritmo (não é estritamente necessário, mas melhora performance)
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

            if(nos_dominados != 0)
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
        for(auto& candidato : Candidatos)
        {
            if(candidato->adj_Dominante || nos_dominados == 0)
                if (candidato->NaoDom.size() >= limite_inferior)
                    RCL.push_back(candidato);
        }

        if(RCL.empty())
        {
            for (auto &candidato : Candidatos)
            {
                if (candidato->adj_Dominante || nos_dominados == 0)
                    RCL.push_back(candidato);
            }
        }
        
        if(RCL.empty() && grafo->in_direcionado)
            for (auto &candidato : Candidatos)
                RCL.push_back(candidato);

        if(RCL.size() == 0)
        {
            std::cerr << "Nenhum candidato.\n";
            exit(0);
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

vector<No *> CDS_guloso::CDS_randomizado(Grafo *grafo, float alpha)
{
    int menor_tam = INT_MAX;
    std::vector<No*> resultado;
    std::vector<No*> melhor_resultado;

    int num_it = 30;
    
   for(int i{0};i<num_it;i++)
   {
        resultado = Construtivo_randomizado(grafo, alpha);
        if(resultado.size() < menor_tam)
        {
            menor_tam = resultado.size();
            melhor_resultado = resultado;   
        }
   }

   return melhor_resultado;
}



//==========================================================================================================================
elemento CDS_guloso::CDS_randomizado_reativo(Grafo *grafo, std::vector<float> alphas, int numIter,int bloco,bool printar)
{
    int numAlphas = static_cast<int>(alphas.size());
    float probInicial = 1.0f/numAlphas;
    float dif = 0.1f;
    
    int melhor_tamanho{INT_MAX};
    float melhor_alpha{};
    double tempo_melhor_solucao{INT_MAX};
    std::vector<No *> Melhor_solucao;
    double melhor_media_tamanho{INT_MAX}, melhor_media_tempo{INT_MAX};

    std::vector<int>    usos(numAlphas, 0);
    std::vector<int>    Tamanhos(numAlphas,0);
    std::vector<double> Tempos(numAlphas, 0.0);
    std::vector<float>  probabilidades(numAlphas,probInicial);
    std::vector<double> mediaTam(numAlphas, 0), mediaTempo(numAlphas, 0);

    elemento res;

    for (int i{0}; i < numIter; i++)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::discrete_distribution<> dist(probabilidades.begin(), probabilidades.end());
        
        int pos = dist(gen);
        float alpha_at = alphas[pos];

        auto ini = std::chrono::high_resolution_clock::now();
        vector<No *> resG = CDS_randomizado(grafo, alpha_at);
        auto fim = std::chrono::high_resolution_clock::now();

        double tempo_at = std::chrono::duration<double>(fim - ini).count();
        int t_at = static_cast<int>(resG.size());

        usos[pos]++;
        Tamanhos[pos] += t_at;
        Tempos[pos] += tempo_at;

        if(t_at < melhor_tamanho)
        {
            tempo_melhor_solucao = tempo_at;
            res.solucao = resG;//Melhor_solucao;
            res.tempo =   tempo_at;//tempo_melhor_solucao;
            res.alpha =  alpha_at;//melhor_alpha;
        }
    
        if (i % bloco == 0)
        {
            std::vector<double> qualidades(numAlphas, 0.0);
            double somaQuali{0.0};
            for (int i = 0; i < numAlphas; i++)
                if (usos[i] > 0)
                {
                    qualidades[i] = 1.0 / ((double)Tamanhos[i] / usos[i]);
                    somaQuali += qualidades[i];
                }

            if (somaQuali > 0)
                for (int i = 0; i < numAlphas; i++)
                    probabilidades[i] = (qualidades[i]+dif) / (somaQuali+dif);
        }
    }

    for (int i = 0; i < numAlphas; i++)
    {
        mediaTam[i] = usos[i] ? (double)Tamanhos[i] / usos[i] : 0;
        mediaTempo[i] = usos[i] ? Tempos[i] / usos[i] : 0;

        if (mediaTam[i] < melhor_media_tamanho)
        {
            melhor_media_tamanho = mediaTam[i];
            //es.alpha = alphas[i];
            melhor_alpha = alphas[i];
        }
    }

    if(printar)
    {
        // Imprime o resultado final do algoritmo
        std::cout << "\n=== Resultado Final ===\n";
        std::cout << "Melhor solução encontrada: { ";
        for (No* no : Melhor_solucao)
            std::cout << no->id << " ";
        
            std::cout << "}\nTamanho: " << res.solucao.size() << "\nAlpha correspondente: " << res.alpha << "\n\n";
        //std::cout << "}\nTamanho: " << melhor_tamanho << "\nAlpha correspondente: " << melhor_alpha << "\n\n";
        std::cout << "Médias por α:\n";

        for (int i = 0; i < numAlphas; i++)
        {
            std::cout << "α = " << alphas[i] << " | Média tamanho: " << mediaTam[i] << " | Média tempo (s): " << mediaTempo[i] << "\n";
        }
    }

    return res;
}