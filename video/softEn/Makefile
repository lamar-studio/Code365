FLAGS = -Wall -g -Wno-deprecated-declarations
INCLUDEPATH = -I/opt/video/x264/include -I/opt/video/ffmpeg/include
LIBPATH = -L/opt/video/x264/lib -L/opt/video/ffmpeg/lib
LIBS= -l avcodec    \
      -l pthread    \
      -l avutil     \
      -l m          \
      -l dl         \
      -l swresample \
      -l x264       \
      -l avformat   \

exe=yuv2h264

$(exe):
	gcc test_soft_encode.c  ${FLAGS}  ${INCLUDEPATH} ${LIBPATH} ${LIBS} -o $@

clean:
	rm -rf ${exe}
