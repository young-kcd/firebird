/*
 *	PROGRAM:	Central Server Interface
 *	MODULE:		css.c
 *	DESCRIPTION:	Central Servers services
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

#include "../jrd/ib_stdio.h"
#include "../csv/csi.h"
#include "../jrd/codes.h"
#include "../jrd/thd.h"
#include "../csv/css_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/isc_proto.h"

#ifdef UNIX
#include <errno.h>
#ifdef sun
#include <sys/mman.h>
#endif
#define SYS_ARG		gds_arg_unix
#endif

#ifdef PC_PLATFORM
#define SYS_ARG		gds_arg_dos
#endif

#ifdef WIN_NT
#define SYS_ARG		gds_arg_win32
#endif

#ifdef VMS
#include lckdef
#ifdef PIPE_SERVER
#include rms
#endif
#define SYS_ARG		gds_arg_vms

static LKSB CSS_lksb;
#endif

#define MAX_PROCESSES	16
#define EVENT_FLAG	15

extern SCHAR *getenv();

#ifdef VMS
static void acquire_ast(void);
#endif
static FRB alloc_global(USHORT, int);
#ifdef UNIX
static int alloc_semaphore(void);
#endif
static void cancel_timer(PRB);
static void delete_process(SLONG);
static void disconnect(SLONG, SLONG);
static void dump(void);
static void dump_que(UCHAR *, SRQ *);
#ifdef VMS
static void error(STATUS *, TEXT *, STATUS);
#endif
static void exit_handler(void *);
static void free_global(FRB);
static void init(void *, SH_MEM, int);
static void insert_tail(SRQ *, SRQ *);
static void mutex_bugcheck(UCHAR *, int);
static void punt(TEXT *);
static int put_message(PTR, MSG, int);
#ifdef UNIX
static void release_semaphore(USHORT);
#endif
static void remove_que(SRQ *);
static void set_timer(USHORT, PRB);

#ifdef PIPE_SERVER
#ifdef VMS
static void shutdown_section(void);
#endif
#endif	// PIPE_SERVER

#ifdef VMS
static void timeout_ast(PRB);
#endif

static USHORT acquire_count, resignal;
static CSH CSS_header, CSS_region;
static SLONG CSS_length, CSS_process;

#ifdef PIPE_SERVER
#ifdef VMS
static SH_MEM_T shmem_data;
#endif
#endif


CSH CSS_acquire(void)
{
/**************************************
 *
 *	C S S _ a c q u i r e
 *
 **************************************
 *
 * Functional description
 *	Acquire exclusive access to shared global region.
 *
 **************************************/
	STATUS status;
	int mutex_state;

	++acquire_count;

#ifdef VMS
	if (!CSS_lksb.lksb_status)
		ISC_wait(&CSS_lksb.lksb_status, EVENT_FLAG);

	status = sys$enq(EVENT_FLAG,	/* event flag */
					 LCK$K_EXMODE,	/* lock mode */
					 &CSS_lksb,	/* Lock status block */
					 LCK$M_CONVERT | LCK$M_SYNCSTS | LCK$M_NODLCKWT,	/* flags */
					 0,			/* resource name */
					 0,			/* parent id */
					 acquire_ast,	/* ast address */
					 0,			/* ast argument */
					 0,			/* blocking ast */
					 0,			/* access mode */
					 0);

	if (!(status & 1))
		lib$signal(status);

	if (!CSS_lksb.lksb_status)
		ISC_wait(&CSS_lksb.lksb_status, EVENT_FLAG);
#else

	if (mutex_state = ISC_mutex_lock(CSS_region->csh_mutex))
		mutex_bugcheck("mutex lock", mutex_state);

#endif

#ifdef DEBUG
	if (CSS_region->csh_current_process)
		punt("CSS_acquire: another process has region acquired");
#endif

	CSS_region->csh_current_process = CSS_process;
	CSS_header = CSS_region;

	if (CSS_header->csh_current_process != CSS_process)
		punt("CSS_acquire: lost acquisition");

	return CSS_header;
}


FRB CSS_alloc_message(USHORT type, int length)
{
/**************************************
 *
 *	C S S _ a l l o c _ m e s s a g e
 *
 **************************************
 *
 * Functional description
 *	Allocate a message in shared global region.  Leave the
 *	region acquired until the subsequent CSS_put_message.
 *
 **************************************/
	FRB block;

	ACQUIRE;
	block = alloc_global(type, length);

	return block;
}


BLK CSS_alloc_local(USHORT type, USHORT length)
{
/**************************************
 *
 *	C S S _ a l l o c _ l o c a l
 *
 **************************************
 *
 * Functional description
 *	Allocate a local piece of memory.
 *
 **************************************/
	BLK block;
	UCHAR *p, *end;

	block = (BLK) gds__alloc((SLONG) length);

	for (p = (UCHAR *) block, end = p + length; p < end;)
		*p++ = 0;

	block->blk_type = type;

	return block;
}


