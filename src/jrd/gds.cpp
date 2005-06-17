/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		gds.c
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

#define IO_RETRY	20

#ifdef SHLIB_DEFS
#define LOCAL_SHLIB_DEFS
#endif

// 11 Sept 2002 Nickolay Samofatov
// this defined in included dsc.h
//#define ISC_TIME_SECONDS_PRECISION		10000L
//#define ISC_TIME_SECONDS_PRECISION_SCALE	-4

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/gdsassert.h"
#include "../jrd/file_params.h"
#include "../jrd/msg_encode.h"
#include "../jrd/iberr.h"
#include "../jrd/gds_proto.h"
#include "../jrd/os/path_utils.h"
#include "../jrd/init.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifndef WIN_NT
#include <sys/param.h>
#include <errno.h>
#endif

#include <stdarg.h>
#include "../jrd/jrd_time.h"
#include "../jrd/misc.h"

#if defined(WIN_NT)
#include <io.h>
#include <process.h>
#include <sys/types.h>
#include <sys/timeb.h>
#endif

#ifdef VMS
#include <file.h>
#include <ib_perror.h>
#include <descrip.h>
#include <types.h>
#include <stat.h>
#include <rmsdef.h>

#else /* !VMS */

#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN_NT
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
/*
#ifdef WIN_NT
#define V4_THREADING
#endif

#ifdef SOLARIS_MT
#define V4_THREADING
#endif

#ifdef SUPERSERVER
#define V4_THREADING			// RFM: 9/22/2000 fix from Inprise tree,
								// Inprise bug 114840
#endif

// The following ifdef was added to build thread safe gds shared
//  library on linux platform. It seems the gdslib works now (20020220)
//  with thread enabled applications. Anyway, more tests should be 
//  done as I don't have deep knowledge of the interbase/firebird 
//  engine and this change may imply side effect I haven't known 
//  about yet. Tomas Nejedlik (tomas@nejedlik.cz)

#if ((defined(LINUX) || defined(FREEBSD)) && defined(SUPERCLIENT))
#define V4_THREADING
#endif
*/


#ifdef SUPERSERVER
static const TEXT gdslogid[] = " (Server)";
#else
#ifdef SUPERCLIENT
static const TEXT gdslogid[] = " (Client)";
#else
static const TEXT gdslogid[] = "";
#endif
#endif

static const char * FB_PID_FILE = "fb_%d";

#include "gen/sql_code.h"
#include "../jrd/thd.h"
#include "../jrd/gdsold.h"
//#include "../jrd/gen/codes.h" // is already included in gdsold.h
#include "../jrd/blr.h"
#include "../jrd/msg.h"
#include "../jrd/fil.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"
#ifndef REQUESTER
#include "../jrd/isc_i_proto.h"
#endif

#ifdef WIN_NT
#define _WINSOCKAPI_

#include <windows.h>
#include <share.h>
#include "thd_proto.h"
#include "err_proto.h"

#undef leave
#ifdef TEXT
#undef TEXT
#endif
#define TEXT		SCHAR
#endif /* WIN_NT */

static void gdsPrefixInit();	// C++ function

extern "C" {

#if !(defined VMS || defined WIN_NT || defined LINUX || defined FREEBSD || defined NETBSD || defined DARWIN )
extern int errno;
extern SCHAR *sys_errlist[];
extern int sys_nerr;
#endif

#ifndef PRINTF
#define PRINTF 			ib_printf
#endif

// Number of times to try to generate new name for temporary file
#define MAX_TMPFILE_TRIES 256

static char *ib_prefix = 0;
static char *ib_prefix_lock = 0;
static char *ib_prefix_msg = 0;



static const SCHAR * const messages[] = {
#include "gen/msgs.h"
	0							/* Null entry to terminate list */
};

#ifndef FOPEN_APPEND_TYPE
#define FOPEN_APPEND_TYPE	"a"
#endif

#ifndef O_BINARY
#define O_BINARY		0
#endif

#ifndef GENERIC_SQLCODE
#define GENERIC_SQLCODE		-999
#endif

static char ib_prefix_val[MAXPATHLEN];
static char ib_prefix_lock_val[MAXPATHLEN];
static char ib_prefix_msg_val[MAXPATHLEN];
static char fbTempDir[MAXPATHLEN];
#ifdef EMBEDDED
static char fbEmbeddedRoot[MAXPATHLEN];
#endif


#ifndef INCLUDE_FB_TYPES_H
#include "../include/fb_types.h"
#endif

typedef struct gds_msg
{
	ULONG msg_top_tree;
	int msg_file;
	USHORT msg_bucket_size;
	USHORT msg_levels;
	SCHAR msg_bucket[1];
} *GDS_MSG;

typedef struct ctl
{
	UCHAR *ctl_blr;				/* Running blr string */
	UCHAR *ctl_blr_start;		/* Original start of blr string */
	void (*ctl_routine) ();		/* Call back */
	SCHAR *ctl_user_arg;		/* User argument */
	TEXT *ctl_ptr;
	SSHORT ctl_language;
	TEXT ctl_buffer[1024];
} *CTL;

#ifdef DEV_BUILD
void GDS_breakpoint(int);
#endif


static void		blr_error(CTL, TEXT *, ...) ATTRIBUTE_FORMAT(2,3);
static void		blr_format(CTL, const char *, ...) ATTRIBUTE_FORMAT(2,3);
static void		blr_indent(CTL, SSHORT);
static void		blr_print_blr(CTL, UCHAR);
static SCHAR	blr_print_byte(CTL);
static SCHAR	blr_print_char(CTL);
static bool		blr_print_cond(CTL);
static int		blr_print_dtype(CTL);
static void		blr_print_join(CTL);
static SLONG	blr_print_line(CTL, SSHORT);
static void		blr_print_verb(CTL, SSHORT);
static int		blr_print_word(CTL);
static int		blr_get_word(CTL);

static void		init(void);
static int		yday(struct tm *);

static void		ndate(SLONG, struct tm *);
static GDS_DATE	nday(struct tm *);
static void		sanitize(TEXT *);

static void		safe_concat_path(TEXT *destbuf, const TEXT *srcbuf);

/* Generic cleanup handlers */

typedef struct clean
{
	struct clean*	clean_next;
	void			(*clean_routine)(void*);
	void*			clean_arg;
} *CLEAN;

static CLEAN	cleanup_handlers = NULL;
static GDS_MSG		default_msg = NULL;
static SLONG	initialized = FALSE;

void* API_ROUTINE gds__alloc_debug(SLONG size_request,
                                   TEXT* filename,
                                   ULONG lineno)
{
	return getDefaultMemoryPool()->allocate(size_request, 0
#ifdef DEBUG_GDS_ALLOC
		, filename, lineno
#endif
	);
}

ULONG API_ROUTINE gds__free(void* blk) {
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



#ifdef SHLIB_DEFS
#define sprintf		(*_libgds_sprintf)
#define vsprintf	(*_libgds_vsprintf)
#define strlen		(*_libgds_strlen)
#define strcpy		(*_libgds_strcpy)
#define _iob		(*_libgds__iob)
#define getpid		(*_libgds_getpid)
#define ib_fprintf	(*_libgds_fprintf)
#define ib_printf	(*_libgds_printf)
#define ib_fopen	(*_libgds_fopen)
#define ib_fclose	(*_libgds_fclose)
#define sys_nerr	(*_libgds_sys_nerr)
#define sys_errlist	(*_libgds_sys_errlist)
#define malloc		(*_libgds_malloc)
#define gettimeofday(*_libgds_gettimeofday)
#define ctime		(*_libgds_ctime)
#define getenv		(*_libgds_getenv)
#define lseek		(*_libgds_lseek)
#define read		(*_libgds_read)
#define memcpy		(*_libgds_memcpy)
#define open		(*_libgds_open)
#define strcat		(*_libgds_strcat)
#define ib_fputs	(*_libgds_fputs)
#define ib_fputc	(*_libgds_fputc)
#define mktemp		(*_libgds_mktemp)
#define unlink		(*_libgds_unlink)
#define close		(*_libgds_close)
#define strncmp		(*_libgds_strncmp)
#define time		(*_libgds_time)
#define ib_fflush	(*_libgds_fflush)
#define errno		(*_libgds_errno)
#define umask		(*_libgds_umask)
#define atexit		(*_libgds_atexit)
#define ib_vfprintf	(*_libgds_vfprintf)

extern int sprintf();
extern int vsprintf();
extern int strlen();
extern SCHAR *strcpy();
extern IB_FILE _iob[];
extern pid_t getpid();
extern int ib_fprintf();
extern int ib_printf();
extern IB_FILE *ib_fopen();
extern int ib_fclose();
extern int sys_nerr;
extern SCHAR *sys_errlist[];
extern void *malloc();
extern int gettimeofday();
extern SCHAR *ctime();
extern SCHAR *getenv();
extern off_t lseek();
extern int read();
extern void *memcpy();
extern int open();
extern SCHAR *strcat();
extern int ib_fputs();
extern int ib_fputc();
extern SCHAR *mktemp();
extern int unlink();
extern int close();
extern int strncmp();
extern time_t time();
extern int ib_fflush();
extern int errno;
extern mode_t umask();
extern int atexit();
extern int ib_vfprintf();
#endif /* SHLIB_DEFS */


/* BLR Pretty print stuff */

#define PRINT_VERB 	blr_print_verb (control, level)
#define PRINT_LINE	blr_print_line (control, (SSHORT) offset)
#define PRINT_BYTE	blr_print_byte (control)
#define PRINT_CHAR	blr_print_char (control)
#define PRINT_WORD	blr_print_word (control)
#define PRINT_COND	blr_print_cond (control)
#define PRINT_DTYPE	blr_print_dtype (control)
#define PRINT_JOIN	blr_print_join (control)
#define BLR_BYTE	*(control->ctl_blr)++
#define BLR_WORD	blr_get_word (control)
#define PUT_BYTE(byte)	*(control->ctl_ptr)++ = byte

#define op_line		 1
#define op_verb		 2
#define op_byte		 3
#define op_word		 4
#define op_pad		 5
#define op_dtype	 6
#define op_message	 7
#define op_literal	 8
#define op_begin	 9
#define op_map		 10
#define op_args		 11
#define op_union	 12
#define op_indent	 13
#define op_join		 14
#define op_parameters	 15
#define op_error_handler 16
#define op_set_error	 17
#define op_literals	 18
#define op_relation	 20
#define op_exec_into 21

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
	set_error[] = { op_line, op_set_error, 0},
	cast[]		= { op_dtype, op_line, op_verb, 0},
	indices[]	= { op_byte, op_line, op_literals, 0},
	lock_relation[] = { op_line, op_indent, op_relation, op_line, op_verb, 0},
	range_relation[] = { op_line, op_verb, op_indent, op_relation, op_line, 0},
	extract[]	= { op_line, op_byte, op_verb, 0},
	user_savepoint[]	= { op_byte, op_byte, op_literal, op_line, 0},
	exec_into[] = { op_line, op_exec_into, 0};

static const struct
{
	const char *blr_string;
	const UCHAR *blr_operators;
} blr_table[] =
{

#pragma FB_COMPILER_MESSAGE("Fix this!")

#include "../jrd/blp.h"
	{0, 0}
};


#define FB_ENV			"FIREBIRD"
#define FB_LOCK_ENV		"FIREBIRD_LOCK"
#define FB_MSG_ENV		"FIREBIRD_MSG"
#define FB_TMP_ENV		"FIREBIRD_TMP"

#ifdef WIN_NT
#define EXPAND_PATH(relative, absolute)		_fullpath(absolute, relative, MAXPATHLEN)
#define COMPARE_PATH(a,b)			_stricmp(a,b)
#else
#define EXPAND_PATH(relative, absolute)		realpath(relative, absolute)
#define COMPARE_PATH(a,b)			strcmp(a,b)
#endif


ISC_STATUS API_ROUTINE gds__decode(ISC_STATUS code, USHORT* fac, USHORT* class_)
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
	*class_ = GET_CLASS(code);
	return GET_CODE(code);

}


void API_ROUTINE isc_decode_date(GDS_QUAD* date, void* times_arg)
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
	isc_decode_timestamp((GDS_TIMESTAMP*) date, times_arg);
}


