/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		gds.cpp
 *	DESCRIPTION:	User callable routines
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "IMP" port
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "M88K" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 * 2003.05.11 Nickolay Samofatov - rework temp stuff
 *
 */

// 11 Sept 2002 Nickolay Samofatov
// this defined in included dsc2.h
//#define ISC_TIME_SECONDS_PRECISION		10000L
//#define ISC_TIME_SECONDS_PRECISION_SCALE	-4

#include "firebird.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/gdsassert.h"
#include "../jrd/file_params.h"
#include "../jrd/msg_encode.h"
#include "../jrd/iberr.h"
#include "../jrd/gds_proto.h"
#include "../jrd/os/path_utils.h"
#include "../jrd/dsc.h"
#include "../jrd/constants.h"

#include "../common/classes/locks.h"
#include "../common/classes/timestamp.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#include <errno.h>

#include <stdarg.h>
#include "../jrd/jrd_time.h"

#if defined(WIN_NT)
#include <io.h> // umask, close, lseek, read, open, _sopen
#include <process.h>
#include <sys/types.h>
#include <sys/timeb.h>
#endif

#ifdef VMS
#include <file.h>
#include <perror.h>
#include <descrip.h>
#include <types.h>
#include <stat.h>
#include <rmsdef.h>

#else /* !VMS */

#include <sys/types.h>
#include <sys/stat.h>
#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif

#ifndef O_RDWR
#include <fcntl.h>
#endif

#ifdef UNIX
#if (defined SUPERSERVER) || (defined SOLARIS)
#include <sys/mman.h>
#include <sys/resource.h>
#include "../jrd/err_proto.h"
#endif
#endif

#endif /* VMS */

#include "../common/config/config.h"

// Turn on V4 mutex protection for gds__alloc/free 
// 03/23/2003 BRS. Those defines don't do anything, V4_ macros are defined in thd.h 
// but this file is included before this definition and so the macros are defined as empty
// Those definitions are commented to allow V4_ macros to be inside V4_THREADING ifdefs
// The include chain is
// gdsassert.h -> gds_proto.h -> fil.h -> thd.h
//
//#ifdef WIN_NT
//#define V4_THREADING
//#endif
//
//#ifdef SOLARIS_MT
//#define V4_THREADING
//#endif
//
//#ifdef SUPERSERVER
//#define V4_THREADING			// RFM: 9/22/2000 fix from Inprise tree,
//								// Inprise bug 114840
//#endif
//
// The following ifdef was added to build thread safe gds shared
//  library on linux platform. It seems the gdslib works now (20020220)
//  with thread enabled applications. Anyway, more tests should be 
//  done as I don't have deep knowledge of the interbase/firebird 
//  engine and this change may imply side effect I haven't known 
//  about yet. Tomas Nejedlik (tomas@nejedlik.cz)
//
//#if ((defined(LINUX) || defined(FREEBSD)) && defined(SUPERCLIENT))
//#define V4_THREADING
//#endif
//


#ifdef SUPERSERVER
static const TEXT gdslogid[] = " (Server)";
#else
#ifdef SUPERCLIENT
static const TEXT gdslogid[] = " (Client)";
#else
static const TEXT gdslogid[] = "";
#endif
#endif

static const char* FB_PID_FILE = "fb_%d";

#include "gen/sql_code.h"
#include "../jrd/thd.h"
#include "gen/iberror.h"
#include "../jrd/ibase.h"

#include "../jrd/blr.h"
#include "../jrd/msg.h"
#include "../jrd/fil.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#ifndef REQUESTER
#include "../jrd/os/isc_i_proto.h"
#endif

#ifdef WIN_NT
#define _WINSOCKAPI_
#include <share.h>
#include "err_proto.h"
#undef leave
#endif /* WIN_NT */

// Number of times to try to generate new name for temporary file
const int MAX_TMPFILE_TRIES		= 256;

static char *ib_prefix = 0;
static char *ib_prefix_lock = 0;
static char *ib_prefix_msg = 0;

#include "gen/msgs.h"

#ifndef O_BINARY
#define O_BINARY	0
#endif

const SLONG GENERIC_SQLCODE		= -999;

static char ib_prefix_val[MAXPATHLEN];
static char ib_prefix_lock_val[MAXPATHLEN];
static char ib_prefix_msg_val[MAXPATHLEN];


#include "../include/fb_types.h"
#include "../jrd/jrd.h"
#include "../common/utils_proto.h"

using Firebird::TimeStamp;

// This structure is used to parse the firebird.msg file.
struct gds_msg
{
	ULONG msg_top_tree;
	int msg_file;
	USHORT msg_bucket_size;
	USHORT msg_levels;
	SCHAR msg_bucket[1];
};


// CVC: This structure has a totally different layout than "class ctl" from
// blob_filter.h and "struct isc_blob_ctl" from ibase.h. These two should match
// for blob filters to work. However, this one is private to gds.cpp and hence
// I renamed it gds_ctl to avoid confusion and possible name clashes.
// However, filters.cpp calls gds__print_blr(), but this struct is not shared
// between the two modules.
struct gds_ctl
{
	const UCHAR* ctl_blr;				/* Running blr string */
	const UCHAR* ctl_blr_start;		/* Original start of blr string */
	FPTR_PRINT_CALLBACK ctl_routine; /* Call back */
	void* ctl_user_arg;			/* User argument */
	TEXT* ctl_ptr;
	SSHORT ctl_language;
	TEXT ctl_buffer[PRETTY_BUFFER_SIZE];
};

#ifdef DEV_BUILD
void GDS_breakpoint(int);
#endif


static void		blr_error(gds_ctl*, const TEXT*, ...) ATTRIBUTE_FORMAT(2,3);
static void		blr_format(gds_ctl*, const char*, ...) ATTRIBUTE_FORMAT(2,3);
static void		blr_indent(gds_ctl*, SSHORT);
static void		blr_print_blr(gds_ctl*, UCHAR);
static SCHAR	blr_print_byte(gds_ctl*);
static SCHAR	blr_print_char(gds_ctl*);
static void		blr_print_cond(gds_ctl*);
static int		blr_print_dtype(gds_ctl*);
static void		blr_print_join(gds_ctl*);
static SLONG	blr_print_line(gds_ctl*, SSHORT);
static void		blr_print_verb(gds_ctl*, SSHORT);
static int		blr_print_word(gds_ctl*);

static void		init(void);
static void		sanitize(TEXT*);

static void		safe_concat_path(TEXT* destbuf, const TEXT* srcbuf);

// New functions that try to be safe.
static SLONG safe_interpret(char* const s, const int bufsize,
	const ISC_STATUS** const vector);


/* Generic cleanup handlers */

struct clean
{
	struct clean*	clean_next;
	void			(*clean_routine)(void*);
	void*			clean_arg;
};

typedef clean *CLEAN;

static CLEAN	cleanup_handlers = NULL;
static gds_msg* global_default_msg = NULL;
static bool initialized = false;

void* API_ROUTINE gds__alloc_debug(SLONG size_request,
                                   const TEXT* filename,
                                   ULONG lineno)
{
	return getDefaultMemoryPool()->allocate_nothrow(size_request, 0
#ifdef DEBUG_GDS_ALLOC
		, filename, lineno
#endif
	);
}

ULONG API_ROUTINE gds__free(void* blk)
{
	getDefaultMemoryPool()->deallocate(blk);
	return 0;
}

#ifdef UNIX
static SLONG gds_pid = 0;
#endif

/* VMS structure to declare exit handler */

#ifdef VMS
static SLONG exit_status = 0;
static struct
{
	SLONG link;
	int (*exit_handler) ();
	SLONG args;
	SLONG arg[1];
} exit_description;
#endif

/* BLR Pretty print stuff */

#define BLR_BYTE	*(control->ctl_blr)++
#define PUT_BYTE(byte)	*(control->ctl_ptr)++ = byte

const int op_line		= 1;
const int op_verb		= 2;
const int op_byte		= 3;
const int op_word		= 4;
const int op_pad		= 5;
const int op_dtype		= 6;
const int op_message	= 7;
const int op_literal	= 8;
const int op_begin		= 9;
const int op_map		= 10;
const int op_args		= 11;
const int op_union		= 12;
const int op_indent		= 13;
const int op_join		= 14;
const int op_parameters	= 15;
const int op_error_handler	= 16;
const int op_set_error	= 17;
const int op_literals	= 18;
const int op_relation	= 20;
const int op_exec_into	= 21;
const int op_cursor_stmt	= 22;

static const UCHAR
	/* generic print formats */
	zero[]		= { op_line, 0 },
	one[]		= { op_line, op_verb, 0},
	two[]		= { op_line, op_verb, op_verb, 0},
	three[]		= { op_line, op_verb, op_verb, op_verb, 0},
	field[]		= { op_byte, op_byte, op_literal, op_pad, op_line, 0},
	byte[]		= { op_byte, op_line, 0},
	byte_args[] = { op_byte, op_line, op_args, 0},
	byte_byte[] = { op_byte, op_byte, op_line, 0},
	byte_verb[] = { op_byte, op_line, op_verb, 0},
	byte_verb_verb[] = { op_byte, op_line, op_verb, op_verb, 0},
	byte_literal[] = { op_byte, op_literal, op_line, 0},
	byte_byte_verb[] = { op_byte, op_byte, op_line, op_verb, 0},
	parm[]		= { op_byte, op_word, op_line, 0},	/* also field id */

	parm2[]		= { op_byte, op_word, op_word, op_line, 0},
	parm3[]		= { op_byte, op_word, op_word, op_word, op_line, 0},

	/* formats specific to a verb */
	begin[]		= { op_line, op_begin, op_verb, 0},
	literal[]	= { op_dtype, op_literal, op_line, 0},
	message[]	= { op_byte, op_word, op_line, op_message, 0},
	rse[]		= { op_byte, op_line, op_begin, op_verb, 0},
	relation[]	= { op_byte, op_literal, op_pad, op_byte, op_line, 0},
	relation2[] = { op_byte, op_literal, op_line, op_indent, op_byte,
					op_literal, op_pad, op_byte, op_line, 0},
	aggregate[] = { op_byte, op_line, op_verb, op_verb, op_verb, 0},
	rid[]		= { op_word, op_byte, op_line, 0},
	rid2[]		= { op_word, op_byte, op_literal, op_pad, op_byte, op_line, 0},
	union_ops[] = { op_byte, op_byte, op_line, op_union, 0},
    map[]  	    = { op_word, op_line, op_map, 0},
	function[]	= { op_byte, op_literal, op_byte, op_line, op_args, 0},
	gen_id[]	= { op_byte, op_literal, op_line, op_verb, 0},
	declare[]	= { op_word, op_dtype, op_line, 0},
	variable[]	= { op_word, op_line, 0},
	indx[]		= { op_line, op_verb, op_indent, op_byte, op_line, op_args, 0},
	find[]		= { op_byte, op_verb, op_verb, op_indent, op_byte, op_line, op_args, 0},
	seek[]		= { op_line, op_verb, op_verb, 0},
	join[]		= { op_join, op_line, 0},
	exec_proc[] = { op_byte, op_literal, op_line, op_indent, op_word, op_line,
					op_parameters, op_indent, op_word, op_line, op_parameters, 0},
	procedure[] = { op_byte, op_literal, op_pad, op_byte, op_line, op_indent,
					op_word, op_line, op_parameters, 0},
	pid[]		= { op_word, op_pad, op_byte, op_line, op_indent, op_word,
					op_line, op_parameters, 0},
	error_handler[] = { op_word, op_line, op_error_handler, 0},
	set_error[] = { op_set_error, op_line, 0},
	cast[]		= { op_dtype, op_line, op_verb, 0},
	indices[]	= { op_byte, op_line, op_literals, 0},
	lock_relation[] = { op_line, op_indent, op_relation, op_line, op_verb, 0},
	range_relation[] = { op_line, op_verb, op_indent, op_relation, op_line, 0},
	extract[]	= { op_line, op_byte, op_verb, 0},
	user_savepoint[]	= { op_byte, op_byte, op_literal, op_line, 0},
	exec_into[] = { op_word, op_line, op_indent, op_exec_into, 0},
	dcl_cursor[] = { op_word, op_line, op_verb, 0},
	cursor_stmt[] = { op_cursor_stmt, 0 };

