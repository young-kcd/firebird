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
 *		Alex Peshkov
 *
 */

#ifndef COMMON_MSG_METADATA_H
#define COMMON_MSG_METADATA_H

#include "firebird/Provider.h"
#include "iberror.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/objects_array.h"
#include "../common/classes/ImplementHelper.h"

namespace Firebird {


class MsgMetadata : public RefCntIface<IMessageMetadata, FB_MESSAGE_METADATA_VERSION>
{
public:
	struct Item
	{
		explicit Item(MemoryPool& pool)
			: field(pool),
			  relation(pool),
			  owner(pool),
			  alias(pool),
			  type(0),
			  subType(0),
			  length(0),
			  scale(0),
			  charset(0),
			  offset(0),
			  nullInd(0),
			  nullable(false),
			  finished(false)
		{
		}

		Item(MemoryPool& pool, const Item& v)
			: field(pool, v.field),
			  relation(pool, v.relation),
			  owner(pool, v.owner),
			  alias(pool, v.alias),
			  type(v.type),
			  subType(v.subType),
			  length(v.length),
			  scale(v.scale),
			  charset(v.charset),
			  offset(v.offset),
			  nullInd(v.nullInd),
			  nullable(v.nullable),
			  finished(v.finished)
		{
		}

		string field;
		string relation;
		string owner;
		string alias;
		unsigned type;
		unsigned subType;
		unsigned length;
		unsigned scale;
		unsigned charset;
		unsigned offset;
		unsigned nullInd;
		bool nullable;
		bool finished;
	};

public:
	MsgMetadata()
		: items(getPool()),
		  length(0)
	{
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

	virtual unsigned FB_CARG getCount(IStatus* /*status*/) const
	{
		return (unsigned) items.getCount();
	}

	virtual const char* FB_CARG getField(IStatus* status, unsigned index) const
	{
		if (index < items.getCount())
			return items[index].field.c_str();

		raiseIndexError(status, index, "getField");
		return NULL;
	}

	virtual const char* FB_CARG getRelation(IStatus* status, unsigned index) const
	{
		if (index < items.getCount())
			return items[index].relation.c_str();

		raiseIndexError(status, index, "getRelation");
		return NULL;
	}

	virtual const char* FB_CARG getOwner(IStatus* status, unsigned index) const
	{
		if (index < items.getCount())
			return items[index].owner.c_str();

		raiseIndexError(status, index, "getOwner");
		return NULL;
	}

	virtual const char* FB_CARG getAlias(IStatus* status, unsigned index) const
	{
		if (index < items.getCount())
			return items[index].alias.c_str();

		raiseIndexError(status, index, "getAlias");
		return NULL;
	}

	virtual unsigned FB_CARG getType(IStatus* status, unsigned index) const
	{
		if (index < items.getCount())
			return items[index].type;

		raiseIndexError(status, index, "getType");
		return 0;
	}

	virtual FB_BOOLEAN FB_CARG isNullable(IStatus* status, unsigned index) const
	{
		if (index < items.getCount())
			return items[index].nullable;

		raiseIndexError(status, index, "isNullable");
		return false;
	}

	virtual unsigned FB_CARG getSubType(IStatus* status, unsigned index) const
	{
		if (index < items.getCount())
			return items[index].subType;

		raiseIndexError(status, index, "getSubType");
		return 0;
	}

	virtual unsigned FB_CARG getLength(IStatus* status, unsigned index) const
	{
		if (index < items.getCount())
			return items[index].length;

		raiseIndexError(status, index, "getLength");
		return 0;
	}

	virtual unsigned FB_CARG getScale(IStatus* status, unsigned index) const
	{
		if (index < items.getCount())
			return items[index].scale;

		raiseIndexError(status, index, "getScale");
		return 0;
	}

	virtual unsigned FB_CARG getCharset(IStatus* status, unsigned index) const
	{
		if (index < items.getCount())
			return items[index].charset;

		raiseIndexError(status, index, "getCharset");
		return 0;
	}

	virtual unsigned FB_CARG getOffset(IStatus* status, unsigned index) const
	{
		if (index < items.getCount())
			return items[index].offset;

		raiseIndexError(status, index, "getOffset");
		return 0;
	}

	virtual unsigned FB_CARG getNullOffset(IStatus* status, unsigned index) const
	{
		if (index < items.getCount())
			return items[index].nullInd;

		raiseIndexError(status, index, "getOffset");
		return 0;
	}

	virtual IMetadataBuilder* FB_CARG getBuilder(IStatus* status) const;

	virtual unsigned FB_CARG getMessageLength(IStatus* /*status*/) const
	{
		return length;
	}

public:
	void makeOffsets();

private:
	void raiseIndexError(IStatus* status, unsigned index, const char* method) const
	{
		status->set((Arg::Gds(isc_invalid_index_val) <<
			Arg::Num(index) << (string("IMessageMetadata::") + method)).value());
	}

public:
	ObjectsArray<Item> items;
	unsigned length;
};


}	// namespace Firebird

#endif	// COMMON_MSG_METADATA_H
