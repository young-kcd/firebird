/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		edit.e
 *	DESCRIPTION:	Form Editor
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
 * $Id: edit.e,v 1.1.1.1 2001-05-23 13:26:34 tamlin Exp $
 * Revision 1.2  2000/11/20 17:17:17  fsg
 * Changes to get edit.e through gpre 6.0
 * Still have to find out why gpre isn't processing
 * PYXYS FORM FIELDS correctly.
 * Probably no one ever will need this.
 * But who cares :-)
 *
 */

#include "../jrd/gds.h"
#include "../pyxis/pyxis.h"
#include "../pyxis/phase2.h"

/***
#define DEBUG 1
***/

#define INITIAL_MAX	32000

DATABASE db = STATIC "forms.gdb";

WINDOW_SCOPE STATIC;

typedef enum opt_t {
	e_some,
	e_most,
	e_all,
	e_continue
} OPT_T;

extern SCHAR *PYXIS_get_keyname();
extern OBJ PYXIS_get_attribute_value();

static ATT add_text(), add_object(), add_field();
static SLONG edit_field();
static cleanup();

#define FIELD_EDIT	impure [0]
#define RESIZE_FORM	impure [1]
#define NAME_PROMPT	impure [2]
#define ENUMERATION	impure [3]
#define STRING1		impure [4]
#define STRING2		impure [5]
#define STRING3		impure [6]
#define STRING4		impure [7]
#define STRING5		impure [8]
#define STRING6		impure [9]
#define MENU1		impure [10]
#define MENU2		impure [11]
#define MENU3		impure [12]
#define MENU4		impure [13]
#define MENU5		impure [14]
#define MENU6		impure [15]
#define MENU7		impure [16]
#define IMPURE_MAX	17

static OBJ impure[IMPURE_MAX];

static UCHAR field_edit_form[] =
#include "../pyxis/field_edit.h"
, form_resize[] =
#include "../pyxis/size_form.h"
, form_form_name[] =
#include "../pyxis/form_name.h"
, form_field_name[] =
#include "../pyxis/field_name.h"
, form_enumerations[] =
#include "../pyxis/enumerations.h"
 ;

typedef struct dtyp {
	UCHAR *dtyp_string;
	USHORT dtyp_internal;
	USHORT dtyp_external;
	USHORT dtyp_length;
} *DTYP;

static struct dtyp dtypes[] = {
	(UCHAR *) "CHAR", dtype_text, blr_text, 0,
	(UCHAR *) "VARYING", dtype_varying, blr_varying, 0,
	(UCHAR *) "SHORT", dtype_short, blr_short, sizeof(SSHORT),
	(UCHAR *) "LONG", dtype_long, blr_long, sizeof(SLONG),
	(UCHAR *) "FLOAT", dtype_real, blr_float, sizeof(float),
	(UCHAR *) "DOUBLE", dtype_double, blr_double, sizeof(double),
#ifdef VMS
	(UCHAR *) "D_FLOAT", dtype_d_float, blr_d_float, sizeof(double),
#else
	(UCHAR *) "D_FLOAT", dtype_double, blr_d_float, sizeof(double),
#endif
	(UCHAR *) "DATE", dtype_timestamp, blr_timestamp, sizeof(GDS__QUAD),
	(UCHAR *) "BLOB", dtype_blob, blr_blob, sizeof(GDS__QUAD),
	NULL, 0, 0
};



