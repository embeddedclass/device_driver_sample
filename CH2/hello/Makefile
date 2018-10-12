

#KERNELDIR =
#INSTALLDIR =

obj-m := hello.o 

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)

install:
	cp -a hello.ko $(INSTALLDIR)
	scp ./hello.ko slash@192.168.0.20:/tmp
clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions modules.order  Module.symvers

.PHONY: modules install clean
