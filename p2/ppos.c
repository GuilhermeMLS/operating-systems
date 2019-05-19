#include <stdio.h>
#include "ppos_data.h"
#include <stdlib.h>

// Implementação das funções da lib ppos.h por Guilherme M. Lopes
#define STACKSIZE 32768

task_t tmj;
int ids = 0;

int task_create (task_t *task, void (*start_func)(void *), void *arg)
{
    char *stack;
    getcontext (&(task->context));
    stack = malloc (STACKSIZE) ;
    if (stack) {
        task->context.uc_stack.ss_sp = stack ;
        task->context.uc_stack.ss_size = STACKSIZE ;
        task->context.uc_stack.ss_flags = 0 ;
        task->context.uc_link = 0 ;
    } else {
        perror ("Erro na criação da pilha: ") ;
        exit (1) ;
    }
#ifndef DEBUG
    printf("task_create(): pilha criada.\n");
#endif

    makecontext (&(task->context), start_func, 1, arg);

    task->id = ids;
    ids++;

#ifndef DEBUG
    printf("task_create(): contexto criado.\n");
#endif

    return task->id;
}

int task_switch (task_t *task)
{
#ifndef DEBUG
    printf("task_switch(): trocando de contexto...\n");
#endif
    setcontext(&(task->context));
#ifndef DEBUG
    printf("task_switch(): contexto trocado.\n");
#endif

    return 0;
}

void ppos_init ()
{
    /* desativa o buffer da saida padrao (stdout), usado pela função printf */
    setvbuf (stdout, 0, _IONBF, 0);
    #ifndef DEBUG
        printf("ppos_init(): PPOS iniciado.\n");
    #endif
}

void task_exit (int exit_code)
{
    task_switch((task_t *) &tmj.context);
}
