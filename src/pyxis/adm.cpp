/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		adm.c
 *	DESCRIPTION:	Apollo Display Manager
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

#include "../pyxis/pyxis.h"
#include "../pyxis/scr.h"

#include <apollo/base.h>
#include <apollo/gpr.h>
#include <apollo/pad.h>
#include <apollo/kbd.h>
#include <apollo/error.h>
#include "/sys/ins/smdu.ins.c"
#include "/sys/ins/streams.ins.c"

#define KEYSET(keyset, key) keyset [7 - key / 32] |= 1 << (key % 32)
#define CHECK_STATUS	if (status.all) dsply_error (status)

static stream_$id_t stream;
static status_$t status;
static gpr_$bitmap_desc_t bitmap, fill_bitmap;
static gpr_$attribute_desc_t fill_attributes;
static gpr_$event_t locator_event = gpr_$locator;
static boolean gpr_true = -1, gpr_false = 0;

static SCHAR beep[] = { 7 };
static UCHAR special_keys[] = {
	KBD_$BS,
	KBD_$CR,
	KBD_$F1,
	KBD_$F2,
	KBD_$F3,
	KBD_$F4,
	KBD_$F5,
	KBD_$F6,
	KBD_$F7,
	KBD_$F1S,					/* maps to C_PF8   */
	KBD_$F2S,					/* maps to C_PF9   */
	KBD_$F9,					/*    ditto        */
	KBD_$F8,					/* maps to C_ENTER */
	KBD_$TAB,
	KBD_$RIGHT_ARROW,
	KBD_$LEFT_ARROW,			/* cursor keys */
	KBD_$UP_ARROW,
	KBD_$DOWN_ARROW,
	KBD_$DOWN_BOX_ARROW2,		/* maps to SCROLL_BOTTOM */
	KBD_$UP_BOX_ARROW2,			/* maps to SCROLL_TOP    */
	KBD_$NUMERIC_KEYPAD,
	KBD_$LINE_DEL,				/* field editing */
	KBD_$CHAR_DEL,				/* field editing */
	KBD_$INS_MODE1,				/* field editing */
	KBD_$INS_MODE2,				/* field editing */
	KBD_$L_BAR_ARROW,			/* field editing */
	KBD_$R_BAR_ARROW,			/* field editing */
	KBD_$EDIT,					/* field editing */
	0
};

static struct knm keyname_table[] = {
	{C_PF1, "<F1>"},
	{C_PF2, "<F2>"},
	{C_PF3, "<F3>"},
	{C_PF4, "<F4>"},
	{C_PF5, "<F5>"},
	{C_PF6, "<F6>"},
	{C_PF7, "<F7>"},
	{C_PF8, "<shift-F1>"},
	{C_PF9, "<F9> or <shift-F2>"},
	{C_ENTER, "<ENTER> or <F8>"},
	{C_LEFT, "<left arrow>"},
	{C_RIGHT, "<right arrow>"},
	{C_UP, "<up arrow>"},
	{C_DOWN, "<down arrow>"},
	{C_DELETE, "<BACKSPACE>"},
	{C_SCROLL_TOP, "<boxed up arrow> or <control-T>"},
	{C_SCROLL_BOTTOM, "<boxed down arrow> or <control-B>"},
	{C_ERASE, "<LINE DEL>"},
	{C_DELETE_NEXT, "<CHAR DEL>"},
	{C_INSERT_OVERSTRIKE, "<INS>"},
	{C_GOTO_START, "<left bar arrow>"},
	{C_GOTO_END, "<right bar arrow>"},
	{C_EDIT, "<EDIT>"},
	{(UCHAR) 0, "<unknown key>"},
};

static int clear_window(), disable(), fini(), get_char(), text(),
update_window();

static SCHAR pending_keystroke;
static SSHORT button1_down = 0, button2_down = 0, button3_down = 0;
static SSHORT text_reverse;
static SSHORT char_width, char_height, char_offset, disabled;
static SSHORT mouse_delta_width, mouse_delta_height;
static gpr_$raster_op_t current_raster_op;

#define COND_ACQUIRE()		acquire()
#define COND_RELEASE()		release()



