/*
 *	PROGRAM:	JRD Journal Server
 *	MODULE:		journal.e
 *	DESCRIPTION:	Journal server for intermediate file journalling
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
 * 2002.10.28 Sean Leyne - Code cleanup, removed obsolete "MPEXL" port
 *
 */

#include <stdio.h>
#include "../jrd/common.h"
#include "../jrd/y_ref.h"
#include "../jrd/ibase.h"

#ifdef VMS
#include <time.h>
#include <types.h>
#include <timeb.h>
#include rms
#include iodef
#include descrip
#include ssdef
#include jpidef
#define SYS_ERROR	isc_arg_vms
#endif

#ifndef VMS
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <sys/file.h>
#define SYS_ERROR	isc_arg_unix

extern int errno;
#endif

#ifndef JOURNAL_DB
#define JOURNAL_DB	"journal.fdb"
#endif

typedef struct blk {
	UCHAR blk_type;
	UCHAR blk_pool_id;
	USHORT blk_length;
} *BLK;

#include "../jrd/jrn.h"
#include "../jrd/license.h"

DATABASE DB = "journal.fdb";

extern struct tm *localtime();
extern UCHAR *gds__alloc();
extern DIR JIO_next_dir();

/* Commands */

enum cmd_t {
	cmd_none,
	cmd_que,					/* Que a file or device */
	cmd_close,					/* Close current file */
	cmd_shutdown,				/* Shut down journalling cleanly */
	cmd_suspend,				/* Suspend all journalling */
	cmd_resume,					/* Resume journalling */
	cmd_status,					/* Report status */
	cmd_help,					/* Get help */
	cmd_version,				/* Show version */
	cmd_print,					/* Print file header */
	cmd_dump,					/* Dump journal file */
	cmd_verbose,				/* yak it up */
	cmd_flush,					/* Flush the journal and suspend */
	cmd_quiet					/* mums the word */
};

typedef struct cmds {
	TEXT *cmds_string;
	enum cmd_t cmds_command;
	TEXT *cmds_help;
} CMDS;

static CMDS commands[] = {
	"QUEUE", cmd_que, "\tque a file/device for journaling",
	"CLOSE", cmd_close, "\tclose the current journal file/device",
	"STATUS", cmd_status, "\tdisplay status of journal server",
	"SHUTDOWN", cmd_shutdown, "\tshutdown journal server",
	"SUSPEND", cmd_suspend, "\tsuspend journaling",
	"RESUME", cmd_resume, "\tresume journaling after suspend",
	"HELP", cmd_help, "\tdisplay this text",
	"VERSION", cmd_version, "\tdisplay software version number",
	"PRINT", cmd_print, "\tprint state of journal file",
	"DUMP", cmd_dump, "\tdump contents of journal file",
	"VERBOSE", cmd_verbose, "\tprint activity",
	"QUIET", cmd_quiet, "\tdon't print activity",
	"FLUSH", cmd_flush, "\tflush journal and suspend journaling",
	NULL, cmd_none
};

/* Active database block */

typedef struct dbb {
	struct dbb *dbb_next;		/* Next block */
	USHORT dbb_generation;		/* Generation of database */
	USHORT dbb_id;				/* Journal file Id of database */
	USHORT dbb_use_count;		/* Active stream on database ??? */
	SLONG dbb_handle;			/* Intermediate journal file handle */
	TEXT dbb_filename[1];		/* Database filename */
} *DBB;

/* Journal data sinks */

typedef struct sink {
	struct sink *sink_next;		/* Next available sink, if any */
	int sink_channel;			/* Stream for output */
	USHORT sink_isi;			/* RMS internal stream id */
	USHORT sink_sequence;		/* Sequence number of journal item */
	USHORT sink_flags;			/* Misc garbage */
	USHORT sink_length;			/* File name length */
	TEXT sink_filename[1];		/* File name */
} *SINK;

#define sink_active	1			/* Sink is, or was, active */
#define sink_magtape	2		/* Sink is magtape */
#define sink_open	4			/* File is open */

static JRN init();
static int signal_quit();
static DBB databases, lookup_database(), make_database(), start_database(),
find_database();
static SINK create_sink();

static SINK sinks;
static USHORT state, max_handle, dump_position[2], sw_verbose,
	sw_flush, flush_position[2], batch_flag;
static ULONG record_number;
static UCHAR message_buffer[MAX_RECORD];
static DBB *dbb_handles;

#define state_shut_pending	1	/* Shutdown has been requested */
#define state_shutdown		2	/* Shutdown in progress */
#define state_suspend		4	/* Suspend operation */
#define state_initialize	8	/* Initializing operation */
#define state_reset		16		/* Reset after improper close */
#define state_status		32	/* Report status */


/* Misc VMS crud */

#ifdef VMS

#define SYS_INPUT	"sys$input"
#define EVENT_FLAG	15

static USHORT chan;

static struct FAB fab;
static struct RAB rab;
#endif


main(argc, argv)
	 int argc;
	 UCHAR *argv[];
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Open server mailbox, ready server database, and wait for something
 *	to happen.
 *
 **************************************/
	DBB database;
	SINK sink;
	UCHAR command_buffer[256];
	UCHAR **arg_end, *p, *prompt;
	JRN journal;
	USHORT wait, position[2];
	ISC_STATUS_ARRAY status_vector;
	SLONG sequence;
	SSHORT poll, n;

