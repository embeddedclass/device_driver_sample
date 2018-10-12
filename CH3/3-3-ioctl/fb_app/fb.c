#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

main()
{
    int fd;
    int ret;
    struct fb_var_screeninfo fbvar;
    struct fb_fix_screeninfo fbfix;

    fd = open("/dev/fb0", O_RDWR);
    ret = ioctl(fd, FBIOGET_VSCREENINFO, &fbvar);
    ret = ioctl(fd, FBIOGET_FSCREENINFO, &fbfix);

    printf("x res      : %d\n", fbvar.xres);
    printf("y res      : %d\n", fbvar.yres);
    printf("x res virt : %d\n", fbvar.xres_virtual);
    printf("y res virt : %d\n", fbvar.yres_virtual);
    printf("bpp        : %d\n", fbvar.bits_per_pixel);
    printf("base       : %x\n", fbfix.smem_start);

    printf("buffer mem len : %d\n", fbfix.smem_len);

    close(fd);
}
