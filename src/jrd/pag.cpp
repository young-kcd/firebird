/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		pag.c
 *	DESCRIPTION:	Page level ods manager
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
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete ports:
 *                          - EPSON, DELTA, IMP, NCR3000, M88K
 *                          - HP9000 s300 and Apollo
 *
 * 2002.10.27 Sean Leyne - Completed removal of obsolete "DG_X86" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "UNIXWARE" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix" port
 * 2002.10.27 Sean Leyne - Code Cleanup, removed obsolete "Ultrix/MIPS" port
 *
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "DGUX" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "MPEXL" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "DecOSF" port
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "SGI" port
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

/*
 * Modified by: Patrick J. P. Griffin
 * Date: 11/29/2000
 * Problem:   Bug 116733 Too many generators corrupt database.
 *            DPM_gen_id was not calculating page and offset correctly.
 * Change:    Caculate pgc_gpg, number of generators per page,
 *            for use in DPM_gen_id.
 */

/* 2001.07.06 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
 *                         conditionals, as the engine now fully supports
 *                         readonly databases.
 *
 * 2001.08.07 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
 *                         conditionals, second attempt
 *
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "MAC" and "MAC_CP" defines
 * 2002.02.15 Sean Leyne - Code Cleanup, removed obsolete "Apollo" port
 *
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <string.h>

#include "../jrd/jrd.h"
#include "../jrd/pag.h"
#include "../jrd/ods.h"
#include "../jrd/pio.h"
#include "../jrd/gds.h"
#include "../jrd/gdsassert.h"
#include "../jrd/license.h"
#include "../jrd/jrn.h"
#include "../jrd/lck.h"
#include "../jrd/sdw.h"
#include "../jrd/cch.h"
#include "../jrd/tra.h"
#include "../jrd/llio.h"
#ifdef VIO_DEBUG
#include "../jrd/vio_debug.h"
#include "../jrd/all.h"
#endif
#include "../jrd/all_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/dpm_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/lck_proto.h"
#include "../jrd/llio_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/pio_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/ail.h"
#include "../jrd/isc_f_proto.h"

extern "C" {

static void find_clump_space(SLONG, WIN *, PAG *, USHORT, SSHORT, UCHAR *,
							 USHORT);
static BOOLEAN find_type(SLONG, WIN *, PAG *, USHORT, USHORT, UCHAR **,
						 UCHAR **);

#define ERR_POST_IF_DATABASE_IS_READONLY(dbb)	{if (dbb->dbb_flags & DBB_read_only) ERR_post (isc_read_only_database, 0);}

/*  This macro enables the ability of the engine to connect to databases
 *  from ODS 8 up to the latest.  If this macro is undefined, the engine
 *  only opens a database of the current ODS major version.
 */
#define ODS_8_TO_CURRENT

// Class definitions (obsolete platforms are commented out)
// Class constant name consists of OS platform and CPU architecture.
//
// For ports created before Firebird 2.0 release 64-bit and 32-bit 
// sub-architectures of the same CPU should use different classes.
// For 64-bit ports first created after or as a part of Firebird 2.0
// release CPU architecture may be the same for both variants.

static const int CLASS_UNKNOWN = 0;
//static const CLASS_APOLLO_68K = 1;  // Apollo 68K, Dn 10K
static const int CLASS_SOLARIS_SPARC = 2; // Sun 68k, Sun Sparc, HP 9000/300, MAC AUX, IMP, DELTA, NeXT, UNIXWARE, DG_X86
static const int CLASS_SOLARIS_I386 = 3;  // Sun 386i
//static const CLASS_VMS_VAX = 4;     // VMS/VAX
//static const CLASS_ULTRIX_VAX = 5;  // Ultrix/VAX
//static const CLASS_ULTRIX_MIPS = 6; // Ultrix/MIPS
static const int CLASS_HPUX_PA = 7;		  // HP-UX on PA-RISC (was: HP 900/800 (precision))
static const int CLASS_NETWARE_I386 = 8;  // NetWare
//static const CLASS_MAC_OS = 9;	  // MAC-OS
static const int CLASS_AIX_PPC = 10;	  // AIX on PowerPC platform (was: IBM RS/6000)
//static const CLASS_DG_AVIION = 11;  // DG AViiON
//static const CLASS_MPE_XL = 12;	  // MPE/XL
static const int CLASS_IRIX_MIPS = 13;	  // Silicon Grpahics/IRIS
static const int CLASS_CRAY = 14;		  // Cray
static const int CLASS_TRU64_ALPHA = 15;  // Tru64 Unix running on Alpha (was: Dec OSF/1)
static const int CLASS_WINDOWS_I386 = 16; // NT -- post 4.0 (delivered 4.0 as class 8)
//static const CLASS_OS2 = 17;		  // OS/2
//static const CLASS_WIN16 = 18;	  // Windows 16 bit
static const int CLASS_LINUX_I386 = 19;   // LINUX on Intel series
static const int CLASS_LINUX_SPARC = 20;  // LINUX on sparc systems
static const int CLASS_FREEBSD_I386 = 21; // FreeBSD/i386
static const int CLASS_NETBSD_I386 = 22;  // NetBSD/i386
static const int CLASS_DARWIN_PPC = 23;   // Darwin/PowerPC
static const int CLASS_LINUX_AMD64 = 24;  // LINUX on AMD64 systems

static const int CLASS_MAX10 = CLASS_LINUX_AMD64;

// ARCHITECTURE COMPATIBILITY CLASSES

// For ODS10 and earlier things which normally define ODS compatibility are:
//  1) endianness (big-endian/little-endian)
//  2) alignment (32-bit or 64-bit), matters for record formats
//  3) pointer size (32-bit or 64-bit), also matters for record formats

enum ArchitectureType { 
	archUnknown, // Unknown architecture, allow opening database only if CLASS matches exactly
	archIntel86  // Little-endian platform with 32-bit pointers and 32-bit alignment (ODS10)
};

// Note that Sparc, HP and PowerPC disk structures should be compatible in theory, 
// but in practice alignment on these platforms varies and actually depends on the 
// compiler used to produce the build. Yes, some 32-bit RISC builds use 64-bit alignment.
// This is why we declare all such builds "Unknown" for ODS10.

static ArchitectureType archMatrix10[CLASS_MAX10 + 1] = {
	archUnknown, // CLASS_UNKNOWN
	archUnknown, // CLASS_APOLLO_68K
	archUnknown, // CLASS_SOLARIS_SPARC
	archIntel86, // CLASS_SOLARIS_I386
	archUnknown, // CLASS_VMS_VAX
	archUnknown, // CLASS_ULTRIX_VAX
	archUnknown, // CLASS_ULTRIX_MIPS
	archUnknown, // CLASS_HPUX_PA
	archUnknown, // CLASS_NETWARE_I386
	archUnknown, // CLASS_MAC_OS
	archUnknown, // CLASS_AIX_PPC
	archUnknown, // CLASS_DG_AVIION
	archUnknown, // CLASS_MPE_XL
	archUnknown, // CLASS_IRIX_MIPS
	archUnknown, // CLASS_CRAY
	archUnknown, // CLASS_TRU64_ALPHA
	archIntel86, // CLASS_WINDOWS_I386
	archUnknown, // CLASS_OS2
	archUnknown, // CLASS_WIN16
	archIntel86, // CLASS_LINUX_I386
	archUnknown, // CLASS_LINUX_SPARC
	archIntel86, // CLASS_FREEBSD_I386
    archIntel86, // CLASS_NETBSD_I386
	archUnknown, // CLASS_DARWIN_PPC
	archUnknown  // CLASS_LINUX_AMD64
};

