/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		sun_ftn_pyxis.c
 *	DESCRIPTION:	"Wrapper" module which compensates for the
 *			Sun Fortran compiler appending an underscore
 *			to external entrypoints.  (Each function in
 *			this module is an InterBase entrypoint with
 *			an underscore appended to it which calls the
 *			real InterBase function).
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

#include <varargs.h>
#include "../jrd/common.h"
#include "../include/jrd/gds.h"
#include "../pyxis/everything.h"
#undef GDS_VAL
#define GDS_VAL(val)	(*val)

typedef STATUS(*PTR) ();

typedef struct handle {
	struct handle **user_handle;
} *HANDLE, *REQ, *DBB, *TRA, *BLB, *ATT, *OBJ, *BLK, *MAP, *WIN, *MENU;

#define PYXIS_COMPILE_MAP	pyxis__compile_map_
#define PYXIS_COMPILE_MENU	pyxis__compile_menu_
#define PYXIS_COMPILE_SUB_MAP	pyxis__compile_sub_map_
#define PYXIS_CREATE_WINDOW	pyxis__create_window_
#define PYXIS_DELETE		pyxis__delete_
#define PYXIS_DELETE_WINDOW	pyxis__delete_window_
#define PYXIS_DRIVE_FORM	pyxis__drive_form_
#define PYXIS_DRIVE_MENU	pyxis__drive_menu_
#define PYXIS_FETCH		pyxis__fetch_
#define PYXIS_GET_ENTREE	pyxis__get_entree_
#define PYXIS_INITIALIZE_MENU	pyxis__initialize_menu_
#define PYXIS_INSERT		pyxis__insert_
#define PYXIS_LOAD_FORM		pyxis__load_form_
#define PYXIS_MENU		pyxis__menu_
#define PYXIS_POP_WINDOW	pyxis__pop_window_
#define PYXIS_PUT_ENTREE	pyxis__put_entree_
#define PYXIS_RESET_FORM	pyxis__reset_form_
#define PYXIS_SUSPEND_WINDOW	pyxis__suspend_window_


int PYXIS_COMPILE_MAP(STATUS *status, OBJ *form_handle, MAP *map_handle, ULONG *length, TEXT *source)
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
	USHORT len;

	len = *length;

	return pyxis__compile_map(status, form_handle, map_handle, &len, source);
}


int PYXIS_COMPILE_MENU(WIN *window_handle, MENU *menu_handle, USHORT *length, TEXT *source)
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

	pyxis__compile_menu(window_handle, menu_handle, length, source);
}



int PYXIS_COMPILE_SUB_MAP(STATUS *status, MAP *parent_handle, MAP *map_handle, ULONG *length, TEXT *source)
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
	USHORT len;

	len = *length;
	return pyxis__compile_sub_map(status, parent_handle,
								  map_handle, &len, source);
}



int PYXIS_CREATE_WINDOW(WIN *window_handle, ULONG *file_name_length, TEXT *file_name, USHORT *width, USHORT *height)
{
/**************************************
 *
 *	p y x i s _ $ c r e a t e _ w i n d o w
 *
 **************************************
 *
 * Functional description
 *	Sun fortran callable version
 *      Create a new window.
 *
 **************************************/
	USHORT name_length;

	name_length = *file_name_length;
	pyxis__create_window(window_handle, &name_length, file_name, width,
						 height);
}


int PYXIS_DELETE(OBJ *object)
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
	pyxis__delete(object);
}


int PYXIS_DELETE_WINDOW(SLONG *window_handle)
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
	pyxis__delete_window(window_handle);
}


int PYXIS_DRIVE_FORM(STATUS *status, SLONG **db_handle, SLONG **tra_handle, WIN *window_handle, MAP *map_handle,
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
	pyxis__drive_form(status, db_handle, tra_handle, window_handle,
					  map_handle, input, output);
}


int PYXIS_DRIVE_MENU(WIN *window_handle, MENU *menu_handle, SLONG *blr_length,
				 TEXT *blr_source, USHORT *title_length, TEXT *title, USHORT *terminator,
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
	USHORT i;

	i = *blr_length;
	pyxis__drive_menu(window_handle, menu_handle, &i,
					  blr_source, title_length, title, terminator,
					  entree_length, entree_text, entree_value);
}

int
PYXIS_FETCH(STATUS *status, SLONG **db_handle, SLONG *tra_handle, MAP *map_handle, UCHAR *output)
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
	pyxis__fetch(status, db_handle, tra_handle, map_handle, output);
}


int PYXIS_GET_ENTREE(MENU *menu_handle, USHORT *entree_length,
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
	pyxis__get_entree(menu_handle, entree_length,
					  entree_text, entree_value, entree_end);
}


int PYXIS_INITIALIZE_MENU(MENU *menu_handle)
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
	pyxis__initialize_menu(menu_handle);
}


int PYXIS_INSERT(STATUS *status, SLONG **db_handle, SLONG *tra_handle, MAP *map_handle, UCHAR *input)
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
	pyxis__insert(status, db_handle, tra_handle, map_handle, input);
}


int PYXIS_LOAD_FORM(STATUS *status_vector, SLONG *dbb, SLONG *transaction,
				OBJ *form_handle, ULONG *form_name_length, TEXT *form_name)
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
	USHORT length;

	length = *form_name_length;
	pyxis__load_form(status_vector, dbb, transaction,
					 form_handle, &length, form_name);
}


OBJ PYXIS_MENU(WIN *window_handle, MENU *menu_handle, USHORT *length, TEXT *source)
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
	return (OBJ) pyxis__menu(window_handle, menu_handle, length, source);
}


int PYXIS_POP_WINDOW(WIN *window_handle)
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
	pyxis__pop_window(window_handle);
}


int PYXIS_PUT_ENTREE(MENU *menu_handle, USHORT *entree_length, TEXT *entree_text, SLONG *entree_value)
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
	pyxis__put_entree(menu_handle, entree_length, entree_text, entree_value);
}


int PYXIS_RESET_FORM(STATUS *status, MAP *map_handle)
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
	pyxis__reset_form(status, map_handle);
}


int PYXIS_SUSPEND_WINDOW(SLONG *window_handle)
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
	pyxis__suspend_window(window_handle);
}