ADM_create_window(window)
	 WIN window;
{
/**************************************
 *
 *	A D M _ c r e a t e _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Create a new display window.
 *
 **************************************/
	gpr_$offset_t size;
	gpr_$position_t position;
	pad_$string_t font_name;
	pad_$window_desc_t window_desc;
	gpr_$event_t event;
	gpr_$keyset_t keyset;
	gpr_$display_config_t config;
	SSHORT font_name_length, font_id, n, x_size, y_size;
	SCHAR event_data;

	window->win_keyname_table = keyname_table;

/* Populate window with action routines */

	window->win_clear = clear_window;
	window->win_disable = disable;
	window->win_fini = fini;
	window->win_getchar = get_char;
	window->win_text = text;
	window->win_update = update_window;

/* Find the current font and character size */

	pad_$inq_font(stream_$stdout, &char_width, &char_height,
				  font_name, (SSHORT) sizeof(font_name), &font_name_length,
				  &status);
	if (status.all) {
		strcpy(font_name, "/sys/dm/fonts/f7x13");
		font_name_length = strlen(font_name);
		char_width = 9;
		char_height = 19;
	}

/*  Adjust mouse sensitivity based on character size.  */

	mouse_delta_width = 3 * char_width;
	mouse_delta_height = 2 * char_height;

	pad_$inq_windows(stream_$stdout, &window_desc, (SSHORT) 1, &n, &status);
	if (status.all)
		return status.all;

/* Adjust window size to reality */

	gpr_$inq_config(&config, &status);
	CHECK_STATUS;

	switch (config) {
	case gpr_$bw_800x1024:
		x_size = 800;
		y_size = 1024;
		break;

	case gpr_$bw_1280x1024:
	case gpr_$color_1280x1024x8:
	case gpr_$color2_1280x1024x8:
		x_size = 1280;
		y_size = 1024;
		break;

	case gpr_$color_1024x1024x4:
	case gpr_$color_1024x1024x8:
		x_size = 1024;
		y_size = 1024;
		break;

	default:
		x_size = 1024;
		y_size = 800;
		break;
	}

/* Create a window of the correct size, allowing for the legend and border. */

	x_size = (x_size / char_width) - 1;
	y_size = (y_size / char_height) - 3;

	if (!window->win_width || x_size < window->win_width)
		window->win_width = x_size;

	if (!window->win_height || y_size < window->win_height)
		window->win_height = y_size;

	window_desc.top = 0;
	window_desc.left = 0;
	window_desc.width = (window->win_width + 1) * char_width;
	window_desc.height = (window->win_height + 3) * char_height;
	pad_$create_window((SCHAR *) NULL, (SSHORT) 0, pad_$transcript,
					   (SSHORT) 1, window_desc, &stream, &status);

	if (status.all)
		return status.all;

/* don't check status for these two commands, we will continue
   if they fail, anyway */
	pad_$dm_cmd(stream, "TN", (SSHORT) 2, &status);
	pad_$set_auto_close(stream, (SSHORT) 1, gpr_true, &status);

	size.x_size = window->win_width * char_width;
	size.y_size = window->win_height * char_height;

/* Initialize the graphics primitives on the window */

	gpr_$init(gpr_$direct, stream, size, (unsigned short) 0, &bitmap,
			  &status);
	CHECK_STATUS;
	gpr_$set_bitmap(bitmap, &status);

/* Find the current font and character size */

	gpr_$load_font_file(font_name, font_name_length, &font_id, &status);
	gpr_$set_text_font(font_id, &status);
	gpr_$set_raster_op(0, 0, &status);
	gpr_$set_text_value((gpr_$pixel_value_t) 1, &status);
	gpr_$set_text_background_value((gpr_$pixel_value_t) - 1, &status);
	gpr_$inq_text_offset("M", 1, &size, &n, &status);
	char_offset = size.y_size;
	text_reverse = FALSE;

/* Turn on "pop if obscured" and "auto-refresh" to save work */

	gpr_$set_obscured_opt(gpr_$block_if_obs, &status);
	gpr_$set_auto_refresh(gpr_true, &status);

/* Enable the events of interest (i.e. left button up) */

	enable_events();

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

	acquire(window);
	gpr_$clear((gpr_$pixel_value_t) - 2, &status);
	release();
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

	clear_window(window);

	if (stream != stream_$stdout) {
		disabled = TRUE;
		pad_$make_invisible(stream, (SSHORT) 1, &status);
		CHECK_STATUS;
	}
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
	gpr_$position_t position;
	UCHAR event_data;
	gpr_$event_t event;
	SSHORT count;

/* Gobble any pending events to avoid gross confusion when the window
   disappears and the context changes. */

	COND_ACQUIRE();

	do
		gpr_$cond_event_wait(&event, &event_data, &position, &status);
	while (!status.all && event != gpr_$no_event);

	force_release();

	if (stream != stream_$stdout) {
		gpr_$terminate(gpr_true, &status);
		stream_$delete(stream, status);
	}
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
	gpr_$position_t position, current_position;
	gpr_$event_t event;
	unsigned char event_data, keypad_flag;
	status_$t temp;
	int delta_x, delta_y, abs_delta_x, abs_delta_y;
	int delta_width, delta_height;

	position.x_coord = x * char_width;
	position.y_coord = y * char_height + char_offset;

	window->win_flags &= ~WIN_synthesized;
	keypad_flag = FALSE;

	COND_ACQUIRE();
	gpr_$set_cursor_position(position, &status);

	for (;;) {
		gpr_$set_cursor_active(gpr_true, &temp);
		gpr_$event_wait(&event, &event_data, &current_position, &status);
		gpr_$set_cursor_active(gpr_false, &temp);
		if (status.all) {
			if ((status.all & 0xffffff) == smd_$wait_quit)
				continue;
			dsply_error(status);
		}
		if (event == gpr_$keystroke)
			if (event_data == KBD_$NUMERIC_KEYPAD)
				keypad_flag = TRUE;
			else
				break;
		if (event == gpr_$locator_stop) {
			gpr_$set_cursor_position(position, &status);
			continue;
		}

/*  Synthesize enter and return on the mouse.  */

		if (event == gpr_$buttons) {
			if (event_data == KBD_$M1U)
				keypad_flag = TRUE;
			else if (event_data == KBD_$M3U)
				keypad_flag = FALSE;
			else
				continue;
			event_data = KBD_$CR;
			window->win_flags |= WIN_synthesized;
			break;
		}

/*  Synthesize cursor keys for mouse movement.  Make mouse sensitivity depend on
 *  the window sensitivity flag.  */

		if (event == gpr_$locator) {
			abs_delta_x = delta_x =
				current_position.x_coord - position.x_coord;
			if (abs_delta_x < 0)
				abs_delta_x = -abs_delta_x;
			abs_delta_y = delta_y =
				current_position.y_coord - position.y_coord;
			if (abs_delta_y < 0)
				abs_delta_y = -abs_delta_y;
			if (window->win_flags & WIN_sensitive) {
				delta_width = 4;
				delta_height = 8;
			}
			else {
				delta_width = mouse_delta_width;
				delta_height = mouse_delta_height;
			}

/*  If a diagonal movement is called for, move horizontally and cache the
 *  vertical move for the next input.  */

			if (abs_delta_x >= delta_width) {
				if (delta_x > 0)
					event_data = KBD_$RIGHT_ARROW;
				else
					event_data = KBD_$LEFT_ARROW;
				window->win_flags |= WIN_synthesized;
			}
			if (abs_delta_y >= delta_height) {
				if (delta_y > 0)
					if (window->win_flags & WIN_synthesized)
						window->win_pending_char = C_DOWN;
					else
						event_data = KBD_$DOWN_ARROW;
				else if (window->win_flags & WIN_synthesized)
					window->win_pending_char = C_UP;
				else
					event_data = KBD_$UP_ARROW;
				window->win_flags |= WIN_synthesized;
			}
			if (window->win_flags & WIN_synthesized)
				break;
		}
	}

	COND_RELEASE();

	switch (event_data) {
	case KBD_$CR:
		return (keypad_flag) ? C_ENTER : '\n';
	case KBD_$TAB:
		return '\t';
	case KBD_$BS:
		return C_DELETE;
	case KBD_$F1:
		return C_PF1;
	case KBD_$F2:
		return C_PF2;
	case KBD_$F3:
		return C_PF3;
	case KBD_$F4:
		return C_PF4;
	case KBD_$F5:
		return C_PF5;
	case KBD_$F6:
		return C_PF6;
	case KBD_$F7:
		return C_PF7;
	case KBD_$F1S:
		return C_PF8;
	case KBD_$F9:
	case KBD_$F2S:
		return C_PF9;
	case KBD_$F8:
		return C_ENTER;
	case KBD_$RIGHT_ARROW:
		return C_RIGHT;
	case KBD_$LEFT_ARROW:
		return C_LEFT;
	case KBD_$UP_ARROW:
		return C_UP;
	case KBD_$DOWN_ARROW:
		return C_DOWN;
	case KBD_$UP_BOX_ARROW2:
	case 'T' - 0100:
		return C_SCROLL_TOP;
	case KBD_$DOWN_BOX_ARROW2:
	case 'B' - 0100:
		return C_SCROLL_BOTTOM;

/* field editing keys */

	case KBD_$LINE_DEL:
		return C_ERASE;
	case KBD_$CHAR_DEL:
		return C_DELETE_NEXT;
	case KBD_$INS_MODE1:
	case KBD_$INS_MODE2:
		return C_INSERT_OVERSTRIKE;
	case KBD_$L_BAR_ARROW:
		return C_GOTO_START;
	case KBD_$R_BAR_ARROW:
		return C_GOTO_END;
	case KBD_$EDIT:
		return C_EDIT;
	}

	return event_data;
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
	UCHAR *p, *end;
	TEXT buffer[512];
	USHORT special, bottom, side, op;
	SSHORT y_char, x1, x2, y1, y2, box_x, box_y;

	if (length == 0)
		return;

	special = FALSE;

	for (p = (UCHAR *) buffer, end = string + length; string < end;
		 string++, p++)
		switch (*p = *string) {
		case C_UL_CORNER:
		case C_UR_CORNER:
		case C_LL_CORNER:
		case C_LR_CORNER:
		case C_HORZ_BAR:
		case C_VERT_BAR:
			*p = ' ';
			special = TRUE;
		}

	string -= length;
	x *= char_width;
	y = y * char_height;
	acquire(window);
	op = 0;

	if (mode & SCR_reverse) {
		op = 15;
		if (!text_reverse) {
			gpr_$set_text_value((gpr_$pixel_value_t) 0, &status);
			text_reverse = TRUE;
		}
	}
	else if (text_reverse) {
		gpr_$set_text_value((gpr_$pixel_value_t) 1, &status);
		text_reverse = FALSE;
	}

	raster_op(op, x - 1, y - 1, length * char_width + 1, char_height);
	y_char = y + char_offset;
	gpr_$move(x, y_char, &status);
	CHECK_STATUS;
	gpr_$text(buffer, length, &status);
	CHECK_STATUS;

/* If no special drawing stuff, we're done */

	if (!special) {
		release();
		return;
	}

	current_raster_op = 3;
	gpr_$set_raster_op(0, current_raster_op, &status);
	box_x = x + char_width / 2;
	box_y = y + char_height / 2;
	bottom = y + char_height - 2;

	for (p = string; p < end; p++, x += char_width, box_x += char_width) {
		side = x + char_width;
		switch (*p) {
		case C_UL_CORNER:
			x1 = box_x;
			y1 = bottom;
			x2 = side;
			y2 = box_y;
			break;

		case C_LL_CORNER:
			x1 = box_x;
			y1 = y - 2;
			x2 = side;
			y2 = box_y;
			break;

		case C_UR_CORNER:
			x1 = x;
			y1 = box_y;
			x2 = box_x;
			y2 = bottom;
			break;

		case C_LR_CORNER:
			x1 = x;
			y1 = box_y;
			x2 = box_x;
			y2 = y - 2;
			break;

		case C_HORZ_BAR:
			x1 = x;
			y1 = box_y;
			x2 = side;
			y2 = box_y;
			break;

		case C_VERT_BAR:
			x1 = box_x;
			y1 = y - 2;
			x2 = box_x;
			y2 = bottom;
			break;

		default:
			continue;
		}
		gpr_$move(x1, y1, &status);
		if (x1 != x2 && y1 != y2)
			gpr_$line(box_x, box_y, &status);
		gpr_$line(x2, y2, &status);
	}

	release();
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

	dsply_restore_cursor(x * char_width, y * char_height);
}


