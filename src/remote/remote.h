/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		remote.h
 *	DESCRIPTION:	Common descriptions
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

#ifndef REMOTE_REMOTE_H
#define REMOTE_REMOTE_H

#include "../jrd/common.h"
#include "../remote/remote_def.h"
#include "../jrd/thd.h"

/* Include some apollo include files for tasking */

#if !(defined VMS || defined WIN_NT)
#include <signal.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#endif /* !VMS || !WIN_NT */


// Uncomment this line if you need to trace module activity
//#define REMOTE_DEBUG

#ifdef REMOTE_DEBUG
DEFINE_TRACE_ROUTINE(remote_trace);
#define REMOTE_TRACE(args) remote_trace args
#else
#define REMOTE_TRACE(args) /* nothing */
#endif

#ifdef DEV_BUILD
/* Debug packet/XDR memory allocation */

/* Temporarily disabling DEBUG_XDR_MEMORY */
/* #define DEBUG_XDR_MEMORY	*/

#endif

const int BLOB_LENGTH		= 16384;

#include "../remote/protocol.h"

/* Block types */

struct blk;

#ifndef INCLUDE_FB_BLK
#include "../include/old_fb_blk.h"
#endif

// fwd. decl.
struct rem_port;

typedef struct rdb
{
	blk				rdb_header;
	USHORT			rdb_id;
	USHORT			rdb_flags;
	FB_API_HANDLE	rdb_handle;			/* database handle */
	rem_port*		rdb_port;			/* communication port */
	struct rtr*		rdb_transactions;	/* linked list of transactions */
	struct rrq*		rdb_requests;		/* compiled requests */
	struct rvnt*	rdb_events;			/* known events */
	struct rsr*		rdb_sql_requests;	/* SQL requests */
	ISC_STATUS*		rdb_status_vector;
	PACKET			rdb_packet;			/* Communication structure */
} *RDB;

// rdb_flags
const USHORT RDB_service	= 1;		/* structure relates to a service */

typedef struct rtr
{
	blk			rtr_header;
	rdb*		rtr_rdb;
	rtr*		rtr_next;
	struct rbl*	rtr_blobs;
	FB_API_HANDLE rtr_handle;
	USHORT		rtr_flags;
	USHORT		rtr_id;
} *RTR;

// rtr_flags
const USHORT RTR_limbo	= 1;

typedef struct rbl
{
	blk			rbl_header;
	rdb*		rbl_rdb;
	rtr*		rbl_rtr;
	rbl*		rbl_next;
	FB_API_HANDLE rbl_handle;
	SLONG		rbl_offset;			/* Apparent (to user) offset in blob */
	USHORT		rbl_id;
	USHORT		rbl_flags;
	UCHAR*		rbl_ptr;
	UCHAR*		rbl_buffer;
	USHORT		rbl_buffer_length;
	USHORT		rbl_length;
	USHORT		rbl_fragment_length;
	USHORT		rbl_source_interp;	/* source interp (for writing) */
	USHORT		rbl_target_interp;	/* destination interp (for reading) */
	UCHAR		rbl_data[1];
} *RBL;

// rbl_flags
const USHORT RBL_eof		= 1;
const USHORT RBL_segment	= 2;
const USHORT RBL_eof_pending= 4;
const USHORT RBL_create		= 8;

typedef struct rvnt
{
	blk			rvnt_header;
	rvnt*		rvnt_next;
	rdb*		rvnt_rdb;
	FPTR_EVENT_CALLBACK	rvnt_ast;
	void*		rvnt_arg;
	SLONG		rvnt_id;
	SLONG		rvnt_rid;	/* used by server to store client-side id */
	rem_port*	rvnt_port;	/* used to id server from whence async came */
	const UCHAR*		rvnt_items;
	SSHORT		rvnt_length;
} *RVNT;

struct rem_vec
{
	blk			vec_header;
	ULONG		vec_count;
	blk*		vec_object[1];
};

