/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		pyxis.c
 *	DESCRIPTION:	External entrypoints
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
#define PYXIS_SOURCE
#include "../pyxis/everything.h"
#include "../pyxis/pyxis.h"
#include "../pyxis/scr.h"
#include "../jrd/gds_proto.h"

#include <string.h>

#define BLKCHK(block, type)	if (((BLK) block)->blk_type != (SCHAR)type) PYXIS_bugcheck ("bad block");
#define ABS(n)			((n >= 0) ? n : -n)

static WIN PYXIS_window;
static LLS PYXIS_handlers;

static int cleanup();
static int auto_scroll(OBJ , int , int );
static int clear_change(OBJ );
static int get_enumerated(WIN , OBJ , ATT_N , OBJ , USHORT *);
static int related(OBJ , OBJ );
static int reset_attribute(OBJ , ATT_N );
static int scroll(OBJ , USHORT , SSHORT , SSHORT );
static int set_attribute(OBJ , ATT_N , int *);

extern BLK PYXIS_alloc(PLB , UCHAR , int );
extern int PYXIS_release(register FRB );
extern int PYXIS_alloc_fini();
extern int PYXIS_alloc_init();
extern BLK PYXIS_pop(register LLS *);
extern int PYXIS_push(BLK , register LLS *);
extern void MOVP_fast( register SCHAR * , register SCHAR * , register ULONG );


int PYXIS_bugcheck(TEXT *string)
{
/**************************************
 *
 *	P Y X I S _ b u g c h e c k
 *
 **************************************
 *
 * Functional description
 *	We've got internal error.
 *
 **************************************/

	fprintf(stderr, "PYXIS bugcheck: %s\n", string);
	exit(FINI_ERROR);
}


OBJ PYXIS_clone(OBJ object)
{
/**************************************
 *
 *	P Y X I S _ c l o n e 
 *
 **************************************
 *
 * Functional description
 *	Clone an object.
 *
 **************************************/
	OBJ clone;
	ATT attribute, new_att;
	BLK value;

	clone = PYXIS_create_object(0, (enum att_n)0);

	for (attribute = object->obj_attributes; attribute;
		 attribute = attribute->att_next) {
		value = (BLK) attribute->att_value;
		if (attribute->att_type == attype_object)
			value = (BLK) PYXIS_clone((OBJ)value);
		new_att =
			PUT_ATTRIBUTE(clone, attribute->att_name, attribute->att_type,
						  (OBJ) value);
		if (attribute == object->obj_display_attribute)
			PYXIS_set_display_attribute(clone, new_att);
	}

	clone->obj_rel_x = object->obj_rel_x;
	clone->obj_rel_y = object->obj_rel_y;
	clone->obj_scroll_x = object->obj_scroll_x;
	clone->obj_scroll_y = object->obj_scroll_y;
	clone->obj_border = object->obj_border;
	clone->obj_width = object->obj_width;
	clone->obj_height = object->obj_height;
	clone->obj_index = object->obj_index;
	clone->obj_flags = object->obj_flags;

	return clone;
}


int PYXIS_compute_size(OBJ object, USHORT *width_ptr, USHORT *height_ptr)
{
/**************************************
 *
 *	P Y X I S _ c o m p u t e _ s i z e
 *
 **************************************
 *
 * Functional description
 *	Compute the displayed size of an object, include sub-objects.
 *
 **************************************/
	OBJ sub;
	ATT attribute;
	USHORT max_width, max_height, width, height;

/* Start with the size of the object. */

	max_width = object->obj_width;
	max_height = object->obj_height;

	if (object->obj_display_attribute && !max_height)
		max_height = 1;

/* Compute the sizes of the various children */

	for (attribute = NULL;
		 attribute = PYXIS_find_object(object, attribute, att_any, TRUE);) {
		sub = attribute->att_value;
		PYXIS_compute_size(sub, &width, &height);
		width += sub->obj_rel_x;
		height += sub->obj_rel_y;
		max_width = MAX(width, max_width);
		max_height = MAX(height, max_height);
	}

	*width_ptr = max_width;
	*height_ptr = max_height;
}


OBJ PYXIS_create_object(TEXT *string, ATT_N name)
{
/**************************************
 *
 *	P Y X I S _ c r e a t e _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Create object of given type.  If a string is passed,
 *	add the string as an attribute and set the display
 *	length.
 *
 **************************************/
	OBJ object;
	ATT attribute;

	if (!PYXIS_permanent_pool)
		PYXIS_init();

	object = (OBJ) ALLOCP(type_obj);

	if (string) {
		attribute = PUT_ATTRIBUTE(object, name, attype_string, (OBJ) string);
		PYXIS_set_display_attribute(object, attribute);
		PUT_ATTRIBUTE(object, att_width, attype_numeric,
					  (OBJ) attribute->att_length);
	}

	return object;
}


WIN PYXIS_create_window(USHORT width, USHORT height)
{
/**************************************
 *
 *	P Y X I S _ c r e a t e _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Create a window for a form.  PYXIS_create_window does
 *	not display anything, however.
 *
 **************************************/
	WIN window;

	if (!PYXIS_permanent_pool)
		PYXIS_init();

	window = (WIN) ALLOCP(type_win);
	window->win_width = width;
	window->win_height = height;
	window->win_form = PYXIS_create_object(0, (enum att_n) 0);

	if (SCR_create_window(window))
		return NULL;

	PUT_ATTRIBUTE(window->win_form, att_width, attype_numeric,
				  (OBJ) window->win_width);
	PUT_ATTRIBUTE(window->win_form, att_height, attype_numeric,
				  (OBJ) window->win_height);

	return window;
}


int PYXIS_delete_attribute(OBJ object, ATT attribute)
{
/**************************************
 *
 *	P Y X I S _ d e l e t e _ a t t r i b u t e
 *
 **************************************
 *
 * Functional description
 *	Delete a known attribute from an object.
 *
 **************************************/
	ATT next, prior;

	BLKCHK(object, type_obj);
	BLKCHK(attribute, type_att);

	clear_change(object);
	object->obj_flags |= OBJ_changed;

	if (object->obj_display_attribute == attribute)
		object->obj_display_attribute = NULL;

	if (prior = attribute->att_prior)
		if (next = prior->att_next = attribute->att_next)
			next->att_prior = prior;
		else
			object->obj_end_attributes = prior;
	else if (next = object->obj_attributes = attribute->att_next)
		next->att_prior = NULL;
	else
		object->obj_end_attributes = NULL;

	if (attribute->att_type == attype_object)
		PYXIS_delete_object(attribute->att_value);

	PYXIS_release((FRB) attribute);
}


