
#include <sys/time.h>

#include "hard_codec.h"
#include "hw_vaapi.h"
#include "hw_encode.h"

/*
 * Helper function for profiling purposes
 */
static inline unsigned int GetTickCount()
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL))
        return 0;
    return tv.tv_usec/1000+tv.tv_sec*1000;
}

//t->video_file
int hard_decode_yuv(struct term *t)
{
    fprintf(t->log, "hard_decode_yuv===================================enter\n");
    fprintf(t->log, "Hard Decode Result: \n");
    int ret = 0;
    int width = 0;
    int height = 0;
    int videoindex;
    AVPacket pkt;
    char* pic_ptr = NULL;
    unsigned long long allTime = 0;
    unsigned long long allFrameNum = 0;
    struct timeval beginTv;
    struct timeval endTv;
    AVFrame* frame = av_frame_alloc();
    AVFormatContext *pFormatCtx;
    HWVaapiDecoder vaapi;

    if (avformat_open_input(&pFormatCtx, t->video_file_hard, NULL, NULL) != 0) {
        fprintf(t->log, "Cannot open input file %s\n", t->video_file_hard);
        goto err0;
    }

    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        fprintf(t->log, "Cannot find input stream information.\n");
        goto err0;
    }

    /* find the video stream information */
    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoindex = i;
            break;
        }
    }
    if (videoindex < 0) {
        fprintf(t->log, "Cannot find a video stream in the input file\n");
        goto err0;
    }

    width = pFormatCtx->streams[videoindex]->codec->width;
    height = pFormatCtx->streams[videoindex]->codec->height;

    if (vaapi.init(width, height, VIDEO_CODEC_TYPE_H264) == false) {
        fprintf(t->log, "Can't init the hw vaapi codec!\n");
        goto err0;
    }

    gettimeofday(&beginTv, NULL);
    while (av_read_frame(pFormatCtx, &pkt) >= 0) {
        if (videoindex == pkt.stream_index) {
            if (vaapi.decodeVideoStream(pkt, frame) == false) {
                av_free_packet(&pkt);
                continue;
            }
        }

        if (vaapi.convertImageToRgb(frame) == false) {
            fprintf(t->log, "convertImageToRgb falsed!\n");
            av_free_packet(&pkt);
            goto err0;
        }

        if ((pic_ptr = (char*)vaapi.getRgbData()) == NULL) {
            fprintf(t->log, "Can't get the rgb data\n");
            goto err0;
        }

        if (vaapi.releaseVaImageData() == false) {
            fprintf(t->log, "Can't releaseVaImageData\n");
            goto err0;
        }
        allFrameNum++;
        av_free_packet(&pkt);
    }
    gettimeofday(&endTv, NULL);
    allTime = (endTv.tv_sec * 1000 + endTv.tv_usec / 1000) -
              (beginTv.tv_sec * 1000 + beginTv.tv_usec / 1000);

    fprintf(t->log, "\tTest file:\t%s \n", t->video_file_hard);
    fprintf(t->log, "\tDecoded all frame:\t%lld \n", allFrameNum);
    fprintf(t->log, "\tDecoded all time:\t%lld (ms) \n", allTime);
    fprintf(t->log, "\tDecoded average time:\t%lld (ms) per frame \n", allTime / allFrameNum);
    fprintf(t->log, "hard_decode_yuv===================================out\n");

    avformat_close_input(&pFormatCtx);
    av_free(frame);

    return 0;

err0:
    if (pFormatCtx != NULL) {
        avformat_close_input(&pFormatCtx);
    }
    if (frame != NULL) {
        av_free(frame);
    }

    return -1;
}


//t->yuv_file,yuv file must nv12(width=1920 heigth=1080)
int hard_encode_h264(struct term *t)
{
    fprintf(t->log, "hard_encode_yuv===================================enter\n");
    int width  = 1920;
    int height = 1080;
    FILE *fin = NULL;
    FILE *fout = NULL;

    if (!(fin = fopen(t->yuv_file_hard, "r"))) {
        fprintf(t->log, "Fail to open input file : %s\n", strerror(errno));
        return -1;
    }

    //just for test performance no need to save
    const char* output_file = "/tmp/ffmpeg_out.h264";
    if (!(fout = fopen(output_file, "w+b"))) {
        fprintf(t->log, "Fail to open output file : %s\n", strerror(errno));
        fclose(fin);
        return -1;
    }

    HWVaapiEncoder vaapi_encoder;
    if (vaapi_encoder.init((void**)&fin, width, height) == false) {
        fprintf(t->log, "Can't init the hw vaapi_encoder!\n");
        fclose(fin);
        fclose(fout);
        return -1;
    }

    static unsigned long long int sum = 0;
    static unsigned long long int count = 0;
    while(1) {
        unsigned int begin = GetTickCount();
        AVFrame* hw_frame = NULL;
        hw_frame = (AVFrame*)vaapi_encoder.getHwFrameData();
        if (hw_frame == NULL && vaapi_encoder.has_file_read_done) {
            fprintf(t->log, "Hard Encode Result: \n");
            break;
        }

        AVPacket enc_pkt;
        av_init_packet(&enc_pkt);
        enc_pkt.data = NULL;
        enc_pkt.size = 0;
        if (vaapi_encoder.encodeStream(hw_frame, enc_pkt) == true) {
            if(fwrite(enc_pkt.data, enc_pkt.size, 1, fout) < 0)
                fprintf(t->log, "enc_pkt write error\n");
            av_packet_unref(&enc_pkt);

            unsigned int total_time = GetTickCount() - begin;
            sum += total_time;
            count++;
        }
        // must call freeHwFrameData after use hw_frame
        vaapi_encoder.freeHwFrameData(hw_frame);
        //fprintf(t->log, "[encode one frame] current FPS = %lld [spend %lld ms]\n",
        //         1000/(sum/count),(sum/count));
    }

    fprintf(t->log, "\tTest file:\t%s \n", t->yuv_file_hard);
    fprintf(t->log, "\tEncoded all frame:\t%lld \n", count);
    fprintf(t->log, "\tEncoded all time:\t%lld (ms) \n", sum);
    fprintf(t->log, "\tEncoded average time:\t%lld (ms) per frame \n", sum/count);
    fprintf(t->log, "\tEncoded average FPS: \t%lld \n", 1000/(sum/count));
    fprintf(t->log, "hard_decode_yuv===================================out\n");

    if (fout)
        fclose(fout);
    if (fin)
        fclose(fin);

    return 0;
}

//t->yuv_file
int hard_encode_h265(struct term *t)
{
    // nonsupport now
    fprintf(t->log, "hard_encode_h265: interface test \n");
    return 0;
}



