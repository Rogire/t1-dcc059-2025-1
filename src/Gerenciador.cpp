#include "Gerenciador.h"
#include <fstream>

/*
Alunos:
Igor Correa Trifilio Campos
Gustavo Duarte Fernandes de Jesus
Enzo Araújo Pinheiro
Gabriel Toledo Gonçalves Barreto

Repositório: https://github.com/Rogire/t1-dcc059-2025-1
*/

void Gerenciador::comandos(Grafo* grafo) {
    cout<<"Digite uma das opcoes abaixo e pressione enter:"<<endl<<endl;
    cout<<"(a) Fecho transitivo direto de um no;"<<endl;
    cout<<"(b) Fecho transitivo indireto de um no;"<<endl;
    cout<<"(c) Caminho minimo (Djikstra);"<<endl;
    cout<<"(d) Caminho minimo (Floyd);"<<endl;
    cout<<"(e) Arvore Geradora Minima (Algoritmo de Prim);"<<endl;
    cout<<"(f) Arvore Geradora Minima (Algoritmo de Kruskal);"<<endl;
    cout<<"(g) Arvore de caminhamento em profundidade;"<<endl;
    cout<<"(h) Raio, diametro, centro e periferia do grafo;"<<endl;
    cout<<"(0) Sair;"<<endl<<endl;

    char resp;
    cin >> resp;
    switch (resp) {
        case 'a': {

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
            cout << "Fecho transitivo direto de " << id_no << ": ";
            for (char id : fecho_transitivo_direto) {
                cout << id << ", ";
            }
            cout << endl;

            if(pergunta_imprimir_arquivo("fecho_trans_dir.txt")) {
                grafo->vetorParaArquivo(fecho_transitivo_direto, "fecho_trans_dir.txt");
            }
            break;
        }

        case 'b':{

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
            cout << "Fecho transitivo indireto de " << id_no << ": ";
            for (char id : fecho_transitivo_indireto) {
                cout << id << ", ";
            }
            cout << endl;

            if(pergunta_imprimir_arquivo("fecho_trans_indir.txt")) {
                grafo->vetorParaArquivo(fecho_transitivo_indireto, "fecho_trans_indir.txt");
            }


            break;
        }

        case 'c': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1,id_no_2);
            
            cout << "Caminho mínimo dijkstra entre " << id_no_1 << " e "<< id_no_2 <<": ";
            for (char id : caminho_minimo_dijkstra)
            {
                cout << id << ", ";
            }
            cout << endl;

            if(pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                grafo->vetorParaArquivo(caminho_minimo_dijkstra, "caminho_minimo_dijkstra.txt");
            }


            break;
        }

        case 'd': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1,id_no_2);
<<<<<<< HEAD
=======
            cout << "Caminho mínimo Floyd entre " << id_no_1 << " e " << id_no_2 << ": ";
            for (char id : caminho_minimo_floyd)
            {
                cout << id << ", ";
            }
            cout << endl;
>>>>>>> main

            if(pergunta_imprimir_arquivo("caminho_minimo_floyd.txt")) {
                grafo->vetorParaArquivo(caminho_minimo_floyd, "caminho_minimo_floyd.txt");
            }


            break;
        }
        case 'e': {

            int tam;
            cout<<"Digite o tamanho do subconjunto: ";
            cin>>tam;

            if(tam > 0 && tam <= grafo->ordem) {

                vector<char> ids = get_conjunto_ids(grafo,tam);
                Grafo* arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);

                if(arvore_geradora_minima_prim == nullptr)
                  break;
                else
                  arvore_geradora_minima_prim->imprimir_grafo();

                if(pergunta_imprimir_arquivo("agm_prim.txt")) {
                    std::ofstream arquivo = arvore_geradora_minima_prim->grafoParaArquivo(*arvore_geradora_minima_prim, "agm_prim.txt");
                }

                delete arvore_geradora_minima_prim;

            }else {
                cout<<"Valor invalido"<<endl;
            }

            break;
        }

        case 'f': {

            int tam;
            cout<<"Digite o tamanho do subconjunto: ";
            cin>>tam;

            if(tam > 0 && tam <= grafo->ordem) {

                vector<char> ids = get_conjunto_ids(grafo,tam);
                Grafo* arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
                if(arvore_geradora_minima_kruskal == nullptr)
                  break;
                else
                  arvore_geradora_minima_kruskal->imprimir_grafo();

                if(pergunta_imprimir_arquivo("agm_kruskal.txt")) {
                    std::ofstream arquivo = arvore_geradora_minima_kruskal->grafoParaArquivo(*arvore_geradora_minima_kruskal, "agm_kruskal.txt");
                }

                delete arvore_geradora_minima_kruskal;

            }else {
                cout<<"Valor invalido"<<endl;
            }

            break;
        }

        case 'g': {

            char id_no = get_id_entrada();
            Grafo* arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);
            

            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                std::ofstream arquivo = arvore_caminhamento_profundidade->grafoParaArquivo(*arvore_caminhamento_profundidade, "arvore_caminhamento_profundidade.txt");
            }

            delete arvore_caminhamento_profundidade;
            break;
        }

        case 'h': {

            int raio = grafo->raio();
            int diametro = grafo->diametro();
            vector<char> centro = grafo->centro();
            vector<char> periferia = grafo->periferia();

            std::cout << "raio: " << raio << '\n';
            std::cout << "diametro: " << diametro << '\n';

            std::cout << "Centrais:\n";
            for (char c : centro)
                std::cout << c << ",";
            std::cout << "\n";

            std::cout << "Periféricos:\n";
            for (char c : periferia)
                std::cout << c << ",";
            std::cout << "\n";

            if (pergunta_imprimir_arquivo("raio_diametro_centro_periferia.txt"))
            {
                std::ofstream arquivo = grafo->h_paraArquivo(par<int,int>(raio, diametro), par<vector<char>*,vector<char>*>(&centro, &periferia), "raio_diametro_centro_periferia.txt");
            }

            break;
        }
        case '0': {
            exit(0);
        }
        default: {
            cout<<"Opção inválida"<<endl;
        }
    }

    comandos(grafo);

}

char Gerenciador::get_id_entrada() {
    cout<<"Digite o id de um no: ";
    char id;
    cin>>id;
    cout<<endl;
    return id;
}

vector<char> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam) {
    vector<char> ids = {};
    while((int)ids.size() < tam) {
        char id_no =get_id_entrada();
        bool existe = false;
        for(No* no: grafo->lista_adj){
            if(no->id == id_no){
                existe = true;
                break;
            }
        }

        if(!existe){
            cout<<"Vertice nao existe"<<endl;
        }else{
            bool repetido = find(ids.begin(), ids.end(),id_no) != ids.end();
            if(repetido) {
                    cout<<"Valor repetido"<<endl;
            }else {
                ids.push_back(id_no);
            }
        }

    }

    return ids;
}


bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo) {

    cout<<"Imprimir em arquivo externo? ("<<nome_arquivo<<")"<<endl;
    cout<<"(1) Sim;"<<endl;
    cout<<"(2) Nao."<<endl;
    int resp;
    cin>>resp;
    cout<<endl;

    switch (resp) {
        case 1:
            return true;
        case 2:
            return false;
        default:
            cout<<"Resposta invalida"<<endl;
            return pergunta_imprimir_arquivo(nome_arquivo);
    }
}