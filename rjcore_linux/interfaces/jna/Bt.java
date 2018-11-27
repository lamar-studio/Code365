package com.ruijie.rcos.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public class Bt {

private static final int json_length = 1024;
public static String BtMakeSeed_block(String request) {
	
    byte[] json_respone = new byte[json_length];
	
    RjCoreLib.INSTANCE.BtMakeSeed_block(request,json_respone,json_length);
	
    String str = new String(json_respone);
	
    return str;
}

public static String BtMakeSeedCancel(String request)
{
    byte[] json_respone = new byte[json_length];
	
    RjCoreLib.INSTANCE.BtMakeSeedCancel(request,json_respone,json_length);
	
    String str = new String(json_respone);
	
    return str;
}

public static String BtShare_Start(String request)
{
    byte[] json_respone = new byte[json_length];
	
    RjCoreLib.INSTANCE.BtShare_Start(request,json_respone,json_length);
	
    String str = new String(json_respone);
	
    return str;
}

public static String BtShare_Stop(String request)
{
    byte[] json_respone = new byte[json_length];
	
    RjCoreLib.INSTANCE.BtShare_Stop(request,json_respone,json_length);
	
    String str = new String(json_respone);
	
    return str;
	
}

public static String BtDownload_block(String request)
{
    byte[] json_respone = new byte[json_length];
	
    RjCoreLib.INSTANCE.BtDownload_block(request,json_respone,json_length);
	
    String str = new String(json_respone);
	
    return str;
}

public static String BtDownloadCancle(String request)
{
    byte[] json_respone = new byte[json_length];
	
    RjCoreLib.INSTANCE.BtDownloadCancle(request,json_respone,json_length);
	
    String str = new String(json_respone);
	
    return str;
}

public static String BtGetStatus(String request)
{
    byte[] json_respone = new byte[json_length];
	
    RjCoreLib.INSTANCE.BtGetStatus(request,json_respone,json_length);
	
    String str = new String(json_respone);
	
    return str;
}
}

