#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>

struct Data {
    int total_ordered;
    int current_records;
    int total_cooked;
    pid_t pid_order;
};

void sem_wait_op(int semid, int index) {
    struct sembuf sb = {index, -1, 0};
    semop(semid, &sb, 1);
}

void sem_signal_op(int semid, int index) {
    struct sembuf sb = {index, 1, 0};
    semop(semid, &sb, 1);
}

int main() {
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, sizeof(struct Data), 0666 | IPC_CREAT);
    struct Data *data = (struct Data *)shmat(shmid, NULL, 0);
    
    int semid = semget(key, 3, 0666 | IPC_CREAT);
    semctl(semid, 0, SETVAL, 1); // index 0 = mutex
    semctl(semid, 1, SETVAL, 25); // index 1 = empty
    semctl(semid, 2, SETVAL, 0); // index 2 = full
    
    data->pid_order = getpid();
    data->total_ordered = 0;
    data->current_records = 0;
    data->total_cooked = 0;

    srand(time(NULL));

    while (1) {
       	sleep(rand() % 3 + 1);
        
	sem_wait_op(semid, 1); // tunggu slot kosong
        sem_wait_op(semid, 0); // kunci mutex
        
        data->total_ordered++;
        data->current_records++;
        printf("ORDER : Pesanan masuk! Total: %d, di meja: %d, sudah dimasak: %d\n", data->total_ordered, data->current_records, data->total_cooked);
        
        sem_signal_op(semid, 0); // lepas mutex
        sem_signal_op(semid, 2); // beritahu cook ada pesanan baru (full++) 
    }
    return 0;
}




