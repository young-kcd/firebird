/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		jrd.cpp
 *	DESCRIPTION:	User visible entrypoints
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
 * 2001.07.06 Sean Leyne - Code Cleanup, removed "#ifdef READONLY_DATABASE"
 *                         conditionals, as the engine now fully supports
 *                         readonly databases.
 * 2001.07.09 Sean Leyne - Restore default setting to Force Write = "On", for
 *                         Windows NT platform, for new database files. This was changed
 *                         with IB 6.0 to OFF and has introduced many reported database
 *                         corruptions.
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "firebird.h"
#include "../jrd/common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../jrd/common.h"
#include "../jrd/thd.h"
#include "../jrd/os/thd_priority.h"
#include <stdarg.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_PWD_H
#include <pwd.h>
#endif
#include <errno.h>

#include "../jrd/ibase.h"
#include "../jrd/jrd.h"
#include "../jrd/irq.h"
#include "../jrd/isc.h"
#include "../jrd/drq.h"
#include "../jrd/req.h"
#include "../jrd/tra.h"
#include "../jrd/blb.h"
#include "../jrd/lck.h"
#include "../jrd/scl.h"
#include "../jrd/license.h"
#include "../jrd/os/pio.h"
#include "../jrd/ods.h"
#include "../jrd/exe.h"
#include "../jrd/val.h"
#include "../jrd/rse.h"
#include "../jrd/all.h"
#include "../jrd/log.h"
#include "../jrd/fil.h"
#include "../jrd/sbm.h"
#include "../jrd/svc.h"
#include "../jrd/sdw.h"
#include "../jrd/lls.h"
#include "../jrd/cch.h"
#include "../jrd/iberr.h"
#include "../jrd/jrd_time.h"
#include "../intl/charsets.h"
#include "../jrd/sort.h"

#include "../jrd/all_proto.h"
#include "../jrd/blb_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/dbg_proto.h"
#include "../jrd/dls_proto.h"
#include "../jrd/dyn_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/ext_proto.h"
#include "../jrd/fun_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/iberr_proto.h"
#include "../jrd/inf_proto.h"
#include "../jrd/ini_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/inuse_proto.h"
#include "../jrd/isc_f_proto.h"
#include "../jrd/isc_proto.h"
#include "../jrd/jrd_proto.h"

#include "../jrd/lck_proto.h"
#include "../jrd/log_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/par_proto.h"
#include "../jrd/os/pio_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/scl_proto.h"
#include "../jrd/sdw_proto.h"
#include "../jrd/shut_proto.h"
#include "../jrd/sort_proto.h"
#include "../jrd/svc_proto.h"
#include "../jrd/thread_proto.h"
#include "../jrd/tra_proto.h"
#include "../jrd/val_proto.h"
#include "../jrd/file_params.h"
#include "../jrd/event_proto.h"
#include "../jrd/flags.h"

#include "../common/config/config.h"
#include "../jrd/plugin_manager.h"
#include "../jrd/db_alias.h"
#include "../common/classes/fb_tls.h"
#include "../common/classes/ClumpletReader.h"


#ifdef GARBAGE_THREAD
#include "vio_proto.h"
#endif

using namespace Jrd;

#ifdef SERVER_SHUTDOWN
struct db_file {
	db_file* dbf_next;
	USHORT dbf_length;
	TEXT dbf_data[2];
};

//#include "../jrd/sort.h"
#endif /* SERVER_SHUTDOWN */

const SSHORT WAIT_PERIOD	= -1;

#ifdef SUPPORT_RAW_DEVICES
#define unlink PIO_unlink
#endif

#ifdef SUPERSERVER
#define V4_THREADING

#endif /* SUPERSERVER */

# ifdef V4_THREADING
#  undef V4_INIT
#  undef V4_GLOBAL_MUTEX_LOCK
#  undef V4_GLOBAL_MUTEX_UNLOCK
#  undef V4_MUTEX_INIT
#  undef V4_MUTEX_LOCK
#  undef V4_MUTEX_UNLOCK
#  undef V4_MUTEX_DESTROY
#  undef V4_JRD_MUTEX_LOCK
#  undef V4_JRD_MUTEX_UNLOCK
#  define V4_INIT			THD_INIT
#  define V4_GLOBAL_MUTEX_LOCK	{THREAD_EXIT(); THD_GLOBAL_MUTEX_LOCK; THREAD_ENTER();}
#  define V4_GLOBAL_MUTEX_UNLOCK	THD_GLOBAL_MUTEX_UNLOCK
#  define V4_MUTEX_INIT(mutx)	THD_MUTEX_INIT (mutx)
#  define V4_MUTEX_LOCK(mutx)	{THREAD_EXIT(); THD_MUTEX_LOCK (mutx); THREAD_ENTER();}
#  define V4_MUTEX_UNLOCK(mutx)	THD_MUTEX_UNLOCK (mutx)
#  define V4_MUTEX_DESTROY(mutx)	THD_MUTEX_DESTROY (mutx)
#  ifndef SUPERSERVER
#   define V4_JRD_MUTEX_LOCK(mutx)	{THREAD_EXIT(); THD_JRD_MUTEX_LOCK (mutx); THREAD_ENTER();}
#   define V4_JRD_MUTEX_UNLOCK(mutx) THD_JRD_MUTEX_UNLOCK (mutx)
#  endif /* SUPERSERVER */
# endif /* V4_THREADING */

#ifdef SUPERSERVER

extern SLONG trace_pools;
static REC_MUTX_T databases_rec_mutex;

// BRS. 03/23/2003
// Those empty defines was substituted with #if defined(V4_THREADING) && !defined(SUPERSERVER)
//#define V4_JRD_MUTEX_LOCK(mutx)
//#define V4_JRD_MUTEX_UNLOCK(mutx)

#define JRD_SS_INIT_MUTEX       THD_rec_mutex_init (&databases_rec_mutex)
#define JRD_SS_DESTROY_MUTEX    THD_rec_mutex_destroy (&databases_rec_mutex)
#define JRD_SS_MUTEX_LOCK       {THREAD_EXIT();\
                                 THD_rec_mutex_lock (&databases_rec_mutex);\
                                 THREAD_ENTER();}
#define JRD_SS_MUTEX_UNLOCK     THD_rec_mutex_unlock (&databases_rec_mutex)
#define JRD_SS_THD_MUTEX_LOCK   THD_rec_mutex_lock (&databases_rec_mutex)
#define JRD_SS_THD_MUTEX_UNLOCK THD_rec_mutex_unlock (&databases_rec_mutex)

#else
#define JRD_SS_INIT_MUTEX
#define JRD_SS_DESTROY_MUTEX
#define JRD_SS_MUTEX_LOCK
#define JRD_SS_MUTEX_UNLOCK
#define JRD_SS_THD_MUTEX_LOCK
#define JRD_SS_THD_MUTEX_UNLOCK
#endif

#ifdef  WIN_NT
#include <windows.h>
/* these should stop a most annoying warning */
#undef TEXT
#define TEXT    SCHAR
#endif	// WIN_NT

void Jrd::Trigger::compile(thread_db* tdbb)
{
	if (!request && !compile_in_progress)
	{
		SET_TDBB(tdbb);

		compile_in_progress = true;
		// Allocate statement memory pool
		JrdMemoryPool* new_pool = JrdMemoryPool::createPool();
		// Trigger request is not compiled yet. Lets do it now
		try {
			Jrd::ContextPoolHolder context(tdbb, new_pool);
			PAR_blr(tdbb, relation, blr.begin(),  NULL, NULL, &request, true,
					(USHORT)(flags & TRG_ignore_perm ? csb_ignore_perm : 0));
		}
		catch (const std::exception&) {
			compile_in_progress = false;
			if (request) {
				CMP_release(tdbb, request);
				request = NULL;
			}
			else {
				JrdMemoryPool::deletePool(new_pool);
			}
			throw;
		}
		
		if (name.hasData()) 
		{
			request->req_trg_name = name.c_str();
		}
		if (sys_trigger)
		{
			request->req_flags |= req_sys_trigger;
		}
		if (flags & TRG_ignore_perm)
		{
			request->req_flags |= req_ignore_perm;
		}

		compile_in_progress = false;
	}
}

void Jrd::Trigger::release(thread_db* tdbb)
{
	if (blr.getCount() == 0 //sys_trigger
				|| !request || CMP_clone_is_active(request))
	{
		return; // FALSE;
	}
	
	CMP_release(tdbb, request);
	request = NULL;
	return; // TRUE;
}

/* Option block for database parameter block */

class DatabaseOptions
{
public:
	USHORT	dpb_wal_action;
	SLONG	dpb_sweep_interval;
	ULONG	dpb_page_buffers;
	bool	dpb_set_page_buffers;
	ULONG dpb_buffers;
	USHORT	dpb_debug;
	USHORT	dpb_verify;
	USHORT	dpb_sweep;
	USHORT	dpb_trace;
	USHORT	dpb_disable;
	USHORT	dpb_dbkey_scope;
	USHORT	dpb_page_size;
	bool	dpb_activate_shadow;
	bool	dpb_delete_shadow;
	USHORT	dpb_no_garbage;
#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	bool	dpb_quit_log;
#endif
	USHORT	dpb_shutdown;
	SSHORT	dpb_shutdown_delay;
	USHORT	dpb_online;
	SSHORT	dpb_force_write;
	UCHAR	dpb_set_force_write;
	UCHAR	dpb_no_reserve;
	UCHAR	dpb_set_no_reserve;
	SSHORT	dpb_interp;
	USHORT	dpb_single_user;
	bool	dpb_overwrite;
	bool	dpb_sec_attach;
	bool	dpb_disable_wal;
	bool	dpb_gsec_attach;
	SLONG	dpb_connect_timeout;
	SLONG	dpb_dummy_packet_interval;
	bool	dpb_db_readonly;
	bool	dpb_set_db_readonly;
	bool	dpb_gfix_attach;
	bool	dpb_gstat_attach;
	USHORT	dpb_sql_dialect;
	USHORT	dpb_set_db_sql_dialect;
// here begin compound objects
// for constructor to work properly dpb_sys_user_name 
// MUST be FIRST
	Firebird::string	dpb_sys_user_name;
	Firebird::string	dpb_user_name;
	Firebird::string	dpb_password;
	Firebird::string	dpb_password_enc;
	Firebird::string	dpb_role_name;
	Firebird::string	dpb_journal;
	Firebird::string	dpb_key;
#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	Firebird::PathName	dpb_log;
#endif
	Firebird::PathName	dpb_lc_messages;
	Firebird::string	dpb_lc_ctype;
	Firebird::string	dpb_gbak_attach;
	Firebird::PathName	dpb_working_directory;
	Firebird::string	dpb_set_db_charset;
public:
	DatabaseOptions()
	{
		memset(this, 0, 
			reinterpret_cast<char*>(&this->dpb_sys_user_name) - 
			reinterpret_cast<char*>(this));
	}
	void get(const UCHAR*, USHORT);
};

static blb*		check_blob(thread_db*, ISC_STATUS*, blb**);
static ISC_STATUS	check_database(thread_db*, Attachment*, ISC_STATUS*);
static void		cleanup(void*);
static ISC_STATUS	commit(ISC_STATUS*, jrd_tra**, const bool);
static bool		drop_files(const jrd_file*);
static ISC_STATUS	error(ISC_STATUS*, const std::exception& ex);
static ISC_STATUS	error(ISC_STATUS*);
static void		find_intl_charset(thread_db*, Attachment*, const DatabaseOptions*);
static jrd_tra*		find_transaction(thread_db*, jrd_tra*, ISC_STATUS);
static ISC_STATUS	handle_error(ISC_STATUS*, ISC_STATUS, thread_db*);
static void		verify_request_synchronization(jrd_req*& request, SSHORT level);
namespace {
	enum vdnResult {vdnFail, vdnOk, vdnSecurity};
}
static vdnResult	verify_database_name(const Firebird::PathName&, ISC_STATUS*);
#if defined (WIN_NT)
#ifdef SERVER_SHUTDOWN
static void		ExtractDriveLetter(const TEXT*, ULONG*);
#else // SERVER_SHUTDOWN
static void		setup_NT_handlers(void);
static BOOLEAN	handler_NT(SSHORT);
#endif	// SERVER_SHUTDOWN
#endif	// WIN_NT

static Database*	init(thread_db*, ISC_STATUS*, const Firebird::PathName&, bool);
static ISC_STATUS	prepare(thread_db*, jrd_tra*, ISC_STATUS*, USHORT, const UCHAR*);
static void		release_attachment(Attachment*);
static ISC_STATUS	return_success(thread_db*);
static bool		rollback(thread_db*, jrd_tra*, ISC_STATUS*, const bool);

static void		shutdown_database(Database*, const bool);
static void		strip_quotes(Firebird::string&);
static void		purge_attachment(thread_db*, ISC_STATUS*, Attachment*, const bool);

static bool		initialized = false;
static Database*		databases = NULL;
#if defined(V4_THREADING) && !defined(SUPERSERVER)
static MUTX_T	databases_mutex[1];
#endif
static ULONG	JRD_cache_default;

#ifdef GOVERNOR
const int ATTACHMENTS_PER_USER = 1;
static ULONG JRD_max_users = 0;
static ULONG num_attached = 0;
#endif /* GOVERNOR */


#if !defined(REQUESTER)

int		debug;

#endif	// !REQUESTER


//____________________________________________________________
//
// check whether we need to perform an autocommit;
// do it here to prevent committing every record update
// in a statement
//
static void check_autocommit(jrd_req* request, thread_db* tdbb)
{
	/* dimitr: we should ignore autocommit for requests
			   created by EXECUTE STATEMENT */
	if (request->req_transaction->tra_callback_count > 0)
		return;

	if (request->req_transaction->tra_flags & TRA_perform_autocommit)
	{
		request->req_transaction->tra_flags &= ~TRA_perform_autocommit;
		TRA_commit(tdbb, request->req_transaction, true);
	}
}

inline static void api_entry_point_init(ISC_STATUS* user_status)
{
	user_status[0] = isc_arg_gds;
	user_status[1] = FB_SUCCESS;
	user_status[2] = isc_arg_end;
}

inline static thread_db* JRD_MAIN_set_thread_data(thread_db& thd_context)
{
	thread_db* tdbb = &thd_context;
	JRD_set_context(tdbb);
	return tdbb;
}

#define CHECK_HANDLE(blk, type, error)					\
	if (!blk || MemoryPool::blk_type(blk) != type)	\
		return handle_error (user_status, error, tdbb)

#define NULL_CHECK(ptr, code)									\
	if (*ptr) return handle_error (user_status, code, tdbb)



const int SWEEP_INTERVAL		= 20000;

const char DBL_QUOTE			= '\042';
const char SINGLE_QUOTE			= '\'';
const int BUFFER_LENGTH128		= 128;
bool invalid_client_SQL_dialect = false;

#define GDS_ATTACH_DATABASE		jrd8_attach_database
#define GDS_BLOB_INFO			jrd8_blob_info
#define GDS_CANCEL_BLOB			jrd8_cancel_blob
#define GDS_CANCEL_EVENTS		jrd8_cancel_events
#define GDS_CANCEL_OPERATION	jrd8_cancel_operation
#define GDS_CLOSE_BLOB			jrd8_close_blob
#define GDS_COMMIT				jrd8_commit_transaction
#define GDS_COMMIT_RETAINING	jrd8_commit_retaining
#define GDS_COMPILE				jrd8_compile_request
#define GDS_CREATE_BLOB2		jrd8_create_blob2
#define GDS_CREATE_DATABASE		jrd8_create_database
#define GDS_DATABASE_INFO		jrd8_database_info
#define GDS_DDL					jrd8_ddl
#define GDS_DETACH				jrd8_detach_database
#define GDS_DROP_DATABASE		jrd8_drop_database
#define GDS_GET_SEGMENT			jrd8_get_segment
#define GDS_GET_SLICE			jrd8_get_slice
#define GDS_OPEN_BLOB2			jrd8_open_blob2
#define GDS_PREPARE				jrd8_prepare_transaction
#define GDS_PUT_SEGMENT			jrd8_put_segment
#define GDS_PUT_SLICE			jrd8_put_slice
#define GDS_QUE_EVENTS			jrd8_que_events
#define GDS_RECONNECT			jrd8_reconnect_transaction
#define GDS_RECEIVE				jrd8_receive
#define GDS_RELEASE_REQUEST		jrd8_release_request
#define GDS_REQUEST_INFO		jrd8_request_info
#define GDS_ROLLBACK			jrd8_rollback_transaction
#define GDS_ROLLBACK_RETAINING	jrd8_rollback_retaining
#define GDS_SEEK_BLOB			jrd8_seek_blob
#define GDS_SEND				jrd8_send
#define GDS_SERVICE_ATTACH		jrd8_service_attach
#define GDS_SERVICE_DETACH		jrd8_service_detach
#define GDS_SERVICE_QUERY		jrd8_service_query
#define GDS_SERVICE_START		jrd8_service_start
#define GDS_START_AND_SEND		jrd8_start_and_send
#define GDS_START				jrd8_start_request
#define GDS_START_MULTIPLE		jrd8_start_multiple
#define GDS_START_TRANSACTION	jrd8_start_transaction
#define GDS_TRANSACT_REQUEST	jrd8_transact_request
#define GDS_TRANSACTION_INFO	jrd8_transaction_info
#define GDS_UNWIND				jrd8_unwind_request


/* External hook definitions */

/* dimitr: just uncomment the following line to use this feature.
		   Requires support from the PIO modules. Only Win32 is 100% ready
		   for this so far. Note that the database encryption code in the
		   PIO layer seems to be incompatible with the SUPERSERVER_V2 code.
		   2003.02.09 */
//#define ISC_DATABASE_ENCRYPTION

static const char* CRYPT_IMAGE = "fbcrypt";
static const char* ENCRYPT = "encrypt";
static const char* DECRYPT = "decrypt";


namespace {

TLS_DECLARE(bool, thread_security_disabled);

}


void	JRD_thread_security_disable(bool disable)
{
/**************************************
 *
 *	J R D _ t h r e a d _ s e c u r i t y _ d i s a b l e
 *
 **************************************
 *
 * Functional description
 *	Disable database attache security for this thread for the purposes of database attach
 *
 **************************************/
	TLS_SET(thread_security_disabled, disable);
}


void JRD_print_pools(const char* filename) {
	FILE *out = fopen(filename, "w");
	ALL_print_memory_pool_info(out, databases);
	fclose(out);	
}


