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

    shmid = shmget((key_t) SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }

    data = (SharedData *) shmat(shmid, NULL, 0);
    if (data == (void *) -1) {
        perror("shmat failed");
        exit(1);
    }

    data->total_orders = 0;
    data->record_count = 0;
    data->cooked_count = 0;

    sem_unlink(SEM_MUTEX);
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);

    mutex = sem_open(SEM_MUTEX, O_CREAT, 0644, 1);
    empty = sem_open(SEM_EMPTY, O_CREAT, 0644, MAX_RECORDS);
    full  = sem_open(SEM_FULL,  O_CREAT, 0644, 0);

    if (mutex == SEM_FAILED || empty == SEM_FAILED || full == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }

    printf("=== ORDER PROGRAM STARTED (max records: %d) ===\n", MAX_RECORDS);

    while (1) {
        sleep_time = (rand() % 3) + 1;
        sleep(sleep_time);

        sem_wait(empty);

        sem_wait(mutex);

        data->total_orders++;
        data->record_count++;
        printf("[ORDER]  Total: %d | In Record: %d | Cooked: %d\n",
               data->total_orders, data->record_count, data->cooked_count);
        fflush(stdout);

        sem_post(mutex);
        sem_post(full);
    }

    shmdt(data);
    sem_close(mutex);
    sem_close(empty);
    sem_close(full);
    exit(0);
}
