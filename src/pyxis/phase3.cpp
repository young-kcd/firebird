/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		phase3.c
 *	DESCRIPTION:	Routines for preprocessor generator stuff
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
/*
$Id: phase3.cpp,v 1.2 2001-07-12 05:46:05 bellardo Exp $
*/

#include <stdio.h>

#include <setjmp.h>
#include "../jrd/common.h"
#include <stdarg.h>
#include "../include/jrd/gds.h"
#include "../jrd/gds_proto.h"
#include "../pyxis/everything.h"
#include "../pyxis/pyxis.h"
#include "../pyxis/phase2.h"
#include "../pyxis/save.h"

BLK PYXIS_alloc(PLB , UCHAR , int );
void MOVP_move( DSC *, DSC *);
SLONG MOVP_get_long(DSC * , SSHORT );
BLK PYXIS_pop(register LLS *);
int PYXIS_push(BLK , register LLS *);

static MENU par_menu(WIN , USHORT *, SCHAR *);
static int get_field_value(WIN , OBJ , OBJ , DSC *);
static OBJ find_field(MAP , OBJ , map::map_repeat *);
static int get_blob(OBJ , DSC *, SLONG **, SLONG **);
static MAP par_message(STATUS *, TEXT **);
static int par_word(TEXT **);
static int propogate_options(map::map_repeat *, SLONG );
static int put_blob(OBJ , DSC *, SLONG **, SLONG **);
static int put_value(MAP , USHORT , UCHAR *, SLONG );
static int unwind(TEXT *);
static int error(STATUS *, TEXT *, ...);

int pyxis__insert(STATUS *, SLONG **, SLONG *, MAP *, UCHAR *);
int pyxis__fetch(STATUS *, SLONG **, SLONG *, MAP *, UCHAR *);
int pyxis__initialize_menu(MENU *);

extern "C" int PYXIS_format_menu(OBJ , TEXT *, int );
extern OBJ PYXIS_drive_menu(WIN , OBJ , USHORT *);
extern "C" OBJ PYXIS_menu(WIN , OBJ );
extern ATT PYXIS_create_entree(OBJ , TEXT *, int , BLK );


#define BLR_BYTE	(*p++)
#define BLR_WORD	par_word (&p)


static jmp_buf unwind_env;
static TEXT error_buffer[256];


int pyxis__compile_map(STATUS *status, OBJ *form_handle, MAP *map_handle,
     USHORT *length, SCHAR *source)
{
/**************************************
 *
 *	p y x i s _ $ c o m p i l e _ m a p
 *
 **************************************
 *
 * Functional description
 *	Compile a form map.
 *
 **************************************/
	MAP map;
	USHORT n;
	OBJ form, field;
	STR string;
	SCHAR *p, c;
	TEXT *q, name[32];
	map::map_repeat *tail;

	form = *form_handle;
	p = source;
	c = BLR_BYTE;

	if (c != PYXIS_MAP_VERSION1)
		return error(status, "unsupported map version %d", c);

	while ((c = BLR_BYTE) != PYXIS_MAP_END)
		switch (c) {
		case PYXIS_MAP_MESSAGE:
			if (!(map = par_message(status, &p)))
				return FALSE;
			map->map_form = form;
			break;

		case PYXIS_MAP_FIELD1:
		case PYXIS_MAP_FIELD2:
			q = name;
			if (n = BLR_BYTE)
				do
					*q++ = BLR_BYTE;
				while (--n);
			*q = 0;
			n = BLR_WORD;
			tail = map->map_rpt + n;
			if (!(tail->map_field = PYXIS_find_field(form, name)))
				return error(status, "field %s not defined", name);
			if (c == PYXIS_MAP_FIELD2) {
				tail->map_field_flags |= MAP_options;
				tail->map_option = BLR_WORD;
			}
			break;

		case PYXIS_MAP_SUB_FIELD:

			/* Get repeating group field name */

			q = name;
			if (n = BLR_BYTE)
				do
					*q++ = BLR_BYTE;
				while (--n);
			*q = 0;

			/* Get target field name */

			n = BLR_BYTE;
			string = (STR) ALLOCPV(type_str, n);
			q = string->str_data;
			if (n)
				do
					*q++ = BLR_BYTE;
				while (--n);

			/* Set up tail slot */

			n = BLR_WORD;
			tail = map->map_rpt + n;
			if (!(tail->map_field = PYXIS_find_field(form, name)))
				return error(status, "field %s not defined", name);
			if (!(field = GET_OBJECT(tail->map_field, att_prototype)))
				return error(status, "field %s is not sub_form", name);
			tail->map_name = string->str_data;

			if (!PYXIS_find_field(field, tail->map_name))
				return error(status, "field %s not defined", tail->map_name);

			tail->map_field_flags |= MAP_options;
			tail->map_option = BLR_WORD;
			break;

		case PYXIS_MAP_OPAQUE:
			map->map_flags &= ~MAP_transparent;
			break;

		case PYXIS_MAP_TRANSPARENT:
			map->map_flags |= MAP_transparent;
			break;

		case PYXIS_MAP_TAG:
			map->map_flags |= MAP_tag;
			break;

		case PYXIS_MAP_TERMINATOR:
			map->map_terminator = par_word(&p) + 1;
			break;

		case PYXIS_MAP_TERMINATING_FIELD:
			map->map_terminating_field = par_word(&p) + 1;
			break;

		default:
			return error(status, "invalid form map");
		}

	*map_handle = map;
}