ISC_STATUS GDS_ATTACH_DATABASE(ISC_STATUS*	user_status,
								SSHORT	file_length,
								const TEXT*	file_name,
								Attachment**	handle,
								SSHORT	dpb_length,
								const UCHAR*	dpb,
								const TEXT* expanded_filename)
{
/**************************************
 *
 *	g d s _ $ a t t a c h _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Attach a moldy, grungy, old database
 *	sullied by user data.
 *
 **************************************/
	api_entry_point_init(user_status);

	if (*handle) {
		return handle_error(user_status, isc_bad_db_handle, 0);
	}

/* Resolve given alias name */

	if (!file_length) 
		file_length = strlen(file_name);
	Firebird::PathName expanded_name(file_name, 
		file_length ? file_length : strlen(file_name));

	const bool is_alias = ResolveDatabaseAlias(expanded_name, expanded_name);
	if (is_alias)
	{
		ISC_expand_filename(expanded_name, false);
	}
	else
	{
		expanded_name = expanded_filename;
	}

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

/* Check database against conf file. */
	const vdnResult vdn = verify_database_name(expanded_name, user_status);
	if (!is_alias && vdn == vdnFail)
	{
		JRD_restore_context();
		return user_status[1];
	} 
	else 
	  user_status[0] = 0; // Clear status vector

/* Unless we're already attached, do some initialization */

	Database* dbb = init(tdbb, user_status, expanded_name, true);
	if (!dbb) {
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_UNLOCK(databases_mutex);
#endif
		JRD_SS_MUTEX_UNLOCK;
		JRD_restore_context();
		return user_status[1];
	}

	// use database context pool
	Jrd::ContextPoolHolder context(tdbb, dbb->dbb_permanent);

	dbb->dbb_flags |= DBB_being_opened;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
	V4_JRD_MUTEX_UNLOCK(databases_mutex);
#endif
	tdbb->tdbb_database = dbb;

/* Initialize special error handling */

	ISC_STATUS* status;
	tdbb->tdbb_status_vector = status = user_status;
	Attachment* attachment;
	tdbb->tdbb_attachment = attachment = NULL;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;

/* Count active thread in database */

	++dbb->dbb_use_count;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	bool initing_security = false;
#endif

/* The following line seems to fix a bug that appears on
   SCO EVEREST.  It probably has to do with the fact that
   the error handler below used to contain the first reference to
   variable transaction, which is actually initialized a few lines
   below that. */

	attachment = *(&attachment);

	try {

/* Process database parameter block */
	DatabaseOptions options;
	options.get(dpb, dpb_length);

#ifndef NO_NFS
/* Don't check nfs if single user */

	if (!options.dpb_single_user)
#endif
	{
		/* Check to see if the database is truly local or if it just looks
		   that way */
      
		if (ISC_check_if_remote(expanded_name, true)) {
			ERR_post(isc_unavailable, 0);
		}
	}

/* If database to be opened is SecurityDatabase, then only 
   gsec or SecurityDatabase may open it. This protects from use
   of old gsec to write wrong password hashes into it. */
	if (vdn == vdnSecurity && !options.dpb_gsec_attach && !options.dpb_sec_attach)
	{
		ERR_post(isc_no_priv,
				 isc_arg_string, "direct",
				 isc_arg_string, "security database",
				 isc_arg_string, 
				 ERR_string(file_name, file_length), 
				 0);
	}

/* Worry about encryption key */

	if (dbb->dbb_decrypt) {
		if (dbb->dbb_filename.hasData() && 
			(dbb->dbb_encrypt_key.hasData() || options.dpb_key.hasData()))
		{
			if ((dbb->dbb_encrypt_key.hasData() && options.dpb_key.isEmpty()) ||
				(dbb->dbb_encrypt_key.empty() && options.dpb_key.hasData()) ||
				(dbb->dbb_encrypt_key != options.dpb_key))
			{
				ERR_post(isc_no_priv,
						 isc_arg_string, "encryption",
						 isc_arg_string, "database",
						 isc_arg_string, 
                         ERR_string(file_name, file_length), 
                         0);
			}
		}
		else if (options.dpb_key.hasData()) 
		{
			dbb->dbb_encrypt_key = options.dpb_key;
		}
	}

	tdbb->tdbb_attachment = attachment = FB_NEW(*dbb->dbb_permanent) Attachment(dbb);
	attachment->att_filename = expanded_name;

	attachment->att_next = dbb->dbb_attachments;
	dbb->dbb_attachments = attachment;
	dbb->dbb_flags &= ~DBB_being_opened;
	dbb->dbb_sys_trans->tra_attachment = attachment;
	tdbb->tdbb_quantum = (ThreadPriorityScheduler::boosted() ? 
		Config::getPriorityBoost() : 1) * QUANTUM;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;
	tdbb->tdbb_inhibit = 0;
	tdbb->tdbb_flags = 0;

	attachment->att_charset = options.dpb_interp;

	if (options.dpb_lc_messages.hasData()) {
		attachment->att_lc_messages = options.dpb_lc_messages;
	}

	if (options.dpb_no_garbage)
		attachment->att_flags |= ATT_no_cleanup;

	if (options.dpb_gbak_attach.hasData())
		attachment->att_flags |= ATT_gbak_attachment;

	if (options.dpb_gstat_attach)
		attachment->att_flags |= ATT_gstat_attachment;

	if (options.dpb_gfix_attach)
		attachment->att_flags |= ATT_gfix_attachment;

	if (options.dpb_working_directory.hasData()) {
		attachment->att_working_directory = options.dpb_working_directory;
	}

/* If we're a not a secondary attachment, initialize some stuff */

	bool first = false;

	LCK_init(tdbb, LCK_OWNER_attachment);	/* For the attachment */
	attachment->att_flags |= ATT_lck_init_done;
	if (dbb->dbb_filename.empty())
	{
		first = true;
		dbb->dbb_filename = expanded_name;
		
		// NS: Use alias as database ID only if accessing database using file name is not possible.
		//
		// This way we:
		// 1. Ensure uniqueness of ID even in presence of multiple processes
		// 2. Make sure that ID value can be used to connect back to database
		//
		if (is_alias && vdn == vdnFail)
			dbb->dbb_database_name.assign(file_name, file_length ? file_length : strlen(file_name));
		else
			dbb->dbb_database_name = expanded_name;
			
		/* Extra LCK_init() done to keep the lock table until the
		   database is shutdown() after the last detach. */
		LCK_init(tdbb, LCK_OWNER_database);
		dbb->dbb_flags |= DBB_lck_init_done;
		INI_init();
		FUN_init();
		dbb->dbb_file =
			PIO_open(dbb, expanded_name.c_str(), expanded_name.length(),
					 options.dpb_trace != 0, NULL, file_name, file_length);
		SHUT_init(dbb);
		PAG_header(file_name, file_length);
		INI_init2();
		PAG_init();
		if (options.dpb_set_page_buffers) {
			dbb->dbb_page_buffers = options.dpb_page_buffers;
		}
		CCH_init(tdbb, (ULONG) options.dpb_buffers);
		
		// Initialize backup difference subsystem. This must be done before WAL and shadowing
		// is enabled because nbackup it is a lower level subsystem
		dbb->backup_manager = FB_NEW(*dbb->dbb_permanent) BackupManager(dbb, nbak_state_unknown);
		
		PAG_init2(0);		
		
#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
		LOG_init(expanded_name, length_expanded);
#endif

		/* initialize shadowing as soon as the database is ready for it
		   but before any real work is done */
		SDW_init(options.dpb_activate_shadow,
				options.dpb_delete_shadow);

		/* dimitr: disabled due to unreliable behaviour of minor ODS upgrades
			a) in the case of any failure it's impossible to attach the database
			b) there's no way to handle failures properly, because upgrade is
			   being made in the context of system transaction which doesn't use
			   the backout logic
		INI_update_database();
		*/
	}

    /* Attachments to a ReadOnly database need NOT do garbage collection */
    if (dbb->dbb_flags & DBB_read_only) {
            attachment->att_flags |= ATT_no_cleanup;
	}

    if (options.dpb_disable_wal) {
		ERR_post(isc_lock_timeout, isc_arg_gds, isc_obj_in_use,
				 isc_arg_string, 
                 ERR_string(file_name, file_length), 
                 0);
	}

	if (options.dpb_buffers && !dbb->dbb_page_buffers) {
		CCH_expand(tdbb, (ULONG) options.dpb_buffers);
	}

	if (!options.dpb_verify && CCH_exclusive(tdbb, LCK_PW, LCK_NO_WAIT))
	{
		TRA_cleanup(tdbb);
	}

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	V4_JRD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
	initing_security = true;
#endif

	if (invalid_client_SQL_dialect)
	{
		ERR_post(isc_inv_client_dialect_specified, isc_arg_number,
				 options.dpb_sql_dialect,
				 isc_arg_gds, isc_valid_client_dialects,
				 isc_arg_string, "1, or 3", 0);
	}
	invalid_client_SQL_dialect = false;

	if (options.dpb_role_name.hasData())
	{
		switch (options.dpb_sql_dialect)
		{
		case 0:
			{
				/*
				   ** V6 Client --> V6 Server, dummy client SQL dialect 0 was passed
				   **   It means that client SQL dialect was not set by user
				   **   and takes DB SQL dialect as client SQL dialect
				 */
				if (ENCODE_ODS(dbb->dbb_ods_version, dbb->dbb_minor_original)
					>= ODS_10_0)
				{
					if (dbb->dbb_flags & DBB_DB_SQL_dialect_3) {
						/*
						   ** DB created in IB V6.0 by client SQL dialect 3
						 */
						options.dpb_sql_dialect = SQL_DIALECT_V6;
					}
					else {
						/*
						   ** old DB was gbaked in IB V6.0
						 */
						options.dpb_sql_dialect = SQL_DIALECT_V5;
					}
				}
				else {
					options.dpb_sql_dialect = SQL_DIALECT_V5;
				}
			}
			break;
		case 99:
			/*
			   ** V5 Client --> V6 Server, old client has no concept of dialect
			 */
			options.dpb_sql_dialect = SQL_DIALECT_V5;
			break;
		default:
			/*
			   ** V6 Client --> V6 Server, but client SQL dialect was set
			   **   by user and was passed.
			 */
			break;
		}

		switch (options.dpb_sql_dialect)
		{
		case SQL_DIALECT_V5:
			{
				strip_quotes(options.dpb_role_name);
				options.dpb_role_name.upper();
			}
			break;
		case SQL_DIALECT_V6_TRANSITION:
		case SQL_DIALECT_V6:
			{
				if (options.dpb_role_name.hasData() && 
					(options.dpb_role_name[0] == DBL_QUOTE ||
					 options.dpb_role_name[0] == SINGLE_QUOTE))
				{
					const char end_quote = options.dpb_role_name[0];
					/*
					   ** remove the delimited quotes and escape quote
					   ** from ROLE name
					 */
					options.dpb_role_name.erase(0, 1);
					for (Firebird::string::iterator p = 
								options.dpb_role_name.begin(); 
						 p < options.dpb_role_name.end(); ++p)
					{
						if (*p == end_quote)
						{
							if (++p < options.dpb_role_name.end() &&
								*p == end_quote)
							{
								// skip the escape quote here
								options.dpb_role_name.erase(p);
							}
							else
							{
								// delimited done
								options.dpb_role_name.erase(--p, 
									options.dpb_role_name.end());
							}
						}
					}
				}
				else
				{
					options.dpb_role_name.upper();
				}
			}
			break;
		default:
			break;
		}
	}

	options.dpb_sql_dialect = 0;

	// Don't run internal handles thru the security gauntlet.

	const bool internal = TLS_GET(thread_security_disabled);

	SCL_init(false,
			 options.dpb_sys_user_name.nullStr(),
			 options.dpb_user_name.nullStr(),
			 options.dpb_password.nullStr(),
			 options.dpb_password_enc.nullStr(),
			 options.dpb_role_name.nullStr(),
			 tdbb,
			 internal);

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	initing_security = false;
	V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif

	if (options.dpb_shutdown)
	{
		/* By releasing the DBB_MUTX_init_fini mutex here, we would be allowing
		   other threads to proceed with their detachments, so that shutdown does
		   not timeout for exclusive access and other threads don't have to wait
		   behind shutdown */

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
		JRD_SS_MUTEX_UNLOCK;
		if (!SHUT_database
			(dbb, options.dpb_shutdown, options.dpb_shutdown_delay)) 
		{
			JRD_SS_MUTEX_LOCK;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
			V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
			if (user_status[1] != FB_SUCCESS)
				ERR_punt();
			else
				ERR_post(isc_no_priv,
						 isc_arg_string, "shutdown or online",
						 isc_arg_string, "database",
						 isc_arg_string, 
                         ERR_string(file_name, file_length), 
                         0);
		}
		JRD_SS_MUTEX_LOCK;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
	}

	if (options.dpb_online)
	{
		/* By releasing the DBB_MUTX_init_fini mutex here, we would be allowing
		   other threads to proceed with their detachments, so that shutdown does
		   not timeout for exclusive access and other threads don't have to wait
		   behind shutdown */

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
		JRD_SS_MUTEX_UNLOCK;
		if (!SHUT_online(dbb, options.dpb_online)) 
		{
			JRD_SS_MUTEX_LOCK;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
			V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
			if (user_status[1] != FB_SUCCESS)
				ERR_punt();
			else
				ERR_post(isc_no_priv,
						 isc_arg_string, "shutdown or online",
						 isc_arg_string, "database",
						 isc_arg_string, 
                         ERR_string(file_name, file_length), 
                         0);
		}
		JRD_SS_MUTEX_LOCK;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
	}

#ifdef SUPERSERVER
/* Check if another attachment has or is requesting exclusive database access.
   If this is an implicit attachment for the security (password) database, don't
   try to get exclusive attachment to avoid a deadlock condition which happens
   when a client tries to connect to the security database itself. */

	if (!options.dpb_sec_attach) {
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
		JRD_SS_MUTEX_UNLOCK;
		bool attachment_succeeded = true;
		if (dbb->dbb_ast_flags & DBB_shutdown_single)
			attachment_succeeded = CCH_exclusive_attachment(tdbb, LCK_none, -1);
		else
			CCH_exclusive_attachment(tdbb, LCK_none, LCK_WAIT);
		JRD_SS_MUTEX_LOCK;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
		if (attachment->att_flags & ATT_shutdown)
			ERR_post(isc_shutdown, isc_arg_string, 
					ERR_string(file_name, file_length), 0);
		if (!attachment_succeeded) {
			ERR_post(isc_shutdown, isc_arg_string, 
					ERR_string(file_name, file_length), 0);
		}
	}
#endif

/* If database is shutdown then kick 'em out. */

	if (dbb->dbb_ast_flags & (DBB_shut_attach | DBB_shut_tran))
	{
		ERR_post(isc_shutinprog, isc_arg_string, 
				ERR_string(file_name, file_length), 0);
	}

	if (dbb->dbb_ast_flags & DBB_shutdown) {
		// Allow only SYSDBA/owner to access database that is shut down
		bool allow_access = attachment->att_user->usr_flags & (USR_locksmith | USR_owner);
		// Handle special shutdown modes
		if (allow_access) {
			if (dbb->dbb_ast_flags & DBB_shutdown_full) {
				// Full shutdown. Deny access always
				allow_access = false;
			}
			else if (dbb->dbb_ast_flags & DBB_shutdown_single) {
				// Single user maintenance. Allow access only if we were able to take exclusive lock
				// Note that logic below this exclusive lock differs for SS and CS builds:
				//   - CS keeps PW database lock from releasing in AST in single-user maintenance mode
				//   - for SS this code effectively checks that no other attachments are present
				//     at call point, ATT_exclusive bit is released just before this procedure exits
				// Things are done this way to handle return to online mode nicely.
				allow_access = CCH_exclusive(tdbb, LCK_PW, WAIT_PERIOD);
			}
		}
		if (!allow_access) {
			// Note we throw exception here when entering full-shutdown mode
			ERR_post(isc_shutdown, isc_arg_string, 
					ERR_string(file_name, file_length), 0);
		}
	}

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	if (options.dpb_quit_log) {
		LOG_disable();
	}
#endif

/* Figure out what character set & collation this attachment prefers */

	find_intl_charset(tdbb, attachment, &options);

	if (options.dpb_verify)
	{
		if (!CCH_exclusive(tdbb, LCK_PW, WAIT_PERIOD)) {
			ERR_post(isc_bad_dpb_content, isc_arg_gds, isc_cant_validate, 0);
		}

#ifdef GARBAGE_THREAD
		/* Can't allow garbage collection during database validation. */

		VIO_fini(tdbb);
#endif
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
		JRD_SS_MUTEX_UNLOCK;
		if (!VAL_validate(tdbb, options.dpb_verify)) {
			JRD_SS_MUTEX_LOCK;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
			V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
			ERR_punt();
		}
		JRD_SS_MUTEX_LOCK;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
	}

	if (options.dpb_journal.hasData()) {
		ERR_post(isc_bad_dpb_content,
				 isc_arg_gds, isc_cant_start_journal,
				 0);
	}

	if (options.dpb_wal_action)
	{
		/* Make sure WAL enabled before taking any WAL action. */

		ERR_post(isc_no_wal, 0);
	}

/*
 * if the attachment is through gbak and this the attachment is not by owner
 * or sysdba then return error. This has been added here to allow for the
 * GBAK security feature of only allowing the owner or sysdba to backup a
 * database. smistry 10/5/98
 */

	if (((attachment->att_flags & ATT_gbak_attachment) ||
		 (attachment->att_flags & ATT_gfix_attachment) ||
		 (attachment->att_flags & ATT_gstat_attachment)) &&
		!(attachment->att_user->usr_flags & (USR_locksmith | USR_owner)))
	{
		ERR_post(isc_adm_task_denied, 0);
	}

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	if (options.dpb_log) {
		if (first) {
			if (!CCH_exclusive(tdbb, LCK_EX, WAIT_PERIOD)) {
				ERR_post(isc_lock_timeout, isc_arg_gds, isc_obj_in_use,
						 isc_arg_string, 
                         ERR_string(file_name, file_length),
                         0);
			}
			LOG_enable(options.dpb_log, strlen(options.dpb_log));
		}
		else {
			ERR_post(isc_bad_dpb_content, isc_arg_gds,
					 isc_cant_start_logging, 0);
		}
	}
#endif

	if (options.dpb_set_db_sql_dialect) {
		PAG_set_db_SQL_dialect(dbb, options.dpb_set_db_sql_dialect);
	}

	if (options.dpb_sweep_interval != -1) {
		PAG_sweep_interval(options.dpb_sweep_interval);
		dbb->dbb_sweep_interval = options.dpb_sweep_interval;
	}

	if (options.dpb_set_force_write) {
		PAG_set_force_write(dbb, options.dpb_force_write);
	}

	if (options.dpb_set_no_reserve) {
		PAG_set_no_reserve(dbb, options.dpb_no_reserve);
	}

	if (options.dpb_set_page_buffers) {
		PAG_set_page_buffers(options.dpb_page_buffers);
	}

	if (options.dpb_set_db_readonly) {
		if (!CCH_exclusive(tdbb, LCK_EX, WAIT_PERIOD)) {
			ERR_post(isc_lock_timeout, isc_arg_gds, isc_obj_in_use,
					 isc_arg_string,
					 ERR_string(file_name, file_length), 
					 0); 
		}
		PAG_set_db_readonly(dbb, options.dpb_db_readonly);
	}

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
/* don't record the attach until now in case the log is added during the attach */

	LOG_call(log_attach2, file_length, file_name, *handle, dpb_length, dpb,
			 expanded_filename);
#endif

#ifdef GARBAGE_THREAD
	VIO_init(tdbb);
#endif

	CCH_release_exclusive(tdbb);

#ifdef GOVERNOR
	if (!options.dpb_sec_attach) {
		if (JRD_max_users) {
			if (num_attached < (JRD_max_users * ATTACHMENTS_PER_USER)) {
				num_attached++;
			}
			else {
				ERR_post(isc_max_att_exceeded, 0);
			}
		}
	}
	else {
		attachment->att_flags |= ATT_security_db;
	}
#endif /* GOVERNOR */

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	V4_JRD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
/* if there was an error, the status vector is all set */

	if (options.dpb_sweep & isc_dpb_records)
	{
		JRD_SS_MUTEX_UNLOCK;
		if (!(TRA_sweep(tdbb, 0)))
		{
			JRD_SS_MUTEX_LOCK;
			ERR_punt();
		}
		JRD_SS_MUTEX_LOCK;
	}

	if (options.dpb_dbkey_scope) {
		attachment->att_dbkey_trans = TRA_start(tdbb, 0, 0);
	}

	JRD_SS_MUTEX_UNLOCK;
	
	// Recover database after crash during backup difference file merge
	dbb->backup_manager->end_backup(true/*do recovery*/); 
	
	*handle = attachment;	

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_handle_returned, *handle);
#endif	

	return return_success(tdbb);

	}	// try
	catch (const std::exception& ex)
	{
		ISC_STATUS_ARRAY temp_status;
		try
		{
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
			if (initing_security)
			{
				V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
			}
#endif
			tdbb->tdbb_status_vector = temp_status;
			dbb->dbb_flags &= ~DBB_being_opened;
			release_attachment(attachment);

			/* At this point, mutex dbb->dbb_mutexes [DBB_MUTX_init_fini] has been
			   unlocked and mutex databases_mutex has been locked. */
			if (MemoryPool::blk_type(dbb) == type_dbb)
			{
				if (!dbb->dbb_attachments)
				{
					shutdown_database(dbb, true);
				}
				else if (attachment)
				{
					delete attachment;
				}
			}
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
			V4_JRD_MUTEX_UNLOCK(databases_mutex);
#endif
		}	// try
		catch (const std::exception&) {}
		tdbb->tdbb_status_vector = status;
		JRD_SS_MUTEX_UNLOCK;
		return error(user_status, ex);
	}
}