#ifdef sun
#ifdef i386
#define CLASS		CLASS_SOLARIS_I386
#else
#define CLASS		CLASS_SOLARIS_SPARC
#endif
#endif

#ifdef hpux
#define CLASS		CLASS_HPUX_PA
#endif

#ifdef VMS
#define CLASS		CLASS_VMS_VAX
#endif

#ifdef AIX
#define CLASS		CLASS_AIX_PPC
#endif

#ifdef AIX_PPC
#define CLASS		CLASS_AIX_PPC
#endif

#ifdef WIN_NT
#define CLASS		CLASS_WINDOWS_I386
#endif

#ifdef SINIXZ
#define CLASS       CLASS_LINUX_I386
#endif

#ifdef LINUX
#ifdef i386
#define CLASS           CLASS_LINUX_I386
#endif
#ifdef i586
#define CLASS           CLASS_LINUX_I386
#endif
#ifdef sparc
#define CLASS           CLASS_LINUX_SPARC
#endif
#ifdef AMD64
#define CLASS           CLASS_LINUX_AMD64
#endif
#endif

#ifdef FREEBSD
#define CLASS           CLASS_FREEBSD_I386
#endif

#ifdef NETBSD
#define CLASS           CLASS_NETBSD_I386
#endif

#ifdef DARWIN
#define CLASS		CLASS_DARWIN_PPC
#endif


int PAG_add_clump(
				  SLONG page_num,
				  USHORT type,
				  USHORT len, UCHAR * entry, USHORT mode, USHORT must_write)
{
/***********************************************
 *
 *	P A G _ a d d _ c l u m p
 *
 ***********************************************
 *
 * Functional description
 *	Adds a clump to log/header page.
 *	mode
 *		0 - add			CLUMP_ADD
 *		1 - replace		CLUMP_REPLACE
 *		2 - replace only!	CLUMP_REPLACE_ONLY
 *	returns
 *		TRUE - modified page
 *		FALSE - nothing done
 *
 **************************************/
	DBB dbb;
	HDR header;
	LIP logp;
	WIN window;
	UCHAR *entry_p, *clump_end, *r;
	USHORT l;
	int found;
	USHORT *end_addr;
	PAG page;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	ERR_POST_IF_DATABASE_IS_READONLY(dbb);

	window.win_page = page_num;
	window.win_flags = 0;
	if (page_num == HEADER_PAGE) {
		page = CCH_FETCH(tdbb, &window, LCK_write, pag_header);
		header = (HDR) page;
		end_addr = &header->hdr_end;
	}
	else {
		page = CCH_FETCH(tdbb, &window, LCK_write, pag_log);
		logp = (LIP) page;
		end_addr = &logp->log_end;
	}

	while (mode != CLUMP_ADD) {
		found = find_type(page_num, &window, &page, LCK_write, type,
						  &entry_p, &clump_end);

		/* If we did'nt find it and it is REPLACE_ONLY, return */

		if ((!found) && (mode == CLUMP_REPLACE_ONLY)) {
			CCH_RELEASE(tdbb, &window);
			return FALSE;
		}

		/* If not found, just go and add the entry */

		if (!found)
			break;

		/* if same size, overwrite it */

		if (entry_p[1] == len) {
			entry_p += 2;
			r = entry;
			if ( (l = len) ) {
				if (must_write)
					CCH_MARK_MUST_WRITE(tdbb, &window);
				else
					CCH_MARK(tdbb, &window);
				do
					*entry_p++ = *r++;
				while (--l);

				if (dbb->dbb_wal)
					CCH_journal_page(tdbb, &window);
			}
			CCH_RELEASE(tdbb, &window);
			return TRUE;
		}

		/* delete the entry

		 * Page is marked must write because of precedence problems.  Later
		 * on we may allocate a new page and set up a precedence relationship.
		 * This may be the lower precedence page and so it cannot be dirty
		 */

		CCH_MARK_MUST_WRITE(tdbb, &window);

		*end_addr -= (2 + entry_p[1]);

		r = entry_p + 2 + entry_p[1];
		l = clump_end - r + 1;

		if (l)
			do
				*entry_p++ = *r++;
			while (--l);

		if (dbb->dbb_wal)
			CCH_journal_page(tdbb, &window);

		CCH_RELEASE(tdbb, &window);

		/* refetch the page */

		window.win_page = page_num;
		if (page_num == HEADER_PAGE) {
			page = CCH_FETCH(tdbb, &window, LCK_write, pag_header);
			header = (HDR) page;
			end_addr = &header->hdr_end;
		}
		else {
			page = CCH_FETCH(tdbb, &window, LCK_write, pag_log);
			logp = (LIP) page;
			end_addr = &logp->log_end;
		}
		break;
	}

/* Add the entry */

	find_clump_space(page_num, &window, &page, type, len, entry, must_write);

	if (dbb->dbb_wal)
		CCH_journal_page(tdbb, &window);

	CCH_RELEASE(tdbb, &window);
	return TRUE;
}


USHORT PAG_add_file(TEXT * file_name, SLONG start)
{
/**************************************
 *
 *	P A G _ a d d _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Add a file to the current database.  Return the sequence
 *	number for the new file.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	FIL file, next;
	WIN window;
	HDR header;
	USHORT sequence;
	ULONG seqno;
	ULONG offset;
	JRNF journal;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	ERR_POST_IF_DATABASE_IS_READONLY(dbb);

/* Find current last file */

	for (file = dbb->dbb_file; file->fil_next; file = file->fil_next);

// Verify database file path against DatabaseAccess entry of firebird.conf
	if (!ISC_verify_database_access(file_name)) {
		ERR_post(gds_conf_access_denied,
			gds_arg_string, "additional database file",
			gds_arg_string, ERR_cstring(file_name),
			gds_arg_end);
	}

/* Create the file.  If the sequence number comes back zero, it didn't
   work, so punt */

	if (!(sequence = PIO_add_file(dbb, dbb->dbb_file, file_name, start)))
		return 0;

/* Create header page for new file */

	next = file->fil_next;

	if (dbb->dbb_flags & DBB_force_write)
		PIO_force_write(next, TRUE);

	window.win_page = next->fil_min_page;
	header = (HDR) CCH_fake(tdbb, &window, 1);
	header->hdr_header.pag_type = pag_header;
	header->hdr_sequence = sequence;
	header->hdr_page_size = dbb->dbb_page_size;
	header->hdr_data[0] = HDR_end;
	header->hdr_end = HDR_SIZE;
	next->fil_sequence = sequence;
	header->hdr_header.pag_checksum = CCH_checksum(window.win_bdb);
	PIO_write(dbb->dbb_file, window.win_bdb, window.win_buffer,
			  tdbb->tdbb_status_vector);
	CCH_RELEASE(tdbb, &window);
	next->fil_fudge = 1;

/* Update the previous header page to point to new file */

	file->fil_fudge = 0;
	window.win_page = file->fil_min_page;
	header = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	if (!file->fil_min_page)
		CCH_MARK_MUST_WRITE(tdbb, &window);
	else
		CCH_MARK(tdbb, &window);

	--start;

	if (file->fil_min_page) {
		PAG_add_header_entry(header, HDR_file, strlen(file_name),
							 (UCHAR *) file_name);
		PAG_add_header_entry(header, HDR_last_page, sizeof(SLONG),
							 (UCHAR *) & start);
	}
	else {
		PAG_add_clump(HEADER_PAGE, HDR_file, strlen(file_name),
					  (UCHAR *) file_name, CLUMP_REPLACE, 1);
		PAG_add_clump(HEADER_PAGE, HDR_last_page, sizeof(SLONG),
					  (UCHAR *) & start, CLUMP_REPLACE, 1);
	}

	if (dbb->dbb_wal) {
		if (!(file->fil_min_page))
			CCH_journal_page(tdbb, &window);

		journal.jrnf_header.jrnh_type = JRN_NEW_FILE;
		journal.jrnf_start = start + 1;
		journal.jrnf_sequence = sequence;
		journal.jrnf_length = strlen(file_name);
		tdbb->tdbb_status_vector[1] = 0;

		AIL_put(dbb,
				tdbb->tdbb_status_vector,
				reinterpret_cast < jrnh * >(&journal),
				JRNF_SIZE,
				reinterpret_cast < UCHAR * >(file_name),
				journal.jrnf_length, 0, 0, &seqno, &offset);

		if (tdbb->tdbb_status_vector[1])
			ERR_punt();
	}

	header->hdr_header.pag_checksum = CCH_checksum(window.win_bdb);
	PIO_write(dbb->dbb_file, window.win_bdb, window.win_buffer,
			  tdbb->tdbb_status_vector);
	CCH_RELEASE(tdbb, &window);
	if (file->fil_min_page)
		file->fil_fudge = 1;

	return sequence;
}


