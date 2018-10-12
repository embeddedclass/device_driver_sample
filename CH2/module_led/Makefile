
#KERNELDIR =
#INSTALLDIR =

PWD := $(shell pwd)

obj-m := module_led.o 

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	sudo cp led.ko $(INSTALLDIR)

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers

.PHONY: modules install clean


