/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ail.cpp
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
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#include "firebird.h"
#include <string.h>
#include "../jrd/jrd.h"
#include "../jrd/ods.h"
#include "../jrd/jrn.h"
#include "../jrd/tra.h"
#include "../jrd/pag.h"
#include "../jrd/os/pio.h"
#include "../jrd/dsc.h"
#include "../wal/wal.h"
#include "gen/iberror.h"
#include "../jrd/flags.h"
#include "../jrd/sbm.h"
#include "../jrd/sdw.h"
#include "../jrd/lls.h"
#include "../jrd/rse.h"
#include "../jrd/ail.h"
#include "../jrd/all_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/iberr_proto.h"
#include "../jrd/jrn_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/misc_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/os/pio_proto.h"
#include "../jrd/rec_proto.h"
#include "../jrd/sbm_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/tra_proto.h"
#include "../jrd/isc_f_proto.h"
#include "../wal/wal_proto.h"
#include "../wal/walc_proto.h"
#include "../wal/walf_proto.h"

#if HAVE_UNISTD_H
#include <unistd.h>
#endif


#ifdef NOT_USED_OR_REPLACED
static void build_wal_param(UCHAR *, LGFILE **, SLONG, LGFILE *, SLONG *);
#endif
static void delete_log_files(TEXT *, SLONG, LLS);
#ifdef NOT_USED_OR_REPLACED
static BOOLEAN get_walinfo(TEXT *);
#endif
static void initialize_wal(TDBB, TEXT *, WIN *, LIP, SSHORT, USHORT, SBM *);
#ifdef NOT_USED_OR_REPLACED
static void process_log_updater(LIP);
static void process_recovery(TDBB, TEXT *, WIN *, LIP *,
							 SSHORT, USHORT, SBM *);
static void set_first_user(LGFILE **, LIP, TEXT *);
#endif

#define MOVE_BYTE(x_from,x_to)	*x_to++ = *x_from++;


void AIL_add_log(void)
{
/**************************************
 *
 *      A I L _ a d d _ l o g
 *
 **************************************
 *
 * Functional description
 *	WAL is being enabled. 
 *
 **************************************/
	DBB dbb;
	WIN window;
	LIP logp;
	SCHAR db_name[MAXPATHLEN];
	SBM sbm_rec;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* 
 * remove the rollover flag in the log page.  Update the current file
 * and control points to point to new file!
 */

	CCH_flush(tdbb, (USHORT) FLUSH_ALL, 0);
	window.win_page = LOG_PAGE;
	window.win_flags = 0;
	logp = (LIP) CCH_FETCH(tdbb, &window, LCK_write, pag_log);
	CCH_MARK_MUST_WRITE(tdbb, &window);

	if (logp->log_flags & log_add) {
		CCH_exclusive(tdbb, LCK_EX, LCK_WAIT);
		strcpy(db_name, dbb->dbb_file->fil_string);

		logp->log_flags &= ~log_no_ail;
		logp->log_mod_tid = 0;
		logp->log_mod_tip = 0;
		AIL_init(db_name, 0, &window, 0, &sbm_rec);

		/* With WAL, database write should be async */
		PAG_set_force_write(dbb, FALSE);
		CCH_release_exclusive(tdbb);
	}

	CCH_RELEASE(tdbb, &window);
}


void AIL_checkpoint_finish(
						   ISC_STATUS * status_vector,
						   DBB dbb,
						   SLONG seq, TEXT * walname, SLONG p_off, SLONG off)
{
/**************************************
 *
 *	A I L _ c h e c k p o i n t _ f i n i s h
 *
 **************************************
 *
 * Functional description
 *	Finish a database checkpoint by flushing the OS cache
 *	and updating the control point on the database log page.
 *
 **************************************/
	WIN window;
	PAG log_page;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;

	PIO_flush(dbb->dbb_file);
	if (dbb->dbb_shadow)
		PIO_flush(dbb->dbb_shadow->sdw_file);

	AIL_upd_cntrl_pt(walname, (USHORT) strlen(walname), seq, off, p_off);

	window.win_page = LOG_PAGE;
	window.win_flags = 0;
	log_page = CCH_FETCH(tdbb, &window, LCK_write, pag_log);
	CCH_MARK(tdbb, &window);
	log_page->pag_checksum = CCH_checksum(window.win_bdb);
	PIO_write(dbb->dbb_file, window.win_bdb, window.win_buffer,
			  status_vector);

	CCH_write_all_shadows(tdbb, 0, window.win_bdb, status_vector, 1, FALSE);
	PIO_flush(dbb->dbb_file);
	if (dbb->dbb_shadow)
		PIO_flush(dbb->dbb_shadow->sdw_file);

	CCH_RELEASE(tdbb, &window);

/* Inform WAL writer that older log files can be reused */

	WAL_checkpoint_recorded(status_vector, dbb->dbb_wal);

/* Make sure there is a journal entry of next transaction ID */

	AIL_journal_tid();
}


void AIL_commit(SLONG number)
{
/**************************************
 *
 *	A I L _ c o m m i t
 *
 **************************************
 *
 * Functional description
 *	Write a commit record to WAL and participate in group commit.
 *	A 0 length will be written out to flush the WAL.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	LTJC commit_rec;
	SLONG seqno, offset;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

	if (!dbb->dbb_wal)
		return;

/* Prepare WAL message */

	MOVE_CLEAR((SLONG*)&commit_rec, LTJC_SIZE);

	commit_rec.ltjc_header.jrnh_type = JRN_COMMIT;
	commit_rec.ltjc_header.jrnh_length = LTJC_SIZE;
	commit_rec.ltjc_header.jrnh_version = JOURNAL_VERSION;

/* Write message to WAL */
	THREAD_EXIT;
	if (WAL_commit
		(tdbb->tdbb_status_vector, dbb->dbb_wal, (UCHAR *) & commit_rec,
		 /* LTJC_SIZE */ 0, &seqno, &offset) != FB_SUCCESS) {
		THREAD_ENTER;
		ERR_punt();
	}
	THREAD_ENTER;
}


