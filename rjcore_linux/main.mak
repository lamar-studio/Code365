#
# IDV client Top Makefile variables definitions.
# Modified area: add new modules here.
#

# Client header files.
# RCLIB_MPATH :=
# ADD_CFLAGS += $(patsubst %,-I$(top_srcdir)/%,$(RCLIB_MPATH))

# Modules common extra FLAGS.
EXTRA_CFLAGS :=
EXTRA_LDFLAGS :=

# Extra Libs to be installed into RCD system.
#extra_alibs := libzint.a
#extra_solibs := libwpa_client.so

# All the target applications to be compiled.
target_app := interfaces/bin 

# All the target libraries to be compiled.
target_lib := interfaces 

# All the system applications & libs following system ISO.
extra_syslibs :=
target_sys := 
target_syslib :=

# target sample:
# target_app += sample
# target_lib += sample/libhello
