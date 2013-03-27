/*
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
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2013 Adriano dos Santos Fernandes <adrianosf@gmail.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/Routine.h"
#include "../jrd/jrd.h"
#include "../common/StatusHolder.h"

using namespace Firebird;


namespace Jrd {


// Create a MsgMetadata from a parameters array.
MsgMetadata* Routine::createMetadata(const Array<NestConst<Parameter> >& parameters)
{
	RefPtr<MsgMetadata> metadata(new MsgMetadata);

	for (Array<NestConst<Parameter> >::const_iterator i = parameters.begin();
		 i != parameters.end();
		 ++i)
	{
		metadata->addItem((*i)->prm_name, (*i)->prm_nullable, (*i)->prm_desc);
	}

	metadata->addRef();
	return metadata;
}

// Create a Format based on an IMessageMetadata.
Format* Routine::createFormat(MemoryPool& pool, IMessageMetadata* params, bool addEof)
{
	LocalStatus status;

	unsigned count = params->getCount(&status);
	status.check();

	Format* format = Format::newFormat(pool, count * 2 + (addEof ? 1 : 0));
	unsigned runOffset = 0;

	dsc* desc = format->fmt_desc.begin();

	for (unsigned i = 0; i < count; ++i)
	{
		unsigned descOffset, nullOffset, descDtype, descLength;

		runOffset = fb_utils::sqlTypeToDsc(runOffset, params->getType(&status, i),
			params->getLength(&status, i), &descDtype, &descLength,
			&descOffset, &nullOffset);
		status.check();

		desc->clear();
		desc->dsc_dtype = descDtype;
		desc->dsc_length = descLength;
		desc->dsc_scale = params->getScale(&status, i);
		desc->dsc_sub_type = params->getSubType(&status, i);
		desc->setTextType(params->getCharset(&status, i));
		desc->dsc_address = (UCHAR*)(IPTR) descOffset;
		desc->dsc_flags = (params->isNullable(&status, i) ? DSC_nullable : 0);

		++desc;
		desc->makeShort(0, (SSHORT*)(IPTR) nullOffset);
		status.check();

		++desc;
	}

	if (addEof)
	{
		// Next item is aligned on USHORT, so as the previous one.
		desc->makeShort(0, (SSHORT*)(IPTR) runOffset);
		runOffset += sizeof(USHORT);
	}

	format->fmt_length = runOffset;

	return format;
}


}	// namespace Jrd