#ifdef GATEWAY
#ifndef PIPE_SERVER
int CSS_check_partner( PTR connection_id, USHORT process_flags)
{
/**************************************
 *
 *	C S S _ c h e c k _ p a r t n e r
 *
 **************************************
 *
 * Functional description
 *	Make sure the process flags of a connection
 *	match the specified flags.
 *
 **************************************/
	CNCT connection;
	PRB partner;
	USHORT result;

	ACQUIRE;
	connection = (CNCT) ABS_PTR(connection_id);

	if (!connection->cnct_partner) {
		RELEASE;
		return FALSE;
	}

	partner = (PRB) ABS_PTR(connection->cnct_partner);
	result =
		((partner->prb_flags & process_flags) ==
		 process_flags) ? TRUE : FALSE;
	RELEASE;

	return result;
}
#endif
#endif


PTR CSS_connect(SLONG proc_offset)
{
/**************************************
 *
 *	C S S _ c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Establish communication connection between client (us) and server.
 *
 **************************************/
	SLONG cnct_id;
	CNCT connection, mirror;
	PRB local, remote;

	ACQUIRE;

	local = (PRB) ABS_PTR(CSS_process);
	remote = (PRB) ABS_PTR(proc_offset);

	connection = (CNCT) alloc_global(type_cnct, sizeof(struct cnct));
	connection->cnct_parent = CSS_process;
	connection->cnct_partner = proc_offset;
	insert_tail(&local->prb_connections, &connection->cnct_connections);

	mirror = (CNCT) alloc_global(type_cnct, sizeof(struct cnct));
	mirror->cnct_parent = proc_offset;
	mirror->cnct_partner = CSS_process;
	mirror->cnct_mirror = REL_PTR(connection);
	insert_tail(&remote->prb_connections, &mirror->cnct_connections);

	connection->cnct_mirror = REL_PTR(mirror);
	cnct_id = REL_PTR(connection);
	RELEASE;

	return cnct_id;
}


PTR CSS_create_process(USHORT process_flag)
{
/**************************************
 *
 *	C S S _ c r e a t e _ p r o c e s s
 *
 **************************************
 *
 * Functional description
 *	Create process block unless it already exists.
 *
 **************************************/
	PRB process, server;
	SLONG semaphore;
	SRQ *que;
	UCHAR *p, *end;

	if (CSS_process)
		return (PTR) CSS_process;

	ACQUIRE;

#ifdef UNIX
	if ((semaphore = alloc_semaphore()) == -1)
		return NULL;
#else
	semaphore = 0;
#endif

	process = (PRB) alloc_global(type_prb, sizeof(struct prb));
	process->prb_flags = process_flag;
	process->prb_protocol_version = CSI_PROTOCOL_VERSION;
	process->prb_semaphore = semaphore;
	process->prb_process_number = ++CSS_header->csh_process_number;
	process->prb_process_id = getpid();

	insert_tail(&CSS_header->csh_processes, &process->prb_processes);
	QUE_INIT(process->prb_connections);
	QUE_INIT(process->prb_messages);
	ISC_event_init(process->prb_event, CSS_header->csh_semid,
				   process->prb_semaphore);
	CSS_header->csh_current_process = CSS_process = REL_PTR(process);
	RELEASE;

	return (PTR) CSS_process;
}


void CSS_disconnect( SLONG cnct_id)
{
/**************************************
 *
 *	C S S _ d i s c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Break one side of a connection.  If the other side
 *	is inactive, finish cleanup.
 *
 **************************************/

	ACQUIRE;
	disconnect(CSS_process, cnct_id);
	RELEASE;
}


void CSS_free_global( FRB block)
{
/**************************************
 *
 *	C S S _ f r e e _ g l o b a l
 *
 **************************************
 *
 * Functional description
 *	Free a previous allocated block.
 *
 **************************************/

	ACQUIRE;
	free_global(block);
	RELEASE;
}


void CSS_free_local( BLK block)
{
/**************************************
 *
 *	C S S _ f r e e _ l o c a l
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	gds__free(block);
}


PTR CSS_find_process( SLONG process_number)
{
/**************************************
 *
 *	C S S _ f i n d _ p r o c e s s
 *
 **************************************
 *
 * Functional description
 *	Find a process block based on a known number.  Since
 *	independent processes are at work, process blocks can
 *	disappear unexpectedly.  Looping through the blocks each
 *	time let's us know if this has happened.
 *
 **************************************/
	SRQ *que;
	PRB process;
	PTR offset;

	ACQUIRE;
	offset = 0;

	QUE_LOOP(CSS_header->csh_processes, que) {
		process = (PRB) ((UCHAR *) que - OFFSET(PRB, prb_processes));
		if ((process->prb_flags & PRB_server) &&
			process->prb_protocol_version == CSI_PROTOCOL_VERSION)
			if (process->prb_process_number == process_number) {
				offset = REL_PTR(process);
				break;
			}
	}

	RELEASE;

	return offset;
}


