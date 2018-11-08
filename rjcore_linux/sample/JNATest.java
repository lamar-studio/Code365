//package com.ruijie.core.linux;
import java.nio.ByteBuffer;
import java.util.Arrays;
import com.ruijie.core.linux.library.*;

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
        System.out.println(Product.getTermialType());
        System.out.println(Product.getProductId());
        System.out.println(Product.getAbsLayerVersion());
        System.out.println(Product.getSystemBit());
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
