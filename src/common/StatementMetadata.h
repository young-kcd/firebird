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
 *
 */

#ifndef COMMON_STATEMENT_METADATA_H
#define COMMON_STATEMENT_METADATA_H

#include "firebird/Provider.h"
#include "iberror.h"
#include "../common/classes/Nullable.h"
#include "../common/classes/array.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/objects_array.h"
#include "../common/classes/ImplementHelper.h"

namespace Firebird {


// Make new metadata support work together with old info-based buffers.
class StatementMetadata : public PermanentStorage
{
public:
	class Parameters : public VersionedIface<IParametersMetadata, FB_PARAMETERS_METADATA_VERSION>,
					   public PermanentStorage
	{
	public:
		struct Item : public PermanentStorage
		{
			explicit Item(MemoryPool& pool)
				: PermanentStorage(pool),
				  field(pool),
				  relation(pool),
				  owner(pool),
				  alias(pool),
				  type(0),
				  subType(0),
				  length(0),
				  scale(0),
				  nullable(false),
				  finished(false)
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
			bool nullable;
			bool finished;
		};

	public:
		explicit Parameters(MemoryPool& pool)
			: PermanentStorage(pool),
			  items(pool),
			  fetched(false)
		{
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

		virtual bool FB_CARG isNullable(IStatus* status, unsigned index) const
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

	private:
		void raiseIndexError(IStatus* status, unsigned index, const char* method) const
		{
			status->set((Arg::Gds(isc_invalid_index_val) <<
				Arg::Num(index) << (string("IParametersMetadata::") + method)).value());
		}

	public:
		ObjectsArray<Item> items;
		bool fetched;
	};

public:
	StatementMetadata(MemoryPool& pool, IStatement* aStatement)
		: PermanentStorage(pool),
		  statement(aStatement),
		  legacyPlan(pool),
		  detailedPlan(pool),
		  inputParameters(pool),
		  outputParameters(pool)
	{
	}

	static unsigned buildInfoItems(Array<UCHAR>& items, unsigned flags);
	static unsigned buildInfoFlags(unsigned itemsLength, const UCHAR* items);

	unsigned getType();
	const char* getPlan(bool detailed);
	const IParametersMetadata* getInputParameters();
	const IParametersMetadata* getOutputParameters();
	ISC_UINT64 getAffectedRecords();

	void clear();
	void parse(unsigned bufferLength, const UCHAR* buffer);
	void getAndParse(unsigned itemsLength, const UCHAR* items, unsigned bufferLength, UCHAR* buffer);
	bool fillFromCache(unsigned itemsLength, const UCHAR* items, unsigned bufferLength, UCHAR* buffer);

private:
	void fetchParameters(UCHAR code, Parameters* parameters);

private:
	IStatement* statement;
	Nullable<unsigned> type;
	string legacyPlan, detailedPlan;
	Parameters inputParameters, outputParameters;
};


}	// namespace Firebird

#endif	// COMMON_STATEMENT_METADATA_H