ISC_STATUS GDS_BLOB_INFO(ISC_STATUS*	user_status,
						blb**	blob_handle,
						SSHORT	item_length,
						const SCHAR*	items,
						SSHORT	buffer_length,
						SCHAR*	buffer)
{
/**************************************
 *
 *	g d s _ $ b l o b _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on blob object.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	const blb* blob = check_blob(tdbb, user_status, blob_handle);
	if (!blob) {
		return user_status[1];
	}

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_blob_info2, *blob_handle, item_length, items, buffer_length);
#endif

	try {
		tdbb->tdbb_status_vector = user_status;

		INF_blob_info(blob, items, item_length, buffer, buffer_length);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_CANCEL_BLOB(ISC_STATUS * user_status, blb** blob_handle)
{
/**************************************
 *
 *	g d s _ $ c a n c e l _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Abort a partially completed blob.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	if (*blob_handle) {
		blb* blob = check_blob(tdbb, user_status, blob_handle);
		if (!blob)
			return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
		LOG_call(log_cancel_blob, *blob_handle);
#endif

		try {
			tdbb->tdbb_status_vector = user_status;

			BLB_cancel(tdbb, blob);
			*blob_handle = NULL;
		}
		catch (const std::exception& ex)
		{
			return error(user_status, ex);
		}
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_CANCEL_EVENTS(ISC_STATUS*	user_status,
							Attachment**	handle,
							SLONG*	id)
{
/**************************************
 *
 *	g d s _ $ c a n c e l _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *	Cancel an outstanding event.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	if (check_database(tdbb, *handle, user_status)) {
		return user_status[1];
	}

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_cancel_events, *handle, *id);
#endif

	try
	{
		tdbb->tdbb_status_vector = user_status;

		EVENT_cancel(*id);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


#ifdef CANCEL_OPERATION
ISC_STATUS GDS_CANCEL_OPERATION(ISC_STATUS* user_status,
								Attachment** handle,
								USHORT option)
{
/**************************************
 *
 *	g d s _ $ c a n c e l _ o p e r a t i o n
 *
 **************************************
 *
 * Functional description
 *	Try to cancel an operation.
 *
 **************************************/
	api_entry_point_init(user_status);

	Attachment* attachment = *handle;

/* Check out the database handle.  This is mostly code from
   the routine "check_database" */

	Database* dbb;
	if (!attachment ||
		(MemoryPool::blk_type(attachment) != type_att) ||
		!(dbb = attachment->att_database) ||
		MemoryPool::blk_type(dbb) != type_dbb)
	{
		return handle_error(user_status, isc_bad_db_handle, 0);
	}

/* Make sure this is a valid attachment */

	const Attachment* attach;
	for (attach = dbb->dbb_attachments; attach; attach = attach->att_next)
		if (attach == attachment)
			break;

	if (!attach)
		return handle_error(user_status, isc_bad_db_handle, 0);

	switch (option) {
	case CANCEL_disable:
		attachment->att_flags |= ATT_cancel_disable;
		break;

	case CANCEL_enable:
		attachment->att_flags &= ~ATT_cancel_disable;
		break;

	case CANCEL_raise:
		attachment->att_flags |= ATT_cancel_raise;
		break;

	default:
		fb_assert(FALSE);
	}

	return FB_SUCCESS;
}
#endif


ISC_STATUS GDS_CLOSE_BLOB(ISC_STATUS * user_status, blb** blob_handle)
{
/**************************************
 *
 *	g d s _ $ c l o s e _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Abort a partially completed blob.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	blb* blob = check_blob(tdbb, user_status, blob_handle);
	if (!blob)
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_close_blob, *blob_handle);
#endif

	try
	{
		tdbb->tdbb_status_vector = user_status;

		BLB_close(tdbb, blob);
		*blob_handle = NULL;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}


	return return_success(tdbb);
}


ISC_STATUS GDS_COMMIT(ISC_STATUS * user_status, jrd_tra** tra_handle)
{
/**************************************
 *
 *	g d s _ $ c o m m i t
 *
 **************************************
 *
 * Functional description
 *	Commit a transaction.
 *
 **************************************/

	api_entry_point_init(user_status);

	if (commit(user_status, tra_handle, false))
		return user_status[1];

	*tra_handle = NULL;

	return FB_SUCCESS;
}


ISC_STATUS GDS_COMMIT_RETAINING(ISC_STATUS * user_status, jrd_tra** tra_handle)
{
/**************************************
 *
 *	g d s _ $ c o m m i t _ r e t a i n i n g
 *
 **************************************
 *
 * Functional description
 *	Commit a transaction.
 *
 **************************************/

	api_entry_point_init(user_status);

	return commit(user_status, tra_handle, true);
}


ISC_STATUS GDS_COMPILE(ISC_STATUS* user_status,
						Attachment** db_handle,
						jrd_req** req_handle,
						SSHORT blr_length,
						const SCHAR* blr)
{
/**************************************
 *
 *	g d s _ $ c o m p i l e
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	NULL_CHECK(req_handle, isc_bad_req_handle);
	Attachment* attachment = *db_handle;

	if (check_database(tdbb, attachment, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_compile, *db_handle, *req_handle, blr_length, blr);
#endif

	try
	{
		tdbb->tdbb_status_vector = user_status;

		jrd_req* request =
			CMP_compile2(tdbb, reinterpret_cast<const UCHAR*>(blr), FALSE);
		request->req_attachment = attachment;
		request->req_request = attachment->att_requests;
		attachment->att_requests = request;
	
		DEBUG;
		*req_handle = request;
	#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
		LOG_call(log_handle_returned, *req_handle);
	#endif
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_CREATE_BLOB2(ISC_STATUS* user_status,
							Attachment** db_handle,
							jrd_tra** tra_handle,
							blb** blob_handle,
							bid* blob_id,
							USHORT bpb_length,
							const UCHAR* bpb)
{
/**************************************
 *
 *	g d s _ $ c r e a t e _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Create a new blob.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	NULL_CHECK(blob_handle, isc_bad_segstr_handle);

	if (check_database(tdbb, *db_handle, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_create_blob2, *db_handle, *tra_handle, *blob_handle, blob_id,
			 bpb_length, bpb);
#endif

	try
	{
		tdbb->tdbb_status_vector = user_status;

		jrd_tra* transaction = find_transaction(tdbb, *tra_handle, isc_segstr_wrong_db);
		blb* blob = BLB_create2(tdbb, transaction, blob_id, bpb_length, bpb);
		*blob_handle = blob;
	
	#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
		LOG_call(log_handle_returned, *blob_handle);
		LOG_call(log_handle_returned, blob_id->bid_stuff.bid_temp_id);
	#endif
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}


	return return_success(tdbb);
}


ISC_STATUS GDS_CREATE_DATABASE(ISC_STATUS*	user_status,
								USHORT	file_length,
								const TEXT*	file_name,
								Attachment**	handle,
								USHORT	dpb_length,
								const UCHAR*	dpb,
								USHORT	db_type,
								const TEXT*	expanded_filename)
{
/**************************************
 *
 *	g d s _ $ c r e a t e _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Create a nice, squeeky clean database, uncorrupted by user data.
 *
 **************************************/
	api_entry_point_init(user_status);

	if (*handle)
		return handle_error(user_status, isc_bad_db_handle, 0);

/* Get length of database file name, if not already known */
	if (!file_length) 
		file_length = strlen(file_name);
	Firebird::PathName expanded_name;
	expanded_name.assign(file_name, file_length);

	const bool is_alias = ResolveDatabaseAlias(expanded_name, expanded_name);
	if (is_alias)
	{
		ISC_expand_filename(expanded_name, false);
	}
	else
	{
		expanded_name = expanded_filename;
	}

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	Database* dbb = init(tdbb, user_status, expanded_name, false);
	if (!dbb) {
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_UNLOCK(databases_mutex);
#endif
		JRD_SS_MUTEX_UNLOCK;
		JRD_restore_context();
		return user_status[1];
	}

	// use database context pool
	Jrd::ContextPoolHolder context(tdbb, dbb->dbb_permanent);

	dbb->dbb_flags |= DBB_being_opened;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
	V4_JRD_MUTEX_UNLOCK(databases_mutex);
#endif
	tdbb->tdbb_database = dbb;

/* Initialize error handling */

	ISC_STATUS* status = user_status;
	tdbb->tdbb_status_vector = status;
	Attachment* attachment = NULL;
	tdbb->tdbb_attachment = attachment;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;

/* Count active thread in database */

	++dbb->dbb_use_count;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	bool initing_security = false;
#endif

	try {

/* Process database parameter block */
	DatabaseOptions options;
	options.get(dpb, dpb_length);
	if (!invalid_client_SQL_dialect && options.dpb_sql_dialect == 99) {
		options.dpb_sql_dialect = 0;
	}


#ifndef NO_NFS
/* Don't check nfs if single user */

	if (!options.dpb_single_user)
#endif
	{
		/* Check to see if the database is truly local or if it just looks
		   that way */

		if (ISC_check_if_remote(expanded_name, true)) 
		{
			ERR_post(isc_unavailable, 0);
		}
	}

	// Check for DatabaseAccess
	const vdnResult vdn = verify_database_name(expanded_name, user_status);
	
	if (!is_alias && vdn == vdnFail) 
	{
		ERR_punt();
	} 
	else 
		user_status[0] = 0; // Clear status vector.

	if (options.dpb_key.hasData()) 
	{
		dbb->dbb_encrypt_key = options.dpb_key;
	}

	tdbb->tdbb_attachment = attachment = FB_NEW(*dbb->dbb_permanent) Attachment(dbb);
	attachment->att_filename = expanded_name;
	attachment->att_next = dbb->dbb_attachments;
	dbb->dbb_attachments = attachment;
	dbb->dbb_flags &= ~DBB_being_opened;
	dbb->dbb_sys_trans->tra_attachment = attachment;
	tdbb->tdbb_quantum = QUANTUM;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;
	tdbb->tdbb_inhibit = 0;
	tdbb->tdbb_flags = 0;

	if (options.dpb_working_directory.hasData()) {
		attachment->att_working_directory = options.dpb_working_directory;
	}

	if (options.dpb_gbak_attach.hasData()) {
		attachment->att_flags |= ATT_gbak_attachment;
	}

	switch (options.dpb_sql_dialect) {
	case 0:
		// This can be issued by QLI, GDEF and old BDE clients.  
		// In this case assume dialect 1
		options.dpb_sql_dialect = SQL_DIALECT_V5;
	case SQL_DIALECT_V5:
		break;
	case SQL_DIALECT_V6:
		dbb->dbb_flags |= DBB_DB_SQL_dialect_3;
		break;
	default:
		ERR_post(isc_database_create_failed, isc_arg_string,
				 expanded_name.c_str(), isc_arg_gds, isc_inv_dialect_specified,
				 isc_arg_number, options.dpb_sql_dialect, isc_arg_gds,
				 isc_valid_db_dialects, isc_arg_string, "1 and 3", 0);
		break;
	}

	attachment->att_charset = options.dpb_interp;

	if (options.dpb_lc_messages.hasData()) {
		attachment->att_lc_messages = options.dpb_lc_messages;
	}

	if (!options.dpb_page_size) {
		options.dpb_page_size = DEFAULT_PAGE_SIZE;
	}

	USHORT page_size;
	for (page_size = MIN_PAGE_SIZE; page_size < MAX_PAGE_SIZE; page_size <<= 1)
	{
		if (options.dpb_page_size < page_size << 1)
			break;
	}

	dbb->dbb_page_size =
		(page_size > MAX_PAGE_SIZE) ? MAX_PAGE_SIZE : page_size;

	LCK_init(tdbb, LCK_OWNER_attachment);	/* For the attachment */
	attachment->att_flags |= ATT_lck_init_done;
/* Extra LCK_init() done to keep the lock table until the
   database is shutdown() after the last detach. */
	LCK_init(tdbb, LCK_OWNER_database);
	dbb->dbb_flags |= DBB_lck_init_done;
	INI_init();
	FUN_init();
	PAG_init();
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	V4_JRD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
	initing_security = true;
#endif

	// Don't run internal handles thru the security gauntlet.

	const bool internal = TLS_GET(thread_security_disabled);

	SCL_init(true,
			 options.dpb_sys_user_name.nullStr(),
			 options.dpb_user_name.nullStr(),
			 options.dpb_password.nullStr(),
			 options.dpb_password_enc.nullStr(),
			 options.dpb_role_name.nullStr(),
			 tdbb,
			 internal);

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	initing_security = false;
	V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
	dbb->dbb_file =
		PIO_create(dbb, expanded_name.c_str(), expanded_name.length(), 
				   options.dpb_overwrite);
	const jrd_file* first_dbb_file = dbb->dbb_file;
	if (options.dpb_set_page_buffers)
		dbb->dbb_page_buffers = options.dpb_page_buffers;
	CCH_init(tdbb, (ULONG) options.dpb_buffers);
	
	// Initialize backup difference subsystem. This must be done before WAL and shadowing
	// is enabled because nbackup it is a lower level subsystem
	dbb->backup_manager = FB_NEW(*dbb->dbb_permanent) BackupManager(dbb, nbak_state_normal); 
	
	PAG_format_header();
	INI_init2();
	PAG_format_log();
	PAG_format_pip();

	if (options.dpb_set_page_buffers)
		PAG_set_page_buffers(options.dpb_page_buffers);

	if (options.dpb_set_no_reserve)
		PAG_set_no_reserve(dbb, options.dpb_no_reserve);

	INI_format(attachment->att_user->usr_user_name, 
			   options.dpb_set_db_charset.c_str());

	// There is no point to move database online at database creation since it is online by default.
	// We do not allow to create database that is fully shut down.
	if (options.dpb_online || (options.dpb_shutdown & isc_dpb_shut_mode_mask) == isc_dpb_shut_full)
		ERR_post(isc_bad_shutdown_mode, isc_arg_string, ERR_string(file_name, file_length), 0);
	
	if (options.dpb_shutdown) {
		/* By releasing the DBB_MUTX_init_fini mutex here, we would be allowing
		   other threads to proceed with their detachments, so that shutdown does
		   not timeout for exclusive access and other threads don't have to wait
		   behind shutdown */

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
		if (!SHUT_database
			(dbb, options.dpb_shutdown, options.dpb_shutdown_delay)) 
		{
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
			V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
			ERR_post(isc_no_priv,
					 isc_arg_string, "shutdown or online",
					 isc_arg_string, "database",
					 isc_arg_string,
					 ERR_string(file_name, file_length), 0);
		}
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
	}
	
	if (options.dpb_sweep_interval != -1) {
		PAG_sweep_interval(options.dpb_sweep_interval);
		dbb->dbb_sweep_interval = options.dpb_sweep_interval;
	}

	if (options.dpb_set_force_write)
		PAG_set_force_write(dbb, options.dpb_force_write);

/* initialize shadowing semaphore as soon as the database is ready for it
   but before any real work is done */

	SDW_init(options.dpb_activate_shadow, options.dpb_delete_shadow);

#ifdef GARBAGE_THREAD
	VIO_init(tdbb);
#endif

    if (options.dpb_set_db_readonly) {
        if (!CCH_exclusive (tdbb, LCK_EX, WAIT_PERIOD))
            ERR_post (isc_lock_timeout, isc_arg_gds, isc_obj_in_use,
                      isc_arg_string, 
                      ERR_string (file_name, file_length), 
                      0);
        
        PAG_set_db_readonly (dbb, options.dpb_db_readonly);
    }

    CCH_release_exclusive(tdbb);

/* Figure out what character set & collation this attachment prefers */

	find_intl_charset(tdbb, attachment, &options);

#ifdef WIN_NT
	dbb->dbb_filename.assign(first_dbb_file->fil_string,
									first_dbb_file->fil_length);
#else
	dbb->dbb_filename = expanded_name;
#endif

	// NS: Use alias as database ID only if accessing database using file name is not possible.
	//
	// This way we:
	// 1. Ensure uniqueness of ID even in presence of multiple processes
	// 2. Make sure that ID value can be used to connect back to database
	//
	if (is_alias && vdn == vdnFail)
		dbb->dbb_database_name.assign(file_name, file_length ? file_length : strlen(file_name));
	else
		dbb->dbb_database_name = dbb->dbb_filename;

#ifdef GOVERNOR
	if (!options.dpb_sec_attach) {
		if (JRD_max_users) {
			if (num_attached < (JRD_max_users * ATTACHMENTS_PER_USER))
				num_attached++;
			else
				ERR_post(isc_max_att_exceeded, 0);
		}
	}
	else {
		attachment->att_flags |= ATT_security_db;
	}
#endif /* GOVERNOR */

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	V4_JRD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
	JRD_SS_MUTEX_UNLOCK;

	*handle = attachment;
	CCH_flush(tdbb, FLUSH_FINI, 0);

	return return_success(tdbb);

	}	// try
	catch (const std::exception& ex)
	{
		ISC_STATUS_ARRAY temp_status;
		try
		{
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
			if (initing_security)
			{
				V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
			}
#endif
			tdbb->tdbb_status_vector = temp_status;
			dbb->dbb_flags &= ~DBB_being_opened;
			release_attachment(attachment);

			/* At this point, mutex dbb->dbb_mutexes [DBB_MUTX_init_fini] has been
			   unlocked and mutex databases_mutex has been locked. */
			if (MemoryPool::blk_type(dbb) == type_dbb)
			{
				if (!dbb->dbb_attachments)
				{
					shutdown_database(dbb, true);
				}
				else if (attachment)
				{
					delete attachment;
				}
			}
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
			V4_JRD_MUTEX_UNLOCK(databases_mutex);
#endif
		}
		catch (const std::exception&) {}
		tdbb->tdbb_status_vector = status;
		JRD_SS_MUTEX_UNLOCK;
		return error(user_status, ex);
	}
}


