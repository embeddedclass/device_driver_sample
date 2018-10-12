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
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/ioport.h>
#include <asm/io.h>

#define DRIVER_NAME "MODULE_LED"
#define EXY_4412_GPIO_BASE 0x11000000
#define EXY_4412_GPM4CON (EXY_4412_GPIO_BASE + 0x02E0)
#define EXY_4412_GPM4DAT (EXY_4412_GPIO_BASE + 0x02E4)

void __iomem *regcon, *regdat;

static int led_init(void)
{
    if (!request_mem_region(EXY_4412_GPM4CON, 0x02E8, DRIVER_NAME)) {
        printk("Request mem region failed\n");
        return -EBUSY;
    }

    regcon = ioremap(EXY_4412_GPM4CON, 4);
    regdat = ioremap(EXY_4412_GPM4DAT, 1);

    writew((readw(regcon) | 0x1111), regcon);
    writeb((readb(regdat) | 0xF), regdat);

    return 0;
}

static void led_cleanup(void)
{
    printk("Led driver removed\n");
    writeb((readb(regdat) & ~0xF), regdat);
    iounmap(regcon);
    iounmap(regdat);
    release_mem_region(EXY_4412_GPM4CON, 0x02E8);
}

module_init(led_init);
module_exit(led_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Slash");
MODULE_DESCRIPTION("This is LED module");
