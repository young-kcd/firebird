/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		utl.cpp
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
 * 2001.06.14 Claudio Valderrama: isc_set_path() will append slash if missing,
 *            so ISC_PATH environment variable won't fail for this cause.
 * 2002.02.15 Sean Leyne - Code Cleanup is required of obsolete "EPSON", "XENIX" ports
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "Apollo" port
 * 23-Feb-2002 Dmitry Yemanov - Events wildcarding
 *
 * 2002.10.27 Sean Leyne - Completed removal of obsolete "DG_X86" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "UNIXWARE" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix/MIPS" port
 *
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "MPEXL" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#include "firebird.h"
#include <limits.h>
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <string.h>
#include "../jrd/common.h"
#include "../jrd/license.h"
#include <stdarg.h>
#include "../jrd/jrd_time.h"
#include "../jrd/misc.h"
#include "../jrd/gdsassert.h"

#include "../jrd/y_ref.h"
#include "../jrd/ibase.h"
#include "../jrd/msg.h"
#include "../jrd/event.h"
#include "../jrd/gds_proto.h"
#include "../jrd/utl_proto.h"
#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
#include "../jrd/blb_proto.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef VMS
#include <file.h>
#include <ib_perror.h>
#include <descrip.h>
#include <types.h>
#include <stat.h>

#else /* !VMS */

#include <sys/types.h>
#include <sys/stat.h>

#if defined(WIN_NT)
#include <io.h>
#include <process.h>
#else
#include <sys/file.h>
#endif

#endif /* VMS */

#define statistics		stat

#ifdef UNIX
#define GDS_EDIT	gds__edit
#endif

#if defined(WIN_NT)
#define GDS_EDIT	gds__edit
#endif

#ifdef VMS
#ifdef __ALPHA
#define EDT_IMAGE	"TPUSHR"
#define EDT_SYMBOL	"TPU$EDIT"
#else
#define EDT_IMAGE	"EDTSHR"
#define EDT_SYMBOL	"EDT$EDIT"
#endif
#endif

/* Bug 7119 - BLOB_load will open external file for read in BINARY mode. */

#ifdef WIN_NT
#define FOPEN_READ_TYPE		"rb"
#define FOPEN_WRITE_TYPE	"wb"
#define FOPEN_READ_TYPE_TEXT	"rt"
#define FOPEN_WRITE_TYPE_TEXT	"wt"
#endif

#ifndef FOPEN_READ_TYPE
#define FOPEN_READ_TYPE		"r"
#endif

#ifndef FOPEN_WRITE_TYPE
#define FOPEN_WRITE_TYPE	"w"
#endif

#ifndef FOPEN_READ_TYPE_TEXT
#define FOPEN_READ_TYPE_TEXT	FOPEN_READ_TYPE
#endif

#ifndef FOPEN_WRITE_TYPE_TEXT
#define FOPEN_WRITE_TYPE_TEXT	FOPEN_WRITE_TYPE
#endif

#define LOWER7(c) ( (c >= 'A' && c<= 'Z') ? c + 'a' - 'A': c )


/* Blob stream stuff */

#define BSTR_input	0
#define BSTR_output	1
#define BSTR_alloc	2

static int dump(GDS_QUAD *, FRBRD *, FRBRD *, IB_FILE *);
static int edit(GDS_QUAD *, FRBRD *, FRBRD *, SSHORT, SCHAR *);
static int get_ods_version(FRBRD **, USHORT *, USHORT *);
static void isc_expand_dpb_internal(const UCHAR** dpb, SSHORT* dpb_size, ...);
static int load(GDS_QUAD *, FRBRD *, FRBRD *, IB_FILE *);


#ifdef VMS
static int display(GDS_QUAD *, int *, int *);
#endif

/* Blob info stuff */

static const char blob_items[] = {
	isc_info_blob_max_segment, isc_info_blob_num_segments,
	isc_info_blob_total_length
};


/* gds__version stuff */

static const char info[] =
	{ isc_info_firebird_version, isc_info_implementation, isc_info_end };

static const char ods_info[] =
	{ isc_info_ods_version, isc_info_ods_minor_version, isc_info_end };

static const TEXT* const impl_class[] = {
	NULL,						/* 0 */
	"access method",			/* 1 */
	"Y-valve",					/* 2 */
	"remote interface",			/* 3 */
	"remote server",			/* 4 */
	NULL,						/* 5 */
	NULL,						/* 6 */
	"pipe interface",			/* 7 */
	"pipe server",				/* 8 */
	"central interface",		/* 9 */
	"central server",			/* 10 */
	"gateway",					/* 11 */
	"classic server",		/* 12 */
	"super server"			/* 13 */
};

static const TEXT* const impl_implementation[] = {
	NULL,						/* 0 */
    "Rdb/VMS",				/* 1 */
    "Rdb/ELN target",		/* 2 */
    "Rdb/ELN development",	/* 3 */
    "Rdb/VMS Y",			/* 4 */
    "Rdb/ELN Y",			/* 5 */
    "JRI",					/* 6 */
    "JSV",					/* 7 */
    NULL,					/* 8 */
    NULL,					/* 9 */
    NULL, // "Firebird/apollo",     /* 10 */
    NULL, // "Firebird/ultrix",		/* 11 */
    "Firebird/vms",		/* 12 */
    "Firebird/sun",		/* 13 */
    NULL, // "Firebird/OS2",        /* 14 */
    NULL,					/* 15 */
    NULL,					/* 16 */
    NULL,					/* 17 */
    NULL,					/* 18 */
    NULL,					/* 19 */
    NULL,					/* 20 */
    NULL,					/* 21 */
    NULL,					/* 22 */
    NULL,					/* 23 */
    NULL,					/* 24 */
    NULL, // "Firebird/apollo",     /* 25 */
    NULL, // "Firebird/ultrix",		/* 26 */
    "Firebird/vms",		/* 27 */
    "Firebird/sun",		/* 28 */
    NULL, // "Firebird/OS2",        /* 29 */
    "Firebird/sun4",		/* 30 */
    NULL, // "Firebird/hpux800",	/* 31 */
    "Firebird/sun386",		/* 32 */
    "Firebird:ORACLE/vms",	/* 33 */
    NULL, // "Firebird/mac/aux",    /* 34 */
    "Firebird/ibm/aix",	/* 35 */
    NULL, // "Firebird/mips/ultrix",	/* 36 */
    NULL, // "Firebird/xenix",      /* 37 */
    NULL, // "Firebird/AViiON",		/* 38 */
    NULL, // "Firebird/hp/mpexl",	/* 39 */
    NULL, // "Firebird/hp/ux300",	/* 40 */
    NULL, // "Firebird/sgi",		/* 41 */
    "Firebird/sco/unix",	/* 42 */
    NULL, // "Firebird/Cray",       /* 43 */
    NULL, // "Firebird/imp",        /* 44 */
    NULL, // "Firebird/delta",      /* 45 */
    NULL, // "Firebird/NeXT",       /* 46 */
    NULL, // "Firebird/DOS",		/* 47 */
    NULL, // "Firebird/m88k",       /* 48 */
    NULL, // "Firebird/UNIXWARE",	/* 49 */
    "Firebird/x86/Windows NT",	/* 50 */
    NULL, // "Firebird/epson",      /* 51 */
    NULL, // "Firebird/DEC/OSF",	/* 52 */
    "Firebird/Alpha/OpenVMS",	/* 53 */
    NULL, // "Firebird/NetWare",	/* 54 */
    "Firebird/Windows",	/* 55 */
    NULL, // "Firebird/NCR3000",    /* 56 */
    NULL, // "Firebird/PPC/Windows NT", /* 57 */
    NULL, // "Firebird/DG_X86",		/* 58 */
    "Firebird/SCO_SV Intel",	/* 59 *//* 5.5 SCO Port */
    "Firebird/linux Intel",	/* 60 */
    "Firebird/FreeBSD/i386",	/* 61 */
    "Firebird/NetBSD/i386",	/* 62 */
    "Firebird/Darwin/PowerPC",	/* 63 */
    "Firebird/SINIX-Z",	/* 64 */
    "Firebird/linux Sparc"	/* 65 */
};


