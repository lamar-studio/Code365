#include <fstream>
#include "video_core.h"
#include "rj_commom.h"
#include "dev_api.h"

namespace rcdev
{

#define XRANDR_OUTFILE "/tmp/xrandr.out"

VideoCore::VideoCore()
{

}

VideoCore::~VideoCore()
{

}

VideoCore* VideoCore::mInstance = NULL;
pthread_once_t  VideoCore::mOnce;

VideoCore* VideoCore::getInstance() {
    pthread_once(&mOnce, init);
    return mInstance;
}

void VideoCore::init() {
    mInstance = new VideoCore();
}

void VideoCore::clearRateInfo(RateInfo& rateInfo)
{
    rateInfo.rate.clear();
    rateInfo.flag = 0;
}

void VideoCore::clearResInfo(ResInfo& resInfo)
{
    resInfo.res.clear();
    resInfo.flag = 0;
    for (auto it = resInfo.rateList.begin(); it != resInfo.rateList.end(); ++it) {
        clearRateInfo(*it);
    }
    resInfo.rateList.clear();
}

void VideoCore::clearDevInfo(DevInfo& devInfo)
{
    devInfo.devType.clear();
    for (auto it = devInfo.resList.begin(); it != devInfo.resList.end(); ++it) {
        clearResInfo(*it);
    }
    devInfo.resList.clear();
}

int VideoCore::getResolutionList(vector<DevInfo>& devList)
{
    char cmd[128];
    vector<string> cmdRes;

    ifstream fin(XRANDR_OUTFILE);
    if (!fin) {
        rjlog_warn("%s does not exist", XRANDR_OUTFILE);

        sprintf(cmd, "xrandr -q > %s", XRANDR_OUTFILE);
        rjlog_info("exec: %s", cmd);
        if (dev_exec(cmd, cmdRes) < 0) {
            rjlog_error("exec failed: %s", cmd);
            return -1;
        }
    }

    sprintf(cmd, "cat %s", XRANDR_OUTFILE);
    rjlog_info("exec: %s", cmd);
    if (dev_exec(cmd, cmdRes) < 0) {
        rjlog_error("exec failed: %s", cmd);
        return -1;
    }


    int devCnt = 0;
    bool isConnected = false;
    bool bInterlaced;
    unsigned width;
    unsigned height;
    size_t strPos0 = 0;
    size_t strPos1 = 0;
    string str0;
    string str1;
    string tmpStr;

    ResInfo resInfo;
    RateInfo rateInfo;
    DevInfo devInfo;

    clearDevInfo(devInfo);

    for (auto it = cmdRes.begin(); it != cmdRes.end(); ++it) {
        /* device type */
        if (it->find("VIRTUAL1") != string::npos) {
            rjlog_info("ending of actual monitors");
            break;
        } else if (it->find(" disconnected") != string::npos) {
            isConnected = false;
        } else if (it->find(" connected") != string::npos) {
            isConnected = true;
            devCnt++;
            if (devCnt > 1) {
                devList.push_back(devInfo);
            }
            clearDevInfo(devInfo);

            rjlog_info("----- monitor info -----");
            strPos0 = it->find(" connected");
            devInfo.devType = it->substr(0, strPos0);
            rjlog_info("devType: %s", devInfo.devType.c_str());
        } else if (isConnected &&
                   it->find("x")   != string::npos &&
                   it->find(" x ") == string::npos) {
            clearResInfo(resInfo);

            /* width and height */
            strPos0 = it->find("x");
            strPos1 = it->rfind(" ", strPos0);
            str0 = it->substr(strPos1 + 1, strPos0 - (strPos1 + 1));

            /* NOTE: may including 'i', such as 1920x1080i */
            strPos1 = it->find(" ", strPos0);
            str1 = it->substr(strPos0 + 1, strPos1 - (strPos0 + 1));

            resInfo.res = str0.append("x").append(str1);

            width = atoi(str0.c_str());
            bInterlaced = false;
            if (str1.find("i") != string::npos) {
                str1.erase(str1.find("i"), 1);
                bInterlaced = true;
            }
            height = atoi(str1.c_str());

            rjlog_info("res: %s, width %d, height %d, interlaced %d",
                         resInfo.res.c_str(), width, height, bInterlaced);

            if (IS_SUPPORTED_RES(width, height) && (!bInterlaced)) {

                /* refresh rate */
                size_t skipNum = 0;
                tmpStr = (*it);
                do {
                    strPos0 = tmpStr.find(".");
                    if (strPos0 == string::npos) {
                        break;
                    }
                    clearRateInfo(rateInfo);

                    strPos1 = tmpStr.rfind(" ", strPos0);
                    str0    = tmpStr.substr(strPos1 + 1, strPos0 - (strPos1 + 1));

                    if (tmpStr.find("*+", strPos0) != string::npos) {
                        strPos1 = tmpStr.find("*+", strPos0);
                        rateInfo.flag = DEV_RES_BEST | DEV_RES_CUR;
                        skipNum = strlen("*+");
                    } else if (tmpStr.find(" +", strPos0) != string::npos) {
                        strPos1 = tmpStr.find(" +", strPos0);
                        rateInfo.flag = DEV_RES_BEST;
                        skipNum = strlen(" +");
                    } else if (tmpStr.find("*", strPos0) != string::npos) {
                        strPos1 = tmpStr.find("*", strPos0);
                        rateInfo.flag = DEV_RES_CUR;
                        skipNum = strlen("*");
                    } else if (tmpStr.find(" ", strPos0) != string::npos) {
                        strPos1 = tmpStr.find(" ", strPos0);
                        skipNum = strlen(" ");
                    } else {
                        skipNum = tmpStr.length() - strPos1;
                    }
                    str1    = tmpStr.substr(strPos0 + 1, strPos1 - (strPos0 + 1));

                    tmpStr  = tmpStr.substr(strPos1 + skipNum);

                    rateInfo.rate = str0.append(".").append(str1);

                    rjlog_info("     rate: %s, flag %d", rateInfo.rate.c_str(), rateInfo.flag);

                    resInfo.flag |= rateInfo.flag;
                    resInfo.rateList.push_back(rateInfo);

                } while (!tmpStr.empty());

                devInfo.resList.push_back(resInfo);
            }
        }
    }

    if (devCnt > 0) {
        devList.push_back(devInfo);
    }

    return 0;
}

int VideoCore::setResolution(string res)
{
    if (res.empty()) {
        rjlog_error("input res is empty");
        return -1;
    }

    vector<DevInfo> devList;
    if (getResolutionList(devList) < 0 || devList.empty()) {
        rjlog_error("get resolution list is empty");
        return -1;
    }

    int ret = 0;
    bool isModeCreated = false;
    char cmd[256];
    string str0;
    string str1;
    string mode;
    string modeLine;
    size_t strPos0;
    size_t strPos1;
    vector<int> isSameResList;
    vector<string> cmdRes;

    for (auto devInfo = devList.begin(); devInfo != devList.end(); ++devInfo) {
        bool newMode = true;
        int  isSameRes = 0;

        /* if res is in the list, output directly */
        for (auto resInfo = devInfo->resList.begin(); resInfo != devInfo->resList.end(); ++resInfo) {
            if (resInfo->res.compare(res) == 0) {
                newMode = false;

                if (resInfo->flag & DEV_RES_CUR) {
                    isSameRes = 1;
                    rjlog_info("same as current resolution");
                } else {
                    sprintf(cmd, "xrandr --output %s --mode %s", devInfo->devType.c_str(), res.c_str());
                    rjlog_info("exec: %s", cmd);
                    if (rj_system(cmd) < 0) {
                        rjlog_error("exec failed: %s", cmd);
                        ret = -1;
                    }
                }

                break;
            }
        }

        /* new mode */
        if (newMode) {

            /* create the same mode twice may cause X Error,
             * leading to xrandr --newmode failed.
             */
            if (!isModeCreated) {
                /* cvt */
                str0 = "cvt ";
                str1 = res;
                str0 = str0.append(str1.replace(str1.find("x"), 1, " "));
                sprintf(cmd, "%s", str0.c_str());
                rjlog_info("exec: %s", cmd);
                if (dev_exec(cmd, cmdRes) < 0 || cmdRes.size() != 2) {
                    rjlog_error("exec failed: %s", cmd);
                    ret = -1;
                    break;
                }

                /* new mode */
                str0 = cmdRes.at(1);
                strPos0 = str0.find("\"");
                strPos1 = str0.find("\"", strPos0 + 1);
                modeLine = str0.substr(strPos1 + 1);

                mode = res;

                rjlog_info("mode: %s, modeLine: %s", mode.c_str(), modeLine.c_str());
                sprintf(cmd, "xrandr --newmode \"%s\" %s", mode.c_str(), modeLine.c_str());
                rjlog_info("exec: %s", cmd);
                if (rj_system(cmd) < 0) {
                    rjlog_error("exec failed: %s", cmd);
                    ret = -1;
                    break;
                }

                isModeCreated = true;
            }

            /* add mode */
            sprintf(cmd, "xrandr --addmode %s %s", devInfo->devType.c_str(), mode.c_str());
            rjlog_info("exec: %s", cmd);
            if (rj_system(cmd) < 0) {
                rjlog_error("exec failed: %s", cmd);
                ret = -1;
                break;
            }

            /* output */
            sprintf(cmd, "xrandr --output %s --mode %s",
                    devInfo->devType.c_str(), mode.c_str());
            rjlog_info("exec: %s", cmd);
            if (rj_system(cmd) < 0) {
                rjlog_error("exec failed: %s", cmd);
                ret = -1;
                break;
            }
        }

        isSameResList.push_back(isSameRes);
    }

    bool isSameAsCurRes = true;
    for (auto it = isSameResList.begin(); it != isSameResList.end(); ++it) {
        if (!(*it)) {
            isSameAsCurRes = false;
            break;
        }
    }

    /* update xrandr -q output to file */
    if (!isSameAsCurRes) {
        sprintf(cmd, "xrandr -q > %s", XRANDR_OUTFILE);
        rjlog_info("exec: %s", cmd);
        if (dev_exec(cmd, cmdRes) < 0) {
            rjlog_error("exec failed: %s", cmd);
            ret = -1;
        }
    }

    if (ret < 0) {
        rjlog_error("set resolution failed");
        return -1;
    }


    return 0;
}

int VideoCore::getBrightness()
{
    return 0;
}

int VideoCore::setBrightness(int brightness)
{
    return 0;
}



} // namespace rcdev