int pyxis__compile_menu(WIN *window_handle, MENU *menu_handle, USHORT *length,
             TEXT *source)
{
/**************************************
 *
 *	p y x i s _ $ c o m p i l e _ m e n u
 *
 **************************************
 *
 * Functional description
 *	Compile a menu.
 *
 **************************************/

	*menu_handle = par_menu(*window_handle, length, source);
}


int pyxis__compile_sub_map(STATUS *status, MAP *parent_handle, MAP *map_handle,
          USHORT *length, SCHAR *source)
{
/**************************************
 *
 *	p y x i s _ $ c o m p i l e _ s u b _ m a p
 *
 **************************************
 *
 * Functional description
 *	Compile a subform map.
 *
 **************************************/
	MAP map, parent;
	USHORT n;
	STR string;
	SCHAR *p, c;
	TEXT *q, name[32];
	map::map_repeat *tail;

	parent = *parent_handle;
	p = source;
	c = BLR_BYTE;

	if (c != PYXIS_MAP_VERSION1)
		return error(status, "unsupported map version %d", c);

	while ((c = BLR_BYTE) != PYXIS_MAP_END)
		switch (c) {
		case PYXIS_MAP_MESSAGE:
			if (!(map = par_message(status, &p)))
				return FALSE;
			break;

		case PYXIS_MAP_SUB_FORM:
			q = name;
			if (n = BLR_BYTE)
				do
					*q++ = BLR_BYTE;
				while (--n);
			*q = 0;
			if (!(map->map_parent = PYXIS_find_field(parent->map_form, name)))
				return error(status, "sub-form %s not defined", name);
			if (!
				(map->map_prototype =
				 GET_OBJECT(map->map_parent,
							att_prototype))) return error(status,
														  "field %s is not a sub-form",
														  name);
			break;

		case PYXIS_MAP_FIELD1:
		case PYXIS_MAP_FIELD2:
			n = BLR_BYTE;
			string = (STR) ALLOCPV(type_str, n);
			q = string->str_data;
			if (n)
				do
					*q++ = BLR_BYTE;
				while (--n);
			n = BLR_WORD;
			tail = map->map_rpt + n;
			tail->map_name = string->str_data;
			if (!map->map_prototype
				|| !PYXIS_find_field(map->map_prototype,
									 tail->map_name)) return error(status,
																   "field %s not defined",
																   tail->map_name);
			if (c == PYXIS_MAP_FIELD2) {
				tail->map_field_flags |= MAP_options;
				tail->map_option = BLR_WORD;
			}
			break;

		case PYXIS_MAP_ITEM_INDEX:
			map->map_item_index = par_word(&p) + 1;
			break;

		default:
			return error(status, "invalid form map");
		}

	*map_handle = map;
}



int pyxis__create_window(WIN *window_handle, USHORT *file_name_length,
              TEXT *file_name, USHORT *width, USHORT *height)
{
/**************************************
 *
 *	p y x i s _ $ c r e a t e _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Create a new window.
 *
 **************************************/
	WIN window;

	if (*window_handle = window = PYXIS_create_window(*width, *height)) {
		*width = window->win_width;
		*height = window->win_height;
	}
}


int pyxis__delete(OBJ *object)
{
/**************************************
 *
 *	p y x i s _ $ d e l e t e
 *
 **************************************
 *
 * Functional description
 *	Delete an object.
 *
 **************************************/

	if (*object)
		PYXIS_delete_object(*object);
}




int pyxis__delete_window(SLONG *window_handle)
{
/**************************************
 *
 *	p y x i s _ $ d e l e t e _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Delete a window.
 *
 **************************************/

	if (*window_handle) {
		PYXIS_delete_window((WIN) *window_handle);
		*window_handle = NULL;
	}
}


