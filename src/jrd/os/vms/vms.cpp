/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		vms.cpp
 *	DESCRIPTION:	VMS specific physical IO
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
#include rms
#include iodef
#include descrip
#include ssdef

#include "../jrd/jrd.h"
#include "../jrd/os/pio.h"
#include "../jrd/lck.h"
#include "../jrd/ods.h"
#include "../jrd/cch.h"
#include "gen/codes.h"
#include "../jrd/all_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/err_proto.h"

#include "../jrd/lck_proto.h"
#include "../jrd/os/pio_proto.h"
#include "../jrd/thd_proto.h"

#define DEFAULT_FILE_NAME	".fdb"
#define EVENT_FLAG		15

/***
#define TRACE	1
***/

#ifdef TRACE
#include "../jrd/ib_stdio.h"
#endif



gds__completion_ast();

static bool extend_file(FIL, ISC_STATUS *);
static FIL seek_file(FIL, BDB, int *);
static FIL setup_file(DBB, const TEXT*, USHORT, USHORT, struct NAM*);
static void setup_trace(FIL, SSHORT);
static void trace_event(FIL, SSHORT, SCHAR *, SSHORT);
static bool vms_io_error(ISC_STATUS *, TEXT *, ISC_STATUS, int, FIL);

#define DVI$_DEVLOCKNAM		240


/* ACP File Information block */

typedef struct fib$ {
	SLONG fib$l_acctl;
	SSHORT fib$w_fid[3];
	SSHORT fib$w_did[3];
	SLONG fib$l_wcc;
	SSHORT fib$w_nmctl;
	SSHORT fib$w_exctl;
	SLONG fib$l_exsz;
	SLONG fib$l_exvbn;
} FIB$;

#define FIB$M_EXTEND	128
#define FIB$M_ALDEF	8
#define FIB$M_ALCONB	2
#define FIB$M_WRITETHRU	524288
#define FIB$M_FINDFID	2048

typedef struct atr$ {
	SSHORT atr$w_size;
	SSHORT atr$w_type;
	SCHAR *atr$l_addr;
	SLONG atr$l_zero;
} ATR$;

#define ATR$S_RECATTR	32
#define ATR$C_RECATTR	4

typedef struct fab$ {
	SCHAR fat$b_rtype;
	SCHAR fat$b_rattrib;
	SSHORT fat$w_rsize;
	USHORT fat$w_hiblk[2];
	USHORT fat$w_efblk[2];
	SSHORT fat$w_ffbyte;
	SCHAR fat$b_bktsize;
	SCHAR fat$b_vfcsize;
	SSHORT fat$w_maxrec;
	SSHORT fat$w_defext;
	SSHORT fat$w_gbc;
	SSHORT fat$w_reserved[3];
	SSHORT fat$w_not_used;
	SSHORT fat$w_versions;
} FAT$;


nt PIO_add_file(DBB dbb, FIL main_file, TEXT * file_name, SLONG start)
{
/**************************************
 *
 *	P I O _ a d d _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Add a file to an existing database.  Return the sequence
 *	number of the new file.  If anything goes wrong, return a
 *	sequence of 0.
 *	NOTE:  This routine does not lock any mutexes on
 *	its own behalf.  It is assumed that mutexes will
 *	have been locked before entry.
 *
 **************************************/
	USHORT sequence;
	FIL file, new_file;

	if (!(new_file = PIO_create(dbb, file_name, strlen(file_name), FALSE)))
		return 0;

	new_file->fil_min_page = start;
	sequence = 1;

	for (file = main_file; file->fil_next; file = file->fil_next)
		++sequence;

	file->fil_max_page = start - 1;
	file->fil_next = new_file;

	return sequence;
}


void PIO_close(FIL main_file)
{
/**************************************
 *
 *	P I O _ c l o s e
 *
 **************************************
 *
 * Functional description
 *	NOTE:  This routine does not lock any mutexes on
 *	its own behalf.  It is assumed that mutexes will
 *	have been locked before entry.
 *
 **************************************/
	FIL file;
	int status;

	for (file = main_file; file; file = file->fil_next) {
		if (!((status = sys$dassgn(file->fil_desc)) & 1))
			V4_MUTEX_DESTROY(file->fil_mutex);
#ifdef TRACE
		if (file->fil_trace) {
			trace_event(dbb->dbb_file, trace_close, 0, 0);
			ib_fclose(file->fil_trace);
		}
#endif
	}
}


