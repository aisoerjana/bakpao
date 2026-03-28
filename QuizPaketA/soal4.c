#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Deklarasi Variabel Global
#define CONTESTANT 3
int mangkukTersedia;
int mangkukTerkonsumsi[CONTESTANT] = {0};
// (A) [QUESTION] Deklarasikan variabel Mutex di sini! -- (2 Points)
pthread_mutex_t lock;

void *makanMie(void *arg){
    int id = *(int *)arg;
    while (1){

        // (B) [QUESTION] Kunci akses ke variabel global -- (1.5 Points)
        pthread_mutex_lock(&lock);
        
        if(mangkukTersedia <= 0){
            // (C) [QUESTION] Apa yang harus dilakukan sebelum keluar dari loop? -- (2 Points)
            pthread_mutex_unlock(&lock);
            break;
        }
        
        mangkukTersedia--;
        mangkukTerkonsumsi[id]++; 
        printf("Kontestan %d makan 1 mangkuk mie. Sisa di meja: %d\n", id+1, mangkukTersedia);
        
        // (D) [QUESTION] Lepas kembali akses variabel global -- (1.5 Points)
        pthread_mutex_unlock(&lock);
        
        sleep(rand() % 5); 
    } 
    return NULL;
}

// Fungsi tampilkanStatus
void *tampilkanStatus(void *arg){
    while(1){
        // (E) [QUESTION] Kunci akses ke variabel global -- (1.5 Points)
        pthread_mutex_lock(&lock);
        
        if(mangkukTersedia <= 0){
            // (F) [QUESTION] Apa yang harus dilakukan sebelum keluar dari loop? -- (2 Points)
            pthread_mutex_unlock (&lock);
            break;
        }
        
        printf("\t[PAPAN SKOR] Sisa mangkuk: %d\n\tK1: %d Mangkuk, K2: %d Mangkuk, K3: %d Mangkuk\n", 
            mangkukTersedia, mangkukTerkonsumsi[0], mangkukTerkonsumsi[1], mangkukTerkonsumsi[2]);
        // (G) [QUESTION] Lepas kembali akses variabel global -- (1.5 Points)
        pthread_mutex_unlock (&lock);
        sleep(2);
    }
    return NULL;
}

// Fungsi Utama
int main (int argc, char *argv[]) {
    srand(time(NULL));
    
    printf("=== FESTIVAL KICAU MANIA JOMOKERTO ===\n");
    printf("Masukkan jumlah total mangkuk mie ayam (11 - 199): ");
    scanf("%d", &mangkukTersedia);

    // Validasi Input
    if(mangkukTersedia < 11 || mangkukTersedia > 199){
        printf("Jumlah mangkuk harus berjumlah antara 11 - 199!\n");
        return 1;
    }

    // Inisiasi Mutex dan Thread sebelum digunakan
    pthread_t kontestan_thread[CONTESTANT], status_thread;
    pthread_mutex_init(&lock, NULL);

    // Buat Thread untuk tiap kontestan
    int ids[CONTESTANT] = {0, 1, 2};
    for(int i = 0; i < CONTESTANT; i++){
        // (H) [QUESTION] Buat thread untuk masing-masing kontestan -- (2 Points) 
        pthread_create(&kontestan_thread[i], NULL, makanMie, &ids[i]);
    }
    
    // (I) [QUESTION] Buat thread untuk menampilkan status -- (1.5 Points)
    pthread_create(&status_thread, NULL, tampilkanStatus, NULL);
    


    // Menunggu semua thread kontestan selesai
    for(int i = 0; i < CONTESTANT; i++){
        // (J) [QUESTION] Tunggu semua thread peserta selesai (join) -- (1.5 Points)
        pthread_join(kontestan_thread[i], NULL);
    }
    
    // (K) [QUESTION] Tunggu thread status selesai (join) -- (1.5 Points)
    pthread_join(status_thread, NULL);
    

    // (L) [QUESTION] Hancurkan Mutex setelah selesai digunakan -- (1.5 Points)
    pthread_mutex_destroy(&lock);

    // Penentuan juara berdasarkan mangkukTerkonsumsi
    printf("\n=== HASIL AKHIR LOMBA MAKAN MIE ===\n");
    int ranking[CONTESTANT] = {0, 1, 2};
    for(int i = 0; i < CONTESTANT - 1; i++){
        for(int j = i + 1; j < CONTESTANT; j++){
            if(mangkukTerkonsumsi[ranking[i]] < mangkukTerkonsumsi[ranking[j]]){
                int temp = ranking[i];
                ranking[i] = ranking[j];
                ranking[j] = temp;
            }
        }
    }

    // Print hasil kompetisi
    for(int i = 0; i < CONTESTANT; i++){
        printf("Juara %d: Kontestan %d dengan total %d mangkuk mie ayam\n", i + 1, ranking[i] + 1, mangkukTerkonsumsi[ranking[i]]);
    }

    return 0;
}