void AIL_disable(void)
{
/**************************************
 *
 *	A I L _ d i s a b l e
 *
 **************************************
 *
 * Functional description
 *	Disable journalling, if enabled.
 * 	Delete journal related entries from header page.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	LTJC record;
	ULONG seqno;
	ULONG offset;
	TEXT journal_dir[MAXPATHLEN];
	UCHAR data[MAXPATHLEN];
	USHORT jd_len, d_len;
	SLONG ret_val;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

	PAG_get_clump(HEADER_PAGE, HDR_backup_info, &d_len, data);
	PAG_get_clump(HEADER_PAGE, HDR_journal_server, &jd_len,
				  reinterpret_cast < UCHAR * >(journal_dir));
	journal_dir[jd_len] = 0;

	if (!jd_len)
		ERR_post(isc_no_jrn, 0);

	if (!dbb->dbb_wal)
		ERR_post(isc_no_wal, 0);

	if ((ret_val = JRN_init(tdbb->tdbb_status_vector, &dbb->dbb_journal,
							dbb->dbb_page_size,
							reinterpret_cast < UCHAR * >(journal_dir), jd_len,
							data, d_len)) != FB_SUCCESS)
		AIL_process_jrn_error(ret_val);

	PAG_delete_clump_entry(HEADER_PAGE, HDR_journal_server);
	PAG_delete_clump_entry(HEADER_PAGE, HDR_backup_info);

	record.ltjc_header.jrnh_type = JRN_DISABLE;
	record.ltjc_page_size = 0;
	record.ltjc_length = d_len;
	record.ltjc_seqno = 0;
	record.ltjc_offset = 0;

	tdbb->tdbb_status_vector[1] = 0;
	AIL_put(dbb, tdbb->tdbb_status_vector,
			reinterpret_cast < jrnh * >(&record), LTJC_SIZE, data, d_len,
			(ULONG) 0, (ULONG) 0, &seqno, &offset);
	if (tdbb->tdbb_status_vector[1])
		ERR_punt();

	record.ltjc_seqno = seqno;
	record.ltjc_offset = offset;

/*
 * Inform WAL manager about disable journal.
 * The WAL manager will roll over to a new file.
 */

	if (WAL_journal_disable(tdbb->tdbb_status_vector, dbb->dbb_wal) !=
		FB_SUCCESS) ERR_punt();

	if (dbb->dbb_journal) {
		if ((ret_val = JRN_disable(tdbb->tdbb_status_vector, dbb->dbb_journal,
								   reinterpret_cast < jrnh * >(&record), data,
								   d_len)) != FB_SUCCESS)
			AIL_process_jrn_error(ret_val);
	}
}


void AIL_drop_log(void)
{
/**************************************
 *
 *      A I L _ d r o p _ l o g
 *
 **************************************
 *
 * Functional description
 *	Do things related to dropping WAL protocol.
 *	Flush cache, disable journaling, initialize log page,
 *	delete log files etc.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	WIN window;
	LIP logp;
	LLS stack = NULL;
	TEXT latest_logname[MAXPATHLEN], journal_dir[MAXPATHLEN];
	SLONG latest_log_p_offset;
	USHORT jd_len;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;
	if (!dbb->dbb_wal)
		return;

/* Get a protected write access to the database; only other
   process (thread) running during this step could be
   that of the cache manager. */

	CCH_exclusive(tdbb, LCK_PW, LCK_WAIT);

/* Flush the cache out so that the database on disk becomes
   consistent.  WAL is not going to help any more ! */

	CCH_flush(tdbb, (USHORT) FLUSH_ALL, 0);

/* journal has to be disabled before dropping WAL */

	if (PAG_get_clump
		(HEADER_PAGE, HDR_journal_server, &jd_len,
		 reinterpret_cast < UCHAR * >(journal_dir))) {
		AIL_disable();
	}

/* Now get a list of all the log files to be deleted. */

	WAL_status(tdbb->tdbb_status_vector, dbb->dbb_wal, NULL,
			   latest_logname, &latest_log_p_offset, NULL,
			   NULL, NULL, NULL, NULL);
	AIL_get_file_list(&stack);

/* Shutdown the WAL subsystem cleanly */

	WAL_set_cleanup_flag(dbb->dbb_wal);
	CCH_do_log_shutdown(tdbb, 1);

/* Turn on sync writes if WAL protocol is dropped */
	PAG_set_force_write(dbb, TRUE);

/* Now get rid of all the log files */

	delete_log_files(latest_logname, latest_log_p_offset, stack);

/* With WAL, transids are bumped up to optimize logging.  So 
   bring the next transaction id to the last bumped up value. */

	REC_update_next_transid();

/* Fetch the log page to reset WAL info on it */

	window.win_page = LOG_PAGE;
	window.win_flags = 0;
	logp = (LIP) CCH_FETCH(tdbb, &window, LCK_write, pag_log);
	CCH_MARK_MUST_WRITE(tdbb, &window);
	AIL_init_log_page(logp, logp->log_file.cp_seqno);
	CCH_RELEASE(tdbb, &window);

	CCH_release_exclusive(tdbb);
}


void AIL_drop_log_force(void)
{
/**************************************
 *
 *      A I L _ d r o p _ l o g _ f o r c e
 *
 **************************************
 *
 * Functional description
 *	Initialize log page to drop WAL protocol forcefully.
 *	May be log files are not available for recovery or they
 *	are corrupted.
 *
 **************************************/
	TDBB tdbb;
	WIN window;
	LIP logp;

	tdbb = GET_THREAD_DATA;


/* Get a protected write access to the database; only other
   process (thread) running during this step could be
   that of the cache manager. */

	CCH_exclusive(tdbb, LCK_PW, LCK_WAIT);

/* Fetch the log page to reset WAL info on it */

	window.win_page = LOG_PAGE;
	window.win_flags = 0;
	logp = (LIP) CCH_FETCH(tdbb, &window, LCK_write, pag_log);

	if (logp->log_flags & log_no_ail) {	/* WAL already disabled */
		CCH_RELEASE(tdbb, &window);
		CCH_release_exclusive(tdbb);
		return;
	}

/* With WAL, transids are bumped up to optimize logging.  So 
   bring the next transaction id to the last bumped up value. */

	REC_update_next_transid();

	CCH_MARK_MUST_WRITE(tdbb, &window);
	AIL_init_log_page(logp, logp->log_file.cp_seqno);
	CCH_RELEASE(tdbb, &window);

	CCH_release_exclusive(tdbb);
}


