#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>

int *reg;

static int mmio_init(void)
{
    printk("mmio driver installed.\n");

    reg = ioremap(0x10000000, 4);
    if (reg) {
        printk("addr  = [%x]\n", reg);
        printk("value = [%x]\n", *reg);
    }

    return 0;
}

static void mmio_exit(void)
{
    iounmap(reg);
    printk("mmio driver removed.\n");
}

module_init(mmio_init);
module_exit(mmio_exit);

MODULE_LICENSE("Dual BSD/GPL"); 
MODULE_AUTHOR("Terence");
MODULE_DESCRIPTION("This is mmio  module");
MODULE_VERSION("V1");