static dsply_error(status)
	 status_$t status;
{
/**************************************
 *
 *	d s p l y _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Complain about system errors.
 *
 **************************************/

	error_$print(status);
}


static dsply_restore_cursor(x, y)
	 SSHORT x, y;
{
/**************************************
 *
 *	d s p l y _ r e s t o r e _ c u r s o r
 *
 **************************************
 *
 * Functional description
 *	Cursor has gotton lost or something.  Move it to an agreeable
 *	spot and re-enable it.
 *
 **************************************/
	gpr_$position_t position;

	pad_$select_window(stream, 1, &status);
	position.x_coord = x;
	position.y_coord = y + char_offset;
	gpr_$set_cursor_position(position, &status);
}


#ifdef UNDEFINED
static dsply_window(file_name, x, y, characters, lines)
	 SCHAR *file_name;
	 SSHORT x, y, characters, lines;
{
/**************************************
 *
 *	d s p l y _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Create a window at position (x, y) "characters" wide to display
 *	a file "lines" SLONG.  It it doesn't fit, do something appropriate.
 *
 *	5/31/90 - jas - routine not referenced, conditionalize out
 *
 **************************************/
	pad_$window_desc_t window;
	stream_$id_t stream_id;
	SCHAR *p, *q, buffer[256];
	SSHORT left, right;
	USHORT length;

	length = characters * 7;
	left = MAX(10, x - length / 2);
	right = MIN(990, x + length / 2);
	window.top = y + 20;
	window.left = left;
	window.width = right - left;
	window.height = MIN(lines * 15, (770 - y));

	for (p = file_name, length = 0, q = buffer; *p;) {
		length++;
		*q++ = *p++;
	}

	pad_$create_window(buffer, length, pad_$read_edit, (SSHORT) 1, window,
					   &stream_id, &status);
	CHECK_STATUS;
	stream_$close(stream_id, status);
	CHECK_STATUS;
}
#endif