int PAG_add_header_entry(HDR header, USHORT type, SSHORT len, UCHAR * entry)
{
/***********************************************
 *
 *	P A G _ a d d _ h e a d e r _ e n t r y
 *
 ***********************************************
 *
 * Functional description
 *	Add an entry to header page.
 *	This will be used mainly for the shadow header page and adding
 *	secondary files.
 *	Will not follow to hdr_next_page
 *	Will not journal changes made to page.
 *	RETURNS
 *		TRUE - modified page
 *		FALSE - nothing done
 *
 **************************************/
	DBB dbb;
	UCHAR *q, *p;
	int free_space;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	ERR_POST_IF_DATABASE_IS_READONLY(dbb);

	q = entry;

	for (p = header->hdr_data; ((*p != HDR_end) && (*p != type));
		 p += 2 + p[1]);

	if (*p != HDR_end)
		return FALSE;

/* We are at HDR_end, add the entry */

	free_space = dbb->dbb_page_size - header->hdr_end;

	if (free_space > (2 + len)) {
		assert(type <= MAX_UCHAR);
		assert(len <= MAX_UCHAR);
		*p++ = static_cast < UCHAR > (type);
		*p++ = static_cast < UCHAR > (len);

		if (len) {
			if (q)
				do
					*p++ = *q++;
				while (--len);
			else
				do
					*p++ = 0;
				while (--len);
		}

		*p = HDR_end;

		header->hdr_end = p - (UCHAR *) header;

		return TRUE;
	}

	BUGCHECK(251);
	return FALSE;				/* Added to remove compiler warning */
}


PAG PAG_allocate(WIN * window)
{
/**************************************
 *
 *	P A G _ a l l o c a t e
 *
 **************************************
 *
 * Functional description
 *	Allocate a page and fake a read with a write lock.  This is
 *	the universal sequence when allocating pages.
 *
 **************************************/
	DBB dbb;
	PIP pip_page;
	PIP new_pip_page;
	PAG new_page = 0;
	WIN pip_window;
	PGC control;
	USHORT i;
	UCHAR *bytes, bit, *end;
	SLONG sequence;
	SLONG relative_bit;
	JRNA record;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	control = dbb->dbb_pcontrol;
	pip_window.win_flags = 0;

/* Find an allocation page with something on it */

	for (sequence = control->pgc_high_water;; sequence++) {
		pip_window.win_page = (sequence == 0) ?
			control->pgc_pip : sequence * control->pgc_ppp - 1;
		pip_page = (PIP) CCH_FETCH(tdbb, &pip_window, LCK_write, pag_pages);
		end = (UCHAR *) pip_page + dbb->dbb_page_size;
		for (bytes = &pip_page->pip_bits[pip_page->pip_min >> 3]; bytes < end;
			 bytes++) {
			if (*bytes != 0) {
				/* 'byte' is not zero, so it describes at least one free page. */
				for (i = 0, bit = 1; i < 8; i++, bit <<= 1) {
					if (bit & *bytes) {
						relative_bit =
							((bytes - pip_page->pip_bits) << 3) + i;
						window->win_page =
							relative_bit + sequence * control->pgc_ppp;
						new_page = CCH_fake(tdbb, window, 0);	/* don't wait on latch */
						if (new_page)
							break;	/* Found a page and successfully fake-ed it */
					}
				}
			}
			if (new_page)
				break;			/* Found a page and successfully fake-ed it */
		}
		if (new_page)
			break;				/* Found a page and successfully fake-ed it */
		CCH_RELEASE(tdbb, &pip_window);
	}

	control->pgc_high_water = sequence;

	CCH_MARK(tdbb, &pip_window);
	*bytes &= ~bit;

	if (dbb->dbb_wal) {
		record.jrna_type = JRNP_PIP;
		record.jrna_allocate = TRUE;
		record.jrna_slot = relative_bit;
		CCH_journal_record(tdbb, &pip_window,
						   reinterpret_cast < UCHAR * >(&record),
						   sizeof(record), 0, 0);
	}

	if (relative_bit != control->pgc_ppp - 1) {
		CCH_RELEASE(tdbb, &pip_window);
		CCH_precedence(tdbb, window, pip_window.win_page);
#ifdef VIO_DEBUG
		if (debug_flag > DEBUG_WRITES_INFO)
			ib_printf("\tPAG_allocate:  allocated page %"SLONGFORMAT"\n",
					  window->win_page);
#endif
		return new_page;
	}

/* We've allocated the last page on the space management page.  Rather
   than returning it, format it as a page inventory page, and recurse. */

	new_pip_page = (PIP) new_page;
	new_pip_page->pip_header.pag_type = pag_pages;
	end = (UCHAR *) new_pip_page + dbb->dbb_page_size;
	for (bytes = new_pip_page->pip_bits; bytes < end;)
		*bytes++ = 0xff;

	if (dbb->dbb_wal)
		CCH_journal_page(tdbb, window);

	CCH_must_write(window);
	CCH_RELEASE(tdbb, window);
	CCH_must_write(&pip_window);
	CCH_RELEASE(tdbb, &pip_window);

	return PAG_allocate(window);
}


SLONG PAG_attachment_id(void)
{
/******************************************
 *
 *	P A G _ a t t a c h m e n t _ i d
 *
 ******************************************
 *
 * Functional description
 *	Get attachment id.  If don't have one, get one.  As a side
 *	effect, get a lock on it as well.
 *
 ******************************************/
	TDBB tdbb;
	DBB dbb;
	ATT attachment;
	HDR header;
	WIN window;
	LCK lock;
	JRNDA record;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

	attachment = tdbb->tdbb_attachment;
	window.win_flags = 0;

/* If we've been here before just return the id */

	if (attachment->att_id_lock)
		return attachment->att_attachment_id;

/* Get new attachment id */

	if (dbb->dbb_flags & DBB_read_only) {
		attachment->att_attachment_id = ++dbb->dbb_attachment_id;
	}
	else {
		window.win_page = HEADER_PAGE;
		header = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
		CCH_MARK(tdbb, &window);
		attachment->att_attachment_id = ++header->hdr_attachment_id;

		/* If journalling is enabled, journal the change */
		if (dbb->dbb_wal) {
			record.jrnda_type = JRNP_DB_ATTACHMENT;
			record.jrnda_data = header->hdr_attachment_id;
			CCH_journal_record(tdbb, &window,
							   reinterpret_cast < UCHAR * >(&record),
							   JRNDA_SIZE, 0, 0);
		}
		CCH_RELEASE(tdbb, &window);
	}

/* Take out lock on attachment id */

	lock = attachment->att_id_lock = FB_NEW_RPT(*dbb->dbb_permanent, sizeof(SLONG)) lck();
	lock->lck_type = LCK_attachment;
	lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);
	lock->lck_parent = dbb->dbb_lock;
	lock->lck_length = sizeof(SLONG);
	lock->lck_key.lck_long = attachment->att_attachment_id;
	lock->lck_dbb = dbb;
	LCK_lock(tdbb, lock, LCK_write, TRUE);

	return attachment->att_attachment_id;
}


