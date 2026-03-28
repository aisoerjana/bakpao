#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

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
    int semid = semget(key, 3, 0666);

    srand(time(NULL) + 1);

    while (1) {
        sem_wait_op(semid, 2); // tunggu ada pesanan
        sem_wait_op(semid, 0); // kunci mutex
        
        data->current_records--;
        data->total_cooked++;
        printf("COOK: Pesanan selesai! Total order: %d, sisa catatan: %d, total matang: %d\n", data->total_ordered, data->current_records, data->total_cooked);
        
       	sem_signal_op(semid, 0); // lepas mutex	
       	sem_signal_op(semid, 1); // kasih tau order ada slot kosong (empty++)
       	
       	sleep(rand() % 4 + 2);
       	
       	// Cek apakah order.c masih nyala
       	if (kill(data->pid_order, 0) != 0)
       	{
       		printf("COOK: order.c sudah mati, cook ikut berhenti\n");
       		break;
    }
    }
    return 0;
}
