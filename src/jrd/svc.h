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

// TMN: To be removed once the C++ conversion is completed
#ifdef INCLUDE_OLD_FB_BLK
#error You can not include both old_fb_blk.h and this file
#endif

#include "../jrd/jrd_pwd.h"
#include "../jrd/isc.h"
#include "../jrd/svc_undoc.h"
#include "../jrd/svc_proto.h"
#include "../jrd/isc_s_proto.h"

#ifndef JRD_IBASE_H
#include "../jrd/y_ref.h"
#include "../jrd/ibase.h"		/* needed for the C++ version of SVC_STATUS_ARG */
#endif

#include "../jrd/jrd_blks.h"
#include "../include/fb_blk.h"


extern "C" {


#define SERVICE_VERSION		2

#define SVC_STDOUT_BUFFER_SIZE	1024

/* Flag of capabilities supported by the server */
#define WAL_SUPPORT					  0x1L	/* Write Ahead Log */
#define MULTI_CLIENT_SUPPORT		  0x2L	/* SuperServer model (vs. multi-inet) */
#define REMOTE_HOP_SUPPORT			  0x4L	/* Server can connect to other server */
#define NO_SVR_STATS_SUPPORT		  0x8L	/* Does not support statistics */
#define NO_DB_STATS_SUPPORT			 0x10L	/* Does not support statistics */
#define LOCAL_ENGINE_SUPPORT		 0x20L	/* The local 16 bit engine */
#define NO_FORCED_WRITE_SUPPORT		 0x40L	/* Can not configure sync writes */
#define NO_SHUTDOWN_SUPPORT			 0x80L	/* Can not shutdown/restart databases */
#define NO_SERVER_SHUTDOWN_SUPPORT	0x100L	/* Can not shutdown server */
#define SERVER_CONFIG_SUPPORT		0x200L	/* Can configure server */
#define QUOTED_FILENAME_SUPPORT		0x400L	/* Can pass quoted filenames in */

/* Range definitions for service actions.  Any action outside of
   this range is not supported */
#define isc_action_min                 1
#define isc_action_max                 14

/* Range definitions for service actions.  Any action outside of
   this range is not supported */
#define isc_info_min                  50
#define isc_info_max                  67


/* switches for username and password used when a username and/or password
 * is specified by the client application
 */
#define USERNAME_SWITCH "-USER"
#define PASSWORD_SWITCH "-PASSWORD"
#if defined ( SUPERSERVER ) || defined ( SUPERCLIENT )
#define SERVICE_THD_PARAM "-svc_thd"
#else
#define SERVICE_THD_PARAM "-svc"
#endif

/* Macro used to store services thread specific data */
/* Currently we store empty string, see bug #10394 */
#ifdef SUPERSERVER
#define SVC_PUTSPECIFIC_DATA	{\
				char    t_data[] = {'\0'};\
				THD_putspecific_data((void*)t_data);\
				}
#else
#define SVC_PUTSPECIFIC_DATA	/* nothing */
#endif

/* Macro used to signify that the service started has done basic
 * initialization and can be considered a successful startup
 */
#ifndef SUPERSERVER

#define SVC_STARTED(service)

#else /* SUPERSERVER */

#define SVC_STARTED(service)	{{EVENT evnt_ptr = service->svc_start_event; \
    				  if (!(service->svc_flags & SVC_evnt_fired)) { \
				      service->svc_flags |= SVC_evnt_fired; \
			   	      ISC_event_post (evnt_ptr);}}}


#endif /* SUPERSERVER */

void SVC_STATUS_ARG(ISC_STATUS*& status, USHORT type, const void* value);

#define CK_SPACE_FOR_NUMERIC 	{{if ((info + 1 + sizeof (ULONG)) > end) \
    				      { \
				      if (info < end) \
					  *info++ = isc_info_truncated; \
				      THREAD_ENTER; \
				      return 0; \
				      }}}

/* Service manager block */
class svc : public pool_alloc<type_svc>
{
public:
	SLONG	svc_handle;			/* "handle" of process/thread running service */
	ISC_STATUS*	svc_status;			/* status vector for svc_handle */
	void*	svc_input;			/* input to service */
	void*	svc_output;			/* output from service */
	ULONG	svc_stdout_head;
	ULONG	svc_stdout_tail;
	UCHAR*	svc_stdout;
	TEXT**	svc_argv;
	ULONG	svc_argc;
	EVENT_T	svc_start_event[1];	/* fired once service has started successfully */
	const struct serv*	svc_service;
	UCHAR*	svc_resp_buf;
	UCHAR*	svc_resp_ptr;
	USHORT	svc_resp_buf_len;
	USHORT	svc_resp_len;
	USHORT	svc_flags;
	USHORT	svc_user_flag;
	USHORT	svc_spb_version;
	BOOLEAN	svc_do_shutdown;
	TEXT	svc_username[33];
	TEXT	svc_enc_password[MAX_PASSWORD_ENC_LENGTH];
	TEXT	svc_reserved[1];
	TEXT*	svc_switches;
};
typedef svc *SVC;

typedef int(*pfn_svc_main) (SVC);

typedef struct serv
{
	USHORT		serv_action;
	const TEXT*	serv_name;
	const TEXT*	serv_std_switches;
	const TEXT*	serv_executable;
	pfn_svc_main	serv_thd;
	BOOLEAN*	in_use;
} *SERV;

/* Bitmask values for the svc_flags variable */

#define SVC_eof			 1
#define SVC_timeout		 2
#define SVC_forked		 4
#define SVC_detached	 8
#define SVC_finished	16
#define SVC_thd_running	32
#define SVC_evnt_fired	64


} /* extern "C" */

typedef int(*pfn_svc_output)(SLONG, UCHAR*);

#endif /* JRD_SVC_H */
