#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// Mendefinisikan konstanta NITERS dengan nilai 10.000.000
#define NITERS 10000000

// Deklarasi prototipe fungsi count
void *count(void *arg);

// Variabel global cnt (penghitung) yang akan diakses oleh kedua thread
unsigned int cnt = 0;

// MUTEX: Variabel global untuk sinkronisasi
// PTHREAD_MUTEX_INITIALIZER untuk inisialisasi mutex secara statis
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    // Deklarasi dua variabel untuk ID thread
    pthread_t tid1, tid2;  // Diperbaiki dari tid1 menjadi tid1

    // Membuat thread pertama
    pthread_create(&tid1, NULL, count, NULL);

    // Membuat thread kedua
    pthread_create(&tid2, NULL, count, NULL);

    // Menunggu thread pertama selesai
    pthread_join(tid1, NULL);

    // Menunggu thread kedua selesai
    pthread_join(tid2, NULL);

    // Memeriksa apakah nilai cnt sesuai (20.000.000)
    if (cnt != (unsigned)NITERS * 2)
        printf("Wrong... cnt = %d\n", cnt);
    else
        printf("OK...\tcnt = %d\n", cnt);

    // Mengakhiri program
    exit(0);
}

// Implementasi fungsi count dengan mutex
void *count(void *arg)
{
    int i;

    // Loop sebanyak NITERS kali
    for (i = 0; i < NITERS; i++)
    {
        // MENGUNCI MUTEX sebelum memasuki critical region
        // Hanya satu thread yang bisa melewati baris ini
        pthread_mutex_lock(&mutex);

        // ---------- CRITICAL REGION ----------
        // Operasi increment pada variabel global cnt
        // Sekarang aman karena hanya satu thread yang bisa mengakses di waktu yang sama
        cnt++;
        // ---------- AKHIR CRITICAL REGION ----------

        // MEMBUKA MUTEX setelah keluar dari critical region
        // Thread lain yang menunggu sekarang bisa masuk
        pthread_mutex_unlock(&mutex);
    }

    // Fungsi count selesai
}