int PIO_connection(const TEXT* file_name, USHORT* file_length)
{
/**************************************
 *
 *	P I O _ c o n n e c t i o n
 *
 **************************************
 *
 * Functional description
 *	Analyze a file specification and determine whether a page/lock
 *	server is required and available.  If so, return a "connection"
 *	block.  If not, return NULL.
 *
 *	Note: The file name must have been expanded prior to this call.
 *
 **************************************/
//	int node;

	return NULL;
}


FIL PIO_create(DBB dbb, const TEXT* string, SSHORT length, BOOLEAN overwrite)
{
/**************************************
 *
 *	P I O _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *	Create a new database file.
 *	NOTE:  This routine does not lock any mutexes on
 *	its own behalf.  It is assumed that mutexes will
 *	have been locked before entry.
 *
 **************************************/
	int status;
	TEXT *address, expanded_name[NAM$C_MAXRSS], temp[256];
	struct FAB fab;
	struct NAM nam;

	fab = cc$rms_fab;
	nam = cc$rms_nam;
	fab.fab$l_nam = &nam;
	nam.nam$l_rsa = expanded_name;
	nam.nam$b_rss = sizeof(expanded_name);
	// CVC: I assume the non-const condition would be needed, can't check.
	fab.fab$l_fna = const_cast<TEXT*>(string);
	fab.fab$b_fns = length;
	fab.fab$l_dna = DEFAULT_FILE_NAME;
	fab.fab$b_dns = sizeof(DEFAULT_FILE_NAME) - 1;
	fab.fab$l_fop = FAB$M_UFO;
	fab.fab$b_rfm = FAB$C_UDF;
	fab.fab$b_shr = FAB$M_SHRGET | FAB$M_SHRPUT | FAB$M_UPI;
	fab.fab$l_alq = 200;
	fab.fab$w_deq = 100;
	fab.fab$b_fac = FAB$M_UPD | FAB$M_PUT;
	status = sys$create(&fab);

	if (!(status & 1))
		ERR_post(isc_io_error,
				 isc_arg_string, "sys$create",
				 isc_arg_cstring, length, ERR_string(string, length),
				 isc_arg_gds, isc_io_create_err, isc_arg_vms, status, 0);

/* File open succeeded.  Now expand the file name. */

	return setup_file(dbb, expanded_name, nam.nam$b_rsl, fab.fab$l_stv, &nam);
}


int PIO_expand(const TEXT* file_name, USHORT file_length, TEXT* expanded_name)
{
/**************************************
 *
 *	P I O _ e x p a n d
 *
 **************************************
 *
 * Functional description
 *	Fully expand a file name.  If the file doesn't exist, do something
 *	intelligent.
 *
 **************************************/
	int length, status;
	TEXT *address, temp[NAM$C_MAXRSS], temp2[NAM$C_MAXRSS];
	struct FAB fab;
	struct NAM nam;

	length = ISC_expand_logical(file_name, file_length, expanded_name);

	fab = cc$rms_fab;
	nam = cc$rms_nam;
	fab.fab$l_nam = &nam;
	nam.nam$l_esa = temp;
	nam.nam$b_ess = sizeof(temp);
	nam.nam$l_rsa = temp2;
	nam.nam$b_rss = sizeof(temp2);
	fab.fab$l_fna = expanded_name;
	fab.fab$b_fns = length;
	fab.fab$l_dna = DEFAULT_FILE_NAME;
	fab.fab$b_dns = sizeof(DEFAULT_FILE_NAME) - 1;

	if ((sys$parse(&fab) & 1) && (sys$search(&fab) & 1)) {
		length = nam.nam$b_rsl;
		MOVE_FAST(temp2, expanded_name, length);
		expanded_name[length] = 0;
	}

	return length;
}