int pyxis__drive_form(STATUS *status, SLONG **db_handle, SLONG *tra_handle, 
                      WIN *window_handle, MAP *map_handle,
		      UCHAR *input, UCHAR *output)
{
/**************************************
 *
 *	p y x i s _ $ d r i v e _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Copy data from a form (or subform).
 *
 **************************************/
	MAP map;
	DSC desc, desc2;
	WIN window;
	OBJ form, field;
	USHORT source, c;
	SSHORT n;

/* If the window doesn't exist, create it */

	if (!(window = *window_handle))
		*window_handle = window = PYXIS_create_window(80, 24);

/* Initialize some local stuff */

	map = *map_handle;
	map->map_window = window;
	form = map->map_form;

/* If there is an input record, initialize form */

	if (input)
		pyxis__insert(status, db_handle, tra_handle, map_handle, input);

/* Drive form */

	if (map->map_flags & MAP_tag)
		PYXIS_push_tag(window, form);
	else
		PYXIS_push_form(window, form,
						(map->map_flags & MAP_transparent) ? FALSE : TRUE);

/* If there isn't an output record, display the form and get out */

	if (!output) {
		PYXIS_update_window(window, 0, 0, 0);
		return SUCCESS;
	}

/* Let user muddle around waiting for input */

	field =
		PYXIS_update(window, form, &map->map_context, &map->map_wakeup_char);
	desc2.dsc_dtype = dtype_cstring;
	desc2.dsc_length = 256;
	desc2.dsc_scale = 0;

/* Return terminator, if appropriate */

	if (n = map->map_terminator)
		put_value(map, n - 1, output, map->map_wakeup_char);

	if (!field)
		map->map_wakeup_char = 0;

/* Handle terminating field */

	if (n = map->map_terminating_field) {
		if (field)
			desc2.dsc_address = (UCHAR *) GET_STRING(field, att_field_name);
		else
			desc2.dsc_address = (UCHAR *) "";
		desc = map->map_rpt[n - 1].map_desc;
		desc.dsc_address = output + (SLONG) desc.dsc_address;
		MOVP_move(&desc2, &desc);
	}

/* Handle general output */

	pyxis__fetch(status, db_handle, tra_handle, map_handle, output);

	return SUCCESS;
}


int pyxis__drive_menu(WIN *window_handle, MENU *menu_handle, USHORT *blr_length,
      SCHAR *blr_source, USHORT *title_length, TEXT *title, USHORT *terminator,
      USHORT *entree_length, TEXT *entree_text, SLONG *entree_value)
{
/**************************************
 *
 *	p y x i s _ $ d r i v e _ m e n u
 *
 **************************************
 *
 * Functional description
 *	Build then drive a dynamic menu.
 *
 **************************************/
	WIN window;
	MENU menu;
	OBJ object, entree;
	USHORT x, i, horizontal, transparent;
	SCHAR *p, c;
	TEXT *fp, *tp, title_buffer[81];

	if (!(window = *window_handle))
		*window_handle = window = PYXIS_create_window(80, 24);

	if (!(menu = *menu_handle)) {
		pyxis__initialize_menu(menu_handle);
		menu = *menu_handle;
	}

	object = menu->menu_object;

	p = blr_source;
	c = BLR_BYTE;

	if (c != PYXIS_MENU_VERSION1) {
		error(0, "unsupported map version %d", c);
		return NULL;
	}

	while ((c = BLR_BYTE) != PYXIS_MENU_END)
		switch (c) {
		case PYXIS_MENU_OPAQUE:
			menu->menu_flags &= ~MENU_transparent;
			break;

		case PYXIS_MENU_TRANSPARENT:
			menu->menu_flags |= MENU_transparent;
			break;

		case PYXIS_MENU_HORIZONTAL:
			object->obj_flags |= OBJ_menu_horizontal;
			break;

		case PYXIS_MENU_VERTICAL:
			object->obj_flags &= ~OBJ_menu_horizontal;
			break;

		default:
			error(0, "invalid menu");
			return 0;
		}

	for (i = *title_length, fp = title, tp = title_buffer; i > 0;
		 i--, fp++, tp++)
		*tp = *fp;
	*tp = '\0';

	horizontal = object->obj_flags & OBJ_menu_horizontal;
	transparent = menu->menu_flags & MENU_transparent;
	PYXIS_format_menu(object, title_buffer, horizontal);

	if (!horizontal) {
		x = (window->win_width - GET_VALUE(object, att_width)) / 2;
		PYXIS_position(object, x, 1);
	}

	if (horizontal)
		PYXIS_push_tag(window, object);
	else
		PYXIS_push_form(window, object, !transparent);

	entree = PYXIS_drive_menu(window, object, terminator);

	PYXIS_pop_form(window);

	if (!entree) {
		*entree_length = 0;
		*entree_value = 0;
	}
	else {
		strcpy(entree_text, GET_STRING(entree, att_literal_string));
		*entree_length = (USHORT) strlen(entree_text);
		*entree_value = GET_VALUE(entree, att_entree_value);
	}

}


int pyxis__fetch(STATUS *status, SLONG **db_handle, SLONG *tra_handle,
                  MAP *map_handle, UCHAR *output)
{
/**************************************
 *
 *	p y x i s _ $ f e t c h
 *
 **************************************
 *
 * Functional description
 *	Copy data from a form (or subform).
 *
 **************************************/
	MAP map;
	DSC desc;
	OBJ form, field;
	USHORT source;
	SSHORT n;
	map::map_repeat *tail, *end;

	map = *map_handle;

/* Find parent */

	if (form = map->map_parent) {
		desc = map->map_rpt[map->map_item_index - 1].map_desc;
		desc.dsc_address = output + (SLONG) desc.dsc_address;
		n = MOVP_get_long(&desc, 0);
		if (!(form = PYXIS_find_index(map->map_parent, n))) {
			put_value(map, map->map_item_index - 1, output, 0);
			return 0;
		}
	}
	else
		form = map->map_form;

	end = map->map_rpt + map->map_count;

/* Handle general output */

	for (tail = map->map_rpt; tail < end; tail++)
		if (tail->map_field_flags & MAP_options) {
			if (!(field = find_field(map, form, tail)))
				continue;
			desc = tail->map_desc;
			desc.dsc_address = output + (SLONG) desc.dsc_address;
			if (desc.dsc_dtype == dtype_blob)
				source = get_blob(field, &desc, db_handle, (SLONG**) tra_handle);
			else
				source = get_field_value(map->map_window, form, field, &desc);
			put_value(map, tail->map_option, output, source);
		}

	return SUCCESS;
}


