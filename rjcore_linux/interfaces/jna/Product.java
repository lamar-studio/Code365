package com.ruijie.rcos.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public class Product{
    private static final int length = 64;

    public static String getProductType() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getProductType(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static String getAbsLayerVersion() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getAbsLayerVersion(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static String getSystemVersion() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getSystemVersion(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static String getHardwareVesion() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getHardwareVesion(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static String getProductId() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getProductId(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static String getSerialNumber() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getSerialNumber(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static String getBiosInfo() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getBiosInfo(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static String getCpuInfo() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getCpuInfo(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static String getMemoryInfo() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getMemoryInfo(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static String getBaseboardInfo() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getBaseboardInfo(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static String getSystemBit() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getSystemBit(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static String getDiskInfo(String type) {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getDiskInfo(type, retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static String getTermialType() {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.getTermialType(retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static String collectLog_block(String fileName) {
        byte[] retbuf = new byte[length];
        RjCoreLib.INSTANCE.collectLog_block(fileName, retbuf, length);
        String str = new String(retbuf);
        retbuf = null;
        return str;
    }

    public static int rmLogFile(String path) {
        int ret = 0;
        ret = RjCoreLib.INSTANCE.rmLogFile(path);
        return ret;
    }

}
