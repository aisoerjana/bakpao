#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// Mendefinisikan konstanta NITERS dengan nilai 10.000.000
// Konstanta ini akan digunakan sebagai jumlah iterasi loop di setiap thread
#define NITERS 10000000

// Deklarasi prototipe fungsi count yang akan dijalankan oleh thread
// Fungsi ini menerima argumen bertipe void* dan mengembalikan void*
void *count(void *arg);

// Variabel global cnt (penghitung) yang akan diakses dan dimodifikasi oleh kedua thread
// Karena bersifat global dan tidak dilindungi, ini akan menyebabkan race condition
unsigned int cnt = 0;

int main()
{
    // Deklarasi dua variabel untuk menampung identifier (ID) thread
    // Catatan: seharusnya tid1 dan tid2, tapi tertulis tid1 (mungkin typo)
    pthread_t tid1, tid2;

    // Membuat thread pertama (tid1) yang akan menjalankan fungsi count
    // Parameter: &tid1 (penyimpan ID thread), NULL (atribut default),
    //            count (fungsi yang dijalankan), NULL (argumen fungsi)
    pthread_create(&tid1, NULL, count, NULL);

    // Membuat thread kedua (tid2) yang juga menjalankan fungsi count yang sama
    pthread_create(&tid2, NULL, count, NULL);

    // Menunggu thread pertama (tid1) selesai dieksekusi
    // pthread_join akan memblokir program utama hingga thread tid1 selesai
    pthread_join(tid1, NULL);

    // Menunggu thread kedua (tid2) selesai dieksekusi
    pthread_join(tid2, NULL);

    // Memeriksa apakah nilai cnt sesuai dengan yang diharapkan
    // Yang diharapkan: setiap thread melakukan increment sebanyak NITERS kali
    // Dua thread = 2 * NITERS = 20.000.000
    if (cnt != (unsigned)NITERS*2)
        // Jika tidak sesuai (karena race condition), cetak pesan kesalahan
        printf("Wrong... cnt = %d\n", cnt);
    else
        // Jika sesuai (hampir tidak pernah terjadi tanpa sinkronisasi), cetak sukses
        printf("OK...\tcnt=%d\n", cnt);

    // Mengakhiri program dengan kode 0 (menandakan sukses)
    exit(0);
}

// Implementasi fungsi count yang akan dijalankan oleh thread
void *count(void *arg)
{
    // Variabel lokal sebagai counter loop
    int i;

    // Loop sebanyak NITERS kali (10 juta kali)
    for (i=0; i<NITERS; i++)

        // INI ADALAH BAGIAN KRITIS:
        // Operasi increment pada variabel global cnt
        // Operasi ini TIDAK ATOMIK (terdiri dari: baca, tambah, tulis)
        // Karena dua thread melakukannya bersamaan, bisa terjadi interleaving
        // yang menyebabkan hasil akhir tidak sesuai harapan (race condition)
        cnt++;

        // Fungsi count tidak mengembalikan nilai (return NULL secara implisit)
}
