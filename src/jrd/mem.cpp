/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		mem.cpp
 *	DESCRIPTION:	UNIX memory slopping routines
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

#include "firebird.h"
#include "../jrd/common.h"


int memcmp(const UCHAR* s1, const UCHAR* s2, int n)
{
/**************************************
 *
 *	m e m c m p
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	const UCHAR *from, *to, *end;

	for (from = s2, to = s1, end = to + n; to < end; to++, from++)
		if (*to != *from)
			return (*to < *from) ? -1 : 1;

	return 0;
}


int memcpy(SCHAR* s1, const SCHAR* s2, int n)
{
/**************************************
 *
 *	m e m c p y
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	const SCHAR *from, *end;
	SCHAR* to;

	for (from = s2, to = s1, end = to + n; to < end;)
		*to++ = *from++;
}


int memset(SCHAR* s, int c, int n)
{
/**************************************
 *
 *	m e m s e t
 *
 **************************************
 *
 * Functional description
 *	Propogate a character.
 *
 **************************************/
	SCHAR *p;
	const SCHAR *end;

	for (p = s, end = p + n; p < end;)
		*p++ = c;
}

