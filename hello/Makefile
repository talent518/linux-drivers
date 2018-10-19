obj-m:=hello.o
KERNELDIR:=/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)
KCFLAGS:=-DTIMEZONE=\"$(shell date '+%:z')\"
modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) KCFLAGS='$(KCFLAGS)' modules
modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
