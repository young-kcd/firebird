/*
 *	PROGRAM:	JRD access method
 *	MODULE:		svc.h
 *	DESCRIPTION:	Service manager declarations
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

#ifndef JRD_SVC_H
#define JRD_SVC_H

#include <stdio.h>

#include "fb_blk.h"

#include "../jrd/jrd_pwd.h"
#include "../jrd/isc.h"
#include "../jrd/svc_undoc.h"
#include "../jrd/ThreadStart.h"

#include "../common/classes/semaphore.h"
#include "../common/classes/array.h"
#include "../common/classes/SafeArg.h"
#include "../common/UtilSvc.h"

// forward decl.
struct serv_entry; 
namespace Firebird {
	class ClumpletReader;
	class StringsBuffer;
	namespace Arg {
		class StatusVector;
	}
}

namespace Jrd {

const ULONG SERVICE_VERSION			= 2;

const int SVC_STDOUT_BUFFER_SIZE	= 1024;

/* Flag of capabilities supported by the server */
//const ULONG WAL_SUPPORT					= 0x1L;	// Write Ahead Log
const ULONG MULTI_CLIENT_SUPPORT		= 0x2L;	/* SuperServer model (vs. multi-inet) */
const ULONG REMOTE_HOP_SUPPORT			= 0x4L;	/* Server can connect to other server */
//const ULONG NO_SVR_STATS_SUPPORT		= 0x8L;	// Does not support statistics

//const ULONG NO_DB_STATS_SUPPORT			= 0x10L;	// Does not support statistics
// Really the 16 bit LIBS here?
//const ULONG LOCAL_ENGINE_SUPPORT		= 0x20L;	// The local 16 bit engine
//const ULONG NO_FORCED_WRITE_SUPPORT		= 0x40L;	// Can not configure sync writes 
//const ULONG NO_SHUTDOWN_SUPPORT			= 0x80L;	// Can not shutdown/restart databases 
const ULONG NO_SERVER_SHUTDOWN_SUPPORT	= 0x100L;	/* Can not shutdown server */
const ULONG SERVER_CONFIG_SUPPORT		= 0x200L;	/* Can configure server */
const ULONG QUOTED_FILENAME_SUPPORT		= 0x400L;	/* Can pass quoted filenames in */

/* Range definitions for service actions.  Any action outside of
   this range is not supported */
const USHORT isc_action_min				= 1;
const USHORT isc_action_max				= 14;

/* Range definitions for service actions.  Any action outside of
   this range is not supported */
//define isc_info_min                  50
//define isc_info_max                  67

/* Bitmask values for the svc_flags variable */
const int SVC_shutdown		= 0x1;
const int SVC_timeout		= 0x2;
//const int SVC_forked		= 0x4;
const int SVC_detached		= 0x8;
const int SVC_finished		= 0x10;
const int SVC_thd_running	= 0x20;
const int SVC_evnt_fired	= 0x40;
const int SVC_cmd_line		= 0x80;

// forward decl.
class thread_db;

// Service manager
class Service : public Firebird::UtilSvc, public TypedHandle<type_svc>
{
public:		// utilities interface with service
	// printf() to svc_stdout
    virtual void printf(const SCHAR* format, ...);
	// returns true - it's service :)
	virtual bool isService();
	// client thread started
	virtual void started();
	// client thread finished
	virtual void finish();
	// put various info items in info buffer
    virtual void putLine(char tag, const char* val);
    virtual void putSLong(char tag, SLONG val);
	virtual void putChar(char tag, char val);
	// append status_vector to service's status
	virtual void stuffStatus(const ISC_STATUS* status_vector);
	// append error message to service's status
	virtual void stuffStatus(const USHORT facility, const USHORT errcode, const MsgFormat::SafeArg& args);
	// no-op for services
	virtual void hidePasswd(ArgvType&, int);
	// return service status
    virtual ISC_STATUS* getStatus();
	// no-op for services
	virtual void checkService();
	// add address path (taken from spb) to dpb if present
	virtual void getAddressPath(Firebird::ClumpletWriter& dpb);
	// dup strings in service's circular buffer
	virtual void makePermanentVector(ISC_STATUS* s);

public:		// external interface with service
	// Attach - service ctor
	Service(const TEXT* service_name, USHORT spb_length, const UCHAR* spb_data);
	// Start service thread
	void start(USHORT spb_length, const UCHAR* spb_data);
	// Query service state (v. 1 & 2)
	void query(USHORT send_item_length, const SCHAR* send_items, USHORT recv_item_length,
			   const SCHAR* recv_items, USHORT buffer_length, SCHAR* info);
	ISC_STATUS query2(thread_db* tdbb, USHORT send_item_length, const SCHAR* send_items,
			   USHORT recv_item_length, const SCHAR* recv_items, USHORT buffer_length, SCHAR* info);
	// Detach from service
	void detach();
	// get service version
	USHORT getVersion() const
	{
		return svc_spb_version;
	}