ISC_STATUS GDS_DATABASE_INFO(ISC_STATUS* user_status,
							Attachment** handle,
							SSHORT item_length,
							const SCHAR* items,
							SSHORT buffer_length,
							SCHAR* buffer)
{
/**************************************
 *
 *	g d s _ $ d a t a b a s e _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on database object.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	if (check_database(tdbb, *handle, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_database_info2, *handle, item_length, items, buffer_length);
#endif

	try
	{
		tdbb->tdbb_status_vector = user_status;

		INF_database_info(items, item_length, buffer, buffer_length);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_DDL(ISC_STATUS* user_status,
					Attachment** db_handle,
					jrd_tra** tra_handle,
					USHORT ddl_length,
					const SCHAR* ddl)
{
/**************************************
 *
 *	g d s _ $ d d l
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	Attachment* attachment = *db_handle;
	if (check_database(tdbb, attachment, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_ddl, *db_handle, *tra_handle, ddl_length, ddl);
#endif

	tdbb->tdbb_status_vector = user_status;
	jrd_tra* transaction = 0;

	try {

		transaction = find_transaction(tdbb, *tra_handle, isc_segstr_wrong_db);

		DYN_ddl(attachment, transaction, ddl_length,
				reinterpret_cast<const UCHAR*>(ddl));

	}	// try
	catch (const std::exception& ex) {
		return error(user_status, ex);
	}

/*
 * Perform an auto commit for autocommit transactions.
 * This is slightly tricky.  If the commit retain works,
 * all is well.  If TRA_commit () fails, we perform
 * a rollback_retain ().  This will backout the
 * effects of the transaction, mark it dead and
 * start a new transaction.

 * For now only ExpressLink will use this feature.  Later
 * a new entry point may be added.
 */

	if (transaction->tra_flags & TRA_perform_autocommit)
	{
		transaction->tra_flags &= ~TRA_perform_autocommit;

		try {
			TRA_commit(tdbb, transaction, true);
		}
		catch (const std::exception& ex) {
			ISC_STATUS_ARRAY temp_status;
			tdbb->tdbb_status_vector = temp_status;
			try {
				TRA_rollback(tdbb, transaction, true);
			}
			catch (const std::exception&) {
			    // CVC, TMN: Do nothing, see FB1 code, this will fall into
			    // the two lines below, achieving the same logic than going
			    // back to the SETJMP(env) in FB1.
			}
			tdbb->tdbb_status_vector = user_status;

			return error(user_status, ex);
		}
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_DETACH(ISC_STATUS* user_status, Attachment** handle)
{
/**************************************
 *
 *	g d s _ $ d e t a c h
 *
 **************************************
 *
 * Functional description
 *	Close down a database.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	Attachment* attachment = *handle;

/* Check out the database handle.  This is mostly code from
   the routine "check_database" */

	Database* dbb;
	if (!attachment ||
		(MemoryPool::blk_type(attachment) != type_att) ||
		!(dbb = attachment->att_database) ||
		MemoryPool::blk_type(dbb) != type_dbb)
	{
		return handle_error(user_status, isc_bad_db_handle, tdbb);
	}

/* Make sure this is a valid attachment */

	Attachment* attach;
	for (attach = dbb->dbb_attachments; attach; attach = attach->att_next)
		if (attach == attachment)
			break;

	if (!attach)
		return handle_error(user_status, isc_bad_db_handle, tdbb);

/* if this is the last attachment, mark dbb as not in use */

	JRD_SS_MUTEX_LOCK;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	V4_JRD_MUTEX_LOCK(databases_mutex);
#endif
	if (dbb->dbb_attachments == attachment && !attachment->att_next &&
		!(dbb->dbb_flags & DBB_being_opened))
		dbb->dbb_flags |= DBB_not_in_use;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	V4_JRD_MUTEX_UNLOCK(databases_mutex);
#endif

	tdbb->tdbb_database = dbb;
	tdbb->tdbb_attachment = attachment;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;

/* Count active thread in database */

	++dbb->dbb_use_count;

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_detach, *handle);
	LOG_call(log_statistics, dbb->dbb_reads, dbb->dbb_writes,
			 dbb->dbb_max_memory);
#endif

/* purge_attachment below can do an ERR_post */

	tdbb->tdbb_status_vector = user_status;

	try {

/* Purge attachment, don't rollback open transactions */

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif

#ifdef CANCEL_OPERATION
	attachment->att_flags |= ATT_cancel_disable;
#endif

#ifdef GOVERNOR
	const USHORT attachment_flags = (USHORT) attachment->att_flags;
#endif

	purge_attachment(tdbb, user_status, attachment, false);

#ifdef GOVERNOR
	if (JRD_max_users) {
		if (!(attachment_flags & ATT_security_db)) {
			fb_assert(num_attached > 0);
			num_attached--;
		}
	}
#endif /* GOVERNOR */

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	V4_JRD_MUTEX_UNLOCK(databases_mutex);
#endif
	JRD_SS_MUTEX_UNLOCK;

	*handle = NULL;

	return return_success(tdbb);

	}	// try
	catch (const std::exception& ex) {
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_LOCK(databases_mutex);
#endif
		dbb->dbb_flags &= ~DBB_not_in_use;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_UNLOCK(databases_mutex);
#endif

		JRD_SS_MUTEX_UNLOCK;
		return error(user_status, ex);
	}
}


ISC_STATUS GDS_DROP_DATABASE(ISC_STATUS* user_status, Attachment** handle)
{
/**************************************
 *
 *	i s c _ d r o p _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Close down and purge a database.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	Attachment* attachment = *handle;

/* Check out the database handle.  This is mostly code from
   the routine "check_database" */

	Database* dbb;
	if (!attachment ||
		(MemoryPool::blk_type(attachment) != type_att) ||
		!(dbb = attachment->att_database) ||
		MemoryPool::blk_type(dbb) != type_dbb)
	{
		return handle_error(user_status, isc_bad_db_handle, tdbb);
	}

/* Make sure this is a valid attachment */

	Attachment* attach;
	for (attach = dbb->dbb_attachments; attach; attach = attach->att_next)
		if (attach == attachment)
			break;

	if (!attach)
		return handle_error(user_status, isc_bad_db_handle, tdbb);

	bool err = false; // so much for uninitialized vars... if something
	// failed before the first call to drop_files, which was the value?
	{
		Jrd::ContextPoolHolder context(tdbb, dbb->dbb_permanent);
		tdbb->tdbb_database = dbb;
		tdbb->tdbb_attachment = attachment;
		tdbb->tdbb_request = NULL;
		tdbb->tdbb_transaction = NULL;

/* Count active thread in database */

		++dbb->dbb_use_count;

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
		LOG_call(log_drop_database, *handle);
		LOG_call(log_statistics, dbb->dbb_reads, dbb->dbb_writes,
			 dbb->dbb_max_memory);
#endif

		tdbb->tdbb_status_vector = user_status;
		try
		{
			if (!(attachment->att_user->usr_flags & (USR_locksmith | USR_owner)))
				ERR_post(isc_no_priv,
					isc_arg_string, "drop",
					isc_arg_string, "database",
					isc_arg_string,
					ERR_cstring((const SCHAR*) tdbb->tdbb_attachment->att_filename.c_str()), 0);

			if (attachment->att_flags & ATT_shutdown)
				ERR_post(isc_shutdown, isc_arg_string,
					ERR_cstring((const SCHAR*) tdbb->tdbb_attachment->att_filename.c_str()), 0);

			if (!CCH_exclusive(tdbb, LCK_PW, WAIT_PERIOD))
				ERR_post(isc_lock_timeout, isc_arg_gds, isc_obj_in_use,
					isc_arg_string,
					ERR_cstring((const SCHAR*) tdbb->tdbb_attachment->att_filename.c_str()), 0);

			JRD_SS_MUTEX_LOCK;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
			V4_JRD_MUTEX_LOCK(databases_mutex);
			V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
		}
		catch(const std::exception& ex)
		{
			return error(user_status, ex);
		}
	
		try {

/* Check if same process has more attachments */

			if ((attach = dbb->dbb_attachments) && (attach->att_next)) {
				ERR_post(isc_no_meta_update, isc_arg_gds, isc_obj_in_use,
						isc_arg_string, "DATABASE", 0);
			}

#ifdef CANCEL_OPERATION
			attachment->att_flags |= ATT_cancel_disable;
#endif

/* Here we have database locked in exclusive mode.
   Just mark the header page with an 0 ods version so that no other
   process can attach to this database once we release our exclusive
   lock and start dropping files. */

		   	WIN window(HEADER_PAGE);
			Ods::header_page* header = (Ods::header_page*) CCH_FETCH(tdbb, &window, LCK_write, pag_header);
			CCH_MARK_MUST_WRITE(tdbb, &window);
			header->hdr_ods_version = 0;
			CCH_RELEASE(tdbb, &window);

		}	// try
		catch (const std::exception& ex) {
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
			V4_JRD_MUTEX_UNLOCK(databases_mutex);
			V4_JRD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
			JRD_SS_MUTEX_UNLOCK;
			return error(user_status, ex);
		}
	} // dbb permanent context
    
/* A default catch all */
	try {

/* This point on database is useless */

/* mark the dbb unusable */

		dbb->dbb_flags |= DBB_not_in_use;
		*handle = NULL;

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_UNLOCK(databases_mutex);
#endif
		const jrd_file* file = dbb->dbb_file;
		const Shadow* shadow = dbb->dbb_shadow;

#ifdef GOVERNOR
		if (JRD_max_users) {
			fb_assert(num_attached > 0);
			num_attached--;
		}
#endif /* GOVERNOR */

/* Unlink attachment from database */

		release_attachment(attachment);

/* At this point, mutex dbb->dbb_mutexes [DBB_MUTX_init_fini] has been
   unlocked and mutex databases_mutex has been locked. */

		shutdown_database(dbb, false);

/* drop the files here. */

		err = drop_files(file);
		for (; shadow; shadow = shadow->sdw_next) 
		{
			err = err || drop_files(shadow->sdw_file);
		}

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_UNLOCK(databases_mutex);
#endif
	}	// try
	catch (const std::exception& ex) {
		JRD_SS_MUTEX_UNLOCK;
		return error(user_status, ex);
	}
	JRD_SS_MUTEX_UNLOCK;

	Database::deleteDbb(dbb);
	tdbb->tdbb_database = NULL;
	if (err) {
		user_status[0] = isc_arg_gds;
		user_status[1] = isc_drdb_completed_with_errs;
		user_status[2] = isc_arg_end;
		return user_status[1];
	}

	return return_success(tdbb);

}


ISC_STATUS GDS_GET_SEGMENT(ISC_STATUS * user_status,
							blb** blob_handle,
							USHORT * length,
							USHORT buffer_length,
							UCHAR * buffer)
{
/**************************************
 *
 *	g d s _ $ g e t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Abort a partially completed blob.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	blb* blob = check_blob(tdbb, user_status, blob_handle);
	if (!blob)
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_get_segment2, *blob_handle, length, buffer_length);
#endif

	try
	{
		tdbb->tdbb_status_vector = user_status;

		*length = BLB_get_segment(tdbb, blob, buffer, buffer_length);
		tdbb->tdbb_status_vector[0] = isc_arg_gds;
		tdbb->tdbb_status_vector[2] = isc_arg_end;
		Database* dbb = tdbb->tdbb_database;
	
		if (blob->blb_flags & BLB_eof) {
			JRD_restore_context();
			--dbb->dbb_use_count;
			return (user_status[1] = isc_segstr_eof);
		}
		else if (!(blob->blb_flags & BLB_stream) && blob->blb_fragment_size) {
			JRD_restore_context();
			--dbb->dbb_use_count;
			return (user_status[1] = isc_segment);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_GET_SLICE(ISC_STATUS* user_status,
						Attachment** db_handle,
						jrd_tra** tra_handle,
						ISC_QUAD* array_id,
						USHORT sdl_length,
						const UCHAR* sdl,
						USHORT param_length,
						const UCHAR* param,
						SLONG slice_length,
						UCHAR* slice,
						SLONG* return_length)
{
/**************************************
 *
 *	g d s _ $ g e t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Snatch a slice of an array.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	if (check_database(tdbb, *db_handle, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_get_slice2, *db_handle, *tra_handle, *array_id, sdl_length,
			 sdl, param_length, param, slice_length);
#endif

	try
	{
		tdbb->tdbb_status_vector = user_status;

		jrd_tra* transaction =
			find_transaction(tdbb, *tra_handle, isc_segstr_wrong_db);
	
		if (!array_id->gds_quad_low && !array_id->gds_quad_high) {
			MOVE_CLEAR(slice, slice_length);
			*return_length = 0;
		}
		else
			*return_length = BLB_get_slice(tdbb,
									   transaction,
									   reinterpret_cast<bid*>(array_id),
									   sdl,
									   param_length,
									   reinterpret_cast<const SLONG*>(param),
									   slice_length, slice);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_OPEN_BLOB2(ISC_STATUS* user_status,
						Attachment** db_handle,
						jrd_tra** tra_handle,
						blb** blob_handle,
						bid* blob_id,
						USHORT bpb_length,
						const UCHAR* bpb)
{
/**************************************
 *
 *	g d s _ $ o p e n _ b l o b 2
 *
 **************************************
 *
 * Functional description
 *	Open an existing blob.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	NULL_CHECK(blob_handle, isc_bad_segstr_handle);

	if (check_database(tdbb, *db_handle, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_open_blob2, *db_handle, *tra_handle, *blob_handle, blob_id,
			 bpb_length, bpb);
#endif

	try
	{
		tdbb->tdbb_status_vector = user_status;

		jrd_tra* transaction =
			find_transaction(tdbb, *tra_handle, isc_segstr_wrong_db);
		blb* blob = BLB_open2(tdbb, transaction, blob_id, bpb_length, bpb);
		*blob_handle = blob;
	
	#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
		LOG_call(log_handle_returned, *blob_handle);
	#endif
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_PREPARE(ISC_STATUS * user_status,
						jrd_tra** tra_handle,
						USHORT length,
						UCHAR * msg)
{
/**************************************
 *
 *	g d s _ $ p r e p a r e
 *
 **************************************
 *
 * Functional description
 *	Prepare a transaction for commit.  First phase of a two
 *	phase commit.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	CHECK_HANDLE((*tra_handle), type_tra, isc_bad_trans_handle);
	jrd_tra* transaction = *tra_handle;

	if (check_database(tdbb, transaction->tra_attachment, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_prepare2, *tra_handle, length, msg);
#endif

	if (prepare(tdbb, transaction, user_status, length, msg))
		return error(user_status);

	return return_success(tdbb);
}


ISC_STATUS GDS_PUT_SEGMENT(ISC_STATUS* user_status,
							blb** blob_handle,
							USHORT buffer_length,
							const UCHAR* buffer)
{
/**************************************
 *
 *	g d s _ $ p u t _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Abort a partially completed blob.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	blb* blob = check_blob(tdbb, user_status, blob_handle);
	if (!blob)
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_put_segment, *blob_handle, buffer_length, buffer);
#endif

	tdbb->tdbb_status_vector = user_status;
	try
	{
		BLB_put_segment(tdbb, blob, buffer, buffer_length);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_PUT_SLICE(ISC_STATUS* user_status,
						Attachment** db_handle,
						jrd_tra** tra_handle,
						ISC_QUAD* array_id,
						USHORT sdl_length,
						const UCHAR* sdl,
						USHORT param_length,
						const UCHAR* param,
						SLONG slice_length,
						UCHAR* slice)
{
/**************************************
 *
 *	g d s _ $ p u t _ s l i c e
 *
 **************************************
 *
 * Functional description
 *	Snatch a slice of an array.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	if (check_database(tdbb, *db_handle, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_put_slice, *db_handle, *tra_handle, *array_id, sdl_length,
			 sdl, param_length, param, slice_length, slice);
#endif

	tdbb->tdbb_status_vector = user_status;
	try
	{
		jrd_tra* transaction =
			find_transaction(tdbb, *tra_handle, isc_segstr_wrong_db);
		BLB_put_slice(tdbb,
				  transaction,
				  reinterpret_cast<bid*>(array_id),
				  sdl,
				  param_length,
				  reinterpret_cast<const SLONG*>(param), slice_length, slice);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_QUE_EVENTS(ISC_STATUS* user_status,
							Attachment** handle,
							SLONG* id,
							SSHORT length,
							const UCHAR* items,
							FPTR_EVENT_CALLBACK ast,
							void* arg)
{
/**************************************
 *
 *	g d s _ $ q u e _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *	Que a request for event notification.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	if (check_database(tdbb, *handle, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_que_events, *handle, length, items);
#endif

	tdbb->tdbb_status_vector = user_status;
	try
	{
		Database* dbb = tdbb->tdbb_database;
		Lock* lock = dbb->dbb_lock;
		Attachment* attachment = tdbb->tdbb_attachment;
	
		if (!attachment->att_event_session &&
			!(attachment->att_event_session = EVENT_create_session(user_status)))
		{
			return error(user_status);
		}
	
		*id = EVENT_que(user_status,
						attachment->att_event_session,
						lock->lck_length,
						(const TEXT*) & lock->lck_key, length, items, ast, arg);
	
	#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
		LOG_call(log_handle_returned, *id);
	#endif
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_RECEIVE(ISC_STATUS * user_status,
						jrd_req** req_handle,
						USHORT msg_type,
						USHORT msg_length,
						SCHAR * msg,
						SSHORT level
#ifdef SCROLLABLE_CURSORS
						, USHORT direction,
						ULONG offset
#endif
	)
{
/**************************************
 *
 *	g d s _ $ r e c e i v e
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	CHECK_HANDLE((*req_handle), type_req, isc_bad_req_handle);
	jrd_req* request = *req_handle;

	if (check_database(tdbb, request->req_attachment, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_receive2, *req_handle, msg_type, msg_length, level);
#endif

	tdbb->tdbb_status_vector = user_status;
	try
	{
		verify_request_synchronization(request, level);
	
	#ifdef SCROLLABLE_CURSORS
		if (direction)
			EXE_seek(tdbb, request, direction, offset);
	#endif
	
		EXE_receive(tdbb, request, msg_type, msg_length,
					reinterpret_cast<UCHAR*>(msg));
	
		check_autocommit(request, tdbb);
	
		if (request->req_flags & req_warning) {
			request->req_flags &= ~req_warning;
			return error(user_status);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_RECONNECT(ISC_STATUS* user_status,
						Attachment** db_handle,
						jrd_tra** tra_handle,
						SSHORT length,
						const UCHAR* id)
{
/**************************************
 *
 *	g d s _ $ r e c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Connect to a transaction in limbo.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	NULL_CHECK(tra_handle, isc_bad_trans_handle);
	Attachment* attachment = *db_handle;

	if (check_database(tdbb, attachment, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_reconnect, *db_handle, *tra_handle, length, id);
#endif

	tdbb->tdbb_status_vector = user_status;
	try
	{
		jrd_tra* transaction = TRA_reconnect(tdbb, id, length);
		*tra_handle = transaction;
	
	#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
		LOG_call(log_handle_returned, *tra_handle);
	#endif
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_RELEASE_REQUEST(ISC_STATUS * user_status, jrd_req** req_handle)
{
/**************************************
 *
 *	g d s _ $ r e l e a s e _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Release a request.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	CHECK_HANDLE((*req_handle), type_req, isc_bad_req_handle);
	jrd_req* request = *req_handle;
	Attachment* attachment = request->req_attachment;

	if (check_database(tdbb, attachment, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_release_request, *req_handle);
#endif

	tdbb->tdbb_status_vector = user_status;
	try
	{
		CMP_release(tdbb, request);
		*req_handle = NULL;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_REQUEST_INFO(ISC_STATUS* user_status,
							jrd_req** req_handle,
							SSHORT level,
							SSHORT item_length,
							const SCHAR* items,
							SSHORT buffer_length,
							SCHAR* buffer)
{
/**************************************
 *
 *	g d s _ $ r e q u e s t _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on blob object.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	jrd_req* request = *req_handle;
	CHECK_HANDLE(request, type_req, isc_bad_req_handle);

	if (check_database(tdbb, request->req_attachment, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_request_info2, *req_handle, level, item_length, items,
			 buffer_length);
#endif

	tdbb->tdbb_status_vector = user_status;
	try
	{
		verify_request_synchronization(request, level);

		INF_request_info(request, items, item_length, buffer, buffer_length);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_ROLLBACK_RETAINING(ISC_STATUS * user_status,
									jrd_tra** tra_handle)
{
/**************************************
 *
 *	i s c _ r o l l b a c k _ r e t a i n i n g
 *
 **************************************
 *
 * Functional description
 *	Abort a transaction but keep the environment valid
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	jrd_tra* transaction = *tra_handle;
	CHECK_HANDLE(transaction, type_tra, isc_bad_trans_handle);

	if (check_database(tdbb, transaction->tra_attachment, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_rollback, *tra_handle);
#endif

	if (rollback(tdbb, transaction, user_status, true))
		return error(user_status);

	return return_success(tdbb);
}


ISC_STATUS GDS_ROLLBACK(ISC_STATUS * user_status, jrd_tra** tra_handle)
{
/**************************************
 *
 *	g d s _ $ r o l l b a c k
 *
 **************************************
 *
 * Functional description
 *	Abort a transaction.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	jrd_tra* transaction = *tra_handle;
	CHECK_HANDLE(transaction, type_tra, isc_bad_trans_handle);

	if (check_database(tdbb, transaction->tra_attachment, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_rollback, *tra_handle);
#endif

	if (rollback(tdbb, transaction, user_status, false))
		return error(user_status);

	*tra_handle = NULL;
	return return_success(tdbb);
}


ISC_STATUS GDS_SEEK_BLOB(ISC_STATUS * user_status,
						blb** blob_handle,
						SSHORT mode,
						SLONG offset,
						SLONG * result)
{
/**************************************
 *
 *	g d s _ $ s e e k _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Seek a stream blob.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	blb* blob = check_blob(tdbb, user_status, blob_handle);
	if (!blob)
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_blob_seek, *blob_handle, mode, offset);
#endif

	tdbb->tdbb_status_vector = user_status;
	try
	{
		*result = BLB_lseek(blob, mode, offset);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_SEND(ISC_STATUS * user_status,
					jrd_req** req_handle,
					USHORT msg_type,
					USHORT msg_length,
					SCHAR * msg,
					SSHORT level)
{
/**************************************
 *
 *	g d s _ $ s e n d
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	CHECK_HANDLE((*req_handle), type_req, isc_bad_req_handle);
	jrd_req* request = *req_handle;

	if (check_database(tdbb, request->req_attachment, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_send, *req_handle, msg_type, msg_length, msg, level);
#endif

	tdbb->tdbb_status_vector = user_status;
	try
	{
		verify_request_synchronization(request, level);
	
		EXE_send(tdbb, request, msg_type, msg_length,
				reinterpret_cast<UCHAR*>(msg));
	
		check_autocommit(request, tdbb);
	
		if (request->req_flags & req_warning) {
			request->req_flags &= ~req_warning;
			return error(user_status);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_SERVICE_ATTACH(ISC_STATUS* user_status,
							USHORT service_length,
							const TEXT* service_name,
							Service** svc_handle,
							USHORT spb_length,
							const SCHAR* spb)
{
/**************************************
 *
 *	g d s _ $ s e r v i c e _ a t t a c h
 *
 **************************************
 *
 * Functional description
 *	Connect to an Interbase service.
 *
 **************************************/
	api_entry_point_init(user_status);

	if (*svc_handle)
		return handle_error(user_status, isc_bad_svc_handle, 0);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	tdbb->tdbb_status_vector = user_status;
	try
	{
		tdbb->tdbb_database = NULL;
	
		*svc_handle = SVC_attach(service_length, service_name, spb_length, spb);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_SERVICE_DETACH(ISC_STATUS* user_status, Service** svc_handle)
{
/**************************************
 *
 *	g d s _ $ s e r v i c e _ d e t a c h
 *
 **************************************
 *
 * Functional description
 *	Close down a service.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	Service* service = *svc_handle;
	CHECK_HANDLE(service, type_svc, isc_bad_svc_handle);

	tdbb->tdbb_status_vector = user_status;
	try
	{
		tdbb->tdbb_database = NULL;
	
		SVC_detach(service);
	
		*svc_handle = NULL;
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_SERVICE_QUERY(ISC_STATUS*	user_status,
							Service**	svc_handle,
							ULONG*	reserved,
							USHORT	send_item_length,
							const SCHAR*	send_items,
							USHORT	recv_item_length,
							const SCHAR*	recv_items,
							USHORT	buffer_length,
							SCHAR*	buffer)
{
/**************************************
 *
 *	g d s _ $ s e r v i c e _ q u e r y
 *
 **************************************
 *
 * Functional description
 *	Provide information on service object.
 *
 *	NOTE: The parameter RESERVED must not be used
 *	for any purpose as there are networking issues
 *	involved (as with any handle that goes over the
 *	network).  This parameter will be implemented at
 *	a later date.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	Service* service = *svc_handle;
	CHECK_HANDLE(service, type_svc, isc_bad_svc_handle);

	tdbb->tdbb_status_vector = user_status;
	tdbb->tdbb_database = NULL;

	try
	{
		if (service->svc_spb_version == isc_spb_version1)
			SVC_query(service, send_item_length, send_items, recv_item_length,
					recv_items, buffer_length, buffer);
		else {
			/* For SVC_query2, we are going to completly dismantle user_status (since at this point it is
			* meaningless anyway).  The status vector returned by this function can hold information about
			* the call to query the service manager and/or a service thread that may have been running.
			*/
	
			SVC_query2(service, tdbb, send_item_length, send_items,
					recv_item_length, recv_items, buffer_length, buffer);
	
			/* if there is a status vector from a service thread, copy it into the thread status */
			int len, warning;
			PARSE_STATUS(service->svc_status, len, warning);
			if (len) {
				MOVE_FASTER(service->svc_status, tdbb->tdbb_status_vector,
							sizeof(ISC_STATUS) * len);
	
				/* Empty out the service status vector */
				memset(service->svc_status, 0,
					ISC_STATUS_LENGTH * sizeof(ISC_STATUS));
			}
	
			if (user_status[1])
				return error(user_status);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}
	return return_success(tdbb);
}


ISC_STATUS GDS_SERVICE_START(ISC_STATUS*	user_status,
							Service**	svc_handle,
							ULONG*	reserved,
							USHORT	spb_length,
							const SCHAR*	spb)
{
/**************************************
 *
 *	g d s _ s e r v i c e _ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Start the specified service
 *
 *	NOTE: The parameter RESERVED must not be used
 *	for any purpose as there are networking issues
 *  	involved (as with any handle that goes over the
 *   	network).  This parameter will be implemented at
 * 	a later date.
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	Service* service = *svc_handle;
	CHECK_HANDLE(service, type_svc, isc_bad_svc_handle);

	tdbb->tdbb_status_vector = user_status;
	tdbb->tdbb_database = NULL;

	try
	{
		SVC_start(service, spb_length, spb);
	
		if (service->svc_status[1]) {
			ISC_STATUS* svc_status = service->svc_status;
			ISC_STATUS* tdbb_status = tdbb->tdbb_status_vector;
	
			while (*svc_status) {
				*tdbb_status++ = *svc_status++;
			}
			*tdbb_status = isc_arg_end;
		}
	
		if (user_status[1]) {
			return error(user_status);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_START_AND_SEND(ISC_STATUS* user_status,
							jrd_req** req_handle,
							jrd_tra** tra_handle,
							USHORT msg_type,
							USHORT msg_length,
							SCHAR* msg,
							SSHORT level)
{
/**************************************
 *
 *	g d s _ $ s t a r t _ a n d _ s e n d
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	jrd_req* request = *req_handle;
	CHECK_HANDLE(request, type_req, isc_bad_req_handle);

	if (check_database(tdbb, request->req_attachment, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_start_and_send, *req_handle, *tra_handle, msg_type,
			 msg_length, msg, level);
#endif

	tdbb->tdbb_status_vector = user_status;
	try
	{
		jrd_tra* transaction = find_transaction(tdbb, *tra_handle, isc_req_wrong_db);
	
		if (level)
			request = CMP_clone_request(tdbb, request, level, false);
	
		EXE_unwind(tdbb, request);
		EXE_start(tdbb, request, transaction);
		EXE_send(tdbb, request, msg_type, msg_length,
				reinterpret_cast<UCHAR*>(msg));
	
		check_autocommit(request, tdbb);
	
		if (request->req_flags & req_warning) {
			request->req_flags &= ~req_warning;
			return error(user_status);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_START(ISC_STATUS * user_status,
					jrd_req** req_handle,
					jrd_tra** tra_handle,
					SSHORT level)
{
/**************************************
 *
 *	g d s _ $ s t a r t
 *
 **************************************
 *
 * Functional description
 *	Get a record from the host program.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	jrd_req* request = *req_handle;
	CHECK_HANDLE(request, type_req, isc_bad_req_handle);

	if (check_database(tdbb, request->req_attachment, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_start, *req_handle, *tra_handle, level);
#endif

	tdbb->tdbb_status_vector = user_status;
	try
	{
		jrd_tra* transaction =
			find_transaction(tdbb, *tra_handle, isc_req_wrong_db);
	
		if (level)
			request = CMP_clone_request(tdbb, request, level, false);
	
		EXE_unwind(tdbb, request);
		EXE_start(tdbb, request, transaction);
	
		check_autocommit(request, tdbb);
	
		if (request->req_flags & req_warning) {
			request->req_flags &= ~req_warning;
			return error(user_status);
		}
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_START_MULTIPLE(ISC_STATUS * user_status,
							jrd_tra** tra_handle,
							USHORT count,
							TEB * vector)
{
/**************************************
 *
 *	g d s _ $ s t a r t _ m u l t i p l e
 *
 **************************************
 *
 * Functional description
 *	Start a transaction.
 *
 **************************************/
	TEB* v;

	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	NULL_CHECK(tra_handle, isc_bad_trans_handle);
	const TEB* const end = vector + count;

	for (v = vector; v < end; v++) {
		if (check_database(tdbb, *v->teb_database, user_status))
			return user_status[1];
		Database* dbb = tdbb->tdbb_database;
		--dbb->dbb_use_count;
	}

	if (check_database(tdbb, *vector->teb_database, user_status))
		return user_status[1];

	jrd_tra* prior = NULL;
	jrd_tra* transaction = NULL;

	try {

	for (v = vector; v < end; v++)
	{
		Attachment* attachment = *v->teb_database;
		if (check_database(tdbb, attachment, user_status)) {
			return user_status[1];
		}
#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
		LOG_call(log_start_multiple, *tra_handle, count, vector);
#endif
		tdbb->tdbb_status_vector = user_status;
		transaction =
			TRA_start(tdbb, v->teb_tpb_length,
					  reinterpret_cast<const char*>(v->teb_tpb));
		transaction->tra_sibling = prior;
		prior = transaction;
		Database* dbb = tdbb->tdbb_database;
		--dbb->dbb_use_count;
	}

	}	// try
	catch (const std::exception& ex) {
		Database* dbb = tdbb->tdbb_database;
		--dbb->dbb_use_count;
		if (prior) {
			ISC_STATUS_ARRAY temp_status;
			rollback(tdbb, prior, temp_status, false);
		}
		return error(user_status, ex);
	}

	*tra_handle = transaction;
#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_handle_returned, *tra_handle);
#endif

	return return_success(tdbb);
}


ISC_STATUS GDS_START_TRANSACTION(ISC_STATUS * user_status,
								jrd_tra** tra_handle,
								SSHORT count,
								...)
{
/**************************************
 *
 *	g d s _ $ s t a r t _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Start a transaction.
 *
 **************************************/
	api_entry_point_init(user_status);

	TEB tebs[16];
	TEB* teb;
	const TEB* end;
	va_list ptr;
	va_start(ptr, count);

	for (teb = tebs, end = teb + count; teb < end; teb++) {
		teb->teb_database = va_arg(ptr, Attachment**);
		teb->teb_tpb_length = va_arg(ptr, int);
		teb->teb_tpb = va_arg(ptr, UCHAR *);
	}
	va_end(ptr);

	return GDS_START_MULTIPLE(user_status, tra_handle, count, tebs);
}


ISC_STATUS GDS_TRANSACT_REQUEST(ISC_STATUS*	user_status,
								Attachment**		db_handle,
								jrd_tra**		tra_handle,
								USHORT	blr_length,
								const SCHAR*	blr,
								USHORT	in_msg_length,
								SCHAR*	in_msg,
								USHORT	out_msg_length,
								SCHAR*	out_msg)
{
/**************************************
 *
 *	i s c _ t r a n s a c t _ r e q u e s t
 *
 **************************************
 *
 * Functional description
 *	Execute a procedure.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	Attachment* attachment = *db_handle;
	if (check_database(tdbb, attachment, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_transact_request, *db_handle, *tra_handle,
			 blr_length, blr, in_msg_length, in_msg, out_msg_length);
#endif

	jrd_req* request = NULL;
	JrdMemoryPool* new_pool = 0;

	tdbb->tdbb_status_vector = user_status;

	try 
	{

	jrd_tra* transaction = find_transaction(tdbb, *tra_handle, isc_req_wrong_db);
	jrd_nod* in_message = NULL;
	jrd_nod* out_message = NULL;

	{
		new_pool = JrdMemoryPool::createPool();
		Jrd::ContextPoolHolder context(tdbb, new_pool);

		CompilerScratch* csb = PAR_parse(tdbb, reinterpret_cast<const UCHAR*>(blr), FALSE);
		request = CMP_make_request(tdbb, csb);
		CMP_verify_access(tdbb, request);

		jrd_nod* node;
		for (size_t i = 0; i < csb->csb_rpt.getCount(); i++)
		{
			if ( (node = csb->csb_rpt[i].csb_message) )
			{
				if ((int) (IPTR) node->nod_arg[e_msg_number] == 0) 
				{
					in_message = node;
				}
				else if ((int) (IPTR) node->nod_arg[e_msg_number] == 1) 
				{
					out_message = node;
				}
			}
		}
	} // new context

	request->req_attachment = attachment;

	USHORT len;
	if (in_msg_length)
	{
		if (in_message) {
			const Format* format = (Format*) in_message->nod_arg[e_msg_format];
			len = format->fmt_length;
		}
		else {
			len = 0;
		}
		if (in_msg_length != len)
		{
			ERR_post(isc_port_len,
					 isc_arg_number, (SLONG) in_msg_length,
					 isc_arg_number, (SLONG) len, 0);
		}
		if ((U_IPTR) in_msg & (ALIGNMENT - 1)) {
			MOVE_FAST(in_msg, (SCHAR *) request + in_message->nod_impure,
					  in_msg_length);
		}
		else {
			MOVE_FASTER(in_msg, (SCHAR *) request + in_message->nod_impure,
						in_msg_length);
		}
	}

	EXE_start(tdbb, request, transaction);

	if (out_message) {
		const Format* format = (Format*) out_message->nod_arg[e_msg_format];
		len = format->fmt_length;
	}
	else {
		len = 0;
	}
	if (out_msg_length != len) {
		ERR_post(isc_port_len,
				 isc_arg_number, (SLONG) out_msg_length,
				 isc_arg_number, (SLONG) len, 0);
	}

	if (out_msg_length) {
		if ((U_IPTR) out_msg & (ALIGNMENT - 1)) {
			MOVE_FAST((SCHAR *) request + out_message->nod_impure, out_msg,
					  out_msg_length);
		}
		else {
			MOVE_FASTER((SCHAR *) request + out_message->nod_impure, out_msg,
						out_msg_length);
		}
	}

	check_autocommit(request, tdbb);

	CMP_release(tdbb, request);

	return return_success(tdbb);

	}	// try
	catch (const std::exception& ex)
	{
		/* Set up to trap error in case release pool goes wrong. */

		try {
			if (request) {
				CMP_release(tdbb, request);
			}
			else if (new_pool) {
				JrdMemoryPool::deletePool(new_pool);
			}
		}	// try
		catch (const std::exception&) {
		}

		return error(user_status, ex);
	}	// catch
}


ISC_STATUS GDS_TRANSACTION_INFO(ISC_STATUS* user_status,
								jrd_tra** tra_handle,
								SSHORT item_length,
								const SCHAR* items,
								SSHORT buffer_length,
								SCHAR* buffer)
{
/**************************************
 *
 *	g d s _ $ t r a n s a c t i o n _ i n f o
 *
 **************************************
 *
 * Functional description
 *	Provide information on blob object.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	jrd_tra* transaction = *tra_handle;
	CHECK_HANDLE(transaction, type_tra, isc_bad_trans_handle);

	if (check_database(tdbb, transaction->tra_attachment, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_transaction_info2, *tra_handle, item_length, items,
			 buffer_length);
#endif

	tdbb->tdbb_status_vector = user_status;
	try
	{
		INF_transaction_info(transaction, items, item_length, buffer,
						 buffer_length);
	}
	catch (const std::exception& ex)
	{
		return error(user_status, ex);
	}

	return return_success(tdbb);
}


ISC_STATUS GDS_UNWIND(ISC_STATUS * user_status,
						jrd_req** req_handle,
						SSHORT level)
{
/**************************************
 *
 *	g d s _ $ u n w i n d
 *
 **************************************
 *
 * Functional description
 *	Unwind a running request.  This is potentially nasty since it can
 *	be called asynchronously.
 *
 **************************************/
	api_entry_point_init(user_status);

	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	CHECK_HANDLE((*req_handle), type_req, isc_bad_req_handle);
	jrd_req* request = *req_handle;

/* Make sure blocks look and feel kosher */

	Database* dbb;
	Attachment* attachment = request->req_attachment;
	if (!attachment ||
		(MemoryPool::blk_type(attachment) != type_att) ||
		!(dbb = attachment->att_database) ||
		MemoryPool::blk_type(dbb) != type_dbb)
	{
		return handle_error(user_status, isc_bad_db_handle, tdbb);
	}

/* Make sure this is a valid attachment */

	Attachment* attach;
	for (attach = dbb->dbb_attachments; attach; attach = attach->att_next)
	{
		if (attach == attachment) {
			break;
		}
	}

	if (!attach) {
		return handle_error(user_status, isc_bad_db_handle, tdbb);
	}

	tdbb->tdbb_database = dbb;

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call(log_unwind, *req_handle, level);
#endif

/* Set up error handling to restore old state */

	tdbb->tdbb_status_vector = user_status;
	tdbb->tdbb_attachment = attachment;

	try {

/* Pick up and validate request level */
		verify_request_synchronization(request, level);

		tdbb->tdbb_request = NULL;
		tdbb->tdbb_transaction = NULL;

/* Unwind request.  This just tweaks some bits */

		EXE_unwind(tdbb, request);

/* Restore old state and get out */

		JRD_restore_context();

		user_status[0] = isc_arg_gds;
		user_status[2] = isc_arg_end;

		return (user_status[1] = FB_SUCCESS);

	}	// try
	catch (const std::exception& ex) {
		Firebird::stuff_exception(user_status, ex);
		JRD_restore_context();
		return user_status[1];
	}
}


#ifdef MULTI_THREAD
void JRD_blocked(Attachment* blocking, BlockingThread** bt_que)
{
/**************************************
 *
 *	J R D _ b l o c k e d
 *
 **************************************
 *
 * Functional description
 *	We're blocked by another thread.  Unless it would cause
 *	a deadlock, wait for the other other thread (it will
 *	wake us up.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	Database*  dbb  = tdbb->tdbb_database;

/* Check for deadlock.  If there is one, complain */

	Attachment* attachment;
	for (attachment = blocking;
		 attachment;
		 attachment = attachment->att_blocking)
	{
		if (attachment == tdbb->tdbb_attachment) {
			ERR_post(isc_deadlock, 0);
		}
	}

	BlockingThread* block = dbb->dbb_free_btbs;
	if (block) {
		dbb->dbb_free_btbs = block->btb_next;
	}
	else {
		block = FB_NEW(*dbb->dbb_permanent) BlockingThread;
	}

	block->btb_thread_id = SCH_current_thread();
	block->btb_next = *bt_que;
	*bt_que = block;
	attachment = tdbb->tdbb_attachment;
	attachment->att_blocking = blocking;

	SCH_hiber();

	attachment->att_blocking = NULL;
}
#endif


#ifdef SUPERSERVER
bool JRD_getdir(Firebird::PathName& buf)
{
/**************************************
 *
 *	J R D _ g e t d i r
 *
 **************************************
 *
 * Functional description
 *	Current working directory is cached in the attachment
 *	block.  get it out. This function could be called before
 *	an attachment is created. In such a case thread specific
 *	data (t_data) will hold the user name which will be used
 *	to get the users home directory.
 *
 **************************************/
	char* t_data = NULL;
	char b[MAXPATHLEN];

	ThreadData::getSpecificData((void**) &t_data);

	if (t_data) {
#ifdef WIN_NT
		GetCurrentDirectory(MAXPATHLEN, b);
		buf = b;
#else
		const struct passwd* pwd;
		strcpy(b, t_data);
		pwd = getpwnam(b);
		if (pwd)
			buf = pwd->pw_dir;
		else	/* No home dir for this users here. Default to server dir */
			return fb_getcwd(buf);
#endif
	}
	else
	{
		thread_db* tdbb = JRD_get_thread_data();

   /** If the server has not done a JRD_set_thread_data prior to this call
       (which will be the case when connecting via IPC), thread_db will
       be NULL so do not attempt to get the attachment handle from
       thread_db. Just return false as described below.  
	   NOTE:  The only time
       this code is entered via IPC is if the database name = "".
   **/

   /** In case of backup/restore APIs, JRD_set_thread_data has been done but
       the thread's context is a 'gbak' specific, so don't try extract
       attachment from there.
   **/

		Attachment* attachment;
		if (tdbb && (tdbb->getType() == ThreadData::tddDBB))
			attachment = tdbb->tdbb_attachment;
		else
			return false;

   /**
    An older version of client will not be sending isc_dpb_working directory
    so in all probabilities attachment->att_working_directory will be null.
    return false so that ISC_expand_filename will create the file in fbserver's dir
   **/
		if (!attachment || attachment->att_working_directory.empty())
		{
			return false;
		}
		buf = attachment->att_working_directory;
	}

	return true;
}
#endif /* SUPERSERVER */


void JRD_mutex_lock(MUTX mutex)
{
/**************************************
 *
 *	J R D _ m u t e x _ l o c k
 *
 **************************************
 *
 * Functional description
 *	Lock a mutex and note this fact
 *	in the thread context block.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	INUSE_insert(&tdbb->tdbb_mutexes, (void *) mutex, true);
	THD_MUTEX_LOCK(mutex);
}


void JRD_mutex_unlock(MUTX mutex)
{
/**************************************
 *
 *	J R D _ m u t e x _ u n l o c k
 *
 **************************************
 *
 * Functional description
 *	Unlock a mutex and note this fact
 *	in the thread context block.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	INUSE_remove(&tdbb->tdbb_mutexes, (void *) mutex, false);
	THD_MUTEX_UNLOCK(mutex);
}


#ifdef SUPERSERVER
void JRD_print_all_counters(const char* fname)
{
/*****************************************************
 *
 *	J R D _ p r i n t _ a l l _ c o u n t e r s
 *
 *****************************************************
 *
 * Functional description
 *	print memory counters from DSQL, ENGINE & REMOTE
 *      component
 *
 ******************************************************/
	// Skidder: This may be ported to new memory pools, but
    // I think this debugging feature may be safely eradicated
	return;
}

#endif

#ifdef DEBUG_PROCS
void JRD_print_procedure_info(thread_db* tdbb, const char* mesg)
{
/*****************************************************
 *
 *	J R D _ p r i n t _ p r o c e d u r e _ i n f o
 *
 *****************************************************
 *
 * Functional description
 *	print name , use_count of all procedures in
 *      cache
 *
 ******************************************************/
	TEXT fname[MAXPATHLEN];

	gds__prefix(fname, "proc_info.log");
	FILE* fptr = fopen(fname, "a+");
	if (!fptr) {
		char buff[256];
		sprintf(buff, "Failed to open %s\n", fname);
		gds__log(buff, 0);
		return;
	}

	if (mesg)
		fputs(mesg, fptr);
	fprintf(fptr,
			   "Prc Name      , prc id , flags  ,  Use Count , Alter Count\n");

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	V4_JRD_MUTEX_LOCK(databases_mutex);
#endif

	vec* procedures = tdbb->tdbb_database->dbb_procedures;
	if (procedures) {
		vec::iterator ptr, end;
		for (ptr = procedures->begin(), end = procedures->end();
					ptr < end; ptr++)
		{
			const jrd_prc* procedure = (jrd_prc*) *ptr;
			if (procedure)
				fprintf(fptr, "%s  ,  %d,  %X,  %d, %d\n",
							(procedure->prc_name->hasData()) ?
								procedure->prc_name->c_str() : "NULL",
							procedure->prc_id,
							procedure->prc_flags, procedure->prc_use_count,
							procedure->prc_alter_count);
		}
	}
	else
		fprintf(fptr, "No Cached Procedures\n");

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	V4_JRD_MUTEX_UNLOCK(databases_mutex);
#endif

	fclose(fptr);

}
#endif /* DEBUG_PROCS */


#ifdef MULTI_THREAD
bool JRD_reschedule(thread_db* tdbb, SLONG quantum, bool punt)
{
/**************************************
 *
 *	J R D _ r e s c h e d u l e
 *
 **************************************
 *
 * Functional description
 *	Somebody has kindly offered to relinquish
 *	control so that somebody else may run.
 *
 **************************************/
	if (tdbb->tdbb_inhibit)
		return false;

/* Force garbage colletion activity to yield the
   processor in case client threads haven't had
   an opportunity to enter the scheduling queue. */

	if (!(tdbb->tdbb_flags & TDBB_sweeper))
		SCH_schedule();
	else {
		THREAD_EXIT();
		THREAD_YIELD();
		THREAD_ENTER();
	}

/* If database has been shutdown then get out */

	Database* dbb = tdbb->tdbb_database;
	Attachment* attachment = tdbb->tdbb_attachment;
	if (attachment) {
		if (dbb->dbb_ast_flags & DBB_shutdown &&
			attachment->att_flags & ATT_shutdown)
		{
			if (punt) {
				CCH_unwind(tdbb, false);
				ERR_post(isc_shutdown, 0);
			}
			else {
				ISC_STATUS* status = tdbb->tdbb_status_vector;
				*status++ = isc_arg_gds;
				*status++ = isc_shutdown;
				*status++ = isc_arg_end;
				return true;
			}
		}
#ifdef CANCEL_OPERATION

		/* If a cancel has been raised, defer its acknowledgement
		   when executing in the context of an internal request or
		   the system transaction. */

		if ((attachment->att_flags & ATT_cancel_raise) &&
			!(attachment->att_flags & ATT_cancel_disable))
		{
			const jrd_tra* transaction;
			jrd_req* request = tdbb->tdbb_request;
			if ((!request ||
				 !(request->req_flags & (req_internal | req_sys_trigger))) &&
				(!(transaction = tdbb->tdbb_transaction) ||
				 !(transaction->tra_flags & TRA_system)))
			{
				attachment->att_flags &= ~ATT_cancel_raise;
				if (punt) {
					CCH_unwind(tdbb, false);
					ERR_post(isc_cancelled, 0);
				}
				else {
					ISC_STATUS* status = tdbb->tdbb_status_vector;
					*status++ = isc_arg_gds;
					*status++ = isc_cancelled;
					*status++ = isc_arg_end;
					return true;
				}
			}
		}
#endif
	}

	tdbb->tdbb_quantum = (tdbb->tdbb_quantum <= 0) ?
		(quantum ? quantum : (ThreadPriorityScheduler::boosted() ? 
			Config::getPriorityBoost() : 1) * QUANTUM) : 
		tdbb->tdbb_quantum;

	return false;
}
#endif


void JRD_restore_context(void)
{
/**************************************
 *
 *	J R D _ r e s t o r e _ c o n t e x t
 *
 **************************************
 *
 * Functional description
 *	Restore the previous thread specific data
 *	and cleanup and objects that remain in use.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	bool cleaned_up =
		INUSE_cleanup(&tdbb->tdbb_mutexes, (FPTR_VOID) THD_mutex_unlock);
	// Logical OR would use short circuit boolean: function may not be called.
	// Left as bitwise OR. Condition may be reversed to use logical OR.
	cleaned_up |=
		INUSE_cleanup(&tdbb->tdbb_rw_locks, (FPTR_VOID) THD_wlck_unlock);

/* Charlie will fill this in
cleaned_up |= INUSE_cleanup (&tdbb->tdbb_pages, (FPTR_VOID) CCH_?);
*/
	ThreadData::restoreSpecific();

#ifdef DEV_BUILD
	if (tdbb->tdbb_status_vector &&
		!tdbb->tdbb_status_vector[1]
		&& cleaned_up)
	{
		gds__log("mutexes or pages in use on successful return");
	}
#endif
}


void JRD_inuse_clear(thread_db* tdbb)
{
/**************************************
 *
 *	J R D _ i n u s e _ c l e a r
 *
 **************************************
 *
 * Functional description
 *	Prepare thread_db for later use .Initialize the in-use 
 *	blocks so that we can unwind cleanly if an error occurs.
 *  Used in constructor and JRD_set_context().
 *
 **************************************/

	INUSE_clear(&tdbb->tdbb_mutexes);
	INUSE_clear(&tdbb->tdbb_rw_locks);
	INUSE_clear(&tdbb->tdbb_pages);
}


void JRD_set_context(thread_db* tdbb)
{
/**************************************
 *
 *	J R D _ s e t _ c o n t e x t
 *
 **************************************
 *
 * Functional description
 *	Set the thread specific data.  Initialize
 *	the in-use blocks so that we can unwind
 *	cleanly if an error occurs.
 *
 **************************************/

	JRD_inuse_clear(tdbb);
	tdbb->tdbb_status_vector = NULL;
	tdbb->putSpecific();
}


#ifdef MULTI_THREAD
void JRD_unblock(BlockingThread** bt_que)
{
/**************************************
 *
 *	J R D _ u n b l o c k
 *
 **************************************
 *
 * Functional description
 *	Unblock a linked list of blocked threads.  Rather
 *	than worrying about which, let 'em all loose.
 *
 **************************************/
	Database* dbb = GET_DBB();

	BlockingThread* block;
	while (block = *bt_que) {
		*bt_que = block->btb_next;
		if (block->btb_thread_id) {
			SCH_wake(block->btb_thread_id);
		}
		block->btb_next = dbb->dbb_free_btbs;
		dbb->dbb_free_btbs = block;
	}
}
#endif


void jrd_vtof(const char* string, char* field, SSHORT length)
{
/**************************************
 *
 *	j r d _ v t o f
 *
 **************************************
 *
 * Functional description
 *	Move a null terminated string to a fixed length
 *	field.
 *	If the length of the string pointed to by 'field'
 *	is less than 'length', this function pads the
 *	destination string with space upto 'length' bytes.
 *
 *	The call is primarily generated  by the preprocessor.
 *
 *	This is the same code as gds__vtof but is used internally.
 *
 **************************************/

	while (*string) {
		*field++ = *string++;
		if (--length <= 0) {
			return;
		}
	}

	if (length) {
		do {
			*field++ = ' ';
		} while (--length);
	}
}

static blb* check_blob(thread_db* tdbb, ISC_STATUS* user_status, blb** blob_handle)
{
/**************************************
 *
 *	c h e c k _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Check out a blob handle for goodness.  Return
 *	the address of the blob if ok, NULL otherwise.
 *
 **************************************/
	jrd_tra* transaction = 0;

	SET_TDBB(tdbb);
	 // this const made to check no accidental changes happen
	const blb* const blob = *blob_handle;

	if (!blob ||
		(MemoryPool::blk_type(blob) != type_blb) ||
		check_database(tdbb, blob->blb_attachment, user_status) ||
		!(transaction = blob->blb_transaction) ||
		MemoryPool::blk_type(transaction) != type_tra)
	{
		handle_error(user_status, isc_bad_segstr_handle, tdbb);
		return NULL;
	}

	tdbb->tdbb_transaction = transaction;

	return const_cast<blb*>(blob); // harmless, see comment above
}


static ISC_STATUS check_database(thread_db* tdbb, Attachment* attachment, ISC_STATUS * user_status)
{
/**************************************
 *
 *	c h e c k _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Check an attachment for validity, setting
 *	up environment.
 *
 **************************************/
	SET_TDBB(tdbb);

/* Make sure blocks look and feel kosher */
	Database* dbb;
	if (!attachment ||
		(MemoryPool::blk_type(attachment) != type_att) ||
		!(dbb = attachment->att_database) ||
		MemoryPool::blk_type(dbb)!= type_dbb)
	{
		return handle_error(user_status, isc_bad_db_handle, tdbb);
	}

/* Make sure this is a valid attachment */

#ifndef SUPERSERVER
	const Attachment* attach;
	for (attach = dbb->dbb_attachments; attach; attach = attach->att_next)
		if (attach == attachment)
			break;

	if (!attach)
		return handle_error(user_status, isc_bad_db_handle, tdbb);
#endif

	tdbb->tdbb_database = dbb;
	tdbb->tdbb_attachment = attachment;
	tdbb->tdbb_quantum = QUANTUM;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;
	Jrd::ContextPoolHolder context(tdbb, 0);
	tdbb->tdbb_inhibit = 0;
	tdbb->tdbb_flags = 0;

/* Count active threads in database */

	++dbb->dbb_use_count;

	// Want to avoid problems of literal strings v/s non-const pointers.
    static TEXT string[] = "can't continue after bugcheck";
    ISC_STATUS* ptr;
    
	if (dbb->dbb_flags & DBB_bugcheck) {
		tdbb->tdbb_status_vector = ptr = user_status;
		*ptr++ = isc_arg_gds;
		*ptr++ = isc_bug_check;
		*ptr++ = isc_arg_string;
		*ptr++ = (ISC_STATUS) string; // Warning: possible address truncation.
		*ptr = isc_arg_end;
		return error(user_status);
	}

	if (attachment->att_flags & ATT_shutdown ||
		((dbb->dbb_ast_flags & DBB_shutdown) &&
		 ((dbb->dbb_ast_flags & DBB_shutdown_full) ||
		 !(attachment->att_user->usr_flags & (USR_locksmith | USR_owner)))))
	{
		tdbb->tdbb_status_vector = ptr = user_status;
		*ptr++ = isc_arg_gds;
		*ptr++ = isc_shutdown;
		*ptr++ = isc_arg_cstring;
		*ptr++ = attachment->att_filename.length();
		*ptr++ = (ISC_STATUS)(IPTR) attachment->att_filename.c_str();
		*ptr = isc_arg_end;
		return error(user_status);
	}

#ifdef CANCEL_OPERATION
	if ((attachment->att_flags & ATT_cancel_raise) &&
		!(attachment->att_flags & ATT_cancel_disable))
	{
		attachment->att_flags &= ~ATT_cancel_raise;
		tdbb->tdbb_status_vector = ptr = user_status;
		*ptr++ = isc_arg_gds;
		*ptr++ = isc_cancelled;
		*ptr++ = isc_arg_end;
		return error(user_status);
	}
#endif

	return FB_SUCCESS;
}


static void cleanup(void* arg)
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
#if defined(V4_THREADING) && !defined(SUPERSERVER)
	V4_MUTEX_DESTROY(databases_mutex);
#endif
	JRD_SS_DESTROY_MUTEX;
	initialized = false;
	databases = NULL;
}


