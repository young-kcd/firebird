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
 *  The Original Code was created by Dmitry Yemanov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2009 Dmitry Yemanov <dimitr@firebirdsql.org>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/jrd.h"
#include "../jrd/btr.h"
#include "../jrd/req.h"
#include "../jrd/intl.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/intl_proto.h"

#include "RecordSource.h"

using namespace Firebird;
using namespace Jrd;

// ----------------------
// Data access: hash join
// ----------------------

class HashJoin::HashTable : public PermanentStorage
{
	static const size_t HASH_SIZE = 1009;

	typedef Firebird::Array<FB_UINT64> CollisionList;

public:
	HashTable(MemoryPool& pool, size_t streamCount, size_t tableSize = HASH_SIZE)
		: PermanentStorage(pool), m_streamCount(streamCount), m_tableSize(tableSize), m_slot(0)
	{
		m_collisions = FB_NEW(pool) CollisionList*[streamCount * tableSize];
		memset(m_collisions, 0, streamCount * tableSize * sizeof(CollisionList*));

		m_iterators = FB_NEW(pool) size_t[streamCount];
		memset(m_iterators, 0, streamCount * sizeof(size_t));
	}

	~HashTable()
	{
		for (size_t i = 0; i < m_streamCount * m_tableSize; i++)
		{
			delete m_collisions[i];
		}

		delete[] m_collisions;
		delete[] m_iterators;
	}

	size_t hash(const UCHAR* address, size_t length, ULONG* prior_value = NULL)
	{
		ULONG hash_value = prior_value ? *prior_value : 0;

		UCHAR* p = NULL;
		const UCHAR* q = address;
		for (size_t l = 0; l < length; l++)
		{
			if (!(l & 3))
			{
				p = (UCHAR*) &hash_value;
			}

			*p++ += *q++;
		}

		if (prior_value)
		{
			*prior_value = hash_value;
		}

		return (hash_value % m_tableSize);
	}

	void put(size_t stream, size_t slot, FB_UINT64 value)
	{
		fb_assert(stream < m_streamCount);
		fb_assert(slot < m_tableSize);

		CollisionList* collisions = m_collisions[stream * m_streamCount + slot];

		if (!collisions)
		{
			collisions = FB_NEW(getPool()) CollisionList(getPool());
			m_collisions[stream * m_streamCount + slot] = collisions;
		}

		collisions->add(value);
	}

	bool setup(size_t slot)
	{
		fb_assert(slot < m_tableSize);

		for (size_t i = 0; i < m_streamCount; i++)
		{
			if (!m_collisions[i * m_streamCount + slot])
			{
				return false;
			}

			reset(i);
		}

		m_slot = slot;
		return true;
	}

	void reset(size_t stream)
	{
		fb_assert(stream < m_streamCount);

		m_iterators[stream] = 0;
	}

	bool iterate(size_t stream, FB_UINT64& value)
	{
		fb_assert(stream < m_streamCount);

		CollisionList* const collisions = m_collisions[stream * m_streamCount + m_slot];
		size_t& iterator = m_iterators[stream];

		if (iterator < collisions->getCount())
		{
			value = (*collisions)[iterator++];
			return true;
		}

		return false;
	}

private:
	const size_t m_streamCount;
	const size_t m_tableSize;
	CollisionList** m_collisions;
	size_t* m_iterators;
	size_t m_slot;
};


HashJoin::HashJoin(CompilerScratch* csb, size_t count,
				   RecordSource* const* args, const NestValueArray* const* keys)
	: m_leader(args[0]), m_leaderKeys(keys[0]), m_args(csb->csb_pool, count - 1),
	  m_keys(csb->csb_pool, count - 1), m_outerJoin(false), m_semiJoin(false), m_antiJoin(false)
{
	fb_assert(count >= 2);

	m_impure = CMP_impure(csb, sizeof(Impure));

	fb_assert(args[0]);
	fb_assert(keys[0]);

	for (size_t i = 1; i < count; i++)
	{
		fb_assert(args[i]);
		m_args.add(FB_NEW(csb->csb_pool) BufferedStream(csb, args[i]));

		fb_assert(keys[i]);
		m_keys.add(keys[i]);
	}
}

