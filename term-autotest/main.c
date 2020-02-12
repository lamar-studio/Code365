#include <getopt.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "common.h"
#include "utils.h"
#include "jpg_bmp.h"
#include "soft_codec.h"
#include "hard_codec.h"

static char *command;

static void usage(char *command)
{
	printf(
    "Usage: %s [OPTION]... \n"
    "\n"
    "-h, --help              help\n"
    "-v, --version           print current version\n"
    "-p, --cpu-pi            test the cpu performance with bc cmd\n"
    "    --soft-encode       test the soft encoding, the yuv file path\n"
    "    --soft-decode       test the soft decoding, the h264 file path\n"
    "    --hard-encode       test the hard encoding, the nv12 file path\n"
    "    --hard-decode       test the hard decoding, the h264 file path\n"
    "    --jpeg-encode       test the jpeg encoding, the bmp file path\n"
    "    --jpeg-decode       test the jpeg decoding, the jpeg file path\n"
    "    --log-file          the path config of log file\n"
    "-a  --all               auto test the all items\n"
	, command);
}

static void version(void)
{
	printf("%s: version " TERM_UTIL_VERSION_STR " by LaMar\n", command);
}

static void sig_handler(int signo)
{
    printf("signo:%d", signo);
    exit(0);
}

static int nfs_materials(char *server, char *path)
{
    char cmd[SIZE_1K] = {0};
    char res[SIZE_1K] = {0};

    snprintf(cmd, sizeof(cmd), "ping -c 1 %s > /dev/null; echo $?", server);
    exec_result(cmd, res, sizeof(res));
    if (res[0] != '0') {
        fprintf(stderr, "connot connect to %s, check network", server);
        return -1;
    }

    memset(cmd, 0, sizeof(cmd));
    memset(res, 0, sizeof(cmd));
    snprintf(cmd, sizeof(cmd), "mountpoint -q %s; echo $?", path);
    exec_result(cmd, res, sizeof(res));
    if (res[0] != '0') {
        memset(cmd, 0, sizeof(cmd));
        snprintf(cmd, sizeof(cmd),
                 "mount -t nfs %s:" NFS_DIR " %s", server, path);
        if (rj_system(cmd) < 0) {
            fprintf(stderr, "cmd: %s err:%s \n", cmd, strerror(errno));
            return -1;
        }
    }

    return 0;
}

static void set_default(struct term *t)
{
	memset(t, 0, sizeof(struct term));

    if (access(RESULT_PATH, 0) != 0) {
        if (mkdir(RESULT_PATH, 0775) != 0) {
            fprintf(stderr, "mkdir %s err:%s \n", RESULT_PATH, strerror(errno));
            return ;
        }
    }

    t->jpg_de.bmp_file= RESULT_BMP_FILE;
    t->jpg_de.jpg_file= NULL;
    t->jpg_en.jpg_file= RESULT_JPEG_FILE;
    t->jpg_en.bmp_file = NULL;
    t->log = stdout;
    t->logarg = NULL;
    t->pi_flag = 0;
    t->video_file = NULL;
    t->video_file_hard = NULL;
    t->s_en.in_file= NULL;
    t->s_en.out_file = RESULT_S_264_FILE;
    t->s_en.width = 1920;
    t->s_en.height = 1080;
    t->s_en.frame_num = 500;
    t->yuv_file_hard = NULL;
}

static void set_default_all(struct term *t)
{

    t->jpg_de.jpg_file= ORI_JPEG_FILE;
    t->jpg_en.bmp_file = ORI_BMP_FILE;
    t->pi_flag = 1;
    t->video_file = ORI_VIDEO_FILE;
    t->video_file_hard = ORI_VIDEO_FILE;
    t->s_en.in_file = ORI_YUV_FILE;
    t->yuv_file_hard = ORI_NV12_FILE;

    return ;
}

static int log_init(struct term *t)
{
	if (t->logarg) {
		t->log = fopen(t->logarg, "wb");
		if (t->log == NULL) {
			fprintf(stderr, "open file:%s err:%s\n", t->logarg, strerror(errno));
			return -1;
		}
	}

    return 0;
}

static int cpu_pi_test(struct term *t)
{
    fprintf(t->log, "cpu_pi_test===================================enter\n");
    fprintf(t->log, "CPU-PI Result: \n");
    char cmd[SIZE_1K] = {0};
    char res[SIZE_1K] = {0};
    struct timeval start;
    struct timeval end;
    long delta_time;

    snprintf(cmd, sizeof(cmd), "echo \"scale=5000; 4*a(1)\" | bc -l -q"
             " > /dev/null; echo $?");
    //fprintf(t->log, "cpu_pi_test cmd:%s \n", cmd);
    gettimeofday(&start, NULL);
    exec_result(cmd, res, sizeof(res));
    if (res[0] != '0') {
        fprintf(stderr, "cmd:%s abnormal", cmd);
        return -1;
    }
    gettimeofday(&end, NULL);
    delta_time = end.tv_sec - start.tv_sec;

    fprintf(t->log, "\t PI-TEST alltime:\t%ld (s) \n", delta_time);
    fprintf(t->log, "cpu_pi_test===================================out\n");

    return 0;
}

