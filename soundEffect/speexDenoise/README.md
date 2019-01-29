# Desc:
    This Demo only can process the PCM format sound data.
    Only support the samplerate: 16K,32K,44K;you can modify the macro "SAMPLERATE"
    You can use cmd: "pacat -r --rate=SAMPLERATE filename" to get the PCM file

# Dev Environment:
1. sudo apt-get install libspeexdsp-dev
2. after install the include file:
    /usr/include/speex/speex_preprocess.h

# Build:
1. gcc -o speexDenoise testdenoise.c -lspeexdsp

# Usage:
1. ./speexDenoise < in.pcm > out.pcm


