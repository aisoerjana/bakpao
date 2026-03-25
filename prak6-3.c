#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NITERS 100000000

pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;

void *thread_1(void *arg);
void *thread_2(void *arg);

int main()
{
    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, thread_1, NULL);
    pthread_create(&tid2, NULL, thread_2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    return 0;
}

void *thread_1(void *arg)
{
    int i;

    for (i = 0; i < NITERS; i++)
    {
        pthread_mutex_lock(&mutex_1);  // Lock mutex 1 dulu
        pthread_mutex_lock(&mutex_2);  // Lock mutex 2 kedua
        printf("Thread 1 \n");
        pthread_mutex_unlock(&mutex_1); // Unlock mutex 1 saja
        //Mutex2 tidak di unlock nihh. Deadlock
    }
}

void *thread_2(void *arg)
{
    int i;

    for (i = 0; i < NITERS; i++)
    {
        pthread_mutex_lock(&mutex_2);  // Lock mutex 2 dulu
        pthread_mutex_lock(&mutex_1);  // Lock mutex 1 kedua
        printf("Thread 2\n");
        pthread_mutex_unlock(&mutex_1); // Unlock mutex 1
        pthread_mutex_unlock(&mutex_2); // Unlock mutex 2
    }
}
