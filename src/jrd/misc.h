/*
 *	PROGRAM:	JRD Access Method
 *	MODULE: 	misc.h
 *	DESCRIPTION:	Miscellaneous definitions
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
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#ifndef _JRD_MISC_H_
#define _JRD_MISC_H_

#include "../jrd/dsc.h"

typedef struct stk {
	UCHAR *stk_object;
	struct stk *stk_next;
} *STK;

/* Miscellaneous defines used for stuffing a parameters block buffer */

#define PARAM_BYTE(PARAM)	(SCHAR) dtype_byte, (SCHAR) PARAM
#define PARAM_SHORT(PARAM)	(SCHAR) dtype_short, (USHORT) PARAM
#define PARAM_LONG(PARAM)	(SCHAR) dtype_long, (SLONG) PARAM
#define PARAM_STRING(PARAM)	(SCHAR) dtype_cstring, (SCHAR*) PARAM
#define PARAM_NBYTES(PARAM1,PARAM2) (SCHAR) dtype_varying, (USHORT) PARAM1, (UCHAR*) PARAM2
#define PARAM_POINTER(PARAM)	(SCHAR) dtype_long, (UCHAR*) PARAM

#endif /* _JRD_MISC_H_ */