static ISC_STATUS commit(
					 ISC_STATUS* user_status,
					 jrd_tra** tra_handle, const bool retaining_flag)
{
/**************************************
 *
 *	c o m m i t
 *
 **************************************
 *
 * Functional description
 *	Commit a transaction.
 *
 **************************************/
	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	CHECK_HANDLE((*tra_handle), type_tra, isc_bad_trans_handle);
	jrd_tra* transaction = *tra_handle;
	jrd_tra* next = transaction;

	if (check_database(tdbb, transaction->tra_attachment, user_status))
		return user_status[1];

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	LOG_call((retaining_flag) ? log_commit_retaining : log_commit,
			 *tra_handle);
#endif

	ISC_STATUS* ptr = user_status;

	if (transaction->tra_sibling &&
		!(transaction->tra_flags & TRA_prepared) &&
		prepare(tdbb, transaction, ptr, 0, NULL))
	{
		return error(user_status);
	}
	
	try {

	while ( (transaction = next) ) {
		next = transaction->tra_sibling;
		check_database(tdbb, transaction->tra_attachment, user_status);
		tdbb->tdbb_status_vector = ptr;
		TRA_commit(tdbb, transaction, retaining_flag);
		Database* dbb = tdbb->tdbb_database;
		--dbb->dbb_use_count;
	}

	return return_success(tdbb);

	}	// try
	catch (const std::exception& ex) {
		Database* dbb = tdbb->tdbb_database;
		--dbb->dbb_use_count;
		return error(user_status, ex);
	}
}


