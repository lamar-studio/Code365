/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RJ_DATA_H__
#define __RJ_DATA_H__

#include "rj_type.h"

#define NET_TAG_SIZE            32

#define NET_MAX(a, b)           ((a) > (b) ? (a) : (b))
#define NET_MAX3(a, b, c)       NET_MAX(NET_MAX(a,b),c)
#define NET_MAX4(a, b, c, d)    NET_MAX((a), NET_MAX3((b), (c), (d)))

#define NET_MIN(a,b)            ((a) > (b) ? (b) : (a))
#define NET_MIN3(a,b,c)         NET_MIN(NET_MIN(a,b),c)
#define NET_MIN4(a, b, c, d)    NET_MIN((a), NET_MIN3((b), (c), (d)))

#define NET_SWAP(type, a, b)    do {type SWAP_tmp = b; b = a; a = SWAP_tmp;} while(0)
#define NET_ARRAY_ELEMS(a)      (sizeof(a) / sizeof((a)[0]))
#define NET_ALIGN(x, a)         (((x)+(a)-1)&~((a)-1))
#define NET_VSWAP(a, b)         { a ^= b; b ^= a; a ^= b; }

#define NET_RB16(x)  ((((const U8*)(x))[0] << 8) | ((const U8*)(x))[1])
#define NET_WB16(p, d) do { \
        ((U8*)(p))[1] = (d); \
        ((U8*)(p))[0] = (d)>>8; } while(0)

#define NET_RL16(x)  ((((const U8*)(x))[1] << 8) | \
                     ((const U8*)(x))[0])
#define NET_WL16(p, d) do { \
        ((U8*)(p))[0] = (d); \
        ((U8*)(p))[1] = (d)>>8; } while(0)

#define NET_RB32(x)  ((((const U8*)(x))[0] << 24) | \
                     (((const U8*)(x))[1] << 16) | \
                     (((const U8*)(x))[2] <<  8) | \
                     ((const U8*)(x))[3])
#define NET_WB32(p, d) do { \
        ((U8*)(p))[3] = (d); \
        ((U8*)(p))[2] = (d)>>8; \
        ((U8*)(p))[1] = (d)>>16; \
        ((U8*)(p))[0] = (d)>>24; } while(0)

#define NET_RL32(x) ((((const U8*)(x))[3] << 24) | \
                    (((const U8*)(x))[2] << 16) | \
                    (((const U8*)(x))[1] <<  8) | \
                    ((const U8*)(x))[0])
#define NET_WL32(p, d) do { \
        ((U8*)(p))[0] = (d); \
        ((U8*)(p))[1] = (d)>>8; \
        ((U8*)(p))[2] = (d)>>16; \
        ((U8*)(p))[3] = (d)>>24; } while(0)

#define NET_RB64(x)  (((U64)((const U8*)(x))[0] << 56) | \
                     ((U64)((const U8*)(x))[1] << 48) | \
                     ((U64)((const U8*)(x))[2] << 40) | \
                     ((U64)((const U8*)(x))[3] << 32) | \
                     ((U64)((const U8*)(x))[4] << 24) | \
                     ((U64)((const U8*)(x))[5] << 16) | \
                     ((U64)((const U8*)(x))[6] <<  8) | \
                     (U64)((const U8*)(x))[7])
#define NET_WB64(p, d) do { \
        ((U8*)(p))[7] = (d);     \
        ((U8*)(p))[6] = (d)>>8;  \
        ((U8*)(p))[5] = (d)>>16; \
        ((U8*)(p))[4] = (d)>>24; \
        ((U8*)(p))[3] = (d)>>32; \
        ((U8*)(p))[2] = (d)>>40; \
        ((U8*)(p))[1] = (d)>>48; \
        ((U8*)(p))[0] = (d)>>56; } while(0)

#define NET_RL64(x)  (((U64)((const U8*)(x))[7] << 56) | \
                     ((U64)((const U8*)(x))[6] << 48) | \
                     ((U64)((const U8*)(x))[5] << 40) | \
                     ((U64)((const U8*)(x))[4] << 32) | \
                     ((U64)((const U8*)(x))[3] << 24) | \
                     ((U64)((const U8*)(x))[2] << 16) | \
                     ((U64)((const U8*)(x))[1] <<  8) | \
                     (U64)((const U8*)(x))[0])
#define NET_WL64(p, d) do { \
        ((U8*)(p))[0] = (d);     \
        ((U8*)(p))[1] = (d)>>8;  \
        ((U8*)(p))[2] = (d)>>16; \
        ((U8*)(p))[3] = (d)>>24; \
        ((U8*)(p))[4] = (d)>>32; \
        ((U8*)(p))[5] = (d)>>40; \
        ((U8*)(p))[6] = (d)>>48; \
        ((U8*)(p))[7] = (d)>>56; } while(0)

#define NET_RB24(x)  ((((const U8*)(x))[0] << 16) | \
                     (((const U8*)(x))[1] <<  8) | \
                     ((const U8*)(x))[2])
#define NET_WB24(p, d) do { \
        ((U8*)(p))[2] = (d); \
        ((U8*)(p))[1] = (d)>>8; \
        ((U8*)(p))[0] = (d)>>16; } while(0)

#define NET_RL24(x)  ((((const U8*)(x))[2] << 16) | \
                     (((const U8*)(x))[1] <<  8) | \
                     ((const U8*)(x))[0])

#define NET_WL24(p, d) do { \
        ((U8*)(p))[0] = (d); \
        ((U8*)(p))[1] = (d)>>8; \
        ((U8*)(p))[2] = (d)>>16; } while(0)

#include <stdio.h>


#define __RETURN                __Return
#define __FAILED                __failed

#define ARG_T(t)                t
#define ARG_N(a,b,c,d,N,...)    N
#define ARG_N_HELPER(...)       ARG_T(ARG_N(__VA_ARGS__))
#define COUNT_ARG(...)          ARG_N_HELPER(__VA_ARGS__,4,3,2,1,0)

#ifndef TEMP_FAILURE_RETRY
/* Used to retry syscalls that can return EINTR. */
#define TEMP_FAILURE_RETRY(exp) ({         \
    __typeof__ (exp) _rc;                      \
    do {                                   \
           _rc = (exp);                       \
    } while (_rc == -1 && errno == EINTR); \
    _rc; })
#endif


#ifdef __cplusplus
extern "C" {
#endif

static __inline S32 net_clip(S32 a, S32 amin, S32 amax)
{
    if      (a < amin) return amin;
    else if (a > amax) return amax;
    else               return a;
}

static __inline U32 net_is_32bit()
{
    return ((sizeof(void *) == 4) ? (1) : (0));
}

#ifdef __cplusplus
}
#endif

#endif /*__NET_COMMON_H__*/