int pyxis__get_entree(MENU *menu_handle, USHORT *entree_length,
           TEXT *entree_text, SLONG *entree_value, USHORT *entree_end)
{
/**************************************
 *
 *	p y x i s _ $ g e t _ e n t r e e
 *
 **************************************
 *
 * Functional description
 *	Get the next entree in a dynamic menu
 *
 **************************************/
	MENU menu;
	OBJ entree_object;

	if (!(menu = *menu_handle)) {
		*entree_end = TRUE;
		return 0;
	}

	menu->menu_current_entree =
		PYXIS_find_object(menu->menu_object, menu->menu_current_entree,
						  att_entree, TRUE);

	if (!menu->menu_current_entree) {
		*entree_value = 0;
		*entree_length = 0;
		*entree_end = TRUE;
	}
	else {
		entree_object = (OBJ) menu->menu_current_entree->att_value;
		strcpy(entree_text, GET_STRING(entree_object, att_literal_string));
		*entree_length = (USHORT) strlen(entree_text);
		*entree_value = GET_VALUE(entree_object, att_entree_value);
		*entree_end = FALSE;
	}

}


int pyxis__initialize_menu(MENU *menu_handle)
{
/**************************************
 *
 *	p y x i s _ $ i n i t i a l i z e _ m e n u
 *
 **************************************
 *
 * Functional description
 *	Create or reinitialize a menu object
 *
 **************************************/
	MENU menu;

	if (!PYXIS_permanent_pool)
		PYXIS_init();

	if (menu = *menu_handle) {
		PYXIS_delete_object(menu->menu_object);
		menu->menu_flags = 0;
		menu->menu_current_entree = (ATT) NULL;
	}
	else
		menu = (MENU) ALLOCP(type_menu);

	menu->menu_object = PYXIS_create_object(0, (enum att_n)0);
	*menu_handle = menu;

}


int pyxis__insert(STATUS *status, SLONG **db_handle, SLONG *tra_handle,
                  MAP *map_handle, UCHAR *input)
{
/**************************************
 *
 *	p y x i s _ $ i n s e r t
 *
 **************************************
 *
 * Functional description
 *	Copy data to a form (or sub-form).
 *
 **************************************/
	MAP map;
	DSC desc;
	OBJ form, field;
	ATT attribute;
	SSHORT n, max_index;
	map::map_repeat *tail, *end;

	map = *map_handle;

/* Find parent */

	if (form = map->map_parent) {
		for (attribute = NULL, max_index = 0;
			 attribute = PYXIS_find_object(form, attribute, att_field, TRUE);) {
			n = GET_VALUE(attribute->att_value, att_index);
			max_index = MAX(n, max_index);
		}
		attribute =
			PYXIS_replicate_prototype(form, map->map_prototype,
									  max_index + 1);
		form = (OBJ) attribute->att_value;
	}
	else
		form = map->map_form;

	end = map->map_rpt + map->map_count;
	PYXIS_set_field_options(form, "all", PYXIS_updatable | PYXIS_wakeup,
							PYXIS_clear);

/* If there is an input record, initialize form */

	for (tail = map->map_rpt; tail < end; tail++)
		if (tail->map_field_flags & MAP_options) {
			desc = map->map_rpt[tail->map_option].map_desc;
			desc.dsc_address = input + (SLONG) desc.dsc_address;
			n = MOVP_get_long(&desc, 0);
			if (tail->map_field && tail->map_name) {
				propogate_options(tail, n);
/*             continue; */
			}
			if (!(field = find_field(map, form, tail)))
				continue;
			if (n & PYXIS_OPT_DISPLAY) {
				desc = tail->map_desc;
				desc.dsc_address = input + (SLONG) desc.dsc_address;
				if (desc.dsc_dtype == dtype_blob)
					put_blob(field, &desc, db_handle, (SLONG**)tra_handle);
				else
					PYXIS_put_desc(field, &desc);
				PYXIS_set_field_options(field, 0, PYXIS_update_present,
										PYXIS_clear);
			}
			if (n & PYXIS_OPT_POSITION) {
				while (map->map_context)
					LLS_POP(&map->map_context);
				for (attribute = NULL;
					 attribute =
					 PYXIS_find_object(form, attribute, att_field, TRUE);)
					if ((OBJ) attribute->att_value == field) {
						LLS_PUSH((BLK)attribute, &map->map_context);
						break;
					}
			}
			if (n & PYXIS_OPT_UPDATE)
				PYXIS_set_field_options(field, 0, PYXIS_updatable,
										PYXIS_updatable);
			if (n & PYXIS_OPT_WAKEUP)
				PYXIS_set_field_options(field, 0, PYXIS_wakeup, PYXIS_wakeup);
		}
}


