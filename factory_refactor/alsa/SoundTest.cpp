/*
 * Create by LaMar at 2018/09/05
 */
#include <alsa/asoundlib.h>
#include <pthread.h>

#include "HWConfig.h"
#include "SoundTest.h"

#define DEFAULT_SAMPLERATE     (44100)
#define DEFAULT_CHANNEL        (2)
#define DEFAULT_FORMAT         (SND_PCM_FORMAT_S16_LE)

#define PLAYBACK               (SND_PCM_STREAM_PLAYBACK)
#define RECORD                 (SND_PCM_STREAM_CAPTURE)

#define RECORD_FRAME_SIZE      (64)
#define PLAYBACK_FRAME_SIZE    (1024)

#define SOUND_RECORD_FILE      ("/tmp/factory_test/sound.wav")
#define DEFAULT_CARD_NAME      ("default")

static SND_INFO_T *g_record_info = NULL;
static SND_INFO_T *g_playback_info = NULL;
static SND_STATUS_T gStatus =  SOUND_UNKNOW;
static pthread_mutex_t gMutex;
SoundTest* SoundTest::mInstance = NULL;

static void initVolume()
{
    int ret = 0;
    const char *name = NULL;
    long minVolume = 0;
    long maxVolume = 0;
    snd_mixer_t *mixer_fd = NULL;
    snd_mixer_elem_t *elem = NULL;

    ret = snd_mixer_open(&mixer_fd, 0);
    if (ret < 0) {
        mlog("open mixer failed ret=%d\n", ret);
        return;
    }

    ret = snd_mixer_attach(mixer_fd, g_record_info->card);
    if (ret < 0) {
        snd_mixer_close(mixer_fd);
        mlog("attach mixer failed ret=%d\n", ret);
        return;
    }

    ret = snd_mixer_selem_register(mixer_fd, NULL, NULL);
    if (ret < 0) {
        snd_mixer_close(mixer_fd);
        mlog("register mixer failed ret=%d \n", ret);
        return;
    }

    ret = snd_mixer_load(mixer_fd);
    if (ret < 0) {
        snd_mixer_close(mixer_fd);
        mlog("load mixer failed ret=%d \n", ret);
        return;
    }

    for (elem = snd_mixer_first_elem(mixer_fd); elem;
            elem = snd_mixer_elem_next(elem)) {
        name = snd_mixer_selem_get_name(elem);
        mlog("name=%s \n", name);

        if ((strcmp(name, "Master") == 0) || (strcmp(name, "Headphone") == 0)
            || (strcmp(name, "Speaker") == 0) || (strcmp(name, "PCM") == 0)
            || (strcmp(name, "Mic") == 0)) {
            //set maxvolume
            ret = snd_mixer_selem_get_playback_volume_range(elem, &minVolume, &maxVolume);
            mlog("name=%s get_playback_volume_range min=%ld max=%ld\n", name, minVolume, maxVolume);
            snd_mixer_selem_set_playback_switch_all(elem, 1);
            if (ret == 0) {
                snd_mixer_selem_set_playback_volume_all(elem, maxVolume);
            } else {
                snd_mixer_selem_set_playback_volume_all(elem, 100);
            }
        } else if (strcmp(name, "Mic Boost") == 0) {
            //set minvolume
            ret = snd_mixer_selem_get_playback_volume_range(elem, &minVolume, &maxVolume);
            mlog("name=%s get_playback_volume_range min=%ld max=%ld\n", name, minVolume, maxVolume);
            snd_mixer_selem_set_capture_switch_all(elem, 1);
            if (ret == 0) {
                snd_mixer_selem_set_capture_volume_all(elem, minVolume);
            } else {
                snd_mixer_selem_set_capture_volume_all(elem, 0);
            }
        } else if(strcmp(name, "Capture") == 0) {
            ret = snd_mixer_selem_get_capture_volume_range(elem, &minVolume, &maxVolume);
            mlog("name=%s get_capture_volume_range min=%ld max=%ld\n", name, minVolume, maxVolume);
            snd_mixer_selem_set_capture_switch_all(elem, 1);

            if (ret == 0) {
                snd_mixer_selem_set_capture_volume_all(elem, maxVolume);
            } else {
                snd_mixer_selem_set_capture_volume_all(elem, 100);
            }
        }
    }

    snd_mixer_close(mixer_fd);
}

