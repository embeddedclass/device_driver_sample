#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#define DEVFILE "/dev/misc_driver"

int main(void)
{
        int fd;

        fd = open(DEVFILE, O_RDWR);
        if (fd == -1) {
                perror("open");
        }
        sleep(3);

        if (close(fd) != 0) {
                perror("close");
        }

        return 0;
}
