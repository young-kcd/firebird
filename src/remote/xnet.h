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
 */

#ifndef _REMOTE_XNET_H_
#define _REMOTE_XNET_H_

#ifndef WIN_NT
#include <sys/types.h>
//typedef short DWORD;
#define PID_T	pid_t
#define CADDR_T	caddr_t
#define FILE_ID	int
#else
#define PID_T  	DWORD
#define CADDR_T	LPVOID
#define FILE_ID	HANDLE
#endif

/* mapped file parameters */

#define XPS_MAPPED_PER_CLI(p)   ((ULONG)(p) * 1024L)
#define XPS_MAPPED_FOR_CLI(p,x) ((ULONG)(x) * XPS_MAPPED_PER_CLI(p))
#define XPS_MAPPED_SIZE(p,c)    ((ULONG)(c) * XPS_MAPPED_PER_CLI(p))
#define XPS_USEFUL_SPACE(p)     (XPS_MAPPED_PER_CLI(p) - sizeof( struct xps))

#define XPS_DEF_NUM_CLI         10      /* default clients per mapped file */
#define XPS_DEF_NUM_MAPS        10      /* default maps available */
#define XPS_DEF_PAGES_PER_CLI   8       /* default 1k pages space per client */

#define XPS_MIN_NUM_CLI         1       /* min clients per mapped file */
#define XPS_MIN_NUM_MAPS        1       /* min maps available */
#define XPS_MIN_PAGES_PER_CLI   1       /* min 1k pages space per client */

#define XPS_MAX_NUM_CLI         64      /* max clients per mapped file */
#define XPS_MAX_NUM_MAPS        64      /* max maps available */
#define XPS_MAX_PAGES_PER_CLI   16      /* max 1k pages space per client */

#define XPS_PACK_PARAMS(x,p,m,u) (((ULONG)(x) << 24) + ((ULONG)(p) << 16) + ((m) << 8) + (u))
#define XPS_UNPACK_MAX_SLOTS(n) (((ULONG)(n) >> 24) & 0xFF)
#define XPS_UNPACK_PAGES(n)     (((ULONG)(n) >> 16) & 0xFF)
#define XPS_UNPACK_MAPNUM(n)    (((ULONG)(n) >> 8) & 0xFF)
#define XPS_UNPACK_SLOTNUM(n)   ((n) & 0xFF)

/* mapped file structure */

typedef struct xpm {
    struct xpm  *xpm_next;              /* pointer to next one */
    USHORT      xpm_count;              /* slots in use */
    USHORT      xpm_number;             /* mapped area number */
    FILE_ID     xpm_handle;             /* handle of mapped memory */
    USHORT      xpm_flags;              /* flag word */
    CADDR_T     xpm_address;            /* address of mapped memory */
    UCHAR       *xpm_ids[XPS_MAX_NUM_CLI]; /* ids */
} *XPM;

/* mapped structure flags */

#define XPMF_SERVER_SHUTDOWN    1       /* server has shut down */

/* thread connection control block */

typedef struct xcc {
    struct xcc  *xcc_next;              /* pointer to next thread */
    struct xpm  *xcc_xpm;               /* pointer back to xpm */
    BOOLEAN	xcc_send_channel_locked;	/* status of send channel */
    SSHORT      xcc_file;               /* this thread's mapped file number */
    USHORT      xcc_slot;               /* this thread's slot number */
    FILE_ID     xcc_file_handle;        /* mapped file's handle */
#ifdef WIN_NT
    HANDLE      xcc_server_proc;        /* server's process handle */
    HANDLE      xcc_client_proc;        /* client's process handle */
#endif
    PID_T       xcc_server_id;          /* server's process id */
    PID_T       xcc_client_id;          /* client's process id */
#ifdef WIN_NT
    HANDLE      xcc_recv_sem;           /* start receive semaphore handle */
    HANDLE      xcc_send_sem;           /* start send semaphore handle */
#endif
    struct xch  *xcc_receive_channel;   /* receive channel structure */
    struct xch  *xcc_send_channel;      /* send channel structure */
    USHORT      xcc_flags;              /* status bits */
    UCHAR       *xcc_mapped_addr;       /* where the thread's mapped to */
} *XCC;

/* xcc structure flags */

#define XCCF_SHUTDOWN           1       /* shutdown in progress */
#define XCCF_SERVER_SHUTDOWN    2       /* server has shut down */

/* comm channel structure - four per connection (client to server data,
   server to client data, client to server events, server to client events) */

typedef struct xch
{
    USHORT      xch_sent;       /* messages sent */
    USHORT      xch_received;   /* messages received */
    USHORT      xch_length;     /* message length */
    USHORT      xch_size;       /* channel data size */
    USHORT      xch_flags;      /* flags */
    UCHAR       *xch_buffer;    /* message */
    UCHAR 	*xch_client_ptr; /* client pointer to xch buffers */
} *XCH;

#define XCHFLAG_more_data       1       /* more data coming */
#define XCHFLAG_data_overrun    2       /* receive buffer full */

/*
    This structure is mapped to the start of the allocated
    communications area between the client and server.
*/

typedef struct xps
{
    ULONG       xps_server_protocol;    /* server's protocol level */
    ULONG       xps_client_protocol;    /* client's protocol level */
#ifdef WIN_NT
    HANDLE      xps_server_proc;        /* server's process handle */
    HANDLE      xps_client_proc;        /* client's process handle */
#endif
    PID_T       xps_server_id;          /* server's process id */
    PID_T       xps_client_id;          /* client's process id */
    USHORT      xps_flags;              /* flags word */
    struct xch  xps_channels[4];        /* comm channels */
    ULONG       xps_data[1];            /* start of data area */
} *XPS;

/* Windows names used to identify various named objects */

#define XPI_PREFIX				"FirebirdXPI"

#define XPI_SERVER_NAME         "%sServer"
#define XPI_SERVER_CLASS        "%sServerClass"
#define XPI_MAPPED_FILE_NAME    "%sMappedArea%d"
#define XPI_C_TO_S_SEM_NAME     "%sCtoS%d_%d"
#define XPI_S_TO_C_SEM_NAME     "%sStoC%d_%d"
#define XPI_C_TO_S_EVT_SEM_NAME "%sCtoSEvent%d_%d"
#define XPI_S_TO_C_EVT_SEM_NAME "%sStoCEvent%d_%d"
#define XPI_EVENT_NAME          "%sEvent%d"
#define XPI_EVENT_CLASS         "%sEventClass"
#define XPI_EVENT_THREAD        "%sEventThread%d_%d"

#define	XPI_CONNECT_MESSAGE	WM_USER + 3

#endif /* _REMOTE_XNET_H_ */
