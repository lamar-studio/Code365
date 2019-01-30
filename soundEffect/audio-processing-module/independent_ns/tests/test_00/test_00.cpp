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

int main(int argc, char *argv[]) {
    /// input PCM:
    ///     SampleRate:16k
    ///     BitDepth:s16le(16bit)
    ///     duration 10ms (frame length)
    if (argc != 3) {
        std::cerr << "./test_00 s16le_16k_mono_in.pcm  s16le_16k_mono_out.pcm" << std::endl;
        return -1;
    }


    auto instance = WebRtcNsx_Create();
    if (WebRtcNsx_Init(instance, 16000)) {
        throw;
    }

    if (WebRtcNsx_set_policy(instance, 3)) {
        throw;
    }

    std::ifstream ifs(argv[1]);
    std::ofstream ofs(argv[2]);
    std::vector<short> buffer(16000/100);   //(16000 / 1000 * 10) - 一帧数据的采样点数,每个点有16bit数据量
    if (!ifs.is_open()) {
        std::cerr << "could not open input file" << std::endl;
        return -1;
    }
    if (!ofs.is_open()) {
        std::cerr << "could not open output file" << std::endl;
        return -1;
    }


    {
        std::vector<short> outBuffer(16000/100);

        for (int index = 0; !ifs.eof(); index++) {
            ifs.read((char *)(buffer.data()), buffer.size()*sizeof(short));
            auto temp = (short*)buffer.data();

            auto outTemp = outBuffer.data();
            WebRtcNsx_Process(instance, &temp, 1, &outTemp);

            ofs.write((char*)(outBuffer.data()), outBuffer.size()*sizeof(short));
        }

    }


    return 0;
}
