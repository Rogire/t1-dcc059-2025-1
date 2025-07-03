#include "Grafo.h"

//feito
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
//feito
void Grafo::imprimir_grafo() {
    cout << endl << "Grafo "
         << (in_direcionado ? "Direcionado" : "Nao direcionado") << ", "
         << (in_ponderado_vertice ? "Ponderado nos vertices" : "Nao ponderado nos vertices") << ", "
         << (in_ponderado_aresta ? "Ponderado nas arestas" : "Nao ponderado nas arestas")
         << endl;

    for (No* i : lista_adj) {
        cout << i->id;

        if (in_ponderado_vertice)
            cout << "(" << i->peso << ")";

        cout << " -> ";

        bool primeira_aresta = true;
        for (Aresta* j : i->arestas) {
            if (!primeira_aresta)
                cout << ", ";
            else
                primeira_aresta = false;

            cout << j->id_no_alvo;

            if (j->retorno) {
                cout << "(RETORNO)";
            } else if (in_ponderado_aresta) {
                cout << "(" << j->peso << ")";
            }
        }

        cout << endl;
    }

    cout << endl;
}

//feito
Grafo::Grafo(){
    ordem=0;
    in_direcionado=0;
    in_ponderado_aresta=0;
    in_ponderado_vertice=0;
}

//feito
Grafo::~Grafo() {
    // Libera memória dos nós e arestas (recomendado implementar isso)
    for (No* no : lista_adj) {
        for (Aresta* aresta : no->arestas) {
            delete aresta;
        }
        delete no;
    }
}

//ajeitar
void Grafo::montar_Grafo_por_arquivo(const string& nome_arquivo) {
    ifstream arquivo(nome_arquivo); // Abre o arquivo

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        exit(-1);
    }

    // Lê as informações iniciais
    arquivo >> in_direcionado >> in_ponderado_vertice >> in_ponderado_aresta >> ordem;

    // Limpa qualquer caractere sobrando antes de começar a ler linhas
    arquivo.ignore(numeric_limits<streamsize>::max(), '\n');

    // Leitura robusta dos nós
    for (int i = 0; i < ordem; i++) {
        No* x;
        string linha;
        getline(arquivo, linha);

        istringstream iss(linha);
        char id;
        iss >> id;

        if (in_ponderado_vertice) {
            int peso = 0;
          
            x = new No(id, peso);
        } else {
            x = new No(id);
        }

        lista_adj.push_back(x);
    }

    // Lê as arestas
    string linha;
    while (getline(arquivo, linha)) {
        istringstream iss(linha);
        char v1, v2;
        iss >> v1 >> v2;

        int idx_v1 = indice_no(v1);
        int idx_v2 = indice_no(v2);

        if (idx_v1 == -1 || idx_v2 == -1) {
            cerr << "Erro: nó não encontrado no grafo: " << v1 << " ou " << v2 << endl;
            continue;
        }

        if (in_ponderado_aresta) {
            int peso;
            if (!(iss >> peso)) {
               
                continue;
            }

            Aresta* final = new Aresta(v2, peso);
            lista_adj[idx_v1]->arestas.push_back(final);

            if (!in_direcionado) {
                Aresta* origem = new Aresta(v1, peso);
                lista_adj[idx_v2]->arestas.push_back(origem);
            }

        } else {
            Aresta* final = new Aresta(v2);
            lista_adj[idx_v1]->arestas.push_back(final);

            if (!in_direcionado) {
                Aresta* origem = new Aresta(v1);
                lista_adj[idx_v2]->arestas.push_back(origem);
            }
        }
    }

    // Ordena as arestas de cada nó pelo id do destino
    for (No* i : lista_adj) {
        std::sort(i->arestas.begin(), i->arestas.end(), [](Aresta* a, Aresta* b) {
            return a->id_no_alvo < b->id_no_alvo;
        });
    }

    // Imprime o grafo
    imprimir_grafo();
}

//feito
vector<char> Grafo::fecho_transitivo_direto(char id_no) {
    
    int idx = indice_no(id_no);
    if (idx == -1) {
        cout << "No nao encontrado" << endl;
        exit(-1);
    }

    queue<No*> fila;
    vector<char> resultado;

    // Reset das marcas
    for (No* no : lista_adj) {
        no->marca = false;
    }

    // Começa com o nó de origem
    fila.push(lista_adj[idx]);
    lista_adj[idx]->marca = true;

    while (!fila.empty()) {
        No* atual = fila.front();
        fila.pop();

        // Adiciona no resultado (se quiser ignorar o próprio nó, pode colocar condicional)
        if (atual->id != id_no)
            resultado.push_back(atual->id);

        // Percorre os vizinhos
        for (Aresta* aresta : atual->arestas) {
            int idx_vizinho = indice_no(aresta->id_no_alvo);
            No* vizinho = lista_adj[idx_vizinho];

            if (!vizinho->marca) {
                vizinho->marca = true;
                fila.push(vizinho);
            }
        }
    }

    std::sort(resultado.begin(), resultado.end());

    return resultado;
}

