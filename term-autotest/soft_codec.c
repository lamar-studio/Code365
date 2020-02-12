
#include "utils.h"
#include "soft_codec.h"

static void encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt, FILE *outfile)
{
    int ret;
    // if (frame)
        // printf("Send frame %3lld\n", frame->pts);

    ret = avcodec_send_frame(enc_ctx, frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending a frame for encoding\n");
        exit(1);
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(enc_ctx, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        } else if (ret < 0) {
            fprintf(stderr, "Error during encoding\n");
            exit(1);
        }
        //printf("Write packet %3lld(size=%5d)\n", pkt->pts, pkt->size);

        fflush(stdout);
        fwrite(pkt->data, 1, pkt->size, outfile);
        av_packet_unref(pkt);
    }
}

// cmd:time ffmpeg -s 1920x1080 -i 1920x1080.yuv -codec libx264 -f h264 -y /dev/null
// yuv to h264
int soft_encode_h264(struct term *t)
{
    fprintf(t->log, "soft_encode_h264===================================enter\n");
    fprintf(t->log, "Soft Encode Result: \n");

    int ret = 0;
    int y_size;
    int size;
    FILE *in_file = NULL;
    FILE *out_file = NULL;
    uint8_t *picture_buf;
    AVFormatContext *pFormatCtx;
    AVOutputFormat *fmt;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;
    AVFrame *pictureFrame;
    AVPacket *pkt;
    AVStream *video_st;
    unsigned long long  allTime = 0;
    unsigned long long  allFrameNum = 0;
    struct timeval      beginTv;
    struct timeval      endTv;

    in_file = fopen(t->s_en.in_file, "rb");
    if (in_file == NULL) {
        fprintf(t->log, "can not open file:%s \n", t->s_en.in_file);
        goto err;
    }

    out_file = fopen(t->s_en.out_file, "wb");
    if (out_file == NULL) {
        fprintf(t->log, "can not open file:%s \n", t->s_en.out_file);
        goto err;
    }

    // init AVFormatContext, get the format according the file name
    pFormatCtx = avformat_alloc_context();
    avformat_alloc_output_context2(&pFormatCtx, NULL, NULL, t->s_en.out_file);
    fmt = pFormatCtx->oformat;

    // init the video stream
    video_st = avformat_new_stream(pFormatCtx, 0);
    if (video_st == NULL) {
        fprintf(t->log, "failed allocating output stream\n");
        goto err;
    }
    video_st->time_base.num = 1;
    video_st->time_base.den = 25;

    // init the Context para
    pCodecCtx = avcodec_alloc_context3(NULL);
    avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[0]->codecpar);
    pCodecCtx->codec_id = fmt->video_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecCtx->width = t->s_en.width;
    pCodecCtx->height = t->s_en.height;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;
    pCodecCtx->bit_rate = 400000;
    pCodecCtx->gop_size = 12;
    if (pCodecCtx->codec_id == AV_CODEC_ID_H264) {
        pCodecCtx->qmin = 10;
        pCodecCtx->qmax = 51;
        pCodecCtx->qcompress = 0.6;
    }
    if (pCodecCtx->codec_id == AV_CODEC_ID_MPEG2VIDEO)
        pCodecCtx->max_b_frames = 2;
    if (pCodecCtx->codec_id == AV_CODEC_ID_MPEG1VIDEO)
        pCodecCtx->mb_decision = 2;

    // find the encoder
    pCodec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!pCodec) {
        fprintf(t->log, "no right encoder!\n");
        goto err;
    }

    // open the encoder
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        fprintf(t->log, "open encoder fail!\n");
        goto err;
    }

    // output the video format
    //av_dump_format(pFormatCtx, 0, out_fileName, 1);

    // the frame before encode
    pictureFrame = av_frame_alloc();
    if (!pictureFrame) {
        fprintf(t->log, "av_frame_alloc fail!\n");
        goto err;
    }
    pictureFrame->width = pCodecCtx->width;
    pictureFrame->height = pCodecCtx->height;
    pictureFrame->format = pCodecCtx->pix_fmt;

    // ffmpeg4.0
    size = av_image_get_buffer_size(pCodecCtx->pix_fmt, pCodecCtx->width,
                                        pCodecCtx->height, 1);

    picture_buf = (uint8_t *)av_malloc(size);
    av_image_fill_arrays(pictureFrame->data, pictureFrame->linesize, picture_buf,
                         AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height,
                         1);

    ret = avformat_write_header(pFormatCtx, NULL);
    if (ret) {
        fprintf(t->log, "Unexpected write_header failure!\n");
        goto err;
    }

    // the packet after encode
    pkt = av_packet_alloc();
    if (!pkt) {
        fprintf(t->log, "av_packet_alloc fail!\n");
        goto err;
    }

    // the size of Context
    y_size = pCodecCtx->width*pCodecCtx->height;

    // process per frame
    gettimeofday(&beginTv, NULL);
    for (int i = 0; i < t->s_en.frame_num; i++) {
        // read the YUV data
        if (fread(picture_buf, 1, y_size * 3 / 2, in_file) <= 0) {
            fprintf(t->log, "read yuv file fail!\n");
            goto err;
        } else if (feof(in_file)) {
            break;
        }
        pictureFrame->data[0] = picture_buf;                     // Y
        pictureFrame->data[1] = picture_buf + y_size;            // U
        pictureFrame->data[2] = picture_buf + y_size * 5 / 4;    // V
        pictureFrame->pts = i;
        // encode the image
        encode(pCodecCtx, pictureFrame, pkt, out_file);
        allFrameNum++;
    }
    gettimeofday(&endTv, NULL);
    allTime = (endTv.tv_sec * 1000 + endTv.tv_usec / 1000) -
              (beginTv.tv_sec * 1000 + beginTv.tv_usec / 1000);

    fprintf(t->log, "\tTest file: \t%s to %s \n", t->s_en.in_file,
            t->s_en.out_file);
    fprintf(t->log, "\tEncoded all frame:\t%lld \n", allFrameNum);
    fprintf(t->log, "\tEncoded all time:\t%lld (ms) \n", allTime);
    fprintf(t->log, "\tEncoded average time:\t%lld (ms) per frame \n",
            allTime / allFrameNum);

    // flush the encoder
    encode(pCodecCtx, NULL, pkt, out_file);
    av_write_trailer(pFormatCtx);
    fprintf(t->log, "soft_encode_h264===================================out\n");

    av_frame_free(&pictureFrame);
    av_packet_free(&pkt);
    avcodec_free_context(&pCodecCtx);
    av_free(picture_buf);
    avformat_free_context(pFormatCtx);
    fclose(out_file);
    fclose(in_file);

    return 0;

