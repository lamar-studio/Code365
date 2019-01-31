/*
 * this demo is test for webrtc denoise function.
 *
 * Para:
 *    samplerate=16K, framesize=320, frametime=20ms
 *
 * Usage:
 *    ./speexDenoise < in.pcm > out.pcm
 *
 * Build:
 *    -lspeexdsp
 *
 * Author:
 *    @LaMar
 */

#include <iostream>
#include "noise_suppression_x.hpp"
#include <fstream>
#include <vector>

// this demo set the frametime = 10ms; samplerate = 16000
// framesize = (frametime*samplerate)/1000 (frametime_unit:ms)
#define FSIZE          (160)
#define SAMPLERATE     (16000)
#define POLICY         (3)

/// input PCM:
///     SampleRate:16k
///     BitDepth:s16le(16bit)
///     duration 10ms (frame length)
int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "./test_00 s16le_16k_mono_in.pcm  s16le_16k_mono_out.pcm" << std::endl;
        return -1;
    }

    auto instance = WebRtcNsx_Create();
    if (WebRtcNsx_Init(instance, SAMPLERATE)) {
        throw;
    }

    if (WebRtcNsx_set_policy(instance, POLICY)) {
        throw;
    }

    std::ifstream ifs(argv[1]);
    std::ofstream ofs(argv[2]);
    std::vector<short> buffer(FSIZE);
    std::vector<short> outBuffer(FSIZE);
    if (!ifs.is_open()) {
        std::cerr << "could not open input file" << std::endl;
        return -1;
    }
    if (!ofs.is_open()) {
        std::cerr << "could not open output file" << std::endl;
        return -1;
    }

    for (int index = 0; !ifs.eof(); index++) {
        ifs.read((char *)(buffer.data()), buffer.size()*sizeof(short));

        auto temp = (short*)buffer.data();
        auto outTemp = outBuffer.data();

        WebRtcNsx_Process(instance, &temp, 1, &outTemp);

        ofs.write((char*)(outBuffer.data()), outBuffer.size()*sizeof(short));
    }

    return 0;
}
