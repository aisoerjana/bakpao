#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NITERS 10000000

void count(void *arg);
unsigned int cnt = 0;

int main()
{
	pthread_t tid1, tid2;
	
	pthread_create(&tid1, NULL, (void *)count, NULL);
	pthread_create(&tid2, NULL, (void *)count, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	
	if (cnt != (unsigned)NITERS*2)
		printf("Wrong... cnt = %d\n", cnt);
	else
		printf("OK...\tcnt = %d\n", cnt);
	exit(0);
}

void count(void *arg)
{
	int i;
	
	for (i = 0; i < NITERS;  i++)
		cnt++;
}
