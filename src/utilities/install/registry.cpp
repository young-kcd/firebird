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
#include "../utilities/install/install_nt.h"
#include "../utilities/install/regis_proto.h"
#include "../utilities/install/registry.h"

static USHORT remove_subkeys(HKEY, bool, USHORT(*)(SLONG, TEXT *, HKEY));

USHORT REGISTRY_install(HKEY hkey_rootnode,
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
	HKEY hkey_instances;
	DWORD disp;
	TEXT path_name[MAXPATHLEN], *p;
	USHORT len;
	SLONG status;

	if ((status = RegCreateKeyEx(hkey_rootnode,
			REG_KEY_ROOT_INSTANCES,
			0,
			"",
			REG_OPTION_NON_VOLATILE,
			KEY_WRITE,
			NULL, &hkey_instances, &disp)) != ERROR_SUCCESS)
	{
		return (*err_handler) (status, "RegCreateKeyEx", NULL);
	}

	len = GetFullPathName(directory, sizeof(path_name), path_name, &p);
	if (len && path_name[len - 1] != '/' && path_name[len - 1] != '\\') {
		path_name[len++] = '\\';
		path_name[len] = 0;
	}

	if ((status = RegSetValueEx(hkey_instances, FB_DEFAULT_INSTANCE, 0,
			REG_SZ, reinterpret_cast<UCHAR*>(path_name),
			(DWORD) (len + 1))) != ERROR_SUCCESS)
	{
		(*err_handler) (status, "RegSetValueEx", hkey_instances);

		// Removes the "Instances" key if we just created it.
		// Else, keep it, because we don't want to trash other instances.
		if (disp == REG_CREATED_NEW_KEY)
			RegDeleteKey(hkey_rootnode, REG_KEY_ROOT_INSTANCES);

		return FB_FAILURE;
	}

	RegCloseKey(hkey_instances);
	return FB_SUCCESS;
}

USHORT REGISTRY_remove(HKEY hkey_rootnode,
					   bool silent_flag, USHORT(*err_handler)(SLONG, TEXT *, HKEY))
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
	HKEY hkey_instances;
	SLONG status;
	DWORD subkeys_count, values_count;

	if ((status = RegOpenKeyEx(hkey_rootnode,
			REG_KEY_ROOT_INSTANCES,
			0,
			KEY_READ | KEY_WRITE,
			&hkey_instances)) != ERROR_SUCCESS)
	{
		if (silent_flag)
			return FB_FAILURE;
		return (*err_handler) (status, "RegOpenKeyEx", NULL);
	}

	if ((status = RegDeleteValue(hkey_instances, FB_DEFAULT_INSTANCE))
			!= ERROR_SUCCESS)
	{
		RegCloseKey(hkey_instances);
		if (silent_flag)
			return FB_FAILURE;
		return (*err_handler) (status, "RegDeleteValue", NULL);
	}

	if (RegQueryInfoKey(hkey_instances, NULL, 0, 0,
			&subkeys_count, NULL, NULL,
			&values_count, 0, 0, NULL, NULL) == ERROR_SUCCESS &&
				subkeys_count == 0 && values_count == 0)
	{
		// Let's remove the instances key itself as we see it is now empty.
		RegCloseKey(hkey_instances);
		RegDeleteKey(hkey_rootnode, REG_KEY_ROOT_INSTANCES);
	}
	else
		RegCloseKey(hkey_instances);

	return FB_SUCCESS;
}

#ifdef THIS_CODE_IS_TEMPORARILY_NOT_USED_ANYMORE
// I keep it here for possible re-use after FB 1.5 release. OM, sept 30, 2003.
static USHORT remove_subkeys(
							 HKEY hkey,
							 bool silent_flag, USHORT(*err_handler)(SLONG, TEXT *, HKEY))
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
#endif
