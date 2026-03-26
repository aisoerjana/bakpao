#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void printchar (char name);

int main()
{
  pid_t pidB, pidC, pidD, pidF;
  int status;
  
  // A membuat B
  pidB = fork();
  if (pidB == 0)
  {
    printchar('B');
    
      // B membuat D
      pidD = fork();
      if (pidD == 0)
      {
        printchar('D');
        exit(EXIT_SUCCESS);
      }
      
      // B membuat F
      pidF = fork();
      if (pidF == 0)
      {
        printchar('F');
        exit(EXIT_SUCCESS);
      }
    
    // B harus menunggu D dan F terlebih dahulu
    waitpid(pidD, &status, 0);
    waitpid(pidF, &status, 0);
    exit(EXIT_SUCCESS);
  }
  
  // A membuat C
  pidC = fork();
  if (pidC == 0)
  {
    printchar('C');
    exit(EXIT_SUCCESS);
  }
  
  // A harus menunggu B dan C terlebih dahulu
  waitpid(pidB, &status, 0);
  waitpid(pidC, &status, 0);
  printchar('A');
  return 0;
}

void print(char name) 
{
  int i;
  for (i = 0; i < 5; i++) 
  {
    printf("Process Name: %c | PID: %d | PPID: %d\n", name, getpid(), getppid());
  }
}
