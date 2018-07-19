
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <ctype.h>

#define RKNAND_GET_SN_SECTOR       _IOW('d', 3, unsigned int)

#define RKNAND_GET_VENDOR_SECTOR0       _IOW('v', 16, unsigned int)
#define RKNAND_STORE_VENDOR_SECTOR0     _IOW('v', 17, unsigned int)

#define RKNAND_GET_VENDOR_SECTOR1       _IOW('v', 18, unsigned int)
#define RKNAND_STORE_VENDOR_SECTOR1     _IOW('v', 19, unsigned int)

#define SN_SECTOR_OP_TAG            0x41444E53 // "SNDA"
#define VENDOR_SECTOR_OP_TAG        0x444E4556 // "VEND"

#define RKNAND_SYS_STORGAE_DATA_LEN 512

typedef     unsigned short      uint16;
typedef     unsigned int        uint32;
typedef     unsigned char       uint8;

typedef struct tagRKNAND_SYS_STORGAE {
    uint32  tag;
    uint32  len;
    uint8   data[RKNAND_SYS_STORGAE_DATA_LEN];
} RKNAND_SYS_STORGAE;

static void rknand_print_hex_data(char *s, uint8 *buf, uint32 len)
{
    uint32          i;

	printf("%s", s);
	for (i = 0; i < len; i++) {
		printf("%c ", buf[i]);
	}
	printf("\n *** ");
	for (i = 0; i < len; i++) {
		if (i % 8 == 0)
			printf("\n");
		printf("%02X ", buf[i]);
	}
	printf(" ***\n");
}

/**
 * rknand_vendor_set_resolution - set resolution infomation to vendor sector
 *
 * totle 6 bits:
 *
 * 1 bit - flag£º0 kernel does nothing, 1 kernel set resolution from vendor sector
 * 2 bit - width
 * 2 bit - height
 * 1 bit - refresh frequency
 */
static int rknand_vendor_set_resolution(uint16 xres, uint16 yres, uint8 refresh, uint8 flag)
{
    int             ret = 0;
    int             fd = 0;
    int             i = 0;
    RKNAND_SYS_STORGAE sysData;
    uint16          *p = NULL;

	fd = open("/dev/rknand_sys_storage", O_RDWR, 0);
	if (fd < 0) {
		perror("rknand_sys_storage open failed");
		return -1;
	}

	//read
	sysData.tag = VENDOR_SECTOR_OP_TAG;
	sysData.len = RKNAND_SYS_STORGAE_DATA_LEN - 8;
	ret = ioctl(fd, RKNAND_GET_VENDOR_SECTOR0, &sysData);
	if (ret < 0) {
		perror("get vendor_sector error");
		close(fd);
		return -1;
	}
/*	rknand_print_hex_data("vendor_sector load : ", (uint8 *) sysData.data, 32); */

	//write
	sysData.tag = VENDOR_SECTOR_OP_TAG;
	sysData.len = RKNAND_SYS_STORGAE_DATA_LEN - 8;

    sysData.data[1] = flag;
	if (flag) {
	    sysData.data[1] = 0xAC;         /* resolution flag */
		p = (uint16 *)&sysData.data[2];
		*p = xres;
		p = (uint16 *)&sysData.data[4];
		*p = yres;
		sysData.data[6] = refresh;
	}
/*	else {
	    sysData.data[2] = 0;
	    sysData.data[3] = 0;
	    sysData.data[4] = 0;
	    sysData.data[5] = 0;
	    sysData.data[6] = 0;
	}
	*/

	ret = ioctl(fd, RKNAND_STORE_VENDOR_SECTOR0, &sysData);
	if (ret < 0) {
		perror("set vendor_sector error");
		close(fd);
		return -1;
	}

	//read first 32.
	sysData.tag = VENDOR_SECTOR_OP_TAG;
	sysData.len = RKNAND_SYS_STORGAE_DATA_LEN - 8;
	ret = ioctl(fd, RKNAND_GET_VENDOR_SECTOR0, &sysData);
	if (ret < 0) {
		perror("get vendor_sector error");
		close(fd);
		return -1;
	}
/*	rknand_print_hex_data("vendor_sector load : ", (uint8 *) sysData.data, 32); */

	close(fd);
	return 0;
}