PYXIS_edit(window, form, dbb, transaction)
	 WIN window;
	 OBJ form;
	 int *dbb, *transaction;
{
/**************************************
 *
 *	P Y X I S _ e d i t
 *
 **************************************
 *
 * Functional description
 *	Edit a form.  Mostly slop fields around.  Return TRUE
 *	if anything really changed.
 *
 **************************************/
	OBJ object;
	ATT attribute, next;
	SLONG stat;
	STATUS status[20];
	USHORT c, value, x, y, change;

	gds__window = (int *) window;
	change = FALSE;
	set_prototypes(form, FALSE);

#ifdef DEBUG
	if (!FIELD_EDIT && dbb) {
		PYXIS_load_form(status, dbb, transaction, &FIELD_EDIT,
						NULL_PTR, "FIELD_EDIT");
		PYXIS_load_form(status, dbb, transaction, &RESIZE_FORM,
						NULL_PTR, "SIZE_FORM");
		PYXIS_load_form(status, dbb, transaction, &ENUMERATION,
						NULL_PTR, "ENUMERATIONS");
	}
#endif

	for (attribute = NULL;;) {
		if (!attribute)
			attribute =
				(ATT) PYXIS_find_object(form, NULL_PTR, att_any, TRUE);
		set_tag_string(window, &STRING6,
					   "EDIT: Move cursor to object and hit %s",
					   PYXIS__KEY_ENTER);
		object = (attribute) ? attribute->att_value : NULL;
		if (object) {
			value = GET_VALUE(object, att_reverse_video);
			REPLACE_ATTRIBUTE(object, att_reverse_video, attype_numeric,
							  !value);
		}
		c = PYXIS_get_char(window, object, 0, 0);
		if (object)
			REPLACE_ATTRIBUTE(object, att_reverse_video, attype_numeric,
							  value);
		PYXIS_pop_form(window);
		switch (c) {
		case PYXIS__KEY_RIGHT:
		case PYXIS__KEY_DOWN:
		case PYXIS__KEY_LEFT:
		case PYXIS__KEY_UP:
		case '\t':
			if (attribute
				&& (next = (ATT) PYXIS_navigate(form, attribute, att_any, c)))
				attribute = next;
			break;

		case PYXIS__KEY_SCROLL_TOP:
			PYXIS_scroll_reset(window->win_form);
			break;

		case PYXIS__KEY_ENTER:
		default:
			if (c != PYXIS__KEY_ENTER && c != '\n')
				PYXIS_return_char(window, c);
			CASE_MENU(MENU_HANDLE MENU1 HORIZONTAL) 'Edit options: '
		    MENU_ENTREE 'SELECT':
                continue;

            MENU_ENTREE 'MOVE':
                if (!object)
                    break;
			    change |= move(window, form, object, attribute);

            MENU_ENTREE 'ADD':
                if (next = add_object(window, form, object, dbb, transaction)) {
                    attribute = next;
                    change = TRUE;
                }

            MENU_ENTREE 'CHANGE':
                if (!object)
                    break;
			    x = GET_VALUE(object, att_display_x);
			    y = GET_VALUE(object, att_display_y);
                if (attribute->att_name == att_label) {
                    set_tag_string(window, &STRING1,
							   "EDIT ADD_TEXT: Enter replacement text, terminate with <RETURN> or %s",
							   PYXIS__KEY_ENTER);
                    add_text(window, form, x, y, object);
                    PYXIS_pop_form(window);
                    change = TRUE;
                }
                else
                    edit_field(window, object, x, y, dbb, transaction);

		    MENU_ENTREE 'DELETE':
                if (!object)
                    break;
			    if (!
                    (next =
                     (ATT) PYXIS_find_object(form, attribute, att_any,
                                             TRUE))) next =
                                                         (ATT) PYXIS_find_object(form, attribute, att_any, FALSE);
                PYXIS_delete_attribute(form, attribute);
                attribute = next;
                change = TRUE;

		    MENU_ENTREE 'REVERSE':
                if (object)
                    if (attribute->att_name == att_label) {
                        value = !GET_VALUE(object, att_reverse_video);
                        REPLACE_ATTRIBUTE(object, att_reverse_video,
                                          attype_numeric, value);
                        change = TRUE;
                    }
                    else
                        change |= reverse(object);

		    MENU_ENTREE 'SAVE':
                save_form(window, form, dbb, transaction);

		    MENU_ENTREE 'Exit':
                /* set_prototypes (form, TRUE); */
                return change;
			END_MENU;
		}
	}
}


static ATT add_field(form, field)
	 OBJ form, field;
{
/**************************************
 *
 *	a d d _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Incrementally add a field to a form.
 *
 **************************************/
	OBJ label;
	ATT attribute, attr;
	USHORT x1, y1, x2, y2, min_width;
	TEXT *name;

/* Make up field label */

	name = GET_STRING(field, att_field_name);
	label = (OBJ) PYXIS_create_object(name, att_literal_string);

	if (!(min_width = default_position(form, att_label, &x1, &y1)))
		min_width = label->obj_width;

	x1 += min_width;
	x1 = MAX(x1, label->obj_width);
	PYXIS_position(label, x1 - label->obj_width, y1);
	attribute = (ATT) PUT_ATTRIBUTE(form, att_label, attype_object, label);
	PYXIS_move(form, label);

	PYXIS_delete_named_attribute(field, att_inactive);

	if (attr = (ATT) GET_ATTRIBUTE(field, att_fill_string))
		PYXIS_set_display_attribute(field, attr);

	default_position(form, att_field, &x2, &y2);
	if (y1 == y2)
		x2 = MAX(x1 + 2, x2);

	PYXIS_position(field, x2, y2);
	REPLACE_ATTRIBUTE(field, att_reverse_for_update, attype_numeric, TRUE);
	PUT_ATTRIBUTE(form, att_field, attype_object, field);
	PYXIS_move(form, field);

	return attribute;
}


static ATT add_object(window, form, object, dbb, transaction)
	 WIN window;
	 OBJ form, object;
	 int *dbb, *transaction;
{
/**************************************
 *
 *	a d d _ o b j e c t
 *
 **************************************
 *
 * Functional description
 *	Drive sub-menu to add objects to form.
 *
 **************************************/
	OBJ field;
	USHORT c, x, y, change;

	CASE_MENU(MENU_HANDLE MENU2 HORIZONTAL) 'Add options: ';
    MENU_ENTREE 'TEXT':
        x = y = 0;
	    if (object) {
            x = GET_VALUE(object, att_display_x);
            y = GET_VALUE(object, att_display_y);
        }
        set_tag_string(window, &STRING2,
				   "EDIT ADD_TEXT: Enter text, terminate with <RETURN> or %s",
				   PYXIS__KEY_ENTER);
        field = (OBJ) add_text(window, form, x, y, NULL_PTR);
        PYXIS_pop_form(window);
        return (ATT) field;

    MENU_ENTREE 'FIELD':
        field = (OBJ) PYXIS_create_object(NULL_PTR, 0);
	    default_position(form, att_field, &x, &y);
        edit_field(window, field, x, y, dbb, transaction);
        if (!GET_VALUE(field, att_field_name)) {
            PYXIS_delete_object(field);
            return NULL;
        }
        return add_field(form, field);

    MENU_ENTREE 'DATABASE FIELDS':
        return (ATT) copy_fields(window, form, object, dbb, transaction);

    MENU_ENTREE 'REPEATING SUB-FORM':
        return (ATT) add_repeating(window, form, object, dbb, transaction);

    MENU_ENTREE 'Exit':
        return NULL;

	END_MENU;
}


