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
 */


#ifndef _JRD_TIME_H_

/* This is the number of seconds in a day */
#define ONE_DAY 86400

/* MIN_YEAR and MAX_YEAR delimit the range for valid years
 * when either inserting data or performing date arithmetic
 */
#define MIN_YEAR 0001
#define MAX_YEAR 9999

#ifdef NETWARE_386
#define _JRD_TIME_H_
#include <sys/time.h>
#include <time.h>
#define sleep(nsec)	delay((nsec) * 1000)
#endif

#if (defined(VMS) || defined(PC_PLATFORM) || defined(__OS2__)) && \
	!defined(_JRD_TIME_H_)
#define _JRD_TIME_H_
#include <time.h>
#endif

#if (defined(_MSC_VER) && defined(WIN32)) || \
	(defined(__BORLANDC__) && defined(__WIN32__))
#define _JRD_TIME_H_
#include <time.h>
#endif

#ifdef LINUX
#include <libio.h>
#endif

#ifndef _JRD_TIME_H_
#define _JRD_TIME_H_
#include <sys/time.h>
#endif

#if defined(_AIX) || defined(IMP) || defined(DELTA) || defined(sgi) || \
	defined(MAC) || defined(M_I386) || defined(DARWIN)
#include <time.h>
#endif

#endif /* _JRD_TIME_H_ */
