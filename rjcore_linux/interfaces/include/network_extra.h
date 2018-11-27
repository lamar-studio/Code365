#ifndef _NETWORK_EXTRA_H
#define _NETWORK_EXTRA_H

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include "rj_commom.h"
#include "rj_ipc.h"
#include "rj_thread.h"
#include "rc_json.h"

#define NET_EVENT_NAME "NetEvent"

#define command_size 512

//#ifdef __cplusplus
//extern "C" {
//#endif

const vector<string> Network_Event =
{
    "ETH_UP",
    "ETH_DOWN",
    "WLAN_UP",
    "WLAN_DOWN",
    "NET_UNAVAILABLE",
    "IP_UNAVAILABLE_br0",
    "IP_UNAVAILABLE_eth0",
    "IP_UNAVAILABLE_wlan0",
    "IP_CONFLICT",
    "TERMINATING"
};

const vector<string> Wifi_Event =
{
    "CONNECT_SUCCESS",
    "DISCONNECTED",
    "SCAN_RESULTS",
    "CONNECT_START",
    "CONNECT_FAILED",
    "TIMEOUT"

};

const vector<string> ERROR_CODE = {
    "RJSUCCESS-0",
    "RJFAIL-14000",      //it indicates that shine does not care the returning value
    "RJFAIL-14001",      //it indicates that shine does setting command failed by calling network interfac
    "RJFAIL-14002",      //it indicates that shine passes error parameters when calling network interface
    "RJFAIL-14003"       //it indicates that json data parse failed
};

enum NUM_CODE {
    ZERO_ORDER = 0,
    FIRST_ORDER,
    SECOND_ORDER,
    THREE_ORDER,
    FOUR_ORDER
};

enum ERROR_CODE_INT {
    RIGHT_RESULT = 0,
    NOT_CARE = -14000,
    INPUT_ERROR,
    COMMAND_FAIL,
    JSON_FAIL,
    ACQUIRE_FAIL
};

enum NetworkType {
    NetworkWired    = 0,
    NetworkWireless = 1,
    NetwWorkall     = 2,
    NetworkOther    = 3
};

enum CALLEVENT {
    WifiEvent = 0,
    NetStatusEvent
};

enum NetStatus {
    NET_UNAVAILABLE = 0x0,
    ETH_DOWN        = 0x1,
    ETH_UP          = 0x2,
    WLAN_DOWN       = 0x4,
    WLAN_UP         = 0x8,
    WLAN_NO_IP      = 0x10,
    ETH_NO_IP       = 0x20,
    IP_CONFLICT     = 0x40,
    TERMINATING     = 0xff,
};

enum WifiEventInfo {
    WIFI_EVENT_SCAN_RESULT       = 0,
    WIFI_EVENT_CONNECT_SUCCESS   = 1,
    WIFI_EVENT_CONNECT_FAILED    = 2,
    WIFI_EVENT_CONNECT_START     = 3,
    WIFI_EVENT_DISCONNECT        = 4,
    WIFI_EVENT_TIMEOUT           = 5,
    WIFI_EVENT_TERMINATE         = 6,
    WIFI_EVENT_DISCONNECT_FAILED = 7,
    WIFI_EVENT_RESERVED          = 8
};

enum DHCP_MODE {
    DHCP = 0,
    STATIC
};

typedef struct ftp {
    string serverIp;
    int port;
    string username;
    string password;
    string localFileName;
    string remoteFileName;
    int timeout;
}ftpInfo;


int rj_execstring(const char *cmd, string &resevc);
int  onEvent(void* data, size_t len, void *ctx);
extern  void network_init(callback cb);
extern void network_del();
bool reconnect(RjClient*& client, bool attach);
void restartNetworkDaemon();
int parseFtpInfo(const string& cmd, ftpInfo& ftpinfo);
int wifiEventJudge(const string& data);
int wifiEventJudge(const string& data);
int acquireIPOnly(const char *interface, char *buf, size_t size);

//#ifdef __cplusplus
//}
//#endif


#endif