static add_repeating(window, form, object, dbb, transaction)
	 WIN window;
	 OBJ form, object;
	 int *dbb, *transaction;
{
/**************************************
 *
 *	a d d _ r e p e a t i n g
 *
 **************************************
 *
 * Functional description
 *	Add a repeating sub-form box.
 *
 **************************************/
	OBJ form_menu, entree, sub_form, box;
	ATT attribute;
	TEXT *form_name;
	USHORT x, y;

/* Generate and format relation menu */

	form_menu = (OBJ) PYXIS_forms(dbb, transaction);
	PYXIS_format_menu(form_menu, "Select Sub-Form", FALSE);
	x = (window->win_width - GET_VALUE(form_menu, att_width)) / 2;
	PYXIS_position(form_menu, x, 1);

/* Let user select relation.  If he doesn't pick anything, give up and
   go away */

	PYXIS_push_form(window, form_menu, FALSE);
	entree = (OBJ) PYXIS_menu(window, form_menu);

	if (!entree) {
		PYXIS_pop_form(window);
		PYXIS_delete_object(form_menu);
		return NULL;
	}

/* Pick up relation name, and build field menu */

	form_name = GET_STRING(entree, att_literal_string);
	PYXIS_load_form(gds__status, dbb, transaction, &sub_form,
					NULL_PTR, form_name);
	PYXIS_compute_size(sub_form, &x, &y);

/* Set up box to hold sub-form */

	box = (OBJ) PYXIS_create_object(NULL_PTR, 0);
	PUT_ATTRIBUTE(box, att_border, attype_numeric, 1);
	PUT_ATTRIBUTE(box, att_blank, attype_numeric, 1);
	PUT_ATTRIBUTE(box, att_box, attype_numeric, 1);
	PUT_ATTRIBUTE(box, att_width, attype_numeric, x + 2);
	PUT_ATTRIBUTE(box, att_height, attype_numeric, 3 * y + 2);
	PUT_ATTRIBUTE(box, att_prototype, attype_object, sub_form);
	PUT_ATTRIBUTE(box, att_field_name, attype_string, form_name);

/* Position box someplace */

	default_position(form, att_field, &x, &y);
	PYXIS_position(box, x, y);
	PUT_ATTRIBUTE(box, att_dtype, attype_numeric, dtype_short);
	PUT_ATTRIBUTE(box, att_length, attype_numeric, sizeof(SSHORT));
	add_field(form, box);

/* Clean up */

	PYXIS_pop_form(window);
	PYXIS_delete_object(form_menu);

	return NULL;
}


static ATT add_text(window, form, x, y, object)
	 WIN window;
	 OBJ form;
	 SLONG x, y;
	 OBJ object;
{
/**************************************
 *
 *	a d d _ t e x t
 *
 **************************************
 *
 * Functional description
 *	Add literal text to a form.  Follow cursor movements until
 *	a significant character shows up, then gobble text.
 *
 **************************************/
	UCHAR text[256], *p;
	ATT attribute, attr;
	SSHORT c;

/* Unless we're doing a straight replacement, follow cursor keys
   until text shows up */

	p = text;

	if (object)
		attribute = (ATT) GET_ATTRIBUTE(form, att_label);
	else {
		c = position(window, form, &x, &y);

		if (c == '\n')
			return NULL;

		*p++ = c;
		object = (OBJ) PYXIS_create_object(NULL_PTR, 0);
		PYXIS_position(object, x, y);
		attribute =
			(ATT) PUT_ATTRIBUTE(form, att_label, attype_object, object);
		PYXIS_move(form, object);
	}

/* Gobble characters until carriage return */

	*p = 0;

	for (;;) {
		attr =
			(ATT) REPLACE_ATTRIBUTE(object, att_literal_string, attype_string,
									text);
		PYXIS_set_display_attribute(object, attr);
		REPLACE_ATTRIBUTE(object, att_width, attype_numeric, p - text);
		c = PYXIS_get_char(window, object, p - text, 0);
		switch (c) {
		case PYXIS__KEY_UP:
		case PYXIS__KEY_DOWN:
		case PYXIS__KEY_RIGHT:
		case PYXIS__KEY_LEFT:
		case PYXIS__KEY_ENTER:
		case '\n':
			return attribute;

		case PYXIS__KEY_DELETE:
			if (p > text)
				*--p = 0;
			break;

		default:
			*p++ = c;
			*p = 0;
		}
	}
}


static cleanup()
{
/**************************************
 *
 *	c l e a n u p
 *
 **************************************
 *
 * Functional description
 *	Cleanup module statics to keep Apollo shared
 *	images happy.
 *
 **************************************/
	OBJ *p, *end;

	gds__window = NULL;
	CLEAR_HANDLES;

	for (p = impure, end = p + IMPURE_MAX; p < end;)
		*p++ = NULL;
}


