/*
 *	PROGRAM:	JRD Journal Server
 *	MODULE:		misc.c
 *	DESCRIPTION:
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "M88K" port
 *
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <string.h>
#include "../jrd/jrd_time.h"
#include "../jrd/common.h"

#ifdef WIN_NT
#ifdef HAVE_SYS_TIMEB_H
# include <sys/timeb.h>
#endif
#include <winsock2.h>
#undef TEXT
#define TEXT	SCHAR
#endif

#include "../jrd/ods.h"
#include "../jrd/gds.h"
#include "../jrd/license.h"
#include "../journal/journal.h"

#include "../jrd/jrn.h"
#include "../journal/gjrn_proto.h"
#include "../journal/miscj_proto.h"
#include "../jrd/gds_proto.h"

#define LETTER(c)	(c >= 'A' && c <= 'Z')
#define DIGIT(c)	(c >= '0' && c <= '9')

#define DECIMAL_LIMIT	(1 << 30) / 5
#define TODAY		"TODAY"
#define NOW		"NOW"
#define TOMORROW	"TOMORROW"
#define YESTERDAY	"YESTERDAY"


static void now_to_date(struct tm *, SLONG[2]);

static TEXT *months[] = {
	"JANUARY",
	"FEBRUARY",
	"MARCH",
	"APRIL",
	"MAY",
	"JUNE",
	"JULY",
	"AUGUST",
	"SEPTEMBER",
	"OCTOBER",
	"NOVEMBER",
	"DECEMBER",
	0
};


UCHAR *MISC_alloc_jrnl(SLONG size)
{
/**************************************
 *
 *	M I S C _ a l l o c _ j r n l
 *
 **************************************
 *
 * Functional description
 *	Allocate block of memory.
 *	Zero it out.
 *
 **************************************/
	UCHAR *p, *block;

	if (!(block = (UCHAR *) gds__alloc((SLONG) size))) {
		GJRN_printf(109, NULL, NULL, NULL, NULL);	// msg 109: malloc failed???
		return NULL;
	}

	p = block;
	do
		*p++ = 0;
	while (--size);

	return block;
}


