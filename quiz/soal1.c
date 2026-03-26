#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void toUpper(char *str)
{
  int i;
  for (i = 0; str[i]; i++)
  {
    str[i] = toupper((unsigned char)str[i]);
  }
}

int main()
{
  pid_t pidB, pidC, pidD;
  int status;
  char input[100];
  char upper[100];

  printf("Masukkan kalimat: ");
  scanf("%[^\n]", input);

  pidB = fork();
  if (pidB == 0)
  {
    printf("Process B: #%d #%d : %s\n", getpid(), getppid(), input);

    pidD = fork();
    if (pidD == 0)
    {
      printf("Process D: #%d #%d : %s\n", getpid(), getppid(), input);
      exit(EXIT_SUCCESS);
    }

    waitpid(pidD, &status, 0);
    exit(EXIT_SUCCESS);
  }

  pidC = fork();
  if (pidC == 0)
  {
    strcpy(upper, input);
    toUpper(upper);
    printf("Process C: #%d #%d : %s\n", getpid(), getppid(), upper);
    exit(EXIT_SUCCESS);
  }

  waitpid(pidB, &status, 0);
  waitpid(pidC, &status, 0);
  printf("Process A: #%d #%d : %s\n", getpid(), getppid(), input);
  return 0;
}