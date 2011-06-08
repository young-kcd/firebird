/*
 *	PROGRAM:		Firebird authentication
 *	MODULE:			Auth.cpp
 *	DESCRIPTION:	Implementation of interfaces, passed to plugins
 *					Plugins loader
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
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2010 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../common/Auth.h"
#include "../jrd/ibase.h"
#include "../common/classes/ImplementHelper.h"
#include "../common/utils_proto.h"

namespace Auth {

WriterImplementation::WriterImplementation(bool svcFlag)
	: body(*getDefaultMemoryPool()),
	  sequence(0), tag(svcFlag ? isc_spb_auth_block : isc_dpb_auth_block)
{ }

void WriterImplementation::store(Firebird::ClumpletWriter& to)
{
	to.deleteWithTag(tag);
	to.insertBytes(tag, body.getBuffer(), body.getBufferLength());
}

void WriterImplementation::reset()
{
	body.clear();
	sequence = 0;
}

void WriterImplementation::add(const char* name, const char* method, const char* details)
{
	body.putLevel(++sequence, name, method, details);
}


DpbImplementation::DpbImplementation(Firebird::ClumpletWriter& base)
	: body(&base)
{ }

int DpbImplementation::find(UCHAR tag)
{
	return body->find(tag) ? 1 : 0;
}

void DpbImplementation::add(UCHAR tag, const void* bytes, unsigned int count)
{
	body->insertBytes(tag, bytes, count);
}

void DpbImplementation::drop()
{
	body->deleteClumplet();
}


bool legacy(const char* nm)
{
	const char* legacyTrusted = "WIN_SSPI";

	return fb_utils::stricmp(legacyTrusted, nm) == 0;
}

} // namespace Auth
