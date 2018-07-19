#include "base.h"
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define STRESS_STAT	("./res/stress")
#define PROC_STAT	    ("/proc/stat")
#define INTERFACE_STAT	("./res/interface")
#define CPU_FREQ_FILE   ("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq")

#define ERROR_LOG_PATH  "/sdcard/log/factory_test.log"
#define ERROR_LOG_BAK   "/sdcard/log/factory_test_old.log"
#define SIMPLE_LOG_PATH "/sdcard/log/ft_simple.log"
#define SIMPLE_LOG_BAK  "/sdcard/log/ft_simple_old.log"
#define DETAIL_LOG_PATH "/sdcard/log/ft_detail.log"
#define DETAIL_LOG_BAK  "/sdcard/log/ft_detail_old.log"
#define LOG_MAX_SIZE    (4 << 20)
#define LOG_BUF_SIZE    512
#define LOG_FNAME_SZ    64

int g_log_type = LOG_SIMPLE;
write_screen g_write_screen_cb = NULL;

static void write_screen_log(char *log, char *screen_file, char *screen_bak)
{
    FILE *fp;
    long file_size;

    fp = fopen(screen_file, "a+");
    if (fp == NULL) {
        LOG_ERROR("Failed to open screen log file %s\n", screen_file);
        return;
    }
    fprintf(fp, "%s", log);
    fflush(fp);
    file_size = ftell(fp);
    fclose(fp);
    if (file_size >= LOG_MAX_SIZE) {
        rename(screen_file, screen_bak);
    }
}

void write_simple_log(const char *fmt, ...)
{
    va_list args;
	char *buf;

    /* backup last simple log file & create a new one. */
    static int first_write = 1;
    if (first_write) {
        char ft_log_file[LOG_FNAME_SZ];
        struct stat ft_stat;
        time_t now = time(NULL);

        if (stat(SIMPLE_LOG_PATH, &ft_stat) == 0) {
            snprintf(ft_log_file, LOG_FNAME_SZ, "/sdcard/log/ft_simple_%u.log", now);
            rename(SIMPLE_LOG_PATH, ft_log_file);
        }
        if (stat(SIMPLE_LOG_BAK, &ft_stat) == 0) {
            snprintf(ft_log_file, LOG_FNAME_SZ, "/sdcard/log/ft_simple_old_%u.log", now);
            rename(SIMPLE_LOG_BAK, ft_log_file);
        }
        first_write = 0;
    }

	buf = (char *)malloc(LOG_BUF_SIZE);
	if (buf == NULL) {
        LOG_ERROR("Failed to alloc memory(size %d) for simple log.\n", LOG_BUF_SIZE);
		return;
	}

	va_start(args, fmt);
	vsnprintf(buf, LOG_BUF_SIZE, fmt, args);
	va_end(args);

    write_screen_log(buf, SIMPLE_LOG_PATH, SIMPLE_LOG_BAK);
    write_screen_log(buf, DETAIL_LOG_PATH, DETAIL_LOG_BAK);
    free(buf);
}

void write_detail_log(const char *fmt, ...)
{
    va_list args;
	char *buf;

    buf = (char *)malloc(LOG_BUF_SIZE);
	if (buf == NULL) {
        LOG_ERROR("Failed to alloc memory(size %d) for detail log.\n", LOG_BUF_SIZE);
		return;
	}

	va_start(args, fmt);
	vsnprintf(buf, LOG_BUF_SIZE, fmt, args);
	va_end(args);

    write_screen_log(buf, DETAIL_LOG_PATH, DETAIL_LOG_BAK);
    free(buf);
}

