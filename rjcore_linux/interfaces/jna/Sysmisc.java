package com.ruijie.rcos.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public class Sysmisc{
    private static final int length = 1024;

    public static String getPlatHWInfo(){
         byte[] retbuf = new byte[length];
         if (RjCoreLib.INSTANCE.getPlatHWInfo(retbuf, length) == 0){
             String str = new String(retbuf);
             retbuf = null;
             return str;
         }else {
             return null;
         }
    }
    
    public static int getDiskStatus_block(){
         return RjCoreLib.INSTANCE.getDiskStatus_block();
    }
    
    public static String getPersonalDiskList_block(){
         byte[] retbuf = new byte[length];
         if (RjCoreLib.INSTANCE.getPersonalDiskList_block(retbuf, length) == 0){
             String str = new String(retbuf);
             retbuf = null;
             return str;
         }else {
             return null;
         }
    }

    public static int formatPersonalDisk_block(String dev){
         return RjCoreLib.INSTANCE.formatPersonalDisk_block(dev);
    }
    
    public static int mountPersonalDisk_block(String dev, String dir, String option){
         return RjCoreLib.INSTANCE.mountPersonalDisk_block(dev, dir, option);
    }
    
    public static int umountPersonalDisk_block(String dir){
         return RjCoreLib.INSTANCE.umountPersonalDisk_block(dir);
    }
    
    public static int checkISOVersion(String verstr){
         return RjCoreLib.INSTANCE.checkISOVersion(verstr);
    }
    
    public static int fastUpgrade(String jsonmsg){
         return RjCoreLib.INSTANCE.fastUpgrade(jsonmsg);
    }
    
    public static int ipxeUpgrade(String verstr){
         return RjCoreLib.INSTANCE.ipxeUpgrade(verstr);
    }

}

