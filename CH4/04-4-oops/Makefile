
KERNELDIR = ~/project/embedded/Linux_Driver/arm_kernel
PWD := $(shell pwd)
INSTALLDIR = ~/project/embedded/Linux_Driver/arm_linux_drv_inst

obj-m := oops_test.o 

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	sudo cp oops_test.ko $(INSTALLDIR)
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers

.PHONY: modules install clean