void write_log(int type, const char *filename, const char *format, ...) {
#if 0
	time_t now;
	va_list args;
	long file_size = 0;
	struct tm* pnowtime;
	FILE* file_fd = NULL;
	char tmpbuf[100] = { 0, };

	file_fd = fopen(ERROR_LOG_PATH, "a+");

	if (NULL != file_fd) {
		time(&now);
		pnowtime = localtime(&now);
		strftime(tmpbuf, sizeof tmpbuf, "%Y-%m-%d %H:%M:%S", pnowtime);

		fprintf(file_fd, "%s %s:%d ", tmpbuf, filename,
				line);

		va_start(args, format);
		vfprintf(file_fd, format, args);
		va_end(args);

		fflush(file_fd);
		file_size = ftell(file_fd);
		fclose(file_fd);

		if (file_size >= LOG_MAX_SIZE) {
			rename(ERROR_LOG_PATH, ERROR_LOG_BAK);
		}
	}
#else
	va_list args;
	char *buf;

	buf = (char *)malloc(LOG_BUF_SIZE);
	if (buf == NULL) {
		LOG_ERROR("Failed to alloc memory(size %d) for detail log.\n", LOG_BUF_SIZE);
		return;
	}

	va_start(args, format);
	vsnprintf(buf, LOG_BUF_SIZE, format, args);
	va_end(args);
	if (type == ANDROID_LOG_INFO)
		LOG_INFO("LOG_INFO: %s\t", buf);
	else if(type == ANDROID_LOG_ERROR)
		LOG_ERROR("LOG_ERROR: %s\t", buf);

	write_screen_log(buf, filename, DETAIL_LOG_BAK);
	free(buf);
#endif
}

int get_file_size(char* filename, int* size) {

	FILE* infile = NULL;

	if ((infile = fopen(filename, "rb")) == NULL) {
		LOG_ERROR("can't open %s\n", filename);
		return FAIL;
	}

	// get file size
	fseek(infile, 0L, SEEK_END);
	*size = ftell(infile);

	fclose(infile);

	return SUCCESS;
}

int write_local_data(char* filename, char* mod, uint8* buf, int size) {
	int i = 0;
	int count = 0;
	FILE * outfile = NULL;

	if ((outfile = fopen(filename, mod)) == NULL) {
		LOG_ERROR("can't open %s\n", filename);
		return FAIL;
	}

	count = fwrite(buf, size, 1, outfile);
	if (count != 1) {
		LOG_ERROR("write data failed file=%s count=%d size=%d\n", filename,
				count, size);
		fclose(outfile);
		return FAIL;
	}

	fflush(outfile);
	fclose(outfile);

	return SUCCESS;
}

int read_local_data(char* filename, uint8* buf, int size) {
	int ret = 0;
	int file_len = 0;
	FILE * infile = NULL;

	if ((infile = fopen(filename, "rb")) == NULL) {
		LOG_ERROR("can't open %s\n", filename);
		return FAIL;
	}

	ret = fread(buf, size, 1, infile);
	if (ret != 1) {
		LOG_ERROR("read file failed %s size=%d\n", filename, size);
		fclose(infile);
		return FAIL;
	}

	fclose(infile);
	return SUCCESS;
}


int remove_local_file(char* filename){
    if (NULL == filename){
        return SUCCESS;
    }
    
    return remove(filename);
}

int check_file_exit(char* filename){
    
    if (NULL == filename){
        return SUCCESS;
    }

    if (0 == access(filename, F_OK)){
        return SUCCESS;
    }
    
    return FAIL;
}

uint64 get_current_time() {
	struct timeval now;
	gettimeofday(&now, NULL);

	return now.tv_sec * 1000000 + now.tv_usec;
}

void register_type_and_cb(int type, write_screen cb) {

	g_log_type = type;
	g_write_screen_cb = cb;
}

void write_data_screen(int type, char *format, ...) {

	va_list args;
	char *buf = NULL;

	buf = (char *)malloc(LOG_BUF_SIZE);
	if (buf == NULL) {
		return;
	}

	va_start(args, format);
	vsnprintf(buf, LOG_BUF_SIZE, format, args);
	va_end(args);

	LOG_INFO("LOG_INFO: %s\n", buf);
    /* free(buf); --> async, buf must be freed later */
}