int PAG_delete_clump_entry(SLONG page_num, USHORT type)
{
/***********************************************
 *
 *	P A G _ d e l e t e _ c l u m p _ e n t r y
 *
 ***********************************************
 *
 * Functional description
 *	Gets rid on the entry 'type' from page.
 *
 **************************************/
	DBB dbb;
	WIN window;
	UCHAR *entry_p, *clump_end, *r;
	USHORT l;
	LIP logp;
	HDR header;
	PAG page;
	USHORT *end_addr;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	ERR_POST_IF_DATABASE_IS_READONLY(dbb);

	window.win_page = page_num;
	window.win_flags = 0;

	if (page_num == HEADER_PAGE)
		page = CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	else
		page = CCH_FETCH(tdbb, &window, LCK_write, pag_log);

	if (!find_type
		(page_num, &window, &page, LCK_write, type, &entry_p, &clump_end)) {
		CCH_RELEASE(tdbb, &window);
		return FALSE;
	}
	CCH_MARK(tdbb, &window);
	if (page_num == HEADER_PAGE) {
		header = (HDR) page;
		end_addr = &header->hdr_end;
	}
	else {
		logp = (LIP) page;
		end_addr = &logp->log_end;
	}

	*end_addr -= (2 + entry_p[1]);

	r = entry_p + 2 + entry_p[1];
	l = clump_end - r + 1;

	if (l)
		do
			*entry_p++ = *r++;
		while (--l);

	if (dbb->dbb_wal)
		CCH_journal_page(tdbb, &window);
	CCH_RELEASE(tdbb, &window);

	return TRUE;
}


void PAG_format_header(void)
{
/**************************************
 *
 *	 P A G _ f o r m a t _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *	Create the header page for a new file.
 *
 **************************************/
	DBB dbb;
	WIN window;
	HDR header;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* Initialize header page */

	window.win_page = HEADER_PAGE;
	header = (HDR) CCH_fake(tdbb, &window, 1);
	MOV_time_stamp(reinterpret_cast <
				   ISC_TIMESTAMP * >(header->hdr_creation_date));
	header->hdr_header.pag_type = pag_header;
	header->hdr_page_size = dbb->dbb_page_size;
	header->hdr_ods_version = ODS_VERSION;
	header->hdr_implementation = CLASS;
	header->hdr_ods_minor = ODS_CURRENT;
	header->hdr_ods_minor_original = ODS_CURRENT;
	header->hdr_oldest_transaction = 1;
	header->hdr_bumped_transaction = 1;
	header->hdr_end = HDR_SIZE;
	header->hdr_data[0] = HDR_end;
#ifdef SYNC_WRITE_DEFAULT
	header->hdr_flags |= hdr_force_write;
#endif

	if (dbb->dbb_flags & DBB_DB_SQL_dialect_3)
		header->hdr_flags |= hdr_SQL_dialect_3;

	dbb->dbb_ods_version = header->hdr_ods_version;
	dbb->dbb_minor_version = header->hdr_ods_minor;
	dbb->dbb_minor_original = header->hdr_ods_minor_original;

	CCH_RELEASE(tdbb, &window);
}


void PAG_format_log(void)
{
/***********************************************
 *
 *	P A G _ f o r m a t _ l o g
 *
 ***********************************************
 *
 * Functional description
 *	Initialize log page.
 *	Set all parameters to 0
 *
 **************************************/
	WIN window;
	LIP logp;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;

	window.win_page = LOG_PAGE;
	logp = (LIP) CCH_fake(tdbb, &window, 1);
	logp->log_header.pag_type = pag_log;

	AIL_init_log_page(logp, (SLONG) 0);

	CCH_RELEASE(tdbb, &window);
}


void PAG_format_pip(void)
{
/**************************************
 *
 *	 P A G _ f o r m a t _ p i p
 *
 **************************************
 *
 * Functional description
 *	Create a page inventory page to
 *	complete the formatting of a new file
 *	into a rudimentary database.
 *
 **************************************/
	DBB dbb;
	WIN window;
	PIP pages;
	UCHAR *p;
	int i;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* Initialize Page Inventory Page */

	dbb->dbb_pcontrol->pgc_pip = window.win_page = 1;
	pages = (PIP) CCH_fake(tdbb, &window, 1);

	pages->pip_header.pag_type = pag_pages;
	pages->pip_min = 4;
	p = pages->pip_bits;
	i = dbb->dbb_page_size - OFFSETA(PIP, pip_bits);

	while (i--)
		*p++ = 0xff;

	pages->pip_bits[0] &= ~(1 | 2 | 4);

	CCH_RELEASE(tdbb, &window);
}


int PAG_get_clump(SLONG page_num, USHORT type, USHORT * len, UCHAR * entry)
{
/***********************************************
 *
 *	P A G _ g e t _ c l u m p
 *
 ***********************************************
 *
 * Functional description
 *	Find 'type' clump in page_num
 *		TRUE  - Found it
 *		FALSE - Not present
 *	RETURNS
 *		value of clump in entry
 *		length in len
 *
 **************************************/
	UCHAR *q, *entry_p;
	USHORT l;
	WIN window;
	PAG page;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;

	*len = 0;
	window.win_page = page_num;
	window.win_flags = 0;

	if (page_num == HEADER_PAGE)
		page = CCH_FETCH(tdbb, &window, LCK_read, pag_header);
	else
		page = CCH_FETCH(tdbb, &window, LCK_read, pag_log);

	if (!find_type(page_num, &window, &page, LCK_read, type, &entry_p, &q)) {
		CCH_RELEASE(tdbb, &window);
		return FALSE;
	}

	*len = l = entry_p[1];
	entry_p += 2;

	q = entry;
	if (l)
		do
			*q++ = *entry_p++;
		while (--l);

	CCH_RELEASE(tdbb, &window);

	return TRUE;
}


