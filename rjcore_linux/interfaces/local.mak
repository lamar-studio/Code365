#
# Module Makefile variables definition.
# Modified area: add module files here to do compiling.
#

#
# Module app name
# APP output file name, that will installed into system bin dir.
#
#LIB_NAME = sysrjcore_vdis1_linux
#LIB_VERSION = 1.0.0

# Module extra FLAGS.
EXTRA_CFLAGS :=
EXTRA_LDFLAGS :=

# options only for cpp compiling.
override ADD_CPPFLAGS += -std=c++11 -ggdb -Wall -finline-functions

# Module include path, separate with while space.
# e.g.
# $(APP_TOPDIR)/include
#
ADD_INCLUDE = ../include

# specify link libs.
ADD_LIB = -lpthread -lpulse 

#
# extra intstall target.
#
.PHONY: install_extra
install_extra:
	@echo "install extra files..."

.PHONY: uninstall_extra
uninstall_extra:
	@echo "uninstall extra files..."

# rootobjs: specify the files of current dir to be compiled.
# e.g.
# root-objs := hello.o
#
SRC := $(wildcard *.c)
root-objs := $(SRC:%.c=%.o)

# subdir to be compiled.
# e.g.
# rootdir-y := cli
#
rootdir-y := control product application common network bt utils sysmisc

# rootobj-y: the lingking targets of subdirs.
# if subdir is xxx, then we add: $(_PDIR)/xxx/_sub_xxx.o
# e.g.
# rootobj-y := $(_PDIR)/cli/_sub_cli.o
#
rootobj-y += $(_PDIR)/control/_sub_control.o
rootobj-y += $(_PDIR)/product/_sub_product.o
rootobj-y += $(_PDIR)/application/_sub_application.o
rootobj-y += $(_PDIR)/common/_sub_common.o
rootobj-y += $(_PDIR)/network/_sub_network.o
rootobj-y += $(_PDIR)/bt/_sub_bt.o
rootobj-y += $(_PDIR)/utils/_sub_utils.o
rootobj-y += $(_PDIR)/sysmisc/_sub_sysmisc.o

jnadir-y += jna