int is_digit(char *str) {

	int ret = 0;
	size_t len = 0;

	len = strlen(str);
	if (0 == len) {
		return FAIL;
	}

	while (len > 0) {
		if (*str < '0' || *str > '9') {
			return FAIL;
		}
		str++;
		len--;
	}

	return SUCCESS;
}

int get_mem_info(struct sysinfo* si) {

	int ret = 0;

	ret = sysinfo(si);
	if (-1 == ret) {
		LOG_ERROR("get mem info failed\n");
		return FAIL;
	}

	return SUCCESS;
}

void get_current_date(TIME_INFO_T* date) {

	time_t now;
	struct tm* time_now;

	time(&now);
	time_now = localtime(&now);

	date->day = time_now->tm_mday;
	date->hour = time_now->tm_hour;
	date->minute = time_now->tm_min;
	date->second = time_now->tm_sec;
}

void diff_date(TIME_INFO_T* dst, TIME_INFO_T* src) {

	if (dst->second < src->second) {
		dst->second += 60;
		dst->minute -= 1;
	}
	dst->second -= src->second;
	dst->second %= 60;

	if (dst->minute < src->minute) {
		dst->minute += 60;
		dst->hour -= 1;
	}
	dst->minute -= src->minute;
	dst->minute %= 60;

	if (dst->hour < src->hour) {
		dst->hour += 24;
		dst->day -= 1;
	}
	dst->hour -= src->hour;
	dst->hour %= 24;

	dst->day -= src->day;
}

int get_cpu_info(CPU_STATUS_T* st_cpu) {

	FILE* fp = NULL;
	char line[8192] = { 0, };

	if ((fp = fopen(PROC_STAT, "r")) == NULL) {
		LOG_ERROR("open %s failed\n", PROC_STAT);
		return FAIL;
	}

	memset(st_cpu, 0, sizeof(CPU_STATUS_T));

	while (fgets(line, sizeof(line), fp) != NULL) {
		if (!strncmp(line, "cpu ", 4)) {

			sscanf(line + 5,
					"%llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
					&st_cpu->cpu_user, &st_cpu->cpu_nice, &st_cpu->cpu_sys,
					&st_cpu->cpu_idle, &st_cpu->cpu_iowait,
					&st_cpu->cpu_hardirq, &st_cpu->cpu_softirq,
					&st_cpu->cpu_steal, &st_cpu->cpu_guest,
					&st_cpu->cpu_guest_nice);
			break;
		}
	}

	fclose(fp);

	return SUCCESS;
}

void write_interface_record(ITF_RECORD_T* record) {

	FILE* fp = NULL;
	char line[8192] = { 0, };

	if ((fp = fopen(INTERFACE_STAT, "w+")) == NULL) {
		LOG_ERROR("open %s failed\n", INTERFACE_STAT);
		return;
	}

	sprintf(line, "%d %d %d %d %d %d %d %d %d %d", record->begin_time,
			record->end_time, record->mem_fail, record->usb_fail,
			record->net_fail, record->edid_fail, record->mem_total,
			record->usb_total, record->net_total, record->edid_total);

	fwrite(line, strlen(line) + 1, 1, fp);
	fclose(fp);
}

void read_interface_record(ITF_RECORD_T* record) {

	FILE* fp = NULL;
	char line[8192] = { 0, };

	memset(record, 0, sizeof(ITF_RECORD_T));
	time(&record->begin_time);
	record->end_time = record->begin_time;

	if ((fp = fopen(INTERFACE_STAT, "r")) == NULL) {
		LOG_ERROR("open %s failed\n", INTERFACE_STAT);
		return;
	}

	while (fgets(line, sizeof(line), fp) != NULL) {
		sscanf(line, "%d %d %d %d %d %d %d %d %d %d", &record->begin_time,
				&record->end_time, &record->mem_fail, &record->usb_fail,
				&record->net_fail, &record->edid_fail, &record->mem_total,
				&record->usb_total, &record->net_total, &record->edid_total);
	}

	fclose(fp);
}

