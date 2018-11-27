#define MODULE_TAG "network"

#include<string.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include "rj_commom.h"
#include "rj_utils.h"

using namespace std;

using std::string;

#define DNS_MODE_CFG                 "/etc/resolv.conf"

#ifdef __cplusplus
extern "C" {
#endif

/**return DHCP if dns is dhcp mode, STATIC if dns mode is static**/
int getDnsMode(void)
{
    struct stat buf;

    if (lstat(DNS_MODE_CFG, &buf) == -1) {
        rjlog_error("dns mode lstat error");
        return ACQUIRE_FAIL;
    }

    if (S_ISLNK(buf.st_mode)) {
        return DHCP;
    }

    return STATIC;
}

int getWiredMac(char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);
    CHECK_FUNCTION_IN();

    char cmd[command_size];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -g wiredmac");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("getWiredMac excute failed");
        return COMMAND_FAIL;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return RIGHT_RESULT;
}

int getNetStatus_block(void)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();

    char cmd[command_size];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -g netstatus");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("getNetStatus excute failed");
        return COMMAND_FAIL;
    }

    return atoi(buf_str.c_str());
}

int getIPInfo_block(char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);
    CHECK_FUNCTION_IN();

    char cmd[command_size];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -g ipinfo");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("getIPInfo excute failed");
        return COMMAND_FAIL;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return RIGHT_RESULT;

}

int getIPOnly(int type, char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);

    char cmd[command_size];
    string buf_str;
    int ret = NOT_CARE;

    if (type == NetworkWired) {
        sprintf(cmd, "ifconfig -a | grep br0");
        int num = rj_execstring(cmd, buf_str);
        if (num <= 0) {
            rjlog_info("have no br0");
            ret = acquireIPOnly("eth0", buf, size);
        } else {
            ret = acquireIPOnly("br0", buf, size);
        }
    } else if (type == NetworkWireless) {
        ret = acquireIPOnly("wlan0", buf, size);
    }

    return ret;
}

int checkPingIp_block(const char *ip, char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);
    CHECK_STR_PARA(ip);

    char cmd[command_size];
    string buf_str;
    string tmp_ip;
    tmp_ip.assign(ip);

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -c ping -d '%s'", tmp_ip.c_str());

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("getPingInfo excute failed");
        return COMMAND_FAIL;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return RIGHT_RESULT;
}

int getCurCardSpeed(char* buf, size_t size)
{
    CHECK_INIT_STR(buf, size);

    char cmd[command_size];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -g curspeed");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("getcurspeed excute failed");
        return COMMAND_FAIL;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return RIGHT_RESULT;
}

int getMaxCardSpeed()
{
    CHECK_INIT_INT();

    char cmd[command_size];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -g speedmaxwired");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("get speedmaxwired excute failed");
        return COMMAND_FAIL;
    }
    return atoi(buf_str.c_str());
}

int getOptionServerIp(int type, char* buf, size_t size)
{
    CHECK_INIT_STR(buf, size);

    char cmd[command_size];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -g option43 -d %d", type);

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("get option43 excute failed");
        return COMMAND_FAIL;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return RIGHT_RESULT;
}

int checkIpConflict_block(const char* ip_addr, char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);
    CHECK_STR_PARA(ip_addr);

    char cmd[command_size];
    string buf_str;
    string tmp_str;

    tmp_str.clear();
    tmp_str.assign(ip_addr);

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -c ipconflict -d %s", tmp_str.c_str());

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("check ipconflict failed");
        return COMMAND_FAIL;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return RIGHT_RESULT;
}

int setDns_block(const char* info, char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);
    CHECK_STR_PARA(info);

    char cmd[command_size];
    string buf_str;
    string tmp_str;

    tmp_str.clear();
    tmp_str.assign(info);

    if (tmp_str.length() < 0) {
        rjlog_error("inputting dns info is empty");
        return COMMAND_FAIL;
    }

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -s dns -d '%s'", tmp_str.c_str());

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("set dns fail");
        return COMMAND_FAIL;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return RIGHT_RESULT;
}

int setIP_block(const char* info, char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);
    CHECK_STR_PARA(info);

    char cmd[command_size];
    string buf_str;
    string tmp_str;

    tmp_str.clear();
    tmp_str.assign(info);

    if (tmp_str.length() < 0) {
        rjlog_error("inputting ip info is empty");
        snprintf((buf), size, "%s", ERROR_CODE.at(THREE_ORDER).c_str());
        return INPUT_ERROR;
    }

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -s ip -d '%s'", tmp_str.c_str());

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("set ip fail");
        return COMMAND_FAIL;
    }
    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return RIGHT_RESULT;
}

