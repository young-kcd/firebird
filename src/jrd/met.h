/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		met.h
 *	DESCRIPTION:	Random meta-data stuff
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
 */

#ifndef JRD_MET_H
#define JRD_MET_H

#include "../jrd/Relation.h"
#include "../jrd/Function.h"

#include "../jrd/val.h"
#include "../jrd/irq.h"
#include "../jrd/drq.h"

#include "../jrd/HazardPtr.h"

// Record types for record summary blob records

enum rsr_t {
	RSR_field_id,
	RSR_field_name,
	RSR_view_context,
	RSR_base_field,
	RSR_computed_blr,
	RSR_missing_value,
	RSR_default_value,
	RSR_validation_blr,
	RSR_security_class,
	RSR_trigger_name,
	RSR_dimensions,
	RSR_array_desc,

	RSR_relation_id,			// The following are Gateway specific
	RSR_relation_name,			// and are used to speed the acquiring
	RSR_rel_sys_flag,			// of relation information
	RSR_view_blr,
	RSR_owner_name,
	RSR_field_type,				// The following are also Gateway
	RSR_field_scale,			// specific and relate to field info
	RSR_field_length,
	RSR_field_sub_type,
	RSR_field_not_null,
	RSR_field_generator_name,
	RSR_field_identity_type
};

// Temporary field block

class TemporaryField : public pool_alloc<type_tfb>
{
public:
	TemporaryField*	tfb_next;		// next block in chain
	USHORT			tfb_id;			// id of field in relation
	USHORT			tfb_flags;
	dsc				tfb_desc;
	Jrd::impure_value	tfb_default;
};

// tfb_flags

const int TFB_computed			= 1;
const int TFB_array				= 2;


const int TRIGGER_PRE_STORE		= 1;
const int TRIGGER_POST_STORE	= 2;
const int TRIGGER_PRE_MODIFY	= 3;
const int TRIGGER_POST_MODIFY	= 4;
const int TRIGGER_PRE_ERASE		= 5;
const int TRIGGER_POST_ERASE	= 6;
const int TRIGGER_MAX			= 7;

// trigger type prefixes
const int TRIGGER_PRE			= 0;
const int TRIGGER_POST			= 1;

// trigger type suffixes
const int TRIGGER_STORE			= 1;
const int TRIGGER_MODIFY		= 2;
const int TRIGGER_ERASE			= 3;

// that's how trigger action types are encoded
/*
	bit 0 = TRIGGER_PRE/TRIGGER_POST flag,
	bits 1-2 = TRIGGER_STORE/TRIGGER_MODIFY/TRIGGER_ERASE (slot #1),
	bits 3-4 = TRIGGER_STORE/TRIGGER_MODIFY/TRIGGER_ERASE (slot #2),
	bits 5-6 = TRIGGER_STORE/TRIGGER_MODIFY/TRIGGER_ERASE (slot #3),
	and finally the above calculated value is decremented

example #1:
	TRIGGER_POST_ERASE =
	= ((TRIGGER_ERASE << 1) | TRIGGER_POST) - 1 =
	= ((3 << 1) | 1) - 1 =
	= 0x00000110 (6)

example #2:
	TRIGGER_PRE_STORE_MODIFY =
	= ((TRIGGER_MODIFY << 3) | (TRIGGER_STORE << 1) | TRIGGER_PRE) - 1 =
	= ((2 << 3) | (1 << 1) | 0) - 1 =
	= 0x00010001 (17)

example #3:
	TRIGGER_POST_MODIFY_ERASE_STORE =
	= ((TRIGGER_STORE << 5) | (TRIGGER_ERASE << 3) | (TRIGGER_MODIFY << 1) | TRIGGER_POST) - 1 =
	= ((1 << 5) | (3 << 3) | (2 << 1) | 1) - 1 =
	= 0x00111100 (60)
*/

// that's how trigger types are decoded
#define TRIGGER_ACTION(value, shift) \
	(((((value + 1) >> shift) & 3) << 1) | ((value + 1) & 1)) - 1

#define TRIGGER_ACTION_SLOT(value, slot) \
	TRIGGER_ACTION(value, (slot * 2 - 1) )

const int TRIGGER_COMBINED_MAX	= 128;

#include "../jrd/exe_proto.h"
#include "../jrd/obj.h"
#include "../dsql/sym.h"

class CharSetContainer;

namespace Jrd {

// Procedure block

class jrd_prc : public Routine
{
public:
	const Format*	prc_record_format;
	prc_t			prc_type;					// procedure type

