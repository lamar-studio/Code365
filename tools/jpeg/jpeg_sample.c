/*
 * Use libjpeg to read an write jpeg format file.
 *
 * Usage:
 * gcc -o jpeg_sample jpeg_sample.c -ljpeg
 * ./jpeg_sample
 *
 * Depends:
 * sudo apt-get install libjpeg62-dev
 */

#include <stdio.h>
#include <jpeglib.h>
#include <stdlib.h>
#include <sys/time.h>

#define IMG_W            2560
#define IMG_H            1440
#define IMG_COLOR        3        /* or 1 for GRACYSCALE images */
#define IMG_COLOR_SPACE  JCS_RGB  /* or JCS_GRAYSCALE for grayscale images */

unsigned char *raw_image = NULL;

int read_jpeg_file(char *filename)
{
    struct timeval start;
    struct timeval end;
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    JSAMPROW row_pointer[1];

    FILE *infile = fopen(filename, "rb");
    long delta_time;
    unsigned long location = 0;
    int i = 0;
    int ret_start = -1;
    int ret_end = -1;

    if (!infile) {
        printf("Error opening jpeg file %s\n!", filename);
        return -1;
    }

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);

    jpeg_start_decompress(&cinfo);
    printf("width = %d\n", cinfo.output_width);
    printf("height = %d\n", cinfo.output_height);
    printf("components = %d\n", cinfo.num_components);

    //malloc the buffer for bmp data
    raw_image = (unsigned char*)malloc(cinfo.output_width*cinfo.output_height*cinfo.num_components);

    //malloc the buffer for one row data
    row_pointer[0] = (unsigned char *)malloc(cinfo.output_width*cinfo.num_components);

    ret_start = gettimeofday(&start, NULL);
    while(cinfo.output_scanline < cinfo.image_height) {
        jpeg_read_scanlines(&cinfo, row_pointer, 1);
        for( i=0; i<cinfo.image_width*cinfo.num_components; i++) {
            raw_image[location++] = row_pointer[0][i];
        }
    }
    ret_end = gettimeofday(&end, NULL);

    if (ret_start != 0 || ret_end != 0) {
        return -1;
    }
    delta_time = (end.tv_sec* 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
    printf("JPEG Decode:\t%ldus\n", delta_time);

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    free(row_pointer[0]);
    fclose(infile);

    return 0;
}

int write_jpeg_file(char *filename)
{
    struct timeval start;
    struct timeval end;
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    JSAMPROW row_pointer[1];
    FILE *outfile = fopen(filename, "wb");
    long delta_time;
    int ret_start = -1;
    int ret_end = -1;

    if (!outfile) {
        printf("Error opening output jpeg file %s\n!", filename);
        return -1;
    }
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = IMG_W;
    cinfo.image_height = IMG_H;
    cinfo.input_components = IMG_COLOR;
    cinfo.in_color_space = IMG_COLOR_SPACE;

    jpeg_set_defaults(&cinfo);
    jpeg_start_compress(&cinfo, TRUE);

    ret_start = gettimeofday(&start, NULL);
    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = &raw_image[cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    ret_end = gettimeofday(&end, NULL);

    if (ret_start != 0 || ret_end != 0) {
        return -1;
    }
    delta_time = (end.tv_sec* 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec);
    printf("JPEG Encode:\t%ldus\n", delta_time);

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(outfile);

    return 1;
}

int main(int argc, char **argv)
{
    char *infilename = "test.jpg";
    char *outfilename = "out.jpg";

    if (read_jpeg_file(infilename) == 0) {
        if (write_jpeg_file(outfilename) < 0) {
            printf("write_jpeg_file fail");
            return -1;
        }
    } else {
        printf("read_jpeg_file fail");
        return -1;
    }

    return 0;
}


