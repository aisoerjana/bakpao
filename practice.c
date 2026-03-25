#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define NUM_PHIL 5
#define SLEEP_TIME 2

// Array mutex yang merepresentasikan fork
// Setiap fork adalah resource yang harus dikunci sebelum digunakan
pthread_mutex_t forks[NUM_PHIL];

// Fungsi yang akan dijalankan oleh setiap thread philosopher
void* philosopher(void* num)
{
    int id = *(int*)num;              // Mengambil ID philosopher dari parameter
    int left_fork = id;               // Fork kiri adalah fork dengan indeks sama dengan ID philosopher
    int right_fork = (id + 1) % NUM_PHIL; // Fork kanan adalah fork berikutnya (melingkar)
    
    // solusi resource hierarchy
    // jadi dengan solusi ini, program akan mengambil fork dengan nomor lebih kecil dulu
    // ini digunakan untuk memutus siklus circular wait yang menyebabkan deadlock
    int first_fork;
    int second_fork;
    if (left_fork < right_fork)
    {
    	first_fork = left_fork;
    	second_fork = right_fork;
    }
    else
    {
    	first_fork = right_fork;
    	second_fork = left_fork;
    }

    while (1) { // Loop terus menerus (philosopher terus berpikir dan makan)

        printf("Philosopher %d is thinking.\n", id);  // Menampilkan bahwa philosopher sedang berpikir
        sleep(rand() % SLEEP_TIME);                    // Delay acak untuk simulasi waktu berpikir

        printf("Philosopher %d is HUNGRY.\n", id);    // Philosopher menjadi lapar

        pthread_mutex_lock(&forks[first_fork]);         // Mengunci fork kiri agar tidak dipakai philosopher lain
        printf("Philosopher %d picked up first fork %d.\n", id, first_fork); // Informasi fork kiri diambil

        sleep(1);                                      // Delay kecil untuk simulasi waktu

        pthread_mutex_lock(&forks[second_fork]);        // Mengunci fork kanan
        printf("Philosopher %d picked up second fork %d and is EATING.\n", id, second_fork); // Mulai makan

        sleep(rand() % SLEEP_TIME);                    // Simulasi waktu makan

        pthread_mutex_unlock(&forks[second_fork]);      // Melepaskan fork kanan setelah selesai makan
        pthread_mutex_unlock(&forks[first_fork]);       // Melepaskan fork kiri

        printf("Philosopher %d finished eating and put down forks.\n", id); // Informasi selesai makan
    }
}

int main()
{
    pthread_t phils[NUM_PHIL];    // Array untuk menyimpan thread philosopher
    int phil_ids[NUM_PHIL];       // Array untuk menyimpan ID masing-masing philosopher

    // Inisialisasi mutex untuk setiap fork
    for (int i = 0; i < NUM_PHIL; i++) {
        pthread_mutex_init(&forks[i], NULL); // Menginisialisasi mutex fork ke-i
    }

    // Membuat thread philosopher
    for (int i = 0; i < NUM_PHIL; i++) {
        phil_ids[i] = i;  // Memberikan ID ke philosopher

        // Membuat thread baru yang menjalankan fungsi philosopher
        pthread_create(&phils[i], NULL, philosopher, &phil_ids[i]);
    }

    // Menunggu semua thread selesai (join)
    for (int i = 0; i < NUM_PHIL; i++) {
        pthread_join(phils[i], NULL); // Program utama menunggu thread philosopher selesai
    }

    return 0; // Program selesai
}
