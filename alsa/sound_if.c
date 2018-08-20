

#include "sound_if.h"

#define DEFAULT_SAMPLERATE     (44100)
#define DEFAULT_CHANNEL        (2)
#define DEFAULT_FORMAT         (SND_PCM_FORMAT_S16_LE)

#define PLAYBACK               (SND_PCM_STREAM_PLAYBACK)
#define RECORD                 (SND_PCM_STREAM_CAPTURE)

#define RECORD_FRAME_SIZE      (64)
#define PLAYBACK_FRAME_SIZE    (1024)
#define SOUND_RECORD_FILE      ("/home/lamar/Music/fenjxc.wav")

#define SOUND_RECORD_START     (1)
#define SOUND_PLAYBACK_START   (2)
#define SOUND_PLAYBACK_END     (3)

#define SOUND_CARD_VDI         (0)
#define SOUND_CARD_IDV         (1)

static const char *card_name = "default";   //指定pcm的设备

SND_INFO_T *g_record_info       = NULL;     //录音
SND_INFO_T *g_playback_info     = NULL;     //放音

static int write_local_data(char* filename, char* mod, char* buf, int size)
{
	int     count   = 0;
	FILE   *outfile = NULL;

	if ((outfile = fopen(filename, mod)) == NULL) {
		printf("can't open %s\n", filename);
		return FAIL;
	}

	count = fwrite(buf, size, 1, outfile);
	if (count != 1) {
		printf("write data failed file=%s count=%d size=%d\n", filename,
				count, size);
		fclose(outfile);
		return FAIL;
	}

	fflush(outfile);
	fclose(outfile);

	return SUCCESS;
}

static int read_local_data(char* filename, char* buf, int size)
{
	int     ret    = 0;
	FILE   *infile = NULL;

	if ((infile = fopen(filename, "rb")) == NULL) {
		printf("can't open %s\n", filename);
		return FAIL;
	}

	ret = fread(buf, size, 1, infile);
	if (ret != 1) {
		printf("read file failed %s size=%d\n", filename, size);
		fclose(infile);
		return FAIL;
	}

	fclose(infile);
	return SUCCESS;
}


int sound_init()
{
    //Capture
    g_record_info = (SND_INFO_T*) malloc(sizeof(SND_INFO_T));
    if (!g_record_info) {
        return FAIL;
    }

    memset(g_record_info, 0, sizeof(SND_INFO_T));
    g_record_info->samplearate 	= DEFAULT_SAMPLERATE;
    g_record_info->channels 	= DEFAULT_CHANNEL;
    g_record_info->format 		= DEFAULT_FORMAT;
    g_record_info->period_size 	= RECORD_FRAME_SIZE;
    g_record_info->direction 	= RECORD;
    g_record_info->pcm          = NULL;
    g_record_info->status       = 0;
    g_record_info->card         = card_name;

    //Playback
    g_playback_info = (SND_INFO_T *) malloc(sizeof(SND_INFO_T));
    if (!g_playback_info) {
        return FAIL;
    }

    memset(g_playback_info, 0, sizeof(SND_INFO_T));
    g_playback_info->samplearate	= DEFAULT_SAMPLERATE;
    g_playback_info->channels       = DEFAULT_CHANNEL;
    g_playback_info->format         = DEFAULT_FORMAT;
    g_playback_info->period_size	= PLAYBACK_FRAME_SIZE;
    g_playback_info->direction      = PLAYBACK;
    g_playback_info->pcm            = NULL;
    g_playback_info->card           = card_name;

    return SUCCESS;
}