err:
    if (pictureFrame)
        av_frame_free(&pictureFrame);
    if (picture_buf)
        av_free(picture_buf);
    if (pCodecCtx)
        avcodec_free_context(&pCodecCtx);
    if (pFormatCtx)
        avformat_free_context(pFormatCtx);
    if (out_file)
        fclose(out_file);
    if (in_file)
        fclose(in_file);

    return -1;
}


// time ffmpeg -i <file_name> -f rawvideo -y /dev/null -an
// h264 to yuv
int soft_decode_yuv(struct term *t)
{
    fprintf(t->log, "soft_decode_yuv===================================enter\n");
    fprintf(t->log, "Soft Decode Result: \n");

#ifdef USE_OUTPUT_IMAGE
    Display *display = XOpenDisplay(getenv("DISPLAY"));
    Window  win;
    XImage *ximage;
#endif

    int                 ret;
    AVFormatContext     *pFormatCtx;
    AVCodec             *pCodec;
    AVCodecContext      *videoCodecCtx;
    AVPacket            *packet;
    AVFrame             *pFrameYUV;
    AVFrame             *pFrame;
    struct SwsContext   *img_convert_ctx;
    int                 video_index;
    unsigned char       *video_out_buffer = NULL;
    int                 got_picture;
    unsigned long long  allTime = 0;
    unsigned long long  allFrameNum = 0;
    struct timeval      beginTv;
    struct timeval      endTv;

    avcodec_register_all();
    av_register_all();

#ifdef USE_OUTPUT_IMAGE
    if (display == NULL) {
        fprintf(t->log, "Can't open the xserver\n");
        goto error;
    }
    win = XCreateSimpleWindow(display,
                              RootWindow(display, DefaultScreen(display)),
                              0, 0, 1, 1, 0,
                              BlackPixel(display, DefaultScreen(display)),
                              BlackPixel(display, DefaultScreen(display)));
    XMapWindow(display, win);
    XSync(display, False);
#endif

    pFormatCtx = avformat_alloc_context();
    pFrame = av_frame_alloc();
    pFrameYUV = av_frame_alloc();
    packet= (AVPacket *)av_malloc(sizeof(AVPacket));

    if (avformat_open_input(&pFormatCtx, t->video_file, NULL, NULL)!=0){
        fprintf(t->log, "Couldn't open input stream.\n");
        goto error;
    }

    // find stream
    if (avformat_find_stream_info(pFormatCtx,NULL)<0) {
        fprintf(t->log, "Couldn't find stream information.");
        goto error;
    }

    video_index = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO,
                                      video_index, -1, NULL, 0);
    if (video_index >= 0) {
        videoCodecCtx = pFormatCtx->streams[video_index]->codec;
        pCodec = avcodec_find_decoder(videoCodecCtx->codec_id);
        if (pCodec == NULL) {
            fprintf(t->log, "codec not found\n");
            goto error;
        }

        if (avcodec_open2(videoCodecCtx, pCodec, NULL) < 0) {
            fprintf(t->log, "can't open the videoCodecCtx\n");
            goto error;
        }

        video_out_buffer = (unsigned char *)av_malloc(
                            avpicture_get_size(AV_PIX_FMT_RGB32,
                                               videoCodecCtx->width,
                                               videoCodecCtx->height));
        avpicture_fill((AVPicture *)pFrameYUV,
                       (const uint8_t *)video_out_buffer,
                       AV_PIX_FMT_RGB32,
                       videoCodecCtx->width,
                       videoCodecCtx->height);

        img_convert_ctx = sws_getContext(videoCodecCtx->width,
                                         videoCodecCtx->height,
                                         videoCodecCtx->pix_fmt,
                                         videoCodecCtx->width,
                                         videoCodecCtx->height,
                                         AV_PIX_FMT_RGB32,
                                         SWS_BICUBIC, NULL, NULL, NULL);
    }

    // dump the audio format info
    //av_dump_format(pFormatCtx,0, 0, 0);

