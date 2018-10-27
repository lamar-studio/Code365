package com.ruijie.core.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public interface RjCallback extends Callback {
    int callback(String eventName, int eventType, String eventContent);
}
  
