package com.ruijie.core.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public class Product{
     public static void product() {
         RjCoreLib.INSTANCE.product();
     }
}