static copy_fields(window, form, object, dbb, transaction)
	 WIN window;
	 OBJ form, object;
	 int *dbb, *transaction;
{
/**************************************
 *
 *	c o p y _ f i e l d s
 *
 **************************************
 *
 * Functional description
 *	Copy fields from dbb onto form.
 *
 **************************************/
	OBJ relation_menu, field_menu, entree, field;
	ATT attribute;
	TEXT *relation_name;
	USHORT x, y;

/* Generate and format relation menu */

	relation_menu = (OBJ) PYXIS_relations(dbb, transaction);
	PYXIS_format_menu(relation_menu, "Select Relation", FALSE);
	x = (window->win_width - GET_VALUE(relation_menu, att_width)) / 2;
	PYXIS_position(relation_menu, x, 1);

/* Let user select relation.  If he doesn't pick anything, give up and
   go away */

	PYXIS_push_form(window, relation_menu, FALSE);
	entree = (OBJ) PYXIS_menu(window, relation_menu);

	if (!entree) {
		PYXIS_pop_form(window);
		PYXIS_delete_object(relation_menu);
		return NULL;
	}

/* Pick up relation name, and build field menu */

	relation_name = GET_STRING(entree, att_literal_string);
	field_menu = (OBJ) PYXIS_relation_fields(dbb, transaction, relation_name);
	PYXIS_format_menu(field_menu, "Select Field(s)", FALSE);
	PYXIS_position(field_menu, x + strlen(relation_name), 3);
	PYXIS_push_form(window, field_menu, FALSE);
	PYXIS_select(window, field_menu, att_entree, NULL_PTR);

/* Pick up selected fields, if any */

	for (attribute = NULL;
		 attribute = (ATT) PYXIS_find_object(field_menu, attribute, att_entree, TRUE);) {
		entree = attribute->att_value;
		if (GET_VALUE(entree, att_reverse_video) &&
			(field = GET_OBJECT(entree, att_entree_value))) {
			x = 0;
			y = 0;
			field = (OBJ) PYXIS_clone(field);
			PYXIS_field_defaults(field);
			add_field(form, field, &x, &y);
		}
	}

/* Clean up */

	PYXIS_pop_form(window);
	PYXIS_pop_form(window);
	PYXIS_delete_object(field_menu);
	PYXIS_delete_object(relation_menu);

	return NULL;
}


static default_position(form, type, x_ptr, y_ptr)
	 OBJ form;
	 ATT_N type;
	 USHORT *x_ptr, *y_ptr;
{
/**************************************
 *
 *	d e f a u l t _ p o s i t i o n
 *
 **************************************
 *
 * Functional description
 *	Compute the default position of an object of a given type.
 *
 **************************************/
	OBJ entree;
	USHORT x, y, max_x, max_y, min_width, count;
	ATT attribute;

	max_x = max_y = count = 0;
	min_width = 9999;
	attribute = NULL;

	if (type == att_label)
		attribute = (ATT) PYXIS_find_object(form, attribute, type, TRUE);

	while (attribute = (ATT) PYXIS_find_object(form, attribute, type, TRUE)) {
		entree = attribute->att_value;
		++count;
		max_x = MAX(max_x, entree->obj_rel_x);
		max_y = MAX(max_y, entree->obj_rel_y);
		min_width = MIN(min_width, entree->obj_width);
	}

	*x_ptr = max_x;
	*y_ptr = max_y + ((count < 10) ? 2 : 1);

	return (count) ? min_width : 0;
}


static edit_enumerations(window, field)
	 WIN window;
	 OBJ field;
{
/**************************************
 *
 *	e d i t _ e n u m e r a t i o n s
 *
 **************************************
 *
 * Functional description
 *	Update enumerations, if any, for a field.
 *
 **************************************/
	OBJ values, value;
	ATT attribute;
	TEXT *data;
	USHORT width, height, x, y, index;

	manual_load(&ENUMERATION, form_enumerations);
	PYXIS_compute_size(ENUMERATION, &width, &height);
	x = (window->win_width - width) / 2;
	y = (window->win_height - height) / 2;
	PYXIS_position(ENUMERATION, x, y);
	values = (OBJ) PYXIS_find_field(ENUMERATION, "VALUES");

	FOR_FORM(FORM_HANDLE ENUMERATION TRANSPARENT) X IN ENUMERATIONS
		PYXIS_purge_segments(values, 1);
		for (index = 1, attribute = NULL;
			 attribute = (ATT) PYXIS_find_enumeration(field, attribute, TRUE);
			 ++index) 
            PYXIS_put_segment(values, attribute->att_value, index);
		DISPLAY X ACCEPTING VALUES;
		if (X.TERMINATOR != PYXIS__KEY_ENTER) {
			pyxis__pop_window(&gds__window);
			return;
		}
		PYXIS_delete_named_attribute(field, att_enumeration);
		for (index = 1; value = (OBJ) PYXIS_find_index(values, index);
			 index++) 
            if ((data = GET_STRING(value, att_update_data))
                || (data = GET_STRING(value, att_data)))
				PUT_ATTRIBUTE(field, att_enumeration, attype_string, data);
    END_FORM;
}


