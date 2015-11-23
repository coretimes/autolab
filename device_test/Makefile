obj-m += sample_module.o
KDIR:=/lib/modules/`uname -r`/build
PWD:=$(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clear:
	rm -rf *.o *.mod.o *.ko *.mod.c .*.o.cmd .*.mo.o.cmd .*.ko.cmd .tmp.versions
