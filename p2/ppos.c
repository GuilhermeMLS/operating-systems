#include <stdio.h>
#include "ppos_data.h"
#include <stdlib.h>

// Implementação das funções da lib ppos.h por Guilherme M. Lopes
#define STACKSIZE 32768

int task_create (task_t *task, void (*start_func)(void *), void *arg)
{
    char *stack;
    getcontext (&task->context) ;

    stack = malloc (STACKSIZE) ;
    if (stack)
    {
        task->context.uc_stack.ss_sp = stack ;
        task->context.uc_stack.ss_size = STACKSIZE ;
        task->context.uc_stack.ss_flags = 0 ;
        task->context.uc_link = 0 ;
    }
    else
    {
        perror ("Erro na criação da pilha: ") ;
        exit (1) ;
    }

    makecontext (&task->context, (void*)(*start_func), 1, "    Ping") ;
}

// TODO: implementar task_switch

int main() {
    printf("Hello, World!\n");
    return 0;
}