MSG CSS_get_message(PTR partner, MSG old_message, SSHORT timeout)
{
/**************************************
 *
 *	C S S _ g e t _ m e s s a g e
 *
 **************************************
 *
 * Functional description
 *	Get next available message.  If necessary, wait on event.  If
 *	a partner connection id is given, wait for message from that
 *	connection only; otherwise accept message from anybody.  If the
 *	timeout is negative, return immediately if no message is pending.
 *
 **************************************/
	PRB process;
	MSG message;
	CNCT connection;
	SRQ *que;
	SLONG count, status;
	EVENT events;

	if (timeout < 0 && acquire_count) {
		resignal = TRUE;
		return NULL;
	}

	ACQUIRE;

	if (old_message)
		free_global(old_message);

	process = (PRB) ABS_PTR(CSS_process);
	events = process->prb_event;

/* If a timeout period has been specified, establish a timer.
   A process block flag will be set if the timer goes off. 
   This method permits only one active timer per process block. */

	if (timeout > 0) {
		process->prb_flags &= ~PRB_timeout;
		set_timer(timeout, process);
	}

	for (;;) {
		process->prb_flags &= ~PRB_signal_me;
		QUE_LOOP(process->prb_messages, que) {
			message = (MSG) ((UCHAR *) que - OFFSET(MSG, msg_que));
			connection = (CNCT) ABS_PTR(message->msg_connection);
			if (!partner || connection->cnct_mirror == partner)
				goto got_one;
		}
		if (timeout < 0) {
			RELEASE;
			return NULL;
		}
		if (partner) {
			connection = (CNCT) ABS_PTR(partner);
			if (!connection->cnct_partner) {
				if (timeout > 0)
					cancel_timer(process);
				RELEASE;
				return NULL;
			}
		}
		if (timeout > 0 && process->prb_flags & PRB_timeout) {
			RELEASE;
			return NULL;
		}
		count = ISC_event_clear(process->prb_event);
		process->prb_flags |= PRB_signal_me;
		RELEASE;
		THREAD_EXIT;
		ISC_event_wait(1, &events, &count, -1, 0, 0);
		THREAD_ENTER;
		ACQUIRE;
		process = (PRB) ABS_PTR(CSS_process);
		events = process->prb_event;
	}

  got_one:
	message->msg_connection = connection->cnct_mirror;
	remove_que(&message->msg_que);

	if (timeout > 0)
		cancel_timer(process);

	RELEASE;

	return message;
}


#ifdef PIPE_SERVER
#ifdef GATEWAY
CSH CSS_init(STATUS * status_vector, USHORT server_flag, TEXT * filename)
{
/**************************************
 *
 *	C S S _ i n i t		( g a t e w a y _ p i p e _ s e r v e r )
 *
 **************************************
 *
 * Functional description
 *	Initialize for access to shared global region.  Return
 *	address of header if region exits, otherwise return NULL.
 *
 **************************************/
	TEXT csi_file[128], *p;
	int (*init_routine) ();
#ifndef VMS
	SH_MEM_T shmem_data;
#endif
	SLONG desc[2];
	STATUS status;

/* If we're already initialized, there's nothing to do */

	if (CSS_region)
		return CSS_region;

	init_routine = server_flag ? init : NULL;

	for (p = csi_file; *p = *filename++; p++);
	for (filename = ".%s"; *p++ = *filename++;);
	filename = csi_file;

#ifdef VMS
	shmem_data->sh_mem_system_flag = TRUE;
#endif
	if (!(CSS_header = ISC_map_file(status_vector,
									filename, init_routine, 0,
									CSI_DEFAULT_SIZE,
									&shmem_data))) return NULL;

	CSS_length = shmem_data.sh_mem_length_mapped;

#ifdef VMS
/* Strip off any device prefix. */

	for (p = filename; *p; p++)
		if (*p == ':' || *p == ']')
			filename = p + 1;
	ISC_make_desc(filename, desc, 0);

	status = sys$enqw(EVENT_FLAG, LCK$K_NLMODE, &CSS_lksb, LCK$M_NODLCKWT, &desc, NULL,	/* Lock parent (not used) */
					  0,		/* AST routine when granted */
					  0, 0, NULL, NULL);

	if (!(status & 1) || !((status = CSS_lksb.lksb_status) & 1)) {
		error(status_vector, "sys$enqw", status);
		return NULL;
	}
#endif

	CSS_region = CSS_header;
	gds__register_cleanup(exit_handler, 0);

	return CSS_header;
}
#endif


#ifndef GATEWAY
CSH CSS_init(STATUS * status_vector, USHORT server_flag, SSHORT id)
{
/**************************************
 *
 *	C S S _ i n i t		( p i p e _ s e r v e r )
 *
 **************************************
 *
 * Functional description
 *	Initialize for access to shared global region.  Return
 *	address of header if region exits, otherwise return NULL.
 *
 **************************************/
	TEXT *filename, csi_file[128], *p;
	int (*init_routine) ();
	SH_MEM_T shmem_data;
	SLONG desc[2];
	STATUS status;

/* If we're already initialized, there's nothing to do */

	if (CSS_region)
		return CSS_region;

	init_routine = server_flag ? init : NULL;

	if (!(filename = getenv("CSS_DEBUG")))
		filename = CSI_FILE;

#ifdef VMS
	shmem_data->sh_mem_system_flag = FALSE;
#endif
#ifdef UNIX
	shmem_data->sh_mem_semaphores = MAX_PROCESSES;
#endif
	if (!(CSS_header = ISC_map_file(status_vector,
									filename, init_routine, 0,
									CSI_DEFAULT_SIZE,
									&shmem_data))) return NULL;

	CSS_length = shmem_data.sh_mem_length_mapped;

#ifdef VMS
/* Strip off any device prefix. */

	for (p = filename; *p; p++)
		if (*p == ':' || *p == ']')
			filename = p + 1;
	ISC_make_desc(filename, desc, 0);

	status = sys$enqw(EVENT_FLAG, LCK$K_NLMODE, &CSS_lksb, LCK$M_SYSTEM | LCK$M_NODLCKWT, &desc, NULL,	/* Lock parent (not used) */
					  0,		/* AST routine when granted */
					  0, 0, NULL, NULL);

	if (!(status & 1) || !((status = CSS_lksb.lksb_status) & 1)) {
		error(status_vector, "sys$enqw", status);
		return NULL;
	}
#endif

	CSS_region = CSS_header;
	gds__register_cleanup(exit_handler, 0);

	return CSS_header;
}
#endif
#endif


