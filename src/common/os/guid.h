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
 *  Adriano dos Santos Fernandes
 *
 */

#ifndef FB_GUID_H
#define FB_GUID_H

#include <stdlib.h>
#include <stdio.h>
#include "fb_types.h"

namespace Firebird {

const int GUID_BUFF_SIZE = 39;
const int GUID_BODY_SIZE = 36;

const char* const GUID_LEGACY_FORMAT =
	"{%04hX%04hX-%04hX-%04hX-%04hX-%04hX%04hX%04hX}";
const char* const GUID_NEW_FORMAT =
	"{%02hX%02hX%02hX%02hX-%02hX%02hX-%02hX%02hX-%02hX%02hX-%02hX%02hX%02hX%02hX%02hX%02hX}";

struct Guid
{
	enum Style
	{
		STYLE_NBACKUP,	// Format introduced with nbackup
		STYLE_BROKEN,	// Format introduced in FB 2.5.0
		STYLE_UUID		// Format as defined in the RFC-4122.
	};

	union
	{
		USHORT data[8];

		struct	// Compatible with Win32 GUID struct layout.
		{
			ULONG data1;
			USHORT data2;
			USHORT data3;
			UCHAR data4[8];
		};
	};
};

void GenerateRandomBytes(void* buffer, size_t size);
void GenerateGuid(Guid* guid);

// These functions receive buffers of at least GUID_BUFF_SIZE length

inline void GuidToString(char* buffer, const Guid* guid, Guid::Style style)
{
	switch (style)
	{
		case Guid::STYLE_NBACKUP:
			sprintf(buffer, GUID_LEGACY_FORMAT,
				guid->data[0], guid->data[1], guid->data[2], guid->data[3],
				guid->data[4], guid->data[5], guid->data[6], guid->data[7]);
			break;

		case Guid::STYLE_BROKEN:
			sprintf(buffer, GUID_NEW_FORMAT,
				USHORT(guid->data[0] & 0xFF), USHORT(guid->data[0] >> 8),
				USHORT(guid->data[1] & 0xFF), USHORT(guid->data[1] >> 8),
				USHORT(guid->data[2] & 0xFF), USHORT(guid->data[2] >> 8),
				USHORT(guid->data[3] & 0xFF), USHORT(guid->data[3] >> 8),
				USHORT(guid->data[4] & 0xFF), USHORT(guid->data[4] >> 8),
				USHORT(guid->data[5] & 0xFF), USHORT(guid->data[5] >> 8),
				USHORT(guid->data[6] & 0xFF), USHORT(guid->data[6] >> 8),
				USHORT(guid->data[7] & 0xFF), USHORT(guid->data[7] >> 8));
			break;

		case Guid::STYLE_UUID:
			sprintf(buffer, GUID_NEW_FORMAT,
				USHORT((guid->data1 >> 24) & 0xFF), USHORT((guid->data1 >> 16) & 0xFF),
				USHORT((guid->data1 >> 8) & 0xFF), USHORT(guid->data1 & 0xFF),
				USHORT((guid->data2 >> 8) & 0xFF), USHORT(guid->data2 & 0xFF),
				USHORT((guid->data3 >> 8) & 0xFF), USHORT(guid->data3 & 0xFF),
				USHORT(guid->data4[0]), USHORT(guid->data4[1]),
				USHORT(guid->data4[2]), USHORT(guid->data4[3]),
				USHORT(guid->data4[4]), USHORT(guid->data4[5]),
				USHORT(guid->data4[6]), USHORT(guid->data4[7]));
			break;
	}
}

inline void StringToGuid(Guid* guid, const char* buffer, Guid::Style style)
{
	if (style == Guid::STYLE_NBACKUP)
	{
		sscanf(buffer, GUID_LEGACY_FORMAT,
			&guid->data[0], &guid->data[1], &guid->data[2], &guid->data[3],
			&guid->data[4], &guid->data[5], &guid->data[6], &guid->data[7]);
	}
	else
	{
		USHORT bytes[16];
		sscanf(buffer, GUID_NEW_FORMAT,
			&bytes[0], &bytes[1], &bytes[2], &bytes[3],
			&bytes[4], &bytes[5], &bytes[6], &bytes[7],
			&bytes[8], &bytes[9], &bytes[10], &bytes[11],
			&bytes[12], &bytes[13], &bytes[14], &bytes[15]);

		if (style == Guid::STYLE_BROKEN)
		{
			guid->data[0] = bytes[0] | (bytes[1] << 8);
			guid->data[1] = bytes[2] | (bytes[3] << 8);
			guid->data[2] = bytes[4] | (bytes[5] << 8);
			guid->data[3] = bytes[6] | (bytes[7] << 8);
			guid->data[4] = bytes[8] | (bytes[9] << 8);
			guid->data[5] = bytes[10] | (bytes[11] << 8);
			guid->data[6] = bytes[12] | (bytes[13] << 8);
			guid->data[7] = bytes[14] | (bytes[15] << 8);
		}
		else if (style == Guid::STYLE_UUID)
		{
			guid->data1 = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
			guid->data2 = (bytes[4] << 8) | bytes[5];
			guid->data3 = (bytes[6] << 8) | bytes[7];
			guid->data4[0] = bytes[8];
			guid->data4[1] = bytes[9];
			guid->data4[2] = bytes[10];
			guid->data4[3] = bytes[11];
			guid->data4[4] = bytes[12];
			guid->data4[5] = bytes[13];
			guid->data4[6] = bytes[14];
			guid->data4[7] = bytes[15];
		}
	}
}

}	// namespace

#endif	// FB_GUID_H
