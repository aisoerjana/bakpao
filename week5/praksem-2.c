#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define NLOOP 500
int g1 = 0;
sem_t semid1, semid2;

void task1(void *arg)
{
    int i;
    for(i=0; i < NLOOP; i++) {
        g1++;
        printf("task1 count:%d g1:%d\n", i+1, g1);
        if (g1%5 == 0) {
            sem_post(&semid2);
            sem_wait(&semid1);
        }
    }
}

void task2(void *arg)
{
    while(1) {
        sem_wait(&semid2);
        if (g1 <= NLOOP) {
            printf(">>>>>>>>>>>>>>>>>> task 2 g1: %d\n", g1);
            sem_post(&semid1);
        	if (g1 == NLOOP) break;
        }
    }
}

int main()
{
    pthread_t thrd1, thrd2;

    sem_init(&semid1, 0, 0);
    sem_init(&semid2, 0, 0);
    pthread_create(&thrd1, NULL, (void *)task1, NULL);
    pthread_create(&thrd2, NULL, (void *)task2, NULL);
    pthread_join(thrd2, NULL);
    pthread_join(thrd1, NULL);
}
