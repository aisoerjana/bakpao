#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>
#define NLOOPS 100000

int main()
{
    int i, child, shmid, key = 1000, *ptrData;
    shmid = shmget(key, sizeof(int), 0644 | IPC_CREAT);
    ptrData = (int *)shmat(shmid, (void *)0, 0);
    *ptrData = 0;

    child = fork();

    if (child == 0)
    {
        for (i = 0; i < NLOOPS; i++)
        {
            (*ptrData)++;
            printf("i=%d child: ptrData's value=%d\n", i, *ptrData);
        }
    }
    else if (child > 0)
    {
        for (i = 0; i < NLOOPS; i++)
        {
            (*ptrData)++;
            printf("i=%d parent: ptrData's value=%d\n", i, *ptrData);
        }
        int status;
        waitpid(child, &status, 0);
    }

    shmdt(ptrData);
    shmctl(shmid, IPC_RMID, (struct shmid_ds *)0);

    return 0;
}