#ifdef VMS
	fab = cc$rms_fab;
	rab = cc$rms_rab;
	fab.fab$b_fac = FAB$M_GET | FAB$M_PUT;
	fab.fab$w_mrs = MAX_RECORD;
	fab.fab$b_rfm = FAB$C_VAR;
	fab.fab$b_shr = FAB$M_NIL;

	rab.rab$l_fab = &fab;
	rab.rab$l_rop = RAB$M_EOF;
#else

	signal(SIGQUIT, signal_quit);
	signal(SIGINT, signal_quit);

#endif

	journal = init(position, status_vector);
	dump_position[0] = position[0];
	dump_position[1] = position[1];

	put_message("Journal server started", NULL);
	state = state_status;

	for (arg_end = argv + argc; ++argv < arg_end;) {
		p = *argv;
		if (*p++ != '-')
			continue;
		while (*p)
			switch (*p++) {
			case 'z':
				printf("Journal server version %s\n", GDS_VERSION);
			}
	}

/* Check in with database */

	READY JOURNAL_DB;
	START_TRANSACTION;
	state |= state_initialize;

	FOR X IN JOURNAL state &= ~state_initialize;
        if (X.USE_COUNT) {
            printf("Journal not properly closed\n");
            FOR J IN JOURNAL_FILES WITH J.STATUS EQ "active"
                MODIFY J USING 
                    strcpy(J.STATUS, "closed");
                END_MODIFY;
            END_FOR;
            FOR D IN DATABASES WITH D.STATUS EQ "active" AND D.USE_COUNT NE 0
	            MODIFY D USING
                    printf("\tResetting database \"%s\" (%d)\n", D.FILENAME, D.ID);
                    strcpy(D.STATUS, "inactive");
                END_MODIFY;
	        END_FOR;
            state |= state_reset;
        }
        if (!strcmp(X.STATUS, "suspended"))
            state |= state_suspend;
        MODIFY X USING 
            X.USE_COUNT = 1;
        END_MODIFY;
	END_FOR;

    if (state & state_initialize) {
		STORE X IN JOURNAL 
            strcpy(X.STATUS, "active");
	        X.USE_COUNT = 1;
            X.ID = 0;
            X.SEQUENCE = 0;
            time_stamp(&X.DATE_INITIALIZED);
        END_STORE;
    }

    FOR J IN JOURNAL_FILES WITH
        J.STATUS EQ "active" OR J.STATUS EQ "queued"
        SORTED BY J.SEQUENCE 

        sink = create_sink(J.FILENAME, FALSE);
        sink->sink_sequence = J.SEQUENCE;
        if (!strcmp(J.STATUS, "active")) {
            sink->sink_flags |= sink_active;
            open_sink(sink);
        }
    END_FOR;

    FOR D IN DATABASES WITH D.STATUS EQ "active"
        database = make_database(D.FILENAME, strlen(D.FILENAME), D.HANDLE);
        database->dbb_id = D.ID;
	END_FOR;

	commit();

/* Sit in loop waiting for either command input or message from client */

	if (sinks)
		state |= state_suspend;

	wait = 0;
	sequence = -1;
	poll = 0;
	prompt = NULL;

	while (!(state & state_shutdown)) {
		if (state & state_shut_pending)
			break;

		while (sinks && !(sinks->sink_flags & sink_open)) {
			open_sink(sinks);
			commit();
		}

		if (state & state_status) {
			state &= ~state_status;
			report_status();
		}

		if (!sinks || (state & state_suspend)) {
			while (!(n = get_command(command_buffer, 30, "suspended: ")));
			if (n == -1)
				state |= state_shut_pending;
			else
				process_command(command_buffer, journal);
			prompt = "gltj> ";
		}
		else if (!poll && (n = get_command(command_buffer, wait, prompt))) {
			if (n == -1)
				state |= state_shut_pending;
			else {
				prompt = "gltj> ";
				process_command(command_buffer, journal);
				if (batch_flag)
					poll = 1;
			}
		}
		else if (n =
				 get_message(journal, message_buffer, sizeof(message_buffer),
							 position)) {
			prompt = NULL;
			if (--poll < 0)
				poll = 20;
			else if (batch_flag)
				poll = 1000;
			process_message(message_buffer, n);
			sequence = ((JRNH *) message_buffer)->jrnh_series;
			if (!(sequence % 1000))
				checkpoint(journal, position, sequence);
			wait = 0;
			dump_position[0] = position[0];
			dump_position[1] = position[1];
			if (sw_flush &&
				position[0] == flush_position[0] &&
				position[1] >= flush_position[1]) {
				sw_flush = FALSE;
				state |= state_suspend;
			}
		}
		else {
			prompt = NULL;
			poll = 0;
			if (sequence != -1) {
				checkpoint(journal, position, record_number);
				sequence = -1;
			}
			wait = 10;
			if (sw_flush) {
				sw_flush = FALSE;
				state |= state_suspend;
			}
		}
	}

/* Close out current file, if there is one */

	if (sinks && (sinks->sink_flags & sink_open))
		close_sink(sinks, "active");

/* Check out with database */

	FOR X IN JOURNAL 
       MODIFY X USING 
           X.USE_COUNT = 0;
	       X.RECORD_NUMBER = record_number;
       END_MODIFY;
	END_FOR;

	for (database = databases; database; database = database->dbb_next)
		if (database->dbb_use_count) {
			database->dbb_use_count = 0;
			set_use_count(database);
		}

	put_message("Journal server shutdown", NULL);

	COMMIT;
	FINISH;
}