static SLONG edit_field(window, field, x, y, dbb, transaction)
	 WIN window;
	 OBJ field;
	 USHORT x, y;
	 int *dbb, *transaction;
{
/**************************************
 *
 *	e d i t _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Edit characteristics of a field.  If the field doesn't exits,
 *	make one up.
 *
 **************************************/
	OBJ prototype;
	DTYP dtype;
	USHORT length, width, offset, height, c;
	UCHAR *string, *p;
	SLONG scale;
	BOOLEAN fnam_upd, edstr_upd, fstr_upd, scale_upd, w_upd;

/* If this is a sub-form, just re-size it for now */

	if (prototype = GET_OBJECT(field, att_prototype)) {
		CASE_MENU(MENU_HANDLE MENU3 HORIZONTAL) 'Change: '
        MENU_ENTREE 'CHARACTERISTICS':
            manual_load(&NAME_PROMPT, form_field_name);
            FOR_FORM(TAG FORM_HANDLE NAME_PROMPT) F IN FIELD_NAME
                strcpy(F.FIELD_NAME, GET_VALUE(field, att_field_name));
                DISPLAY F DISPLAYING FIELD_NAME ACCEPTING FIELD_NAME WAKING ON FIELD_NAME;
                if (F.FIELD_NAME.STATE == PYXIS__OPT_USER_DATA)
                    REPLACE_ATTRIBUTE(field, att_field_name, attype_string, F.FIELD_NAME);
                END_FORM;
                return;

        MENU_ENTREE 'REGION':
            size_form(window, field);
            return;

        MENU_ENTREE 'SUB-ITEM':
            PYXIS_edit(window, prototype, dbb, transaction);
            return;

        MENU_ENTREE 'SIZE':
            size_form(window, prototype);
            return;

        MENU_ENTREE 'Exit':
            return;
	   END_MENU
    }
	else
	   CASE_MENU(MENU_HANDLE MENU4 HORIZONTAL) 'Change: '
       MENU_ENTREE 'CHARACTERISTICS':

       MENU_ENTREE 'ENUMERATIONS':
            return edit_enumerations(window, field);

       MENU_ENTREE 'EXIT':
            return;
	   END_MENU;

/* Set up data type */

	dtype = dtypes;

	if (c = GET_VALUE(field, att_datatype)) {
		for (dtype = dtypes; dtype->dtyp_string; dtype++)
			if (c == dtype->dtyp_external)
				break;
	}
	else if (c = GET_VALUE(field, att_dtype)) {
		for (dtype = dtypes; dtype->dtyp_string; dtype++)
			if (c == dtype->dtyp_internal)
				break;
	}

	if (!dtype->dtyp_string)
		dtype = dtypes;

/* Initialize the field edit form, if necessary */

	manual_load(&FIELD_EDIT, field_edit_form);

	PYXIS_compute_size(FIELD_EDIT, &width, &height);
	x = MIN(x, window->win_width - width - 1);
	y = MIN(y, window->win_height - height - 1);
	PYXIS_position(FIELD_EDIT, x, y);

	FOR_FORM(FORM_HANDLE FIELD_EDIT TRANSPARENT) X IN FIELD_EDIT
		put_string(X.FIELD_NAME, sizeof X.FIELD_NAME, field, att_field_name);
		put_string(X.EDIT_STRING, sizeof X.EDIT_STRING, field,
				   att_edit_string);
		put_string(X.FILL_STRING, sizeof X.FILL_STRING, field,
				   att_fill_string);
		strcpy(X.DATATYPE, dtype->dtyp_string);
		X.WIDTH = GET_VALUE(field, att_width);
		X.LENGTH = GET_VALUE(field, att_length);
		X.SCALE = GET_VALUE(field, att_scale);
		strcpy(X.UPCASE, (GET_VALUE(field, att_upcase)) ? "Y" : "N");
		strcpy(X.ALIGN_RIGHT,
			   (GET_VALUE(field, att_insert_right)) ? "Y" : "N");
		fnam_upd = edstr_upd = fstr_upd = scale_upd = w_upd = FALSE;

		/* Update all fields */

		for (;;) {
			if (dtype->dtyp_length) {
				X.LENGTH = dtype->dtyp_length;
				X.LENGTH.STATE = PYXIS__OPT_DISPLAY;
			}
			else
				X.LENGTH.STATE = PYXIS__OPT_UPDATE | PYXIS__OPT_DISPLAY;

			switch (dtype->dtyp_internal) {
			case dtype_short:
			case dtype_long:
			case dtype_quad:
				X.SCALE.STATE = PYXIS__OPT_DISPLAY | PYXIS__OPT_UPDATE;
				break;

			default:
				X.SCALE.STATE = 0;
			}

			DISPLAY X DISPLAYING *ACCEPTING *
				WAKING ON DATATYPE OVERRIDING LENGTH, SCALE;
			for (dtype = dtypes; dtype->dtyp_string; dtype++)
				if (!strcmp(X.DATATYPE, dtype->dtyp_string))
					break;

			if (X.FIELD_NAME.STATE == PYXIS__OPT_USER_DATA)
				fnam_upd = TRUE;
			if (X.EDIT_STRING.STATE == PYXIS__OPT_USER_DATA)
				edstr_upd = TRUE;
			if (X.FILL_STRING.STATE == PYXIS__OPT_USER_DATA)
				fstr_upd = TRUE;
			if (X.SCALE.STATE == PYXIS__OPT_USER_DATA)
				scale_upd = TRUE;
			if (X.WIDTH.STATE == PYXIS__OPT_USER_DATA)
				w_upd = TRUE;
			if (strcmp(X.TERMINATING_FIELD, "DATATYPE"))
				break;
		}

		if (X.TERMINATOR == PYXIS__KEY_ENTER) {
			REPLACE_ATTRIBUTE(field, att_dtype, attype_numeric,
							  dtype->dtyp_internal);
			REPLACE_ATTRIBUTE(field, att_length, attype_numeric, X.LENGTH);

			if (fnam_upd)
				REPLACE_ATTRIBUTE(field, att_field_name, attype_string,
								  X.FIELD_NAME);

			if (edstr_upd)
				REPLACE_ATTRIBUTE(field, att_edit_string, attype_string,
								  X.EDIT_STRING);

			if (fstr_upd)
				REPLACE_ATTRIBUTE(field, att_fill_string, attype_string,
								  X.FILL_STRING);

			if (scale_upd)
				REPLACE_ATTRIBUTE(field, att_scale, attype_numeric, X.SCALE);

			if (w_upd) {
				REPLACE_ATTRIBUTE(field, att_width, attype_numeric, X.WIDTH);
				if (dtype->dtyp_internal == dtype_text && X.LENGTH != X.WIDTH)
					REPLACE_ATTRIBUTE(field, att_length, attype_numeric,
									  X.WIDTH);
			}

			PYXIS_field_defaults(field);

			if (X.UPCASE[0] == 'Y')
				REPLACE_ATTRIBUTE(field, att_upcase, attype_numeric, TRUE);
			else
				PYXIS_delete_named_attribute(field, att_upcase);

			if (X.ALIGN_RIGHT[0] == 'Y')
				REPLACE_ATTRIBUTE(field, att_insert_right, attype_numeric,
								  TRUE);
			else
				PYXIS_delete_named_attribute(field, att_insert_right);
		}
	END_FORM;

	return (SLONG) field;
}