#ifdef USE_OUTPUT_IMAGE
    XResizeWindow(display, win, videoCodecCtx->width, videoCodecCtx->height);
    XSync(display, False);
#endif

    gettimeofday(&beginTv, NULL);
    while (av_read_frame(pFormatCtx, packet) >= 0) {
        if (packet->stream_index == video_index) {
            ret = avcodec_decode_video2(videoCodecCtx, pFrame,
                                        &got_picture, packet);
            if (ret < 0) {
                fprintf(t->log, "Decode Error\n");
                goto error;
            }
            if (got_picture) {
                sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data,
                          pFrame->linesize, 0, videoCodecCtx->height,
                          pFrameYUV->data, pFrameYUV->linesize);

#ifdef USE_OUTPUT_IMAGE
    ximage = XCreateImage(display,
                          DefaultVisual(display, DefaultScreen(display)),
                          DefaultDepth(display, DefaultScreen(display)),
                          ZPixmap, 0, pFrameYUV->data[0], pFrame->width,
                          pFrame->height, 8, pFrameYUV->linesize[0]);

    XPutImage (display, win, DefaultGC(display, 0), ximage, 0, 0, 0, 0,
               pFrame->width, pFrame->height);
    XFlush(display);
#endif
            }
            allFrameNum++;
            //fprintf(t->log, "soft decoded frame %lld cnt\n", allFrameNum);
        }
        av_free_packet(packet);
    }
    gettimeofday(&endTv, NULL);
    allTime = (endTv.tv_sec * 1000 + endTv.tv_usec / 1000) -
              (beginTv.tv_sec * 1000 + beginTv.tv_usec / 1000);

    fprintf(t->log, "\tTest file: \t%s \n", t->video_file);
    fprintf(t->log, "\tDecoded all frame:\t%lld \n", allFrameNum);
    fprintf(t->log, "\tDecoded all time:\t%lld (ms) \n", allTime);
    fprintf(t->log, "\tDecoded average time:\t%lld (ms) per frame \n",
            allTime / allFrameNum);

#ifdef USE_OUTPUT_IMAGE
    XDestroyImage(ximage);
#endif
    fprintf(t->log, "soft_decode_yuv===================================out\n");

    sws_freeContext(img_convert_ctx);
    av_frame_free(&pFrameYUV);
    av_frame_free(&pFrame);
    avcodec_close(videoCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;

error:
    if (img_convert_ctx)
        sws_freeContext(img_convert_ctx);
    if (pFrameYUV)
        av_frame_free(&pFrameYUV);
    if (pFrame)
        av_frame_free(&pFrame);
    if (videoCodecCtx)
        avcodec_close(videoCodecCtx);
    if (pFormatCtx)
        avformat_close_input(&pFormatCtx);

    return -1;
}


