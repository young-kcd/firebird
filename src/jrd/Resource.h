/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		Resource.h
 *	DESCRIPTION:	Resource used by request / transaction
 *
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
 * 2001.07.28: Added rse_skip to class RecordSelExpr to support LIMIT.
 * 2002.09.28 Dmitry Yemanov: Reworked internal_info stuff, enhanced
 *                            exception handling in SPs/triggers,
 *                            implemented ROWS_AFFECTED system variable
 * 2002.10.21 Nickolay Samofatov: Added support for explicit pessimistic locks
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 * Adriano dos Santos Fernandes
 */

#ifndef JRD_RESOURCE_H
#define JRD_RESOURCE_H

#include "../jrd/MetaName.h"
#include "../common/classes/Bits.h"

namespace Jrd {

class jrd_rel;
class Routine;
class Collation;

struct Resource
{
	enum rsc_s : UCHAR
	{
		rsc_relation,
		rsc_index,
		rsc_collation,
		rsc_procedure,
		rsc_function,
		rsc_MAX
	};

	Resource(rsc_s type, USHORT id, jrd_rel* rel)
		: rsc_rel(rel), rsc_routine(nullptr), rsc_coll(nullptr),
		  rsc_id(id), rsc_type(type), rsc_state(State::Registered)
	{
		fb_assert(rsc_type == rsc_relation || rsc_type == rsc_index);
	}

	Resource(rsc_s type, USHORT id, Routine* routine)
		: rsc_rel(nullptr), rsc_routine(routine), rsc_coll(nullptr),
		  rsc_id(id), rsc_type(type), rsc_state(State::Registered)
	{
		fb_assert(rsc_type == rsc_procedure || rsc_type == rsc_function);
	}

	Resource(rsc_s type, USHORT id, Collation* coll)
		: rsc_rel(nullptr), rsc_routine(nullptr), rsc_coll(coll),
		  rsc_id(id), rsc_type(type), rsc_state(State::Registered)
	{
		fb_assert(rsc_type == rsc_collation);
	}

	Resource(rsc_s type)
		: rsc_rel(nullptr), rsc_routine(nullptr), rsc_coll(nullptr),
		  rsc_id(0), rsc_type(type), rsc_state(State::Registered)
	{ }

	static constexpr rsc_s next(rsc_s type)
	{
		fb_assert(type != rsc_MAX);
		return static_cast<rsc_s>(static_cast<int>(type) + 1);
	}

	// Resource state makes sense only for permanently (i.e. in some list) stored resource
	enum class State : UCHAR
	{
		Registered,
		Posted,
		Counted,
		Locked,
		Extra,
		Unlocking
	};

	jrd_rel*	rsc_rel;		// Relation block
	Routine*	rsc_routine;	// Routine block
	Collation*	rsc_coll;		// Collation block
	USHORT		rsc_id;			// Id of the resource
	rsc_s		rsc_type;		// Resource type
	State		rsc_state;		// What actions were taken with resource

	static bool greaterThan(const Resource& i1, const Resource& i2)
	{
		// A few places of the engine depend on fact that rsc_type
		// is the first field in ResourceList ordering
		if (i1.rsc_type != i2.rsc_type)
			return i1.rsc_type > i2.rsc_type;
		if (i1.rsc_type == rsc_index)
		{
			// Sort by relation ID for now
			if (i1.relId() != i2.relId())
				return i1.relId() > i2.relId();
		}
		return i1.rsc_id > i2.rsc_id;
	}

	bool operator>(const Resource& i2) const
	{
		return greaterThan(*this, i2);
	}

	USHORT relId() const;
};

} // namespace Jrd

#endif // JRD_RESOURCE_H

