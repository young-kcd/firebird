/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		guid.h
 *	DESCRIPTION:	Portable GUID (win32)
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
 * Created by: Nickolay Samofatov <skidder@bssys.com>
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */
 
#include <windows.h>
#include <objbase.h>
#include <stdio.h>

#include "../jrd/os/guid.h"
#include "firebird.h"
#include "fb_exception.h"

void GenerateGuid(FB_GUID *guid) {
	if (!SUCCEEDED(CoCreateGuid((GUID*)guid)))
		Firebird::system_call_failed::raise();
}

void GuidToString(char* buffer, FB_GUID *guid) {
	sprintf(buffer, "{%04hX%04hX-%04hX-%04hX-%04hX-%04hX%04hX%04hX}", 
		guid->data[0], guid->data[1], guid->data[2], guid->data[3],
		guid->data[4], guid->data[5], guid->data[6], guid->data[7]);
}

void StringToGuid(FB_GUID *guid, char* buffer) {
	sscanf(buffer, "{%04hX%04hX-%04hX-%04hX-%04hX-%04hX%04hX%04hX}", 
		&guid->data[0], &guid->data[1], &guid->data[2], &guid->data[3],
		&guid->data[4], &guid->data[5], &guid->data[6], &guid->data[7]);
}