//feito
vector<char> Grafo::fecho_transitivo_indireto(char id_no) {
    // Obtém o índice do nó na lista de adjacência
    int idx = indice_no(id_no);

    // Se o nó não existe, exibe erro e encerra
    if (idx == -1) {
        cout << "No nao encontrado" << endl;
        exit(-1);
    }

    // Vetor para armazenar o fecho transitivo indireto
    vector<char> resultado;

    // Se o grafo não é direcionado, o fecho indireto é igual ao direto
    if (!in_direcionado) {
        resultado = fecho_transitivo_direto(id_no);
    } 
    else {
        // Caso seja direcionado, executa busca inversa

        // Para cada nó do grafo, verifica se existe caminho até o nó 'id_no'
        for (No* no : lista_adj) {
            // Ignora o próprio nó, pois estamos buscando quem chega até ele
            if (no->id == id_no) continue;

            // Resetar as marcas antes de cada busca
            for (No* temp : lista_adj) {
                temp->marca = false;
            }

            // Cria uma fila para busca em largura (BFS)
            queue<No*> fila;

            // Marca o nó inicial e adiciona na fila
            fila.push(no);
            no->marca = true;

            // Flag para verificar se encontrou caminho até 'id_no'
            bool encontrou = false;

            // Executa a BFS
            while (!fila.empty()) {
                No* atual = fila.front();
                fila.pop();

                // Se encontrou o nó de interesse, marca e para a busca
                if (atual->id == id_no) {
                    encontrou = true;
                    break;
                }

                // Percorre os vizinhos (arestas de saída do nó atual)
                for (Aresta* aresta : atual->arestas) {
                    // Obtém o índice do nó vizinho
                    int idx_vizinho = indice_no(aresta->id_no_alvo);
                    No* vizinho = lista_adj[idx_vizinho];

                    // Se o vizinho ainda não foi visitado
                    if (!vizinho->marca) {
                        vizinho->marca = true;
                        fila.push(vizinho);
                    }
                }
            }

            // Se encontrou caminho até 'id_no', adiciona este nó ao resultado
            if (encontrou) {
                resultado.push_back(no->id);
            }
        }
    }

    // Ordena o vetor resultado em ordem alfabética
    std::sort(resultado.begin(), resultado.end());

    // Retorna o vetor com o fecho transitivo indireto
    return resultado;
}

vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

