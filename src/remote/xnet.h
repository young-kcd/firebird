/*
 *	PROGRAM:	Interprocess Interface definitions
 *      MODULE:         xnet.h
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
 * 2003.05.01 Victor Seryodkin, Dmitry Yemanov: Completed XNET implementation
 */

#ifndef REMOTE_XNET_H
#define REMOTE_XNET_H

#ifndef WIN_NT
#include <sys/types.h>
#define PID_T	pid_t
#define CADDR_T	caddr_t
#define FILE_ID	int
#else
#define PID_T  	DWORD
#define CADDR_T	LPVOID
#define FILE_ID	HANDLE
#endif

const DWORD XNET_CONNECT_TIMEOUT	= 10000;	/* client connect timeout (ms) */

const DWORD XNET_RECV_WAIT_TIMEOUT	= 10000;	/* Receive wait timeout (ms) */
const DWORD XNET_SEND_WAIT_TIMEOUT	= XNET_RECV_WAIT_TIMEOUT; /* Send wait timeout (ms) */

/* mapped file parameters */

#define XPS_MAPPED_PER_CLI(p)        ((ULONG)(p) * 1024L)
#define XPS_SLOT_OFFSET(pages, slot)  (XPS_MAPPED_PER_CLI(pages) * (ULONG)(slot))
#define XPS_MAPPED_SIZE(users, pages) ((ULONG)(users) * XPS_MAPPED_PER_CLI(pages))

#define XPS_USEFUL_SPACE(p)          (XPS_MAPPED_PER_CLI(p) - sizeof(struct xps))

const ULONG XPS_DEF_NUM_CLI			= 10;	/* default clients per mapped file */
const ULONG XPS_DEF_PAGES_PER_CLI	= 8;	/* default 1k pages space per client */

//#define XPS_MIN_NUM_CLI         1       /* min clients per mapped file */
//#define XPS_MIN_PAGES_PER_CLI   1       /* min 1k pages space per client */

const ULONG XPS_MAX_NUM_CLI			= 64;	/* max clients per mapped file */
const ULONG XPS_MAX_PAGES_PER_CLI	= 16;	/* max 1k pages space per client */

const ULONG XNET_INVALID_MAP_NUM	= 0xFFFFFFFF;

const ULONG XNET_EVENT_SPACE		= 100; /* half of space (bytes) for event handling per connection */

/* mapped file structure */

typedef struct xpm {
    struct xpm  *xpm_next;              /* pointer to next one */
    ULONG       xpm_count;              /* slots in use */
    ULONG       xpm_number;             /* mapped area number */
    FILE_ID     xpm_handle;             /* handle of mapped memory */
    USHORT      xpm_flags;              /* flag word */
    CADDR_T     xpm_address;            /* address of mapped memory */
    UCHAR       xpm_ids[XPS_MAX_NUM_CLI]; /* ids */
    time_t      xpm_timestamp;          /* timestamp to avoid map name confilcts */
} *XPM;

/* mapped structure flags */

const USHORT XPMF_SERVER_SHUTDOWN	= 1;	/* server has shut down */
const USHORT XPM_FREE				= 0;	/* xpm structure is free for use */
const USHORT XPM_BUSY				= 1;	/* xpm structure is in use */


/* xch comm channel structure - four per connection (client to server data,
   server to client data, client to server events, server to client events) */

typedef struct xch
{
    ULONG		xch_length;      /* message length */
    ULONG		xch_size;        /* channel data size */
    USHORT      xch_flags;       /* flags */
    UCHAR       *xch_buffer;     /* message */
    UCHAR 	    *xch_client_ptr; /* client pointer to xch buffers */
} *XCH;


/* thread connection control block (xcc) */

typedef struct xcc {
    struct xcc  *xcc_next;              /* pointer to next thread */
    XPM         xcc_xpm;               /* pointer back to xpm */
    ULONG       xcc_map_num;            /* this thread's mapped file number */
    ULONG       xcc_slot;               /* this thread's slot number */
    FILE_ID     xcc_map_handle;         /* mapped file's handle */
#ifdef WIN_NT
    HANDLE      xcc_proc_h;             /* for server client's process handle
		                                       for client server's process handle */

    HANDLE      xcc_event_send_channel_filled; /* xcc_send_channel ready for reading */
    HANDLE      xcc_event_send_channel_empted; /* xcc_send_channel ready for writting */
    HANDLE      xcc_event_recv_channel_filled; /* xcc_receive_channel ready for reading */
    HANDLE      xcc_event_recv_channel_empted; /* xcc_receive_channel ready for writing */
#endif
    XCH         xcc_recv_channel;				/* receive channel structure */
    XCH         xcc_send_channel;       /* send channel structure */
    ULONG       xcc_flags;              /* status bits */
    UCHAR       *xcc_mapped_addr;       /* where the thread's mapped to */
} *XCC;