void API_ROUTINE isc_decode_sql_date(GDS_DATE* date, void* times_arg)
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
	struct tm *times;

	times = (struct tm *) times_arg;
	memset(times, 0, sizeof(*times));

	ndate(*date, times);
	times->tm_yday = yday(times);
	if ((times->tm_wday = (*date + 3) % 7) < 0)
		times->tm_wday += 7;
}


void API_ROUTINE isc_decode_sql_time(GDS_TIME * sql_time, void *times_arg)
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
	ULONG minutes;
	struct tm *times;

	times = (struct tm *) times_arg;
	memset(times, 0, sizeof(*times));

	minutes = *sql_time / (ISC_TIME_SECONDS_PRECISION * 60);
	times->tm_hour = minutes / 60;
	times->tm_min = minutes % 60;
	times->tm_sec = (*sql_time / ISC_TIME_SECONDS_PRECISION) % 60;
}


void API_ROUTINE isc_decode_timestamp(GDS_TIMESTAMP * date, void *times_arg)
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
 *	Note: the date arguement is really an ISC_TIMESTAMP -- however the
 *	definition of ISC_TIMESTAMP is not available from all the source
 *	modules that need to use isc_encode_timestamp
 *
 **************************************/
	SLONG minutes;
	struct tm *times;

	times = (struct tm *) times_arg;
	memset(times, 0, sizeof(*times));

	ndate(date->timestamp_date, times);
	times->tm_yday = yday(times);
	if ((times->tm_wday = (date->timestamp_date + 3) % 7) < 0)
		times->tm_wday += 7;

	minutes = date->timestamp_time / (ISC_TIME_SECONDS_PRECISION * 60);
	times->tm_hour = minutes / 60;
	times->tm_min = minutes % 60;
	times->tm_sec = (date->timestamp_time / ISC_TIME_SECONDS_PRECISION) % 60;
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


void API_ROUTINE isc_encode_date(void *times_arg, GDS_QUAD * date)
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
	isc_encode_timestamp(times_arg, (GDS_TIMESTAMP *) date);
}


void API_ROUTINE isc_encode_sql_date(void *times_arg, GDS_DATE * date)
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

	*date = nday((struct tm *) times_arg);
}


void API_ROUTINE isc_encode_sql_time(void *times_arg, GDS_TIME * isc_time)
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
	struct tm *times;

	times = (struct tm *) times_arg;
	*isc_time = ((times->tm_hour * 60 + times->tm_min) * 60 +
				 times->tm_sec) * ISC_TIME_SECONDS_PRECISION;
}


void API_ROUTINE isc_encode_timestamp(void *times_arg, GDS_TIMESTAMP * date)
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
 *	Note: the date arguement is really an ISC_TIMESTAMP -- however the
 *	definition of ISC_TIMESTAMP is not available from all the source
 *	modules that need to use isc_encode_timestamp
 *
 **************************************/
	struct tm *times;

	times = (struct tm *) times_arg;

	date->timestamp_date = nday(times);
	date->timestamp_time =
		((times->tm_hour * 60 + times->tm_min) * 60 +
		 times->tm_sec) * ISC_TIME_SECONDS_PRECISION;
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


SINT64 API_ROUTINE isc_portable_integer(UCHAR* ptr, SSHORT length)
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

	assert(length <= 8);
	value = shift = 0;

	while (--length >= 0) {
		value += ((SINT64) * ptr++) << shift;
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

void API_ROUTINE gds_alloc_report(ULONG flags, char* filename, int lineno)
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
}

