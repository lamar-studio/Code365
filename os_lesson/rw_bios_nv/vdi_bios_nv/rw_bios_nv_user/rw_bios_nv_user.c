#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <linux/ioctl.h>
#include "bios_nv.h"
#include "rw_bios_nv.h"

#define DEVICE_NAME "/dev/RW_BIOS_NV"
void bios_nv_read_user(unsigned int nv_name,unsigned int *nv_value)
{
    int fd;
    int cmd;
    GET_VAR_STRUCTURE get_var_struct;
	  
    get_var_struct.var_name = nv_name;
    get_var_struct.var_value= 0;
    get_var_struct.RetStatus = 0;

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
	*nv_value = get_var_struct.var_value;
        printf("read_nv_value_:%x\n",*nv_value);
   }

	
  
exit:    	
    close(fd); 
	system("rmmod rw_bios_nv.ko");
}


void bios_nv_write_user(unsigned int nv_name,unsigned int nv_value)
{
    int fd;

	int cmd;
	SET_VAR_STRUCTURE set_var_struct;	  
	
	set_var_struct.var_name = nv_name;
    set_var_struct.var_value = nv_value;

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
   unsigned int nv_value;


   //set AC POWER LOSS to POWER ON
   printf("set BOX power on !!\n");
   bios_nv_write_user(AC_POWER_VAR_NAME,1);
   bios_nv_read_user(AC_POWER_VAR_NAME,&nv_value);

   //printf("set BOX power off !!\n");
   //set AC POWER LOSS to POWER OFF
   //bios_nv_write_user(AC_POWER_VAR_NAME,0);
   //bios_nv_read_user(AC_POWER_VAR_NAME,&nv_value);

}