static void *recordLoop(void *arg)
{
    int count = 0;
    FILE * outfile = NULL;

    int ret = 0;
    SND_INFO_T *info = (SND_INFO_T *) arg;
    int retry_cnt = 80;
    char *buf = NULL;
    int buffer_size = 0;
    snd_pcm_sframes_t recv_len = 0;

    pthread_detach(pthread_self());

    ret = SoundTest::getInstance()->openSoundCard(info);
    if (ret != SUCCESS) {
        mlog("openSoundCardit fail");
        return NULL;
    }

    buffer_size = snd_pcm_frames_to_bytes(info->pcm, info->period_size);
    buf = (char *)malloc(buffer_size);
    if (buf == NULL) {
        mlog("malloc sound buffer failed! \n");
        goto err_record;
    }

    while (gStatus == SOUND_RECORD_START) {
        mlog("in the recordLoop");
        memset(buf, 0x00, buffer_size);
        recv_len = snd_pcm_readi(info->pcm, buf, info->period_size);
        if (recv_len < 0) {
            mlog("pcm readi errno=%s \n", snd_strerror(errno));
            if (recv_len == -EPIPE) {
                snd_pcm_prepare(info->pcm);
            } else {
                snd_pcm_recover(info->pcm, recv_len, 1);
            }
            continue;
        }
        if (recv_len == info->period_size) {
            if ((outfile = fopen(SOUND_RECORD_FILE, "a+")) == NULL) {
                mlog("can't open %s\n", SOUND_RECORD_FILE);
                break;
            }

            count = fwrite(buf, buffer_size, 1, outfile);
            if (count != 1) {
                mlog("write data failed file=%s count=%d size=%d\n", SOUND_RECORD_FILE, count, buffer_size);
                fclose(outfile);
                break;
            }
            fflush(outfile);
            fclose(outfile);
        } else {
            mlog("read size not period_size:%ld", recv_len);
        }
    }

    mlog("exit the recordLoop");
err_record:
    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }
    SoundTest::getInstance()->closeSoundCard(info);

    return NULL;
}

static void *playbackLoop(void *arg)
{
    int ret = 0;
    SND_INFO_T *info = (SND_INFO_T *)arg;
    FILE *infile = NULL;
    snd_pcm_sframes_t write_frame;
    char *buf = NULL;
    int buffer_size = 0;

    pthread_detach(pthread_self());

    ret = SoundTest::getInstance()->openSoundCard(info);
    if (ret != SUCCESS) {
        mlog("openSoundCardit fail");
        return NULL;
    }

    if ((info->samplearate != g_record_info->samplearate)
        || (info->channels != g_record_info->channels)
        || (info->format != g_record_info->format)) {
        mlog("playback params is different from record params!");
        goto err_playback;
    }

    buffer_size = snd_pcm_frames_to_bytes(info->pcm, info->period_size);

    buf = (char *)malloc(buffer_size);
    if (buf == NULL) {
        mlog("malloc sound buffer failed! \n");
        goto err_playback;
    }

    if ((infile = fopen(SOUND_RECORD_FILE, "r")) == NULL) {
        mlog("can't open %s\n", SOUND_RECORD_FILE);
        goto err_playback;
    }

    while (gStatus == SOUND_PLAYBACK_START) {
        mlog("in the playbackLoop");
        memset(buf, 0x00, buffer_size);
        ret = fread(buf, buffer_size, 1, infile);
        if (ret == 0) {
            mlog("read end of file \n");
            break;
        }

        if (!info->pcm) {
            mlog("sound pcm not init! \n");
            break;
        }

        write_frame = snd_pcm_writei(info->pcm, buf, info->period_size);
        if (write_frame < 0) {
            mlog("pcm write errno=%s \n", snd_strerror(errno));
            if (write_frame == -EPIPE) {
                snd_pcm_prepare(info->pcm);
            } else {
                snd_pcm_recover(info->pcm, write_frame, 1);
            }
            continue;
        }
    }

err_playback:
    if (infile != NULL) {
        fclose(infile);
        infile = NULL;
    }

    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }
    SoundTest::getInstance()->closeSoundCard(info);
    remove(SOUND_RECORD_FILE);

    return NULL;
}


