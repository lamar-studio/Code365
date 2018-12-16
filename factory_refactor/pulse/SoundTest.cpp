/*
 * Create by LaMar at 2018/09/05
 */
#include <string.h>
#include <pthread.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <errno.h>

#include "HWConfig.h"
#include "SoundTest.h"

#define SOUND_RECORD_FILE      ("/tmp/sound.wav")
#define BUF_SIZE               (1024)
#define APP_NAME               ("sound_test")

static pa_simple *pa_play = NULL;
static pa_simple *pa_rec = NULL;
static int pa_error;
static pthread_t rec_thread;
static pthread_t play_thread;
static int rec_stop = 0;
static int play_stop = 0;


/* A simple routine calling UNIX write() in a loop */
static ssize_t loop_write(FILE *fp, const void *data, size_t size) {
    ssize_t ret = 0;

    while (size > 0) {
        ssize_t r;

        if ((r = fwrite(data, 1, size, fp)) < 0)
            return r;

        if (r == 0)
            break;

        ret += r;
        data = (const uint8_t*) data + r;
        size -= (size_t) r;
    }

    return ret;
}

static void *recordLoop(void *arg)
{
    int count = 0;
    int recv_len = 0;
    FILE * outfile = NULL;

    if ((outfile = fopen(SOUND_RECORD_FILE, "w")) == NULL) {
        mlog("can't open %s\n", SOUND_RECORD_FILE);
        goto finish;
    }

    while (!rec_stop) {
        mlog("in the recordLoop");
        uint8_t buf[BUF_SIZE];

        /* Record some data ... */
        if (pa_simple_read(pa_rec, buf, sizeof(buf), &pa_error) < 0) {
            log("pa_simple_read() failed: %s", pa_strerror(pa_error));
            goto finish;
        }
        mlog("buf %d,%d,%d,%d,%d", buf[0], buf[1], buf[2], buf[3], buf[4]);
        /* And write it to file */
        if (loop_write(outfile, buf, sizeof(buf)) != sizeof(buf)) {
            log("loop_write() failed: %s", strerror(errno));
            goto finish;
        }
    }
    fflush(outfile);

finish:
    if (outfile != NULL) {
        fclose(outfile);
        outfile = NULL;
    }
    mlog("exit the recordLoop");

    return NULL;
}

static void *playbackLoop(void *arg)
{
    FILE *infile = NULL;

    if ((infile = fopen(SOUND_RECORD_FILE, "r")) == NULL) {
        mlog("can't open %s\n", SOUND_RECORD_FILE);
        goto finish;
    }

    while (!play_stop) {
        uint8_t buf[BUF_SIZE];
        ssize_t ret;

        /* read some data */
        if ((ret = fread(buf, sizeof(buf), 1, infile)) <= 0) {
            if (ret == 0) {     /* EOF */
                log("read end of file \n");
                break;
            }
            log("fread() failed:%s", strerror(errno));
        }
        mlog("buf %d,%d,%d,%d,%d", buf[0], buf[1], buf[2], buf[3], buf[4]);
        /* ... and play it */
        if (pa_simple_write(pa_play, buf, (size_t)ret, &pa_error) < 0) {
            mlog("pa_simple_write() failed: %s", pa_strerror(pa_error));
            goto finish;
        }
    }

    /* Make sure that every single sample was played */
    if (pa_simple_drain(pa_play, &pa_error) < 0) {
        mlog("pa_simple_drain() failed: %s", pa_strerror(pa_error));
    }

finish:
    if (infile != NULL) {
        fclose(infile);
        infile = NULL;
    }
    mlog("exit the playbackLoop");

    return NULL;
}


SoundTest::SoundTest()
{
    init();
}

SoundTest::~SoundTest()
{
    deInit();
}

bool SoundTest::startRecord()
{
    mlog("sound test record start");
    rec_stop = 0;
    pthread_create(&rec_thread, NULL, recordLoop, NULL);

    return SUCCESS;
}

bool SoundTest::stopRecord()
{
    mlog("sound test record stop");
    rec_stop = 1;
    pthread_join(rec_thread, NULL);

    return SUCCESS;
}

bool SoundTest::startPlayback()
{
    mlog("sound test playback start");
    play_stop = 0;
    pthread_create(&play_thread, NULL, playbackLoop, NULL);

    return SUCCESS;
}

bool SoundTest::stopPlayback()
{
    mlog("sound test playback stop");
    play_stop = 1;
    pthread_join(rec_thread, NULL);

    return SUCCESS;
}

bool SoundTest::init()
{
    CHECK_FUNCTION_IN();
    static const pa_sample_spec ss = {
            .format = PA_SAMPLE_S16LE,
            .rate = 44100,
            .channels = 2
    };

    /* Create a new playback stream */
    pa_play = pa_simple_new(NULL,
            APP_NAME,
            PA_STREAM_PLAYBACK,
            NULL,
            "player_test",
            &ss, NULL, NULL,
            &pa_error);

    if (!pa_play)
        log("Could not connect to pulseaudio server: %s", pa_strerror(pa_error));

    /* Create the recording stream */
    pa_rec = pa_simple_new(NULL,
            APP_NAME,
            PA_STREAM_RECORD,
            NULL,
            "recoder_test",
            &ss, NULL, NULL,
            &pa_error);

    if (!pa_rec)
        log("Could not connect to pulseaudio server: %s", pa_strerror(pa_error));

    return 0;

    //initVolume();

    return SUCCESS;
}


bool SoundTest::deInit()
{
    CHECK_FUNCTION_IN();
    if (pa_play)
        pa_simple_free(pa_play);
    pa_play = NULL;

    if (pa_rec)
        pa_simple_free(pa_rec);
    pa_rec = NULL;

    return SUCCESS;
}












