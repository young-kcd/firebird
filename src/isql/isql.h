/*
 *	PROGRAM:	Interactive SQL utility
 *	MODULE:		isql.h
 *	DESCRIPTION:	Component wide include file
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
 * $Id: isql.h,v 1.9.2.1 2005-04-01 07:48:24 robocop Exp $
 * Revision 1.2  2000/11/18 16:49:24  fsg
 * Increased PRINT_BUFFER_LENGTH to 2048 to show larger plans
 * Fixed Bug #122563 in extract.e get_procedure_args
 * Apparently this has to be done in show.e also,
 * but that is for another day :-)
 *
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */

#ifndef _ISQL_ISQL_H_
#define _ISQL_ISQL_H_

#include "../jrd/flags.h"
#include <stdlib.h>

/* Define lengths used in isql.e */

/* Increased this to allow display of somewhat lengthy plans
   hope this will last a year or so :-)
   FSG 17.Nov.2000 
*/
#define PRINT_BUFFER_LENGTH	2048
#define MAXTERM_LENGTH  	32	/* SQL termination character */
#define USER_LENGTH 		128
#define PASSWORD_LENGTH 	128
#define ROLE_LENGTH     	128

/* these constants are purely idiotic; there's no point in having 
   a predefined constant with no meaning, but that's Ed Simon the 
   master programmer for you! */

#define BUFFER_LENGTH128	128
#define BUFFER_LENGTH155	155
#define BUFFER_LENGTH256	256
#define BUFFER_LENGTH360	360
#define BUFFER_LENGTH400	400
#define BUFFER_LENGTH512	512
#define BUFFER_LENGTH80		 80
#define BUFFER_LENGTH60		 60
#define BUFFER_LENGTH120	120
#define BUFFER_LENGTH180	180

/* Define the possible states of processing commands */

#define CONT		0
#define EXIT		1
#define BACKOUT		2
#define ERR		3
#define END		4
#define SKIP		5
#define FAIL		6
#define EXTRACT		7
#define EXTRACTALL	8
#define FETCH		9

/* Which blob subtypes to print */

#define ALL_BLOBS	-2
#define NO_BLOBS	-1

/* Flag to decode all vs sql only objects */
#define SQL_objects	0
#define ALL_objects	1

/* Show source for triggers or not */
#define NO_SOURCE 	0
#define SHOW_SOURCE	1


#define WORDLENGTH	32
#define DEFTERM		";"
#define DEFCHARSET	"NONE"
#define NULL_DISP_LEN    6

#define NULL_PTR	((void*) 0)

/* Error codes */

#define MSG_LENGTH	1024
#define ISQL_MSG_FAC	17

