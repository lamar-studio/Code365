package com.ruijie.core.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public interface RjCoreLib extends Library {
    RjCoreLib INSTANCE = (RjCoreLib)Native.loadLibrary("sysrjcore_linux", RjCoreLib.class);

/* control start */
    public int Init(String comName, RjCallback cb);
/* control end */

/* control start */
    public int control(String arg, byte[] retbuf);
    public int getBrightness();
    public int setBrightness(int brightness);
    public int getVoiceVolume();
    public int setVoiceVolume(int volume);
    public int getHdmiVoiceStatus();
    public int setHdmiVoiceStatus(int status);
    public int getCurrentResolution(byte[] retbuf);
    public int setDeviceResolution(int xres, int yres, int refresh);
    public int getSupportResolution(byte[] retbuf);
    public int shutdownSelf();
    public int reboot();
    public int setPowerState(int powerState);
    public int getPowerState();
    public int getSleeptime();
    public int setSleeptime(int time);
    public int getLedStatus(int color);
    public int setLedStatus(int color, int state);
    public int getHostname(byte[] retbuf, int size);
    public int setHostname(String hostname);
    public int setSystemTime(String time);
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
    public int collectLog(String filename, byte[] retbuf, int size);
    public int rmLogFile(String path);
/* product end */

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
    public int application();
    public int isProcessRunning(String processname);
    public int installApk_block(String apkPath);
    public int uninstallApk_block(String packageName);
    public int installDeb_block(String debPath);
    public int uninstallDeb_block(String debPath);
    public int mergeDeltaPacket_block(String old, String delta, String newpack);
/* application end */

/* network start */
    public int network();
    public int getWiredMac(byte[] retbuf);
    public int getNetStatus_block();
    public int getIPInfo_block(byte[] retbuf);
    public int checkPingIp_block(String ip, byte[] retbuf);
    public int getCurCardSpeed(byte[] retbuf);
    public int getMaxCardSpeed();
    public int getOptionServerIp(int type, byte[] retbuf);
    public int checkIpConflict_block(String ip, byte[] retbuf);
    public int setDns_block(String info, byte[] retbuf);
    public int setIP_block(String info, byte[] retbuf);
/* network end */

/* log start */
    public void logEmerg(String logInfo);
    public void logError(String logInfo);
    public void logWarn(String logInfo);
    public void logInfo(String logInfo);
    public void logDebug(String logInfo);
/* log end */
}
