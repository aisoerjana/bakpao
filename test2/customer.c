#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

/* ============================================================
 * customer.c  —  Proses Customer (Sleeping Barber Problem)
 *
 * Kompilasi : gcc customer.c -o customer -lpthread
 * Jalankan  : ./barber <num_chairs> lebih dulu, lalu
 *             ./customer <num_customers> <rand_seed>
 * ============================================================ */

#define MAX_CUSTOMERS 25

/* Nama-nama named semaphore (harus sama persis dengan barber.c) */
#define SEM_WAITING_ROOM  "/sem_waitingRoom"
#define SEM_BARBER_CHAIR  "/sem_barberChair"
#define SEM_BARBER_PILLOW "/sem_barberPillow"
#define SEM_SEAT_BELT     "/sem_seatBelt"
#define SEM_ALL_DONE      "/sem_allDone"

/* Variabel global semaphore — dipakai oleh semua thread customer */
sem_t *waitingRoom;
sem_t *barberChair;
sem_t *barberPillow;
sem_t *seatBelt;
sem_t *allDone;

int numCustomers;

/* ------------------------------------------------------------ */
void randwait(int secs)
{
    int len;
    len = (int) ((drand48() * secs) + 1);
    sleep(len);
}

/* ------------------------------------------------------------ */
/* Fungsi thread untuk setiap customer                          */
void *customer(void *number)
{
    int  num       = *(int *) number;
    int  free_chair;

    /* Jalan-jalan dulu sebelum datang ke barbershop */
    printf("Customer %d meninggalkan barbershop.\n", num);
    randwait(5);

    /* Coba masuk ruang tunggu */
    sem_wait(waitingRoom);
    sem_getvalue(waitingRoom, &free_chair);
    printf("Customer %d memasuki ruang tunggu.\n", num);
    printf("Kursi bebas di ruang tunggu = %d\n", free_chair);

    /* Tunggu kursi barber kosong */
    sem_wait(barberChair);
    printf("Customer %d duduk di kursi barber.\n", num);

    /* Lepaskan slot ruang tunggu */
    sem_post(waitingRoom);
    sem_getvalue(waitingRoom, &free_chair);
    printf("Customer %d melepas kursinya di ruang tunggu.\n", num);
    printf("Kursi bebas di ruang tunggu = %d\n", free_chair);

    /* Bangunkan barber */
    printf("Customer %d membangunkan barber.\n", num);
    sem_post(barberPillow);

    /* Tunggu barber selesai memotong rambut */
    sem_wait(seatBelt);

    /* Lepaskan kursi barber */
    sem_post(barberChair);
    printf("Customer %d meninggalkan barbershop.\n", num);

    return NULL;
}

/* ------------------------------------------------------------ */
int main(int argc, char *argv[])
{
    pthread_t tid[MAX_CUSTOMERS];
    int       Number[MAX_CUSTOMERS];
    long      RandSeed;
    int       i;

    /* Validasi argumen */
    if (argc != 3)
    {
        printf("Penggunaan: ./customer <num_customers> <rand_seed>\n");
        exit(-1);
    }

    numCustomers = atoi(argv[1]);
    RandSeed     = atoi(argv[2]);

    if (numCustomers > MAX_CUSTOMERS)
    {
        printf("Jumlah customer maksimum adalah %d\n", MAX_CUSTOMERS);
        exit(-1);
    }

    /* Buka named semaphore yang sudah dibuat oleh proses barber */
    waitingRoom  = sem_open(SEM_WAITING_ROOM,  0);
    barberChair  = sem_open(SEM_BARBER_CHAIR,  0);
    barberPillow = sem_open(SEM_BARBER_PILLOW, 0);
    seatBelt     = sem_open(SEM_SEAT_BELT,     0);
    allDone      = sem_open(SEM_ALL_DONE,      0);

    if (waitingRoom  == SEM_FAILED || barberChair == SEM_FAILED ||
        barberPillow == SEM_FAILED || seatBelt    == SEM_FAILED ||
        allDone      == SEM_FAILED)
    {
        perror("sem_open gagal — pastikan ./barber sudah dijalankan lebih dulu");
        exit(1);
    }

    printf("=== Proses Customer dimulai (PID: %d) ===\n", getpid());
    printf("Jumlah customer: %d\n\n", numCustomers);

    srand48(RandSeed);

    /* Inisialisasi array nomor customer */
    for (i = 0; i < MAX_CUSTOMERS; i++)
        Number[i] = i;

    /* Buat thread untuk setiap customer */
    for (i = 0; i < numCustomers; i++)
        pthread_create(&tid[i], NULL, customer, (void *) &Number[i]);

    /* Tunggu semua thread customer selesai */
    for (i = 0; i < numCustomers; i++)
        pthread_join(tid[i], NULL);

    /* Beri sinyal ke barber bahwa semua customer sudah selesai */
    sem_post(allDone);
    sem_post(barberPillow); /* bangunkan barber agar dia bisa keluar loop */

    /* Tutup semaphore (tidak di-unlink — barber yang melakukan unlink) */
    sem_close(waitingRoom);
    sem_close(barberChair);
    sem_close(barberPillow);
    sem_close(seatBelt);
    sem_close(allDone);

    printf("\nSemua customer telah dilayani. Proses customer selesai.\n");
    return EXIT_SUCCESS;
}
