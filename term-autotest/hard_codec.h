
#ifndef __HARD_CODEC_H_
#define __HARD_CODEC_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "common.h"


int hard_encode_h264(struct term *t);
int hard_encode_h265(struct term *t);
int hard_decode_yuv(struct term *t);

#endif