void PIO_flush(FIL file)
{
/**************************************
 *
 *	P I O _ f l u s h
 *
 **************************************
 *
 * Functional description
 *	Flush the operating system cache back to good, solid oxide.
 *
 **************************************/
}


void PIO_force_write(FIL file, USHORT flag)
{
/**************************************
 *
 *	P I O _ f o r c e _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Set (or clear) force write, if possible, for the database.
 *
 **************************************/
}


void PIO_header(DBB dbb, SCHAR * address, int length)
{
/**************************************
 *
 *	P I O _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *	Read the page header.  This assumes that the file has not been
 *	repositioned since the file was originally mapped.
 *
 **************************************/
	SSHORT iosb[4];
	int status, block;
	FIL file;

	file = dbb->dbb_file;

#ifdef ISC_DATABASE_ENCRYPTION
	if (dbb->dbb_encrypt_key) {
		SLONG spare_buffer[MAX_PAGE_SIZE / sizeof(SLONG)];

		status = sys$qio(EVENT_FLAG,	/* Event flag */
						 file->fil_desc,	/* Channel */
						 IO$_READVBLK,	/* Function */
						 iosb,	/* IO status block */
						 gds__completion_ast,	/* AST address */
						 NULL,	/* AST parameter */
						 spare_buffer,	/* P1 (buffer) */
						 (length & ~1),	/* P2 (length) */
						 1,		/* P3 (virtual block) */
						 NULL, NULL, NULL);

		if (status & 1)
			ISC_wait(iosb, EVENT_FLAG);

		if (!(status & 1) || !((status = iosb[0]) & 1))
			ERR_post(isc_io_error,
					 isc_arg_string, "QIO readvblk (get header)",
					 isc_arg_cstring, file->fil_length,
					 ERR_string(file->fil_string, file->fil_length),
					 isc_arg_gds, isc_io_read_err, isc_arg_vms, status, 0);

		(*dbb->dbb_decrypt) (dbb->dbb_encrypt_key->str_data,
							 spare_buffer, length, address);
	}
	else
#endif
	{
		status = sys$qio(EVENT_FLAG,	/* Event flag */
						 file->fil_desc,	/* Channel */
						 IO$_READVBLK,	/* Function */
						 iosb,	/* IO status block */
						 gds__completion_ast,	/* AST address */
						 NULL,	/* AST parameter */
						 address,	/* P1 (buffer) */
						 (length & ~1),	/* P2 (length) */
						 1,		/* P3 (virtual block) */
						 NULL, NULL, NULL);

		if (status & 1)
			ISC_wait(iosb, EVENT_FLAG);

		if (!(status & 1) || !((status = iosb[0]) & 1))
			ERR_post(isc_io_error,
					 isc_arg_string, "QIO readvblk (get header)",
					 isc_arg_cstring, file->fil_length,
					 ERR_string(file->fil_string, file->fil_length),
					 isc_arg_gds, isc_io_read_err, isc_arg_vms, status, 0);
	}
}


SLONG PIO_max_alloc(DBB dbb)
{
/**************************************
 *
 *	P I O _ m a x _ a l l o c
 *
 **************************************
 *
 * Functional description
 *	Compute last physically allocated page of database.
 *
 **************************************/
	USHORT length;
	FIL file;
	USHORT iosb[4];
	ATR$ atr;
	FIB$ fib;
	FAT$ fat;
	int status, size;
	struct dsc$descriptor_s desc;

/* Get last file block */

	for (file = dbb->dbb_file; file->fil_next; file = file->fil_next);

	ISC_make_desc(&fib, &desc, sizeof(fib));

	atr.atr$w_size = ATR$S_RECATTR;
	atr.atr$w_type = ATR$C_RECATTR;
	atr.atr$l_addr = &fat;
	atr.atr$l_zero = 0;

/* Get current size */

	MOVE_CLEAR(&fib, sizeof(fib));
	MOVE_FAST(file->fil_did, fib.fib$w_did, sizeof(fib.fib$w_did));
	MOVE_FAST(file->fil_fid, fib.fib$w_fid, sizeof(fib.fib$w_fid));
	fib.fib$w_nmctl = FIB$M_FINDFID;

	status = sys$qiow(EVENT_FLAG,	/* Event flag */
					  file->fil_desc,	/* Channel */
					  IO$_ACCESS,	/* Function */
					  iosb,		/* IO status block */
					  NULL,		/* AST address */
					  NULL,		/* AST parameter */
					  &desc,	/* P1 (buffer) */
					  NULL,		/* P2 (length) */
					  NULL,		/* P3 (virtual block) */
					  NULL,		/* P4 */
					  &atr,		/* P5 */
					  NULL);	/* P6 */

	if (!(status & 1) || !((status = iosb[0]) & 1))
		ERR_post(isc_io_error,
				 isc_arg_string, "QIO IO$_ACCESS",
				 isc_arg_cstring, file->fil_length,
				 ERR_string(file->fil_string, file->fil_length), isc_arg_gds,
				 isc_io_access_err, isc_arg_vms, status, 0);

	size = (fat.fat$w_hiblk[1] + (fat.fat$w_hiblk[0] << 16)) * 512;


	return file->fil_min_page - file->fil_fudge + size / dbb->dbb_page_size;
}