int pyxis__load_form(STATUS *status, SLONG *db, SLONG *trans, SLONG *form,
                      SSHORT *length, SCHAR *name)
{
/**************************************
 *
 *	p y x i s _ $ l o a d _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Load a form given an attach database 
 *	and outstanding transaction.
 *
 **************************************/

	return (int) PYXIS_load_form((int*)status, (SLONG**)db, (SLONG**)trans, (OBJ*)form, (USHORT*) length, name);
}


OBJ pyxis__menu(WIN *window_handle, MENU *menu_handle, USHORT *length,
                        TEXT *source)
{
/**************************************
 *
 *	p y x i s _ $ m e n u
 *
 **************************************
 *
 * Functional description
 *	Build then drive a menu.
 *
 **************************************/
	WIN window;
	MENU menu;
	OBJ object, entree;

	if (!(window = *window_handle))
		*window_handle = window = PYXIS_create_window(80, 24);

	if (!(menu = *menu_handle))
		menu = *menu_handle = par_menu(window, length, source);

	object = menu->menu_object;

	if (object->obj_flags & OBJ_menu_horizontal)
		PYXIS_push_tag(window, object);
	else
		PYXIS_push_form(window, object,
						(menu->menu_flags & MENU_transparent) ? FALSE : TRUE);

	entree = PYXIS_menu(window, object);
	PYXIS_pop_form(window);

	if (!entree)
		return 0;

	return GET_OBJECT(entree, att_entree_value);
}


int pyxis__menu_d(WIN *window_handle, MENU *menu_handle, USHORT *length,
                  TEXT *source, OBJ *object)
{
/**************************************
 *
 *	p y x i s _ $ m e n u _ d
 *
 **************************************
 *
 * Functional description
 *	Call pyxis__menu for COBOL which is a 
 *	limited language and in some implementations
 *	lacks the concept of 'function return value'
 *
 **************************************/
	*object = pyxis__menu(window_handle, menu_handle, length, source);
}

extern "C"
int pyxis__pop_window(WIN *window_handle)
{
/**************************************
 *
 *	p y x i s _ $ p o p _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Pop a form off the window stack.
 *
 **************************************/
	WIN window;

	if (window = *window_handle)
		PYXIS_pop_form(window);
}


int pyxis__put_entree(MENU *menu_handle, USHORT *entree_length,
                      TEXT *entree_text, SLONG *entree_value)
{
/**************************************
 *
 *	p y x i s _ $ p u t _ e n t r e e
 *
 **************************************
 *
 * Functional description
 *	Add an entree to a dynamic menu
 *
 **************************************/
	MENU menu;
	OBJ object;
	USHORT i;
	TEXT *fp, *tp, entree_buffer[81];

	if (!(menu = *menu_handle)) {
		pyxis__initialize_menu(menu_handle);
		menu = *menu_handle;
	}

	object = menu->menu_object;

	for (i = *entree_length, fp = entree_text, tp = entree_buffer; i > 0;
		 i--, fp++, tp++)
		*tp = *fp;
	*tp = '\0';

	PYXIS_create_entree(object, entree_buffer, attype_numeric, (BLK) *entree_value);

}


int pyxis__reset_form(STATUS *status, MAP *map_handle)
{
/**************************************
 *
 *	p y x i s _ $ r e s e t _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Reset a compiled form to a known state.
 *
 **************************************/
	MAP map;
	ATT attribute;

	map = *map_handle;
	PYXIS_scroll_reset(map->map_form);
	map->map_wakeup_char = 0;

	PYXIS_set_field_options(map->map_form, "all", PYXIS_reset,
							PYXIS_fill_displayed);

	for (attribute = NULL;
		 attribute =
		 PYXIS_find_object(map->map_form, attribute, att_field, TRUE);)
		PYXIS_delete_named_attribute(attribute->att_value, att_current_index);

	while (map->map_context)
		LLS_POP(&map->map_context);
}


int pyxis__suspend_window(SLONG *window_handle)
{
/**************************************
 *
 *	p y x i s _ $ s u s p e n d _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Temporarily suspend a window.
 *
 **************************************/

	if (*window_handle)
		PYXIS_disable_window((WIN) *window_handle);
}


static int error(STATUS * status, TEXT * text, ...)
{
/**************************************
 *
 *	e r r o r
 *
 **************************************
 *
 * Functional description
 *	Produce civilized error (eventually).
 *
 **************************************/
	TEXT buffer[256];
	va_list ptr;

	VA_START(ptr, text);
	vsprintf(buffer, text, ptr);
	gds__put_error(buffer);
}


static OBJ find_field(MAP map, OBJ form, map::map_repeat *tail)
{
/**************************************
 *
 *	f i n d _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Based on map information, locate a target field.
 *
 **************************************/
	USHORT index;
	OBJ field;

/* If this is a simple field, things were easy */

	if ((field = tail->map_field) && !tail->map_name)
		return field;

/* If this is a sub-map, look up the field */

	if (!field)
		return PYXIS_find_field(form, tail->map_name);

/* This is a sub-item of a repeating group */

	if (!(index = GET_VALUE(field, att_current_index)))
		return NULL;

	if (!(field = PYXIS_find_index(field, index)))
		return NULL;

	return PYXIS_find_field(field, tail->map_name);
}