#ifdef VMS
ISC_STATUS API_ROUTINE gds__attach_database_d(
										  ISC_STATUS * user_status,
										  struct dsc$descriptor_s *file_name,
										  void **handle,
										  SSHORT dpb_length,
	const SCHAR* dpb, SSHORT db_type)
{
/**************************************
 *
 *	g d s _ $ a t t a c h _ d a t a b a s e _ d
 *
 **************************************
 *
 * Functional description
 *	An attach database for COBOL to call
 *
 **************************************/

	return gds_attach_database(user_status, file_name->dsc$w_length,
							   file_name->dsc$a_pointer, handle, dpb_length,
							   dpb, db_type);
}
#endif


int API_ROUTINE gds__blob_size(
							   FRBRD ** b,
							   SLONG * size,
							   SLONG * seg_count, SLONG * max_seg)
{
/**************************************
 *
 *	g d s _ $ b l o b _ s i z e
 *
 **************************************
 *
 * Functional description
 *	Get the size, number of segments, and max
 *	segment length of a blob.  Return true
 *	if it happens to succeed.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	SLONG n;
	SSHORT l;
	SCHAR *p, item, buffer[64];

#pragma FB_COMPILER_MESSAGE("Fix! Bad casts.")

	if (isc_blob_info(status_vector, b, sizeof(blob_items),
					   blob_items,
					   sizeof(buffer), buffer)) {
		isc_print_status(status_vector);
		return FALSE;
	}

	p = buffer;

	while ((item = *p++) != isc_info_end) {
		l =
			static_cast<SSHORT>
			(gds__vax_integer(reinterpret_cast<UCHAR*>(p), 2));
		p += 2;
		n = gds__vax_integer(reinterpret_cast<UCHAR*>(p), l);
		p += l;
		switch (item) {
		case isc_info_blob_max_segment:
			if (max_seg)
				*max_seg = n;
			break;

		case isc_info_blob_num_segments:
			if (seg_count)
				*seg_count = n;
			break;

		case isc_info_blob_total_length:
			if (size)
				*size = n;
			break;

		default:
			return FALSE;
		}
	}

	return TRUE;
}


/* 17 May 2001 - isc_expand_dpb is DEPRECATED */
void API_ROUTINE_VARARG isc_expand_dpb(SCHAR** dpb, SSHORT* dpb_size, ...)
{
/**************************************
 *
 *	i s c _ e x p a n d _ d p b
 *
 **************************************
 *
 * Functional description
 *	Extend a database parameter block dynamically
 *	to include runtime info.  Generated
 *	by gpre to provide host variable support for
 *	READY statement	options.
 *	This expects the list of variable args
 *	to be zero terminated.
 *
 *	Note: dpb_size is signed short only for compatibility
 *	with other calls (isc_attach_database) that take a dpb length.
 *
 * TMN: Note: According to Ann Harrison:
 * That routine should be deprecated.  It doesn't do what it
 * should, and does do things it shouldn't, and is harder to
 * use than the natural alternative.
 *
 **************************************/
	SSHORT	length;
	char*	p = 0;
	const char*	q;
	va_list	args;
	USHORT	type;
	UCHAR* new_dpb;

/* calculate length of database parameter block,
   setting initial length to include version */

   	SSHORT new_dpb_length;
	if (!*dpb || !(new_dpb_length = *dpb_size))
	{
		new_dpb_length = 1;
	}

	VA_START(args, dpb_size);

	while (type = va_arg(args, int))
	{
		switch (type)
		{
		case isc_dpb_user_name:
		case isc_dpb_password:
		case isc_dpb_sql_role_name:
		case isc_dpb_lc_messages:
		case isc_dpb_lc_ctype:
		case isc_dpb_reserved:
			q = va_arg(args, char*);
			if (q)
			{
				length = strlen(q);
				new_dpb_length += 2 + length;
			}
			break;

		default:
			va_arg(args, int);
			break;
		}
	}

/* if items have been added, allocate space
   for the new dpb and copy the old one over */

	if (new_dpb_length > *dpb_size)
	{
		/* Note: gds__free done by GPRE generated code */

		new_dpb = (UCHAR*)gds__alloc((SLONG)(sizeof(UCHAR) * new_dpb_length));
		p = reinterpret_cast<char*>(new_dpb);
		/* FREE: done by client process in GPRE generated code */
		if (!new_dpb)
		{			/* NOMEM: don't trash existing dpb */
			DEV_REPORT("isc_extend_dpb: out of memory");
			return;				/* NOMEM: not really handled */
		}

		q = *dpb;
		for (length = *dpb_size; length; length--)
		{
			*p++ = *q++;
		}

	}
	else
	{
	    // CVC: Notice this case is new_dpb_length <= *dpb_size, but since
	    // we have new_dpb_length = MAX(*dpb_size, 1) our case is reduced
	    // to new_dpb_length == *dpb_size. Therefore, this code is a waste
	    // of time, since the function didn't find any param to add and thus,
		// the loop below won't find anything worth adding either.
	    // Notice, too that the original input dpb is used, yet the pointer "p"
	    // is positioned exactly at the end, so if something was added at the
		// tail, it would be a memory failure, unless the caller lies and is
		// always passing a dpb bigger than *dpb_size.
		new_dpb = reinterpret_cast<UCHAR*>(*dpb);
		p = reinterpret_cast<char*>(new_dpb + *dpb_size);
	}

	if (!*dpb_size)
		*p++ = isc_dpb_version1;

/* copy in the new runtime items */

	VA_START(args, dpb_size);

	while (type = va_arg(args, int))
	{
		switch (type)
		{
		case isc_dpb_user_name:
		case isc_dpb_password:
		case isc_dpb_sql_role_name:
		case isc_dpb_lc_messages:
		case isc_dpb_lc_ctype:
		case isc_dpb_reserved:
			q = va_arg(args, char*);
			if (q)
			{
				length = strlen(q);
				fb_assert(type <= CHAR_MAX);
				*p++ = (char) type;
				fb_assert(length <= CHAR_MAX);
				*p++ = (char) length;
				while (length--)
					*p++ = *q++;
			}
			break;

		default:
			va_arg(args, int);
			break;
		}
	}

	*dpb_size = p - reinterpret_cast<char*>(new_dpb);
	*dpb = reinterpret_cast<SCHAR*>(new_dpb);
}


int API_ROUTINE isc_modify_dpb(SCHAR**	dpb,
							   SSHORT*	dpb_size,
							   USHORT	type,
							   const SCHAR*	str,
							   SSHORT	str_len)
{
/**************************************
 *
 *	i s c _ m o d i f y _ d p b
 *
 **************************************
 * CVC: This is exactly the same login than isc_expand_dpb, but for one param.
 * However, the difference is that when presented with a dpb type it that's
 * unknown, it returns FB_FAILURE immediately. In contrast, isc_expand_dpb
 * doesn't complain and instead treats those as integers and tries to skip
 * them, hoping to sync in the next iteration.
 *
 * Functional description
 *	Extend a database parameter block dynamically
 *	to include runtime info.  Generated
 *	by gpre to provide host variable support for
 *	READY statement	options.
 *	This expects one arg at a time.
 *      the length of the string is passed by the caller and hence
 * 	is not expected to be null terminated.
 * 	this call is a variation of isc_expand_dpb without a variable
 * 	arg parameters.
 * 	Instead, this function is called recursively
 *	Alternatively, this can have a parameter list with all possible
 *	parameters either nulled or with proper value and type.
 *
 *  	**** This can be modified to be so at a later date, making sure
 *	**** all callers follow the same convention
 *
 *	Note: dpb_size is signed short only for compatibility
 *	with other calls (isc_attach_database) that take a dpb length.
 *
 **************************************/

/* calculate length of database parameter block,
   setting initial length to include version */

	SSHORT new_dpb_length;
	if (!*dpb || !(new_dpb_length = *dpb_size))
	{
		new_dpb_length = 1;
	}

	switch (type)
	{
	case isc_dpb_user_name:
	case isc_dpb_password:
	case isc_dpb_sql_role_name:
	case isc_dpb_lc_messages:
	case isc_dpb_lc_ctype:
	case isc_dpb_reserved:
		new_dpb_length += 2 + str_len;
		break;

	default:
		return FB_FAILURE;
	}

/* if items have been added, allocate space
   for the new dpb and copy the old one over */

	UCHAR* new_dpb;
	UCHAR* p;
	if (new_dpb_length > *dpb_size)
	{
		/* Note: gds__free done by GPRE generated code */

		new_dpb = (UCHAR*)gds__alloc((SLONG)(sizeof(UCHAR) * new_dpb_length));

		/* FREE: done by client process in GPRE generated code */
		if (!new_dpb)
		{			/* NOMEM: don't trash existing dpb */
			DEV_REPORT("isc_extend_dpb: out of memory");
			return FB_FAILURE;		/* NOMEM: not really handled */
		}

		p = new_dpb;
		const UCHAR* q = reinterpret_cast<const UCHAR*>(*dpb);
		for (SSHORT length = *dpb_size; length; length--)
		{
			*p++ = *q++;
		}

	}
	else
	{
		new_dpb = reinterpret_cast<UCHAR*>(*dpb);
		p = new_dpb + *dpb_size;
	}

	if (!*dpb_size)
	{
		*p++ = isc_dpb_version1;
	}

/* copy in the new runtime items */

	switch (type)
	{
	case isc_dpb_user_name:
	case isc_dpb_password:
	case isc_dpb_sql_role_name:
	case isc_dpb_lc_messages:
	case isc_dpb_lc_ctype:
	case isc_dpb_reserved:
		{
		    const UCHAR* q = reinterpret_cast<const UCHAR*>(str);
			if (q)
			{
				SSHORT length = str_len;
				fb_assert(type <= MAX_UCHAR);
				*p++ = (UCHAR) type;
				fb_assert(length <= MAX_UCHAR);
				*p++ = (UCHAR) length;
				while (length--)
				{
					*p++ = *q++;
				}
			}
			break;
		}

	default:
		return FB_FAILURE;
	}

	*dpb_size = p - new_dpb;
	*dpb = (SCHAR *) new_dpb;

	return FB_SUCCESS;
}


#ifdef GDS_EDIT
int API_ROUTINE GDS_EDIT(TEXT* file_name, USHORT type)
{
/**************************************
 *
 *	g d s _ $ e d i t
 *
 **************************************
 *
 * Functional description
 *	Edit a file.
 *
 **************************************/
	TEXT *editor, buffer[256];
	struct stat before, after;

#ifndef WIN_NT
	if (!(editor = getenv("VISUAL")) && !(editor = getenv("EDITOR")))
		editor = "vi";
#else
	if (!(editor = getenv("EDITOR")))
		editor = "Notepad";
#endif

	statistics(file_name, &before);
	sprintf(buffer, "%s %s", editor, file_name);

	system(buffer);

	statistics(file_name, &after);

	return (before.st_mtime != after.st_mtime ||
			before.st_size != after.st_size);
}
#endif


#ifdef VMS
int API_ROUTINE gds__edit(TEXT * file_name, USHORT type)
{
/**************************************
 *
 *	g d s _ $ e d i t
 *
 **************************************
 *
 * Functional description
 *	Open a blob, dump it to a file, allow the user to edit the
 *	window, and dump the data back into a blob.  If the user
 *	bails out, return FALSE, otherwise return TRUE.
 *
 **************************************/
	int status, (*editor) ();
	struct dsc$descriptor_s desc, symbol, image;
	struct stat before, after;

	stat(file_name, &before);
	ISC_make_desc(file_name, &desc, 0);
	ISC_make_desc(EDT_SYMBOL, &symbol, 0);
	ISC_make_desc(EDT_IMAGE, &image, 0);

	lib$find_image_symbol(&image, &symbol, &editor);
	status = (*editor) (&desc, &desc);
	stat(file_name, &after);

	return (before.st_ctime != after.st_ctime ||
			before.st_ino[0] != after.st_ino[0] ||
			before.st_ino[1] != after.st_ino[1] ||
			before.st_ino[2] != after.st_ino[2]);
}
#endif


SLONG API_ROUTINE gds__event_block(SCHAR ** event_buffer,
								   SCHAR ** result_buffer, USHORT count, ...)
{
/**************************************
 *
 *	g d s _ $ e v e n t _ b l o c k
 *
 **************************************
 *
 * Functional description
 *	Create an initialized event parameter block from a
 *	variable number of input arguments.
 *	Return the size of the block.
 *
 *	Return 0 as the size if the event parameter block cannot be
 *	created for any reason.
 *
 **************************************/
	SCHAR *p, *q;
	SCHAR *end;
	SLONG length;
	va_list ptr;
	USHORT i;

	VA_START(ptr, count);

/* calculate length of event parameter block, 
   setting initial length to include version
   and counts for each argument */

	length = 1;
	i = count;
	while (i--) {
		q = va_arg(ptr, SCHAR *);
		length += strlen(q) + 5;
	}

	p = *event_buffer =
		(SCHAR *) gds__alloc((SLONG) (sizeof(SCHAR) * length));
/* FREE: unknown */
	if (!*event_buffer)			/* NOMEM: */
		return 0;
	*result_buffer = (SCHAR *) gds__alloc((SLONG) (sizeof(SCHAR) * length));
/* FREE: unknown */
	if (!*result_buffer) {		/* NOMEM: */
		gds__free(*event_buffer);
		*event_buffer = NULL;
		return 0;
	}

#ifdef DEBUG_GDS_ALLOC
/* I can't find anywhere these items are freed */
/* 1994-October-25 David Schnepper  */
	gds_alloc_flag_unfreed((void *) *event_buffer);
	gds_alloc_flag_unfreed((void *) *result_buffer);
#endif /* DEBUG_GDS_ALLOC */

/* initialize the block with event names and counts */

	*p++ = EPB_version1;

	VA_START(ptr, count);

	i = count;
	while (i--) {
		q = va_arg(ptr, SCHAR *);

		/* Strip trailing blanks from string */

		for (end = q + strlen(q); --end >= q && *end == ' ';);
		*p++ = end - q + 1;
		while (q <= end)
			*p++ = *q++;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}

	return p - *event_buffer;
}


USHORT API_ROUTINE gds__event_block_a(SCHAR ** event_buffer,
									  SCHAR ** result_buffer,
									  SSHORT count,
									  SCHAR ** name_buffer)
{
/**************************************
 *
 *	g d s _ $ e v e n t _ b l o c k _ a 
 *
 **************************************
 *
 * Functional description
 *	Create an initialized event parameter block from a
 *	vector of input arguments. (Ada needs this)
 *	Assume all strings are 31 characters long.
 *	Return the size of the block.
 *
 **************************************/
	SCHAR *p, *q;
	SCHAR *end, **nb;
	SLONG length;
	USHORT i;

/* calculate length of event parameter block, 
   setting initial length to include version
   and counts for each argument */

	i = count;
	nb = name_buffer;
	length = 0;
	while (i--) {
		q = *nb++;

		/* Strip trailing blanks from string */

		for (end = q + 31; --end >= q && *end == ' ';);
		length += end - q + 1 + 5;
	}

	i = count;
	p = *event_buffer =
		(SCHAR *) gds__alloc((SLONG) (sizeof(SCHAR) * length));
/* FREE: unknown */
	if (!*event_buffer)			/* NOMEM: */
		return 0;
	*result_buffer = (SCHAR *) gds__alloc((SLONG) (sizeof(SCHAR) * length));
/* FREE: unknown */
	if (!*result_buffer) {		/* NOMEM: */
		gds__free(*event_buffer);
		*event_buffer = NULL;
		return 0;
	}

#ifdef DEBUG_GDS_ALLOC
/* I can't find anywhere these items are freed */
/* 1994-October-25 David Schnepper  */
	gds_alloc_flag_unfreed((void *) *event_buffer);
	gds_alloc_flag_unfreed((void *) *result_buffer);
#endif /* DEBUG_GDS_ALLOC */

	*p++ = EPB_version1;

	nb = name_buffer;

	while (i--) {
		q = *nb++;

		/* Strip trailing blanks from string */

		for (end = q + 31; --end >= q && *end == ' ';);
		*p++ = end - q + 1;
		while (q <= end)
			*p++ = *q++;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
		*p++ = 0;
	}

	return (p - *event_buffer);
}


void API_ROUTINE gds__event_block_s(
									SCHAR ** event_buffer,
									SCHAR ** result_buffer,
									SSHORT count,
SCHAR ** name_buffer, SSHORT * return_count)
{
/**************************************
 *
 *	g d s _ $ e v e n t _ b l o c k _ s
 *
 **************************************
 *
 * Functional description
 *	THIS IS THE COBOL VERSION of gds__event_block_a for Cobols
 *	that don't permit return values.
 *
 **************************************/

	*return_count =
		gds__event_block_a(event_buffer, result_buffer, count, name_buffer);
}


void API_ROUTINE isc_event_counts(
					ULONG * result_vector,
					SSHORT buffer_length,
					SCHAR * event_buffer,
					SCHAR * result_buffer)
{
/**************************************
 *
 *	g d s _ $ e v e n t _ c o u n t s
 *
 **************************************
 *
 * Functional description
 *	Get the delta between two events in an event
 *	parameter block.  Used to update gds_events
 *	for GPRE support of events.
 *
 **************************************/
	SCHAR *p, *q, *end;
	USHORT i, length;
	ULONG *vec;
	ULONG initial_count, new_count;

	vec = result_vector;
	p = event_buffer;
	q = result_buffer;
	length = buffer_length;
	end = p + length;

/* analyze the event blocks, getting the delta for each event */

	p++;
	q++;
	while (p < end) {
		/* skip over the event name */

		i = (USHORT) * p++;
		p += i;
		q += i + 1;

		/* get the change in count */

		initial_count =
			gds__vax_integer(reinterpret_cast<UCHAR*>(p), sizeof(SLONG));
		p += sizeof(SLONG);
		new_count =
			gds__vax_integer(reinterpret_cast<UCHAR*>(q), sizeof(SLONG));
		q += sizeof(SLONG);
		*vec++ = new_count - initial_count;
	}

/* copy over the result to the initial block to prepare
   for the next call to gds__event_wait */

	p = event_buffer;
	q = result_buffer;
	do
		*p++ = *q++;
	while (--length);
}


void API_ROUTINE isc_get_client_version(SCHAR *buffer)
{
/**************************************
 *
 *	g d s _ $ g e t _ c l i e n t _ v e r s i o n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	if (buffer)
		strcpy(buffer, FB_VERSION);
}


int API_ROUTINE isc_get_client_major_version()
{
/**************************************
 *
 *	g d s _ $ g e t _ c l i e n t _ m a j o r _ v e r s i o n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return atoi(FB_MAJOR_VER);
}


int API_ROUTINE isc_get_client_minor_version()
{
/**************************************
 *
 *	g d s _ $ g e t _ c l i e n t _ m i n o r _ v e r s i o n
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	return atoi(FB_MINOR_VER);
}


void API_ROUTINE gds__map_blobs(int *handle1, int *handle2)
{
/**************************************
 *
 *	g d s _ $ m a p _ b l o b s
 *
 **************************************
 *
 * Functional description
 *	Map an old blob to a new blob.
 *	This call is intended for use by REPLAY,
 *	and is probably not generally useful
 *	for anyone else.
 *
 **************************************/

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
#ifndef SUPERCLIENT
/* Note: gds__map_blobs is almost like an API call,
   it needs a TDBB structure setup for it in order
   to function properly.  This currently does
   not function.
   1996-Nov-06 David Schnepper  */
	deliberate_compile_error++;
	BLB_map_blobs(NULL, handle1, handle2);
#endif
#endif
}


#if !(defined REQUESTER)
void API_ROUTINE isc_set_debug(int value)
{
/**************************************
 *
 *	G D S _ S E T _ D E B U G
 *
 **************************************
 *
 * Functional description
 *	Set debugging mode, whatever that may mean.
 *
 **************************************/

#pragma FB_COMPILER_MESSAGE("Empty function?!")
}
#endif


void API_ROUTINE isc_set_login(const UCHAR** dpb, SSHORT* dpb_size)
{
/**************************************
 *
 *	i s c _ s e t _ l o g i n
 *
 **************************************
 *
 * Functional description
 *	Pickup any ISC_USER and ISC_PASSWORD
 *	environment variables, and stuff them
 *	into the dpb (if there is no user name
 *	or password already referenced).
 *
 **************************************/
#ifndef SUPERSERVER

/* look for the environment variables */

	const TEXT* username = getenv("ISC_USER");
	const TEXT* password = getenv("ISC_PASSWORD");

	if (!username && !password)
		return;

/* figure out whether the username or 
   password have already been specified */
   
	bool user_seen = false, password_seen = false;

	if (*dpb && *dpb_size) {
	    const UCHAR* p = *dpb;
		for (const UCHAR* const end_dpb = p + *dpb_size; p < end_dpb;) {
			const int item = *p++;

			if (item == isc_dpb_version1)
				continue;

			switch (item) {
			case isc_dpb_sys_user_name:
			case isc_dpb_user_name:
				user_seen = true;
				break;

			case isc_dpb_password:
			case isc_dpb_password_enc:
				password_seen = true;
				break;
			}

			/* get the length and increment past the parameter. */
			const USHORT l = *p++;
			p += l;
		}
	}

	if (username && !user_seen) {
		if (password && !password_seen)
			isc_expand_dpb_internal(dpb, dpb_size,
						   isc_dpb_user_name, username, isc_dpb_password,
						   password, 0);
		else
			isc_expand_dpb_internal(dpb, dpb_size,
						   isc_dpb_user_name, username, 0);
	}
	else if (password && !password_seen)
		isc_expand_dpb_internal(dpb, dpb_size,
					   isc_dpb_password, password, 0);
#endif
}


BOOLEAN API_ROUTINE isc_set_path(TEXT* file_name,
								 USHORT file_length, TEXT* expanded_name)
{
/**************************************
 *
 *	i s c _ s e t _ p a t h
 *
 **************************************
 *
 * Functional description
 *	Set a prefix to a filename based on 
 *	the ISC_PATH user variable.
 *
 **************************************/
	TEXT *pathname, *p;

/* look for the environment variables to tack 
   onto the beginning of the database path */

	if (!(pathname = getenv("ISC_PATH")))
		return FALSE;

	if (!file_length)
		file_length = strlen(file_name);
	file_name[file_length] = 0;

/* if the file already contains a remote node
   or any path at all forget it */

	for (p = file_name; *p; p++)
		if (*p == ':' || *p == '/' || *p == '\\')
			return FALSE;

/* concatenate the strings */

	strcpy(expanded_name, pathname);

    /* CVC: Make the concatenation work if no slash is present. */
    p = expanded_name + (strlen (expanded_name) - 1);
    if (*p != ':' && *p != '/' && *p != '\\') {
        strcat(expanded_name, "/");
    }

	strcat(expanded_name, file_name);

	return TRUE;
}


void API_ROUTINE isc_set_single_user(const UCHAR** dpb,
									 SSHORT* dpb_size, const TEXT* single_user)
{
/****************************************
 *
 *      i s c _ s e t _ s i n g l e _ u s e r
 *
 ****************************************
 *
 * Functional description
 *      Stuff the single_user flag into the dpb
 *      if the flag doesn't already exist in the dpb.
 *
 ****************************************/

/* Discover if single user access has already been specified */

	bool single_user_seen = false;

	if ((*dpb) && (*dpb_size)) {
		const UCHAR* p = *dpb;
		for (const UCHAR* const end_dpb = p + *dpb_size; p < end_dpb;) {

			const int item = *p++;

			if (item == isc_dpb_version1)
				continue;

			switch (item) {

			case isc_dpb_reserved:

				single_user_seen = true;
				break;

			}

/* Get the length and increment past the parameter. */

			const USHORT l = *p++;
			p += l;

		}
	}

	if (!single_user_seen)
		isc_expand_dpb_internal(dpb, dpb_size,
					   isc_dpb_reserved, single_user, 0);

}


int API_ROUTINE isc_version(FRBRD** handle,
							 FPTR_VOID routine, void* user_arg)
{
/**************************************
 *
 *	g d s _ $ v e r s i o n
 *
 **************************************
 *
 * Functional description
 *	Obtain and print information about a database.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	USHORT buf_len, len, implementation, class_, ods_version,
		ods_minor_version;
	UCHAR count, item, l, *buf, buffer[256], *p;
	TEXT *versions, *implementations,
		*class_string, *implementation_string, s[128];
	BOOLEAN redo;

	if (!routine) {
		routine = (FPTR_VOID) ib_printf;
		user_arg = (void*)"\t%s\n";
	}

	buf = buffer;
	buf_len = sizeof(buffer);

	do {
		if (isc_database_info(status_vector,
							  handle,
							  sizeof(info),
							  info,
							  buf_len, reinterpret_cast<char*>(buf))) {
			if (buf != buffer)
				gds__free((SLONG *) buf);
			return FB_FAILURE;
		}

		p = buf;
		redo = FALSE;

		while (!redo && *p != isc_info_end && p < buf + buf_len) {
			item = *p++;
			len = static_cast<USHORT>(gds__vax_integer(p, 2));
			p += 2;
			switch (item) {
			case isc_info_firebird_version:
				versions = (TEXT *) p;
				break;

			case isc_info_implementation:
				implementations = (TEXT *) p;
				break;

			case isc_info_truncated:
				redo = TRUE;
				break;

			default:
				if (buf != buffer)
					gds__free((SLONG *) buf);
				return FB_FAILURE;
			}
			p += len;
		}

		/* Our buffer wasn't large enough to hold all the information,
		 * make a larger one and try again.
		 */
		if (redo) {
			if (buf != buffer)
				gds__free((SLONG *) buf);
			buf_len += 1024;
			buf = (UCHAR *) gds__alloc((SLONG) (sizeof(UCHAR) * buf_len));
			/* FREE: freed within this module */
			if (!buf)			/* NOMEM: */
				return FB_FAILURE;
		}
	} while (redo);

	count = MIN(*versions, *implementations);
	++versions;
	++implementations;

	while (count-- > 0) {
		implementation = *implementations++;
		class_ = *implementations++;
		l = *versions++;
		if (implementation >= FB_NELEM(impl_implementation)
			|| !(implementation_string =
				 const_cast<char*>(impl_implementation[implementation])))
			implementation_string = "**unknown**";
		if (class_ >= FB_NELEM(impl_class) ||
			!(class_string = const_cast<char*>(impl_class[class_])))
			class_string = "**unknown**";
		sprintf(s, "%s (%s), version \"%.*s\"",
				implementation_string, class_string, l, versions);

#pragma FB_COMPILER_MESSAGE("Fix! Bad function ptr type cast!")
		reinterpret_cast<void (*)(...)>(*routine)(user_arg, s);
		versions += l;
	}

	if (buf != buffer)
		gds__free((SLONG *) buf);

	if (get_ods_version(handle, &ods_version, &ods_minor_version) == FB_FAILURE)
		return FB_FAILURE;

	sprintf(s, "on disk structure version %d.%d", ods_version,
			ods_minor_version);
	reinterpret_cast<void (*)(...)>(*routine)(user_arg, s);

	return FB_SUCCESS;
}