#ifndef PIPE_SERVER
CSH CSS_init(STATUS * status_vector, USHORT server_flag)
{
/**************************************
 *
 *	C S S _ i n i t		( c e n t r a l _ s e r v e r )
 *
 **************************************
 *
 * Functional description
 *	Initialize for access to shared global region.  Return
 *	address of header if region exits, otherwise return NULL.
 *
 **************************************/
	TEXT *filename, *p;
	int (*init_routine) ();
	SH_MEM_T shmem_data;
	SLONG desc[2];
	STATUS status;

/* If we're already initialized, there's nothing to do */

	if (CSS_region)
		return CSS_region;

	init_routine = server_flag ? init : NULL;

	if (!(filename = getenv("CSS_DEBUG")))
		filename = CSI_FILE;

#ifdef VMS
	shmem_data.sh_mem_system_flag = FALSE;
#endif
#ifdef UNIX
	shmem_data.sh_mem_semaphores = MAX_PROCESSES;
#endif
	if (!(CSS_header = ISC_map_file(status_vector,
									filename, init_routine, 0,
									CSI_DEFAULT_SIZE,
									&shmem_data))) return NULL;

	CSS_length = shmem_data.sh_mem_length_mapped;

#ifdef VMS
/* Strip off any device prefix. */

	for (p = filename; *p; p++)
		if (*p == ':' || *p == ']')
			filename = p + 1;
	ISC_make_desc(filename, desc, 0);

	status = sys$enqw(EVENT_FLAG, LCK$K_NLMODE, &CSS_lksb, LCK$M_SYSTEM | LCK$M_NODLCKWT, &desc, NULL,	/* Lock parent (not used) */
					  0,		/* AST routine when granted */
					  0, 0, NULL, NULL);

	if (!(status & 1) || !((status = CSS_lksb.lksb_status) & 1)) {
		error(status_vector, "sys$enqw", status);
		return NULL;
	}
#endif

	CSS_region = CSS_header;
	gds__register_cleanup(exit_handler, 0);

	return CSS_header;
}
#endif


void CSS_probe_processes(void)
{
/**************************************
 *
 *	C S S _ p r o b e _ p r o c e s s e s
 *
 **************************************
 *
 * Functional description
 *	Probe to see if processes still exists.  If not, zap it.
 *
 **************************************/
	SRQ *que, *next;
	PRB process;
	PTR ptr;
	ULONG item;

	QUE_LOOP(CSS_header->csh_processes, que) {
		process = (PRB) ((UCHAR *) que - OFFSET(PRB, prb_processes));
		ptr = REL_PTR(process);
		if (ptr != CSS_process)
			if (!ISC_check_process_existence
				(process->prb_process_id, process->prb_group_id, FALSE)) {
				que = (SRQ *) ABS_PTR(que->srq_backward);
				delete_process(ptr);
			}
	}
}


int CSS_put_message( PTR connection_id, MSG message, MSG old_message)
{
/**************************************
 *
 *	C S S _ p u t _ m e s s a g e
 *
 **************************************
 *
 * Functional description
 *	Post a message (formatted) to a given processes msg que.
 *	Assume that the region has already been acquired by
 *	CSS_alloc_message.
 *
 **************************************/

	if (old_message)
		free_global(old_message);

	return put_message(connection_id, message, TRUE);
}


void CSS_release(void)
{
/**************************************
 *
 *	C S S _ r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release exclusive control of shared global region.
 *
 **************************************/
	STATUS status;
	int mutex_state;
	PRB process;
	SLONG pid, port;

#ifdef DEBUG_VALIDATE
	CSS_validate();
#endif

	if (!acquire_count || CSS_process
		&& CSS_header->csh_current_process !=
		CSS_process) punt("CSS_release: lost acquisition");

	CSS_header->csh_current_process = 0;

#ifndef MULTI_THREAD
/* In case we get (or have gotten) signaled, save our
   process id so we can signal ourselves */

	process = (PRB) ABS_PTR(CSS_process);
	pid = process->prb_process_id;
#endif

#ifdef VMS
	status = sys$enqw(0,		/* event flag */
					  LCK$K_NLMODE,	/* lock mode */
					  &CSS_lksb,	/* Lock status block */
					  LCK$M_CONVERT,	/* flags */
					  0,		/* resource name */
					  0,		/* parent id */
					  0,		/* ast address */
					  0,		/* ast argument */
					  0,		/* blocking ast */
					  0,		/* access mode */
					  0);

	if (!(status & 1))
		lib$signal(status);
#else

	if (mutex_state = ISC_mutex_unlock(CSS_header->csh_mutex))
		mutex_bugcheck("mutex unlock", mutex_state);

#endif

	CSS_header = (CSH) - 1;
	--acquire_count;

#ifndef GATEWAY
#ifndef MULTI_THREAD
	if (resignal) {
		resignal = FALSE;
		ISC_kill(pid, EVENT_SIGNAL);
	}
#endif
#endif
}


