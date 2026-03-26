#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#define key ((key_t) 7890)
#define key2 ((key_t) 7891)
int main(int argc, char **argv)
{
    int i, nloop=500000, zero=0, value, *ptr;
    int shmid, semid, mode, shm_size, sem_size, status;
    sem_t *mutex;
    shm_size=sizeof(int);
    shmid=shmget(key, shm_size,0644 | IPC_CREAT);
    sem_size=sizeof(sem_t);
    semid=shmget(key2,sem_size,0644 | IPC_CREAT);
    ptr = (int *) shmat(shmid,(void *)0,0);
    (*ptr) = 0;
    mutex = (sem_t *) shmat(semid,(void *)0,0);
    sem_init(mutex,1,1);
    if (fork() == 0) {  // child process
        for (i = 0; i < nloop; i++) {

            sem_wait(mutex);
            value = ++(*ptr); 
            sem_post(mutex);

            printf("child process %d %d : %d\n", getpid(), getppid(), value);
            fflush(NULL);
        }
        exit(0);
    }
    // parent process
    for (i = 0; i < nloop; i++) {
 
        sem_wait(mutex);
        value = ++(*ptr); 
        sem_post(mutex);
        
        printf("parent process %d %d : %d\n", getpid(), getppid(), value);
        fflush(NULL);
    }
    wait(&status);
    shmdt(ptr);
    shmdt(mutex);
    shmctl(shmid,IPC_RMID,(struct shmid_ds *)0);
    shmctl(semid,IPC_RMID,(struct shmid_ds *)0);
    exit(0);
}