void AIL_enable(
				TEXT * journal_name,
				USHORT j_length, UCHAR * data, USHORT d_len, SSHORT archive)
{
/**************************************
 *
 *	A I L _ e n a b l e
 *
 **************************************
 *
 * Functional description
 *	Enable journalling for the database.
 *	Add entries to header page
 *	RETURNS	
 *		journal descriptor in ret_journal.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	LTJC jrecord;
	ULONG seqno;
	ULONG offset;
	LGFILE *log_files[MAX_LOG_FILES];
	LGFILE *log_ovflow;
	SLONG ret_val;
	ULONG number, i;
	USHORT jd_len;
	TEXT journal_dir[MAXPATHLEN];

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

/* Can enable journal only if wal sub system is in use */

	if (!dbb->dbb_wal)
		ERR_post(isc_no_wal_no_jrn, 0);

/* check if journal is already enabled */

	if (PAG_get_clump(HEADER_PAGE, HDR_journal_server, &jd_len,
					  reinterpret_cast <UCHAR *>(journal_dir)))
	{
		ERR_post(isc_jrn_present, 0);
	}

/*
 * check if we are trying to enable long term journaling without
 * specifying archive and we are using round robin.
 */

	if (!archive) {
		MET_get_walinfo(tdbb, log_files, &number, &log_ovflow);

		for (i = 0; i < number; i++)
			if (!(log_files[i]->lg_flags & LOG_serial)) {
				for (i = 0; i < number; i++)
					delete log_files[i];
				ERR_post(isc_no_archive, 0);
			}
	}

/* Put a enable record in the WAL and get the current seqno/offset pair */

	jrecord.ltjc_header.jrnh_type = JRN_ENABLE;
	jrecord.ltjc_page_size = dbb->dbb_page_size;
	jrecord.ltjc_length = d_len;
	jrecord.ltjc_seqno = 0;
	jrecord.ltjc_offset = 0;

	tdbb->tdbb_status_vector[1] = 0;
	AIL_put(dbb, tdbb->tdbb_status_vector,
			reinterpret_cast < jrnh * >(&jrecord), LTJC_SIZE, data, d_len,
			(ULONG) 0, (ULONG) 0, &seqno, &offset);

	if (tdbb->tdbb_status_vector[1])
		ERR_punt();

/* Inform the journal server about enable */

	jrecord.ltjc_seqno = seqno;
	jrecord.ltjc_offset = offset;

	if ((ret_val = JRN_enable(tdbb->tdbb_status_vector, &dbb->dbb_journal,
							  journal_name, j_length, data, d_len,
							  &jrecord)) != FB_SUCCESS)
		AIL_process_jrn_error(ret_val);

/* Inform wal subsystem about enable */

	if (WAL_journal_enable(tdbb->tdbb_status_vector, dbb->dbb_wal,
						   journal_name, d_len,
						   reinterpret_cast < char *>(data)) != FB_SUCCESS)
		  ERR_punt();

/* Add journal entries to header page */

	PAG_add_clump(HEADER_PAGE, HDR_journal_server, j_length,
				  reinterpret_cast < UCHAR * >(journal_name), CLUMP_ADD, 0);
	PAG_add_clump(HEADER_PAGE, HDR_backup_info, d_len, data, CLUMP_ADD, 1);
	WAL_flush(tdbb->tdbb_status_vector, dbb->dbb_wal,
			  reinterpret_cast < long *>(&seqno),
			  reinterpret_cast < long *>(&offset), false);
}