int PYXIS_delete_named_attribute(OBJ object, ATT_N name)
{
/**************************************
 *
 *	P Y X I S _ d e l e t e _ n a m e d _ a t t r i b u t e
 *
 **************************************
 *
 * Functional description
 *	Delete all instances of a particular attribute.
 *
 **************************************/
	ATT attribute;

	if (reset_attribute(object, name))
		return 0;

	while (attribute = GET_ATTRIBUTE(object, name))
		PYXIS_delete_attribute(object, attribute);
}


int PYXIS_delete_object(OBJ object)
{
/**************************************
 *
 *	P Y X I S _ d e l e t e _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Decrement the use count of an object.  If it does to zero,
 *	delete object.
 *
 **************************************/
	ATT attribute;

	BLKCHK(object, type_obj);

	if (--object->obj_reference_count)
		return 0;

	while (attribute = object->obj_attributes)
		PYXIS_delete_attribute(object, attribute);

	PYXIS_release((FRB) object);
}


int PYXIS_delete_window(WIN window)
{
/**************************************
 *
 *	P Y X I S _ d e l e t e _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Shutdown.
 *
 **************************************/
	FILE *file;

	if (file = (FILE *) window->win_trace_in)
		fclose(file);

	if (file = (FILE *) window->win_trace_out)
		fclose(file);

	SCR_fini(window);
}


int PYXIS_disable_window(WIN window)
{
/**************************************
 *
 *	P Y X I S _ d i s a b l e _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Disable window display.
 *
 **************************************/

	SCR_disable(window);
}


int PYXIS_error(TEXT *string)
{
/**************************************
 *
 *	P Y X I S _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	We've got a user error.
 *
 **************************************/
	int (*handler) (TEXT*);

	if (PYXIS_handlers) {
		handler = (int (*)(TEXT*)) PYXIS_handlers->lls_object;
		return (*handler) (string);
	}

	fprintf(stderr, "PYXIS error: %s\n", string);
}


ATT PYXIS_find_enumeration(OBJ parent, ATT attribute, USHORT direction)
{
/**************************************
 *
 *	P Y X I S _ f i n d _ e n u m e r a t i o n
 *
 **************************************
 *
 * Functional description
 *	Find next or prior enumeration attribute.
 *
 **************************************/
/*OBJ	object;*/

	if (direction) {
		while (attribute =
			   (attribute) ? attribute->
			   att_next : parent->obj_attributes) if (attribute->att_name ==
													  att_enumeration) return
					attribute;
	}
	else
		while (attribute =
			   (attribute) ? attribute->
			   att_prior : parent->obj_end_attributes) if (attribute->
														   att_name ==
														   att_enumeration)
					return attribute;

	return NULL;
}



ATT PYXIS_find_object(OBJ parent, ATT attribute, ATT_N name, USHORT direction)
{
/**************************************
 *
 *	P Y X I S _ f i n d _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Find next object valued attribute of given name.
 *
 **************************************/
	OBJ object;

	if (direction) {
		while (attribute = (attribute) ? attribute->att_next : parent->obj_attributes) 
			if ((name == attribute->att_name || name == att_any) &&
			    attribute->att_type == attype_object && 
                (object = attribute->att_value) &&
			    !(object->obj_flags & OBJ_inactive))
				return attribute;
	}
	else
		while (attribute = (attribute) ? attribute-> att_prior : parent->obj_end_attributes) 
			if ((name == attribute->att_name || name == att_any) &&
			    attribute->att_type == attype_object && 
		        (object = attribute->att_value) && 
                !(object->obj_flags & OBJ_inactive))
				return attribute;

	return NULL;
}


int PYXIS_fini()
{
/**************************************
 *
 *	P Y X I S _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Shutdown.
 *
 **************************************/

	PYXIS_alloc_fini();
}


int PYXIS_format_form(OBJ form, USHORT width, USHORT height)
{
/**************************************
 *
 *	P Y X I S _ f o r m a t _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Provide a default format for a form.  If nothing is floating,
 *	nothing is done.  If, however, something doesn't fit, all
 *	bets are off.
 *
 **************************************/
	ATT attr;
	OBJ object;
	SSHORT x, y;
	USHORT length, x_margin, y_margin, label_length, field_length, count,
		delta_y, n;

	label_length = field_length = count = 0;
	delta_y = 1;
	y = y_margin = 2;
	x = x_margin = 3;

/* Compute the max length of the various label */

	for (attr = form->obj_attributes; attr; attr = attr->att_next)
		if (attr->att_type == attype_object && (object = attr->att_value))
			switch (attr->att_name) {
			case att_label:
			case att_literal_string:
				label_length = MAX(label_length, object->obj_width);
				++count;
				break;

			case att_field:
				field_length = MAX(field_length, object->obj_width);
				if (n = GET_VALUE(object, att_height))
					count += n;
				break;
			}

/* Based on sizes and counts, pick single or double spacing */

	x_margin = width / 2 - (label_length + 1);
	if (count * 2 + y_margin + 1 < height)
		delta_y = 2;

/* Make a second pass the form objects assigning locations */

	y = 1 - delta_y;
	x = 0;

	for (attr = form->obj_attributes; attr; attr = attr->att_next)
		if (attr->att_type == attype_object &&
			(object = attr->att_value) && (length = object->obj_width))
			switch (attr->att_name) {
			case att_label:
			case att_literal_string:
				x = label_length - object->obj_width;
				y += delta_y;
				PYXIS_position(object, x, y);
				x = label_length + 2;
				break;

			case att_field:
				PYXIS_position(object, x, y);
				x += field_length + 1;
				if (n = GET_VALUE(object, att_height))
					y += n;
				break;
			}

	if (object = GET_OBJECT(form, att_tag))
		REPLACE_ATTRIBUTE(object, att_display_y, attype_numeric, height - 1);
}



ATT PYXIS_get_attribute(OBJ object, ATT_N name)
{
/**************************************
 *
 *	P Y X I S _ g e t _ a t t r i b u t e
 *
 **************************************
 *
 * Functional description
 *	Get first attribute of known name.  If no attribute of
 *	that name is found, or the type is wrong, return NULL.
 *
 **************************************/
	ATT attribute;

	for (attribute = object->obj_attributes; attribute;
		 attribute = attribute->att_next) if (attribute->att_name == name)
			return attribute;

	return NULL;
}