void PAG_header(TEXT * file_name, USHORT file_length)
{
/**************************************
 *
 *	P A G _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *	Checkout database header page.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	HDR header;
	VCL vector;
	JRD_REL relation;
	SCHAR *temp_buffer, *temp_page;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

/* allocate a spare buffer which is large enough,
   and set up to release it in case of error; note
   that dbb_page_size has not been set yet, so we
   can't depend on this.

   Make sure that buffer is aligned on a page boundary
   and unit of transfer is a multiple of physical disk
   sector for raw disk access. */

	temp_buffer = (SCHAR*)gds__alloc((SLONG) 2 * MIN_PAGE_SIZE);
	temp_page =
		(SCHAR *) (((U_IPTR) temp_buffer + MIN_PAGE_SIZE - 1) &
				   ~((U_IPTR) MIN_PAGE_SIZE - 1));

	try {

	header = (HDR) temp_page;
	PIO_header(dbb, temp_page, MIN_PAGE_SIZE);

	if (header->hdr_header.pag_type != pag_header || header->hdr_sequence)
		ERR_post(gds_bad_db_format,
				 gds_arg_cstring, file_length, ERR_string(file_name,
														  file_length), 0);

#ifdef ODS_8_TO_CURRENT
/* This Server understands ODS greater than 8 *ONLY* upto current major
   ODS_VERSION defined in ods.h, Refuse connections to older or newer ODS's */
	if ((header->hdr_ods_version < ODS_VERSION8)
		|| (header->hdr_ods_version > ODS_VERSION))
#else
	if (header->hdr_ods_version != ODS_VERSION)
#endif
		ERR_post(gds_wrong_ods,
				 gds_arg_cstring, file_length, ERR_string(file_name,
														  file_length),
				 gds_arg_number, (SLONG) header->hdr_ods_version,
				 gds_arg_number, (SLONG) ODS_VERSION, 0);

/****
Note that if this check is turned on, it should be recoded in order that
the Intel platforms can share databases.  At present (Feb 95) it is possible
to share databases between Windows and NT, but not with NetWare.  Sharing
databases with OS/2 is unknown and needs to be investigated.  The CLASS was
initially 8 for all Intel platforms, but was changed after 4.0 was released
in order to allow differentiation between databases created on various
platforms.  This should allow us in future to identify where databases were
created.  Even when we get to the stage where databases created on PC platforms
are sharable between all platforms, it would be useful to identify where they
were created for debugging purposes.  - Deej 2/6/95

Re-enable and recode the check to avoid BUGCHECK messages when database
is accessed with engine built for another architecture. - Nickolay 9-Feb-2005
****/

	if (header->hdr_implementation != CLASS &&
		(header->hdr_implementation < 0 || header->hdr_implementation > CLASS_MAX10 ||
		 archMatrix10[header->hdr_implementation] == archUnknown ||
		 archMatrix10[header->hdr_implementation] != archMatrix10[CLASS])
	   )
	{
	    ERR_post (gds_bad_db_format,
			gds_arg_cstring, file_length,  ERR_string(file_name, file_length), 0);
	}

	if (header->hdr_page_size < MIN_PAGE_SIZE ||
		header->hdr_page_size > MAX_PAGE_SIZE)
			ERR_post(gds_bad_db_format,
					 gds_arg_cstring, file_length, ERR_string(file_name,
															  file_length),
					 0);

	if (header->hdr_next_transaction) {
		if (header->hdr_oldest_active > header->hdr_next_transaction)
			BUGCHECK(266);		/*next transaction older than oldest active */

		if (header->hdr_oldest_transaction > header->hdr_next_transaction)
			BUGCHECK(267);		/* next transaction older than oldest transaction */
	}

	dbb->dbb_ods_version = header->hdr_ods_version;
	dbb->dbb_minor_version = header->hdr_ods_minor;
	dbb->dbb_minor_original = header->hdr_ods_minor_original;

	if (header->hdr_flags & hdr_SQL_dialect_3)
		dbb->dbb_flags |= DBB_DB_SQL_dialect_3;

	relation = MET_relation(tdbb, 0);
	if (!relation->rel_pages) {
		// 21-Dec-2003 Nickolay Samofatov
		// No need to re-set first page for RDB$PAGES relation since 
		// current code cannot change its location after database creation
		// Currently, this change only affects isc_database_info call,
		// the only call which may call PAG_header multiple times.
		// In fact, this isc_database_info behavior seems dangerous to me,
		// but let somebody else fix that problem, I just fix the memory leak.
		relation->rel_pages = vector = vcl::newVector(*dbb->dbb_permanent, 1);
		(*vector)[0] = header->hdr_PAGES;
	}

	dbb->dbb_page_size = header->hdr_page_size;
	dbb->dbb_page_buffers = header->hdr_page_buffers;
	dbb->dbb_next_transaction = header->hdr_next_transaction;
	dbb->dbb_oldest_transaction = header->hdr_oldest_transaction;
	dbb->dbb_oldest_active = header->hdr_oldest_active;
	dbb->dbb_oldest_snapshot = header->hdr_oldest_snapshot;

	dbb->dbb_attachment_id = header->hdr_attachment_id;

	if (header->hdr_flags & hdr_read_only) {
		/* If Header Page flag says the database is ReadOnly, gladly accept it. */
		dbb->dbb_flags &= ~DBB_being_opened_read_only;
		dbb->dbb_flags |= DBB_read_only;
	}

/* If hdr_read_only is not set... */
	if (!(header->hdr_flags & hdr_read_only)
		&& (dbb->dbb_flags & DBB_being_opened_read_only)) {
		/* Looks like the Header page says, it is NOT ReadOnly!! But the database
		 * file system permission gives only ReadOnly access. Punt out with
		 * gds__no_priv error (no privileges)
		 */
		ERR_post(gds_no_priv,
				 gds_arg_string, "read-write",
				 gds_arg_string, "database",
				 gds_arg_cstring, file_length, ERR_string(file_name,
														  file_length), 0);
	}

	if (header->hdr_flags & hdr_force_write) {
		dbb->dbb_flags |= DBB_force_write;
		if (!(header->hdr_flags & hdr_read_only))
			PIO_force_write(dbb->dbb_file, TRUE);
	}

	if (header->hdr_flags & hdr_no_reserve)
		dbb->dbb_flags |= DBB_no_reserve;

	if (header->hdr_flags & hdr_shutdown)
		dbb->dbb_ast_flags |= DBB_shutdown;

	if (temp_buffer)
		gds__free(temp_buffer);
	}	// try
	catch (const std::exception&) {
		if (temp_buffer) {
			gds__free(temp_buffer);
		}
		ERR_punt();
	}
}


void PAG_init(void)
{
/**************************************
 *
 *	P A G _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize stuff for page handling.
 *
 **************************************/
	DBB dbb;
	PGC control;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	dbb->dbb_pcontrol = control = FB_NEW(*dbb->dbb_permanent) pgc();
	control->pgc_bytes = dbb->dbb_page_size - OFFSETA(PIP, pip_bits);
	control->pgc_ppp = control->pgc_bytes * 8;
	control->pgc_tpt =
		(dbb->dbb_page_size - OFFSETA(TIP, tip_transactions)) * 4;
	control->pgc_pip = 1;
/* dbb_ods_version can be 0 when a new database is being created */
	if ((dbb->dbb_ods_version == 0)
		|| (dbb->dbb_ods_version >= ODS_VERSION10))
		control->pgc_gpg =
			(dbb->dbb_page_size -
			 OFFSETA(GPG, gpg_values)) / sizeof(((GPG) 0)->gpg_values);
	else
		control->pgc_gpg =
			(dbb->dbb_page_size -
			 OFFSETA(PPG, ppg_page)) / sizeof(((PPG) 0)->ppg_page);




/* Compute the number of data pages per pointer page.  Each data page
   requires a 32 bit pointer and a 2 bit control field. */

	dbb->dbb_dp_per_pp = (dbb->dbb_page_size - OFFSETA(PPG, ppg_page)) * 8 /
		(BITS_PER_LONG + 2);

/* Compute the number of records that can fit on a page using the
   size of the record index (dpb_repeat) and a record header.  This
   gives an artificially high number, reducing the density of db_keys. */

	dbb->dbb_max_records = (dbb->dbb_page_size - sizeof(struct dpg)) /
		(sizeof(dpg::dpg_repeat) + OFFSETA(RHD, rhd_data));

/* Compute the number of index roots that will fit on an index root page,
   assuming that each index has only one key */

	dbb->dbb_max_idx = (dbb->dbb_page_size - OFFSETA(IRT, irt_rpt)) /
		(sizeof(irt::irt_repeat) + (1 * (sizeof(irtd))));

	// dimitr:	temporary fix. We allow no more than 256 indices in FB 1.5
	//			because of theoretically possible BTR instability and
	//			known optimizer limits.
	dbb->dbb_max_idx = MIN(dbb->dbb_max_idx, 256);

/* Compute prefetch constants from database page size and maximum prefetch
   transfer size. Double pages per prefetch request so that cache reader
   can overlap prefetch I/O with database computation over previously
   prefetched pages. */

	dbb->dbb_prefetch_sequence = PREFETCH_MAX_TRANSFER / dbb->dbb_page_size;
	dbb->dbb_prefetch_pages = dbb->dbb_prefetch_sequence * 2;
}