#define GEN_ERR 	0			/* General non-zero SQLCODE error */
#define USAGE1 		1			/* Usage message 1 */
#define SWITCH 		2			/* Bad command line arg */
#define NO_DB 		3			/* No database specified */
#define FILE_OPEN_ERR	4		/* Can't open specified file */
#define COMMIT_PROMPT	5		/* Commit or rollback question */
#define COMMIT_MSG	6			/* Committing ... */
#define ROLLBACK_MSG	7		/* Rolling back */
#define	CMD_ERR		8			/* Unknown frontend command */
#define ADD_PROMPT	9			/* Prompt for add function */
#define VERSION		10			/* Version string for -z */
#define USAGE2 		11			/* Usage message 2 */
#define NUMBER_PAGES	12		/* Number of DB pages allocated = %d \n */
#define SWEEP_INTERV	13		/* Sweep interval = %d \n */
#define NUM_WAL_BUFF	14		/* Number of wal buffers = %d \n */
#define WAL_BUFF_SIZE	15		/* Wal buffer size = %d \n */
#define CKPT_LENGTH	16			/* Check point length = %d \n */
#define CKPT_INTERV	17			/* Check point interval = %d \n */
#define WAL_GRPC_WAIT	18		/* Wal group commit wait = %d \n */
#define BASE_LEVEL	19			/* Base level = %d \n */
#define LIMBO		20			/* Transaction in limbo = %d \n */
/* Help list */
#define HLP_FRONTEND	21		/* Frontend commands:\n */
#define HLP_BLOBVIEW	22		/* BLOBVIEW [<blobid as high:low>] -- edit a blob\n */
#define HLP_BLOBDMP	23			/* BLOBDUMP <blobid as high:low> <file> -- dump blob to a file\n */
#define HLP_EDIT	24			/* EDIT [<filename>] -- edit and read a SQL file\n\tWithout file name, edits current command buffer\n */
#define HLP_INPUT	25			/* INput <filename> -- enter a named SQL file\n */
#define HLP_OUTPUT	26			/* OUTput [<filename>] -- write output to named file\n */
#define HLP_SHELL	27			/* SHELL <shell command> -- execute command shell\n */
#define HLP_HELP	28			/* HELP -- Displays this menu\n */
#define HLP_SETCOM	29			/* "Set commands: " */
#define HLP_SET		30			/* \tSET -- Display current options \n */
#define HLP_SETAUTO	31			/* \tSET AUTOcommit  -- toggle autocommit of DDL statments\n */
#define HLP_SETBLOB	32			/* \tSET BLOBdisplay [ALL|N]-- Display blobs of type N\n\t SET BLOB turns off blob display\n */
#define HLP_SETCOUNT	33		/* \tSET COUNT  -- toggle count of selected rows on/off \n */
#define HLP_SETECHO	34			/* \tSET ECHO  -- toggle command echo on/off \n */
#define HLP_SETSTAT	35			/* \tSET STATs -- toggles performance statistics display\n */
#define HLP_SETTERM	36			/* \tSET TERM <string> -- changes termination character\n */
#define HLP_SHOW	37			/* SHOW <object type> [<object name>] -- displays information on metadata\n */
#define HLP_OBJTYPE	38			/* "  <object type> = 'DB', 'TABLE', 'PROCedure', 'INDEX',\n   'GRANT', 'DOMAIN', 'VERSION'\n" */
#define HLP_EXIT	39			/* EXIT -- Exit program and commit changes\n */
#define HLP_QUIT	40			/* QUIT -- Exit program and rollback changes\n\n */
#define HLP_ALL		41			/* All commands may be abbreviated to letters in CAPs\n */
#define HLP_SETSCHEMA	42		/* \tSET SCHema/DB <db name> -- changes current database\n */
#define YES_ANS		43			/* Yes */
#define REPORT1		44			/* Current memory = !c\nDelta memory = !d\nMax memory = !x\nElapsed time= !e sec\n */
#if (defined WIN_NT)
#define REPORT2		93			/* Buffers = !b\nReads = !r\nWrites = !w\nFetches = !f\n */
#else
#define REPORT2		45			/* Cpu = !u sec\nBuffers = !b\nReads = !r\nWrites = !w\nFetches = !f\n */
#endif
#define BLOB_SUBTYPE	46		/* Blob display set to subtype %d. This blob: subtype = %d\n */
#define BLOB_PROMPT	47			/* Blob: %s, type 'edit' or filename to load> */
#define DATE_PROMPT	48			/* Enter %s as M/D/Y> */
#define NAME_PROMPT	49			/* Enter %s> */
#define DATE_ERR	50			/* Bad date %s\n */
#define CON_PROMPT	51			/* "CON> " */
#define HLP_SETLIST	52			/* \tSET LIST -- toggles column or table display\n */
#define NOT_FOUND 	53			/* %s not found\n */
#define COPY_ERR 	54			/* Errors occured(possibly duplicate domains) in creating %s in %s\n" */
#define SERVER_TOO_OLD  55		/* Server version too old to support the isql command */
#define REC_COUNT 	56			/* Total returned: %ld */
#define	UNLICENSED	57			/* Unlicensed for database "%s" */
#define HLP_SETWIDTH	58		/* \tSET WIDTH <column name> [<width>] --sets/unsets print width for column name */
#define HLP_SETPLAN	59			/* Toggle display of query access plan */
#define HLP_SETTIME	60			/* Toggle display of timestamp with DATE values */
#define HLP_EDIT2	61			/* edits current command buffer\n */
#define HLP_OUTPUT2	62			/* \tWithout file name, returns output to ib_stdout\n */
#define HLP_SETNAMES	63		/* Set current character set */
#define HLP_OBJTYPE2	64		/* More objects */
#define HLP_SETBLOB2	65		/* \t SET BLOB turns off blob display\n */
#define HLP_SET_ROOT	66		/* (Use HELP SET for set options) */
#define	NO_TABLES	67			/* There are no tables in this database */
#define	NO_TABLE	68			/* There is no table %s in this database */
#define	NO_VIEWS	69			/* There are no views in this database */
#define	NO_VIEW		70			/* There is no view %s in this database */
#define	NO_INDICES_ON_REL 71	/* There are no indices on table %s in this database */
#define	NO_REL_OR_INDEX	72		/* There is no table or index %s in this database */
#define	NO_INDICES	73			/* There are no indices in this database */
#define	NO_DOMAIN	74			/* There is no domain %s in this database */
#define	NO_DOMAINS	75			/* There are no domains in this database */
#define	NO_EXCEPTION	76		/* There is no exception %s in this database */
#define	NO_EXCEPTIONS	77		/* There are no exceptions in this database */
#define	NO_FILTER	78			/* There is no filter %s in this database */
#define	NO_FILTERS	79			/* There are no filters in this database */
#define	NO_FUNCTION	80			/* There is no user-defined function %s in this database */
#define	NO_FUNCTIONS	81		/* There are no user-defined functions in this database */
#define	NO_GEN		82			/* There is no generator %s in this database */
#define	NO_GENS		83			/* There are no generators in this database */
#define	NO_GRANT_ON_REL	84		/* There is no privilege granted on table %s in this database */
#define	NO_GRANT_ON_PROC 85		/* There is no privilege granted on stored procedure %s in this database */
#define	NO_REL_OR_PROC	86		/* There is no table or stored procedure %s in this database */
#define	NO_PROC		87			/* There is no stored procedure %s in this database */
#define	NO_PROCS	88			/* There are no stored procedures in this database */
#define	NO_TRIGGERS_ON_REL 89	/* There are no triggers on table %s in this database */
#define	NO_REL_OR_TRIGGER 90	/* There is no table or trigger %s in this database */
#define	NO_TRIGGERS	91			/* There are no triggers in this database */
#define	NO_CHECKS_ON_REL 92		/* There are no check constraints on table %s in this database */
#define BUFFER_OVERFLOW  94		/* An isql command exceeded maximum buffer size */
#ifdef SCROLLABLE_CURSORS
#define HLP_SETFETCH	95		/* \tSET AUTOfetch  -- toggle autofetch of
								   records\n */
