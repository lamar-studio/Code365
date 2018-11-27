#include <string.h>

#include "rj_log.h"
#include "rj_data.h"
#include "rj_thread.h"

#define NET_THREAD_DBG_FUNCTION     (0x00000001)

static U32 thread_debug = 0;

#define thread_dbg(flag, fmt, ...)  _rj_dbg(thread_debug, flag, fmt, ## __VA_ARGS__)

RjThread::RjThread(RjThreadFunc func, void *ctx, const char *name)
    : mThread(0),
      mStatus(NET_THREAD_UNINITED),
      mFunction(func),
      mContext(ctx)
{
    if (name) {
        strncpy(mName, name, sizeof(mName));
    } else {
        snprintf(mName, sizeof(mName), "rj_thread");
    }
}

RjThreadStatus RjThread::get_status() const
{
    return mStatus;
}
void RjThread::set_status(RjThreadStatus status)
{
    mStatus = status;
}

void RjThread::start()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    if (NET_THREAD_UNINITED == mStatus) {

        mStatus = NET_THREAD_RUNNING;
        if (0 == pthread_create(&mThread, &attr, mFunction, mContext)) {
            S32 ret = pthread_setname_np(mThread, mName);
            if (ret) {
                rjlog_error("thread %p setname %s failed\n", mFunction, mName);
            }
            thread_dbg(NET_THREAD_DBG_FUNCTION, "thread %s %p context %p create success\n",
                       mName, mFunction, mContext);
        } else {
            mStatus = NET_THREAD_UNINITED;
        }
    }
    pthread_attr_destroy(&attr);
}

void RjThread::stop()
{
    if (NET_THREAD_UNINITED != mStatus) {
        lock();
        mStatus = NET_THREAD_STOPPING;
        rjlog_info("NET_THREAD_STOPPING status set mThread %p", this);
        signal();
        unlock();
        void *dummy;
        //pthread_cancel(mThread);
        pthread_join(mThread, &dummy);
        thread_dbg(NET_THREAD_DBG_FUNCTION, "thread %s %p context %p destroy success\n",
                   mName, mFunction, mContext);

        mStatus = NET_THREAD_UNINITED;
    }
}
