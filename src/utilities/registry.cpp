/*
 *	PROGRAM:	Windows NT registry installation program
 *	MODULE:		registry.c
 *	DESCRIPTION:	Registry update routines
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

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include "../jrd/isc_proto.h"
#include <stdlib.h>
#include <windows.h>
#include "../jrd/common.h"
#include "../jrd/license.h"
#include "../utilities/install_nt.h"
#include "../utilities/regis_proto.h"

static USHORT remove_subkeys(HKEY, USHORT, USHORT(*)());


USHORT REGISTRY_install(HKEY hkey_node,
						TEXT * directory, USHORT(*err_handler) ())
{
/**************************************
 *
 *	R E G I S T R Y _ i n s t a l l
 *
 **************************************
 *
 * Functional description
 *	Install InterBase in the registry.
 *
 **************************************/
	HKEY hkey_kit;
	DWORD disp;
	TEXT path_name[260], *p;
	USHORT len;
	SLONG status;

	if ((status = RegCreateKeyEx(hkey_node,
                                 WIN32_REG_KEY_PATH_CURRENT_VERSION,
								 0,
								 "",
								 REG_OPTION_NON_VOLATILE,
								 KEY_WRITE,
								 NULL, &hkey_kit, &disp)) != ERROR_SUCCESS) {
		return (*err_handler) (status, "RegCreateKeyEx", NULL);
	}

	len = GetFullPathName(directory, sizeof(path_name), path_name, &p);
	if (len && path_name[len - 1] != '/' && path_name[len - 1] != '\\') {
		path_name[len++] = '\\';
		path_name[len] = 0;
	}

	if ((status = RegSetValueEx(hkey_kit, "RootDirectory", 0,
								REG_SZ, path_name,
								(DWORD) (len + 1))) != ERROR_SUCCESS
		|| (status =
			RegSetValueEx(hkey_kit, "Version", 0, REG_SZ, GDS_VERSION,
						  (DWORD) sizeof(GDS_VERSION))) != ERROR_SUCCESS) {
		(*err_handler) (status, "RegSetValueEx", hkey_kit);
		if (disp == REG_CREATED_NEW_KEY)
			REGISTRY_remove(hkey_node, TRUE, err_handler);
		return FAILURE;
	}

	RegCloseKey(hkey_kit);

	return SUCCESS;
}


USHORT REGISTRY_remove(HKEY hkey_node,
					   USHORT silent_flag, USHORT(*err_handler) ())
{
/**************************************
 *
 *	R E G I S T R Y _ r e m o v e
 *
 **************************************
 *
 * Functional description
 *	Remove InterBase from the registry.
 *
 **************************************/
	HKEY hkey_kit;
	USHORT ret;
	SLONG status;

	if ((status = RegOpenKeyEx(hkey_node,
                               WIN32_REG_KEY_PATH,
							   0,
							   KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE |
							   KEY_WRITE, &hkey_kit)) != ERROR_SUCCESS) {
		if (silent_flag)
			return FAILURE;
		return (*err_handler) (status, "RegOpenKeyEx", NULL);
	}

	ret = remove_subkeys(hkey_kit, silent_flag, err_handler);

	RegCloseKey(hkey_kit);

	if (ret == FAILURE)
		return FAILURE;

	if (status = RegDeleteKey(hkey_node, WIN32_REG_KEY_PATH)) {
		if (silent_flag)
			return FAILURE;
		return (*err_handler) (status, "RegDeleteKey", NULL);
	}

	return SUCCESS;
}


static USHORT remove_subkeys(
							 HKEY hkey,
							 USHORT silent_flag, USHORT(*err_handler) ())
{
/**************************************
 *
 *	r e m o v e _ s u b k e y s
 *
 **************************************
 *
 * Functional description
 *	Remove all sub-keys of an InterBase key from the registry.
 *
 **************************************/
	TEXT *sub_key, buffer[260], *p;
	DWORD n_sub_keys, max_sub_key, sub_key_len, buf_len, i, status;
	FILETIME last_write_time;
	HKEY hkey_sub;
	USHORT ret;

	buf_len = sizeof(buffer);
	status = RegQueryInfoKey(hkey,
							 buffer, &buf_len,
							 NULL,
							 &n_sub_keys,
							 &max_sub_key,
							 &i, &i, &i, &i, &i, &last_write_time);
	if (status != ERROR_SUCCESS && status != ERROR_MORE_DATA) {
		if (silent_flag)
			return FAILURE;
		return (*err_handler) (status, "RegQueryInfoKey", NULL);
	}

	sub_key = (++max_sub_key > sizeof(buffer)) ?
		(TEXT *) malloc((SLONG) max_sub_key) : buffer;

	for (i = 0, p = NULL; i < n_sub_keys; i++) {
		sub_key_len = max_sub_key;
		if ((status = RegEnumKeyEx(hkey, i, sub_key, &sub_key_len,
								   NULL, NULL, NULL,
								   &last_write_time)) != ERROR_SUCCESS) {
			p = "RegEnumKeyEx";
			break;
		}
		if ((status = RegOpenKeyEx(hkey, sub_key,
								   0,
								   KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE |
								   KEY_WRITE, &hkey_sub)) != ERROR_SUCCESS) {
			p = "RegOpenKeyEx";
			break;
		}
		ret = remove_subkeys(hkey_sub, silent_flag, err_handler);
		RegCloseKey(hkey_sub);
		if (ret == FAILURE)
			return FAILURE;
		if ((status = RegDeleteKey(hkey, sub_key)) != ERROR_SUCCESS) {
			p = "RegDeleteKey";
			break;
		}
	}

	if (buffer != sub_key)
		free(sub_key);

	if (p) {
		if (silent_flag)
			return FAILURE;
		return (*err_handler) (status, p, NULL);
	}

	return SUCCESS;
}
