#include <iostream>
#include <iomanip>
#include <chrono>
#include "Gerenciador.h"
#include "DominatingSetSolver.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <arquivo_instancia>" << endl;
        return 1;
    }

    // Carrega o grafo
    Grafo grafo;
    grafo.montar_Grafo_por_arquivo(argv[1]);
    

    DominatingSetSolver solver(grafo);

    // =============================
    // 1. Guloso Puro
    // =============================
    cout << "\n=== Guloso Puro ===\n";
    auto ini1 = high_resolution_clock::now();
    set<char> solucaoGuloso = solver.guloso();
    auto fim1 = high_resolution_clock::now();
    double tempo1 = duration<double>(fim1 - ini1).count();

    cout << "Conjunto Dominante Conexo (Guloso): { ";
    for (char c : solucaoGuloso)
        cout << c << " ";
    cout << "}\nTamanho: " << solucaoGuloso.size()
         << " | Tempo (s): " << tempo1 << endl;

    // =============================
    // 2. Guloso Randomizado (por alpha)
    // =============================
    cout << "\n=== Guloso Randomizado (por alpha) ===\n";
    vector<double> alphas = {0.05, 0.1, 0.15, 0.3, 0.5};
    int numIteracoes = 10;

    for (double alpha : alphas) {
        cout << "\n=== Alpha = " << alpha << " ===" << endl;

        int melhor = INT_MAX;
        double soma = 0;
        double somaTempo = 0;
        set<char> melhorConjunto;

        for (int i = 1; i <= numIteracoes; ++i) {
            auto inicio = high_resolution_clock::now();
            set<char> solucao = solver.gulosoRandomizado(alpha);
            auto fim = high_resolution_clock::now();

            double duracao = duration<double>(fim - inicio).count();
            somaTempo += duracao;

            int tamanho = solucao.size();
            soma += tamanho;

            if (tamanho < melhor) {
                melhor = tamanho;
                melhorConjunto = solucao;
            }

            cout << "Iteração " << i << " | Tamanho: " << tamanho << " | Solução: { ";
            for (char c : solucao)
                cout << c << " ";
            cout << "}" << endl;
        }

        cout << fixed << setprecision(8);
        cout << "Resumo para α = " << alpha
             << ": Melhor: " << melhor
             << " | Média: " << (soma / numIteracoes)
             << " | Tempo médio (s): " << (somaTempo / numIteracoes) << endl;

        cout << "Melhor conjunto solução: { ";
        for (char c : melhorConjunto)
            cout << c << " ";
        cout << "}" << endl;
    }

    // =============================
    // 3. Guloso Randomizado Reativo
    // =============================
    cout << "\n=== Guloso Randomizado Reativo ===\n";
    int iteracoes = 2500;
    int bloco = 50;
    solver.gulosoRandomizadoReativo(iteracoes, alphas, bloco);

    return 0;
}