int main(int argc, char *argv[])
{
	int option_index, c, ret;
    char res[SIZE_1K] = {0};
	static const char short_options[] = "hvpa";
    struct term t;

	static const struct option long_options[] = {
		{"help", 0, 0, 'h'},
		{"version", 0, 0, 'v'},
		{"cpu-pi", 0, 0, 'p'},
		{"soft-encode", 1, 0, OPT_VIDEO_SOFT_ENCODE},
        {"soft-decode", 1, 0, OPT_VIDEO_SOFT_DECODE},
        {"hard-encode", 1, 0, OPT_VIDEO_HARD_ENCODE},
        {"hard-decode", 1, 0, OPT_VIDEO_HARD_DECODE},
        {"jpeg-encode", 1, 0, OPT_JPEG_ENCODE},
        {"jpeg-decode", 1, 0, OPT_JPEG_DECODE},
        {"log-file", 1, 0, OPT_LOG},
        {"all", 0, 0, 'a'},
		{0, 0, 0, 0}
	};

    set_default(&t);
    ret = nfs_materials(NFS_SERVER, NFS_MOUNT);
    if (ret < 0) {
        fprintf(stderr, "nfs_materials fial! \n");
        return 1;
    }

 	command = argv[0];
	if (argc == 1) {
		usage(command);
		return 1;
	}

    signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);

	while ((c = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
		switch (c) {
		case 'h':
			usage(command);
			return 0;
		case 'v':
			version();
			return 0;
        case 'p':
            t.pi_flag = 1;
            break;
		case OPT_VIDEO_SOFT_ENCODE:
            t.s_en.in_file= optarg;
			break;
        case OPT_VIDEO_SOFT_DECODE:
            t.video_file = optarg;
            break;
        case OPT_VIDEO_HARD_ENCODE:
            t.yuv_file_hard = optarg;
            break;
        case OPT_VIDEO_HARD_DECODE:
            t.video_file_hard = optarg;
            break;
        case OPT_JPEG_ENCODE:
            t.jpg_en.bmp_file = optarg;
            break;
        case OPT_JPEG_DECODE:
            t.jpg_de.jpg_file = optarg;
            break;
        case OPT_LOG:
            t.logarg= optarg;
            break;
        case 'a':
            set_default_all(&t);
            break;
		default:
			fprintf(stderr, "Try `%s --help' for more information.\n", command);
			return 1;
		}
	}

    if (t.logarg) {
        ret = log_init(&t);
        if (ret < 0) {
			fprintf(stderr, "log init fial! \n");
            goto exit;
        }
    }

    if (t.pi_flag) {
        ret = cpu_pi_test(&t);
        if (ret < 0) {
			fprintf(t.log, "cpu_pi_test fial! \n");
            goto exit;
        }
    }

    if (t.jpg_en.bmp_file) {
        ret = process_bmp_jpg(&t);
        if (ret < 0) {
			fprintf(t.log, "process_bmp_jpg fial! \n");
            goto exit;
        }
    }

    if (t.jpg_de.jpg_file) {
        ret = process_jpg_bmp(&t);
        if (ret < 0) {
			fprintf(t.log, "process_jpg_bmp fial! \n");
            goto exit;
        }
    }

    if (t.video_file_hard) {
        ret = hard_decode_yuv(&t);
        if (ret < 0) {
			fprintf(t.log, "hard_decode_yuv fial! \n");
            goto exit;
        }
    }

    if (t.yuv_file_hard) {
        ret = hard_encode_h264(&t);
        if (ret < 0) {
			fprintf(t.log, "hard_encode_h264 fial! \n");
            goto exit;
        }
    }

    if (t.s_en.in_file) {
        ret = soft_encode_h264(&t);
        if (ret < 0) {
			fprintf(t.log, "soft_encode_h264 fial! \n");
            goto exit;
        }
    }

    if (t.video_file) {
        ret = soft_decode_yuv(&t);
        if (ret < 0) {
			fprintf(t.log, "soft_decode_yuv fial! \n");
            goto exit;
        }
    }

exit:
    if (t.log) {
		fclose(t.log);
    }

    return ret;
}



