void API_ROUTINE isc_format_implementation(
										   USHORT implementation,
										   USHORT ibuflen,
										   TEXT * ibuf,
USHORT class_, USHORT cbuflen, TEXT * cbuf)
{
/**************************************
 *
 *	i s c _ f o r m a t _ i m p l e m e n t a t i o n
 *
 **************************************
 *
 * Functional description
 *	Convert the implementation and class codes to strings
 * 	by looking up their values in the internal tables.
 *
 **************************************/
	int len;

	if (ibuflen > 0) {
		if (implementation >= FB_NELEM(impl_implementation) ||
			!(impl_implementation[implementation])) {
			strncpy(ibuf, "**unknown**", ibuflen - 1);
			ibuf[MIN(11, ibuflen - 1)] = '\0';
		}
		else {
			strncpy(ibuf, impl_implementation[implementation], ibuflen - 1);
			len = strlen(impl_implementation[implementation]);
			ibuf[MIN(len, ibuflen - 1)] = '\0';
		}
	}

	if (cbuflen > 0) {
		if (class_ >= FB_NELEM(impl_class) || !(impl_class[class_])) {
			strncpy(cbuf, "**unknown**", cbuflen - 1);
			ibuf[MIN(11, cbuflen - 1)] = '\0';
		}
		else {
			strncpy(cbuf, impl_class[class_], cbuflen - 1);
			len = strlen(impl_class[class_]);
			ibuf[MIN(len, cbuflen - 1)] = '\0';
		}
	}

}


