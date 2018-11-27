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

#define GETSYSTEMINFO_SERIALNUM     "serialnum="
#define GETSYSTEMINFO_PRODUCTID     "productid="
#define GETSYSTEMINFO_PRODUCTNAME   "productname="
#define GETSYSTEMINFO_HDVERSION     "hdversion="
#define GETSYSTEMINFO_SYSVERSION    "sysversion="
#define GETSYSTEMINFO_PLATFORM      "platform="
#define GETSYSTEMINFO_CPU           "cpu="
#define GETSYSTEMINFO_BIT           "bit="
#define GETSYSTEMINFO_MEMORY        "memory="
#define GETSYSTEMINFO_BIOSVERSION   "biosversion="
#define GETSYSTEMINFO_SYSSIZE       "syssize="
#define GETSYSTEMINFO_DATASIZE      "datasize="
#define LOG_FILE_PATH               "/tmp/log/"
#define DATA_PARTITION              "/opt/lessons"
#define DATA_SPACE                  "data"
#define SYSTEM_SPACE                "system"

typedef struct tag_rj_systeminfo_t {
    char serial_number[SIZE_64];        //eg. xxxxxxx
    char product_id[SIZE_64];           //eg. 80060011
    char product_name[SIZE_64];         //eg. RG-Rain400W(128)
    char hardware_version[SIZE_64];     //eg. V1.00
    char os_version[SIZE_64];           //eg. RCC_RainOS_V3.3_R0.6
    char cpu[SIZE_64];                  //eg. Inte(R) Core(TM) i3-6100U CPU @ 2.30GHz
    char memory[SIZE_64];               //eg. 4024232 kB
    char bit[SIZE_64];                  //eg. 64
    char bios_version[SIZE_64];         //eg. RAIN3032
    char platform_type[SIZE_64];        //eg. IDV
    char sys_size[SIZE_64];             //eg. 500106788kB
    char data_size[SIZE_64];            //eg. 31268536kB
} rj_systeminfo_t;

static rj_systeminfo_t sysInfo;

