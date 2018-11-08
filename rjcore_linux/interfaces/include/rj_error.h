#ifndef __RJ_ERROR_H__
#define __RJ_ERROR_H__
#include <string>

using namespace std;

string rj_strerror(int error);

/* common */
#define SUCCESS_0   0
#define ERROR_10000 -10000
#define ERROR_10001 -10001   /* Init failed */

/* application */
#define ERROR_11000 -11000   /* Not found the File */



#endif /*__RJ_ERROR_H__*/