SLONG API_ROUTINE gds__interprete(char *s, ISC_STATUS ** vector)
{
/**************************************
 *
 *	g d s _ $ i n t e r p r e t e
 *
 **************************************
 *
 * Functional description
 *	Translate a status code with arguments to a string.  Return the
 *	length of the string while updating the vector address.  If the
 *	message is null (end of messages) or invalid, return 0;
 *
 **************************************/
	TEXT *p, *q, *temp;
	SSHORT l, temp_len;

	TEXT **arg, *args[10];
	ISC_STATUS code, *v;
	UCHAR x;
	ISC_STATUS decoded;
#ifdef VMS
	ISC_STATUS status;
	TEXT flags[4];
	struct dsc$descriptor_s desc;
#endif

	if (!**vector)
		return 0;

	temp = NULL;

	/* handle a case: "no errors, some warning(s)" */
	if ((*vector)[1] == 0 && (*vector)[2] == isc_arg_warning) {
		v = *vector + 4;
		code = (*vector)[3];
	}
	else {
		v = *vector + 2;
		code = (*vector)[1];
	}

	arg = args;

	/* Parse and collect any arguments that may be present */

	for (;;) {
		x = (UCHAR) * v++;
		switch (x) {
		case gds_arg_string:
		case gds_arg_number:
			*arg++ = (TEXT *) * v++;
			continue;

		case gds_arg_cstring:
			if (!temp) {
				/* We need a temporary buffer when cstrings are involved.
				   Give up if we can't get one. */

				p = temp = (TEXT*) gds__alloc((SLONG) BUFFER_SMALL);
				temp_len = (SSHORT) BUFFER_SMALL;
				/* FREE: at procedure exit */
				if (!temp)		/* NOMEM: */
					return 0;
			}
			l = (SSHORT) * v++;
			q = (TEXT *) * v++;
			*arg++ = p;

			/* ensure that we do not overflow the buffer allocated */
			l = (temp_len < l) ? temp_len : l;
			if (l)
				do
					*p++ = *q++;
				while (--l);
			*p++ = 0;
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
	case gds_arg_gds:
		{
			USHORT fac = 0, class_ = 0;
			decoded = gds__decode(code, &fac, &class_);
			if (gds__msg_format(0, fac, (USHORT) decoded,
								128, s, args[0], args[1], args[2], args[3],
								args[4]) < 0) {
				if ((decoded < FB_NELEM(messages) - 1) && (decoded >= 0))
					sprintf(s, messages[decoded], args[0], args[1], args[2],
							args[3], args[4]);
				else
					sprintf(s, "unknown ISC error %ld", code);	/* TXNN */
			}
		}
		break;

	case gds_arg_interpreted:
		p = s;
		q = (TEXT *) (*vector)[1];
		while ((*p++ = *q++) /*!= NULL*/);
		break;

	case gds_arg_unix:
		if (code > 0 && code < sys_nerr && (p = (TEXT*)sys_errlist[code]))
			strcpy(s, p);
		else if (code == 60)
			strcpy(s, "connection timed out");
		else if (code == 61)
			strcpy(s, "connection refused");
		else
			sprintf(s, "unknown unix error %ld", code);	/* TXNN */
		break;

	case gds_arg_dos:

		sprintf(s, "unknown dos error %ld", code);	/* TXNN */
		break;

#ifdef VMS
	case gds_arg_vms:
		l = 0;
		desc.dsc$b_class = DSC$K_CLASS_S;
		desc.dsc$b_dtype = DSC$K_DTYPE_T;
		desc.dsc$w_length = 128;
		desc.dsc$a_pointer = s;
		status = sys$getmsg(code, &l, &desc, 15, flags);
		if (status & 1)
			s[l] = 0;
		else
			sprintf(s, "uninterpreted VMS code %x", code);	/* TXNN */
		break;
#endif

#ifdef WIN_NT
	case gds_arg_win32:
		if (!(l = (SSHORT) FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
										 NULL,
										 code,
										 GetUserDefaultLangID(),
										 s,
										 128,
						                 NULL))
		  && !(l = (SSHORT)FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
										 NULL,
										 code,
										 0, /* TMN: Fallback to system known language */
										 s,
										 128,
										 NULL))) 
			sprintf(s, "unknown Win32 error %ld", code);	/* TXNN */
		break;
#endif

	default:
		if (temp)
			gds__free((SLONG *) temp);
		return 0;
	}


	if (temp)
		gds__free((SLONG *) temp);

	*vector = v;
	p = s;
	while (*p)
		p++;

	return p - s;
}


void API_ROUTINE gds__interprete_a(
								   SCHAR * s,
								   SSHORT * length,
								   ISC_STATUS * vector, SSHORT * offset)
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
	ISC_STATUS *v;

	v = vector + *offset;
	*length = (SSHORT) gds__interprete(s, &v);
	*offset = v - vector;
}


void API_ROUTINE gds__log(const TEXT * text, ...)
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
	IB_FILE *file;
	int oldmask;
	time_t now;
	TEXT name[MAXPATHLEN];

#ifdef HAVE_GETTIMEOFDAY
	{
	    struct timeval tv;
#ifdef GETTIMEOFDAY_RETURNS_TIMEZONE
	    (void)gettimeofday(&tv, (struct timezone *)0);
#else
	    (void)gettimeofday(&tv);
#endif
	    now = tv.tv_sec;
	}
#else
	now = time((time_t *)0);
#endif

	gds__prefix(name, LOGFILE);

	oldmask = umask(0111);

	if ((file = ib_fopen(name, FOPEN_APPEND_TYPE)) != NULL)
	{
		ib_fprintf(file, "%s%s\t%.25s\t", ISC_get_host(name, MAXPATHLEN),
 				   gdslogid, ctime(&now));
		VA_START(ptr, text);
		ib_vfprintf(file, text, ptr);
		ib_fprintf(file, "\n\n");
		ib_fclose(file);
	}

	umask(oldmask);

}


void API_ROUTINE gds__log_status(TEXT * database, ISC_STATUS * status_vector)
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
	TEXT *buffer, *p;

	buffer = (TEXT *) gds__alloc((SLONG) BUFFER_XLARGE);
/* FREE: at procedure exit */
	if (!buffer)				/* NOMEM: */
		return;

	p = buffer;
	sprintf(p, "Database: %s", (database) ? database : "");

	do {
		while (*p)
			p++;
		*p++ = '\n';
		*p++ = '\t';
	} while (gds__interprete(p, &status_vector));

	p[-2] = 0;
	gds__log(buffer, 0);
	gds__free((SLONG *) buffer);
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

	GDS_MSG message = reinterpret_cast < GDS_MSG > (handle);

	if (!message) {
		if (!default_msg) {
			return 0;
		}
		message = default_msg;
	}

	default_msg = (GDS_MSG) NULL;

	int fd = message->msg_file;

	FREE_LIB_MEMORY(message);

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
	TEXT *p, *formatted, *end;
	SLONG size;
	int n;

	size = (SLONG) (((arg1) ? MAX_ERRSTR_LEN : 0) +
					((arg2) ? MAX_ERRSTR_LEN : 0) +
					((arg3) ? MAX_ERRSTR_LEN : 0) +
					((arg4) ? MAX_ERRSTR_LEN : 0) +
					((arg5) ? MAX_ERRSTR_LEN : 0) + MAX_ERRMSG_LEN);

	size = (size < length) ? length : size;

	formatted = (TEXT *) gds__alloc((SLONG) size);

	if (!formatted)				/* NOMEM: */
		return -1;

/* Let's assume that the text to be output will never be shorter
   than the raw text of the message to be formatted.  Then we can
   use the caller's buffer to temporarily hold the raw text. */

	n = gds__msg_lookup(handle, facility, number, length, buffer, NULL);

	if (n > 0 && n < length)
	{
		sprintf(formatted, buffer, arg1, arg2, arg3, arg4, arg5);
	}
	else
	{
		sprintf(formatted, "can't format message %d:%d -- ", facility,
				number);
		if (n == -1)
			strcat(formatted, "message text not found");
		else if (n == -2) {
			strcat(formatted, "message file ");
			for (p = formatted; *p;)
				p++;
			gds__prefix_msg(p, MSG_FILE);
			strcat(p, " not found");
		}
		else {
			for (p = formatted; *p;)
				p++;
			sprintf(p, "message system code %d", n);
		}
	}

	USHORT l = strlen(formatted);
	end = buffer + length - 1;

	for (p = formatted; *p && buffer < end;) {
		*buffer++ = *p++;
	}
	*buffer = 0;

	gds__free((SLONG *) formatted);
	return ((n > 0) ? l : -l);
}


