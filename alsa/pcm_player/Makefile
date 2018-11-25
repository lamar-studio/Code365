##################
# file  : the Makefile for list project
# author: lamar
# date  : 2018/04/07
####


#####  模块配置  #####
TARGET  ?= sound
SRC = $(shell ls *.c)
OBJ = $(SRC:.c=.o)


#####  编译选项  #####
CC   	?= gcc
INCLUDE  = -I. -I./include
CFLAGS  += -fPIC -ansi -pedantic -std=c99 -lasound
SHARE_LIB = -L/usr/lib/alsa/lib

ifndef DEBUG
#用于代码优化
ADDITIONAL_CFLAGS  ?= -O2
else
#加入调试信息,用于GDB调试
ADDITIONAL_CFLAGS  ?= -g
endif

CFLAGS += ${ADDITIONAL_CFLAGS}

# 打印过程信息
QUIET_AR		= @echo "AR	$@";
QUIET_CC		= @echo "CC	$@";
QUIET_LINK		= @echo "LINK	$@";
QUIET_RANLIB	= @echo "RANLIB	$@";


#####  shell工具  #####
RM         ?= rm -f
CLEAN_OK    = @echo "=== clean ok =====";


#####  Makefile编译规则  #####
all: $(TARGET)

$(TARGET): $(OBJ)
	$(QUIET_LINK) $(CC) -o $@ $(OBJ) $(SHARE_LIB) $(CFLAGS)

%.o: %.c
	$(QUIET_CC) $(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE)

clean:
	$(RM) $(OBJ) $(TARGET)
	$(CLEAN_OK)