OBJ PYXIS_get_attribute_value(OBJ object, ATT_N name)
{
/**************************************
 *
 *	P Y X I S _ g e t _ a t t r i b u t e _ v a l u e
 *
 **************************************
 *
 * Functional description
 *	Get the value of an attribute.  If the attribute doesn't
 *	exist or is of wrong type, return NULL.
 *
 **************************************/
	SLONG bit;
	ATT attribute;

/* If the requested attribute is one that is maintained in the object block, this
   whole process is a lot SSHORTer */

	switch (name) {
	case att_display_x:
		return (OBJ) object->obj_rel_x;
	case att_display_y:
		return (OBJ) object->obj_rel_y;
	case att_scroll_x:
		return (OBJ) object->obj_scroll_x;
	case att_scroll_y:
		return (OBJ) object->obj_scroll_y;
	case att_width:
		return (OBJ) object->obj_width;
	case att_height:
		return (OBJ) object->obj_height;
	case att_border:
		return (OBJ) object->obj_border;
	case att_index:
		return (OBJ) object->obj_index;

	case att_inactive:
		bit = OBJ_inactive;
		break;
	case att_reverse_video:
		bit = OBJ_reverse_video;
		break;
	case att_bold:
		bit = OBJ_bold;
		break;
	case att_underline:
		bit = OBJ_underline;
		break;
	case att_box:
		bit = OBJ_box;
		break;
	case att_blank:
		bit = OBJ_blank;
		break;
	case att_insert_right:
		bit = OBJ_insert_right;
		break;
	case att_updatable_flag:
		bit = OBJ_updatable_flag;
		break;
	case att_wakeup_flag:
		bit = OBJ_wakeup_flag;
		break;
	case att_reverse_for_update:
		bit = OBJ_reverse_for_update;
		break;

	default:
		if (!(attribute = PYXIS_get_attribute(object, name)))
			return NULL;
		return attribute->att_value;
	}

	return (OBJ) ((object->obj_flags & bit) ? TRUE : FALSE);
}


int PYXIS_get_char(WIN window, OBJ object, int x_offset, int y_offset)
{
/**************************************
 *
 *	P Y X I S _ g e t _ c h a r
 *
 **************************************
 *
 * Functional description
 *	Get a character from the window.
 *
 **************************************/
	SSHORT c;
	FILE *file;

	if (object)
		auto_scroll(object, x_offset, y_offset);

	PYXIS_update_window(window, object, x_offset, y_offset);

	if (file = (FILE *) window->win_trace_in)
		c = getc(file);
	else
		for (;;) {
			c = SCR_getchar(window, object, x_offset, y_offset);
			if (c == 18)
				SCR_refresh_window(window, object, x_offset, y_offset);
#ifdef DEBUG
			else if (c == 16)
				PYXIS_print(window->win_form, 0);
#endif
			else
				break;
		}

	if (file = (FILE *) window->win_trace_out)
		putc(c, file);

	return c;
}


int PYXIS_get_data(WIN window, OBJ object, ATT_N name, OBJ form, USHORT *term_char)
{
/**************************************
 *
 *	P Y X I S _ g e t _ d a t a
 *
 **************************************
 *
 * Functional description
 *	Get data attribute from keyboard.  Return length of string
 *	stored, zero if nothing happened.  In either case return the
 *	terminating character.
 *
 **************************************/
	ATT old_display, temp_data, attribute;
	USHORT length, upcase, max_length;
	TEXT buffer[256], *p, *end;
	SSHORT c, edit_mode, overstrike_mode, insert_right, modified_flag;

	if (GET_VALUE(object, att_enumeration))
		return get_enumerated(window, object, name, form, term_char);

	upcase = GET_VALUE(object, att_upcase);
	old_display = object->obj_display_attribute;

	if (p = GET_STRING(object, att_update_data))
		strcpy(buffer, p);
	else if (p = GET_STRING(object, att_data))
		strcpy(buffer, p);
	else if (p = GET_STRING(object, att_default_data))
		strcpy(buffer, p);
	else
		buffer[0] = 0;

	max_length = object->obj_width;

/* Trim leading spaces from right justified data.  */

	insert_right = object->obj_flags & OBJ_insert_right;

	if (insert_right) {
		for (p = buffer; *p; p++)
			if (!isspace(*p))
				break;
		if (*p)
			strcpy(buffer, p);
		p = &buffer[strlen(buffer)];
	}
	else
		p = buffer;

	length = strlen(buffer);
	edit_mode = FALSE;
	overstrike_mode = FALSE;
	modified_flag = FALSE;

	while (TRUE) {
		object->obj_display_attribute = temp_data =
			REPLACE_ATTRIBUTE(object, att_temp_data, attype_string, buffer);

		switch (c = PYXIS_get_char(window, object,
								   (insert_right) ? (max_length -
													 strlen(p)) : (p -
																   buffer),
								   0)) {
		case C_PF1:
		case C_PF2:
		case C_PF3:
		case C_PF4:
		case C_PF5:
		case C_PF6:
		case C_PF7:
		case C_PF8:
		case C_PF9:
		case C_ENTER:
		case C_EOF:
		case '\n':
		case '\t':
		case C_SCROLL_TOP:
		case C_SCROLL_BOTTOM:
			goto exit;

		case C_UP:				/* ignore vertical mouse movement in edit mode */
		case C_DOWN:
			if (edit_mode && (window->win_flags & WIN_synthesized))
				break;
			goto exit;

		case C_GOTO_START:		/* go to start of field. */
			if (!edit_mode)
				break;
			if (window->win_flags & WIN_synthesized)
				break;
			p = buffer;
			break;

		case C_GOTO_END:		/* go to end of field. */
			if (!edit_mode)
				break;
			if (window->win_flags & WIN_synthesized)
				break;
			p = &buffer[length];
			break;

		case C_INSERT_OVERSTRIKE:	/* start edit mode and toggle insert/overstrike mode. */
			edit_mode = TRUE;
			overstrike_mode = !overstrike_mode;
			break;

		case C_EDIT:			/* start or terminate edit mode */
			edit_mode = !edit_mode;
			if (insert_right)
				p = &buffer[length];
			else
				p = buffer;
			break;

		case C_RIGHT:			/* move one character to the right. */
			if (!edit_mode)
				goto exit;
			if (!*p)
				break;
			p++;
			break;

		case C_LEFT:			/* move one character to the left. */
			if (!edit_mode)
				goto exit;
			if (p == buffer)
				break;
			p--;
			break;

		case C_DELETE:			/* delete character to the left of the cursor */
			if (!edit_mode)
				goto exit;
			modified_flag = TRUE;
			if (p == buffer)
				break;
			if (overstrike_mode)
				p[-1] = ' ';
			else {
				strcpy(&p[-1], p);
				length--;
			}
			p--;
			break;

		case C_DELETE_NEXT:	/* delete the character to the right of the cursor */
			if (!edit_mode)
				break;
			modified_flag = TRUE;
			if (*p == 0)
				break;
			strcpy(p, &p[1]);
			length--;
			break;

		case C_ERASE:			/* start edit mode and clear the entire field.  */

		  erase_field:
			edit_mode = TRUE;
			modified_flag = TRUE;
			p = buffer;
			*p = 0;
			length = 0;
			break;

		default:

/* start edit mode and enter the new character, if there is room and it is printable. */

			modified_flag = TRUE;

/* if we have not explicitly entered edit mode, erase the field before entering this character */

			if (!edit_mode) {
				PYXIS_return_char(window, c);
				goto erase_field;
			}

			if (!isprint(c))
				break;

/* if the field is full, ring a bell to warn the user and discard the character.  */

			if ((length == max_length) && (!overstrike_mode || (!*p))) {
				fprintf(stderr, "\07");
				break;
			}

			edit_mode = TRUE;
			if (upcase)
				c = UPPER(c);
			if ((!overstrike_mode) || (!*p)) {

/*  Make room for new char.  Can't use strcpy due to overlapping buffers. */

				for (end = &buffer[length]; end >= p; end--)
					end[1] = *end;
				length++;
			}
			*p++ = c;
			break;
		}
	}

  exit:

	if (modified_flag)
		old_display = REPLACE_ATTRIBUTE(object, name, attype_string, buffer);
	else
		length = 0;

	*term_char = c;
	object->obj_display_attribute = old_display;
	PYXIS_delete_attribute(object, temp_data);

	return length;
}

