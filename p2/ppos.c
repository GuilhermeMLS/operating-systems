// habilita compatibilidade POSIX no MacOS X (para ucontext.h)
#define _XOPEN_SOURCE 600
#include <stdio.h>
#include "ppos_data.h"
#include <stdlib.h>

// Implementação das funções da lib ppos.h por Guilherme M. Lopes
#define STACKSIZE 32768
task_t task_main, *current_task;
int task_ids;

// Constantes para mensagens de erro e debug coloridas
#define RED "\x1B[31m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define RESET "\x1B[0m"

// Mensagens de erro ou debugger
void print_message(char *color, char *func, char *message)
{
#ifdef DEBUG
    printf("%s%s: %s%s\n", color, func, RESET, message);
#endif
}

int task_create (
    task_t *task,
    void (*start_func)(void *),
    void *arg
) {

    getcontext (&(task->context));
    task->stack = malloc(STACKSIZE);
    if (task->stack) {
        task->context.uc_stack.ss_sp = task->stack;
        task->context.uc_stack.ss_size = STACKSIZE;
        task->context.uc_stack.ss_flags = 0;
        task->context.uc_link = 0;
    } else {
        print_message(RED, "task_create()", "Erro ao criar a pilha");
        exit (1) ;
    }
    task->id = task_ids;
    task_ids++;
    makecontext (&(task->context), (void*)(*start_func), 1, arg);
    print_message(MAG, "task_create()", "Tarefa %s criada");

    return task->id;
}

int task_switch (task_t *task)
{
    task_t *t = current_task;
    current_task = task;
    print_message(MAG, "task_switch()", "Trocando contexto");
    swapcontext(&(t->context), &(current_task->context));

    return task->id;
}

void ppos_init ()
{
    print_message(MAG, "ppos_init()", "Iniciando sistema operacional");
    /* desativa o buffer da saida padrao (stdout), usado pela função printf */
    setvbuf (stdout, 0, _IONBF, 0);
    task_ids = 1;
    task_main.id = 0;
    getcontext(&(task_main.context));
    current_task = &task_main;
}

void task_exit (int exit_code)
{
    print_message(MAG, "task_exit()", "Saindo da tarefa");
    task_switch(&task_main);
}

int task_id() {
    return current_task->id;
}
