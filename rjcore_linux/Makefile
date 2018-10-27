#
# Makefile
# Copyright (C)2017 Ruijie Network Inc. All rights reserved.
# Author: yejx@ruijie.com.cn
#
# IDV Client top Makefile
#

# Top directory definition.
top_srcdir := $(shell pwd)

# ALL target.
all: do-it-all

# common header files.
MPATH := include
ADD_CFLAGS := $(patsubst %,-I$(top_srcdir)/%,$(MPATH))

# add options for cpp compiling only.
ADD_CPPFLAGS :=

# Build output directory.
OUTPUT_DIR := $(top_srcdir)/build
BUILD_LIBDIR := $(OUTPUT_DIR)/rclib
BUILD_JARDIR := $(OUTPUT_DIR)/rcjar

export top_srcdir OUTPUT_DIR BUILD_LIBDIR ADD_CFLAGS ADD_CPPFLAGS BUILD_JARDIR
unexport target_dir target_sysdir target_all

include $(top_srcdir)/env.mak

# ================ Modified Area START ================ #

include $(top_srcdir)/main.mak
include $(top_srcdir)/platform.mak

override CFLAGS := $(BUILD_CFLAGS) $(ADD_CFLAGS) $(EXTRA_CFLAGS)
override LDFLAGS := $(BUILD_LDFLAGS) $(EXTRA_LDFLAGS)

# ================= Modified Area END ================= #

# rjsyscore modules.
target_app := $(wildcard $(target_app))
target_lib := $(wildcard $(target_lib))
target_dir := $(target_app) $(target_lib)

# system modules on ISO.
target_sys := $(wildcard $(target_sys))
target_syslib := $(wildcard $(target_syslib))
target_sysdir := $(target_sys) $(target_syslib)

target_all := $(target_dir) $(target_sysdir)

LOCAL_LDFLAGS := -L$(BUILD_LIBDIR) -L$(extra_libdir)
SYS_LDFLAGS := -L$(install_dir)/lib
# local lib precedes sys lib.
override LDFLAGS += $(LOCAL_LDFLAGS) $(SYS_LDFLAGS)

#
# Remove unused code, to reduce binary file size.
#
#override CFLAGS += -fdata-sections -ffunction-sections
#override LDFLAGS += --gc-sections

export CFLAGS LDFLAGS

.PHONY: do-it-all
do-it-all: build

.PHONY: build
build:
	$(MAKE) build-rcd

# Libraries must be compiled first.
.PHONY: build-rcd
build-rcd: build-lib
	$(MAKE) build-app

.PHONY: build-app
build-app: $(patsubst %, _dir_%, $(target_app))
$(patsubst %, _dir_%, $(target_app)):
	@echo "Compiling $(top_srcdir)/$(patsubst _dir_%,%,$@)"
	$(MAKE) CFLAGS="$(CFLAGS)" -C $(top_srcdir)/$(patsubst _dir_%,%,$@) _PDIR="$(OUTPUT_DIR)/$(patsubst _dir_%,%,$@)"
	@echo "Compiled OK! find output in $(OUTPUT_DIR)/$(patsubst _dir_%,%,$@)"
	@echo

.PHONY: build-lib
build-lib: $(patsubst %, _dir_%, $(target_lib))
$(patsubst %, _dir_%, $(target_lib)):
	@echo "Compiling library $(top_srcdir)/$(patsubst _dir_%,%,$@)"
	$(MAKE) -C $(top_srcdir)/$(patsubst _dir_%,%,$@) _PDIR="$(OUTPUT_DIR)/$(patsubst _dir_%,%,$@)"
	@echo "Compiled OK! find output in $(OUTPUT_DIR)/$(patsubst _dir_%,%,$@)"
	@echo

clean: $(patsubst %, _c_%, $(target_dir))
$(patsubst %, _c_%, $(target_dir)):
	$(MAKE) -C $(top_srcdir)/$(patsubst _c_%,%,$@) _PDIR="$(OUTPUT_DIR)/$(patsubst _c_%,%,$@)" clean
	@rm -rf $(OUTPUT_DIR)

install: $(patsubst %, _inst_%, $(target_dir)) install-extralib
$(patsubst %, _inst_%, $(target_dir)):
	$(MAKE) -C $(top_srcdir)/$(patsubst _inst_%,%,$@) _PDIR="$(OUTPUT_DIR)/$(patsubst _inst_%,%,$@)" install

uninstall: $(patsubst %, _uninst_%, $(target_dir)) uninstall-extralib
$(patsubst %, _uninst_%, $(target_dir)):
	$(MAKE) -C $(top_srcdir)/$(patsubst _uninst_%,%,$@) _PDIR="$(OUTPUT_DIR)/$(patsubst _uninst_%,%,$@)" uninstall

.PHONY: install-extralib
install-extralib: $(extra_solibs)
$(extra_solibs):
	cp -f $(extra_libdir)/$@ $(install_libdir)
	if ! echo $@ | grep -q '.so$$'; then \
		rm -f $(install_libdir)/$${@%.so*}.so; \
		cd $(install_libdir) && ln -s $$@ $${@%.so*}.so; fi

.PHONY: uninstall-extralib
uninstall-extralib: $(patsubst %, _uninst_%, $(extra_solibs))
$(patsubst %, _uninst_%, $(extra_solibs)):
	rm -f $(install_libdir)/$(patsubst _uninst_%,%,$@)
	if ! echo $@ | grep -q '.so$$'; then \
		rm -f $(install_libdir)/$${@%.so*}.so; fi

# We can do only one module target.
$(target_all): dummy
	$(MAKE) $(patsubst %,_dir_%,$@)
	@rm -f .dummy

.PHONY: dummy
dummy:
	@touch .dummy
