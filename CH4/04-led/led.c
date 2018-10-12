 
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
#include <linux/proc_fs.h>

/*
 *   pr_emerg()
 *   pr_alert()
 *   pr_crit()
 *   pr_err()
 *   pr_warning()
 *   pr_warn pr_warning()
 *   pr_notice()
 *   pr_info()
 *   pr_cont()
 */

#define PROC_DIR_NAME "slash_info"
#define PROC_NAME "slash"


struct chrdev_rw_data {
    int id;
    struct class *chrdev_rw_class;
    struct device *chrdev_rw_device;
    unsigned int chrdev_rw_major;
    struct cdev chrdev_rw_cdev;
    struct proc_dir_entry *proc_dir;
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

int enable_dbg = 0;

int proc_open(struct inode *inode, struct file *file)
{
    if (enable_dbg)
        pr_info("%s\r\n", __func__);

    return 0;
}

int proc_close(struct inode *inode, struct file *file)
{
    if (enable_dbg)
        pr_info("%s\r\n", __func__);

    return 0;
}

ssize_t proc_read(struct file *filp, char __user *buf,
    size_t count, loff_t *f_pos)
{
    int ret = 0;
    unsigned char led_val = 0;

    if (enable_dbg)
        pr_info("%s\r\n", __func__);

    led_val = gpio_get_value(led_gpios[0]);
    led_val = led_val | (gpio_get_value(led_gpios[1]) << 1);
    led_val = led_val | (gpio_get_value(led_gpios[2]) << 2);
    led_val = led_val | (gpio_get_value(led_gpios[3]) << 3);

    ret = copy_to_user(buf, &led_val, 1);
    if(ret)
        return -EFAULT;

    if (enable_dbg)
        pr_info("led_val 0x%x\r\n", led_val);

    return ret;
}

ssize_t proc_write(struct file *filp, const char __user *buf,
    size_t count, loff_t *f_pos)
{
    int ret = 0, i = 0;
    char val[50] = {0};

    ret = copy_from_user(&val, buf, count - 1);
    if(ret)
        return -EFAULT;

    if (!strcmp(val, "enable"))
        enable_dbg = 1;
    else
        enable_dbg = 0;

    return count;
}

struct file_operations proc_ops = {
    .open = proc_open,
    .release = proc_close,
    .read  = proc_read,
    .write  = proc_write,
};


ssize_t chrdev_rw_read(struct file *filp, char __user *buf,
    size_t count, loff_t *f_pos)
{
    int ret;
    unsigned char led_val = 0;

    if (enable_dbg)
        pr_info("%s\r\n", __func__);

    led_val = gpio_get_value(led_gpios[0]);
    led_val = led_val | (gpio_get_value(led_gpios[1]) << 1);
    led_val = led_val | (gpio_get_value(led_gpios[2]) << 2);
    led_val = led_val | (gpio_get_value(led_gpios[3]) << 3);

    ret = copy_to_user(buf, &led_val, count - 1);
    if(ret)
        return -EFAULT;

    return ret;
}



ssize_t chrdev_rw_write(struct file *filp, const char __user *buf,
    size_t count, loff_t *f_pos)
{
    int ret = 0;
    unsigned char chr_val = 0, led_sel = 0, led_val = 0;

    if (enable_dbg)
        pr_info("%s\r\n", __func__);

    ret = copy_from_user(&chr_val, buf, count);
    if(ret)
        return -EFAULT;

    /**********************
    * 0000 CCVV
    *********************/

    led_sel = (chr_val & 0x0C) >> 2;
    led_val = chr_val & 0x01;

    gpio_set_value(led_gpios[led_sel], led_val);

    return ret;
}


static int chrdev_rw_open(struct inode *inode, struct file *file)
{
    if (enable_dbg)
        pr_info("%s\r\n", __func__);

    _chrdev_rw_data->id = 10;
    file->private_data = _chrdev_rw_data;
    return 0;
}

static int chrdev_rw_close(struct inode *inode, struct file *file)
{
    struct chrdev_rw_data *data;

    if (enable_dbg)
        pr_info("%s\r\n", __func__);

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
    int i = 0, ret = 0;

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

    if (enable_dbg)
        pr_info("%s\r\n", __func__);

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

    if (enable_dbg)
        pr_info("%s\r\n", __func__);

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

    /* proc file */
    _chrdev_rw_data->proc_dir = proc_mkdir(PROC_DIR_NAME, NULL);
    proc_create(PROC_NAME, 666, _chrdev_rw_data->proc_dir, &proc_ops);

    ret = led_init();
    if (ret < 0)
        pr_err("led_init() fail\r\n");

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

    if (enable_dbg)
        pr_info("%s\r\n", __func__);

    led_deinit();

    dev = MKDEV(_chrdev_rw_data->chrdev_rw_major, 0);
    cdev_del(&(_chrdev_rw_data->chrdev_rw_cdev));
    device_del(_chrdev_rw_data->chrdev_rw_device);
    class_destroy(_chrdev_rw_data->chrdev_rw_class);
    unregister_chrdev_region(dev, DEVICE_NUM);

    remove_proc_entry(PROC_NAME, _chrdev_rw_data->proc_dir);
    remove_proc_entry(PROC_DIR_NAME, NULL);

    kfree(_chrdev_rw_data);

}

module_init(tiny4412_led_dev_init);
module_exit(tiny4412_led_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tiny4412");

