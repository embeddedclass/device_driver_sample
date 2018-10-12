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

#undef PDEBUG
#ifdef HELLO_DEBUG
# ifdef __KERNEL__
# define PDEBUG(fmt, args...) printk(KERN_DEBUG "PDEBUG : " fmt, ## args) // KERN_DEBUG "<7>"
# else
# define PDEBUG(fmt, args...) fprintf(stderr,fmt, ## args)
# endif
#else
# define PDEBUG(fmt,args...)
#endif

static int hello_init(void)
{
   	printk(KERN_NOTICE"This File is [%s]\n",__FILE__); // KERN_WARNING "<4>"
   	printk(KERN_NOTICE"In Function [%s]\n",__func__);
   	printk(KERN_NOTICE"At Line [%d]\n",__LINE__);

	printk(KERN_NOTICE "hello_init...\n"); // KERN_NOTICE "<5>"

   	PDEBUG("Debug message here\n");

   	return 0;
}

static void hello_exit(void)
{
   	printk("Call %s\n",__func__);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Terence");
MODULE_DESCRIPTION("This is hello debug module");
MODULE_VERSION("V1");
