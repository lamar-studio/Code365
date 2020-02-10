
1. 编译方法
  1)在当前目录下直接make即可
  2)如果想让解码的数据显示出来需要把Makefile中如下行的注释去掉
  CFLAGS += -DUSE_OUTPUT_IMAGE
  
2. 使用方法
  1)将解码的时间数据重定向到log文件中
  ./softDecode > log
  2)解析log文件中的平均解码时间
  testDecodeSpeed.sh log