ERR_post(stuff)
	 ISC_STATUS stuff;
{
/**************************************
 *
 *	E R R _ p o s t
 *
 **************************************
 *
 * Functional description
 *	Post an error sequence to the status vector.  Since an error
 *	sequence can, in theory, be arbitrarily lock, pull a cheap
 *	trick to get the address of the argument vector.
 *
 **************************************/
	ISC_STATUS *p, *q
	ISC_STATUS_ARRAY status_vector;
	int type;

/* Get the addresses of the argument vector and the status vector, and do
   word-wise copy. */

	p = status_vector;
	q = &stuff;

/* Copy first argument */

	*p++ = isc_arg_gds;
	*p++ = *((SLONG *) q)++;

/* Pick up remaining args */

	while (*p++ = type = *((int *) q)++)
		switch (type) {
		case isc_arg_gds:
			*p++ = *((SLONG *) q)++;
			break;

		case isc_arg_number:
		case isc_arg_vms:
		case isc_arg_unix:
		case isc_arg_string:
		case isc_arg_interpreted:
			*p++ = *q++;
			break;

		case isc_arg_cstring:
			*p++ = *((int *) q)++;
			*p++ = *q++;
			break;
		}

	gds__print_status(status_vector);
	exit(FINI_ERROR);
}


static UCHAR *alloc(size)
	 USHORT size;
{
/**************************************
 *
 *	a l l o c
 *
 **************************************
 *
 * Functional description
 *	Allocate a block.
 *
 **************************************/
	UCHAR *blk, *p;

	p = blk = gds__alloc((SLONG) size);
	do
		*p++ = 0;
	while (--size);

	return blk;
}


static checkpoint(journal, position, record_number)
	 JRN journal;
	 USHORT *position;
	 SLONG record_number;
{
/**************************************
 *
 *	c h e c k p o i n t
 *
 **************************************
 *
 * Functional description
 *	Checkpoint the journal.
 *
 **************************************/

#ifdef VMS
	sys$flush(&rab);
#endif

	JIO_truncate(journal, position, record_number);
}


static clear(block, size)
	 UCHAR *block;
	 USHORT size;
{
/**************************************
 *
 *	c l e a r
 *
 **************************************
 *
 * Functional description
 *	Clear a hunk of memory.
 *
 **************************************/

	if (size)
		do
			*block++ = 0;
		while (--size);
}



static close_sink(sink, sink_status)
	 SINK sink;
	 UCHAR *sink_status;
{
/**************************************
 *
 *	c l o s e _ s i n k
 *
 **************************************
 *
 * Functional description
 *	Open a data sink.
 *
 **************************************/
	SINK *ptr;
	USHORT length;

/* Close sink */

	if (sink->sink_flags & sink_open) {
#ifdef VMS
		fab.fab$w_ifi = sink->sink_channel;
		sys$close(&fab);
#endif

#ifndef VMS
		close(sink->sink_channel);
#endif
		sink->sink_flags &= ~sink_open;
	}

/* Check out with database */

	FOR X IN JOURNAL_FILES WITH X.SEQUENCE == sink->sink_sequence
        MODIFY X USING 
           strcpy(X.STATUS, sink_status);
	    END_MODIFY;
	END_FOR;

/* Take it out of list of sinks */

	for (ptr = &sinks; *ptr; ptr = &(*ptr)->sink_next)
		if (*ptr == sink) {
			*ptr = sink->sink_next;
			break;
		}
}


static commit()
{
/**************************************
 *
 *	c o m m i t
 *
 **************************************
 *
 * Functional description
 *	Commit the current transaction and start another.
 *
 **************************************/

	COMMIT;
	START_TRANSACTION;
}


static SINK create_sink(string, flag)
	 UCHAR *string;
	 USHORT flag;
{
/**************************************
 *
 *	c r e a t e _ s i n k
 *
 **************************************
 *
 * Functional description
 *	Create a new data sink, but don't open it (yet).
 *
 **************************************/
	USHORT n, l;
	SINK sink, *ptr;

/* Find the end of the sink chain */

	for (ptr = &sinks; *ptr; ptr = &(*ptr)->sink_next);

/* Allocate a block */

	l = strlen(string);
	*ptr = sink = (SINK) alloc(sizeof(struct sink) + l);
	sink->sink_length = l;
	strcpy(sink->sink_filename, string);

	if (!flag)
		return sink;

/* Check in with database */

    FOR X IN JOURNAL 
        MODIFY X USING 
           sink->sink_sequence = ++X.SEQUENCE;
	    END_MODIFY;
	END_FOR;

	STORE X IN JOURNAL_FILES 
        strcpy(X.FILENAME, sink->sink_filename);
	    X.SEQUENCE = sink->sink_sequence;
	    strcpy(X.STATUS, "queued");
	END_STORE 

    return sink;
}


static delete_database(database)
	 DBB database;
{
/**************************************
 *
 *	d e l e t e _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	We've lost interest in a database.  Get rid of its block.
 *
 **************************************/
	DBB *ptr;

/* Disconnect block from data structures */

	for (ptr = &databases; *ptr; ptr = &(*ptr)->dbb_next)
		if (*ptr == database) {
			*ptr = database->dbb_next;
			break;
		}

	free(database);
}


