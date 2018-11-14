#define MODULE_TAG "network"

#include<string.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include "rj_commom.h"
#include "rj_utils.h"

using namespace std;

using std::string;

int rj_execstring(const char *cmd, string &resevc)
{
    int num = 0;
    resevc.clear();

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

#ifdef __cplusplus
extern "C" {
#endif

int network()
{
    CHECK_INIT_INT();
    rjlog_info("function enter.");

    g_callback("network", 1, "network_callback");

    rjlog_info("function exit.");
    return 0;
}

int getWiredMac(char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);
    CHECK_FUNCTION_IN();

    char cmd[128];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -g wiredmac");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("getWiredMac excute failed");
        return -1;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return (buf_str.size());
}

int getNetStatus_block(void)
{
    CHECK_INIT_INT();
    CHECK_FUNCTION_IN();

    char cmd[128];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -g netstatus");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("getNetStatus excute failed");
        return -1;
    }

    return atoi(buf_str.c_str());
    //return (buf_str().size());
}

int getIPInfo_block(char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);
    CHECK_FUNCTION_IN();

    char cmd[128];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -g ipinfo");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("getIPInfo excute failed");
        return -1;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return (buf_str.size());

}

int checkPingIp_block(const char *ip, char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);
    CHECK_STR_PARA(ip);

    char cmd[128];
    string buf_str;
    string tmp_ip;
    tmp_ip.assign(ip);

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -c ping -d '%s'", tmp_ip.c_str());

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("getPingInfo excute failed");
        return -1;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return (buf_str.size());
}

int getCurCardSpeed(char* buf, size_t size)
{
    CHECK_INIT_STR(buf, size);

    char cmd[128];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -g curspeed");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("getcurspeed excute failed");
        return -1;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return (buf_str.size());
}

int getMaxCardSpeed()
{
    CHECK_INIT_INT();

    char cmd[128];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -g speedmaxwired");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("get speedmaxwired excute failed");
        return -1;
    }
    return atoi(buf_str.c_str());
}

int getOptionServerIp(int type, char* buf, size_t size)
{
    CHECK_INIT_STR(buf, size);

    char cmd[128];
    string buf_str;

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -g option43 -d %d", type);

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("get option43 excute failed");
        return -1;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return (buf_str.size());
}

int checkIpConflict_block(const char* ip_addr, char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);
    CHECK_STR_PARA(ip_addr);

    char cmd[128];
    string buf_str;
    string tmp_str;

    tmp_str.clear();
    tmp_str.assign(ip_addr);

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -c ipconflict -d %s", tmp_str.c_str());

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("check ipconflict failed");
        return -1;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return (buf_str.size());
}

int setDns_block(const char* info, char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);
    CHECK_STR_PARA(info);

    char cmd[128];
    string buf_str;
    string tmp_str;

    tmp_str.clear();
    tmp_str.assign(info);

    if (tmp_str.length() < 0) {
        rjlog_error("inputting dns info is empty");
        return -1;
    }

    rjlog_info("=================%s", tmp_str.c_str());

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -s dns -d '%s'", tmp_str.c_str());

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("set dns fail");
        return -1;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return (buf_str.size());
}

int setIP_block(const char* info, char *buf, size_t size)
{
    CHECK_INIT_STR(buf, size);
    CHECK_STR_PARA(info);

    char cmd[128];
    string buf_str;
    string tmp_str;

    tmp_str.clear();
    tmp_str.assign(info);

    if (tmp_str.length() < 0) {
        rjlog_error("inputting ip info is empty");
        return -1;
    }

    rjlog_info("=================%s", tmp_str.c_str());

    sprintf(cmd, "/usr/local/bin/netmanager/network_cli -s ip -d '%s'", tmp_str.c_str());

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("set ip fail");
        return -1;
    }

    if (buf_str.size() < size) {
        size = buf_str.size();
    }

    strncpy(buf, buf_str.c_str(), size);
    return (buf_str.size());
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

    char cmd[256];
    ftpInfo info;
    string data;
    data.assign(uploadInfo);

    int ret = parseFtpInfo(data, info);
    if (ret < 0) {
        rjlog_error("parse ftp info failed");
        return -1;
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

    ret = rj_system(cmd);
    if (ret < 0) {
        rjlog_error("curl upload fail");
    }

    return 0;

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

    char cmd[256];
    ftpInfo info;
    string data;
    data.assign(downloadInfo);

    int ret = parseFtpInfo(data, info);
    if (ret < 0) {
        rjlog_error("parse ftp info failed");
        return -1;
    }

    if (info.username.empty() && info.password.empty()) {
        sprintf(cmd, "wget -O %s ftp://%s:%d/%s >/dev/null 2>&1 -T %d", info.localFileName.c_str()
                  ,info.serverIp.c_str(), info.port, info.remoteFileName.c_str(), info.timeout);
    } else {
        sprintf(cmd, "wget -O %s ftp://%s:%s@%s:%d/%s >/dev/null 2>&1 -T %d", info.localFileName.c_str()
                  , info.username.c_str(), info.password.c_str(), info.serverIp.c_str(), info.port
                  , info.remoteFileName.c_str(), info.timeout);
    }
    rjlog_info("ftpupload cmd is %s", cmd);

    ret = rj_system(cmd);
    if (ret < 0) {
        rjlog_error("curl upload fail");
    }

    if (access(info.localFileName.c_str(), F_OK) == 0) {
        string res;
        memset(cmd, 0, 256);
        sprintf(cmd, "du -h %s | awk '{print $1}'", info.localFileName.c_str());
        ret = rj_execstring(cmd, res);
        if (ret < 0) {
            rjlog_error("compute downloading file size fail");
            return -1;
        }

        if (res.size() < size) {
            size = res.size();
        }

        strncpy(buf, res.c_str(), size);
    }

    return 0;

}

#ifdef __cplusplus
}
#endif
