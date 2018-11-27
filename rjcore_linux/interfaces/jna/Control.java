package com.ruijie.rcos.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public class Control {
    private static final int length = 1024;

    public static int getBrightness() {
        int ret = RjCoreLib.INSTANCE.getBrightness();
        return ret;
    }

    public static int setBrightness(int brightness) {
        int ret = RjCoreLib.INSTANCE.setBrightness(brightness);
        return ret;
    }

    public static int isSupportBrightness() {
        int ret = RjCoreLib.INSTANCE.isSupportBrightness();
        return ret;
    }

    public static int getVoiceVolume() {
        int ret = RjCoreLib.INSTANCE.getVoiceVolume();
        return ret;
    }

    public static int setVoiceVolume(int volume) {
        int ret = RjCoreLib.INSTANCE.setVoiceVolume(volume);
        return ret;
    }

    public static int getHdmiVoiceStatus() {
        int ret = RjCoreLib.INSTANCE.getHdmiVoiceStatus();
        return ret;
    }

    public static int setHdmiVoiceStatus(int status) {
        int ret = RjCoreLib.INSTANCE.setHdmiVoiceStatus(status);
        return ret;
    }

    public static int setDeviceResolution(String res, int refresh) {
        int ret = RjCoreLib.INSTANCE.setDeviceResolution(res, refresh);
        return ret;
    }

    public static String getSupportResolution() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getSupportResolution(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static String getCurrentResolution() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getCurrentResolution(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static String getOptimumResolution() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getOptimumResolution(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static int shutDown() {
        int ret = RjCoreLib.INSTANCE.shutDown();
        return ret;
    }

    public static int reboot() {
        int ret = RjCoreLib.INSTANCE.reboot();
        return ret;
    }

    public static int setPowerState(int powerState) {
        int ret = RjCoreLib.INSTANCE.setPowerState(powerState);
        return ret;
    }

    public static int getPowerState() {
        int ret = RjCoreLib.INSTANCE.getPowerState();
        return ret;
    }

    public static int setSleeptime(int second) {
        int ret = RjCoreLib.INSTANCE.setSleeptime(second);
        return ret;
    }

    public static String getHostname() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getHostname(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static int setHostname(String hostname) {
        int ret = RjCoreLib.INSTANCE.setHostname(hostname);
        return ret;
    }


    public static int syncServerTime_block(String serverip) {
        int ret = RjCoreLib.INSTANCE.syncServerTime_block(serverip);
        return ret;
    }

    public static int setLanguage(String language) {
        int ret = RjCoreLib.INSTANCE.setLanguage(language);
        return ret;
    }

    public static int startConsole() {
        int ret = RjCoreLib.INSTANCE.startConsole();
        return ret;
    }

/*
    public static int getUsbPathForOffine(byte[] retbuf) {

    }
*/

}
