/*
 *        PROGRAM:        JRD Access Method
 *        MODULE:         iberr.h
 *        DESCRIPTION:    Interbase error handling definitions.
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#ifndef JRD_IBERR_H
#define JRD_IBERR_H

#include "../jrd/gdsassert.h"
#include "../jrd/status.h"

/*
 * TMN: Leave INIT_STATUS as macro for now. At least "why.c" (what
 * is to become "y_valve.cpp" need it.
 */

#define INIT_STATUS(status)	status [0] = gds_arg_gds;\
				status [1] = 0;\
				status [2] = gds_arg_end


#if 1

/* TMN: The hits just keep on coming. The macro STUFF_STATUS MUST be
 * defined so it can get instantiated in the "calling" place because of
 * the use of varargs. I trimmed this one a bit though by putting the
 * majority of the macro into a function.
 */

#define STUFF_STATUS(status_vector,status)              \
{                                                       \
    va_list args;                                       \
    VA_START (args, status);                            \
    STUFF_STATUS_function(status_vector, status, args); \
}

#else	/* 0/1 */

#define STUFF_STATUS(status_vector,status)      \
{						\
va_list args;                                   \
STATUS  *p, *q;                                 \
int     type, len;                              \
                                                \
VA_START (args, status);                        \
p = status_vector;                              \
                                                \
*p++ = (STATUS)gds_arg_gds;                     \
*p++ = status;                                  \
                                                \
while ((type = va_arg (args, int)) && ((p - status_vector) < 17))    \
    switch (*p++ = type)                        \
    {                                           \
    case gds_arg_gds:                           \
        *p++ = (STATUS) va_arg (args, STATUS);  \
        break;                                  \
                                                \
    case gds_arg_string:                        \
        q = (STATUS*) va_arg (args, TEXT*);	\
	if (strlen ((TEXT*)q) >= MAX_ERRSTR_LEN)\
	    {					\
	    *(p -1) = gds_arg_cstring;		\
	    *p++ = (STATUS) MAX_ERRSTR_LEN;	\
	    }					\
	*p++ = (STATUS) q;			\
        break;                                  \
                                                \
    case gds_arg_interpreted:                   \
	*p++ = (STATUS) va_arg (args, TEXT*);   \
	break;                                  \
						\
    case gds_arg_cstring:                       \
	len = (int) va_arg (args, int);		\
        *p++ = (STATUS) (len >= MAX_ERRSTR_LEN) ? MAX_ERRSTR_LEN : len;\
        *p++ = (STATUS) va_arg (args, TEXT*);   \
        break;                                  \
                                                \
    case gds_arg_number:                        \
        *p++ = (STATUS) va_arg (args, SLONG);   \
        break;                                  \
                                                \
    case gds_arg_vms:                           \
    case gds_arg_unix:                          \
    case gds_arg_domain:                        \
    case gds_arg_dos:                           \
    case gds_arg_mpexl:                         \
    case gds_arg_next_mach:                     \
    case gds_arg_netware:                       \
    case gds_arg_win32:                         \
    default:                                    \
        *p++ = (STATUS) va_arg (args, int);     \
        break;                                  \
    }						\
*p = gds_arg_end;				\
}
/* end of STUFF_STATUS */			

#endif	/* 0/1 */

#endif /* JRD_IBERR_H */
