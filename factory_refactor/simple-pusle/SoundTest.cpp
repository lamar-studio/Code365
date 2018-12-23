/*
 * Create by LaMar at 2018/09/05
 */
#include <stdlib.h>
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

static int pa_error;
static int rec_stop = 0;
static int play_stop = 0;
static pthread_t rec_pid;
static pthread_t play_pid;

static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
};

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
    FILE *outfile = NULL;
    pa_simple *pa_rec = NULL;

    /* Create the recording stream */
    pa_rec = pa_simple_new(NULL,
            APP_NAME,
            PA_STREAM_RECORD,
            NULL,
            "recoder_test",
            &ss, NULL, NULL,
            &pa_error);

    if (!pa_rec) {
        log("Could not connect to pulseaudio server: %s", pa_strerror(pa_error));
        goto finish;
    }


    if ((outfile = fopen(SOUND_RECORD_FILE, "w")) == NULL) {
        log("can't open %s\n", SOUND_RECORD_FILE);
        goto finish;
    }

    while (!rec_stop) {
        uint8_t buf[BUF_SIZE];
        mlog("in the recordLoop buf:%p buf:%d,%d,%d,%d,%d", buf, buf[0], buf[1], buf[2], buf[3], buf[4]);

        /* Record some data ... */
        if (pa_simple_read(pa_rec, buf, sizeof(buf), &pa_error) < 0) {
            log("pa_simple_read() failed: %s", pa_strerror(pa_error));
            goto finish;
        }

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

    if (pa_rec) {
        pa_simple_free(pa_rec);
        pa_rec = NULL;
    }
    log("exit the recordLoop");

    return NULL;
}

static void *playbackLoop(void *arg)
{
    FILE *infile = NULL;
    pa_simple *pa_play = NULL;

    /* Create a new playback stream */
    pa_play = pa_simple_new(NULL,
            APP_NAME,
            PA_STREAM_PLAYBACK,
            NULL,
            "player",
            &ss, NULL, NULL,
            &pa_error);

    if (!pa_play) {
        log("Could not connect to pulseaudio server: %s", pa_strerror(pa_error));
        goto finish;
    }

    if ((infile = fopen(SOUND_RECORD_FILE, "r")) == NULL) {
        log("can't open %s\n", SOUND_RECORD_FILE);
        goto finish;
    }

    while (!play_stop) {
        uint8_t buf[BUF_SIZE] = {0};
        ssize_t ret;
        mlog("in the playbackLoop buf:%p buf:%d,%d,%d,%d,%d", buf, buf[0], buf[1], buf[2], buf[3], buf[4]);
        /* read some data */
        if ((ret = fread(buf, 1, sizeof(buf), infile)) <= 0) {
            if (ret == 0) {     /* EOF */
                log("read end of file");
                break;
            }
            log("fread() failed:%s", strerror(errno));
        }

        /* ... and play it */
        if (pa_simple_write(pa_play, buf, sizeof(buf), &pa_error) < 0) {
            log("pa_simple_write() failed: %s", pa_strerror(pa_error));
            goto finish;
        }
    }

    /* Make sure that every single sample was played */
    if (pa_simple_drain(pa_play, &pa_error) < 0) {
        log("pa_simple_drain() failed: %s", pa_strerror(pa_error));
    }

finish:
    if (infile != NULL) {
        fclose(infile);
        infile = NULL;
    }

    if (pa_play) {
        pa_simple_free(pa_play);
        pa_play = NULL;
    }
    remove(SOUND_RECORD_FILE);
    log("exit the playbackLoop");

    return NULL;
}


SoundTest::SoundTest()
{
    initVolume();
}

SoundTest::~SoundTest()
{

}

bool SoundTest::startRecord()
{
    log("sound test record start");
    rec_stop = 0;
    pthread_create(&rec_pid, NULL, recordLoop, NULL);

    return SUCCESS;
}

bool SoundTest::stopRecord()
{
    log("sound test record stop");
    rec_stop = 1;
    pthread_join(rec_pid, NULL);

    return SUCCESS;
}

bool SoundTest::startPlayback()
{
    log("sound test playback start");
    play_stop = 0;
    pthread_create(&play_pid, NULL, playbackLoop, NULL);

    return SUCCESS;
}

bool SoundTest::stopPlayback()
{
    log("sound test playback stop");
    play_stop = 1;
    pthread_join(play_pid, NULL);

    return SUCCESS;
}

bool SoundTest::initVolume()
{
    CHECK_FUNCTION_IN();

    system("pactl set-sink-volume @DEFAULT_SINK@ 100%");
    system("pactl set-source-volume @DEFAULT_SOURCE@ 100%");

    return SUCCESS;
}


