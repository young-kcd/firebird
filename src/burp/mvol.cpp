/*
 *	PROGRAM:	JRD Backup and Restore Program
 *	MODULE:		multivol.c
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
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#include "../burp/burp.h"
#ifdef WIN_NT
#include <windows.h>
#include <winnt.h>
#ifdef TEXT
#undef TEXT
#endif
#define TEXT char
#endif
#include "../burp/burp_proto.h"
#include "../burp/mvol_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/gdsassert.h"
#include "../jrd/thd_proto.h"
#ifndef VMS
#include <fcntl.h>
#include <sys/types.h>
#else
#include <types.h>
#include <file.h>
#endif
#if (defined WIN_NT)
#include <io.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif



#define	OPEN_MASK	((int) 0666)

#ifdef VMS
#define TERM_INPUT	"sys$input"
#define TERM_OUTPUT	"sys$error"
#endif

#ifdef WIN_NT
#define TERM_INPUT	"CONIN$"
#define TERM_OUTPUT	"CONOUT$"
#endif

#ifndef TERM_INPUT
#define TERM_INPUT	"/dev/tty"
#define TERM_OUTPUT	"/dev/tty"
#endif

#define MAX_HEADER_SIZE		512

#define BITS_ON(word,bits)	((bits) == ((word)&(bits)))

#define GET()				(--(tdgbl->mvol_io_cnt) >= 0 ? *(tdgbl->mvol_io_ptr)++ : 255)
#define GET_ATTRIBUTE(att)		((att) = (ATT_TYPE) GET())

#define PUT(c)				--(tdgbl->mvol_io_cnt); *(tdgbl->mvol_io_ptr)++ = (UCHAR) (c)
#define PUT_NUMERIC(attribute, value)	put_numeric ((attribute), (value))
#define PUT_ASCIZ(attribute, string)	put_asciz ((attribute), (string))

static void  bad_attribute(USHORT, USHORT);
static void  file_not_empty(void);
static SLONG get_numeric(void);
static int   get_text(UCHAR*, SSHORT);
static void  prompt_for_name(SCHAR*, int);
static void  put_asciz(SCHAR, SCHAR*);
static void  put_numeric(SCHAR, int);
static bool  read_header(DESC, ULONG*, USHORT*, USHORT);
static bool  write_header(DESC, ULONG, USHORT);
static DESC	 next_volume(DESC, int, USHORT);


//____________________________________________________________
//
//
UINT64 MVOL_fini_read()
{
	TGBL tdgbl = GET_THREAD_DATA;

	if (strcmp(tdgbl->mvol_old_file, "stdin") != 0)
	{
		CLOSE(tdgbl->file_desc);

		FIL	file;
		for (file = tdgbl->gbl_sw_backup_files; file; file = file->fil_next)
		{
			if (file->fil_fd == tdgbl->file_desc) {
				file->fil_fd = INVALID_HANDLE_VALUE;
			}
		}
	}

	tdgbl->file_desc = INVALID_HANDLE_VALUE;
	BURP_FREE(tdgbl->mvol_io_buffer);
	tdgbl->mvol_io_buffer = NULL;
	tdgbl->io_cnt = 0;
	tdgbl->io_ptr = NULL;
	return tdgbl->mvol_cumul_count;
}


//____________________________________________________________
//
//
UINT64 MVOL_fini_write(int* io_cnt, UCHAR** io_ptr)
{
	TGBL tdgbl;
	FIL	file;

	tdgbl = GET_THREAD_DATA;

	(void) MVOL_write(rec_end, io_cnt, io_ptr);
	FLUSH(tdgbl->file_desc);
	if (strcmp(tdgbl->mvol_old_file, "stdout") != 0)
	{
		CLOSE(tdgbl->file_desc);
		for (file = tdgbl->gbl_sw_backup_files; file; file = file->fil_next)
		{
			if (file->fil_fd == tdgbl->file_desc)
				file->fil_fd = INVALID_HANDLE_VALUE;
		}
	}
	tdgbl->file_desc = INVALID_HANDLE_VALUE;
	BURP_FREE(tdgbl->mvol_io_header);
	tdgbl->mvol_io_header = NULL;
	tdgbl->mvol_io_buffer = NULL;
	tdgbl->io_cnt = 0;
	tdgbl->io_ptr = NULL;
	return tdgbl->mvol_cumul_count;
}


//____________________________________________________________
//
//
void MVOL_init(ULONG io_buf_size)
{
	TGBL tdgbl = GET_THREAD_DATA;

	tdgbl->mvol_io_buffer_size = io_buf_size;
}


//____________________________________________________________
//
// Read init record from backup file
//
void MVOL_init_read(UCHAR*	database_name,
					UCHAR*	file_name,
					USHORT*	format,
					int*	cnt,
					UCHAR**	ptr)
{
	ULONG temp_buffer_size;
	UCHAR *new_buffer;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	tdgbl->mvol_volume_count = 1;
	tdgbl->mvol_empty_file = TRUE;

	if (file_name != (UCHAR *) NULL)
	{
		strncpy(tdgbl->mvol_old_file, (char*) file_name, MAX_FILE_NAME_LENGTH);
		tdgbl->mvol_old_file[MAX_FILE_NAME_LENGTH - 1] = 0;
	}
	else
	{
		tdgbl->mvol_old_file[0] = 0;
	}

	tdgbl->mvol_actual_buffer_size = temp_buffer_size =
		tdgbl->mvol_io_buffer_size;
	tdgbl->mvol_io_buffer = BURP_ALLOC(temp_buffer_size);
	tdgbl->gbl_backup_start_time[0] = 0;

	read_header(tdgbl->file_desc, &temp_buffer_size, format, TRUE);

	if (temp_buffer_size > tdgbl->mvol_actual_buffer_size)
	{
		new_buffer = BURP_ALLOC(temp_buffer_size);
		memcpy(new_buffer, tdgbl->mvol_io_buffer, tdgbl->mvol_io_buffer_size);
		BURP_FREE(tdgbl->mvol_io_buffer);
		tdgbl->mvol_io_ptr =
			new_buffer + (tdgbl->mvol_io_ptr - tdgbl->mvol_io_buffer);
		tdgbl->mvol_io_buffer = new_buffer;
	}

	tdgbl->mvol_actual_buffer_size = tdgbl->mvol_io_buffer_size =
		temp_buffer_size;
	*cnt = tdgbl->mvol_io_cnt;
	*ptr = tdgbl->mvol_io_ptr;
}


//____________________________________________________________
//
// Write init record to the backup file
//
void MVOL_init_write(UCHAR*		database_name,
					 UCHAR*		file_name,
					 int*		cnt,
					 UCHAR**	ptr)
{
	ULONG temp_buffer_size;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	tdgbl->mvol_volume_count = 1;
	tdgbl->mvol_empty_file = TRUE;

	if (file_name != (UCHAR *) NULL)
	{
		strncpy(tdgbl->mvol_old_file, (char*)file_name, MAX_FILE_NAME_LENGTH);
		tdgbl->mvol_old_file[MAX_FILE_NAME_LENGTH - 1] = 0;
	}
	else
	{
		tdgbl->mvol_old_file[0] = 0;
	}

	tdgbl->mvol_actual_buffer_size = tdgbl->mvol_io_buffer_size;
	temp_buffer_size = tdgbl->mvol_io_buffer_size * tdgbl->gbl_sw_blk_factor;
	tdgbl->mvol_io_ptr = tdgbl->mvol_io_buffer =
		BURP_ALLOC(temp_buffer_size + MAX_HEADER_SIZE);
	tdgbl->mvol_io_cnt = tdgbl->mvol_actual_buffer_size;

	while (!write_header(tdgbl->file_desc, temp_buffer_size, FALSE))
	{
		if (tdgbl->action->act_action == ACT_backup_split)
		{
			BURP_error(269, tdgbl->action->act_file->fil_name, 0, 0, 0, 0);
			/* msg 269 can't write a header record to file %s */
		}
		tdgbl->file_desc = next_volume(tdgbl->file_desc, MODE_WRITE, FALSE);
	}

	tdgbl->mvol_actual_buffer_size = temp_buffer_size;

	*cnt = tdgbl->mvol_io_cnt;
	*ptr = tdgbl->mvol_io_ptr;
}


