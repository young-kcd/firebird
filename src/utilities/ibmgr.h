/*
 *	PROGRAM:	InterBase server manager
 *	MODULE:		ibmgr.h
 *	DESCRIPTION:	Header file for the IBMGR program
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
 * $Id: ibmgr.h,v 1.4 2003-02-10 11:48:23 dimitr Exp $
 */

#ifndef _UTILITIES_IBMGR_H_
#define _UTILITIES_IBMGR_H_


#if (defined WIN_NT)
#include <io.h>
#endif

#define OUTFILE			ib_stderr

#define SYSDBA_USER_NAME	"SYSDBA"
#define FIREBIRD_USER_NAME	"firebird"
#define INTERBASE_USER_NAME	"interbase"
#define INTERBASE_USER_SHORT	"interbas"
#define SERVER_GUARDIAN		"bin/fbguard"
#define USER_INFO_NAME		"security.fdb"

#define MSG_FAC		18
#define MSG_LEN		128

 
/* Basic operation definitions
*/
#define OP_NONE			0
#define OP_START		1
#define OP_SHUT			2
#define OP_SET			3
#define OP_SHOW			4
#define OP_QUIT			5
#define OP_HELP			6
#define OP_VERSION		7
#define OP_PRINT		8


/* Suboperation definitions
*/
#define SOP_NONE		0
#define SOP_START_ONCE		1
#define SOP_START_FOREVER	2
#define SOP_SHUT_NOW		3
#define SOP_SHUT_NOAT		4
#define SOP_SHUT_NOTR		5
#define SOP_SHUT_IGN		6
#define SOP_PRINT_POOL		7
#define SOP_START_SIGNORE       8
/* Flags that are used in "par_entered" field of the IBMGR_DATA.
*/
#define ENT_HOST	0x1
#define ENT_PASSWORD	0x2
#define ENT_USER	0x4


/* Flags that are used in "reattach" field of the IBMGR_DATA.
   Note: "par_entered" field is cleared each time we are going
	 to get a new command line, while the "reattach" field
	 is cleared only after new attachment to service is made
*/
#define REA_HOST	ENT_HOST
#define REA_PASSWORD	ENT_PASSWORD
#define REA_USER	ENT_USER

/* structure to hold all information
*/
typedef struct ibmgr_data_t {

    USHORT		operation;	/* what's to be done */
    USHORT		suboperation;	/* suboperation */
    USHORT		par_entered;	/* parameters that were entered */
    TEXT		host[128];	/* server's host */
    TEXT		user[128];	/* the user name */
    TEXT		real_user[128];	/* the os user name */
    TEXT		password[32];	/* user's passwd */
    BOOLEAN		shutdown;	/* shutdown is in progress */
    USHORT		reattach;	/* need to reattach because host,
					   passwd or user has been changed */
    isc_svc_handle	attached;	/* !=NULL if we attached to service */
    TEXT		print_file[MAXPATHLEN];/* Dump file name */

} IBMGR_DATA;

	
/* Messages tag definitions
*/
#define MSG_PROMPT	1	/* "IBMGR> "  (the prompt) */
#define MSG_VERSION	2	/* ibmgr version */

#define MSG_OPSPEC	5	/* operation already specified */
#define MSG_NOOPSPEC	6	/* no operation specified */
#define MSG_INVSW	7	/* invalid switch */
#define MSG_INVOP	8	/* invalid operation */
#define MSG_INVSWSW	9	/* invalid switch combination */
#define MSG_INVSWOP	10	/* invalid operation/switch combination */
#define MSG_AMBSW	11	/* ambiguous switch */
#define MSG_INVPAR	12	/* invalid parameter, no switch specified */
#define MSG_SWNOPAR	13	/* switch does not take any parameter */
#define MSG_SHUTDOWN	14	/* shutdown is in progress */
#define MSG_CANTSHUT	15	/* can not start another shutdown */
#define MSG_CANTQUIT	16	/* can not quit ibmgr now */
#define MSG_CANTCHANGE	17	/* can not change host, password or user */
#define MSG_WARNPASS	18	/* warning: only 8 significant bytes of password used */
#define MSG_INVUSER	19	/* invalid user (only 32 bytes allowed) */
#define MSG_REQPAR	20	/* switch requires parameter */
#define MSG_SYNTAX	21	/* syntax error in command line */

#define MSG_GETPWFAIL	25	/* can not get password entry */
#define MSG_ATTFAIL	26	/* can not attach to server */
#define MSG_SSHUTFAIL	27	/* can not start server shutdown */
#define MSG_SHUTOK	28	/* server shutdown completed */
#define MSG_STARTERR	29	/* can not start server */
#define MSG_STARTFAIL	30	/* can not start server */
#define MSG_SRVUP	31	/* server is alreary running */
#define MSG_SRVUPOK	32	/* server has been successfully started */
#define MSG_NOPERM	33	/* no permissions to perform operation */
#define MSG_PRPOOLFAIL	34	/* Failed to print pool info */
#define MSG_PRPOOLOK	35	/* Print pool successfull */
#define MSG_FLNMTOOLONG	36	/* FIle name too long */
 

#endif /* _UTILITIES_IBMGR_H_ */
