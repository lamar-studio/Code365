##################
# file  : the Makefile for list project
# author: lamar
# date  : 2018/04/07
####


#####  模块配置  #####
TARGET  ?= term_test
SRC = $(shell ls *.c)
OBJ = $(SRC:.c=.o)


#####  编译选项  #####
CC   	= g++
INCLUDE  = -I. 
INCLUDE  += -I/opt/libva2-env-lib/install/include
INCLUDE  += -I/home/rcd/lib_ffmpeg_so/va_2_1
INCLUDE  += -I/home/rcd/libVaapi/include
CFLAGS  += -g -DUSE_FFMPEG_VAAPI -DHAVE_SWSCALE -std=c++11 -Wno-deprecated-declarations
#CFLAGS  += -DUSE_OUTPUT_IMAGE
LDFLAGS += -L/home/rcd/libVaapi/lib -lvaapi
LDFLAGS += -L/home/rcd/cpb_encode/libyuv -lyuv
LDFLAGS += -L/opt/libva2-env-lib/install/lib -lavformat -lavcodec -lavdevice \
           -lswscale -lavutil -lswresample -lswscale
LDFLAGS += -l:libjpeg.a -l:libx264.a
LDFLAGS += -lz -pthread -lm -ldl
LDFLAGS += -lX11 -lXext
LDFLAGS += -lva-x11 -lva-drm -lva


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
	$(QUIET_LINK) $(CC) -o $@ $(OBJ) $(INCLUDE) $(LDFLAGS) 

%.o: %.c
	$(QUIET_CC) $(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE)

clean:
	$(RM) $(OBJ) $(TARGET)
	$(CLEAN_OK)