int startFtpUpload_block(const char* uploadInfo)
{
   /* string serverIp;
    int port;
    string username;
    string password;
    string localFileName;
    string remoteFileName;
    int timeout;*/
    CHECK_INIT_INT();
    CHECK_STR_PARA(uploadInfo);

    char cmd[command_size];
    ftpInfo info;
    string data;
    data.assign(uploadInfo);

    int ret = parseFtpInfo(data, info);
    if (ret < 0) {
        rjlog_error("parse ftp info failed");
        return JSON_FAIL;
    }

    if (info.username.empty() && info.password.empty()) {
        sprintf(cmd, "curl -m %d -T %s ftp://%s:%d/%s >/dev/null 2>&1", info.timeout, info.localFileName.c_str()
                  , info.serverIp.c_str(), info.port, info.remoteFileName.c_str());
    } else {
        sprintf(cmd, "curl -m %d -T %s ftp://%s:%s@%s:%d/%s >/dev/null 2>&1", info.timeout, info.localFileName.c_str()
                  , info.username.c_str(), info.password.c_str(), info.serverIp.c_str(), info.port
                  , info.remoteFileName.c_str());
    }
    rjlog_info("ftpupload cmd is %s", cmd);

    //0 if curl upload finish in time, 28 if curl upload time out
    ret = rj_system(cmd);
    if (ret != 0) {
        if (ret == 28) {
            string tmp_ping;
            memset(cmd, 0, command_size);
            sprintf(cmd, "ping -c 1 -W 1 -w 1 %s | grep '0 received'", info.serverIp.c_str());
            int num = rj_execstring(cmd, tmp_ping);
            if (num <= 0) {
                rjlog_error("ftpupload ping successful");
                return RIGHT_RESULT;
            }
        }
        rjlog_error("curl upload fail");
        return COMMAND_FAIL;
    }

    return RIGHT_RESULT;

}

int startFtpDownload_block(const char* downloadInfo, char* buf, size_t size)
{
   /* string serverIp;
    int port;
    string username;
    string password;
    string localFileName;
    string remoteFileName;
    int timeout;*/
    CHECK_INIT_STR(buf, size);
    CHECK_STR_PARA(downloadInfo);

    char cmd[command_size];
    ftpInfo info;
    string data;
    data.assign(downloadInfo);

    int ret = parseFtpInfo(data, info);
    if (ret < 0) {
        rjlog_error("parse ftp info failed");
        snprintf((buf), size, "%s", ERROR_CODE.at(FOUR_ORDER).c_str());
        return JSON_FAIL;
    }

    if (info.username.empty() && info.password.empty()) {
        sprintf(cmd, "curl -m %d -o %s ftp://%s:%d/%s >/dev/null 2>&1", info.timeout, info.localFileName.c_str()
                  , info.serverIp.c_str(), info.port, info.remoteFileName.c_str());
    } else {
        sprintf(cmd, "curl -m %d -o %s ftp://%s:%s@%s:%d/%s >/dev/null 2>&1", info.timeout, info.localFileName.c_str()
                  , info.username.c_str(), info.password.c_str(), info.serverIp.c_str(), info.port
                  , info.remoteFileName.c_str());
    }
    rjlog_info("ftpupload cmd is %s", cmd);

    ret = rj_system(cmd);
    if (ret != 0) {
        if (ret == 28) {
            string tmp_ping;
            memset(cmd, 0, command_size);
            sprintf(cmd, "ping -c 1 -W 1 -w 1 %s | grep '0 received'", info.serverIp.c_str());
            int num = rj_execstring(cmd, tmp_ping);
            if (num <= 0) {
                rjlog_error("ftpdownload ping successful");
                return RIGHT_RESULT;
            }
        }

        rjlog_error("curl download fail");
        snprintf((buf), size, "%s", ERROR_CODE.at(SECOND_ORDER).c_str());
        return COMMAND_FAIL;
    }

    if (access(info.localFileName.c_str(), F_OK) == 0) {
        string res;
        memset(cmd, 0, 256);
        sprintf(cmd, "du -h %s | awk '{print $1}'", info.localFileName.c_str());
        ret = rj_execstring(cmd, res);
        if (ret < 0) {
            rjlog_error("compute downloading file size fail");
            snprintf((buf), size, "%s", ERROR_CODE.at(SECOND_ORDER).c_str());
            return COMMAND_FAIL;
        }

        if (res.size() < size) {
            size = res.size();
        }

        strncpy(buf, res.c_str(), size);
    }

    return RIGHT_RESULT;

}

/**
 *wifi interface
 */

int getNetCardStatus(int card_type)
{
    CHECK_INIT_INT();

    char cmd[command_size];
    string buf_str;

    if (card_type == NetworkWired) {
        sprintf(cmd, "/usr/local/bin/netmanager/network_cli -g wiredcard");
    } else if (card_type == NetworkWireless) {
        sprintf(cmd, "/usr/local/bin/netmanager/network_cli -g wirelesscard");
    }

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("get card status excute failed");
        return COMMAND_FAIL;
    }

    return (atoi(buf_str.c_str()));
}

int getWifiStatus_block(void)
{
    CHECK_INIT_INT();

    char cmd[command_size];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -a wifimanager -g wifistatus");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("get wifistatus excute failed");
        return COMMAND_FAIL;
    }

    return atoi(buf_str.c_str());
}

