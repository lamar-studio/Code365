
# Desc
  This Demo is separate from the WebRTC Audio Processing Module, Only contains the NS function
  Support the samplerate: 8K, 16K, 32K. You can modify the macro "SAMPLERATE" to config it.
  The Default policy level is 3. You can modify the macro "POLICY" to config it.

# Dev Environment
  You should config the cmake build environment first.
```
sudo apt install cmake
sudo apt install g++
```

# Build
1. mkdir build
2. cd build
3. cmake ..
4. make

# Usage
1. ./webrtcDenoise in.pcm out.pcm

## How to get source code
The whole module code
```
git clone git://anongit.freedesktop.org/pulseaudio/webrtc-audio-processing
```