#include "../jrd/blp.h"

const char* const FB_LOCK_ENV		= "FIREBIRD_LOCK";
const char* const FB_MSG_ENV		= "FIREBIRD_MSG";
const char* const FB_TMP_ENV		= "FIREBIRD_TMP";

#ifdef WIN_NT
#define EXPAND_PATH(relative, absolute)		_fullpath(absolute, relative, MAXPATHLEN)
#define COMPARE_PATH(a, b)			_stricmp(a, b)
#else
#define EXPAND_PATH(relative, absolute)		realpath(relative, absolute)
#define COMPARE_PATH(a, b)			strcmp(a, b)
#endif


// This function is very crude, but signal-safe.
// CVC: This function converts and ULONG into a string. I renamed the param
// maxlen to minlen because it represents the minimum length the number will
// be printed in. However, this means buffer should be large enough to contain
// any ULONG (11 bytes) and thus prevent a buffer overflow. If minlen >= 11,
// then buffer should have be of size = (minlen + 1). A small anomaly means
// that when "value" is zero, only filler is stored in buffer: no ACSCII(48)
// appears in buffer, unless filler itself is ASCII(48) == '0'.
void gds__ulstr(char* buffer, ULONG value, const int minlen, const char filler)
{
	ULONG n = value;
	int c = 0;
	while (n) {
		n = n / 10;
		c++;
	}
	if (minlen > c)
		c = minlen;
		
	char* p = buffer + c;
	while (value) {
		*--p = '0' + (value % 10);
		value = value / 10;
	}
	while (p != buffer) {
		*--p = filler;
	}
	buffer[c] = 0;
}


ISC_STATUS API_ROUTINE gds__decode(ISC_STATUS code, USHORT* fac, USHORT*
	code_class)
{
/**************************************
 *
 *	g d s _ $ d e c o d e
 *
 **************************************
 *
 * Functional description
 *	Translate a status codes from system dependent form to
 *	network form.
 *
 **************************************/

	if (!code)
	{
		return FB_SUCCESS;
	}
	else if ((code & ISC_MASK) != ISC_MASK)
	{
		/* not an ISC error message */
		return code;
	}

	*fac = GET_FACILITY(code);
	*code_class = GET_CLASS(code);
	return GET_CODE(code);

}


void API_ROUTINE isc_decode_date(const ISC_QUAD* date, void* times_arg)
{
/**************************************
 *
 *	i s c _ d e c o d e _ d a t e
 *
 **************************************
 *
 * Functional description
 *	Convert from internal timestamp format to UNIX time structure.
 *
 *	Note: this API is historical - the prefered entrypoint is
 *	isc_decode_timestamp
 *
 **************************************/
	isc_decode_timestamp((const GDS_TIMESTAMP*) date, times_arg);
}


void API_ROUTINE isc_decode_sql_date(const GDS_DATE* date, void* times_arg)
{
/**************************************
 *
 *	i s c _ d e c o d e _ s q l _ d a t e
 *
 **************************************
 *
 * Functional description
 *	Convert from internal DATE format to UNIX time structure.
 *
 **************************************/
	tm* times = (struct tm*) times_arg;
	TimeStamp::decode_date(*date, times);
}


void API_ROUTINE isc_decode_sql_time(const GDS_TIME* sql_time, void* times_arg)
{
/**************************************
 *
 *	i s c _ d e c o d e _ s q l _ t i m e
 *
 **************************************
 *
 * Functional description
 *	Convert from internal TIME format to UNIX time structure.
 *
 **************************************/
	tm* times = (struct tm*) times_arg;
	memset(times, 0, sizeof(*times));

	const ULONG minutes = *sql_time / (ISC_TIME_SECONDS_PRECISION * 60);
	times->tm_hour = minutes / 60;
	times->tm_min = minutes % 60;
	times->tm_sec = (*sql_time / ISC_TIME_SECONDS_PRECISION) % 60;
}


void API_ROUTINE isc_decode_timestamp(const GDS_TIMESTAMP* date, void* times_arg)
{
/**************************************
 *
 *	i s c _ d e c o d e _ t i m e s t a m p
 *
 **************************************
 *
 * Functional description
 *	Convert from internal timestamp format to UNIX time structure.
 *
 *	Note: This routine is intended only for public API use. Engine itself and 
 *  utilities should be using TimeStamp class directly in type-safe manner.
 *
 **************************************/

	Firebird::TimeStamp(*date).decode(reinterpret_cast<tm*>(times_arg));
}


ISC_STATUS API_ROUTINE gds__encode(ISC_STATUS code, USHORT facility)
{
/**************************************
 *
 *	g d s _ $ e n c o d e
 *
 **************************************
 *
 * Functional description
 *	Translate a status codes from network format to system
 *	dependent form.
 *
 **************************************/

	if (!code)
		return FB_SUCCESS;

	return ENCODE_ISC_MSG(code, facility);
}


void API_ROUTINE isc_encode_date(const void* times_arg, ISC_QUAD* date)
{
/**************************************
 *
 *	i s c _ e n c o d e _ d a t e
 *
 **************************************
 *
 * Functional description
 *	Convert from UNIX time structure to internal timestamp format.
 *
 *	Note: This API is historical -- the prefered entrypoint is
 *	isc_encode_timestamp
 *
 **************************************/
	Firebird::TimeStamp temp;
	temp.encode(reinterpret_cast<const tm*>(times_arg));
	*date = (ISC_QUAD&)temp.value();
}


void API_ROUTINE isc_encode_sql_date(const void* times_arg, GDS_DATE* date)
{
/**************************************
 *
 *	i s c _ e n c o d e _ s q l _ d a t e
 *
 **************************************
 *
 * Functional description
 *	Convert from UNIX time structure to internal date format.
 *
 **************************************/

	*date = TimeStamp::encode_date((const struct tm*) times_arg);
}


void API_ROUTINE isc_encode_sql_time(const void* times_arg, GDS_TIME* isc_time)
{
/**************************************
 *
 *	i s c _ e n c o d e _ s q l _ t i m e
 *
 **************************************
 *
 * Functional description
 *	Convert from UNIX time structure to internal TIME format.
 *
 **************************************/
	const tm* times = (const struct tm*) times_arg;
	*isc_time = ((times->tm_hour * 60 + times->tm_min) * 60 +
				 times->tm_sec) * ISC_TIME_SECONDS_PRECISION;
}


void API_ROUTINE isc_encode_timestamp(const void* times_arg, GDS_TIMESTAMP* date)
{
/**************************************
 *
 *	i s c _ e n c o d e _ t i m e s t a m p
 *
 **************************************
 *
 * Functional description
 *	Convert from UNIX time structure to internal timestamp format.
 *
 *	Note: This routine is intended only for public API use. Engine itself and 
 *  utilities should be using TimeStamp class directly in type-safe manner.
 *
 **************************************/
	Firebird::TimeStamp temp;
	temp.encode(reinterpret_cast<const tm*>(times_arg));
	*date = temp.value();
}


#ifdef DEV_BUILD

void GDS_breakpoint(int parameter)
{
/**************************************
 *
 *	G D S _ b r e a k p o i n t 
 *
 **************************************
 *
 * Functional description
 *	Just a handy place to put a debugger breakpoint
 *	Calls to this can then be embedded for DEV_BUILD
 *	using the BREAKPOINT(x) macro.
 *
 **************************************/
/* Put a breakpoint here via the debugger */
}
#endif


SINT64 API_ROUTINE isc_portable_integer(const UCHAR* ptr, SSHORT length)
{
/**************************************
 *
 *	i s c _ p o r t a b l e _ i n t e g e r
 *
 **************************************
 *
 * Functional description
 *	Pick up (and convert) a Little Endian (VAX) style integer 
 *      of length 1, 2, 4 or 8 bytes to local system's Endian format.
 *
 *   various parameter blocks (i.e., dpb, tpb, spb) flatten out multibyte
 *   values into little endian byte ordering before network transmission.
 *   programs need to use this function in order to get the correct value
 *   of the integer in the correct Endian format.
 *
 * **Note**
 *
 *   This function is similar to gds__vax_integer() in functionality.
 *   The difference is in the return type. Changed from SLONG to SINT64
 *   Since gds__vax_integer() is a public API routine, it could not be
 *   changed. This function has been made public so gbak can use it.
 *
 **************************************/
	SINT64 value;
	SSHORT shift;

	fb_assert(length <= 8);
	value = shift = 0;

	while (--length >= 0) {
		value += ((SINT64) *ptr++) << shift;
		shift += 8;
	}

	return value;
}

void API_ROUTINE gds_alloc_flag_unfreed(void *blk)
{
/**************************************
 *
 *	g d s _ a l l o c _ f l a g _ u n f r e e d
 *
 **************************************
 *
 * Functional description
 *	Flag a buffer as "known to be unfreed" so we
 *	don't report it in gds_alloc_report
 *
 **************************************/
// JMB: need to rework this for the new pools
// Skidder: Not sure we need to rework this routine. 
// What we really need is to fix all memory leaks including very old.
}


void API_ROUTINE gds_alloc_report(ULONG flags, const char* filter_filename, int lineno)
{
/**************************************
 *
 *	g d s _ a l l o c _ r e p o r t 
 *
 **************************************
 *
 * Functional description
 *	Print buffers that might be memory leaks.
 *	Or that might have been clobbered.
 *
 **************************************/
// Skidder: Calls to this function must be replaced with MemoryPool::print_contents
	char report_name[MAXPATHLEN];
	gds__prefix(report_name, "fbsrvreport.txt");
	// Our new facilities don't expose flags for reporting.
	const bool used_only = !(flags & ALLOC_verbose);
	getDefaultMemoryPool()->print_contents(report_name, used_only, filter_filename);
}


/**
fb_interpret

	@brief Buffer overrun-aware version of the old gds__interprete
	without the double underscore and without the misspelling.
	Translate a status code with arguments to a string.  Return the
	length of the string while updating the vector address.  If the
	message is null (end of messages) or invalid, return 0;
	
	@param s the output buffer where a human readable version of the error is put
	@param bufsize the size of the output buffer
	@param vector the input, the address of const pointer to the status vector
	    that was filled by an API call that reported an error. The function
	    positions the pointer on the next element of the vector.
**/
SLONG API_ROUTINE fb_interpret(char* s, int bufsize, const ISC_STATUS** vector)
{
	return safe_interpret(s, bufsize, vector);
}


/* CVC: This non-const signature is needed for compatibility. The reason is
that, unlike int* that can be assigned to const int* transparently to the caller,
int** CANNOT be assigned to const int**, so applications would have to be
fixed to provide such const int**; while it may be more correct from the
semantic POV, we can't estimate how much work it's for app developers.
Therefore, we go back to the old signature and provide fb_interpret
for compliance, to be used inside the engine, too. September, 2003.
November, 2004: We agree that fb_interpret is the new, safe interface.
Both gds__interprete and isc_interprete are deprecated. */

