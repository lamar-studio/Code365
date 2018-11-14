#define MODULE_TAG "network_extra"

#include "network_extra.h"
#include "rj_utils.h"
using namespace std;
using namespace RcJson;

callback mCb;

RjClient* mMonitor;


void network_init(callback cb)
{
    mMonitor = new RjClient(NET_EVENT_NAME);
    mMonitor->attach(onEvent, NULL);
    mCb = cb;
}

void network_del()
{
    if (mMonitor != NULL) {
        delete mMonitor;
    }
}

int onEvent(void* data, size_t len, void *ctx)
{
    string event;
    size_t pos = 0;
    string eventName;
    int eventType;
    string eventContent;
    eventContent.append("");

    event.assign((const char*)data, len);

    rjlog_info("get event: %s", event.c_str());

    eventName.append("NetworkEvent");
    while (1) {

        if (eventName.compare("NetworkEvent") == 0) {

            if ((pos = event.find("ETH_DOWN")) == 0) {
                eventType = ETH_DOWN;
                pos += strlen("ETH_DOWN");
            } else if ((pos = event.find("ETH_UP")) == 0) {
                eventType = ETH_UP;
                pos += strlen("ETH_UP");
            } else if ((pos = event.find("WLAN_DOWN")) == 0) {
                eventType = WLAN_DOWN;
                pos += strlen("WLAN_DOWN");
            } else if ((pos = event.find("WLAN_UP")) == 0) {
                eventType = WLAN_UP;
                pos += strlen("WLAN_UP");
            } else if ((pos = event.find("NET_UNAVAILABLE")) == 0) {
                eventType = NET_UNAVAILABLE;
                pos += strlen("NET_UNAVAILABLE");
            } else if ((pos = event.find("IP_UNAVAILABLE_wlan0")) == 0) {
                eventType = WLAN_NO_IP;
                pos += strlen("IP_UNAVAILABLE_wlan0");
            } else if ((pos = event.find("IP_UNAVAILABLE_br0")) == 0) {
                eventType = ETH_NO_IP;
                pos += strlen("IP_UNAVAILABLE_br0");
            } else if (event.find("TERMINATING") == 0) {
                rjlog_error("service terminating connection!");
                eventType = TERMINATING;
                pos += strlen("TERMINATING");
                rjlog_info("mMonitor reconnect start");
                reconnect(mMonitor, true);
                rjlog_info("reconnect end");
            } else {
                rjlog_error("invalid event: %s", event.c_str());
                break;
            }

            if (mCb) {
                rjlog_info("normal callback mCb begin");
                mCb(eventName.c_str(), eventType, eventContent.c_str());
                rjlog_info("normal callback mCb end");
            } else {
                rjlog_info("warning: mCb does not exist!");
            }

            if (pos >= len) {
                break;
            }
            event = event.substr(pos);
            pos = 0;
            len = event.length();
            rjlog_info("event has other data, sub str : %s", event.c_str());

            sleep(1);
        }
    }

    return 0;
}

bool reconnect(RjClient*& client, bool attach)
{
    rjlog_info("reconnect client : %p, attach : %d", client, attach);
    delete client;
    sleep(2);
    rjlog_info("reconnect start");
    client = new RjClient(NET_EVENT_NAME);
    if (!client->getConnected()) {
        rjlog_error("cannot connect to service, try to restart it");
        delete client;
        restartNetworkDaemon();
        client = new RjClient(NET_EVENT_NAME);
    }

    if (attach) {
        client->attach(&onEvent, NULL);
    }

   rjlog_info("reconnect end");

    return true;
}

void restartNetworkDaemon()
{
    char cmd[256];

    sprintf(cmd, "pkill network_daemon");

    rj_exec(cmd);

    sprintf(cmd, "systemctl start network-daemon.service");

    rj_exec(cmd);
}

int parseFtpInfo(const string& cmd, ftpInfo& ftpinfo)
{
    if (cmd.empty()) {
        rjlog_info("ftpinfo json data is empty");
        return -1;
    }

    ftpinfo.serverIp = rc_json_get_string(cmd,"serverIp");
    ftpinfo.username = rc_json_get_string(cmd,"username");
    ftpinfo.password = rc_json_get_string(cmd,"password");
    ftpinfo.localFileName = rc_json_get_string(cmd,"localFileName");
    ftpinfo.remoteFileName = rc_json_get_string(cmd,"remoteFileName");
    ftpinfo.port = rc_json_get_int(cmd,"port");
    ftpinfo.timeout = rc_json_get_int(cmd,"timeout");

    return 0;
}