U_IPTR API_ROUTINE isc_baddress(SCHAR * object)
{
/**************************************
 *
 *        i s c _ b a d d r e s s
 *
 **************************************
 *
 * Functional description
 *      Return the address of whatever is passed in
 *
 **************************************/

	return (U_IPTR) object;
}


void API_ROUTINE isc_baddress_s(SCHAR * object, U_IPTR * address)
{
/**************************************
 *
 *        i s c _ b a d d r e s s _ s
 *
 **************************************
 *
 * Functional description
 *      Return the address of whatever is passed in
 *
 **************************************/

	*address = (U_IPTR) object;
}


#ifdef VMS
void API_ROUTINE gds__wake_init(void)
{
/**************************************
 *
 *	g d s _ $ w a k e _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Set up to be awakened by another process thru a blocking AST.
 *
 **************************************/

	ISC_wake_init();
}
#endif


int API_ROUTINE BLOB_close(BSTREAM * bstream)
{
/**************************************
 *
 *	B L O B _ c l o s e
 *
 **************************************
 *
 * Functional description
 *	Close a blob stream.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	USHORT l;

	if (!bstream->bstr_blob)
		return FALSE;

	if (bstream->bstr_mode & BSTR_output) {
		l = (bstream->bstr_ptr - bstream->bstr_buffer);
		if (l > 0)
			if (isc_put_segment(status_vector, &bstream->bstr_blob, l,
								 bstream->bstr_buffer)) 
			{
				return FALSE;
			}
	}

	isc_close_blob(status_vector, &bstream->bstr_blob);

	if (bstream->bstr_mode & BSTR_alloc)
		gds__free(bstream->bstr_buffer);

	gds__free(bstream);

	return TRUE;
}


int API_ROUTINE blob__display(
							  SLONG blob_id[2],
							  FRBRD **database,
							  FRBRD **transaction,
							  TEXT * field_name, SSHORT * name_length)
{
/**************************************
 *
 *	b l o b _ $ d i s p l a y
 *
 **************************************
 *
 * Functional description
 *	PASCAL callable version of EDIT_blob.
 *
 **************************************/
	TEXT *p, *q, temp[32];
	USHORT l;

	if ((l = *name_length) != 0) {
		p = temp;
		q = field_name;
		do
			*p++ = *q++;
		while (--l);
		*p = 0;
	}

	return BLOB_display(reinterpret_cast<GDS_QUAD*>(blob_id), *database,
						*transaction, temp);
}


