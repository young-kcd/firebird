/*
 *	PROGRAM:	JRD Utilties
 *	MODULE:		vms_command.c
 *	DESCRIPTION:	Parse & translate VMS command line
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
#include descrip

static SCHAR output_buffer[1024], *args[16];


int VMS_parse(SCHAR *** argv, int argc)
{
/**************************************
 *
 *	V M S _ p a r s e
 *
 **************************************
 *
 * Functional description
 *	Parse a VMS foreign command line into C style
 *
 **************************************/
	SCHAR *p, *q, *out, input[256], **arg, filename, **temp;

/*
 * Rebuild the argv elements into a single string
 * rather than calling get_foreign to avoid having
 * all the arguments upcased for us by VMS. 
 * Why must they do us these favors?
 */

	q = input;
	temp = *argv;
	while (--argc) {
		for (p = *++temp; *p; p++)
			*q++ = *p;
		*q++ = ' ';
	}
	*q = 0;

	*argv = args;
	arg = args + 1;
	p = input;
	out = output_buffer;

/* 
 * Command line arguments can be switches which start with '/' or
 * an '=' (which we ignore) or file names.  Stuff them into the
 * argument vector.
 */

	while (*p) {
		filename = TRUE;
		while (*p == ' ' || *p == '\t' || *p == '=')
			p++;
		if (!*p)
			break;
		*arg++ = out;
		if (*p == '/' || *p == '-') {
			*out++ = '-';
			p++;
			filename = FALSE;
		}
		while (*p && *p != ' ' && *p != '=' && *p != '\t') {
			if (!filename && *p == '/')
				break;
			*out++ = *p++;
		}
		*out++ = 0;
	}

	return arg - args;
}
