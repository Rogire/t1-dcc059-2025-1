#include <iostream>
#include "Gerenciador.h"
/*
Alunos:
Igor Correa Trifilio Campos
Gustavo Duarte Fernandes de Jesus
Enzo Araújo Pinheiro
Gabriel Toledo Gonçalves Barreto

Repositório: https://github.com/Rogire/t1-dcc059-2025-1
*/
using namespace std;
int main(int argc, char *argv[])
{

    Grafo* grafo = new Grafo();
    grafo->montar_Grafo_por_arquivo(argv[1]);

    grafo->CDS_guloso();

    //Gerenciador::comandos(grafo);
    return 0;
}