void write_stress_record(STRESS_RECORD_T* record, int index, int num) {

	int i = 0;
	FILE* fp = NULL;
	char line[8192] = { 0, };

	if ((fp = fopen(STRESS_STAT, "wb+")) == NULL) {
		LOG_ERROR("open %s failed\n", STRESS_STAT);
		return;
	}

	for (i = 0; i < num; i++) {

		sprintf(line, "%d %d %d %d %d %d %d\n",  record[index].date.day, record[index].date.hour,
				record[index].date.minute, record[index].date.second,
				record[index].encode, record[index].decode, record[index].result);

		index++;
		index %= STRESS_RECORD_NUM;
		fwrite(line, strlen(line), 1, fp);
	}

	fclose(fp);
}

void read_stress_record(STRESS_RECORD_T* record, int* num) {

	int i = 0;
	int ret = 0;
	FILE* fp = NULL;
	char line[8192] = { 0, };

	*num = 0;

	if ((fp = fopen(STRESS_STAT, "r")) == NULL) {
		LOG_ERROR("open %s failed\n", STRESS_STAT);
		return;
	}

	while (fgets(line, sizeof(line), fp) != NULL) {
		memset(&record[i], 0, sizeof(STRESS_RECORD_T));

		ret = sscanf(line, "%d %d %d %d %d %d %d\n", &record[i].date.day,
				&record[i].date.hour, &record[i].date.minute, &record[i].date.second,
				&record[i].encode, &record[i].decode, &record[i].result);

		i++;
	}

	*num = i;

	fclose(fp);
}


void print_screen_test_result(ITF_RECORD_T* record) {

	int total = 0;
	char end[64] = { 0, };
	char begin[64] = { 0, };
	struct tm *timenow = NULL;

	total = record->mem_fail + record->usb_fail + record->net_fail
			+ record->edid_fail;

#if 0
	timenow = localtime(&record->begin_time);
	sprintf(begin, "%d-%02d-%02d %02d:%02d:%02d", timenow->tm_year + 1900,
			timenow->tm_mon, timenow->tm_mday, timenow->tm_hour,
			timenow->tm_min, timenow->tm_sec);

	timenow = localtime(&record->end_time);
	sprintf(end, "%d-%02d-%02d %02d:%02d:%02d", timenow->tm_year + 1900,
			timenow->tm_mon, timenow->tm_mday, timenow->tm_hour,
			timenow->tm_min, timenow->tm_sec);
#else
	timenow = localtime(&record->begin_time);
    strftime(begin, 64, "%Y-%m-%d %H:%M:%S", timenow);
    
	timenow = localtime(&record->end_time);
    strftime(end, 64, "%Y-%m-%d %H:%M:%S", timenow);
#endif

	SCREEN_SIMPLE("----------------------------------------------");
	SCREEN_SIMPLE("----------------------------------------------\n");
	SCREEN_SIMPLE(" \t\t\tWelcome to Factory Test Software\t\t\t\n");
	SCREEN_SIMPLE("----------------------------------------------");
	SCREEN_SIMPLE("----------------------------------------------\n");
	SCREEN_SIMPLE("\t\t\t\t\t\t[%s]\n", PRINT_RESULT1(total));
	SCREEN_SIMPLE("==========================================\n");
	SCREEN_SIMPLE("Begin Time:%s\t End Time;%s\n\n", begin, end);
	SCREEN_SIMPLE("================ Auto Test Result ================\n");

    if (0 != record->mem_total){
	    SCREEN_SIMPLE("memory\t\t\t\t %s(Times:%d, Errors:%d)\n",
		    	PRINT_RESULT1(record->mem_fail), record->mem_total, record->mem_fail);
    }

    if (0 != record->usb_total){
    	SCREEN_SIMPLE("usb\t\t\t\t\t %s(Times:%d, Errors:%d)\n",
    			PRINT_RESULT1(record->usb_fail), record->usb_total, record->usb_fail);
    }

    if (0 != record->net_total){
    	SCREEN_SIMPLE("netcard\t\t\t\t %s(Times:%d, Errors:%d)\n",
    			PRINT_RESULT1(record->net_fail), record->net_total, record->net_fail);
    }

    if (0 != record->edid_total){
    	SCREEN_SIMPLE("EDID\t\t\t\t %s(Times:%d, Errors:%d)\n",
    			PRINT_RESULT1(record->edid_fail), record->edid_total, record->edid_fail);
    }
    
	SCREEN_SIMPLE("----------------------------------------------");
	SCREEN_SIMPLE("----------------------------------------------\n");
}

