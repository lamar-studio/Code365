#include <string.h>

#include "net_thread.h"

#define NET_THREAD_DBG_FUNCTION     (0x00000001)

static uint32_t thread_debug = 0;

NetThread::NetThread(NetThreadFunc func, void *ctx, const char *name)
    : mThread(0),
      mStatus(NET_THREAD_UNINITED),
      mFunction(func),
      mContext(ctx)
{
    if (name) {
        strncpy(mName, name, sizeof(mName));
    } else {
        snprintf(mName, sizeof(mName), "net_thread");
    }
}

NetThreadStatus NetThread::get_status() const
{
    return mStatus;
}
void NetThread::set_status(NetThreadStatus status)
{
    mStatus = status;
}

void NetThread::start()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    if (NET_THREAD_UNINITED == mStatus) {

        mStatus = NET_THREAD_RUNNING;
        if (0 == pthread_create(&mThread, &attr, mFunction, mContext)) {
            S32 ret = pthread_setname_np(mThread, mName);
            if (ret) {
                log("thread %p setname %s failed\n", mFunction, mName);
            }
            thread_dbg(NET_THREAD_DBG_FUNCTION, "thread %s %p context %p create success\n",
                       mName, mFunction, mContext);
        } else {
            mStatus = NET_THREAD_UNINITED;
        }
    }
    pthread_attr_destroy(&attr);
}

void NetThread::stop()
{
    if (NET_THREAD_UNINITED != mStatus) {
        lock();
        mStatus = NET_THREAD_STOPPING;
        log("NET_THREAD_STOPPING status set mThread %p", this);
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
