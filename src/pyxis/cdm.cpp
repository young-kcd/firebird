/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		cdm.c
 *	DESCRIPTION:	Curses Display Manager
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

#include <curses.h>
#undef DEBUG_CURSES
#ifdef DEBUG_CURSES
#include "../jrd/ib_stdio.h"
#endif
#include "../pyxis/pyxis.h"
#include "../pyxis/scr.h"

/* 
   Unfortunately, curses tends to be a very buggy system, so we have to provide
   what workarounds we can. Two categories of problems have been encountered:
   1. requiring initscr/endwin when all we want to do is temporarily stop using forms.
      this is triggered by defining DISABLE_BUG
   2. requiring that after endwin we explicitly return stdin to nonbuffered mode.
      this is triggered by define BUFF_MODE_BUG & DISABLE_BUG
*/

#ifdef hpux
#define DISABLE_BUG
#endif

#ifdef sun
#define DISABLE_BUG
#endif

#ifdef sgi
#define DISABLE_BUG
#define BUFF_MODE_BUG
#endif

#ifdef DELTA
#define DISABLE_BUG
#define BUFF_MODE_BUG
#endif

#ifdef IMP
#define DISABLE_BUG
#define BUFF_MODE_BUG
#endif

#ifdef DGUX
#define DISABLE_BUG
#define BUFF_MODE_BUG
#endif

#ifdef DEBUG_CURSES
static FILE *log_file;
#endif

#ifndef NETBSD
#define getmaxx(s) ((s)->_maxx)
#define getmaxy(s) ((s)->_maxy)
#endif

static USHORT disabled = 1;
static USHORT debug_curses = 0;

static int clear_window(), disable(), fini(), get_char(), text(),
update_window();

typedef struct seq {
	UCHAR seq_key;
	UCHAR *seq_sequence;
} SEQ;

static SEQ escape_sequences[] = {
	(UCHAR) C_UP, (UCHAR *) "[A",
	(UCHAR) C_DOWN, (UCHAR *) "[B",
	(UCHAR) C_RIGHT, (UCHAR *) "[C",
	(UCHAR) C_LEFT, (UCHAR *) "[D",
	(UCHAR) C_ENTER, (UCHAR *) "[222z",	/* R15 */
	(UCHAR) C_PF1, (UCHAR *) "[208z",	/* R1  */
	(UCHAR) C_PF2, (UCHAR *) "[209z",	/* R2  */
	(UCHAR) C_PF3, (UCHAR *) "[210z",	/* R3  */
	(UCHAR) C_PF4, (UCHAR *) "[211z",	/* R4  */
	(UCHAR) C_PF5, (UCHAR *) "[212z",	/* R5  */
	(UCHAR) C_PF6, (UCHAR *) "[213z",	/* R6  */
	(UCHAR) C_PF7, (UCHAR *) "[214z",	/* R7  */
	(UCHAR) C_UP, (UCHAR *) "[215z",	/* R8 - up arrow */
	(UCHAR) C_PF8, (UCHAR *) "[216z",	/* R9  */
	(UCHAR) C_LEFT, (UCHAR *) "[217z",	/* R10- left arrow */
	(UCHAR) C_PF9, (UCHAR *) "[218z",	/* R11 */
	(UCHAR) C_RIGHT, (UCHAR *) "[219z",	/* R12 - right arrow */
	(UCHAR) C_DOWN, (UCHAR *) "[221z",	/* R14 - down arrow */
	(UCHAR) C_ENTER, (UCHAR *) "[250z",	/* ENTER */
	NULL, NULL
};

#ifdef CURSES_KEYPAD
typedef struct kpd {
	UCHAR kpd_key;
	int kpd_value;
} KPD;

static KPD keypad_values[] = {
	(UCHAR) C_UP, KEY_UP,
	(UCHAR) C_DOWN, KEY_DOWN,
	(UCHAR) C_RIGHT, KEY_RIGHT,
	(UCHAR) C_LEFT, KEY_LEFT,
	(UCHAR) C_PF1, KEY_F(1),
	(UCHAR) C_PF2, KEY_F(2),
	(UCHAR) C_PF3, KEY_F(3),
	(UCHAR) C_PF4, KEY_F(4),
	(UCHAR) C_PF5, KEY_F(5),
	(UCHAR) C_PF6, KEY_F(6),
	(UCHAR) C_PF7, KEY_F(7),
	(UCHAR) C_PF8, KEY_F(8),
	(UCHAR) C_PF9, KEY_F(9),
	(UCHAR) C_SCROLL_TOP, KEY_SR,
	(UCHAR) C_SCROLL_BOTTOM, KEY_SF,
	(UCHAR) C_ENTER, KEY_HOME,
	(UCHAR) C_DELETE, KEY_DC,
	NULL, NULL
};
#endif