void print_stress_test_result(STRESS_RECORD_T* record, int index, int num) {

	int i = 0;
	char line[8192] = { 0, };

	SCREEN_SIMPLE("The last Stress test result is...\n");
	SCREEN_SIMPLE("===================Stress Test Result===================\n");

	for (i = 0; i < num; i++) {

		sprintf(line, "%s 运行时间:%d天%d小时%d分%d秒 编码状态:%s 解码状态:%s\n",
				PRINT_RESULT1(record[index].result),
				record[index].date.day, record[index].date.hour,
				record[index].date.minute, record[index].date.second,
				PRINT_RESULT1(record[index].encode),
				PRINT_RESULT1(record[index].decode));

		index++;
		index %= STRESS_RECORD_NUM;
		SCREEN_SIMPLE("%s", line);
	}

	SCREEN_SIMPLE("================================================\n");
}

#if 0
void get_qr_data_by_string(int type, uint8* data, char* file_name){

	struct zint_symbol* my_symbol = NULL;

	my_symbol = ZBarcode_Create();

	if (IMAGE_BAR_CODE == type){
		my_symbol->symbology = BARCODE_CODE128;
	}else if (IMAGE_QR_CODE == type){
		my_symbol->symbology = BARCODE_QRCODE;
	}

	strncpy(my_symbol->outfile, file_name, strlen(file_name));

	ZBarcode_Encode_and_Print(my_symbol, data, strlen(data), 0);

	ZBarcode_Delete(my_symbol);
}
#endif

int get_cpu_freq_by_id(int id){

    int ret = 0;
	int size = 0;
	char* tmp = NULL;
	char cmd[128] = {0,};
	char buf[128] = {0, };

	sprintf(cmd, "cat /sys/devices/system/cpu/cpu%d/cpufreq/cpuinfo_cur_freq > ./cpu_freq ", id);
	system(cmd);

	ret = get_file_size("./cpu_freq", &size);
	if (FAIL == ret){
		return 0;
	}

	ret = read_local_data("./cpu_freq", buf, size);
	if (FAIL == ret){
		return 0;
	}

	// strip /n
	tmp = strchr(buf, '\n');
	if (NULL == tmp){
		return 0;
	}
	*tmp = '\0';

	ret = is_digit(buf);
	if (FAIL == ret){
		return 0;
	}

	return atoi(buf);
    
}

int get_current_cpu_freq(){

    int i = 0;
    int ret = 0;
    int size = 0;
    int cpu_cur = 0;
    int cpu_max = 0;
	char cmd[128] = {0,};
	char buf[128] = {0,};

    sprintf(cmd, "cat /proc/cpuinfo| grep processor| wc -l > ./cpu_num");
	system(cmd);

	ret = get_file_size("./cpu_num", &size);
	if (FAIL == ret){
		return 0;
	}

	ret = read_local_data("./cpu_num", buf, size);
	if (FAIL == ret){
		return 0;
	}
	
	for (i = 0; i < atoi(buf); i++){
        cpu_cur = get_cpu_freq_by_id(i);

        if (cpu_max < cpu_cur){
            cpu_max = cpu_cur;
        }        
    }

    return cpu_max;
}

