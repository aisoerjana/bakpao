#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>

int main(void)
{
    int fdAb, fdBc, fdCa;
    int len;
    char buf[PIPE_BUF];
    pid_t pid_b, pid_c;
    int status;

    if (mkfifo("fifo_AB", 0666) < 0 && errno != EEXIST)
    {
        perror("mkfifo fifo_AB");
        exit(EXIT_FAILURE);
    }
    if (mkfifo("fifo_BC", 0666) < 0 && errno != EEXIST)
    {
        perror("mkfifo fifo_BC");
        exit(EXIT_FAILURE);
    }
    if (mkfifo("fifo_CA", 0666) < 0 && errno != EEXIST)
    {
        perror("mkfifo fifo_CA");
        exit(EXIT_FAILURE);
    }

    pid_b = fork();
    if (pid_b < 0)
    {
        perror("fork B");
        exit(EXIT_FAILURE);
    }

    if (pid_b == 0)
    {
        fdAb = open("fifo_AB", O_RDONLY);
        fdBc = open("fifo_BC", O_WRONLY);

        if (fdAb < 0 || fdBc < 0)
        {
            perror("B open");
            exit(EXIT_FAILURE);
        }

        while ((len = read(fdAb, buf, PIPE_BUF - 1)) > 0)
        {
            buf[len] = '\0';
            printf("Proses B menerima: %s\n", buf);
            write(fdBc, buf, len);
            printf("Proses B forward ke C\n");
        }

        close(fdAb);
        close(fdBc);
        exit(EXIT_SUCCESS);
    }

    pid_c = fork();
    if (pid_c < 0)
    {
        perror("fork C");
        exit(EXIT_FAILURE);
    }

    if (pid_c == 0)
    {
       
        fdBc = open("fifo_BC", O_RDONLY);
        fdCa = open("fifo_CA", O_WRONLY);

        if (fdBc < 0 || fdCa < 0)
        {
            perror("C open");
            exit(EXIT_FAILURE);
        }

        while ((len = read(fdBc, buf, PIPE_BUF - 1)) > 0)
        {
            buf[len] = '\0';
            printf("Proses C menerima: %s\n", buf);
            write(fdCa, buf, len);
            printf("Proses C forward ke A\n");
        }

        close(fdBc);
        close(fdCa);
        exit(EXIT_SUCCESS);
    }

    fdAb = open("fifo_AB", O_WRONLY);
    fdCa = open("fifo_CA", O_RDONLY);

    if (fdAb < 0 || fdCa < 0)
    {
        perror("A open");
        exit(EXIT_FAILURE);
    }

    char *messages[] = {"Abraham Immanuel Soerjana", "NIM : 00000140413", "Jurusan Informatika"};
    int i;
    for (i = 0; i < 3; i++)
    {
        len = strlen(messages[i]);
        printf("\nProses A mengirim: %s\n", messages[i]);
        write(fdAb, messages[i], len);

      
        len = read(fdCa, buf, PIPE_BUF - 1);
        if (len > 0)
        {
            buf[len] = '\0';
            printf("Proses A echo menerima: %s\n", buf);
        }
    }

    close(fdAb);
    close(fdCa);

    waitpid(pid_b, &status, 0);
    waitpid(pid_c, &status, 0);

    remove("fifo_AB");
    remove("fifo_BC");
    remove("fifo_CA");

    exit(EXIT_SUCCESS);
}
