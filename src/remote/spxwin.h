/*
 *	PROGRAM:	JRD Remote Interface
 *	MODULE:		spxwin.h
 *	DESCRIPTION:	Windows SPX communications interface definitions
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

#include <nwipxspx.h>
#include <nwbindry.h>
#include <nwcalls.h>

#define MAX_DATA	576 - sizeof(SPXHeader)
#define MAX_SEQUENCE	256
#define BUFFER_SIZE	4000
#define MAXPATHLEN	1024
#define MAXHOSTLEN	64
#define NOFILE		20
#define INTERBASE_SOCKET 0x8794
#define RX_POOL_SIZE 5
#define TX_POOL_SIZE 5


typedef struct spx_port_handle
    {
    WORD  connection_id;
    DWORD task_id;
    void FAR *rx_pool;
    void FAR   *tx_pool;
    WORD       local_socket;
    IPXAddress server_address;
    SLONG      next_fragment;
    HGLOBAL    rx_global;
    HGLOBAL    tx_global;
    HGLOBAL    global_handle;
    HWND       window_handle;
    struct     spx_port_handle *next;
} spx_port_handle;

