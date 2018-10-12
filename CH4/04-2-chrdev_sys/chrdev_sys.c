/* 
 * Copy from Terence
 *
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

#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define DRIVER_NAME "chrdev_sys"
#define DEVICE_ID "0123456"
static unsigned int chrdev_sys_major = 0;
static struct class *chrdev_sys_class = NULL;
struct device *chrdev_sys_device;

struct device_attribute device_id_attr;

static int chrdev_sys_open(struct inode *inode, struct file *file)
{
	printk("Call chrdev_sys_open...\n");
	return 0;
}

static int chrdev_sys_close(struct inode *inode, struct file *file)
{
	printk("Call chrdev_sys_close\n");
	return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open    = chrdev_sys_open,
    .release = chrdev_sys_close,
};

static ssize_t device_id_show(struct device *class_dev,
			 struct device_attribute *attr,
			 char *buf)
{

	return snprintf(buf, 10, "%s\n", DEVICE_ID);
}

static int chrdev_sys_init(void) 
{
	int ret;

	ret = register_chrdev(chrdev_sys_major, DRIVER_NAME, &fops);
	if(ret <= 0 )
		return ret;

    
    sysfs_attr_init(&device_id_attr.attr);
	device_id_attr.attr.name = "id";
	device_id_attr.attr.mode = S_IRUGO;
	device_id_attr.show = device_id_show;
	device_id_attr.store = NULL;


	/* Create sys class */
	chrdev_sys_class = class_create(THIS_MODULE, DRIVER_NAME); 
	if (IS_ERR(chrdev_sys_class))
        return -1;

	/* Register sysfs */
	chrdev_sys_device = device_create(chrdev_sys_class, NULL, MKDEV(250, 0),
	    NULL, "chrdev_sys_info");

    device_create_file(chrdev_sys_device, &device_id_attr);

	chrdev_sys_major = ret;
	printk("%s driver(major %d) installed.\n", DRIVER_NAME, chrdev_sys_major);

	return 0;
} 

static void chrdev_sys_exit(void) 
{
    dev_t dev = MKDEV(chrdev_sys_major, 0);       

    printk("%s driver removed\n",DRIVER_NAME);
    /* Unregister class */
    device_destroy(chrdev_sys_class, dev);
	class_destroy(chrdev_sys_class);
    unregister_chrdev(dev, DRIVER_NAME);
} 

module_init(chrdev_sys_init); 
module_exit(chrdev_sys_exit);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Terence");
MODULE_DESCRIPTION("This is sysfs test module");
MODULE_VERSION("V1");