void AIL_fini(void)
{
/**************************************
 *
 *	A I L _ f i n i
 *
 **************************************
 *
 * Functional description
 *    Sign off with WAL substem and journal server (if needed).
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	SLONG ret_val;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

	if (!dbb->dbb_wal)
		return;

	WAL_fini(tdbb->tdbb_status_vector, &dbb->dbb_wal);

	dbb->dbb_wal = 0;

	if (!dbb->dbb_journal)
		return;

	if ((ret_val = JRN_fini(tdbb->tdbb_status_vector, &dbb->dbb_journal)) !=
		FB_SUCCESS) AIL_process_jrn_error(ret_val);

	dbb->dbb_journal = 0;
}


void AIL_get_file_list(LLS * stack)
{
/**************************************
 *
 *	A I L _ g e t _ f i l e _ l i s t
 *
 **************************************
 *
 * Functional description
 *    Get list of WAL files to drop
 *
 **************************************/
	DBB dbb;
	WALS WAL_segment;
	ISC_STATUS_ARRAY status_vector;
	SCHAR *curr_name;
	SLONG curr_log_partition_offset;
	SCHAR *prev_name;
	SLONG prev_log_partition_offset;
	SLONG log_flags;

	SCHAR log_name1[MAXPATHLEN];
	SCHAR log_name2[MAXPATHLEN];

	SCHAR *temp_name;
	SLONG log_seqno;
	SLONG log_length;
	STR fname;
	SCHAR *temp_fname;
	SSHORT count;
	SSHORT fname_term_length;
	LOGF *logf;

	dbb = GET_DBB;
	if (!dbb->dbb_wal)
		return;

	WALC_acquire(dbb->dbb_wal, &WAL_segment);

/* 
** Do we have round robin files defined.
** If yes then put them on stack of files
** to be deleted.
*/

	if ((count = WAL_segment->wals_max_logfiles) > 0) {
		while (--count >= 0) {
			logf = LOGF_INFO(count);
			if (logf->logf_flags & LOGF_RAW)
				continue;
			temp_fname = LOGF_NAME(logf);
			fname_term_length = strlen(temp_fname) + 1;
			fname = FB_NEW_RPT(*dbb->dbb_permanent, fname_term_length) str();
			MOVE_FAST(temp_fname, (SCHAR*)fname->str_data, fname_term_length);
			LLS_PUSH(fname, stack);
		}
	}
/* 
** Now check for overflow files or serial files.
** Start from the current logfile and retrieve existing
** log file names which need to be deleted.
*/

/* WAL_segment->wals_logname is the current log file */
	curr_name = log_name1;
	prev_name = log_name2;
	strcpy(curr_name, WAL_segment->wals_logname);
	curr_log_partition_offset = WAL_segment->wals_log_partition_offset;

	WALC_release(dbb->dbb_wal);

	if (WALF_get_log_info(status_vector, dbb->dbb_file->fil_string, curr_name,
						  curr_log_partition_offset, &log_seqno, &log_length,
						  &log_flags) != FB_SUCCESS) {
		gds__free((SLONG *) log_name1);
		gds__free((SLONG *) log_name2);
		return;
	}

	while (TRUE) {
		if (!(log_flags & WALFH_RAW)) {
			fname_term_length = strlen(curr_name) + 1;
			fname = FB_NEW_RPT(*dbb->dbb_permanent, fname_term_length) str();
			MOVE_FAST(curr_name, (SCHAR*)fname->str_data, fname_term_length);
			LLS_PUSH(fname, stack);
		}

		if (WALF_get_next_log_info(status_vector, dbb->dbb_file->fil_string,
								   curr_name, curr_log_partition_offset,
								   prev_name, &prev_log_partition_offset,
								   &log_seqno, &log_length, &log_flags,
								   -1) != FB_SUCCESS) break;

		temp_name = prev_name;
		prev_name = curr_name;
		curr_name = temp_name;
		curr_log_partition_offset = prev_log_partition_offset;
	}
}


void AIL_init(
			  TEXT * filename,
			  SSHORT file_len,
			  WIN * in_win, USHORT activate_shadow, SBM * sbm_rec)
{
/**************************************
 *
 *	A I L _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Init WAL handle at database attachment time.
 *	If this is the first attachment, read logfile information and
 *	start WAL subsystem.
 *	If WAL has never been used for the database, do some set up work.
 *	Perform short term recovery if required.
 **************************************/
	TDBB tdbb;
	DBB dbb;
	WIN *win, window;
	LIP logp;
	TEXT dbname[MAXPATHLEN];

/* null out the sparse bit map */

	*sbm_rec = (SBM) 0;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

/* Get WAL file name */

	dbb->dbb_wal = 0;

	if (file_len) {
		MOVE_FAST(filename, dbname, file_len);
		dbname[file_len] = 0;
	}
	else
		strcpy(dbname, filename);

/* 
 * The first user has to initialize the WAL subsystem after doing
 * some checks.
 */

	if (in_win) {
		win = in_win;
		win->win_flags = 0;
		logp = (LIP) win->win_buffer;
	}
	else {
		window.win_page = LOG_PAGE;
		window.win_flags = 0;
		logp = (LIP) CCH_FETCH(tdbb, &window, LCK_write, pag_log);
		win = &window;
	}

	if (CCH_exclusive(tdbb, LCK_EX, LCK_NO_WAIT)) {
		initialize_wal(tdbb, dbname, win, logp, 1, activate_shadow, sbm_rec);
		if (!in_win)
			CCH_RELEASE(tdbb, win);
		return;
	}

/*
 * Handle the case where the process doing short term recovery aborted.
 * Anyone getting the log page in exclusive access will restart recovery.
 * During recovery, we have to ensure that the log_rec_in_progress is not
 * reset.
 */

	if ((logp->log_flags & log_recover)
		&& (logp->log_flags & log_rec_in_progress)) {
		initialize_wal(tdbb, dbname, win, logp, 0, activate_shadow, sbm_rec);
		if (!in_win)
			CCH_RELEASE(tdbb, win);
		return;
	}

	if (logp->log_flags & log_no_ail) {
		if (!in_win)
			CCH_RELEASE(tdbb, win);
		return;
	}

	if (!in_win)
		CCH_RELEASE(tdbb, win);

	dbb->dbb_wal = 0;

/* If not the first user, call WAL_attach () */

	if (WAL_attach(tdbb->tdbb_status_vector, &dbb->dbb_wal, dbname) !=
		FB_SUCCESS) ERR_punt();
}


void AIL_init_log_page(LIP logp, SLONG seqno)
{
/**************************************
 *
 *	A I L _ i n i t _ l o g _ p a g e
 *
 **************************************
 *
 * Functional description
 *	(Re-)initialize the passed log page.
 *      Set all parameters to 0 except the log file
 *      sequence number which is set to the passed value.   
 **************************************/
	UCHAR *p;
	SLONG len;

/* First zero out the whole structure except for the page header portion */

	len = sizeof(log_info_page) - sizeof(pag);
	p = (UCHAR *) logp + sizeof(pag);
	do
		*p++ = 0;
	while (--len);

/* Now set individual fields as appropriate */

	logp->log_flags = log_no_ail;
	logp->log_file.cp_seqno = seqno;

	p = logp->log_data;

/* Set control point 1 file name */

	*p++ = LOG_ctrl_file1;
	*p++ = CTRL_FILE_LEN;
	len = CTRL_FILE_LEN;
	do
		*p++ = 0;
	while (--len);

/* Set control point 2 file name */

	*p++ = LOG_ctrl_file2;
	*p++ = CTRL_FILE_LEN;
	len = CTRL_FILE_LEN;
	do
		*p++ = 0;
	while (--len);

/* Set current log file */

	*p++ = LOG_logfile;
	*p++ = CTRL_FILE_LEN;
	len = CTRL_FILE_LEN;
	do
		*p++ = 0;
	while (--len);

	*p = LOG_end;

	logp->log_end = (USHORT) (p - (UCHAR *) logp);
}