SoundTest::SoundTest()
{
    init();
}

SoundTest::~SoundTest()
{

}

SoundTest* SoundTest::getInstance()
{
    if (mInstance == NULL) {
        mInstance = new SoundTest();
    }

    return mInstance;
}

int SoundTest::openSoundCard(SND_INFO_T *info)
{
    int err = -1;
    int result = FAIL;

    int direction   = info->direction;
    int sample_rate = info->samplearate;
    int channels    = info->channels;
    snd_pcm_format_t format       = info->format;;
    snd_pcm_uframes_t period_size = info->period_size;
    snd_pcm_hw_params_t *hw_params = NULL;

    mlog("open audio device %s\n", info->card);

    err = snd_pcm_open(&info->pcm, info->card, info->direction, 0);
    if (err < 0) {
        mlog("cannot open audio device %s \n", snd_strerror(err));
        return result;
    }

    err = snd_pcm_hw_params_malloc(&hw_params);
    if (err < 0) {
        mlog("cannot allocate hardware parameter structure %s \n",
                  snd_strerror(err));
        goto err;
    }

    err = snd_pcm_hw_params_any(info->pcm, hw_params);
    if (err < 0) {
        mlog("cannot initialize hardware parameter structure %s \n",
                  snd_strerror(err));
        goto err;
    }

    err = snd_pcm_hw_params_set_access(info->pcm, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0) {
        mlog("cannot set access type %s \n", snd_strerror(err));
        goto err;
    }

    err = snd_pcm_hw_params_set_format(info->pcm, hw_params, format);
    if (err < 0) {
        mlog("cannot set sample format %s \n", snd_strerror(err));
        goto err;
    }

    err = snd_pcm_hw_params_set_channels(info->pcm, hw_params, channels);
    if (err < 0) {
        mlog("cannot set channel count %s \n", snd_strerror(err));
        goto err;
    }

    err = snd_pcm_hw_params_set_rate_near(info->pcm, hw_params, (unsigned int *)&sample_rate, 0);
    if (err < 0) {
        mlog("cannot set sample rate %s \n", snd_strerror(err));
        goto err;
    }

    err = snd_pcm_hw_params_set_period_size_near(info->pcm, hw_params, &period_size, &direction);
    if (err < 0) {
        mlog("cannot set period size near %s \n", snd_strerror(err));
        goto err;
    }

    if ((err = snd_pcm_hw_params(info->pcm, hw_params)) < 0) {
        mlog("cannot set parameters %s \n", snd_strerror(err));
        goto err;
    }

    err = snd_pcm_hw_params_get_period_size(hw_params, &period_size, &direction);
    if (err < 0) {
        mlog("get frame size failed %s \n", snd_strerror(err));
        goto err;
    }

    err = snd_pcm_hw_params_get_rate(hw_params, (unsigned int *)&sample_rate, &direction);
    if (err < 0) {
        mlog("get rate failed %s \n", snd_strerror(err));
        goto err;
    }

    err = snd_pcm_hw_params_get_channels(hw_params, (unsigned int *)&channels);
    if (err < 0) {
        mlog("get channel failed %s \n", snd_strerror(err));
        goto err;
    }

    err = snd_pcm_hw_params_get_format(hw_params, &format);
    if (err < 0) {
        mlog("get format failed %s \n", snd_strerror(err));
        goto err;
    }

    info->samplearate = sample_rate;
    info->channels    = channels;
    info->format      = format;
    info->period_size = period_size;

    result = SUCCESS;

    mlog("set hardware params success samplarate:%d,channels:%d,format:0x%x,period_size:%ld \n",
         sample_rate, channels, format, period_size);

err:
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

void SoundTest::closeSoundCard(SND_INFO_T *info)
{
    if (info->pcm) {
        snd_pcm_close(info->pcm);
        info->pcm = NULL;
    }
    mlog("closeSoundCard");
}

bool SoundTest::startRecord()
{
    if (!g_record_info || SOUND_RECORD_START == gStatus) {
        mlog("it is not ready to record \n");
        return FAIL;
    }
    pthread_t pid_t;

    pthread_mutex_lock(&gMutex);
    gStatus = SOUND_RECORD_START;
    pthread_mutex_unlock(&gMutex);

    mlog("sound test record start");
    pthread_create(&pid_t, NULL, recordLoop, g_record_info);

    return SUCCESS;
}

bool SoundTest::stopRecord()
{
    if (SOUND_RECORD_STOP == gStatus) {
        return FAIL;
    }

    pthread_mutex_lock(&gMutex);
    gStatus = SOUND_RECORD_STOP;
    pthread_mutex_unlock(&gMutex);
    mlog("sound test record stop");

    return SUCCESS;
}

bool SoundTest::startPlayback()
{
    if (!g_record_info || SOUND_PLAYBACK_START == gStatus) {
        mlog("it is not ready to playback");
        return FAIL;
    }
    pthread_t pid_t;

    pthread_mutex_lock(&gMutex);
    gStatus = SOUND_PLAYBACK_START;
    pthread_mutex_unlock(&gMutex);

    mlog("sound test playback start");
    pthread_create(&pid_t, NULL, playbackLoop, g_playback_info);

    return SUCCESS;
}

bool SoundTest::stopPlayback()
{
    if (SOUND_PLAYBACK_STOP == gStatus) {
        return FAIL;
    }

    pthread_mutex_lock(&gMutex);
    gStatus = SOUND_PLAYBACK_STOP;
    pthread_mutex_unlock(&gMutex);
    mlog("sound test playback stop");

    return SUCCESS;
}

bool SoundTest::init()
{
    g_record_info = (SND_INFO_T *) malloc(sizeof(SND_INFO_T));
    if (!g_record_info) {
        return FAIL;
    }

    memset(g_record_info, 0, sizeof(SND_INFO_T));
    g_record_info->samplearate  = DEFAULT_SAMPLERATE;
    g_record_info->channels     = DEFAULT_CHANNEL;
    g_record_info->format       = DEFAULT_FORMAT;
    g_record_info->period_size  = RECORD_FRAME_SIZE;
    g_record_info->direction    = RECORD;
    g_record_info->pcm          = NULL;
    g_record_info->card         = DEFAULT_CARD_NAME;

    g_playback_info = (SND_INFO_T *) malloc(sizeof(SND_INFO_T));
    if (!g_playback_info) {
        return FAIL;
    }

    memset(g_playback_info, 0, sizeof(SND_INFO_T));
    g_playback_info->samplearate    = DEFAULT_SAMPLERATE;
    g_playback_info->channels       = DEFAULT_CHANNEL;
    g_playback_info->format         = DEFAULT_FORMAT;
    g_playback_info->period_size    = PLAYBACK_FRAME_SIZE;
    g_playback_info->direction      = PLAYBACK;
    g_playback_info->pcm            = NULL;
    g_playback_info->card           = DEFAULT_CARD_NAME;

#if 0
    if (is_product_idv(global_product_id)) {
        if (system("if ps -ef | grep -v \"color\" | grep -q \"pulseaudio\"; then pulseaudio -k; else touch /tmp/no_pulseaudio; fi") < 0) {
            mlog("pulseaudio -k error\n");
            return FAIL;
        }

        if (system("if ! lsmod | grep -q \"snd_hda_intel\"; then modprobe snd_hda_intel; fi") < 0) {
            mlog("modprobe snd_hda_intel error\n");
            return FAIL;
        }

    }
#endif

    if (pthread_mutex_init(&gMutex, NULL) != 0) {
        return FAIL;
    }
    gStatus = SOUND_UNKNOW;

    initVolume();

    return SUCCESS;
}

