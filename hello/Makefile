obj-m:=hello.o
KERNELDIR:=/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
clean:
	rm -vrf *.ko.cmd *.o.cmd .tmp_versions .*.cmd *.ko *.mod.c Module.symvers modules.order *.o
