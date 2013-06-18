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
 *  Copyright (c) 2011 Adriano dos Santos Fernandes <adrianosf at gmail.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *		Alex Peshkoff
 *
 */

#include "firebird.h"
#include "../common/MsgMetadata.h"
#include "../common/utils_proto.h"
#include "../common/classes/MetaName.h"
#include "../jrd/align.h"

using namespace Firebird;

namespace {

class MetadataBuilder : public RefCntIface<IMetadataBuilder, FB_METADATA_BUILDER_VERSION>
{
public:
	MetadataBuilder(const MsgMetadata* from)
		: msgMetadata(new MsgMetadata)
	{
		msgMetadata->items = from->items;
	}

	virtual int FB_CARG release()
	{
		if (--refCounter != 0)
		{
			return 1;
		}

		delete this;
		return 0;
	}

	// IMetadataBuilder implementation

	virtual void FB_CARG setType(IStatus* status, unsigned index, unsigned type)
	{
		try
		{
			MutexLockGuard g(mtx, FB_FUNCTION);

			indexError(index, "setType");
			msgMetadata->items[index].type = type;
			if (!msgMetadata->items[index].length)
			{
				unsigned dtype;
				fb_utils::sqlTypeToDsc(0, type, 0, &dtype, NULL, NULL, NULL);
				if (dtype < DTYPE_TYPE_MAX)
					msgMetadata->items[index].length = type_lengths[dtype];
			}

			// Setting type & length is enough for an item to be ready for use
			if (msgMetadata->items[index].length)
				msgMetadata->items[index].finished = true;
		}
		catch (const Exception& ex)
		{
			ex.stuffException(status);
		}
	}

	virtual void FB_CARG setSubType(IStatus* status, unsigned index, unsigned subType)
	{
		try
		{
			MutexLockGuard g(mtx, FB_FUNCTION);

			indexError(index, "setSubType");
			msgMetadata->items[index].subType = subType;
		}
		catch (const Exception& ex)
		{
			ex.stuffException(status);
		}
	}

	virtual void FB_CARG setLength(IStatus* status, unsigned index, unsigned length)
	{
		try
		{
			MutexLockGuard g(mtx, FB_FUNCTION);

			indexError(index, "setLength");
			msgMetadata->items[index].length = length;

			// Setting type & length is enough for an item to be ready for use
			if (msgMetadata->items[index].type)
				msgMetadata->items[index].finished = true;
		}
		catch (const Exception& ex)
		{
			ex.stuffException(status);
		}
	}

	virtual void FB_CARG setCharSet(IStatus* status, unsigned index, unsigned charSet)
	{
		try
		{
			MutexLockGuard g(mtx, FB_FUNCTION);

			indexError(index, "setCharSet");
			msgMetadata->items[index].charSet = charSet;
		}
		catch (const Exception& ex)
		{
			ex.stuffException(status);
		}
	}

	virtual void FB_CARG setScale(IStatus* status, unsigned index, unsigned scale)
	{
		try
		{
			MutexLockGuard g(mtx, FB_FUNCTION);

			indexError(index, "setScale");
			msgMetadata->items[index].scale = scale;
		}
		catch (const Exception& ex)
		{
			ex.stuffException(status);
		}
	}

	virtual void FB_CARG truncate(IStatus* status, unsigned count)
	{
		try
		{
			MutexLockGuard g(mtx, FB_FUNCTION);

			if (count != 0)
				indexError(count - 1, "truncate");

			msgMetadata->items.shrink(count);
		}
		catch (const Exception& ex)
		{
			ex.stuffException(status);
		}
	}

	virtual void FB_CARG moveNameToIndex(IStatus* status, const char* name, unsigned index)
	{
		try
		{
			MutexLockGuard g(mtx, FB_FUNCTION);

			indexError(index, "moveNameToIndex");

			for (ObjectsArray<MsgMetadata::Item>::iterator i = msgMetadata->items.begin();
				 i != msgMetadata->items.end();
				 ++i)
			{
				if (i->field == name)
				{
					MsgMetadata::Item copy(getPool(), *i);
					msgMetadata->items.remove(i);
					msgMetadata->items.insert(index, copy);
					return;
				}
			}

			(Arg::Gds(isc_random) << (string("Name not found in IMetadataBuilder: ") + name)).raise();
		}
		catch (const Exception& ex)
		{
			ex.stuffException(status);
		}
	}

	virtual IMessageMetadata* FB_CARG getMetadata(IStatus* status)
	{
		try
		{
			MutexLockGuard g(mtx, FB_FUNCTION);

			metadataError("getMetadata");

			unsigned i = msgMetadata->makeOffsets();
			if (i != ~0u)
			{
				(Arg::Gds(isc_item_finish) << Arg::Num(i)).raise();
			}

			IMessageMetadata* rc = msgMetadata;
			rc->addRef();
			msgMetadata = NULL;
			return rc;
		}
		catch (const Exception& ex)
		{
			ex.stuffException(status);
		}
		return NULL;
	}

private:
	RefPtr<MsgMetadata> msgMetadata;
	Mutex mtx;

	void metadataError(const char* functionName)
	{
		if (!msgMetadata)
		{
			(Arg::Gds(isc_random) << (string("IMetadataBuilder interface is already inactive: "
				"IMetadataBuilder::") + functionName)).raise();
		}
	}

	void indexError(unsigned index, const char* functionName)
	{
		metadataError(functionName);

		if (index >= msgMetadata->items.getCount())
		{
			(Arg::Gds(isc_invalid_index_val) << Arg::Num(index) << (string("IMetadataBuilder::") +
				functionName)).raise();
		}
	}
};

}	// namespace anonymous


namespace Firebird {


// Add an item based on a descriptor.
void MsgMetadata::addItem(const MetaName& name, bool nullable, const dsc& desc)
{
	Item& item = items.add();
	item.field = name.c_str();
	item.nullable = nullable;

	SLONG sqlLen, sqlSubType, sqlScale, sqlType;
	desc.getSqlInfo(&sqlLen, &sqlSubType, &sqlScale, &sqlType);

	item.type = sqlType;
	item.subType = sqlSubType;
	item.length = sqlLen;
	item.scale = sqlScale;

	item.finished = true;
}


// returns ~0 on success or index of not finished item
unsigned MsgMetadata::makeOffsets()
{
	length = 0;

	for (unsigned n = 0; n < items.getCount(); ++n)
	{
		Item* param = &items[n];
		if (!param->finished)
		{
			length = 0;
			return n;
		}
		length = fb_utils::sqlTypeToDsc(length, param->type, param->length,
			NULL /*dtype*/, NULL /*length*/, &param->offset, &param->nullInd);
	}

	return ~0;
}


IMetadataBuilder* MsgMetadata::getBuilder(IStatus* status) const
{
	IMetadataBuilder* rc = new MetadataBuilder(this);
	rc->addRef();
	return rc;
}


}	// namespace Firebird
