#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <time.h>

#define SHM_KEY     1234
#define MAX_RECORDS 30

#define SEM_MUTEX "/resto_mutex"
#define SEM_EMPTY "/resto_empty"
#define SEM_FULL  "/resto_full"

typedef struct {
    int total_orders;
    int record_count;
    int cooked_count;
} SharedData;

int main()
{
    int shmid, sleep_time;
    SharedData *data;
    sem_t *mutex, *empty, *full;

    srand(time(NULL));

    shmid = shmget((key_t) SHM_KEY, sizeof(SharedData), 0666);
    if (shmid < 0) {
        perror("shmget failed (run order.c first)");
        exit(1);
    }

    data = (SharedData *) shmat(shmid, NULL, 0);
    if (data == (void *) -1) {
        perror("shmat failed");
        exit(1);
    }

    mutex = sem_open(SEM_MUTEX, 0);
    empty = sem_open(SEM_EMPTY, 0);
    full  = sem_open(SEM_FULL,  0);

    if (mutex == SEM_FAILED || empty == SEM_FAILED || full == SEM_FAILED) {
        perror("sem_open failed (run order.c first)");
        exit(1);
    }

    printf("=== COOK PROGRAM STARTED ===\n");

    while (1) {
        sem_wait(full);

        sem_wait(mutex);

        data->record_count--;
        data->cooked_count++;
        printf("[COOK]   Total: %d | In Record: %d | Cooked: %d\n",
               data->total_orders, data->record_count, data->cooked_count);
        fflush(stdout);

        sem_post(mutex);
        sem_post(empty);

        sleep_time = (rand() % 4) + 1;
        sleep(sleep_time);
    }

    shmdt(data);
    sem_close(mutex);
    sem_close(empty);
    sem_close(full);
    exit(0);
}
