/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		scr.c
 *	DESCRIPTION:	Logical Screen Handler
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

static SCR create_screen();

static windows;

typedef struct {
	SSHORT min_x;
	SSHORT min_y;
	SSHORT max_x;
	SSHORT max_y;
} REGION;

#ifdef DEBUG
static debug;
#endif


SCR_clear_object(window, object)
	 WIN window;
	 OBJ object;
{
/**************************************
 *
 *	S C R _ c l e a r _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Clear the image of an object to blanks.
 *
 **************************************/

	clear_object(window->win_logical, object);
}


SCR_clear_window(window)
	 WIN window;
{
/**************************************
 *
 *	S C R _ c l e a r _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Clear a complete screen.
 *
 **************************************/
	SCR logical;

	logical = window->win_logical;
	logical->scr_flags |= SCR_clear;
	clear_screen(logical);
}


SCR_create_window(window)
	 WIN window;
{
/**************************************
 *
 *	S C R _ c r e a t e _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Set up a window.
 *
 **************************************/
	int status;

	++windows;

#ifdef VMS
#define WINDOW_CREATION
	status = VDM_create_window(window);
#endif

#ifndef WINDOW_CREATION
#ifndef sgi
#define WINDOW_CREATION
	if ((strcmp(getenv("TERM"), "vt100")) &&
		(strcmp(getenv("TERM"), "vt200")) &&
		(strcmp(getenv("TERM"), "vt300"))) status = CDM_create_window(window);
	else
		status = VT100_create_window(window);
#endif
#endif

#ifndef WINDOW_CREATION
	status = CDM_create_window(window);
#endif

	if (status)
		return status;

	window->win_logical =
		create_screen(window->win_width, window->win_height);
	window->win_physical =
		create_screen(window->win_width, window->win_height);
	(*window->win_clear) (window);

	return SUCCESS;
}


SCR_disable(window)
	 WIN window;
{
/**************************************
 *
 *	S C R _ d i s a b l e
 *
 **************************************
 *
 * Functional description
 *	Disable window.  Like make it disappear, go
 *	inactive, or just get out of the way.
 *
 **************************************/

	(*window->win_disable) (window);
	clear_screen(window->win_physical);
}


SCR_fini(window)
	 WIN window;
{
/**************************************
 *
 *	S C R _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Get rid of a screen.
 *
 **************************************/

	--windows;
	(*window->win_fini) (window);
	PYXIS_release(window->win_logical);
	PYXIS_release(window->win_physical);
}


SCR_getchar(window, object, x_offset, y_offset)
	 WIN window;
	 OBJ object;
	 int x_offset, y_offset;
{
/**************************************
 *
 *	S C R _ g e t c h a r
 *
 **************************************
 *
 * Functional description
 *	Update the physical screen based on the logical screen.
 *
 **************************************/
	SSHORT c, x, y;

	if (c = window->win_pending_char) {
		window->win_pending_char = 0;
		return c;
	}

	if (!object)
		return (*window->win_getchar) (window, x_offset, y_offset);

	x = object->obj_x + x_offset;
	y = object->obj_y + y_offset;

	return (*window->win_getchar) (window, x, y);
}


SCR_refresh_window(window, object, x_offset, y_offset)
	 WIN window;
	 OBJ object;
	 int x_offset, y_offset;
{
/**************************************
 *
 *	S C R _ r e f r e s h
 *
 **************************************
 *
 * Functional description
 *	Refresh the display, presumable because of extern interference.
 *
 **************************************/
	OBJ form;

	(*window->win_clear) (window);
	clear_screen(window->win_physical);
	form = window->win_form;
	form->obj_flags |= OBJ_changed;
	SCR_update_window(window, object, x_offset, y_offset);
}