//feito
Grafo* Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {

    if (in_direcionado) {
        cout << "ERRO: AGM nao e definida para grafos direcionados." << endl;
        return nullptr;
    }
    
    int n_grafo = (int)lista_adj.size(); // Número total de nós no grafo original

    // Cria um vetor booleano para marcar quais nós pertencem ao subconjunto dado
    vector<bool> no_subconjunto(n_grafo, false);

    // Marca como true os índices dos nós que estão no subconjunto (ids_nos)
    for (char id : ids_nos) {
        int idx = indice_no(id);  // Obtém o índice do nó a partir do seu id
        no_subconjunto[idx] = true;
    }

    // Vetor para armazenar as arestas que farão parte da AGM
    // Cada elemento guarda um par de nós e o peso da aresta entre eles
    vector<pair<pair<No*, No*>, int>> F;

    No* v1 = nullptr;
    No* v2 = nullptr;
    int aresta_minima_peso = INT_MAX;

    // Busca a aresta de menor peso dentro do subgrafo induzido pelos nós do subconjunto
    for (int idx_u = 0; idx_u < n_grafo; idx_u++) {
        if (!no_subconjunto[idx_u]) continue;  // Ignora nós fora do subconjunto

        No* u = lista_adj[idx_u];  // Nó atual

        // Percorre todas as arestas do nó u
        for (Aresta* a : u->arestas) {
            int idx_v = indice_no(a->id_no_alvo);  // Índice do nó alvo da aresta
            if (idx_v == -1 || !no_subconjunto[idx_v]) continue;  // Ignora arestas para nós fora do subconjunto

            int peso = a->peso;
            No* v = lista_adj[idx_v];

            // Atualiza a menor aresta encontrada
            if (peso < aresta_minima_peso) {
                aresta_minima_peso = peso;
                v1 = u;
                v2 = v;
            }
        }
    }


    // Caso não encontre nenhuma aresta no subconjunto, indica que o subgrafo está desconexo
    if (!v1 || !v2) {
        cout << "ERRO: Não foi possível encontrar uma aresta no subconjunto. Subgrafo desconexo." << endl;
        return nullptr;
    }

    // Inicializa as marcas (flags) dos nós no subconjunto para false
    for (int i = 0; i < n_grafo; i++) {
        if (no_subconjunto[i])
            lista_adj[i]->marca = false;
    }
    // Marca os dois nós da aresta mínima inicial como pertencentes à AGM
    v1->marca = true;
    v2->marca = true;

    // Adiciona a primeira aresta da AGM, que é a de menor peso encontrada
    F.push_back({{v1, v2}, aresta_minima_peso});

    int contador = 0;
    int n_sub = (int)ids_nos.size();  // Número de nós no subconjunto

    // Loop para adicionar as outras arestas da AGM até conectar todos os nós do subconjunto
    // O laço roda n_sub-2 vezes porque já temos uma aresta conectando 2 nós
    while (contador < n_sub - 2) {
        int menor_peso = INT_MAX;
        No* no_u = nullptr;
        No* no_v = nullptr;

        // Para cada nó u marcado na AGM, procura a menor aresta que conecta a um nó v ainda não marcado
        for (int idx_u = 0; idx_u < n_grafo; idx_u++) {
            if (!no_subconjunto[idx_u]) continue;

            No* u = lista_adj[idx_u];
            if (u->marca) {  // Nó já está na AGM
                for (Aresta* a : u->arestas) {
                    int idx_v = indice_no(a->id_no_alvo);
                    if (idx_v == -1 || !no_subconjunto[idx_v]) continue;

                    No* v = lista_adj[idx_v];
                    // Verifica se v ainda não está na AGM e se a aresta tem peso menor que o menor atual
                    if (!v->marca && a->peso < menor_peso) {
                        menor_peso = a->peso;
                        no_u = u;
                        no_v = v;
                    }
                }
            }
        }

        // Se não encontrar nenhuma nova aresta, significa que o subconjunto é desconexo
        if (!no_v) {
            cout << "ERRO: Não é possível criar a AGM. O subconjunto de nós forma um grafo desconexo." << endl;
            // Limpa as marcas antes de sair
            for (int i = 0; i < n_grafo; i++) {
                if (no_subconjunto[i])
                    lista_adj[i]->marca = false;
            }
            return nullptr;
        }

        // Marca o novo nó como pertencente à AGM e adiciona a aresta encontrada
        no_v->marca = true;
        F.push_back({{no_u, no_v}, menor_peso});
        contador++;
    }

    // Cria o novo grafo que será a AGM
    Grafo* agm = new Grafo();
    agm->ordem = n_sub;                  // Número de nós do subconjunto
    agm->in_direcionado = this->in_direcionado;         // AGM é um grafo não direcionado
    agm->in_ponderado_aresta = this->in_ponderado_aresta;     // As arestas possuem pesos
    agm->in_ponderado_vertice = this->in_ponderado_vertice; // Mantém ponderação dos vértices igual ao grafo original

    // Copia os nós do subconjunto para a AGM
    for (int i = 0; i < n_grafo; i++) {
        if (no_subconjunto[i]) {
            No* no = lista_adj[i];
            No* novo_no = new No(no->id, no->peso);
            agm->lista_adj.push_back(novo_no);
        }
    }

    // Adiciona as arestas encontradas para a AGM, conectando os nós correspondentes
    for (auto& aresta : F) {
        int idx_u = agm->indice_no(aresta.first.first->id);
        int idx_v = agm->indice_no(aresta.first.second->id);
        int peso = aresta.second;

        // Adiciona aresta 
        Aresta* a1 = new Aresta(agm->lista_adj[idx_v]->id, peso);
        agm->lista_adj[idx_u]->arestas.push_back(a1);

        Aresta* a2 = new Aresta(agm->lista_adj[idx_u]->id, peso);
        agm->lista_adj[idx_v]->arestas.push_back(a2);
    }

    // Limpa as marcas no grafo original para não afetar futuras operações
    for (int i = 0; i < n_grafo; i++) {
        if (no_subconjunto[i])
            lista_adj[i]->marca = false;
    }

    
    int custoTotal = 0;
    for (auto& aresta : F) {
        custoTotal += aresta.second;
    }
    cout <<endl<< "Custo total: " << custoTotal;

    return agm; // Retorna o grafo que representa a AGM
}


Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
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