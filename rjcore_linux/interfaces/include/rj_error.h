#ifndef __RJ_ERROR_H__
#define __RJ_ERROR_H__
#include <string>

using namespace std;

string rj_strerror(int error);
 
#define ERROR_10000 -10000
#define ERROR_10001 -10001   /* Init failed */

#endif /*__RJ_ERROR_H__*/
