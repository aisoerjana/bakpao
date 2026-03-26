#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int f1,f2,f3;

  f1 = fork();
  f2 = fork();
  f3 = fork();

  printf("PID:%d | f1:%d f2:%d f3:%d\n", getpid(), f1, f2, f3);
return 0;
}
