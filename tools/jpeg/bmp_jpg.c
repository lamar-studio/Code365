/*
 * Use libjpeg convert a bmp file to bmp file.
 *
 * Usage:
 * gcc -o bmp_jpg bmp_jpg.c -ljpeg
 * ./bmp_jpg test.bmp test.jpg
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

#define JPEG_QUALITY     100    // picture quality
#define FILENAMELEN     30
#define IMG_W            1920
#define IMG_H            1080
#define IMG_COLOR        3        /* or 1 for GRACYSCALE images */
#define IMG_COLOR_SPACE  JCS_RGB  /* or JCS_GRAYSCALE for grayscale images */

int Bmp2Jpg(const char *bmp_file, const char *jeg_file)
{
    FILE *fd;
    int ret;
    unsigned char *data;
    long sizeImage;
    int depth = 3;
    JSAMPROW * row_pointer;
    long rgb_index = 0;
    int i = 0;
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    struct timeval start;
    struct timeval end;
    long delta_time;
    FILE *outfile;

    // Read bmp image data
    sizeImage = IMG_W*IMG_H*IMG_COLOR;
    data = (unsigned char*)malloc(sizeImage);
    fd = fopen(bmp_file, "rb");
    if (!fd) {
        printf("ERROR1: Can not open the image.\n");
        free(data);
        return -1;
    }

    fseek(fd, 54, SEEK_SET);   // ignore the header message
    ret = fread(data, sizeof(unsigned char)*sizeImage, 1, fd);
    if (ret == 0) {
        if (ferror(fd)) {
            printf("\nERROR2: Can not read the pixel data.\n");
            free(data);
            fclose(fd);
            return -1;
        }
    }

    // Convert BMP to JPG
    if ((outfile = fopen(jeg_file, "wb")) == NULL) {
        fprintf(stderr, "can't open %s\n", jeg_file);
        return -1;
    }

    // Now we can initialize the JPEG compression object.
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = IMG_W;         // image width and height
    cinfo.image_height = IMG_H;
    cinfo.input_components = IMG_COLOR;    // pixels of color components per pixel
    cinfo.in_color_space = IMG_COLOR_SPACE;    // colorspace of input image
    jpeg_set_defaults(&cinfo);

    // Now you can set any non-default parameters you wish to.
    // Here we just illustrate the use of quality (quantization table) scaling:
    jpeg_set_quality(&cinfo, JPEG_QUALITY, TRUE); // limit to baseline-JPEG values
    jpeg_start_compress(&cinfo, TRUE);

    //“ª¥Œ–¥»Î
    int j = 0;
    unsigned char *lineData = malloc(IMG_W*IMG_COLOR);
    row_pointer = malloc(IMG_H*IMG_W*IMG_COLOR);
    char *line[IMG_H];

    for (i = 0; i < IMG_H; i++) {
        unsigned char * lineData = NULL;
        lineData = malloc(IMG_W*IMG_COLOR);
        line[i] = lineData;
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
    printf("JPEG Encode:\t%ldus\n", delta_time);

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    printf("jpeg_finish_compress.\n");

    for (i=0; i < IMG_H; i++) {
        free(line[i]);
    }

    free(row_pointer);
    free(data);
    fclose(fd);
    fclose(outfile);

    return 0;
}

int main(int argc,char*argv[]) {

    char bmpfile[FILENAMELEN] = {0};
    char jpgfile[FILENAMELEN] = {0};

    if(argc != 2) {
        printf("Parameters Error!\n");
        printf("Usage:\n");
        printf("\tbmp2jpg <bmpfilenameWithNoSuffix> \n");
        printf("\tbmp2jpg test\n");
        return -1;
    }

    sprintf(bmpfile,"%s.bmp",argv[1]);
    sprintf(jpgfile,"%s.jpg",argv[1]);

    printf("BMP FILE NAME:%s\n",bmpfile);
    printf("JPG FILE NAME:%s\n",jpgfile);

    printf("Start BMP 2 JPG Convert...\n");
    Bmp2Jpg(bmpfile, jpgfile);
    printf("bmp 2 jpg conversion done!\n");
    return 0;

}

