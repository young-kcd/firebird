/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		pcsleep.c
 *	DESCRIPTION:	Sleep function which suspends execution for interval
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


/*[]------------------------------------------------------------[]*/
/*|                                                              |*/
/*|     C/C++ Run Time Library - Version 4.0                     |*/
/*|                                                              |*/
/*|                                                              |*/
/*|     Copyright (c) 1987, 1991 by Borland International        |*/
/*|     All Rights Reserved.                                     |*/
/*|                                                              |*/
/*[]------------------------------------------------------------[]*/


#include "firebird.h"
#include <dos.h>
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

/*---------------------------------------------------------------------*

Name            sleep - suspends execution for interval

Usage           unsigned sleep(unsigned seconds);

Prototype in    dos.h

Description     With a call to sleep, the current program is
                suspended from execution for the number of seconds specified
                by the argument seconds. The interval is only accurate to
                the nearest hundredth of a second, or the accuracy of the
                MS-DOS clock, whichever is less accurate.

Return value    None

*---------------------------------------------------------------------*/
void sleep(unsigned seconds)
{
	struct tm *t;
	time_t tt;
	int secs;

	time(&tt);
	t = localtime(&tt);
	while (seconds--) {
		secs = t->tm_sec;
		do
			time(&tt);
		while (secs == t->tm_sec);
	}
}