FIL PIO_open(DBB dbb,
			 const TEXT* string,
			 SSHORT length,
			 SSHORT trace_flag,
			 BLK connection, const TEXT* file_name, USHORT file_length)
{
/**************************************
 *
 *	P I O _ o p e n
 *
 **************************************
 *
 * Functional description
 *	Open a database file.  If a "connection" block is provided, use
 *	the connection to communication with a page/lock server.
 *
 **************************************/
	FIL file;
	int status;
	TEXT *address, expanded_name[NAM$C_MAXRSS], temp[256];
	struct FAB fab;
	struct NAM nam;

	fab = cc$rms_fab;
	nam = cc$rms_nam;
	fab.fab$l_nam = &nam;
	nam.nam$l_rsa = expanded_name;
	nam.nam$b_rss = sizeof(expanded_name);
	// CVC: I assume the non-const condition would be needed, can't check.
	fab.fab$l_fna = const_cast<TEXT*>(string);
	fab.fab$b_fns = length;
	fab.fab$l_dna = DEFAULT_FILE_NAME;
	fab.fab$b_dns = sizeof(DEFAULT_FILE_NAME) - 1;
	fab.fab$l_fop = FAB$M_UFO;
	fab.fab$b_rfm = FAB$C_UDF;
	fab.fab$b_shr = FAB$M_SHRGET | FAB$M_SHRPUT | FAB$M_UPI;
	fab.fab$b_fac = FAB$M_UPD | FAB$M_PUT;
	status = sys$open(&fab);

	if (!(status & 1))
		ERR_post(isc_io_error,
				 isc_arg_string, "sys$open",
				 isc_arg_cstring, file_length, ERR_string(file_name,
														  file_length),
				 isc_arg_gds, isc_io_open_err, isc_arg_vms, status, 0);

/* File open succeeded.  Now expand the file name. */

	file = setup_file(dbb, string, length, fab.fab$l_stv, &nam);
	MOVE_FAST(nam.nam$w_did, file->fil_did, sizeof(file->fil_did));
	MOVE_FAST(nam.nam$w_fid, file->fil_fid, sizeof(file->fil_fid));

#ifdef TRACE
	if (trace_flag)
		setup_trace(file, trace_open);
#endif

	return file;
}