/* xcc structure flags */
const ULONG XCCF_SERVER_SHUTDOWN		= 2;	/* server has shutdown detected */


/* This structure (xps) is mapped to the start of the allocated
    communications area between the client and server. */

typedef struct xps
{
    ULONG       xps_server_protocol;    /* server's protocol level */
    ULONG       xps_client_protocol;    /* client's protocol level */
    PID_T       xps_server_proc_id;     /* server's process id */
    PID_T       xps_client_proc_id;     /* client's process id */
    USHORT      xps_flags;              /* flags word */
    struct xch  xps_channels[4];        /* comm channels */
    ULONG       xps_data[1];            /* start of data area */
} *XPS;


/* xps_channel numbers */

const int XPS_CHANNEL_C2S_DATA		= 0;	/* 0 - client to server data */
const int XPS_CHANNEL_S2C_DATA		= 1;	/* 1 - server to client data */
const int XPS_CHANNEL_C2S_EVENTS	= 2;	/* 2 - client to server events */
const int XPS_CHANNEL_S2C_EVENTS	= 3;	/* 3 - server to client events */


const ULONG XPI_CLIENT_PROTOCOL_VERSION		= 3L;
const ULONG XPI_SERVER_PROTOCOL_VERSION		= 3L;

/* XNET_RESPONSE - server response on client connect request */
typedef struct{
	ULONG proc_id;
	ULONG slots_per_map;
	ULONG pages_per_slot;
	ULONG map_num;
	ULONG slot_num;
	time_t timestamp;
} XNET_RESPONSE, *PXNET_RESPONSE;

/* XNET_CONNECT_RESPONZE_SIZE - amount of bytes server writes on connect response */
const size_t XNET_CONNECT_RESPONZE_SIZE	= sizeof(XNET_RESPONSE);

/* Windows names used to identify various named objects */

const char* XNET_PREFIX					= "FirebirdXNET";
const char* XNET_MAPPED_FILE_NAME		= "%s_MAP_%"ULONGFORMAT"_%"ULONGFORMAT;

const char* XNET_MA_CONNECT_MAP			= "%s_CONNECT_MAP";
const char* XNET_MU_CONNECT_MUTEX		= "%s_CONNECT_MUTEX";
const char* XNET_E_CONNECT_EVENT		= "%s_E_CONNECT_EVENT";
const char* XNET_E_RESPONSE_EVENT		= "%s_E_RESPONSE_EVENT";
 
const char* XNET_E_C2S_DATA_CHAN_FILLED	= "%s_E_C2S_DATA_FILLED_%"ULONGFORMAT"_%"ULONGFORMAT"_%"ULONGFORMAT;
const char* XNET_E_C2S_DATA_CHAN_EMPTED	= "%s_E_C2S_DATA_EMPTED_%"ULONGFORMAT"_%"ULONGFORMAT"_%"ULONGFORMAT;
const char* XNET_E_S2C_DATA_CHAN_FILLED	= "%s_E_S2C_DATA_FILLED_%"ULONGFORMAT"_%"ULONGFORMAT"_%"ULONGFORMAT;
const char* XNET_E_S2C_DATA_CHAN_EMPTED	= "%s_E_S2C_DATA_EMPTED_%"ULONGFORMAT"_%"ULONGFORMAT"_%"ULONGFORMAT;

const char* XNET_E_C2S_EVNT_CHAN_FILLED	= "%s_E_C2S_EVNT_FILLED_%"ULONGFORMAT"_%"ULONGFORMAT"_%"ULONGFORMAT;
const char* XNET_E_C2S_EVNT_CHAN_EMPTED	= "%s_E_C2S_EVNT_EMPTED_%"ULONGFORMAT"_%"ULONGFORMAT"_%"ULONGFORMAT;
const char* XNET_E_S2C_EVNT_CHAN_FILLED	= "%s_E_S2C_EVNT_FILLED_%"ULONGFORMAT"_%"ULONGFORMAT"_%"ULONGFORMAT;
const char* XNET_E_S2C_EVNT_CHAN_EMPTED	= "%s_E_S2C_EVNT_EMPTED_%"ULONGFORMAT"_%"ULONGFORMAT"_%"ULONGFORMAT;

#endif /* REMOTE_XNET_H */

