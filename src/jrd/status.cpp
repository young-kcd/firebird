// status.cpp
//
// Copyright 2001 Mikael Nordell <tamlin@users.sourceforge.net>
//
// This file is part of the firebird RDBMS engine, and as such
// is only allowed to be used within that engine, where it may
// be used freely. Any other use is prohibited.
//
// 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "MPEXL" port
//
// 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
//
//

#include "firebird.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "../jrd/status.h"
#include "../jrd/gdsassert.h"
#include "gen/codes.h"
#include "../jrd/gds_proto.h"


/* The following function is used to stuff variable number of error message
   arguments from stack to the status_vector.   This macro should be the 
   first statement in a routine where it is invoked. */

/* Get the addresses of the argument vector and the status vector, and do
   word-wise copy. */

void STUFF_STATUS_function(ISC_STATUS* status_vector, ISC_STATUS status, va_list args)
{
	int type, len;

	ISC_STATUS* p = status_vector;

	*p++ = gds_arg_gds;
	*p++ = status;

	while ((type = va_arg(args, int)) && ((p - status_vector) < 17))
	{
		switch (*p++ = type)
		{
			case gds_arg_gds:
				*p++ = va_arg(args, ISC_STATUS);
				break;

			case gds_arg_string:
				{
					ISC_STATUS* q = va_arg(args, ISC_STATUS*);
					if (strlen((TEXT *) q) >= MAX_ERRSTR_LEN)
					{
						*(p - 1) = gds_arg_cstring;
						*p++ = (ISC_STATUS) MAX_ERRSTR_LEN;
					}
					*p++ = (ISC_STATUS) q;
				}
				break;

			case gds_arg_interpreted:
				*p++ = (ISC_STATUS) va_arg(args, TEXT *);
				break;

			case gds_arg_cstring:
				len = (int) va_arg(args, int);
				*p++ = (ISC_STATUS) (len >= MAX_ERRSTR_LEN) ? MAX_ERRSTR_LEN : len;
				*p++ = (ISC_STATUS) va_arg(args, TEXT *);
				break;

			case gds_arg_number:
				*p++ = (ISC_STATUS) va_arg(args, SLONG);
				break;

			case gds_arg_vms:
			case gds_arg_unix:
			case gds_arg_win32:
			default:
				*p++ = (ISC_STATUS) va_arg(args, int);
				break;
		}
	}
	*p = gds_arg_end;
}


/** Check that we never overrun the status vector.  The status
 *  vector is 20 elements.  The maximum is 3 entries for a 
 * type.  So check for 17 or less
 */

void PARSE_STATUS(ISC_STATUS * status_vector, int &length, int &warning)
{
	warning = 0;
	length = 0;

    int i = 0;

	for (; status_vector[i] != gds_arg_end; i++, length++)
	{
		switch (status_vector[i])
		{
		case isc_arg_warning:
			if (!warning)
				warning = i;	// find the very first
			// fallthrought intended
		case gds_arg_gds:
		case gds_arg_string:
		case gds_arg_number:
		case gds_arg_interpreted:
		case gds_arg_vms:
		case gds_arg_unix:
		case gds_arg_win32:
			i++;
			length++;
			break;

		case gds_arg_cstring:
			i += 2;
			length += 2;
			break;

		default:
			fb_assert(FALSE);
			break;
		}
	}
	if (i) {
		length++;				// gds_arg_end is counted
	}
}
