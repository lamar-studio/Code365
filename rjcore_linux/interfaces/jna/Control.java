package com.ruijie.core.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public class Control {
     private static final int length = 1024;
     public static String control(String arg) {
         byte[] retbuf = new byte[length];
         RjCoreLib.INSTANCE.control(arg, retbuf);
         String str = new String(retbuf);
         retbuf = null;
         return str;
     }

    public static int getBrightness() {
        int ret = RjCoreLib.INSTANCE.getBrightness();
        return ret;
    }

}
