package com.ruijie.core.linux.library;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Callback;

public class Bt {

private static final int json_length = 1024;
public static String BT_MakeSeed_block(String request) {
	
	String json_request = null;
	byte[] json_respone = new byte[json_length];
	
	RjCoreLib.INSTANCE.BT_MakeSeed_block(request,json_respone);
	
	String str = new String(json_respone);
	
	return str;
}

public static String BT_MakeSeed_cancel(String request)
{
	String json_request = null;
	byte[] json_respone = new byte[json_length];
	
	RjCoreLib.INSTANCE.BT_MakeSeed_cancel(request,json_respone);
	
	String str = new String(json_respone);
	
	return str;
}

public static String BT_Share_Start(String request)
{
	String json_request = null;
	byte[] json_respone = new byte[json_length];
	
	RjCoreLib.INSTANCE.BT_Share_Start(request,json_respone);
	
	String str = new String(json_respone);
	
	return str;
}

public static String BT_Share_Stop(String request)
{
	String json_request = null;
	byte[] json_respone = new byte[json_length];
	
	RjCoreLib.INSTANCE.BT_Share_Stop(request,json_respone);
	
	String str = new String(json_respone);
	
	return str;
	
}

public static String BT_Download_block(String request)
{
	String json_request = null;
	byte[] json_respone = new byte[json_length];
	
	RjCoreLib.INSTANCE.BT_Download_block(request,json_respone);
	
	String str = new String(json_respone);
	
	return str;
}

public static String BT_Download_cancle(String request)
{
	String json_request = null;
	byte[] json_respone = new byte[json_length];
	
	RjCoreLib.INSTANCE.BT_Download_cancle(request,json_respone);
	
	String str = new String(json_respone);
	
	return str;
}

public static String BT_Get_Status(String request)
{
	String json_request = null;
	byte[] json_respone = new byte[json_length];
	
	RjCoreLib.INSTANCE.BT_Get_Status(request,json_respone);
	
	String str = new String(json_respone);
	
	return str;
}
}
