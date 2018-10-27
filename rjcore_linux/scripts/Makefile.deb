#
# common libraries main Makefile
#
include $(top_srcdir)/env.mak
include $(top_srcdir)/platform.mak

#
# ALL target
#
all: do-it-all jar

# LIB root dir.
LIBTOPDIR :=$(shell pwd)

# LIB compiling options.
LIB_CFLAGS := -fPIC -shared -fstrict-aliasing -Wstrict-aliasing
override CFLAGS += $(LIB_CFLAGS)
override LDFLAGS += $(LIB_CFLAGS)

# ================ Modified Area START ================ #

include $(LIBTOPDIR)/local.mak

override CFLAGS += $(EXTRA_CFLAGS)
override LDFLAGS += $(EXTRA_LDFLAGS)

# ================= Modified Area END ================= #

ifdef QUIET
Q:=@
else
Q:=
endif

#
# target LIB output path.
#
LIB_FILE = lib$(LIB_NAME).so.$(LIB_VERSION)
LIB_LINK = libsysrjcore_linux.so
LIB_OUTPUT = $(_PDIR)/$(LIB_FILE)
JAVA_SRC := $(wildcard ./$(jnadir-y)/*.java)
JAR_FILE = $(JAR_NAME)-$(LIB_VERSION).jar
JAR_LINK = sysrjcore_linux.jar
JAR_OUTPUT = $(top_srcdir)/build/rcjar/$(JAR_FILE)
JAR_LIB := $(LIBTOPDIR)/$(jnadir-y)/lib
JAR_MF := $(LIBTOPDIR)/$(jnadir-y)/META-INF/MANIFEST.MF
#LIB_LINK = lib$(LIB_NAME).so

override SUBDIRS := $(rootdir-y)
override ALL_OBJS := $(rootobj-y) $(addprefix $(_PDIR)/,$(root-objs))
override CFLAGS += $(patsubst %,-I%,$(ADD_INCLUDE))

.PHONY: do-it-all
do-it-all: install-build

.PHONY: install-build
install-build: build
	mkdir -p $(BUILD_LIBDIR)
	cp -f $(LIB_OUTPUT) $(BUILD_LIBDIR)
	rm -f $(BUILD_LIBDIR)/$(LIB_LINK)
	ln -s $(BUILD_LIBDIR)/$(LIB_FILE) $(BUILD_LIBDIR)/$(LIB_LINK)

build: root_targets header
	$Q$(MAKE) ngsa

.PHONY: header
header: $(patsubst %, _h_%, $(LIB_HEADERS))
$(patsubst %, _h_%, $(LIB_HEADERS)):
	mkdir -p $(common_headerdir)/$(LIB_NAME)
	rm -f $(common_headerdir)/$(LIB_NAME)/$(patsubst _h_%,%,$@)
	ln -s $(LIBTOPDIR)/$(patsubst _h_%,%,$@) $(common_headerdir)/$(LIB_NAME)/$(patsubst _h_%,%,$@)

# generate target lib file.
ngsa: ngsasubdirs
	$(CPP) $(LDFLAGS) $(ALL_OBJS) $(ADD_LIB) -o $(LIB_OUTPUT)

.PHONY: make-dir
make-dir:
	@mkdir -p $(_PDIR)

root_targets: make-dir
	$Q$(MAKE) root-compile

#
# subdirs: traversal to do compiling.
#
SUBDIRS := $(wildcard $(SUBDIRS))
ngsasubdirs: $(patsubst %, _dir_%, $(SUBDIRS))
$(patsubst %, _dir_%, $(SUBDIRS)):
	$(MAKE) CFLAGS="$(CFLAGS)" -C $(patsubst _dir_%, %, $@) _PDIR="$(_PDIR)/$(patsubst _dir_%,%,$@)"

# depend files.
root-deps := $(patsubst %.o,%.d,$(root-objs))

root-compile: $(addprefix $(_PDIR)/,$(root-deps)) $(addprefix $(_PDIR)/,$(root-objs))

$(addprefix $(_PDIR)/,$(root-deps)):
$(addprefix $(_PDIR)/,$(root-objs)):

# generate depend files.
$(_PDIR)/%.d: %.c
	$Q$(top_srcdir)/scripts/mkcdep $< "$(CFLAGS) $(ADD_CFLAGS) $(CFLAGS_$(patsubst $(_PDIR)/%,%,$@))" > $@

$(_PDIR)/%.d: %.cpp
	$Q$(top_srcdir)/scripts/mkcppdep $< "$(CFLAGS) $(ADD_CFLAGS) $(ADD_CPPFLAGS) $(CFLAGS_$(patsubst $(_PDIR)/%,%,$@))" > $@

# contain .d files.
FILES_DEPEND_EXIST := $(wildcard $(addprefix $(_PDIR)/,$(root-deps)))
ifneq ($(FILES_DEPEND_EXIST),)
include $(FILES_DEPEND_EXIST)
endif

# target compiling rules.
$(_PDIR)/%.o: %.c
ifdef QUIET
	@echo "[*] Compiling `pwd`/$<"
endif
	$Q$(CPP) $(CFLAGS) $(ADD_CFLAGS) $(CFLAGS_$(patsubst $(_PDIR)/%,%,$@)) -c $< -o $@
	$Q ( \
	    echo 'ifeq ($(strip $(subst $(comma),:,$(CFLAGS) $(ADD_CFLAGS) $(CFLAGS_$(patsubst $(_PDIR)/%,%,$@)))),$$(strip $$(subst $$(comma),:,$$(CFLAGS) $$(ADD_CFLAGS) $$(CFLAGS_$(patsubst $(_PDIR)/%,%,$@)))))' ; \
	    echo 'FILES_FLAGS_UP_TO_DATE += $@' ; \
	    echo 'endif' \
	) > $(dir $@)/.$(notdir $@).flags

$(_PDIR)/%.o: %.cpp
ifdef QUIET
	@echo "[*] Compiling `pwd`/$<"
endif
	$Q$(CPP) $(CFLAGS) $(ADD_CFLAGS) $(ADD_CPPFLAGS) $(CFLAGS_$(patsubst $(_PDIR)/%,%,$@)) -c $< -o $@
	$Q ( \
	    echo 'ifeq ($(strip $(subst $(comma),:,$(CFLAGS) $(ADD_CFLAGS) $(ADD_CPPFLAGS) $(CFLAGS_$(patsubst $(_PDIR)/%,%,$@)))),$$(strip $$(subst $$(comma),:,$$(CFLAGS) $$(ADD_CFLAGS) $$(ADD_CPPFLAGS) $$(CFLAGS_$(patsubst $(_PDIR)/%,%,$@)))))' ; \
	    echo 'FILES_FLAGS_UP_TO_DATE += $@' ; \
	    echo 'endif' \
	) > $(dir $@)/.$(notdir $@).flags

.PHONY: install
install:
	mkdir -p $(install_libdir)
	cp -f $(LIB_OUTPUT) $(install_libdir)
	cp -f $(JAR_OUTPUT) $(install_libdir)
	cp -f $(JAR_LIB)/jna-5.0.0.jar $(install_libdir)
	rm -f $(install_libdir)/$(LIB_LINK)
	rm -f $(install_libdir)/$(JAR_LINK)
	cd $(install_libdir) && ln -s $(LIB_FILE) $(LIB_LINK) && ln -s $(JAR_FILE) $(JAR_LINK)
	$Q$(MAKE) install_extra

.PHONY: uninstall
uninstall:
	rm -f $(install_libdir)/$(LIB_FILE) $(install_libdir)/$(LIB_LINK)
	rm -f $(install_libdir)/$(JAR_FILE) $(install_libdir)/$(JAR_LINK)
	rm -f $(install_libdir)/jna-5.0.0.jar
	$Q$(MAKE) uninstall_extra

.PHONY: clean
clean:
	rm -rf $(_PDIR)
	rm -rf $(common_headerdir)/$(LIB_NAME)
	rm -f $(BUILD_LIBDIR)/$(LIB_FILE) $(BUILD_LIBDIR)/$(LIB_LINK)
	rm -f $(BUILD_JARDIR)/$(JAR_FILE) $(BUILD_JARDIR)/$(JAR_LINK)

.PHONY: jar 
javac: preinit
	$(JAVAC) $(JAVA_SRC) -d $(BUILD_JARDIR)
preinit:
	mkdir -p $(BUILD_JARDIR) 
	#cp -rf $(JAR_LIB) $(BUILD_JARDIR)
jar: javac
	#$(JAR) -cvfm $(BUILD_JARDIR)/$(JAR_FILE) $(JAR_MF) -C $(BUILD_JARDIR) ./lib -C $(BUILD_JARDIR) ./com 
	$(JAR) -cvfm $(BUILD_JARDIR)/$(JAR_FILE) $(JAR_MF) -C $(BUILD_JARDIR) ./com 