#ifndef WIN_NT
//____________________________________________________________
//
// Read a buffer's worth of data. (non-WIN_NT)
//
int MVOL_read(int* cnt, UCHAR** ptr)
{
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	for (;;)
	{
		tdgbl->mvol_io_cnt =
			read(	tdgbl->file_desc,
					tdgbl->mvol_io_buffer,
					tdgbl->mvol_io_buffer_size);
		tdgbl->mvol_io_ptr = tdgbl->mvol_io_buffer;

		if (tdgbl->mvol_io_cnt > 0) {
			break;
		}

		{
			if (!tdgbl->mvol_io_cnt || errno == EIO)
			{
				tdgbl->file_desc = next_volume(tdgbl->file_desc, MODE_READ, FALSE);
				if (tdgbl->mvol_io_cnt > 0)
				{
					break;
				}
			}

			else if (!SYSCALL_INTERRUPTED(errno))
			{
				if (cnt)
				{
					BURP_error_redirect(0, 220, NULL, NULL);
				/* msg 220 Unexpected I/O error while reading from backup file */
				}
				else
				{
					BURP_error_redirect(0, 50, NULL, NULL);
				/* msg 50 unexpected end of file on backup file */
				}
			}
		}
	}

	tdgbl->mvol_cumul_count += tdgbl->mvol_io_cnt;
	file_not_empty();

	*ptr = tdgbl->mvol_io_ptr + 1;
	*cnt = tdgbl->mvol_io_cnt - 1;

	return *(tdgbl->mvol_io_ptr);
}


#else
//____________________________________________________________
//
// Read a buffer's worth of data. (WIN_NT)
//
int MVOL_read(int* cnt, UCHAR** ptr)
{
	BOOL ret;

	TGBL tdgbl = GET_THREAD_DATA;

	for (;;)
	{
		ret = ReadFile(tdgbl->file_desc,
					   tdgbl->mvol_io_buffer,
					   tdgbl->mvol_io_buffer_size,
					   reinterpret_cast<DWORD*>(&tdgbl->mvol_io_cnt),
					   NULL);
		tdgbl->mvol_io_ptr = tdgbl->mvol_io_buffer;
		if (tdgbl->mvol_io_cnt > 0)
			break;
		else
		{
			if (!tdgbl->mvol_io_cnt)
			{
				tdgbl->file_desc = next_volume(tdgbl->file_desc, MODE_READ, FALSE);
				if (tdgbl->mvol_io_cnt > 0)
					break;
			}
			else if (GetLastError() != ERROR_HANDLE_EOF)
			{
				if (cnt)
					BURP_error_redirect(0, 220, NULL, NULL);
				/* msg 220 Unexpected I/O error while reading from backup file */
				else
					BURP_error_redirect(0, 50, NULL, NULL);
				/* msg 50 unexpected end of file on backup file */
			}
		}
	}

	tdgbl->mvol_cumul_count += tdgbl->mvol_io_cnt;
	file_not_empty();

	*ptr = tdgbl->mvol_io_ptr + 1;
	*cnt = tdgbl->mvol_io_cnt - 1;

	return *(tdgbl->mvol_io_ptr);
}
#endif /* !WIN_NT */


