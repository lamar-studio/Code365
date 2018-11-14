package com.ruijie.rcos.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public class Application {
    public static int isProcessRunning(String processname) {
        int ret = RjCoreLib.INSTANCE.isProcessRunning(processname);
        return ret;
    }

    public int startProcess(String processname) {
        int ret = RjCoreLib.INSTANCE.startProcess(processname);
        return ret;
    }

    public static int installDeb_block(String debPath) {
        int ret = RjCoreLib.INSTANCE.installDeb_block(debPath);
        return ret;
    }

    public static int uninstallDeb_block(String debPath) {
        int ret = RjCoreLib.INSTANCE.uninstallDeb_block(debPath);
        return ret;
    }

    public static int mergeDeltaPacket_block(String old, String delta, String newpack) {
        int ret = RjCoreLib.INSTANCE.mergeDeltaPacket_block(old, delta, newpack);
        return ret;
    }

}
