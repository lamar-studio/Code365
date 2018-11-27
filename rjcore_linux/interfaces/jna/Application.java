package com.ruijie.rcos.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public class Application {
    public static int isProcessRunning(String processname) {
        int ret = RjCoreLib.INSTANCE.isProcessRunning(processname);
        return ret;
    }

    public static int startProcess(String path, String processname) {
        int ret = RjCoreLib.INSTANCE.startProcess(path, processname);
        return ret;
    }

    public static int installDeb_block(String debPath, String debName) {
        int ret = RjCoreLib.INSTANCE.installDeb_block(debPath, debName);
        return ret;
    }

    public static int uninstallDeb_block(String debName) {
        int ret = RjCoreLib.INSTANCE.uninstallDeb_block(debName);
        return ret;
    }

    public static int mergeDeltaPacket_block(String old, String delta, String newpack) {
        int ret = RjCoreLib.INSTANCE.mergeDeltaPacket_block(old, delta, newpack);
        return ret;
    }

}
