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

#include "jpg_bmp.h"

#define JPEG_QUALITY     100      /* picture quality */
#define FILENAMELEN      30
#define IMG_W            1920
#define IMG_H            1080
#define IMG_COLOR        3        /* or 1 for GRACYSCALE images */
#define IMG_COLOR_SPACE  JCS_RGB  /* or JCS_GRAYSCALE for grayscale images */

static FILE *input_file;
static FILE *output_file;

static void write_bmp_header(j_decompress_ptr cinfo)
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

	//init the bmp para
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

    //add the platte to gray image
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

static void write_bmp_data(j_decompress_ptr cinfo, unsigned char *src_buff)
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

    //the bmp format against to jpg, backward writing
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
        // write one line data
		fwrite(dst_width_buff, bufwidth, 1, output_file);
	}

	free(dst_width_buff);
}

static void analyse_jpeg(struct term *t)
{
    struct timeval start;
    struct timeval end;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
    long delta_time;
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

	fprintf(t->log, "\twidth:\t%d \n", cinfo.output_width);
	fprintf(t->log, "\theight:\t%d \n", cinfo.output_height);
	fprintf(t->log, "\tdepth:\t%d \n", cinfo.num_components);

	src_buff = (unsigned char *) malloc(width * height * depth);
	memset(src_buff, 0, sizeof(unsigned char) * width * height * depth);

	point = src_buff;
    gettimeofday(&start, NULL);
	while (cinfo.output_scanline < height) {
		jpeg_read_scanlines(&cinfo, &point, 1);
        //read the raw data to buffer
		point += width * depth;
	}
    gettimeofday(&end, NULL);
    delta_time = (end.tv_sec* 1000000 + end.tv_usec) -
                 (start.tv_sec * 1000000 + start.tv_usec);

    fprintf(t->log, "\tTest file:\t%s to %s\n", t->jpg_de.jpg_file,
            t->jpg_de.bmp_file);
    fprintf(t->log, "\tJPEG Decode time:\t%ldus\n", delta_time);

	write_bmp_header(&cinfo);
	write_bmp_data(&cinfo, src_buff);

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(src_buff);
}


int process_bmp_jpg(struct term *t)
{
    fprintf(t->log, "process_bmp_jpg===================================enter\n");
    fprintf(t->log, "Bmp2Jpg Result: \n");
    FILE *fd;
    int ret;
    unsigned char *data;
    long sizeImage;
    int depth = 3;
    JSAMPROW *row_pointer;
    long rgb_index = 0;
    int i = 0;
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    struct timeval start;
    struct timeval end;
    long delta_time;
    FILE *outfile;

    // read bmp image data
    sizeImage = IMG_W*IMG_H*IMG_COLOR;
    data = (unsigned char*)malloc(sizeImage);
    fd = fopen(t->jpg_en.bmp_file, "rb");
    if (!fd) {
        fprintf(t->log, "ERROR1: Can not open the image.\n");
        free(data);
        return -1;
    }

    // ignore the header message
    fseek(fd, 54, SEEK_SET);
    ret = fread(data, sizeof(unsigned char)*sizeImage, 1, fd);
    if (ret == 0) {
        if (ferror(fd)) {
            fprintf(t->log, "ERROR: Can not read the pixel data.\n");
            free(data);
            fclose(fd);
            return -1;
        }
    }

    // Convert BMP to JPG
    if ((outfile = fopen(t->jpg_en.jpg_file, "wb")) == NULL) {
        fprintf(t->log, "can't open %s\n", t->jpg_en.jpg_file);
        free(data);
        fclose(fd);
        return -1;
    }

    // Now we can initialize the JPEG compression object.
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = IMG_W;             // image width and height
    cinfo.image_height = IMG_H;
    cinfo.input_components = IMG_COLOR;    // pixels of color components per pixel
    cinfo.in_color_space = IMG_COLOR_SPACE;// colorspace of input image
    jpeg_set_defaults(&cinfo);

    // Now you can set any non-default parameters you wish to.
    // Here we just illustrate the use of quality (quantization table) scaling:
    jpeg_set_quality(&cinfo, JPEG_QUALITY, TRUE); // limit to baseline-JPEG values
    jpeg_start_compress(&cinfo, TRUE);

    // write once
    int j = 0;
    unsigned char *lineData = (unsigned char*)malloc(IMG_W*IMG_COLOR);
    row_pointer = (unsigned char**)malloc(IMG_H*IMG_W*IMG_COLOR);
    char *line[IMG_H];
    for (i = 0; i < IMG_H; i++) {
        unsigned char * lineData = NULL;
        lineData = (unsigned char*)malloc(IMG_W*IMG_COLOR);
        line[i] = (char*)lineData;
        for (j = 0; j < IMG_W; j++) {
            lineData[j*3+2] = data[rgb_index];
            rgb_index ++;
            lineData[j*3+1] = data[rgb_index];
            rgb_index ++;
            lineData[j*3+0] = data[rgb_index];
            rgb_index ++;
        }
        row_pointer[IMG_H-i-1] = lineData;
    }
    gettimeofday(&start, NULL);
    jpeg_write_scanlines(&cinfo, row_pointer, IMG_H);
    gettimeofday(&end, NULL);
    delta_time = (end.tv_sec* 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
    fprintf(t->log, "\tTest file:\t%s to %s\n", t->jpg_en.bmp_file, t->jpg_en.jpg_file);
    fprintf(t->log, "\tJPEG Encode time:\t%ldus\n", delta_time);

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    for (i=0; i < IMG_H; i++) {
        free(line[i]);
    }
    fprintf(t->log, "process_bmp_jpg===================================out\n");

    free(row_pointer);
    free(data);
    fclose(fd);
    fclose(outfile);

    return 0;
}

int process_jpg_bmp(struct term *t)
{
    fprintf(t->log, "process_jpg_bmp===================================enter\n");
    fprintf(t->log, "Jpg2Bmp Result:\"%s\" Covert to \"%s\"\n",
            t->jpg_de.jpg_file, t->jpg_de.bmp_file);

	input_file = fopen(t->jpg_de.jpg_file, "rb");
	if (input_file == NULL) {
		return -1;
	}

	output_file = fopen(t->jpg_de.bmp_file, "wb");
	if (output_file == NULL) {
		fclose(input_file);
		return -1;
	}
	analyse_jpeg(t);
    fprintf(t->log, "process_jpg_bmp===================================out\n");

	fclose(input_file);
	fclose(output_file);

    return 0;
}







