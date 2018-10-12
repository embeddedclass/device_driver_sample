
#KERNELDIR = ~/Linux_Device_Driver_FA/linux_3.5.0_tiny4412_new/
#INSTALLDIR = ~/Linux_Device_Driver_FA/root_mkfs/lib/modules/3.5.0-FriendlyARM/kernel/
PWD := $(shell pwd)

obj-m := led.o 

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	sudo cp led.ko $(INSTALLDIR)
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers

.PHONY: modules install clean


