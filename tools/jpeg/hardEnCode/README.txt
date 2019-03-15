
测试方法：
1. 将libva-1.7.0.zip拷贝至/root目录下，解压
2. 进入libva-1.7.0/install/bin目录
3. 执行 ./loadjpeg <需要测试的jpg图片>
4. 查看log中的时间

大体日志如下：
Decoding JPEG image 1920x1080...
libva info: VA-API version 0.39.0
libva info: va_getDriverName() returns 0
libva info: Trying to open /root/libva-1.7.0/install/lib/dri/i965_drv_video.so
libva info: Found init function __vaDriverInit_0_39
libva info: va_openDriver() returns 0
Decoding finished in:   49647us      # 该时间即为解码时间
Decoding finished in 32287 ticks


