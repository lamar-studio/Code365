##################
# file  : the Makefile for list project
# author: lamar
# date  : 2018/04/07
####


#####  ģ������  #####
TARGET  ?= sound
SRC = $(shell ls *.c)
OBJ = $(SRC:.c=.o)


#####  ����ѡ��  #####
CC   	?= gcc
INCLUDE  = -I. -I./include
CFLAGS  += -fPIC -ansi -pedantic -std=c99 -lasound
SHARE_LIB = -L/usr/lib/alsa/lib

ifndef DEBUG
#���ڴ����Ż�
ADDITIONAL_CFLAGS  ?= -O2
else
#���������Ϣ,����GDB����
ADDITIONAL_CFLAGS  ?= -g
endif

CFLAGS += ${ADDITIONAL_CFLAGS}

# ��ӡ������Ϣ
QUIET_AR		= @echo "AR	$@";
QUIET_CC		= @echo "CC	$@";
QUIET_LINK		= @echo "LINK	$@";
QUIET_RANLIB	= @echo "RANLIB	$@";


#####  shell����  #####
RM         ?= rm -f
CLEAN_OK    = @echo "=== clean ok =====";


#####  Makefile�������  #####
all: $(TARGET)

$(TARGET): $(OBJ)
	$(QUIET_LINK) $(CC) -o $@ $(OBJ) $(SHARE_LIB) $(CFLAGS)

%.o: %.c
	$(QUIET_CC) $(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE)

clean:
	$(RM) $(OBJ) $(TARGET)
	$(CLEAN_OK)





