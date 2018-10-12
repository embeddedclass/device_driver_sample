
#KERNELDIR =
#INSTALLDIR =
PWD := $(shell pwd)


obj-m := misc_driver.o

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	sudo cp misc_driver.ko $(INSTALLDIR)
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers

.PHONY: modules install clean
