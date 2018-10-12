#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#define DEVFILE "/dev/chrdev_sys_info"

int main(void)
{
        int fd;

        fd = open(DEVFILE, O_RDWR);
        if (fd == -1) {
            perror("open");
            return 0;
		}

        sleep(3);

        if (close(fd) != 0)
                perror("close");

        return 0;
}
