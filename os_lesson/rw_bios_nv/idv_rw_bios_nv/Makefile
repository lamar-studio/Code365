BIN=rw_bios_nv_user
obj-m := rw_bios_nv.o

KERNELBUILD :=/lib/modules/$(shell uname -r)/build

default:rw_bios_nv.ko $(BIN)

rw_bios_nv.ko:
	 make -C $(KERNELBUILD) M=$(shell pwd) modules

#CFLAGS =  -g

rw_bios_nv_user:rw_bios_nv_user.o
	cc -o $@ $+

clean:
	@echo CLEAN
	@rm -rf *.o $(BIN)
	@rm -rf *.o *.ko *.mod.c .*.cmd *.markers *.order *.symvers .tmp_versions 