static bool drop_files(const jrd_file* file)
{
/**************************************
 *
 *	d r o p _ f i l e s
 *
 **************************************
 *
 * Functional description
 *	drop a linked list of files
 *
 **************************************/
	ISC_STATUS_ARRAY status;

	status[1] = FB_SUCCESS;

	for (; file; file = file->fil_next)
	{
		if (unlink(file->fil_string))
		{
			IBERR_build_status(status, isc_io_error,
							   isc_arg_string, "unlink",
							   isc_arg_string,
							   ERR_cstring(file->fil_string),
							   isc_arg_gds, isc_io_delete_err,
							   SYS_ERR, errno,
							   0);
			Database* dbb = GET_DBB();
			gds__log_status(dbb->dbb_file->fil_string, status);
		}
	}

	return status[1] ? true : false;
}


static jrd_tra* find_transaction(thread_db* tdbb, jrd_tra* transaction, ISC_STATUS error_code)
{
/**************************************
 *
 *	f i n d _ t r a n s a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Find the element of a possible multiple database transaction
 *	that corresponds to the current database.
 *
 **************************************/

	SET_TDBB(tdbb);

	if (!transaction || MemoryPool::blk_type(transaction) != type_tra)
		ERR_post(isc_bad_trans_handle, 0);

	for (; transaction; transaction = transaction->tra_sibling)
		if (transaction->tra_attachment == tdbb->tdbb_attachment) {
			tdbb->tdbb_transaction = transaction;
			return transaction;
		}

	ERR_post(error_code, 0);
	return NULL;		/* Added to remove compiler warnings */
}


static ISC_STATUS error(ISC_STATUS* user_status, const std::exception& ex)
{
/**************************************
 *
 *	e r r o r
 *
 **************************************
 *
 * Functional description
 *	An error returned has been trapped.  Return a status code.
 *
 **************************************/
	Firebird::stuff_exception(user_status, ex);
	return error(user_status);
}

static ISC_STATUS error(ISC_STATUS* user_status)
{
/**************************************
 *
 *	e r r o r
 *
 **************************************
 *
 * Functional description
 *	An error returned has been trapped.  Return a status code.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

/* Decrement count of active threads in database */
	Database* dbb = tdbb->tdbb_database;
	if (dbb) {
		--dbb->dbb_use_count;
	}

	JRD_restore_context();

/* This is debugging code which is meant to verify that
   the database use count is cleared on exit from the
   engine. Database shutdown cannot succeed if the database
   use count is erroneously left set. */

#if (defined DEV_BUILD && !defined MULTI_THREAD)
	if (dbb && dbb->dbb_use_count && !(dbb->dbb_flags & DBB_security_db)) {
		dbb->dbb_use_count = 0;
		ISC_STATUS* p = user_status;
		*p++ = isc_arg_gds;
		*p++ = isc_random;
		*p++ = isc_arg_string;
		*p++ = (ISC_STATUS) "database use count set on error return";
		*p = isc_arg_end;
	}
#endif

	return user_status[1];
}


static void find_intl_charset(thread_db* tdbb, Attachment* attachment, const DatabaseOptions* options)
{
/**************************************
 *
 *	f i n d _ i n t l _ c h a r s e t
 *
 **************************************
 *
 * Functional description
 *	Attachment has declared it's prefered character set
 *	as part of LC_CTYPE, passed over with the attachment
 *	block.  Now let's resolve that to an internal subtype id.
 *
 **************************************/
	SET_TDBB(tdbb);

	if (options->dpb_lc_ctype.isEmpty()) {
		/* No declaration of character set, act like 3.x Interbase */
		attachment->att_charset = DEFAULT_ATTACHMENT_CHARSET;
		return;
	}

	SSHORT id;
	ISC_STATUS_ARRAY local_status;

	if (MET_get_char_subtype(tdbb,
							&id,
							reinterpret_cast<const UCHAR*>
								(options->dpb_lc_ctype.c_str()),
							options->dpb_lc_ctype.length()) &&
		INTL_defined_type(tdbb, local_status, id) &&
		(id != CS_BINARY))
	{
		attachment->att_charset = id;
	}
	else
	{
		/* Report an error - we can't do what user has requested */
		ERR_post(isc_bad_dpb_content,
				isc_arg_gds, isc_charset_not_found,
				isc_arg_string, ERR_cstring(options->dpb_lc_ctype),
				0);
	}
}


