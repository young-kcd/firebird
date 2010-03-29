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
 *
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "MPEXL" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#ifndef JRD_IBERR_H
#define JRD_IBERR_H

#include "../jrd/gdsassert.h"
#include "../jrd/status.h"

/*
 * TMN: Leave INIT_STATUS as macro for now. At least "why.c" (what
 * is to become "y_valve.cpp" need it.
 */

#define INIT_STATUS(status)	status [0] = isc_arg_gds;\
				status [1] = 0;\
				status [2] = isc_arg_end



/* TMN: The hits just keep on coming. The macro STUFF_STATUS MUST be
 * defined so it can get instantiated in the "calling" place because of
 * the use of varargs. I trimmed this one a bit though by putting the
 * majority of the macro into a function.
 */

#define STUFF_STATUS(status_vector, status)             \
{                                                       \
    va_list args;                                       \
    va_start (args, status);                            \
    STUFF_STATUS_function(status_vector, status, args); \
}

#endif /* JRD_IBERR_H */
