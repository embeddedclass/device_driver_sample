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
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define PROC_DIR_NAME "slash_info"
#define PROC_NAME "slash"

struct proc_dir_entry *proc_dir;

int proc_open(struct inode *inode, struct file *file)
{
    pr_info("%s\r\n", __func__);

    return 0;
}

int proc_close(struct inode *inode, struct file *file)
{
    pr_info("%s\r\n", __func__);

    return 0;
}

ssize_t proc_read(struct file *filp, char __user *buf,
    size_t count, loff_t *f_pos)
{
    int ret = 0;
    pr_info("%s\r\n", __func__);

    return ret;
}

ssize_t proc_write(struct file *filp, const char __user *buf,
    size_t count, loff_t *f_pos)
{
    pr_info("%s\r\n", __func__);

    return count;
}

struct file_operations proc_ops = {
    .open = proc_open,
    .release = proc_close,
    .read  = proc_read,
    .write  = proc_write,
};

static int procfs_rw_init(void) 
{
    int ret = 0;

    proc_dir = proc_mkdir(PROC_DIR_NAME, NULL);

    printk(KERN_ALERT "Call Procfs init...\n");

    proc_create(PROC_NAME, 666, proc_dir, &proc_ops);

out:
    return ret;
}

static void procfs_rw_exit(void) 
{
    printk(KERN_ALERT "Procfs exit\n");
    remove_proc_entry(PROC_NAME, proc_dir);
    remove_proc_entry(PROC_DIR_NAME, NULL);
}

module_init(procfs_rw_init);
module_exit(procfs_rw_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Terence");
MODULE_DESCRIPTION("This is proc write filesystem test module");
MODULE_VERSION("V1");