	const ExtEngineManager::Procedure* getExternal() const { return prc_external; }
	void setExternal(ExtEngineManager::Procedure* value) { prc_external = value; }

private:
	const ExtEngineManager::Procedure* prc_external;

public:
	explicit jrd_prc(MemoryPool& p)
		: Routine(p),
		  prc_record_format(NULL),
		  prc_type(prc_legacy),
		  prc_external(NULL)
	{
	}

public:
	virtual int getObjectType() const
	{
		return obj_procedure;
	}

	virtual SLONG getSclType() const
	{
		return obj_procedure;
	}

	virtual void releaseFormat()
	{
		delete prc_record_format;
		prc_record_format = NULL;
	}

	virtual ~jrd_prc()
	{
		delete prc_external;
	}

	virtual bool checkCache(thread_db* tdbb) const;
	virtual void clearCache(thread_db* tdbb);

	virtual void releaseExternal()
	{
		delete prc_external;
		prc_external = NULL;
	}

protected:
	virtual bool reload(thread_db* tdbb);	// impl is in met.epp
};


// Parameter block

class Parameter : public pool_alloc<type_prm>
{
public:
	USHORT		prm_number;
	dsc			prm_desc;
	NestConst<ValueExprNode>	prm_default_value;
	bool		prm_nullable;
	prm_mech_t	prm_mechanism;
	MetaName prm_name;			// asciiz name
	MetaName prm_field_source;
	FUN_T		prm_fun_mechanism;

public:
	explicit Parameter(MemoryPool& p)
		: prm_name(p),
		  prm_field_source(p)
	{
	}
};

struct index_desc;
struct DSqlCacheItem;

// index status
enum IndexStatus
{
	MET_object_active,
	MET_object_deferred_active,
	MET_object_inactive,
	MET_object_unknown
};

class MetadataCache : public Firebird::PermanentStorage
{
	class Generator : public HazardObject
	{
	public:
		typedef MetaName Key;

		Generator(MetaName name)
			: value(name)
		{ }

		Generator()
		{ }

		bool hasData() const
		{
			return value.hasData();
		}

		MetaName getKey() const
		{
			return value;
		}

	public:
		MetaName value;
	};


public:
	MetadataCache(MemoryPool& pool)
		: Firebird::PermanentStorage(pool),
		  mdc_relations(getPool()),
		  mdc_procedures(getPool()),
		  mdc_functions(getPool()),
		  mdc_generators(getPool()),
		  mdc_internal(getPool()),
		  mdc_dyn_req(getPool()),
		  mdc_charsets(getPool()),
		  mdc_charset_ids(getPool())
	{
		mdc_internal.grow(irq_MAX);
		mdc_dyn_req.grow(drq_MAX);
	}

	~MetadataCache();

	Request* findSystemRequest(thread_db* tdbb, USHORT id, InternalRequest which);

	void releaseIntlObjects(thread_db* tdbb);			// defined in intl.cpp
	void destroyIntlObjects(thread_db* tdbb);			// defined in intl.cpp

	void releaseRelations(thread_db* tdbb);
	void releaseLocks(thread_db* tdbb);
	void releaseGTTs(thread_db* tdbb);
	void runDBTriggers(thread_db* tdbb, TriggerAction action);
	void invalidateReplSet(thread_db* tdbb);
	HazardPtr<Function> lookupFunction(thread_db* tdbb, const QualifiedName& name, USHORT setBits, USHORT clearBits);
	HazardPtr<jrd_rel> getRelation(thread_db* tdbb, ULONG rel_id);
	HazardPtr<jrd_rel> getRelation(Attachment* att, ULONG rel_id);
	void setRelation(thread_db* tdbb, ULONG rel_id, jrd_rel* rel);
	USHORT relCount();
	void releaseTrigger(thread_db* tdbb, USHORT triggerId, const MetaName& name);
	TrigVectorPtr* getTriggers(USHORT triggerId);

	void cacheRequest(InternalRequest which, USHORT id, Statement* stmt)
	{
		if (which == IRQ_REQUESTS)
			mdc_internal[id] = stmt;
		else if (which == DYN_REQUESTS)
			mdc_dyn_req[id] = stmt;
		else
		{
			fb_assert(false);
		}
	}

	HazardPtr<Function> getFunction(thread_db* tdbb, USHORT id, bool grow = false)
	{
		HazardPtr<Function> rc(tdbb);

		if (id >= mdc_functions.getCount())
		{
			if (grow)
				mdc_functions.grow(id + 1);
		}
		else
			mdc_functions.load(id, rc);

		return rc;
	}

	HazardPtr<Function> setFunction(thread_db* tdbb, USHORT id, Function* f)
	{
		return mdc_functions.store(tdbb, id, f);
	}