static int get_blob(OBJ form, DSC *desc, SLONG **db_handle, SLONG **tra_handle)
{
/**************************************
 *
 *	g e t _ b l o b
 *
 **************************************
 *
 * Functional description
 *	If blob has been changed, make a new one.
 *
 **************************************/
	USHORT length, n, source;
	TEXT *data, *p, buffer[1024];
	ATT attribute;
	OBJ field;
	SLONG *blob_handle, *blob_id;
	STATUS status_vector[20];

	source = PYXIS_OPT_NULL;

	if (!*db_handle || !*tra_handle)
		return source;

/* Determine whether there is any old data */

	for (attribute = NULL;
		 attribute = PYXIS_find_object(form, attribute, att_field, TRUE);) {
		field = attribute->att_value;
		if (GET_VALUE(field, att_data)) {
			source = PYXIS_OPT_INITIAL;
			break;
		}
	}

	if (!attribute) {
		blob_id = (SLONG *) desc->dsc_address;
		blob_id[0] = blob_id[1] = 0;
	}

/* Determine whether there is any new data */

	for (attribute = NULL;
		 attribute = PYXIS_find_object(form, attribute, att_field, TRUE);) {
		field = attribute->att_value;
		if (GET_VALUE(field, att_update_data)) {
			source = PYXIS_OPT_USER_DATA;
			break;
		}
	}

	if (!attribute)
		return source;

/* There is new data -- update blob */

	blob_handle = NULL;

	if (gds__create_blob(status_vector,
						 (void**)GDS_VAL(db_handle),
						 (void**)GDS_VAL(tra_handle),
						 (void**)GDS_REF(blob_handle), (GDS__QUAD*)GDS_VAL(desc->dsc_address))) {
		gds__print_status(status_vector);
		return PYXIS_OPT_NULL;
	}

	for (n = 1;; n++) {
		if (!(field = PYXIS_find_index(form, n)))
			break;
		if (!(data = GET_STRING(field, att_update_data)) &&
			!(data = GET_STRING(field, att_data)))
			data = "";
		for (p = buffer; *data;)
			*p++ = *data++;
		*p++ = '\n';
		*p = 0;
		length = p - buffer;
		if (gds__put_segment(status_vector,
							 (void**)GDS_REF(blob_handle), length, buffer)) break;
	}

	gds__close_blob(status_vector, (void**)GDS_REF(blob_handle));

	return source;
}


static int get_field_value(WIN window, OBJ form, OBJ field, DSC *desc)
{
/**************************************
 *
 *	g e t _ f i e l d _ v a l u e
 *
 **************************************
 *
 * Functional description
 *	Pick up a value from a field, move it to a given
 *	descriptor.  Return the source of the value.
 *
 **************************************/
	DSC desc2;
	OBJ error_object;
	TEXT *error_string, *field_name, buffer[80];
	USHORT source, c;

	desc2.dsc_dtype = dtype_cstring;
	desc2.dsc_length = 256;
	desc2.dsc_scale = 0;

	PYXIS_push_handler((int (*)()) unwind);

	if (desc2.dsc_address = (UCHAR *) GET_STRING(field, att_update_data))
		source = PYXIS_OPT_USER_DATA;
	else if (desc2.dsc_address = (UCHAR *) GET_STRING(field, att_data))
		source = PYXIS_OPT_INITIAL;
	else if (desc2.dsc_address =
			 (UCHAR *) GET_STRING(field, att_default_data)) source =
			PYXIS_OPT_DEFAULT;
	else if (desc2.dsc_address = (UCHAR *) GET_STRING(field, att_enumeration))
		source = PYXIS_OPT_DEFAULT;
	else {
/*    desc2.dsc_address = " ";  */
		source = PYXIS_OPT_NULL;
		return source;
	}

/* if (source == PYXIS_OPT_INITIAL)
    return source;
*/

	error_string = (TEXT *) setjmp(unwind_env);

	if (error_string) {
		PYXIS_set_field_options(form, "all", PYXIS_updatable | PYXIS_wakeup,
								PYXIS_clear);
		PYXIS_set_field_options(field, 0, PYXIS_updatable, PYXIS_updatable);
		PYXIS_push_form(window, form, 0);
		if (!(field_name = GET_STRING(field, att_field_name)))
			field_name = "(unnamed)";
		sprintf(buffer, "*** %s for field %s ***", error_string, field_name);
		error_object = PYXIS_create_object(buffer, att_literal_string);
		PYXIS_push_tag(window, error_object);
		PYXIS_get_data(window, field, att_update_data, form, &c);
		PYXIS_pop_form(window);
		PYXIS_pop_form(window);
		if (desc2.dsc_address = (UCHAR *) GET_STRING(field, att_update_data))
			source = PYXIS_OPT_USER_DATA;
		else if (desc2.dsc_address = (UCHAR *) GET_STRING(field, att_data))
			source = PYXIS_OPT_INITIAL;
		else if (desc2.dsc_address =
				 (UCHAR *) GET_STRING(field, att_default_data)) source =
				PYXIS_OPT_DEFAULT;
		else if (desc2.dsc_address =
				 (UCHAR *) GET_STRING(field, att_enumeration)) source =
				PYXIS_OPT_DEFAULT;
		else {
			source = PYXIS_OPT_NULL;
			PYXIS_pop_handler(0);
			return source;
		}
	}

	MOVP_move(&desc2, desc);
	PYXIS_pop_handler(0);

	return source;
}


