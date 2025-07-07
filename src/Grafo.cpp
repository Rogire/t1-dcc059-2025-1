#include "Grafo.h"
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <queue>
#include <algorithm>
#include <iostream>

using namespace std;

// Função que retorna o índice do nó com identificador 'id' na lista de adjacência
int Grafo::indice_no(char id) {
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

//Funciona
Grafo::Grafo(){
    ordem=0;
    in_direcionado=0;
    in_ponderado_aresta=0;
    in_ponderado_vertice=0;
}

//Funciona
Grafo::~Grafo() {
    // Libera memória dos nós e arestas (recomendado implementar isso)
    for (No* no : lista_adj) {
        for (Aresta* aresta : no->arestas) {
            delete aresta;
        }
        delete no;
    }
}

//Funciona
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


//Funciona
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

//Funciona
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
    // Função auxiliar para encontrar um No* pelo id
    auto encontra_no = [&](char id) -> No* {
        for (No* no : lista_adj) {
            if (no->id == id)
                return no;
        }
        return nullptr;
    };

    No* origem = encontra_no(id_no_a);
    No* destino = encontra_no(id_no_b);

    if (!origem || !destino) {
        cerr << "Erro: um dos nós especificados não existe no grafo.\n";
        return {};
    }

    // Verificação de pesos negativos
    for (No* no : lista_adj) {
        for (Aresta* aresta : no->arestas) {
            if (aresta->peso < 0) {
                cerr << "Erro: o algoritmo de Dijkstra não suporta arestas com peso negativo.\n";
                return {};
            }
        }
    }

    unordered_map<char, float> distancias;
    unordered_map<char, char> predecessores;
    unordered_set<char> visitados;

    auto comp = [](const pair<char, float>& a, const pair<char, float>& b) {
        return a.second > b.second;
    };

    priority_queue<pair<char, float>, vector<pair<char, float>>, decltype(comp)> fila_prioridade(comp);

    for (No* no : lista_adj) {
        distancias[no->id] = numeric_limits<float>::infinity();
    }

    distancias[id_no_a] = 0.0f;
    fila_prioridade.push({id_no_a, 0.0f});

    while (!fila_prioridade.empty()) {
        char atual_id = fila_prioridade.top().first;
        fila_prioridade.pop();

        if (visitados.count(atual_id)) continue;
        visitados.insert(atual_id);

        if (atual_id == id_no_b) break;

        No* atual = encontra_no(atual_id);
        for (Aresta* aresta : atual->arestas) {
            char vizinho_id = aresta->id_no_alvo;
            float nova_dist = distancias[atual_id] + aresta->peso;

            if (nova_dist < distancias[vizinho_id]) {
                distancias[vizinho_id] = nova_dist;
                predecessores[vizinho_id] = atual_id;
                fila_prioridade.push({vizinho_id, nova_dist});
            }
        }
    }

    // Reconstruir caminho
    vector<char> caminho;

    if (id_no_a == id_no_b) {
        caminho.push_back(id_no_a);
        return caminho;
    }

    if (predecessores.find(id_no_b) == predecessores.end()) {
        return {}; // Sem caminho possível
    }

    for (char at = id_no_b; at != id_no_a; at = predecessores[at]) {
        caminho.push_back(at);
    }
    caminho.push_back(id_no_a);
    reverse(caminho.begin(), caminho.end());

    return caminho;
}

