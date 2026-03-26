#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>

int main(void)
{
    int fd;
    int len;
    char buf[PIPE_BUF]; // defined in /usr/include/linux/limits.h
    mode_t mode = 0666; // mode for file attributes

    if ((mkfifo("fifo1", mode)) < 0)
    {
    	int error = errno;
        if (error == EEXIST)
            printf("Continue using an existing pipe\n");
        else
        {
            perror("mkfifo");
            exit(EXIT_FAILURE);
        }
    }

    // Open the FIFO read-only
    if ((fd = open("fifo1", O_RDONLY)) < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read and display the FIFO's output until EOF
    while ((len = read(fd, buf, PIPE_BUF-1)) > 0 )
        printf("rdfifo read: %s", buf);

    close(fd);
    remove("fifo1");
    exit(EXIT_SUCCESS);
}
