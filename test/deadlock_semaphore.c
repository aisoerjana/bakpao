#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/* ============================================================
 * deadlock_semaphore.c
 * Konversi mekanisme mutex -> semaphore biner
 * dengan strategi penghindaran deadlock (resource ordering)
 *
 * Kompilasi: gcc deadlock_semaphore.c -o deadlock_semaphore -lpthread
 * ============================================================ */

/* Dua semaphore biner menggantikan pthread_mutex_t lock1 dan lock2 */
sem_t sem1;
sem_t sem2;

/* ------------------------------------------------------------ */
/* Thread 1: mengunci sem1 dulu, lalu sem2
 * (urutan konsisten: sem1 → sem2)                             */
void *resource1(void *arg)
{
    /* Kunci resource 1 terlebih dahulu */
    sem_wait(&sem1);
    printf("Thread 1: locked resource 1\n");

    sleep(1); /* Simulasi penggunaan resource */

    /* Kunci resource 2 setelah resource 1 berhasil dikunci */
    sem_wait(&sem2);
    printf("Thread 1: locked resource 2\n");

    /* Lepas dalam urutan terbalik */
    sem_post(&sem2);
    sem_post(&sem1);

    return NULL;
}

/* ------------------------------------------------------------ */
/* Thread 2: mengunci sem1 dulu, lalu sem2
 * (urutan SAMA dengan thread 1 → deadlock dihindari)          */
void *resource2(void *arg)
{
    /* Kunci resource 1 terlebih dahulu (bukan resource 2) */
    sem_wait(&sem1);
    printf("Thread 2: locked resource 1\n");

    sleep(1); /* Simulasi penggunaan resource */

    /* Kunci resource 2 setelah resource 1 berhasil dikunci */
    sem_wait(&sem2);
    printf("Thread 2: locked resource 2\n");

    /* Lepas dalam urutan terbalik */
    sem_post(&sem2);
    sem_post(&sem1);

    return NULL;
}

/* ------------------------------------------------------------ */
int main(void)
{
    pthread_t t1, t2;

    /* Inisialisasi semaphore biner (nilai awal = 1, seperti mutex) */
    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 1);

    pthread_create(&t1, NULL, resource1, NULL);
    pthread_create(&t2, NULL, resource2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    /* Bersihkan semaphore */
    sem_destroy(&sem1);
    sem_destroy(&sem2);

    return EXIT_SUCCESS;
}