//struct rem_vcl
//{
//	blk			vcl_header;
//	ULONG		vcl_count;
//	SLONG		vcl_long[1];
//};

/* Random string block -- jack of all kludges */

struct rem_str
{
	blk			str_header;
	USHORT		str_length;
	SCHAR		str_data[2];
};

/* Include definition of descriptor */

#include "../jrd/dsc.h"


struct rem_fmt
{
	blk			fmt_header;
	USHORT		fmt_length;
	USHORT		fmt_net_length;
	USHORT		fmt_count;
	USHORT		fmt_version;
	USHORT		fmt_flags;
	struct dsc	fmt_desc[1];
};

// fmt_flags (not used)
//#define FMT_has_P10_specific_datatypes	0x1	/* datatypes don't exist in P9 */

/* Windows declares a msg structure, so rename the structure 
   to avoid overlap problems. */

typedef struct message
{
	blk			msg_header;
	message*	msg_next;	/* Next available message */
#ifdef SCROLLABLE_CURSORS
	message*	msg_prior;	/* Next available message */
	ULONG		msg_absolute; 		/* Absolute record number in cursor result set */
#endif
	/* Please DO NOT re-arrange the order of following two fields.
	   This could result in alignment problems while trying to access
	   'msg_buffer' as a 'long', leading to "core" drops 
		Sriram - 04-Jun-97 */
	USHORT		msg_number;			/* Message number */
	UCHAR*		msg_address;		/* Address of message */
	UCHAR		msg_buffer[1];		/* Allocated message */
} *REM_MSG;

/* remote stored procedure request */

typedef struct rpr
{
	blk			rpr_header;
	rdb*		rpr_rdb;
	rtr*		rpr_rtr;
	FB_API_HANDLE rpr_handle;
	message*	rpr_in_msg;		/* input message */
	message*	rpr_out_msg;	/* output message */
	rem_fmt*	rpr_in_format;	/* Format of input message */
	rem_fmt*	rpr_out_format;	/* Format of output message */
	USHORT		rpr_flags;
} *RPR;

// rpr_flags (not used)
//#define RPR_eof		1		/* End-of-stream encountered */

struct rrq
{
	blk		rrq_header;
	rdb*	rrq_rdb;
	rtr*	rrq_rtr;
	rrq*	rrq_next;
	rrq*	rrq_levels;		/* RRQ block for next level */
	FB_API_HANDLE rrq_handle;
	USHORT		rrq_id;
	USHORT		rrq_max_msg;
	USHORT		rrq_level;
	ISC_STATUS_ARRAY	rrq_status_vector;
	struct		rrq_repeat
	{
		rem_fmt*	rrq_format;		/* format for this message */
		message*	rrq_message; 	/* beginning or end of cache, depending on whether it is client or server */
		message*	rrq_xdr;		/* point at which cache is read or written by xdr */
#ifdef SCROLLABLE_CURSORS
		message*	rrq_last;		/* last message returned */
		ULONG		rrq_absolute;	/* current offset in result set for record being read into cache */
		USHORT		rrq_flags;
#endif
		USHORT		rrq_msgs_waiting;	/* count of full rrq_messages */
		USHORT		rrq_rows_pending;	/* How many rows in waiting */
		USHORT		rrq_reorder_level;	/* Reorder when rows_pending < this level */
		USHORT		rrq_batch_count;	/* Count of batches in pipeline */

	} rrq_rpt[1];
};

// rrq_flags
const USHORT RRQ_backward			= 1;	/* the cache was created in the backward direction */ 
const USHORT RRQ_absolute_backward	= 2;	/* rrq_absolute is measured from the end of the stream */
const USHORT RRQ_last_backward		= 4;	/* last time, the next level up asked for us to scroll in the backward direction */

/* remote SQL request */