int PIO_read(FIL file, BDB bdb, PAG page, ISC_STATUS* status_vector)
{
/**************************************
 *
 *	P I O _ r e a d
 *
 **************************************
 *
 * Functional description
 *	Read a data page.  Oh wow.
 *
 **************************************/
	SSHORT iosb[4];
	int status, block;
	DBB dbb;

	dbb = bdb->bdb_dbb;
	file = seek_file(file, bdb, &block);

#ifdef ISC_DATABASE_ENCRYPTION
	if (dbb->dbb_encrypt_key) {
		SLONG spare_buffer[MAX_PAGE_SIZE / sizeof(SLONG)];

		status = sys$qio(EVENT_FLAG,	/* Event flag */
						 file->fil_desc,	/* Channel */
						 IO$_READVBLK,	/* Function */
						 iosb,	/* IO status block */
						 gds__completion_ast,	/* AST address */
						 NULL,	/* AST parameter */
						 spare_buffer,	/* P1 (buffer) */
						 dbb->dbb_page_size,	/* P2 (length) */
						 block,	/* P3 (virtual block) */
						 NULL, NULL, NULL);

		if (status & 1)
			ISC_wait(iosb, EVENT_FLAG);

		(*dbb->dbb_decrypt) (dbb->dbb_encrypt_key->str_data,
							 spare_buffer, dbb->dbb_page_size, page);
	}
	else
#endif
	{
		status = sys$qio(EVENT_FLAG,	/* Event flag */
						 file->fil_desc,	/* Channel */
						 IO$_READVBLK,	/* Function */
						 iosb,	/* IO status block */
						 gds__completion_ast,	/* AST address */
						 NULL,	/* AST parameter */
						 page,	/* P1 (buffer) */
						 dbb->dbb_page_size,	/* P2 (length) */
						 block,	/* P3 (virtual block) */
						 NULL, NULL, NULL);

		if (status & 1)
			ISC_wait(iosb, EVENT_FLAG);
	}

	if (!(status & 1) || !((status = iosb[0]) & 1))
		return vms_io_error(status_vector, "QIO readvblk", isc_io_read_err,
							status, file);

#ifdef TRACE
	trace_event(file, trace_read, &bdb->bdb_page, sizeof(bdb->bdb_page));
#endif

	return TRUE;
}


int PIO_write(FIL file, BDB bdb, PAG page, ISC_STATUS * status_vector)
{
/**************************************
 *
 *	P I O _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Write a data page.  Oh wow.
 *
 **************************************/
	SSHORT iosb[4];
	int status, block;
	DBB dbb;

	dbb = bdb->bdb_dbb;
	file = seek_file(file, bdb, &block);

	for (;;) {
#ifdef ISC_DATABASE_ENCRYPTION
		if (dbb->dbb_encrypt_key) {
			SLONG spare_buffer[MAX_PAGE_SIZE / sizeof(SLONG)];

			(*dbb->dbb_encrypt) (dbb->dbb_encrypt_key->str_data,
								 page, dbb->dbb_page_size, spare_buffer);

			status = sys$qio(EVENT_FLAG,	/* Event flag */
							 file->fil_desc,	/* Channel */
							 IO$_WRITEVBLK,	/* Function */
							 iosb,	/* IO status block */
							 gds__completion_ast,	/* AST address */
							 NULL,	/* AST parameter */
							 spare_buffer,	/* P1 (buffer) */
							 dbb->dbb_page_size,	/* P2 (length) */
							 block,	/* P3 (virtual block) */
							 NULL, NULL, NULL);
		}
		else
#endif
		{
			status = sys$qio(EVENT_FLAG,	/* Event flag */
							 file->fil_desc,	/* Channel */
							 IO$_WRITEVBLK,	/* Function */
							 iosb,	/* IO status block */
							 gds__completion_ast,	/* AST address */
							 NULL,	/* AST parameter */
							 page,	/* P1 (buffer) */
							 dbb->dbb_page_size,	/* P2 (length) */
							 block,	/* P3 (virtual block) */
							 NULL, NULL, NULL);
		}

		if (status & 1)
			ISC_wait(iosb, EVENT_FLAG);

		if ((status & 1) && ((status = iosb[0]) & 1))
			break;

		if (status == SS$_ENDOFFILE) {
			if (!extend_file(file, status_vector))
				return FALSE;
		}
		else
			return vms_io_error(status_vector, "QIO writevblk",
								isc_io_write_err, status, file);
	}

#ifdef TRACE
	trace_event(file, trace_write, &bdb->bdb_page, sizeof(bdb->bdb_page));
#endif

	return TRUE;
}


