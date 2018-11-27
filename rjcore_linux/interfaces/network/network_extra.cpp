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

int rj_execstring(const char *cmd, string &resevc)
{
    int num = 0;
    resevc.clear();

    if (cmd == NULL || strlen(cmd) == 0) {
        return -1;
    }

    FILE *pp = popen(cmd, "r");
    if (!pp) {
        return -1;
    }

    char tmp[1024];
    while (fgets(tmp, sizeof(tmp), pp) != NULL) {
        resevc.append(tmp);
        num++;
    }
    pclose(pp);

    return num;
}

int networkEventJudge(const string& data)
{
    if (data.empty()) {
        rjlog_error("network callbak event is empty");
        return -1;
    }

    int i = 0;

    for (std::vector<string>::const_iterator it = Network_Event.begin() ; it != Network_Event.end(); ++it) {
        string param = *it;

        if ((data.find(param.c_str()) != string::npos)) {
            return i;
        }
        i++;
    }

    return -1;
}

int wifiEventJudge(const string& data)
{
    if (data.empty()) {
        rjlog_error("wifi callbak event is empty");
        return -1;
    }

    int i = 0;
    for (std::vector<string>::const_iterator it = Wifi_Event.begin() ; it != Wifi_Event.end(); ++it) {
        string param = *it;
        if ((data.find(param.c_str()) != string::npos)) {
            return i;
        }
        i++;
    }

    return -1;
}

int onEvent(void* data, size_t len, void *ctx)
{
    string event;
    size_t pos = 0;
    string eventName;
    int eventType;
    string eventContent;
    eventContent.assign("");

    event.assign((const char*)data, len);

    rjlog_info("get event: %s", event.c_str());

    int ret = -1;
    if (networkEventJudge(event) < 0) {
        ret = wifiEventJudge(event);
        if (ret < 0) {
            rjlog_error("unvailed event");
            return -1;
        }
        eventName.assign("WifiEvent");
    } else {
        eventName.assign("NetworkEvent");
    }

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
            } else if ((pos = event.find("IP_UNAVAILABLE_eth0")) == 0) {
                eventType = ETH_NO_IP;
                pos += strlen("IP_UNAVAILABLE_eth0");
            } else if ((pos = event.find("IP_CONFLICT")) == 0) {
                eventType = IP_CONFLICT;
                int conflict_pos1 = event.find("\t");
                pos += strlen("IP_CONFLICT");
                if (conflict_pos1 > 0) {
                    pos += strlen("\t");
                    int conflict_pos2 = event.find("\t", conflict_pos1+1);
                    if (conflict_pos2 > 0) {
                        int conflict_size = conflict_pos2 - conflict_pos1;
                        if (conflict_size > 0) {
                            eventContent = event.substr(conflict_pos1+1, conflict_size);
                        }
                        pos += conflict_size + strlen("\t");
                    }
                }
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
    char cmd[command_size];

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

int acquireIPOnly(const char *interface, char *buf, size_t size)
{
    char cmd[command_size];

    string buf_str;

    sprintf(cmd, "ifconfig %s | grep 'inet addr' | awk '{print $2}' | awk -F: '{print $2}'", interface);
    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_info("have no ip");
        memset(cmd, 0 , command_size);

        sprintf(cmd, "ifconfig %s:avahi | grep 'inet addr:169' | grep -v '127.0.0.1' \
              | cut -d: -f2 | awk '{print $1}'", interface);
        num = rj_execstring(cmd, buf_str);
        if (num <=0 ) {
            rjlog_info("have no wired avahi");
            snprintf(buf, size, "0.0.0.0");
            return NOT_CARE;
        } else {
            if (buf_str.size() < size) {
                size = buf_str.size();
            }
            strncpy(buf, buf_str.c_str(), size);
        }
     } else {
        if (buf_str.size() < size) {
            size = buf_str.size();
        }
        strncpy(buf, buf_str.c_str(), size);
    }

    return RIGHT_RESULT;
}
