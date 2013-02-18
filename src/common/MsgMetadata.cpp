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

using namespace Firebird;

namespace {

class CoerceMetadata : public RefCntIface<ICoerceMetadata, FB_COERCE_METADATA_VERSION>
{
public:
	CoerceMetadata(const MsgMetadata* from)
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

	// ICoerceMetadata implementation
	virtual void FB_CARG setType(IStatus* status, unsigned index, unsigned type)
	{
		MutexLockGuard g(mtx, FB_FUNCTION);

		if (!indexError(status, index, "setType"))
			msgMetadata->items[index].type = type;
	}

	virtual void FB_CARG setSubType(IStatus* status, unsigned index, unsigned subType)
	{
		MutexLockGuard g(mtx, FB_FUNCTION);

		if (!indexError(status, index, "setSubType"))
			msgMetadata->items[index].subType = subType;
	}

	virtual void FB_CARG setLength(IStatus* status, unsigned index, unsigned length)
	{
		MutexLockGuard g(mtx, FB_FUNCTION);

		if (!indexError(status, index, "setLength"))
			msgMetadata->items[index].length = length;
	}

	virtual void FB_CARG setScale(IStatus* status, unsigned index, unsigned scale)
	{
		MutexLockGuard g(mtx, FB_FUNCTION);

		if (!indexError(status, index, "setScale"))
			msgMetadata->items[index].scale = scale;
	}

	virtual IMessageMetadata* FB_CARG getMetadata(IStatus* status)
	{
		MutexLockGuard g(mtx, FB_FUNCTION);

		if (metadataError(status, "getMetadata"))
		{
			return NULL;
		}

		msgMetadata->makeOffsets();
		IMessageMetadata* rc = msgMetadata;
		rc->addRef();
		msgMetadata = NULL;
		return rc;
	}

private:
	RefPtr<MsgMetadata> msgMetadata;
	Mutex mtx;

	bool metadataError(IStatus* status, const char* functionName)
	{
		if (!msgMetadata)
		{
			status->set((Arg::Gds(isc_random) <<
				(string("Coerce interface is already inactive: ICoerceMetadata::") + functionName)).value());
			return true;
		}

		return false;
	}

	bool indexError(IStatus* status, unsigned index, const char* functionName)
	{
		if (metadataError(status, functionName))
		{
			return true;
		}

		if (index >= msgMetadata->items.getCount())
		{
			status->set((Arg::Gds(isc_invalid_index_val) <<
				Arg::Num(index) << (string("ICoerceMetadata::") + functionName)).value());
			return true;
		}

		return false;
	}
};

}	// namespace anonymous


namespace Firebird {


void MsgMetadata::makeOffsets()
{
	length = 0;

	for (unsigned n = 0; n < items.getCount(); ++n)
	{
		Item* param = &items[n];
		if (!param->finished)
		{
			length = 0;
			return;
		}
		length = fb_utils::sqlTypeToDsc(length, param->type, param->length,
			NULL /*dtype*/, NULL /*length*/, &param->offset, &param->nullInd);
	}
}


ICoerceMetadata* MsgMetadata::coerce(IStatus* status) const
{
	ICoerceMetadata* rc = new CoerceMetadata(this);
	rc->addRef();
	return rc;
}


}	// namespace Firebird
