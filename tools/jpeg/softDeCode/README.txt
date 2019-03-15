
编译方法：
1. 需要安装libjpeg开发库
  sudo apt-get install libjpeg62-dev
2. gcc编译
  gcc -o jpt_bmp jpt_bmp.c -ljpeg
  
使用方法：
1. 执行
./jpt_bmp 1920x1080_100.jpg 1.bmp
2. 根据log确认解码时间
---------------------------------------
"1920x1080_100.jpg" Covert to "1.bmp"
width:	1920
height:	1080
depth:	3
JPEG Decode:	42885us      # 解码时间
Creat 1.bmp finish!
---------------------------------------

