package com.ruijie.core.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public interface RjCoreLib extends Library {
    RjCoreLib INSTANCE = (RjCoreLib)Native.loadLibrary("sysrjcore_linux", RjCoreLib.class);

/* control start */
    public int Init(String comName, RjCallback cb);
/* control end */

/* control start */
    public int control(String arg, byte[] retbuf);
/* control end */
    
/* product start */
    public int product();
/* product end */
    
/* bt start */
    public int bt();
/* bt end */

/* application start */
    public int application();
/* application end */

/* network start */
    public int network();
/* network end */

/* log start */
    public void logEmerg(String logInfo);
    public void logError(String logInfo);
    public void logWarn(String logInfo);
    public void logInfo(String logInfo);
    public void logDebug(String logInfo);
/* log end */
}