#ifdef CURSES_KEYPAD
static struct knm keyname_table[] = {
	{C_PF1, "<F1>"},
	{C_PF2, "<F2>"},
	{C_PF3, "<F3>"},
	{C_PF4, "<F4>"},
	{C_PF5, "<F5>"},
	{C_PF6, "<F6>"},
	{C_PF7, "<F7>"},
	{C_PF8, "<F8>"},
	{C_PF9, "<R11>"},
	{C_ENTER, "<HOME>"},
	{C_LEFT, "<left arrow>"},
	{C_RIGHT, "<right arrow>"},
	{C_UP, "<up arrow>"},
	{C_DOWN, "<down arrow>"},
	{C_DELETE, "<DELETE>"},
	{C_SCROLL_TOP, "<control-T>"},
	{C_SCROLL_BOTTOM, "<control-B>"},
	{C_ERASE, "<control-U>"},
	{C_DELETE_NEXT, "<control-F>"},
	{C_INSERT_OVERSTRIKE, "<control-A>"},
	{C_GOTO_START, "<control-H>"},
	{C_GOTO_END, "<control-E>"},
	{C_EDIT, "<control-G>"},
	{(UCHAR) 0, "<unknown key>"},
};

#else
static struct knm keyname_table[] = {
	{C_PF1, "<R1>"},
	{C_PF2, "<R2>"},
	{C_PF3, "<R3>"},
	{C_PF4, "<R4>"},
	{C_PF5, "<R5>"},
	{C_PF6, "<R6>"},
	{C_PF7, "<R7>"},
	{C_PF8, "<R9>"},
	{C_PF9, "<R11>"},
	{C_ENTER, "<ENTER> or <R15>"},
	{C_LEFT, "<R10>"},
	{C_RIGHT, "<R12>"},
	{C_UP, "<R8>"},
	{C_DOWN, "<R14>"},
	{C_DELETE, "<DELETE>"},
	{C_SCROLL_TOP, "<control-T>"},
	{C_SCROLL_BOTTOM, "<control-B>"},
	{C_ERASE, "<control-U>"},
	{C_DELETE_NEXT, "<control-F>"},
	{C_INSERT_OVERSTRIKE, "<control-A>"},
	{C_GOTO_START, "<control-H>"},
	{C_GOTO_END, "<control-E>"},
	{C_EDIT, "<control-G>"},
	{(UCHAR) 0, "<unknown key>"},
};
#endif