static delete_sink(sink)
	 SINK sink;
{
/**************************************
 *
 *	d e l e t e _ s i n k
 *
 **************************************
 *
 * Functional description
 *	Get rid of a sink block.
 *
 **************************************/
	SINK *ptr;

	for (ptr = &sinks; *ptr; ptr = &(*ptr)->sink_next)
		if (*ptr == sink) {
			*ptr = sink->sink_next;
			break;
		}

	free(sink);
}


static disable(msg, length)
	 JRNC *msg;
	 USHORT length;
{
/**************************************
 *
 *	d i s a b l e
 *
 **************************************
 *
 * Functional description
 *	Disable journalling for a database.
 *
 **************************************/
	DBB database;
	JRNR reply;
	UCHAR *p, *q;
	USHORT l;

	database = find_database(msg->jrnc_header.jrnh_handle);
	msg->jrnc_header.jrnh_handle = database->dbb_id;

/* Journal the message */

	write_data(database, msg, length);
	put_message("Journaling disabled", database);

	FOR D IN DATABASES WITH D.ID EQ database->dbb_id
	   MODIFY D USING 
           strcpy(D.STATUS, "disabled");
	   END_MODIFY;
	END_FOR;

	commit();

	delete_database(database);
}


static dump(journal)
	 JRN journal;
{
/**************************************
 *
 *	d u m p
 *
 **************************************
 *
 * Functional description
 *	Dump contents of journal.
 *
 **************************************/
	SSHORT n;

	while (n = JIO_read(journal, dump_position,
						message_buffer, sizeof(message_buffer)))
			print_record(message_buffer, n);
}


static enable(msg, length)
	 JRNC *msg;
	 USHORT length;
{
/**************************************
 *
 *	e n a b l e
 *
 **************************************
 *
 * Functional description
 *	Enable journalling for a database.
 *
 **************************************/
	DBB database;
	JRNR reply;
	UCHAR *p, *q;
	USHORT l;

/* If database already exists, wipe out old connections and start again */

	if (database = lookup_database(msg->jrnc_filename, msg->jrnc_length))
		delete_database(database);

/* Make up database block */

	database =
		start_database(msg->jrnc_filename, msg->jrnc_length,
					   msg->jrnc_header.jrnh_handle);

/* Journal the message */

	write_data(database, msg, length);
}


static error(filename, errno, string)
	 TEXT *filename;
	 int errno;
	 TEXT *string;
{
/**************************************
 *
 *	e r r o r
 *
 **************************************
 *
 * Functional description
 *	We've had an unexpected error -- punt.
 *
 **************************************/
	ISC_STATUS_ARRAY status_vector;
	ISC_STATUS *s;

	s = status_vector;
	*s++ = isc_arg_gds;
	*s++ = isc_io_error, *s++ = isc_arg_string;
	*s++ = (SLONG) string;
	*s++ = isc_arg_string;
	*s++ = (SLONG) filename;
	*s++ = SYS_ERROR;
	*s++ = errno;
	*s = 0;

	gds__print_status(status_vector);
}


static DBB find_database(handle)
	 SLONG handle;
{
/**************************************
 *
 *	f i n d _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Find database block from intermediate journal file handle.
 *
 **************************************/
	TEXT *p;
	DBB database;

	if (dbb_handles && handle < max_handle &&
		(database = dbb_handles[handle])) return database;

	p = "ORPHAN RECORDS";

	return start_database(p, strlen(p), handle);
}


static free(blk)
	 UCHAR *blk;
{
/**************************************
 *
 *	f r e e
 *
 **************************************
 *
 * Functional description
 *	Free a block.
 *
 **************************************/

	gds__free(blk);
}


#ifdef VMS
static get_command(buffer, wait, prompt)
	 TEXT *buffer;
	 USHORT wait;
	 SCHAR *prompt;
{
/**************************************
 *
 *	g e t _ c o m m a n d    ( V M S )
 *
 **************************************
 *
 * Functional description
 *	If a command is pending, gobble it up.  Return 1 if
 *	we got something, 0 if nothing was pending, and -1
 *	for end of file.
 *
 **************************************/
	SSHORT item, iosb[4], length;
	SLONG status, mode;
	struct dsc$descriptor string;

/* If this is the first time, get things moving */

	if (!chan && !batch_flag) {
		/* Determine if this is an interactive process.
		   If so, use qio.  Otherwise use fgets. */

		item = JPI$_MODE;
		status = lib$getjpi(&item, NULL, NULL, &mode, NULL, NULL);
		if ((status & 1) && mode != JPI$K_INTERACTIVE)
			batch_flag = TRUE;
		else {
			string.dsc$b_class = DSC$K_CLASS_S;
			string.dsc$b_dtype = DSC$K_DTYPE_T;
			string.dsc$w_length = sizeof(SYS_INPUT) - 1;
			string.dsc$a_pointer = SYS_INPUT;
			status = sys$assign(&string, &chan, 0, 0);
			if (!(status & 1))
				lib$signal(status);
		}
	}

	if (prompt) {
		fputs(prompt, stdout);
		fflush(stdout);
	}

	if (batch_flag) {
		/* If we're not running interactively, shut things down if
		   we encounter an input error or EOF. */

		if (!fgets(buffer, 256, stdin))
			return -1;
		length = strlen(buffer);
	}
	else {
		status = sys$qiow(EVENT_FLAG,	/* Event flag */
						  chan,	/* Channel */
						  IO$_READVBLK | IO$M_TIMED,	/* Function */
						  iosb,	/* IO status block */
						  NULL,	/* AST address */
						  NULL,	/* AST parameter */
						  buffer,	/* P1 (buffer) */
						  80,	/* P2 (length) */
						  wait,	/* P3 (virtual block) */
						  NULL, NULL, NULL);

		if (!(status & 1))
			lib$signal(status);

		/* If we got an error, give up in disgust */

		if (iosb[0] != SS$_TIMEOUT && iosb[0] != 1)
			lib$signal(iosb[0]);

		length = iosb[1];

		if (length && iosb[0] == SS$_TIMEOUT) {
			status = sys$qiow(EVENT_FLAG,	/* Event flag */
							  chan,	/* Channel */
							  IO$_READVBLK,	/* Function */
							  iosb,	/* IO status block */
							  NULL,	/* AST address */
							  NULL,	/* AST parameter */
							  buffer + length,	/* P1 (buffer) */
							  80,	/* P2 (length) */
							  0,	/* P3 (virtual block) */
							  NULL, NULL, NULL);
			length += iosb[1];
		}

		buffer[length] = 0;
	}

	return length;
}