static manual_load(handle, def)
	 OBJ *handle;
	 UCHAR *def;
{
/**************************************
 *
 *	m a n u a l _ l o a d 
 *
 **************************************
 *
 * Functional description
 *	Manually load a form from a literal, unless it is
 *	already loaded.
 *
 **************************************/

	if (!*handle)
		*handle = (OBJ) PYXIS_load(&def);
}


static motion(window, form, field, x_ptr, y_ptr)
	 WIN window;
	 OBJ form, field;
	 int *x_ptr, *y_ptr;
{
/**************************************
 *
 *	m o t i o n
 *
 **************************************
 *
 * Functional description
 *	Pick up a single cursor motion, including count.
 *	Compute new coordinates, and return character.
 *
 **************************************/
	SSHORT x, y;
	SSHORT c, count, cnt, border;

	border = GET_VALUE(form, att_border);
	x = *x_ptr;
	y = *y_ptr;
	count = 0;

/*  Sensitize any mouse movement.  */

	window->win_flags |= WIN_sensitive;

	for (;;) {
		if (field)
			c = PYXIS_get_char(window, field, 0, 0);
		else
			c = PYXIS_get_char(window, form, x + border, y + border);
		cnt = (count) ? count : 1;
		switch (c) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			count = count * 10 + c - '0';
			continue;

		case PYXIS__KEY_UP:
			if (window->win_flags & WIN_synthesized)
				cnt = 1;
			y -= cnt;
			if (y < 0)
				y = 0;
			count = 0;
			break;

		case PYXIS__KEY_DOWN:
			if (window->win_flags & WIN_synthesized)
				cnt = 1;
			y += cnt;
			count = 0;
			break;

		case PYXIS__KEY_LEFT:
			if (window->win_flags & WIN_synthesized)
				cnt = 1;
			x -= cnt;
			if (x < 0)
				x = 0;
			count = 0;
			break;

		case PYXIS__KEY_RIGHT:
			if (window->win_flags & WIN_synthesized)
				cnt = 1;
			x += cnt;
			count = 0;
			break;

		default:
			break;
		}
		*x_ptr = x;
		*y_ptr = y;
		window->win_flags &= ~WIN_sensitive;
		return c;
	}
}


