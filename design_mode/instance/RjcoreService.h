
#ifndef ANDROID_RJCORESERVICE_H
#define ANDROID_RJCORESERVICE_H

#include <stddef.h>
#include <stdint.h>
#include <utils/threads.h>
#include <utils/Errors.h>
#include <utils/KeyedVector.h>
#include <utils/String8.h>
#include <utils/String16.h>
#include <utils/Vector.h>

#include "IRjcoreService.h"

#define SIZE_1K 1024
#define SIZE_2K 2048
#define SIZE_1K_HALF 512

namespace android {

class RjcoreService : public BnRjcoreService
{
public:
    static void instantiate();

    //the instance for RjcoreService
    static RjcoreService* getInstance();
private:
    RjcoreService();
    virtual ~RjcoreService();

    static RjcoreService *mInstance;

};

}; // namespace android

#endif // ANDROID_RJCORESERVICE_H
