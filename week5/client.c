#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHMSZ 27
char SEM_NAME[] = "mysem";

int main()
{
    char ch;
    int shmid;
    key_t key;
    char *shm, *s;
    sem_t *mutex;

    // name the shared memory segment
    key = 1000;

    // create & initialize semaphore
    mutex = sem_open(SEM_NAME, O_CREAT, 0644, 1);

    // create the shared memory segment with this key
    shmid = shmget(key, SHMSZ, IPC_CREAT | 0666);

    // attach this segment to shared memory
    shm = shmat(shmid, NULL, 0);

    // the below loop could be replaced by binary semaphore
    while (*shm != 'A') {
        sleep(1);
    }

    for (s = shm; *s != '\0'; s++) {
        sem_wait(mutex);
        putchar(*s);
        sem_post(mutex);
    }

    printf("\n");

    // once done signal exiting (this can be replaced by another semaphore)
    *shm = '*';

    sem_close(mutex);
    sem_unlink(SEM_NAME);
    shmctl(shmid, IPC_RMID, 0);
    exit(0);
}
