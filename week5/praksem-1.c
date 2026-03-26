#include <semaphore.h>
#include <stdio.h>

#define INITVALUE 1
#define MAXCOUNT 10

int main()
{
    sem_t semid;
    int i, sem_value;

    if (sem_init(&semid, 0, INITVALUE) == -1) {
        perror("Failed to initialize semaphore");
        return 1;
    }

    for (i = 0; i < MAXCOUNT; i++) {
        if (sem_getvalue(&semid, &sem_value) == -1)
            perror("Failed to get semaphore value\n");

        printf("Semaphore_value = %d \n", sem_value);

        if (sem_wait(&semid) == -1) // down semaphore, atomically
            perror("Failed to decrement semaphore\n");
    }
}
