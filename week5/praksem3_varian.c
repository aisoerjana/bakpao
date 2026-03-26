#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

#define key  ((key_t) 789)
#define key2 ((key_t) 7891)

int main(int argc, char **argv)
{
    int i, nloop = 500000, value, *ptr;
    int shmid, semid, status;
    sem_t *mutex;

    // =========================
    // Membuat shared memory untuk integer
    shmid = shmget(key, sizeof(int), 0644 | IPC_CREAT);
    ptr = (int *) shmat(shmid, NULL, 0);
    *ptr = 0;   // Inisialisasi shared variable

    // =========================
    // Membuat shared memory untuk semaphore
    semid = shmget(key2, sizeof(sem_t), 0644 | IPC_CREAT);
    mutex = (sem_t *) shmat(semid, NULL, 0);

    // Inisialisasi semaphore
    // pshared = 1 → antar proses
    // value = 1 → binary semaphore (mutex)
    sem_init(mutex, 1, 1);

    // =========================
    // Membuat child process
    if (fork() == 0) {   // CHILD

        for (i = 0; i < nloop; i++) {

            // ===== CRITICAL SECTION START =====
            sem_wait(mutex);     // Lock

            value = ++(*ptr);    // Increment shared variable

            sem_post(mutex);     // Unlock
            // ===== CRITICAL SECTION END =====

            printf("Child %d value: %d\n", getpid(), value);
        }

        exit(0);
    }

    // =========================
    // PARENT PROCESS
    for (i = 0; i < nloop; i++) {

        // ===== CRITICAL SECTION START =====
        sem_wait(mutex);     // Lock

        value = ++(*ptr);    // Increment shared variable

        sem_post(mutex);     // Unlock
        // ===== CRITICAL SECTION END =====

        printf("Parent %d value: %d\n", getpid(), value);
    }

    wait(&status);   // Tunggu child selesai

    // =========================
    // Cleanup
    shmdt(ptr);
    shmdt(mutex);
    shmctl(shmid, IPC_RMID, NULL);
    shmctl(semid, IPC_RMID, NULL);

    return 0;
}
