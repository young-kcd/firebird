/*
 *	PROGRAM:	Security data base manager
 *	MODULE:		gsec.h
 *	DESCRIPTION:	Header file for the GSEC program
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

#ifndef _UTILITIES_GSEC_H_
#define _UTILITIES_GSEC_H_

#include "../jrd/ibsetjmp.h"
#include "../jrd/thd.h"
#include "../jrd/thd_proto.h"

#if (defined WIN_NT)
#include <io.h>
#endif

#define GSEC_MSG_FAC	18
#define MSG_LENGTH	128
#define QUERY_LENGTH	256
 
/* structure to hold information from the command line, including the
   operation to perform and any parameters entered (sizes are determined
   by the size of the fields in the USERS relation in USERINFO.GDB) */

#define ADD_OPER	1
#define DEL_OPER	2
#define DIS_OPER	3
#define MOD_OPER	4
#define QUIT_OPER	5
#define HELP_OPER	6
#define VERSION_OPER	7

#define USER_NAME_LEN 133
#define ALT_NAME_LEN  129
#define NAME_LEN       33

typedef int (DLL_EXPORT* OUTPUTPROC) (SLONG, UCHAR *);

#ifdef GET_THREAD_DATA
#undef GET_THREAD_DATA
#endif

#if !(defined REMOTE_REMOTE_H || defined JRD_JRD_H)
#ifndef INCLUDE_FB_BLK
#include "../include/fb_blk.h"
#endif
#endif

#include "../jrd/svc.h"
#include "../jrd/svc_proto.h"

#ifdef SET_THREAD_DATA
#undef SET_THREAD_DATA
#endif
#ifdef RESTORE_THREAD_DATA
#undef RESTORE_THREAD_DATA
#endif
#ifdef SUPERSERVER
#define GET_THREAD_DATA	        ((TSEC) THD_get_specific())
#define SET_THREAD_DATA         THD_put_specific ((THDD) tdsec); \
				tdsec->tsec_thd_data.thdd_type = THDD_TYPE_TSEC
#define RESTORE_THREAD_DATA     THD_restore_specific();
#else
#define GET_THREAD_DATA	        (gdsec)
#define SET_THREAD_DATA         gdsec = tdsec; \
				tdsec->tsec_thd_data.thdd_type = THDD_TYPE_TSEC
#define RESTORE_THREAD_DATA     
#endif

typedef struct user_data {
    int		operation;		/* what's to be done */
    TEXT	user_name [USER_NAME_LEN];	/* the user's name */
    BOOLEAN	user_name_entered;	/* user name entered flag */
    int		uid;			/* the user's id */
    BOOLEAN	uid_entered;		/* UID entered flag */
    BOOLEAN	uid_specified;		/* UID specified flag */
    int		gid;			/* the user's group id */
    BOOLEAN	gid_entered;		/* GID entered flag */
    BOOLEAN	gid_specified;		/* GID specified flag */
    TEXT	sys_user_name [ALT_NAME_LEN];	/* the sys_user's name */
    BOOLEAN	sys_user_entered;	/* sys_user entered flag */
    BOOLEAN	sys_user_specified;	/* sys_user specified flag */
    TEXT	group_name [ALT_NAME_LEN];	/* the group name */
    BOOLEAN	group_name_entered;	/* group_name entered flag */
    BOOLEAN	group_name_specified;	/* group_name specified flag */
    TEXT	password [NAME_LEN];		/* the user's password */
    BOOLEAN	password_entered;	/* password entered flag */
    BOOLEAN	password_specified;	/* password specified flag */
    TEXT	first_name [NAME_LEN];	/* the user's first name */
    BOOLEAN	first_name_entered;	/* first name entered flag */
    BOOLEAN	first_name_specified;	/* first name specified flag */
    TEXT	middle_name [NAME_LEN];	/* the user's middle name */
    BOOLEAN	middle_name_entered;	/* middle name entered flag */
    BOOLEAN	middle_name_specified;	/* middle name specified flag */
    TEXT	last_name [NAME_LEN];		/* the user's last name */
    BOOLEAN	last_name_entered;	/* last name entered flag */
    BOOLEAN	last_name_specified;	/* last name specified flag */
    TEXT	dba_user_name [USER_NAME_LEN];	/* the user's name */
    BOOLEAN	dba_user_name_entered;	/* user name entered flag */
    BOOLEAN	dba_user_name_specified;/* database specified flag */
    TEXT	dba_password [NAME_LEN];	/* the user's name */
    BOOLEAN	dba_password_entered;	/* user name entered flag */
    BOOLEAN	dba_password_specified;	/* database specified flag */
    TEXT	sql_role_name [NAME_LEN];	/* the user's name */
    BOOLEAN	sql_role_name_entered;	/* user name entered flag */
    BOOLEAN	sql_role_name_specified;/* database specified flag */
    TEXT	database_name [512];	/* database pathname */
    BOOLEAN	database_entered;	/* database entered flag */
    BOOLEAN	database_specified;	/* database specified flag */

} *USER_DATA;

typedef struct tsec {
    struct thdd		tsec_thd_data;
    struct user_data	*tsec_user_data;
    int			tsec_exit_code;
    jmp_buf*	tsec_env;
    ISC_STATUS		*tsec_status;
    ISC_STATUS_ARRAY	tsec_status_vector;
    BOOLEAN		tsec_interactive;
    BOOLEAN		tsec_sw_version;
    BOOLEAN		tsec_service_gsec;
    BOOLEAN		tsec_service_thd;
    OUTPUTPROC  	tsec_output_proc;
    SLONG       	tsec_output_data;
    IB_FILE        	*tsec_output_file;
    SVC			tsec_service_blk;
} *TSEC;