static get_command_ast()
{
/**************************************
 *
 *	g e t _ c o m m a n d _ a s t
 *
 **************************************
 *
 * Functional description
 * 	Read has completed, do something.
 *
 **************************************/

	sys$wake(0, 0);
}
#endif


#ifndef VMS
static get_command(buffer, wait, prompt)
	 TEXT *buffer;
	 USHORT wait;
	 SCHAR *prompt;
{
/**************************************
 *
 *	g e t _ c o m m a n d	( U N I X )
 *
 **************************************
 *
 * Functional description
 *	If a command is pending, gobble it up.  Return 1 if
 *	we got something, 0 if nothing was pending, and -1
 *	for end of file.
 *
 **************************************/
	TEXT *p;
	int c, reads, writes, excepts;
	struct timeval timeout;

	if (prompt) {
		fputs(prompt, stdout);
		fflush(stdout);
	}

	timeout.tv_sec = wait;
	timeout.tv_usec = 0;
	reads = 1;
	writes = 0;
	excepts = 0;
	select(5, &reads, &writes, &excepts, &timeout);
	p = buffer;

	if (!reads)
		return 0;

	while ((c = getchar()) != '\n')
		if (c == EOF)
			return -1;
		else
			*p++ = c;

	*p = 0;

	return p - buffer;
}
#endif


static get_message(journal, buffer, size, position)
	 JRN journal;
	 UCHAR *buffer;
	 USHORT size;
	 USHORT position[2];
{
/**************************************
 *
 *	g e t _ m e s s a g e
 *
 **************************************
 *
 * Functional description
 *	Get message if one is waiting.  If we get one, return length.
 *	If not, return 0.  If something goes wrong, return -1.
 *
 **************************************/

	return JIO_read(journal, position, buffer, size);
}


static JRN init(position, status_vector)
	 USHORT position[2];
	 ISC_STATUS *status_vector;
{
/**************************************
 *
 *	i n i t
 *
 **************************************
 *
 * Functional description
 *	Checkin with journal file.
 *
 **************************************/
	JRN journal;
	TEXT *p, *q, pwd[128];
	USHORT l;

#ifndef VMS
	LOCK_init(status_vector);
#endif

/* Allocate and initialize journal block, and open journal file */

	l = sizeof(struct jrn) + sizeof(JOURNAL_FILE);

#ifndef VMS
	getwd(pwd);
	l += strlen(pwd);
#endif

	journal = (JRN) gds__alloc((SLONG) l);
	clear(journal, l);
	p = journal->jrn_server;

#ifndef VMS
	for (q = pwd; *q;)
		*p++ = *q++;
	*p++ = '/';
#endif

	strcpy(p, JOURNAL_FILE);
	journal->jrn_status_vector = status_vector;

	JIO_open(journal);
	record_number = JIO_rewind(journal, position);

	return journal;
}


static DBB lookup_database(string, length)
	 TEXT *string;
	 USHORT length;
{
/**************************************
 *
 *	l o o k u p _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Find the database if it is active.
 *
 **************************************/
	DBB database;
	USHORT l;
	TEXT *p, *q;

	for (database = databases; database; database = database->dbb_next) {
		p = string;
		q = database->dbb_filename;
		for (l = length; l && *q; --l, p++, q++)
			if (*p != *q)
				break;
		if (!l && !*q)
			return database;
	}

	return NULL;
}


static DBB make_database(string, length, handle)
	 UCHAR *string;
	 USHORT length;
	 SLONG handle;
{
/**************************************
 *
 *	m a k e _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Make up a database block.
 *
 **************************************/
	DBB database, *temp;
	USHORT n, l;

/* Allocate database block */

	database = (DBB) alloc(sizeof(struct dbb) + length);
	database->dbb_next = databases;
	databases = database;

	strncpy(database->dbb_filename, string, length);

/* Checkin with database handle vector */

	if (!dbb_handles || handle >= max_handle) {
		n = (handle + 49) / 50 * 50;
		l = n * sizeof(DBB);
		temp = (DBB *) alloc(l);
		if (dbb_handles) {
			move(dbb_handles, temp, max_handle * sizeof(DBB));
			free(dbb_handles);
		}
		dbb_handles = temp;
		max_handle = n;
	}

	dbb_handles[handle] = database;

	return database;
}


