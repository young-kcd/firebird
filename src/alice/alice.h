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

#include <stdio.h>

#include "../jrd/ibase.h"
#include "../jrd/thd.h"
#include "../alice/all.h"
#include "../include/fb_blk.h"
#include "../common/classes/alloc.h"
#include "../common/classes/array.h"

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

enum alice_shut_mode {
	SHUT_DEFAULT = 0,
	SHUT_NORMAL = 1,
	SHUT_MULTI = 2,
	SHUT_SINGLE = 3,
	SHUT_FULL = 4
};

struct user_action
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
	alice_shut_mode ua_shutdown_mode;
};




//  String block: used to store a string of constant length. 

class alice_str : public pool_alloc_rpt<UCHAR, alice_type_str>
{
public:
	USHORT str_length;
	UCHAR str_data[2];
};

//  Transaction block: used to store info about a multidatabase transaction. 

struct tdr : public pool_alloc<alice_type_tdr>
{
	tdr* tdr_next;				// next subtransaction 
	SLONG tdr_id;				// database-specific transaction id 
	alice_str* tdr_fullpath;			// full (possibly) remote pathname 
	const TEXT* tdr_filename;	// filename within full pathname 
	alice_str* tdr_host_site;			// host for transaction 
	alice_str* tdr_remote_site;		// site for remote transaction 
	FB_API_HANDLE tdr_handle;			// reconnected transaction handle 
	FB_API_HANDLE tdr_db_handle;		// reattached database handle 
	USHORT tdr_db_caps;			// capabilities of database 
	USHORT tdr_state;			// see flags below 
};

typedef tdr* TDR;
// Transaction Description Record 

const int TDR_VERSION		= 1;
enum tdr_vals {
	TDR_HOST_SITE		= 1,
	TDR_DATABASE_PATH	= 2,
	TDR_TRANSACTION_ID	= 3,
	TDR_REMOTE_SITE		= 4,
	TDR_PROTOCOL		= 5
};

// flags for tdr_db_caps 

enum tdr_db_caps_vals {
	CAP_none			= 0,
	CAP_transactions	= 1
};
// db has a RDB$TRANSACTIONS relation 

// flags for tdr_state 
enum tdr_state_vals {
	TRA_none		= 0,		// transaction description record is missing 
	TRA_limbo		= 1,		// has been prepared 
	TRA_commit		= 2,		// has committed 
	TRA_rollback	= 3,		// has rolled back 
	TRA_unknown		= 4 		// database couldn't be reattached, state is unknown 
};


// Global switches and data 

#include "../jrd/svc.h"

enum redirect_vals {
	NOREDIRECT = 0,
	REDIRECT = 1,
	NOOUTPUT = 2
};


#ifndef SUPERSERVER
class AliceGlobals;
extern AliceGlobals* gdgbl;
#endif

class AliceGlobals : public ThreadData
{
private:
	AliceMemoryPool* ALICE_default_pool;
	friend class Firebird::SubsystemContextPoolHolder <AliceGlobals, AliceMemoryPool>;

	void setDefaultPool(AliceMemoryPool* p)
	{
		ALICE_default_pool = p;
	}

public:
	AliceGlobals(Jrd::pfn_svc_output outProc, Jrd::Service* outData) 
		: ThreadData(ThreadData::tddALICE), 
		ALICE_default_pool(0),
		output_proc(outProc), 
		output_data(outData)
	{
	}

	AliceMemoryPool* getDefaultPool()
	{
		return ALICE_default_pool;
	}
	
	user_action		ALICE_data;
	ISC_STATUS_ARRAY	status_vector;
	int				exit_code;
	Jrd::pfn_svc_output  output_proc;
	Jrd::Service*	output_data;
	FILE*		output_file;
	Jrd::Service*	service_blk;
	isc_db_handle	db_handle;
	isc_tr_handle	tr_handle;
	ISC_STATUS*		status;
	redirect_vals	sw_redirect;
	bool			sw_service;
	bool			sw_service_thd;

#ifdef SUPERSERVER
	static inline AliceGlobals* getSpecific() {
		ThreadData* tData = ThreadData::getSpecific();
		fb_assert (tData->getType() == ThreadData::tddALICE)
		return (AliceGlobals*) tData;
	}
	static inline void putSpecific(AliceGlobals* tdgbl) {
		tdgbl->ThreadData::putSpecific();
	}
	static inline void restoreSpecific() {
		ThreadData::restoreSpecific();
	}
#else
	static inline AliceGlobals* getSpecific() {
		return gdgbl;
	}
	static inline void putSpecific(AliceGlobals* tdgbl) {
		gdgbl = tdgbl;
	}
	static inline void restoreSpecific() {
	}
#endif
};

typedef Firebird::SubsystemContextPoolHolder <AliceGlobals, AliceMemoryPool> 
	AliceContextPoolHolder;

#endif	// ALICE_ALICE_H

