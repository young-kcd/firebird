/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		guid.h
 *	DESCRIPTION:	Portable GUID definition
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
 *  $Id: guid.h,v 1.4 2003-10-03 01:48:13 brodsom Exp $
 *
 */

#ifndef GUID_H
#define GUID_H

#include "fb_types.h"

#define GUID_BUFF_SIZE 39

struct FB_GUID {
	USHORT data[8];
};

void GenerateGuid(FB_GUID *guid);

// This functions receive buffers of at least GUID_BUFF_SIZE length
void GuidToString(char* buffer, FB_GUID *guid);
void StringToGuid(FB_GUID *guid, char* buffer);

#endif