static bool extend_file(FIL file, ISC_STATUS* status_vector)
{
/**************************************
 *
 *	e x t e n d _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Extend the length of a file.
 *
 **************************************/
	USHORT iosb[4], l;
	struct dsc$descriptor_s desc;
	ATR$ atr;
	FIB$ fib;
	FAT$ fat;
	int status, old_size, new_size, delta;
	SCHAR *p;

	ISC_make_desc(&fib, &desc, sizeof(fib));
	atr.atr$w_size = ATR$S_RECATTR;
	atr.atr$w_type = ATR$C_RECATTR;
	atr.atr$l_addr = &fat;
	atr.atr$l_zero = 0;

/* Get current size */

	p = &fib;
	l = sizeof(fib);
	do
		*p++ = 0;
	while (--l);

	MOVE_FAST(file->fil_did, fib.fib$w_did, sizeof(fib.fib$w_did));
	MOVE_FAST(file->fil_fid, fib.fib$w_fid, sizeof(fib.fib$w_fid));
	fib.fib$w_nmctl = FIB$M_FINDFID;

	status = sys$qiow(EVENT_FLAG,	/* Event flag */
					  file->fil_desc,	/* Channel */
					  IO$_ACCESS,	/* Function */
					  iosb,		/* IO status block */
					  NULL,		/* AST address */
					  NULL,		/* AST parameter */
					  &desc,	/* P1 (buffer) */
					  NULL,		/* P2 (length) */
					  NULL,		/* P3 (virtual block) */
					  NULL,		/* P4 */
					  &atr,		/* P5 */
					  NULL);	/* P6 */

	if (!(status & 1) || !((status = iosb[0]) & 1))
		return vms_io_error(status_vector, "QIO IO$_ACCESS",
							isc_io_access_err, status, file);

	old_size = fat.fat$w_hiblk[1] + (fat.fat$w_hiblk[0] << 16);
	new_size = old_size + MIN(old_size, 16000);
	delta = new_size - old_size;

/* Extend file */

	fib.fib$l_exsz = delta;
	fib.fib$l_exvbn = 0;
	fib.fib$l_acctl = FIB$M_WRITETHRU;
	fib.fib$w_exctl = FIB$M_EXTEND | FIB$M_ALDEF | FIB$M_ALCONB;

	fat.fat$w_efblk[0] = (new_size + 1) >> 16;
	fat.fat$w_efblk[1] = (new_size + 1);

	status = sys$qiow(EVENT_FLAG,	/* Event flag */
					  file->fil_desc,	/* Channel */
					  IO$_MODIFY,	/* Function */
					  iosb,		/* IO status block */
					  NULL,		/* AST address */
					  NULL,		/* AST parameter */
					  &desc,	/* P1 (buffer) */
					  NULL,		/* P2 (length) */
					  NULL,		/* P3 (virtual block) */
					  NULL,		/* P4 */
					  &atr,		/* P5 */
					  NULL);	/* P6 */

	if (!(status & 1) || !((status = iosb[0]) & 1))
		return vms_io_error(status_vector, "QIO IO$_MODIFY",
							isc_io_access_err, status, file);

	return true;
}


static FIL seek_file(FIL file, BDB bdb, int *block)
{
/**************************************
 *
 *	s e e k _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Given a buffer descriptor block, find the appropropriate
 *	file block and seek to the proper page in that file.
 *
 **************************************/
	ULONG page;
	DBB dbb;

	dbb = bdb->bdb_dbb;
	page = bdb->bdb_page;

	for (;; file = file->fil_next)
		if (!file)
			CORRUPT(158);		/* msg 158 database file not available */
		else if (page >= file->fil_min_page && page <= file->fil_max_page)
			break;

	page -= file->fil_min_page - file->fil_fudge;
	*block = 1 + (page * dbb->dbb_page_size) / 512;

	return file;
}


