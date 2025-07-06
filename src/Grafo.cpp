#include "Grafo.h"
#include <queue>

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

Grafo::Grafo(){
    ordem=0;
    in_direcionado=0;
    in_ponderado_aresta=0;
    in_ponderado_vertice=0;
}
// Construtor do grafo que recebe o nome do arquivo com os dados do grafo
void Grafo::montar_Grafo_por_arquivo(const string& nome_arquivo) {
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
   if(!in_direcionado)//verifica se o grafo não é direcionado
   {
    cout<<"O grafo não é direcionado! Para verificar o fecho transitivo direto do grafo, é preciso que ele seja direcionado"<<endl;
   }
   else
   {
    vector<char> resultado = AuxDireto(id_no);//chama função auxiliar, retorna o fecho transitivo direto deste nó
    cout<<"Resultado do Transitivo Direto:"<<endl;
     for(char a :resultado)
        {
            cout<<a<<", ";
        }
    return resultado;
   }
  
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
    if(in_direcionado)
    {
        cout<<"O grafo não pode ser direcionado!"<<endl;
        return nullptr;
    }
    if(!in_ponderado_aresta)
    {
        cout<<"O grafo deve possuir arestas ponderadas!"<<endl;
    }
    //Inicialmente, apenas verifica se o subgrafo é conexo;
    Grafo* subgrafo = new Grafo();//Começo a criar o subgrafo só com os nós presentes no vector ids_nos
    subgrafo->ordem = ids_nos.size();
    subgrafo->in_direcionado = this->in_direcionado;
    subgrafo->in_ponderado_aresta = this->in_ponderado_aresta;
    subgrafo->in_ponderado_vertice = this->in_ponderado_vertice;
    for(char id : ids_nos) 
    { //adiciona os vertices no subgrafo
    int indiceAux = this->indice_no(id);
    if(indiceAux != -1)
     {
        No* novono = new No(lista_adj[indiceAux]->id, lista_adj[indiceAux]->peso);
        subgrafo->lista_adj.push_back(novono);
    }
}
 for(char id : ids_nos) {// adiciona as arestas no subgrafo
        int indiceAux = this->indice_no(id);
        if(indiceAux == -1)// verifica se o indice aux existe no original
        {
            continue;
        }
        for(Aresta* aresta : lista_adj[indiceAux]->arestas) {// Só adiciona se o destino também está em ids_nos
            if(std::find(ids_nos.begin(), ids_nos.end(), aresta->id_no_alvo) != ids_nos.end())
             { // Adiciona a aresta ao nó correspondente no subgrafo
                int indiceSub = subgrafo->indice_no(id);
                if(indiceSub != -1) // verifica se o indice existe no subgrafo
                {
                        subgrafo->lista_adj[indiceSub]->arestas.push_back(new Aresta(aresta->id_no_alvo, aresta->peso));
                }
            }
        }
    }
    //Verifica se é conexo
    bool conexo = subgrafo->EhConexo(ids_nos[0]);
    if(!conexo)// se não é conexo, retorna erro
    {
        cout<<" O subgrafo não é conexo! Não é possivel fazer a AGM"<<endl;
        return nullptr;
    }
    //terminar kruskal em si
    
    return nullptr;
}

Grafo * Grafo::arvore_caminhamento_profundidade(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

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
vector<char> Grafo::AuxDireto(char id_no){
    int atual =-1; // valor numerico do local que o nó está;Usado para testar se ele está ou não na lista de adj
    vector<char> adicionado; // vetor que é retornado no fim, todo nó que é FTD de id_no é incluso aqui
    int tamlista = lista_adj.size();
    
 for(int i=0;i<tamlista&&atual==-1;i++) //procura o vértice na lista de adj;
    {
     if(lista_adj.at(i)->id==id_no)
     {
        atual=i;
     }
    }
    if(atual==-1)
    {
        cout<<"Nó não está incluso na lista de adj"<<endl;
        return {};
    }
     queue<char> fila; // fila dos nós, utilzado para fazer a busca em largura
     fila.push(id_no);
    while(!fila.empty())
    {
     char primeiro = fila.front(); // pega o primeiro da fila(mais velho) pra depois tira-lo( usa pra fazer a comparação) 
     fila.pop();
     
     atual =-1;
     for(int i =0;i<tamlista && atual ==-1;i++) // verifica se primeiro esta na lista de adj, e depois faz com que atual == primeiro
     {
        if(lista_adj.at(i)->id==primeiro)
        {
        atual =i;
        }
     }
     if(atual==-1)
     {
        continue; /// se primeiro nao estiver , pula este while
     }
       int tamanhoVizinhos = lista_adj.at(atual)->arestas.size();
        for(int j = 0; j < tamanhoVizinhos; j++) { // percorre os vizinhos do no atual
            char vizinho = lista_adj.at(atual)->arestas.at(j)->id_no_alvo;//vizinho atual
            // Só adiciona se ainda não estiver em adicionado
            bool ja_adicionado = false;
            for(char elemento : adicionado) {// procura o vizinho na lista de adicionado
                if(vizinho == elemento) { // se ja estiver adicionado, para
                    ja_adicionado = true;
                    break;
                }
            }
            if(!ja_adicionado) {/// se não estiver adicionado, poe na fila e no vetor final(adicionado)
                fila.push(vizinho);
                adicionado.push_back(vizinho);
            }
    }
}
  return adicionado;
}

bool Grafo::EhConexo(char id_no)// usa a função AuxDireto para ver se o grafo é conexo
{
    int tamanho = lista_adj.size() -1;//tamanho = ordem do grafo-1(pra tirar o proprio nó que será comparado)
    vector <char> visitados = AuxDireto(id_no);// retorna oo FTD do nó
    if(visitados.size()!= tamanho)// verifica se o FTD do nó é igual o tamanho, se não for, o grafo não é conexo
    {
    return false;
    }
    return true;
}