/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		guid.h
 *	DESCRIPTION:	Portable GUID definition
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

#ifndef GUID_H
#define GUID_H

#include "fb_types.h"

#define GUID_BUFF_SIZE 39

typedef struct FB_GUID {
	USHORT data[8];
};

extern void GenerateGuid(FB_GUID *guid);

// This functions receive buffers of at least GUID_BUFF_SIZE length
extern void GuidToString(char* buffer, FB_GUID *guid);
extern void StringToGuid(FB_GUID *guid, char* buffer);

#endif