int sound_open(SND_INFO_T *info)
{
    int err = -1;
    int result = FAIL;

    int direction   = info->direction;
    int sample_rate = info->samplearate;
    int channels    = info->channels;
    snd_pcm_format_t format       = info->format;;
    snd_pcm_uframes_t period_size = info->period_size;

    snd_pcm_hw_params_t* hw_params = NULL;

    printf("open audio playback device %s\n", info->card);

    /* Open PCM device for playback/capture */
    err = snd_pcm_open(&info->pcm, info->card, info->direction, 0);
    if (err < 0) {
        printf("cannot open audio record device %s \n", snd_strerror(err));
        return result;
    }

    /* Allocate a hardware parameters object. */
    if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
        printf("cannot allocate hardware parameter structure %s \n",
                  snd_strerror(err));
        goto ERROR;
    }

    /* Fill it in with default values. */
    if ((err = snd_pcm_hw_params_any(info->pcm, hw_params)) < 0) {
        printf("cannot initialize hardware parameter structure %s \n",
                  snd_strerror(err));
        goto ERROR;
    }

    /* Interleaved mode */
    if ((err = snd_pcm_hw_params_set_access(info->pcm, hw_params,
                                            SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        printf("cannot set access type %s \n", snd_strerror(err));
        goto ERROR;
    }

    if ((err = snd_pcm_hw_params_set_format(info->pcm, hw_params,
                                            format)) < 0) {
        printf("cannot set sample format %s \n", snd_strerror(err));
        goto ERROR;
    }

    if ((err = snd_pcm_hw_params_set_channels(info->pcm, hw_params,
               channels)) < 0) {
        printf("cannot set channel count %s \n", snd_strerror(err));
        goto ERROR;
    }

    if ((err = snd_pcm_hw_params_set_rate_near(info->pcm, hw_params,
               (unsigned int *)&sample_rate, 0)) < 0) {
        printf("cannot set sample rate %s \n", snd_strerror(err));
        goto ERROR;
    }

    if ((err = snd_pcm_hw_params_set_period_size_near(info->pcm,
               hw_params, &period_size, &direction)) < 0) {
        printf("cannot set period size near %s \n", snd_strerror(err));
        goto ERROR;
    }

    /* Write the parameters to the driver */
    if ((err = snd_pcm_hw_params(info->pcm, hw_params)) < 0) {
        printf("cannot set parameters %s \n", snd_strerror(err));
        goto ERROR;
    }

    /* 验证参数是否正确设置 */
    err = snd_pcm_hw_params_get_period_size(hw_params, &period_size, &direction);
    if (err < 0) {
        printf("get frame size failed %s \n", snd_strerror(err));
        goto ERROR;
    }

    err = snd_pcm_hw_params_get_rate(hw_params, (unsigned int *)&sample_rate, &direction);
    if (err < 0) {
        printf("get rate failed %s \n", snd_strerror(err));
        goto ERROR;
    }

    err = snd_pcm_hw_params_get_channels(hw_params, (unsigned int *)&channels);
    if (err < 0) {
        printf("get channel failed %s \n", snd_strerror(err));
        goto ERROR;
    }

    err = snd_pcm_hw_params_get_format(hw_params, &format);
    if (err < 0) {
        printf("get format failed %s \n", snd_strerror(err));
        goto ERROR;
    }

    info->samplearate   = sample_rate;
    info->channels      = channels;
    info->format        = format;
    info->period_size   = period_size;

    result = SUCCESS;

    printf("set hardware params success samplarate : %d, channels : %d, format : 0x%x, period_size : %d \n",
              sample_rate, channels, format, period_size);

ERROR:
    if (result == FAIL) {
        snd_pcm_close(info->pcm);
        info->pcm = NULL;
    }

    if (hw_params != NULL) {
        snd_pcm_hw_params_free(hw_params);
        hw_params = NULL;
    }

    return result;
}

int sound_close(SND_INFO_T *info)
{
    if (info->pcm) {
        snd_pcm_close(info->pcm);
        info->pcm = NULL;
    }

    return 0;
}

int sound_playback(SND_INFO_T *playback)
{
    int         ret    = 0;
    FILE       *infile = NULL;
    char    *buf    = NULL;
    snd_pcm_sframes_t write_frame;

    ret = sound_open(playback);
    if (FAIL == ret) {
        printf("record thread init failed\n");
        goto err;
    }

    if ((playback->samplearate != g_record_info->samplearate) ||
            (playback->channels != g_record_info->channels) ||
            (playback->format != g_record_info->format)) {
        printf("playback params is different from record params!");
        goto err;
    }

    int buffer_size = snd_pcm_frames_to_bytes(playback->pcm, playback->period_size);

    buf = (unsigned char *)malloc(buffer_size);
    if (buf == NULL) {
        printf("malloc sound buffer failed! \n");
        goto err;
    }

    if ((infile = fopen(SOUND_RECORD_FILE, "r")) == NULL) {
        printf("can't open %s\n", SOUND_RECORD_FILE);
        goto err;
    }

    while (1) {
        memset(buf, 0x00, buffer_size);
        ret = fread(buf, buffer_size, 1, infile);
        if (ret == 0) {
            printf("read end of file \n");
            break;
        }

        if (!playback->pcm) {
            printf("sound pcm not init! \n");
            break;
        }

        write_frame = snd_pcm_writei(playback->pcm, buf, playback->period_size);
        if (write_frame < 0) {
            printf("pcm write errno=%d \n", errno);
            if (write_frame == -EPIPE) {
                snd_pcm_prepare(playback->pcm);
            } else {
                snd_pcm_recover(playback->pcm, write_frame, 1);
            }
            continue;
        }
    }

err:
    if (infile != NULL) {
        fclose(infile);
        infile = NULL;
    }

    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }

    return ret;
}


