#
# env.mak
# Copyright (C)2017 Ruijie Network Inc. All rights reserved.
# Author: yejx@ruijie.com.cn
#
# IDV Client compiling environment definition.
#

# Tools.
CC      := gcc
CPP     := g++
LD      := ld
AR      := ar
NM      := nm
OBJDUMP := objdump
JAVA    := java
JAVAC   := javac
JAR     := jar

export CC CPP LD AR NM OBJDUMP JAVA JAVAC JAR

common_headerdir := $(top_srcdir)/include
extra_libdir := $(top_srcdir)/extralib

install_dir := /usr/local
install_sysconf := /etc
install_bindir := $(install_dir)/bin
install_libdir := $(install_dir)/lib
install_incdir := $(install_dir)/include

install_extradir := $(install_sysconf)/rainsys
install_resdir := $(install_extradir)/res
install_confdir := $(install_extradir)
install_scriptdir := $(install_extradir)/scripts

# Basic compiling options.
BUILD_CFLAGS := -Os -Wall -g
BUILD_LDFLAGS := -Os -g