#endif

#define NO_ROLES		95		/* There are no roles in this database */
#define NO_REL_OR_PROC_OR_ROLE	96	/* There is no table, stored procedure, or
									   role %s in this database */
#define NO_GRANT_ON_ROL		97	/* There is no membership privilege granted
								   on %s in this database */
#define UNEXPECTED_EOF		98	/* Expected end of statement, encountered
								   EOF */
#define TIME_ERR		101		/* Bad TIME: %s\n */
#define HLP_OBJTYPE3	102		/* Some more objects */
#define NO_ROLE			103
#define USAGE3 			104		/* Usage message 3 */
#define INCOMPLETE_STR  105		/* Incomplete string in %s */
#define HLP_SETSQLDIALECT	106	/* \tSET SQL DIALECT <n>    -- set sql dialect to <n> */
#define NO_GRANT_ON_ANY 107 /* There is no privilege granted in this database. */
#define HLP_SETPLANONLY 108 /* Toggle display of query plan without executing */

#ifndef DEBUG
#define Trace		0
#endif

#ifdef DEBUG
#define Trace		1
#endif

#ifdef ISQL_MAIN
#define EXTERN
#else
#define EXTERN	extern
#endif

/* Utility transaction handle */

EXTERN isc_tr_handle M__trans;
EXTERN isc_tr_handle D__trans;
EXTERN isc_stmt_handle Stmt;
EXTERN IB_FILE *Ofp, *Out, *Ifp, *Errfp;
EXTERN SCHAR Term[MAXTERM_LENGTH];
EXTERN SCHAR Db_name[MAXPATHLEN];
EXTERN SCHAR Target_db[MAXPATHLEN];
EXTERN SCHAR User[128], Password[128], Role[128];
EXTERN SCHAR Numbufs[16];		/* # of cache buffers on connect */
EXTERN SSHORT Merge_stderr;
EXTERN USHORT SQL_dialect;
EXTERN USHORT db_SQL_dialect;
#undef EXTERN

typedef vary VARY;

typedef struct sqltypes {
	SSHORT type;
	SCHAR type_name[WORDLENGTH];
} SQLTYPES;

#define MAXSUBTYPES 8			/* Top of subtypes array */

#define SMALLINT	7
#define INTEGER		8
#define QUAD		9
#define FLOAT		10
#define FCHAR		14
#define DOUBLE_PRECISION 27
#define DATE		35
#define VARCHAR		37
#define CSTRING		40
#define BLOB_ID		45
#define BLOB		261

#ifdef VMS
#include <descrip.h>
#define LIB$_INPSTRTRU	0x15821c
#endif

#ifdef UNIX
#define UNIX_LINE	1
#endif

#if (defined WIN_NT)
#include <io.h>
#define UNIX_LINE	1
#define PC_FILE_SEEK
#endif

#ifdef SMALL_FILE_NAMES
#define SCRATCH		"fb_q"
#else
#define SCRATCH		"fb_query_"
#endif


#ifndef	STDERROUT
#define	STDERROUT(st,l)	{ ib_fprintf (Errfp, "%s", st);\
			  if (l) ib_fprintf (Errfp, "\n");\
			  ib_fflush (Errfp); }
#endif

#ifndef ISQL_ALLOC
#define ISQL_ALLOC(x)     gds__alloc (x)
#endif

#ifndef ISQL_FREE
#define ISQL_FREE(x)     {isc_free ((char*) x); x = NULL;}
#endif

#ifndef NEWLINE
#define NEWLINE 	"\n"
#endif

#define TAB_AS_SPACES "        "

#endif /* _ISQL_ISQL_H_ */
