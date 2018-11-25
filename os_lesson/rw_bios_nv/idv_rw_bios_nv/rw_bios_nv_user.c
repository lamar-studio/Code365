#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/ioctl.h>
#include "bios_nv.h"
#include "rw_bios_nv.h"

#define DEVICE_NAME "/dev/RW_BIOS_NV"
void bios_nv_read_user(unsigned int nv_name)
{
    int fd;
    int cmd;
    BIOS_VAR_STRUCTURE get_var_struct;

    get_var_struct.var_name = nv_name;

    system("insmod rw_bios_nv.ko");
    fd=open(DEVICE_NAME,O_RDWR);
    if(fd == -1){
        perror("open");
		goto exit;
    }
	 
    cmd = RW_BIOS_NV_IOCGETBIOSNV;

    if (ioctl(fd, cmd, &get_var_struct) < 0)
    {
        printf("Call RW_BIOS_NV_IOCGETBIOSNV fail\n");
		goto exit;
    } 	

   if(get_var_struct.RetStatus==1){
         //call successfull
        printf("ac:%d rtcwake:%d h:%d m:%d s:%d inc:%d\n",get_var_struct.var_value, get_var_struct.var_value_RtcWake, get_var_struct.var_value_hour, get_var_struct.var_value_minute,
		get_var_struct.var_value_second, get_var_struct.var_value_MinIncrease);
   }else {
	   	printf("%s error\n", __FUNCTION__);
   }
  
exit:    	
    close(fd); 
	system("rmmod rw_bios_nv.ko");
}


void bios_nv_write_user(unsigned int nv_name,char *argv[])
{
    int fd;

	int cmd;
	BIOS_VAR_STRUCTURE set_var_struct;
	
	set_var_struct.var_name = nv_name;
    set_var_struct.var_value = atoi(argv[1]);
    set_var_struct.var_value_RtcWake = atoi(argv[2]);
    set_var_struct.var_value_hour = atoi(argv[3]);
    set_var_struct.var_value_minute = atoi(argv[4]);
    set_var_struct.var_value_second = atoi(argv[5]);
    set_var_struct.var_value_MinIncrease = atoi(argv[6]);

	system("insmod rw_bios_nv.ko");		   
    fd=open(DEVICE_NAME,O_RDWR);
    if(fd == -1){
        perror("open");
		goto exit;
     }			
    //fisrt read type length
    cmd = RW_BIOS_NV_IOCSETBIOSNV;

    if (ioctl(fd, cmd, &set_var_struct) < 0)
    {
        printf("Call RW_BIOS_NV_IOCSETBIOSNV fail\n");
    }
	
	if(set_var_struct.RetStatus!=1){
		printf("Call RW_BIOS_NV_IOCSETBIOSNV fail\n");
	}
	
exit:		 
	close(fd);
	system("rmmod rw_bios_nv.ko");
	return;
}


int main(int argc,char *argv[]){
   bios_nv_read_user(AC_POWER_VAR_NAME);
   if (argc > 6) {
     bios_nv_write_user(AC_POWER_VAR_NAME,argv);
     bios_nv_read_user(AC_POWER_VAR_NAME);
   }
}

