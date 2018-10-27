#ifndef __SYSRJCORE_LINUX_H__
#define __SYSRJCORE_LINUX_H__

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef int(*callback)(const char *eventName, int eventType, const char *eventContent);
int Init(const char *comName, callback cb);
int control(const char *arg, char *retbuf);

#ifdef __cplusplus
}
#endif

#endif /*__SYSRJCORE_LINUX_H__*/
