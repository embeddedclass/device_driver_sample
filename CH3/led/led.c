 
/* 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h> 
#include <linux/gpio.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <asm/uaccess.h>

struct chrdev_rw_data {
    int id;
    struct class *chrdev_rw_class;
    struct device *chrdev_rw_device;
    unsigned int chrdev_rw_major;
    struct cdev chrdev_rw_cdev;
};

 
static int led_gpios[] = {
	EXYNOS4X12_GPM4(0),
	EXYNOS4X12_GPM4(1),
	EXYNOS4X12_GPM4(2),
	EXYNOS4X12_GPM4(3),
};

struct chrdev_rw_data *_chrdev_rw_data;

#define DEVICE_NAME "slash-led"
#define DEVICE_NUM 1
#define LED_NUM ARRAY_SIZE(led_gpios)
#define LED_ENABLE 0
#define LED_DISABLE 1


ssize_t chrdev_rw_read(struct file *filp, char __user *buf,
    size_t count, loff_t *f_pos)
{
	int ret;
	unsigned char led_val = 0;


    led_val = gpio_get_value(led_gpios[0]);
    led_val = led_val | (gpio_get_value(led_gpios[1]) << 1);
    led_val = led_val | (gpio_get_value(led_gpios[2]) << 2);
    led_val = led_val | (gpio_get_value(led_gpios[3]) << 3);

    printk("Call chrdev_rw_read...\n");

    ret = copy_to_user(buf, &led_val, count);
    if(ret)
        return -EFAULT;

    return ret;
}



ssize_t chrdev_rw_write(struct file *filp, const char __user *buf,
    size_t count, loff_t *f_pos)
{
    int ret = 0;
	unsigned char chr_val = 0, led_sel = 0, led_val = 0;

    printk("Call chrdev_rw_write...\n");

	ret = copy_from_user(&chr_val, buf, count);
    if(ret)
        return -EFAULT;

	printk("Call chrdev_rw_write... chr_val = 0x%x\n", chr_val);

    /**********************
    * 0000 CCVV
    *********************/

    led_sel = (chr_val & 0x0C) >> 2;
    led_val = chr_val & 0x01;

	printk("Call chrdev_rw_write... led_sel = 0x%x\n", led_sel);
	printk("Call chrdev_rw_write... led_val = 0x%x\n", led_val);

    gpio_set_value(led_gpios[led_sel], led_val);

    return ret;
}


static int chrdev_rw_open(struct inode *inode, struct file *file)
{
    /* struct chrdev_rw_data *data;  */

    printk("Call chrdev_rw_open...\n");

    _chrdev_rw_data->id = 10;

    file->private_data = _chrdev_rw_data;

    return 0;
}

static int chrdev_rw_close(struct inode *inode, struct file *file)
{
    printk("Call chrdev_rw_close...\n");

    return 0;
}


struct file_operations fops = {
    .owner	 = THIS_MODULE,
    .open	 = chrdev_rw_open,
    .release = chrdev_rw_close,
    .read 	 = chrdev_rw_read,
    .write 	 = chrdev_rw_write,
};

int led_init(void)
{
	int ret, i;

    for (i = 0; i < LED_NUM; i++) {
        ret = gpio_request(led_gpios[i], "LED");
        if (ret) {
            printk("%s: request GPIO %d for LED failed, ret = %d\n",
                DEVICE_NAME, led_gpios[i], ret);
            return ret;
        }
        s3c_gpio_cfgpin(led_gpios[i], S3C_GPIO_OUTPUT);
        gpio_set_value(led_gpios[i], 0);
        gpio_direction_output(led_gpios[i], LED_DISABLE);
        printk("GPIO [%d] - %d\n", led_gpios[i], i);
    }
    return 0;
}

void led_deinit(void)
{
    int i;

    for (i = 0; i < LED_NUM; i++) {
        gpio_set_value(led_gpios[i], LED_DISABLE);
        gpio_free(led_gpios[i]);
    }
}

static int __init tiny4412_led_dev_init(void) 
{
	int ret;
    int alloc_ret = 0;
    int cdev_ret = 0;
    dev_t devt;
    struct class *dev_class;


    _chrdev_rw_data = kmalloc(sizeof(struct chrdev_rw_data), GFP_KERNEL);

    alloc_ret = alloc_chrdev_region(&devt, 0, DEVICE_NUM, DEVICE_NAME);
    if(alloc_ret)
        goto error;

    cdev_init(&(_chrdev_rw_data->chrdev_rw_cdev), &fops);
    cdev_ret = cdev_add(&(_chrdev_rw_data->chrdev_rw_cdev), devt, DEVICE_NUM);
    if (cdev_ret)
        goto error;

    _chrdev_rw_data->chrdev_rw_class = class_create(THIS_MODULE, DEVICE_NAME);

    dev_class = _chrdev_rw_data->chrdev_rw_class;
    _chrdev_rw_data->chrdev_rw_device = device_create(dev_class, NULL,
        devt, NULL, "%s-%d", DEVICE_NAME, 0);


	cdev_ret = led_init();
    if (cdev_ret) {
		pr_err("led_init fail\r\n");
	}

    _chrdev_rw_data->chrdev_rw_major = MAJOR(devt);
    printk(DEVICE_NAME"initialized\n");

    return 0;

error:
    if (cdev_ret == 0)
        cdev_del(&_chrdev_rw_data->chrdev_rw_cdev);

    if (alloc_ret == 0)
        unregister_chrdev_region(devt, DEVICE_NUM);

    kfree(_chrdev_rw_data);

    return ret;
}

static void __exit tiny4412_led_dev_exit(void) 
{
    dev_t dev;

	led_deinit();
    dev = MKDEV(_chrdev_rw_data->chrdev_rw_major, 0);
    cdev_del(&(_chrdev_rw_data->chrdev_rw_cdev));
    device_del(_chrdev_rw_data->chrdev_rw_device);
    class_destroy(_chrdev_rw_data->chrdev_rw_class);
    unregister_chrdev_region(dev, DEVICE_NUM);
    kfree(_chrdev_rw_data);
}

module_init(tiny4412_led_dev_init);
module_exit(tiny4412_led_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tiny4412");

