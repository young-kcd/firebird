/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		vdm.c
 *	DESCRIPTION:	VMS Display Manager
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

#include descrip
#include iodef

#include "../pyxis/pyxis.h"
#include "../pyxis/scr.h"

#define DISABLE		"\33[0m\33>\33(B\n"
#define ENABLE		"\33[0m\33="
#define CSI		0x9b

static int clear_window(), disable(), fini(), get_char(), text(),
update_window();
static SSHORT graphics_mode;

typedef struct seq {
	UCHAR seq_key;
	UCHAR *seq_sequence;
} SEQ;

static SEQ escape_sequences[] = {
	C_UP, "\33[A",
	C_DOWN, "\33[B",
	C_RIGHT, "\33[C",
	C_LEFT, "\33[D",
	C_UP, "\233A",
	C_DOWN, "\233B",
	C_RIGHT, "\233C",
	C_LEFT, "\233D",
	C_PF5, "\33[31~",			/* F17 function key */
	C_PF6, "\33[18~",			/* F7 function key */
	C_PF7, "\33[19~",			/* F8 function key */
	C_PF8, "\33[20~",			/* F9 function key */
	C_PF9, "\33[21~",			/* F10 function key */
	C_SCROLL_TOP, "\33[5~",		/* Prev Scrn */
	C_SCROLL_BOTTOM, "\33[6~",	/* Next Scrn */
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


VDM_create_window(window)
	 WIN window;
{
/**************************************
 *
 *	V D M _ c r e a t e _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Create a new display window.
 *
 **************************************/
	struct dsc$descriptor sysinput, locdev_dsc;
	UCHAR *p, buffer[32];
	SCHAR locdev[64];
	SLONG l, status, width, height;

	window->win_keyname_table = keyname_table;

/* Populate window with action routines */

	window->win_clear = &clear_window;
	window->win_disable = &disable;
	window->win_fini = &fini;
	window->win_getchar = &get_char;
	window->win_text = &text;
	window->win_update = &update_window;

	sysinput.dsc$w_length = sizeof("SYS$INPUT") - 1;
	sysinput.dsc$b_class = DSC$K_CLASS_S;
	sysinput.dsc$b_dtype = DSC$K_DTYPE_T;
	sysinput.dsc$a_pointer = "SYS$INPUT";

	locdev_dsc.dsc$w_length = 63;
	locdev_dsc.dsc$b_class = DSC$K_CLASS_S;
	locdev_dsc.dsc$b_dtype = DSC$K_DTYPE_T;
	locdev_dsc.dsc$a_pointer = &locdev[0];

/*
status = sys$trnlog(&sysinput, &l, &locdev_dsc, 0, 0, 0);
locdev[l] = 0;
strcpy (locdev, &locdev[4]);
locdev_dsc.dsc$w_length = sizeof (locdev);
status = sys$assign (&locdev_dsc, &trm_channel, 0, 0);
*/

	status = sys$assign(&sysinput, &window->win_input, 0, 0);
	enable_forms(window);

	for (p = keypad_equiv; *p; p += 2)
		keypad[p[1]] = p[0];

	if (get_size(window, &width, &height)) {
		if (window->win_width && window->win_width > width) {
			put_line(window, "\33[?3h", 5, 0);
			window->win_flags |= WIN_reset_80;
			get_size(window, &width, &height);
		}
		window->win_height = (window->win_height) ?
			MIN(window->win_height, height) : height;
		window->win_width = (window->win_width) ?
			MIN(window->win_width, width) : width;
	}
	else {
		window->win_width = 79;
		window->win_height = 23;
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
 *	Clear out a window.
 *
 **************************************/

/* Set normal characters, keypad application mode */

	put_line(window, ENABLE, sizeof(ENABLE) - 1, 0);

	lib$set_cursor(&(1), &(1));
	lib$erase_page();
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

/* Set normal characters, numeric keypad, non-graphics */

	put_line(window, DISABLE, sizeof(DISABLE) - 1, 0);
	position(window, 0, window->win_height);
	window->win_flags |= WIN_disabled;
}


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

	window->win_current_mode = 0;
	window->win_flags &= ~WIN_disabled;
	clear_window(window);
}



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
		put_line(window, "\33[?3l", 5, 0);

	disable(window);
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
	UCHAR buffer[16], *p, *q;

	position(window, x, y);

	for (;;) {
		if (!(n = get_input(window, buffer)))
			return 0;
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
			for (p = buffer; *p == *q; p++, q++)
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

	if (c == 0217) {
		c = read_char(window);
		buffer[0] = keypad[c];
		return 1;
	}

	if (c == 0233) {
		*p++ = read_char(window);
		return 2;
	}
	else if (c != 033)
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
	UCHAR buffer[32];
	int n;

/* Position cursor to lower right and request report */

	position(window, 999, 999);
	put_line(window, "\33[6n", 4, 0);
	n = get_input(window, buffer);
	buffer[n] = 0;
	n = sscanf(buffer, "\33[%d;%dR", height, width);

	return (n == 2);
}



static position(window, x, y)
	 WIN window;
	 USHORT x, y;
{
/**************************************
 *
 *	p o s i t i o n
 *
 **************************************
 *
 * Functional description
 *	Position cursor.
 *
 **************************************/
	SSHORT flag, x1, y1;

	if (window->win_flags & WIN_disabled)
		enable_forms(window);

	x1 = x + 1;
	y1 = y + 1;
	lib$set_cursor(&y1, &x1);
}


static put_line(window, string, length, flags)
	 WIN window;
	 UCHAR *string;
	 USHORT length;
	 USHORT flags;
{
/**************************************
 *
 *	p u t _ l i n e
 *
 **************************************
 *
 * Functional description
 *	Write out a bunch of stuff.
 *
 **************************************/
	struct dsc$descriptor text;

	text.dsc$w_length = length;
	text.dsc$b_class = DSC$K_CLASS_S;
	text.dsc$b_dtype = DSC$K_DTYPE_T;
	text.dsc$a_pointer = string;

	scr$put_line(&text, 0, flags);
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
	SSHORT iosb[4];
	int status;

#define FUNCTION 	IO$_READVBLK | IO$M_NOECHO | IO$M_NOFILTR

	status = sys$qiow(15,		/* Event flag */
					  window->win_input,	/* Channel */
					  FUNCTION,	/* Function */
					  iosb,		/* IO status block */
					  NULL,		/* AST address */
					  NULL,		/* AST parameter */
					  &c,		/* P1 (buffer) */
					  1,		/* P2 (length) */
					  NULL, NULL, NULL, NULL);	/* P3  - P6 */

	return c;
}


static text(window, string, length, x, y, mode)
	 WIN window;
	 UCHAR *string;
	 USHORT length, x, y;
	 USHORT mode;
{
/**************************************
 *
 *	t e x t
 *
 **************************************
 *
 * Functional description
 *	Display some text in a given rendition.
 *
 **************************************/
	SSHORT flag, sw;
	UCHAR c, *p, *end, buffer[1024];

	p = buffer;

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
			*p++ = 033;
			*p++ = '(';
			*p++ = (sw) ? '0' : 'B';
			graphics_mode = sw;
		}
		*p++ = c;
	}

	position(window, x, y);

	if (mode & SCR_reverse)
		flag = 2;
	else
		flag = 0;

	put_line(window, buffer, p - buffer, flag);
}


static update_window(window, x_offset, y_offset)
	 WIN window;
	 int x_offset, y_offset;
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
	USHORT x, y;

	x = x_offset + 1;
	y = y_offset + 1;
	lib$set_cursor(&y, &x);
}