static move(from, to, length)
	 UCHAR *from;
	 UCHAR *to;
	 USHORT length;
{
/**************************************
 *
 *	m o v e
 *
 **************************************
 *
 * Functional description
 *	Move some bytes.
 *
 **************************************/

	if (length)
		do
			*to++ = *from++;
		while (--length);
}


static open_sink(sink)
	 SINK sink;
{
/**************************************
 *
 *	o p e n _ s i n k
 *
 **************************************
 *
 * Functional description
 *	Open a data sink.
 *
 **************************************/
	USHORT length;
	TEXT s[128], *op, *p, *q;
	int status, flags, n;
	SLONG filenum, domain, recfm, access, exclusive, multi, lrecl,
		buffering, disposition, binary;

#ifdef VMS
	fab.fab$w_ifi = 0;
	fab.fab$l_fna = sink->sink_filename;
	fab.fab$b_fns = strlen(fab.fab$l_fna);

	if (sink->sink_flags & sink_active) {
		op = "sys$open";
		status = sys$open(&fab);
	}
	else {
		status = sys$create(&fab);
		op = "sys$create";
	}

	if (status & 1) {
		status = sys$connect(&rab);
		if (!(status & 1))
			sys$close(&fab);
		sink->sink_isi = rab.rab$w_isi;
	}

	sink->sink_channel = fab.fab$w_ifi;
#endif

#ifndef VMS
	flags =
		(sink->sink_flags & sink_active) ? O_WRONLY | O_APPEND : O_WRONLY |
		O_EXCL | O_CREAT;
	sink->sink_channel = open(sink->sink_filename, flags, 0x400);

	if (sink->sink_channel == -1)
		sink->sink_channel = 0;

	op = "open";
	status = errno;
#endif

    FOR X IN JOURNAL_FILES WITH X.SEQUENCE == sink->sink_sequence
	   if (!sink->sink_channel) 
           ERASE X
       else
	       MODIFY X USING 
               strcpy(X.STATUS, "active");
               if (!(sink->sink_flags & sink_active))
                   time_stamp(&X.DATE_OPENED);
               if (!X.RECORD_NUMBER)
                   X.RECORD_NUMBER = record_number;
           END_MODIFY;
	END_FOR;

	if (!sink->sink_channel) {
		error(sink->sink_filename, status, op);
		delete_sink(sink);
	}

	sprintf(s, "Journal file \"%s\" opened", sink->sink_filename);
	put_message(s, NULL);
	sink->sink_flags |= sink_open;
}


static print_header(journal)
	 JRN journal;
{
/**************************************
 *
 *	p r i n t _ h e a d e r
 *
 **************************************
 *
 * Functional description
 *	Dump a journal file.
 *
 **************************************/
	DIR dir, end;
	TEXT id_space[ID_SPACE], buffer[80], *p, *yellow;
	USHORT n;
	struct jfh header;

	if (!JIO_read_header
		(journal, &header, sizeof(header), id_space, sizeof(id_space))) {
		printf("Can't read journal header\n");
		return;
	}

	printf("Journal file \"%s\":\n", journal->jrn_server);
	printf("\tAllocate = %d, id space used = %d, next id = %d\n",
		   header.jfh_allocation, header.jfh_id_space, header.jfh_next_id);
	printf("\tData slot = %d, free slot = %d, sequence = %ld\n",
		   header.jfh_data, header.jfh_free, header.jfh_sequence);
	printf("\tLast copied = %d, last written = %d, series = %ld\n",
		   header.jfh_copy_block, header.jfh_write_block, header.jfh_series);

	printf("    Data segments:\n");
	for (n = 0; n <= header.jfh_data; n++)
		printf("\t%d. %d\n", n, header.jfh_segments[n]);

	printf("    Free segments:\n");
	for (n = header.jfh_free; n < SEGMENT_SLOTS; n++)
		printf("\t%d. %d\n", n, header.jfh_segments[n]);

	printf("    Enabled databases:\n");

	for (dir = (DIR) id_space, end = (DIR) (id_space + header.jfh_id_space);
		 dir < end; dir = JIO_next_dir(dir))
		printf("\t%d: %.*s\n", dir->dir_id, dir->dir_length, dir->dir_name);

	printf("\n");
}


static print_record(msg, length)
	 JRNH *msg;
	 USHORT length;
{
/**************************************
 *
 *	p r i n t _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *	Write some nice output data.
 *
 **************************************/
	TEXT *type, temp[128];
	JRND *data;

	if (length & 1)
		++length;

/* Map journal handle into database journal id */

	type = temp;

	switch (msg->jrnh_type) {
	case JRN_ENABLE:
		type = "ENABLE";
		break;
	case JRN_DISABLE:
		type = "DISABLE";
		break;
	case JRN_SIGN_ON:
		type = "SIGN_ON";
		break;
	case JRN_SIGN_OFF:
		type = "SIGN_OFF";
		break;
	case JRN_ON_LINE:
		type = "ON_LINE";
		break;
	case JRN_OFF_LINE:
		type = "OFF_LINE";
		break;
	case JRN_SYNC:
		type = "SYNC";
		break;
	case JRN_ACK:
		type = "ACK";
		break;
	case JRN_RESPONSE:
		type = "RESPONSE";
		break;
	case JRN_SET_TRANSACTION:
		type = "SET_TRANSACTION";
		break;
	case JRN_DUMMY:
		type = "DUMMY";
		break;
	case JRN_COMMIT:
		type = "COMMIT";
		break;

	case JRN_PAGE:
		data = (JRND *) msg;
		sprintf(temp, "PAGE %ld", data->jrnd_page);
		break;

	default:
		sprintf(temp, "unknown type %d", msg->jrnh_type);
	}

	printf("Record: type %s, length %d, series %d\n",
		   type, msg->jrnh_length, msg->jrnh_series);
}


