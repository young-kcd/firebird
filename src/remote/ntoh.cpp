/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		ntoh.c
 *	DESCRIPTION:	Network to host and reverse conversions
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

/* only include this code if we are not compiling for windows 32 bit
   in which case winsock.dll provides the functionality */

#ifndef __WIN32__
#include "firebird.h"
#include "../jrd/common.h"

typedef union
{
	SCHAR	string[4];
	SLONG	integer;
} either;


SLONG htonl(either value)
{
/**************************************
 *
 *	h t o n l
 *
 **************************************
 *
 * Functional description
 *	Host to network conversion.
 *
 **************************************/
	SCHAR c;

	c = value.string[0];
	value.string[0] = value.string[3];
	value.string[3] = c;

	c = value.string[1];
	value.string[1] = value.string[2];
	value.string[2] = c;

	return value.integer;
}


SLONG ntohl(either value)
{
/**************************************
 *
 *	n t o h l
 *
 **************************************
 *
 * Functional description
 *	Network to host conversion.
 *
 **************************************/
	SCHAR c;

	c = value.string[0];
	value.string[0] = value.string[3];
	value.string[3] = c;

	c = value.string[1];
	value.string[1] = value.string[2];
	value.string[2] = c;

	return value.integer;
}


#endif /* !__WIN32__ */
