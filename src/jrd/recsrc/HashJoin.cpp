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
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/req.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/mov_proto.h"

#include "RecordSource.h"

using namespace Firebird;
using namespace Jrd;

// ----------------------
// Data access: hash join
// ----------------------

HashJoin::HashJoin(CompilerScratch* csb, RecordSource* outer, RecordSource* inner,
				   const jrd_nod* outerKeys, const jrd_nod* innerKeys)
	: m_outer(outer), m_inner(FB_NEW(csb->csb_pool) BufferedStream(csb, inner)),
	  m_outerKeys(outerKeys), m_innerKeys(innerKeys),
	  m_outerJoin(false), m_semiJoin(false), m_antiJoin(false)
{
	fb_assert(m_outer && m_inner);
	fb_assert(m_outerKeys && m_outerKeys->nod_type == nod_list);
	fb_assert(m_innerKeys && m_innerKeys->nod_type == nod_list);

	m_impure = CMP_impure(csb, sizeof(Impure));
}

void HashJoin::open(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	impure->irsb_flags = irsb_open | irsb_mustread;

	for (size_t i = 0; i < HASH_SIZE; i++)
	{
		delete impure->irsb_hash_table[i];
		impure->irsb_hash_table[i] = NULL;
	}

	// Read and cache the inner stream. While doing that,
	// hash the join condition values and populate a hash table.

	m_inner->open(tdbb);
	FB_UINT64 position = 0;

	while (m_inner->getRecord(tdbb))
	{
		const USHORT hash_slot = hashKeys(tdbb, request, false);
		CollisionList* collisions = impure->irsb_hash_table[hash_slot];

		if (!collisions)
		{
			MemoryPool& pool = *tdbb->getDefaultPool();
			collisions = FB_NEW(pool) CollisionList(pool);
			impure->irsb_hash_table[hash_slot] = collisions;
		}

		collisions->add(position++);
	}

	impure->irsb_hash_slot = 0;
	impure->irsb_collision = 0;

	m_outer->open(tdbb);
}

void HashJoin::close(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();

	invalidateRecords(request);

	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (impure->irsb_flags & irsb_open)
	{
		impure->irsb_flags &= ~irsb_open;

		for (size_t i = 0; i < HASH_SIZE; i++)
		{
			delete impure->irsb_hash_table[i];
			impure->irsb_hash_table[i] = NULL;
		}

		m_outer->close(tdbb);
		m_inner->close(tdbb);
	}
}

bool HashJoin::getRecord(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (!(impure->irsb_flags & irsb_open))
	{
		return false;
	}

	while (true)
	{
		if (impure->irsb_flags & irsb_mustread)
		{
			if (!m_outer->getRecord(tdbb))
			{
				return false;
			}

			const USHORT hash_slot = hashKeys(tdbb, request, true);

			if (!impure->irsb_hash_table[hash_slot])
			{
				continue;
			}

			impure->irsb_hash_slot = hash_slot;
			impure->irsb_collision = 0;
			impure->irsb_flags &= ~irsb_mustread;
		}

		CollisionList* const collisions = impure->irsb_hash_table[impure->irsb_hash_slot];

		if (impure->irsb_collision >= collisions->getCount())
		{
			impure->irsb_flags |= irsb_mustread;
			continue;
		}

		const FB_UINT64 position = (*collisions)[impure->irsb_collision++];
		m_inner->locate(tdbb, position);

		if (!m_inner->getRecord(tdbb))
		{
			fb_assert(false);
			return false;
		}

		if (compareKeys(tdbb, request))
		{
			break;
		}
	}

	return true;
}

bool HashJoin::refetchRecord(thread_db* tdbb)
{
	return true;
}

bool HashJoin::lockRecord(thread_db* tdbb)
{
	status_exception::raise(Arg::Gds(isc_record_lock_not_supp));
	return false; // compiler silencer
}

void HashJoin::dump(thread_db* tdbb, UCharBuffer& buffer)
{
	buffer.add(isc_info_rsb_begin);

	buffer.add(isc_info_rsb_type);
	buffer.add(isc_info_rsb_hash);

	buffer.add(2);

	m_outer->dump(tdbb, buffer);
	m_inner->dump(tdbb, buffer);

	buffer.add(isc_info_rsb_end);
}

void HashJoin::markRecursive()
{
	m_outer->markRecursive();
	m_inner->markRecursive();
}

void HashJoin::findUsedStreams(StreamsArray& streams)
{
	m_outer->findUsedStreams(streams);
	m_inner->findUsedStreams(streams);
}

void HashJoin::invalidateRecords(jrd_req* request)
{
	m_outer->invalidateRecords(request);
	m_inner->invalidateRecords(request);
}

void HashJoin::nullRecords(thread_db* tdbb)
{
	m_outer->nullRecords(tdbb);
	m_inner->nullRecords(tdbb);
}

void HashJoin::saveRecords(thread_db* tdbb)
{
	m_outer->saveRecords(tdbb);
	m_inner->saveRecords(tdbb);
}

void HashJoin::restoreRecords(thread_db* tdbb)
{
	m_outer->restoreRecords(tdbb);
	m_inner->restoreRecords(tdbb);
}

USHORT HashJoin::hashKeys(thread_db* tdbb, jrd_req* request, bool outer)
{
	const jrd_nod* const list = outer ? m_outerKeys : m_innerKeys;

	ULONG hash_value = 0;

	for (size_t i = 0; i < list->nod_count; i++)
	{
		const dsc* const desc = EVL_expr(tdbb, list->nod_arg[i]);

		if (desc && !(request->req_flags & req_null))
		{
			USHORT length = desc->dsc_length;
			UCHAR* address = desc->dsc_address;

			if (desc->dsc_dtype == dtype_varying)
			{
				vary* const string = (vary*) address;
				length = string->vary_length;
				address = (UCHAR*) string->vary_string;
			}
			else if (desc->dsc_dtype == dtype_cstring)
			{
				length = strlen((char*) address);
			}

			UCHAR* p = NULL;
			const UCHAR* q = address;
			for (USHORT l = 0; l < length; l++)
			{
				if (!(l & 3))
				{
					p = (UCHAR*) &hash_value;
				}

				*p++ += *q++;
			}
		}
	}

	return (USHORT) (hash_value % HASH_SIZE);
}

bool HashJoin::compareKeys(thread_db* tdbb, jrd_req* request)
{
	for (size_t i = 0; i < m_outerKeys->nod_count; i++)
	{
		const dsc* const desc1 = EVL_expr(tdbb, m_outerKeys->nod_arg[i]);
		const bool null1 = (request->req_flags & req_null);

		const dsc* const desc2 = EVL_expr(tdbb, m_innerKeys->nod_arg[i]);
		const bool null2 = (request->req_flags & req_null);

		if (null1 != null2)
		{
			return false;
		}

		if (!null1 && !null2 && MOV_compare(desc1, desc2) != 0)
		{
			return false;
		}
	}

	return true;
}