SSHORT API_ROUTINE gds__msg_lookup(void *handle,
								   USHORT facility,
								   USHORT number,
								   USHORT length,
								   TEXT * buffer, USHORT * flags)
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
	GDS_MSG message;
	int status;
	USHORT n;
	MSGREC leaf;
	MSGNOD node, end;
	ULONG position, code;
	TEXT *p;

/* Handle default message file */

	if (!(message = (GDS_MSG) handle) && !(message = default_msg)) {
		/* Try environment variable setting first */

		if ((p = getenv("ISC_MSGS")) == NULL ||
			(status =
			 gds__msg_open(reinterpret_cast < void **>(&message), p))) {
			TEXT translated_msg_file[sizeof(MSG_FILE_LANG) + LOCALE_MAX + 1];
			TEXT *msg_file;

			/* Try declared language of this attachment */
			/* This is not quite the same as the language declared on the
			   READY statement */

			msg_file = (TEXT *) gds__alloc((SLONG) MAXPATHLEN);
			/* FREE: at block exit */
			if (!msg_file)		/* NOMEM: */
				return -2;

			p = getenv("LC_MESSAGES");
			if (p != NULL) {
				sanitize(p);
				sprintf(translated_msg_file, MSG_FILE_LANG, p);
				gds__prefix_msg(msg_file, translated_msg_file);
				status =
					gds__msg_open(reinterpret_cast < void **>(&message),
								  msg_file);
			}
			else
				status = 1;
			if (status) {
				/* Default to standard message file */

				gds__prefix_msg(msg_file, MSG_FILE);
				status =
					gds__msg_open(reinterpret_cast < void **>(&message),
								  msg_file);
			}
			gds__free((SLONG *) msg_file);
		}

		if (status)
			return status;

		default_msg = message;
	}

/* Search down index levels to the leaf.  If we get lost, punt */

	code = MSG_NUMBER(facility, number);
	end = (MSGNOD) ((SCHAR *) message->msg_bucket + message->msg_bucket_size);
	position = message->msg_top_tree;

	for (n = 1, status = 0; !status; n++) {
		if (lseek(message->msg_file, LSEEK_OFFSET_CAST position, 0) < 0)
			status = -6;
		else if (read(message->msg_file, message->msg_bucket,
					  message->msg_bucket_size) < 0)
			status = -7;
		else if (n == message->msg_levels)
			break;
		else {
			for (node = (MSGNOD) message->msg_bucket; !status; node++) {
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
		for (leaf = (MSGREC) message->msg_bucket; !status;
			 leaf = NEXT_LEAF(leaf)) {
			if (leaf >= (MSGREC) end || leaf->msgrec_code > code) {
				status = -1;
				break;
			}
			else if (leaf->msgrec_code == code) {
				/* We found the correct message, so return it to the user */
				n = MIN(length - 1, leaf->msgrec_length);
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


int API_ROUTINE gds__msg_open(void **handle, TEXT * filename)
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
	int n;
	GDS_MSG message;
	ISC_MSGHDR header;

	if ((n = open(filename, O_RDONLY | O_BINARY, 0)) < 0)
		return -2;

	if (read(n, &header, sizeof(header)) < 0) {
		(void) close(n);
		return -3;
	}

	if (header.msghdr_major_version != MSG_MAJOR_VERSION ||
		(SSHORT) header.msghdr_minor_version < MSG_MINOR_VERSION) {
		(void) close(n);
		return -4;
	}

	message =
		(GDS_MSG) ALLOC_LIB_MEMORY((SLONG) sizeof(gds_msg) +
							   header.msghdr_bucket_size - 1);
/* FREE: in gds__msg_close */
	if (!message) {				/* NOMEM: return non-open error */
		(void) close(n);
		return -5;
	}

#ifdef DEBUG_GDS_ALLOC
/* This will only be freed if the client closes the message file for us */
	gds_alloc_flag_unfreed((void *) message);
#endif

	message->msg_file = n;
	message->msg_bucket_size = header.msghdr_bucket_size;
	message->msg_levels = header.msghdr_levels;
	message->msg_top_tree = header.msghdr_top_tree;


	*handle = message;

	return 0;
}


void API_ROUTINE gds__msg_put(
							  void *handle,
							  USHORT facility,
							  USHORT number,
							  TEXT * arg1,
TEXT * arg2, TEXT * arg3, TEXT * arg4, TEXT * arg5)
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
	TEXT formatted[512];

	gds__msg_format(handle, facility, number, sizeof(TEXT) * BUFFER_MEDIUM,
					formatted, arg1, arg2, arg3, arg4, arg5);
	gds__put_error(formatted);
}


SLONG API_ROUTINE gds__get_prefix(SSHORT arg_type, TEXT * passed_string)
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

	char *prefix_ptr;
	int count = 0;

	if (arg_type < IB_PREFIX_TYPE || arg_type > IB_PREFIX_MSG_TYPE)
		return ((SLONG) - 1);

	gdsPrefixInit();

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
			|| *passed_string == 13 || (count == MAXPATHLEN)) {
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
void API_ROUTINE gds__prefix(TEXT *resultString, const TEXT *file)
{
/**************************************
 *
 *	g d s _ $ p r e f i x	( n o n - V M S )
 *
 **************************************
 *
 * Functional description
 *	Find appropriate file prefix.
 *	Override conditional defines with
 *	the enviroment variable FIREBIRD if it is set.
 *
 **************************************/
	resultString[0] = 0;

	gdsPrefixInit();

	strcat(resultString, ib_prefix);
	safe_concat_path(resultString, file);
}
#endif /* !defined(VMS) */


#ifdef VMS
void API_ROUTINE gds__prefix(TEXT * string, const TEXT * root)
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
	TEXT temp[256], *p, *q;
	ISC_VMS_PREFIX prefix;
	SSHORT len;

	if (*root != '[') {
		strcpy(string, root);
		return;
	}

/* Check for the existence of an InterBase logical name.  If there is 
   one use it, otherwise use the system directories. */

	if (ISC_expand_logical_once(ISC_LOGICAL, sizeof(ISC_LOGICAL) - 2, temp)) {
		strcpy(string, ISC_LOGICAL);
		strcat(string, root);
		return;
	}

	for (p = temp, q = root; *p = UPPER7(*q); q++)
		if (*p++ == ']')
			break;

	len = p - temp;
	for (prefix = trans_prefix; prefix->isc_prefix; prefix++)
		if (!strncmp(temp, prefix->isc_prefix, len)) {
			strcpy(string, prefix->vms_prefix);
			strcat(string, &root[len]);
			return;
		}

	strcpy(string, &root[len]);
}
#endif


#ifndef VMS
void API_ROUTINE gds__prefix_lock(TEXT * string, const TEXT * root)
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

	gdsPrefixInit();

#ifdef EMBEDDED
	char buf[MAXPATHLEN];
	fb_utils::snprintf(buf, MAXPATHLEN, root, fbEmbeddedRoot);
	root = buf;
#endif

	strcpy(string, ib_prefix_lock);	// safe - no BO
	safe_concat_path(string, root);
}
#endif


#ifdef VMS
void API_ROUTINE gds__prefix_lock(TEXT * string, const TEXT * root)
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
	TEXT temp[256], *p, *q;
	ISC_VMS_PREFIX prefix;
	SSHORT len;

	if (*root != '[') {
		strcpy(string, root);
		return;
	}

/* Check for the existence of a Firebird logical name.  If there is 
   one use it, otherwise use the system directories. */

	if (ISC_expand_logical_once
		(ISC_LOGICAL_LOCK, sizeof(ISC_LOGICAL_LOCK) - 2, temp)) {
		strcpy(string, ISC_LOGICAL_LOCK);
		strcat(string, root);
		return;
	}

	for (p = temp, q = root; *p = UPPER7(*q); q++)
		if (*p++ == ']')
			break;

	len = p - temp;
	for (prefix = trans_prefix; prefix->isc_prefix; prefix++)
		if (!strncmp(temp, prefix->isc_prefix, len)) {
			strcpy(string, prefix->vms_prefix);
			strcat(string, &root[len]);
			return;
		}

	strcpy(string, &root[len]);
}
#endif