vector<char> Grafo::caminho_minimo_floyd(char id_no_a, char id_no_b)
{
    // Verifica se o grafo é direcionado
    if (in_direcionado)
    {
        cout << "ERRO: O algoritmo de Floyd-Warshall não é aplicável a grafos direcionados." << endl;
        return {};
    }
    // Verifica se o grafo é vazio
    if (lista_adj.empty())
    {
        cout << "ERRO: O grafo está vazio." << endl;
        return {};
    }
    // Verifica se os nós existem no grafo
    int idx_no_a = indice_no(id_no_a);
    int idx_no_b = indice_no(id_no_b);
    if (idx_no_a == -1 || idx_no_b == -1)
    {
        cout << "ERRO: Um ou ambos os nós não existem no grafo." << endl;
        return {};
    }

    int ordem = (int)lista_adj.size();
    vector<vector<int>> dist(ordem, vector<int>(ordem, INT_MAX));
    vector<vector<int>> pai(ordem, vector<int>(ordem, -1));

    // Inicializa distâncias e predecessores
    for (int i = 0; i < ordem; i++)
    {
        dist[i][i] = 0;
        for (Aresta *aresta : lista_adj[i]->arestas)
        {
            int idx_vizinho = indice_no(aresta->id_no_alvo);
            if (idx_vizinho != -1)
            {
                dist[i][idx_vizinho] = aresta->peso;
                pai[i][idx_vizinho] = i;
            }
        }
    }

    // Floyd-Warshall
    for (int k = 0; k < ordem; k++)
    {
        for (int i = 0; i < ordem; i++)
        {
            for (int j = 0; j < ordem; j++)
            {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    pai[i][j] = pai[k][j];
                }
            }
        }
    }

    // Se não há caminho
    if (dist[idx_no_a][idx_no_b] == INT_MAX)
    {
        cout << "Não há caminho entre os nós " << id_no_a << " e " << id_no_b << "." << endl;
        return {};
    }

    // Reconstrução do caminho mínimo
    vector<char> caminho;
    int atual = idx_no_b;
    while (atual != idx_no_a)
    {
        if (atual == -1)
        {
            cout << "ERRO: Caminho mínimo não encontrado entre os nós " << id_no_a << " e " << id_no_b << "." << endl;
            return {};
        }
        caminho.push_back(lista_adj[atual]->id);
        atual = pai[idx_no_a][atual];
    }
    caminho.push_back(lista_adj[idx_no_a]->id);
    std::reverse(caminho.begin(), caminho.end());

    // Exibe o caminho mínimo encontrado
    cout << "Caminho minimo de " << id_no_a << " para " << id_no_b << ": ";
    for (char id : caminho)
    {
        cout << id << " ";
    }
    cout << endl;

    return caminho;
}

vector<char> Grafo::caminho_minimo_floyd_sem_print(char id_no_a, char id_no_b)
{
    // Verifica se o grafo é direcionado
    if (in_direcionado)
    {
        cout << "ERRO: O algoritmo de Floyd-Warshall não é aplicável a grafos direcionados." << endl;
        return {};
    }
    // Verifica se o grafo é vazio
    if (lista_adj.empty())
    {
        cout << "ERRO: O grafo está vazio." << endl;
        return {};
    }
    // Verifica se os nós existem no grafo
    int idx_no_a = indice_no(id_no_a);
    int idx_no_b = indice_no(id_no_b);
    if (idx_no_a == -1 || idx_no_b == -1)
    {
        cout << "ERRO: Um ou ambos os nós não existem no grafo." << endl;
        return {};
    }

    int ordem = (int)lista_adj.size();
    vector<vector<int>> dist(ordem, vector<int>(ordem, INT_MAX));
    vector<vector<int>> pai(ordem, vector<int>(ordem, -1));

    // Inicializa distâncias e predecessores
    for (int i = 0; i < ordem; i++)
    {
        dist[i][i] = 0;
        for (Aresta *aresta : lista_adj[i]->arestas)
        {
            int idx_vizinho = indice_no(aresta->id_no_alvo);
            if (idx_vizinho != -1)
            {
                dist[i][idx_vizinho] = aresta->peso;
                pai[i][idx_vizinho] = i;
            }
        }
    }

    // Floyd-Warshall
    for (int k = 0; k < ordem; k++)
    {
        for (int i = 0; i < ordem; i++)
        {
            for (int j = 0; j < ordem; j++)
            {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    pai[i][j] = pai[k][j];
                }
            }
        }
    }

    // Se não há caminho
    if (dist[idx_no_a][idx_no_b] == INT_MAX)
    {
        cout << "Não há caminho entre os nós " << id_no_a << " e " << id_no_b << "." << endl;
        return {};
    }

    // Reconstrução do caminho mínimo
    vector<char> caminho;
    int atual = idx_no_b;
    while (atual != idx_no_a)
    {
        if (atual == -1)
        {
            cout << "ERRO: Caminho mínimo não encontrado entre os nós " << id_no_a << " e " << id_no_b << "." << endl;
            return {};
        }
        caminho.push_back(lista_adj[atual]->id);
        atual = pai[idx_no_a][atual];
    }
    caminho.push_back(lista_adj[idx_no_a]->id);
    std::reverse(caminho.begin(), caminho.end());

    return caminho;
}