void HashJoin::open(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = request->getImpure<Impure>(m_impure);

	impure->irsb_flags = irsb_open | irsb_mustread;

	delete impure->irsb_hash_table;
	MemoryPool& pool = *tdbb->getDefaultPool();
	impure->irsb_hash_table = FB_NEW(pool) HashTable(pool, m_args.getCount());

	for (size_t i = 0; i < m_args.getCount(); i++)
	{
		// Read and cache the inner streams. While doing that,
		// hash the join condition values and populate hash tables.

		m_args[i]->open(tdbb);

		FB_UINT64 position = 0;
		while (m_args[i]->getRecord(tdbb))
		{
			const size_t hash_slot = hashKeys(tdbb, request, impure->irsb_hash_table, m_keys[i]);
			impure->irsb_hash_table->put(i, hash_slot, position++);
		}
	}

	m_leader->open(tdbb);
}

void HashJoin::close(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = request->getImpure<Impure>(m_impure);

	invalidateRecords(request);

	if (impure->irsb_flags & irsb_open)
	{
		impure->irsb_flags &= ~irsb_open;

		delete impure->irsb_hash_table;
		impure->irsb_hash_table = NULL;

		for (size_t i = 0; i < m_args.getCount(); i++)
		{
			m_args[i]->close(tdbb);
		}

		m_leader->close(tdbb);
	}
}

bool HashJoin::getRecord(thread_db* tdbb) const
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = request->getImpure<Impure>(m_impure);

	if (!(impure->irsb_flags & irsb_open))
	{
		return false;
	}

	while (true)
	{
		if (impure->irsb_flags & irsb_mustread)
		{
			// Fetch the record from the leading stream

			if (!m_leader->getRecord(tdbb))
			{
				return false;
			}

			// Hash the comparison keys

			const size_t hash_slot = hashKeys(tdbb, request, impure->irsb_hash_table, m_leaderKeys);

			// Ensure the every inner stream having matches for this hash slot.
			// Setup the hash table for the iteration through collisions.

			if (!impure->irsb_hash_table->setup(hash_slot))
			{
				continue;
			}

			impure->irsb_flags &= ~irsb_mustread;
			impure->irsb_flags |= irsb_first;
		}

		// Fetch collisions from the inner streams

		if (impure->irsb_flags & irsb_first)
		{
			for (size_t i = 0; i < m_args.getCount(); i++)
			{
				if (!fetchRecord(tdbb, impure->irsb_hash_table, i))
				{
					fb_assert(false);
					return false;
				}
			}

			impure->irsb_flags &= ~irsb_first;
		}
		else if (!fetchRecord(tdbb, impure->irsb_hash_table, m_args.getCount() - 1))
		{
			impure->irsb_flags |= irsb_mustread;
			continue;
		}

		// Ensure that the comparison keys are really equal

		if (compareKeys(tdbb, request))
		{
			break;
		}
	}

	return true;
}

bool HashJoin::refetchRecord(thread_db* /*tdbb*/) const
{
	return true;
}

bool HashJoin::lockRecord(thread_db* /*tdbb*/) const
{
	status_exception::raise(Arg::Gds(isc_record_lock_not_supp));
	return false; // compiler silencer
}

void HashJoin::print(thread_db* tdbb, string& plan, bool detailed, unsigned level) const
{
	if (detailed)
	{
		plan += printIndent(++level) + "Hash Join (inner)";
		m_leader->print(tdbb, plan, true, level);
		for (size_t i = 0; i < m_args.getCount(); i++)
		{
			m_args[i]->print(tdbb, plan, true, level);
		}
	}
	else
	{
		level++;
		plan += "HASH (";
		m_leader->print(tdbb, plan, false, level);
		plan += ", ";
		for (size_t i = 0; i < m_args.getCount(); i++)
		{
			if (i)
			{
				plan += ", ";
			}
			m_args[i]->print(tdbb, plan, false, level);
		}
		plan += ")";
	}
}

void HashJoin::markRecursive()
{
	m_leader->markRecursive();
	for (size_t i = 0; i < m_args.getCount(); i++)
	{
		m_args[i]->markRecursive();
	}
}

void HashJoin::findUsedStreams(StreamList& streams, bool expandAll) const
{
	m_leader->findUsedStreams(streams, expandAll);
	for (size_t i = 0; i < m_args.getCount(); i++)
	{
		m_args[i]->findUsedStreams(streams, expandAll);
	}
}