static MENU par_menu(WIN window, USHORT *length, SCHAR *source)
{
/**************************************
 *
 *	p a r _ m e n u
 *
 **************************************
 *
 * Functional description
 *	Parse a menu block.
 *
 **************************************/
	USHORT value, n, x;
	MENU menu;
	OBJ object, entree;
	USHORT flags, horizontal;
	SCHAR *p, c;
	TEXT *q, label[256], string[256];
	struct menu_repeat *tail;

	p = source;
	c = BLR_BYTE;
	object = PYXIS_create_object(0, (enum att_n) 0);
	label[0] = 0;
	flags = horizontal = 0;

	if (c != PYXIS_MENU_VERSION1) {
		error(0, "unsupported map version %d", c);
		return NULL;
	}

	while ((c = BLR_BYTE) != PYXIS_MENU_END)
		switch (c) {
		case PYXIS_MENU_LABEL:
			q = label;
			if (n = BLR_BYTE)
				do
					*q++ = BLR_BYTE;
				while (--n);
			*q = 0;
			break;

		case PYXIS_MENU_ENTREE:
			value = BLR_BYTE;
			q = string;
			if (n = BLR_BYTE)
				do
					*q++ = BLR_BYTE;
				while (--n);
			*q = 0;
			PYXIS_create_entree(object, string, attype_numeric, (BLK) value);
			break;

		case PYXIS_MENU_OPAQUE:
			break;

		case PYXIS_MENU_TRANSPARENT:
			flags |= MENU_transparent;
			break;

		case PYXIS_MENU_HORIZONTAL:
			horizontal = TRUE;
			break;

		case PYXIS_MENU_VERTICAL:
			break;

		default:
			error(0, "invalid menu");
			return NULL;
		}

	menu = (MENU) ALLOCP(type_menu);
	menu->menu_flags = flags;
	menu->menu_object = object;
	q = NULL;

	if (label[0])
		q = label;

	if (horizontal) {
		object->obj_flags |= OBJ_menu_horizontal;
		PYXIS_format_menu(object, q, TRUE);
	}
	else {
		PYXIS_format_menu(object, q, FALSE);
		x = (window->win_width - GET_VALUE(object, att_width)) / 2;
		PYXIS_position(object, x, 1);
	}

	return menu;
}


static MAP par_message(STATUS *status, TEXT **ptr)
{
/**************************************
 *
 *	p a r _ m e s s a g e
 *
 **************************************
 *
 * Functional description
 *	Parse a message declaration, including operator byte.
 *
 **************************************/
	USHORT n, dtype, alignment, offset;
	TEXT *p;
	DSC *desc;
	MAP map;
	map::map_repeat *tail, *end;

/* Get message number, register it in the compile scratch block, and
   allocate a node to represent the message */

	p = *ptr;
	BLR_BYTE;
	n = BLR_WORD;
	map = (MAP) ALLOCPV(type_map, n);
	map->map_count = n;
	offset = 0;

/* Get the number of parameters in the message and prepare to fill
   out the format block */

	for (tail = map->map_rpt, end = tail + n; tail < end; ++tail) {
		desc = &tail->map_desc;
		alignment = 4;
		switch (dtype = BLR_BYTE) {
		case blr_text:
			desc->dsc_dtype = dtype_text;
			desc->dsc_length = BLR_WORD;
			alignment = 0;
			break;

		case blr_cstring:
			desc->dsc_dtype = dtype_cstring;
			desc->dsc_length = BLR_WORD;
			alignment = 0;
			break;

		case blr_varying:
			desc->dsc_dtype = dtype_varying;
			desc->dsc_length = BLR_WORD + 2;
			alignment = 2;
			break;

		case blr_short:
			desc->dsc_dtype = dtype_short;
			desc->dsc_length = 2;
			desc->dsc_scale = (SSHORT) BLR_BYTE;
			alignment = 2;
			break;

		case blr_long:
			desc->dsc_dtype = dtype_long;
			desc->dsc_length = 4;
			desc->dsc_scale = (SSHORT) BLR_BYTE;
			break;

		case blr_quad:
			desc->dsc_dtype = dtype_quad;
			desc->dsc_length = 8;
			desc->dsc_scale = (SSHORT) BLR_BYTE;
			break;

		case blr_float:
			desc->dsc_dtype = dtype_real;
			desc->dsc_length = 4;
			break;

		case blr_timestamp:
			desc->dsc_dtype = dtype_timestamp;
			desc->dsc_length = 8;
			break;

		case blr_double:
#ifndef VMS
		case blr_d_float:
#endif
			desc->dsc_dtype = dtype_double;
			desc->dsc_length = 8;
			break;

#ifdef VMS
		case blr_d_float:
			desc->dsc_dtype = dtype_d_float;
			desc->dsc_length = 8;
			break;
#endif

		case blr_blob_id:
			desc->dsc_dtype = dtype_blob;
			desc->dsc_length = 8;
			desc->dsc_sub_type = BLR_WORD;
			break;

		case blr_sql_date:
		case blr_sql_time:
		default:
			error(status, "datatype %d not supported", dtype);
			return NULL;
		}

		if (alignment)
			offset = FB_ALIGN(offset, alignment);
		desc->dsc_address = (UCHAR *) offset;
		offset += desc->dsc_length;
	}

	*ptr = p;

	return map;
}