extern "C"
OBJ PYXIS_get_value(ATT attribute)
{
/**************************************
 *
 *	P Y X I S _ g e t _ v a l u e
 *
 **************************************
 *
 * Functional description
 *	Get the value of an attribute.  This is not used internally, since the
 *	information is readily available from the attribute block.  Externally,
 *	however, the attribute block is not available.
 *
 **************************************/

	return attribute->att_value;
}


SCHAR *PYXIS_get_keyname(WIN window, UCHAR key_code)
{
/**************************************
 *
 *	P Y X I S _ g e t _ k e y n a m e
 *
 **************************************
 *
 * Functional description
 *	Get the name of the key(s) which correspond to the
 *	given keycode for this window (keyboard).  Return the string.
 *
 **************************************/

	KNM knm_p;

	knm_p = window->win_keyname_table;
	while (knm_p->knm_keycode && knm_p->knm_keycode != key_code)
		knm_p++;

	return knm_p->knm_keyname;
}


int PYXIS_init()
{
/**************************************
 *
 *	P Y X I S _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize the form system if not already initialized.
 *
 **************************************/

	if (!PYXIS_permanent_pool) {
		gds__register_cleanup((void (*)(void*)) cleanup, 0);
		PYXIS_alloc_init();
	}
}


int PYXIS_move(OBJ form, OBJ object)
{
/**************************************
 *
 *	P Y X I S _ m o v e
 *
 **************************************
 *
 * Functional description
 *	Move an object to a new location.
 *
 **************************************/
	ATT next, prior, attribute;
	OBJ object2;

/* Start by finding and removing object from it's parent */

	for (attribute = form->obj_attributes; attribute;
		 attribute = attribute->att_next)
			if (attribute->att_type == attype_object &&
				attribute->att_value == object)
			break;

	if (!attribute)
		return 0;

/* Indicate form needs refreshing */

	form->obj_flags |= OBJ_changed;

/* Special case single attribute */

	if (form->obj_attributes == form->obj_end_attributes &&
		form->obj_attributes == attribute)
		return 0;

/* Splice attribute out of list */

	if (next = attribute->att_next)
		next->att_prior = attribute->att_prior;
	else
		form->obj_end_attributes = attribute->att_prior;

	if (prior = attribute->att_prior)
		prior->att_next = attribute->att_next;
	else
		form->obj_attributes = attribute->att_next;

/* Find new home and re-insert */

	for (next = form->obj_attributes; next; next = next->att_next)
		if (next->att_type == attype_object) {
			object2 = next->att_value;
			if (object->obj_rel_y < object2->obj_rel_y ||
				(object->obj_rel_y == object2->obj_rel_y &&
				 object->obj_rel_x < object2->obj_rel_x)) {
				attribute->att_next = next;
				attribute->att_prior = prior = next->att_prior;
				next->att_prior = attribute;
				if (prior)
					prior->att_next = attribute;
				else
					form->obj_attributes = attribute;
				return 0;
			}
		}

/* Must be last object */

	if (prior = form->obj_end_attributes) {
		prior->att_next = attribute;
		attribute->att_prior = prior;
		attribute->att_next = NULL;
		form->obj_end_attributes = attribute;
	}
}