typedef struct rsr
{
	blk				rsr_header;
	rsr*			rsr_next;
	rdb*			rsr_rdb;
	rtr*			rsr_rtr;
	FB_API_HANDLE	rsr_handle;
	rem_fmt*		rsr_bind_format;		/* Format of bind message */
	rem_fmt*		rsr_select_format;		/* Format of select message */
	rem_fmt*		rsr_user_select_format; /* Format of user's select message */
	rem_fmt*		rsr_format;				/* Format of current message */
	message*		rsr_message;			/* Next message to process */
	message*		rsr_buffer;				/* Next buffer to use */
	ISC_STATUS_ARRAY	rsr_status_vector;	/* saved status for buffered errors */
	USHORT			rsr_id;
	USHORT			rsr_flags;
	USHORT			rsr_fmt_length;

	ULONG			rsr_rows_pending;	/* How many rows are pending */
	USHORT			rsr_msgs_waiting; 	/* count of full rsr_messages */
	USHORT			rsr_reorder_level; 	/* Trigger pipelining at this level */
	USHORT			rsr_batch_count; 	/* Count of batches in pipeline */
} *RSR;

// rsr_flags
const USHORT RSR_fetched	= 1;		/* Cleared by execute, set by fetch */
const USHORT RSR_eof		= 2;		/* End-of-stream encountered */
const USHORT RSR_blob		= 4;		/* Statement relates to blob op */
const USHORT RSR_no_batch	= 8;		/* Do not batch fetch rows */
const USHORT RSR_stream_err	= 16;		/* There is an error pending in the batched rows */


enum blk_t
{
	type_MIN = 0,
	type_vec,
	type_rdb,
	type_fmt,
	type_rrq,
	type_rtr,
	type_str,
	type_rbl,
	type_port,
	type_msg,
	type_rsr,
	type_rvnt,
	type_rpr,
	type_rmtque,
	type_MAX
};


#include "../remote/xdr.h"


/* Generalized port definition. */

enum rem_port_t
{
	port_mailbox,		/* Apollo mailbox */
	port_pcic,			/* IBM PC interconnect */
	port_inet,			/* Internet (TCP/IP) */
	port_asyn_homebrew,	/* homebrew asynchronous connection */
	port_decnet,		/* DECnet connection */
	port_ipc,			/* NetIPC connection */
	port_pipe,			/* Windows NT named pipe connection */
	port_mslan,			/* Microsoft LanManager connection */
	port_spx,			/* Novell SPX connection */
	//port_ipserver,		// InterBase interprocess server
	port_xnet			/* Windows NT named xnet connection */
};

enum state_t
{
	state_closed,		/* no connection */
	state_pending,		/* connection is pending */
	state_eof,			/* other side has shut down */
	state_broken,		/* connection is broken */
	state_active,		/* connection is complete */
	state_disconnected          /* port is disconnected */
};


#ifndef WIN_NT
typedef int HANDLE;
#endif  /* WIN_NT */


//////////////////////////////////////////////////////////////////
// fwd. decl.
struct p_cnct;
struct rmtque;

class port_interface
{
public:
	virtual int		accept_(rem_port* pPort, p_cnct* pConnection) = 0;
};

//typedef XDR_INT (*t_event_ast)();
typedef void (*t_event_ast)(rem_port*);
typedef rem_port* (*t_port_connect)(rem_port*, PACKET*, t_event_ast);

struct rem_port
{
	blk				port_header;
	enum rem_port_t	port_type;			/* type of port */
	enum state_t	port_state;			/* state of port */
	P_ARCH			port_client_arch;	/* so we can tell arch of client */
	rem_port*		port_clients;		/* client ports */
	rem_port*		port_next;			/* next client port */
	rem_port*		port_parent;		/* parent port (for client ports) */
	rem_port*		port_async;			/* asynchronous sibling port */
	struct srvr*	port_server;		/* server of port */
	USHORT			port_server_flags;	/* TRUE if server */
	USHORT			port_protocol;		/* protocol version number */
	USHORT			port_buff_size;		/* port buffer size (approx) */
	USHORT			port_flags;			/* Misc flags */
	SLONG			port_connect_timeout;   /* Connection timeout value */
	SLONG			port_dummy_packet_interval; /* keep alive dummy packet interval */
	SLONG			port_dummy_timeout;	/* time remaining until keepalive packet */
	ISC_STATUS*		port_status_vector;
	HANDLE			port_handle;		/* handle for connection (from by OS) */
	int				port_channel;		/* handle for connection (from by OS) */
	int				port_misc1;
	SLONG			port_semaphore;
	struct linger	port_linger;		/* linger value as defined by SO_LINGER */

