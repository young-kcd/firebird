/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		guid.cpp
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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

void GenerateGuid(FB_GUID *guid) {
	int fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
		Firebird::system_call_failed::raise();
	if (read(fd, guid, sizeof(FB_GUID)) != sizeof(FB_GUID))
		Firebird::system_call_failed::raise();
	close(fd);
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
