#define MODULE_TAG    "product"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include "rj_commom.h"

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#define GETSYSTEMINFO_SERIALNUM	    "serialnum="
#define GETSYSTEMINFO_PRODUCTID	    "productid="
#define GETSYSTEMINFO_PRODUCTNAME	"productname="
#define GETSYSTEMINFO_HDVERSION	    "hdversion="
#define GETSYSTEMINFO_OSVERSION	    "osversion="
#define GETSYSTEMINFO_PLATFORM		"platform="
#define GETSYSTEMINFO_CPU		    "cpu="
#define GETSYSTEMINFO_BIT		    "bit="
#define GETSYSTEMINFO_MEMORY	    "memory="
#define GETSYSTEMINFO_BIOSVERSION   "biosversion="
#define GETSYSTEMINFO_SYSSIZE       "syssize="
#define GETSYSTEMINFO_DATASIZE      "datasize="
#define LOG_FILE_PATH               "/tmp/log/"
#define DATA_PARTTION               "/opt/lessons"
#define DATA_SPACE                  "data"
#define SYSTEM_SPACE                "system"

typedef struct tag_rj_systeminfo_t {
	char serial_number[64]; 	    //eg. xxxxxxx
	char product_id[64];		    //eg. 80060011
	char product_name[64];		    //eg. RG-Rain400W(128)
	char hardware_version[64];	    //eg. V1.00
	char os_version[64];	        //eg. RCC_RainOS_V3.3_R0.6
	char cpu[64];			        //eg. Inte(R) Core(TM) i3-6100U CPU @ 2.30GHz
	char memory[64];		        //eg. 4024232 kB
	char bit[64];                   //eg. 64
    char bios_version[64];          //eg. RAIN3032
    char platform_type[64];         //eg. IDV
    char sys_size[64];              //eg. 500106788kB
    char data_size[64];             //eg. 31268536kB
} rj_systeminfo_t;

static rj_systeminfo_t sysInfo;

/**
 * Function: rc_getsysteminfo
 * Description: read system information
 * Return:	-1 read failed
 * 			 0 read ok. maybe some field cann't read as permission, and this field will return null string
 */
static int rc_getsysteminfo(rj_systeminfo_t *system_info) {
	if (system_info == NULL) {
		rjlog_error("invalid system_info pt");
		return ERROR_10000;
	}

    if (access(RJ_SCRIPT_PATH "getSysInfo", F_OK) != 0) {
    	rjlog_error("getSysInfo script Not found please Check.");
        return ERROR_10000;
    }

	char cmd[512] = {0};
    char res[1024] = {0};
    char *pt = NULL;
	size_t n = 0;

    snprintf(cmd, sizeof(cmd), "bash " RJ_SCRIPT_PATH "getSysInfo");
	rj_exec_result(cmd, res, sizeof(res));
    if (res[0] == '\0') {
        rjlog_error("cmd:%s the result is empty.", cmd);
        return ERROR_10000;
    }

	rjlog_debug("\n==================INFO=======================\n"
                "%s=============================================", res);

#define GETSYSTEMINFO_STRXXX(tag, to)									            \
    if ((pt = strstr(res, tag)) != NULL) {									        \
	    pt = pt + strlen(tag);												        \
	    n = rj_strncp_line(system_info->to, pt, sizeof(system_info->to));	        \
	    if (n < sizeof(system_info->to)) {                                          \
            system_info->to[n] = '\0';                                              \
        } else {                                                                    \
            system_info->to[sizeof(system_info->to) - 1] = '\0';                    \
        }                                                                           \
    } else {																	    \
	    system_info->to[0] = '\0';												    \
    }

	GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_SERIALNUM, serial_number);
	GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_PRODUCTID, product_id);
	GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_PRODUCTNAME, product_name);
	GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_HDVERSION, hardware_version);
	GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_OSVERSION, os_version);
	GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_PLATFORM, platform_type);
	GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_CPU, cpu);
	GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_MEMORY, memory);
	GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_BIT, bit);
	GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_BIOSVERSION, bios_version);
	GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_SYSSIZE, sys_size);
	GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_DATASIZE, data_size);