static process_command(string, journal)
	 TEXT *string;
	 JRN journal;
{
/**************************************
 *
 *	p r o c e s s _ c o m m a n d
 *
 **************************************
 *
 * Functional description
 *	Process an operator command.
 *
 **************************************/
	CMDS *command;
	TEXT cmd[32], operand1[64], operand2[64], *p, *q;
	USHORT n, l;
	SINK sink, *ptr;

	n = sscanf(string, "%s%s%s", cmd, operand1, operand2);

	for (command = commands; p = command->cmds_string; command++) {
		q = cmd;
		while (*q && UPPER(*q) == *p++)
			q++;
		if (!*q)
			break;
	}

	switch (command->cmds_command) {
	case cmd_close:
		if (n == 1 && sinks)
			close_sink(sinks, "closed");
		else if (n == 2)
			if (!(n = atoi(operand1))) {
				printf
					("Optional close argument is a journal file sequence number");
				break;
			}
			else {
				for (sink = sinks; sink; sink = sink->sink_next)
					if (sink->sink_sequence == n) {
						close_sink(sink, "unused");
						break;
					}
				if (!sink) {
					printf("Journal file sequence number %ld does not exist",
						   (SLONG) n);
					break;
				}
			}
		commit();
		state |= state_status;
		return;

	case cmd_que:
		if (n != 2)
			break;
		create_sink(operand1, TRUE);
		commit();
		return;

	case cmd_resume:
	    FOR X IN JOURNAL 
            MODIFY X USING 
               strcpy(X.STATUS, "active");
	        END_MODIFY;
	    END_FOR;
		commit();
		state &= ~state_suspend;
		state |= state_status;
		return;

	case cmd_shutdown:
		state |= state_shut_pending;
		return;

	case cmd_status:
		report_status();
		return;

	case cmd_suspend:
        FOR X IN JOURNAL 
            MODIFY X USING 
                strcpy(X.STATUS, "suspended");
            END_MODIFY;
        END_FOR;
		state |= state_suspend | state_status;
		commit();
		return;

	case cmd_help:
		printf("Commands are:\n");
		for (command = commands; p = command->cmds_string; command++)
			printf("\t%s%\t%s\n", command->cmds_string, command->cmds_help);
		return;

	case cmd_version:
		printf("Journal server version %s\n", GDS_VERSION);
		return;

	case cmd_print:
		print_header(journal);
		return;

	case cmd_dump:
		dump(journal);
		return;

	case cmd_verbose:
		sw_verbose = TRUE;
		return;

	case cmd_quiet:
		sw_verbose = FALSE;
		return;

	case cmd_flush:
		sw_flush = TRUE;
		JIO_get_position(journal, flush_position);
		return;
	}

	fprintf(stderr, "Don't understand command \"%s\"\n", string);
}


static process_message(msg, length)
	 JRNH *msg;
	 USHORT length;
{
/**************************************
 *
 *	p r o c e s s _ m e s s a g e
 *
 **************************************
 *
 * Functional description
 *	Process a journal message.
 *
 **************************************/

	if (sw_verbose)
		print_record(msg, length);

	switch (msg->jrnh_type) {
	case JRN_ENABLE:
		enable(msg, length);
		break;

	case JRN_DISABLE:
		disable(msg, length);
		break;

	case JRN_PAGE:
	case JRN_COMMIT:
		write_data(find_database(msg->jrnh_handle), msg, length);
		break;

	case JRN_SIGN_ON:
		sign_on(msg, length);
		break;

	case JRN_SIGN_OFF:
		sign_off(msg, length);
		break;

	case JRN_SYNC:
	case JRN_DUMMY:
		break;

	default:
		fprintf(stderr, "don't recognize journal message %d\n",
				msg->jrnh_type);
	}
}


static put_message(message, database)
	 UCHAR *message;
	 DBB database;
{
/**************************************
 *
 *	p u t _ m e s s a g e
 *
 **************************************
 *
 * Functional description
 *	Write out informational message.
 *
 **************************************/
	SLONG clock;
	struct tm times;
	TEXT s[128];

	clock = time(NULL);
	times = *localtime(&clock);

	if (database)
		sprintf(s, " for \"%s\" (%d)", database->dbb_filename,
				database->dbb_id);
	else
		s[0] = 0;

	printf("%.2d:%.2d:%.2d %.2d/%.2d/%.2d %s%s\n",
		   times.tm_hour, times.tm_min, times.tm_sec,
		   times.tm_mon + 1, times.tm_mday, times.tm_year, message, s);

	fflush(stdout);
}