#ifdef DEBUG_VALIDATE
void CSS_validate(void)
{
/**************************************
 *
 *	C S S _ v a l i d a t e
 *
 **************************************
 *
 * Functional description
 *	Make sure everything looks ok.
 *
 **************************************/
	SLONG offset;
	HDR *block;
	PTR next_free;

/* Check consistency of global region (debugging only) */

	next_free = 0;

	for (offset = sizeof(struct csh); offset < CSS_header->csh_length;
		 offset += block->hdr_length) {
		block = (HDR *) ABS_PTR(offset);
		if (!block->hdr_length || !block->hdr_type
			|| block->hdr_type > type_msg) {
			punt("bad block length or type");
			break;
		}
		if (next_free)
			if (offset == next_free)
				next_free = 0;
			else if (offset > next_free)
				punt("bad free chain");
		if (block->hdr_type == type_frb) {
			next_free = ((FRB) block)->frb_next;
			if (next_free >= CSS_header->csh_length)
				punt("bad frb_next");
		}
	}

	if (offset != CSS_header->csh_length)
		punt("bad block length");
}
#endif


#ifdef VMS
static void acquire_ast(void)
{
/**************************************
 *
 *	a c q u i r e _ a s t
 *
 **************************************
 *
 * Functional description
 *	Completion ast for acquire.
 *
 **************************************/

	sys$wake(0, 0);
}
#endif


static FRB alloc_global( USHORT type, int length)
{
/**************************************
 *
 *	a l l o c _ g l o b a l
 *
 **************************************
 *
 * Functional description
 *	Allocate a block in shared global region.
 *
 **************************************/
	PTR *ptr, *best;
	FRB free;
	SLONG tail, best_tail;

	length = (length + 3) & ~3;
	best = NULL;

	for (ptr = &CSS_header->csh_free; (free = (FRB) ABS_PTR(*ptr)) && *ptr;
		 ptr = &free->frb_next) {
		tail = free->frb_header.hdr_length - length;
		if (tail >= 0 && (!best || tail < best_tail)) {
			best = ptr;
			best_tail = tail;
		}
	}

	if (!best) {
		RELEASE;
		gds__put_error("(CSS) alloc_global: space exhausted");
		abort(1);
	}

	free = (FRB) ABS_PTR(*best);

	if (best_tail < sizeof(struct frb))
		*best = free->frb_next;
	else {
		free->frb_header.hdr_length -= length;
		free = (FRB) ((UCHAR *) free + free->frb_header.hdr_length);
		free->frb_header.hdr_length = length;
	}

	free->frb_header.hdr_type = type;

	return free;
}


#ifdef UNIX
static int alloc_semaphore(void)
{
/**************************************
 *
 *	a l l o c _ s e m a p h o r e
 *
 **************************************
 *
 * Functional description
 *	Release a no-longer needed (or newly allocated) semaphore.
 *
 **************************************/
	SSHORT n, bit;
	UCHAR *byte;

	for (n = 0; n < MAX_PROCESSES; n++) {
		byte = CSS_header->csh_semaphores + (n >> 3);
		bit = 1 << (n & 7);
		if (*byte & bit) {
			*byte &= ~bit;
			return n;
		}
	}

	return -1;
}
#endif


#ifdef VMS
static void cancel_timer( PRB process)
{
/**************************************
 *
 *	c a n c e l _ t i m e r 	( V M S )
 *
 **************************************
 *
 * Functional description
 *	Cancel an unexpired timer.
 *
 **************************************/

	if (!(process->prb_flags & PRB_timeout))
		sys$cantim(process, 0);
}
#endif


#ifndef VMS
static void cancel_timer( PRB process)
{
/**************************************
 *
 *	c a n c e l _ t i m e r 	( g e n e r i c )
 *
 **************************************
 *
 * Functional description
 *	Cancel an unexpired timer.
 *
 **************************************/
}
#endif