static move(window, form, object, start)
	 WIN window;
	 OBJ form, object;
	 ATT start;
{
/**************************************
 *
 *	m o v e
 *
 **************************************
 *
 * Functional description
 *	Move an object around.  Return TRUE if it really moved.
 *
 **************************************/
	OBJ first;
	ATT attribute, attr;
	OPT_T option;
	USHORT c, change, reverse;
	SLONG x, y, min_x, min_y, delta_x, delta_y;

	CASE_MENU(MENU_HANDLE MENU5 HORIZONTAL) 'Move options: '
    MENU_ENTREE 'ITEM':
        return move_item(window, form, object);

    MENU_ENTREE 'SOME':
        option = e_some;

    MENU_ENTREE 'MOST':
        option = e_most;

    MENU_ENTREE 'ALL':
        option = e_all;

    MENU_ENTREE 'Exit':
        return FALSE;
	END_MENU;

/* Mass move -- start by saving current states of reverse video */

	change = FALSE;
	attribute = NULL;

	while (attribute = (ATT) PYXIS_find_object(form, attribute, att_any, TRUE)) {
		object = attribute->att_value;
		REPLACE_ATTRIBUTE(object, att_temp, attype_numeric,
						  GET_VALUE(object, att_reverse_video));
		REPLACE_ATTRIBUTE(object, att_reverse_video, attype_numeric,
						  (option == e_some) ? FALSE : TRUE);
	}

/* Let user make fine adjustments */

	if (option != e_all) {
		set_tag_string(window, &STRING4,
					   "EDIT MOVE: use <RETURN> to select/deselect items", 0);
		PYXIS_select(window, form, att_any, start);
		PYXIS_pop_form(window);
	}

/* Find the top, rightmost selected object */

	attribute = NULL;
	first = NULL;
	min_x = min_y = INITIAL_MAX;

	while (attribute = (ATT) PYXIS_find_object(form, attribute, att_any, TRUE)) {
		object = attribute->att_value;
		if (GET_VALUE(object, att_reverse_video)) {
			x = GET_VALUE(object, att_display_x);
			y = GET_VALUE(object, att_display_y);
			min_x = MIN(min_x, x);
			min_y = MIN(min_y, y);
			if (!first)
				first = object;
		}
	}

/* Move group */

	x = min_x;
	y = min_y;

	for (;;) {
		delta_x = x;
		delta_y = y;
		set_tag_string(window, &STRING5,
					   "EDIT MOVE: Use cursor keys to move field; hit %s when done",
					   PYXIS__KEY_ENTER);
		c = motion(window, form, first, &x, &y);
		PYXIS_pop_form(window);
		if (c == PYXIS__KEY_ENTER || c == '\n')
			break;
		switch (c) {
		case PYXIS__KEY_UP:
		case PYXIS__KEY_DOWN:
		case PYXIS__KEY_LEFT:
		case PYXIS__KEY_RIGHT:
			break;

		default:
			continue;
		}
		delta_x = x - delta_x;
		delta_y = y - delta_y;
		change = TRUE;
		form->obj_flags |= OBJ_changed;
		while (attribute =
			   (ATT) PYXIS_find_object(form, attribute, att_any, TRUE)) {
			object = attribute->att_value;
			if (GET_VALUE(object, att_reverse_video)) {
				min_x = GET_VALUE(object, att_display_x);
				min_y = GET_VALUE(object, att_display_y);
				PYXIS_position(object, min_x + delta_x, min_y + delta_y);
			}
		}
	}

/* Restore original reverse video */

	while (attribute =
		   (ATT) PYXIS_find_object(form, attribute, att_any, TRUE)) {
		object = attribute->att_value;
		if (attr = (ATT) GET_ATTRIBUTE(object, att_temp)) {
			PYXIS_move(form, object);
			REPLACE_ATTRIBUTE(object, att_reverse_video, attype_numeric,
							  attr->att_value);
			PYXIS_delete_attribute(object, attr);
			attribute = NULL;
		}
	}

	return change;
}


static move_item(window, form, object)
	 WIN window;
	 OBJ form, object;
{
/**************************************
 *
 *	m o v e _ i t e m
 *
 **************************************
 *
 * Functional description
 *	Move an object around.  Return TRUE if it really moved.
 *
 **************************************/
	USHORT c, change;
	SLONG x, y;

	change = FALSE;
	x = GET_VALUE(object, att_display_x);
	y = GET_VALUE(object, att_display_y);

	for (;;) {
		set_tag_string(window, &STRING3,
					   "EDIT MOVE: Use cursor keys to move field; hit %s when done",
					   PYXIS__KEY_ENTER);
		/*  c = motion (window, form, &x, &y);  */
		c = motion(window, form, object, &x, &y);
		PYXIS_pop_form(window);
		switch (c) {
		case PYXIS__KEY_UP:
		case PYXIS__KEY_DOWN:
		case PYXIS__KEY_LEFT:
		case PYXIS__KEY_RIGHT:
			break;

		case '\n':
		case PYXIS__KEY_ENTER:
			PYXIS_move(form, object);
			return change;

		default:
			continue;
		}
		change = TRUE;
		PYXIS_position(object, x, y);
		form->obj_flags |= OBJ_changed;
	}
}


static position(window, form, x_ptr, y_ptr)
	 WIN window;
	 OBJ form;
	 int *x_ptr, *y_ptr;
{
/**************************************
 *
 *	p o s i t i o n
 *
 **************************************
 *
 * Functional description
 *	Position ourselves somewhere within the form.  Stop
 *	on a significant character and return.
 *
 **************************************/
	SSHORT c;

	for (;;)
		switch (c = motion(window, form, NULL_PTR, x_ptr, y_ptr)) {
		case PYXIS__KEY_UP:
		case PYXIS__KEY_DOWN:
		case PYXIS__KEY_LEFT:
		case PYXIS__KEY_RIGHT:
			break;

		default:
			return c;
		}
}


static put_string(target, target_size, field, attribute)
	 UCHAR *target;
	 int target_size;
	 OBJ field;
	 USHORT attribute;
{
/**************************************
 *
 *	p u t _ s t r i n g
 *
 **************************************
 *
 * Functional description
 *	Copy string from form field to form.
 *
 **************************************/
	TEXT *string;

	if (!(string = GET_STRING(field, attribute)))
		string = "";

	strncpy(target, string, target_size);
}