#ifndef VMS
void API_ROUTINE gds__prefix_msg(TEXT * string, const TEXT * root)
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

	gdsPrefixInit();

	strcpy(string, ib_prefix_msg);	// safe - no BO
	safe_concat_path(string, root);
}
#endif

#ifdef VMS
void API_ROUTINE gds__prefix_msg(TEXT * string, const TEXT * root)
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
	TEXT temp[256], *p, *q;
	ISC_VMS_PREFIX prefix;
	SSHORT len;

	if (*root != '[') {
		strcpy(string, root);
		return;
	}


/* Check for the existence of an InterBase logical name.  If there is
   one use it, otherwise use the system directories. */

/* ISC_LOGICAL_MSG macro needs to be defined, check non VMS version of routine
   for functionality. */
	if (ISC_expand_logical_once
		(ISC_LOGICAL_MSG, sizeof(ISC_LOGICAL_MSG) - 2, te mp)) {
		strcpy(string, ISC_LOGICAL_MSG);
		strcat(string, root);
		return;
	}

	for (p = temp, q = root; *p = UPPER7(*q); q++)
		if (*p++ == ']')
			break;

	len = p - temp;
	for (prefix = trans_prefix; prefix->isc_prefix; prefix++)
		if (!strncmp(temp, prefix->isc_prefix, len)) {
			strcpy(string, prefix->vms_prefix);
			strcat(string, &root[len]);
			return;
		}

	strcpy(string, &root[len]);
}
#endif


ISC_STATUS API_ROUTINE gds__print_status(ISC_STATUS * vec)
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
	ISC_STATUS *vector;
	TEXT *s;

	if (!vec || (!vec[1] && vec[2] == gds_arg_end))
		return FB_SUCCESS;

	s = (TEXT *) gds__alloc((SLONG) BUFFER_LARGE);
/* FREE: at procedure return */
	if (!s)						/* NOMEM: */
		return vec[1];

	vector = vec;

	if (!gds__interprete(s, &vector)) {
		gds__free((SLONG *) s);
		return vec[1];
	}

	gds__put_error(s);
	s[0] = '-';

	while (gds__interprete(s + 1, &vector))
		gds__put_error(s);

	gds__free((SLONG *) s);

	return vec[1];
}


USHORT API_ROUTINE gds__parse_bpb(USHORT bpb_length,
								  UCHAR * bpb,
								  USHORT * source, USHORT * target)
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
								   UCHAR * bpb,
								   SSHORT * source,
								   SSHORT * target,
								   USHORT * source_interp,
								   USHORT * target_interp)
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
	UCHAR *p, *end, op;
	USHORT type, length;

	type = *source = *target = 0;

	if (source_interp)
		*source_interp = 0;
	if (target_interp)
		*target_interp = 0;

	if (!bpb_length)
		return type;

	p = bpb;
	end = p + bpb_length;

	if (*p++ != gds_bpb_version1)
		return type;

	while (p < end) {
		op = *p++;
		length = *p++;
		switch (op) {
		case gds_bpb_source_type:
			*source = (USHORT) gds__vax_integer(p, length);
			break;

		case gds_bpb_target_type:
			*target = (USHORT) gds__vax_integer(p, length);
			break;

		case gds_bpb_type:
			type = (USHORT) gds__vax_integer(p, length);
			break;

		case gds_bpb_source_interp:
			if (source_interp)
				*source_interp = (USHORT) gds__vax_integer(p, length);
			break;

		case gds_bpb_target_interp:
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


SLONG API_ROUTINE gds__ftof(SCHAR * string,
							USHORT GDS_VAL(length1),
							SCHAR * field,
							USHORT GDS_VAL(length2))
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
	USHORT l, fill;

	fill = GDS_VAL(length2) - GDS_VAL(length1);

	if ((l = MIN(GDS_VAL(length1), GDS_VAL(length2))) > 0)
		do
			*field++ = *string++;
		while (--l);

	if (fill > 0)
		do
			*field++ = ' ';
		while (--fill);

	return 0;
}


int API_ROUTINE gds__print_blr(
							   UCHAR * blr,
							   FPTR_VOID routine,
							   SCHAR * user_arg, SSHORT language)
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
	struct ctl ctl, *control;
	SCHAR eoc;
	SLONG offset;
	SSHORT version, level;

	try {

	control = &ctl;
	level = 0;
	offset = 0;

	if (!routine) {
		routine = (void (*)()) PRINTF;
		user_arg = "%4d %s\n";
	}

	control->ctl_routine = routine;
	control->ctl_user_arg = user_arg;
	control->ctl_blr = control->ctl_blr_start = blr;
	control->ctl_ptr = control->ctl_buffer;
	control->ctl_language = language;

	version = BLR_BYTE;

	if ((version != blr_version4) && (version != blr_version5))
		blr_error(control, "*** blr version %d is not supported ***",
				  (int) version);

	blr_format(control,
			   (version == blr_version4) ? "blr_version4," : "blr_version5,");
	PRINT_LINE;
	PRINT_VERB;

	offset = control->ctl_blr - control->ctl_blr_start;
	eoc = BLR_BYTE;

	if (eoc != blr_eoc)
		blr_error(control, "*** expected end of command, encounted %d ***",
				  (int) eoc);

	blr_format(control, "blr_eoc");
	PRINT_LINE;

	}	// try
	catch (const std::exception&) {
		return -1;
	}

	return 0;
}


void API_ROUTINE gds__put_error(TEXT * string)
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
#define PUT_ERROR
	struct dsc$descriptor_s desc;

	ISC_make_desc(string, &desc, 0);
	lib$put_output(&desc);
#endif

#ifdef PUT_ERROR
#undef PUT_ERROR
#else
	ib_fputs(string, ib_stderr);
	ib_fputc('\n', ib_stderr);
	ib_fflush(ib_stderr);
#endif
}


void API_ROUTINE gds__qtoq(void *quad_in, void *quad_out)
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

	*((GDS_QUAD *) quad_out) = *((GDS_QUAD *) quad_in);
}


void API_ROUTINE gds__register_cleanup(FPTR_VOID_PTR routine, void *arg)
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

	CLEAN clean;

	if (!initialized)
		init();

	clean = (CLEAN) ALLOC_LIB_MEMORY((SLONG) sizeof(struct clean));
	clean->clean_next = cleanup_handlers;
	cleanup_handlers = clean;
	clean->clean_routine = routine;
	clean->clean_arg = arg;

#ifdef DEBUG_GDS_ALLOC
/* Startup function - known to be unfreed */
	gds_alloc_flag_unfreed((void *) clean);
#endif
}


SLONG API_ROUTINE gds__sqlcode(ISC_STATUS * status_vector)
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
	USHORT code;
	SLONG sqlcode;
	ISC_STATUS *s;
	USHORT have_sqlcode;

	if (!status_vector) {
		DEV_REPORT("gds__sqlcode: NULL status vector");
		return GENERIC_SQLCODE;
	}

	have_sqlcode = FALSE;
	sqlcode = GENERIC_SQLCODE;	/* error of last resort */

/* SQL code -999 (GENERIC_SQLCODE) is generic, meaning "no other sql code
 * known".  Now scan the status vector, seeing if there is ANY sqlcode
 * reported.  Make note of the first error in the status vector who's 
 * SQLCODE is NOT -999, that will be the return code if there is no specific
 * sqlerr reported.
 */

	s = status_vector;
	while (*s != gds_arg_end)
	{
		if (*s == gds_arg_gds)
		{
			s++;
			if (*s == gds_sqlerr)
			{
				return *(s + 2);
			}

			if (!have_sqlcode) {
				/* Now check the hard-coded mapping table of gds_codes to
				   sql_codes */
				USHORT fac = 0, class_ = 0;

				code = (USHORT) gds__decode(status_vector[1], &fac, &class_);

				if ((code < FB_NELEM(gds__sql_code)) &&
					(gds__sql_code[code] != GENERIC_SQLCODE))
				{
					sqlcode = gds__sql_code[code];
					have_sqlcode = TRUE;
				}
			}
			s++;
		}
		else if (*s == gds_arg_cstring)
			s += 3;				/* skip: gds_arg_cstring <len> <ptr> */
		else
			s += 2;				/* skip: gds_arg_* <item> */
	};

	return sqlcode;
}