	// Firebird log reader
	static THREAD_ENTRY_DECLARE readFbLog(THREAD_ENTRY_PARAM arg);
	// Shuts all service threads (should be called after databases shutdown)
	static void shutdownServices();

private:
	// Service must have private destructor, called from finish
	// when both (server and client) threads are finished
	~Service();
	void	readFbLog();
	// Create argv, argc and svc_parsed_sw
	void	parseSwitches();
	// Get character from stdout buffer
	UCHAR	dequeueByte();
	// Put character to stdout buffer
	void	enqueueByte(const UCHAR c);
	// true if there is any data in stdout buffer
	bool	empty() const;
	// true if no more space in stdout buffer
	bool	full() const;
	// start service thread
	void	start(ThreadEntryPoint* service_thread);
	// Set the flag (either SVC_finished for the main service thread or SVC_detached for the client thread).
	// If both main thread and client thread are completed that is main thread is finished and 
	// client is detached then free memory used by service.
	void	finish(USHORT flag);
	// Throws shutdown exception if global flag is set for it
	bool	checkForShutdown();
	// Transfer data from svc_stdout into buffer
	void	get(SCHAR* buffer, USHORT length, USHORT flags, USHORT timeout, USHORT* return_length);
	// Designed to send output to a service - does nothing.
	void	put(const SCHAR* buffer, USHORT length);

	// Increment circular buffer pointer
	static USHORT		add_one(USHORT i);
	// Convert spb flags to utility switches
	static void			conv_switches(Firebird::ClumpletReader& spb, Firebird::string& switches);
	// Find spb switch in switch table
	static const TEXT*	find_switch(int in_spb_sw, const in_sw_tab_t* table);
	// Loop through the appropriate switch table looking for the text for the given command switch
	static bool			process_switches(Firebird::ClumpletReader& spb, Firebird::string& switches);
	// Get bitmask from within spb buffer, find corresponding switches within specified table,
	// add them to the command line
	static bool get_action_svc_bitmask(const Firebird::ClumpletReader& spb, const in_sw_tab_t* table, 
									   Firebird::string& sw);
	// Get string from within spb buffer, add it to the command line
	static void get_action_svc_string(const Firebird::ClumpletReader& spb, Firebird::string& sw);
	// Get integer from within spb buffer, add it to the command line
	static void get_action_svc_data(const Firebird::ClumpletReader& spb, Firebird::string& sw);
	// Get parameter from within spb buffer, find corresponding switch within specified table,
	// add it to the command line
	static bool get_action_svc_parameter(UCHAR tag, const in_sw_tab_t* table, Firebird::string&);
	// Create 'SYSDBA needed' error in status vector 
	static void need_admin_privs(Firebird::Arg::StatusVector& status, const char* message);
	// Does info buffer have enough space for SLONG?
	static bool ck_space_for_numeric(char*& info, const char* const end);

private:
	ISC_STATUS_ARRAY svc_status_array;
	Firebird::string svc_parsed_sw;		// Here point elements of argv
	SLONG	svc_handle;					// "handle" of process/thread running service
	ISC_STATUS*	svc_status;				// status vector for svc_handle
	ULONG	svc_stdout_head;
	ULONG	svc_stdout_tail;
	UCHAR	svc_stdout[SVC_STDOUT_BUFFER_SIZE + 1];		// output from service
	Firebird::Semaphore	svcStart;
	const serv_entry*	svc_service;
	Firebird::Array<UCHAR> svc_resp_alloc;
	UCHAR*	svc_resp_buf;
	const UCHAR*	svc_resp_ptr;
	USHORT	svc_resp_buf_len;
	USHORT	svc_resp_len;
	USHORT	svc_flags;
	USHORT	svc_user_flag;
	USHORT	svc_spb_version;
	bool	svc_do_shutdown;
	Firebird::string	svc_username;
	Firebird::string	svc_enc_password;
	Firebird::string	svc_trusted_login;
	bool                svc_trusted_role;
	bool				svc_uses_security_database;
	Firebird::string	svc_switches;	// Full set of switches
	Firebird::string	svc_perm_sw;	// Switches, taken from services table and/or passed using spb_command_line
	Firebird::string	svc_address_path;
	Firebird::StringsBuffer* svc_strings_buffer;
};

} //namespace Jrd

#endif // JRD_SVC_H