int API_ROUTINE BLOB_display(
							 GDS_QUAD * blob_id,
							 FRBRD *database,
							 FRBRD *transaction, TEXT * field_name)
{
/**************************************
 *
 *	B L O B _ d i s p l a y
 *
 **************************************
 *
 * Functional description
 *	Open a blob, dump it to a file, allow the user to read the
 *	window. 
 *
 **************************************/

/* On VMS use the system library routines to do the output */

#ifdef VMS
	return display(blob_id, database, transaction);
#else

/* On UNIX, just dump the file to ib_stdout */

	return dump(blob_id, database, transaction, ib_stdout);

#endif
}


int API_ROUTINE blob__dump(
						   SLONG blob_id[2],
						   FRBRD **database,
						   FRBRD **transaction,
						   TEXT * file_name, SSHORT * name_length)
{
/**************************************
 *
 *	b l o b _ $ d u m p
 *
 **************************************
 *
 * Functional description
 *	Translate a pascal callable dump
 *	into an internal dump call.
 *
 **************************************/
	TEXT *p, *q, temp[129];
	USHORT l;

	if ((l = *name_length) != 0) {
		p = temp;
		q = file_name;
		do
			*p++ = *q++;
		while (--l);
		*p = 0;
	}

	return BLOB_dump(reinterpret_cast<GDS_QUAD*>(blob_id), *database,
					 *transaction, temp);
}


