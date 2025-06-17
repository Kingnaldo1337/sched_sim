#include <stdio.h>

#include "queue.h"
#include "proc.h"
#include "stats.h"
#include "utils.h"

// Variáveis globais
extern struct queue * ready;    
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
                enqueue(ready, current);
                count_ready_in(current);
                break;

            case BLOCKED:
                enqueue(blocked, current);
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

    // Se a fila de aptos está vazia, não há processo para executar
    if (isempty(ready))
    {
        return NULL;
    }

    // Estratégia LJF: encontrar o processo com maior tempo restante
    struct proc * aux = ready->head;
    selected = aux;

    while (aux != NULL)
    {
        if (aux->remaining_time > selected->remaining_time)
        {
            selected = aux;
        }
        aux = aux->next;
    }

    // Remover o processo selecionado da fila
    dequeue_bypid(ready, selected->pid);

    // Atualizar estatísticas
    count_ready_out(selected);

    // Definir estado do processo para execução
    selected->state = RUNNING;

    return selected;
}