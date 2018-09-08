
#define LOG_TAG "RjcoreService"
//#define LOG_NDEBUG 0

#include <utils/Log.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dirent.h>
#include <unistd.h>

#include <string.h>

#include <cutils/atomic.h>
#include <cutils/properties.h> // for property_get

#include <utils/misc.h>

#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/MemoryHeapBase.h>
#include <binder/MemoryBase.h>
#include <utils/Errors.h>  // for status_t
#include <utils/String8.h>
#include <utils/String16.h>
#include <utils/SystemClock.h>
#include <utils/Timers.h>
#include <utils/Vector.h>

#include "RjcoreService.h"


namespace android {

void RjcoreService::instantiate() {
    defaultServiceManager()->addService(
            String16("ruijie.core.service"), new RjcoreService());
}

RjcoreService* RjcoreService::getInstance()
{
    if (mInstance == NULL) {
        mInstance = new RjcoreService();
    }

    return mInstance;
}

RjcoreService::RjcoreService() {
    ALOGV("RjcoreService created");
}

RjcoreService::~RjcoreService() {
    ALOGV("RjcoreService destroyed");
}

}; // namespace android