#undef GETSYSTEMINFO_STRXXX

	return 0;
}

int productInit()
{
    CHECK_FUNCTION_IN();
    int ret = 0;

    memset(&sysInfo, 0, sizeof(rj_systeminfo_t));
    ret = rc_getsysteminfo(&sysInfo);
    if(ret != 0) {
        rjlog_error("rc_getsysteminfo error, ret = %d", ret);
    }

    return ret;
}

int getProductType(char *type, size_t size)
{
    CHECK_INIT_STR(type, size);
    CHECK_FUNCTION_IN();
    if (size > sizeof(sysInfo.product_name)) {
        size = sizeof(sysInfo.product_name);
    }

    strncpy(type, sysInfo.product_name, size);

    return 0;
}

int getAbsLayerVersion(char *version, size_t size)
{
    CHECK_INIT_STR(version, size);
    CHECK_FUNCTION_IN();
    if (size > (strlen(STRINGIZE_VALUE_OF(VERSION)) + 1)) {
        size = strlen(STRINGIZE_VALUE_OF(VERSION)) + 1;
    }

    strncpy(version, STRINGIZE_VALUE_OF(VERSION), size);

    return 0;
}

int getSystemVersion(char *sysVer, size_t size)
{
    CHECK_INIT_STR(sysVer, size);
    CHECK_FUNCTION_IN();
    if (size > sizeof(sysInfo.os_version)) {
        size = sizeof(sysInfo.os_version);
    }

    strncpy(sysVer, sysInfo.os_version, size);

    return 0;
}

int getHardwareVesion(char *hardVer, size_t size)
{
    CHECK_INIT_STR(hardVer, size);
    CHECK_FUNCTION_IN();
    if (size > sizeof(sysInfo.hardware_version)) {
        size = sizeof(sysInfo.hardware_version);
    }

    strncpy(hardVer, sysInfo.hardware_version, size);

    return 0;
}

int getProductId(char *id, size_t size)
{
    CHECK_INIT_STR(id, size);
    CHECK_FUNCTION_IN();
    if (size > sizeof(sysInfo.product_id)) {
        size = sizeof(sysInfo.product_id);
    }

    strncpy(id, sysInfo.product_id, size);

    return 0;
}

int getSerialNumber(char *serial, size_t size)
{
    CHECK_INIT_STR(serial, size);
    CHECK_FUNCTION_IN();
    if (size > sizeof(sysInfo.serial_number)) {
        size = sizeof(sysInfo.serial_number);
    }

    strncpy(serial, sysInfo.serial_number, size);

    return 0;
}

int getBiosInfo(char *bios, size_t size)
{
    CHECK_INIT_STR(bios, size);
    CHECK_FUNCTION_IN();
    if (size > sizeof(sysInfo.bios_version)) {
        size = sizeof(sysInfo.bios_version);
    }

    strncpy(bios, sysInfo.bios_version, size);

    return 0;
}

int getCpuInfo(char *cpu, size_t size)
{
    CHECK_INIT_STR(cpu, size);
    CHECK_FUNCTION_IN();
    if (size > sizeof(sysInfo.cpu)) {
        size = sizeof(sysInfo.cpu);
    }

    strncpy(cpu, sysInfo.cpu, size);

    return 0;
}

int getMemoryInfo(char *mem, size_t size)
{
    CHECK_INIT_STR(mem, size);
    CHECK_FUNCTION_IN();
    if (size > sizeof(sysInfo.memory)) {
        size = sizeof(sysInfo.memory);
    }

    strncpy(mem, sysInfo.memory, size);

    return 0;
}