SCR_update_window(window, object, x_offset, y_offset)
	 WIN window;
	 OBJ object;
	 int x_offset, y_offset;
{
/**************************************
 *
 *	S C R _ u p d a t e _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Update the physical screen based on the logical screen.
 *
 **************************************/
	SCR logical, physical;
	USHORT *lline, *pline, x, y, start, force;
	ATT attribute;
	REGION clip;

/* Start by updating logical screen */

	logical = window->win_logical;
	physical = window->win_physical;
	clip.min_x = 0;
	clip.min_y = 0;
	clip.max_x = window->win_width;
	clip.max_y = window->win_height;
	display(logical, clip, window->win_form, 0, FALSE);

#ifdef DEBUG
	if (debug) {
		PYXIS_print_screen(logical, "Logical");
		PYXIS_print_screen(physical, "Physical");
	}
#endif

/* If there has been a clear since the last update, zap the whole
   thing now. */

	if (logical->scr_flags & SCR_clear) {
		(*window->win_clear) (window);
		clear_screen(physical);
		logical->scr_flags &= ~SCR_clear;
	}

/* Look for differences between physical and logical screens.  Update
   physical screen */

	lline = logical->scr_screen;
	pline = physical->scr_screen;

	for (y = 0; y < logical->scr_height; y++,
		 pline += physical->scr_line_length, lline +=
		 logical->scr_line_length) if (*lline >> 8) {
			*lline = FALSE;
			update_line(window, y, lline + 1, pline + 1);
		}

	if (object) {
		x = object->obj_x + object->obj_border + x_offset;
		y = object->obj_y + object->obj_border + y_offset;
	}
	else {
		x = window->win_width;
		y = window->win_height;
	}

	(*window->win_update) (window, x, y);
}


static clear_box(screen, object)
	 SCR screen;
	 OBJ object;
{
/**************************************
 *
 *	c l e a r _ b o x
 *
 **************************************
 *
 * Functional description
 *	Display a box, possible outlined.
 *
 **************************************/
	USHORT *start, *p, *end, n, *line, l;

	start = object->obj_display_address;
	l = screen->scr_line_length;
	n = (start - screen->scr_screen) / l;
	line = screen->scr_screen + n * l;

	for (n = 0; n < object->obj_display_height; n++, line += l, start += l) {
		*line = -1;
		p = start;
		end = p + object->obj_display_length;
		while (p < end)
			*p++ = ' ';
	}

	return TRUE;
}



static clear_object(screen, object)
	 SCR screen;
	 OBJ object;
{
/**************************************
 *
 *	c l e a r _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Clear the image of an object to blanks.
 *
 **************************************/
	USHORT *line, *p;
	ATT attribute;
	SSHORT l;

	if (object->obj_occluded || (object->obj_flags & OBJ_inactive))
		return;

	object->obj_flags &= ~OBJ_displayed;

	if (object->obj_flags & OBJ_blank)
		clear_box(screen, object);

/* Check for, any display, and sub-objects */

	for (attribute = object->obj_attributes; attribute;
		 attribute = attribute->att_next)
			if (attribute->att_type == attype_object)
				clear_object(screen, attribute->att_value);

/* Next, clear this object, assuming it has an associated string */

	if (object->obj_display_attribute && (l = object->obj_display_length)) {
		line = screen->scr_screen + screen->scr_line_length * object->obj_y;
		*line = -1;
		p = object->obj_display_address;
		do
			*p++ = ' ';
		while (--l);
		object->obj_display_length = 0;
	}

	return TRUE;
}


static clear_screen(screen)
	 SCR screen;
{
/**************************************
 *
 *	c l e a r _ s c r e e n
 *
 **************************************
 *
 * Functional description
 *	Clear a complete screen.
 *
 **************************************/
	USHORT *p, *end;

	p = screen->scr_screen;
	end = p + screen->scr_length;

	while (p < end)
		*p++ = ' ';
}


static SCR create_screen(width, height)
	 USHORT width, height;
{
/**************************************
 *
 *	c r e a t e _ s c r e e n
 *
 **************************************
 *
 * Functional description
 *	Create a screen block of given dimension, and zero/blank
 *	same.
 *
 **************************************/
	USHORT length, line_length;
	SCR screen;

	line_length = width + 1;
	length = line_length * height;
	screen = (SCR) ALLOCDV(type_scr, length);
	screen->scr_width = width;
	screen->scr_height = height;
	screen->scr_length = length;
	screen->scr_line_length = line_length;
	clear_screen(screen);

	return screen;
}