int API_ROUTINE BLOB_text_dump(
							   GDS_QUAD * blob_id,
							   FRBRD *database,
							   FRBRD *transaction, SCHAR * file_name)
{
/**************************************
 *
 *	B L O B _ t e x t _ d u m p
 *
 **************************************
 *
 * Functional description
 *	Dump a blob into a file.
 *      This call does CR/LF translation on NT.
 *
 **************************************/
	IB_FILE *file;
	int ret;

	if (!(file = ib_fopen(file_name, FOPEN_WRITE_TYPE_TEXT)))
		return FALSE;

	ret = dump(blob_id, database, transaction, file);
	ib_fclose(file);

	return ret;
}


int API_ROUTINE BLOB_dump(
						  GDS_QUAD * blob_id,
						  FRBRD *database,
						  FRBRD *transaction, SCHAR * file_name)
{
/**************************************
 *
 *	B L O B _ d u m p
 *
 **************************************
 *
 * Functional description
 *	Dump a blob into a file.
 *
 **************************************/
	IB_FILE *file;
	int ret;

	if (!(file = ib_fopen(file_name, FOPEN_WRITE_TYPE)))
		return FALSE;

	ret = dump(blob_id, database, transaction, file);
	ib_fclose(file);

	return ret;
}


int API_ROUTINE blob__edit(
						   SLONG blob_id[2],
						   FRBRD **database,
						   FRBRD **transaction,
						   TEXT * field_name, SSHORT * name_length)
{
/**************************************
 *
 *	b l o b _ $ e d i t
 *
 **************************************
 *
 * Functional description
 *	Translate a pascal callable edit
 *	into an internal edit call.
 *
 **************************************/
	TEXT *p, *q, temp[32];
	USHORT l;

	if ((l = *name_length) != 0) {
		p = temp;
		q = field_name;
		do
			*p++ = *q++;
		while (--l);
		*p = 0;
	}

	return BLOB_edit(reinterpret_cast<GDS_QUAD*>(blob_id), *database,
					 *transaction, temp);
}


int API_ROUTINE BLOB_edit(
						  GDS_QUAD * blob_id,
						  FRBRD *database,
						  FRBRD *transaction, SCHAR * field_name)
{
/**************************************
 *
 *	B L O B _ e d i t
 *
 **************************************
 *
 * Functional description
 *	Open a blob, dump it to a file, allow the user to edit the
 *	window, and dump the data back into a blob.  If the user
 *	bails out, return FALSE, otherwise return TRUE.
 *
 **************************************/

	return edit(blob_id, database, transaction, TRUE, field_name);
}


int API_ROUTINE BLOB_get(BSTREAM * bstream)
{
/**************************************
 *
 *	B L O B _ g e t
 *
 **************************************
 *
 * Functional description
 *	Return the next byte of a blob.  If the blob is exhausted, return
 *	EOF.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;

	if (!bstream->bstr_buffer)
		return EOF;

	while (1) {
		if (--bstream->bstr_cnt >= 0)
			return *bstream->bstr_ptr++ & 0377;

		isc_get_segment(status_vector, &bstream->bstr_blob,
						 reinterpret_cast<USHORT*>(&bstream->bstr_cnt),
						 bstream->bstr_length, bstream->bstr_buffer);
		if (status_vector[1] && status_vector[1] != isc_segment) {
			bstream->bstr_ptr = 0;
			bstream->bstr_cnt = 0;
			if (status_vector[1] != isc_segstr_eof)
				isc_print_status(status_vector);
			return EOF;
		}
		bstream->bstr_ptr = bstream->bstr_buffer;
	}
}


int API_ROUTINE blob__load(
						   SLONG blob_id[2],
						   FRBRD **database,
						   FRBRD **transaction,
						   TEXT * file_name, SSHORT * name_length)
{
/**************************************
 *
 *	b l o b _ $ l o a d
 *
 **************************************
 *
 * Functional description
 *	Translate a pascal callable load
 *	into an internal load call.
 *
 **************************************/
	TEXT *p, *q, temp[129];
	USHORT l;

	if ((l = *name_length) != 0) {
		p = temp;
		q = file_name;
		do
			*p++ = *q++;
		while (--l);
		*p = 0;
	}

	return BLOB_load(reinterpret_cast<GDS_QUAD*>(blob_id), *database,
					 *transaction, temp);
}


int API_ROUTINE BLOB_text_load(
							   GDS_QUAD * blob_id,
							   FRBRD *database,
							   FRBRD *transaction, TEXT * file_name)
{
/**************************************
 *
 *	B L O B _ t e x t _ l o a d
 *
 **************************************
 *
 * Functional description
 *	Load a  blob with the contents of a file.  
 *      This call does CR/LF translation on NT.
 *      Return TRUE is successful.
 *
 **************************************/
	IB_FILE *file;
	int ret;

	if (!(file = ib_fopen(file_name, FOPEN_READ_TYPE_TEXT)))
		return FALSE;

	ret = load(blob_id, database, transaction, file);

	ib_fclose(file);

	return ret;
}


int API_ROUTINE BLOB_load(
						  GDS_QUAD * blob_id,
						  FRBRD *database, FRBRD *transaction, TEXT * file_name)
{
/**************************************
 *
 *	B L O B _ l o a d
 *
 **************************************
 *
 * Functional description
 *	Load a blob with the contents of a file.  Return TRUE is successful.
 *
 **************************************/
	IB_FILE *file;
	int ret;

	if (!(file = ib_fopen(file_name, FOPEN_READ_TYPE)))
		return FALSE;

	ret = load(blob_id, database, transaction, file);

	ib_fclose(file);

	return ret;
}


