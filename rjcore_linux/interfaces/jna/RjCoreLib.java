package com.ruijie.rcos.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public interface RjCoreLib extends Library {
    RjCoreLib INSTANCE = (RjCoreLib)Native.loadLibrary("sysrjcore_linux", RjCoreLib.class);

/* comomon start */
    public int Init(String comName, RjCallback cb);
/* common end */

/* control start */
    public int getBrightness();
    public int setBrightness(int brightness);
    public int isSupportBrightness();
    public int getVoiceVolume();
    public int setVoiceVolume(int volume);
    public int getHdmiVoiceStatus();
    public int setHdmiVoiceStatus(int status);
    public int setDeviceResolution(String res, int refresh);
    public int getSupportResolution(byte[] retbuf, int size);
    public int getCurrentResolution(byte[] retbuf, int size);
    public int getOptimumResolution(byte[] retbuf, int size);
    public int shutDown();
    public int reboot();
    public int setPowerState(int powerState);
    public int getPowerState();
    public int setSleeptime(int time);
    public int getHostname(byte[] retbuf, int size);
    public int setHostname(String hostname);
    public int syncServerTime(String serverip);
    public int setLanguage(String language);
    public int startConsole();
    public int getUsbPathForOffine(byte[] retbuf);
/* control end */

/* product start */
    public int getProductType(byte[] type, int size);
    public int getAbsLayerVersion(byte[] version, int size);
    public int getSystemVersion(byte[] sysVer, int size);
    public int getHardwareVesion(byte[] hardVer, int size);
    public int getProductId(byte[] id, int size);
    public int getSerialNumber(byte[] serial, int size);
    public int getBiosInfo(byte[] bios, int size);
    public int getCpuInfo(byte[] cpu, int size);
    public int getMemoryInfo(byte[] mem, int size);
    public int getBaseboardInfo(byte[] retbuf, int size);
    public int getSystemBit(byte[] retbuf, int size);
    public int getDiskInfo(String type, byte[] retbuf, int size);
    public int getTermialType(byte[] retbuf, int size);
    public int collectLog_block(String filename, byte[] retbuf, int size);
    public int rmLogFile(String path);
/* product end */

/* sysmisc start */
	public int getPlatHWInfo(byte[] info, int len);
	public int getDiskStatus_block();
	public int getPersonalDiskList_block(byte[] list, int len);
	public int formatPersonalDisk_block(String dev);
	public int mountPersonalDisk_block(String dev, String dir, String option);
	public int umountPersonalDisk_block(String dir);
	public int checkISOVersion(String verstr);
	public int fastUpgrade(String jsonmsg);
	public int ipxeUpgrade(String verstr);
/* sysmisc end */

/* bt start */
    public void BT_MakeSeed_block(String request, byte[] respone);
    public void BT_MakeSeed_cancel(String request, byte[] respone);
    public void BT_Share_Start (String request, byte[] respone);
    public void BT_Share_Stop (String request, byte[] respone);
    public void BT_Download_block(String request, byte[] respone);
    public void BT_Download_cancle(String request, byte[] respone);
    public void BT_Get_Status (String request, byte[] respone);
/* bt end */

/* application start */
    public int isProcessRunning(String processname);
    public int startProcess(String processname);
    public int installDeb_block(String debPath);
    public int uninstallDeb_block(String debPath);
    public int mergeDeltaPacket_block(String old, String delta, String newpack);
/* application end */

/* network start */
    public int network();
    public int getWiredMac(byte[] retbuf, int length);
    public int getNetStatus_block();
    public int getIPInfo_block(byte[] retbuf, int length);
    public int checkPingIp_block(String ip, byte[] retbuf, int length);
    public int getCurCardSpeed(byte[] retbuf, int length);
    public int getMaxCardSpeed();
    public int getOptionServerIp(int type, byte[] retbuf, int length);
    public int checkIpConflict_block(String ip, byte[] retbuf, int length);
    public int setDns_block(String info, byte[] retbuf, int length);
    public int setIP_block(String info, byte[] retbuf, int length);
    public int startFtpUpload_block(String uploadInfo);
    public int startFtpDownload_block(String downloadInfo, byte[] retbuf, int length);
/* network end */

/* log start */
    public void logEmerg(String logInfo);
    public void logError(String logInfo);
    public void logWarn(String logInfo);
    public void logInfo(String logInfo);
    public void logDebug(String logInfo);
/* log end */
}