static UCHAR key_pad[256], keypad_equiv[] = {
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


CDM_create_window(window)
	 WIN window;
{
/**************************************
 *
 *	C D M _ c r e a t e _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Create a new display window.
 *
 **************************************/
	UCHAR *p;

	window->win_keyname_table = keyname_table;

/* Populate window with action routines */

	window->win_clear = clear_window;
	window->win_disable = disable;
	window->win_fini = fini;
	window->win_getchar = get_char;
	window->win_text = text;
	window->win_update = update_window;

#ifdef DEBUG_CURSES
	if (debug_curses)
		if (!(log_file = fopen("pyxis.log", "w")))
			printf("ERROR opening log file!\n");
#endif

/*
 work around bugs in HP & SUN curses by having enable/disable do
 initscr/endwin.
*/
#ifndef DISABLE_BUG
	initscr();
#ifdef DEBUG_CURSES
	if (debug_curses)
		fprintf(log_file, "initscr\n");
#endif
#ifdef CURSES_TERM
	saveterm();
#ifdef DEBUG_CURSES
	if (debug_curses)
		fprintf(log_file, "saveterm\n");
#endif
#endif
#endif

	enable(window);

	window->win_width = (window->win_width) ?
		MIN(getmaxx(stdscr), window->win_width) : getmaxx(stdscr);

	window->win_height = (window->win_height) ?
		MIN(getmaxy(stdscr), window->win_height) : getmaxy(stdscr);

	for (p = keypad_equiv; *p; p += 2)
		key_pad[p[1]] = p[0];

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

	enable();
	clear();
	wrefresh(stdscr);
#ifdef DEBUG_CURSES
	if (debug_curses) {
		fprintf(log_file, "clear\n");
		fprintf(log_file, "wrefresh\n");
	}
#endif
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
#ifdef BUFF_MODE_BUG
	SCHAR buf[BUFSIZ];
#endif

	if (disabled)
		return;

	wmove(stdscr, getmaxy(stdscr) - 1, 0);
	clear();
	wrefresh(stdscr);
	wstandend(stdscr);

#ifdef DEBUG_CURSES
	if (debug_curses) {
		fprintf(log_file, "wmove %d %d\n", stdscr->_maxy - 1, 0);
		fprintf(log_file, "clear\n");
		fprintf(log_file, "wrefresh\n");
		fprintf(log_file, "wstandend\n");
	}
#endif

#ifdef CURSES_TERM

	resetterm();
#ifdef DEBUG_CURSES
	if (debug_curses)
		fprintf(log_file, "resetterm\n");
#endif

#else

	echo();
	nocrmode();
#ifdef DEBUG_CURSES
	if (debug_curses) {
		fprintf(log_file, "echo\n");
		fprintf(log_file, "nocrmode\n");
	}
#endif
#ifdef CURSES_KEYPAD
	keypad(stdscr, FALSE);
#ifdef DEBUG_CURSES
	if (debug_curses)
		fprintf(log_file, "keypad FALSE\n");
#endif
#endif

#endif

#ifdef DISABLE_BUG
/*
 work around bugs in curses by having enable/disable do
 initscr/endwin.
*/
	endwin();
#ifdef BUFF_MODE_BUG
	setvbuf(stdout, buf, _IOLBF, BUFSIZ);
#endif
#ifdef DEBUG_CURSES
	if (debug_curses)
		fprintf(log_file, "endwin\n");
#endif
#endif

	disabled = TRUE;
}


static enable(window)
	 WIN window;
{
/**************************************
 *
 *	e n a b l e
 *
 **************************************
 *
 * Functional description
 *	Enable forms package.
 *
 **************************************/

	if (!disabled)
		return;

#ifdef DISABLE_BUG
/*
 work around bugs in curses by having enable/disable do
 initscr/endwin.
*/
	initscr();
#ifdef DEBUG_CURSES
	if (debug_curses)
		fprintf(log_file, "initscr\n");
#endif
#endif

#ifdef CURSES_TERM

	fixterm();
#ifdef DEBUG_CURSES
	if (debug_curses)
		fprintf(log_file, "fixterm\n");
#endif

#else

	noecho();
	crmode();
#ifdef DEBUG_CURSES
	if (debug_curses) {
		fprintf(log_file, "noecho\n");
		fprintf(log_file, "crmode\n");
	}
#endif

#endif

#ifdef CURSES_KEYPAD
	keypad(stdscr, TRUE);
#ifdef DEBUG_CURSES
	if (debug_curses)
		fprintf(log_file, "keypad TRUE\n");
#endif
#endif

	clear();
#ifdef DEBUG_CURSES
	if (debug_curses)
		fprintf(log_file, "clear\n");
#endif
	disabled = FALSE;
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

	disable(window);
	endwin();
#ifdef DEBUG_CURSES
	if (debug_curses)
		fprintf(log_file, "endwin\n");
#endif
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

/***
position (window, x, y);
fflush (window->win_output);
***/

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
#ifdef CURSES_KEYPAD
	KPD *kv;
#endif
	UCHAR *p;
	int c;

	p = buffer;
	c = read_char(window);

	if (!c)
		return 0;

#ifdef CURSES_KEYPAD
	if (c > 0400) {
		for (kv = keypad_values; kv->kpd_value; kv++)
			if (kv->kpd_value == c) {
				*p = kv->kpd_key;
				return 1;
			}
		/* treat everything else as a tab */
		*p = '\t';
		return 1;
	}
#endif

	if (c != 033) {
		*p = c;
		return 1;
	}

/* Escape or control sequence coming in */

	*p++ = c;
	*p++ = c = read_char(window);

/* If this a keypad character, handling it specially */

	if (c == 'O') {
		c = read_char(window);
		buffer[0] = key_pad[c];
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
	int c;

	if (disabled)
		enable(window);

	c = wgetch(stdscr);
#ifdef DEBUG_CURSES
	if (debug_curses)
		fprintf(log_file, "read %d\n", c);
#endif
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
	UCHAR *p, *end;

	for (p = string, end = p + length; p < end; p++)
		switch (*p) {
		case C_UL_CORNER:
		case C_UR_CORNER:
		case C_LL_CORNER:
		case C_LR_CORNER:
			*p = '+';
			break;

		case C_HORZ_BAR:
			*p = '-';
			break;

		case C_VERT_BAR:
			*p = '|';
			break;
		}

	if (disabled)
		enable(window);

	string[length] = 0;

	if (mode & SCR_reverse) {
#ifdef DEBUG_CURSES
		if (debug_curses)
			fprintf(log_file, "wstandout\n");
#endif
		wstandout(stdscr);
	}

	mvwaddstr(stdscr, y, x, string);
#ifdef DEBUG_CURSES
	if (debug_curses)
		fprintf(log_file, "mvwaddstr %d %d %s\n", y, x, string);
#endif

	if (mode & SCR_reverse) {
#ifdef DEBUG_CURSES
		if (debug_curses)
			fprintf(log_file, "wstandend\n");
#endif
		wstandend(stdscr);
	}
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

	if (disabled)
		enable(window);

	wmove(stdscr, y, x);
	wrefresh(stdscr);
#ifdef DEBUG_CURSES
	if (debug_curses) {
		fprintf(log_file, "wrefresh\n");
		fprintf(log_file, "wmove %d %d\n", y, x);
	}
#endif
}
