#include <semaphore.h>   // Library untuk semaphore POSIX
#include <stdio.h>       // Library untuk printf() dan perror()

#define INITVALUE 1      // Nilai awal semaphore
#define MAXCOUNT 10      // Jumlah perulangan

int main()
{
    sem_t semid;        // Variabel semaphore
    int i, sem_value;   // i untuk loop, sem_value untuk menyimpan nilai semaphore

    // Inisialisasi semaphore
    // 0 = hanya digunakan dalam proses ini (bukan antar proses)
    // INITVALUE = nilai awal semaphore
    if (sem_init(&semid, 0, INITVALUE) == -1) {
        perror("Failed to initialize semaphore");
        return 1;
    }

    // Loop sebanyak 10 kali
    for(i = 0; i < MAXCOUNT; i++){

        // Ambil nilai semaphore saat ini
        if (sem_getvalue(&semid, &sem_value) == -1)
            perror("Failed to get semaphore value");

        // Cetak nilai semaphore sebelum dilakukan sem_wait
        printf("Semaphore_value = %d \n", sem_value);

        // Operasi DOWN (mengurangi nilai semaphore)
        if (sem_wait(&semid) == -1)
            perror("Failed to decrement semaphore");

        // Operasi UP (menambah nilai semaphore kembali)
        /*if (sem_post(&semid) == -1)
            perror("Failed to increment semaphore");*/

        sem_post(&semid);
    }

    // Hapus semaphore dari memori
    sem_destroy(&semid);

    return 0;
}
