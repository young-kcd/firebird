/*
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
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 *
 * Adriano dos Santos Fernandes
 */

#include "firebird.h"
#include "../jrd/Routine.h"
#include "../jrd/jrd.h"
#include "../jrd/exe.h"
#include "../common/StatusHolder.h"
#include "../jrd/par_proto.h"

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

// Parse routine BLR.
void Routine::parseBlr(thread_db* tdbb, CompilerScratch* csb, bid* blob_id)
{
	Jrd::Attachment* attachment = tdbb->getAttachment();

	UCharBuffer tmp;

	if (blob_id)
	{
		blb* blob = blb::open(tdbb, attachment->getSysTransaction(), blob_id);
		ULONG length = blob->blb_length + 10;
		UCHAR* temp = tmp.getBuffer(length);
		length = blob->BLB_get_data(tdbb, temp, length);
		tmp.resize(length);
	}

	parseMessages(tdbb, csb, BlrReader(tmp.begin(), (unsigned) tmp.getCount()));

	JrdStatement* statement = getStatement();
	PAR_blr(tdbb, NULL, tmp.begin(), (ULONG) tmp.getCount(), NULL, &csb, &statement, false, 0);
	setStatement(statement);

	if (!blob_id)
		setImplemented(false);
}

// Parse the messages of a blr request. For specified message, allocate a format (Format) block.
void Routine::parseMessages(thread_db* tdbb, CompilerScratch* csb, BlrReader blrReader)
{
	if (blrReader.getLength() < 2)
		status_exception::raise(Arg::Gds(isc_metadata_corrupt));

	csb->csb_blr_reader = blrReader;

	const SSHORT version = csb->csb_blr_reader.getByte();

	switch (version)
	{
		case blr_version4:
		case blr_version5:
		//case blr_version6:
			break;

		default:
			status_exception::raise(
				Arg::Gds(isc_metadata_corrupt) <<
				Arg::Gds(isc_wroblrver2) << Arg::Num(blr_version4) << Arg::Num(blr_version5/*6*/) <<
					Arg::Num(version));
	}

	if (csb->csb_blr_reader.getByte() != blr_begin)
		status_exception::raise(Arg::Gds(isc_metadata_corrupt));

	while (csb->csb_blr_reader.getByte() == blr_message)
	{
		const USHORT msgNumber = csb->csb_blr_reader.getByte();
		USHORT count = csb->csb_blr_reader.getWord();
		Format* format = Format::newFormat(*tdbb->getDefaultPool(), count);

		USHORT padField;
		const bool shouldPad = csb->csb_message_pad.get(msgNumber, padField);

		USHORT maxAlignment = 0;
		ULONG offset = 0;
		USHORT i = 0;

		for (Format::fmt_desc_iterator desc = format->fmt_desc.begin(); i < count; ++i, ++desc)
		{
			const USHORT align = PAR_desc(tdbb, csb, &*desc);
			if (align)
				offset = FB_ALIGN(offset, align);

			desc->dsc_address = (UCHAR*)(IPTR) offset;
			offset += desc->dsc_length;

			maxAlignment = MAX(maxAlignment, align);

			if (maxAlignment && shouldPad && i + 1 == padField)
				offset = FB_ALIGN(offset, maxAlignment);
		}

		if (offset > MAX_MESSAGE_SIZE)
			status_exception::raise(Arg::Gds(isc_imp_exc) << Arg::Gds(isc_blktoobig));

		format->fmt_length = offset;

		switch (msgNumber)
		{
			case 0:
				setInputFormat(format);
				break;

			case 1:
				setOutputFormat(format);
				break;

			default:
				delete format;
		}
	}
}


}	// namespace Jrd