void AIL_journal_tid(void)
{
/**************************************
 *
 *	A I L _ j o u r n a l _ t i d
 *
 **************************************
 *
 * Functional description
 *	Journal the next transaction id on the header page.
 *	This will be used at WAL_init and control point time.
 **************************************/
	WIN window;
	JRNDH journal;

	TDBB tdbb = GET_THREAD_DATA;
	DBB  dbb  = tdbb->tdbb_database;

	window.win_page = HEADER_PAGE;
	window.win_flags = 0;
	HDR hdr = (HDR) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
	CCH_MARK_MUST_WRITE(tdbb, &window);

	SLONG fake_tid = hdr->hdr_next_transaction + MOD_START_TRAN;

	SLONG seqno = hdr->hdr_next_transaction / dbb->dbb_pcontrol->pgc_tpt;
	if (seqno != (SLONG)((fake_tid + 1) / dbb->dbb_pcontrol->pgc_tpt))
	{
		fake_tid = ((seqno + 1) * dbb->dbb_pcontrol->pgc_tpt) - 1;
	}

	hdr->hdr_bumped_transaction = fake_tid;

	journal.jrndh_type = JRNP_DB_HEADER;
	journal.jrndh_nti = fake_tid;
	journal.jrndh_oit = hdr->hdr_oldest_transaction;
	journal.jrndh_oat = hdr->hdr_oldest_active;

	CCH_journal_record(tdbb, &window, reinterpret_cast < UCHAR * >(&journal),
					   JRNDH_SIZE, 0, 0);

	CCH_RELEASE(tdbb, &window);

	SLONG offset;
	WAL_flush(tdbb->tdbb_status_vector, dbb->dbb_wal, &seqno, &offset, false);
}


void AIL_process_jrn_error(SLONG ret_val)
{
/**************************************
 *
 *	A I L _ p r o c e s s _ j r n _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Handle error from a journal call.
 *
 **************************************/

	if (ret_val == FB_FAILURE)
		ERR_punt();
	else if (ret_val < 0)
		BUGCHECK(-1 * ret_val);
	else
		IBERROR(ret_val);
}


void AIL_put(
			 DBB dbb,
			 ISC_STATUS * status,
			 JRNH * header,
			 USHORT h_length,
			 UCHAR * data,
USHORT d_length,
ULONG prev_seqno, ULONG prev_offset, ULONG * seqno, ULONG * offset)
{
/**************************************
 *
 *	A I L _ p u t
 *
 **************************************
 *
 * Functional description
 *	Write a record to WAL.
 *	Returns seqno & offset pair.
 *
 *	NOTE:  AIL_put () can be called from signal handlers to write 
 *	       pages.  So ERR_punt () cannot be called from within the routine
 *	       Check status [1] on return.
 **************************************/

	if (!dbb->dbb_wal) {
		*seqno = prev_seqno;
		*offset = prev_offset;
		return;
	}

	switch (header->jrnh_type) {
	case JRN_CNTRL_PT:
	case JRN_START_ONLINE_DMP:
	case JRN_END_ONLINE_DMP:
	case JRN_ONLINE_DMP_FILE:
	case JRN_WAL_NAME:
		MOV_time_stamp(reinterpret_cast <
					   ISC_TIMESTAMP * >(((LTJW *) header)->ltjw_date));
		break;
	}

/* Prepare WAL message */

	header->jrnh_handle = 0;
	header->jrnh_length = h_length + d_length;
	header->jrnh_series = 0;
	header->jrnh_version = JOURNAL_VERSION;

	header->jrnh_prev_seqno = prev_seqno;
	header->jrnh_prev_offset = prev_offset;

#ifdef DEV_BUILD
/* Checksum the log record after zeroing out the field */

	header->jrnh_series =
		MISC_checksum_log_rec(reinterpret_cast < UCHAR * >(header), h_length,
							  data, d_length);
#endif /* DEV_BUILD */

/* Write message to WAL */

	THREAD_EXIT;
	WAL_put(status,
			dbb->dbb_wal,
			(UCHAR *) header,
			h_length,
			data,
			d_length,
			reinterpret_cast < long *>(seqno),
			reinterpret_cast < long *>(offset));
	THREAD_ENTER;
}


void AIL_recover_page(SLONG page_no, PAG page)
{
/**************************************
 *
 *	A I L _ r e c o v e r _ p a g e
 *
 **************************************
 *
 * Functional description
 *	recovers a single page from the second last control point.
 *	assumes that the 'corrupt' page is not in the cache.
 **************************************/
	TDBB tdbb;
	DBB dbb;
	WIN window;
	LIP logp;
	CP cp1;
	UCHAR *p;
	SCHAR rwal[MAXPATHLEN];
	SLONG seqno, offset;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

/* Flush the records on WAL buffer before trying to recover the
 * page
 */

	WAL_flush(tdbb->tdbb_status_vector, dbb->dbb_wal, &seqno, &offset, false);

	window.win_page = LOG_PAGE;
	window.win_flags = 0;
	logp = (LIP) CCH_FETCH(tdbb, &window, LCK_read, pag_log);

/* If there is no log, just return */

	if (logp->log_flags & log_no_ail) {
		CCH_RELEASE(tdbb, &window);
		return;
	}

	cp1.cp_seqno = logp->log_cp_1.cp_seqno;
	cp1.cp_offset = logp->log_cp_1.cp_offset;
	cp1.cp_p_offset = logp->log_cp_1.cp_p_offset;

	for (p = logp->log_data; (*p != LOG_ctrl_file1); p += 2 + p[1]);

	MOVE_FAST((SCHAR*)(p + 2), rwal, logp->log_cp_1.cp_fn_length);

	rwal[logp->log_cp_1.cp_fn_length] = 0;

	CCH_RELEASE(tdbb, &window);

	REC_recover_page(dbb->dbb_file->fil_string, rwal, &cp1, page_no, page);
}


