/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		plserver.h
 *	DESCRIPTION:	Page/lock server
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

#ifndef _ISC_LOCK_PLSERVER_H_
#define _ISC_LOCK_PLSERVER_H_

/* General usage packet types */

#define PL_CONNECT	1			/* Establish connection */
#define PL_DISCONNECT	2		/* Break connection */
#define PL_ACK		3			/* Acknowledge message */
#define PL_ERROR	4			/* An error occurred */
#define PL_REPLY	5			/* Generic reply */

/* Packet types for page operations */

#define PL_PAGE_OPEN	10		/* Open existing file */
#define PL_PAGE_CREATE	11		/* Create new file */
#define PL_PAGE_READ	12		/* Read file data */
#define PL_PAGE_WRITE	13		/* Write file data */

/* Packet types for lock operations */

#define PL_LOCK_ENQ	20			/* Enqueue on lock */
#define PL_LOCK_CVT	21			/* Convert existing lock */
#define PL_LOCK_DEQ	22			/* Dequeue lock */
#define PL_LOCK_GRANT	23		/* Grant lock */
#define PL_LOCK_DENY	24		/* Deny lock */
#define PL_LOCK_BLOCK	25		/* Existing lock is blocking another process */

/* Generate packet format */

typedef struct phd {
	UCHAR phd_type;				/* Packet type */
	UCHAR phd_flags;			/* Misc flags */
	USHORT phd_sequence;		/* Packet sequence number */
	USHORT phd_ack;				/* Acknowleding packet */
	int *phd_connection;		/* Connection handle */
	int *phd_handle;			/* Internal handle */
	USHORT phd_length;			/* Miscellaneous usage */
	int phd_misc;				/* Also miscellaneous usage */
	UCHAR phd_string[16];		/* Associated string */
} PHD;

#define PHD_SERVER	1			/* Connection is from a server */

#define BUMP_SEQUENCE(n)	n = (++n == 10000) ? 1 : n;

#endif /* _ISC_LOCK_PLSERVER_H_ */
