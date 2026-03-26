#define _GNU_SOURCE
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void* thread_function(void* arg)
{
	printf("child thread:\t pid = %d \t t_id (Linux) = %d \t t_id (Posix) = %lu\n", getpid(), getppid(), pthread_self());
	while(1);
}

int main()
{
	pthread_t thread1, thread2;
	int ret;
	
	printf("main thread:\t pid = %d \t t_id (Linux) = %d \t t_id (Posix) = %lu\n", getpid(), gettid(), pthread_self());
	ret = pthread_create(&thread1, NULL, &thread_function, NULL);
	if (ret != 0)
	{
		perror("pthread_create: task1\n");
		exit (EXIT_FAILURE);
	}
	
	ret = pthread_create(&thread2, NULL, &thread_function, NULL);
	if (ret != 0)
	{
		perror("pthread_create: task2\n");
		exit (EXIT_FAILURE);
	}


pthread_join(thread1, NULL);
while (1);
// pthread_join(thread2, NULL);
return 0;
}