ATT PYXIS_navigate(OBJ form, ATT attribute, ATT_N name, USHORT c)
{
/**************************************
 *
 *	P Y X I S _ n a v i g a t e
 *
 **************************************
 *
 * Functional description
 *	Use a character to navigate thru a form to find 
 *	an object-valued attribute.
 *
 **************************************/
	OBJ object, from_object, target, parent, parent2;
	ATT next, best, attr;
	LLS *temp, tempstk;
	USHORT direction;
	SSHORT x1, x2, a1, a2, y, y2, delta_x, delta_y, best_xy, xy;


	if (c == C_DELETE || c == C_LEFT || c == C_UP)
		direction = FALSE;
	else
		direction = TRUE;

	from_object = attribute->att_value;

/*if (GET_ATTRIBUTE (from_object, att_field))
   {
   tempstk = NULL;
   temp = &tempstk;
   do {
      attr = (ATT) LLS_POP (context);
      LLS_PUSH (attr, temp); 
      } while (GET_ATTRIBUTE(attr->att_value, att_field));
   from_object = attr->att_value;
   do {
      attr = (ATT) LLS_POP (temp);
      LLS_PUSH (attr, context);
      } while (tempstk);
   }*/

	switch (c) {
	case '\n':
	case '\t':
	case C_DELETE:
		if (!(from_object->obj_parent == form))
			attribute = NULL;
		return PYXIS_find_object(form, attribute, name, direction);
	case C_UP:
	case C_DOWN:
	case C_RIGHT:
	case C_LEFT:

		/* Determine the common parent & the position 
		   of the source field relative to common parent */

		next = best = NULL;
		parent2 = NULL;
		y = x1 = 0;
		best_xy = 32000;

		for (parent = form; !parent2; parent = parent->obj_parent)
			for (parent2 = from_object; parent2 = parent2->obj_parent;)
				if (parent == parent2)
					break;

		for (object = from_object; object != parent2;
			 object = object->obj_parent) {
			y =
				y + object->obj_rel_y + object->obj_border +
				object->obj_scroll_y;
			x1 =
				x1 + object->obj_rel_x + object->obj_border +
				object->obj_scroll_x;
		}
		x2 = x1 + from_object->obj_width;

		while (next = PYXIS_find_object(form, next, name, direction))
			if ((target = next->att_value) != from_object) {
				a1 = y2 = 0;
				for (object = target; object != parent2;
					 object = object->obj_parent) {
					a1 =
						a1 + object->obj_rel_x + object->obj_border +
						object->obj_scroll_x;
					y2 =
						y2 + object->obj_rel_y + object->obj_border +
						object->obj_scroll_y;
				}
				a2 = a1 + target->obj_width;

				if ((x1 >= a1 && x1 <= a2) || (x2 >= a1 && x2 <= a2) ||
					(a1 >= x1 && a1 <= x2) || (a2 >= x1 && a2 <= x2))
					delta_x = 0;	/* if target is "under" from_obj at all */
				else
					delta_x = (a1 > x2) ? a1 - x2 : a2 - x1;
				delta_y = y2 - y;
				switch (c) {
				case C_UP:
					if (delta_y >= 0)
						continue;
					xy = 5 * ABS(delta_x) - delta_y;
					break;
				case C_DOWN:
					if (delta_y <= 0)
						continue;
					xy = 5 * ABS(delta_x) + delta_y;
					break;
				case C_RIGHT:
					if (delta_x <= 0)
						continue;
					xy = 5 * ABS(delta_y) + delta_x;
					break;
				case C_LEFT:
					if (delta_x >= 0)
						continue;
					xy = 5 * ABS(delta_y) - delta_x;
					break;
				}
				if (xy < best_xy) {
					best_xy = xy;
					best = next;
				}
			}
		return best;
	}

	return attribute;
}


int PYXIS_pop_form(WIN window)
{
/**************************************
 *
 *	P Y X I S _ p o p _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Pop a form off the window stack.  Return the form poped.
 *
 **************************************/
	OBJ form, object;
	ATT attribute;

/* If there isn't a current form, just return NULL */

	if (!
		(attribute =
		 PYXIS_find_object(window->win_form, 0, att_form,
						   FALSE))) return NULL;

/* Delete form from top level meta-form */

	form = attribute->att_value;

	if (form->obj_reference_count > 1) {
		PYXIS_delete_attribute(window->win_form, attribute);
		--form->obj_reference_count;
	}
	else
		PYXIS_delete_attribute(window->win_form, attribute);

/* If the exposed form was occluded, expose it and everything else */

	if ((object = (OBJ) PYXIS_top_form(window)) && object->obj_occluded) {
		SCR_clear_window(window);
		for (attribute = NULL;
			 attribute =
			 PYXIS_find_object(window->win_form, attribute, att_form, TRUE);) {
			object = attribute->att_value;
			--object->obj_occluded;
		}
	}

/*
if (!object)
    PYXIS_disable_window (window);
*/

	return (int) form;
}

extern "C"
int PYXIS_pop_handler( int (*handler)() )
{
/**************************************
 *
 *	P Y X I S _ p o p _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Pop an error handler from the handler stack.
 *
 **************************************/

	LLS_POP(&PYXIS_handlers);
}


int PYXIS_position(OBJ object, USHORT x, USHORT y)
{
/**************************************
 *
 *	P Y X I S _ p o s i t i o n
 *
 **************************************
 *
 * Functional description
 *	Set x and y coordinates of an object.
 *
 **************************************/

	REPLACE_ATTRIBUTE(object, att_display_x, attype_numeric, x);
	REPLACE_ATTRIBUTE(object, att_display_y, attype_numeric, y);
}


int PYXIS_push_form(WIN window, OBJ form, USHORT occlude)
{
/**************************************
 *
 *	P Y X I S _ p u s h _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Push a form on a window.  If the form is already on the
 *	stack, move it to the top.
 *
 **************************************/
	OBJ object;
	ATT attribute;

/* If we're top, don't worry much */

	if ((attribute = PYXIS_find_object(window->win_form, 0, att_form, FALSE))
		&& attribute->att_value == form)
		return 0;

	if (occlude) {
		SCR_clear_window(window);
		for (attribute = NULL;
			 attribute =
			 PYXIS_find_object(window->win_form, attribute, att_form, FALSE);) {
			object = attribute->att_value;
			clear_change(object);
			++object->obj_occluded;
		}
	}

/* Make sure we're not already somewhere */

	if (attribute)
		while (attribute =
			   PYXIS_find_object(window->win_form, attribute, att_form,
								 FALSE)) if (attribute->att_value == form) {
				PYXIS_delete_attribute(form, attribute);	/*??amg _ (window->win_form, att)? */
				--form->obj_reference_count;	/* but would form still exist for this operat? */
				break;
			}

	++form->obj_reference_count;
	form->obj_occluded = 0;
	PUT_ATTRIBUTE(window->win_form, att_form, attype_object, form);
}

extern "C"
int PYXIS_push_handler( int (*handler)() )
{
/**************************************
 *
 *	P Y X I S _ p u s h _ h a n d l e r
 *
 **************************************
 *
 * Functional description
 *	Push an error handler on the handler stack.
 *
 **************************************/

	LLS_PUSH((BLK) handler, &PYXIS_handlers);
}


