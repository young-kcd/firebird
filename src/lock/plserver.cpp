/*
 *	PROGRAM:	JRD Remote Interface/Server
 *	MODULE:		plserver.c
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

#include "../jrd/ib_stdio.h"

#include "../jrd/common.h"
#include "plserver.h"

#include "/sys/ins/base.ins.c"
#include "/sys/ins/ipc.ins.c"
#include "/sys/ins/error.ins.c"
#include "/sys/ins/proc2.ins.c"
#include "/sys/ins/ms.ins.c"

/* Block used to manage connections */

typedef struct plc {
	struct plc *plc_next;
	int plc_send_sequence;		/* Next available packet sequence number */
	int plc_recv_sequence;		/* Sequence number of last packet received */
	UCHAR plc_socket[20];		/* Apollo socket handle */
	struct fat *plc_attachments;
	struct blk *plc_server;		/* Server connection on client node */
} *PLC;

/* File attachment blocks */

typedef struct fat {
	struct plc *fat_connection;	/* Connection block for file */
	struct file *fat_file;		/* File block for attachment */
	struct fat *fat_next_connection;	/* Next FAT in connections list */
	struct fat *fat_next_file;	/* Next FAT in file list */
} *FAT;

/* File window and file blocks */

typedef struct fwin {
	struct pag *fwin_address;	/* Address of mapped region */
	SLONG fwin_length;			/* Length of mapped region */
	SLONG fwin_offset;			/* Offset in bytes of region */
	SLONG fwin_activity;		/* Accesses since window turn */
} FWIN;

#define WINDOWS		3
#define WINDOW_LENGTH	32768

typedef struct fil {
	struct file *fil_next;		/* Next file in server */
	FAT fil_attachments;		/* List of attachments to file */
	USHORT fil_count;			/* Number of window */
	FWIN fil_windows[WINDOWS];	/* File windows */
	USHORT fil_length;			/* File name length */
	UCHAR fil_name[1];			/* File name */
} *FIL;

static UCHAR *alloc(USHORT);
static int client_error(PLC, UCHAR *, status_$t);
static PLC connect(PHD *, UCHAR *);
static void detach(FAT);
static void disconnect(PHD *);
static void ipc_error(status_$t, UCHAR *);
static UCHAR *map_file(PLC, FIL, ULONG);
static void move(UCHAR *, UCHAR *, USHORT);
static void open(PHD *, UCHAR *);
static void page_read(PHD *);
static void page_write(PHD *, UCHAR *);
static void release(UCHAR *);
static void reply(PLC, PHD *, USHORT, UCHAR *, USHORT);
static int socket_eql(UCHAR *, UCHAR *);

static PLC connections, servers;
static FIL files;
static ipc_$socket_handle_t server_handle;

static PLC connect();


void main( int argc, char **argv)
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Apollo specific page/lock server.  The page and lock functions
 *	are independent (sigh), but there is no reason to have multiple
 *	servers.
 *
 **************************************/
	UCHAR file[32], header_buffer[128], data_buffer[1024];
	USHORT header_length, data_length;
	PHD *header;
	PLC connection;
	status_$t status;
	uid_$t process_uid;
	ipc_$socket_handle_t client_handle;

/* Open the socket for activity */

	proc2_$who_am_i(process_uid);
	sprintf(file, "/sys/gds.plserver_%x", process_uid.low & 0xFFFFF);
	data_length = strlen(file);

	ipc_$create(file, data_length, status);

	ipc_$open(file, data_length, 4, server_handle, status);
	if (status.all)
		ipc_error(status, "ipc_$open");

/* Process messages */

	header = header_buffer;

	for (;;) {
		for (;;) {
			ipc_$wait(server_handle, 4 * 10, status);
			if (!status.all)
				break;
			if (status.all != ipc_$timeout)
				ipc_error(status, "ipc_$wait");
			for (connection = connections; connection;
				 connection = connection->plc_next) {
			}
		}
		ipc_$rcv(server_handle,
				 sizeof(header_buffer),
				 sizeof(data_buffer),
				 client_handle,
				 header_buffer,
				 header_length, data_buffer, data_length, status);
		if (status.all)
			ipc_error(status, "ipc_$rcv");

		if (!(connection = header->phd_connection))
			connection = connect(header, &client_handle);

		/* Get rid of duplicate packet */

		if (header->phd_sequence == connection->plc_recv_sequence)
			continue;
		connection->plc_recv_sequence = header->phd_sequence;

		switch (header->phd_type) {
		case PL_CONNECT:
			break;

		case PL_DISCONNECT:
			disconnect(header);
			break;

		case PL_PAGE_OPEN:
			open(header, data_buffer);
			break;

		case PL_PAGE_READ:
			page_read(header);
			break;

		case PL_PAGE_WRITE:
			page_write(header, data_buffer);
			break;

		default:
			ib_fprintf(ib_stderr, "packet type %d not understood\n",
					   header->phd_type);
		}
	}
}