//____________________________________________________________
//
// Read a chunk of data from the IO buffer.
// Return a pointer to the first position NOT read into.
//
UCHAR* MVOL_read_block(TGBL tdgbl, UCHAR * ptr, ULONG count)
{
/* To handle tape drives & Multi-volume boundaries, use the normal
 * read function, instead of doing a more optimal bulk read.
 */

	while (count)
	{
		ULONG n;

		/* If buffer empty, reload it */
		if (tdgbl->io_cnt <= 0)
		{
			*ptr++ = MVOL_read(&tdgbl->io_cnt, &tdgbl->io_ptr);

			/* One byte was "read" by MVOL_read */
			count--;
		}

		n = MIN(count, (ULONG) tdgbl->io_cnt);

		/* Copy data from the IO buffer */

		(void) memcpy(ptr, tdgbl->io_ptr, n);
		ptr += n;

		/* Skip ahead in current buffer */

		count -= n;
		tdgbl->io_cnt -= n;
		tdgbl->io_ptr += n;

	}
	return ptr;
}


//____________________________________________________________
//
// Skip head in the IO buffer.  Often used when only
// doing partial restores.
//
void MVOL_skip_block( TGBL tdgbl, ULONG count)
{
/* To handle tape drives & Multi-volume boundaries, use the normal
 * read function, instead of doing a more optimal seek.
 */

	while (count)
	{
		ULONG n;

		/* If buffer empty, reload it */
		if (tdgbl->io_cnt <= 0)
		{
			(void) MVOL_read(&tdgbl->io_cnt, &tdgbl->io_ptr);

			/* One byte was "read" by MVOL_read */
			count--;
		}

		n = MIN(count, (ULONG) tdgbl->io_cnt);

		/* Skip ahead in current buffer */

		count -= n;
		tdgbl->io_cnt -= n;
		tdgbl->io_ptr += n;
	}
}


#ifdef WIN_NT
//____________________________________________________________
//
// detect if it's a tape, rewind if so
// and set the buffer size
//
DESC MVOL_open(char * name, ULONG mode, ULONG create)
{
	TGBL tdgbl;
	HANDLE handle;
	TAPE_GET_MEDIA_PARAMETERS param;
	DWORD size = sizeof(param);

	tdgbl = GET_THREAD_DATA;

	if (strnicmp(name, "\\\\.\\tape", 8))
	{
		handle = CreateFile(name, mode,
							mode == MODE_WRITE ? 0 : FILE_SHARE_READ,
							NULL, create, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else
	{
		/* it's a tape device */
		/* Note: we *want* to open the tape in Read-only mode or in
		 * write-only mode, but it turns out that on NT SetTapePosition 
		 * will fail (thereby not rewinding the tape) if the tape is 
		 * opened write-only, so we will make sure that we always have 
		 * read access. So much for standards!
		 * Ain't Windows wonderful???
		 */
		/* Note: we *want* to open the tape in FILE_EXCLUSIVE_WRITE, but
		 * during testing discovered that several NT tape drives do not
		 * work unless we specify FILE_SHARE_WRITE as the open mode.
		 * So it goes...
		 */
		handle = CreateFile(name,
							mode | MODE_READ,
							mode ==
								MODE_WRITE ? FILE_SHARE_WRITE : FILE_SHARE_READ,
							0, OPEN_EXISTING, 0, NULL);
		if (handle != INVALID_HANDLE_VALUE)
		{
			/* emulate UNIX rewinding the tape on open:
			 * This MUST be done since Windows does NOT have anything
			 * like mt to allow the user to do tape management. The 
			 * implication here is that we will be able to write ONLY
			 * one (1) database per tape. This is bad if the user wishes to
			 * backup several small databases.
			 * Note: We are intentionally NOT trapping for errors during
			 * rewind, since if we can not rewind, we are either a non-rewind
			 * device (then it is user controlled) or we have a problem with
			 * the physical media.  In the latter case I would rather wait for 
			 * the write to fail so that we can loop and prompt the user for 
			 * a different file/device. 
			 */
			SetTapePosition(handle, TAPE_REWIND, 0, 0, 0, FALSE);
			if (GetTapeParameters(	handle,
									GET_TAPE_MEDIA_INFORMATION,
									&size,
									&param) == NO_ERROR)
			{
				tdgbl->io_buffer_size = param.BlockSize;
			}
		}
	}
	return handle;
}
#endif /* WIN_NT */


//____________________________________________________________
//
// Write a buffer's worth of data.
//
UCHAR MVOL_write(UCHAR c, int *io_cnt, UCHAR ** io_ptr)
{
	UCHAR *ptr;
	ULONG left, cnt, size_to_write;
	USHORT full_buffer;
	TGBL tdgbl;
	FIL	file;

#ifdef WIN_NT
	DWORD err;
#endif /* WIN_NT */

	tdgbl = GET_THREAD_DATA;

	size_to_write = BURP_UP_TO_BLOCK(*io_ptr - tdgbl->mvol_io_buffer);

	for (ptr = tdgbl->mvol_io_buffer, left = size_to_write;
		 left > 0;
		 ptr += cnt, left -= cnt)
	{
		if (tdgbl->action->act_action == ACT_backup_split)
		{
			/* Write to the current file till fil_lingth > 0, otherwise
			   switch to the next one */
			if (tdgbl->action->act_file->fil_length == 0)
			{
				if (tdgbl->action->act_file->fil_next)
				{
					CLOSE(tdgbl->file_desc);
					for (file = tdgbl->gbl_sw_backup_files; file; file = file->fil_next)
					{
						if (file->fil_fd == tdgbl->file_desc)
							file->fil_fd = INVALID_HANDLE_VALUE;
					}
					tdgbl->action->act_file->fil_fd = INVALID_HANDLE_VALUE;
					tdgbl->action->act_file = tdgbl->action->act_file->fil_next;
					tdgbl->file_desc = tdgbl->action->act_file->fil_fd;
				}
				else
				{
					/* This is a last file. Keep writing in a hope that there is
					   enough free disk space ... */
					tdgbl->action->act_file->fil_length = MAX_LENGTH;
				}
			}
		}
#ifndef WIN_NT
		cnt = write(tdgbl->file_desc, ptr,
					((tdgbl->action->act_action == ACT_backup_split) &&
					 (tdgbl->action->act_file->fil_length < left) ?
					 tdgbl->action->act_file->fil_length : left));
#else
		const DWORD nBytesToWrite =
			(tdgbl->action->act_action == ACT_backup_split &&
			 (tdgbl->action->act_file->fil_length < left) ?
			 tdgbl->action->act_file->fil_length : left);
		if (!WriteFile(tdgbl->file_desc,
					   ptr,
					   nBytesToWrite,
					   reinterpret_cast < DWORD * >(&cnt), NULL)) {
			err = GetLastError();
		}
#endif /* !WIN_NT */
		tdgbl->mvol_io_buffer = tdgbl->mvol_io_data;
		if (cnt > 0)
		{
			tdgbl->mvol_cumul_count += cnt;
			file_not_empty();
			if (tdgbl->action->act_action == ACT_backup_split)
			{
				if (tdgbl->action->act_file->fil_length < left)
					tdgbl->action->act_file->fil_length = 0;
				else
					tdgbl->action->act_file->fil_length -= left;
			}
		}
		else
		{
			if (!cnt ||
#ifndef WIN_NT
				errno == ENOSPC || errno == EIO || errno == ENXIO ||
				errno == EFBIG)
#else
				err == ERROR_DISK_FULL || err == ERROR_HANDLE_DISK_FULL)
#endif /* !WIN_NT */
			{
				if (tdgbl->action->act_action == ACT_backup_split)
				{
					/* Close the current file and switch to the next one.
					   If there is no more specified files left then
					   issue an error and give up */
					if (tdgbl->action->act_file->fil_next)
					{
						CLOSE(tdgbl->file_desc);
						for (file = tdgbl->gbl_sw_backup_files; file; file = file->fil_next)
						{
							if (file->fil_fd == tdgbl->file_desc)
								file->fil_fd = INVALID_HANDLE_VALUE;
						}
						
						tdgbl->action->act_file->fil_fd = INVALID_HANDLE_VALUE;
						BURP_print(272,
									tdgbl->action->act_file->fil_name,
									(TEXT *) tdgbl->action->act_file->fil_length,
									tdgbl->action->act_file->fil_next->fil_name,
									0, 0);	/* msg 272 Warning -- free disk space exhausted for file %s, the rest of the bytes (%d) will be written to file %s */
						tdgbl->action->act_file->fil_next->fil_length +=
							tdgbl->action->act_file->fil_length;
						tdgbl->action->act_file =
							tdgbl->action->act_file->fil_next;
						tdgbl->file_desc = tdgbl->action->act_file->fil_fd;
					}
					else
					{
						BURP_error(270, 0, 0, 0, 0, 0);	/* msg 270 free disk space exhausted */
					}
					cnt = 0;
					continue;
				}
				/* Note: there is an assumption here, that if header data is being
				   written, it is really being rewritten, so at least all the
				   header data will be written */

				if (left != size_to_write)
				{
					/* Wrote some, move remainder up in buffer. */

					/* NOTE: We should NOT use memcpy here.  We're moving overlapped
					   data and memcpy does not guanantee the order the data
					   is moved in */
					memcpy(tdgbl->mvol_io_data, ptr, left);
				}
				left += tdgbl->mvol_io_data - tdgbl->mvol_io_header;
				if (left >=  tdgbl->mvol_io_buffer_size)
					full_buffer = TRUE;
				else
					full_buffer = FALSE;
				tdgbl->file_desc =
					next_volume(tdgbl->file_desc, MODE_WRITE, full_buffer);
				if (full_buffer)
				{
					left -= tdgbl->mvol_io_buffer_size;
					memcpy(tdgbl->mvol_io_data,
						   tdgbl->mvol_io_header + tdgbl->mvol_io_buffer_size,
						   left);
					tdgbl->mvol_cumul_count +=
						tdgbl->mvol_io_buffer_size;
					tdgbl->mvol_io_buffer = tdgbl->mvol_io_data;
				}
				else
					tdgbl->mvol_io_buffer = tdgbl->mvol_io_header;
				break;
			}
			else if (!SYSCALL_INTERRUPTED(errno))
			{
				BURP_error_redirect(0, 221, NULL, NULL);
				/* msg 221 Unexpected I/O error while writing to backup file */
			}
		}
        if (left < cnt) {    // this is impossible, but...
            cnt = left;
        }

	}

#ifdef DEBUG
	{
		int dbg_cnt;
		if (debug_on)
			for (dbg_cnt = 0; dbg_cnt < cnt; dbg_cnt++)
				ib_printf("%d,\n", *(ptr + dbg_cnt));
	}
#endif

/* After the first block of first volume is written (using a default block size)
   change the block size to one that reflects the user's blocking factor.  By
   making the first block a standard size we will avoid restore problems. */

	tdgbl->mvol_io_buffer_size = tdgbl->mvol_actual_buffer_size;

	ptr = tdgbl->mvol_io_buffer + left;
	*ptr++ = c;
	*io_ptr = ptr;
	*io_cnt = tdgbl->mvol_io_buffer_size - 1 - left;

	return c;
}


//____________________________________________________________
//
// Read a chunk of data from the IO buffer.
// Return a pointer to the first position NOT written from.
//
UCHAR *MVOL_write_block(TGBL tdgbl, UCHAR * ptr, ULONG count)
{
/* To handle tape drives & Multi-volume boundaries, use the normal
 * write function, instead of doing a more optimal bulk write.
 */

	while (count)
	{
		ULONG n;

		/* If buffer full, dump it */
		if (tdgbl->io_cnt <= 0)
		{
			(void) MVOL_write(*ptr++, &tdgbl->io_cnt, &tdgbl->io_ptr);

			/* One byte was written by MVOL_write */
			count--;
		}

		n = MIN(count, (ULONG) tdgbl->io_cnt);

		/* Copy data to the IO buffer */

		(void) memcpy(tdgbl->io_ptr, ptr, n);
		ptr += n;

		/* Skip ahead in current buffer */

		count -= n;
		tdgbl->io_cnt -= n;
		tdgbl->io_ptr += n;

	}
	return ptr;
}


//____________________________________________________________
//
// We ran into an unsupported attribute.  This shouldn't happen,
// but it isn't the end of the world.
//
static void bad_attribute(USHORT attribute, USHORT type)
{
	SSHORT l;
	TEXT name[128];
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	gds__msg_format(0, 12, type, sizeof(name), name, 0, 0, 0, 0, 0);
	BURP_print(80, name, (TEXT *) (ULONG) attribute, NULL, NULL, NULL);
	/* msg 80  don't recognize %s attribute %ld -- continuing */
	l = GET();
	if (l)
	{
		do {
			GET();
		} while (--l);
	}
}


//____________________________________________________________
//
//
static void file_not_empty(void)
{
	TGBL tdgbl = GET_THREAD_DATA;

	tdgbl->mvol_empty_file = FALSE;
}


//____________________________________________________________
//
// Get a numeric value from the input stream.
//
static SLONG get_numeric(void)
{
	SLONG value[2];

	SSHORT length = get_text((UCHAR*) value, sizeof(value));

	return gds__vax_integer((UCHAR*) value, length);
}


//____________________________________________________________
//
// Move a text attribute to a string and fill.
//
static int get_text(UCHAR* text, SSHORT length)
{
	unsigned int l, l2;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	l = GET();
	length -= l;
	l2 = l;

	if (length < 0)
	{
		BURP_error_redirect(0, 46, NULL, NULL);	/* msg 46 string truncated */
	}

	if (l)
	{
		do {
			*text++ = GET();
		} while (--l);
	}

	*text = 0;

	return l2;
}


//____________________________________________________________
//
// Get specification for the next volume (tape).
// Try to open it. Return file descriptor.
//
static DESC next_volume( DESC handle, int mode, USHORT full_buffer)
{
	SCHAR new_file[MAX_FILE_NAME_LENGTH];
	DESC new_desc;
	ULONG temp_buffer_size;
	USHORT format;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

/* We must  close the old handle before the user inserts
   another tape, or something. */

#ifdef WIN_NT
	if (handle != INVALID_HANDLE_VALUE)
#else
	if (handle > -1)
#endif /* WIN_NT */
	{
		CLOSE(handle);
	}

	if (tdgbl->action->act_action == ACT_restore_join)
	{
		tdgbl->action->act_file->fil_fd = INVALID_HANDLE_VALUE;
		if ((tdgbl->action->act_total > tdgbl->action->act_file->fil_seq) &&
			(tdgbl->action->act_file = tdgbl->action->act_file->fil_next) &&
			(tdgbl->action->act_file->fil_fd != INVALID_HANDLE_VALUE))
		{
			return tdgbl->action->act_file->fil_fd;
		}

		BURP_error_redirect(0, 50, NULL, NULL);	/* msg 50 unexpected end of file on backup file */
	}

/* If we got here, we've got a live one... Up the volume number unless
   the old file was empty */

	if (!tdgbl->mvol_empty_file)
		(tdgbl->mvol_volume_count)++;

	tdgbl->mvol_empty_file = TRUE;

/* Loop until we have opened a file successfully */

	for (new_desc = INVALID_HANDLE_VALUE;;)
	{
		/* We aim to keep our descriptors clean */

		if (new_desc != INVALID_HANDLE_VALUE) {
			CLOSE(new_desc);
			new_desc = INVALID_HANDLE_VALUE;
		}

		/* Get file name to try */

		prompt_for_name(new_file, sizeof(new_file));

#ifdef WIN_NT
		new_desc = MVOL_open(new_file, mode, OPEN_ALWAYS);
		if (new_desc == INVALID_HANDLE_VALUE)
#else
		new_desc = open(new_file, mode, OPEN_MASK);
		if (new_desc < 0)
#endif /* WIN_NT */
		{
			BURP_print(222, new_file, 0, 0, 0, 0);
			/* msg 222 \n\nCould not open file name \"%s\"\n */
			continue;
		}

		/* If the file is to be writable, probe it, and make sure it is... */

#ifdef WIN_NT
		if (mode == MODE_WRITE)
#else
		if (BITS_ON(mode, O_WRONLY) || BITS_ON(mode, O_RDWR))
#endif /* WIN_NT */
		{
			if (!write_header(new_desc, 0L, full_buffer))
			{
				BURP_print(223, new_file, 0, 0, 0, 0);
				/* msg223 \n\nCould not write to file \"%s\"\n */
				continue;
			}
			else
			{
				BURP_msg_put(261, (TEXT *) (SLONG) (tdgbl->mvol_volume_count),
							 new_file, 0, 0, 0);
				/* Starting with volume #vol_count, new_file */
				BURP_verbose(75, new_file, 0, 0, 0, 0);	/* msg 75  creating file %s */
			}
		}
		else
		{
			/* File is open for read only.  Read the header. */

			if (!read_header(new_desc, &temp_buffer_size, &format, FALSE))
			{
				BURP_print(224, new_file, 0, 0, 0, 0);
				continue;
			}
			else
			{
				BURP_msg_put(261, (TEXT *) (SLONG) (tdgbl->mvol_volume_count),
							 new_file, 0, 0, 0);
				/* Starting with volume #vol_count, new_file */
				BURP_verbose(100, new_file, 0, 0, 0, 0);	/* msg 100  opened file %s */
			}
		}

		strcpy(tdgbl->mvol_old_file, new_file);
		return new_desc;
	}
}


//____________________________________________________________
//
//
static void prompt_for_name(SCHAR* name, int length)
{
	IB_FILE*	term_in;
	IB_FILE*	term_out;
	SCHAR*		name_ptr;
	TEXT		msg[128];

	TGBL tdgbl = GET_THREAD_DATA;

/* Unless we are operating as a service, stdin can't necessarily be trusted.
   Get a location to read from. */

	if (tdgbl->gbl_sw_service_gbak ||
		isatty(ib_fileno(ib_stdout)) ||
		!(term_out = ib_fopen(TERM_OUTPUT, "w")))
	{
		term_out = ib_stdout;
	}
	if (tdgbl->gbl_sw_service_gbak ||
		isatty(ib_fileno(ib_stdin)) ||
		!(term_in = ib_fopen(TERM_INPUT, "r")))
	{
		term_in = ib_stdin;
	}

/* Loop until we have a file name to try */

	for (;;)
	{
		/* If there was an old file name, use that prompt */

		if (strlen(tdgbl->mvol_old_file) > 0)
		{
			BURP_msg_get(225, msg, (TEXT *) (SLONG) (tdgbl->mvol_volume_count - 1),
						 tdgbl->mvol_old_file, 0, 0, 0);
			ib_fprintf(term_out, msg);
			BURP_msg_get(226, msg, 0, 0, 0, 0, 0);
			/* \tPress return to reopen that file, or type a new\n\tname followed by return to open a different file.\n */
			ib_fprintf(term_out, msg);
		}
		else	/* First volume */
		{
			BURP_msg_get(227, msg, 0, 0, 0, 0, 0);
			/* Type a file name to open and hit return */
			ib_fprintf(term_out, msg);
		}
		BURP_msg_get(228, msg, 0, 0, 0, 0, 0);	/* "  Name: " */
		ib_fprintf(term_out, msg);

		if (tdgbl->gbl_sw_service_gbak)
		{
			ib_putc('\001', term_out);
		}
		ib_fflush(term_out);
		if (ib_fgets(name, length, term_in) == (SCHAR*) NULL)
		{
			BURP_msg_get(229, msg, 0, 0, 0, 0, 0);
			/* \n\nERROR: Backup incomplete\n */
			ib_fprintf(term_out, msg);
			exit(FINI_ERROR);
		}

		/* If the user typed just a carriage return, they
		   want the old file.  If there isn't one, reprompt */

		if (name[0] == '\n')
		{
			if (strlen(tdgbl->mvol_old_file) > 0)
			{
				strcpy(name, tdgbl->mvol_old_file);
				break;
			}
			else				/* reprompt */
				continue;
		}

		/* OK, its a file name, strip the carriage return */

		name_ptr = name;
		while (*name_ptr && *name_ptr != '\n')
		{
			name_ptr++;
		}
		*name_ptr = 0;
		break;
	}

	if (term_out != ib_stdout) {
		ib_fclose(term_out);
	}
	if (term_in != ib_stdin) {
		ib_fclose(term_in);
	}
}


//____________________________________________________________
//
// Write an attribute starting with a null terminated string.
//
static void put_asciz( SCHAR attribute, TEXT * string)
{
	TEXT *p;
	SSHORT l;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	for (p = string, l = 0; *p; p++)
	{
		l++;
	}

	PUT(attribute);
	PUT(l);
	if (l)
	{
		do
		{
			PUT(*string++);
		} while (--l);
	}
}


//____________________________________________________________
//
// Write a numeric value as an attribute.  The number is represented
// low byte first, high byte last, as in VAX.
//
static void put_numeric( SCHAR attribute, int value)
{
	ULONG vax_value;
	USHORT i;
	UCHAR *p;
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

	vax_value = gds__vax_integer((UCHAR *) & value, sizeof(value));
	p = (UCHAR *) & vax_value;

	PUT(attribute);
	PUT(sizeof(value));

	for (i = 0; i < sizeof(value); i++) {
		PUT(*p++);
	}
}


//____________________________________________________________
//
// Functional description
//
static bool read_header(DESC    handle,
						ULONG*  buffer_size,
						USHORT* format,
						USHORT  init_flag)
{
	int attribute, temp;
	SSHORT l;
	ULONG temp_buffer_size;
	TEXT buffer[256], *p, msg[128];
	TGBL tdgbl;

	tdgbl = GET_THREAD_DATA;

/* Headers are a version number, and a volume number */

#ifndef WIN_NT
	tdgbl->mvol_io_cnt =
		read(handle, tdgbl->mvol_io_buffer, tdgbl->mvol_actual_buffer_size);
#else
	ReadFile(handle, tdgbl->mvol_io_buffer, tdgbl->mvol_actual_buffer_size,
			 reinterpret_cast<DWORD*>(&tdgbl->mvol_io_cnt), NULL);
#endif
	tdgbl->mvol_io_ptr = tdgbl->mvol_io_buffer;

	if (GET_ATTRIBUTE(attribute) != rec_burp)
		BURP_error_redirect(0, 45, NULL, NULL);	/* msg 45 expected backup description record */

	while (GET_ATTRIBUTE(attribute) != att_end)
	{
		switch (attribute)
		{
		case att_backup_blksize:
			temp_buffer_size = get_numeric();
			if (init_flag)
				*buffer_size = temp_buffer_size;
			break;

		case att_backup_compress:
			temp = get_numeric();
			if (init_flag)
				tdgbl->gbl_sw_compress = temp;
			break;

		case att_backup_date:
			l = GET();
			if (init_flag)
				p = tdgbl->gbl_backup_start_time;
			else
				p = buffer;
			if (l)
				do
					*p++ = GET();
				while (--l);
			*p = 0;
			if (!init_flag && strcmp(buffer, tdgbl->gbl_backup_start_time))
			{
				BURP_msg_get(230, msg,
							 tdgbl->gbl_backup_start_time, buffer, 0, 0, 0);
				/* Expected backup start time %s, found %s\n */
				ib_printf(msg);
				return false;
			}
			break;

		case att_backup_file:
			l = GET();
			if (init_flag)
			{
				p = tdgbl->mvol_db_name_buffer;
			}
			else
			{
				p = buffer;
			}
			if (l)
			{
				do {
					*p++ = GET();
				} while (--l);
			}
			*p = 0;
			if (!init_flag && strcmp(buffer, tdgbl->gbl_database_file_name))
			{
				BURP_msg_get(231, msg,
							 tdgbl->gbl_database_file_name, buffer, 0, 0, 0);
				/* Expected backup database %s, found %s\n */
				ib_printf(msg);
				return false;
			}
			if (init_flag)
			{
				tdgbl->gbl_database_file_name = tdgbl->mvol_db_name_buffer;
			}
			break;

		case att_backup_format:
			temp = get_numeric();
			if (init_flag)
				*format = temp;
			break;

		case att_backup_transportable:
			temp = get_numeric();
			if (init_flag)
			{
				tdgbl->gbl_sw_transportable = temp;
			}
			break;

		case att_backup_volume:
			temp = get_numeric();
			if (temp != tdgbl->mvol_volume_count)
			{
				BURP_msg_get(232, msg,
							 (TEXT *) (SLONG) (tdgbl->mvol_volume_count),
							 (TEXT *) (SLONG) temp, 0, 0, 0);
				/* Expected volume number %d, found volume %d\n */
				ib_printf(msg);
				return false;
			}
			break;

		default:
			/* TMN: Here we should really have the following assert */
			/* assert(attribute <= MAX_USHORT); */
			bad_attribute((USHORT) attribute, 59);	/* msg 59 backup */
		}
	}

	return true;
}


//____________________________________________________________
//
//
static bool write_header(DESC   handle,
						 ULONG  backup_buffer_size,
						 USHORT full_buffer)
{
	ULONG vax_value;
	USHORT i;
	UCHAR *p, *q;
	TGBL tdgbl;
#ifdef WIN_NT
	DWORD bytes_written, err;
#else
	ULONG bytes_written;
#endif

	tdgbl = GET_THREAD_DATA;

	if (backup_buffer_size)
	{
		tdgbl->mvol_io_header = tdgbl->mvol_io_buffer;

		PUT(rec_burp);
		PUT_NUMERIC(att_backup_format, ATT_BACKUP_FORMAT);

		if (tdgbl->gbl_sw_compress)
			PUT_NUMERIC(att_backup_compress, 1);

		if (tdgbl->gbl_sw_transportable)
			PUT_NUMERIC(att_backup_transportable, 1);

		PUT_NUMERIC(att_backup_blksize, backup_buffer_size);

		tdgbl->mvol_io_volume = tdgbl->mvol_io_ptr + 2;
		PUT_NUMERIC(att_backup_volume, tdgbl->mvol_volume_count);

		PUT_ASCIZ(att_backup_file, tdgbl->gbl_database_file_name);
		PUT_ASCIZ(att_backup_date, tdgbl->gbl_backup_start_time);
		PUT(att_end);

		tdgbl->mvol_io_data = tdgbl->mvol_io_ptr;
	}
	else
	{
		vax_value =
			gds__vax_integer((UCHAR*) &(tdgbl->mvol_volume_count),
							 sizeof(tdgbl->mvol_volume_count));
		p = (UCHAR *) & vax_value;
		q = tdgbl->mvol_io_volume;
		for (i = 0; i < sizeof(int); i++)
		{
			*q++ = *p++;
		}
	}

	if (full_buffer)
	{
#ifdef WIN_NT
		err =
			WriteFile(handle, tdgbl->mvol_io_header,
					  tdgbl->mvol_io_buffer_size, &bytes_written, NULL);
#else
		bytes_written = write(handle, tdgbl->mvol_io_header,
							  tdgbl->mvol_io_buffer_size);
#endif /* WIN_NT */

		if (bytes_written != tdgbl->mvol_io_buffer_size)
		{
			return false;
		}

		if (tdgbl->action->act_action == ACT_backup_split)
		{
			if (tdgbl->action->act_file->fil_length > bytes_written)
			{
				tdgbl->action->act_file->fil_length -= bytes_written;
			}
			else
			{
				tdgbl->action->act_file->fil_length = 0;
			}
		}
		tdgbl->mvol_empty_file = FALSE;
	}

	return true;
}


//____________________________________________________________
//
// Write a header record for split operation
//
bool MVOL_split_hdr_write(void)
{
	TGBL tdgbl;
	TEXT buffer[HDR_SPLIT_SIZE + 1];
	time_t seconds;
#ifdef WIN_NT
	DWORD bytes_written;
#else
	ULONG bytes_written;
#endif


	tdgbl = GET_THREAD_DATA;

	assert(tdgbl->action->act_action == ACT_backup_split);
	assert(tdgbl->action->act_file->fil_fd != INVALID_HANDLE_VALUE);

	if (tdgbl->action->act_file->fil_length < HDR_SPLIT_SIZE) {
		return false;
	}

	seconds = time((time_t *) NULL);

	sprintf(buffer, "%s%.24s      , file No. %4d of %4d, %-27.27s",
			HDR_SPLIT_TAG,
			ctime(&seconds),
			tdgbl->action->act_file->fil_seq,
			tdgbl->action->act_total,
			tdgbl->action->act_file->fil_name);

#ifdef WIN_NT
	WriteFile(tdgbl->action->act_file->fil_fd,
							buffer,
							HDR_SPLIT_SIZE,
							&bytes_written,
							NULL);
#else
	bytes_written =
		write(tdgbl->action->act_file->fil_fd, buffer, HDR_SPLIT_SIZE);
#endif /* WIN_NT */

	if (bytes_written != HDR_SPLIT_SIZE) {
		return false;
	}

	tdgbl->action->act_file->fil_length -= bytes_written;
	return true;
}


//____________________________________________________________
//
// Read a header record for join operation
//
bool MVOL_split_hdr_read(void)
{
	TGBL tdgbl = GET_THREAD_DATA;

	assert(tdgbl->action->act_file->fil_fd != INVALID_HANDLE_VALUE);

	if (tdgbl->action && tdgbl->action->act_file &&
		(tdgbl->action->act_file->fil_fd != INVALID_HANDLE_VALUE))
	{
		TEXT buffer[HDR_SPLIT_SIZE];
		int cnt;

#ifdef WIN_NT
		ReadFile(tdgbl->action->act_file->fil_fd,
					buffer,
					HDR_SPLIT_SIZE,
					reinterpret_cast<DWORD*>(&cnt),
					NULL);
#else
		cnt = read(tdgbl->action->act_file->fil_fd, buffer, HDR_SPLIT_SIZE);
#endif
		if ((cnt == HDR_SPLIT_SIZE) &&
			((strncmp(buffer, HDR_SPLIT_TAG, (sizeof(HDR_SPLIT_TAG) - 1)) == 0) ||
			(strncmp(buffer, HDR_SPLIT_TAG5, (sizeof(HDR_SPLIT_TAG) - 1)) == 0)))
		{
			HDR_SPLIT hdr = (HDR_SPLIT) buffer;
			if ((tdgbl->action->act_file->fil_seq = atoi(hdr->hdr_split_sequence)) > 0 &&
				(tdgbl->action->act_total = atoi(hdr->hdr_split_total)) > 0 &&
				(tdgbl->action->act_file->fil_seq <= tdgbl->action->act_total))
			{
				return true;
			}
		}
	}

	return false;
}

