#ifndef FB_MISC_STATUS_H
#define FB_MISC_STATUS_H

#include <stdlib.h>				// size_t
#include "../jrd/common.h"		// STATUS

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif

#define MAX_ERRMSG_LEN  128
#define MAX_ERRSTR_LEN	255

#ifdef __cplusplus
/* only allow these functions from C++ code (use of reference) */

//void STUFF_STATUS_function(STATUS* status_vector, STATUS status, va_list args);
void STUFF_STATUS_function(STATUS* , STATUS , va_list );

void PARSE_STATUS(STATUS * status_vector, int &length, int &warning);
#endif

#endif // FB_MISC_STATUS_H