void HashJoin::invalidateRecords(jrd_req* request) const
{
	m_leader->invalidateRecords(request);
	for (size_t i = 0; i < m_args.getCount(); i++)
	{
		m_args[i]->invalidateRecords(request);
	}
}

void HashJoin::nullRecords(thread_db* tdbb) const
{
	m_leader->nullRecords(tdbb);
	for (size_t i = 0; i < m_args.getCount(); i++)
	{
		m_args[i]->nullRecords(tdbb);
	}
}

void HashJoin::saveRecords(thread_db* tdbb) const
{
	m_leader->saveRecords(tdbb);
	for (size_t i = 0; i < m_args.getCount(); i++)
	{
		m_args[i]->saveRecords(tdbb);
	}
}

void HashJoin::restoreRecords(thread_db* tdbb) const
{
	m_leader->restoreRecords(tdbb);
	for (size_t i = 0; i < m_args.getCount(); i++)
	{
		m_args[i]->restoreRecords(tdbb);
	}
}

size_t HashJoin::hashKeys(thread_db* tdbb, jrd_req* request, HashTable* table,
	const NestValueArray* keys) const
{
	ULONG hash_value = 0;
	size_t hash_slot = 0;

	for (size_t i = 0; i < keys->getCount(); i++)
	{
		const dsc* const desc = EVL_expr(tdbb, request, (*keys)[i]);

		if (desc && !(request->req_flags & req_null))
		{
			fb_assert(!desc->isBlob());

			USHORT length = desc->dsc_length;
			const UCHAR* address = desc->dsc_address;

			MoveBuffer buffer;

			if (desc->isText())
			{
				// Adjust the data length to the real string length

				if (desc->dsc_dtype == dtype_varying)
				{
					const vary* const string = (vary*) address;
					length = string->vary_length;
					address = (const UCHAR*) string->vary_string;
				}
				else if (desc->dsc_dtype == dtype_cstring)
				{
					length = static_cast<USHORT>(strlen((char*) address));
				}

				if (IS_INTL_DATA(desc))
				{
					// Convert the INTL string into the binary comparable form

					TextType* const obj = INTL_texttype_lookup(tdbb, desc->getTextType());
					const USHORT key_length = obj->key_length(length);
					length = obj->string_to_key(length, address, key_length,
												buffer.getBuffer(key_length),
												INTL_KEY_UNIQUE);
					address = buffer.begin();
				}
				else
				{
					// Adjust the data length to ignore trailing spaces

					CHARSET_ID charset = desc->getCharSet();
					if (charset == ttype_dynamic)
					{
						charset = tdbb->getCharSet();
					}
					const UCHAR space = (charset == ttype_binary) ? '\0' : ' ';
					const UCHAR* ptr = address + length;
					while (length && *--ptr == space)
					{
						length--;
					}
				}
			}

			hash_slot = table->hash(address, length, &hash_value);
		}
	}

	return hash_slot;
}

bool HashJoin::compareKeys(thread_db* tdbb, jrd_req* request) const
{
	for (size_t i = 0; i < m_leaderKeys->getCount(); ++i)
	{
		const dsc* const desc1 = EVL_expr(tdbb, request, (*m_leaderKeys)[i]);
		const bool null1 = (request->req_flags & req_null);

		for (size_t j = 0; j < m_keys.getCount(); j++)
		{
			const dsc* const desc2 = EVL_expr(tdbb, request, (*m_keys[j])[i]);
			const bool null2 = (request->req_flags & req_null);

			if (null1 != null2)
				return false;

			if (!null1 && !null2 && MOV_compare(desc1, desc2) != 0)
				return false;
		}
	}

	return true;
}

bool HashJoin::fetchRecord(thread_db* tdbb, HashTable* table, size_t stream) const
{
	const BufferedStream* const arg = m_args[stream];
	FB_UINT64 position;

	if (table->iterate(stream, position))
	{
		arg->locate(tdbb, position);

		if (arg->getRecord(tdbb))
		{
			return true;
		}
	}

	while (true)
	{
		if (stream == 0 || !fetchRecord(tdbb, table, stream - 1))
		{
			return false;
		}

		table->reset(stream);

		if (table->iterate(stream, position))
		{
			arg->locate(tdbb, position);

			if (arg->getRecord(tdbb))
			{
				return true;
			}
		}
	}
}