static void delete_process( SLONG process_offset)
{
/**************************************
 *
 *	d e l e t e _ p r o c e s s
 *
 **************************************
 *
 * Functional description
 *	Delete a process block including friends and relations.
 *
 **************************************/
	PRB process;
	MSG message;

	process = (PRB) ABS_PTR(process_offset);

/* Eliminate all connections */

	while (!QUE_EMPTY(process->prb_connections))
		disconnect(process_offset,
				   process->prb_connections.srq_forward - OFFSET(CNCT,
																 cnct_connections));

/* Eliminating any messages */

	while (!QUE_EMPTY(process->prb_messages)) {
		message =
			(MSG) ABS_PTR(process->prb_messages.srq_forward -
						  OFFSET(MSG, msg_que));
		remove_que(&message->msg_que);
		free_global(message);
	}

#ifdef UNIX
/* Release semaphore */

	release_semaphore(process->prb_semaphore);
#endif

/* Untangle and release process block */

	remove_que(&process->prb_processes);
	free_global(process);

	if (CSS_process == process_offset)
		CSS_process = NULL;
}


static void disconnect( SLONG process_offset, SLONG cnct_id)
{
/**************************************
 *
 *	d i s c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Break one side of a connection.  If the other side
 *	is inactive, finish cleanup.
 *
 **************************************/
	CNCT connection, mirror;
	MSG message;
	PRB process, partner;
	SRQ *que;

	connection = (CNCT) ABS_PTR(cnct_id);
	mirror = (CNCT) ABS_PTR(connection->cnct_mirror);
	process = (PRB) ABS_PTR(process_offset);

/* Purge message que of any traffic from connection */

	QUE_LOOP(process->prb_messages, que) {
		message = (MSG) ((UCHAR *) que - OFFSET(MSG, msg_que));
		if (message->msg_connection == connection->cnct_mirror) {
			que = (SRQ *) ABS_PTR(que->srq_backward);
			remove_que(&message->msg_que);
			free_global(message);
		}
	}

/* Untangle the two connection blocks from our process block */

	remove_que(&connection->cnct_connections);

/* If the partner connection is still active, post a disconnect
   message and break our side of the connection */

	if (connection->cnct_partner) {
		partner = (PRB) ABS_PTR(connection->cnct_partner);
		partner->prb_flags |= PRB_signal_me;
		message = (MSG) alloc_global(type_msg, sizeof(struct msg));
		message->msg_type = MSG_disconnect;
		put_message(cnct_id, message, FALSE);
		mirror->cnct_partner = 0;
		connection->cnct_parent = 0;
	}
	else {
		free_global(connection);
		free_global(mirror);
	}
}


#ifdef DEBUG
static void dump(void)
{
/**************************************
 *
 *	d u m p
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	SRQ *que;
	HDR *block;
	PRB process;
	MSG message;
	FRB free;
	CNCT connection;
	SLONG offset;

	printf("%.5d GLOBAL REGION HEADER\n", 0);
	printf("\tLength: %ld, version: %d, free: %ld\n", CSS_region->csh_length,
		   CSS_region->csh_version, CSS_region->csh_free);
	printf("\tSemid: %ld, current process: %ld\n",
		   CSS_region->csh_semid, CSS_region->csh_current_process);
	dump_que("\tProcesses", &CSS_region->csh_processes);

	for (offset = sizeof(struct csh); offset < CSS_region->csh_length;
		 offset += block->hdr_length) {
		printf("\n%.5ld ", offset);
		block = (HDR *) ABS_PTR(offset);
		switch (block->hdr_type) {
		case type_prb:
			printf("PROCESS_BLOCK (%ld)\n", block->hdr_length);
			process = (PRB) block;
			printf("\tProcess_id: %ld, flags: %d, protocol: %d\n",
				   process->prb_process_id, process->prb_flags,
				   process->prb_protocol_version);
			printf("\tSemaphore: %d, group_id: %ld, number: %ld\n",
				   process->prb_semaphore, process->prb_group_id,
				   process->prb_process_number);
			dump_que("\tProcesses", &process->prb_processes);
			dump_que("\tConnections", &process->prb_connections);
			dump_que("\tMessages", &process->prb_messages);
			break;

		case type_frb:
			printf("FREE BLOCK (%ld)\n", block->hdr_length);
			free = (FRB) block;
			printf("\tNext: %ld\n", free->frb_next);
			break;

		case type_msg:
			printf("MSG (%ld)\n", block->hdr_length);
			message = (MSG) block;
			printf("\tType: %d, connection: %ld\n", message->msg_type,
				   message->msg_connection);
			dump_que("\tMsg que", &message->msg_que);
			break;

		case type_cnct:
			printf("CONNECTION (%ld)\n", block->hdr_length);
			connection = (CNCT) block;
			printf("\tParent: %ld, partner: %ld, mirror: %ld\n",
				   connection->cnct_parent, connection->cnct_partner,
				   connection->cnct_mirror);
			dump_que("\tConnections", &connection->cnct_connections);
			break;

		default:
			printf("*** UNKNOWN *** (%ld)\n", block->hdr_length);
			break;
		}
		if (!block->hdr_length)
			break;
	}

	return 1;
}


static void dump_que( UCHAR * string, SRQ * que)
{
/**************************************
 *
 *	d u m p _ q u e
 *
 **************************************
 *
 * Functional description
 *	Print the contents of a self-relative que.
 *
 **************************************/
	SLONG offset;

	offset = REL_PTR(que);

	if (offset == que->srq_forward && offset == que->srq_backward)
		printf("%s: *empty*\n", string);
	else
		printf("%s: forward: %d, backward: %d\n",
			   string, que->srq_forward, que->srq_backward);
}
#endif