static display(screen, clip, object, parent, force)
	 SCR screen;
	 REGION clip;
	 OBJ object, parent;
	 USHORT force;
{
/**************************************
 *
 *	d i s p l a y
 *
 **************************************
 *
 * Functional description
 *	Walk an object updating a screen.
 *
 **************************************/
	UCHAR *string;
	USHORT mode, *line, *p, *end, *min, *max;
	SSHORT l, n;
	ATT attribute;

	if (object->obj_occluded || (object->obj_flags & OBJ_inactive))
		return;

	if (object->obj_flags & OBJ_changed)
		force = TRUE;

	object->obj_x = object->obj_rel_x + object->obj_scroll_x;
	object->obj_y = object->obj_rel_y + object->obj_scroll_y;

	if (parent) {
		object->obj_x += parent->obj_x + parent->obj_border;
		object->obj_y += parent->obj_y + parent->obj_border;
	}

/* If object is box, clear it */


	if (force) {
		object->obj_display_length = 0;
		if (object->obj_flags & OBJ_blank)
			display_box(screen, clip, object);
	}

/* Compute clipping region */

	n = object->obj_x + object->obj_border;
	if (n > clip.min_x)
		clip.min_x = n;

	n = object->obj_y + object->obj_border;
	if (n > clip.min_y)
		clip.min_y = n;

	if (l = object->obj_width) {
		l += object->obj_x - object->obj_border;
		clip.max_x = MIN(clip.max_x, l);
	}

	if (l = object->obj_height) {
		l += object->obj_y - object->obj_border;
		clip.max_y = MIN(clip.max_y, l);
	}

/* Reset flags to indicate state */

	object->obj_flags |= OBJ_displayed;
	object->obj_flags &= ~OBJ_changed;

	if (force &&
		(attribute = object->obj_display_attribute) &&
		(string = (UCHAR *) attribute->att_value) &&
		object->obj_y >= clip.min_y &&
		object->obj_y < clip.max_y && clip.min_x < clip.max_x) {
		mode = 0;
		if (object->obj_flags & OBJ_reverse_video)
			mode |= SCR_reverse << 8;
		if (object->obj_flags & OBJ_bold)
			mode |= SCR_bold << 8;
		if (object->obj_flags & OBJ_underline)
			mode |= SCR_underline << 8;

		object->obj_width = MAX(attribute->att_length, object->obj_width);
		line = screen->scr_screen + screen->scr_line_length * object->obj_y;
		*line++ = -1;
		min = line + clip.min_x;
		max = line + clip.max_x;
		p = line + object->obj_x;
		object->obj_display_address = MAX(p, min);

		/* Handle leading blanks first */

		l = object->obj_width - attribute->att_length;

		if (l && (object->obj_flags & OBJ_insert_right))
			for (; l && p < max; --l, ++p)
				if (p >= min)
					*p = mode + ' ';

		/* Handle main string */

		for (; *string && p < max; p++, string++)
			if (p >= min)
				*p = mode + *string;

		/* Finish up with any trailing blanks */

		for (; l && p < max; --l, ++p)
			if (p >= min)
				*p = mode + ' ';

		p = MIN(p, max);
		l = p - object->obj_display_address;
		object->obj_display_length = (l >= 0) ? l : 0;
	}

/* Check for, any display, and sub-objects */

	for (attribute = object->obj_attributes; attribute;
		 attribute = attribute->att_next)
			if (attribute->att_type == attype_object)
				display(screen, clip, attribute->att_value, object, force);

	return TRUE;
}


