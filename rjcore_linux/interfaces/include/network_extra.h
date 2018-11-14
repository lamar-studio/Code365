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

//#ifdef __cplusplus
//extern "C" {
//#endif

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

typedef struct ftp {
    string serverIp;
    int port;
    string username;
    string password;
    string localFileName;
    string remoteFileName;
    int timeout;
}ftpInfo;

int  onEvent(void* data, size_t len, void *ctx);
extern  void network_init(callback cb);
extern void network_del();
bool reconnect(RjClient*& client, bool attach);
void restartNetworkDaemon();
int parseFtpInfo(const string& cmd, ftpInfo& ftpinfo);

//#ifdef __cplusplus
//}
//#endif


#endif
