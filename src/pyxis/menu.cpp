/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		menu.c
 *	DESCRIPTION:	Menu manipulation routines
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
#include <ctype.h>

#include "../pyxis/pyxis.h"

ATT PYXIS_get_attribute(), PYXIS_put_attribute(), REPLACE_ATTRIBUTE(),
PYXIS_find_object(), PYXIS_navigate();

OBJ PYXIS_create_object(), PYXIS_get_attribute_value();

static ATT lookup_entree();




PYXIS_box(object)
{
/**************************************
 *
 *	P Y X I S _ b o x
 *
 **************************************
 *
 * Functional description
 *	Box a complete object.
 *
 **************************************/
	USHORT width, height;

	width = GET_VALUE(object, att_width);
	height = GET_VALUE(object, att_height);

	if (!width || !height) {
		PYXIS_compute_size(object, &width, &height);
		width += 3;
		height += 3;
	}

	REPLACE_ATTRIBUTE(object, att_width, attype_numeric, width);
	REPLACE_ATTRIBUTE(object, att_height, attype_numeric, height);
	REPLACE_ATTRIBUTE(object, att_box, attype_numeric, TRUE);
	REPLACE_ATTRIBUTE(object, att_blank, attype_numeric, TRUE);
	REPLACE_ATTRIBUTE(object, att_border, attype_numeric, 1);
}


ATT PYXIS_create_entree(menu, string, type, value)
	 OBJ menu;
	 TEXT *string;
	 BLK value;
{
/**************************************
 *
 *	P Y X I S _ c r e a t e _ e n t r e e
 *
 **************************************
 *
 * Functional description
 *	Add another entree to a menu.
 *
 **************************************/
	OBJ object;

	object = PYXIS_create_object(string, att_literal_string);

	if (type)
		PUT_ATTRIBUTE(object, att_entree_value, type, value);

	return PUT_ATTRIBUTE(menu, att_entree, attype_object, object);
}


OBJ PYXIS_drive_menu(window, menu, terminator)
	 WIN window;
	 OBJ menu;
	 USHORT *terminator;
{
/**************************************
 *
 *	P Y X I S _ d r i v e _ m e n u
 *
 **************************************
 *
 * Functional description
 *	Drive a menu, return selected object and terminator.
 *
 **************************************/
	ATT attribute, prior;
	OBJ entree;
	TEXT string[32], *p;
	USHORT c, direction, horizontal;

	*terminator = 0;
	PYXIS_scroll_reset(menu);

/* Reset all bolding */

	attribute = NULL;

	while (attribute = PYXIS_find_object(menu, attribute, att_entree, TRUE))
		REPLACE_ATTRIBUTE(attribute->att_value, att_reverse_video,
						  attype_numeric, 0);

	p = string;

/* If there is an initial character that uniquely identifies the entree,
   just return it immediately. */

	if (!attribute &&
		!(attribute = PYXIS_find_object(menu, 0, att_entree, TRUE)))
			return NULL;

	horizontal = (menu->obj_flags & OBJ_menu_horizontal);

	prior = NULL;

	for (;;) {
		entree = (OBJ) attribute->att_value;
		if (prior != attribute) {
			if (prior)
				REPLACE_ATTRIBUTE(prior->att_value, att_reverse_video,
								  attype_numeric, 0);
			REPLACE_ATTRIBUTE(entree, att_reverse_video, attype_numeric, 1);
		}
		c = PYXIS_get_char(window, entree, -1, 0);
		prior = attribute;

		switch (c) {
		case C_ENTER:
		case '\n':
			*terminator = c;
			return entree;

		case C_UP:
		case C_LEFT:
		case C_DOWN:
		case C_RIGHT:
			p = string;
			if (attribute = PYXIS_navigate(menu, attribute, att_entree, c))
				break;

			direction = (c == C_UP || c == C_LEFT) ? FALSE : TRUE;

			if (!(window->win_flags & WIN_synthesized)
				|| (horizontal && (c == C_RIGHT || c == C_LEFT))
				|| (!horizontal && (c == C_UP || c == C_DOWN)))
				attribute = PYXIS_find_object(menu, 0, att_entree, direction);
			else
				attribute = prior;

			break;

		case C_SCROLL_TOP:
			attribute = PYXIS_find_object(menu, 0, att_entree, TRUE);
			break;

		case C_SCROLL_BOTTOM:
			attribute = PYXIS_find_object(menu, 0, att_entree, FALSE);
			break;

		default:
			if (!isprint(c)) {
				*terminator = c;
				return entree;
			}

			*p++ = c;
			*p = 0;
			if (attribute = lookup_entree(menu, 0, string))
				break;
			p = string;
			*p++ = c;
			*p = 0;
			if (attribute = lookup_entree(menu, 0, string))
				break;
			p = string;
			attribute = prior;
			break;
		}
	}
}


