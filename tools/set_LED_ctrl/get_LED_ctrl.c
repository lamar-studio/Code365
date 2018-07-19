#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define GREEN 0
#define RED   1
#define BLUE  2

int main(int argc,char *argv[])
{
    unsigned char color = 0xAA;
    int ctrl = 0xAA;
    int dev_fd;
    dev_fd = open("/dev/led",O_RDWR | O_NONBLOCK);
    if ( dev_fd == -1 ) {
	printf("Cann't open file /dev/led\n");
	exit(1);
    }
    //printf("argc = %d\n", argc);
    if ( argc == 2)
    {
	color = *argv[1]-'0';		
	if (color <= 2)
	{
    	    ctrl = ioctl(dev_fd, color + 9, 0);
	    printf("led status is %d\n", ctrl);
	}
        else
    	    printf("color id is fault: %d\n", color);
    }
    else
	printf("fault param \n");
    return 0;
}
