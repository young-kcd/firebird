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
 *
 * 01-Feb-2002 Paul Reeves: Removed hard-coded registry path
 *
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
#include "../utilities/registry.h"

static USHORT remove_subkeys(HKEY, USHORT, USHORT(*)(SLONG, TEXT *, HKEY));

USHORT REGISTRY_install(HKEY hkey_node,
						TEXT * directory, USHORT(*err_handler)(SLONG, TEXT *, HKEY))
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
	TEXT path_name[MAXPATHLEN], *p;
	USHORT len;
	SLONG status;

	if ((status = RegCreateKeyEx(hkey_node,
                                 REG_KEY_ROOT_CUR_VER,
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
								REG_SZ, reinterpret_cast<UCHAR*>(path_name),
								(DWORD) (len + 1))) != ERROR_SUCCESS
		|| (status =
			RegSetValueEx(hkey_kit, "Version", 0, REG_SZ,
						  reinterpret_cast<UCHAR*>(const_cast<char*>(GDS_VERSION)),
						  (DWORD) sizeof(GDS_VERSION))) != ERROR_SUCCESS) {
		(*err_handler) (status, "RegSetValueEx", hkey_kit);
		if (disp == REG_CREATED_NEW_KEY)
			REGISTRY_remove(hkey_node, TRUE, err_handler);
		return FB_FAILURE;
	}

	/* We might as well add ServerDirectory here */
/*
	strcat(path_name, "\\bin");
	len = GetFullPathName(path_name, sizeof(path_name), path_name, &p);
	if (len && path_name[len - 1] != '/' && path_name[len - 1] != '\\') {
    	path_name[len++] = '\\';
    	path_name[len] = 0;
    }

	if ((status = RegSetValueEx(hkey_kit, "ServerDirectory", 0,
								REG_SZ, reinterpret_cast<UCHAR*>(path_name),
								(DWORD)(len + 1))) != ERROR_SUCCESS) {
	    (*err_handler) (status, "RegSetValueEx", hkey_kit);
    	if (disp == REG_CREATED_NEW_KEY)
			REGISTRY_remove(hkey_node, TRUE, err_handler);
    	return FB_FAILURE;
    }

	if ((status = RegSetValueEx(hkey_kit, "GuardianOptions", 0,
								REG_SZ, reinterpret_cast<UCHAR*>("1"),
								1)) != ERROR_SUCCESS) {
	    (*err_handler) (status, "RegSetValueEx", hkey_kit);
    	if (disp == REG_CREATED_NEW_KEY)
			REGISTRY_remove(hkey_node, TRUE, err_handler);
    	return FB_FAILURE;
    }
*/
	RegCloseKey(hkey_kit);

	return FB_SUCCESS;
}


USHORT REGISTRY_remove(HKEY hkey_node,
					   USHORT silent_flag, USHORT(*err_handler)(SLONG, TEXT *, HKEY))
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
                               REG_KEY_ROOT,
							   0,
							   KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE |
							   KEY_WRITE, &hkey_kit)) != ERROR_SUCCESS) {
		if (silent_flag)
			return FB_FAILURE;
		return (*err_handler) (status, "RegOpenKeyEx", NULL);
	}

	ret = remove_subkeys(hkey_kit, silent_flag, err_handler);

	RegCloseKey(hkey_kit);

	if (ret == FB_FAILURE)
		return FB_FAILURE;

	if (status = RegDeleteKey(hkey_node, REG_KEY_ROOT)) {
		if (silent_flag)
			return FB_FAILURE;
		return (*err_handler) (status, "RegDeleteKey", NULL);
	}

	return FB_SUCCESS;
}


static USHORT remove_subkeys(
							 HKEY hkey,
							 USHORT silent_flag, USHORT(*err_handler)(SLONG, TEXT *, HKEY))
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
	TEXT *sub_key, buffer[MAXPATHLEN], *p;
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
			return FB_FAILURE;
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
		if (ret == FB_FAILURE)
			return FB_FAILURE;
		if ((status = RegDeleteKey(hkey, sub_key)) != ERROR_SUCCESS) {
			p = "RegDeleteKey";
			break;
		}
	}

	if (buffer != sub_key)
		free(sub_key);

	if (p) {
		if (silent_flag)
			return FB_FAILURE;
		return (*err_handler) (status, p, NULL);
	}

	return FB_SUCCESS;
}