int getWifiSavedList_block(char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);

    char cmd[command_size];

    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -a wifimanager -a wifisavelist");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("get wifisavedlist excute failed");
        snprintf((buf), size, "%s", ERROR_CODE.at(SECOND_ORDER).c_str());
        return COMMAND_FAIL;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return RIGHT_RESULT;

}

int getScanResult_block(char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);

    char cmd[command_size];

    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -a wifimanager -g scanresults");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("get wifisavedlist excute failed");
        snprintf((buf), size, "%s", ERROR_CODE.at(SECOND_ORDER).c_str());
        return COMMAND_FAIL;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return RIGHT_RESULT;
}

int getWifiInfo_block(char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);

    char cmd[command_size];

    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -a wifimanager -g wifiinfo");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("get wifiinfo excute failed");
        snprintf((buf), size, "%s", ERROR_CODE.at(SECOND_ORDER).c_str());
        return COMMAND_FAIL;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return RIGHT_RESULT;
}

int getWhiteList_block(char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);

    char cmd[command_size];

    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -a wifimanager -g whitelist");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("get whitelist excute failed");
        snprintf((buf), size, "%s", ERROR_CODE.at(SECOND_ORDER).c_str());
        return COMMAND_FAIL;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return RIGHT_RESULT;
}


int setNetCard(int card_type, bool disable)
{
    CHECK_INIT_INT();

    char cmd[command_size];
    string buf_str;
    string tmp_str;

    if (card_type == NetworkWired) {
        sprintf(cmd, "/usr/local/bin/netmanager/network_cli -s wiredcardstatus -d %d", disable);
    } else if (card_type == NetworkWireless) {
        sprintf(cmd, "/usr/local/bin/netmanager/network_cli -a wifimanager -s wirelesscardstatus -d %d", disable);
    }

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("set card status fail");
        return COMMAND_FAIL;
    }

    return RIGHT_RESULT;

}

int setForceScan_block(char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);
    CHECK_FUNCTION_IN();

    char cmd[command_size];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -a wifimanager -s scan");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("force scan excute failed");
        snprintf((buf), size, "%s", ERROR_CODE.at(SECOND_ORDER).c_str());
        return COMMAND_FAIL;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return RIGHT_RESULT;

}

int setConnectInfo_block(const char *info)
{
    CHECK_INIT_INT();
    CHECK_STR_PARA(info);

    char cmd[command_size];
    string buf_str;
    string tmp_str;

    tmp_str.clear();
    tmp_str.assign(info);

    if (tmp_str.length() < 0) {
        rjlog_error("inputting connect info is empty");
        return INPUT_ERROR;
    }

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -a wifimanager -s connectinfo -d '%s'", tmp_str.c_str());

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("connect a new wifi fail");
        return COMMAND_FAIL;
    }

    return RIGHT_RESULT;

}

int setConnectId_block(int net_id)
{
    CHECK_INIT_INT();

    char cmd[command_size];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -a wifimanager -s connectid -d %d", net_id);

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("connect a saved wifi fail");
        return COMMAND_FAIL;
    }

    return RIGHT_RESULT;

}

int setForgetId_block(int net_id)
{
    CHECK_INIT_INT();

    char cmd[command_size];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -a wifimanager -s forget -d %d", net_id);

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("forget a saved wifi fail");
        return COMMAND_FAIL;
    }

    return RIGHT_RESULT;
}

int setDisconnect_block(void)
{
    CHECK_INIT_INT();

    char cmd[command_size];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -a wifimanager -s disconnect");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("forget a saved wifi fail");
        return COMMAND_FAIL;
    }

    return RIGHT_RESULT;
}

int setWhiteList_block(const char *info)
{
    CHECK_INIT_INT();
    CHECK_STR_PARA(info);

    char cmd[command_size];
    string buf_str;
    string tmp_str;

    tmp_str.clear();
    tmp_str.assign(info);

    if (tmp_str.length() < 0) {
        rjlog_error("inputting whitelist info is empty");
        return INPUT_ERROR;
    }

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -a wifimanager -s whitelist -d '%s'", tmp_str.c_str());

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("connect a new wifi fail");
        return COMMAND_FAIL;
    }

    return RIGHT_RESULT;

}

int checkWifiTerminal_block(void)
{
    CHECK_INIT_INT();

    char cmd[command_size];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -c wifiterminal");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("check ipconflict failed");
        return COMMAND_FAIL;
    }

    return atoi(buf_str.c_str());
}

int checkWifiSaved_block(const char* info)
{
    CHECK_INIT_INT();
    CHECK_STR_PARA(info);

    char cmd[command_size];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -a wifimanager -c wifisaved -d '%s'", info);

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("check ipconflict failed");
        return COMMAND_FAIL;
    }

    return atoi(buf_str.c_str());
}

#ifdef __cplusplus
}
#endif
