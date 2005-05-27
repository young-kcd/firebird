/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		guid.h
 *	DESCRIPTION:	Portable GUID definition
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Nickolay Samofatov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Nickolay Samofatov <nickolay@broadviewsoftware.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 *  $Id: guid.h,v 1.9 2005-05-27 22:44:11 asfernandes Exp $
 *
 */

#ifndef GUID_H
#define GUID_H

#include <stdlib.h>

#include "fb_types.h"

const int GUID_BUFF_SIZE = 39;

struct FB_GUID {
	USHORT data[8];
};

void GenerateRandomBytes(void* buffer, size_t size);
void GenerateGuid(FB_GUID* guid);

// This functions receive buffers of at least GUID_BUFF_SIZE length
void GuidToString(char* buffer, const FB_GUID* guid);
void StringToGuid(FB_GUID* guid, const char* buffer);

#endif

