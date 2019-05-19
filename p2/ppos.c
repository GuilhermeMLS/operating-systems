// habilita compatibilidade POSIX no MacOS X (para ucontext.h)
#define _XOPEN_SOURCE 600
#include <stdio.h>
#include "ppos_data.h"
#include <stdlib.h>

// Implementação das funções da lib ppos.h por Guilherme M. Lopes
#define STACKSIZE 32768
task_t task_main, *current_task;
int task_ids;

int task_create (task_t *task, void (*start_func)(void *), void *arg)
{
    getcontext (&(task->context));
    task->stack = malloc(STACKSIZE);
    if (task->stack) {
        task->context.uc_stack.ss_sp = task->stack ;
        task->context.uc_stack.ss_size = STACKSIZE ;
        task->context.uc_stack.ss_flags = 0 ;
        task->context.uc_link = 0 ;
    } else {
        perror ("Erro na criação da pilha: ") ;
        exit (1) ;
    }
    task->id = task_ids;
    task_ids++;
    makecontext (&(task->context), (void*)(*start_func), 1, arg);

    return task->id;
}

int task_switch (task_t *task)
{
    task_t *t = current_task;
    current_task = task;
    swapcontext(&(t->context), &(current_task->context));

    return task->id;
}

void ppos_init ()
{
    /* desativa o buffer da saida padrao (stdout), usado pela função printf */
    setvbuf (stdout, 0, _IONBF, 0);
    task_ids = 0;
    task_main.id = 0;
    getcontext(&(task_main.context));
    current_task = &task_main;
}

void task_exit (int exit_code)
{
    task_switch(&task_main);
}
