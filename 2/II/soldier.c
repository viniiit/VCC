#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>

typedef struct
{
    pid_t self_pid;
    pid_t parent_pid;
} driver_arg_t;

#define CHNG_PARENT _IOW('q', 1, driver_arg_t *)

int open_driver(const char *dri_name)
{

    int fd_dri = open(dri_name, O_RDWR);
    if (fd_dri == -1)
    {
        perror("ERROR: could not open driver");
    }

    return fd_dri;
}

void close_driver(const char *dri_name, int fd_dri)
{

    int result = close(fd_dri);
    if (result == -1)
    {
        perror("ERROR: could not close driver");
    }
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Usage: %s <parent_pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    pid_t self_pid = getpid();
    pid_t parent_pid = atoi(argv[1]);
    

    const char *dri_name = "/dev/ioctl_dr_sold";
    int fd_dri = open_driver(dri_name);

    driver_arg_t args;
    args.self_pid = self_pid;
    args.parent_pid = parent_pid;
    

    int result = ioctl(fd_dri, CHNG_PARENT, &args);
    if (result == -1)
    {
        perror("ERROR: ioctl failed");
    }

    // close ioctl driver
    close_driver(dri_name, fd_dri);

    return EXIT_SUCCESS;
}