SLONG API_ROUTINE gds__interprete(char* s, ISC_STATUS** vector)
{
/**************************************
 *
 *	g d s _ $ i n t e r p r e t e
 *
 **************************************
 *
 * Functional description
 * See safe_interpret for details. Now this is a wrapper for that function.
 * CVC: Since this routine doesn't get the size of the input buffer,
 * it's DEPRECATED and we'll assume the buffer size was twice MAXPATHLEN.
 *
 **************************************/
	return safe_interpret(s, MAXPATHLEN << 1, const_cast<const ISC_STATUS**>(vector));
}

 
/**
safe_interpret

	@brief Translate a status code with arguments to a string.  Return the
	length of the string while updating the vector address.  If the
	message is null (end of messages) or invalid, return 0;

	@param s the output buffer where a human readable version of the error is put
	@param bufsize the size of the output buffer
	@param vector the input, the address of const pointer to the status vector
	    that was filled by an API call that reported an error. The function
	    positions the pointer on the next element of the vector.

**/
static SLONG safe_interpret(char* const s, const int bufsize,
	const ISC_STATUS** const vector)
{
	// CVC: It doesn't make sense to provide a buffer smaller than 50 bytes.
	// Return error otherwise.
	// Also, if the first element of the vector doesn't signal an error, return.
	if (!**vector || bufsize < 50)
		return 0;

	const ISC_STATUS* v;
	ISC_STATUS code;
	/* handle a case: "no errors, some warning(s)" */
	if ((*vector)[1] == 0 && (*vector)[2] == isc_arg_warning) {
		v = *vector + 4;
		code = (*vector)[3];
	}
	else {
		v = *vector + 2;
		code = (*vector)[1];
	}

	const TEXT* args[10];
	const TEXT** arg = args;
	const char* const* const argend = arg + FB_NELEM(args);

	/* Parse and collect any arguments that may be present */
	
	TEXT* p = 0;
	const TEXT* q;
	int temp_len = BUFFER_SMALL;
	TEXT* temp = NULL;

	for (;;)
	{
		// CVC: Close overflow in "args".
		if (arg >= argend)
			break;

		int len;
		const UCHAR x = (UCHAR) *v++;
		switch (x)
		{
		case isc_arg_string:
		case isc_arg_number:
			*arg++ = (TEXT*) *v++;
			continue;

		case isc_arg_cstring:
			if (!temp) {
				/* We need a temporary buffer when cstrings are involved.
				   Give up if we can't get one. */

				p = temp = (TEXT*) gds__alloc((SLONG) temp_len);
				/* FREE: at procedure exit */
				if (!temp)		/* NOMEM: */
					return 0;
			}
			len = 1 + (int) *v++; // CVC: Add one for the needed terminator.
			q = (const TEXT*) *v++;

			// ensure that we do not overflow the buffer allocated
			len = (temp_len < len) ? temp_len : len;
			if (len)
			{
				// CVC: On the next iteration, we don't have the full buffer
				// but only the remainer of the buffer. We decrement here before
				// the loop changes "len".
				temp_len -= len;
				*arg++ = p;
				// We'll silently truncate the parameter to our available space.
				while (--len) // CVC: Decrement first to make room for the null terminator.
					*p++ = *q++;
					
				*p++ = 0;
			}
			else // No space at all, pass the empty string.
				*arg++ = "";
				
			continue;

		default:
			break;
		}
		--v;
		break;
	}

/* Handle primary code on a system by system basis */

	switch ((UCHAR) (*vector)[0]) {
	case isc_arg_warning:
	case isc_arg_gds:
		{
			USHORT fac = 0, dummy_class = 0;
			const ISC_STATUS decoded = gds__decode(code, &fac, &dummy_class);
			if (gds__msg_format(0, fac, (USHORT) decoded,
								bufsize, s, args[0], args[1], args[2], args[3],
								args[4]) < 0)
			{
				if ((decoded < FB_NELEM(messages) - 1) && (decoded >= 0))
					fb_utils::snprintf(s, bufsize, messages[decoded], args[0], args[1], args[2],
							args[3], args[4]);
				else
					fb_utils::snprintf(s, bufsize, "unknown ISC error %ld", code);	/* TXNN */
			}
		}
		break;

	case isc_arg_interpreted:
		//p = s;
		q = (const TEXT*) (*vector)[1];
		//while ((*p++ = *q++) /*!= NULL*/);
		strncpy(s, q, bufsize);
		s[bufsize - 1] = 0;
		break;

	case isc_arg_unix:
		/* The  strerror()  function  returns  the appropriate description
		   string, or an unknown error message if the error code is unknown. */
		q = (const TEXT*) strerror(code);
		strncpy(s, q, bufsize);
		s[bufsize - 1] = 0;
		break;

	case isc_arg_dos:
		fb_utils::snprintf(s, bufsize, "unknown dos error %ld", code);	/* TXNN */
		break;

#ifdef VMS
	case isc_arg_vms:
		{
			SSHORT ll = 0;
			struct dsc$descriptor_s desc;
			desc.dsc$b_class = DSC$K_CLASS_S;
			desc.dsc$b_dtype = DSC$K_DTYPE_T;
			desc.dsc$w_length = bufsize;
			desc.dsc$a_pointer = s;
			TEXT flags[4];
			ISC_STATUS status = sys$getmsg(code, &ll, &desc, 15, flags);
			if (status & 1)
				s[ll] = 0;
			else
				sprintf(s, "uninterpreted VMS code %x", code);	/* TXNN */
		}
		break;
#endif

#ifdef WIN_NT
	case isc_arg_win32:
		if (!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
										 NULL,
										 code,
										 GetUserDefaultLangID(),
										 s,
										 bufsize,
						                 NULL)
		  && !FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
										 NULL,
										 code,
										 0, // TMN: Fallback to system known language
										 s,
										 bufsize,
										 NULL))
		{
			fb_utils::snprintf(s, bufsize, "unknown Win32 error %ld", code);	/* TXNN */
		}
		break;
#endif

	default:
		if (temp)
			gds__free(temp);
		return 0;
	}


	if (temp)
		gds__free(temp);

	*vector = v;
	const TEXT* end = s;
	while (*end)
		end++;

	return static_cast<SLONG>(end - s);
}


/* CVC: This special function for ADA has been restored to non-const vector,
 too, in case its usage was broken. */
void API_ROUTINE gds__interprete_a(
								   SCHAR* s,
								   SSHORT* length,
								   ISC_STATUS* vector, SSHORT* offset)
{
/**************************************
 *
 *	g d s _ $ i n t e r p r e t e _ a
 *
 **************************************
 *
 * Functional description
 *	Translate a status code with arguments to a string.  Return the
 *	length of the string while updating the vector address.  If the
 *	message is null (end of messages) or invalid, return 0;
 *	Ada being an unsatisfactory excuse for a language, fall back on
 *	the concept of indexing into the vector.
 *
 **************************************/
	ISC_STATUS *v = vector + *offset;
	*length = (SSHORT) gds__interprete(s, &v);
	*offset = v - vector;
}


const int SECS_PER_HOUR	= 60 * 60;
const int SECS_PER_DAY	= SECS_PER_HOUR * 24;

#ifdef WIN_NT
Firebird::Mutex trace_mutex;
HANDLE trace_file_handle = INVALID_HANDLE_VALUE;
#endif

