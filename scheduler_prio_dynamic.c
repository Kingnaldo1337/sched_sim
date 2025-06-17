#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "proc.h"
#include "stats.h"
#include "utils.h"

// Variáveis globais
extern struct queue * ready;    
extern struct queue * ready2;   
extern struct queue * blocked;  
extern struct queue * finished; 

extern int MAX_TIME;

struct proc * scheduler(struct proc * current)
{
    struct proc * selected = NULL;

    // Tratando o processo que estava em execução
    if (current != NULL)
    {
        switch (current->state)  
        {
            case READY:
                // O processo mantém a fila original
                if (current->queue == 0)
                    enqueue(ready, current);
                else
                    enqueue(ready2, current);
                
                count_ready_in(current);
                break;

            case BLOCKED:
                // Processos que saem de bloqueado vão para `ready`
                current->queue = 0;
                enqueue(ready, current);
                count_blocked_in(current);
                break;

            case FINISHED:
                enqueue(finished, current);
                count_finished_in(current);
                break;

            default:
                printf("@@ ERRO no estado de saída do processo %d\n", current->pid);
        }
    }

    // Se ambas as filas de aptos estão vazias, não há processo para executar
    if (isempty(ready) && isempty(ready2))
    {
        return NULL;
    }

    // Estratégia PRIO_DYNAMIC: escolher fila com probabilidade 80% (ready) e 20% (ready2)
    double prob = ((double) rand() / RAND_MAX);
    struct queue * target_queue = (prob < 0.8) ? ready : ready2;

    // Se a fila escolhida estiver vazia, usa a outra
    if (isempty(target_queue))
        target_queue = (target_queue == ready) ? ready2 : ready;

    // Selecionar o primeiro processo da fila escolhida (FIFO)
    selected = dequeue(target_queue);

    // Se o processo saiu por preempção, ele deve retornar para `ready2`
    if (selected->state == READY)
        selected->queue = 1;

    // Atualizar estatísticas
    count_ready_out(selected);

    // Definir estado do processo para execução
    selected->state = RUNNING;

    return selected;
}