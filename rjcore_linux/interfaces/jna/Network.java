package com.ruijie.core.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public class Network {
     public static void network() {
         RjCoreLib.INSTANCE.network();
     }
}
