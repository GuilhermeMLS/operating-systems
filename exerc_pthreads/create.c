#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 16 //número de threads que serão criadas no loop

void *threadBody (void *id) // função chamada assim que a thread é criada
{
    long tid = (long) id ; //atribui um valor inteiro para ser o id da thread

    printf ("t%02ld: Olá!\n", tid) ;
    sleep (3) ;
    printf ("t%02ld: Tchau!\n", tid) ;

    pthread_exit (NULL) ; // volta para a função main
}

int main (int argc, char *argv[])
{
    pthread_t thread [NUM_THREADS] ; // cria um vetor de 16 threads
    long i, status ;

    for (i=0; i<NUM_THREADS; i++) // itera o vetor criando cada thread
    {
        printf ("Main: criando thread %02ld\n", i) ;
        /*
         *  Cria uma thread para cada índice do vetor
         *  Cada thread chama sua threadBody sequencialmente
         * */
        status = pthread_create (&thread[i], NULL, threadBody, (void *) i) ;

        // Verifica o status da thread recém criada
        if (status)
        {
            perror ("pthread_create") ;
            exit (1) ;
        }
    }
    // não sei porque ele precisa de um p  thread_exit aqui ainda
    pthread_exit (NULL) ;
}