void MISC_down_case(UCHAR * in,
					UCHAR * out)
{
/**************************************
 *
 *	M I S C _ d o w n _ c a s e
 *
 **************************************
 *
 * Functional description
 *	Copy a string, down casing as we go.
 *
 **************************************/
	UCHAR c;

	while (c = *in++)
		*out++ = (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;

	*out = 0;
}


void MISC_free_jrnl( int *block)
{
/**************************************
 *
 *	M I S C _ f r e e _ j r n l
 *
 **************************************
 *
 * Functional description
 *	Release an unwanted block.
 *
 **************************************/

	gds__free(block);
}


bool MISC_get_line(TEXT * prompt,
				   TEXT * buffer,
				   SSHORT size)
{
/**************************************
 *
 *	M I S C _ g e t _ l i n e
 *
 **************************************
 *
 * Functional description
 *	Eat a command line throwing away extra bytes.  Return FALSE
 *	if end of file.
 *
 **************************************/
	TEXT *end;
	SSHORT c;
	SSHORT count;

	GJRN_output("%s", prompt);
	end = buffer + size - 1;
	count = 0;

	if (sw_service_gjrn)
		GJRN_output("\001");
	fflush(stdout);

	while ((c = getchar()) != '\n') {
		count++;
		if (c == EOF) {
			*buffer = 0;
			GJRN_output("%s", "\n");
			return false;
		}
		if (buffer < end)
			*buffer++ = c;
	}

	*buffer = 0;

	if (!count)
		return false;

	return true;
}


void MISC_get_new_value(SCHAR * string1,
						SCHAR * buffer,
						int size)
{
/**************************************
 *
 *	M I S C _ g e t _ n e w _ v a l u e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SCHAR temp[25];
	SCHAR buff[MSG_LENGTH];

	GJRN_output("%s is: %s\n", string1, buffer);

	GJRN_get_msg(84, buff, NULL, NULL, NULL);
	MISC_get_line(buff, temp, sizeof(temp));

	MISC_down_case((UCHAR*) temp, (UCHAR*) temp);

	if (temp[0] != 'y')
		return;

	GJRN_get_msg(85, buff, NULL, NULL, NULL);
	MISC_get_line(buff, buffer, size);
}


void MISC_get_time( struct timeval *current)
{
/**************************************
 *
 *      M I S C _ g e t _ t i m e
 *
 **************************************
 *
 * Functional description
 *      Get the current time of day.
 *
 **************************************/
#ifdef HAVE_GETTIMEOFDAY
#ifdef GETTIMEOFDAY_RETURNS_TIMEZONE
	(void)gettimeofday(current, (struct timezone *)0);
#else
	(void)gettimeofday(current);
#endif
#else
	struct timeb buffer;

	ftime(&buffer);
	current->tv_sec = buffer.time;
	current->tv_usec = (SLONG) buffer.millitm * 1000L;
#endif
}


void MISC_get_wal_info(LTJC * msg,
					   SCHAR * db_name,
					   SCHAR * dir_name)
{
/**************************************
 *
 *	M I S C _ g e t _ w a l _ i n f o
 *
 **************************************
 *
 * Functional description
 *	get wal info from message
 *
 **************************************/
	SCHAR *p, *q, *b;
	SSHORT l;

	db_name[0] = 0;
	dir_name[0] = 0;

	for (p = msg->ltjc_data; p[0] != JRNW_END; p += 2 + p[1]) {
		switch (*p) {
		case gds_dpb_wal_backup_dir:
			b = dir_name;
			q = &p[2];
			l = p[1];
			if (l)
				do
					*b++ = *q++;
				while (--l);
			*b++ = 0;
			break;

		case JRNW_DB_NAME:
			b = db_name;
			q = &p[2];
			l = p[1];
			if (l)
				do
					*b++ = *q++;
				while (--l);
			*b++ = 0;
			break;

		default:;
		}
	}
}


void MISC_print_journal_syntax(void)
{
/**************************************
 *
 *	M I S C _ p r i n t _ j o u r n a l _ s y n t a x
 *
 **************************************
 *
 * Functional description
 *	print possible options
 *
 **************************************/

	GJRN_print_syntax();

	GJRN_printf(7, NULL, NULL, NULL, NULL);
	// msg 7: <utility> : -enable | -disable | -online_dump | -server | -console | -restore

// server options
	GJRN_printf(75, NULL, NULL, NULL, NULL); // msg 75: <server options>
	GJRN_printf(8, NULL, NULL, NULL, NULL); // msg 8: -j <journal directory>

// console options
	GJRN_printf(76, NULL, NULL, NULL, NULL); // msg 76: <console options>
	GJRN_printf(8, NULL, NULL, NULL, NULL); // msg 8: -j <journal directory>
	GJRN_printf(77, NULL, NULL, NULL, NULL); // msg 77: -s <command>

// enable options
	GJRN_printf(78, NULL, NULL, NULL, NULL); // msg 78: <enable options>
	GJRN_printf(8, NULL, NULL, NULL, NULL); // msg 8: -j <journal directory>
	GJRN_printf(9, NULL, NULL, NULL, NULL); // msg 9: -a <archive base name>

// disable options
	GJRN_printf(79, NULL, NULL, NULL, NULL); // msg 79: <disable options>

// restore options
	GJRN_printf(80, NULL, NULL, NULL, NULL); // msg 80: <restore options>
	GJRN_printf(8, NULL, NULL, NULL, NULL); // msg 8: -j <journal directory>
	GJRN_printf(11, NULL, NULL, NULL, NULL); // msg 11: -u <until options>
	GJRN_printf(10, NULL, NULL, NULL, NULL); // msg 10: -i              -interactive mode
	GJRN_printf(142, NULL, NULL, NULL, NULL); // msg 142: -m <output message file>
	GJRN_printf(196, NULL, NULL, NULL, NULL); // msg 196: -a\t\t-activate partially recovered database
	GJRN_printf(197, NULL, NULL, NULL, NULL); // msg 197: \t\t-p <database name for partial recovery>

// online dump options
	GJRN_printf(81, NULL, NULL, NULL, NULL); // msg 81: <online dump options>
	GJRN_printf(10, NULL, NULL, NULL, NULL); // msg 10: -i              -interactive mode
	GJRN_printf(82, NULL, NULL, NULL, NULL); // msg 82: -f <file name>
	GJRN_printf(83, NULL, NULL, NULL, NULL); // msg 83: -s <file size>

	GJRN_abort(0);
}


int MISC_time_convert(TEXT * string,
					  USHORT length,
					  SLONG date[2])
{
/**************************************
 *
 *	M I S C _ t i m e _ c o n v e r t
 *
 **************************************
 *
 * Functional description
 *	Convert an arbitrary string to a date.  Return FB_SUCCESS if successful,
 *	otherwise FB_FAILURE.
 *
 **************************************/
	TEXT c, *p, temp[15], *t, *end, **month_ptr, *m;
	USHORT n, month_position, i, components[7];
	SLONG xclock;
	struct tm times, times2;

	p = string;
	end = p + length;
	month_position = 0;

	xclock = time(0);
	times2 = *localtime(&xclock);

	for (i = 0; i < 7; i++)
		components[i] = 0;

// Parse components

	for (i = 0; i < 7; i++) {

		// Skip leading blanks.  If we run out of characters, we're done
		// with parse.

		while (p < end && (*p == ' ' || *p == '\t'))
			p++;
		if (p == end)
			break;

		// Handle digit or character strings

		c = UPPER(*p);
		if (DIGIT(c)) {
			n = 0;
			while (p < end && DIGIT(*p))
				n = n * 10 + *p++ - '0';
		}
		else if (LETTER(c)) {
			t = temp;
			while (p < end && LETTER(c)) {
				c = UPPER(*p);
				if (!LETTER(c))
					break;
				*t++ = c;
				p++;
			}
			*t = 0;
			month_ptr = months;
			while (true) {
				if (!*month_ptr) {

					// it's not a month name, so it's either a magic word or
					// a non-date string.  If there are more characters, it's bad

					while (++p < end)
						if (*p != ' ' && *p != '\t' && *p != 0)
							return FB_FAILURE;
					if (strcmp(temp, NOW) == 0) {
						now_to_date(&times2, date);
						return FB_SUCCESS;
					}
					times2.tm_hour = times2.tm_min = times2.tm_sec = 0;
					gds__encode_date(&times2, (GDS_QUAD_t*) date);
					if (strcmp(temp, TODAY) == 0)
						return FB_SUCCESS;
					if (strcmp(temp, TOMORROW) == 0) {
						++date[0];
						return FB_SUCCESS;
					}
					if (strcmp(temp, YESTERDAY) == 0) {
						--date[0];
						return FB_SUCCESS;
					}
					return FB_FAILURE;
				}
				for (t = temp, m = *month_ptr++; *t && *t == *m; t++, m++);
				if (!*t)
					break;
			}
			n = month_ptr - months;
			month_position = i;
		}
		else
			return FB_FAILURE;
		components[i] = n;
		while (p < end && (*p == ' ' || *p == '\t'))
			p++;
		if (*p == '/' || *p == '-' || *p == ',' || *p == ':') {
			p++;
			continue;
		}
		if (*p == '.') {
			if (!month_position && i < 2)
				month_position = 1;
			p++;
			continue;
		}
	}

// Slide things into day, month, year form

	if (!components[0] && !components[1] && !components[2]) {
		date[0] = date[1] = 0;
		return FB_SUCCESS;
	}

	if (month_position) {
		times.tm_mon = components[1];
		times.tm_mday = components[0];
	}
	else {
		times.tm_mon = components[0];
		times.tm_mday = components[1];
	}

// Handle defaulting of year

	if ((times.tm_year = components[2]) == 0)
		times.tm_year = times2.tm_year + 1900;
	else if (times.tm_year < 100) {
		if (times.tm_year < (times2.tm_year - 50) % 100)
			times.tm_year += 2000;
		else
			times.tm_year += 1900;
	}

	times.tm_year -= 1900;
	times.tm_mon -= 1;
	times.tm_hour = components[3];
	times.tm_min = components[4];
	times.tm_sec = components[5];

// convert day/month/year to Julian and validate result

	gds__encode_date(&times, (GDS_QUAD_t*) date);
	gds__decode_date((GDS_QUAD_t*)date, &times2);

	if (times.tm_year != times2.tm_year ||
		times.tm_mon != times2.tm_mon || times.tm_mday != times2.tm_mday)
		return FB_FAILURE;

	return FB_SUCCESS;
}


static void now_to_date(struct tm *xtime,
						SLONG date[2])
{
/**************************************
 *
 *	n o w _ t o _ d a t e
 *
 **************************************
 *
 * Functional description
 *	Convert "now" (or best guess) to
 *	a date attempting to get millisecond
 *	precision.  This unfortunately means
 *	that we use one routine for VAX, and
 *	another for Unix.
 *
 **************************************/

	gds__encode_date(xtime, (GDS_QUAD_t*) date);
}