int PYXIS_push_tag(WIN window, OBJ object)
{
/**************************************
 *
 *	P Y X I S _ p u s h _ t a g
 *
 **************************************
 *
 * Functional description
 *	Set the position of an object to the tag line of a window.
 *
 **************************************/

	PYXIS_position(object, 0, window->win_height - 1);
	REPLACE_ATTRIBUTE(object, att_blank, attype_numeric, TRUE);
	REPLACE_ATTRIBUTE(object, att_width, attype_numeric, window->win_width);
	REPLACE_ATTRIBUTE(object, att_height, attype_numeric, 1);
	PYXIS_push_form(window, object, FALSE);
}



ATT PYXIS_put_attribute(OBJ object, ATT_N name, ATT_T type, OBJ value)
{
/**************************************
 *
 *	P Y X I S _ p u t _ a t t r i b u t e
 *
 **************************************
 *
 * Functional description
 *	Add an attribute to an existing object.  This does NOT replace
 *	other attributes of the same name.  If the type is string, copy
 *	the string.
 *
 **************************************/
	USHORT length;
	TEXT *p, *q;
	ATT attribute, prior;

	BLKCHK(object, type_obj);
	object->obj_flags |= OBJ_changed;
	clear_change(object);

/* If the attribute is special cased, don't do anything else */

	if (set_attribute(object, name, (int*) value))
		return NULL;

/* If value is an object, bump reference count */

	if (type == attype_object) {
		BLKCHK(value, type_obj);
		PYXIS_reference(value);
		value->obj_parent = object;
	}

/* Allocate new attribute block */

	length = (type == attype_string) ? strlen((char*) value) : 0;
	attribute = (ATT) ALLOCPV(type_att, length);
	attribute->att_type = type;
	attribute->att_name = name;

/* Link attribute into object at end */

	if (prior = object->obj_end_attributes) {
		attribute->att_prior = prior;
		prior->att_next = attribute;
		object->obj_end_attributes = attribute;
	}
	else
		object->obj_attributes = object->obj_end_attributes = attribute;

/* Unless attribute is a string, just set the value and return */

	if (type != attype_string) {
		attribute->att_value = value;
		return attribute;
	}

/* Attribute is a string, sigh, and strings must be copied */

	attribute->att_value = (OBJ) attribute->att_data;
	MOVP_fast((SCHAR*) value, (SCHAR*) attribute->att_value, length);
	attribute->att_length = length;

	return attribute;
}


int PYXIS_reference(OBJ object)
{
/**************************************
 *
 *	P Y X I S _ r e f e r e n c e
 *
 **************************************
 *
 * Functional description
 *	Increment reference count for an object.
 *
 **************************************/

	++object->obj_reference_count;
}


ATT PYXIS_replace_attribute(OBJ object, ATT_N name, ATT_T type, OBJ value)
{
/**************************************
 *
 *	P Y X I S _ r e p l a c e _ a t t r i b u t e
 *
 **************************************
 *
 * Functional description
 *	Add an attribute to an existing object.  If the object already
 *	exists, replace it.
 *
 **************************************/
	ATT attribute;
	USHORT display, length;

	BLKCHK(object, type_obj);
	object->obj_flags |= OBJ_changed;
	clear_change(object);

/* Handle special cased attributes */

	if (set_attribute(object, name, (int*) value))
		return NULL;

	if (type == attype_object) {
		BLKCHK(value, type_obj);
		value->obj_parent = object;
	}

	display = FALSE;

	if (attribute = PYXIS_get_attribute(object, name)) {
		if (attribute->att_type == attype_numeric &&
			attribute->att_value == value) return attribute;
		if (type != attype_string) {
			if (value == attribute->att_value)
				return attribute;
			if (type == attype_object) {
				PYXIS_reference(value);
				PYXIS_delete_object(attribute->att_value);
			}
			attribute->att_value = value;
			return attribute;
		}
		length = strlen((char*) value);
		if (length <= attribute->att_length) {
			strcpy((char*)attribute->att_data, (char*)value);
			attribute->att_length = length;
			return attribute;
		}
		if (object->obj_display_attribute == attribute)
			display = TRUE;
		PYXIS_delete_attribute(object, attribute);
	}

	attribute = PYXIS_put_attribute(object, name, type, value);

	if (display)
		PYXIS_set_display_attribute(object, attribute);

	return attribute;
}


int PYXIS_return_char(WIN window, USHORT character)
{
/**************************************
 *
 *	P Y X I S _ r e t u r n _ c h a r
 *
 **************************************
 *
 * Functional description
 *	Register a character to be faked on next call
 *	to PYXIS_get_char.
 *
 **************************************/

	window->win_pending_char = character;
}


int PYXIS_scroll_reset(OBJ object)
{
/**************************************
 *
 *	P Y X I S _ s c r o l l _ r e s e t
 *
 **************************************
 *
 * Functional description
 *	Starting from an arbitrary spot in a complex
 *	object, reset scrolling to non-scrolled state.
 *
 **************************************/
	ATT attribute;

	if (object->obj_scroll_x)
		REPLACE_ATTRIBUTE(object, att_scroll_x, attype_numeric, 0);

	if (object->obj_scroll_y)
		REPLACE_ATTRIBUTE(object, att_scroll_y, attype_numeric, 0);

	for (attribute = NULL;
		 attribute = PYXIS_find_object(object, attribute, att_any, TRUE);)
		PYXIS_scroll_reset(attribute->att_value);
}


int PYXIS_set_display_attribute(OBJ object, ATT attribute)
{
/**************************************
 *
 *	P Y X I S _ s e t _ d i s p l a y _ a t t r i b u t e
 *
 **************************************
 *
 * Functional description
 *	Set the display attribute to an object.
 *
 **************************************/

	BLKCHK(object, type_obj);

	if (attribute)
		BLKCHK(attribute, type_att);

	object->obj_display_attribute = attribute;
	object->obj_flags |= OBJ_changed;
}


int PYXIS_top_form(WIN window)
{
/**************************************
 *
 *	P Y X I S _ t o p _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Find the top form in window.
 *
 **************************************/
	ATT attribute;

	if (attribute = PYXIS_find_object(window->win_form, 0, att_form, FALSE))
		return (int) attribute->att_value;

	return NULL;
}


int PYXIS_trace_in(WIN window, SCHAR *name)
{
/**************************************
 *
 *	P Y X I S _ t r a c e _ i n
 *
 **************************************
 *
 * Functional description
 *	Set up to receive input from a file rather than
 *	from the user hitting keys at a terminal!
 *
 **************************************/
	TEXT buffer[64];
	FILE *file;

	if (file = (FILE *) window->win_trace_in)
		fclose(file);

	if (!(file = fopen(name, "r"))) {
		sprintf(buffer, "%s.pyx", name);
		if (!(file = fopen(buffer, "r")))
			return FALSE;
	}
	window->win_trace_in = (int *) file;
	return TRUE;
}


