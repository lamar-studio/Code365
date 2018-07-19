#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    unsigned char color = 0xAA;
    unsigned char ctrl = 0xAA;
    int dev_fd;
    dev_fd = open("/dev/led",O_RDWR | O_NONBLOCK);
    if ( dev_fd == -1 ) {
	printf("Cann't open file /dev/led\n");
	exit(1);
    }
    if (argc>=2)
    {
	color = *argv[1]-'0';
        ctrl = *argv[2]-'0';
	if ((color <= 2) && (ctrl <= 1))
	{
	    //printf("led ctrl param : color = %d, ctrl = %d\n", color, ctrl);
	    ioctl(dev_fd, 4 + color * 2 - ctrl, 0);
	    //printf("2 + color * 2 - ctrl = %d\n", 2 + color * 2 - ctrl);
	}
	else
	{
	    printf("led ctrl param is fault: color = %d, ctrl = %d\n", color, ctrl);
	}
    }
    else
	printf("input no param\n");
    return 0;
}