#ifdef VMS
static void error( STATUS * status_vector, TEXT * string, STATUS status)
{
/**************************************
 *
 *	e r r o r 
 *
 **************************************
 *
 * Functional description
 *	We've encountered an error, report it.
 *
 **************************************/

	*status_vector++ = gds_arg_gds;
	*status_vector++ = gds__sys_request;
	*status_vector++ = gds_arg_string;
	*status_vector++ = (STATUS) string;
	*status_vector++ = SYS_ARG;
	*status_vector++ = status;
	*status_vector++ = gds_arg_end;
}
#endif



static void exit_handler( void *arg)
{
/**************************************
 *
 *	e x i t _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Cleanup on exit.
 *
 **************************************/

	if (CSS_process) {
		if (!acquire_count)
			ACQUIRE;
		delete_process(CSS_process);
	}

	while (acquire_count > 0)
		RELEASE;

#ifdef PIPE_SERVER
#ifdef VMS
	shutdown_section();
#endif
#endif

#ifdef UNIX
#ifndef MAP_TYPE
	shmdt(CSS_region);
#endif
#endif

	CSS_region = NULL;
}


static void free_global( FRB block)
{
/**************************************
 *
 *	f r e e _ g l o b a l
 *
 **************************************
 *
 * Functional description
 *	Free a previous allocated block.
 *
 **************************************/
	PTR *ptr, offset;
	FRB free, prior;

	prior = NULL;
	offset = REL_PTR(block);
	block->frb_header.hdr_type = type_frb;

	for (ptr = &CSS_header->csh_free; free = (FRB) ABS_PTR(*ptr);
		 prior = free, ptr = &free->frb_next)
		if (!*ptr) {
			free = NULL;
			break;
		}
		else if ((SCHAR HUGE_PTR *) block < (SCHAR HUGE_PTR *) free)
			break;

	if (offset <= 0 || offset > CSS_header->csh_length ||
		(prior
		 && (UCHAR HUGE_PTR *) block <
		 (UCHAR HUGE_PTR *) prior + prior->frb_header.hdr_length)) {
		punt("(CSS)free_global: bad block");
		return;
	}

/* Start by linking block into chain */

	block->frb_next = *ptr;
	*ptr = offset;

/* Try to merge free block with next block */

	if (free
		&& (SCHAR *) block + block->frb_header.hdr_length == (SCHAR *) free) {
		block->frb_header.hdr_length += free->frb_header.hdr_length;
		block->frb_next = free->frb_next;
	}

/* Next, try to merge the free block with the prior block */

	if (prior
		&& (SCHAR *) prior + prior->frb_header.hdr_length ==
		(SCHAR *) block) {
		prior->frb_header.hdr_length += block->frb_header.hdr_length;
		prior->frb_next = block->frb_next;
	}
}


static void init( void *arg, SH_MEM shmem_data, int initialize)
{
/**************************************
 *
 *	i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize global region header.
 *
 **************************************/
	USHORT n;
	UCHAR *p, *end;
	FRB free;
	int mutex_state;

	if (!initialize)
		return;

	p = (UCHAR *) shmem_data->sh_mem_address;
	for (end = p + sizeof(struct csh); p < end;)
		*p++ = 0;

	CSS_header = shmem_data->sh_mem_address;
	CSS_header->csh_length = shmem_data->sh_mem_length_mapped;
	CSS_header->csh_version = CSI_VERSION;
	CSS_header->csh_semid = shmem_data->sh_mem_mutex_arg;
	QUE_INIT(CSS_header->csh_processes);
	if (mutex_state =
		ISC_mutex_init(CSS_header->csh_mutex,
					   shmem_data->
					   sh_mem_mutex_arg)) mutex_bugcheck("mutex init",
														 mutex_state);

	free = (FRB) ((UCHAR *) CSS_header + sizeof(struct csh));
	free->frb_header.hdr_length =
		shmem_data->sh_mem_length_mapped - sizeof(struct csh);
	free->frb_header.hdr_type = type_frb;
	free->frb_next = 0;

	CSS_header->csh_free = (UCHAR *) free - (UCHAR *) CSS_header;

#ifdef UNIX
/* Initialize free semaphore bit vector for UNIX.  Semaphore
   0 is used implicitly for the mutex on the global section */

	for (n = 1; n < shmem_data->sh_mem_semaphores; n++)
		release_semaphore(n);
#endif
}


static void insert_tail( SRQ * que, SRQ * node)
{
/**************************************
 *
 *	i n s e r t _ t a i l
 *
 **************************************
 *
 * Functional description
 *	Insert a node at the tail of a que.
 *
 **************************************/
	SRQ *prior;

	node->srq_forward = REL_PTR(que);
	node->srq_backward = que->srq_backward;

	prior = (SRQ *) ABS_PTR(que->srq_backward);
	prior->srq_forward = REL_PTR(node);
	que->srq_backward = REL_PTR(node);
}