int sound_capture(SND_INFO_T *capture)
{
    int               ret       = 0;
    int               retry_cnt = 80;
    int               loop_cnt  = 100;
    unsigned char          *buf       = NULL;
    snd_pcm_sframes_t recv_len  = 0;

    while (retry_cnt) {
        ret = sound_open(capture);
        if (FAIL == ret) {
            if (--retry_cnt < 0) {
                printf("record thread init failed \n");
                goto err;
            }
            printf("record thread init failed, retry... \n");
            usleep(200000);
        } else {
            capture->status = 1;
            break;
        }
    }

    int buffer_size = snd_pcm_frames_to_bytes(capture->pcm, capture->period_size);

    buf = (unsigned char *)malloc(buffer_size);
    if (buf == NULL) {
        printf("malloc sound buffer failed! \n");
        goto err;
    }

    while (loop_cnt) {
        loop_cnt--;
        memset(buf, 0x00, buffer_size);
        recv_len = snd_pcm_readi(capture->pcm, buf, capture->period_size);
        if (recv_len < 0) {
            printf("pcm readi errno=%d \n", errno);
            if (recv_len == -EPIPE) {
                snd_pcm_prepare(capture->pcm);
            } else {
                snd_pcm_recover(capture->pcm, recv_len, 1);
            }
            continue;
        }

        if (recv_len == capture->period_size) {
            write_local_data(SOUND_RECORD_FILE, "a+", buf, buffer_size);
        } else {
            printf("read size not period_size : %d", recv_len);
        }
    }

err:
    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }

    return NULL;
}




void config_list()
{
    int val;
    snd_pcm_t *handle;

    printf("ALSA library version: %s\n", SND_LIB_VERSION_STR);

    printf("\n***PCM stream types***:\n");
    for (val = 0;val <= SND_PCM_STREAM_LAST;val++)
            printf("%s\n", snd_pcm_stream_name((snd_pcm_stream_t)val));

    printf("\n***PCM access types***:\n");
    for (val = 0; val <= SND_PCM_ACCESS_LAST; val++)
            printf("%s\n", snd_pcm_access_name((snd_pcm_access_t)val));

    printf("\n***PCM format***:\n");
    for (val = 0; val <= SND_PCM_FORMAT_LAST; val++)
            if (snd_pcm_format_name((snd_pcm_format_t)val) != NULL)
                    printf("%s (%s)\n", snd_pcm_format_name((snd_pcm_format_t)val),
                                    snd_pcm_format_description((snd_pcm_format_t)val));

    printf("\n***PCM subformat***:\n");
    for (val = 0; val <= SND_PCM_SUBFORMAT_LAST; val++)
            printf("%s (%s)\n", snd_pcm_subformat_name((snd_pcm_subformat_t)val),
                            snd_pcm_subformat_description((snd_pcm_subformat_t)val));

    printf("\n***PCM states***:\n");
    for (val = 0;val < SND_PCM_STATE_LAST;val++)
            printf("%s\n", snd_pcm_state_name((snd_pcm_state_t)val));
}