void API_ROUTINE gds__sqlcode_s(ISC_STATUS * status_vector, ULONG * sqlcode)
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
 *	that.  THIS IS THE COBOL VERSION.  (Some cobols son't have 
 *	return values for calls...
 *
 **************************************/

	*sqlcode = gds__sqlcode(status_vector);
}


void API_ROUTINE gds__temp_dir(TEXT * buffer)
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
	buffer[0] = 0;

	gdsPrefixInit();

	strcpy(buffer, fbTempDir);	// safe - no BO
}

	
void * API_ROUTINE gds__temp_file(
					 BOOLEAN stdio_flag, TEXT * string, 
					 TEXT * expanded_string, TEXT * dir, BOOLEAN unlink_flag)
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
 *      Function returns untyped handle that needs to be casted to either IB_FILE
 *      or used as file descriptor. This is ugly and needs to be fixed probably 
 *      via introducing two functions with different return types.
 *
 **************************************/
	TEXT temp_dir[MAXPATHLEN];

	TEXT *directory = dir;
	if (!directory) {
		gds__temp_dir(temp_dir);
		directory = temp_dir;
	}
	if (strlen(directory) >= MAXPATHLEN-strlen(string)-strlen(TEMP_PATTERN)-2)
		return (void *)-1;

	void *result;

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
		if (file_name[strlen(file_name)-1] != '\\')
			strcat(file_name, "\\");
		strcat(file_name, string);
		char suffix[] = TEMP_PATTERN;
		__int64 temp = randomness;
		for (int i=0; i<sizeof(suffix)-1; i++) {
			suffix[i] = letters[temp % (sizeof(letters)-1)];
			temp /= sizeof(letters)-1;
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
		if (!(result = ib_fdopen((int) result, "w+b")))
			return (void *)-1;
	}
#else
	TEXT file_name[MAXPATHLEN];
	strcpy(file_name, directory);
	if (file_name[strlen(file_name)-1] != '/')
		strcat(file_name, "/");
	strcat(file_name, string);
	strcat(file_name, TEMP_PATTERN);
	
#ifdef HAVE_MKSTEMP
	result = (void *)mkstemp(file_name);
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
		if (!(result = ib_fdopen((int) result, "w+")))
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
	CLEAN *clean_ptr, clean;

	for (clean_ptr = &cleanup_handlers; clean = *clean_ptr;
		 clean_ptr = &clean->clean_next) {
        if (clean->clean_routine == routine
            && clean->clean_arg == arg) {
			*clean_ptr = clean->clean_next;
			FREE_LIB_MEMORY(clean);
			break;
		}
    }
}


#ifndef VMS
BOOLEAN API_ROUTINE gds__validate_lib_path(TEXT * module,
										   TEXT * ib_env_var,
										   TEXT * resolved_module,
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
	TEXT *p, *q;
	TEXT *token;
	TEXT abs_module[MAXPATHLEN];
	TEXT abs_module_path[MAXPATHLEN];
	TEXT abs_path[MAXPATHLEN];
	TEXT path[MAXPATHLEN];
	TEXT *ib_ext_lib_path = 0;

	if (!(ib_ext_lib_path = getenv(ib_env_var))) {
		strncpy(resolved_module, module, length);
		return TRUE;		/* The variable is not defined.  Retrun TRUE */
	}

	if (EXPAND_PATH(module, abs_module)) {
		/* Extract the path from the absolute module name */
		for (p = abs_module, q = NULL; *p; p++)
			if ((*p == '\\') || (*p == '/'))
				q = p;

		memset(abs_module_path, 0, MAXPATHLEN);
		strncpy(abs_module_path, abs_module, q - abs_module);

		/* Check to see if the module path is in the lib path
		   if it is return TURE.  If it does not find it, then
		   the module path is not valid so return FALSE */
		token = strtok(ib_ext_lib_path, ";");
		while (token != NULL) {
			strcpy(path, token);
			/* make sure that there is no traing slash on the path */
			p = path + strlen(path);
			if ((p != path) && ((p[-1] == '/') || (p[-1] == '\\')))
				p[-1] = 0;
			if ((EXPAND_PATH(path, abs_path))
				&& (!COMPARE_PATH(abs_path, abs_module_path))) {
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
BOOLEAN API_ROUTINE gds__validate_lib_path(TEXT * module,
										   TEXT * ib_env_var,
										   TEXT * resolved_module,
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
		DOES.if (!
				 (ISC_expand_logical_once
				  (ib_env_var, strlen(ib_env_var) - 2, path))) return TRUE;

	if (ISC_expand_logical_once(module, strlen(module) - 2, abs_module)) {
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


SLONG API_ROUTINE gds__vax_integer(const UCHAR * ptr, SSHORT length)
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
	SLONG value;
	SSHORT shift;

	value = shift = 0;

	while (--length >= 0) {
		value += ((SLONG) * ptr++) << shift;
		shift += 8;
	}

	return value;
}


void API_ROUTINE gds__vtof(
						   SCHAR * string,
						   SCHAR * field, USHORT length)
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
		*field++ = *string++;
		if (--length <= 0)
			return;
	}

	if (length > 0)
		do
			*field++ = ' ';
		while (--length);
}


void API_ROUTINE gds__vtov(const SCHAR* string, char* field, SSHORT length)
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
 *	preprocessor.  Until gds__vtof, the target string
 *	is null terminated.
 *
 **************************************/

	--length;

	while ((*field++ = *string++) != 0)
		if (--length <= 0) {
			*field = 0;
			return;
		}
}


void API_ROUTINE isc_print_sqlerror(SSHORT sqlcode, ISC_STATUS * status)
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
	TEXT error_buffer[192], *p;

	sprintf(error_buffer, "SQLCODE: %d\nSQL ERROR:\n", sqlcode);
	for (p = error_buffer; *p;)
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
									TEXT * buffer, SSHORT length)
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
	TEXT *str = "";

	if (sqlcode < 0)
		gds__msg_format(0, 13, (USHORT) (1000 + sqlcode), length, buffer,
						str, str, str, str, str);
	else
		gds__msg_format(0, 14, sqlcode, length, buffer,
						str, str, str, str, str);
}


#ifdef VMS
int unlink(SCHAR * file)
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


static void blr_error(CTL control, TEXT * string, ...)
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

	VA_START(args, string);
	blr_format(control, string, args);
	offset = 0;
	PRINT_LINE;
	Firebird::status_exception::raise(-1);
}


static void blr_format(CTL control, const char * string, ...)
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

	VA_START(ptr, string);
	vsprintf(control->ctl_ptr, string, ptr);
	while (*control->ctl_ptr)
		control->ctl_ptr++;
}


static void blr_indent(CTL control, SSHORT level)
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



static void blr_print_blr(CTL control, UCHAR operator_)
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
	SCHAR *p;

	if (operator_ > FB_NELEM(blr_table) ||
		!(p = (SCHAR *) /* const_cast */ blr_table[operator_].blr_string))
		blr_error(control, "*** blr operator %d is undefined ***",
				  (int) operator_);

	blr_format(control, "blr_%s, ", p);
}


static SCHAR blr_print_byte(CTL control)
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
	UCHAR v;

	v = BLR_BYTE;
	blr_format(control, (control->ctl_language) ? "chr(%d), " : "%d, ",
			   (int) v);

	return v;
}


static SCHAR blr_print_char(CTL control)
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
	SSHORT printable;

	v = c = BLR_BYTE;
	printable = (c >= 'a' && c <= 'z') ||
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


