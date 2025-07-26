#include <iostream>
#include "Gerenciador.h"
#include "DominatingSetSolver.h"
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

    Grafo grafo;
    grafo.montar_Grafo_por_arquivo(argv[1]);
    
    DominatingSetSolver solver(grafo);
    std::set<char> resultado = solver.guloso();

    std::cout << "Conjunto Dominante Conexo (Guloso): ";
    for (char no : resultado) {
        std::cout << no << " ";
    }
    std::cout << "\nTamanho: " << resultado.size() << std::endl;

    return 0;
}