static acquire(window)
	 WIN window;
{
/**************************************
 *
 *	a c q u i r e
 *
 **************************************
 *
 * Functional description
 *	Acquire the display.
 *
 **************************************/

	if (disabled) {
		pad_$select_window(stream, (SSHORT) 1, &status);
		disabled = FALSE;
	}

	gpr_$acquire_display(&status);
	CHECK_STATUS;
}


static enable_events()
{
/**************************************
 *
 *	e n a b l e _ e v e n t s
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	gpr_$keyset_t keyset;
	SSHORT i, key;
	UCHAR *string;

	for (i = 0; i < 8; i++)
		keyset[i] = 0;

/*  Grab mouse buttons 1 and 3.  */

	KEYSET(keyset, KBD_$M1D);
	KEYSET(keyset, KBD_$M1U);
	KEYSET(keyset, KBD_$M3D);
	KEYSET(keyset, KBD_$M3U);

	gpr_$enable_input(gpr_$buttons, keyset, &status);
	CHECK_STATUS;
	gpr_$enable_input(gpr_$locator_stop, keyset, &status);
	CHECK_STATUS;
	gpr_$enable_input(gpr_$locator, keyset, &status);
	CHECK_STATUS;

	for (i = 0; i < 127; i++)
		KEYSET(keyset, i);

	string = special_keys;

	while (key = *string++)
		KEYSET(keyset, key);

	gpr_$enable_input(gpr_$keystroke, keyset, &status);
	CHECK_STATUS;
}


static force_release()
{
/**************************************
 *
 *	f o r c e _ r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release display.
 *
 **************************************/
	SSHORT count;

	gpr_$force_release(&count, &status);
}


static raster_op(op, x, y, x_size, y_size)
	 USHORT op;
	 SSHORT x, y, x_size, y_size;
{
/**************************************
 *
 *	r a s t e r _ o p
 *
 **************************************
 *
 * Functional description
 *	Perform a raster operation on a rectangular region.
 *
 **************************************/
	gpr_$window_t window;
	gpr_$position_t destination;
	SSHORT unit;

	if (op != current_raster_op) {
		gpr_$set_raster_op(0, op, &status);
		current_raster_op = op;
	}

	window.window_base.x_coord = x;
	window.window_base.y_coord = y;
	window.window_size.x_size = x_size;
	window.window_size.y_size = y_size;
	destination.x_coord = x;
	destination.y_coord = y;
	gpr_$pixel_blt(bitmap, window, destination, &status);
	CHECK_STATUS;
}


static release()
{
/**************************************
 *
 *	r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release display.
 *
 **************************************/

	gpr_$release_display(&status);
	CHECK_STATUS;
}