static int par_word(TEXT **ptr)
{
/**************************************
 *
 *	p a r _ w o r d
 *
 **************************************
 *
 * Functional description
 *	Pick up a BLR word.
 *
 **************************************/
	unsigned char low, high;

	/*
	 * Much magic here.  We are going to extract the
	 * byte at each location by casting the pointer
	 * to that location to an unsigned char.  We
	 * assign the result to unsigned chars, so that
	 * the sloppy return value (which is implicitly
	 * casting everything to int) won't sign extend.
	 */
	low = *((unsigned char *) (*ptr));
	(*ptr)++;
	high = *((unsigned char *) (*ptr));
	(*ptr)++;

	return high << 8 | low;
}


static int propogate_options(map::map_repeat *tail, SLONG options)
{
/**************************************
 *
 *	p r o p o g a t e _ o p t i o n s
 *
 **************************************
 *
 * Functional description
 *	Propogate options of a named field 
 *      to all elements of a repeating group.
 *
 **************************************/
	OBJ form, item, field;
	TEXT *name;
	ATT attribute;

	form = tail->map_field;
	name = tail->map_name;

	for (attribute = NULL;
		 attribute = PYXIS_find_object(form, attribute, att_any, 0);)
		if (attribute->att_name == att_prototype
			|| attribute->att_name == att_field) {
			item = attribute->att_value;
			if (field = PYXIS_find_field(item, name)) {
				if (options & PYXIS_OPT_UPDATE)
					PYXIS_set_field_options(field, 0, PYXIS_updatable,
											PYXIS_updatable);
				if (options & PYXIS_OPT_WAKEUP)
					PYXIS_set_field_options(field, 0, PYXIS_wakeup,
											PYXIS_wakeup);
			}
		}
}


static int put_blob(OBJ field, DSC *desc, SLONG **db_handle, SLONG **tra_handle)
{
/**************************************
 *
 *	p u t _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Copy blob to form.
 *
 **************************************/
	TEXT buffer[1024];
	USHORT length, index;
	OBJ prototype, segment;
	SLONG *blob_handle;
	STATUS status_vector[20];

	PYXIS_purge_segments(field, 1);
	prototype = GET_OBJECT(field, att_prototype);
	blob_handle = NULL;

	if (!*db_handle || !*tra_handle)
		return FALSE;

	if (gds__open_blob(status_vector,
					   (void**)GDS_VAL(db_handle),
					   (void**)GDS_VAL(tra_handle),
					   (void**)GDS_REF(blob_handle), (GDS__QUAD*) GDS_VAL(desc->dsc_address))) {
		gds__print_status(status_vector);
		return FALSE;
	}

	for (index = 1;; index++) {
		if (gds__get_segment(status_vector,
							 (void**)GDS_REF(blob_handle),
							 GDS_REF(length), sizeof(buffer), buffer))
			break;
		buffer[length] = 0;
		segment = PYXIS_clone(prototype);
		REPLACE_ATTRIBUTE(segment, att_index, attype_numeric, index);
		REPLACE_ATTRIBUTE(segment, att_display_y, attype_numeric, index - 1);
		PYXIS_set_display_attribute(segment,
									REPLACE_ATTRIBUTE(segment, att_data,
													  attype_string, buffer));
		PUT_ATTRIBUTE(field, att_field, attype_object, segment);
	}

	gds__close_blob(status_vector, (void**)GDS_REF(blob_handle));

	return TRUE;
}


static int put_value(MAP map, USHORT slot, UCHAR *record, SLONG value)
{
/**************************************
 *
 *	p u t _ v a l u e
 *
 **************************************
 *
 * Functional description
 *	Copy a value to a field in a record.
 *
 **************************************/
	DSC desc, desc2;

	desc = map->map_rpt[slot].map_desc;
	desc.dsc_address = record + (SLONG) desc.dsc_address;

	desc2.dsc_dtype = dtype_long;
	desc2.dsc_length = sizeof(SLONG);
	desc2.dsc_scale = 0;
	desc2.dsc_address = (UCHAR *) & value;

	MOVP_move(&desc2, &desc);
}


static int unwind(TEXT *error_string)
{
/**************************************
 *
 *	u n w i n d
 *
 **************************************
 *
 * Functional description
 *	Unwind the current call to a given environment.
 *
 **************************************/

	strcpy(error_buffer, error_string);
	longjmp(unwind_env, (int) error_buffer);
}
