#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define GREEN_ON	1
#define GREEN_OFF 	2
#define RED_ON		3
#define RED_OFF 	4	
#define BLUE_ON		5
#define BLUE_OFF 	6

int main()
{
	int i = 0;
	int dev_fd;
	dev_fd = open("/dev/led",O_RDWR | O_NONBLOCK);
	
	if ( dev_fd == -1 ) {
		printf("Cann't open file /dev/led \n");
		exit(1);
	}
	while(1)
	{
	ioctl(dev_fd,RED_ON,0);
  //     printf("on\n");
	sleep(1);
	ioctl(dev_fd,RED_OFF,0);
//     printf("off\n");
	sleep(1);
	ioctl(dev_fd,GREEN_ON,0);
  //     printf("on\n");
	sleep(1);
	ioctl(dev_fd,GREEN_OFF,0);
//     printf("off\n");
	sleep(1);	
	ioctl(dev_fd,BLUE_ON,0);
  //     printf("on\n");
	sleep(1);
	ioctl(dev_fd,BLUE_OFF,0);
//     printf("off\n");
	sleep(1);
	}
	//getchar();
	return 0;
}
