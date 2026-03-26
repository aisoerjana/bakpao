#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NITERS 1000000

void *count(void *arg);

// Shared counter
unsigned int cnt = 0;

// Global mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{
	pthread_t tid1, tid2;
	
	// Create two threads
	pthread_create(&tid1, NULL, count, NULL);
	pthread_create(&tid2, NULL, count, NULL);
	
	// Wait for both therads to finish
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	
	// Check result
	if (cnt != (unsigned) NITERS * 2)
		printf("Wrong... cnt = %u\n", cnt);
	else
		printf("OK...\tcnt = %u\n", cnt);
	return 0;
}

void *count(void *arg)
{
	int i;
	
	for (i=0; i<NITERS; i++)
	{
		// ===== Critical Region START =====
		pthread_mutex_lock(&mutex);
		
		cnt++; // shared variable modification
		
		pthread_mutex_unlock(&mutex);
		// ===== Critical Region END =====
	}
	return NULL;
}