static bool blr_print_cond(CTL control)
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

	USHORT ctype = BLR_BYTE;
	bool has_verb = false;

	switch (ctype) {
	case blr_gds_code:
		blr_format(control, "blr_gds_code, ");
		n = PRINT_BYTE;
		while (--n >= 0)
			PRINT_CHAR;
		break;

	case blr_exception:
		blr_format(control, "blr_exception, ");
		n = PRINT_BYTE;
		while (--n >= 0)
			PRINT_CHAR;
		break;

	case blr_sql_code:
		blr_format(control, "blr_sql_code, ");
		PRINT_WORD;
		break;

	case blr_default_code:
		blr_format(control, "blr_default_code, ");
		break;

	case blr_raise:
		blr_format(control, "blr_raise, ");
		break;

	case blr_exception_msg:
		blr_format(control, "blr_exception_msg, ");
		n = PRINT_BYTE;
		while (--n >= 0)
			PRINT_CHAR;
		has_verb = true;
		break;

	default:
		blr_error(control, "*** invalid condition type ***");
		break;
	}

	return has_verb;
}


static int blr_print_dtype(CTL control)
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
	USHORT dtype;
	SCHAR *string;
	SSHORT length;
	UCHAR v1, v2;

	dtype = BLR_BYTE;

/* Special case blob (261) to keep down the size of the
   jump table */

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
		string = "double";

		/* for double literal, return the length of the numeric string */

		v1 = *(control->ctl_blr);
		v2 = *(control->ctl_blr + 1);
		length = ((v2 << 8) | v1) + 2;
		break;

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
		length = PRINT_WORD;
		break;

	case blr_varying:
		length = PRINT_WORD + 2;
		break;

	case blr_short:
	case blr_long:
	case blr_quad:
	case blr_int64:
		PRINT_BYTE;
		break;

	case blr_text2:
		PRINT_WORD;
		length = PRINT_WORD;
		break;

	case blr_varying2:
		PRINT_WORD;
		length = PRINT_WORD + 2;
		break;

	case blr_cstring2:
		PRINT_WORD;
		length = PRINT_WORD;
		break;

	default:
		if (dtype == blr_cstring)
			length = PRINT_WORD;
		break;
	}

	return length;
}


static void blr_print_join(CTL control)
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
	USHORT join_type;
	SCHAR *string;

	join_type = BLR_BYTE;

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


static SLONG blr_print_line(CTL control, SSHORT offset)
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

#pragma FB_COMPILER_MESSAGE("Fix! Ugly function pointer cast!")

	((void (*)(...)) (*control->ctl_routine)) (control->ctl_user_arg, offset, control->ctl_buffer);
	control->ctl_ptr = control->ctl_buffer;

	return control->ctl_blr - control->ctl_blr_start;
}


static void blr_print_verb(CTL control, SSHORT level)
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
	SSHORT n, n2;
	SLONG offset;

	offset = control->ctl_blr - control->ctl_blr_start;
	blr_indent(control, level);
	UCHAR operator_ = BLR_BYTE;

	if ((SCHAR) operator_ == (SCHAR) blr_end) {
		blr_format(control, "blr_end, ");
		PRINT_LINE;
		return;
	}

	blr_print_blr(control, operator_);
	level++;
	const UCHAR *ops = blr_table[operator_].blr_operators;

	while (*ops)
		switch (*ops++) {
		case op_verb:
			PRINT_VERB;
			break;

		case op_line:
			offset = PRINT_LINE;
			break;

		case op_byte:
			n = PRINT_BYTE;
			break;

		case op_word:
			n = PRINT_WORD;
			break;

		case op_pad:
			PUT_BYTE(' ');
			break;

		case op_dtype:
			n = PRINT_DTYPE;
			break;

		case op_literal:
			while (--n >= 0)
				PRINT_CHAR;
			break;

		case op_join:
			PRINT_JOIN;
			break;

		case op_message:
			while (--n >= 0) {
				blr_indent(control, level);
				PRINT_DTYPE;
				offset = PRINT_LINE;
			}
			break;

		case op_parameters:
			level++;
			while (--n >= 0)
				PRINT_VERB;
			level--;
			break;

		case op_error_handler:
			while (--n >= 0) {
				blr_indent(control, level);
				PRINT_COND;
				offset = PRINT_LINE;
			}
			break;

		case op_set_error:
			blr_indent(control, level);
			if (PRINT_COND) {
				level++;
				offset = PRINT_LINE;
				PRINT_VERB;
				level--;
			}
			offset = PRINT_LINE;
			break;

		case op_indent:
			blr_indent(control, level);
			break;

		case op_begin:
			while ((SCHAR) * (control->ctl_blr) != (SCHAR) blr_end)
				PRINT_VERB;
			break;

		case op_map:
			while (--n >= 0) {
				blr_indent(control, level);
				PRINT_WORD;
				offset = PRINT_LINE;
				PRINT_VERB;
			}
			break;

		case op_args:
			while (--n >= 0)
				PRINT_VERB;
			break;

		case op_literals:
			while (--n >= 0) {
				blr_indent(control, level);
				n2 = PRINT_BYTE;
				while (--n2 >= 0)
					PRINT_CHAR;
				offset = PRINT_LINE;
			}
			break;

		case op_union:
			while (--n >= 0) {
				PRINT_VERB;
				PRINT_VERB;
			}
			break;

		case op_relation:
			operator_ = BLR_BYTE;
			blr_print_blr(control, operator_);
			if (operator_ != blr_relation && operator_ != blr_rid)
				blr_error(control,
						  "*** blr_relation or blr_rid must be object of blr_lock_relation, %d found ***",
						  (int) operator_);

			if (operator_ == blr_relation) {
				n = PRINT_BYTE;
				while (--n >= 0)
					PRINT_CHAR;
			}
			else
				PRINT_WORD;
			break;

		case op_exec_into:
			n = BLR_WORD /*e_exec_into_count*/ ;
			PRINT_VERB;
			if (! BLR_BYTE) {// ! singleton
				PRINT_VERB;
			}
			/*e_exec_into_list*/
			while (n-- > 0) {
				PRINT_VERB;
			}
			break;

		default:
			assert(FALSE);
			break;
		}
}


static int blr_print_word(CTL control)
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
	UCHAR v1, v2;

	v1 = BLR_BYTE;
	v2 = BLR_BYTE;
	blr_format(control,
			   (control->ctl_language) ? "chr(%d),chr(%d), " : "%d,%d, ",
			   (int) v1, (int) v2);

	return (v2 << 8) | v1;
}


static int blr_get_word(CTL control)
{
/**************************************
 *
 *	b l r _ g e t _ w o r d
 *
 **************************************
 *
 * Functional description
 *	Return a VAX word as a numeric value.
 *
 **************************************/
	UCHAR v1, v2;

	v1 = BLR_BYTE;
	v2 = BLR_BYTE;

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
	CLEAN clean;
	FPTR_VOID_PTR routine;
	void *arg;

#ifdef UNIX
	if (gds_pid != getpid())
		return;
#endif

	gds__msg_close(NULL);

	while (clean = cleanup_handlers) {
		cleanup_handlers = clean->clean_next;
		routine = clean->clean_routine;
		arg = clean->clean_arg;

		/* We must free the handler before calling it because there
		   may be a handler (and is) that frees all memory that has
		   been allocated. */

		FREE_LIB_MEMORY(clean);

#pragma FB_COMPILER_MESSAGE("Fix! Ugly function pointer cast!")

		((void (*)(void *)) (*routine)) (arg);
	}

#ifdef V4_THREADING
/* V4_DESTROY; */
#endif
	initialized = 0;
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
#if defined(VMS)
	ISC_STATUS status;
#endif

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
	status = sys$dclexh(&exit_description);
#endif

#ifdef UNIX
	gds_pid = getpid();
#ifdef SUPERSERVER
#if (defined SOLARIS || defined HPUX || defined LINUX)
	{
		/* Increase max open files to hard limit for Unix
		   platforms which are known to have low soft limits. */

		struct rlimit old, new_;

		if (!getrlimit(RLIMIT_NOFILE, &old) && old.rlim_cur < old.rlim_max) {
			new_.rlim_cur = new_.rlim_max = old.rlim_max;
			if (!setrlimit(RLIMIT_NOFILE, &new_))
			{
#if _FILE_OFFSET_BITS == 64 && defined SOLARIS
				gds__log("64 bit i/o support is on.");
				gds__log("Open file limit increased from %lld to %lld",
						 old.rlim_cur, new_.rlim_cur);
		       
#else
				gds__log("Open file limit increased from %d to %d",
						 old.rlim_cur, new_.rlim_cur);
#endif
			}
		}
	}
#endif
#endif /* SUPERSERVER */
#endif /* UNIX */

	atexit(gds__cleanup);

	initialized = 1;

    gdsPrefixInit();

#ifndef REQUESTER
	ISC_signal_init();
#endif

	/* V4_GLOBAL_MUTEX_UNLOCK; */
}


