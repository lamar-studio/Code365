#
# Module Makefile variables definition.
# Modified area: add module files here to do compiling.
#

#
# Module app name
# APP output file name, that will installed into system bin dir.
#
APP_TARGET = rjcore_linux

# Module extra FLAGS.
EXTRA_CFLAGS :=
EXTRA_LDFLAGS :=

# options only for cpp compiling.
override ADD_CPPFLAGS += -std=c++11 -Wall -finline-functions

# Module include path, separate with while space.
# e.g.
# $(APP_TOPDIR)/include
#
ADD_INCLUDE = ./inc ../inc ../../inc

# specify link libs.
ADD_LIB = -lpthread 

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

UTILS_PATH=../utils
NETWORK_PATH=../core/network
WIFI_PATH=../core/wifi

SRC := $(wildcard ./*.cpp)

root-objs := $(SRC:%.cpp=%.o)
#root-objs := main.o


# subdir to be compiled.
# e.g.
# rootdir-y := cli
#
#rootdir-y := ./network_daemon/

# rootobj-y: the lingking targets of subdirs.
# if subdir is xxx, then we add: $(_PDIR)/xxx/_sub_xxx.o
# e.g.
# rootobj-y := $(_PDIR)/cli/_sub_cli.o
#
#rootobj-y := $(_PDIR)/core/bin/network_daemon/_sub_network.o