void DatabaseOptions::get(const UCHAR* dpb, USHORT dpb_length)
{
/**************************************
 *
 *	g e t _ o p t i o n s
 *
 **************************************
 *
 * Functional description
 *	Parse database parameter block picking up options and things.
 *
 **************************************/
	SSHORT num_old_files = 0;

	Database* dbb = GET_DBB();

	dpb_buffers = JRD_cache_default;
	dpb_sweep_interval = -1;
	dpb_overwrite = true;
	dpb_sql_dialect = 99;
	invalid_client_SQL_dialect = false;

	if (dpb_length == 0)
	{
		return;
	}
	if (dpb == NULL)
	{
		ERR_post(isc_bad_dpb_form, 0);
	}

	Firebird::ClumpletReader rdr(true, dpb, dpb_length);

	if (rdr.getBufferTag() != isc_dpb_version1)
	{
		ERR_post(isc_bad_dpb_form, isc_arg_gds, isc_wrodpbver, 0);
	}

	for (; !(rdr.isEof()); rdr.moveNext())
	{
		switch (rdr.getClumpTag())
		{
		case isc_dpb_working_directory:
			{
				rdr.getPath(dpb_working_directory);

				// CLASSIC have no thread data. Init to zero.
				char* t_data = 0;
				ThreadData::getSpecificData((void **) &t_data);

				/*
				   Null value for working_directory implies remote database. So get
				   the users HOME directory
				 */
#ifndef WIN_NT
				if (dpb_working_directory.isEmpty()) {
					struct passwd *passwd = NULL;

					if (t_data)
						passwd = getpwnam(t_data);
					if (passwd) 
					{
						dpb_working_directory = passwd->pw_dir;
					}
					else {		/*No home dir for this users here. Default to server dir */
						fb_getcwd(dpb_working_directory);
					}
				}
#endif
				if (t_data)
				{
					free(t_data);
					t_data = NULL;
				}
				/* Null out the thread local data so that further references will fail */
				ThreadData::putSpecificData(0);
			}
			break;

		case isc_dpb_set_page_buffers:
			dpb_page_buffers = rdr.getInt();
			if (dpb_page_buffers &&
				(dpb_page_buffers < MIN_PAGE_BUFFERS ||
				 dpb_page_buffers > MAX_PAGE_BUFFERS))
			{
				ERR_post(isc_bad_dpb_content, 0);
			}
			dpb_set_page_buffers = true;
			break;

		case isc_dpb_num_buffers:
			dpb_buffers = rdr.getInt();
			if (dpb_buffers < 10)
			{
				ERR_post(isc_bad_dpb_content, 0);
			}
			break;

		case isc_dpb_page_size:
			dpb_page_size = (USHORT) rdr.getInt();
			break;

		case isc_dpb_debug:
			dpb_debug = (USHORT) rdr.getInt();
			break;

		case isc_dpb_sweep:
			dpb_sweep = (USHORT) rdr.getInt();
			break;

		case isc_dpb_sweep_interval:
			dpb_sweep_interval = rdr.getInt();
			break;

		case isc_dpb_verify:
			dpb_verify = (USHORT) rdr.getInt();
			if (dpb_verify & isc_dpb_ignore)
				dbb->dbb_flags |= DBB_damaged;
			break;

		case isc_dpb_trace:
			dpb_trace = (USHORT) rdr.getInt();
			break;

		case isc_dpb_damaged:
			if (rdr.getInt() & 1)
				dbb->dbb_flags |= DBB_damaged;
			break;

		case isc_dpb_enable_journal:
		    rdr.getString(dpb_journal);
			break;

		case isc_dpb_wal_backup_dir:
			// ignore, skip
			break;

		case isc_dpb_drop_walfile:
			dpb_wal_action = (USHORT) rdr.getInt();
			break;

		case isc_dpb_old_dump_id:
		case isc_dpb_online_dump:
		case isc_dpb_old_file_size:
		case isc_dpb_old_num_files:
		case isc_dpb_old_start_page:
		case isc_dpb_old_start_seqno:
		case isc_dpb_old_start_file:
			// ignore, skip
			break;

		case isc_dpb_old_file:
			//if (num_old_files >= MAX_OLD_FILES) complain here, for now.
				ERR_post(isc_num_old_files, 0);
			// following code is never executed now !
			num_old_files++;
			break;

		case isc_dpb_wal_chkptlen:
		case isc_dpb_wal_numbufs:
		case isc_dpb_wal_bufsize:
		case isc_dpb_wal_grp_cmt_wait:
			// ignore, skip
			break;

		case isc_dpb_dbkey_scope:
			dpb_dbkey_scope = (USHORT) rdr.getInt();
			break;

		case isc_dpb_sys_user_name:
		    rdr.getString(dpb_sys_user_name);
			break;

		case isc_dpb_sql_role_name:
		    rdr.getString(dpb_role_name);
			break;

		case isc_dpb_user_name:
		    rdr.getString(dpb_user_name);
			break;

		case isc_dpb_password:
		    rdr.getString(dpb_password);
			break;

		case isc_dpb_password_enc:
		    rdr.getString(dpb_password_enc);
			break;

		case isc_dpb_encrypt_key:
#ifdef ISC_DATABASE_ENCRYPTION
		    rdr.getString(dpb_key);
#else
			/* Just in case there WAS a customer using this unsupported
			 * feature - post an error when they try to access it in 4.0
			 */
			ERR_post(isc_uns_ext, isc_arg_gds, isc_random,
					 isc_arg_string, "Encryption not supported", 0);
#endif
			break;

		case isc_dpb_no_garbage_collect:
			dpb_no_garbage = TRUE;
			break;

		case isc_dpb_disable_journal:
			dpb_disable = TRUE;
			break;

		case isc_dpb_activate_shadow:
			dpb_activate_shadow = true;
			break;

		case isc_dpb_delete_shadow:
			dpb_delete_shadow = true;
			break;

		case isc_dpb_force_write:
			dpb_set_force_write = TRUE;
			dpb_force_write = (SSHORT) rdr.getInt();
			break;

		case isc_dpb_begin_log:
#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
			rdr.getPath(dpb_log);
#endif
			break;

		case isc_dpb_quit_log:
#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
			dpb_quit_log = true;
#endif
			break;

		case isc_dpb_no_reserve:
			dpb_set_no_reserve = TRUE;
			dpb_no_reserve = (UCHAR) rdr.getInt();
			break;

		case isc_dpb_interp:
			dpb_interp = (SSHORT) rdr.getInt();
			break;

		case isc_dpb_lc_messages:
		    rdr.getPath(dpb_lc_messages);
			break;

		case isc_dpb_lc_ctype:
		    rdr.getString(dpb_lc_ctype);
			break;

		case isc_dpb_shutdown:
			dpb_shutdown = (USHORT) rdr.getInt();
			// Enforce default
			if ((dpb_shutdown & isc_dpb_shut_mode_mask) == isc_dpb_shut_default)
				dpb_shutdown |= isc_dpb_shut_multi;
			break;

		case isc_dpb_shutdown_delay:
			dpb_shutdown_delay = (SSHORT) rdr.getInt();
			break;

		case isc_dpb_online:
			dpb_online = (USHORT) rdr.getInt();
			// Enforce default
			if ((dpb_online & isc_dpb_shut_mode_mask) == isc_dpb_shut_default)
			{
				dpb_online |= isc_dpb_shut_normal;
			}
			break;

		case isc_dpb_reserved:
			{
				Firebird::string single;
				rdr.getString(single);
		    	if (single == "YES")
				{
					dpb_single_user = TRUE;
				}
			}
			break;

		case isc_dpb_overwrite:
			dpb_overwrite = rdr.getInt() != 0;
			break;

		case isc_dpb_sec_attach:
			dpb_sec_attach = rdr.getInt() != 0;
			dpb_buffers = 50;
			dbb->dbb_flags |= DBB_security_db;
			break;

		case isc_dpb_gbak_attach:
		    rdr.getString(dpb_gbak_attach);
			break;

		case isc_dpb_gstat_attach:
			dpb_gstat_attach = true;
			break;

		case isc_dpb_gfix_attach:
			dpb_gfix_attach = true;
			break;

		case isc_dpb_gsec_attach:
			dpb_gsec_attach = rdr.getBoolean();
			break;

		case isc_dpb_disable_wal:
			dpb_disable_wal = true;
			break;

		case isc_dpb_connect_timeout:
			dpb_connect_timeout = rdr.getInt();
			break;

		case isc_dpb_dummy_packet_interval:
			dpb_dummy_packet_interval = rdr.getInt();
			break;

		case isc_dpb_sql_dialect:
			dpb_sql_dialect = (USHORT) rdr.getInt();
			if (dpb_sql_dialect > SQL_DIALECT_V6)
					invalid_client_SQL_dialect = true;
			break;

		case isc_dpb_set_db_sql_dialect:
			dpb_set_db_sql_dialect = (USHORT) rdr.getInt();
			break;

		case isc_dpb_set_db_readonly:
			dpb_set_db_readonly = true;
			dpb_db_readonly = rdr.getInt() != 0;
			break;

		case isc_dpb_set_db_charset:
			rdr.getString(dpb_set_db_charset);
			break;

		default:
			break;
		}
	}

	if (! rdr.isEof())
	{
		ERR_post(isc_bad_dpb_form, 0);
	}
}


static ISC_STATUS handle_error(ISC_STATUS* user_status, ISC_STATUS code, thread_db* tdbb)
{
/**************************************
 *
 *	h a n d l e _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	An invalid handle has been passed in.  If there is a user status
 *	vector, make it reflect the error.  If not, emulate the routine
 *	"error" and abort.
 *
 **************************************/
	if (tdbb)
		JRD_restore_context();

	ISC_STATUS* vector = user_status;
	*vector++ = isc_arg_gds;
	*vector++ = code;
	*vector = isc_arg_end;

	return code;
}


#if defined (WIN_NT) && !defined(SERVER_SHUTDOWN)
static BOOLEAN handler_NT(SSHORT controlAction)
{
/**************************************
 *
 *      h a n d l e r _ N T
 *
 **************************************
 *
 * Functional description
 *	For some actions, get NT to issue a popup asking
 *	the user to delay.
 *
 **************************************/

	switch (controlAction) {
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		return TRUE;			/* NT will issue popup */

	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
		return FALSE;			/* let it go */
	}
/* So, what are we to return here?! */
	return FALSE;				/* let it go */
}
#endif


static Database* init(thread_db*	tdbb,
				ISC_STATUS*	user_status,
				const Firebird::PathName& expanded_filename,
				bool attach_flag)
{
/**************************************
 *
 *	i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize for database access.  First call from both CREATE and
 *	OPEN.
 *
 **************************************/
	MUTX_T temp_mutx[DBB_MUTX_max];
	WLCK_T temp_wlck[DBB_WLCK_max];

	SET_TDBB(tdbb);

/* If this is the first time through, initialize local mutexes and set
   up a cleanup handler.  Regardless, then lock the database mutex. */

	if (!initialized) {
		THD_INIT;
		THREAD_EXIT();
		THD_GLOBAL_MUTEX_LOCK;
		THREAD_ENTER();
		PluginManager::load_engine_plugins();
		if (!initialized) {
#if defined(V4_THREADING) && !defined(SUPERSERVER)
			V4_MUTEX_INIT(databases_mutex);
#endif
			JRD_SS_INIT_MUTEX;
			gds__register_cleanup(cleanup, 0);
			initialized = true;
			JRD_cache_default = Config::getDefaultDbCachePages();
			if (JRD_cache_default < MIN_PAGE_BUFFERS)
				JRD_cache_default = MIN_PAGE_BUFFERS;
			if (JRD_cache_default > MAX_PAGE_BUFFERS)
				JRD_cache_default = MAX_PAGE_BUFFERS;
#if defined (WIN_NT) && !defined(SERVER_SHUTDOWN)
			setup_NT_handlers();
#endif
		}
		THD_GLOBAL_MUTEX_UNLOCK;
	}

	JRD_SS_MUTEX_LOCK;
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	V4_JRD_MUTEX_LOCK(databases_mutex);
#endif

/* Check to see if the database is already actively attached */

	Database* dbb;
	for (dbb = databases; dbb; dbb = dbb->dbb_next)
	{
		if (!(dbb->dbb_flags & (DBB_bugcheck | DBB_not_in_use)) &&
#ifndef SUPERSERVER
			!(dbb->dbb_ast_flags & DBB_shutdown &&
			  dbb->dbb_ast_flags & DBB_shutdown_locks) &&
#endif
			 (dbb->dbb_filename == expanded_filename))
		{
			return (attach_flag) ? dbb : NULL;
		}
	}

/* Clean up temporary Database */

	/* MOVE_CLEAR(&temp, (SLONG) sizeof(Database)); */

/* set up the temporary database block with fields that are
   required for doing the ALL_init() */

	tdbb->tdbb_status_vector = user_status;
	tdbb->tdbb_database = 0;

	try {
#ifdef SUPERSERVER
	Firebird::MemoryStats temp_stats;
	JrdMemoryPool* perm = JrdMemoryPool::createDbPool(temp_stats);
	dbb = Database::newDbb(*perm);
	perm->setStatsGroup(dbb->dbb_memory_stats);
#else
	JrdMemoryPool* perm = JrdMemoryPool::createDbPool(MemoryPool::default_stats_group);
	dbb = Database::newDbb(*perm);
#endif
	//temp.blk_type = type_dbb;
	dbb->dbb_permanent = perm;
	dbb->dbb_mutexes = temp_mutx;
	dbb->dbb_rw_locks = temp_wlck;
	tdbb->tdbb_database = dbb;

	ALL_init();
	// provide context pool for the rest stuff
	Jrd::ContextPoolHolder context(tdbb, perm);

	dbb->dbb_next = databases;
	databases = dbb;

	dbb->dbb_mutexes = FB_NEW(*dbb->dbb_permanent) MUTX_T[DBB_MUTX_max];
	dbb->dbb_rw_locks = FB_NEW(*dbb->dbb_permanent) WLCK_T[DBB_WLCK_max];
	vec* vector = vec::newVector(*dbb->dbb_permanent, irq_MAX);
	dbb->dbb_internal = vector;
	dbb->dbb_dyn_req = vector = vec::newVector(*dbb->dbb_permanent, drq_MAX);
	dbb->dbb_flags |= DBB_exclusive;
	dbb->dbb_sweep_interval = SWEEP_INTERVAL;

/* set a garbage collection policy */

	if ((dbb->dbb_flags & (DBB_gc_cooperative | DBB_gc_background)) == 0)
	{
		Firebird::string gc_policy = Config::getGCPolicy();
		gc_policy.lower();
		if (gc_policy == GCPolicyCooperative) {
			dbb->dbb_flags |= DBB_gc_cooperative;
		}
		else if (gc_policy == GCPolicyBackground) {
			dbb->dbb_flags |= DBB_gc_background;
		}
		else if (gc_policy == GCPolicyCombined) {
			dbb->dbb_flags |= DBB_gc_cooperative | DBB_gc_background;
		}
		else // config value is invalid, use default
		{
			if (GCPolicyDefault == GCPolicyCooperative) {
				dbb->dbb_flags |= DBB_gc_cooperative;
			}
			else if (GCPolicyDefault == GCPolicyBackground) {
				dbb->dbb_flags |= DBB_gc_background;
			}
			else if (GCPolicyDefault == GCPolicyCombined) {
				dbb->dbb_flags |= DBB_gc_cooperative | DBB_gc_background;
			}
			else 
				fb_assert(false);
		}
	}

/* Initialize a number of subsystems */

	TRA_init(tdbb);

/* Lookup some external "hooks" */

	PluginManager::Plugin crypt_lib =
		PluginManager::enginePluginManager().findPlugin(CRYPT_IMAGE);
	if (crypt_lib) {
		Firebird::string encrypt_entrypoint(ENCRYPT);
		Firebird::string decrypt_entrypoint(DECRYPT);
		dbb->dbb_encrypt =
			(Database::crypt_routine) crypt_lib.lookupSymbol(encrypt_entrypoint);
		dbb->dbb_decrypt =
			(Database::crypt_routine) crypt_lib.lookupSymbol(decrypt_entrypoint);
	}

	INTL_init(tdbb);

	SecurityDatabase::initialize();

	return dbb;

	}	// try
	catch (const std::exception& ex) {
		Firebird::stuff_exception(user_status, ex);
		return 0;
	}
}



static ISC_STATUS prepare(thread_db*		tdbb,
					  jrd_tra*		transaction,
					  ISC_STATUS*	status_vector,
					  USHORT	length,
					  const UCHAR*	msg)
{
/**************************************
 *
 *	p r e p a r e
 *
 **************************************
 *
 * Functional description
 *	Attempt to prepare a transaction.  If it fails at any point, return
 *	an error.
 *
 **************************************/
	SET_TDBB(tdbb);

	try {

	for (; transaction; transaction = transaction->tra_sibling) {
		check_database(tdbb, transaction->tra_attachment, status_vector);
		tdbb->tdbb_status_vector = status_vector;
		TRA_prepare(tdbb, transaction, length, msg);
		Database* dbb = tdbb->tdbb_database;
		--dbb->dbb_use_count;
	}

	}	// try
	catch (const std::exception& ex) {
		Firebird::stuff_exception(status_vector, ex);
		Database* dbb = tdbb->tdbb_database;
		--dbb->dbb_use_count;
		return status_vector[1];
	}

	return FB_SUCCESS;
}


static void release_attachment(Attachment* attachment)
{
/**************************************
 *
 *	r e l e a s e _ a t t a c h m e n t
 *
 **************************************
 *
 * Functional description
 *	Disconnect attachment block from database block.
 *	NOTE:  This routine assumes that upon entry,
 *	mutex DBB_MUTX_init_fini will be locked.
 *	Before exiting, there is a handoff from this
 *	mutex to the databases_mutex mutex.  Upon exit,
 *	that mutex remains locked.  It is the
 *	responsibility of the caller to unlock it.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();
	Database*  dbb  = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	if (!attachment) {
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
		V4_JRD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
		V4_JRD_MUTEX_LOCK(databases_mutex);
#endif
		return;
	}

	if (attachment->att_event_session)
		EVENT_delete_session(attachment->att_event_session);

	if (attachment->att_id_lock)
		LCK_release(tdbb, attachment->att_id_lock);

	for (vcl** vector = attachment->att_counts;
		 vector < attachment->att_counts + DBB_max_count; ++vector)
	{
		if (*vector)
			delete *vector;
	}

/* Release any validation error vector allocated */

	if (attachment->att_val_errors) {
		delete attachment->att_val_errors;
		attachment->att_val_errors = NULL;
	}

/* Release the persistent locks taken out during the attachment */
	vec* lock_vector = attachment->att_relation_locks;
	if (lock_vector)
	{
		size_t i = 0;
		for (vec::iterator lock = lock_vector->begin();
			 i < lock_vector->count(); i++, lock++)
		{
			if (*lock)
			{
				LCK_release(tdbb, (Lock*)(*lock));
				delete *lock;
			}
		}
		delete lock_vector;
	}

    Lock* record_lock;
	for (record_lock = attachment->att_record_locks; record_lock;
		 record_lock = record_lock->lck_att_next)
	{
		LCK_release(tdbb, record_lock);
	}

/* bug #7781, need to null out the attachment pointer of all locks which
   were hung off this attachment block, to ensure that the attachment
   block doesn't get dereferenced after it is released */

	// Disable delivery of ASTs for the moment while queue of locks is in flux
	LCK_ast_inhibit();
	record_lock = attachment->att_long_locks;
	while (record_lock) {
		Lock* next = record_lock->lck_next;
		record_lock->lck_attachment = NULL;
		record_lock->lck_next = NULL;
		record_lock->lck_prior = NULL;
		record_lock = next;
	}
	LCK_ast_enable();

	if (attachment->att_flags & ATT_lck_init_done)
		LCK_fini(tdbb, LCK_OWNER_attachment);	/* For the attachment */

	if (attachment->att_compatibility_table)
		delete attachment->att_compatibility_table;

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
	V4_JRD_MUTEX_UNLOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
	V4_JRD_MUTEX_LOCK(databases_mutex);
#endif

	if (MemoryPool::blk_type(dbb) != type_dbb)
		return;

/* remove the attachment block from the dbb linked list */

	for (Attachment** ptr = &dbb->dbb_attachments; *ptr; ptr = &(*ptr)->att_next) {
		if (*ptr == attachment) {
			*ptr = attachment->att_next;
			break;
		}
	}
}


static ISC_STATUS return_success(thread_db* tdbb)
{
/**************************************
 *
 *	r e t u r n _ s u c c e s s
 *
 **************************************
 *
 * Functional description
 *	Set up status vector to reflect successful execution.
 *
 **************************************/
	SET_TDBB(tdbb);

/* Decrement count of active threads in database */

	Database* dbb = tdbb->tdbb_database;
	if (dbb)
		--dbb->dbb_use_count;

	ISC_STATUS* const user_status = tdbb->tdbb_status_vector;
	ISC_STATUS* p = user_status;

/* If the status vector has not been initialized, then
   initilalize the status vector to indicate success.
   Else pass the status vector along at it stands.  */
	if (p[0] != isc_arg_gds ||
		p[1] != FB_SUCCESS ||
		(p[2] != isc_arg_end && p[2] != isc_arg_gds &&
		 p[2] != isc_arg_warning))
	{
		*p++ = isc_arg_gds;
		*p++ = FB_SUCCESS;
		*p = isc_arg_end;
	}

	JRD_restore_context();

/* This is debugging code which is meant to verify that
   the database use count is cleared on exit from the
   engine. Database shutdown cannot succeed if the database
   use count is erroneously left set. */

#if (defined DEV_BUILD && !defined MULTI_THREAD)
	if (dbb && dbb->dbb_use_count && !(dbb->dbb_flags & DBB_security_db)) {
		dbb->dbb_use_count = 0;
		p = user_status;
		*p++ = isc_arg_gds;
		*p++ = isc_random;
		*p++ = isc_arg_string;
		*p++ = (ISC_STATUS) "database use count set on success return";
		*p = isc_arg_end;
	}
#endif

	return user_status[1];
}