/**
 * Function: rc_getsysteminfo
 * Description: read system information
 * Return:   nozero read failed
 *           0 read ok. maybe some field cann't read as permission, and this field will return null string
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

    char cmd[SIZE_512] = {0};
    char res[SIZE_1K] = {0};
    char *pt = NULL;
    size_t n = 0;

    snprintf(cmd, sizeof(cmd), "bash " RJ_SCRIPT_PATH "getSysInfo");
    rj_exec_result(cmd, res, sizeof(res));
    if (res[0] == '\0') {
        rjlog_error("cmd:%s the result is empty.", cmd);
        return ERROR_10000;
    }

#define GETSYSTEMINFO_STRXXX(tag, to)                                               \
    if ((pt = strstr(res, tag)) != NULL) {                                          \
        pt = pt + strlen(tag);                                                      \
        n = rj_strncp_line(system_info->to, pt, sizeof(system_info->to));           \
        if (n < sizeof(system_info->to)) {                                          \
            system_info->to[n] = '\0';                                              \
        } else {                                                                    \
            system_info->to[sizeof(system_info->to) - 1] = '\0';                    \
        }                                                                           \
    } else {                                                                        \
        system_info->to[0] = '\0';                                                  \
    }

    GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_SERIALNUM, serial_number);
    GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_PRODUCTID, product_id);
    GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_PRODUCTNAME, product_name);
    GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_HDVERSION, hardware_version);
    GETSYSTEMINFO_STRXXX(GETSYSTEMINFO_SYSVERSION, os_version);
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
    char type[SIZE_64] = {0};

    memset(&sysInfo, 0, sizeof(rj_systeminfo_t));
    ret = rc_getsysteminfo(&sysInfo);
    if(ret != 0) {
        rjlog_error("rc_getsysteminfo error, ret = %d", ret);
        return ERROR_10001;
    }
    rjlog_debug("==================INFO=======================");
    rjlog_debug("getProductType:%s", sysInfo.product_name);
    rjlog_debug("getAbsLayerVersion:%s", STRINGIZE_VALUE_OF(VERSION));
    rjlog_debug("getSystemVersion:%s", sysInfo.os_version);
    rjlog_debug("getHardwareVesion:%s", sysInfo.hardware_version);
    rjlog_debug("getProductId:%s", sysInfo.product_id);
    rjlog_debug("getSerialNumber:%s", sysInfo.serial_number);
    rjlog_debug("getBiosInfo:%s", sysInfo.bios_version);
    rjlog_debug("getCpuInfo:%s", sysInfo.cpu);
    rjlog_debug("getMemoryInfo:%s", sysInfo.memory);
    rjlog_debug("getSystemBit:%s", sysInfo.bit);
    rjlog_debug("getSysDiskInfo:%s", sysInfo.sys_size);
    rjlog_debug("getTermialType:%s", sysInfo.platform_type);
    rjlog_debug("==================INFO=======================");

    snprintf(type, sizeof(type), "%s", sysInfo.platform_type);
    ret = startPaService(type);
    if (ret < 0) {
        rjlog_error("startPaService error, ret = %d", ret);
    }

    return ret;
}

int getProductType(char *type, size_t size)
{
    CHECK_INIT_STR(type, size);
    CHECK_FUNCTION_IN();

    snprintf(type, size, "%s", sysInfo.product_name);

    return SUCCESS_0;
}

int getAbsLayerVersion(char *version, size_t size)
{
    CHECK_INIT_STR(version, size);
    CHECK_FUNCTION_IN();

    snprintf(version, size, "%s", STRINGIZE_VALUE_OF(VERSION));

    return SUCCESS_0;
}

int getSystemVersion(char *sysVer, size_t size)
{
    CHECK_INIT_STR(sysVer, size);
    CHECK_FUNCTION_IN();

    snprintf(sysVer, size, "%s", sysInfo.os_version);

    return SUCCESS_0;
}

int getHardwareVesion(char *hardVer, size_t size)
{
    CHECK_INIT_STR(hardVer, size);
    CHECK_FUNCTION_IN();

    snprintf(hardVer, size, "%s", sysInfo.hardware_version);

    return SUCCESS_0;
}

int getProductId(char *id, size_t size)
{
    CHECK_INIT_STR(id, size);
    CHECK_FUNCTION_IN();

    snprintf(id, size, "%s", sysInfo.product_id);

    return SUCCESS_0;
}

int getSerialNumber(char *serial, size_t size)
{
    CHECK_INIT_STR(serial, size);
    CHECK_FUNCTION_IN();

    snprintf(serial, size, "%s", sysInfo.serial_number);

    return SUCCESS_0;
}

int getBiosInfo(char *bios, size_t size)
{
    CHECK_INIT_STR(bios, size);
    CHECK_FUNCTION_IN();

    snprintf(bios, size, "%s", sysInfo.bios_version);

    return SUCCESS_0;
}

int getCpuInfo(char *cpu, size_t size)
{
    CHECK_INIT_STR(cpu, size);
    CHECK_FUNCTION_IN();

    snprintf(cpu, size, "%s", sysInfo.cpu);

    return SUCCESS_0;
}

int getMemoryInfo(char *mem, size_t size)
{
    CHECK_INIT_STR(mem, size);
    CHECK_FUNCTION_IN();

    snprintf(mem, size, "%s", sysInfo.memory);

    return SUCCESS_0;
}

int getBaseboardInfo(char *retbuf, size_t size)
{
    CHECK_INIT_STR(retbuf, size);
    CHECK_FUNCTION_IN();
    //TODO: implement in the next Version.
    return SUCCESS_0;
}

int getSystemBit(char *retbuf, size_t size)
{
    CHECK_INIT_STR(retbuf, size);
    CHECK_FUNCTION_IN();

    snprintf(retbuf, size, "%s", sysInfo.bit);

    return SUCCESS_0;
}

int getDiskInfo(const char *type, char *retbuf, size_t size)
{
    //unit: kB [1kB = 1000B]
    CHECK_INIT_STR(retbuf, size);
    CHECK_FUNCTION_IN();
    CHECK_STR_PARA_STR(type, retbuf, size);
    int len = 0;
    char cmd[SIZE_64] = {0};
    char res[SIZE_64] = {0};

    if (strncmp(type, DATA_SPACE, strlen(DATA_SPACE)) == 0) {
        if (access(DATA_PARTITION, F_OK) != 0) {
            rjlog_warn("Not Found the data partition");
            snprintf(retbuf, size, "Total:empty Available:empty");
            return SUCCESS_0;
        }

        snprintf(cmd, sizeof(cmd), "df -BKB %s | awk 'NR>1{print $4}'", DATA_PARTITION);
        rj_exec_result(cmd, res, sizeof(res));
        if (res[0] == '\0') {
            rjlog_warn("the result is empty.");
            snprintf(retbuf, size, "Total:%s Available:empty", sysInfo.data_size);
            return SUCCESS_0;
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
            snprintf(retbuf, size, "Total:%s Available:empty", sysInfo.sys_size);
            return SUCCESS_0;
        }

        len = strlen(res);
        if (res[len - 1] == '\n') {
            res[len - 1] = '\0';
        }

        snprintf(retbuf, size, "Total:%s Available:%s", sysInfo.sys_size, res);

    } else {
        rjlog_warn("unknow type:%s", type);
    }

    return SUCCESS_0;
}

int getTermialType(char *retbuf, size_t size)
{
    CHECK_INIT_STR(retbuf, size);
    CHECK_FUNCTION_IN();

    snprintf(retbuf, size, "%s", sysInfo.platform_type);

    return SUCCESS_0;
}

int collectLog_block(char *fileName, char *retbuf, size_t size)
{
    CHECK_INIT_STR(retbuf, size);
    CHECK_FUNCTION_IN();
    CHECK_STR_PARA_STR(fileName, retbuf, size);
    char cmd_buf[SIZE_1K] = {0};
    char result[SIZE_128] = {0};
    int len = 0;

    sprintf(cmd_buf, "bash " RJ_SCRIPT_PATH "collet_log.sh %s ", fileName);
    rj_exec_result(cmd_buf, result, sizeof(result));
    if (result[0] == '\0') {
        rjlog_error("collectLog_block fail");
        return ERROR_10000;
    }

    len = strlen(result);
    if (result[len - 1] == '\n') {
        result[len - 1] = '\0';
    }
    snprintf(retbuf, size, "%s", result);

    return SUCCESS_0;
}

int rmLogFile(const char *path)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();
    CHECK_STR_PARA(path);
    char cmd_buf[SIZE_128] = {0};

    if (strncmp(path, LOG_FILE_PATH, strlen(LOG_FILE_PATH)) != 0) {
        rjlog_error("the path:%s invalid", path);
        return ERROR_10000;
    }

    if (access(path, F_OK) != 0) {
        rjlog_warn("no found the file:%s ", path);
        return SUCCESS_0;
    }

    snprintf(cmd_buf, sizeof(cmd_buf), "rm -f %s", path);
    if (rj_system(cmd_buf) != 0) {
        rjlog_error("rm LogFile fail");
        return ERROR_10000;
    }

    return SUCCESS_0;
}

#ifdef __cplusplus
}
#endif
