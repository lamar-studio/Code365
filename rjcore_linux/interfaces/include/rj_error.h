#ifndef __RJ_ERROR_H__
#define __RJ_ERROR_H__
#include <string>

using namespace std;

string rj_strerror(int error);

/* common */
#define SUCCESS_0   0
#define ERROR_10000 -10000
#define ERROR_10001 -10001   /* Init failed */

/* control */
#define ERROR_11000 -11000   /* Not found the File */
#define ERROR_11001 -11001   /* Nonsupport this Feature */
#define ERROR_11002 -11002   /* System cmd execute Failure */
#define ERROR_11003 -11003   /* The parameter is Invalid */



#endif /*__RJ_ERROR_H__*/
