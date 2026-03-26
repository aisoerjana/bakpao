#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <time.h>

int main(void)
{
    int fd;
    int len;
    char buf[PIPE_BUF];
    time_t tp;

    // Open the FIFO write only
    if ((fd = open("fifo1", O_WRONLY)) < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // generate some data to write
    while (1)
    {
        time(&tp); // Get the current time

        // create the string to write
        len = sprintf(buf, "Wrfifo writes %d sends %s", getpid(), ctime(&tp));

        if (write(fd, buf, len + 1) < 0)
        {
            perror("write");
            close(fd);
            exit(EXIT_FAILURE);
        }

        sleep(3);
    }

    close(fd);
    exit(EXIT_SUCCESS);
}
