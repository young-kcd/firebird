/*
 *	PROGRAM:	Alice
 *	MODULE:		alice.h
 *	DESCRIPTION:	Block definitions
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

#ifndef ALICE_ALICE_H
#define ALICE_ALICE_H

#include "../jrd/ib_stdio.h"

#include "../jrd/y_ref.h"
#include "../jrd/ibase.h"
#include "../jrd/ibsetjmp.h"
#include "../jrd/thd.h"
#include "../alice/all.h"
#include "../include/fb_blk.h"
#include "../common/classes/alloc.h"

#include <vector>

#include "../alice/blk.h"

enum val_errors {
	VAL_INVALID_DB_VERSION	= 0,
	VAL_RECORD_ERRORS		= 1,
	VAL_BLOB_PAGE_ERRORS	= 2,
	VAL_DATA_PAGE_ERRORS	= 3,
	VAL_INDEX_PAGE_ERRORS	= 4,
	VAL_POINTER_PAGE_ERRORS	= 5,
	VAL_TIP_PAGE_ERRORS		= 6,
	VAL_PAGE_ERRORS			= 7,
	MAX_VAL_ERRORS			= 8
};

typedef struct user_action
{
	ULONG ua_switches;
	UCHAR* ua_user;
	UCHAR* ua_password;
	bool ua_use;
	bool ua_force;
	bool ua_read_only;
	SLONG ua_shutdown_delay;
	SLONG ua_sweep_interval;
	SLONG ua_transaction;
	SLONG ua_page_buffers;
	USHORT ua_debug;
	SLONG ua_val_errors[MAX_VAL_ERRORS];
	TEXT ua_log_file[MAXPATHLEN];
	USHORT ua_db_SQL_dialect;
} *USER_ACTION;




/*  String block: used to store a string of constant length. */

class str : public pool_alloc_rpt<UCHAR, alice_type_str>
{
public:
	USHORT str_length;
	UCHAR str_data[2];
};
typedef str *STR;

/*  Transaction block: used to store info about a multidatabase transaction. */

typedef struct tdr : public pool_alloc<alice_type_tdr>
{
	tdr* tdr_next;				/* next subtransaction */
	SLONG tdr_id;				/* database-specific transaction id */
	str* tdr_fullpath;			/* full (possibly) remote pathname */
	const TEXT* tdr_filename;	/* filename within full pathname */
	str* tdr_host_site;			/* host for transaction */
	str* tdr_remote_site;		/* site for remote transaction */
	FRBRD* tdr_handle;			/* reconnected transaction handle */
	FRBRD* tdr_db_handle;		/* reattached database handle */
	USHORT tdr_db_caps;			/* capabilities of database */
	USHORT tdr_state;			/* see flags below */
} *TDR;

/* Transaction Description Record */

const int TDR_VERSION		= 1;
enum tdr_vals {
	TDR_HOST_SITE		= 1,
	TDR_DATABASE_PATH	= 2,
	TDR_TRANSACTION_ID	= 3,
	TDR_REMOTE_SITE		= 4,
	TDR_PROTOCOL		= 5
};

/* flags for tdr_db_caps */

enum tdr_db_caps_vals {
	CAP_none			= 0,
	CAP_transactions	= 1
};
/* db has a RDB$TRANSACTIONS relation */

/* flags for tdr_state */
enum tdr_state_vals {
	TRA_none		= 0,		/* transaction description record is missing */
	TRA_limbo		= 1,		/* has been prepared */
	TRA_commit		= 2,		/* has committed */
	TRA_rollback	= 3,		/* has rolled back */
	TRA_unknown		= 4 		/* database couldn't be reattached, state is unknown */
};


/* a couple of obscure blocks used only in data allocator routines */

class vec : public pool_alloc_rpt<class blk*, alice_type_vec>
{
public:
	ULONG vec_count;
	blk *vec_object[1];
};
typedef vec* VEC;

class vcl : public pool_alloc_rpt<SLONG, alice_type_vcl>
{
	ULONG vcl_count;
	SLONG vcl_long[1];
};
typedef vcl* VCL;

/* Global switches and data */

#include "../jrd/svc.h"

enum redirect_vals {
	NOREDIRECT = 0,
	REDIRECT = 1,
	NOOUTPUT = 2
};

class tgbl
{
public:
	tgbl(AliceMemoryPool* p) : pools(0, (AliceMemoryPool*)0,
				pool_vec_t::allocator_type(*p)) {}
	
	thdd			tgbl_thd_data;
	user_action		ALICE_data;
	AliceMemoryPool* ALICE_permanent_pool;
	AliceMemoryPool* ALICE_default_pool;
	ISC_STATUS_ARRAY	status_vector;
	typedef			std::vector<AliceMemoryPool*, Firebird::allocator<AliceMemoryPool*> > pool_vec_t;
	pool_vec_t		pools;
	int				exit_code;
	int(*output_proc)(SLONG, UCHAR*);
	SLONG			output_data;
	IB_FILE*		output_file;
	SVC				service_blk;
	isc_db_handle	db_handle;
	isc_tr_handle	tr_handle;
	ISC_STATUS*		status;
	redirect_vals	sw_redirect;
	bool			sw_service;
	bool			sw_service_thd;
};
typedef tgbl* TGBL;

#ifdef GET_THREAD_DATA
#undef GET_THREAD_DATA
#endif

#ifdef SUPERSERVER
#define GET_THREAD_DATA		((TGBL) THD_get_specific())
#define SET_THREAD_DATA		THD_put_specific ((THDD) tdgbl);	\
				tdgbl->tgbl_thd_data.thdd_type =				\
					THDD_TYPE_TALICE
#define RESTORE_THREAD_DATA	THD_restore_specific();
#else
extern tgbl *gdgbl;

#define GET_THREAD_DATA		(gdgbl)
#define SET_THREAD_DATA		gdgbl = const_cast<tgbl*>(tdgbl); \
				tdgbl->tgbl_thd_data.thdd_type = THDD_TYPE_TGBL
#define RESTORE_THREAD_DATA
#endif

#endif	// ALICE_ALICE_H