static int read_vender_resolution(void)
{
    int             fd = 0;
    int             ret = 0;
    RKNAND_SYS_STORGAE sysData;
    uint16          *p = NULL;
    uint8           flag = 0;
    uint16          xres = 0;
    uint16          yres = 0;
    uint8           refresh = 0;

    fd = open("/dev/rknand_sys_storage", O_RDWR, 0);
    if (fd < 0) {
        perror("rknand_sys_storage open failed");
        return -1;
    }

    //read
    sysData.tag = VENDOR_SECTOR_OP_TAG;
    sysData.len = RKNAND_SYS_STORGAE_DATA_LEN - 8;
    ret = ioctl(fd, RKNAND_GET_VENDOR_SECTOR0, &sysData);
    if (ret < 0) {
        perror("get vendor_sector error");
        close(fd);
        return -1;
    }
    flag = sysData.data[1];
    p = (uint16 *)&sysData.data[2];
    xres = *p;
    p = (uint16 *)&sysData.data[4];
    yres = *p;
    refresh = sysData.data[6];

    printf("%d\n%d\n%d\n%d\n", flag, xres, yres, refresh);

    close(fd);
    return 0;
}

static uint16 string_to_int16(char *str, int *err)
{
	uint16 res = 0;
	int i = 0;
	int abyte;

    *err = 0;
    for (i = 0; str[i] != '\0'; i++) {
        abyte = str[i] - '0';
        if (abyte < 0 || abyte > 9) {
            *err = 1;
        } else {
            res = res * 10 + abyte;
        }
    }

	return res;
}

#define HELP_INFO                               \
	"Usage:\n"                                  \
	"/system/bin/set_resolution (0 or 1) [other OPTION...]\n\n"\
	"option1:\n"                                \
	"1 - 0 or 1\n\n"                            \
	"other Options:\n"                          \
	"2 - width reslution(800 ~ 2560)\n"         \
	"3 - height resolution(600 ~ 1600)\n"       \
	"4 - refresh resolution\n\n"                \
	"example: \n"                               \
	"     /system/bin/set_resolution 1 1920 1080 60\n" \
	"    or\n"                                  \
    "    /system/bin/set_resolution 0\n"

#define ERROR_PARA  \
	"error paramater"

int main(int argc, char *argv[])
{
    uint8           flag = 0;
    uint16          xres = 0;
    uint16          yres = 0;
    uint16          ref_tmp = 0;
    uint8           refresh = 0;
    int             ret = 0, err = 0;

    /* parse paramater */
    switch (argc) {
    case 0:
	case 1:
    	/* no paramater */
        if (read_vender_resolution() < 0) {
            printf("error......\n");
        }
//		printf("%s", HELP_INFO);
		goto end;
	case 2:
	case 3:
    	flag = *argv[1] - '0';
    	if (flag != 0) {
    		printf("%s: flag:%d\n", ERROR_PARA, flag);
    		printf("%s\n", HELP_INFO);
    		goto end;
    	}
		break;
    case 4:
    	flag = *argv[1] - '0';
		xres = string_to_int16(argv[2], &err);
		yres = string_to_int16(argv[3], &err);
		refresh = 60;
		break;
    case 5:
    	flag = *argv[1] - '0';
		xres = string_to_int16(argv[2], &err);
		yres = string_to_int16(argv[3], &err);
		ref_tmp = string_to_int16(argv[4], &err);
		refresh = (uint8)ref_tmp;
		break;
    default:
    	break;
    }

    /* paramater check */
    if (err == 1){
        printf("%s: xres:%s, yres:%s\n", ERROR_PARA, argv[2], argv[3]);
        goto end;
    } else if (argc >= 4) {
		if ((xres < 800 || xres > 2560) || (yres < 600 || yres > 1600)) {
			printf("%s: xres:%d, yres:%d refresh:%d\n", ERROR_PARA, xres, yres, refresh);
			printf("%s\n", HELP_INFO);
			goto end;
		}
	}

	/* set to vendor */
	ret = rknand_vendor_set_resolution(xres, yres, refresh, flag);

	/* print result */
	if (ret < 0) {
        printf("%d\n", 0);
	} else {
        printf("%d\n", 1);
	}

end:
    return 0;
}