int getBaseboardInfo(char *retbuf, size_t size)
{
    CHECK_INIT_STR(retbuf, size);
    CHECK_FUNCTION_IN();
    //TODO: implement in the next Version.
    return 0;
}

int getSystemBit(char *retbuf, size_t size)
{
    CHECK_INIT_STR(retbuf, size);
    CHECK_FUNCTION_IN();
    if (size > sizeof(sysInfo.bit)) {
        size = sizeof(sysInfo.bit);
    }

    strncpy(retbuf, sysInfo.bit, size);

    return 0;
}

int getDiskInfo(const char *type, char *retbuf, size_t size)
{
    //unit: kB [1kB = 1000B]
    CHECK_INIT_STR(retbuf, size);
    CHECK_FUNCTION_IN();
    CHECK_STR_PARA(type);
    int len = 0;
    char cmd[64] = {0};
    char res[64] = {0};

    if (strncmp(type, DATA_SPACE, sizeof(DATA_SPACE)) == 0) {
        snprintf(cmd, sizeof(cmd), "df -BKB %s | awk 'NR>1{print $4}'", DATA_PARTTION);
        rj_exec_result(cmd, res, sizeof(res));
        if (res[0] == '\0') {
            rjlog_warn("the result is empty.");
        }

        len = strlen(res);
        if (res[len - 1] == '\n') {
            res[len - 1] = '\0';
        }

        snprintf(retbuf, size, "Total:%s Available:%s", sysInfo.data_size, res);

    } else if (strncmp(type, SYSTEM_SPACE, sizeof(SYSTEM_SPACE)) == 0) {
        snprintf(cmd, sizeof(cmd), "df -BKB / | awk 'NR>1{print $4}'");
        rj_exec_result(cmd, res, sizeof(res));
        if (res[0] == '\0') {
            rjlog_warn("the result is empty.");
        }

        len = strlen(res);
        if (res[len - 1] == '\n') {
            res[len - 1] = '\0';
        }

        snprintf(retbuf, size, "Total:%s Available:%s", sysInfo.sys_size, res);

    } else {
        rjlog_warn("unknow type:%s", type);
    }

    return 0;
}

int getTermialType(char *retbuf, size_t size)
{
    CHECK_INIT_STR(retbuf, size);
    CHECK_FUNCTION_IN();
    if (size > sizeof(sysInfo.platform_type)) {
        size = sizeof(sysInfo.platform_type);
    }

    strncpy(retbuf, sysInfo.platform_type, size);

    return 0;
}

int collectLog_block(char *fileName, char *retbuf, size_t size)
{
    CHECK_INIT_STR(retbuf, size);
    CHECK_FUNCTION_IN();
    CHECK_STR_PARA(fileName);
    char cmd_buf[1024] = {0};
    char result[128] = {0};
    int len = 0;

    sprintf(cmd_buf, "bash " RJ_SCRIPT_PATH "collet_log.sh %s ", fileName);
    if (rj_exec_result(cmd_buf, result, sizeof(result)) < 0) {
        rjlog_error("collectLog_block fail");
        return ERROR_10000;
    }

    if (size > sizeof(result)) {
        size = sizeof(result);
    }

    len = strlen(result);
    if (result[len - 1] == '\n') {
        result[len - 1] = '\0';
    }
    strncpy(retbuf, result, size);

    return 0;
}

int rmLogFile(const char *path)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    char cmd_buf[1024] = {0};

    if (strncmp(path, LOG_FILE_PATH, strlen(LOG_FILE_PATH)) != 0) {
    	rjlog_error("the path:%s invalid", path);
        return ERROR_10000;
    }

    if (access(path, F_OK) != 0) {
    	rjlog_warn("no found the file:%s ", path);
        return 0;
    }

    sprintf(cmd_buf, "rm %s", path);
    if (rj_system(cmd_buf) != 0) {
    	rjlog_error("rm LogFile fail");
        return ERROR_10000;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif
