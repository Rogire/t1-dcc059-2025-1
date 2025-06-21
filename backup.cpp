#include "Grafo.h"

// Função que retorna o índice do nó com identificador 'id' na lista de adjacência
int Grafo::indice_no(char id)
{
    // Percorre toda a lista de nós
    for (int i = 0; i < (int)lista_adj.size(); i++)
    {
        // Se o id do nó atual for igual ao procurado, retorna o índice i
        if (lista_adj[i]->id == id)
            return i;
    }
    // Caso não encontre, retorna -1 indicando nó não encontrado
    return -1;
}

// Função que imprime o grafo na tela
void Grafo::imprimir_grafo()
{
    cout << "Grafo "
         << (in_direcionado ? "Direcionado" : "Nao direcionado") << ", "
         << (in_ponderado_vertice ? "Ponderado nos vertices" : "Nao ponderado nos vertices") << ", "
         << (in_ponderado_aresta ? "Ponderado nas arestas" : "Nao ponderado nas arestas")
         << endl;

    // Percorre todos os nós da lista de adjacência
    for (No *i : lista_adj)
    {
        // Imprime o identificador do nó
        cout << i->id;

        // Se o grafo for ponderado nos vértices, imprime também o peso do vértice
        if (in_ponderado_vertice)
            cout << "(" << i->peso << ")";

        cout << " -> ";

        // Percorre todas as arestas do nó atual
        for (Aresta *j : i->arestas)
        {
            // Imprime o identificador do nó alvo da aresta
            cout << j->id_no_alvo;

            // Se o grafo for ponderado nas arestas, imprime também o peso da aresta
            if (in_ponderado_aresta)
                cout << "(" << j->peso << ")" << " ";
            else
                cout << " ";
        }

        cout << endl; // Quebra de linha para o próximo nó
    }

    cout << endl; // Linha extra após imprimir todo o grafo
}

// Construtor do grafo que recebe o nome do arquivo com os dados do grafo
Grafo::Grafo(const string &nome_arquivo)
{
    ifstream arquivo(nome_arquivo); // Abre o arquivo

    // Verifica se o arquivo foi aberto corretamente
    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo!" << endl;
        exit(-1); // Encerra o programa em caso de erro
    }

    // Lê do arquivo as informações iniciais:
    // se é direcionado, se tem peso nos vértices, se tem peso nas arestas, e a ordem (número de nós)
    arquivo >> in_direcionado >> in_ponderado_vertice >> in_ponderado_aresta >> ordem;

    // Lê os nós do grafo
    for (int i = 0; i < ordem; i++)
    {
        No *x;
        char id;
        arquivo >> id; // lê o identificador do nó

        // Se o grafo tem peso nos vértices, lê o peso também
        if (in_ponderado_vertice)
        {
            int peso;
            arquivo >> peso;
            x = new No(id, peso); // cria nó com peso
        }
        else
        {
            x = new No(id); // cria nó sem peso
        }

        lista_adj.push_back(x); // adiciona o nó na lista de adjacência
    }

    // Consome o caractere de nova linha após os nós para preparar leitura das arestas
    string linha;
    getline(arquivo, linha);

    // Lê as arestas enquanto houver linhas no arquivo
    while (getline(arquivo, linha))
    {
        istringstream iss(linha); // cria um stream a partir da linha para facilitar a leitura

        char v1, v2;
        iss >> v1 >> v2; // lê os identificadores dos nós que formam a aresta

        // Busca o índice dos nós na lista de adjacência usando a função auxiliar
        int idx_v1 = indice_no(v1);
        int idx_v2 = indice_no(v2);

        // Verifica se os índices foram encontrados, senão mostra erro e ignora essa aresta
        if (idx_v1 == -1 || idx_v2 == -1)
        {
            cerr << "Erro: nó não encontrado no grafo: " << v1 << " ou " << v2 << endl;
            continue; // pula para próxima linha
        }

        // Caso o grafo tenha peso nas arestas, lê o peso
        if (in_ponderado_aresta)
        {
            int peso;
            iss >> peso;

            // Cria uma aresta do v1 para v2 com peso
            Aresta *final = new Aresta(v2, peso);
            lista_adj[idx_v1]->arestas.push_back(final);

            // Se o grafo não for direcionado, adiciona a aresta inversa (v2 para v1)
            if (!in_direcionado)
            {
                Aresta *origem = new Aresta(v1, peso);
                lista_adj[idx_v2]->arestas.push_back(origem);
            }
        }
        else
        { // Se não tem peso nas arestas
            // Cria aresta do v1 para v2 sem peso
            Aresta *final = new Aresta(v2);
            lista_adj[idx_v1]->arestas.push_back(final);

            // Se o grafo não for direcionado, adiciona aresta inversa (v2 para v1)
            if (!in_direcionado)
            {
                Aresta *origem = new Aresta(v1);
                lista_adj[idx_v2]->arestas.push_back(origem);
            }
        }
    }

    // Ordena as arestas de cada nó pelo id do nó de destino
    for (No *i : lista_adj)
    {
        std::sort(i->arestas.begin(), i->arestas.end(), [](Aresta *a, Aresta *b)
                  { return a->id_no_alvo < b->id_no_alvo; });
    }

    // Imprime o grafo após a leitura
    imprimir_grafo();
}