static report_status()
{
/**************************************
 *
 *	r e p o r t _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Report current state of journalling sub-system.
 *
 **************************************/
	DBB database;
	SINK sink;

/* Report on current state */

	if (!sinks)
		printf("Journaling disabled; no output file\n");
	else if (state & state_suspend)
		printf("Journaling suspended; current output file: %s\n",
			   sinks->sink_filename);
	else
		printf("Journaling enabled; current output file: %s\n",
			   sinks->sink_filename);

/* Report on files queued */

	if (sinks && sinks->sink_next) {
		printf("Output files queued:\n");
		for (sink = sinks->sink_next; sink; sink = sink->sink_next)
			printf("\t%s\n", sink->sink_filename);
	}

/* Report on active databases */

	if (databases) {
		printf("Known databases:\n");
		for (database = databases; database; database = database->dbb_next)
			printf("\t%s (%d)\n", database->dbb_filename, database->dbb_id);
	}
}


static set_use_count(database)
	 DBB database;
{
/**************************************
 *
 *	s e t _ u s e _ c o u n t
 *
 **************************************
 *
 * Functional description
 *	Set "use_count" for database.  This is called only for significant
 *	use count transitions.
 *
 **************************************/

   FOR D IN DATABASES WITH
       D.STATUS EQ "active" AND D.FILENAME EQ database->dbb_filename
       MODIFY D USING 
           D.USE_COUNT = database->dbb_use_count;
       END_MODIFY;
   END_FOR;

   commit();
}


static sign_off(msg, length)
	 JRNH *msg;
	 USHORT length;
{
/**************************************
 *
 *	s i g n _ o f f
 *
 **************************************
 *
 * Functional description
 *	Sign off a database.
 *
 **************************************/
	DBB database;
	UCHAR s[128];

	database = find_database(msg->jrnh_handle);
	put_message("Process sign_off", database);
}


static sign_on(msg, length)
	 JRNC *msg;
	 USHORT length;
{
/**************************************
 *
 *	s i g n _ o n
 *
 **************************************
 *
 * Functional description
 *	Sign a database process onto the journal system.
 *
 **************************************/
	DBB database, dbb;
	JRNR reply;
	UCHAR *p, *q;
	USHORT l;

/* If we already know about the database, this is easy */

	if (database = lookup_database(msg->jrnc_filename, msg->jrnc_length)) {
		put_message("Process sign_on", database);
		if (!database->dbb_use_count++)
			set_use_count(database);
		return;
	}

/* Make up database block */

	database = start_database(msg->jrnc_filename, msg->jrnc_length,
							  msg->jrnc_header.jrnh_handle);

/* Turn the "sign_on" into an "enable" and journal the message */

	msg->jrnc_header.jrnh_type = JRN_ENABLE;
	write_data(database, msg, length);
}


#ifndef VMS
static signal_quit()
{
/**************************************
 *
 *	s i g n a l _ q u i t
 *
 **************************************
 *
 * Functional description
 *	Stop whatever we happened to be doing.
 *
 **************************************/

	state |= state_shut_pending;
	signal(SIGQUIT, NULL);
	signal(SIGINT, NULL);
}
#endif


static DBB start_database(string, length, handle)
	 UCHAR *string;
	 SSHORT length;
	 SLONG handle;
{
/**************************************
 *
 *	s t a r t _ d a t a b a s e
 *
 **************************************
 *
 * Functional description
 *	Start journaling database.
 *
 **************************************/
	DBB database;

	database = make_database(string, length, handle);

/* Start by marking any other databases of same name as inactive */

	FOR D IN DATABASES WITH D.FILENAME EQ database->dbb_filename
	    MODIFY D USING 
           strcpy(D.STATUS, "inactive");
	    END_MODIFY;
	END_FOR;

/* Store new database record */

	STORE X IN DATABASES 
        strcpy(X.FILENAME, database->dbb_filename);
	    strcpy(X.STATUS, "active");
        X.USE_COUNT = 1;
        X.SEQUENCE_ENABLED = sinks->sink_sequence;
        X.HANDLE = handle;
        time_stamp(&X.DATE_ENABLED);
        FOR J IN JOURNAL 
            MODIFY J USING 
                X.ID = database->dbb_id = ++J.ID;
            END_MODIFY;
	   END_FOR;
	END_STORE;

	put_message("Journaling enabled", database);
	commit();

	return database;
}


static time_stamp(date)
	 SLONG *date;
{
/**************************************
 *
 *	t i m e _ s t a m p
 *
 **************************************
 *
 * Functional description
 *	Get the current time in gds format.
 *
 **************************************/
	SLONG clock;
	struct tm times;

	clock = time(NULL);
	times = *localtime(&clock);
	gds__encode_date(&times, date);
}


static write_data(database, msg, length)
	 DBB database;
	 JRNH *msg;
	 SLONG length;
{
/**************************************
 *
 *	w r i t e _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Write some nice output data.
 *
 **************************************/
	int status;
	UCHAR *p, *end;
	SSHORT errcode;

	msg->jrnh_series = ++record_number;

#ifdef VMS
	rab.rab$w_isi = sinks->sink_isi;
	rab.rab$l_rbf = msg;
	rab.rab$w_rsz = length;
	status = sys$put(&rab);

	if (!(status & 1)) {
		error(sinks->sink_filename, status, "sys$put");
		close_sink(sinks);
	}
#endif

#ifndef VMS
	write(sinks->sink_channel, msg, length);
#endif
}
