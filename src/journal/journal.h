/*
 *	PROGRAM:	JRD Journal Server
 *	MODULE:		journal.h
 *	DESCRIPTION:	
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

#ifndef JOURNAL_JOURNAL_H
#define JOURNAL_JOURNAL_H

#define JOURNAL_PATH_LENGTH     257
#define MSG_LENGTH		1024

#define GJRN_MSG_FAC		16

#define ARCHIVED		"archived"
#define ARCHIVE_PENDING		"pending"
#define NOT_ARCHIVED		"not archived"

#define LOG_DELETED		"deleted"
#define LOG_NOT_DELETED		"not deleted"

#define LOG_OPENED		"opened"
#define LOG_CLOSED		"closed"

#define DB_DISABLED		"disabled"
#define DB_ACTIVE		"active"
#define DB_INACTIVE		"inactive"

#define MEDIA_DISK		"disk"

#define NEED_ARCH		"needs archive"
#define NO_ARCH			"no archive"

#define DB_DELETE		"delete"
#define DB_RETAIN		"retain"

/* Define global switches */

extern bool sw_service_gjrn;

/* Define global console messages */

/* Commands */

enum cmd_t {
	cmd_none,
	cmd_disable,				/* disable database */
	cmd_drop,					/* drop database records */
	cmd_erase,					/* erase database records  for disabled databases */
	cmd_exit,
	cmd_help,					/* Get help */
	cmd_reset,					/* reset delete database */
	cmd_set,					/* set delete flag for database */
	cmd_shutdown,				/* Shut down journalling cleanly */
	cmd_status,					/* Report status */
	cmd_version,				/* Show version */
	cmd_archive					/* restart archive for a database */
};

struct cmds {
	TEXT *cmds_string;
	enum cmd_t cmds_command;
	int cmds_msg;
	TEXT *cmds_help;
};

/* Message codes */

#define JRN_DIR_PROMPT	91		/* enter journal directory name: */
#define CON_DISCONN	95			/* Console disconnected */
#define PROCESS_DISCONN	96		/* Process disconnect */
#define JRN_DISABL	97			/* Journaling disabled */
#define SHUTDOWN_REQ	98		/* Shutdown requested */
#define PS_OFF		99			/* Process sign_off */
#define PS_ON		100			/* Process sign_on */
#define JRN_ENABL	101			/* Journaling enabled */
#define JRN_START	102			/* Journal server started */
#define JRN_SHUTDOWN	103		/* Journal server shutdown */
#define RELEASE_ERR	108			/* Release connection failed. */
#define GEN_ERR		111			/* Error occurred during \"%s\" */
#define COMMDS		112			/* Commands are: */
#define CMD_ERR		116			/* Don't understand command \"%s\" */
#define JRN_MSG_ERR	123			/* don't recognize journal message %d */
#define CLOSE_ERR	136			/* Journal not properly closed */
#define DB_RESET	137			/* \tResetting database \"%s\" (%d) */
#define DOS_ERR1	139			/* DOS error code %ld */
#define CON_CONN	146			/* Console connected */
#define DOS_ERR2	173			/* Dos error %d */
#define DOS_ERR3	174			/* Dos error %s */
#define MBOX_MESS_ERR	175		/* Don't understand mailbox message type %d */
#define NT_ERR		211			/* Windows NT error %d */
#define CONSOLE_PROG	215		/* Journal server console program */
#define SERV_EOF	216			/* unexpected end of file from server */
#define MAILBOX_ERR	217			/* Can't open mailbox %s */
#define OPTION_PROMPT	218		/* enter journal option: */
#define DB_PROMPT	219			/* enter database name: */
#define BAK_DIR_PROMPT	220		/* enter backup directory name: */
#define JRN_VERSION	221			/* enter backup directory name: */
#define JRN_FILE_ERR	222		/* "couldn't open journal file \"%s\"-" */
#define JRN_FILE_OPEN	223		/* Journal file \"%s\" opened */
#define CLOSE_ARG	224			/* Optional close argument is a journal file sequence number */
#define SEQ_NUM_ERR	225			/* Journal file sequence number %ld does not exist */
#define SUB_SYS_ERR	226			/* unsupported journal sub-system version (expected %d, encountered %d) */
#define OUTFILE_ERR  227		/* no output file */
#define JRN_SUSPND	228			/* Journaling suspended */
#define OUTFILE		229			/* current output file: %s */
#define OUTFILE_QUEUE	230		/* Output files queued: */
#define KNOWN_DBS	231			/* Known databases and connections: */

#endif /* JOURNAL_JOURNAL_H */