static FIL setup_file(DBB dbb,
					  const TEXT* file_name,
					  USHORT file_length, USHORT chan, struct NAM* nam)
{
/**************************************
 *
 *	s e t u p _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Set up file and lock blocks for a file.
 *
 **************************************/
	FIL file;
	LCK lock;
	UCHAR lock_id[64], devlock[64], *p, *q;
	USHORT l, iosb[4];
	ITM items[2];
	int status;

/* Allocate file block and move in file name */

	file = FB_NEW_RPT(dbb->dbb_permanent, file_length + 1) fil();
	file->fil_desc = chan;
	file->fil_length = file_length;
	file->fil_max_page = -1;
	MOVE_FAST(file_name, file->fil_string, file_length);
	file->fil_string[file_length] = '\0';
	V4_MUTEX_INIT(file->fil_mutex);

/* If this isn't the primary file, we're done */

	if (dbb->dbb_file)
		return file;

/* Get device lock string */

	items[0].itm_code = DVI$_DEVLOCKNAM;
	items[0].itm_length = sizeof(devlock);
	items[0].itm_buffer = devlock;
	items[0].itm_return_length = &l;

	items[1].itm_code = 0;
	items[1].itm_length = 0;
	items[1].itm_buffer = 0;

	status = sys$getdviw(15, chan, NULL, items, iosb, NULL, NULL, NULL);
	if (status != 1)
		ERR_post(isc_io_error,
				 isc_arg_string, "sys$getdviw",
				 isc_arg_cstring, file_length, ERR_string(file_name,
														  file_length),
				 isc_arg_gds, isc_io_access_err, isc_arg_vms, status, 0);

/* Build lock string */

	p = lock_id;
	q = devlock;
	if (l)
		do
			*p++ = *q++;
		while (--l);

	q = &nam->nam$w_fid;
	l = 6;
	do
		*p++ = *q++;
	while (--l);
	l = p - lock_id;

	dbb->dbb_lock = lock = FB_NEW_RPT(dbb->dbb_permanent, l) lck();
	lock->lck_type = LCK_database;
	lock->lck_owner_handle = LCK_get_owner_handle(NULL, lock->lck_type);
	lock->lck_object = reinterpret_cast<blk*>(dbb);
	lock->lck_dbb = dbb;
	lock->lck_length = l;
	lock->lck_ast = CCH_down_grade_dbb;
	MOVE_FAST(lock_id, lock->lck_key.lck_string, l);

	dbb->dbb_flags |= DBB_exclusive;
	if (!LCK_lock(NULL, lock, LCK_EX, LCK_NO_WAIT)) {
		dbb->dbb_flags &= ~DBB_exclusive;
		LCK_lock(NULL, lock,
				 (dbb->dbb_flags & DBB_cache_manager) ? LCK_SR : LCK_PW,
				 LCK_WAIT);
	}

	return file;
}


#ifdef TRACE
static void setup_trace(FIL file, SSHORT event)
{
/**************************************
 *
 *	s e t u p _ t r a c e
 *
 **************************************
 *
 * Functional description
 *	Perform setup to create trace file.
 *
 **************************************/

	file->fil_trace = ib_fopen("trace.log", "w");
	trace_event(file, event, file->fil_string, strlen(file->fil_string));
}


static void trace_event(FIL file, SSHORT type, SCHAR * ptr, SSHORT length)
{
/**************************************
 *
 *	t r a c e _ e v e n t
 *
 **************************************
 *
 * Functional description
 *	Write trace event to the trace file.
 *
 **************************************/

	if (!file->fil_trace)
		return;

	ib_putc(type, file->fil_trace);
	ib_putc(length, file->fil_trace);

	while (--length >= 0)
		ib_putc(*ptr++, file->fil_trace);
}
#endif


static bool vms_io_error(
							ISC_STATUS * status_vector,
							TEXT * string,
							ISC_STATUS operation, int code, FIL file)
{
/**************************************
 *
 *	v m s _ i o _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Report I/O error.
 *
 **************************************/

	*status_vector++ = isc_arg_gds;
	*status_vector++ = isc_io_error;
	*status_vector++ = isc_arg_string;
	*status_vector++ = (ISC_STATUS) string;
	*status_vector++ = isc_arg_string;
	*status_vector++ =
		(ISC_STATUS) ERR_string(file->fil_string, file->fil_length);
	*status_vector++ = isc_arg_gds;
	*status_vector++ = operation;
	*status_vector++ = isc_arg_vms;
	*status_vector++ = code;
	*status_vector++ = isc_arg_end;

	return false;
}