PYXIS_format_menu(menu, string, horizontal)
	 OBJ menu;
	 TEXT *string;
{
/**************************************
 *
 *	P Y X I S _ f o r m a t _ m e n u
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	ATT attribute;
	OBJ object, label;
	USHORT count, x, y, length, delta_y, max_width, string_length;

/*  Clear out any previous formatting.  */

	REPLACE_ATTRIBUTE(menu, att_width, attype_numeric, 0);
	REPLACE_ATTRIBUTE(menu, att_height, attype_numeric, 0);
	REPLACE_ATTRIBUTE(menu, att_box, attype_numeric, FALSE);
	REPLACE_ATTRIBUTE(menu, att_blank, attype_numeric, FALSE);
	REPLACE_ATTRIBUTE(menu, att_border, attype_numeric, 0);


	string_length = max_width = 0;
	label = NULL;

/* If there is a title, make up an object to display it */

	if (string) {
		label = PYXIS_create_object(string, att_literal_string);
		REPLACE_ATTRIBUTE(menu, att_label, attype_object, label);
		string_length = max_width = strlen(string);
	}

/* Count objects and pick up length of the various entrees */

	attribute = NULL;
	count = 0;

	while (attribute = PYXIS_find_object(menu, attribute, att_entree, TRUE)) {
		++count;
		object = (OBJ) attribute->att_value;
		length = GET_VALUE(object, att_width);
		max_width = MAX(max_width, length);
	}

	max_width += 2;
	delta_y = (count < 10) ? 2 : 1;

/* Initial things for either vertical or horizontal formatting */

	x = y = 0;

	if (!horizontal)
		x = (max_width - string_length) / 2;

/* Position menu title */

	if (label) {
		REPLACE_ATTRIBUTE(label, att_display_x, attype_numeric, x);
		REPLACE_ATTRIBUTE(label, att_display_y, attype_numeric, y);
		if (horizontal)
			x += 2 + string_length;
		else
			y += 2;
	}

/* Format entrees */

	while (attribute = PYXIS_find_object(menu, attribute, att_entree, TRUE)) {
		object = (OBJ) attribute->att_value;
		length = GET_VALUE(object, att_width);
		if (!horizontal)
			x = (max_width - length) / 2;
		REPLACE_ATTRIBUTE(object, att_display_x, attype_numeric, x);
		REPLACE_ATTRIBUTE(object, att_display_y, attype_numeric, y);
		if (horizontal)
			x += 2 + length;
		else
			y += delta_y;
	}

	if (!horizontal)
		PYXIS_box(menu);
}


OBJ PYXIS_menu(window, menu)
	 WIN window;
	 OBJ menu;
{
/**************************************
 *
 *	P Y X I S _ m e n u
 *
 **************************************
 *
 * Functional description
 *	Drive a menu, return selected object.
 *
 **************************************/
	USHORT c;

	return PYXIS_drive_menu(window, menu, &c);

}


PYXIS_select(window, menu, attribute_name, start)
	 WIN window;
	 OBJ menu;
	 ATT_N attribute_name;
	 ATT start;
{
/**************************************
 *
 *	P Y X I S _ s e l e c t
 *
 **************************************
 *
 * Functional description
 *	Drive a menu, but allow selection of multiple items.  Selection
 *	for an entree is visually indicated by toggling reverse video.
 *	For now, the reverse video attribute also indicates selection.
 *
 **************************************/
	ATT attribute, prior;
	OBJ entree;
	TEXT string[32], *p;
	UCHAR c;

	if (!(attribute = start) &&
		!(attribute = PYXIS_find_object(menu, 0, attribute_name, TRUE)))
		return NULL;

	p = string;

	for (;;) {
		entree = (OBJ) attribute->att_value;
		c = PYXIS_get_char(window, entree, -1, 0);
		if (c == C_ENTER)
			return NULL;
		prior = attribute;
		switch (c) {
		case '\n':
			REPLACE_ATTRIBUTE(entree, att_reverse_video, attype_numeric,
							  !GET_VALUE(entree, att_reverse_video));
			break;

		case C_DOWN:
		case C_RIGHT:
		case C_UP:
		case C_LEFT:
			p = string;
			if (!
				(attribute =
				 PYXIS_navigate(menu, attribute, attribute_name,
								c))) attribute = prior;
			break;

		default:
			*p++ = c;
			*p = 0;
			if (attribute = lookup_entree(menu, 0, string))
				break;
			p = string;
			*p++ = c;
			*p = 0;
			if (attribute = lookup_entree(menu, 0, string))
				break;
			p = string;
			attribute = prior;
			break;
		}
	}
}


static ATT lookup_entree(menu, attribute, string)
	 OBJ menu;
	 ATT attribute;
	 TEXT *string;
{
/**************************************
 *
 *	l o o k u p _ e n t r e e
 *
 **************************************
 *
 * Functional description
 *	Lookup the next entree that matches a string.
 *
 **************************************/
	OBJ entree;
	TEXT *p, *q;

	while (attribute = PYXIS_find_object(menu, attribute, att_entree, TRUE)) {
		entree = (OBJ) attribute->att_value;
		if (!(p = GET_STRING(entree, att_literal_string)))
			continue;
		for (q = string; *p && *q; p++, q++)
			if (UPPER(*p) != UPPER(*q))
				break;
		if (!*q)
			return attribute;
	}

	return NULL;
}