int PYXIS_trace_out(WIN window, SCHAR *name)
{
/**************************************
 *
 *	P Y X I S _ t r a c e _ o u t 
 *
 **************************************
 *
 * Functional description
 *	Log all keyed input to a file.
 *
 **************************************/
	FILE *file;

	if (file = (FILE *) window->win_trace_out)
		fclose(file);

	if (!(file = fopen(name, "w")))
		return FALSE;

	window->win_trace_out = (int *) file;
	return TRUE;
}


int PYXIS_update_window(WIN window, OBJ object, int x_offset, int y_offset)
{
/**************************************
 *
 *	P Y X I S _ u p d a t e _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Update a window.  Leave the cursor pointing at the given object plus
 *	a given offset.
 *
 **************************************/

	SCR_update_window(window, object, x_offset, y_offset);
	PYXIS_window = window;
}


static int auto_scroll(OBJ object, int x_offset, int y_offset)
{
/**************************************
 *
 *	a u t o _ s c r o l l
 *
 **************************************
 *
 * Functional description
 *	Automatically scroll stuff to make object visible
 *	(if possible, of course).
 *
 **************************************/
	SSHORT x, y, length, end, width, height, delta_x, delta_y, border;
	OBJ parent;

/* Get current coordinates relative to parent */

	x = object->obj_rel_x + object->obj_scroll_x;
	y = object->obj_rel_y + object->obj_scroll_y;
	length = object->obj_width;
	end = x + length;

/* Make everything fit */

	while (parent = object->obj_parent) {
		delta_x = delta_y = 0;
		border = 2 * parent->obj_border;
		if (width = GET_VALUE(parent, att_width))
			width -= border;
		if (height = GET_VALUE(parent, att_height))
			height -= border;

		/* See if horizontal scrolling is required to make object visible */

		if (x < 0)
			delta_x = -x;
		else if (width && end > width)
			delta_x = (length < width) ? width - end : -x;

		/* Check vertical scrolling */

		if (y < 0)
			delta_y = -y;
		else if (height && y >= height)
			delta_y = height - y - 1;

		/* Check if to see if object is visible if scrolling is cancelled */

		if (x - object->obj_scroll_x >= 0 &&
			end - object->obj_scroll_x < width)
				delta_x = -object->obj_scroll_x;

		y -= object->obj_scroll_y;
		if (y >= 0 && y < height)
			delta_y = -object->obj_scroll_y;

		if (delta_x || delta_y)
			scroll(parent, TRUE, delta_x, delta_y);

		x = parent->obj_rel_x + parent->obj_scroll_x + parent->obj_border +
			object->obj_rel_x + object->obj_scroll_x + object->obj_border;
		y = parent->obj_rel_y + parent->obj_scroll_y + parent->obj_border +
			object->obj_rel_y + object->obj_scroll_y + object->obj_border;
		if (!width)
			width = length;
		end = x + MIN(width, length);
		object = parent;
	}
}


static int cleanup()
{
/**************************************
 *
 *	c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Exit handler for image exit.
 *
 **************************************/

	PYXIS_permanent_pool = NULL;
	PYXIS_handlers = NULL;
}


static int clear_change(OBJ object)
{
/**************************************
 *
 *	c l e a r _ c h a n g e
 *
 **************************************
 *
 * Functional description
 *	An object is being modified.  If it is currently displayed,
 *	clear it.
 *
 **************************************/

	if (!(object->obj_flags & OBJ_displayed) ||
		!PYXIS_window || !related(PYXIS_window->win_form, object))
		return 0;

	SCR_clear_object(PYXIS_window, object);
}


static int get_enumerated(WIN window, OBJ object, ATT_N name, OBJ form,
            USHORT *term_char)
{
/**************************************
 *
 *	g e t _ e n u m e r a t e d
 *
 **************************************
 *
 * Functional description
 *	Get one of several enumerated values.
 *
 **************************************/
	ATT old_display, temp_data;
	USHORT length, offset, upcase;
	TEXT buffer[256], *p, *q;
	ATT attribute, temp;
	SSHORT c, change;

	upcase = GET_VALUE(object, att_upcase);
	old_display = object->obj_display_attribute;
	attribute = PYXIS_find_enumeration(object, 0, TRUE);
	change = FALSE;

/* Find starting value */

	if (!(p = GET_STRING(object, att_update_data)) &&
		!(p = GET_STRING(object, att_data)))
			p = (TEXT *) attribute->att_value;


	temp_data = REPLACE_ATTRIBUTE(object, att_temp_data, attype_string, p);
	PYXIS_set_display_attribute(object, temp_data);

	p = buffer;
	*p = 0;

	for (;;) {
		offset = p - buffer;
		switch (c = PYXIS_get_char(window, object, offset, 0)) {
		case '\n':
			if (offset)
				goto exit;
			change = TRUE;
			while (!
				   (attribute =
					PYXIS_find_enumeration(object, attribute, TRUE)));
			break;

		case C_DELETE:
			if (p == buffer) {
				change = TRUE;
				while (!
					   (attribute =
						PYXIS_find_enumeration(object, attribute, FALSE)));
			}
			else
				*--p = 0;
			break;

		case C_UP:
		case C_DOWN:
		case C_RIGHT:
		case C_LEFT:
		case C_PF1:
		case C_PF2:
		case C_PF3:
		case C_PF4:
		case C_PF5:
		case C_PF6:
		case C_PF7:
		case C_PF8:
		case C_PF9:
		case C_ENTER:
		case C_EOF:
		case C_SCROLL_TOP:
		case C_SCROLL_BOTTOM:
		case '\t':
			goto exit;

		default:
			if (upcase)
				c = UPPER(c);
			*p++ = c;
			*p = 0;
			length = p - buffer;

			/* Check remaining attributes */

			for (temp = attribute; temp;
				 temp = PYXIS_find_enumeration(object, temp, TRUE))
				if (!strncmp(buffer, (char*)temp->att_value, length))
					break;

			/* Check preceding attributes */

			if (!temp)
				while (temp = PYXIS_find_enumeration(object, temp, TRUE))
					if (!strncmp(buffer, (char*)temp->att_value, length))
						break;

			/* Still can't find it, start over with string */

			if (!temp) {
				p = buffer;
				*p++ = c;
				*p = 0;
				for (temp = NULL;
					 temp = PYXIS_find_enumeration(object, temp, TRUE);)
					if (!(strncmp(buffer, (char*)temp->att_value, 1)))
						break;
			}

			if (temp) {
				change = TRUE;
				attribute = temp;
			}
			else if (p > buffer)
				*--p = 0;
			break;
		}
		object->obj_display_attribute = temp_data =
			REPLACE_ATTRIBUTE(object, att_temp_data, attype_string,
							  attribute->att_value);
	}

  exit:

	if (change)
		old_display =
			REPLACE_ATTRIBUTE(object, name, attype_string,
							  attribute->att_value);

	*term_char = c;
	object->obj_display_attribute = old_display;
	PYXIS_delete_attribute(object, temp_data);

	return length;
}


