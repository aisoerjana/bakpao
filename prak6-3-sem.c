#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NITERS 100

sem_t resource_A;
sem_t resource_B;
sem_t turn_1;  // Semaphore untuk thread 1
sem_t turn_2;  // Semaphore untuk thread 2

void *thread_1(void *arg);
void *thread_2(void *arg);

int main()
{
    pthread_t tid1, tid2;

    sem_init(&resource_A, 0, 1);
    sem_init(&resource_B, 0, 1);
    sem_init(&turn_1, 0, 1);  // Thread 1 jalan dulu
    sem_init(&turn_2, 0, 0);  // Thread 2 menunggu

    pthread_create(&tid1, NULL, thread_1, NULL);
    pthread_create(&tid2, NULL, thread_2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    sem_destroy(&resource_A);
    sem_destroy(&resource_B);
    sem_destroy(&turn_1);
    sem_destroy(&turn_2);

    return 0;
}

void *thread_1(void *arg)
{
    int i;

    for (i = 0; i < NITERS; i++)
    {
        sem_wait(&turn_1);  // Tunggu giliran thread 1

        sem_wait(&resource_A);
        sem_wait(&resource_B);
        
        printf("Thread 1\n");
        
        sem_post(&resource_B);
        sem_post(&resource_A);

        sem_post(&turn_2);  // Beri giliran ke thread 2
    }

    return NULL;
}

void *thread_2(void *arg)
{
    int i;

    for (i = 0; i < NITERS; i++)
    {
        sem_wait(&turn_2);  // Tunggu giliran thread 2

        sem_wait(&resource_A);
        sem_wait(&resource_B);
        
        printf("Thread 2\n");
        
        sem_post(&resource_B);
        sem_post(&resource_A);

        sem_post(&turn_1);  // Beri giliran ke thread 1
    }

    return NULL;
}