void PAG_init2(USHORT shadow_number)
{
/**************************************
 *
 *	P A G _ i n i t 2
 *
 **************************************
 *
 * Functional description
 *	Perform second phase of page initialization -- the eternal
 *	search for additional files.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	FIL file;
	SDW shadow;
	HDR header;
	WIN window;
	SCHAR *temp_buffer = NULL, *temp_page;
	USHORT file_length, sequence;
	TEXT *p, *file_name;
	ULONG last_page;
	struct bdb temp_bdb;
	SLONG next_page;
	TEXT buf[MAXPATHLEN + 1];
	ISC_STATUS *status;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	status = tdbb->tdbb_status_vector;

/* allocate a spare buffer which is large enough,
   and set up to release it in case of error. Align
   the temporary page buffer for raw disk access. */

	temp_buffer = (SCHAR*) gds__alloc((SLONG) dbb->dbb_page_size + MIN_PAGE_SIZE);
	temp_page =
		(SCHAR *) (((U_IPTR) temp_buffer + MIN_PAGE_SIZE - 1) &
				   ~((U_IPTR) MIN_PAGE_SIZE - 1));

	try {

	file = dbb->dbb_file;
	if (shadow_number) {
		for (shadow = dbb->dbb_shadow; shadow; shadow = shadow->sdw_next)
			if (shadow->sdw_number == shadow_number) {
				file = shadow->sdw_file;
				break;
			}
		if (!shadow)
			BUGCHECK(161);		/* msg 161 shadow block not found */
	}

	sequence = 1;
	window.win_flags = 0;

/* Loop thru files and header pages until everything is open */

	for (;;)
	{
		file_name = NULL;
		window.win_page = file->fil_min_page;
		do {
			/* note that we do not have to get a read lock on
			   the header page (except for header page 0) because
			   the only time it will be modified is when adding a file,
			   which must be done with an exclusive lock on the database --
			   if this changes, this policy will have to be reevaluated;
			   at any rate there is a problem with getting a read lock
			   because the corresponding page in the main database file
			   may not exist */

			if (!file->fil_min_page)
				CCH_FETCH(tdbb, &window, LCK_read, pag_header);

			header = (HDR) temp_page;
			temp_bdb.bdb_buffer = (PAG) header;
			temp_bdb.bdb_page = window.win_page;
			temp_bdb.bdb_dbb = dbb;

			/* Read the required page into the local buffer */
			PIO_read(file, &temp_bdb, (PAG) header, status);

			if ((shadow_number) && (!file->fil_min_page))
				CCH_RELEASE(tdbb, &window);

			for (p = reinterpret_cast<TEXT*>(header->hdr_data); 
				 *p != HDR_end; 
				 p += 2 + p[1])
				switch (*p) {
				case HDR_file:
					file_length = p[1];
					file_name = buf;
					MOVE_FAST(p + 2, buf, file_length);
					break;

				case HDR_last_page:
					MOVE_FAST(p + 2, &last_page, sizeof(last_page));
					break;

				case HDR_sweep_interval:
					if (!(dbb->dbb_flags & DBB_read_only))
						MOVE_FAST(p + 2, &dbb->dbb_sweep_interval,
								  sizeof(SLONG));
					break;
				}

			next_page = header->hdr_next_page;

			if ((!shadow_number) && (!file->fil_min_page))
				CCH_RELEASE(tdbb, &window);

			window.win_page = next_page;

			/*
			 * Make sure the header page and all the overflow header
			 * pages are traversed.  For V4.0, only the header page for
			 * the primary database page will have overflow pages.
			 */

		} while (next_page);

		if (file->fil_min_page)
			file->fil_fudge = 1;
		if (!file_name)
			break;

// Verify database file path against DatabaseAccess entry of firebird.conf
		file_name[file_length] = 0;
		if (!ISC_verify_database_access(file_name)) {
			ERR_post(gds_conf_access_denied,
				gds_arg_string, "additional database file",
				gds_arg_string, ERR_cstring(file_name),
				gds_arg_end);
		}

		file->fil_next = PIO_open(dbb,
								  file_name,
								  file_length,
								  FALSE,
								  0,
								  file_name,
								  file_length);
		file->fil_max_page = last_page;
		file = file->fil_next;
		if (dbb->dbb_flags & DBB_force_write)
			PIO_force_write(file, TRUE);
		file->fil_min_page = last_page + 1;
		file->fil_sequence = sequence++;
	}

	if (temp_buffer) {
		gds__free(temp_buffer);
	}
	}	// try
	catch (const std::exception&) {
		if (temp_buffer) {
			gds__free(temp_buffer);
		}
		ERR_punt();
	}
}


SLONG PAG_last_page(void)
{
/**************************************
 *
 *	P A G _ l a s t _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Compute the highest page allocated.  This is called by the
 *	shadow stuff to dump a database.
 *
 **************************************/
	DBB dbb;
	ULONG relative_bit;
	SSHORT bit;
	UCHAR *bits;
	USHORT sequence;
	ULONG pages_per_pip;
	WIN window;
	PIP page;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	pages_per_pip = dbb->dbb_pcontrol->pgc_ppp;
	window.win_flags = 0;

/* Find the last page allocated */

	for (sequence = 0;; ++sequence) {
		window.win_page =
			(!sequence) ? dbb->dbb_pcontrol->pgc_pip : sequence *
			pages_per_pip - 1;
		page = (PIP) CCH_FETCH(tdbb, &window, LCK_read, pag_pages);
		for (bits = page->pip_bits + (pages_per_pip >> 3) - 1;
			 *bits == (UCHAR) - 1; --bits);
		for (bit = 7; bit >= 0; --bit)
			if (!(*bits & (1 << bit)))
				break;
		relative_bit = (bits - page->pip_bits) * 8 + bit;
		CCH_RELEASE(tdbb, &window);
		if (relative_bit != pages_per_pip - 1)
			break;
	}

	return sequence * pages_per_pip + relative_bit;
}


void PAG_modify_log(SLONG tid, SLONG flag)
{
/***********************************************
 *
 *	P A G _ m o d i f y _ l o g
 *
 ***********************************************
 *
 * Functional description
 *	Will set the flag in the log page.
 *	Set the transaction id and tip for the transaction.
 *	Journal change to page.
 **************************************/
	DBB dbb;
	WIN window;
	LIP page;
	JRNL record;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	window.win_page = LOG_PAGE;
	window.win_flags = 0;
	page = (LIP) CCH_FETCH(tdbb, &window, LCK_write, pag_log);
	CCH_MARK_MUST_WRITE(tdbb, &window);

/* get the TIP page for this transaction */

	if (flag & TRA_add_log)
		page->log_flags |= log_add;
	else if (flag & TRA_delete_log)
		page->log_flags |= log_delete;

	page->log_mod_tid = tid;

	if (dbb->dbb_wal) {
		record.jrnl_type = JRNP_LOG_PAGE;
		record.jrnl_flags = page->log_flags;
		record.jrnl_tip = 0;
		record.jrnl_tid = page->log_mod_tid;
		CCH_journal_record(tdbb, &window,
						   reinterpret_cast < UCHAR * >(&record), JRNL_SIZE,
						   0, 0);
	}

	CCH_RELEASE(tdbb, &window);
}


