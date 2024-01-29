#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define GET_PHY_ADDR _IO('q',1)
#define CHNG_VAL _IO('q',2)
typedef struct
{
    unsigned long val,pid, virt_addr,phys_addr;
}arg_t;
 
int main(int argc, char *argv[])
{
    char *file_name = "/dev/ioctl_driver";
    int fd = open("/dev/ioctl_driver", O_RDWR);
    if (fd < 0) {
        perror("Error opening the device");
        return fd;
    }
    
    char *alloc = malloc(1);
    if (!alloc) {
        perror("Error allocating memory");
        close(fd);
        return -1;
    }
    *alloc = 6;

    printf("Virtual Address: %p, Value: %d\n", alloc, *alloc);

    arg_t data;
    data.virt_addr= (unsigned long)alloc;
    data.pid=getpid();
    if (ioctl(fd,GET_PHY_ADDR, &data) < 0) {
        perror("Error in ioctl (GET_PHYSICAL_ADDR)");
        free(alloc);
        close(fd);
        return -1;
    }

    printf("Physical Address: 0x%lx\n", data.phys_addr);

    data.val = 5;
    if (ioctl(fd, CHNG_VAL, &data) < 0) {
        perror("Error in ioctl (WRITE_MEMORY)");
        free(alloc);
        close(fd);
        return -1;
    }

    printf("Modified Value: %d\n", *alloc);

    free(alloc);
    close(fd);


    return 0;
}