/*
 *	PROGRAM:	Alice
 *	MODULE:		info.h
 *	DESCRIPTION:	gds__version definitions
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

#ifndef ALICE_INFO_H
#define ALICE_INFO_H

static UCHAR info[] =
{ 
    gds_info_version,
	gds_info_implementation,
	gds_info_end
};


/* this table describes the implementations that 
   require a node_name,protocol combination be 
   generated */

static BOOLEAN generate_protocol[] =
{
	0,							/* NULL, 0 */
	0,							/* access method */
	0,							/* Y-valve */
	0,							/* remote interface */
	1,							/* remote server */
	0,
	0,
	0,							/* pipe interface */
	1,							/* pipe server */
	0,							/* central interface */
	1,							/* central server */
	1,							/* gateway */
};



/* this table describes the protocols that can be
   used for getting from a node of a given type */

#define	DECNET_PROTOCOL	 1		/* :: */
#define TCP_PROTOCOL	 2		/* : */
#define VMS_TCP_PROTOCOL 4		/* ^ */
#define APOLLO_PROTOCOL	 8		/* // */
#define MSLAN_PROTOCOL	16		/* \\ */

static USHORT protocols_supported[] =
{
	0,
	DECNET_PROTOCOL | VMS_TCP_PROTOCOL,	/* "Rdb/VMS" */
	DECNET_PROTOCOL | VMS_TCP_PROTOCOL,	/* "Rdb/ELN target" */
	DECNET_PROTOCOL | VMS_TCP_PROTOCOL,	/* "Rdb/ELN development" */
	DECNET_PROTOCOL | VMS_TCP_PROTOCOL,	/* "Rdb/VMS Y" */
	DECNET_PROTOCOL | VMS_TCP_PROTOCOL,	/* "Rdb/ELN Y" */
	DECNET_PROTOCOL | VMS_TCP_PROTOCOL,	/* "JRI" */
	DECNET_PROTOCOL | VMS_TCP_PROTOCOL,	/* "JSV" */
	0,
	0,
	TCP_PROTOCOL | APOLLO_PROTOCOL,		/* "InterBase/apollo" */
	DECNET_PROTOCOL | TCP_PROTOCOL,		/* "InterBase/ultrix" */
	DECNET_PROTOCOL | VMS_TCP_PROTOCOL,	/* "InterBase/vms" */
	TCP_PROTOCOL,						/* "InterBase/sun" */
	TCP_PROTOCOL,						/* "InterBase/OS2" */
	0,									/* 15 */
	0,									/* 16 */
	0,									/* 17 */
	0,									/* 18 */
	0,									/* 19 */
	0,									/* 20 */
	0,									/* 21 */
	0,									/* 22 */
	0,									/* 23 */
	0,									/* 24 */
	TCP_PROTOCOL | APOLLO_PROTOCOL,		/* "InterBase/apollo" */
	DECNET_PROTOCOL | TCP_PROTOCOL,		/* "InterBase/ultrix" */
	DECNET_PROTOCOL | VMS_TCP_PROTOCOL,	/* "InterBase/vms" */
	TCP_PROTOCOL,						/* "InterBase/sun" */
	TCP_PROTOCOL,						/* "InterBase/OS2" */
	TCP_PROTOCOL,						/* "InterBase/sun4" */
	TCP_PROTOCOL,						/* "InterBase/hpux" */
	TCP_PROTOCOL,						/* "InterBase/sun386" */
	DECNET_PROTOCOL | VMS_TCP_PROTOCOL,	/* "InterBase:ORACLE/vms" */
	TCP_PROTOCOL,						/* "InterBase/mac/aux" */
	TCP_PROTOCOL,						/* "InterBase/ibm/rt" */
	DECNET_PROTOCOL | TCP_PROTOCOL,		/* "InterBase/mips/ultrix" */
};

#endif /* ALICE_INFO_H */
