obj-m := demo.o

KERNELDIR := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNELDIR) M=`pwd` V=1

.PHONY: ins rm rei clean

clean:
	rm -fr *.ko *.mod.ko *.o *.mod.c Module.symvers .*.cmd .tmp_versions
