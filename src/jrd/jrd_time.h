/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		time.h
 *	DESCRIPTION:	OS independent time include file
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


#ifndef _JRD_TIME_H_
#define _JRD_TIME_H_


#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#ifdef HAVE_TIME_H
#include <time.h>
#endif

#ifdef HAVE_LIBIO_H  /* was linux only - I really wonder if it is actuall y needed*/
#include <libio.h>
#endif

/* This is the number of seconds in a day */
#define ONE_DAY 86400

/* MIN_YEAR and MAX_YEAR delimit the range for valid years
 * when either inserting data or performing date arithmetic
 */
#define MIN_YEAR 0001
#define MAX_YEAR 9999

#endif /* _JRD_TIME_H_ */
