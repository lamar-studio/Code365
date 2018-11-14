//package com.ruijie.core.linux;
import java.nio.ByteBuffer;
import java.util.Arrays;
import com.ruijie.rcos.linux.library.*;

public class JNATest {
    public static void main(String[] args) {
        JNATest ts = new JNATest();

        RjCallback rjcallback = new RjCallback(){
            public int callback(String a, int b, String c) {
                System.out.print(a);
                System.out.print(" ");
                System.out.print(b);
                System.out.print(" ");
                System.out.println(c);
                return 0;
            }
        };

        System.out.println("Test enter!!!");
        Common.Init("shine", rjcallback);
        String arg = "My name is control.";
        String str = Control.control(arg);
        System.out.println(str);

        System.out.println(Product.getProductType());
        System.out.println(Product.getAbsLayerVersion());
        System.out.println(Product.getSerialNumber());
        System.out.println(Product.getDiskInfo("system"));
        System.out.println(Product.getDiskInfo("data"));

        System.out.println("Sysmisc.getPlatHWInfo()" + Sysmisc.getPlatHWInfo());
        System.out.println("Sysmisc.getDiskStatus_block()" + Sysmisc.getDiskStatus_block());
        System.out.println("Sysmisc.getPersonalDiskList_block()" + Sysmisc.getPersonalDiskList_block());
        System.out.println("Sysmisc.formatPersonalDisk_block()" + Sysmisc.formatPersonalDisk_block("sda"));
        System.out.println("Sysmisc.mountPersonalDisk_block()" + Sysmisc.mountPersonalDisk_block("sda", "/mnt", null));
        System.out.println("Sysmisc.umountPersonalDisk_block()" + Sysmisc.umountPersonalDisk_block("/tmp"));
        System.out.println("Sysmisc.checkISOVersion()" + Sysmisc.checkISOVersion("888"));
        System.out.println("Sysmisc.fastUpgrade()" + Sysmisc.fastUpgrade("{\"ip\":\"1.2.3.5\",\"mask\":\"255.255.255.0\",\"gateway\":\"1.2.3.1\",\"ser_ip\":\"1.2.3.7\",\"mode\":\"1\",\"img_name\":\"rainos_img\",\"postrun\":\"postrun.sh\",\"isover\":\"888\"}"));
        System.out.println("Sysmisc.ipxeUpgrade()" + Sysmisc.ipxeUpgrade("88"));

        Application.application();
        Network.network();

        try {
            Thread.sleep(60000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("Test out!!!");
    }
}
