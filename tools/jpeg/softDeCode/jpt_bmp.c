/*
 * Use libjpeg to read a jpeg file and save to bmp file.
 *
 * Usage:
 * gcc -o jpt_bmp jpt_bmp.c -ljpeg
 * ./jpt_bmp test.jpg test.bmp
 *
 * Depends:
 * sudo apt-get install libjpeg62-dev
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <jpeglib.h>
#include <sys/time.h>

#pragma pack(2)             //两字节对齐，否则bmp_fileheader会占16Byte
struct bmp_fileheader
{
	uint16_t bfType;        //若不对齐，这个会占4Byte
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

FILE *input_file;
FILE *output_file;

void write_bmp_header(j_decompress_ptr cinfo)
{
	struct bmp_fileheader bfh;
	struct bmp_infoheader bih;

	uint32_t width;
	uint32_t height;
	uint16_t depth;
	uint32_t headersize;
	uint32_t filesize;
	uint32_t bmpsize;

	width = cinfo->output_width;
	height = cinfo->output_height;
	depth = cinfo->output_components;

	if (depth == 1) {
		headersize = 14 + 40 + 256 * 4;
	}

	if (depth == 3) {
		headersize = 14 + 40;
	}
	bmpsize = (((width * depth + 3) / 4) * 4) * height;
	filesize = headersize +  bmpsize;

	memset(&bfh, 0, sizeof(struct bmp_fileheader));
	memset(&bih, 0, sizeof(struct bmp_infoheader));

	//写入比较关键的几个bmp头参数
	bfh.bfType = 0x4D42;
	bfh.bfSize = filesize;
	bfh.bfOffBits = headersize;

	bih.biSize = 40;
	bih.biWidth = width;
	bih.biHeight = height;
	bih.biPlanes = 1;
	bih.biBitCount = (uint16_t) depth * 8;
	bih.biSizeImage = bmpsize;
	bih.biXPelsPerMeter = 0x99c1;
	bih.biYPelsPerMeter = 0x99c1;

	fwrite(&bfh, sizeof(struct bmp_fileheader), 1, output_file);
	fwrite(&bih, sizeof(struct bmp_infoheader), 1, output_file);

    //灰度图像要添加调色板
	if (depth == 1) {
		unsigned char *platte;
		platte = (unsigned char *) malloc(256 * 4);
		unsigned char j = 0;
		int i;
		for (i = 0; i < 1024; i += 4) {
			platte[i] = j;
			platte[i + 1] = j;
			platte[i + 2] = j;
			platte[i + 3] = 0;
			j++;
		}
		fwrite(platte, sizeof(unsigned char) * 1024, 1, output_file);
		free(platte);
	}
}

void write_bmp_data(j_decompress_ptr cinfo, unsigned char *src_buff)
{
	unsigned char *dst_width_buff;
	unsigned char *point;

	uint32_t width;
	uint32_t height;
	uint16_t depth;

	uint32_t bufwidth;
	uint32_t bmpwidth;

	width = cinfo->output_width;
	height = cinfo->output_height;
	depth = cinfo->output_components;

	bmpwidth = width * depth;
	bufwidth = ((bmpwidth + 3) / 4) * 4;

	dst_width_buff = (unsigned char *) malloc(bufwidth);
	memset(dst_width_buff, 0, bufwidth);

    //倒着写数据，bmp格式是倒的，jpg是正的
	point = src_buff + bmpwidth * (height - 1);
	uint32_t i;
	for (i = 0; i < height; i++) {
		uint32_t j;
		for (j = 0; j < width * depth; j += depth) {
            //grayscale process
			if (depth == 1) {
				dst_width_buff[j] = point[j];
			}

            // RGB process
			if (depth == 3) {
				dst_width_buff[j + 2] = point[j + 0];
				dst_width_buff[j + 1] = point[j + 1];
				dst_width_buff[j + 0] = point[j + 2];
			}
		}
		point -= bmpwidth;
		fwrite(dst_width_buff, bufwidth, 1, output_file);    //一次写一行
	}

	free(dst_width_buff);
}

void analyse_jpeg()
{
    struct timeval start;
    struct timeval end;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
    long delta_time;
    int ret_start = -1;
    int ret_end = -1;
	unsigned char *src_buff;
	unsigned char *point;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, input_file);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	uint32_t width = cinfo.output_width;
	uint32_t height = cinfo.output_height;
	uint16_t depth = cinfo.output_components;

	printf("width:\t%d\n", cinfo.output_width);
	printf("height:\t%d\n", cinfo.output_height);
	printf("depth:\t%d\n", cinfo.num_components);

	src_buff = (unsigned char *) malloc(width * height * depth);
	memset(src_buff, 0, sizeof(unsigned char) * width * height * depth);

	point = src_buff;
    ret_start = gettimeofday(&start, NULL);
	while (cinfo.output_scanline < height) {
		jpeg_read_scanlines(&cinfo, &point, 1);
		point += width * depth;                    //read the raw data to buffer
	}
    ret_end = gettimeofday(&end, NULL);

    if (ret_start != 0 || ret_end != 0) {
        return;
    }
    delta_time = (end.tv_sec* 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
    printf("JPEG Decode:\t%ldus\n", delta_time);

	write_bmp_header(&cinfo);            //写bmp文件头
	write_bmp_data(&cinfo, src_buff);    //写bmp像素数据

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(src_buff);
}

int main(int argc, const char *argv[])
{
	if (argc < 3) {
		printf("%s jpg bmp\n", argv[0]);
		return -1;
	}
    printf("\"%s\" Covert to \"%s\"\n", argv[1], argv[2]);
	input_file = fopen(argv[1], "rb");
	if (input_file == NULL) {
		return -1;
	}
	output_file = fopen(argv[2], "wb");
	if (output_file == NULL) {
		fclose(input_file);
		return -1;
	}

	analyse_jpeg();
    printf("Creat %s finish!\n", argv[2]);

	fclose(input_file);
	fclose(output_file);

	return 0;
}
