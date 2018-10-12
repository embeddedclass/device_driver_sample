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
#include <linux/miscdevice.h>

#define DRIVER_NAME "misc_driver"

static int misc_open(struct inode *inode, struct file *file)
{
	printk("Call misc_open\n");
	return 0;
}

static int misc_close(struct inode *inode, struct file *file)
{
        printk("Call misc_close\n");
        return 0;
}

struct file_operations fops = {
	.owner 	 = THIS_MODULE,
    .open    = misc_open,
    .release = misc_close,
};

static struct miscdevice misc_dev =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name  = DRIVER_NAME,
	.fops  = &fops,
};

static int misc_init(void) 
{
	printk("%s driver loaded\n", DRIVER_NAME);
	int ret;

	ret = misc_register(&misc_dev);
	if (ret){
		printk("misc_register error\n");
	  	return ret;
	} 

	return 0;
} 

static void misc_exit(void) 
{
	misc_deregister(&misc_dev);
	printk("%s driver removed\n",DRIVER_NAME); 
} 

module_init(misc_init); 
module_exit(misc_exit);

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Slash");
MODULE_DESCRIPTION("This is misc  module");
MODULE_VERSION("V1");
