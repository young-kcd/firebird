#ifndef FB_MISC_STATUS_H
#define FB_MISC_STATUS_H

#include <stdlib.h>				// size_t
#include "../jrd/common.h"		// ISC_STATUS

const int MAX_ERRMSG_LEN	= 128;
const int MAX_ERRSTR_LEN	= 1024;

void STUFF_STATUS_function(ISC_STATUS* status_vector, ISC_STATUS status, va_list args);

void PARSE_STATUS(ISC_STATUS * status_vector, int &length, int &warning);

#endif // FB_MISC_STATUS_H