static display_box(screen, clip, object)
	 SCR screen;
	 REGION clip;
	 OBJ object;
{
/**************************************
 *
 *	d i s p l a y _ b o x
 *
 **************************************
 *
 * Functional description
 *	Display a box, possible outlined.
 *
 **************************************/
	SSHORT min_x, min_y, max_x, max_y, x, y, right, bottom,
		mode, *p, *end, c, top, side, fill, box;

	box = (object->obj_flags & OBJ_box) ? TRUE : FALSE;
	right = object->obj_x + object->obj_width;
	bottom = object->obj_y + object->obj_height;
	min_x = MAX(object->obj_x, clip.min_x);
	min_y = MAX(object->obj_y, clip.min_y);
	max_x = MIN(right, clip.max_x);
	max_y = MIN(bottom, clip.max_y);

	if (min_x >= max_x || min_y >= max_y)
		return;

	object->obj_display_length = max_x - min_x;
	object->obj_display_height = max_y - min_y;
	object->obj_display_address = screen->scr_screen +
		screen->scr_line_length * min_y + 1 + min_x;

	mode = (object->obj_flags & OBJ_reverse_video) ? SCR_reverse << 8 : 0;
	fill = ' ' | mode;
	side = C_VERT_BAR | mode;
	top = C_HORZ_BAR | mode;
	--bottom;

/* Start by blanking the box */

	for (y = min_y; y < max_y; y++) {
		c = fill;
		if (box && (y == object->obj_y || y == bottom))
			c = top;
		p = (SSHORT *) screen->scr_screen + screen->scr_line_length * y;
		*p++ = -1;
		p += min_x;
		end = p + object->obj_display_length;
		if (box && min_x == object->obj_x)
			*p++ = side;
		while (p < end)
			*p++ = c;
		if (box && max_x == right)
			p[-1] = side;
	}

/* Unless the object is explicitly outlined, we're done */

	if (!(object->obj_flags & OBJ_box))
		return;

/* Groan -- fill in the corners */

	set_point(object->obj_x, object->obj_y, clip, screen, C_UL_CORNER | mode);
	set_point(object->obj_x + object->obj_width - 1, object->obj_y,
			  clip, screen, C_UR_CORNER | mode);
	set_point(object->obj_x, object->obj_y + object->obj_height - 1,
			  clip, screen, C_LL_CORNER | mode);
	set_point(object->obj_x + object->obj_width - 1,
			  object->obj_y + object->obj_height - 1, clip, screen,
			  C_LR_CORNER | mode);
}


static set_point(x, y, clip, screen, value)
	 USHORT x, y;
	 REGION clip;
	 SCR screen;
	 USHORT value;
{
/**************************************
 *
 *	s e t _ p o i n t
 *
 **************************************
 *
 * Functional description
 *	Set a single cell on a screen if it falls within a region.
 *
 **************************************/
	USHORT *p;

	if (x < clip.min_x ||
		x >= clip.max_x || y < clip.min_y || y >= clip.max_y) return FALSE;

	p = screen->scr_screen + screen->scr_line_length * y + 1 + x;
	*p = value;

	return TRUE;
}


static update_line(window, y, logical, physical)
	 WIN window;
	 USHORT y, *logical, *physical;
{
/**************************************
 *
 *	u p d a t e _ l i n e
 *
 **************************************
 *
 * Functional description
 *	Compare a logical line to the physical screen image and make
 *	any appropriate screen calls.
 *
 **************************************/
	USHORT *end, c, mode, x, start, width;
	UCHAR buffer[1024], *p;

	width = window->win_logical->scr_width;
	p = buffer;

	for (x = 0; x < width; x++, physical++) {
		c = *logical++;
		if (c == *physical) {
			if (p == buffer)
				continue;
			if (c != ' ' || *logical == physical[1]) {
				(*window->win_text) (window, buffer, p - buffer, start, y, mode);
				p = buffer;
				continue;
			}
		}
		if (p == buffer) {
			start = x;
			mode = c >> 8;
		}
		else if (c >> 8 != mode) {
			(*window->win_text) (window, buffer, p - buffer, start, y, mode);
			p = buffer;
			start = x;
			mode = c >> 8;
		}
		*p++ = c;
		*physical = c;
	}

	if (p != buffer) {
		(*window->win_text) (window, buffer, p - buffer, start, y, mode);
		p = buffer;
	}
}