static void mutex_bugcheck( UCHAR * string, int number)
{
/**************************************
 *
 *	m u t e x _ b u g c h e c k
 *
 **************************************
 *
 * Functional description
 *	Disasterous mutex bug.  Issue message and abort process.
 *
 **************************************/
	SCHAR msg[128];

	sprintf(msg, "CSS error: %s, status: %d", string, number);
	gds__log(msg);
	fprintf(stderr, "%s\n", string);
	abort();
}


static void punt( TEXT * string)
{
/**************************************
 *
 *	p u n t
 *
 **************************************
 *
 * Functional description
 *
 **************************************/

	printf("(CSS)punt: global region corrupt -- %s\n", string);

#ifdef DEBUG
	dump();
#endif
}


static int put_message( PTR connection_id, MSG message, int release_flag)
{
/**************************************
 *
 *	p u t _ m e s s a g e
 *
 **************************************
 *
 * Functional description
 *	Post a message (formatted) to a given processes msg que.
 *
 **************************************/
	CNCT connection;
	PRB process;
	int sig_flag;

	connection = (CNCT) ABS_PTR(connection_id);

	if (!connection->cnct_partner) {
		free_global(message);
		return FALSE;
	}

	message->msg_connection = connection_id;
	process = (PRB) ABS_PTR(connection->cnct_partner);
	insert_tail(&process->prb_messages, &message->msg_que);

	if (release_flag)
		RELEASE;

	ISC_event_post(process->prb_event);

#ifndef GATEWAY
#ifndef MULTI_THREAD
	if (message->msg_type == MSG_event)
		ISC_kill(process->prb_process_id, EVENT_SIGNAL);
#endif
#endif

	return TRUE;
}


#ifdef UNIX
static void release_semaphore( USHORT semaphore)
{
/**************************************
 *
 *	r e l e a s e _ s e m a p h o r e
 *
 **************************************
 *
 * Functional description
 *	Release a no-longer needed (or newly allocated) semaphore.
 *
 **************************************/

	CSS_header->csh_semaphores[semaphore >> 3] |= (1 << (semaphore & 7));
}
#endif


static void remove_que( SRQ * node)
{
/**************************************
 *
 *	r e m o v e _ q u e
 *
 **************************************
 *
 * Functional description
 *	Remove a node from a self-relative que.
 *
 **************************************/
	SRQ *que;

	que = (SRQ *) ABS_PTR(node->srq_forward);
	que->srq_backward = node->srq_backward;

	que = (SRQ *) ABS_PTR(node->srq_backward);
	que->srq_forward = node->srq_forward;
	node->srq_forward = node->srq_backward = 0;
}


#ifdef VMS
static void set_timer( USHORT timeout, PRB process)
{
/**************************************
 *
 *	s e t _ t i m e r 	( V M S )
 *
 **************************************
 *
 * Functional description
 *	Establish a timer to go off after a specified period
 *	of time.  The timeout period is given in seconds and
 *	is assumed to be less than 3600 seconds.
 *
 **************************************/
	UCHAR asc_time[10];
	SLONG bin_time[2], desc[2];

	sprintf(asc_time, "0 :%d:%d", timeout / 60, timeout % 60);
	ISC_make_desc(asc_time, desc, 0);
	if (sys$bintim(desc, bin_time) & 1)
		sys$setimr(EVENT_FLAG, bin_time, timeout_ast, process, 0);
}
#endif


#ifndef VMS
static void set_timer( USHORT timeout, PRB process)
{
/**************************************
 *
 *	s e t _ t i m e r 	( g e n e r i c )
 *
 **************************************
 *
 * Functional description
 *	Establish a timer to go off after a specified period
 *	of time.  The timeout period is given in seconds and
 *	is assumed to be less than 3600 seconds.
 *
 **************************************/
}
#endif


#ifdef PIPE_SERVER
#ifdef VMS
static void shutdown_section(void)
{
/**************************************
 *
 *	s h u t d o w n _ s e c t i o n
 *
 **************************************
 *
 * Functional description
 *	Remove the mapped file.
 *
 **************************************/
	struct FAB fab;

/* Delete the shared address space and deassign the file. */

	sys$deltva(shmem_data->sh_mem_retadr, 0, 0);
	sys$dassgn((USHORT) shmem_data->sh_mem_channel);

/* Now try to delete the file by opening it with the delete flag set. */

	fab = cc$rms_fab;
	fab.fab$l_fna = shmem_data->sh_mem_filename
		fab.fab$b_fns = strlen(shmem_data->sh_mem_filename);
	fab.fab$l_fop = FAB$M_DLT;
	fab.fab$b_fac = FAB$M_UPD | FAB$M_PUT;
	fab.fab$b_shr = FAB$M_SHRGET | FAB$M_SHRPUT | FAB$M_UPI;
	fab.fab$b_rfm = FAB$C_UDF;

	if (sys$open(&fab) & 1)
		sys$dassgn(fab.fab$l_stv);
}
#endif
#endif


#ifdef VMS
static void timeout_ast( PRB process)
{
/**************************************
 *
 *	t i m e o u t _ a s t		( V M S )
 *
 **************************************
 *
 * Functional description
 *	Completion ast for set_timer.
 *
 **************************************/

	ACQUIRE;

	process->prb_flags |= PRB_timeout;
	ISC_event_post(process->prb_event);

	RELEASE;
}
#endif

