#define MODULE_TAG "network"

#include<string.h>
#include <string>
#include <iostream>
#include "rj_commom.h"

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

int getWiredMac(char *buf)
{
    char cmd[128];
    string buf_str;

    sprintf(cmd, "/usr/bin/network_cli -g wiredmac");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("getWiredMac excute failed");
        return -1;
    }

    strcpy(buf, buf_str.c_str());
    return (buf_str.size());
}

int getNetStatus_block(void)
{
    char cmd[128];
    string buf_str;

    sprintf(cmd, "/usr/bin/network_cli -g netstatus");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("getNetStatus excute failed");
        return -1;
    }

    return atoi(buf_str.c_str());
    //return (buf_str().size());
}

int getIPInfo_block(char *buf)
{
    char cmd[128];
    string buf_str;

    sprintf(cmd, "/usr/bin/network_cli -g ipinfo");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("getIPInfo excute failed");
        return -1;
    }

    strcpy(buf, buf_str.c_str());
    return (buf_str.size());

}

int checkPingIp_block(char *ip, char *buf)
{
    char cmd[128];
    string buf_str;
    string tmp_ip;
    tmp_ip.assign(ip);

    sprintf(cmd, "/usr/bin/network_cli -c ping -d %s", tmp_ip.c_str());

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("getIPInfo excute failed");
        return -1;
    }

    strcpy(buf, buf_str.c_str());
    return (buf_str.size());
}

int getCurCardSpeed(char* buf)
{
    char cmd[128];
    string buf_str;

    sprintf(cmd, "/usr/bin/network_cli -g curspeed");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("getcurspeed excute failed");
        return -1;
    }

    strcpy(buf, buf_str.c_str());
    return (buf_str.size());
}

int getMaxCardSpeed()
{
    char cmd[128];
    string buf_str;

    sprintf(cmd, "/usr/bin/network_cli -g speedmaxwired");

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("get speedmaxwired excute failed");
        return -1;
    }
    return atoi(buf_str.c_str());
}

int getOptionServerIp(int type, char* buf)
{
    char cmd[128];
    string buf_str;

    sprintf(cmd, "/usr/bin/network_cli -g option43 -d %d", type);

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("get option43 excute failed");
        return -1;
    }

    strcpy(buf, buf_str.c_str());
    return (buf_str.size());
}

int checkIpConflict_block(const char* ip_addr, char *buf)
{
    char cmd[128];
    string buf_str;
    string tmp_str;

    tmp_str.clear();
    tmp_str.assign(ip_addr);

    sprintf(cmd, "/usr/bin/network_cli -c ipconflict -d %s", tmp_str.c_str());

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("check ipconflict failed");
        return -1;
    }

    strcpy(buf, buf_str.c_str());
    return (buf_str.size());
}

int setDns_block(const char* info, char *buf)
{
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

    sprintf(cmd, "/usr/bin/network_cli -s dns -d '%s'", tmp_str.c_str());

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("set dns fail");
        return -1;
    }

    strcpy(buf, buf_str.c_str());
    return (buf_str.size());
}

int setIP_block(const char* info, char *buf)
{
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

    sprintf(cmd, "/usr/bin/network_cli -s ip -d '%s'", tmp_str.c_str());

    int num = rj_execstring(cmd, buf_str);
    if (num <= 0) {
        rjlog_error("set ip fail");
        return -1;
    }

    strcpy(buf, buf_str.c_str());
    return (buf_str.size());
}


#ifdef __cplusplus
}
#endif