void PAG_release_page(SLONG number, SLONG prior_page)
{
/**************************************
 *
 *	P A G _ r e l e a s e _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Release a page to the free page page.
 *
 **************************************/
	DBB dbb;
	PIP pages;
	WIN pip_window;
	PGC control;
	JRNA record;
	SLONG sequence;
	SLONG relative_bit;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

#ifdef VIO_DEBUG
	if (debug_flag > DEBUG_WRITES_INFO)
		ib_printf("\tPAG_release_page:  about to release page %"SLONGFORMAT"\n", number);
#endif

	control = dbb->dbb_pcontrol;
	sequence = number / control->pgc_ppp;
	relative_bit = number % control->pgc_ppp;

	pip_window.win_page = (sequence == 0) ?
		control->pgc_pip : sequence * control->pgc_ppp - 1;

	pip_window.win_flags = 0;

/* if shared cache is being used, the page which is being freed up
 * may have a journal buffer which in no longer valid after the
 * page has been freed up.  Zero out the journal buffer.
 * It is possible that the shared cache manager will write out the
 * record as part of a scan.

 * A similar problem can happen without shared cache.
 */

/*******************************
CCH_release_journal (tdbb, number);
********************************/

	pages = (PIP) CCH_FETCH(tdbb, &pip_window, LCK_write, pag_pages);
	CCH_precedence(tdbb, &pip_window, prior_page);
	CCH_MARK(tdbb, &pip_window);
	pages->pip_bits[relative_bit >> 3] |= 1 << (relative_bit & 7);
	pages->pip_min = MIN(pages->pip_min, relative_bit);

	if (dbb->dbb_wal) {
		record.jrna_type = JRNP_PIP;
		record.jrna_allocate = FALSE;
		record.jrna_slot = relative_bit;
		CCH_journal_record(tdbb, &pip_window,
						   reinterpret_cast < UCHAR * >(&record),
						   sizeof(record), 0, 0);
	}

	CCH_RELEASE(tdbb, &pip_window);

	control->pgc_high_water = MIN(control->pgc_high_water, sequence);
}


void PAG_set_force_write(DBB dbb, SSHORT flag)
{
/**************************************
 *
 *	P A G _ s e t _ f o r c e _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Turn on/off force write.
 *      The value 2 for flag means set to default.
 *
 **************************************/
	WIN window;
	HDR header;
	FIL file;
	SDW shadow;
	JRNDA record;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;

	ERR_POST_IF_DATABASE_IS_READONLY(dbb);

	window.win_page = HEADER_PAGE;
	window.win_flags = 0;
	header = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	CCH_MARK_MUST_WRITE(tdbb, &window);

	if (flag == 2)
		/* Set force write to the default for the platform */
#ifdef SYNC_WRITE_DEFAULT
		flag = 1;
#else
		flag = 0;
#endif

	if (flag) {
		header->hdr_flags |= hdr_force_write;
		dbb->dbb_flags |= DBB_force_write;
	}
	else {
		header->hdr_flags &= ~hdr_force_write;
		dbb->dbb_flags &= ~DBB_force_write;
	}

/* If journalling is enabled, journal the change */

	if (dbb->dbb_wal) {
		record.jrnda_type = JRNP_DB_HDR_FLAGS;
		record.jrnda_data = header->hdr_flags;
		CCH_journal_record(tdbb, &window,
						   reinterpret_cast < UCHAR * >(&record), JRNDA_SIZE,
						   0, 0);
	}
	CCH_RELEASE(tdbb, &window);

	for (file = dbb->dbb_file; file; file = file->fil_next)
		PIO_force_write(file, flag);

	for (shadow = dbb->dbb_shadow; shadow; shadow = shadow->sdw_next)
		for (file = shadow->sdw_file; file; file = file->fil_next)
			PIO_force_write(file, flag);
}


void PAG_set_no_reserve(DBB dbb, USHORT flag)
{
/**************************************
 *
 *	P A G _ s e t _ n o _ r e s e r v e
 *
 **************************************
 *
 * Functional description
 *	Turn on/off reserving space for versions
 *
 **************************************/
	WIN window;
	HDR header;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;

	ERR_POST_IF_DATABASE_IS_READONLY(dbb);

	window.win_page = HEADER_PAGE;
	window.win_flags = 0;
	header = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	CCH_MARK_MUST_WRITE(tdbb, &window);

	if (flag) {
		header->hdr_flags |= hdr_no_reserve;
		dbb->dbb_flags |= DBB_no_reserve;
	}
	else {
		header->hdr_flags &= ~hdr_no_reserve;
		dbb->dbb_flags &= ~DBB_no_reserve;
	}

	CCH_RELEASE(tdbb, &window);
}


void PAG_set_db_readonly(DBB dbb, SSHORT flag)
{
/*********************************************
 *
 *	P A G _ s e t _ d b _ r e a d o n l y
 *
 *********************************************
 *
 * Functional description
 *	Set database access mode to readonly OR readwrite
 *
 *********************************************/
	WIN window;
	HDR header;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;

	window.win_page = HEADER_PAGE;
	window.win_flags = 0;
	header = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);

	if (!flag) {
		/* If the database is transitioning from RO to RW, reset the
		 * in-memory DBB flag which indicates that the database is RO.
		 * This will allow the CCH subsystem to allow pages to be MARK'ed
		 * for WRITE operations
		 */
		header->hdr_flags &= ~hdr_read_only;
		dbb->dbb_flags &= ~DBB_read_only;
	}

	CCH_MARK_MUST_WRITE(tdbb, &window);

	if (flag) {
		header->hdr_flags |= hdr_read_only;
		dbb->dbb_flags |= DBB_read_only;
	}

	CCH_RELEASE(tdbb, &window);
}


void PAG_set_db_SQL_dialect(DBB dbb, SSHORT flag)
{
/*********************************************
 *
 *	P A G _ s e t _ d b _ S Q L _ d i a l e c t
 *
 *********************************************
 *
 * Functional description
 *	Set database SQL dialect to SQL_DIALECT_V5 or SQL_DIALECT_V6
 *
 *********************************************/
	WIN window;
	HDR header;
	TDBB tdbb;
	USHORT major_version, minor_original;

	tdbb = GET_THREAD_DATA;

	major_version = (SSHORT) dbb->dbb_ods_version;
	minor_original = (SSHORT) dbb->dbb_minor_original;

	window.win_page = HEADER_PAGE;
	window.win_flags = 0;
	header = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);

	if ((flag) && (ENCODE_ODS(major_version, minor_original) >= ODS_10_0)) {
		switch (flag) {
		case SQL_DIALECT_V5:

			if (dbb->dbb_flags & DBB_DB_SQL_dialect_3 ||
				header->hdr_flags & hdr_SQL_dialect_3)
					ERR_post_warning(isc_dialect_reset_warning, 0);

			dbb->dbb_flags &= ~DBB_DB_SQL_dialect_3;	/* set to 0 */
			header->hdr_flags &= ~hdr_SQL_dialect_3;	/* set to 0 */
			break;

		case SQL_DIALECT_V6:
			dbb->dbb_flags |= DBB_DB_SQL_dialect_3;	/* set to dialect 3 */
			header->hdr_flags |= hdr_SQL_dialect_3;	/* set to dialect 3 */
			break;

		default:
			CCH_RELEASE(tdbb, &window);
			ERR_post(isc_inv_dialect_specified, isc_arg_number, flag,
					 isc_arg_gds, isc_valid_db_dialects, isc_arg_string,
					 "1 and 3", isc_arg_gds, isc_dialect_not_changed, 0);
			break;
		}
	}

	CCH_MARK_MUST_WRITE(tdbb, &window);

	CCH_RELEASE(tdbb, &window);
}