#define GsecMsg0	0	/* empty message */
#define GsecMsg1	1	/* "GSEC> "  (the prompt) */
#define GsecMsg2	2	/* gsec  (lower case version of the prompt for help display) */
#define GsecMsg3	3	/* ADD          add user */
#define GsecMsg4	4	/* DELETE       delete user */
#define GsecMsg5	5	/* DISPLAY      display user(s) */
#define GsecMsg6	6	/* MODIFY       modify user */
#define GsecMsg7	7	/* PW           user's password */
#define GsecMsg8	8	/* UID          user's ID */
#define GsecMsg9	9	/* GID          user's group ID */
#define GsecMsg10	10	/* PROJ         user's project name */
#define GsecMsg11	11	/* ORG          user's organization name */
#define GsecMsg12	12	/* FNAME        user's first name */
#define GsecMsg13	13	/* MNAME        user's middle name/initial */   
#define GsecMsg14	14	/* LNAME        user's last name */
#define GsecMsg15	15	/* gsec - unable to open database */
#define GsecMsg16	16	/* gsec - error in switch specifications */
#define GsecMsg17	17	/* gsec - no operation specified */
#define GsecMsg18	18	/* gsec - no user name specified */
#define GsecMsg19	19	/* gsec - add record error */ 
#define GsecMsg20	20	/* gsec - modify record error */
#define GsecMsg21	21	/* gsec - find/modify record error */
#define GsecMsg22	22	/* gsec - record not found for user: */
#define GsecMsg23	23	/* gsec - delete record error */
#define GsecMsg24	24	/* gsec - find/delete record error */
#define GsecMsg25	25	/* users defined for node */
#define GsecMsg26	26	/*     user name     uid   gid     project   organization       full name */
#define GsecMsg27	27	/* ---------------- ----- ----- ------------ ------------ -------------------- */
#define GsecMsg28	28	/* gsec - find/display record error */
#define GsecMsg29	29	/* gsec - invalid parameter, no switch defined */
#define GsecMsg30	30	/* gsec - operation already specified */
#define GsecMsg31	31	/* gsec - password already specified */
#define GsecMsg32	32	/* gsec - uid already specified */
#define GsecMsg33	33	/* gsec - gid already specified */
#define GsecMsg34	34	/* gsec - project already specified */
#define GsecMsg35	35	/* gsec - organization already specified */
#define GsecMsg36	36	/* gsec - first_name already specified */
#define GsecMsg37	37	/* gsec - middle_name already specified */
#define GsecMsg38	38	/* gsec - last_name already specified */
#define GsecMsg39	39	/* gsec version */
#define GsecMsg40	40	/* gsec - invalid switch specified */
#define GsecMsg41	41	/* gsec - ambiguous switch specified */
#define GsecMsg42	42	/* gsec - no operation specified for parameters */
#define GsecMsg43	43	/* gsec - no parameters allowed for this operation */
#define GsecMsg44	44	/* gsec - incompatible switches specified */
#define GsecMsg45	45	/* gsec utility - maintains user password database" */
#define GsecMsg46	46	/* command line usage: */
#define GsecMsg47	47	/* <command> [ <parameter> ... ] */
#define GsecMsg48	48	/* interactive usage: */
#define GsecMsg49	49	/* available commands: */
#define GsecMsg50	50	/* adding a new user: */
#define GsecMsg51	51	/* add <name> [ <parameter> ... ] */
#define GsecMsg52	52	/* deleting a current user: */
#define GsecMsg53	53	/* delete <name> */
#define GsecMsg54	54	/* displaying all users: */
#define GsecMsg55	55	/* display */
#define GsecMsg56	56	/* displaying one user: */
#define GsecMsg57	57	/* display <name> */
#define GsecMsg58	58	/* modifying a user's parameters: */
#define GsecMsg59	59	/* modify <name> <parameter> [ <parameter> ... ] */
#define GsecMsg60	60	/* help: */
#define GsecMsg61	61	/* ? (interactive only) */
#define GsecMsg62	62	/* help */
#define GsecMsg63	63	/* quit interactive session: */
#define GsecMsg64	64	/* quit (interactive only) */
#define GsecMsg65	65	/* available parameters: */
#define GsecMsg66	66	/* -pw <password> */
#define GsecMsg67	67	/* -uid <uid> */
#define GsecMsg68	68	/* -gid <uid> */
#define GsecMsg69	69	/* -proj <projectname> */
#define GsecMsg70	70	/* -org <organizationname> */
#define GsecMsg71	71	/* -fname <firstname> */
#define GsecMsg72	72	/* -mname <middlename> */
#define GsecMsg73	73	/* -lname <lastname> */
#define GsecMsg74	74	/* gsec - memory allocation error */
#define GsecMsg75	75	/* gsec error */
#define	GsecMsg76	76	/* invalid user name (maximum 31 bytes allowed) */
#define	GsecMsg77	77	/* invalid password (maximum 16 bytes allowed) */
#define GsecMsg78	78	/* gsec - database alrady specified */
#define GsecMsg79	79	/* gsec - database administrator name already specified */
#define GsecMsg80	80	/* gsec - database administrator password already specified */
#define GsecMsg81	81	/* gsec - SQL role name laready specified */
#define GsecMsg82	82	/* [ <options ... ] */
#define GsecMsg83	83	/* available options: */
#define GsecMsg84	84	/* -user <database administrator name> */
#define GsecMsg85	85	/* -password <database administrator password> */
#define GsecMsg86	86	/* -role <database administrator SQL role name> */
#define GsecMsg87	87	/* -database <security database> */
#define GsecMsg88	88	/* -z */
#define GsecMsg89	89	/* displaying version number: */
#define GsecMsg90	90	/* z (interactive only) */

#endif /* _UTILITIES_GSEC_H_ */