void AIL_set_log_options(
						 SLONG chkpt_len,
						 SSHORT num_bufs, USHORT bufsize, SLONG grp_cmt_wait)
{
/**************************************
 *
 *	A I L _ s e t _ l o g _ o p t i o n s
 *
 **************************************
 *
 * Functional description
 *	Set any log options if specified.
 *
 **************************************/
	WIN window;
	TDBB tdbb;
	DBB dbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

/* why fetch log page if you don't need it */
	if (!(chkpt_len || num_bufs || bufsize || (grp_cmt_wait >= 0)))
		return;

	window.win_page = LOG_PAGE;
	window.win_flags = 0;
	CCH_FETCH(tdbb, &window, LCK_write, pag_log);

	if (chkpt_len) {
		PAG_add_clump(LOG_PAGE, LOG_chkpt_len, sizeof(SLONG),
					  (UCHAR *) & chkpt_len, CLUMP_REPLACE, 0);
		if (dbb->dbb_wal)
			WAL_set_checkpoint_length(tdbb->tdbb_status_vector, dbb->dbb_wal,
									  chkpt_len);
	}

	if (num_bufs)
		PAG_add_clump(LOG_PAGE, LOG_num_bufs, sizeof(SSHORT),
					  (UCHAR *) & num_bufs, CLUMP_REPLACE, 0);

	if (bufsize)
		PAG_add_clump(LOG_PAGE, LOG_bufsize, sizeof(USHORT),
					  (UCHAR *) & bufsize, CLUMP_REPLACE, 0);

	if (grp_cmt_wait >= 0) {
		PAG_add_clump(LOG_PAGE, LOG_grp_cmt_wait, sizeof(SLONG),
					  (UCHAR *) & grp_cmt_wait, CLUMP_REPLACE, 0);
		if (dbb->dbb_wal)
			WAL_set_grpc_wait_time(tdbb->tdbb_status_vector, dbb->dbb_wal,
								   grp_cmt_wait);
	}
	CCH_RELEASE(tdbb, &window);
}


void AIL_shutdown(
				  SCHAR * walname,
				  SLONG * seqno,
				  SLONG * offset, SLONG * p_offset, SSHORT force_archive)
{
/**************************************
 *
 *	A I L _ s h u t d o w n
 *
 **************************************
 *
 * Functional description
 *	Shutdown wal subsystem after a flush.
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

	WAL_flush(tdbb->tdbb_status_vector, dbb->dbb_wal, seqno, offset, false);
	WAL_shutdown(tdbb->tdbb_status_vector, dbb->dbb_wal, seqno, walname,
				 p_offset, offset, (force_archive));
	AIL_fini();
}


void AIL_upd_cntrl_pt(
					  TEXT * walname,
					  USHORT w_len, ULONG seqno, ULONG offset, ULONG p_offset)
{
/***********************************************
 *
 *	A I L _  u p d _ c n t r l _  p t
 *
 ***********************************************
 *
 * Functional description
 *	Updates the log page control point.
 *	Makes the 1st control point the second control point
 *	Updates the second control point with the new values.
 *
 *	Assumption:: All control point entries are in one page
 *		  :: All control point entries are contiguous.
 *
 **************************************/
	LIP logp;
	WIN window;
	UCHAR *p1, *p2, *p3, *q, *p;
	USHORT len;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;


	window.win_page = LOG_PAGE;
	window.win_flags = 0;
	logp = (LIP) CCH_FETCH(tdbb, &window, LCK_write, pag_log);
	CCH_MARK_MUST_WRITE(tdbb, &window);

	p1 = p2 = p3 = 0;

	for (p = logp->log_data; (*p != LOG_end); p += 2 + p[1]) {
		if (*p == LOG_ctrl_file1)
			p1 = p;

		if (*p == LOG_ctrl_file2)
			p2 = p;

		if (*p == LOG_logfile)
			p3 = p;

		if (p1 && p2 && p3)
			break;
	}

	if (*p == LOG_end)
		BUGCHECK(269);

/* update the second last entry */

	p = p2;
	*p = LOG_ctrl_file1;

	logp->log_cp_1.cp_seqno = logp->log_cp_2.cp_seqno;
	logp->log_cp_1.cp_offset = logp->log_cp_2.cp_offset;
	logp->log_cp_1.cp_p_offset = logp->log_cp_2.cp_p_offset;
	logp->log_cp_1.cp_fn_length = logp->log_cp_2.cp_fn_length;

/* add the new entries */

	p = p1;
	*p++ = LOG_ctrl_file2;
	p++;
	q = reinterpret_cast < UCHAR * >(walname);
	if ( (len = w_len) )
		do
			*p++ = *q++;
		while (--len);

	logp->log_cp_2.cp_seqno = seqno;
	logp->log_cp_2.cp_offset = offset;
	logp->log_cp_2.cp_p_offset = p_offset;
	logp->log_cp_2.cp_fn_length = w_len;

/* update the current file also */

	p = p3 + 2;
	q = reinterpret_cast < UCHAR * >(walname);
	if ( (len = w_len) )
		do
			*p++ = *q++;
		while (--len);

	logp->log_file.cp_seqno = seqno;
	logp->log_file.cp_offset = offset;
	logp->log_file.cp_p_offset = p_offset;
	logp->log_file.cp_fn_length = w_len;

	CCH_RELEASE(tdbb, &window);
}

