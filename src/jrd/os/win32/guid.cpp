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
 
#include "../jrd/os/guid.h"
#include "firebird.h"
#include "fb_exception.h"
#include <windows.h>

void GenerateGuid(FB_GUID *guid) {
	if (!SUCCEEDED(CoCreateGuid((GUID*)guid)))
		Firebird::system_call_failed::raise();
}

void GuidToString(char* buffer, FB_GUID *guid) {
	char* temp;
	if (UuidToString((UUID*)guid,&temp)!=RPC_S_OK)
		Firebird::system_call_failed::raise();
	strcpy(buffer, temp);
	RpcStringFree(&temp);
}

void StringToGuid(FB_GUID *guid, char* buffer) {
	if (UuidFromString((unsigned char*)buffer, (UUID*)guid) != RPC_S_OK)
		Firebird::system_call_failed::raise();
}