void API_ROUTINE gds__trace_raw(const char* text, unsigned int length)
{
/**************************************
 *
 *	g d s _ t r a c e _ r a w
 *
 **************************************
 *
 * Functional description
 *	Write trace event to a log file
 *  This function tries to be async-signal safe
 *
 **************************************/
	if (!length) 
		length = strlen(text);
#ifdef WIN_NT
	// Note: thread-safe code

	// Nickolay Samofatov, 12 Sept 2003. Windows open files extremely slowly. 
	// Slowly enough to make such trace useless. Thus we cache file handle !
	trace_mutex.enter();
	while (true) {
		if (trace_file_handle == INVALID_HANDLE_VALUE) {
			TEXT name[MAXPATHLEN];
			gds__prefix(name, LOGFILE); // B.O.
			// We do not care to close this file. 
			// It will be closed automatically when our process terminates.
			trace_file_handle = CreateFile(name, GENERIC_WRITE, 
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (trace_file_handle == INVALID_HANDLE_VALUE)
				break;
		}
		DWORD bytesWritten;
		SetFilePointer(trace_file_handle, 0, NULL, FILE_END);
		WriteFile(trace_file_handle, text, length, &bytesWritten, NULL);
		if (bytesWritten != length) {
			// Handle the case when file was deleted by another process on Win9x
			// On WinNT we are not going to notice that fact :(
			CloseHandle(trace_file_handle);
			trace_file_handle = INVALID_HANDLE_VALUE;
			continue;
		}
		break;
	}
	trace_mutex.leave();
#else
	TEXT name[MAXPATHLEN];

	// This function is not truly signal safe now.
	// It calls string::c_str() and may call getenv(), not good.
	// We can only hope that failure is unlikely in it...
	gds__prefix(name, LOGFILE); // B.O.

	// Note: signal-safe code
	int file = open(name, O_CREAT | O_APPEND | O_WRONLY, 0660);
	if (file == -1) return;
	write(file, text, length);
	close(file);
#endif
}

void API_ROUTINE gds__trace(const TEXT * text)
{
/**************************************
 *
 *	g d s _ t r a c e
 *
 **************************************
 *
 * Functional description
 *	Post trace event to a log file. Function records date and time
 *  This function tries to be async-signal safe
 *
 **************************************/
	
	const time_t now = time((time_t *)0); // is specified in POSIX to be signal-safe
	
	// 07 Sept 2003, Nickolay Samofatov.
	// Since we cannot call ctime/localtime_r or anything else like this from 
	// signal hanlders we need to decode time by hand. 

	const int days = now / SECS_PER_DAY;
	int rem = now % SECS_PER_DAY;

	tm today;
	TimeStamp::decode_date(days + TimeStamp::GDS_EPOCH_START, &today);
    today.tm_hour = rem / SECS_PER_HOUR;
    rem %= SECS_PER_HOUR;
    today.tm_min = rem / 60;
    today.tm_sec = rem % 60;

	char buffer[1024]; // 1K should be enough for the trace message
	char* p = buffer;
	gds__ulstr(p, today.tm_year + 1900, 4, '0'); p += 4;
	*p++ = '-';
	gds__ulstr(p, today.tm_mon, 2, '0'); p += 2;
	*p++ = '-';
	gds__ulstr(p, today.tm_mday, 2, '0'); p += 2;
	*p++ = 'T';
	gds__ulstr(p, today.tm_hour, 2, '0'); p += 2;
	*p++ = ':';
	gds__ulstr(p, today.tm_min, 2, '0'); p += 2;
	*p++ = ':';
	gds__ulstr(p, today.tm_sec, 2, '0'); p += 2;
	*p++ = ' ';
	gds__ulstr(p, 
#ifdef WIN_NT
#ifdef SUPERSERVER
			     GetCurrentThreadId(),
#else
				 GetCurrentProcessId(),				   
#endif
#else
				 getpid(),
#endif
				 5, ' '); p += 5;
	*p++ = ' ';
	strcpy(p, text); p += strlen(p);
	strcat(p, "\n"); p += strlen(p);
	gds__trace_raw(buffer, p - buffer);
}

void API_ROUTINE gds__log(const TEXT* text, ...)
{
/**************************************
 *
 *	g d s _ l o g
 *
 **************************************
 *
 * Functional description
 *	Post an event to a log file.
 *
 **************************************/
	va_list ptr;
	time_t now;
	TEXT name[MAXPATHLEN];

#ifdef HAVE_GETTIMEOFDAY
	struct timeval tv;
	GETTIMEOFDAY(&tv);
	now = tv.tv_sec;
#else
	now = time((time_t *)0);
#endif

	gds__prefix(name, LOGFILE); // B.O.

	const int oldmask = umask(0111);
#ifdef WIN_NT
	trace_mutex.enter();
#endif
	FILE* file = fopen(name, "a");
	if (file != NULL)
	{
		fprintf(file, "\n%s%s\t%.25s\t", 
				   ISC_get_host(name, MAXPATHLEN), gdslogid, ctime(&now));
		va_start(ptr, text);
		vfprintf(file, text, ptr);
		va_end(ptr);
		fprintf(file, "\n\n");
		fclose(file);
	}
#ifdef WIN_NT
	trace_mutex.leave();
#endif

	umask(oldmask);

}

void API_ROUTINE gds__print_pool(JrdMemoryPool* pool, const TEXT* text, ...)
{
/**************************************
 *
 *	g d s _ p r i n t _ p o o l
 *
 **************************************
 *
 * Functional description
 *	Print pool contents to the log file. 
 * Preced it with normal log record as in gds__log
 *
 **************************************/
	va_list ptr;
	time_t now;
	TEXT name[MAXPATHLEN];

#ifdef HAVE_GETTIMEOFDAY
	struct timeval tv;
	GETTIMEOFDAY(&tv);
	now = tv.tv_sec;
#else
	now = time((time_t *)0);
#endif

	gds__prefix(name, LOGFILE); // B.O.

	const int oldmask = umask(0111);
#ifdef WIN_NT
	trace_mutex.enter();
#endif
	FILE* file = fopen(name, "a");
	if (file != NULL)
	{
		fprintf(file, "\n%s%s\t%.25s\t", 
				   ISC_get_host(name, MAXPATHLEN), gdslogid, ctime(&now));
		va_start(ptr, text);
		vfprintf(file, text, ptr);
		va_end(ptr);
		fprintf(file, "\n");
		pool->print_contents(file, false);
		fprintf(file, "\n");
		fclose(file);
	}
#ifdef WIN_NT
	trace_mutex.leave();
#endif

	umask(oldmask);

}


void API_ROUTINE gds__log_status(const TEXT* database,
	const ISC_STATUS* status_vector)
{
/**************************************
 *
 *	g d s _ $ l o g _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Log error to error log.
 *
 **************************************/
	TEXT* const buffer = (TEXT*) gds__alloc((SLONG) BUFFER_XLARGE);
/* FREE: at procedure exit */
	if (!buffer)				/* NOMEM: */
		return;
		
#ifdef DEV_BUILD
	const size_t db_len = database ? strlen(database) : 0;
	fb_assert(db_len < BUFFER_XLARGE - 2);
	fb_assert(db_len < MAXPATHLEN);
#endif

	TEXT* p = buffer;
	const TEXT* const end = p + BUFFER_XLARGE;
	const int max_db_len = int(BUFFER_XLARGE - 12);
	sprintf(p, "Database: %.*s", max_db_len, (database) ? database : "");

	do {
		while (*p)
			p++;
			
		*p++ = '\n';
		*p++ = '\t';
	} while (p < end && safe_interpret(p, end - p, &status_vector));

	p[-2] = 0;
	gds__log(buffer, 0);
	gds__free(buffer);
}


int API_ROUTINE gds__msg_close(void *handle)
{
/**************************************
 *
 *	g d s _ $ m s g _ c l o s e
 *
 **************************************
 *
 * Functional description
 *	Close the message file and zero out static variable.
 *
 **************************************/

	gds_msg* messageL = static_cast<gds_msg*>(handle);

	if (!messageL) {
		if (!global_default_msg) {
			return 0;
		}
		messageL = global_default_msg;
	}

	global_default_msg = NULL;

	const int fd = messageL->msg_file;

	FREE_LIB_MEMORY(messageL);

	if (fd <= 0)
		return 0;

	return close(fd);
}


SSHORT API_ROUTINE gds__msg_format(void*       handle,
								   USHORT      facility,
								   USHORT      number,
								   USHORT      length,
								   TEXT*       buffer,
								   const TEXT* arg1,
								   const TEXT* arg2,
								   const TEXT* arg3,
								   const TEXT* arg4,
								   const TEXT* arg5)
{
/**************************************
 *
 *	g d s _ $ m s g _ f o r m a t
 *
 **************************************
 *
 * Functional description
 *	Lookup and format message.  Return as much of formatted string
 *	as fits in caller's buffer.
 *
 **************************************/
	SLONG size = (SLONG) (((arg1) ? MAX_ERRSTR_LEN : 0) +
						  ((arg2) ? MAX_ERRSTR_LEN : 0) +
						  ((arg3) ? MAX_ERRSTR_LEN : 0) +
						  ((arg4) ? MAX_ERRSTR_LEN : 0) +
						  ((arg5) ? MAX_ERRSTR_LEN : 0) + MAX_ERRMSG_LEN);

	size = (size < length) ? length : size;

	TEXT* formatted = (TEXT *) gds__alloc((SLONG) size);

	if (!formatted)				/* NOMEM: */
		return -1;

/* Let's assume that the text to be output will never be shorter
   than the raw text of the message to be formatted.  Then we can
   use the caller's buffer to temporarily hold the raw text. */

	const int n = gds__msg_lookup(handle, facility, number, length, buffer, NULL);

	if (n > 0 && n < length)
	{
		fb_utils::snprintf(formatted, size, buffer, arg1, arg2, arg3, arg4, arg5);
	}
	else
	{
		Firebird::string s;
		s.printf("can't format message %d:%d -- ", facility, number);
		if (n == -1)
			s += "message text not found";
		else if (n == -2) {
			s += "message file ";
			gds__prefix_msg(formatted, MSG_FILE); // B.O.
			s += formatted;
			s += " not found";
		}
		else {
			fb_utils::snprintf(formatted, size, "message system code %d", n);
			s += formatted;
		}
		s.copyTo(formatted, size);
	}

	const USHORT l = strlen(formatted);
	const TEXT* const end = buffer + length - 1;

	for (const TEXT* p = formatted; *p && buffer < end;) {
		*buffer++ = *p++;
	}
	*buffer = 0;

	gds__free(formatted);
	return ((n > 0) ? l : -l);
}


SSHORT API_ROUTINE gds__msg_lookup(void* handle,
								   USHORT facility,
								   USHORT number,
								   USHORT length,
								   TEXT* buffer, USHORT* flags)
{
/**************************************
 *
 *	g d s _ $ m s g _ l o o k u p
 *
 **************************************
 *
 * Functional description
 *	Lookup a message.  Return as much of the record as fits in the
 *	callers buffer.  Return the FULL size of the message, or negative
 *	number if we can't find the message.
 *
 **************************************/
// Handle default message file
	int status = -1;
	gds_msg* messageL = (gds_msg*) handle;
	if (!messageL && !(messageL = global_default_msg)) {
		/* Try environment variable setting first */

		TEXT* p = getenv("ISC_MSGS");
		if (p == NULL ||
			(status =
			 gds__msg_open(reinterpret_cast<void**>(&messageL), p)))
		{
			TEXT translated_msg_file[sizeof(MSG_FILE_LANG) + LOCALE_MAX + 1];

			/* Try declared language of this attachment */
			/* This is not quite the same as the language declared on the
			   READY statement */

			TEXT* msg_file = (TEXT *) gds__alloc((SLONG) MAXPATHLEN);
			/* FREE: at block exit */
			if (!msg_file)		/* NOMEM: */
				return -2;

			p = getenv("LC_MESSAGES");
			if (p != NULL) {
				sanitize(p); // CVC: Sanitizing environment variable???
				fb_utils::snprintf(translated_msg_file,
					sizeof(translated_msg_file), MSG_FILE_LANG, p);
				gds__prefix_msg(msg_file, translated_msg_file); // B.O.
				status =
					gds__msg_open(reinterpret_cast<void**>(&messageL),
								  msg_file);
			}
			else
				status = 1;
			if (status) {
				/* Default to standard message file */

				gds__prefix_msg(msg_file, MSG_FILE); // B.O.
				status =
					gds__msg_open(reinterpret_cast<void**>(&messageL),
								  msg_file);
			}
			gds__free(msg_file);
		}

		if (status)
			return status;

		global_default_msg = messageL;
	}

/* Search down index levels to the leaf.  If we get lost, punt */

	const ULONG code = MSG_NUMBER(facility, number);
	const msgnod* const end = 
		(msgnod*) ((char*) messageL->msg_bucket + messageL->msg_bucket_size);
	ULONG position = messageL->msg_top_tree;

	status = 0;
	for (USHORT n = 1; !status; n++) {
		if (lseek(messageL->msg_file, LSEEK_OFFSET_CAST position, 0) < 0)
			status = -6;
		else if (read(messageL->msg_file, messageL->msg_bucket,
					  messageL->msg_bucket_size) < 0)
			status = -7;
		else if (n == messageL->msg_levels)
			break;
		else {
			for (const msgnod* node = (msgnod*) messageL->msg_bucket; !status; node++) {
				if (node >= end) {
					status = -8;
					break;
				}
				else if (node->msgnod_code >= code) {
					position = node->msgnod_seek;
					break;
				}
			}
		}
	}

	if (!status) {
		/* Search the leaf */
		for (const msgrec* leaf = (msgrec*) messageL->msg_bucket; !status;
			 leaf = NEXT_LEAF(leaf)) 
		{
			if (leaf >= (const msgrec*) end || leaf->msgrec_code > code) {
				status = -1;
				break;
			}
			else if (leaf->msgrec_code == code) {
				/* We found the correct message, so return it to the user */
				const USHORT n = MIN(length - 1, leaf->msgrec_length);
				memcpy(buffer, leaf->msgrec_text, n);
				buffer[n] = 0;

				if (flags)
					*flags = leaf->msgrec_flags;

				status = leaf->msgrec_length;
				break;
			}
		}
	}

	return status;
}


int API_ROUTINE gds__msg_open(void** handle, const TEXT* filename)
{
/**************************************
 *
 *	g d s _ $ m s g _ o p e n
 *
 **************************************
 *
 * Functional description
 *	Try to open a message file.  If successful, return a status of 0
 *	and update a message handle.
 *
 **************************************/
	const int n = open(filename, O_RDONLY | O_BINARY, 0);
	if (n < 0)
		return -2;

	isc_msghdr header;
	if (read(n, &header, sizeof(header)) < 0) {
		close(n);
		return -3;
	}
	// Trick to silence compiler when MSG_MINOR_VERSION == 0
	const USHORT minor = MSG_MINOR_VERSION;

	if (header.msghdr_major_version != MSG_MAJOR_VERSION ||
		header.msghdr_minor_version < minor)
	{
		close(n);
		return -4;
	}

	gds_msg* messageL =
		(gds_msg*) ALLOC_LIB_MEMORY((SLONG) sizeof(gds_msg) +
							   header.msghdr_bucket_size - 1);
/* FREE: in gds__msg_close */
	if (!messageL) {				/* NOMEM: return non-open error */
		close(n);
		return -5;
	}

#ifdef DEBUG_GDS_ALLOC
/* This will only be freed if the client closes the message file for us */
	gds_alloc_flag_unfreed((void *) messageL);
#endif

	messageL->msg_file = n;
	messageL->msg_bucket_size = header.msghdr_bucket_size;
	messageL->msg_levels = header.msghdr_levels;
	messageL->msg_top_tree = header.msghdr_top_tree;


	*handle = messageL;

	return 0;
}


void API_ROUTINE gds__msg_put(
							  void* handle,
							  USHORT facility,
							  USHORT number,
							  const TEXT* arg1, const TEXT* arg2,
							  const TEXT* arg3, const TEXT* arg4,
							  const TEXT* arg5)
{
/**************************************
 *
 *	g d s _ $ m s g _ p u t
 *
 **************************************
 *
 * Functional description
 *	Lookup and format message.  Return as much of formatted string
 *	as fits in callers buffer.
 *
 **************************************/
	TEXT formatted[BUFFER_MEDIUM];

	gds__msg_format(handle, facility, number, sizeof(formatted),
					formatted, arg1, arg2, arg3, arg4, arg5);
	gds__put_error(formatted);
}


SLONG API_ROUTINE gds__get_prefix(SSHORT arg_type, const TEXT* passed_string)
{
/**************************************
 *
 *	g d s _ $ g e t _ p r e f i x
 *
 **************************************
 *
 * Functional description
 *	Find appropriate InterBase command line arguments 
 *	for Interbase file prefix.
 *
 *      arg_type is 0 for $INTERBASE, 1 for $INTERBASE_LOCK 
 *      and 2 for $INTERBASE_MSG
 *
 *      Function returns 0 on success and -1 on failure
 **************************************/
	int count = 0;

	if (arg_type < IB_PREFIX_TYPE || arg_type > IB_PREFIX_MSG_TYPE)
		return ((SLONG) - 1);

	char* prefix_ptr;
	switch (arg_type) {
	case IB_PREFIX_TYPE:
		prefix_ptr = ib_prefix = ib_prefix_val;
		break;
	case IB_PREFIX_LOCK_TYPE:
		prefix_ptr = ib_prefix_lock = ib_prefix_lock_val;
		break;
	case IB_PREFIX_MSG_TYPE:
		prefix_ptr = ib_prefix_msg = ib_prefix_msg_val;
		break;
	default:
		return ((SLONG) - 1);
	}
/* the command line argument was 'H' for interbase home */
	while (*prefix_ptr++ = *passed_string++) {
		/* if the next character is space, newline or carriage return OR
		   number of characters exceeded */
		if (*passed_string == ' ' || *passed_string == 10
			|| *passed_string == 13 || (count == MAXPATHLEN))
		{
			*(prefix_ptr++) = '\0';
			break;
		}
		count++;
	}
	if (!count) {
		prefix_ptr = NULL;
		return ((SLONG) - 1);
	}
	return ((SLONG) 0);
}


#ifndef VMS
void API_ROUTINE gds__prefix(TEXT* resultString, const TEXT* file)
{
/**************************************
 *
 *	g d s _ $ p r e f i x	( n o n - V M S )
 *
 **************************************
 *
 * Functional description
 *	Find appropriate file prefix.
 *
 **************************************/
	resultString[0] = 0;

	if (ib_prefix == NULL) {
		// Try and get value from config file
		const char *regPrefix = Config::getRootDirectory();

		const size_t len = strlen(regPrefix);
		if (len > 0) {
			if (len > sizeof(ib_prefix_val)) {
				perror("ib_prefix path size too large - truncated");                      
			}
			strncpy(ib_prefix_val, regPrefix, sizeof(ib_prefix_val) - 1);
			ib_prefix_val[sizeof(ib_prefix_val) - 1] = 0;
			ib_prefix = ib_prefix_val;
		}
		else {
			ib_prefix = FB_PREFIX;
			strcat(ib_prefix_val, ib_prefix);
		}
		ib_prefix = ib_prefix_val;
	}
	strcpy(resultString, ib_prefix);
	safe_concat_path(resultString, file);
}
#endif /* !defined(VMS) */


#ifdef VMS
void API_ROUTINE gds__prefix(TEXT* string, const TEXT* root)
{
/**************************************
 *
 *	g d s _ $ p r e f i x	( V M S )
 *
 **************************************
 *
 * Functional description
 *	Find appropriate InterBase file prefix.
 *	Override conditional defines with
 *	the enviroment variable INTERBASE if it is set.
 *
 **************************************/
	if (*root != '[') {
		strcpy(string, root);
		return;
	}

/* Check for the existence of an InterBase logical name.  If there is 
   one use it, otherwise use the system directories. */
	TEXT temp[256];
	if (ISC_expand_logical_once(ISC_LOGICAL, sizeof(ISC_LOGICAL) - 2, temp, sizeof(temp)))
	{
		strcpy(string, ISC_LOGICAL);
		strcat(string, root);
		return;
	}

	TEXT* p = temp;
	for (const TEXT* q = root; *p = UPPER7(*q); q++)
		if (*p++ == ']')
			break;

	const SSHORT len = p - temp;
	for (const isc_vms_prefix* prefix = trans_prefix; prefix->isc_prefix; prefix++)
		if (!strncmp(temp, prefix->isc_prefix, len)) {
			strcpy(string, prefix->vms_prefix);
			strcat(string, &root[len]);
			return;
		}

	strcpy(string, &root[len]);
}
#endif


#ifndef VMS
void API_ROUTINE gds__prefix_lock(TEXT* string, const TEXT* root)
{
/********************************************************
 *
 *	g d s _ $ p r e f i x _ l o c k	( n o n - V M S )
 *
 ********************************************************
 *
 * Functional description
 *	Find appropriate Firebird lock file prefix.
 *	Override conditional defines with the enviroment
 *      variable FIREBIRD_LOCK if it is set.
 *
 **************************************/
	string[0] = 0;

	if (ib_prefix_lock == NULL) {
		if (!(ib_prefix_lock = getenv(FB_LOCK_ENV))) {
#ifdef EMBEDDED
			ib_prefix_lock = ib_prefix_lock_val;
			gds__temp_dir(ib_prefix_lock); // B.O.
			// Generate filename based on the current PID
			TEXT tmp_buf[MAXPATHLEN];
			sprintf(tmp_buf, FB_PID_FILE, getpid());
			sprintf(tmp_buf, root, tmp_buf);
			root = tmp_buf;
#else
			ib_prefix_lock = ib_prefix_lock_val;
			gds__prefix(ib_prefix_lock, ""); // B.O.
#endif
		}
		else {
			strcat(ib_prefix_lock_val, ib_prefix_lock);
			ib_prefix_lock = ib_prefix_lock_val;
		}
	}
	strcat(string, ib_prefix_lock);
	safe_concat_path(string, root);
}
#endif


#ifdef VMS
void API_ROUTINE gds__prefix_lock(TEXT* string, const TEXT* root)
{
/************************************************
 *
 *	g d s _ $ p r e f i x _ l o c k	( V M S )
 *
 ************************************************
 *
 * Functional description
 *	Find appropriate Firebird lock file prefix.
 *	Override conditional defines with the enviroment
 *      variable FIREBIRD_LOCK if it is set.
 *
 *************************************************/
	if (*root != '[') {
		strcpy(string, root);
		return;
	}

/* Check for the existence of a Firebird logical name.  If there is 
   one use it, otherwise use the system directories. */
	TEXT temp[256];
	if (ISC_expand_logical_once
		(ISC_LOGICAL_LOCK, sizeof(ISC_LOGICAL_LOCK) - 2, temp, sizeof(temp)))
	{
		strcpy(string, ISC_LOGICAL_LOCK);
		strcat(string, root);
		return;
	}

	TEXT* p = temp;
	for (const TEXT* q = root; *p = UPPER7(*q); q++)
		if (*p++ == ']')
			break;

	const SSHORT len = p - temp;
	for (const isc_vms_prefix* prefix = trans_prefix; prefix->isc_prefix; prefix++)
		if (!strncmp(temp, prefix->isc_prefix, len)) {
			strcpy(string, prefix->vms_prefix);
			strcat(string, &root[len]);
			return;
		}

	strcpy(string, &root[len]);
}
#endif

#ifndef VMS
void API_ROUTINE gds__prefix_msg(TEXT* string, const TEXT* root)
{
/********************************************************
 *
 *      g d s _ $ p r e f i x _ m s g ( n o n - V M S )
 *
 ********************************************************
 *
 * Functional description
 *      Find appropriate Firebird message file prefix.
 *      Override conditional defines with the enviroment
 *      variable FIREBIRD_MSG if it is set.
 *
 *
 **************************************/
	string[0] = 0;

	if (ib_prefix_msg == NULL) {
		if (!(ib_prefix_msg = getenv(FB_MSG_ENV))) {
			ib_prefix_msg = ib_prefix_msg_val;
			gds__prefix(ib_prefix_msg, ""); // B.O.
		}
		else {
			strcat(ib_prefix_msg_val, ib_prefix_msg);
			ib_prefix_msg = ib_prefix_msg_val;
		}
	}
	strcat(string, ib_prefix_msg);
	safe_concat_path(string, root);
}
#endif

#ifdef VMS
void API_ROUTINE gds__prefix_msg(TEXT* string, const TEXT* root)
{
/************************************************
 *
 *      g d s _ $ p r e f i x _ m s g ( V M S )
 *
 ************************************************
 *
 * Functional description
 *      Find appropriate Firebird message file prefix.
 *      Override conditional defines with the enviroment
 *      variable FIREBIRD_MSG if it is set.
 *
 *************************************************/
	if (*root != '[') {
		strcpy(string, root);
		return;
	}


/* Check for the existence of an InterBase logical name.  If there is
   one use it, otherwise use the system directories. */

/* ISC_LOGICAL_MSG macro needs to be defined, check non VMS version of routine
   for functionality. */
	TEXT temp[256];
	if (ISC_expand_logical_once
		(ISC_LOGICAL_MSG, sizeof(ISC_LOGICAL_MSG) - 2, temp, sizeof(temp)))
	{
		strcpy(string, ISC_LOGICAL_MSG);
		strcat(string, root);
		return;
	}

	TEXT* p = temp;
	for (const TEXT* q = root; *p = UPPER7(*q); q++)
		if (*p++ == ']')
			break;

	const SSHORT len = p - temp;
	for (const isc_vms_prefix* prefix = trans_prefix; prefix->isc_prefix; prefix++)
		if (!strncmp(temp, prefix->isc_prefix, len)) {
			strcpy(string, prefix->vms_prefix);
			strcat(string, &root[len]);
			return;
		}

	strcpy(string, &root[len]);
}
#endif


ISC_STATUS API_ROUTINE gds__print_status(const ISC_STATUS* vec)
{
/**************************************
 *
 *	g d s _ p r i n t _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Interprete a status vector.
 *
 **************************************/
	if (!vec || (!vec[1] && vec[2] == isc_arg_end))
		return FB_SUCCESS;

	TEXT* s = (TEXT *) gds__alloc((SLONG) BUFFER_LARGE);
/* FREE: at procedure return */
	if (!s)						/* NOMEM: */
		return vec[1];

	const ISC_STATUS* vector = vec;

	if (!safe_interpret(s, BUFFER_LARGE, &vector)) {
		gds__free(s);
		return vec[1];
	}

	gds__put_error(s);
	s[0] = '-';

	while (safe_interpret(s + 1, BUFFER_LARGE - 1, &vector))
		gds__put_error(s);

	gds__free(s);

	return vec[1];
}


USHORT API_ROUTINE gds__parse_bpb(USHORT bpb_length,
								  const UCHAR* bpb,
								  USHORT* source,
								  USHORT* target)
{
/**************************************
 *
 *	g d s _ p a r s e _ b p b
 *
 **************************************
 *
 * Functional description
 *	Get blob type, source sub_type and target sub-type
 *	from a blob parameter block.
 *
 **************************************/

  /* SIGN ERROR */

	return gds__parse_bpb2(bpb_length, bpb, (SSHORT*)source, (SSHORT*)target, NULL, NULL);
}


USHORT API_ROUTINE gds__parse_bpb2(USHORT bpb_length,
								   const UCHAR* bpb,
								   SSHORT* source,
								   SSHORT* target,
								   USHORT* source_interp,
								   USHORT* target_interp)
{
/**************************************
 *
 *	g d s _ p a r s e _ b p b 2
 *
 **************************************
 *
 * Functional description
 *	Get blob type, source sub_type and target sub-type
 *	from a blob parameter block.
 *	Basically gds__parse_bpb() with additional:
 *	source_interp and target_interp.
 *
 **************************************/
	USHORT type = 0;
	*source = *target = 0;

	if (source_interp)
		*source_interp = 0;
	if (target_interp)
		*target_interp = 0;

	if (!bpb_length || !bpb)
		return type;

	const UCHAR* p = bpb;
	const UCHAR* const end = p + bpb_length;

	if (*p++ != isc_bpb_version1)
		return type;

	while (p < end) {
		const UCHAR op = *p++;
		const USHORT length = *p++;
		switch (op) {
		case isc_bpb_source_type:
			*source = (USHORT) gds__vax_integer(p, length);
			break;

		case isc_bpb_target_type:
			*target = (USHORT) gds__vax_integer(p, length);
			break;

		case isc_bpb_type:
			type = (USHORT) gds__vax_integer(p, length);
			break;

		case isc_bpb_source_interp:
			if (source_interp)
				*source_interp = (USHORT) gds__vax_integer(p, length);
			break;

		case isc_bpb_target_interp:
			if (target_interp)
				*target_interp = (USHORT) gds__vax_integer(p, length);
			break;

		default:
			break;
		}
		p += length;
	}

	return type;
}


SLONG API_ROUTINE gds__ftof(const SCHAR* string,
							const USHORT length1,
							SCHAR* fieldL,
							const USHORT length2)
{
/**************************************
 *
 *	g d s _ f t o f
 *
 **************************************
 *
 * Functional description
 *	Move a fixed length string to a fixed length string.
 *	This is typically generated by the preprocessor to
 *	move strings around.
 *
 **************************************/
	USHORT fill = 0;
	// CVC: a logic bug rendered the fill > 0 test useless
	if (length2 > length1)
		fill = length2 - length1;

	USHORT l = MIN(length1, length2);
	if (l > 0)
		do {
			*fieldL++ = *string++;
		} while (--l);

	if (fill > 0)
		do {
			*fieldL++ = ' ';
		} while (--fill);

	return 0;
}


int API_ROUTINE gds__print_blr(
							   const UCHAR* blr,
							   FPTR_PRINT_CALLBACK routine,
							   void* user_arg, SSHORT language)
{
/**************************************
 *
 *	g d s _ $ p r i n t _ b l r
 *
 **************************************
 *
 * Functional description
 *	Pretty print blr thru callback routine.
 *
 **************************************/
	try {

	gds_ctl ctl;
	gds_ctl* control = &ctl;

	if (!routine) {
		routine = gds__default_printer;
		user_arg = NULL;
	}

	control->ctl_routine = routine;
	control->ctl_user_arg = user_arg;
	control->ctl_blr = control->ctl_blr_start = blr;
	control->ctl_ptr = control->ctl_buffer;
	control->ctl_language = language;

	const SSHORT version = BLR_BYTE;

	if ((version != blr_version4) && (version != blr_version5))
		blr_error(control, "*** blr version %d is not supported ***",
				  (int) version);

	blr_format(control,
			   (version == blr_version4) ? "blr_version4," : "blr_version5,");

	SSHORT level = 0;
	SLONG offset = 0;
	blr_print_line(control, (SSHORT) offset);
	blr_print_verb(control, level);

	offset = control->ctl_blr - control->ctl_blr_start;
	const SCHAR eoc = BLR_BYTE;

	if (eoc != blr_eoc)
		blr_error(control, "*** expected end of command, encounted %d ***",
				  (int) eoc);

	blr_format(control, "blr_eoc");
	blr_print_line(control, (SSHORT) offset);

	}	// try
	catch (const std::exception&) {
		return -1;
	}

	return 0;
}


void API_ROUTINE gds__put_error(const TEXT* string)
{
/**************************************
 *
 *	g d s _ p u t _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Put out a line of error text.
 *
 **************************************/

#ifdef VMS
	struct dsc$descriptor_s desc;

	ISC_make_desc(string, &desc, 0);
	lib$put_output(&desc);
#else
	fputs(string, stderr);
	fputc('\n', stderr);
	fflush(stderr);
#endif
}


void API_ROUTINE gds__qtoq(const void* quad_in, void* quad_out)
{
/**************************************
 *
 *	g d s _ q t o q
 *
 **************************************
 *
 * Functional description
 *	Move a quad value to another quad value.  This
 *      call is generated by the preprocessor when assigning
 *      quad values in FORTRAN.
 *
 **************************************/

	*((ISC_QUAD*) quad_out) = *((ISC_QUAD*) quad_in);
}


void API_ROUTINE gds__register_cleanup(FPTR_VOID_PTR routine, void* arg)
{
/**************************************
 *
 *	g d s _ r e g i s t e r _ c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Register a cleanup handler.
 *
 **************************************/

/* 
 * Ifdef out for windows client.  We have not implemented any way of 
 * determining when a task ends, therefore this never gets called.
*/

	if (!initialized)
		init();

	CLEAN clean = (CLEAN) ALLOC_LIB_MEMORY((SLONG) sizeof(struct clean));
	clean->clean_next = cleanup_handlers;
	cleanup_handlers = clean;
	clean->clean_routine = routine;
	clean->clean_arg = arg;

#ifdef DEBUG_GDS_ALLOC
/* Startup function - known to be unfreed */
	gds_alloc_flag_unfreed((void *) clean);
#endif
}


SLONG API_ROUTINE gds__sqlcode(const ISC_STATUS* status_vector)
{
/**************************************
 *
 *	g d s _ s q l c o d e
 *
 **************************************
 *
 * Functional description
 *	Translate GDS error code to SQL error code.  This is a little
 *	imprecise (to say the least) because we don't know the proper
 *	SQL codes.  One must do what what can; stiff upper lip, and all
 *	that.
 *
 *	Scan a status vector - if we find gds__sqlerr in the "right"
 *	positions, return the code that follows.  Otherwise, for the
 *	first code for which there is a non-generic SQLCODE, return it.
 *
 **************************************/
	if (!status_vector) {
		DEV_REPORT("gds__sqlcode: NULL status vector");
		return GENERIC_SQLCODE;
	}

	bool have_sqlcode = false;
	SLONG sqlcode = GENERIC_SQLCODE;	/* error of last resort */

/* SQL code -999 (GENERIC_SQLCODE) is generic, meaning "no other sql code
 * known".  Now scan the status vector, seeing if there is ANY sqlcode
 * reported.  Make note of the first error in the status vector who's 
 * SQLCODE is NOT -999, that will be the return code if there is no specific
 * sqlerr reported.
 */

	const ISC_STATUS* s = status_vector;
	while (*s != isc_arg_end)
	{
		if (*s == isc_arg_gds)
		{
			s++;
			if (*s == isc_sqlerr)
			{
				return *(s + 2);
			}

			if (!have_sqlcode) {
				/* Now check the hard-coded mapping table of gds_codes to
				   sql_codes */
				USHORT fac = 0, dummy_class = 0;

				const USHORT code = (USHORT) gds__decode(status_vector[1], &fac, &dummy_class);

				if ((code < FB_NELEM(gds__sql_code)) &&
					(gds__sql_code[code] != GENERIC_SQLCODE))
				{
					sqlcode = gds__sql_code[code];
					have_sqlcode = true;
				}
			}
			s++;
		}
		else if (*s == isc_arg_cstring)
			s += 3;				/* skip: isc_arg_cstring <len> <ptr> */
		else
			s += 2;				/* skip: isc_arg_* <item> */
	}

	return sqlcode;
}


void API_ROUTINE gds__sqlcode_s(const ISC_STATUS* status_vector, ULONG* sqlcode)
{
/**************************************
 *
 *	g d s _ s q l c o d e _ s
 *
 **************************************
 *
 * Functional description
 *	Translate GDS error code to SQL error code.  This is a little
 *	imprecise (to say the least) because we don't know the proper
 *	SQL codes.  One must do what what can; stiff upper lip, and all
 *	that.  THIS IS THE COBOL VERSION.  (Some cobols don't have
 *	return values for calls...
 *
 **************************************/

	*sqlcode = gds__sqlcode(status_vector);
}


void API_ROUTINE gds__temp_dir(TEXT* buffer)
{
/**************************************
 *
 *      g d s _ _ t e m p _ d i r
 *
 **************************************
 *
 * Functional description
 *      Return temporary directory.
 *
 **************************************/
	const TEXT* directory = getenv(FB_TMP_ENV);
	if (!directory) {
#ifdef WIN_NT
		TEXT temp_dir[MAXPATHLEN];
		const DWORD len = GetTempPath(sizeof(temp_dir), temp_dir);
		// This checks "TEMP" and "TMP" environment variables
		if (len && len < sizeof(temp_dir))
			directory = temp_dir;
#else
		directory = getenv("TMP");
#endif
	}
	if (!directory || strlen(directory) >= MAXPATHLEN)
		directory = WORKFILE;
	strcpy(buffer, directory);
}

	
void* API_ROUTINE gds__temp_file(
					 BOOLEAN stdio_flag, const TEXT* string,
					 TEXT* expanded_string, TEXT* dir, BOOLEAN unlink_flag)
{
/**************************************
 *
 *      g d s _ _ t e m p _ f i l e
 *
 **************************************
 *
 * Functional description
 *      Create and open a temp file with a given location.
 *      Unless the address of a buffer for the expanded file name string is
 *      given, make up the file "pre-deleted". Return -1 on failure.
 *      If unlink_flag is TRUE than file is marked as pre-deleted even if 
 *      expanded_string is not NULL.
 * NOTE 
 *      Function returns untyped handle that needs to be casted to either FILE
 *      or used as file descriptor. This is ugly and needs to be fixed probably 
 *      via introducing two functions with different return types.
 *
 **************************************/
	TEXT temp_dir[MAXPATHLEN];

	const TEXT* directory = dir;
	if (!directory) {
		gds__temp_dir(temp_dir); // B.O.
		directory = temp_dir;
	}
	if (strlen(directory) >= MAXPATHLEN - strlen(string) - strlen(TEMP_PATTERN) - 2)
		return (void *)-1;

	void* result;

#ifdef WIN_NT
	/* These are the characters used in temporary filenames.  */
	static const char letters[] = "abcdefghijklmnopqrstuvwxyz0123456789";

	_timeb t;
	_ftime(&t);
	__int64 randomness = t.time;
	randomness *= 1000;
	randomness += t.millitm;
	for (int tryCount = 0; tryCount < MAX_TMPFILE_TRIES; tryCount++) {
		char file_name[MAXPATHLEN];
		strcpy(file_name, directory);
		if (file_name[strlen(file_name) - 1] != '\\')
			strcat(file_name, "\\");
		strcat(file_name, string);
		char suffix[] = TEMP_PATTERN;
		__int64 temp = randomness;
		for (size_t i = 0; i < sizeof(suffix) - 1; i++) {
			suffix[i] = letters[temp % (sizeof(letters) - 1)];
			temp /= sizeof(letters) - 1;
		}
		strcat(file_name, suffix);
		if (expanded_string)
			strcpy(expanded_string, file_name);
		result = (void*)_sopen(file_name, _O_CREAT | _O_TRUNC | _O_RDWR | 
			_O_BINARY | _O_SHORT_LIVED | _O_NOINHERIT | _O_EXCL |
			(expanded_string && !unlink_flag ? 0 : _O_TEMPORARY),
			_SH_DENYRW, _S_IREAD|_S_IWRITE);
		if ((int)result != -1 || (errno != EACCES && errno != EEXIST))
			break;
		randomness++;
	}
	if ((int)result == -1) return result;
	if (stdio_flag) {
		if (!(result = fdopen((int) result, "w+b")))
			return (void *)-1;
	}
#else
	TEXT file_name[MAXPATHLEN];
	strcpy(file_name, directory);
	if (file_name[strlen(file_name) - 1] != '/')
		strcat(file_name, "/");
	strcat(file_name, string);
	strcat(file_name, TEMP_PATTERN);
	
#ifdef HAVE_MKSTEMP
	result = (void *)(IPTR)mkstemp(file_name);
#else
	if (mktemp(file_name) == (char *)0)
		return (void *)-1;

	do {
		result = (void *)open(file_name, O_RDWR | O_EXCL| O_CREAT);
	} while (result == (void *)-1 && errno == EINTR);
#endif
	if (result == (void *)-1)
		return result;

	if (stdio_flag)
		if (!(result = fdopen((int)(IPTR)result, "w+")))
			return (void *)-1;

	if (expanded_string)
		strcpy(expanded_string, file_name);

	if (!expanded_string || unlink_flag)
		unlink(file_name);
#endif

	return result;
}


void API_ROUTINE gds__unregister_cleanup(FPTR_VOID_PTR routine, void *arg)
{
/**************************************
 *
 *	g d s _ u n r e g i s t e r _ c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Unregister a cleanup handler.
 *
 **************************************/
	CLEAN clean;

	for (CLEAN* clean_ptr = &cleanup_handlers; clean = *clean_ptr;
		 clean_ptr = &clean->clean_next) 
	{
        if (clean->clean_routine == routine
            && clean->clean_arg == arg) 
		{
			*clean_ptr = clean->clean_next;
			FREE_LIB_MEMORY(clean);
			break;
		}
    }
}


#ifndef VMS
BOOLEAN API_ROUTINE gds__validate_lib_path(const TEXT* module,
										   const TEXT* ib_env_var,
										   TEXT* resolved_module,
										   SLONG length)
{
/**************************************
 *
 *	g d s _ $ v a l i d a t e _ l i b _ p a t h	( n o n - V M S )
 *
 **************************************
 *
 * Functional description
 *	Find the external library path variable.
 *	Validate that the path to the library module name 
 *	in the path specified.  If the external lib path
 *	is not defined then accept any path, and return 
 *	TRUE. If the module is in the path then return TRUE 
 * 	else, if the module is not in the path return FALSE.
 *
 **************************************/
	TEXT* ib_ext_lib_path = getenv(ib_env_var);
	if (!ib_ext_lib_path) {
		strncpy(resolved_module, module, length);
		return TRUE;		/* The variable is not defined. Return TRUE */
	}

	TEXT abs_module[MAXPATHLEN];
	if (EXPAND_PATH(module, abs_module)) {
		/* Extract the path from the absolute module name */
		const TEXT* q = NULL;
		for (const TEXT* mp = abs_module; *mp; mp++)
			if ((*mp == '\\') || (*mp == '/'))
				q = mp;

		TEXT abs_module_path[MAXPATHLEN];
		memset(abs_module_path, 0, MAXPATHLEN);
		strncpy(abs_module_path, abs_module, q - abs_module);

		/* Check to see if the module path is in the lib path
		   if it is return TRUE.  If it does not find it, then
		   the module path is not valid so return FALSE */

		TEXT abs_path[MAXPATHLEN];
		TEXT path[MAXPATHLEN];

		// CVC: Warning, strtok is changing the environment variable!
		// If this function is called again, only the first substring will be found.
		// Multiple engines in the same machine => shudder.
		const TEXT* token = strtok(ib_ext_lib_path, ";");
		while (token != NULL) {
			strcpy(path, token);
			/* make sure that there is no traing slash on the path */
			TEXT* p = path + strlen(path);
			if ((p != path) && ((p[-1] == '/') || (p[-1] == '\\')))
				p[-1] = 0;
			if ((EXPAND_PATH(path, abs_path))
				&& (!COMPARE_PATH(abs_path, abs_module_path))) 
			{
				strncpy(resolved_module, abs_module, length);
				return TRUE;
			}
			token = strtok(NULL, ";");
		}
	}
	return FALSE;
}
#endif


#ifdef VMS
BOOLEAN API_ROUTINE gds__validate_lib_path(const TEXT* module,
										   const TEXT* ib_env_var,
										   TEXT* resolved_module,
										   SLONG length)
{
/**************************************
 *
 *	g d s _ $ v a l i d a t e _ l i b _ p a t h	( V M S )
 *
 **************************************
 *
 * Functional description
 *	Find the InterBase external library path variable.
 *	Validate that the path to the library module name 
 *	in the path specified.  If the external lib path
 *	is not defined then accept any path, and return true.
 * 	If the module is not in the path return FALSE.
 *
 **************************************/
	TEXT *p, *q;
	TEXT path[MAXPATHLEN];
	TEXT abs_module_path[MAXPATHLEN];
	TEXT abs_module[MAXPATHLEN];

/* Check for the existence of an InterBase logical name.  If there is 
   one use it, otherwise use the system directories. */

	COMPILER ERROR ! BEFORE DOING A VMS POST PLEASE
		MAKE SURE THAT THIS FUNCTION WORKS THE SAME WAY
		AS THE NON -
		VMS ONE
		DOES.
	if (!
				 (ISC_expand_logical_once
				  (ib_env_var, strlen(ib_env_var) - 2, path, sizeof(path))))
		return TRUE;

	if (ISC_expand_logical_once(module, strlen(module) - 2, abs_module, sizeof(abs_module)))
	{
		/* Extract the path from the module name */
		for (p = abs_module, q = NULL; *p; p++)
			if (*p == ']')
				q = p;

		memset(abs_module_path, 0, MAXPATHLEN);
		strncpy(abs_module_path, abs_module, q - abs_module + 1);

		/* Check to see if the module path is the same as lib path
		   if it is return TURE.  If not then the module path is 
		   not valid so return FALSE */
		if (!strcmp(path, abs_module_path))
			return TRUE;
	}
	return FALSE;
}
#endif


SLONG API_ROUTINE gds__vax_integer(const UCHAR* ptr, SSHORT length)
{
/**************************************
 *
 *	g d s _ $ v a x _ i n t e g e r
 *
 **************************************
 *
 * Functional description
 *	Pick up (and convert) a VAX style integer of length 1, 2, or 4
 *	bytes.
 *
 **************************************/
	SLONG value = 0;
	SSHORT shift = 0;

	while (--length >= 0) {
		value += ((SLONG) * ptr++) << shift;
		shift += 8;
	}

	return value;
}


void API_ROUTINE gds__vtof(
						   const SCHAR* string,
						   SCHAR* fieldL, USHORT length)
{
/**************************************
 *
 *	g d s _ v t o f
 *
 **************************************
 *
 * Functional description
 *	Move a null terminated string to a fixed length
 *	field.  The call is primarily generated  by the
 *	preprocessor.
 *
 **************************************/

	while (*string) {
		*fieldL++ = *string++;
		if (--length <= 0)
			return;
	}

	if (length > 0)
		do {
			*fieldL++ = ' ';
		} while (--length);
}


void API_ROUTINE gds__vtov(const SCHAR* string, char* fieldL, SSHORT length)
{
/**************************************
 *
 *	g d s _ v t o v
 *
 **************************************
 *
 * Functional description
 *	Move a null terminated string to a fixed length
 *	field.  The call is primarily generated  by the
 *	preprocessor.  Unless gds__vtof, the target string
 *	is null terminated.
 *
 **************************************/

	--length;

	while ((*fieldL++ = *string++) != 0)
		if (--length <= 0) {
			*fieldL = 0;
			return;
		}
}


void API_ROUTINE isc_print_sqlerror(SSHORT sqlcode, const ISC_STATUS* status)
{
/**************************************
 *
 *	i s c _ p r i n t _ s q l e r r o r
 *
 **************************************
 *
 * Functional description
 *	Given an sqlcode, give as much info as possible.
 *      Decide whether status is worth mentioning.
 *
 **************************************/
	TEXT error_buffer[192];

	sprintf(error_buffer, "SQLCODE: %d\nSQL ERROR:\n", sqlcode);
	TEXT* p = error_buffer;
	while (*p)
		p++;
		
	isc_sql_interprete(sqlcode, p,
					   (SSHORT) (sizeof(error_buffer) - (p - error_buffer) -
								 2));
	while (*p)
		p++;
		
	*p++ = '\n';
	*p = 0;
	gds__put_error(error_buffer);

	if (status && status[1]) {
		gds__put_error("ISC STATUS: ");
		gds__print_status(status);
	}
}


void API_ROUTINE isc_sql_interprete(
									SSHORT sqlcode,
									TEXT* buffer, SSHORT length)
{
/**************************************
 *
 *	i s c _ s q l _ i n t e r p r e t e
 *
 **************************************
 *
 * Functional description
 *	Given an sqlcode, return a buffer full of message
 *	This is very simple because all we have is the
 *      message number, and the sqlmessages have as yet no
 *	arguments
 *
 *	NOTE: As of 21-APR-1999, sqlmessages HAVE arguments hence use
 *	      an empty string instead of NULLs.
 *	      
 **************************************/
	const TEXT* str = "";

	if (sqlcode < 0)
		gds__msg_format(0, 13, (USHORT) (1000 + sqlcode), length, buffer,
						str, str, str, str, str);
	else
		gds__msg_format(0, 14, sqlcode, length, buffer,
						str, str, str, str, str);
}


#ifdef VMS
int unlink(SCHAR* file)
{
/**************************************
 *
 *	u n l i n k
 *
 **************************************
 *
 * Functional description
 *	Get rid of a file and all of its versions.
 *
 **************************************/
	int status;
	struct dsc$descriptor_s desc;

	ISC_make_desc(file, &desc, 0);

	for (;;) {
		status = lib$delete_file(&desc);
		if (!(status & 1))
			break;
	}

	if (!status)
		return 0;
	else if (status != RMS$_FNF)
		return -1;
	else
		return 0;
}
#endif


static void blr_error(gds_ctl* control, const TEXT* string, ...)
{
/**************************************
 *
 *	b l r _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Report back an error message and unwind.
 *
 **************************************/
	USHORT offset;
	va_list args;

	va_start(args, string);
	blr_format(control, string, args);
	va_end(args);
	offset = 0;
	blr_print_line(control, (SSHORT) offset);
	Firebird::status_exception::raise();
}


static void blr_format(gds_ctl* control, const char* string, ...)
{
/**************************************
 *
 *	b l r _ f o r m a t
 *
 **************************************
 *
 * Functional description
 *	Format an utterance.
 *
 **************************************/
	va_list ptr;

	va_start(ptr, string);
	vsprintf(control->ctl_ptr, string, ptr);
	va_end(ptr);
	while (*control->ctl_ptr)
		control->ctl_ptr++;
}


static void blr_indent(gds_ctl* control, SSHORT level)
{
/**************************************
 *
 *	b l r _ i n d e n t
 *
 **************************************
 *
 * Functional description
 *	Indent for pretty printing.
 *
 **************************************/

	level *= 3;
	while (--level >= 0)
		PUT_BYTE(' ');
}



static void blr_print_blr(gds_ctl* control, UCHAR blr_operator)
{
/**************************************
 *
 *	b l r _ p r i n t _ b l r
 *
 **************************************
 *
 * Functional description
 *	Print a blr item.
 *
 **************************************/
	const char* p;

	if (blr_operator > FB_NELEM(blr_table) ||
		!(p = blr_table[blr_operator].blr_string))
	{
		blr_error(control, "*** blr operator %d is undefined ***",
				  (int) blr_operator);
	}

	blr_format(control, "blr_%s, ", p);
}


static SCHAR blr_print_byte(gds_ctl* control)
{
/**************************************
 *
 *	b l r _ p r i n t _ b y t e
 *
 **************************************
 *
 * Functional description
 *	Print a byte as a numeric value and return same.
 *
 **************************************/
	const UCHAR v = BLR_BYTE;
	blr_format(control, (control->ctl_language) ? "chr(%d), " : "%d, ",
			   (int) v);

	return v;
}


static SCHAR blr_print_char(gds_ctl* control)
{
/**************************************
 *
 *	b l r _ p r i n t _ c h a r
 *
 **************************************
 *
 * Functional description
 *	Print a byte as a numeric value and return same.
 *
 **************************************/
	SCHAR c;
	UCHAR v;

	v = c = BLR_BYTE;
	const bool printable = (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
	    (c >= '0' && c <= '9') ||
	     c == '$' ||
	     c == '_';

	if (printable)
		blr_format(control, "'%c',", (char) c);
	else if (control->ctl_language)
		blr_format(control, "chr(%d),", (int) v);
	else
		blr_format(control, "%d,", (int) c);

	return c;
}


static void blr_print_cond(gds_ctl* control)
{
/**************************************
 *
 *	b l r _ p r i n t _ c o n d
 *
 **************************************
 *
 * Functional description
 *	Print an error condition.
 *
 **************************************/
	SSHORT n;

	const USHORT ctype = BLR_BYTE;

	switch (ctype) {
	case blr_gds_code:
		blr_format(control, "blr_gds_code, ");
		n = blr_print_byte(control);
		while (--n >= 0)
			blr_print_char(control);
		break;

	case blr_exception:
		blr_format(control, "blr_exception, ");
		n = blr_print_byte(control);
		while (--n >= 0)
			blr_print_char(control);
		break;

	case blr_sql_code:
		blr_format(control, "blr_sql_code, ");
		blr_print_word(control);
		break;

	case blr_default_code:
		blr_format(control, "blr_default_code, ");
		break;

	default:
		blr_error(control, "*** invalid condition type ***");
		break;
	}
	return;
}


static int blr_print_dtype(gds_ctl* control)
{
/**************************************
 *
 *	b l r _ p r i n t _ d t y p e
 *
 **************************************
 *
 * Functional description
 *	Print a datatype sequence and return the length of the
 *	data described.
 *
 **************************************/
	SSHORT length;

	const USHORT dtype = BLR_BYTE;

/* Special case blob (261) to keep down the size of the
   jump table */
	const TEXT* string;

	switch (dtype) {
	case blr_short:
		string = "short";
		length = 2;
		break;

	case blr_long:
		string = "long";
		length = 4;
		break;

	case blr_int64:
		string = "int64";
		length = 8;
		break;

	case blr_quad:
		string = "quad";
		length = 8;
		break;

	case blr_timestamp:
		string = "timestamp";
		length = 8;
		break;

	case blr_sql_time:
		string = "sql_time";
		length = 4;
		break;

	case blr_sql_date:
		string = "sql_date";
		length = 4;
		break;

	case blr_float:
		string = "float";
		length = 4;
		break;

	case blr_double:
		{
			string = "double";

			/* for double literal, return the length of the numeric string */
			const UCHAR v1 = *(control->ctl_blr);
			const UCHAR v2 = *(control->ctl_blr + 1);
			length = ((v2 << 8) | v1) + 2;
			break;
		}

	case blr_d_float:
		string = "d_float";
		length = 8;
		break;

	case blr_text:
		string = "text";
		break;

	case blr_cstring:
		string = "cstring";
		break;

	case blr_varying:
		string = "varying";
		break;

	case blr_text2:
		string = "text2";
		break;

	case blr_cstring2:
		string = "cstring2";
		break;

	case blr_varying2:
		string = "varying2";
		break;

	default:
		blr_error(control, "*** invalid data type ***");
		break;
	}

	blr_format(control, "blr_%s, ", string);

	switch (dtype) {
	case blr_text:
		length = blr_print_word(control);
		break;

	case blr_varying:
		length = blr_print_word(control) + 2;
		break;

	case blr_short:
	case blr_long:
	case blr_quad:
	case blr_int64:
		blr_print_byte(control);
		break;

	case blr_text2:
		blr_print_word(control);
		length = blr_print_word(control);
		break;

	case blr_varying2:
		blr_print_word(control);
		length = blr_print_word(control) + 2;
		break;

	case blr_cstring2:
		blr_print_word(control);
		length = blr_print_word(control);
		break;

	default:
		if (dtype == blr_cstring)
			length = blr_print_word(control);
		break;
	}

	return length;
}


static void blr_print_join(gds_ctl* control)
{
/**************************************
 *
 *	b l r _ p r i n t _ j o i n
 *
 **************************************
 *
 * Functional description
 *	Print a join type.
 *
 **************************************/
	const TEXT *string;

	const USHORT join_type = BLR_BYTE;

	switch (join_type) {
	case blr_inner:
		string = "inner";
		break;

	case blr_left:
		string = "left";
		break;

	case blr_right:
		string = "right";
		break;

	case blr_full:
		string = "full";
		break;

	default:
		blr_error(control, "*** invalid join type ***");
		break;
	}

	blr_format(control, "blr_%s, ", string);
}


static SLONG blr_print_line(gds_ctl* control, SSHORT offset)
{
/**************************************
 *
 *	b l r _ p r i n t _ l i n e
 *
 **************************************
 *
 * Functional description
 *	Invoke callback routine to print (or do something with) a line.
 *
 **************************************/

	*control->ctl_ptr = 0;

	(*control->ctl_routine)(control->ctl_user_arg, offset, control->ctl_buffer);
	control->ctl_ptr = control->ctl_buffer;

	return control->ctl_blr - control->ctl_blr_start;
}


static void blr_print_verb(gds_ctl* control, SSHORT level)
{
/**************************************
 *
 *	b l r _ p r i n t _ v e r b
 *
 **************************************
 *
 * Functional description
 *	Primary recursive routine to print BLR.
 *
 **************************************/
	SLONG offset = control->ctl_blr - control->ctl_blr_start;
	blr_indent(control, level);
	UCHAR blr_operator = BLR_BYTE;

	if ((SCHAR) blr_operator == (SCHAR) blr_end) {
		blr_format(control, "blr_end, ");
		blr_print_line(control, (SSHORT) offset);
		return;
	}

	blr_print_blr(control, blr_operator);
	level++;
	const UCHAR* ops = blr_table[blr_operator].blr_operators;
	SSHORT n;

	while (*ops)
		switch (*ops++) {
		case op_verb:
			blr_print_verb(control, level);
			break;

		case op_line:
			offset = blr_print_line(control, (SSHORT) offset);
			break;

		case op_byte:
			n = blr_print_byte(control);
			break;

		case op_word:
			n = blr_print_word(control);
			break;

		case op_pad:
			PUT_BYTE(' ');
			break;

		case op_dtype:
			n = blr_print_dtype(control);
			break;

		case op_literal:
			while (--n >= 0)
				blr_print_char(control);
			break;

		case op_join:
			blr_print_join(control);
			break;

		case op_message:
			while (--n >= 0) {
				blr_indent(control, level);
				blr_print_dtype(control);
				offset = blr_print_line(control, (SSHORT) offset);
			}
			break;

		case op_parameters:
			level++;
			while (--n >= 0)
				blr_print_verb(control, level);
			level--;
			break;

		case op_error_handler:
			while (--n >= 0) {
				blr_indent(control, level);
				blr_print_cond(control);
				offset = blr_print_line(control, (SSHORT) offset);
			}
			break;

		case op_set_error:
			blr_print_cond(control);
			break;

		case op_indent:
			blr_indent(control, level);
			break;

		case op_begin:
			while ((SCHAR) * (control->ctl_blr) != (SCHAR) blr_end)
				blr_print_verb(control, level);
			break;

		case op_map:
			while (--n >= 0) {
				blr_indent(control, level);
				blr_print_word(control);
				offset = blr_print_line(control, (SSHORT) offset);
				blr_print_verb(control, level);
			}
			break;

		case op_args:
			while (--n >= 0)
				blr_print_verb(control, level);
			break;

		case op_literals:
			while (--n >= 0) {
				blr_indent(control, level);
				SSHORT n2 = blr_print_byte(control);
				while (--n2 >= 0)
					blr_print_char(control);
				offset = blr_print_line(control, (SSHORT) offset);
			}
			break;

		case op_union:
			while (--n >= 0) {
				blr_print_verb(control, level);
				blr_print_verb(control, level);
			}
			break;

		case op_relation:
			blr_operator = BLR_BYTE;
			blr_print_blr(control, blr_operator);
			if (blr_operator != blr_relation && blr_operator != blr_rid)
				blr_error(control,
						  "*** blr_relation or blr_rid must be object of blr_lock_relation, %d found ***",
						  (int) blr_operator);

			if (blr_operator == blr_relation) {
				n = blr_print_byte(control);
				while (--n >= 0)
					blr_print_char(control);
			}
			else
				blr_print_word(control);
			break;
		
		case op_exec_into: {
			blr_print_verb(control, level);
			if (! blr_print_byte(control)) {
				blr_print_verb(control, level);
			}
			while (n-- > 0) {
				blr_print_verb(control, level);
			}
			break;
		}

		case op_cursor_stmt: {
			blr_operator = blr_print_byte(control);
			blr_print_word(control);
			if (blr_operator == blr_cursor_fetch) {
#ifdef SCROLLABLE_CURSORS
				if (BLR_PEEK == blr_seek) {
					blr_print_verb(control, level);
				}
#endif
			}
			offset = blr_print_line(control, (SSHORT) offset);
			break;
		}

		default:
			fb_assert(false);
			break;
		}
}


static int blr_print_word(gds_ctl* control)
{
/**************************************
 *
 *	b l r _ p r i n t _ w o r d
 *
 **************************************
 *
 * Functional description
 *	Print a VAX word as a numeric value an return same.
 *
 **************************************/
	const UCHAR v1 = BLR_BYTE;
	const UCHAR v2 = BLR_BYTE;
	blr_format(control,
			   (control->ctl_language) ? "chr(%d),chr(%d), " : "%d,%d, ",
			   (int) v1, (int) v2);

	return (v2 << 8) | v1;
}


void gds__cleanup(void)
{
/**************************************
 *
 *	c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Exit handler for image exit.
 *
 **************************************/
#ifdef UNIX
	if (gds_pid != getpid())
		return;
#endif

	gds__msg_close(NULL);

	CLEAN clean;

	while (clean = cleanup_handlers) {
		cleanup_handlers = clean->clean_next;
		FPTR_VOID_PTR routine = clean->clean_routine;
		void* arg = clean->clean_arg;

		/* We must free the handler before calling it because there
		   may be a handler (and is) that frees all memory that has
		   been allocated. */

		FREE_LIB_MEMORY(clean);

		(*routine)(arg);
	}

#ifdef V4_THREADING
/* V4_DESTROY; */
#endif
	initialized = false;
}


static void init(void)
{
/**************************************
 *
 *	i n i t
 *
 **************************************
 *
 * Functional description
 *	Do anything necessary to initialize module/system.
 *
 **************************************/
	if (initialized)
		return;

	/* V4_INIT; */
	/* V4_GLOBAL_MUTEX_LOCK; */
	if (initialized) {
		/*  V4_GLOBAL_MUTEX_UNLOCK; */
		return;
	}

#ifdef VMS
	exit_description.exit_handler = cleanup;
	exit_description.args = 1;
	exit_description.arg[0] = &exit_status;
	ISC_STATUS status = sys$dclexh(&exit_description);
#endif

#ifdef UNIX
	gds_pid = getpid();
#ifdef SUPERSERVER
#if (defined SOLARIS || defined HPUX || defined LINUX)
	{
		/* Increase max open files to hard limit for Unix
		   platforms which are known to have low soft limits. */

		struct rlimit old;

		if (!getrlimit(RLIMIT_NOFILE, &old) && old.rlim_cur < old.rlim_max) {
			struct rlimit new_max;
			new_max.rlim_cur = new_max.rlim_max = old.rlim_max;
			if (!setrlimit(RLIMIT_NOFILE, &new_max))
			{
#if _FILE_OFFSET_BITS == 64 && defined SOLARIS
				gds__log("64 bit i/o support is on.");
				gds__log("Open file limit increased from %lld to %lld",
						 old.rlim_cur, new_max.rlim_cur);
		       
#else
				gds__log("Open file limit increased from %d to %d",
						 old.rlim_cur, new_max.rlim_cur);
#endif
			}
		}
	}
#endif
#endif /* SUPERSERVER */
#endif /* UNIX */

	atexit(gds__cleanup);

	initialized = true;

#ifndef REQUESTER
	ISC_signal_init();
#endif

	/* V4_GLOBAL_MUTEX_UNLOCK; */
}

static void sanitize(TEXT* locale)
{
/**************************************
 *
 *	s a n i t i z e
 *
 **************************************
 *
 * Functional description
 *	Clean up a locale to make it acceptable for use in file names
 *      for Windows NT, PC.
 *	replace any period with '_' for NT or PC.
 *
 **************************************/

	while (*locale) {
		if (*locale == '.')
			*locale = '_';
		locale++;
	}
}

static void safe_concat_path(TEXT *resultString, const TEXT *appendString)
{
/**************************************
 *
 *	s a f e _ c o n c a t _ p a t h
 *
 **************************************
 *
 * Functional description
 *	Safely appends appendString to resultString using paths rules.
 *  resultString must be at most MAXPATHLEN size.
 *
 **************************************/
	int len = strlen(resultString);
	if (resultString[len - 1] != PathUtils::dir_sep && len < MAXPATHLEN - 1) {
		resultString[len++] = PathUtils::dir_sep;
		resultString[len] = 0;
	}
	int alen = strlen(appendString);
	if (len + alen > MAXPATHLEN - 1)
		alen = MAXPATHLEN - 1 - len;
	fb_assert(alen >= 0);
	memcpy(&resultString[len], appendString, alen);
	resultString[len + alen] = 0;
}


void gds__default_printer(void* arg, SSHORT offset, const TEXT* line)
{
	printf("%4d %s\n", offset, line);
}


void gds__trace_printer(void* arg, SSHORT offset, const TEXT* line)
{
	// Assume that line is not too long
	char buffer[PRETTY_BUFFER_SIZE + 10];
	char* p = buffer;
	gds__ulstr(p, offset, 4, ' ');
	p += strlen(p);
	*p++ = ' ';
	strcpy(p, line); p += strlen(p);
	*p++ = '\n';
	*p = 0;
	gds__trace_raw(buffer);
}

#undef gds__alloc

void* API_ROUTINE gds__alloc(SLONG size_request)
{
	return getDefaultMemoryPool()->allocate_nothrow(size_request, 0
#ifdef DEBUG_GDS_ALLOC
		, __FILE__, __LINE__
#endif
	);
}