static UCHAR *alloc( USHORT size)
{
/**************************************
 *
 *	a l l o c
 *
 **************************************
 *
 * Functional description
 *	Allocate a zero filled block.
 *
 **************************************/
	UCHAR *blk, *p, *end;

	blk = p = gds__alloc(size);
	do
		*p++ = 0;
	while (--size);

	return blk;
}


static int client_error( PLC connection, UCHAR * string, status_$t status)
{
/**************************************
 *
 *	c l i e n t _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Report an error to the client.
 *
 **************************************/
	PHD packet;

	packet.phd_type = PL_ERROR;
	packet.phd_length = 0;
	packet.phd_misc = status.all;
	strcpy(packet.phd_string, string);
	reply(connection, &packet, sizeof(packet), NULL, 0);

	return -1;
}


static PLC connect( PHD * header, UCHAR * socket)
{
/**************************************
 *
 *	c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Accept incoming connection.
 *
 **************************************/
	PLC connection;

/* Check for connection already active */

	for (connection = connections; connection;
		 connection =
		 connection->plc_next) if (socket_eql(connection->plc_socket, socket))
			break;

	if (!connection) {
		connection = alloc(sizeof(struct plc));
		connection->plc_next = connections;
		connections = connection;
	}

	header->phd_handle = connection;
	connection->plc_recv_sequence = header->phd_sequence;
	move(socket, connection->plc_socket, sizeof(connection->plc_socket));
	reply(connection, header, sizeof(struct phd), NULL, 0);

	return connection;
}


static void detach( FAT attachment)
{
/**************************************
 *
 *	d e t a c h
 *
 **************************************
 *
 * Functional description
 *	Detach from a file segment.
 *
 **************************************/
	PLC connection;
	FIL file, *file_ptr;
	FAT *fat;
	FWIN *win, *end;
	status_$t status;

	connection = attachment->fat_connection;
	file = attachment->fat_file;

/* Disconnect attachment from connection. */

	for (fat = &connection->plc_attachments; *fat;
		 fat = &(*fat)->fat_next_connection) if (*fat == attachment) {
			*fat = attachment->fat_next_connection;
			break;
		}

/* Disconnect attachment from file.  If file loses its last attachment,
   release the file */

	for (fat = &file->fil_attachments; *fat; fat = &(*fat)->fat_next_file)
		if (*fat == attachment) {
			*fat = attachment->fat_next_file;
			break;
		}

	if (!file->fil_attachments) {
		for (win = file->fil_windows, end = win + file->fil_count; win < end;
			 win++)
			ms_$unmap(win->fwin_address, win->fwin_length, status);
		for (file_ptr = &files; *file_ptr; file_ptr = &(*file_ptr)->fil_next)
			if (*file_ptr = file) {
				*file_ptr = file->fil_next;
				break;
			}
		release(file);
	}

	release(attachment);
}


static void disconnect( PHD * packet)
{
/**************************************
 *
 *	d i s c o n n e c t
 *
 **************************************
 *
 * Functional description
 *	Break a connection.
 *
 **************************************/
	PLC connection, *ptr;

	connection = packet->phd_connection;
	packet->phd_type = PL_REPLY;
	reply(connection, packet, sizeof(struct phd), NULL, 0);

/* Get rid of file segments */

	while (connection->plc_attachments)
		detach(connection->plc_attachments);

/* Get rid of connection block */

	for (ptr = &connections; *ptr; ptr = &(*ptr)->plc_next)
		if (*ptr == connection) {
			*ptr = connection->plc_next;
			break;
		}
}


static void ipc_error( status_$t status, UCHAR * string)
{
/**************************************
 *
 *	i p c _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	A more or less fatal error occurred.  Seppicu time.
 *
 **************************************/

	ib_fprintf(ib_stderr, "Error during %s operation\n", string);
	error_$print(status);
	abort();
}


static UCHAR *map_file( PLC connection, FIL file, ULONG offset)
{
/**************************************
 *
 *	m a p _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Map a page in a file.
 *
 **************************************/
	FWIN *window, *win, *end;
	SLONG section;
	status_$t status;
	USHORT i, length;
	UCHAR *address;

	win = NULL;

/* Search for a window that is already mapped. */

	for (window = file->fil_windows, end = window + file->fil_count;
		 window < end; window++) {
		if (offset >= window->fwin_offset &&
			offset < window->fwin_offset + window->fwin_length) goto exit;
		if (!win || window->fwin_activity < win->fwin_activity)
			win = window;
	}

/* If not all windows have been used, use another */

	section = offset & ~(WINDOW_LENGTH - 1);

	if (file->fil_count < WINDOWS) {
		address = ms_$mapl(file->fil_name, file->fil_length, section,
						   (SLONG) WINDOW_LENGTH, ms_$cowriters, ms_$wr,
						   (SSHORT) - 1, window->fwin_length, status);

		if (status.all)
			return client_error(connection, "ms_$mapl", status);
		++file->fil_count;
		goto finish;
	}

/* Window not found, remap it.  */

	window = win;
	address = ms_$remap(window->fwin_address, section,
						(SLONG) WINDOW_LENGTH, window->fwin_length, status);

	if (status.all)
		return client_error(connection, "ms_$remap", status);

/* Down grade activity after window shift */

	for (win = file->fil_windows; win < end; win++)
		win->fwin_activity >>= 2;

  finish:

	window->fwin_address = address;
	window->fwin_offset = section;
	window->fwin_activity = 0;

  exit:

	++window->fwin_activity;
	return (SCHAR *) window->fwin_address + offset - window->fwin_offset;
}



