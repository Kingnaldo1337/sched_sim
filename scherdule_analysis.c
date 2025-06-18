#include <stdio.h>
#include <stdlib.h>

#define NUM_ALGOS 5
#define NUM_RUNS 10
#define NUM_PROCS 10

// Lista de nomes dos algoritmos implementados
const char *algorithms[NUM_ALGOS] = {
    "scheduler_sjf",
    "scheduler_ljf",
    "scheduler_prio_static",
    "scheduler_prio_dynamic",
    "scheduler_prio_dynamic_quantum"
};

// Simular a execução de um algoritmo com um número específico de processos
double run_algorithm(const char *algo, int num_procs) {
    double TME = 0;
    
    for (int i = 0; i < NUM_RUNS; i++) {
        // Simulação da execução (substituir pela chamada real ao escalonador)
        double tme_run = (rand() % 100) / 10.0; // Simula um valor de TME
        TME += tme_run;
    }

    return TME / NUM_RUNS; // Média do Tempo Médio de Espera
}

int main() {
    FILE *fp = fopen("resultados.csv", "w");
    if (fp == NULL) {
        printf("Erro ao criar arquivo!\n");
        return 1;
    }

    // Cabeçalho do CSV
    fprintf(fp, "Algoritmo,Num_Procs,TME\n");

    // Executar todos os algoritmos variando a quantidade de processos
    for (int a = 0; a < NUM_ALGOS; a++) {
        for (int num_procs = NUM_PROCS; num_procs <= 100; num_procs += 10) {
            double TME = run_algorithm(algorithms[a], num_procs);
            fprintf(fp, "%s,%d,%.2f\n", algorithms[a], num_procs, TME);
        }
    }

    fclose(fp);
    printf("Simulação concluída! Resultados salvos em 'resultados.csv'.\n");

    return 0;
}