static int yday(struct tm *times)
{
/**************************************
 *
 *	y d a y
 *
 **************************************
 *
 * Functional description
 *	Convert a calendar date to the day-of-year.
 *
 *	The unix time structure considers
 *	january 1 to be Year day 0, although it
 *	is day 1 of the month.   (Note that QLI,
 *	when printing Year days takes the other
 *	view.)   
 *
 **************************************/
	SSHORT day, month, year;
	const BYTE *days;
	static const BYTE month_days[] =
		{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	day = times->tm_mday;
	month = times->tm_mon;
	year = times->tm_year + 1900;

	--day;

	for (days = month_days; days < month_days + month; days++)
		day += *days;

	if (month < 2)
		return day;

/* Add a day as we're past the leap-day */
	if (!(year % 4))
		day++;

/* Ooops - year's divisible by 100 aren't leap years */
	if (!(year % 100))
		day--;

/* Unless they are also divisible by 400! */
	if (!(year % 400))
		day++;

	return day;
}


static void ndate(SLONG nday, struct tm *times)
{
/**************************************
 *
 *	n d a t e
 *
 **************************************
 *
 * Functional description
 *	Convert a numeric day to [day, month, year].
 *
 * Calenders are divided into 4 year cycles.
 * 3 Non-Leap years, and 1 leap year.
 * Each cycle takes 365*4 + 1 == 1461 days.
 * There is a further cycle of 100 4 year cycles.
 * Every 100 years, the normally expected leap year
 * is not present.  Every 400 years it is.
 * This cycle takes 100 * 1461 - 3 == 146097 days
 * The origin of the constant 2400001 is unknown.
 * The origin of the constant 1721119 is unknown.
 * The difference between 2400001 and 1721119 is the
 * number of days From 0/0/0000 to our base date of
 * 11/xx/1858. (678882)
 * The origin of the constant 153 is unknown.
 *
 * This whole routine has problems with ndates
 * less than -678882 (Approx 2/1/0000).
 *
 **************************************/
	SLONG year, month, day;
	SLONG century;

	nday -= 1721119 - 2400001;
	century = (4 * nday - 1) / 146097;
	nday = 4 * nday - 1 - 146097 * century;
	day = nday / 4;

	nday = (4 * day + 3) / 1461;
	day = 4 * day + 3 - 1461 * nday;
	day = (day + 4) / 4;

	month = (5 * day - 3) / 153;
	day = 5 * day - 3 - 153 * month;
	day = (day + 5) / 5;

	year = 100 * century + nday;

	if (month < 10)
		month += 3;
	else {
		month -= 9;
		year += 1;
	}

	times->tm_mday = (int) day;
	times->tm_mon = (int) month - 1;
	times->tm_year = (int) year - 1900;
}


static GDS_DATE nday(struct tm *times)
{
/**************************************
 *
 *	n d a y
 *
 **************************************
 *
 * Functional description
 *	Convert a calendar date to a numeric day
 *	(the number of days since the base date).
 *
 **************************************/
	SSHORT day, month, year;
	SLONG c, ya;

	day = times->tm_mday;
	month = times->tm_mon + 1;
	year = times->tm_year + 1900;

	if (month > 2)
		month -= 3;
	else {
		month += 9;
		year -= 1;
	}

	c = year / 100;
	ya = year - 100 * c;

	return (GDS_DATE) (((SINT64) 146097 * c) / 4 +
					   (1461 * ya) / 4 +
					   (153 * month + 2) / 5 + day + 1721119 - 2400001);
}


static void sanitize(TEXT * locale)
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
 *  resultString must be at least MAXPATHLEN size.
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
	assert(alen >= 0);
	memcpy(&resultString[len], appendString, alen);
	resultString[len + alen] = 0;
}

#undef gds__alloc

void* API_ROUTINE gds__alloc(SLONG size_request)
{
	return getDefaultMemoryPool()->allocate(size_request, 0
#ifdef DEBUG_GDS_ALLOC
		, __FILE__, __LINE__
#endif
	);
}

} // extern "C"

class InitPrefix
{
private:
	static size_t copyTo(const Firebird::string& from, char* to, size_t toSize)
	{
		fb_assert(to);
		fb_assert(toSize);
		if (--toSize > from.length())
		{
			toSize = from.length();
		}
		memcpy(to, from.c_str(), toSize);
		to[toSize] = 0;
		return toSize;
	}
public:
	static void init()
	{
		// Get ib_prefix value from config file
#ifdef BOOT_BUILD
		Firebird::string prefix(getenv(FB_ENV) ? getenv(FB_ENV) : "");
#else
		Firebird::string prefix(Config::getRootDirectory());
#endif
		if (prefix.length() == 0)
		{
			prefix = FB_PREFIX;
		}
		copyTo(prefix, ib_prefix_val, sizeof(ib_prefix_val));
		ib_prefix = ib_prefix_val;

		// Find appropiate temp directory
		const char* tempDir = getenv(FB_TMP_ENV);
		if (!tempDir) 
		{
#ifdef WIN_NT
			const DWORD len = GetTempPath(sizeof(fbTempDir), fbTempDir);
			// This checks "TEMP" and "TMP" environment variables
			if (len && len < sizeof(fbTempDir))
			{
				tempDir = fbTempDir;
			}
#else
			tempDir = getenv("TMP");
#endif
		}
		if (!tempDir || strlen(tempDir) >= MAXPATHLEN)
		{
			tempDir = WORKFILE;
		}
		strcpy(fbTempDir, tempDir);

#ifdef EMBEDDED
		// Generate filename based on the current PID
		Firebird::string buf;
		buf.printf(FB_PID_FILE, getpid());
		copyTo(buf, fbEmbeddedRoot, sizeof(fbEmbeddedRoot));
#endif

		// Find appropriate Firebird lock file prefix
		// Override conditional defines with the enviroment
		// variable FIREBIRD_LOCK if it is set.
		Firebird::string lockPrefix(getenv(FB_LOCK_ENV) ? getenv(FB_LOCK_ENV) : "");
		if (lockPrefix.length() == 0) 
		{
#ifdef EMBEDDED
			lockPrefix = tempDir;
#else
			lockPrefix = prefix;
#endif
		}
		copyTo(lockPrefix, ib_prefix_lock_val, sizeof(ib_prefix_lock_val));
		ib_prefix_lock = ib_prefix_lock_val;

		// Find appropriate Firebird message file prefix.
		Firebird::string msgPrefix(getenv(FB_MSG_ENV) ? getenv(FB_MSG_ENV) : "");
		if (msgPrefix.length() == 0) 
		{
			msgPrefix = prefix;
		}
		copyTo(msgPrefix, ib_prefix_msg_val, sizeof(ib_prefix_msg_val));
		ib_prefix_msg = ib_prefix_msg_val;
	}
	static void cleanup()
	{
	}
};

static Firebird::InitMutex<InitPrefix> initPrefix;

static void gdsPrefixInit()
{
/**************************************
 *
 *	g d s P r e f i x I n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize all data in various ib_prefixes.
 *	Calling it before any signal can be caught (from init())
 *	makes gds__prefix* family of functions signal-safe.
 *	In order not to break external API, call to gdsPrefixInit
 *	must be present in all gds__prefix functions. Due to correct
 *	init this doesn't make them unsafe.
 *
 **************************************/
	initPrefix.init();
}
