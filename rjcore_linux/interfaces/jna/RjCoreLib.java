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
    public int getHostname(byte[] retbuf);
    public int setHostname(String hostname);
    public int setSystemTime(String time);
    public int setLanguage(String language);
    public int startConsole();
    public int getUsbPathForOffine(byte[] retbuf);
/* control end */

/* product start */
    public int product();
    public int getProductType(byte[] retbuf);
    public int getAbsLayerVersion(byte[] retbuf);
    public int getSystemVersion(byte[] retbuf);
    public int getHardwareVesion(byte[] retbuf);
    public int getProductId(byte[] retbuf);
    public int getSerialNumber(byte[] retbuf);
    public int getMacAddress(byte[] retbuf);
    public int getBiosInfo(byte[] retbuf);
    public int getCpuInfo(byte[] retbuf);
    public int getMemoryInfo(byte[] retbuf);
    public int getBaseboardInfo(byte[] retbuf);
    public int getSystemBit(byte[] retbuf);
    public int getTermialType(byte[] retbuf);
    public int collectLog(byte[] retbuf);
    public int rmLogFile(byte[] path);
/* product end */

/* bt start */
    public int bt();
/* bt end */

/* application start */
    public int application();
    public int isProcessRunning(String processname);
    public int installApk_block(String apkPath);
    public int uninstallApk_block(String packageName);
    public int installDeb_block(String debPath);
    public int uninstallDeb_block(String debPath);
    public int mergeDeltaPacket();
/* application end */

/* network start */
    public int network();
/* network end */

/* log start */
    public void logEmerg(String logInfo);
    public void logError(String logInfo);
    public void logWarn(String logInfo);
    public void logInfo(String logInfo);
    public void logDebug(String logInfo);
/* log end */
}
