
#ifndef __COMMON_H_
#define __COMMON_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/time.h>

#define TERM_UTIL_VERSION_STR   "1.0.0"
#define NFS_DIR                 "/home/linzaorong/code/nfs/term-autotest"
#define NFS_SERVER              ((char*)"172.28.108.202")
#define NFS_MOUNT               ((char*)"/mnt/")
#define RESULT_PATH             ((char*)"result")
#define RESULT_BMP_FILE         ((char*)"./result/res.bmp")
#define RESULT_JPEG_FILE        ((char*)"./result/res.jpeg")
#define RESULT_S_264_FILE       ((char*)"./result/softout.264")
#define ORI_YUV_FILE            ((char*)"/mnt/1920x1080.yuv")
#define ORI_NV12_FILE           ((char*)"/mnt/1920x1080_nv12.yuv")
#define ORI_VIDEO_FILE          ((char*)"/mnt/out.264")
#define ORI_BMP_FILE            ((char*)"/mnt/test_1920x1080.bmp")
#define ORI_JPEG_FILE           ((char*)"/mnt/test_1920x1080_q100.jpg")

enum {
    OPT_VIDEO_SOFT_ENCODE = 1,
    OPT_VIDEO_SOFT_DECODE,
    OPT_VIDEO_HARD_ENCODE,
    OPT_VIDEO_HARD_DECODE,
    OPT_JPEG_ENCODE,
    OPT_JPEG_DECODE,
    OPT_LOG,
};

struct jpeg {
    char *jpg_file;
    char *bmp_file;
};

struct s_codec {
    int width;
    int height;
    int frame_num;
    char *in_file;
    char *out_file;
};

struct term {
    struct s_codec s_en;    /* soft encode info */
    char *video_file;       /* soft decode info */
    char *yuv_file_hard;    /* hard encode info */
    char *video_file_hard;  /* hard decode info */
    struct jpeg jpg_en;     /* jpg encode info */
    struct jpeg jpg_de;     /* jpg decode info */
    char *logarg;           /* path name of log file */
    FILE *log;              /* the fd of log file */
    int pi_flag;            /* the flag of cpu test */
};


#endif


