
#ifndef __SOFT_CODEC_H_
#define __SOFT_CODEC_H_


#ifdef __cplusplus
extern "C" {
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>      /* BitmapOpenFailed, etc. */
#include <sys/shm.h>
#include <X11/extensions/XShm.h>

#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libavutil/parseutils.h>

#ifdef __cplusplus
}
#endif


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "common.h"

int soft_encode_h264(struct term *t);
int soft_decode_yuv(struct term *t);

#endif