#ifdef NOT_USED_OR_REPLACED
static void build_wal_param(
							UCHAR * wpb,
							LGFILE ** log_files,
							SLONG number,
							LGFILE * log_ovflow, SLONG * wpb_len)
{
/**************************************
 *
 *      b u i l d _ w a l _ p a r a m
 *
 **************************************
 *
 * Functional description
 *	Build the wal parameter block and return length
 *
 **************************************/
	ULONG param1;
	USHORT param2;
	USHORT plen;
	TEXT journal_dir[MAXPATHLEN];
	USHORT jd_len, d_len;
	UCHAR data[MAXPATHLEN], *p;

/* Get journal information, if any */

	PAG_get_clump(HEADER_PAGE, HDR_backup_info, &d_len, data);
	PAG_get_clump(HEADER_PAGE, HDR_journal_server, &jd_len,
				  reinterpret_cast < UCHAR * >(journal_dir));
	journal_dir[jd_len] = 0;

/* Build the wal param block */

	p = wpb;

	if (jd_len) {
		p += MISC_build_parameters_block(p,
										 PARAM_BYTE(WAL_PARAM_JRN_DIRNAME),
										 PARAM_STRING(journal_dir),
										 PARAM_BYTE(WAL_PARAM_JRN_DATA),
										 PARAM_NBYTES(d_len, data),
										 (SCHAR) 0);
	}

	if (log_files[0]->lg_flags & LOG_serial) {
		p += MISC_build_parameters_block(p,
										 PARAM_BYTE(WAL_PARAM_SERIAL_LOG),
										 PARAM_POINTER(log_files[0]),
										 (SCHAR) 0);
	}
	else {
		p += MISC_build_parameters_block(p,
										 PARAM_BYTE(WAL_PARAM_RR_LOGS_COUNT),
										 PARAM_SHORT(number),
										 PARAM_BYTE(WAL_PARAM_RR_LOGS),
										 PARAM_POINTER(log_files), (SCHAR) 0);
	}

	if (log_ovflow)
		p += MISC_build_parameters_block(p,
										 PARAM_BYTE(WAL_PARAM_OVFLOW_LOG),
										 PARAM_POINTER(log_ovflow),
										 (SCHAR) 0);


	if (PAG_get_clump
		(LOG_PAGE, LOG_chkpt_len, &plen,
		 reinterpret_cast < UCHAR * >(&param1))) {
		p +=
			MISC_build_parameters_block(p, PARAM_BYTE(WAL_PARAM_CKPT_INTRVL),
										PARAM_LONG(param1), (SCHAR) 0);
	}

	if (PAG_get_clump
		(LOG_PAGE, LOG_num_bufs, &plen,
		 reinterpret_cast < UCHAR * >(&param2))) {
		p +=
			MISC_build_parameters_block(p, PARAM_BYTE(WAL_PARAM_BUF_COUNT),
										PARAM_SHORT(param2), (SCHAR) 0);
	}

	if (PAG_get_clump
		(LOG_PAGE, LOG_bufsize, &plen,
		 reinterpret_cast < UCHAR * >(&param2))) {
		p +=
			MISC_build_parameters_block(p, PARAM_BYTE(WAL_PARAM_BUF_LEN),
										PARAM_SHORT(param2), (SCHAR) 0);
	}

	if (PAG_get_clump
		(LOG_PAGE, LOG_grp_cmt_wait, &plen,
		 reinterpret_cast < UCHAR * >(&param1))) {
		p +=
			MISC_build_parameters_block(p,
										PARAM_BYTE(WAL_PARAM_GRPC_WAIT_USECS),
										PARAM_LONG(param1), (SCHAR) 0);
	}

	p += MISC_build_parameters_block(p, PARAM_BYTE(WAL_PARAM_END), (SCHAR) 0);

	*wpb_len = p - wpb;
}
#endif

static void delete_log_files(
							 TEXT * latest_logname,
							 SLONG latest_log_p_offset,
							 LLS stack)
{
/**************************************
 *
 *      d e l e t e _ l o g _ f i l e s
 *
 **************************************
 *
 * Functional description
 *      Delete the log files listed in the stack.  First give
 *	some time to the journal server, if configured, to archive
 *	the last log file(s).
 *
 **************************************/
	TDBB tdbb;
	DBB dbb;
	STR fname;
	ISC_STATUS_ARRAY local_status;

	tdbb = GET_THREAD_DATA;
	dbb = tdbb->tdbb_database;

	while (stack) {
		fname = (STR) LLS_POP(&stack);
		if (unlink(reinterpret_cast < const char *>(fname->str_data))) {
			IBERR_build_status(local_status, isc_io_error,
							   isc_arg_string, "unlink",
							   isc_arg_string, fname->str_data,
							   isc_arg_gds, isc_io_delete_err, 0);
			gds__log_status(dbb->dbb_file->fil_string, local_status);
		}
	}
}

#ifdef NOT_USED_OR_REPLACED
static BOOLEAN get_walinfo(TEXT * walname)
{
/**************************************
 *
 *      g e t _ w a l i n f o
 *
 **************************************
 *
 * Functional description
 *      Get wal name and wal number from header page.
 *	Build the log name the way subsystem understands it.
 *      RETURNS
 *	    TRUE if WAL exists
 *	    FALSE otherwise
 *
 **************************************/
	USHORT len;
	WIN window;
	LIP logp;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;

	window.win_page = LOG_PAGE;
	window.win_flags = 0;
	logp = (LIP) CCH_FETCH(tdbb, &window, LCK_read, pag_log);

	if (!PAG_get_clump
		(LOG_PAGE, LOG_logfile, &len,
		 reinterpret_cast < UCHAR * >(walname))) {
		CCH_RELEASE(tdbb, &window);
		return FALSE;
	}

	len = logp->log_file.cp_fn_length;
	walname[len] = 0;

	CCH_RELEASE(tdbb, &window);

	return TRUE;
}
#endif

static void initialize_wal(
						   TDBB tdbb,
						   TEXT * dbname,
						   WIN * window,
						   LIP logp,
						   SSHORT release,
						   USHORT activate_shadow,
						   SBM * sbm_rec)
{
/**************************************
 *
 *      i n i t i a l i z e _ w a l
 *
 **************************************
 *
 * Functional description
 *	Initialize wal subsystem.
 *	If release is true, release log page before recovery.
 *	logp can be modified inside this procedure, so do not use it
 *	after it is called.  The page is released.
 *
 **************************************/
	SET_TDBB(tdbb);

/* Get WAL file name */

	tdbb->tdbb_database->dbb_wal = 0;

/* If there is no log, just return */

	if ((logp->log_flags & log_no_ail) && (!(logp->log_flags & log_add))) {
		/* if partial recovery is in progress, reset bit */

		if (logp->log_flags & log_partial_rebuild) {
			CCH_MARK_MUST_WRITE(tdbb, window);
			logp->log_flags &= ~log_partial_rebuild;
		}

		return;
	}
}

