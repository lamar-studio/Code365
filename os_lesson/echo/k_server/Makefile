
TARGET:= server
obj-m := $(TARGET).o


KERNELDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd) 

.PHONY: build install clean

all: build

build: $(OBJ)
	$(MAKE) $(EXTRA_CFLAGS) -C $(KERNELDIR) M=$(PWD) modules

install: build
	mkdir -p $(install_sbin_dir)
	cp -a $(TARGET).ko $(install_sbin_dir)
	$Q$(MAKE) cp_header
	
clean:
	@echo CLEAN
	@rm -rf *.o *.ko *.mod.c *.markers *.order *.symvers .*.cmd .tmp_versions