static int related(OBJ parent, OBJ object)
{
/**************************************
 *
 *	r e l a t e d
 *
 **************************************
 *
 * Functional description
 *	Check to see if two objects are related.
 *
 **************************************/
	ATT attribute;

	BLKCHK(parent, type_obj);
	BLKCHK(object, type_obj);

	if (parent == object)
		return TRUE;

	for (attribute = parent->obj_attributes; attribute;
		 attribute = attribute->att_next)
			if (attribute->att_type == attype_object &&
				related(attribute->att_value, object))
			return TRUE;

	return FALSE;
}


static int reset_attribute(OBJ object, ATT_N name)
{
/**************************************
 *
 *	r e s e t _ a t t r i b u t e
 *
 **************************************
 *
 * Functional description
 *	Set an attribute.  This is used to catch hardcoded attributes.
 *
 **************************************/

	switch (name) {
	case att_inactive:
		object->obj_flags &= ~OBJ_inactive;
		break;

	case att_reverse_video:
		object->obj_flags &= ~OBJ_reverse_video;
		break;

	case att_bold:
		object->obj_flags &= ~OBJ_bold;
		break;

	case att_underline:
		object->obj_flags &= ~OBJ_underline;
		break;

	case att_box:
		object->obj_flags &= ~OBJ_box;
		break;

	case att_blank:
		object->obj_flags &= ~OBJ_blank;
		break;

	case att_insert_right:
		object->obj_flags &= ~OBJ_insert_right;
		break;

	case att_updatable_flag:
		object->obj_flags &= ~OBJ_updatable_flag;
		break;

	case att_wakeup_flag:
		object->obj_flags &= ~OBJ_wakeup_flag;
		break;

	case att_reverse_for_update:
		object->obj_flags &= ~OBJ_reverse_for_update;
		break;

	case att_display_x:
		object->obj_rel_x = 0;
		break;

	case att_display_y:
		object->obj_rel_y = 0;
		break;

	case att_scroll_x:
		object->obj_scroll_x = 0;
		break;

	case att_scroll_y:
		object->obj_scroll_y = 0;
		break;

	case att_border:
		object->obj_border = 0;
		break;

	case att_width:
		object->obj_width = 0;
		break;

	case att_height:
		object->obj_height = 0;
		break;

	case att_index:
		object->obj_index = 0;
		break;

	default:
		return FALSE;
	}

	return TRUE;
}


static int scroll(OBJ parent, USHORT relative, SSHORT delta_x, SSHORT delta_y)
{
/**************************************
 *
 *	s c r o l l
 *
 **************************************
 *
 * Functional description
 *	Scroll all objects with object.
 *
 **************************************/
	ATT attribute;
	OBJ object;
	SSHORT n;

	attribute = NULL;

	while (attribute = PYXIS_find_object(parent, attribute, att_any, TRUE)) {
		object = attribute->att_value;
		if (relative) {
			if (delta_x) {
				n = delta_x + GET_VALUE(object, att_scroll_x);
				REPLACE_ATTRIBUTE(object, att_scroll_x, attype_numeric, n);
			}
			if (delta_y) {
				n = delta_y + GET_VALUE(object, att_scroll_y);
				REPLACE_ATTRIBUTE(object, att_scroll_y, attype_numeric, n);
			}
		}
		else {
			REPLACE_ATTRIBUTE(object, att_scroll_x, attype_numeric, delta_x);
			REPLACE_ATTRIBUTE(object, att_scroll_y, attype_numeric, delta_y);
		}
	}
}



static int set_attribute(OBJ object, ATT_N name, int *value)
{
/**************************************
 *
 *	s e t _ a t t r i b u t e
 *
 **************************************
 *
 * Functional description
 *	Set an attribute.  This is used to catch hardcoded attributes.
 *	Return TRUE if attribute is handled, otherwise FALSE.
 *
 **************************************/
	USHORT bit;

	switch (name) {
	case att_inactive:
		bit = OBJ_inactive;
		break;

	case att_reverse_video:
		bit = OBJ_reverse_video;
		break;

	case att_bold:
		bit = OBJ_bold;
		break;

	case att_underline:
		bit = OBJ_underline;
		break;

	case att_box:
		bit = OBJ_box;
		break;

	case att_blank:
		bit = OBJ_blank;
		break;

	case att_insert_right:
		bit = OBJ_insert_right;
		break;

	case att_reverse_for_update:
		bit = OBJ_reverse_for_update;
		break;

	case att_updatable_flag:
		bit = OBJ_updatable_flag;
		break;

	case att_wakeup_flag:
		bit = OBJ_wakeup_flag;
		break;

	case att_display_x:
		object->obj_rel_x = (SSHORT) value;
		return TRUE;

	case att_display_y:
		object->obj_rel_y = (SSHORT) value;
		return TRUE;

	case att_scroll_x:
		object->obj_scroll_x = (SSHORT) value;
		return TRUE;

	case att_scroll_y:
		object->obj_scroll_y = (SSHORT) value;
		return TRUE;

	case att_border:
		object->obj_border = (USHORT) value;
		return TRUE;

	case att_width:
		object->obj_width = (USHORT) value;
		return TRUE;

	case att_height:
		object->obj_height = (USHORT) value;
		return TRUE;

	case att_index:
		object->obj_index = (USHORT) value;
		return TRUE;

	default:
		return FALSE;
	}

/* Handle bit encode attributes */

	if (value)
		object->obj_flags |= bit;
	else
		object->obj_flags &= ~bit;

	return TRUE;
}
