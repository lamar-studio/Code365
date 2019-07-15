/*
 * make: gcc -o test resTest.c -I/usr/loacl/include -L/usr/local/lib -lsysabslayer_linux
 */


#include <stdio.h>
#include <stdlib.h>

#include "rj_commom.h"
#include "json.h"
#include "rc_json.h"

using namespace RcJson;
using namespace std;

static int createDisplayConfig(int isEnabled, const char *res, const char *pos ,char *cfg)
{
    memset(cfg, 0, 1024);

    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        rjlog_error("json create failed");
        return -1;
    }

    cJSON_AddNumberToObject(root, "enable", isEnabled);
    cJSON_AddStringToObject(root, "res", res);
    cJSON_AddStringToObject(root, "pos", pos);

    char *p = cJSON_Print(root);
    if (p == NULL) {
        cJSON_Delete(root);
        return -1;
    }

    cJSON_Delete(root);
    strcpy(cfg, p);
    free(p);

    rjlog_info("cfg: %s", cfg);

    return 0;
}

static void dumpFinalDisplayInfo(const char *info)
{
    rjlog_info("############## dumpFinalDisplayInfo ##############");
    rjlog_info("%s", info);
}

int main(int argc, char **argv)
{
    int ret = 0;
    string bufList;
    string bufCurRes;
    string resList_2;
    char ret_buf[1024];
    char cfg_buf[1024];
    char pos_buf[1024];
    char pos[1024];
    char buf[1024];
    int cnt_1 = 0;
    int cnt_2 = 0;

    const char *res_1[] = {"1920x1080", "1680x1050", "1600x900", "1280x1024", "1280x800", "1280x960", "1280x720", "1024x768", "1152x864", "800x600", "640x480", "720x400"};

    const char *res_2[] = {"1920x1080", "1680x1050", "1600x900", "1280x1024",
                         "1440x900", "1280x800", "1280x960", "1152x864", "1280x720", "1024x768", "832x624", "800x600", "720x576", "720x480", "640x480", "720x400"};

    ret = InitForEST("testvideo", NULL);
    if (ret != 0) {
        rjlog_error("init testvideo fail.");
        return ERROR_10001;
    }

while(1) {
    if (cnt_1 > 11)
        cnt_1 = 0;

    if (cnt_2 > 15)
        cnt_2 = 0;


    /* setDisplayConfig */
    createDisplayConfig(1, res_1[cnt_1], "0x0", cfg_buf);
    rjlog_info("------------------- setDisplayConfig port 1 -------------------");
    ret = setDisplayConfig(1, 0xf, cfg_buf);
    if (ret != SUCCESS_0) {
        rjlog_error("set display config port 1 failed, ret %d", ret);
    }

    /* getDisplayInfo */
    rjlog_info("------------------- getDisplayInfo port 1 -------------------");
    ret = getDisplayInfo(1, 0x28, ret_buf, 1024);
    if (ret != SUCCESS_0) {
        rjlog_error("get display info port 1 failed, ret %d", ret);
    }
    bufCurRes = rc_json_get_string(ret_buf, "curRes");
    bufList = rc_json_get_string(ret_buf, "resList");

    //printf("1-devList:%s \n", bufList.c_str());
    if (strcmp(res_1[cnt_1], bufCurRes.c_str()) == 0) {
        rjlog_info("1-setDisplayConfig res:%s success", res_1[cnt_1]);
        printf("1-setDisplayConfig res:%s success \n", res_1[cnt_1]);
    } else {
        rjlog_info("1-setDisplayConfig setRes:%s getRes:%s fail", res_1[cnt_1], bufCurRes.c_str());
        printf("1-setDisplayConfig setRes:%s getRes:%s fail \n", res_1[cnt_1], bufCurRes.c_str());
    }


    sscanf(res_1[cnt_1], "%[^x]x%s", pos_buf, buf);
    sprintf(pos, "%sx0", pos_buf);


    /* setDisplayConfig */
    createDisplayConfig(2, res_2[cnt_2], pos, cfg_buf);
    rjlog_info("------------------- setDisplayConfig port 2 -------------------");
    ret = setDisplayConfig(2, 0xf, cfg_buf);
    if (ret != SUCCESS_0) {
        rjlog_error("set display config port 1 failed, ret %d", ret);
    }

    /* getDisplayInfo */
    rjlog_info("------------------- getDisplayInfo port 2 -------------------");
    ret = getDisplayInfo(2, 0x28, ret_buf, 1024);
    if (ret != SUCCESS_0) {
        rjlog_error("get display info port 1 failed, ret %d", ret);
    }
    bufCurRes = rc_json_get_string(ret_buf, "curRes");
    bufList = rc_json_get_string(ret_buf, "resList");

    //printf("2-devList:%s \n", bufList.c_str());
    if (strcmp(res_2[cnt_2], bufCurRes.c_str()) == 0) {
        rjlog_info("2-setDisplayConfig res:%s success", res_2[cnt_2]);
        printf("2-setDisplayConfig res:%s success \n", res_2[cnt_2]);
    } else {
        rjlog_info("2-setDisplayConfig setRes:%s getRes:%s fail", res_2[cnt_2], bufCurRes.c_str());
        printf("2-setDisplayConfig setRes:%s getRes:%s fail \n", res_2[cnt_2], bufCurRes.c_str());
    }

    cnt_1++;
    cnt_2++;
    sleep(2);

}

    return 0;
}