void PAG_set_page_buffers(ULONG buffers)
{
/**************************************
 *
 *	P A G _ s e t _ p a g e _ b u f f e r s
 *
 **************************************
 *
 * Functional description
 *	Set database-specific page buffer cache
 *
 **************************************/
	WIN window;
	HDR header;
	TDBB tdbb;
	DBB dbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	ERR_POST_IF_DATABASE_IS_READONLY(dbb);

	window.win_page = HEADER_PAGE;
	window.win_flags = 0;
	header = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	CCH_MARK_MUST_WRITE(tdbb, &window);
	header->hdr_page_buffers = buffers;
	CCH_RELEASE(tdbb, &window);
}


void PAG_sweep_interval(SLONG interval)
{
/**************************************
 *
 *	P A G _ s w e e p _ i n t e r v a l
 *
 **************************************
 *
 * Functional description
 *	Set sweep interval.
 *
 **************************************/

	PAG_add_clump(HEADER_PAGE, HDR_sweep_interval, sizeof(SLONG),
				  (UCHAR *) & interval, CLUMP_REPLACE, 1);
}


int PAG_unlicensed(void)
{
/**************************************
 *
 *	P A G _ u n l i c e n s e d
 *
 **************************************
 *
 * Functional description
 *	Log unlicenses activity.  Return current count of this
 *	sort of non-sense.
 *
 **************************************/
	WIN window;
	SLONG count;
	USHORT len;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;

	window.win_page = HEADER_PAGE;
	window.win_flags = 0;
	CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	CCH_MARK_MUST_WRITE(tdbb, &window);

	if (PAG_get_clump(HEADER_PAGE, HDR_unlicensed, &len, (UCHAR *) & count)) {
		count++;
		PAG_add_clump(HEADER_PAGE, HDR_unlicensed, sizeof(count),
					  (UCHAR *) & count, CLUMP_REPLACE_ONLY, 1);
	}
	else {
		count = 1;
		PAG_add_clump(HEADER_PAGE, HDR_unlicensed, sizeof(count),
					  (UCHAR *) & count, CLUMP_REPLACE, 1);
	}
	CCH_RELEASE(tdbb, &window);

	return count;
}


static void find_clump_space(
							 SLONG page_num,
							 WIN * window,
							 PAG * ppage,
							 USHORT type,
SSHORT len, UCHAR * entry, USHORT must_write)
{
/***********************************************
 *
 *	f i n d _ c l u m p _ s p a c e
 *
 ***********************************************
 *
 * Functional description
 *	Find space for the new clump.
 *	Add the entry at the end of clumplet list.
 *	Allocate a new page if required.
 *
 **************************************/
	DBB dbb;
	UCHAR *p, *ptr;
	WIN new_window;
	HDR new_header, header;
	LIP new_logp, logp;
	PAG new_page, page;
	USHORT *end_addr;
	SLONG next_page;
	SLONG free_space;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	ptr = entry;
	page = *ppage;

	while (TRUE) {
		if (page_num == HEADER_PAGE) {
			header = (HDR) page;
			next_page = header->hdr_next_page;
			free_space = dbb->dbb_page_size - header->hdr_end;
			end_addr = &header->hdr_end;
			p = (UCHAR *) header + header->hdr_end;
		}
		else {
			logp = (LIP) page;
			next_page = logp->log_next_page;
			free_space = dbb->dbb_page_size - logp->log_end;
			end_addr = &logp->log_end;
			p = (UCHAR *) logp + logp->log_end;
		}

		if (free_space > (2 + len)) {
			if (must_write)
				CCH_MARK_MUST_WRITE(tdbb, window);
			else
				CCH_MARK(tdbb, window);

			assert(type <= MAX_UCHAR);
			assert(len <= MAX_UCHAR);
			*p++ = static_cast < UCHAR > (type);
			*p++ = static_cast < UCHAR > (len);

			if (len)
				do
					*p++ = *ptr++;
				while (--len);

			*p = HDR_end;

			*end_addr = (USHORT) (p - (UCHAR *) page);
			return;
		}

		if (!next_page)
			break;

		/* Follow chain of header pages */

		if (page_num == HEADER_PAGE)
			*ppage = page =
				CCH_HANDOFF(tdbb, window, next_page, LCK_write, pag_header);
		else
			*ppage = page =
				CCH_HANDOFF(tdbb, window, next_page, LCK_write, pag_log);
	}

	new_page = (PAG) DPM_allocate(tdbb, &new_window);

	if (must_write)
		CCH_MARK_MUST_WRITE(tdbb, &new_window);
	else
		CCH_MARK(tdbb, &new_window);

	if (page_num == HEADER_PAGE) {
		new_header = (HDR) new_page;
		new_header->hdr_header.pag_type = pag_header;
		new_header->hdr_end = HDR_SIZE;
		new_header->hdr_page_size = dbb->dbb_page_size;
		new_header->hdr_data[0] = HDR_end;
		next_page = new_window.win_page;
		end_addr = &new_header->hdr_end;
		p = new_header->hdr_data;
	}
	else {
		new_logp = (LIP) new_page;
		new_logp->log_header.pag_type = pag_log;
		new_logp->log_data[0] = LOG_end;
		new_logp->log_end = LIP_SIZE;
		next_page = new_window.win_page;
		end_addr = &new_logp->log_end;
		p = new_logp->log_data;
	}

	assert(type <= MAX_UCHAR);
	assert(len <= MAX_UCHAR);
	*p++ = static_cast < UCHAR > (type);
	*p++ = static_cast < UCHAR > (len);

	if (len) {
		do {
			*p++ = *ptr++;
		} while (--len);
	}

	*p = HDR_end;
	*end_addr = (USHORT) (p - (UCHAR *) new_page);

	CCH_RELEASE(tdbb, &new_window);

	CCH_precedence(tdbb, window, next_page);

	CCH_MARK(tdbb, window);

	if (page_num == HEADER_PAGE)
		header->hdr_next_page = next_page;
	else
		logp->log_next_page = next_page;
}


static BOOLEAN find_type(
						 SLONG page_num,
						 WIN * window,
						 PAG * ppage,
						 USHORT lock,
						 USHORT type, UCHAR ** entry_p, UCHAR ** clump_end)
{
/***********************************************
 *
 *	f i n d _ t y p e
 *
 ***********************************************
 *
 * Functional description
 *	Find the requested type in a page.
 *	RETURNS
 *		pointer to type, pointer to end of page, header.
 *		TRUE  - Found it
 *		FALSE - Not present
 *
 **************************************/
	UCHAR *q, *p;
	SLONG next_page;
	HDR header;
	LIP logp;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;

	q = 0;

	while (TRUE) {
		if (page_num == HEADER_PAGE) {
			header = (HDR) (*ppage);
			p = header->hdr_data;
			next_page = header->hdr_next_page;
		}
		else {
			logp = (LIP) (*ppage);
			p = logp->log_data;
			next_page = logp->log_next_page;
		}

		for (; (*p != HDR_end); p += 2 + p[1]) {
			if (*p == type)
				q = p;
		}

		if (q) {
			*entry_p = q;
			*clump_end = p;
			return TRUE;
		}

		/* Follow chain of pages */

		if (next_page) {
			if (page_num == HEADER_PAGE)
				*ppage =
					CCH_HANDOFF(tdbb, window, next_page, lock, pag_header);
			else
				*ppage = CCH_HANDOFF(tdbb, window, next_page, lock, pag_log);
		}
		else
			return FALSE;
	}
}

} // extern "C"