	/* port function pointers (C "emulation" of virtual functions) */
	int				(*port_accept)(rem_port*, p_cnct*);
	void			(*port_disconnect)(rem_port*);
	rem_port*		(*port_receive_packet)(rem_port*, PACKET*);
	XDR_INT			(*port_send_packet)(rem_port*, PACKET*);
	XDR_INT			(*port_send_partial)(rem_port*, PACKET*);
	t_port_connect	port_connect;	/* Establish secondary connection */
	rem_port*		(*port_request)(rem_port*, PACKET*);	/* Request to establish secondary connection */

	rdb*			port_context;
	t_event_ast		port_ast;		/* AST for events */
	XDR				port_receive;
	XDR				port_send;
#ifdef DEBUG_XDR_MEMORY
	rem_vec*		port_packet_vector;	/* Vector of send/receive packets */
#endif
	rem_vec*		port_object_vector;
	BLK*			port_objects;
	rem_str*		port_version;
	rem_str*		port_host;			/* Our name */
	rem_str*		port_connection;	/* Name of connection */
	rem_str*		port_user_name;
	rem_str*		port_passwd;
	rem_str*		port_protocol_str;	// String containing protocol name for this port
	rem_str*		port_address_str;	// Protocol-specific address string for the port
	rpr*			port_rpr;			/* port stored procedure reference */
	rsr*			port_statement;		/* Statement for execute immediate */
	rmtque*			port_receive_rmtque;	/* for client, responses waiting */
	USHORT			port_requests_queued;	/* requests currently queued */
#ifdef VMS
	USHORT			port_iosb[4];
#endif
	void*			port_xcc;              /* interprocess structure */
	UCHAR			port_buffer[1];

	/* TMN: Beginning of C++ port */
	/* TMN: ugly, but at least a start */
	int		accept(p_cnct* cnct);
	void	disconnect();
	rem_port*	receive(PACKET* pckt);
	XDR_INT	send(PACKET* pckt);
	XDR_INT	send_partial(PACKET* pckt);
	rem_port*	connect(PACKET* pckt, t_event_ast);
	rem_port*	request(PACKET* pckt);

	/* TMN: The following member functions are conceptually private
	 *      to server.cpp and should be _made_ private in due time!
	 *      That is, if we don't factor these method out.
	 */
	ISC_STATUS	compile(P_CMPL*, PACKET*);
	ISC_STATUS	ddl(P_DDL*, PACKET*);
	void	disconnect(PACKET*, PACKET*);
	void	drop_database(P_RLSE*, PACKET*);

