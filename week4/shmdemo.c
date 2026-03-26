#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024 // 1K byte shared memory

int main(int argc, char *argv[])
{
    key_t key;
    int shmid, mode;
    char *data;

    if (argc > 2)
    {
        fprintf(stderr, "usage: ./shmdemo <data string>\n");
        exit(1);
    }

    // generate key
    key = ftok("shmdemo.c", 'R');

    // connect to segment memory or create segment memory
    shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);

    // attach to the segment to get a pointer to it
    data = (char *)shmat(shmid, (void *)0, 0);

    // read or modify the segment, based on the command line
    if (argc == 2)
    {
        printf("write: \"%s\"\n", argv[1]);
        strncpy(data, argv[1], SHM_SIZE);
    }
    else
        printf("read : \"%s\"\n", data);

    // detach from the segment
    shmdt(data);

    return 0;
}