// Destrutor do grafo (aqui você deve liberar memória, se desejar implementar)
Grafo::~Grafo()
{
    // Libera memória dos nós e arestas (recomendado implementar isso)
    for (No *no : lista_adj)
    {
        for (Aresta *aresta : no->arestas)
        {
            delete aresta;
        }
        delete no;
    }
}

vector<char> Grafo::fecho_transitivo_direto(char id_no)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::fecho_transitivo_indireto(char id_no)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

Grafo *Grafo::arvore_geradora_minima_prim(vector<char> ids_nos)
{
    cout << "Metodo nao implementado" << endl;
    return nullptr;
}

Grafo *Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos)
{
    cout << "Metodo nao implementado" << endl;
    return nullptr;
}

//=========================================================================

// classe auxiliar
template <typename K, typename V>
class par
{
public:
    par(K k, V v) : key(k), value(v) {}

    K getKey() { return key; };
    V getValue() { return value; };

    void setKey(K k) { key = k; };
    void setValue(V v) { value = v; };

private:
    K key;
    V value;
};

//=========================================================================
Grafo *Grafo::arvore_caminhamento_profundidade(char id_no)
{
    std::vector<char> JaFoi;
    std::vector<par<No *, bool>> JaPassou;
    std::vector<par<No *, int>> OrdemVisitacao;

    No *comeco;

    for (No *no : this->lista_adj)
    {
        if (no->id == id_no)
        {
            comeco = no;
            break;
        }
    }

    for (auto node : this->lista_adj)
    {
        if (std::find(JaFoi.begin(), JaFoi.end(), node->id) == JaFoi.end()) // se não passou pelo nó
        {
            JaFoi.push_back(node->id);
            PROF(node, JaFoi);
        }
    }
    return nullptr;
};

void PROF(No *NoAt, std::vector<char> JaP)
{
    for (auto &atual : NoAt->arestas)
    {
        char at = atual->id_no_alvo;

        if (std::find_if(JaP.begin(), JaP.end(), [at](const char &it)
                         { return at == it; }) == JaP.end()) // se não passou pelo nó
        {
            JaP.push_back(at);
            PROF(node, JaP);
        }
    }

    for (int i{0}; i < static_cast<int>(NoAt->arestas.size()); i++)
    {
        char atual = NoAt->arestas[i]->id_no_alvo;

        if (std::find(JaP.begin(), JaP.end(), atual) == JaP.end()) // se não passou pelo nó
        {
            JaP.push_back(atual);
            PROF(node, JaFoi);
        }

        for (auto &par : JaP)
        {
            if (par.getKey()->id == prox)
            {
                if (!par.getValue())
                    PROF(par.getKey(), JaP);
                break;
            }
        }
    }
}

int Grafo::raio()
{
    cout << "Metodo nao implementado" << endl;
    return 0;
}

int Grafo::diametro()
{
    cout << "Metodo nao implementado" << endl;
    return 0;
}

vector<char> Grafo::centro()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::periferia()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::vertices_de_articulacao()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}