	ISC_STATUS	end_blob(P_OP, P_RLSE*, PACKET*);
	ISC_STATUS	end_database(P_RLSE*, PACKET*);
	ISC_STATUS	end_request(P_RLSE*, PACKET*);
	ISC_STATUS	end_statement(P_SQLFREE*, PACKET*);
	ISC_STATUS	end_transaction(P_OP, P_RLSE*, PACKET*);
	ISC_STATUS	execute_immediate(P_OP, P_SQLST*, PACKET*);
	ISC_STATUS	execute_statement(P_OP, P_SQLDATA*, PACKET*);
	ISC_STATUS	fetch(P_SQLDATA*, PACKET*);
	ISC_STATUS	fetch_blob(P_SQLDATA*, PACKET*);
	OBJCT	get_id(BLK);
	ISC_STATUS	get_segment(P_SGMT*, PACKET*);
	ISC_STATUS	get_slice(P_SLC*, PACKET*);
	ISC_STATUS	info(P_OP, P_INFO*, PACKET*);
	ISC_STATUS	insert(P_SQLDATA*, PACKET*);
	ISC_STATUS	open_blob(P_OP, P_BLOB*, PACKET*);
	ISC_STATUS	prepare(P_PREP*, PACKET*);
	ISC_STATUS	prepare_statement(P_SQLST*, PACKET*);
	ISC_STATUS	put_segment(P_OP, P_SGMT*, PACKET*);
	ISC_STATUS	put_slice(P_SLC*, PACKET*);
	ISC_STATUS	que_events(P_EVENT*, PACKET*);
	ISC_STATUS	receive_after_start(P_DATA*, PACKET*, ISC_STATUS*);
	ISC_STATUS	receive_msg(P_DATA*, PACKET*);
	ISC_STATUS	seek_blob(P_SEEK*, PACKET*);
	ISC_STATUS	send_msg(P_DATA*, PACKET*);
	ISC_STATUS	send_response(PACKET*, OBJCT, USHORT, const ISC_STATUS*);
	ISC_STATUS	service_attach(P_ATCH*, PACKET*);
	ISC_STATUS	service_end(P_RLSE*, PACKET*);
	ISC_STATUS	service_start(P_INFO*, PACKET*);
	ISC_STATUS	set_cursor(P_SQLCUR*, PACKET*);
	ISC_STATUS	start(P_OP, P_DATA*, PACKET*);
	ISC_STATUS	start_and_send(P_OP, P_DATA*, PACKET*);
	ISC_STATUS	start_transaction(P_OP, P_STTR*, PACKET*);
	ISC_STATUS	transact_request(P_TRRQ *, PACKET*);

};

// port_flags
const USHORT PORT_symmetric		= 1;	/* Server/client archiectures are symmetic */
const USHORT PORT_rpc			= 2;	/* Protocol is remote procedure call */
const USHORT PORT_pend_ack		= 4;	/* An ACK is pending on the port */
const USHORT PORT_broken		= 8;	/* Connect is broken */
const USHORT PORT_async			= 16;	/* Port is asynchronous channel for events */
const USHORT PORT_no_oob		= 32;	/* Don't send out of band data */
const USHORT PORT_disconnect	= 64;	/* Disconnect is in progress */
const USHORT PORT_pend_rec		= 128;	/* A record is pending on the port */
const USHORT PORT_not_trusted	= 256;	/* Connection is from an untrusted node */
const USHORT PORT_impersonate	= 512;	/* A remote user is being impersonated */
const USHORT PORT_dummy_pckt_set= 1024;	/* A dummy packet interval is set  */


/* Misc declarations */

#include "../remote/allr_proto.h"
#include "../jrd/thd.h"

/* Thread specific remote database block */

class trdb : public ThreadData
{
public:
	trdb(ISC_STATUS* status) 
		: ThreadData(ThreadData::tddRDB), trdb_status_vector(status)
	{
		trdb_database = 0;
	}
	rdb*	trdb_database;
	ISC_STATUS*	trdb_status_vector;
};

typedef trdb* TRDB;


inline trdb* REM_get_thread_data() {
	return (trdb*) ThreadData::getSpecific();
}
inline void REM_set_thread_data(trdb* &tdrdb, trdb* thd_context) {
	tdrdb = thd_context;
	tdrdb->putSpecific();
}
inline void REM_restore_thread_data() {
	ThreadData::restoreSpecific();
}

/* Queuing structure for Client batch fetches */

typedef bool (*t_rmtque_fn)(trdb*, rem_port*, rmtque*, ISC_STATUS*, USHORT);

typedef struct rmtque
{
	blk					rmtque_header;	// Memory allocator header
	rmtque*				rmtque_next;	// Next entry in queue
	void*				rmtque_parm;	// What request has response in queue
	rrq::rrq_repeat*	rmtque_message;	// What message is pending
	rdb*				rmtque_rdb;		// What database has pending msg

	/* Fn that receives queued entry */
	t_rmtque_fn			rmtque_function;
} *RMTQUE;

#endif // REMOTE_REMOTE_H

