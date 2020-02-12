
#ifndef __JPG_BMP_H_
#define __JPG_BMP_H_

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

#include "common.h"

#pragma pack(2)
struct bmp_fileheader
{
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReverved1;
	uint16_t bfReverved2;
	uint32_t bfOffBits;
};

struct bmp_infoheader
{
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};


int process_jpg_bmp(struct term *t);
int process_bmp_jpg(struct term *t);


#endif

