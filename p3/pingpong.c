#define _XOPEN_SOURCE 600
// Trabalho do Gustavo UTFPR

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "../../../Downloads/2018/projeto3/pingpong.h"
#include "../../../Downloads/2018/projeto3/datatypes.h"
#include "../../../Downloads/2018/projeto3/queue.h"

#define STACKSIZE 32768

task_t MAIN;
task_t DISPATCHER;

task_t *CURRENT_TASK;
task_t *USER_TASK_QUEUE;

int count_id = 0;

/*
  Verifica qual a task a ser executada
  Aqui ainda eh escolhida a primeira da fila
*/
task_t *scheduler()
{
#ifdef DEBUG
  printf("Scheduler selecionou Task %d\n", USER_TASK_QUEUE->tid);
#endif

  return USER_TASK_QUEUE;
}

/*
  Faz o controle de trocar para a task escolhida e aplicar as acoes devidas antes da troca
*/
void dispatcher_body()
{
  task_t *next;

  while (USER_TASK_QUEUE)
  {
    next = scheduler();
    if (next)
    {
      // ações antes de lançar a tarefa "next", se houverem
      queue_remove((queue_t **)&USER_TASK_QUEUE, (queue_t *)next);
      // switch
#ifdef DEBUG
  printf("Trocando para Task com ID %d\n", next->tid);
#endif
      task_switch(next);
      // ações após de lançar a tarefa "next", se houverem
    }
  }
  task_exit(0);
}

/*
  Inicializador do SO
  Inicia MAIN e DISPATCHER
*/
void ppos_init()
{
  setvbuf(stdout, 0, _IONBF, 0);

  ucontext_t main_context;
  getcontext(&main_context);
  MAIN.next = NULL;
  MAIN.prev = NULL;
  MAIN.tid = count_id++;
  MAIN.context = main_context;
  CURRENT_TASK = &MAIN;

  task_create(&DISPATCHER, (void *)(dispatcher_body), NULL);

#ifdef DEBUG
  printf("Task MAIN criada com ID %d\n", MAIN.tid);
#endif
}

/*
  Factory para criar tasks
*/
int task_create(task_t *task, void (*start_routine)(void *), void *arg)
{
  ucontext_t context;
  getcontext(&context);

  char *stack;
  stack = (char *)malloc(STACKSIZE);
  if (stack)
  {
    context.uc_stack.ss_sp = stack;
    context.uc_stack.ss_size = STACKSIZE;
    context.uc_stack.ss_flags = 0;
    context.uc_link = 0;
  }
  else
  {
    perror("Erro na pilha");
    return (-1);
  }

  task->prev = NULL;
  task->next = NULL;
  task->tid = count_id++;
  makecontext(&context, (void *)(*start_routine), task->tid, arg);
  task->context = context;

  if (task != &DISPATCHER)
    queue_append((queue_t **)&USER_TASK_QUEUE, (queue_t *)task);

#ifdef DEBUG
  printf("Task de usuário criada com ID %d\n", task->tid);
#endif

  return task->tid;
}

/*
  Troca de contexto entre tasks
*/
int task_switch(task_t *task)
{
#ifdef DEBUG
  printf("Vai trocar de contexto de ID %d para ID %d\n", CURRENT_TASK->tid, task->tid);
#endif

  task_t *last_task = CURRENT_TASK;
  CURRENT_TASK = task;
  swapcontext(&(last_task->context), &(CURRENT_TASK->context));

  return 0;
}

/*
  Finalizacao de task
  Retorna comando para dispatcher se eh tarefa de usuario
  Retorna comando para main se eh dispatcher
*/
void task_exit(int exit_code)
{
#ifdef DEBUG
  printf("Task com ID %d encerrada\n", CURRENT_TASK->tid);
#endif

  if (USER_TASK_QUEUE) {
    queue_remove((queue_t **)&USER_TASK_QUEUE, (queue_t *)CURRENT_TASK);
    task_switch(&DISPATCHER);
  }
  else
    task_switch(&MAIN);
  // if (CURRENT_TASK == &DISPATCHER) {
  //   queue_remove((queue_t **)&USER_TASK_QUEUE, (queue_t *)CURRENT_TASK);
  //   task_switch(&DISPATCHER);
  // }
  // else
  //   task_switch(&DISPATCHER);
}

/*
  Retorna ID da task atual
*/
int task_id()
{
#ifdef DEBUG
  printf("Retornando ID da task com ID %d\n", CURRENT_TASK->tid);
#endif

  return CURRENT_TASK->tid;
}

/*
  Retorna controle para o DISPATCHER
  Retorna a task que estava executando para a fila de prontas
*/
void task_yield()
{
#ifdef DEBUG
  printf("Trocando contexto para o dispatcher, chamado pela task %d\n", CURRENT_TASK->tid);
#endif

  // if (CURRENT_TASK != &MAIN && CURRENT_TASK != &DISPATCHER)
  if (CURRENT_TASK != &MAIN)
    queue_append((queue_t **)&USER_TASK_QUEUE, (queue_t *)CURRENT_TASK);

  task_switch(&DISPATCHER);
}