#ifdef NOT_USED_OR_REPLACED
static void process_log_updater(LIP logp)
{
/**************************************
 *
 *      p r o c e s s _ l o g _ u p d a t e r
 *
 **************************************
 *
 * Functional description
 *	Check if the process which updated the log page committed.
 *	Fixup the log_flags appropriately.
 *
 **************************************/
	SSHORT state;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;


	state = TRA_get_state(tdbb, logp->log_mod_tid);

	logp->log_mod_tid = 0;
	logp->log_mod_tip = 0;

	if (state == tra_committed) {
		if (logp->log_flags & log_delete) {
			logp->log_flags &= ~log_delete;
			logp->log_flags |= log_no_ail;
		}
		if (logp->log_flags & log_add) {
			logp->log_flags &= ~log_no_ail;
		}
	}
	else {
		logp->log_flags &= ~(log_add | log_delete);
	}
}


static void process_recovery(
							 TDBB tdbb,
							 TEXT * dbname,
							 WIN * window,
							 LIP * logpp,
							 SSHORT release,
USHORT activate_shadow, SBM * sbm_rec)
{
/**************************************
 *
 *      p r o c e s s _ r e c o v e r y
 *
 **************************************
 *
 * Functional description
 *	Do short term recovery.
 *	If release flag is set, release LOG_PAGE before recovery and 
 *	fetch after recovery.  Holding the page will act as a lock
 *	to prevent multiple users from running recovery.  The first
 *	user will run in exclusive mode.
 *
 **************************************/
	TEXT root_db[MAXPATHLEN], rwal[MAXPATHLEN];
	CP cp1;
	LIP logp;
	UCHAR *p;
	WIN win;
	HDR hdr;

	SET_TDBB(tdbb);

	logp = *logpp;

	cp1.cp_seqno = logp->log_cp_1.cp_seqno;
	cp1.cp_offset = logp->log_cp_1.cp_offset;
	cp1.cp_p_offset = logp->log_cp_1.cp_p_offset;

	for (p = logp->log_data; (*p != LOG_ctrl_file1); p += 2 + p[1]);

	MOVE_FAST((SCHAR*)(p + 2), rwal, logp->log_cp_1.cp_fn_length);

	rwal[logp->log_cp_1.cp_fn_length] = 0;

	if (release) {
		/* Page is already marked must_write */
		logp->log_flags |= log_rec_in_progress;
		CCH_RELEASE(tdbb, window);
	}

	if (activate_shadow) {
		/* If activating shadow, recovery needs root file name */
		win.win_page = HEADER_PAGE;
		win.win_flags = 0;
		hdr = (HDR) CCH_FETCH(tdbb, &win, LCK_read, pag_header);
		for (p = hdr->hdr_data; (*p != HDR_root_file_name); p += 2 + p[1]);
		MOVE_FAST((SCHAR*)(p + 2), root_db, p[1]);
		root_db[p[1]] = 0;
		CCH_RELEASE(tdbb, &win);
		REC_recover(root_db, rwal, &cp1, sbm_rec, activate_shadow);
	}
	else
		REC_recover(dbname, rwal, &cp1, sbm_rec, activate_shadow);

	if (release) {
		logp = (LIP) CCH_FETCH(tdbb, window, LCK_write, pag_log);
		CCH_MARK_MUST_WRITE(tdbb, window);
	}

/* Since a recovery updates the LOG_PAGE, make sure it will be
 * written to the shadows also
 */

	SBM_set(tdbb, sbm_rec, LOG_PAGE);

	logp->log_flags &= ~log_rec_in_progress;

/* If a shadow is being activated, roll over log file */

	if (activate_shadow)
		logp->log_flags |= log_add;

	*logpp = logp;
}


static void set_first_user(LGFILE ** log_files, LIP logp, TEXT * walname)
{
/**************************************
 *
 *      s e t _ f i r s t _ u s e r
 *
 **************************************
 *
 * Functional description
 *	Initialize log page when using the WAL for the first time.
 *
 **************************************/
	SSHORT len;
	UCHAR *p;

	logp->log_flags &= ~log_add;
	logp->log_flags &= ~log_no_ail;
	logp->log_file.cp_seqno++;
	MOV_time_stamp(reinterpret_cast <
				   ISC_TIMESTAMP * >(logp->log_creation_date));

	if (log_files[0]->lg_flags & LOG_serial) {
		WALC_build_logname(walname, log_files[0]->lg_name,
						   logp->log_file.cp_seqno);
	}
	else
		strcpy(walname, log_files[0]->lg_name);

	len = strlen(walname);
	logp->log_file.cp_fn_length = len;

	for (p = logp->log_data; (*p != LOG_end); p += 2 + p[1]) {
		switch (*p) {
		case LOG_ctrl_file1:
			MOVE_FAST(walname, (SCHAR*)(p + 2), len);
			break;

		case LOG_ctrl_file2:
			MOVE_FAST(walname, (SCHAR*)(p + 2), len);
			break;

		case LOG_logfile:
			MOVE_FAST(walname, (SCHAR*)(p + 2), len);
			break;
		}
	}

	logp->log_cp_1.cp_fn_length = len;
	logp->log_cp_1.cp_seqno = logp->log_file.cp_seqno;
	logp->log_cp_1.cp_offset = 0;
	logp->log_cp_1.cp_p_offset = 0;

	logp->log_cp_2.cp_fn_length = len;
	logp->log_cp_2.cp_seqno = logp->log_file.cp_seqno;
	logp->log_cp_2.cp_offset = 0;
	logp->log_cp_2.cp_p_offset = 0;
}
#endif
