package com.ruijie.rcos.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public class Network {
    private static final int length = 4096;
    public static int getDnsMode() {
        int status = RjCoreLib.INSTANCE.getDnsMode();
        return status;
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
    public static String getIPOnly(int type) {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getIPOnly(type, retbuf, length);
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

    //wifi interface
    public static int getNetCardStatus(int type) {
        int status = RjCoreLib.INSTANCE.getNetCardStatus(type);
        return status;
    }
    public static int getWifiStatus_block() {
        int status = RjCoreLib.INSTANCE.getWifiStatus_block();
        return status;
    }
    public static String getWifiSavedList_block() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getWifiSavedList_block(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }
    public static String getScanResult_block() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getScanResult_block(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }
    public static String getWifiInfo_block() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getWifiInfo_block(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }
    public static String getWhiteList_block() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getWhiteList_block(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }
    public static int setNetCard(int card_type, boolean disable) {
        int status = RjCoreLib.INSTANCE.setNetCard(card_type, disable);
        return status;
    }
    public static String setForceScan_block() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.setForceScan_block(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }
    public static int setConnectInfo_block(String info) {
        int result = RjCoreLib.INSTANCE.setConnectInfo_block(info);
        return result;
    }
    public static int setConnectId_block(int net_id) {
        int result = RjCoreLib.INSTANCE.setConnectId_block(net_id);
        return result;
    }
    public static int setForgetId_block(int net_id) {
        int result = RjCoreLib.INSTANCE.setForgetId_block(net_id);
        return result;
    }
    public static int setDisconnect_block() {
        int result = RjCoreLib.INSTANCE.setDisconnect_block();
        return result;
    }
    public static int setWhiteList_block(String info) {
        int result = RjCoreLib.INSTANCE.setWhiteList_block(info);
        return result;
    }
    public static int checkWifiTerminal_block() {
        int result = RjCoreLib.INSTANCE.checkWifiTerminal_block();
        return result;
    }
    public static int checkWifiSaved_block(String info) {
        int result = RjCoreLib.INSTANCE.checkWifiSaved_block(info);
        return result;
    }
}