static bool rollback(thread_db*	tdbb,
						jrd_tra*		next,
						ISC_STATUS*	status_vector,
						const bool	retaining_flag)
{
/**************************************
 *
 *	r o l l b a c k
 *
 **************************************
 *
 * Functional description
 *	Abort a transaction.
 *
 **************************************/
	jrd_tra* transaction;
	ISC_STATUS_ARRAY local_status;

	SET_TDBB(tdbb);

	while ( (transaction = next) )
	{
		next = transaction->tra_sibling;
		check_database(tdbb, transaction->tra_attachment, status_vector);

		try {
		tdbb->tdbb_status_vector = status_vector;
		TRA_rollback(tdbb, transaction, retaining_flag);
		Database* dbb = tdbb->tdbb_database;
		--dbb->dbb_use_count;

		}	// try
		catch (const std::exception& ex) {
			Firebird::stuff_exception(status_vector, ex);
			status_vector = local_status;
			Database* dbb = tdbb->tdbb_database;
			--dbb->dbb_use_count;
			continue;
		}
	}

	return (status_vector == local_status);
}


#if defined (WIN_NT) && !defined(SERVER_SHUTDOWN)
static void setup_NT_handlers()
{
/**************************************
 *
 *      s e t u p _ N T _ h a n d l e r s
 *
 **************************************
 *
 * Functional description
 *      Set up Windows NT console control handlers for
 *      things that can happen.   The handler used for
 *      all cases, handler_NT(), will flush and close
 *      any open database files.
 *
 **************************************/

	SetConsoleCtrlHandler((PHANDLER_ROUTINE) handler_NT, TRUE);
}
#endif


static void shutdown_database(Database* dbb, const bool release_pools)
{
/**************************************
 *
 *	s h u t d o w n _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Shutdown physical database environment.
 *	NOTE:  This routine assumes that upon entry,
 *	mutex databases_mutex will be locked.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

/* Shutdown file and/or remote connection */

#ifdef SUPERSERVER_V2
	TRA_header_write(tdbb, dbb, 0L);	/* Update transaction info on header page. */
#endif

#ifdef GARBAGE_THREAD
	VIO_fini(tdbb);
#endif
	CMP_fini(tdbb);
	CCH_fini(tdbb);
	delete dbb->backup_manager;
	FUN_fini(tdbb);

	if (dbb->dbb_shadow_lock)
		LCK_release(tdbb, dbb->dbb_shadow_lock);

	if (dbb->dbb_retaining_lock)
		LCK_release(tdbb, dbb->dbb_retaining_lock);

	if (dbb->dbb_lock)
		LCK_release(tdbb, dbb->dbb_lock);

#ifdef REPLAY_OSRI_API_CALLS_SUBSYSTEM
	if (dbb->dbb_log)
		LOG_fini();
#endif

/* Shut down any extern relations */

	if (dbb->dbb_relations)
	{
		vec* vector = dbb->dbb_relations;
		vec::iterator ptr = vector->begin(), end = vector->end();

		for (; ptr < end; ++ptr)
		{
			if (*ptr && ((jrd_rel*)(*ptr))->rel_file)
			{
				EXT_fini((jrd_rel*)(*ptr));
			}
		}
	}

	Database** d_ptr;	// Intentionally left outside loop (HP/UX compiler)
	for (d_ptr = &databases; *(d_ptr); d_ptr = &(*d_ptr)->dbb_next) {
		if (*d_ptr == dbb) {
			*d_ptr = dbb->dbb_next;
			break;
		}
	}

	if (dbb->dbb_flags & DBB_lck_init_done) {
		LCK_fini(tdbb, LCK_OWNER_database);	/* For the database */
		dbb->dbb_flags &= ~DBB_lck_init_done;
	}

/* Remove objects from the in-use strutures before destroying them */

	USHORT i;
	for (i = 0; i < DBB_MUTX_max; i++) {
		INUSE_remove(&tdbb->tdbb_mutexes, dbb->dbb_mutexes + i, true);
	}
	for (i = 0; i < DBB_WLCK_max; i++) {
		INUSE_remove(&tdbb->tdbb_rw_locks, dbb->dbb_rw_locks + i, true);
	}

	delete[] dbb->dbb_mutexes;
	delete[] dbb->dbb_rw_locks;
#ifdef SUPERSERVER
	if (dbb->dbb_flags & DBB_sp_rec_mutex_init) {
		THD_rec_mutex_destroy(&dbb->dbb_sp_rec_mutex);
	}
#endif
	if (release_pools) {
		Database::deleteDbb(dbb);
		tdbb->tdbb_database = NULL;
	}

	SecurityDatabase::shutdown();
}


static void strip_quotes(Firebird::string& out)
{
/**************************************
 *
 *	s t r i p _ q u o t e s
 *
 **************************************
 *
 * Functional description
 *	Get rid of quotes around strings
 *
 **************************************/
	if (out.isEmpty()) 
	{
		return;
	}

	char quote = 0;
	if (out[0] == DBL_QUOTE || out[0] == SINGLE_QUOTE)
	{
// Skip any initial quote
		quote = out[0];
		out.erase(0, 1);
// Search for same quote
		size_t pos = out.find(quote);
		if (pos != Firebird::string::npos)
		{
			out.erase(pos);
		}
	}
}


void JRD_set_cache_default(ULONG* num_ptr)
{
/**************************************
 *
 *	J R D _ s e t _ c a c h e _ d e f a u l t
 *
 **************************************
 *
 * Functional description
 *	Set the number of cache pages to use for each
 *	database, but don't go less than MIN_PAGE_BUFFERS and
 *      more than MAX_PAGE_BUFFERS.
 *	Currently MIN_PAGE_BUFFERS = 50L,
 *		  MAX_PAGE_BUFFERS = 65535L.
 *
 **************************************/

	if (*num_ptr < MIN_PAGE_BUFFERS)
		*num_ptr = MIN_PAGE_BUFFERS;
	if (*num_ptr > MAX_PAGE_BUFFERS)
		*num_ptr = MAX_PAGE_BUFFERS;
	JRD_cache_default = *num_ptr;
}


#ifdef SERVER_SHUTDOWN
TEXT* JRD_num_attachments(TEXT* const buf, USHORT buf_len, USHORT flag,
						USHORT* atts, USHORT* dbs)
{
/**************************************
 *
 *	J R D _ n u m _ a t t a c h m e n t s
 *
 **************************************
 *
 * Functional description
 *	Count the number of active databases and
 *	attachments.  If flag is set then put
 *	what it says into buf, if it fits. If it does not fit
 *	then allocate local buffer, put info into there, and
 *	return pointer to caller (in this case a caller must
 *	release memory allocated for local buffer).
 *
 **************************************/
	USHORT num_dbs = 0;
	USHORT num_att = 0;
	USHORT total = 0;
	ULONG drive_mask = 0L;
	db_file* dbf = NULL;
	db_file* dbfp = NULL;

/* protect against NULL value for buf */

	TEXT* lbuf = buf;
	if (!lbuf)
		buf_len = 0;

#ifdef WIN_NT
/* Check that the buffer is big enough for the requested
 * information.  If not, unset the flag
 */

	if (flag == JRD_info_drivemask) {
		if (buf_len < sizeof(ULONG)) {
		    lbuf = (TEXT*) gds__alloc((SLONG) (sizeof(ULONG)));
			if (!lbuf)
				flag = 0;
		}
	}
#endif

	THREAD_ENTER();

/* Zip through the list of databases and count the number of local
 * connections.  If buf is not NULL then copy all the database names
 * that will fit into it. */

	for (Database* dbb = databases; dbb; dbb = dbb->dbb_next) {
#ifdef WIN_NT
		/* Get drive letters for db files */

		if (flag == JRD_info_drivemask)
		{
			for (jrd_file* files = dbb->dbb_file; files; files = files->fil_next)
				ExtractDriveLetter(files->fil_string, &drive_mask);
		}
#endif

		if (!(dbb->dbb_flags & (DBB_bugcheck | DBB_not_in_use | DBB_security_db)) &&
			!(dbb->dbb_ast_flags & DBB_shutdown
			  && dbb->dbb_ast_flags & DBB_shutdown_locks))
		{
			num_dbs++;
			if (flag == JRD_info_dbnames) {
				if (dbfp == NULL) {
					dbfp = (db_file*) gds__alloc((SLONG) (sizeof(db_file) +
													 sizeof(TEXT) *
													 dbb->dbb_filename.length()));
					if (!dbfp)
						throw std::bad_alloc();
					dbf = dbfp;
				}
				else {
					dbfp->dbf_next = (db_file*)
						gds__alloc((SLONG)
								   (sizeof(db_file) +
									sizeof(TEXT) *
									dbb->dbb_filename.length()));
					if (!dbfp)
						throw std::bad_alloc();
					dbfp = dbfp->dbf_next;
				}
				if (dbfp) {
					dbfp->dbf_length = dbb->dbb_filename.length();
					dbfp->dbf_next = NULL;
					MOVE_FAST(dbb->dbb_filename.c_str(), dbfp->dbf_data,
							  dbfp->dbf_length);
					total += sizeof(USHORT) + dbfp->dbf_length;
				}
				else
					flag = 0;
			}

			for (const Attachment* attach = dbb->dbb_attachments; attach;
				attach = attach->att_next)
			{
				num_att++;

#ifdef WIN_NT
				/* Get drive letters for temp directories */

				if (flag == JRD_info_drivemask) {
					mutexed_dir_list* ptr = DLS_get_access();
					for (dir_list* dirs = ptr->mdls_dls; dirs; dirs = dirs->dls_next) {
						ExtractDriveLetter(dirs->dls_directory, &drive_mask);
					}
				}

				/* Get drive letters for sort files */

				if (flag == JRD_info_drivemask)
				{
					for (const sort_context* scb = attach->att_active_sorts; scb; 
						scb = scb->scb_next)
					{
						for (const sort_work_file* sfb = scb->scb_sfb; sfb;
							sfb = sfb->sfb_next)
						{
							ExtractDriveLetter(sfb->sfb_file_name, &drive_mask);
						}
					}
				}
#endif
			}
		}
	}

	THREAD_EXIT();

	*atts = num_att;
	*dbs = num_dbs;

	if (dbf)
	{
		if (flag == JRD_info_dbnames)
		{
			if (buf_len < (sizeof(USHORT) + total))
			{
				lbuf = (TEXT *) gds__alloc((SLONG) (sizeof(USHORT) + total));
			}
			TEXT* lbufp = lbuf;
			if (lbufp)
			{
				/*  Put db info into buffer. Format is as follows:

				   number of dbases sizeof (USHORT)
				   1st db name length   sizeof (USHORT)
				   1st db name      sizeof (TEXT) * length
				   2nd db name length
				   2nd db name
				   ...
				   last db name length
				   last db name
				 */

				lbufp += sizeof(USHORT);
				total = 0;
				for (dbfp = dbf; dbfp; dbfp = dbfp->dbf_next) {
					*lbufp++ = (TEXT) dbfp->dbf_length;
					*lbufp++ = dbfp->dbf_length >> 8;
					MOVE_FAST(dbfp->dbf_data, lbufp, dbfp->dbf_length);
					lbufp += dbfp->dbf_length;
					total++;
				}
				fb_assert(total == num_dbs);
				lbufp = lbuf;
				*lbufp++ = (TEXT) total;
				*lbufp++ = total >> 8;
			}
		}

		for (dbfp = dbf; dbfp;) {
			db_file* x = dbfp->dbf_next;
			gds__free(dbfp);
			dbfp = x;
		}
	}

#ifdef WIN_NT
	if (flag == JRD_info_drivemask)
		*(ULONG *) lbuf = drive_mask;
#endif

// CVC: Apparently, the original condition will leak memory, because flag
// may be JRD_info_drivemask and memory could be allocated for that purpose,
// as few as sizeof(ULONG), but a leak is a leak! I added the ifdef below.
	if (num_dbs == 0)
	{
#ifdef WIN_NT
		if (flag == JRD_info_drivemask && lbuf != buf)
		    gds__free(lbuf);
#endif
		lbuf = NULL;
	}

	return lbuf;
}


#ifdef WIN_NT
static void ExtractDriveLetter(const TEXT* file_name, ULONG* drive_mask)
{
/**************************************
 *
 *	E x t r a c t D r i v e L e t t e r
 *
 **************************************
 *
 * Functional description
 *	Determine the drive letter of file_name
 *	and set the proper bit in the bit mask.
 *		bit 0 = drive A
 *		bit 1 = drive B and so on...
 *	This function is used to determine drive
 *	usage for use with Plug and Play for
 *	MS Windows 4.0.
 *
 **************************************/
	ULONG mask = 1;

	const SHORT shift = (*file_name - 'A');
	mask <<= shift;
	*drive_mask |= mask;
}
#endif


ULONG JRD_shutdown_all()
{
/**************************************
 *
 *	J R D _ s h u t d o w n _ a l l
 *
 **************************************
 *
 * Functional description
 *	rollback every transaction,
 *	release every attachment,
 *	and shutdown every database.
 *
 **************************************/
	thread_db thd_context;
	thread_db* tdbb = JRD_MAIN_set_thread_data(thd_context);

	if (initialized) {
		JRD_SS_MUTEX_LOCK;
	}
	ISC_STATUS_ARRAY user_status;

	Database* dbb_next;
	for (Database* dbb = databases; dbb; dbb = dbb_next)
	{
		dbb_next = dbb->dbb_next;
		if (!(dbb->dbb_flags & (DBB_bugcheck | DBB_not_in_use | DBB_security_db)) &&
			!(dbb->dbb_ast_flags & DBB_shutdown &&
			  dbb->dbb_ast_flags & DBB_shutdown_locks))
		{
			Attachment* att_next;
			for (Attachment* attach = dbb->dbb_attachments; attach; attach = att_next)
			{
				att_next = attach->att_next;
				tdbb->tdbb_database = dbb;
				tdbb->tdbb_attachment = attach;
				tdbb->tdbb_request = NULL;
				tdbb->tdbb_transaction = NULL;
				Jrd::ContextPoolHolder context(tdbb, 0);

				++dbb->dbb_use_count;

				/* purge_attachment below can do an ERR_post */

				tdbb->tdbb_status_vector = user_status;

				try {
					/* purge_attachment, rollback any open transactions */

#if defined(V4_THREADING) && !defined(SUPERSERVER) 
					V4_JRD_MUTEX_LOCK(dbb->dbb_mutexes + DBB_MUTX_init_fini);
#endif
					purge_attachment(tdbb, user_status, attach, true);
#if defined(V4_THREADING) && !defined(SUPERSERVER) 
					V4_JRD_MUTEX_UNLOCK(databases_mutex);
#endif
				}	// try
				catch (const std::exception& ex) {
					if (initialized) {
						JRD_SS_MUTEX_UNLOCK;
					}
					return error(user_status, ex);
				}
			}
		}
	}

	if (initialized) {
		JRD_SS_MUTEX_UNLOCK;
    }

	JRD_restore_context();

	return FB_SUCCESS;
}
#endif /* SERVER_SHUTDOWN */


static void purge_attachment(thread_db*		tdbb,
							 ISC_STATUS*	user_status,
							 Attachment*		attachment,
							 const bool	force_flag)
{
/**************************************
 *
 *	p u r g e _ a t t a c h m e n t
 *
 **************************************
 *
 * Functional description
 *	Zap an attachment, shutting down the database
 *	if it is the last one.
 *	NOTE:  This routine assumes that upon entry,
 *	mutex databases_mutex will be locked.
 *
 **************************************/
	SET_TDBB(tdbb);
	Database* dbb = attachment->att_database;

	if (!(dbb->dbb_flags & DBB_bugcheck)) {
		/* Check for any pending transactions */

		int count = 0;
		jrd_tra* next;
		for (jrd_tra* transaction = attachment->att_transactions;
			 transaction;
			 transaction = next)
		{
			next = transaction->tra_next;
			if (transaction != attachment->att_dbkey_trans)
			{
				if (transaction->tra_flags & TRA_prepared ||
					dbb->dbb_ast_flags & DBB_shutdown ||
					attachment->att_flags & ATT_shutdown)
				{
					TRA_release_transaction(tdbb, transaction);
				}
				else if (force_flag)
					TRA_rollback(tdbb, transaction, false);
				else
					++count;
			}
		}

		if (count)
			ERR_post(isc_open_trans, isc_arg_number, (SLONG) count, 0);

		/* If there's a side transaction for db-key scope, get rid of it */

		jrd_tra* trans_dbk = attachment->att_dbkey_trans;
		if (trans_dbk)
		{
			attachment->att_dbkey_trans = NULL;
			if (dbb->dbb_ast_flags & DBB_shutdown ||
				attachment->att_flags & ATT_shutdown)
			{
				TRA_release_transaction(tdbb, trans_dbk);
			}
			else
			{
				TRA_commit(tdbb, trans_dbk, false);
			}
		}

		SORT_shutdown(attachment);
	}

/* Unlink attachment from database */

	release_attachment(attachment);

/* At this point, mutex dbb->dbb_mutexes [DBB_MUTX_init_fini] has been
   unlocked and mutex databases_mutex has been locked. */

/* If there are still attachments, do a partial shutdown */

	if (MemoryPool::blk_type(dbb) == type_dbb)
	{
		if (dbb->dbb_attachments || (dbb->dbb_flags & DBB_being_opened))
		{
			// There are still attachments so do a partial shutdown

            // Both CMP_release() and SCL_release() do advance the pointer
			// before the deallocation.
			jrd_req* request;
			while ( (request = attachment->att_requests) ) {
				CMP_release(tdbb, request);
			}
			
			SecurityClass* sec_class;
			while ( (sec_class = attachment->att_security_classes) ) {
				SCL_release(sec_class);
			}
			
			UserId* user = attachment->att_user;
			if (user) {
				delete user;
			}
			
			Bookmark* bookmark;
			while ( (bookmark = attachment->att_bookmarks) ) {
				attachment->att_bookmarks = bookmark->bkm_next;
				delete bookmark;
			}
			
			if (attachment->att_bkm_quick_ref) {
				delete attachment->att_bkm_quick_ref;
			}
			if (attachment->att_lck_quick_ref) {
				delete attachment->att_lck_quick_ref;
			}

			delete attachment;
		}
		else
		{
			shutdown_database(dbb, true);
		}
	}
}


// verify_request_synchronization
//
// @brief Finds the sub-requests at the given level and replaces with it the
// original passed request (note the pointer by reference). If that specific
// sub-request is not found, throw the dreaded "request synchronization error".
// Notice that at this time, the calling function's "request" pointer has been
// set to null, so remember that if you write a debugging routine.
// This function replaced a chunk of code repeated four times.
//
// @param request The incoming, parent request to be replaced.
// @param level The level of the sub-request we need to find.
static void verify_request_synchronization(jrd_req*& request, SSHORT level)
{
	const USHORT lev = level;
	if (lev) {
		const vec* vector = request->req_sub_requests;
		if (!vector || lev >= vector->count() ||
			!(request = static_cast<jrd_req*>((*vector)[lev])))
		{
			ERR_post(isc_req_sync, 0);
		}
	}
}


/**
  
 	verify_database_name
  
    @brief	Verify database name for open/create 
	against given in conf file list of available directories

    @param name
    @param status

 **/
static vdnResult verify_database_name(const Firebird::PathName& name, ISC_STATUS* status)
{
	// Check for security.fdb
	static TEXT SecurityNameBuffer[MAXPATHLEN] = "";
	static Firebird::PathName ExpandedSecurityNameBuffer(*getDefaultMemoryPool());
	if (! SecurityNameBuffer[0]) {
		SecurityDatabase::getPath(SecurityNameBuffer);
		ExpandedSecurityNameBuffer = SecurityNameBuffer;
		ISC_expand_filename(ExpandedSecurityNameBuffer, false);
	}
	if (name == SecurityNameBuffer || name == ExpandedSecurityNameBuffer)
		return vdnSecurity;
	
	// Check for .conf
	if (!ISC_verify_database_access(name)) {
		status[0] = isc_arg_gds;
		status[1] = isc_conf_access_denied;
		status[2] = isc_arg_string;
		// CVC: Using STATUS to hold pointer to literal string!
		status[3] = reinterpret_cast<ISC_STATUS>("database");
		status[4] = isc_arg_string;
		status[5] = (ISC_STATUS)(U_IPTR) ERR_cstring(name.c_str());
		status[6] = isc_arg_end;
		return vdnFail;
	}
	return vdnOk;
}

