#include "Grafo.h"

// Função que retorna o índice do nó com identificador 'id' na lista de adjacência
int Grafo::indice_no(char id) {
    // Percorre toda a lista de nós
    for (int i = 0; i < (int)lista_adj.size(); i++) {
        // Se o id do nó atual for igual ao procurado, retorna o índice i
        if (lista_adj[i]->id == id)
            return i;
    }
    // Caso não encontre, retorna -1 indicando nó não encontrado
    return -1;
}

// Função que imprime o grafo na tela
void Grafo::imprimir_grafo() {
    cout << "Grafo "
         << (in_direcionado ? "Direcionado" : "Nao direcionado") << ", "
         << (in_ponderado_vertice ? "Ponderado nos vertices" : "Nao ponderado nos vertices") << ", "
         << (in_ponderado_aresta ? "Ponderado nas arestas" : "Nao ponderado nas arestas")
         << endl;

    // Percorre todos os nós da lista de adjacência
    for (No* i : lista_adj) {
        // Imprime o identificador do nó
        cout << i->id;

        // Se o grafo for ponderado nos vértices, imprime também o peso do vértice
        if (in_ponderado_vertice)
            cout << "(" << i->peso << ")";

        cout << " -> ";

        // Percorre todas as arestas do nó atual
        for (Aresta* j : i->arestas) {
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
Grafo::Grafo(const string& nome_arquivo) {
    ifstream arquivo(nome_arquivo); // Abre o arquivo

    // Verifica se o arquivo foi aberto corretamente
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        exit(-1); // Encerra o programa em caso de erro
    }

    // Lê do arquivo as informações iniciais:
    // se é direcionado, se tem peso nos vértices, se tem peso nas arestas, e a ordem (número de nós)
    arquivo >> in_direcionado >> in_ponderado_vertice >> in_ponderado_aresta >> ordem;

    // Lê os nós do grafo
    for (int i = 0; i < ordem; i++) {
        No* x;
        char id;
        arquivo >> id; // lê o identificador do nó

        // Se o grafo tem peso nos vértices, lê o peso também
        if (in_ponderado_vertice) {
            int peso;
            arquivo >> peso;
            x = new No(id, peso); // cria nó com peso
        } else {
            x = new No(id); // cria nó sem peso
        }

        lista_adj.push_back(x); // adiciona o nó na lista de adjacência
    }

    // Consome o caractere de nova linha após os nós para preparar leitura das arestas
    string linha;
    getline(arquivo, linha);

    // Lê as arestas enquanto houver linhas no arquivo
    while (getline(arquivo, linha)) {
        istringstream iss(linha); // cria um stream a partir da linha para facilitar a leitura

        char v1, v2;
        iss >> v1 >> v2; // lê os identificadores dos nós que formam a aresta

        // Busca o índice dos nós na lista de adjacência usando a função auxiliar
        int idx_v1 = indice_no(v1);
        int idx_v2 = indice_no(v2);

        // Verifica se os índices foram encontrados, senão mostra erro e ignora essa aresta
        if (idx_v1 == -1 || idx_v2 == -1) {
            cerr << "Erro: nó não encontrado no grafo: " << v1 << " ou " << v2 << endl;
            continue; // pula para próxima linha
        }

        // Caso o grafo tenha peso nas arestas, lê o peso
        if (in_ponderado_aresta) {
            int peso;
            iss >> peso;

            // Cria uma aresta do v1 para v2 com peso
            Aresta* final = new Aresta(v2, peso);
            lista_adj[idx_v1]->arestas.push_back(final);

            // Se o grafo não for direcionado, adiciona a aresta inversa (v2 para v1)
            if (!in_direcionado) {
                Aresta* origem = new Aresta(v1, peso);
                lista_adj[idx_v2]->arestas.push_back(origem);
            }

        } else { // Se não tem peso nas arestas
            // Cria aresta do v1 para v2 sem peso
            Aresta* final = new Aresta(v2);
            lista_adj[idx_v1]->arestas.push_back(final);

            // Se o grafo não for direcionado, adiciona aresta inversa (v2 para v1)
            if (!in_direcionado) {
                Aresta* origem = new Aresta(v1);
                lista_adj[idx_v2]->arestas.push_back(origem);
            }
        }
    }

    // Ordena as arestas de cada nó pelo id do nó de destino
    for (No* i : lista_adj) {
        std::sort(i->arestas.begin(), i->arestas.end(), [](Aresta* a, Aresta* b) {
            return a->id_no_alvo < b->id_no_alvo;
        });
    }

    // Imprime o grafo após a leitura
    imprimir_grafo();
}

// Destrutor do grafo (aqui você deve liberar memória, se desejar implementar)
Grafo::~Grafo() {
    // Libera memória dos nós e arestas (recomendado implementar isso)
    for (No* no : lista_adj) {
        for (Aresta* aresta : no->arestas) {
            delete aresta;
        }
        delete no;
    }
}

vector<char> Grafo::fecho_transitivo_direto(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::fecho_transitivo_indireto(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

//=====================classes auxiliares================================

template <typename K, typename V > class par
{
    public:
        par(K k, V v) : key(k), value(v){} 
        par(){}

        K getKey() { return key; };
        V getValue() { return value; };

        void setKey(K k) { key = k;};
        void setValue(V v) { value = v;};

    private:
        K key;
        V value;
};

template <typename K, typename V> class HASH
{
    public:
        HASH(std::vector<K>& lista_adj)
        {
            if (lista_adj.empty())
            {
                std::cerr << "Erro: lista_adj está vazia!" << std::endl;
                return;
            }

            int maior = lista_adj.at(0)->id;

            for (auto No : lista_adj)
                if (No->id > maior)
                    maior = No->id;

            this->tam = maior + 1;

            this->Hash = new par<K, V> *[this->tam];

            for (int i = 0; i < this->tam; i++)
                this->Hash[i] = nullptr;
        };

        void InitHash(std::vector<K> &lista_adj, V valInicial)
        {
            for (auto node : lista_adj)
            {
                if (node->id >= 0 && node->id < this->tam)
                {
                    if (this->Hash[node->id] != nullptr)
                        delete this->Hash[node->id];

                    this->Hash[node->id] = new par<K, V>(node, valInicial);
                }
            }
        }

        void printHash(std::vector<K> &lista_adj)
        {
            std::cout <<" HASH MONTADA: \n";

            for (auto node : lista_adj)
            {
                std::cout << this->Hash[node->id]->getKey()->id << " \n";
            }
        }

        ~HASH()
        {
            for(int i{0}; i < tam; i++)
                delete this->Hash[i];

            delete[] this->Hash;
        }

        par<K,V>* get(int id)
        {
            if (id >=0 && id<tam)
                return this->Hash[id];

            return nullptr;
        }

    private:
        par<K,V> **Hash;
        int tam{};
};
//=========================================================================
// TODO: ACHO QUE TÁ PRONTO, FALTA TESTAR

// Adiciona as arestas lista no formato que elas vão ser escritas no grafo "x y"
// A adição das arestas é feita fora da verificação se já passou ou não, pra evitar
// repetições de arestas invertidas ex: "a b" e "b a", ele percorre e compara se
// o inverso da aresta atual já está lista, se não está, adiciona
void PROF(No *NoAt, HASH<No *, bool>*hash_nodes, std::vector<std::string> *listaAdjRet)
{
    for (Aresta *at : NoAt->arestas)
    {   
        par<No*, bool> *ParAt = hash_nodes->get(at->id_no_alvo);
        bool add{};
        std::string strAtual = std::string(1, NoAt->id) + " " + std::string(1, ParAt->getKey()->id);

        for (std::string str : *listaAdjRet)
        {
            add = true;

            if (str == strAtual)
            {
                add = false;
                break;
            }

            // evita duplicatas (a b, e b a)
            if (str.size() == 3 && strAtual.size() == 3)
                if (strAtual[0] == str[2] && strAtual[2] == str[0])
                {
                    add = false;
                    break;
                }
        }
        if (add)
            listaAdjRet->push_back(strAtual);  
        
            
        // se não passou pelo vertice atual
        if (!ParAt->getValue())
        {
            ParAt->setValue(true);
            // PROF(vertice Atual, Hash, lista Strings)
            PROF(ParAt->getKey(), hash_nodes, listaAdjRet);
        }
    }
}

void PROF_teste(No *NoAt, HASH<No *, bool> *hash_nodes, std::vector<par<std::string,int>> *listaAdjRet)
{
    for (Aresta *at : NoAt->arestas)
    {
        par<No *, bool> *ParAt = hash_nodes->get(at->id_no_alvo);
        bool add{};
        std::string strAtual = std::string(1, NoAt->id) + " " + std::string(1, ParAt->getKey()->id);

        for (par<std::string,int> p : *listaAdjRet)
        {
            add = true;

            if (p.getKey() == strAtual)
            {
                add = false;
                break;
            }

            // evita duplicatas (a b, e b a)
            if (p.getKey().size() == 3 && strAtual.size() == 3)
                if (strAtual[0] == p.getKey()[2] && strAtual[2] == p.getKey()[0])
                {
                    add = false;
                    break;
                }
        }
        if (add)
            listaAdjRet->push_back(par(strAtual,at->peso));
        
        // se não passou pelo vertice atual
        if (!ParAt->getValue())
        {
            ParAt->setValue(true);
            // PROF(vertice Atual, Hash, lista Strings)
            PROF_teste(ParAt->getKey(), hash_nodes, listaAdjRet);
        }
    }
}

//TODO: FALTA USAR O listaArestas PRA ESCREVER O ARQUIVO DO GRAFO

Grafo *Grafo::arvore_caminhamento_profundidade(char id_no)
{   
    HASH<No*,bool> Hash = HASH<No*,bool>(this->lista_adj);
    Hash.InitHash(this->lista_adj, false);

    // std::vector<std::string> listaArestas;
    std::vector<par<std::string,int>> listaArestas;

    auto *comeco = Hash.get(id_no);
    comeco->setValue(true);

    if (!comeco->getKey()->arestas.empty())
    {
        char idInic = comeco->getKey()->arestas[0]->id_no_alvo;
        char idNoIn = comeco->getKey()->id;

        std::string strIn = std::string(1, idNoIn) + " " + std::string(1, idInic);
        
        listaArestas.push_back(par(strIn,comeco->getKey()->arestas[0]->peso));
    }

    PROF_teste(comeco->getKey(), &Hash, &listaArestas);

    std::cout << "VETOR FINAL: \n";
        for (auto s : listaArestas)
            std::cout << s.getKey() << " "<< s.getValue() << " \n";

        return nullptr;
};


int Grafo::raio() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

int Grafo::diametro() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

vector<char> Grafo::centro() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::periferia() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::vertices_de_articulacao() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}
