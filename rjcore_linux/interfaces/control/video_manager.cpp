#include "dev_api.h"
#include "rj_commom.h"
#include "video_core.h"

namespace rcdev
{


VideoManager::VideoManager()
    : mCb(NULL)
    , mJcb(NULL)
    , mUser(NULL)
{

}

VideoManager::~VideoManager()
{

}

void VideoManager::registerCallback(onDevEvent cb, onJsonEvent jcb, void* user)
{
    mCb = cb;
    mJcb = jcb;
    mUser = user;
}

void VideoManager::doEvent(DevMsg& msg)
{
    if (mCb == NULL) {
        rjlog_error("mCb is NULL!");
        return;
    }
    rjlog_info("send DevEvent, id: %d, msg: %s", msg.msg_id, msg.msg.c_str());
    mCb(msg, mUser);
}

void VideoManager::doJsonEvent(string& json)
{
    if (mJcb == NULL) {
        rjlog_error("mJcb is NULL!");
        return;
    }
    rjlog_info("send json: %s", json.c_str());
    mJcb(json, mUser);
}

void VideoManager::doJsonCommand(int handle, const string& json)
{
    DevMsg out_msg;
    string out_json;

    rjlog_info("recv json: %s", json.c_str());
    switch (handle) {
        case DEV_HANDLE_REQUEST_BACKLIGHT:
            out_json = "{\"handle\": 211}";
            doJsonEvent(out_json);
            break;
        case DEV_HANDLE_SET_BACKLIGHT:
            out_json = "{\"handle\": 212}";
            doJsonEvent(out_json);
            break;
        case DEV_HANDLE_SAVE_BACKLIGHT:
            out_msg.msg_id = DEV_EVENT_SAVE_BACKLIGHT;
            out_msg.msg = "save backlight value";
            out_json = "{\"handle\": 212}";
            doEvent(out_msg);
            doJsonEvent(out_json);
            break;
        default:
            break;
    }
}

int VideoManager::getBrightness()
{
    return 0;
}

int VideoManager::setBrightness(int brightness)
{
    return 0;
}

int VideoManager::getResolutionList(vector<DevResInfo>& resList)
{
    int ret = 0;
    vector<DevInfo> devList;

    resList.clear();

    ret = VideoCore::getInstance()->getResolutionList(devList);
    if (ret  < 0) {
        rjlog_error("get dev list failed");
        return -1;
    }

    if (devList.size() > 2) {
        rjlog_error("more than 2 monitors are connected");
        return -1;
    }

    if (devList.size() == 0) {
        rjlog_info("no monitor is connected");
        return 0;
    }

    int finalFlag = 0;

    /* single monitor */
    if (devList.size() == 1) {
        vector<ResInfo> srcList;
        DevResInfo dst;

        rjlog_info("single monitor");

        srcList = devList.at(0).resList;

        for (auto it = srcList.begin(); it != srcList.end(); ++it) {
            dst.res = it->res;
            dst.flag = it->flag;
            finalFlag |= dst.flag;

            resList.push_back(dst);

            rjlog_info("res: %s, flag %d", dst.res.c_str(), dst.flag);
        }
    } else if (devList.size() == 2) {
       /* in dual monitor scene, select the intersection of the resolutions */
        vector<ResInfo> srcList0;
        vector<ResInfo> srcList1;
        DevResInfo dst;

        rjlog_info("dual monitor");

        srcList0 = devList.at(0).resList;
        srcList1 = devList.at(1).resList;
        for (auto it0 = srcList0.begin(); it0 != srcList0.end(); ++it0) {
            for (auto it1 = srcList1.begin(); it1 != srcList1.end(); ++it1) {
                if (it0->res.compare(it1->res) == 0) {
                    dst.res = it0->res;
                    dst.flag = it0->flag & it1->flag;
                    finalFlag |= dst.flag;

                    resList.push_back(dst);
                    rjlog_info("res: %s, flag %d", dst.res.c_str(), dst.flag);
                }
            }
        }
    }

    /* if there is no BEST or CURRENT resolution,
     * choose the first resolution in the list by default.
     */
    if (!resList.empty()) {
        if (!(finalFlag & DEV_RES_BEST)) {
            auto it = resList.begin();
            it->flag |= DEV_RES_BEST;
        }
        if (!(finalFlag & DEV_RES_CUR)) {
            auto it = resList.begin();
            it->flag |= DEV_RES_CUR;
        }
    } else {
        rjlog_warn("no common resolution between 2 monitors!");
    }

    return 0;
}

string VideoManager::getSupportResolution()
{
    string support;
    vector<DevResInfo> resList;

    getResolutionList(resList);
    if (resList.size() == 0) {
        rjlog_warn("the support res is empty");
        return string("");
    }

    for (auto it = resList.begin(); it != resList.end(); ++it) {
        support += it->res;
        support += ";";
    }
    rjlog_warn("the support:%s", support.c_str());

    return support;
}

string VideoManager::getCurrentResolution()
{
    string current;
    vector<DevResInfo> resList;

    getResolutionList(resList);
    if (resList.size() == 0) {
        rjlog_warn("the current res is empty");
        return string("");
    }

    for (auto it = resList.begin(); it != resList.end(); ++it) {
        if (it->flag & DEV_RES_CUR) {
            current = it->res;
        }
    }
    rjlog_info("the current:%s", current.c_str());

    return current;
}

string VideoManager::getOptimumResolution()
{
    string optimum;
    vector<DevResInfo> resList;

    getResolutionList(resList);
    if (resList.size() == 0) {
        rjlog_warn("the optimum res is empty");
        return string("");
    }

    for (auto it = resList.begin(); it != resList.end(); ++it) {
        if (it->flag & DEV_RES_BEST) {
            optimum = it->res;
        }
    }
    rjlog_warn("the optimum:%s", optimum.c_str());

    return optimum;
}

int VideoManager::setResolution(string res)
{
    int ret = 0;
    ret = VideoCore::getInstance()->setResolution(res);

    if (ret < 0) {
        rjlog_error("set resolution failed");
        return -1;
    }

    return 0;
}

} //namespace
