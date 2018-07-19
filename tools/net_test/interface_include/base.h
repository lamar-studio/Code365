#ifndef _BASE_H_
#define _BASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#include <zint.h>
#include <time.h>
#include <android/log.h>
#define LOG_TAG "factory_test"
#include <utils/Log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SUCCESS   (0)
#define FAIL      (1)
#define OUT       (2)
#define AGAIN     (3)

#define LOG_SIMPLE  (0)
#define LOG_DETAIL  (1)

#define PRINT_RESULT(x)  (x == SUCCESS ? "OK" : "FAIL")
#define PRINT_RESULT1(x) (x == SUCCESS ? "PASS" : "FAIL")

#define LOG_DIR "/sdcard/log/"

#define ALIGN_16(x)  (((x) + 15) & (~15))
#define STRESS_RECORD_NUM    (10)

#define IMAGE_BAR_CODE   (0)
#define IMAGE_QR_CODE    (1)

#define MAC_ADDR_LEN     (6)

/* Product ID definition */
#define PRODUCT_RAIN100_V1_1    0x80020011
#define PRODUCT_RAIN100_V1_3    0x80020013
#define PRODUCT_RAIN200_V1_01   0x80020021
#define PRODUCT_RAIN200_V1_10   0x80020022
#define PRODUCT_RAIN100S_V1_0   0x80020031
#define PRODUCT_RAIN100S_V2_0   0x80020032
#define PRODUCT_RAIN200S_V1_0   0x80020041
#define PRODUCT_RAIN200C_V1_0   0x80020051
#define PRODUCT_RAIN100V2_V1_0  0x80020061
#define PRODUCT_RAIN200V2_V1_0  0x80020071

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long int uint64;

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int int64;

typedef unsigned char volatile uint8_v;
typedef unsigned short volatile uint16_v;
typedef unsigned long volatile uint32_v;

typedef void (*write_screen)(char* data);

typedef struct tagTimeInfo {
	int day;
	int hour;
	int minute;
	int second;
} TIME_INFO_T;

typedef struct tagCpuStatus {
	uint64 cpu_user;
	uint64 cpu_nice;
	uint64 cpu_sys;
	uint64 cpu_idle;
	uint64 cpu_iowait;
	uint64 cpu_steal;
	uint64 cpu_hardirq;
	uint64 cpu_softirq;
	uint64 cpu_guest;
	uint64 cpu_guest_nice;
} CPU_STATUS_T;

typedef struct tagItfRecord {
	int loop;
	int mem_fail;
	int usb_fail;
	int net_fail;
	int edid_fail;

	int mem_total;
	int usb_total;
	int net_total;
	int edid_total;

	time_t end_time;
	time_t begin_time;
} ITF_RECORD_T;

typedef struct tagStressRecord {
	int result;
	int encode;
	int decode;
	TIME_INFO_T date;
} STRESS_RECORD_T;

extern int get_file_size(char* filename, int* size);
extern int write_local_data(char* filename, char* mod, uint8* buf, int size);
extern int read_local_data(char* filename, uint8* buf, int size);
extern int remove_local_file(char* filename);
extern int check_file_exit(char* filename);
extern uint64 get_current_time();

extern void write_data_screen(int type, char* format, ...);
extern void register_type_and_cb(int type, write_screen cb);

extern int is_digit(char *str);
extern int get_mem_info(struct sysinfo* si);
extern void get_current_date(TIME_INFO_T* date);
extern void diff_date(TIME_INFO_T* dst, TIME_INFO_T* src);
extern int get_cpu_info(CPU_STATUS_T* st_cpu);
extern void print_screen_test_result(ITF_RECORD_T* record);
extern void read_interface_record(ITF_RECORD_T* record);
extern void write_interface_record(ITF_RECORD_T* record);
extern void write_stress_record(STRESS_RECORD_T* record, int index, int num);
extern void read_stress_record(STRESS_RECORD_T* record, int* num);
extern void print_stress_test_result(STRESS_RECORD_T* record, int index, int num);
extern int get_current_cpu_freq();
extern void get_qr_data_by_string(int type, uint8* data, char* file_name);
extern void write_log(int type, const char* filename, const char* format, ...);
extern void write_simple_log(const char *fmt, ...);
extern void write_detail_log(const char *fmt, ...);

#define SCREEN_SIMPLE(format, args...)  \
    do {    \
        write_data_screen(LOG_SIMPLE, format, ##args);  \
        write_simple_log(format, ##args);   \
    } while (0)
#define SCREEN_DETAIL(format, args...)  \
    do {    \
        write_data_screen(LOG_DETAIL, format, ##args);  \
        write_detail_log(format, ##args);   \
    } while (0)

#define WRITE_INFO(filename, format, args...) write_log(ANDROID_LOG_INFO, filename, format, ##args )
#define WRITE_ERROR(filename, format, args...) write_log(ANDROID_LOG_ERROR, filename, format, ##args )

#define LOG_ERROR(...)  ALOGE(__VA_ARGS__)
#define LOG_INFO(...)   ALOGI(__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif
