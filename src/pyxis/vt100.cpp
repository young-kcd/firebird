/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		vt100.c
 *	DESCRIPTION:	VT100 Terminal Manager (more like vt220)
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
#include <errno.h>

#ifdef VMS
#include <file.h>
#include <types.h>
#include <stat.h>

#else
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sgtty.h>
#endif

#include "../pyxis/pyxis.h"
#include "../pyxis/scr.h"

#ifndef O_RDWR
#include <fcntl.h>
#endif

static USHORT width, height, graphics_mode;
static int clear_window(), disable(), fini(), get_char(), text(),
update_window();

typedef struct seq {
	UCHAR seq_key;
	UCHAR *seq_sequence;
} SEQ;

static SEQ escape_sequences[] = {
	(UCHAR) C_UP, (UCHAR *) "[A",
	(UCHAR) C_DOWN, (UCHAR *) "[B",	/* cursor keys */
	(UCHAR) C_RIGHT, (UCHAR *) "[C",
	(UCHAR) C_LEFT, (UCHAR *) "[D",
	(UCHAR) C_PF5, (UCHAR *) "[31~",	/* F17 function key */
	(UCHAR) C_PF6, (UCHAR *) "[18~",	/* F7 function key */
	(UCHAR) C_PF7, (UCHAR *) "[19~",	/* F8 function key */
	(UCHAR) C_PF8, (UCHAR *) "[20~",	/* F9 function key */
	(UCHAR) C_PF9, (UCHAR *) "[21~",	/* F10 function key */
	(UCHAR) C_SCROLL_TOP, (UCHAR *) "[5~",	/* Prev Scrn */
	(UCHAR) C_SCROLL_BOTTOM, (UCHAR *) "[6~",	/* Next Scrn */
	NULL, NULL
};

static struct knm keyname_table[] = {
	{C_PF1, "<PF1>"},
	{C_PF2, "<PF2>"},
	{C_PF3, "<PF3>"},
	{C_PF4, "<PF4>"},
	{C_PF5, "<F17>"},
	{C_PF6, "<F7>"},
	{C_PF7, "<F8>"},
	{C_PF8, "<F9>"},
	{C_PF9, "<F10>"},
	{C_ENTER, "<ENTER>"},
	{C_LEFT, "<left arrow>"},
	{C_RIGHT, "<right arrow>"},
	{C_UP, "<up arrow>"},
	{C_DOWN, "<down arrow>"},
	{C_DELETE, "<DELETE>"},
	{C_SCROLL_TOP, "<PREV SCREEN> or <control-T>"},
	{C_SCROLL_BOTTOM, "<NEXT SCREEN> of <control-B>"},
	{C_ERASE, "<control-U>"},
	{C_DELETE_NEXT, "<control-F>"},
	{C_INSERT_OVERSTRIKE, "<control-A>"},
	{C_GOTO_START, "<control-H>"},
	{C_GOTO_END, "<control-E>"},
	{C_EDIT, "<control-G>"},
	{(UCHAR) 0, "<unknown key>"},
};

static UCHAR keypad[256], keypad_equiv[] = {
	C_ENTER, 'M',
	C_PF1, 'P',
	C_PF2, 'Q',
	C_PF3, 'R',
	C_PF4, 'S',
	'-', 'm',
	',', 'l',
	'.', 'n',
	'0', 'p',
	'1', 'q',
	'2', 'r',
	'3', 's',
	'4', 't',
	'5', 'u',
	'6', 'v',
	'7', 'w',
	'8', 'x',
	'9', 'y',
	0, 0
};


VT100_create_window(window)
	 WIN window;
{
/**************************************
 *
 *	V T 1 0 0 _ c r e a t e _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Create a new display window.
 *
 **************************************/
	UCHAR *p;
	int width, height;

	window->win_keyname_table = keyname_table;

/* Populate window with action routines */

	window->win_clear = clear_window;
	window->win_disable = disable;
	window->win_fini = fini;
	window->win_getchar = get_char;
	window->win_text = text;
	window->win_update = update_window;

	if ((int) (window->win_input = (int *) open("/dev/tty", O_RDWR, 0660)) <
		0)
		return FAILURE;
	else
		if (!
			(window->win_output =
			 (int *) fdopen((int) window->win_input, "w"))) return FAILURE;

	enable_forms(window);

	for (p = keypad_equiv; *p; p += 2)
		keypad[p[1]] = p[0];

/* If we can get the screen size, check if adjustment is
   necessary */

	if (get_size(window, &width, &height)) {
		if (window->win_width && window->win_width > width) {
			fputs("\33[?3h", (FILE *) window->win_output);
			window->win_flags |= WIN_reset_80;
			get_size(window, &width, &height);
		}
		window->win_height = (window->win_height) ?
			MIN(window->win_height, height) : height;
		window->win_width = (window->win_width) ?
			MIN(window->win_width, width) : width;
	}
	else {
		window->win_width = 80;
		window->win_height = 24;
	}

	return SUCCESS;
}


static clear_window(window)
	 WIN window;
{
/**************************************
 *
 *	c l e a r _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Zap a window clean.
 *
 **************************************/

/* Set normal characters, keypad application mode */

	fputs("\33[0m\33=", (FILE *) window->win_output);

/* Clear screen */

	fprintf((FILE *) window->win_output, "\33[2J");
}


static disable(window)
	 WIN window;
{
/**************************************
 *
 *	d i s a b l e
 *
 **************************************
 *
 * Functional description
 *	Make window disappear.
 *
 **************************************/

	disable_forms(window);
	window->win_flags |= WIN_disabled;
}


#ifndef LINUX
static disable_forms(window)
	 WIN window;
{
/**************************************
 *
 *	d i s a b l e _ f o r m s
 *
 **************************************
 *
 * Functional description
 *	Undo forms more for terminal.
 *
 **************************************/
	int n;

#ifndef VMS
	struct sgttyb cruft;

	n = ioctl(window->win_input, TIOCGETP, &cruft);

/* If CBREAK is not supported by ioctl, use RAW.  (for HP)  */

#ifndef CBREAK
	cruft.sg_flags &= ~RAW;		/* RAW off */
#else
	cruft.sg_flags &= ~CBREAK;	/* CBREAK off */
#endif

	cruft.sg_flags |= ECHO;		/* ECHO on */
	n = ioctl(window->win_input, TIOCSETP, &cruft);
#endif

/* Set normal characters, numeric keypad, non-graphics */

	fputs("\33[0m\33>\33(B\n", (FILE *) window->win_output);
	clear_window(window);
	fflush((FILE *) window->win_output);
	window->win_current_mode = 0;
}
#else /** Linux Stub **/
static disable_forms(window)
	 WIN window;
{
/**************************************
 *
 *	d i s a b l e _ f o r m s
 *
 **************************************
 *
 * Functional description
 *	stub for linux
 *
 **************************************/
}
#endif



#ifndef LINUX
static enable_forms(window)
	 WIN window;
{
/**************************************
 *
 *	e n a b l e _ f o r m s
 *
 **************************************
 *
 * Functional description
 *	Put terminal in forms mode.
 *
 **************************************/
	int n;

#ifndef VMS
	struct sgttyb cruft;

	n = ioctl(window->win_input, TIOCGETP, &cruft);

/* If CBREAK is not supported by ioctl, use RAW.  (for HP)  */

#ifndef CBREAK
	cruft.sg_flags |= RAW;		/* RAW on */
#else
	cruft.sg_flags |= CBREAK;	/* CBREAK on */
#endif

	cruft.sg_flags &= ~ECHO;	/* ECHO off */
	n = ioctl(window->win_input, TIOCSETP, &cruft);
#endif

	window->win_current_mode = 0;
	window->win_flags &= ~WIN_disabled;
	clear_window(window);
}
#else  /** stub function for LINUX **/
static enable_forms(window)
	 WIN window;
{
/**************************************
 *
 *	e n a b l e _ f o r m s
 *
 **************************************
 *
 * Functional description
 *	Stubb for LINUX
 *
 **************************************/
}
#endif


static fini(window)
	 WIN window;
{
/**************************************
 *
 *	f i n i
 *
 **************************************
 *
 * Functional description
 *	Sign off the screen package.
 *
 **************************************/

	if (window->win_flags & WIN_reset_80)
		fputs("\33[?3l", (FILE *) window->win_output);

	disable_forms(window);
		fclose((FILE *) window->win_output);
}


static get_char(window, x, y)
	 WIN window;
{
/**************************************
 *
 *	g e t _ c h a r
 *
 **************************************
 *
 * Functional description
 *	Get a character from the user.
 *
 **************************************/
	int n;
	SEQ *sequence;
	UCHAR buffer[16], *p, *q, c;

	position(window, x, y);
	fflush((FILE *) window->win_output);

	for (;;) {
		if (!(n = get_input(window, buffer)))
			fprintf(stderr, "read failed\n");
		if (n == 1)
			switch (buffer[0]) {
			case '\n':
			case '\r':
				return '\n';

			case 'T' - 0100:
				return C_SCROLL_TOP;
			case 'B' - 0100:
				return C_SCROLL_BOTTOM;
			case 'Z' - 0100:
				return C_EOF;

/* Edit mode */

			case 'F' - 0100:
				return C_DELETE_NEXT;
			case 'G' - 0100:
				return C_EDIT;
			case 'H' - 0100:
				return C_GOTO_START;
			case 'E' - 0100:
				return C_GOTO_END;
			case 'A' - 0100:
				return C_INSERT_OVERSTRIKE;
			case 'U' - 0100:
				return C_ERASE;
			default:
				return buffer[0];
			}
		buffer[n] = 0;
		for (sequence = escape_sequences; q = sequence->seq_sequence;
			 sequence++)
			for (p = buffer + 1; *p == *q; p++, q++)
				if (!*p)
					return sequence->seq_key;
	}
}


static get_input(window, buffer)
	 WIN window;
	 UCHAR *buffer;
{
/**************************************
 *
 *	g e t _ i n p u t
 *
 **************************************
 *
 * Functional description
 *	Get a single character or escape sequence from terminal.
 *	Return number of characters read.
 *
 **************************************/
	UCHAR *p, c;

	p = buffer;
	*p++ = c = read_char(window);

	if (!c)
		return 0;

	if (c != 033)
		return 1;

/* Escape or control sequence sequence coming in */

	*p++ = c = read_char(window);

/* If this a keypad character, handling it specially */

	if (c == 'O') {
		c = read_char(window);
		buffer[0] = keypad[c];
		return 1;
	}

/* Handle escape sequences and control sequences slightly differently */

	if (c >= ' ' && c < '/')
		for (;;) {
			*p++ = c = read_char(window);
			if (c >= '0')
				break;
		}
	else
		for (;;) {
			*p++ = c = read_char(window);
			if (c >= '@')
				break;
		}

	return p - buffer;
}


static get_size(window, width, height)
	 WIN window;
	 int *width, *height;
{
/**************************************
 *
 *	g e t _ s i z e
 *
 **************************************
 *
 * Functional description
 *	Try to get size of terminal.
 *
 **************************************/
	TEXT buffer[32];
	int n;

/* Position cursor to lower right and request report */

	position(window, 999, 999);
	fputs("\33[6n", (FILE *) window->win_output);
	fflush((FILE *) window->win_output);
	n = get_input(window, buffer);
	buffer[n] = 0;
	n = sscanf(buffer, "\33[%d;%dR", height, width);

	return (n == 2);
}


static position(window, x, y)
	 WIN window;
	 SSHORT x, y;
{
/**************************************
 *
 *	p o s i t i o n
 *
 **************************************
 *
 * Functional description
 *	Position cursor to given location.
 *
 **************************************/
	FILE *file;
	USHORT temp;
	UCHAR c;

	if (window->win_flags & WIN_disabled)
		enable_forms(window);

	file = (FILE *) window->win_output;
	fprintf(file, "\33[%d;%dH", y + 1, x + 1);
}


static read_char(window)
	 WIN window;
{
/**************************************
 *
 *	r e a d _ c h a r
 *
 **************************************
 *
 * Functional description
 *	Get a character from the user.
 *
 **************************************/
	SCHAR c;

	if (read(window->win_input, &c, 1) <= 0)
		return 0;

	return c;
}




static text(window, string, length, x, y, mode)
	 WIN window;
	 UCHAR *string;
	 SSHORT length, x, y;
	 USHORT mode;
{
/**************************************
 *
 *	t e x t
 *
 **************************************
 *
 * Functional description
 *	Insert text in a given location.
 *
 **************************************/
	FILE *file;
	USHORT temp, sw;
	UCHAR *end, c;

	position(window, x, y);
	file = (FILE *) window->win_output;

	if (mode != window->win_current_mode) {
		fputs("\33[0", file);
		if (mode & SCR_reverse)
			fputs(";7", file);
		if (mode & SCR_bold)
			fputs(";1", file);
		if (mode & SCR_underline)
			fputs(";4", file);
		fputs("m", file);
		window->win_current_mode = mode;
	}

	for (end = string + length; string < end; string++) {
		sw = TRUE;
		switch (c = *string) {
		case C_UL_CORNER:
			c = 'l';
			break;

		case C_UR_CORNER:
			c = 'k';
			break;

		case C_LL_CORNER:
			c = 'm';
			break;

		case C_LR_CORNER:
			c = 'j';
			break;

		case C_HORZ_BAR:
			c = 'q';
			break;

		case C_VERT_BAR:
			c = 'x';
			break;

		default:
			sw = FALSE;
		}
		if (sw != graphics_mode) {
			fputs((sw) ? "\33(0" : "\33(B", file);
			graphics_mode = sw;
		}
		putc(c, file);
	}

/*fflush (file);*/
}


static update_window(window, x, y)
	 WIN window;
	 int x, y;
{
/**************************************
 *
 *	u p d a t e _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Update a window leaving the cursor at a given object location.
 *
 **************************************/

	position(window, x, y);
}