static void move( UCHAR * from, UCHAR * to, USHORT length)
{
/**************************************
 *
 *	m o v e
 *
 **************************************
 *
 * Functional description
 *	Move a fixed length string.
 *
 **************************************/

	if (length)
		do
			*to++ = *from++;
		while (--length);
}


static void open( PHD * packet, UCHAR * data)
{
/**************************************
 *
 *	o p e n
 *
 **************************************
 *
 * Functional description
 *	Open a file.
 *
 **************************************/
	PLC connection;
	FIL file;
	FAT attachment;
	FWIN *window;
	status_$t status;

	data[packet->phd_length] = 0;
	connection = packet->phd_connection;

/* Check to see if file is already known */

	for (file = files; file; file = file->fil_next)
		if (!strcmp(data, file->fil_name))
			break;

/* If file doesn't exist, create it now */

	if (!file) {
		file = alloc(sizeof(struct fil) + packet->phd_length);
		file->fil_length = packet->phd_length;
		strcpy(file->fil_name, data);
		file->fil_next = files;
		files = file;
		file->fil_count = 1;

		/* Map the file as appropriate */

		window = file->fil_windows;
		window->fwin_address = ms_$mapl(file->fil_name, file->fil_length,
										window->fwin_offset,
										(SLONG) WINDOW_LENGTH, ms_$cowriters,
										ms_$wr, (SSHORT) - 1,
										window->fwin_length, status);
		if (status.all)
			client_error(connection, "ms_$mapl", status);
		file->fil_count = 1;
	}

/* Connect file and connection block */

	attachment = alloc(sizeof(struct fat));
	attachment->fat_connection = connection;
	attachment->fat_file = file;
	attachment->fat_next_file = file->fil_attachments;
	file->fil_attachments = attachment;
	attachment->fat_next_connection = connection->plc_attachments;
	connection->plc_attachments = attachment;

/* Send a reply */

	packet->phd_handle = attachment;
	packet->phd_type = PL_REPLY;
	reply(connection, packet, sizeof(struct phd), NULL, 0);
}


static void page_read( PHD * packet)
{
/**************************************
 *
 *	p a g e _ r e a d
 *
 **************************************
 *
 * Functional description
 *	Read a database page.
 *
 **************************************/
	PLC connection;
	FIL file;
	FAT attachment;
	UCHAR *ptr;
	status_$t status;

	connection = packet->phd_connection;
	attachment = packet->phd_handle;
	file = attachment->fat_file;

/* Map appropriate part of file */

	ptr = map_file(connection, file, packet->phd_misc);
	if (ptr == -1)
		return;

/* Send a reply */

	packet->phd_handle = attachment;
	packet->phd_type = PL_REPLY;
	reply(connection, packet, sizeof(struct phd), ptr, packet->phd_length);
}


static void page_write( PHD * packet, UCHAR * data)
{
/**************************************
 *
 *	p a g e _ w r i t e
 *
 **************************************
 *
 * Functional description
 *	Write a database page.
 *
 **************************************/
	PLC connection;
	FIL file;
	FAT attachment;
	UCHAR *ptr;
	status_$t status;

	attachment = packet->phd_handle;
	connection = packet->phd_connection;
	file = attachment->fat_file;

/* Map appropriate part of file */

	ptr = map_file(connection, file, packet->phd_misc);
	if (ptr == -1)
		return;

	move(data, ptr, packet->phd_length);

/* Send a reply */

	packet->phd_handle = attachment;
	packet->phd_type = PL_REPLY;
	reply(connection, packet, sizeof(struct phd), NULL, 0);
}


static void release( UCHAR * blk)
{
/**************************************
 *
 *	r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release a block.
 *
 **************************************/

	gds__free(blk);
}


static void reply(
				  PLC connection,
				  PHD * header,
				  USHORT header_len, UCHAR * data, USHORT data_len)
{
/**************************************
 *
 *	r e p l y 
 *
 **************************************
 *
 * Functional description
 *	Reply to a request.
 *
 **************************************/
	status_$t status;

	BUMP_SEQUENCE(connection->plc_send_sequence);
	header->phd_sequence = connection->plc_send_sequence;
	header->phd_ack = connection->plc_recv_sequence;

	ipc_$send(connection->plc_socket, server_handle,
			  *header, header_len, *data, data_len, status);
	if (status.all)
		ipc_error(status, "icp_$send");
}


static int socket_eql( UCHAR * socket1, UCHAR * socket2)
{
/**************************************
 *
 *	s o c k e t _ e q l
 *
 **************************************
 *
 * Functional description
 *	Compare two sockets for equality.
 *
 **************************************/
	UCHAR *end;

	end = socket1 + sizeof(struct ipc_$socket_handle);

	while (socket1 < end)
		if (*socket1++ != socket2++)
			return FALSE;

	return TRUE;
}
