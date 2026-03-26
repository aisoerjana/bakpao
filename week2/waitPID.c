#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
	pid_t child;
	int status;
	time_t when;
	
	if ((child = fork()) == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (child == 0)
	{
		time(&when);
		printf("In child process.....Started at%s", ctime(&when));
		printf("\tchild pid = %d\n", getpid());
		printf("\tchild ppid = %d\n", getppid());
		for(;;);
		exit(EXIT_SUCCESS);
	}
	else
	{
		time(&when);
		waitpid(child,&status, 0);
		printf("In parent process.....Started at %s", ctime(&when));
		printf("\tparent pid = %d\n", getpid());
		printf("\tparent ppid = %d\n", getppid());
		printf("\tchild exited with %d\n", status);
		for(;;);
		exit(EXIT_SUCCESS);
	}
}
