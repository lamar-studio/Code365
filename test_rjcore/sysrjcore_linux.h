#ifndef __SYSRJCORE_LINUX_H__
#define __SYSRJCORE_LINUX_H__

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef int(*callback)(const char *eventName, int eventType, const char *eventContent);
int Init(const char *comName, callback cb);

/* application */
int isProcessRunning(const char *processname);
int startProcess(const char *processname);
int installDeb_block(const char *debPath);
int uninstallDeb_block(const char *debPath);
int mergeDeltaPacket_block(const char *old, const char *delta, const char *newpack);

/* control */
int controlInit();
int getBrightness();
int setBrightness(int brightness);
int isSupportBrightness();
int startPaService();
int getVoiceVolume();
int setVoiceVolume(int volume);
int getHdmiVoiceStatus();
int setHdmiVoiceStatus(int status);
int setDeviceResolution(const char *res, int refresh);
int getCurrentResolution(char *retbuf, size_t size);
int getSupportResolution(char *retbuf, size_t size);
int getOptimumResolution(char *retbuf, size_t size);
int shutDown();
int reboot();
int setPowerState(int powerState);
int getPowerState();
int setSleeptime(int time);
int getHostname(char *hostname, size_t size);
int setHostname(const char *hostname);
int syncServerTime(const char *serverip);
int setLanguage(const char *language);
int startConsole();
int getUsbPathForOffine(char *retbuf, size_t size);
int mountUsb(const char *dev, char *retbuf, size_t size);
int getUsbList(char *retbuf, size_t size);
int unmountUsb(const char *path);

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

/* sysmisc */
int getPlatHWInfo(char *info, int len);
int getDiskStatus_block();
int getPersonalDiskList_block(char *list, int len);
int formatPersonalDisk_block(const char *dev);
int mountPersonalDisk_block(const char *dev,const char *dir, const char*option);
int umountPersonalDisk_block(const char *dir);
int checkISOVersion(const char *verstr);
int fastUpgrade(const char*jsonmsg);
int ipxeUpgrade(const char *verstr);

#ifdef __cplusplus
}
#endif

#endif /*__SYSRJCORE_LINUX_H__*/
