/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		altp.c
 *	DESCRIPTION:	Alternative entrypoints
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

#include "../jrd/common.h"
#include "../include/jrd/gds.h"
#include "../pyxis/pyxis.h"
#include "../pyxis/phase3.h"

STATUS isc_compile_map(STATUS * status_vector,
					   void **form_handle,
					   void **map_handle, SSHORT * length, SCHAR * map)
{
	return pyxis__compile_map(status_vector,
				  (p_obj**)form_handle, (MAP*) map_handle, (USHORT*) length, map);
}

STATUS isc_compile_menu(STATUS * status_vector,
						void **win_handle,
						void **menu_handle, SSHORT * length, SCHAR * menu)
{
	if (status_vector) {
		status_vector[0] = 1;
		status_vector[1] = 0;
	}
	return pyxis__compile_menu((WIN*) win_handle, (MENU*) menu_handle, (USHORT*) length, menu);
}

STATUS isc_compile_sub_map(STATUS * status_vector,
						   void **map_handle,
						   void **submap_handle, SSHORT * length, SCHAR * map)
{
	return pyxis__compile_sub_map(status_vector,
				  (MAP*) map_handle, (MAP*) submap_handle, (USHORT*) length, map);
}

STATUS isc_create_window(STATUS * status_vector,
						 void **win_handle,
						 SSHORT * filename_length,
						 SCHAR * filename, SSHORT * width, SSHORT * height)
{
	if (status_vector) {
		status_vector[0] = 1;
		status_vector[1] = 0;
	}
	return pyxis__create_window((WIN*) win_handle,
			(USHORT*) filename_length, (TEXT*) filename,
                        (USHORT*) width, (USHORT*) height);
}

STATUS isc_delete_window(STATUS * status_vector, void **win_handle)
{
	if (status_vector) {
		status_vector[0] = 1;
		status_vector[1] = 0;
	}
	return pyxis__delete_window((SLONG*) win_handle);
}

STATUS isc_drive_form(STATUS * status_vector,
					  void **db_handle,
					  void **tra_handle,
					  void **win_handle,
					  void **map_handle, UCHAR * input, UCHAR * output)
{
	return pyxis__drive_form(status_vector,
					 (SLONG**) db_handle,
					 (SLONG*) tra_handle,
					 (WIN*) win_handle, (MAP*)map_handle,
 					input, output);
}

STATUS isc_drive_menu(STATUS * status_vector,
					  void **win_handle,
					  void **menu_handle,
					  SSHORT * blr_length,
					  SCHAR * blr,
					  SSHORT * title_length,
					  SCHAR * title,
					  SSHORT * terminator,
					  SSHORT * entree_length,
					  SCHAR * entree, SLONG * entree_value)
{
	if (status_vector) {
		status_vector[0] = 1;
		status_vector[1] = 0;
	}
	return pyxis__drive_menu((WIN*) win_handle,
					 (MENU*) menu_handle,
					 (USHORT*) blr_length,
							 blr,
					 (USHORT*) title_length,
					 (TEXT*) title,
					 (USHORT*) terminator,
					 (USHORT*) entree_length, (TEXT*)entree,
					 entree_value);
}

STATUS isc_form_delete(STATUS * status_vector, void **map_handle)
{
	if (status_vector) {
		status_vector[0] = 1;
		status_vector[1] = 0;
	}
	return pyxis__delete((p_obj**) map_handle);
}

STATUS isc_form_fetch(STATUS * status_vector,
					  void **db_handle,
					  void **tra_handle, void **map_handle, UCHAR * output)
{
	return pyxis__fetch(status_vector,
						(SLONG**) db_handle,
						(SLONG*) tra_handle,
						(MAP*) map_handle, output);
}

STATUS isc_form_insert(STATUS * status_vector,
					   void **db_handle,
					   void **tra_handle, void **map_handle, UCHAR * input)
{
	return pyxis__insert(status_vector,
			 (SLONG**) db_handle, (SLONG*) tra_handle,
			 (MAP*) map_handle, input);
}

STATUS isc_get_entree(STATUS * status_vector,
					  void **menu_handle,
					  SSHORT * entree_length,
					  SCHAR * entree,
					  SLONG * entree_value, SSHORT * entree_end)
{
	if (status_vector) {
		status_vector[0] = 1;
		status_vector[1] = 0;
	}
	return pyxis__get_entree((MENU*) menu_handle,
				 (USHORT*) entree_length, (TEXT*) entree,
				 entree_value, (USHORT*) entree_end);
}

STATUS isc_initialize_menu(STATUS * status_vector, void **menu_handle)
{
	if (status_vector) {
		status_vector[0] = 1;
		status_vector[1] = 0;
	}
	return pyxis__initialize_menu((MENU*) menu_handle);
}

STATUS isc_load_form(STATUS * status_vector,
					 void **db_handle,
					 void **tra_handle,
					 void **form_handle, SSHORT * length, SCHAR * name)
{
	return pyxis__load_form(status_vector,
				(SLONG*) db_handle, (SLONG*) tra_handle,
				(SLONG*) form_handle, length, name);
}

STATUS isc_menu(STATUS * status_vector,
				void **win_handle,
				void **menu_handle, SSHORT * length, SCHAR * source)
{
	if (status_vector) {
		status_vector[0] = 1;
		status_vector[1] = 0;
	}
	return (long int) pyxis__menu((WIN*) win_handle, (MENU*) menu_handle,
				(USHORT*) length, (TEXT*) source);
}

STATUS isc_pop_window(STATUS * status_vector, void **win_handle)
{
	if (status_vector) {
		status_vector[0] = 1;
		status_vector[1] = 0;
	}
	return pyxis__pop_window((WIN*) win_handle);
}

STATUS isc_put_entree(STATUS * status_vector,
					  void **menu_handle,
					  SSHORT * entree_length,
					  SCHAR * entree, SLONG * entree_value)
{
	if (status_vector) {
		status_vector[0] = 1;
		status_vector[1] = 0;
	}
	return pyxis__put_entree((MENU*) menu_handle,
				 (USHORT*) entree_length, (TEXT*) entree,
				 entree_value);
}

STATUS isc_reset_form(STATUS * status_vector, void **map_handle)
{
	return pyxis__reset_form(status_vector, (MAP*) map_handle);
}

STATUS isc_suspend_window(STATUS * status_vector, void **win_handle)
{
	if (status_vector) {
		status_vector[0] = 1;
		status_vector[1] = 0;
	}
	return pyxis__suspend_window((SLONG*) win_handle);
}