static reverse(object)
	 OBJ object;
{
/**************************************
 *
 *	r e v e r s e
 *
 **************************************
 *
 * Functional description
 *	Set reverse video family of attributes.
 *
 **************************************/
	SLONG value;

	CASE_MENU(MENU_HANDLE MENU6 HORIZONTAL) 'Reverse options: '
    MENU_ENTREE 'INVERT':
        value = !GET_VALUE(object, att_reverse_video);
	    REPLACE_ATTRIBUTE(object, att_reverse_video, attype_numeric, value);
        return TRUE;

    MENU_ENTREE 'REVERSE ON UPDATE':
        REPLACE_ATTRIBUTE(object, att_reverse_for_update, attype_numeric, TRUE);
	    PYXIS_delete_named_attribute(object, att_reverse_video);
        return TRUE;

    MENU_ENTREE 'NORMAL':
        PYXIS_delete_named_attribute(object, att_reverse_for_update);
	    PYXIS_delete_named_attribute(object, att_reverse_video);
        return TRUE;

    MENU_ENTREE 'Exit':
        return FALSE;

	END_MENU;
}


static save_form(window, form, dbb, transaction)
	 WIN window;
	 OBJ form;
	 int *dbb, *transaction;
{
/**************************************
 *
 *	s a v e _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Save a form in a dbb.
 *
 **************************************/

	manual_load(&NAME_PROMPT, form_form_name);

	FOR_FORM(TAG FORM_HANDLE NAME_PROMPT) F IN FORM_NAME
		for (;;) {
			DISPLAY F ACCEPTING FORM_NAME WAKING ON FORM_NAME;
			if (F.FORM_NAME.STATE != PYXIS__OPT_USER_DATA ||
				validate_name(F.FORM_NAME)) 
                break;
		}
		if (F.FORM_NAME.STATE == PYXIS__OPT_USER_DATA)
			PYXIS_store_form(dbb, transaction, F.FORM_NAME, form);
	END_FORM;
}


static set_prototypes(object, state)
	 OBJ object;
	 USHORT state;
{
/**************************************
 *
 *	s e t _ p r o t o t y p e s
 *
 **************************************
 *
 * Functional description
 *	Set prototypes active or inactive.
 *
 **************************************/
	OBJ sub;
	ATT attribute;

	for (attribute = object->obj_attributes; attribute;
		 attribute =
		 attribute->att_next) 
        if (attribute->att_type == attype_object) {
			sub = attribute->att_value;
			if (attribute->att_name == att_prototype)
				REPLACE_ATTRIBUTE(sub, att_inactive, attype_numeric, state);
			set_prototypes(sub, state);
		}
}


static set_tag_string(window, ptr, string, keycode)
	 WIN window;
	 OBJ *ptr;
	 UCHAR *string, keycode;
{
/**************************************
 *
 *	s e t _ t a g _ s t r i n g
 *
 **************************************
 *
 * Functional description
 *	Set up tag string, including a possible key name.
 *
 **************************************/

	UCHAR buffer[100];

	if (!*ptr) {
		if (keycode) {
			sprintf(buffer, string, PYXIS_get_keyname(window, keycode));
			string = buffer;
		}
		*ptr = (OBJ) PYXIS_create_object(string, att_literal_string);
	}

	PYXIS_push_tag(window, *ptr);
}


static size_form(window, form)
	 WIN window;
	 OBJ form;
{
/**************************************
 *
 *	s i z e _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Change size or outline of form.
 *
 **************************************/
	UCHAR *p;
	USHORT c, width, height, x, y, box;

	manual_load(&RESIZE_FORM, form_resize);
	PYXIS_compute_size(RESIZE_FORM, &x, &y);
	x = (window->win_width - x) / 2;
	y = (window->win_height - y) / 2;
	PYXIS_position(RESIZE_FORM, x, y);

/* Compute "current" size of form */

	box = GET_VALUE(form, att_box);
	width = GET_VALUE(form, att_width);
	height = GET_VALUE(form, att_height);
	PYXIS_compute_size(form, &x, &y);

	if (!width)
		width = x + 2;
	if (!height)
		height = y + 2;

	REPLACE_ATTRIBUTE(form, att_blank, attype_numeric, TRUE);

	FOR_FORM(FORM_HANDLE RESIZE_FORM TRANSPARENT) X IN SIZE_FORM
		X.WIDTH = width;
		X.HEIGHT = height;
		strcpy(X.OUTLINE_FORM, (box) ? "Y" : "N");
		DISPLAY X DISPLAYING *ACCEPTING *;
		if (X.TERMINATOR == PYXIS__KEY_ENTER) {
			if (X.WIDTH.STATE == PYXIS__OPT_USER_DATA)
				REPLACE_ATTRIBUTE(form, att_width, attype_numeric, X.WIDTH);
			if (X.HEIGHT.STATE == PYXIS__OPT_USER_DATA)
				REPLACE_ATTRIBUTE(form, att_height, attype_numeric, X.HEIGHT);
			if (X.OUTLINE_FORM.STATE == PYXIS__OPT_USER_DATA)
				if (X.OUTLINE_FORM[0] == 'Y') {
					REPLACE_ATTRIBUTE(form, att_box, attype_numeric, TRUE);
					REPLACE_ATTRIBUTE(form, att_border, attype_numeric, TRUE);
				}
				else {
					PYXIS_delete_named_attribute(form, att_box);
					PYXIS_delete_named_attribute(form, att_border);
				}
		}
	END_FORM;
}


static validate_name(string)
	 UCHAR *string;
{
/**************************************
 *
 *	v a l i d a t e _ n a m e
 *
 **************************************
 *
 * Functional description
 *	Validate a file and/or form name.
 *
 **************************************/

	for (; *string; string++)
		if (*string == ' ' || *string == '\t')
			return FALSE;

	return TRUE;
}
