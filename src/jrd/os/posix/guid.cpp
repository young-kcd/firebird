/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		guid.h
 *	DESCRIPTION:	Portable GUID (posix)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * You may obtain a copy of the Licence at
 * http://www.gnu.org/licences/lgpl.html
 * 
 * As a special exception this file can also be included in modules
 * with other source code as long as that source code has been 
 * released under an Open Source Initiative certificed licence.  
 * More information about OSI certification can be found at: 
 * http://www.opensource.org 
 * 
 * This module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public Licence for more details.
 * 
 * This module was created by members of the firebird development 
 * team.  All individual contributions remain the Copyright (C) of 
 * those individuals and all rights are reserved.  Contributors to 
 * this file are either listed below or can be obtained from a CVS 
 * history command.
 *
 *  Created by: Nickolay Samofatov <skidder@bssys.com>
 *
 *  Contributor(s):
 * 
 *
 *  $Id: guid.cpp,v 1.3 2003-09-10 15:09:18 skidder Exp $
 *
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
	// do not use /dev/random because it may return lesser data than we need.
	int fd = open("/dev/urandom", O_RDONLY);
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
