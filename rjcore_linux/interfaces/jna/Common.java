package com.ruijie.rcos.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;


public class Common {
    public static void Init(String comName, RjCallback cb) {
        RjCoreLib.INSTANCE.Init(comName, cb);
    }
}

