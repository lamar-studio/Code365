package com.ruijie.rcos.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public class Network {
    private static final int length = 4096;
    public static void network() {
        RjCoreLib.INSTANCE.network();
    }
    public static String getWiredMac() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getWiredMac(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }
    public static int getNetStatus_block() {
        int status = RjCoreLib.INSTANCE.getNetStatus_block();
        return status;
    }
    public static String getIPInfo_block() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getIPInfo_block(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }
    public static String checkPingIp_block(String ip) {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.checkPingIp_block(ip, retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }
    public static String getCurCardSpeed() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getCurCardSpeed(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }
    public static int getMaxCardSpeed() {
        int status = RjCoreLib.INSTANCE.getMaxCardSpeed();
        return status;
    }
    public static String getOptionServerIp(int type) {
        byte[] retbuf = new byte[length];
        int status = RjCoreLib.INSTANCE.getOptionServerIp(type, retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }
    public static String checkIpConflict_block(String ip) {
        byte[] retbuf = new byte[length];
        int status = RjCoreLib.INSTANCE.checkIpConflict_block(ip, retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }
    public static String setDns_block(String dns) {
        byte[] retbuf = new byte[length];
        int status = RjCoreLib.INSTANCE.setDns_block(dns, retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }
    public static String setIP_block(String ip) {
        byte[] retbuf = new byte[length];
        int status = RjCoreLib.INSTANCE.setIP_block(ip, retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }
    public static boolean startFtpUpload_block(String info) {
        byte[] retbuf = new byte[length];
        int result = RjCoreLib.INSTANCE.startFtpUpload_block(info);
        if (result < 0) {
            return false;
        }
        return true;
    }
    public static String startFtpDownload_block(String info) {
        byte[] retbuf = new byte[length];
        int status = RjCoreLib.INSTANCE.startFtpDownload_block(info, retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }
}
