#include <stdio.h>
#include "queue.h"
//#define DEBUG


int pertenceFila(queue_t **queue, queue_t *elem) {
    int pertence = 0;
    if (*queue == elem) {
        pertence = 1;
        return pertence;
    }
    queue_t *aux = (*queue)->next;
    while (aux != *queue) {
        if (aux == elem) {
            pertence = 1;
            return pertence;
        }
        aux = aux->next;
    }
    return pertence;
}

void queue_append (queue_t **queue, queue_t *elem) {
    if (elem == NULL) {
        #ifndef DEBUG
            printf("queue_append: elemento não pode ser nulo");
        #endif

        return;
    }
    if (elem->next != NULL || elem->prev != NULL) {
        #ifndef DEBUG
            printf("queue_append: elemento pertence a outra fila");
        #endif

        return;
    }
    if (*queue == NULL) {
        *queue = elem;
        elem->prev = elem;
        elem->next = elem;
    } else {
        (*queue)->prev->next = elem;
        elem->prev = (*queue)->prev;
        (*queue)->prev = elem;
        elem->next = (*queue);
    }
}

queue_t *queue_remove (queue_t **queue, queue_t *elem){
    if (*queue == NULL) {
        #ifndef DEBUG
           printf("queue_remove: a fila não existe.\n");
        #endif
        return elem;
    }
    if (queue_size((queue_t *) *queue) == 0) {
        #ifndef DEBUG
                printf("queue_remove: a fila está vazia.\n");
        #endif
        return elem;
    }
    if (!elem) {
        #ifndef DEBUG
                printf("queue_remove: o elemento não existe.\n");
        #endif
        return elem;
    }
    // verificar se o elemento pertence à fila
    if (!pertenceFila(queue, elem)) {
        #ifndef DEBUG
            printf("queue_remove: o elemento não pertence à fila.\n");
        #endif
        return elem;
    }
    // se a fila tem apenas um elemento
    if((*queue)->next == *queue) {
        *queue = NULL;
        elem->next = NULL;
        elem->prev = NULL;
        return elem;
    }
    // se quer removero primeiro elemento da fila
    if (*queue == elem) {
        *queue = elem->next;
    }

    elem->prev->next = elem->next;
    elem->next->prev = elem->prev;
    elem->next = NULL;
    elem->prev = NULL;

    return elem;
}

int queue_size (queue_t *queue) {
    if (queue != NULL) {
        queue_t *aux = queue->next;
        int i = 1;
        while (aux != queue) {
            aux = aux->next;
            i++;
        }
        return i;
     } else {
        return 0;
    }
}

void queue_print (char *name, queue_t *queue)
{
    printf("%s", name);
    if (queue == NULL) {
        printf("[]\n");
    } else {
        printf("[ ");
        printf(" <%p (%p) %p> ", queue->prev, queue, queue->next);
        queue_t *aux = (queue)->next;
        printf(" ");
        while (aux != queue) {
            printf(" <%p (%p) %p> ", aux->prev, aux, aux->next);
            printf(" ");
            aux = aux->next;
        }
        printf("]\n");
    }

}