//Funciona
Grafo* Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {

    if (in_direcionado) {
        cout << "ERRO: AGM de Prmm nao e definida para grafos direcionados." << endl;
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

//Funciona
Grafo* Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {

    if (in_direcionado) {
        cout << "ERRO: AGM de Kurskal nao e definida para grafos direcionados." << endl;
        return nullptr;
    }

    // Cria uma lista que armazenará as arestas do subconjunto
    // Cada aresta é representada como um par: (peso, (nó_u, nó_v))
    vector<pair<int, pair<char, char>>> L;

    // Percorre todos os nós do grafo original
    for (No* no : lista_adj) {
        // Verifica se o nó atual pertence ao subconjunto de interesse
        bool no_presente = false;
        for (char id : ids_nos) {
            if (no->id == id) {
                no_presente = true;
                break;
            }
        }
        // Se o nó não pertence ao subconjunto, ignora e vai para o próximo
        if (!no_presente) continue;

        // Para o nó pertencente ao subconjunto, percorre suas arestas
        for (Aresta* aresta : no->arestas) {
            // Verifica se o nó destino da aresta também está no subconjunto
            bool destino_presente = false;
            for (char id : ids_nos) {
                if (aresta->id_no_alvo == id) {
                    destino_presente = true;
                    break;
                }
            }
            // Se o destino não está no subconjunto, pula essa aresta
            if (!destino_presente) continue;

            // Extrai informações da aresta: nós e peso
            char u = no->id;
            char v = aresta->id_no_alvo;
            int peso = aresta->peso;

            // Para grafos não direcionados, adiciona cada aresta só uma vez
            if (in_direcionado || u < v) {
                L.push_back({peso, {u, v}});
            }
        }
    }

    // Ordena as arestas por peso crescente para aplicar o Kruskal corretamente
    sort(L.begin(), L.end());

    // Inicializa estrutura Union-Find para controle dos conjuntos disjuntos
    // Essa estrutura ajuda a detectar ciclos ao adicionar arestas
    int n_sub = ids_nos.size();           // número de nós no subconjunto
    vector<int> parent(n_sub);             // vetor para representar pai de cada conjunto
    for (int i = 0; i < n_sub; i++) {
        parent[i] = i;                     // inicialmente cada nó é seu próprio pai (conjunto único)
    }

    // Função 'find' que retorna a raiz do conjunto do elemento x
    auto find = [&](int x) {
        while (parent[x] != x) {
            x = parent[x];  // sobe na árvore até a raiz
        }
        return x;
    };

    // Função 'union_set' que une dois conjuntos diferentes em um só
    auto union_set = [&](int x, int y) {
        int raiz_x = find(x);
        int raiz_y = find(y);
        parent[raiz_x] = raiz_y;          // faz o pai da raiz_x ser raiz_y, unindo os conjuntos
    };

    // Cria um novo grafo vazio para armazenar a AGM resultante
    Grafo* agm = new Grafo();
    agm->ordem = n_sub;                      // número de nós na AGM igual ao do subconjunto
    agm->in_direcionado = this->in_direcionado;             // AGM é grafo não direcionado
    agm->in_ponderado_aresta = this->in_ponderado_aresta;         // arestas da AGM têm peso
    agm->in_ponderado_vertice = this->in_ponderado_vertice; // herda ponderação dos vértices do grafo original

    // Cria os nós no grafo AGM copiando os nós do grafo original que estão no subconjunto
    for (char id : ids_nos) {
        int idx = indice_no(id);             // localiza índice do nó no grafo original
        No* no_original = lista_adj[idx];    // acessa o nó original
        No* novo_no = new No(no_original->id, no_original->peso); // cria cópia do nó
        agm->lista_adj.push_back(novo_no);   // adiciona o nó no grafo AGM
    }

    // Variáveis para controle do número de arestas adicionadas e soma dos pesos
    int contador = 0;    // contador de arestas da AGM
    int custoTotal = 0;  // custo total da AGM

    // Percorre as arestas ordenadas para construir a AGM sem formar ciclos
    for (auto& aresta : L) {
        char u = aresta.second.first;   // nó origem da aresta
        char v = aresta.second.second;  // nó destino da aresta
        int peso = aresta.first;         // peso da aresta

        // Busca os índices dos nós u e v dentro do vetor ids_nos para usar no Union-Find
        int idx_u = -1;
        int idx_v = -1;
        for (int i = 0; i < n_sub; i++) {
            if (ids_nos[i] == u) 
               idx_u = i;
            if (ids_nos[i] == v) 
               idx_v = i;
        }

        // Se algum índice não for encontrado (erro), ignora essa aresta
        if (idx_u == -1 || idx_v == -1) continue;

        // Se u e v estão em conjuntos diferentes (não formam ciclo)
        if (find(idx_u) != find(idx_v)) {
            union_set(idx_u, idx_v); // une os conjuntos
            contador++;              // incrementa número de arestas da AGM

            // Adiciona a aresta no grafo AGM em ambos os sentidos (pois é não direcionado)
            int agm_u = agm->indice_no(u);
            int agm_v = agm->indice_no(v);

            // Cria e adiciona aresta de u para v
            Aresta* a1 = new Aresta(agm->lista_adj[agm_v]->id, peso);
            agm->lista_adj[agm_u]->arestas.push_back(a1);

            // Cria e adiciona aresta de v para u
            Aresta* a2 = new Aresta(agm->lista_adj[agm_u]->id, peso);
            agm->lista_adj[agm_v]->arestas.push_back(a2);

            custoTotal += peso; // acumula o peso da aresta no custo total
        }

        // Se já adicionou n_sub - 1 arestas, a AGM está completa e o algoritmo termina
        if (contador == n_sub - 1) break;
    }

    // Caso não tenha conseguido formar uma AGM completa, o grafo é desconexo
    if (contador != n_sub - 1) {
        cout << "ERRO: O subgrafo é desconexo. Não foi possível formar a AGM." << endl;
        delete agm;       // libera a memória alocada para a AGM
        return nullptr;   // indica falha retornando ponteiro nulo
    }

    
    cout << endl<<"Custo total: " << custoTotal;
    
    // Retorna o ponteiro para a AGM construída
    return agm;
}

// Adiciona as arestas lista no formato que elas vão ser escritas no grafo "x y"
// A adição das arestas é feita fora da verificação se já passou ou não, pra evitar
// repetições de arestas invertidas ex: "a b" e "b a", ele percorre e compara se
// o inverso da aresta atual já está lista, se não está, adiciona
void Grafo::PROF(No *NoAt, std::vector<par<std::string, int>> *listaAdjRet)
{
    for (Aresta *at : NoAt->arestas)
    {
        par<No *, bool> *ParAt = this->Hash_n->get(at->id_no_alvo);
        if (!ParAt)
        {
            std::cerr << "Erro: nó alvo " << at->id_no_alvo << " não encontrado na hash!" << std::endl;
            continue;
        }

        bool add{};
        std::string strAtual = std::string(1, NoAt->id) + " " + std::string(1, ParAt->getKey()->id);

        if (this->in_direcionado == 0)
        {
            for (par<std::string, int> p : *listaAdjRet)
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
                listaAdjRet->push_back(par(strAtual, at->peso));

            // se não passou pelo vertice atual
            if (!ParAt->getValue())
            {
                ParAt->setValue(true);
                // PROF(vertice Atual, lista Strings)
                PROF(ParAt->getKey(), listaAdjRet);
            }
        }
        else
        {
            for (par<std::string, int> p : *listaAdjRet)
            {
                add = true;

                if (p.getKey() == strAtual)
                {
                    add = false;
                    break;
                }
            }

            if (add)
                listaAdjRet->push_back(par(strAtual, at->peso));

            if (!ParAt->getValue())
            {
                ParAt->setValue(true);
                PROF(ParAt->getKey(), listaAdjRet);
            }
        }
    }
}

Grafo *Grafo::arvore_caminhamento_profundidade(char id_no)
{
    this->Hash_n = new HASH<No *, bool>(this->lista_adj);
    this->Hash_n->InitHash(this->lista_adj, false);

    std::vector<par<std::string, int>> listaArestas;

    auto *comeco = this->Hash_n->get(id_no);
    if (!comeco)
    {
        std::cerr << "Erro: nó inicial " << id_no << " não encontrado na hash!" << std::endl;
        return nullptr;
    }

    comeco->setValue(true);

    if (!comeco->getKey()->arestas.empty())
    {
        char idInic = comeco->getKey()->arestas[0]->id_no_alvo;
        char idNoIn = comeco->getKey()->id;

        std::string strIn = std::string(1, idNoIn) + " " + std::string(1, idInic);
        listaArestas.push_back(par(strIn, comeco->getKey()->arestas[0]->peso));
        PROF(comeco->getKey(), &listaArestas);
    }

    std::ofstream arq = this->grafoParaArquivo(listaArestas, "CaminhamentoProfundidade.txt");

    Grafo *ret = new Grafo();
    ret->montar_Grafo_por_arquivo("CaminhamentoProfundidade.txt");

    // considerando que o exercício foi feito pensando para rodar em ambientes UNIX
    system("rm CaminhamentoProfundidade.txt");

    this->Hash_n->~HASH();
    return ret;
};

std::ofstream Grafo::grafoParaArquivo(std::vector<par<std::string, int>> &listaArestas, std::string nomeArq)
{
    std::ofstream arq(nomeArq);

    if (!arq.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo para escrita!" << std::endl;
        return std::ofstream();
    }

    // cabeçalho do txt
    arq << this->in_direcionado << " " << this->in_ponderado_aresta << " " << this->in_ponderado_vertice << std::endl;
    arq << ordem << std::endl;

    for (No *node : this->lista_adj)
    {
        arq << node->id;

        if (node->peso != 0)
            arq << " " << node->peso;

        arq << "\n";
    }

    // arestas do vértice
    if (this->in_ponderado_aresta)
        for (auto t : listaArestas)
            arq << t.getKey() << " " << t.getValue() << "\n";
    else
        for (auto t : listaArestas)
            arq << t.getKey() << "\n";

    arq.close();

    return arq;
}

std::ofstream Grafo::grafoParaArquivo(const Grafo &grafo, std::string nomeArq)
{
    std::ofstream arq(nomeArq);

    if (!arq.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo para escrita!" << std::endl;
        return std::ofstream();
    }

    // cabeçalho do txt
    arq << grafo.in_direcionado << " " << grafo.in_ponderado_aresta << " " << grafo.in_ponderado_vertice << std::endl;
    arq << ordem << std::endl;

    for (No *node : grafo.lista_adj)
    {
        arq << node->id;

        if (node->peso != 0)
            arq << " " << node->peso;

        arq << "\n";
    }

    // arestas do vértice
    if (grafo.in_ponderado_aresta)
        for (No *node : grafo.lista_adj)
            for (Aresta *t : node->arestas)
                arq << node->id << " " << t->id_no_alvo << " " << t->peso << "\n";
    else
        for (No *node : grafo.lista_adj)
            for (Aresta *t : node->arestas)
                arq << node->id << " " << t->id_no_alvo << "\n";

    arq.close();

    return arq;
}

std::ofstream Grafo::vetorParaArquivo(const std::vector<char> &vetor, std::string nomeArq)
{
    std::ofstream arq(nomeArq);

    if (!arq.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo para escrita!" << std::endl;
        return std::ofstream();
    }

    for(char c : vetor)
    {
        arq << c << ",";
    }

    arq.close();

    return arq;
}
int Grafo::excentricidade(char id_no_a)
{
    vector<char> caminho_minimo;
    int maior{0};

    for (No *n : this->lista_adj)
    {
        if (n->id != id_no_a)
        {
            caminho_minimo = this->caminho_minimo_floyd_sem_print(id_no_a, n->id);

            if (caminho_minimo.size() > maior)
                maior = caminho_minimo.size();
        }
    }
    return maior - 1;
}

std::ofstream Grafo::h_paraArquivo(par<int, int> raio_diametro, par<std::vector<char>*, std::vector<char>*> vetores, std::string nomeArq)
{
    std::ofstream arq(nomeArq);

    if (!arq.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo para escrita!" << std::endl;
        return std::ofstream();
    }
    arq << raio_diametro.getKey() << "\n";
    arq << raio_diametro.getValue() << "\n";

    for (auto c : *(vetores.getKey()))
        arq << c << ",";

    arq << "\n";

    for (auto c : *(vetores.getValue()))
        arq << c << ",";

    arq.close();
    return arq;
}

int Grafo::raio()
{
    int menor{INT_MAX}, atual{};

    for (No *node : this->lista_adj)
    {
        atual = this->excentricidade(node->id);
        if (atual < menor)
            menor = atual;
    }
    return menor;
}

int Grafo::diametro()
{

    int maior{0}, atual{};

    for (No *node : this->lista_adj)
    {
        atual = this->excentricidade(node->id);

        if (atual > maior)
            maior = atual;
    }

    return maior;
}

vector<char> Grafo::centro()
{
    vector<char> ret;
    int r = this->raio();

    for (No *node : this->lista_adj)
        if (this->excentricidade(node->id) == r)
            ret.push_back(node->id);

    return ret;
}

vector<char> Grafo::periferia()
{
    vector<char> ret;
    int d = this->diametro();

    for (No *node : this->lista_adj)
        if (this->excentricidade(node->id) == d)
            ret.push_back(node->id);

    return ret;
}
