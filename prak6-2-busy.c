#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>  // Untuk menggunakan EBUSY

// Mendefinisikan konstanta NITERS dengan nilai 10.000.000
#define NITERS 10000000

// Deklarasi prototipe fungsi count
void *count(void *arg);

// Variabel global cnt (penghitung) yang akan diakses oleh kedua thread
unsigned int cnt = 0;

// Mutex global untuk sinkronisasi
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    // Deklarasi dua variabel untuk ID thread
    pthread_t tid1, tid2;

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

// Implementasi fungsi count dengan metode busy-waiting (polling)
void *count(void *arg)
{
    int i;
    int ret;  // Variabel untuk menampung return value dari pthread_mutex_trylock()

    // Loop sebanyak NITERS kali
    for (i = 0; i < NITERS; i++)
    {
        // BUSY-WAITING / POLLING:
        // Mencoba mengunci mutex dengan trylock (non-blocking)
        // Jika mutex sedang terkunci (EBUSY), loop akan terus berulang
        // Ini membuat CPU terus bekerja selama menunggu (busy-waiting)
        while ((ret = pthread_mutex_trylock(&mutex)) == EBUSY)
        {
            // Tidak melakukan apa-apa, hanya terus memeriksa
            // CPU akan sibuk selama loop ini berjalan
        }

        // ---------- CRITICAL REGION ----------
        // Setelah berhasil mendapatkan lock, lakukan increment
        cnt++;
        // ---------- AKHIR CRITICAL REGION ----------

        // Membuka kunci mutex
        pthread_mutex_unlock(&mutex);
    }

    // Fungsi count selesai
}