	HazardPtr<jrd_prc> getProcedure(thread_db* tdbb, USHORT id, bool grow = false)
	{
		HazardPtr<jrd_prc> rc(tdbb);

		if (id >= mdc_procedures.getCount())
		{
			if (grow)
				mdc_procedures.grow(id + 1);
		}
		else
			mdc_procedures.load(id, rc);

		return rc;
	}

	void setProcedure(thread_db* tdbb, USHORT id, jrd_prc* p)
	{
		mdc_procedures.store(tdbb, id, p);
	}

	SLONG lookupSequence(thread_db* tdbb, const MetaName& name)
	{
		return mdc_generators.lookup(tdbb, name);
	}

	bool getSequence(thread_db* tdbb, SLONG id, MetaName& name)
	{
		HazardPtr<Generator> hp(tdbb);

		if (!mdc_generators.load(id, hp))
			return false;

		name = hp->value;
		return true;
	}

	void setSequence(thread_db* tdbb, SLONG id, MetaName name)
	{
		Generator* genObj = FB_NEW_POOL(getPool()) Generator(name);
		mdc_generators.store(tdbb, id, genObj);
	}

	CharSetContainer* getCharSet(USHORT id)
	{
		if (id >= mdc_charsets.getCount())
			return nullptr;
		return mdc_charsets[id];
	}

	void setCharSet(USHORT id, CharSetContainer* cs)
	{
		if (id >= mdc_charsets.getCount())
			mdc_charsets.grow(id + 10);

		mdc_charsets[id] = cs;
	}

	// former met_proto.h
#ifdef DEV_BUILD
	static void verify_cache(thread_db* tdbb);
#else
	static void verify_cache(thread_db* tdbb) { }
#endif
	static void clear_cache(thread_db* tdbb);
	static void update_partners(thread_db* tdbb);
	static bool routine_in_use(thread_db* tdbb, HazardPtr<Routine> routine);
	void load_db_triggers(thread_db* tdbb, int type);
	void load_ddl_triggers(thread_db* tdbb);
	static HazardPtr<jrd_prc> lookup_procedure(thread_db* tdbb, const QualifiedName& name, bool noscan);
	static HazardPtr<jrd_prc> lookup_procedure_id(thread_db* tdbb, USHORT id, bool return_deleted, bool noscan, USHORT flags);
	static HazardPtr<jrd_rel> lookup_relation(thread_db*, const MetaName&);
	static HazardPtr<jrd_rel> lookup_relation_id(thread_db*, SLONG, bool);
	static void lookup_index(thread_db* tdbb, MetaName& index_name, const MetaName& relation_name, USHORT number);
	static SLONG lookup_index_name(thread_db* tdbb, const MetaName& index_name,
								   SLONG* relation_id, IndexStatus* status);
	static void post_existence(thread_db* tdbb, jrd_rel* relation);
	static HazardPtr<jrd_prc> findProcedure(thread_db* tdbb, USHORT id, bool noscan, USHORT flags);
    static HazardPtr<jrd_rel> findRelation(thread_db* tdbb, USHORT id);
	static bool get_char_coll_subtype(thread_db* tdbb, USHORT* id, const UCHAR* name, USHORT length);
	static bool resolve_charset_and_collation(thread_db* tdbb, USHORT* id,
											  const UCHAR* charset, const UCHAR* collation);
	static DSqlCacheItem* get_dsql_cache_item(thread_db* tdbb, sym_type type, const QualifiedName& name);
	static void dsql_cache_release(thread_db* tdbb, sym_type type, const MetaName& name, const MetaName& package = "");
	static bool dsql_cache_use(thread_db* tdbb, sym_type type, const MetaName& name, const MetaName& package = "");
	// end of met_proto.h

private:
	HazardArray<jrd_rel>			mdc_relations;			// relations
	HazardArray<jrd_prc>			mdc_procedures;			// scanned procedures
	TrigVectorPtr					mdc_triggers[DB_TRIGGER_MAX];
	TrigVectorPtr					mdc_ddl_triggers;
	HazardArray<Function>			mdc_functions;			// User defined functions
	HazardArray<Generator>			mdc_generators;

	Firebird::Array<Statement*>	mdc_internal;				// internal statements
	Firebird::Array<Statement*>	mdc_dyn_req;				// internal dyn statements

	Firebird::Array<CharSetContainer*>	mdc_charsets;		// intl character set descriptions
	Firebird::GenericMap<Firebird::Pair<Firebird::Left<
		MetaName, USHORT> > > mdc_charset_ids;	// Character set ids

	Firebird::Mutex mdc_db_triggers_mutex;					// Also used for load DDL triggers
};

} // namespace Jrd

#endif // JRD_MET_H
