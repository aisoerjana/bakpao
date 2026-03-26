#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

int g1 = 0;

void task1 (int *counter)
{
	while (*counter < 10) {
		printf("task1 count: %d\n", *counter);
		(*counter)++;
		sleep(1); 
		}
	g1 = 10;
}

void task2 (int *counter)
{
	while (*counter <10) {
		printf("task2 count: %d\n", *counter);
		(*counter)++;
		sleep(1);
		}
	g1 = 20;
}

int main()
{
	pthread_t thrd1, thrd2;
	int a = 0, b = 0, ret;
	
	printf("Global variable g1 = %d\n", g1);
	
	// Buat thread1
	ret = pthread_create (&thrd1, NULL, (void *) task1, (void *) &a);
	if (ret != 0) 
	{
	  perror("pthread_create: task1\n"); 
	  exit(EXIT_FAILURE);
	}
	// Tunggu thread1 selesai
	pthread_join(thrd1, NULL);
	
	// Buat thread2
	ret = pthread_create (&thrd2, NULL, (void *) task2, (void *) &b);
	if (ret != 0)
	{
	  perror("pthread_create: task2\n"); 
	  exit(EXIT_FAILURE);
	}
	//Tunggu thread2 selesai
	pthread_join (thrd2, NULL); 
    
	printf("Global variable g1 = %d\n", g1);
}	
