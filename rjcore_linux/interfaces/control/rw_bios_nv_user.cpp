#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "rj_commom.h"
#include "rw_bios_nv_user.h"

#define DEVICE_NAME "/dev/RW_BIOS_NV"

void vdi_bios_nv_read_user(unsigned int nv_name, unsigned int *nv_value)
{
    int fd;
    int cmd;
    GET_VAR_STRUCTURE get_var_struct;

    get_var_struct.var_name = nv_name;
    get_var_struct.var_value = 0;
    get_var_struct.RetStatus = 0;

    rj_system("insmod /usr/local/bin/rw_bios_nv.ko");
    fd = open(DEVICE_NAME,O_RDWR);
    if (fd == -1) {
		rjlog_error("open:%s error", DEVICE_NAME);
		goto exit;
     }

    cmd = VDI_RW_BIOS_NV_IOCGETBIOSNV;

    if (ioctl(fd, cmd, &get_var_struct) < 0) {
        rjlog_error("Call RW_BIOS_NV_IOCGETBIOSNV fail\n");
		goto exit;
    }

    //call successfull
    if (get_var_struct.RetStatus == 1) {
        *nv_value = get_var_struct.var_value;
        rjlog_info("read_nv_value_:%x\n", *nv_value);
    }

exit:
    close(fd);
    rj_system("rmmod /usr/local/bin/rw_bios_nv.ko");

    return;
}

void vdi_bios_nv_write_user(unsigned int nv_name, unsigned int nv_value)
{
    int fd;
	int cmd;
	SET_VAR_STRUCTURE set_var_struct;

	set_var_struct.var_name = nv_name;
    set_var_struct.var_value = nv_value;

    rj_system("insmod /usr/local/bin/rw_bios_nv.ko");
    fd = open(DEVICE_NAME,O_RDWR);
    if (fd == -1) {
		rjlog_error("open:%s error", DEVICE_NAME);
		goto exit;
     }

    //fisrt read type length
    cmd = VDI_RW_BIOS_NV_IOCSETBIOSNV;

    if (ioctl(fd, cmd, &set_var_struct) < 0) {
        rjlog_error("Call RW_BIOS_NV_IOCSETBIOSNV fail\n");
		goto exit;
    }

	if (set_var_struct.RetStatus != 1) {
		rjlog_error("Call RW_BIOS_NV_IOCSETBIOSNV fail\n");
		goto exit;
	}

exit:
    close(fd);
	rj_system("rmmod /usr/local/bin/rw_bios_nv.ko");
	return;
}

void idv_bios_nv_read_user(unsigned int nv_name, unsigned int *nv_value)
{
    int fd;
    int cmd;
    BIOS_VAR_STRUCTURE get_var_struct;

    get_var_struct.var_name = nv_name;

    rj_system("insmod /usr/local/bin/rw_bios_nv.ko");
    fd = open(DEVICE_NAME, O_RDWR);
    if (fd == -1) {
        rjlog_error("open:%s fail", DEVICE_NAME);
		goto exit;
    }

    cmd = IDV_RW_BIOS_NV_IOCGETBIOSNV;

    if (ioctl(fd, cmd, &get_var_struct) < 0) {
        rjlog_error("Call RW_BIOS_NV_IOCGETBIOSNV fail\n");
		goto exit;
    }

   if (get_var_struct.RetStatus == 1) {
        //call successfull
        rjlog_info("ac:%d rtcwake:%d h:%d m:%d s:%d inc:%d\n",
                   get_var_struct.var_value,
                   get_var_struct.var_value_RtcWake,
                   get_var_struct.var_value_hour,
                   get_var_struct.var_value_minute,
                   get_var_struct.var_value_second,
                   get_var_struct.var_value_MinIncrease);

        *nv_value = get_var_struct.var_value;
   } else {
	   	rjlog_error("get_var_struct.RetStatus error");
   }

exit:
    close(fd);
	rj_system("rmmod /usr/local/bin/rw_bios_nv.ko");
}


void idv_bios_nv_write_user(unsigned int nv_name, unsigned int nv_value)
{
    int fd;
	int cmd;
    BIOS_VAR_STRUCTURE set_var_struct;

    set_var_struct.var_name = nv_name;

	rj_system("insmod /usr/local/bin/rw_bios_nv.ko");
    fd = open(DEVICE_NAME,O_RDWR);
    if (fd == -1) {
        perror("open");
        goto exit;
    }

    cmd = IDV_RW_BIOS_NV_IOCGETBIOSNV;

    if (ioctl(fd, cmd, &set_var_struct) < 0) {
        rjlog_error("Call RW_BIOS_NV_IOCGETBIOSNV fail");
        goto exit;
    }

    if (set_var_struct.RetStatus == 1) {
         //call successfull
         rjlog_info("get => ac:%d rtcwake:%d h:%d m:%d s:%d inc:%d",
                    set_var_struct.var_value,
                    set_var_struct.var_value_RtcWake,
                    set_var_struct.var_value_hour,
                    set_var_struct.var_value_minute,
                    set_var_struct.var_value_second,
                    set_var_struct.var_value_MinIncrease);

        //fisrt read type length
        cmd = IDV_RW_BIOS_NV_IOCSETBIOSNV;
        set_var_struct.var_value = nv_value;
        rjlog_info("set => ac:%d", nv_value);

        if (ioctl(fd, cmd, &set_var_struct) < 0) {
            rjlog_error("Call RW_BIOS_NV_IOCSETBIOSNV fail");
            goto exit;
        }

    	if (set_var_struct.RetStatus != 1) {
    		rjlog_error("Call RW_BIOS_NV_IOCSETBIOSNV fail");
            goto exit;
    	}
    } else {
	   	rjlog_error("Call RW_BIOS_NV_IOCGETBIOSNV fail");
    }

exit:
	close(fd);
	rj_system("rmmod /usr/local/bin/rw_bios_nv.ko");
	return;
}

#if 0
// idv
int main(int argc,char *argv[]){
   bios_nv_read_user(AC_POWER_VAR_NAME);
   if (argc > 6) {
     bios_nv_write_user(AC_POWER_VAR_NAME,argv);
     bios_nv_read_user(AC_POWER_VAR_NAME);
   }
}

//vdi
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
#endif

