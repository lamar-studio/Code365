#ifndef __SYSRJCORE_LINUX_H__
#define __SYSRJCORE_LINUX_H__

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef int(*callback)(const char *eventName, int eventType, const char *eventContent);
int Init(const char *comName, callback cb);
int control(const char *arg, char *retbuf, int len);

/* application */
int isProcessRunning(const char *processname);
int startProcess(const char *processname);
int installDeb_block(const char *debPath);
int uninstallDeb_block(const char *debPath);
int mergeDeltaPacket_block(const char *old, const char *delta, const char *newpack);

/* control */



/* product */
int productInit();
int getProductType(char *type, size_t size);
int getAbsLayerVersion(char *version, size_t size);
int getSystemVersion(char *sysVer, size_t size);
int getHardwareVesion(char *hardVer, size_t size);
int getProductId(char *id, size_t size);
int getSerialNumber(char *serial, size_t size);
int getBiosInfo(char *bios, size_t size);
int getCpuInfo(char *cpu, size_t size);
int getMemoryInfo(char *mem, size_t size);
int getBaseboardInfo(char *retbuf, size_t size);
int getSystemBit(char *retbuf, size_t size);
int getDiskInfo(const char *type, char *retbuf, size_t size);
int getTermialType(char *retbuf, size_t size);
int collectLog_block(char *fileName, char *retbuf, size_t size);
int rmLogFile(const char *path);

#ifdef __cplusplus
}
#endif

#endif /*__SYSRJCORE_LINUX_H__*/
