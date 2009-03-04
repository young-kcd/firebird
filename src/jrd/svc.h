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

#include "../jrd/thd.h"
#include "../jrd/jrd_pwd.h"
#include "../jrd/isc.h"
#include "../jrd/svc_undoc.h"
#include "../jrd/svc_proto.h"
#include "../jrd/isc_s_proto.h"
#include "../common/classes/semaphore.h"

#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"
#include "../common/classes/array.h"
#include "../common/classes/SafeArg.h"

void SVC_STATUS_ARG(ISC_STATUS*& status, const MsgFormat::safe_cell& value);
void SVC_STATUS_ARG(ISC_STATUS*& status, const char* value);

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


/* switches for username and password used when a username and/or password
 * is specified by the client application
 */
#define USERNAME_SWITCH "-USER"
#define PASSWORD_SWITCH "-PASSWORD"
#ifdef SERVICE_THREAD
#define SERVICE_THD_PARAM "-svc_thd"
#else
#define SERVICE_THD_PARAM "-svc"
#endif
#ifdef TRUSTED_SERVICES
#define TRUSTED_USER_SWITCH "-TRUSTED_SVC"
#endif

/* Macro used to store services thread specific data */
/* Currently we store empty string, see bug #10394 */
// BRS 01/07/2004 commented
/*
#ifdef SUPERSERVER
#define SVC_PUTSPECIFIC_DATA	{\
				char    t_data[] = {'\0'};\
				thdd::putSpecificData((void*)t_data);\
				}
#else
#define SVC_PUTSPECIFIC_DATA	// nothing
#endif
*/

struct serv_entry; // forward decl.

/* Service manager block */
class Service : public pool_alloc<type_svc>
{
private:
	ISC_STATUS_ARRAY svc_status_array;
	Firebird::string svc_parsed_sw;		// Here point elements of svc_argv

public:
	Service(serv_entry *se, Firebird::MemoryPool& p);
	~Service();
	
	SLONG	svc_handle;			/* "handle" of process/thread running service */
	ISC_STATUS*	svc_status;		/* status vector for svc_handle */
	FILE*	svc_input;			/* input to service */
	FILE*	svc_output;			/* output from service */
	ULONG	svc_stdout_head;
	ULONG	svc_stdout_tail;
	UCHAR*	svc_stdout;
	Firebird::HalfStaticArray<const char*, 20>	svc_argv;
	ULONG	svc_argc;
	Firebird::Semaphore	svcStart;
	serv_entry*	svc_service;
	UCHAR*	svc_resp_buf;
	const UCHAR*	svc_resp_ptr;
	USHORT	svc_resp_buf_len;
	USHORT	svc_resp_len;
	USHORT	svc_flags;
	USHORT	svc_user_flag;
	USHORT	svc_spb_version;
	bool	svc_do_shutdown;
	bool	svc_uses_security_database;
	Firebird::string	svc_username;
	Firebird::string	svc_enc_password;
#ifdef TRUSTED_SERVICES
	Firebird::string	svc_trusted_login;
#endif
	Firebird::string	svc_switches;	// Full set of switches
	Firebird::string	svc_perm_sw;	// Switches, taken from services table 
										// and/or passed using spb_command_line
	
	void	svc_started();
	void	parseSwitches();			// Create svc_argv, svc_argc and svc_parsed_sw
};

/* Bitmask values for the svc_flags variable */

//const int SVC_eof			= 1;
const int SVC_timeout		= 2;
const int SVC_forked		= 4;
const int SVC_detached		= 8;
const int SVC_finished		= 16;
const int SVC_thd_running	= 32;
const int SVC_evnt_fired	= 64;
const int SVC_cmd_line		= 128;

// Method used to signify that the service started has done basic
// initialization and can be considered a successful startup.

#ifndef SERVICE_THREAD

inline void Service::svc_started()
{
	// null definition, no overhead.
}

#endif /* SERVICE_THREAD */

typedef int (*pfn_svc_output)(Service*, const UCHAR*);

struct serv_entry
{
	USHORT				serv_action;
	const TEXT*			serv_name;
	const TEXT*			serv_std_switches;
	const TEXT*			serv_executable;
	ThreadEntryPoint*	serv_thd;
	bool*				serv_in_use;
};

} //namespace Jrd

#endif // JRD_SVC_H