BSTREAM* API_ROUTINE Bopen(GDS_QUAD* blob_id,
						   FRBRD* database, FRBRD* transaction,
						   const SCHAR* mode)
{
/**************************************
 *
 *	B o p e n
 *
 **************************************
 *
 * Functional description
 *	Initialize a blob-stream block.
 *
 **************************************/
	FRBRD *blob;
	ISC_STATUS_ARRAY status_vector;
	BSTREAM *bstream;
	USHORT bpb_length;
	UCHAR *bpb;

	// bpb is irrelevant, not used.
	bpb_length = 0;
	bpb = NULL;

	blob = NULL;

	if (*mode == 'w' || *mode == 'W') {
		if (isc_create_blob2(status_vector, &database, &transaction, &blob,
							  blob_id, bpb_length,
							  reinterpret_cast<const char*>(bpb)))
		{
			return NULL;
		}
	}
	else if (*mode == 'r' || *mode == 'R') {
		if (isc_open_blob2(status_vector, &database, &transaction, &blob,
							blob_id, bpb_length,
							bpb))
		{
			return NULL;
		}
	}
	else
		return NULL;

	bstream = BLOB_open(blob, NULL, 0);

	if (*mode == 'w' || *mode == 'W') {
		bstream->bstr_mode |= BSTR_output;
		bstream->bstr_cnt = bstream->bstr_length;
		bstream->bstr_ptr = bstream->bstr_buffer;
	}
	else {
		bstream->bstr_cnt = 0;
		bstream->bstr_mode |= BSTR_input;
	}

	return bstream;
}


BSTREAM* API_ROUTINE BLOB_open(FRBRD *blob, SCHAR * buffer, int length)
{
/**************************************
 *
 *	B L O B _ o p e n
 *
 **************************************
 *
 * Functional description
 *	Initialize a blob-stream block.
 *
 **************************************/
	BSTREAM *bstream;

	if (!blob)
		return NULL;

	bstream = (BSTREAM *) gds__alloc((SLONG) sizeof(BSTREAM));
/* FREE: This structure is freed by BLOB_close */
	if (!bstream)				/* NOMEM: */
		return NULL;

#ifdef DEBUG_gds__alloc
/* This structure is handed to the user process, we depend on the client
 * to call BLOB_close() for it to be freed.
 */
	gds_alloc_flag_unfreed((void *) bstream);
#endif

	bstream->bstr_blob = blob;
	bstream->bstr_length = (length) ? length : 512;
	bstream->bstr_mode = 0;
	bstream->bstr_cnt = 0;
	bstream->bstr_ptr = 0;

	if (!(bstream->bstr_buffer = buffer)) {
		bstream->bstr_buffer = (SCHAR *)
			gds__alloc((SLONG) (sizeof(SCHAR) * bstream->bstr_length));
		/* FREE: This structure is freed in BLOB_close() */
		if (!bstream->bstr_buffer) {	/* NOMEM: */
			gds__free(bstream);
			return NULL;
		}
#ifdef DEBUG_gds__alloc
		/* This structure is handed to the user process, we depend on the client
		 * to call BLOB_close() for it to be freed.
		 */
		gds_alloc_flag_unfreed((void *) bstream->bstr_buffer);
#endif

		bstream->bstr_mode |= BSTR_alloc;
	}

	return bstream;
}


int API_ROUTINE BLOB_put(SCHAR x, BSTREAM * bstream)
{
/**************************************
 *
 *	B L O B _ p u t
 *
 **************************************
 *
 * Functional description
 *	Write a segment to a blob. First
 *	stick in the final character, then
 *	compute the length and send off the
 *	segment.  Finally, set up the buffer
 *	block and retun TRUE if all is well. 
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	USHORT l;

	if (!bstream->bstr_buffer)
		return FALSE;

	*bstream->bstr_ptr++ = (x & 0377);
	l = (bstream->bstr_ptr - bstream->bstr_buffer);
	if (isc_put_segment(status_vector, &bstream->bstr_blob,
						 l, bstream->bstr_buffer)) {
		return FALSE;
	}
	bstream->bstr_cnt = bstream->bstr_length;
	bstream->bstr_ptr = bstream->bstr_buffer;
	return TRUE;
}


#ifdef VMS
static display(GDS_QUAD * blob_id, void *database, void *transaction)
{
/**************************************
 *
 *	d i s p l a y
 *
 **************************************
 *
 * Functional description
 *	Display a file on VMS
 *
 **************************************/
	SCHAR buffer[256], *p;
	SSHORT short_length, l;
	ISC_STATUS_ARRAY status_vector;
	int *blob;
	struct dsc$descriptor_s desc;

/* Open the blob.  If it failed, what the hell -- just return failure */

	blob = NULL;
	if (isc_open_blob(status_vector, &database, &transaction, &blob, blob_id)) {
		isc_print_status(status_vector);
		return FALSE;
	}

/* Copy data from blob to scratch file */

	short_length = sizeof(buffer);

	for (;;) {
		isc_get_segment(status_vector, &blob, &l, short_length, buffer);
		if (status_vector[1] && status_vector[1] != isc_segment) {
			if (status_vector[1] != isc_segstr_eof)
				isc_print_status(status_vector);
			break;
		}
		buffer[l] = 0;
		ISC_make_desc(buffer, &desc, 0);
		lib$put_output(&desc);
	}

/* Close the blob */

	isc_close_blob(status_vector, &blob);

	return TRUE;
}
#endif /* VMS */


static int dump(
				GDS_QUAD * blob_id,
				FRBRD *database, FRBRD *transaction, IB_FILE * file)
{
/**************************************
 *
 *	d u m p
 *
 **************************************
 *
 * Functional description
 *	Dump a blob into a file.
 *
 **************************************/
	SCHAR buffer[256], *p;
	SSHORT short_length, l;
	ISC_STATUS_ARRAY status_vector;
	FRBRD *blob;
	USHORT bpb_length;
	UCHAR *bpb;

	// bpb is irrelevant, not used.
	bpb_length = 0;
	bpb = NULL;

/* Open the blob.  If it failed, what the hell -- just return failure */

	blob = NULL;
	if (isc_open_blob2(status_vector, &database, &transaction, &blob, blob_id,
						bpb_length, bpb)) {
		isc_print_status(status_vector);
		return FALSE;
	}

/* Copy data from blob to scratch file */

	short_length = sizeof(buffer);

	for (;;) {
		isc_get_segment(status_vector, &blob,
						 reinterpret_cast<USHORT*>(&l),
						 short_length, buffer);
		if (status_vector[1] && status_vector[1] != isc_segment) {
			if (status_vector[1] != isc_segstr_eof)
				isc_print_status(status_vector);
			break;
		}
		p = buffer;
		if (l)
			do
				ib_fputc(*p++, file);
			while (--l);
	}

/* Close the blob */

	isc_close_blob(status_vector, &blob);

	return TRUE;
}


static int edit(
				GDS_QUAD * blob_id,
				FRBRD *database,
				FRBRD *transaction, SSHORT type, SCHAR * field_name)
{
/**************************************
 *
 *	e d i t
 *
 **************************************
 *
 * Functional description
 *	Open a blob, dump it to a file, allow the user to edit the
 *	window, and dump the data back into a blob.  If the user
 *	bails out, return FALSE, otherwise return TRUE.
 *
 *	If the field name coming in is too big, truncate it.
 *
 **************************************/
	TEXT file_name[50], *p, *q;
#if (defined WIN_NT)
	TEXT buffer[9];
#else
	TEXT buffer[25];
#endif
	IB_FILE *file;
#ifdef HAVE_MKSTEMP
	int fd;
#endif

	if (!(q = field_name))
		q = "gds_edit";

	for (p = buffer; *q && p < buffer + sizeof(buffer) - 1; q++)
		if (*q == '$')
			*p++ = '_';
		else
			*p++ = LOWER7(*q);
	*p = 0;

/* Moved this out of #ifndef mpexl to get mktemp/mkstemp to work for Linux
   This has been done in the inprise tree some days ago.
   Would have saved me a lot of time, if I had seen this earlier :-(
   FSG 15.Oct.2000
*/
	sprintf(file_name, "%sXXXXXX", buffer);

#ifdef HAVE_MKSTEMP
	fd = mkstemp(file_name);
	if (!(file = fdopen(fd, "w+"))) {
		close(fd);
		return FALSE;
	}
#else
	if (mktemp(file_name) == (char *)0)
		return FALSE;
	if (!(file = ib_fopen(file_name, FOPEN_WRITE_TYPE)))
		return FALSE;
	ib_fclose(file);

	if (!(file = ib_fopen(file_name, FOPEN_WRITE_TYPE_TEXT)))
		return FALSE;
#endif

	if (!dump(blob_id, database, transaction, file)) {
		ib_fclose(file);
		unlink(file_name);
		return FALSE;
	}

	ib_fclose(file);

	if (type = gds__edit(file_name, type)) {

		if (!(file = ib_fopen(file_name, FOPEN_READ_TYPE_TEXT))) {
			unlink(file_name);
			return FALSE;
		}

		load(blob_id, database, transaction, file);

		ib_fclose(file);

	}

	unlink(file_name);

	return type;
}


