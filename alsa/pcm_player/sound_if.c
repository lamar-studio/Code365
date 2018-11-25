

#include <alsa/mixer.h>
#include <unistd.h>
#include "sound_if.h"

#define DEFAULT_SAMPLERATE     (44100)
#define DEFAULT_CHANNEL        (2)
#define DEFAULT_FORMAT         (SND_PCM_FORMAT_S16_LE)

#define PLAYBACK               (SND_PCM_STREAM_PLAYBACK)
#define RECORD                 (SND_PCM_STREAM_CAPTURE)

#define RECORD_FRAME_SIZE      (64)
#define PLAYBACK_FRAME_SIZE    (1024)
#define SOUND_RECORD_FILE      ("/home/lamar/Music/capture.wav")
#define SOUND_RECORD_FILE_SIZE (20)              //单位为 M
#define SOUND_PLAYBACK_FILE    ("/home/lamar/Music/playback.wav")

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

static int capture_data(const void *buffer, size_t bytes, int Msize, const char *path)
{
    static FILE* fd = NULL;
    static int offset = 0;

    if (fd == NULL) {
        fd = fopen(path, "wb+");
        if (fd == NULL) {
            printf("DEBUG open %s error = %d", path, errno);
            offset = 0;
        }
    }
    fwrite(buffer,bytes,1,fd);
    offset += bytes;
    fflush(fd);
    if (offset >= Msize*1024*1024) {
        Msize = 0;
        fclose(fd);
        fd = NULL;
        offset = 0;
        printf("playback pcmfile end");

        return 1;
    }

    return 0;
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

    /* Allocate a hardware parameters object. 分配一个硬件参数实体 */
    if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0) {
        printf("cannot allocate hardware parameter structure %s \n",
                  snd_strerror(err));
        goto ERROR;
    }

    /* Fill it in with default values. 初始化硬件的默认参数 */
    if ((err = snd_pcm_hw_params_any(info->pcm, hw_params)) < 0) {
        printf("cannot initialize hardware parameter structure %s \n",
                  snd_strerror(err));
        goto ERROR;
    }

    //修改硬件中的特定参数
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

    //更新硬件参数到驱动
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

    //帧转换成字节
    int buffer_size = snd_pcm_frames_to_bytes(playback->pcm, playback->period_size);
    printf("%d:buffer_size:%d", __LINE__, buffer_size);

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

        //一个周期一个周期地写入数据
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
    unsigned char    *buf       = NULL;
    snd_pcm_sframes_t recv_len  = 0;

    ret = sound_open(capture);
    if (FAIL == ret) {
        printf("record thread init failed\n");
        goto err;
    }

    //帧转换成字节
    int buffer_size = snd_pcm_frames_to_bytes(capture->pcm, capture->period_size);

    buf = (unsigned char *)malloc(buffer_size);
    if (buf == NULL) {
        printf("malloc sound buffer failed! \n");
        goto err;
    }

    while (1) {
        memset(buf, 0x00, buffer_size);
        //一个周期一个周期地读数据，需要把帧转换成字节
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
            //保存录音文件,超过SOUND_RECORD_FILE_SIZE大小时，退出循环
            if (capture_data(buf, buffer_size, SOUND_RECORD_FILE_SIZE, SOUND_RECORD_FILE) != 0)
                break;
        } else {
            printf("read size not period_size : %ld", recv_len);
        }
    }

err:
    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }

    return NULL;
}


void pcm_config_list()
{
    int val;

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


void ctl_config_list()
{
    snd_mixer_t *mixer_fd;
    snd_mixer_elem_t* elem = NULL;
    const char *card = "default";

    ret = snd_mixer_open(&mixer_fd, 0);
    if (ret < 0) {
        LOG_ERROR("open mixer failed ret=%d\n", ret);
        return;
    }

    ret = snd_mixer_attach(mixer_fd, card);
    if (ret < 0) {
        snd_mixer_close(mixer_fd);
        LOG_ERROR("attach mixer failed ret=%d\n", ret);
        return;
    }

    ret = snd_mixer_selem_register(mixer_fd, NULL, NULL);
    if (ret < 0) {
        snd_mixer_close(mixer_fd);
        LOG_ERROR("register mixer failed ret=%d \n", ret);
        return;
    }

    ret = snd_mixer_load(mixer_fd);

    if (ret < 0) {
        snd_mixer_close(mixer_fd);
        LOG_ERROR("load mixer failed ret=%d \n", ret);
        return;
    }


    for (elem = snd_mixer_first_elem(mixer_fd); elem;
        elem = snd_mixer_elem_next(elem)) {
        name = snd_mixer_selem_get_name(elem);
        printf("name=%s \n", name);
    }
}


void SetAlsaMasterVolume(long volume)
{
    long min, max;
    int lvol=0, rvol=0, maxvol=70;

    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "Line";

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    printf("min=%i, max=%i\n", min, max);
    snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, maxvol * max / 100);
    snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, maxvol * max / 100);
    for (rvol=0;rvol<maxvol;rvol++) {
        usleep(100000);
        snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, rvol * max / 100);
    }
    for (lvol=maxvol;lvol>=0;lvol--) {
        usleep(100000);
        snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, lvol * max / 100);
    }
    snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, maxvol * max / 100);
    snd_mixer_selem_set_playback_volume(elem, SND_MIXER_SCHN_FRONT_RIGHT, maxvol * max / 100);
    snd_mixer_close(handle);
}