static int get_ods_version(
						   FRBRD** handle,
						   USHORT* ods_version, USHORT* ods_minor_version)
{
/**************************************
 *
 *	g e t _ o d s _ v e r s i o n
 *
 **************************************
 *
 * Functional description
 *	Obtain information about a on-disk structure (ods) versions
 *	of the database.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	USHORT n, l;
	UCHAR item, buffer[16], *p;

	isc_database_info(status_vector,
					  handle,
					  sizeof(ods_info),
					  ods_info,
					  sizeof(buffer), reinterpret_cast<char*>(buffer));

	if (status_vector[1])
		return FB_FAILURE;

	p = buffer;

	while ((item = *p++) != isc_info_end) {
		l = static_cast<USHORT>(gds__vax_integer(p, 2));
		p += 2;
		n = static_cast<USHORT>(gds__vax_integer(p, l));
		p += l;
		switch (item) {
		case isc_info_ods_version:
			*ods_version = n;
			break;

		case isc_info_ods_minor_version:
			*ods_minor_version = n;
			break;

		default:
			return FB_FAILURE;
		}
	}

	return FB_SUCCESS;
}


// CVC: I just made this alternative function to let the original unchanged.
// However, the original logic doesn't make sense.
static void isc_expand_dpb_internal(const UCHAR** dpb, SSHORT* dpb_size, ...)
{
/**************************************
 *
 *	i s c _ e x p a n d _ d p b _ i n t e r n a l
 *
 **************************************
 *
 * Functional description
 *	Extend a database parameter block dynamically
 *	to include runtime info.  Generated
 *	by gpre to provide host variable support for
 *	READY statement	options.
 *	This expects the list of variable args
 *	to be zero terminated.
 *
 *	Note: dpb_size is signed short only for compatibility
 *	with other calls (isc_attach_database) that take a dpb length.
 *
 * TMN: Note: According to Ann Harrison:
 * That routine should be deprecated.  It doesn't do what it
 * should, and does do things it shouldn't, and is harder to
 * use than the natural alternative.
 *
 * CVC: This alternative version returns either the original dpb or a
 * new one, but never overwrites the original dpb. More accurately, it's
 * clearer than the original function that really never modifies its source
 * dpb, but there appears to be a logic failure on an impossible path.
 * Also, since the change from UCHAR** to const UCHAR** is not transparent,
 * a new version was needed to make sure the old world wasn't broken.
 *
 **************************************/
	SSHORT	length;
	unsigned char* p = 0;
	const char* q;
	const unsigned char* uq;
	va_list	args;
	USHORT	type;
	UCHAR* new_dpb;

/* calculate length of database parameter block,
   setting initial length to include version */

	SSHORT new_dpb_length;
	if (!*dpb || !(new_dpb_length = *dpb_size))
	{
		new_dpb_length = 1;
	}

	va_start(args, dpb_size);

	while (type = va_arg(args, int))
	{
		switch (type)
		{
		case isc_dpb_user_name:
		case isc_dpb_password:
		case isc_dpb_sql_role_name:
		case isc_dpb_lc_messages:
		case isc_dpb_lc_ctype:
		case isc_dpb_reserved:
			q = va_arg(args, char*);
			if (q)
			{
				length = strlen(q);
				new_dpb_length += 2 + length;
			}
			break;

		default:
			va_arg(args, int);
			break;
		}
	}
	va_end(args);

/* if items have been added, allocate space
   for the new dpb and copy the old one over */

	if (new_dpb_length > *dpb_size)
	{
		/* Note: gds__free done by GPRE generated code */

		new_dpb = (UCHAR*)gds__alloc((SLONG)(sizeof(UCHAR) * new_dpb_length));
		p = new_dpb;
		/* FREE: done by client process in GPRE generated code */
		if (!new_dpb)
		{			/* NOMEM: don't trash existing dpb */
			DEV_REPORT("isc_extend_dpb: out of memory");
			return;				/* NOMEM: not really handled */
		}

		uq = *dpb;
		for (length = *dpb_size; length; length--)
		{
			*p++ = *uq++;
		}

	}
	else
	{
		// CVC: Notice the initialization is: new_dpb_length = *dpb_size
		// Therefore, the worst case is new_dpb_length == *dpb_size
		// Also, if *dpb_size == 0, then new_dpb_length is set to 1,
		// so there will be again a bigger new buffer.
		// Hence, this else just means "we found nothing that we can
		// recognize in the variable params list to add and thus,
		// there's nothing to do". The case for new_dpb_length being less
		// than the original length simply can't happen. Therefore,
		// the input can be declared const.
		return;
	}

	if (!*dpb_size)
		*p++ = isc_dpb_version1;

/* copy in the new runtime items */

	va_start(args, dpb_size);

	while (type = va_arg(args, int))
	{
		switch (type)
		{
		case isc_dpb_user_name:
		case isc_dpb_password:
		case isc_dpb_sql_role_name:
		case isc_dpb_lc_messages:
		case isc_dpb_lc_ctype:
		case isc_dpb_reserved:
		    q = va_arg(args, char*);
			if (q)
			{
				length = strlen(q);
				fb_assert(type <= CHAR_MAX);
				*p++ = (unsigned char) type;
				fb_assert(length <= CHAR_MAX);
				*p++ = (unsigned char) length;
				while (length--)
					*p++ = *q++;
			}
			break;

		default:
			va_arg(args, int);
			break;
		}
	}
	va_end(args);

	*dpb_size = p - new_dpb;
	*dpb = new_dpb;
}


static int load(
				GDS_QUAD * blob_id,
				FRBRD *database, FRBRD *transaction, IB_FILE * file)
{
/**************************************
 *
 *     l o a d  
 *
 **************************************
 *
 * Functional description
 *      Load a blob from a file.
 *
 **************************************/
	TEXT buffer[512], *p, *buffer_end;
	SSHORT l, c;
	ISC_STATUS_ARRAY status_vector;
	FRBRD *blob;

/* Open the blob.  If it failed, what the hell -- just return failure */

	blob = NULL;
	if (isc_create_blob(status_vector, &database, &transaction, &blob,
						 blob_id)) {
		isc_print_status(status_vector);
		return FALSE;
	}

/* Copy data from file to blob.  Make up boundaries at end of of line. */

	p = buffer;
	buffer_end = buffer + sizeof(buffer);

	for (;;) {
		c = ib_fgetc(file);
		if (ib_feof(file))
			break;
		*p++ = static_cast<TEXT>(c);
		if ((c != '\n') && p < buffer_end)
			continue;
		l = p - buffer;
		if (isc_put_segment(status_vector, &blob, l, buffer)) {
			isc_print_status(status_vector);
			isc_close_blob(status_vector, &blob);
			return FALSE;
		}
		p = buffer;
	}

	if ((l = p - buffer) != 0)
		if (isc_put_segment(status_vector, &blob, l, buffer)) {
			isc_print_status(status_vector);
			isc_close_blob(status_vector, &blob);
			return FALSE;
		}

	isc_close_blob(status_vector, &blob);

	return TRUE;
}

