/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		exe.h
 *	DESCRIPTION:	Execution block definitions
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

#ifndef JRD_EXE_H
#define JRD_EXE_H

#include "../jrd/blb.h"
#include "../jrd/Relation.h"
#include "../common/classes/array.h"
#include "../common/classes/MetaName.h"
#include "../common/classes/NestConst.h"

#include "gen/iberror.h"

#include "../jrd/dsc.h"
#include "../jrd/rse.h"

#include "../jrd/err_proto.h"
#include "../jrd/scl.h"
#include "../jrd/sbm.h"
#include "../jrd/sort.h"

#include "../jrd/DebugInterface.h"
#include "../jrd/BlrReader.h"
#include "../dsql/Nodes.h"
#include "../dsql/Visitors.h"

// This macro enables DSQL tracing code
//#define CMP_DEBUG

#ifdef CMP_DEBUG
DEFINE_TRACE_ROUTINE(cmp_trace);
#define CMP_TRACE(args) cmp_trace args
#else
#define CMP_TRACE(args) // nothing
#endif

class VaryingString;
struct dsc;

namespace Jrd {

#define NODE(type, name, keyword) type,

enum nod_t {
#include "../jrd/nod.h"
	nod_MAX
#undef NODE
};

class jrd_rel;
class jrd_nod;
class Sort;
struct sort_key_def;
template <typename T> class vec;
class jrd_prc;
class Collation;
struct index_desc;
struct IndexDescAlloc;
class Format;
class Cursor;
class PlanNode;
class RecordSource;

// NOTE: The definition of structures RecordSelExpr and lit must be defined in
//       exactly the same way as structure jrd_nod through item nod_count.
//       Now, inheritance takes care of those common data members.
class jrd_node_base : public pool_alloc_rpt<jrd_nod*, type_nod>
{
public:
	NestConst<jrd_nod>	nod_parent;
	ULONG		nod_impure;			// Inpure offset from request block
	nod_t		nod_type;			// Type of node
	USHORT		nod_flags;
	SCHAR		nod_scale;			// Target scale factor
	USHORT		nod_count;			// Number of arguments
};


class jrd_nod : public jrd_node_base
{
public:
	jrd_nod*	nod_arg[1];

	// Replace the line above by this block to check deep const-correctness.
	/***
	struct
	{
		NestConst<jrd_nod> arg[1];

		jrd_nod*& operator [](size_t index) { return *arg[index].getAddress(); }
		const jrd_nod* const& operator [](size_t index) const { return *arg[index].getAddress(); }

		operator jrd_nod** () { return arg[0].getAddress(); }
		operator const jrd_nod* const* () const { return arg[0].getAddress(); }
	} nod_arg;
	***/
};

const int nod_id			= 1;		// marks a field node as a blr_fid guy
const int nod_quad			= 2;		// compute in quad (default is long)
const int nod_double		= 4;
const int nod_date			= 8;
const int nod_value			= 16;		// full value area required in impure space
const int nod_agg_dbkey		= 32;		// dbkey of an aggregate
const int nod_invariant		= 64;		// node is recognized as being invariant

// Types of nulls placement for each column in sort order
const int rse_nulls_default	= 0;
const int rse_nulls_first	= 1;
const int rse_nulls_last	= 2;


// Literal value

class Literal : public jrd_node_base
{
public:
	dsc		lit_desc;
	SINT64	lit_data[1]; // Defined this way to prevent SIGBUS error in 64-bit ports
};

const size_t lit_delta	= ((sizeof(Literal) - sizeof(jrd_nod) - sizeof(SINT64)) / sizeof(jrd_nod**));


// Aggregate Sort Block (for DISTINCT aggregates)

class AggregateSort : protected Firebird::PermanentStorage
{
public:
	explicit AggregateSort(Firebird::MemoryPool& p)
		: PermanentStorage(p),
		  length(0),
		  intl(false),
		  impure(0),
		  keyItems(p)
	{
		desc.clear();
	}

	dsc desc;
	USHORT length;
	bool intl;
	ULONG impure;
	Firebird::HalfStaticArray<sort_key_def, 2> keyItems;
};

// Inversion (i.e. nod_index) impure area

struct impure_inversion
{
	RecordBitmap* inv_bitmap;
};


// AggregateSort impure area

struct impure_agg_sort
{
	Sort* iasb_sort;
};


// Various field positions

const int e_msg_number			= 0;
const int e_msg_format			= 1;
const int e_msg_impure_flags	= 2;
const int e_msg_length			= 3;

const int e_fld_stream		= 0;
const int e_fld_id			= 1;
const int e_fld_format		= 2;		// relation or procedure latest format when compiling
const int e_fld_default_value	= 3;	// hold column default value info if any, (Literal*)
const int e_fld_length		= 4;

const int e_sto_statement	= 0;
const int e_sto_statement2	= 1;
const int e_sto_sub_store	= 2;
const int e_sto_validate	= 3;
const int e_sto_relation	= 4;
const int e_sto_length		= 5;

const int e_erase_statement	= 0;
const int e_erase_sub_erase	= 1;
const int e_erase_stream	= 2;
const int e_erase_rsb		= 3;
const int e_erase_length	= 4;

const int e_mod_statement	= 0;
const int e_mod_statement2	= 1;
const int e_mod_sub_mod		= 2;
const int e_mod_validate	= 3;
const int e_mod_map_view	= 4;
const int e_mod_org_stream	= 5;
const int e_mod_new_stream	= 6;
const int e_mod_rsb			= 7;
const int e_mod_length		= 8;

const int e_send_statement	= 0;
const int e_send_message	= 1;
const int e_send_length		= 2;

const int e_asgn_from		= 0;
const int e_asgn_to			= 1;
const int e_asgn_missing	= 2;	// Value for comparison for missing
const int e_asgn_missing2	= 3;	// Value for substitute for missing
const int e_asgn_length		= 4;

const int e_lbl_statement	= 0;
const int e_lbl_label		= 1;
const int e_lbl_length		= 2;

const int e_val_boolean		= 0;
const int e_val_value		= 1;
const int e_val_length		= 2;

// Statistical expressions

const int e_stat_rse		= 0;
const int e_stat_value		= 1;
const int e_stat_default	= 2;
const int e_stat_rsb		= 3;
const int e_stat_length		= 4;

// Execute stored procedure

const int e_esp_inputs		= 0;
const int e_esp_in_msg		= 1;
const int e_esp_outputs		= 2;
const int e_esp_out_msg		= 3;
const int e_esp_procedure	= 4;
const int e_esp_length		= 5;

// Generate id

const int e_gen_value		= 0;
const int e_gen_id			= 1;	// Generator id (replaces e_gen_relation)
const int e_gen_length		= 2;

// Protection mask

const int e_pro_class		= 0;
const int e_pro_relation	= 1;
const int e_pro_length		= 2;

// Variable declaration

const int e_var_id			= 0;
const int e_var_variable	= 1;
const int e_var_info		= 2;
const int e_var_length		= 3;

const int e_dcl_id			= 0;
const int e_dcl_desc		= 1;
const int e_dcl_length		= (1 + sizeof (DSC) / sizeof(::Jrd::jrd_nod*));	// Room for descriptor

const int e_scl_field		= 0;	// Scalar expression (blr_index)
const int e_scl_subscripts	= 1;
const int e_scl_length		= 2;

const int e_blk_action		= 0;
const int e_blk_handlers	= 1;
const int e_blk_length		= 2;

const int e_err_action		= 0;
const int e_err_conditions	= 1;
const int e_err_length		= 2;

// Datatype cast operator

const int e_cast_source		= 0;
const int e_cast_fmt		= 1;
const int e_cast_iteminfo	= 2;
const int e_cast_length		= 3;

// SQL Date supporting nodes
const int e_extract_value	= 0;	// Node
const int e_extract_part	= 1;	// Integer
const int e_extract_count	= 1;	// Number of nodes
const int e_extract_length	= 2;	// Number of entries in nod_args

const int e_dcl_cur_rse		= 0;
const int e_dcl_cur_refs	= 1;
const int e_dcl_cur_number	= 2;
const int e_dcl_cur_cursor	= 3;
const int e_dcl_cur_length	= 4;

const int e_cursor_stmt_op			= 0;
const int e_cursor_stmt_number		= 1;
const int e_cursor_stmt_scroll_op	= 2;
const int e_cursor_stmt_scroll_val	= 3;
const int e_cursor_stmt_into		= 4;
const int e_cursor_stmt_length		= 5;

const int e_strlen_value	= 0;
const int e_strlen_type		= 1;
const int e_strlen_count	= 1;
const int e_strlen_length	= 2;

const int e_trim_value			= 0;
const int e_trim_characters		= 1;
const int e_trim_specification	= 2;
const int e_trim_count			= 2;
const int e_trim_length			= 3;

// nod_src_info
const int e_src_info_line		= 0;
const int e_src_info_col		= 1;
const int e_src_info_node		= 2;
const int e_src_info_length		= 3;

// nod_init_variable
const int e_init_var_id			= 0;
const int e_init_var_variable	= 1;
const int e_init_var_info		= 2;
const int e_init_var_length		= 3;

// nod_domain_validation
const int e_domval_desc			= 0;
const int e_domval_length		= sizeof (DSC) / sizeof(::Jrd::jrd_nod*);	// Room for descriptor

// nod_exec_stmt
const int e_exec_stmt_stmt_sql		= 0;
const int e_exec_stmt_data_src		= 1;
const int e_exec_stmt_user			= 2;
const int e_exec_stmt_password		= 3;
const int e_exec_stmt_role			= 4;
const int e_exec_stmt_proc_block	= 5;
const int e_exec_stmt_fixed_count	= 6;

const int e_exec_stmt_extra_inputs		= 0;
const int e_exec_stmt_extra_input_names	= 1;
const int e_exec_stmt_extra_outputs		= 2;
const int e_exec_stmt_extra_tran		= 3;
const int e_exec_stmt_extra_privs		= 4;
const int e_exec_stmt_extra_count		= 5;

// nod_stmt_expr
const int e_stmt_expr_stmt		= 0;
const int e_stmt_expr_expr		= 1;
const int e_stmt_expr_length	= 2;

// nod_derived_expr
const int e_derived_expr_expr			= 0;
const int e_derived_expr_stream_count	= 1;
const int e_derived_expr_stream_list	= 2;
const int e_derived_expr_count			= 1;
const int e_derived_expr_length			= 3;

// index (in nod_list) for external procedure blr
const int e_extproc_input_message	= 0;
const int e_extproc_output_message	= 1;
const int e_extproc_input_assign	= 2;
const int e_extproc_output_assign	= 3;

// Request resources

struct Resource
{
	enum rsc_s
	{
		rsc_relation,
		rsc_procedure,
		rsc_index,
		rsc_collation,
		rsc_function
	};

	rsc_s		rsc_type;
	USHORT		rsc_id;			// Id of the resource
	jrd_rel*	rsc_rel;		// Relation block
	jrd_prc*	rsc_prc;		// Procedure block
	Collation*	rsc_coll;		// Collation block
	Function*	rsc_fun;		// Function block

	static bool greaterThan(const Resource& i1, const Resource& i2)
	{
		// A few places of the engine depend on fact that rsc_type
		// is the first field in ResourceList ordering
		if (i1.rsc_type != i2.rsc_type)
			return i1.rsc_type > i2.rsc_type;
		if (i1.rsc_type == rsc_index)
		{
			// Sort by relation ID for now
			if (i1.rsc_rel->rel_id != i2.rsc_rel->rel_id)
				return i1.rsc_rel->rel_id > i2.rsc_rel->rel_id;
		}
		return i1.rsc_id > i2.rsc_id;
	}

	Resource(rsc_s type, USHORT id, jrd_rel* rel, jrd_prc* prc, Collation* coll)
		: rsc_type(type), rsc_id(id), rsc_rel(rel), rsc_prc(prc), rsc_coll(coll)
	{ }
};

typedef Firebird::SortedArray<Resource, Firebird::EmptyStorage<Resource>,
	Resource, Firebird::DefaultKeyValue<Resource>, Resource> ResourceList;

// Access items
// In case we start to use MetaName with required pool parameter,
// access item to be reworked!
// This struct seems better located in scl.h.

struct AccessItem
{
	Firebird::MetaName		acc_security_name;
	SLONG					acc_view_id;
	Firebird::MetaName		acc_name, acc_r_name;
	SLONG					acc_type;
	SecurityClass::flags_t	acc_mask;

	static bool greaterThan(const AccessItem& i1, const AccessItem& i2)
	{
		int v;

		/* CVC: Disabled this horrible hack.
		// Relations and procedures should be sorted before
		// columns, hence such a tricky inverted condition
		if ((v = -strcmp(i1.acc_type, i2.acc_type)) != 0)
			return v > 0;
		*/
		if (i1.acc_type != i2.acc_type)
			return i1.acc_type > i2.acc_type;

		if ((v = i1.acc_security_name.compare(i2.acc_security_name)) != 0)
			return v > 0;

		if (i1.acc_view_id != i2.acc_view_id)
			return i1.acc_view_id > i2.acc_view_id;

		if (i1.acc_mask != i2.acc_mask)
			return i1.acc_mask > i2.acc_mask;

		if ((v = i1.acc_name.compare(i2.acc_name)) != 0)
			return v > 0;

		if ((v = i1.acc_r_name.compare(i2.acc_r_name)) != 0)
			return v > 0;

		return false; // Equal
	}

	AccessItem(const Firebird::MetaName& security_name, SLONG view_id,
		const Firebird::MetaName& name, SLONG type,
		SecurityClass::flags_t mask, const Firebird::MetaName& relName)
		: acc_security_name(security_name), acc_view_id(view_id), acc_name(name),
			acc_r_name(relName), acc_type(type), acc_mask(mask)
	{}
};

typedef Firebird::SortedArray<AccessItem, Firebird::EmptyStorage<AccessItem>,
	AccessItem, Firebird::DefaultKeyValue<AccessItem>, AccessItem> AccessItemList;

// Triggers and procedures the request accesses
struct ExternalAccess
{
	enum exa_act
	{
		exa_procedure,
		exa_function,
		exa_insert,
		exa_update,
		exa_delete
	};
	exa_act exa_action;
	USHORT exa_prc_id;
	USHORT exa_fun_id;
	USHORT exa_rel_id;
	USHORT exa_view_id;

	// Procedure
	ExternalAccess(exa_act action, USHORT id) :
		exa_action(action),
		exa_prc_id(action == exa_procedure ? id : 0),
		exa_fun_id(action == exa_function ? id : 0),
		exa_rel_id(0), exa_view_id(0)
	{ }

	// Trigger
	ExternalAccess(exa_act action, USHORT rel_id, USHORT view_id) :
		exa_action(action), exa_prc_id(0), exa_fun_id(0),
		exa_rel_id(rel_id), exa_view_id(view_id)
	{ }

	static bool greaterThan(const ExternalAccess& i1, const ExternalAccess& i2)
	{
		if (i1.exa_action != i2.exa_action)
			return i1.exa_action > i2.exa_action;
		if (i1.exa_prc_id != i2.exa_prc_id)
			return i1.exa_prc_id > i2.exa_prc_id;
		if (i1.exa_fun_id != i2.exa_fun_id)
			return i1.exa_fun_id > i2.exa_fun_id;
		if (i1.exa_rel_id != i2.exa_rel_id)
			return i1.exa_rel_id > i2.exa_rel_id;
		if (i1.exa_view_id != i2.exa_view_id)
			return i1.exa_view_id > i2.exa_view_id;
		return false; // Equal
	}
};

typedef Firebird::SortedArray<ExternalAccess, Firebird::EmptyStorage<ExternalAccess>,
	ExternalAccess, Firebird::DefaultKeyValue<ExternalAccess>, ExternalAccess> ExternalAccessList;

// The three structs below are used for domains DEFAULT and constraints in PSQL
struct Item
{
	enum Type
	{
		TYPE_VARIABLE,
		TYPE_PARAMETER,
		TYPE_CAST
	};

	Item(Type aType, UCHAR aSubType, USHORT aIndex)
		: type(aType),
		  subType(aSubType),
		  index(aIndex)
	{
	}

	Item(Type aType, USHORT aIndex = 0)
		: type(aType),
		  subType(0),
		  index(aIndex)
	{
	}

	Type type;
	UCHAR subType;
	USHORT index;

	bool operator >(const Item& x) const
	{
		if (type == x.type)
		{
			if (subType == x.subType)
				return index > x.index;

			return subType > x.subType;
		}

		return type > x.type;
	}
};

struct FieldInfo
{
	FieldInfo()
		: nullable(false), defaultValue(NULL), validation(NULL)
	{}

	bool nullable;
	NestConst<jrd_nod> defaultValue;
	NestConst<jrd_nod> validation;
};

struct ItemInfo
{
	ItemInfo(MemoryPool& p, const ItemInfo& o)
		: name(p, o.name),
		  field(p, o.field),
		  nullable(o.nullable),
		  explicitCollation(o.explicitCollation),
		  fullDomain(o.fullDomain)
	{
	}

	explicit ItemInfo(MemoryPool& p)
		: name(p),
		  field(p),
		  nullable(true),
		  explicitCollation(false),
		  fullDomain(false)
	{
	}

	ItemInfo()
		: name(),
		  field(),
		  nullable(true),
		  explicitCollation(false),
		  fullDomain(false)
	{
	}

	bool isSpecial() const
	{
		return !nullable || fullDomain;
	}

	Firebird::MetaName name;
	Firebird::MetaNamePair field;
	bool nullable;
	bool explicitCollation;
	bool fullDomain;
};

struct LegacyNodeOrRseNode
{
	LegacyNodeOrRseNode(jrd_nod* aLegacyNode)
		: legacyNode(aLegacyNode),
		  boolExprNode(NULL),
		  rseNode(NULL)
	{
	}

	LegacyNodeOrRseNode(BoolExprNode* aBoolExprNode)
		: legacyNode(NULL),
		  boolExprNode(aBoolExprNode),
		  rseNode(NULL)
	{
	}

	LegacyNodeOrRseNode(RseNode* aRseNode)
		: legacyNode(NULL),
		  boolExprNode(NULL),
		  rseNode(aRseNode)
	{
	}

	jrd_nod* legacyNode;
	BoolExprNode* boolExprNode;
	RseNode* rseNode;
};

typedef Firebird::GenericMap<Firebird::Pair<Firebird::Left<Firebird::MetaNamePair, FieldInfo> > >
	MapFieldInfo;
typedef Firebird::GenericMap<Firebird::Pair<Firebird::Right<Item, ItemInfo> > > MapItemInfo;

// Compile scratch block

class CompilerScratch : public pool_alloc<type_csb>
{
	CompilerScratch(MemoryPool& p, size_t len, const Firebird::MetaName& domain_validation)
	:	/*csb_node(0),
		csb_variables(0),
		csb_dependencies(0),
		csb_count(0),
		csb_n_stream(0),
		csb_msg_number(0),
		csb_impure(0),
		csb_g_flags(0),*/
#ifdef CMP_DEBUG
		csb_dump(p),
#endif
		csb_external(p),
		csb_access(p),
		csb_resources(p),
		csb_dependencies(p),
		csb_fors(p),
		csb_exec_sta(p),
		csb_invariants(p),
		csb_current_nodes(p),
		csb_pool(p),
		csb_dbg_info(p),
		csb_map_field_info(p),
		csb_map_item_info(p),
		csb_domain_validation(domain_validation),
		csb_rpt(p, len)
	{}

public:
	struct Dependency
	{
		explicit Dependency(int aObjType)
		{
			memset(this, 0, sizeof(*this));
			objType = aObjType;
		}

		int objType;

		union
		{
			jrd_rel* relation;
			const Function* function;
			const jrd_prc* procedure;
			const Firebird::MetaName* name;
			SLONG number;
		};

		const Firebird::MetaName* subName;
		SLONG subNumber;
	};

	static CompilerScratch* newCsb(MemoryPool& p, size_t len,
								   const Firebird::MetaName& domain_validation = Firebird::MetaName())
	{
		return FB_NEW(p) CompilerScratch(p, len, domain_validation);
	}

	int nextStream(bool check = true)
	{
		if (csb_n_stream >= MAX_STREAMS && check)
		{
			ERR_post(Firebird::Arg::Gds(isc_too_many_contexts));
		}
		return csb_n_stream++;
	}

#ifdef CMP_DEBUG
	void dump(const char* format, ...)
	{
		va_list params;
		va_start(params, format);

		Firebird::string s;
		s.vprintf(format, params);

		va_end(params);

		csb_dump += s;
	}

	Firebird::string csb_dump;
#endif

	BlrReader		csb_blr_reader;
	jrd_nod*		csb_node;
	ExternalAccessList csb_external;			// Access to outside procedures/triggers to be checked
	AccessItemList	csb_access;					// Access items to be checked
	vec<jrd_nod*>*	csb_variables;				// Vector of variables, if any
	ResourceList	csb_resources;				// Resources (relations and indexes)
	Firebird::Array<Dependency>	csb_dependencies;	// objects that this statement depends upon
	Firebird::Array<const RecordSource*> csb_fors;	// record sources
	Firebird::Array<jrd_nod*> csb_exec_sta;		// Array of exec_into nodes
	Firebird::Array<ULONG*> csb_invariants;		// stack of pointer to nodes invariant offsets
	Firebird::Array<LegacyNodeOrRseNode> csb_current_nodes;	// RseNode's and other invariant
												// candidates within whose scope we are
	USHORT			csb_n_stream;				// Next available stream
	USHORT			csb_msg_number;				// Highest used message number
	ULONG			csb_impure;					// Next offset into impure area
	USHORT			csb_g_flags;
	MemoryPool&		csb_pool;					// Memory pool to be used by csb
	Firebird::DbgInfo	csb_dbg_info;			// Debug information
	MapFieldInfo		csb_map_field_info;		// Map field name to field info
	MapItemInfo			csb_map_item_info;		// Map item to item info
	Firebird::MetaName	csb_domain_validation;	// Parsing domain constraint in PSQL

	// used in cmp.cpp/pass1
	jrd_rel*	csb_view;
	USHORT		csb_view_stream;
	bool		csb_validate_expr;
	USHORT		csb_remap_variable;

	struct csb_repeat
	{
		// We must zero-initialize this one
		csb_repeat()
		:	csb_stream(0),
			csb_view_stream(0),
			csb_flags(0),
			csb_indices(0),
			csb_relation(0),
			csb_alias(0),
			csb_procedure(0),
			csb_view(0),
			csb_idx(0),
			csb_message(0),
			csb_format(0),
			csb_internal_format(0),
			csb_fields(0),
			csb_cardinality(0.0),	// TMN: Non-natural cardinality?!
			csb_plan(0),
			csb_map(0),
			csb_rsb_ptr(0)
		{}

		UCHAR csb_stream;				// Map user context to internal stream
		UCHAR csb_view_stream;			// stream number for view relation, below
		USHORT csb_flags;
		USHORT csb_indices;				// Number of indices

		jrd_rel* csb_relation;
		Firebird::string* csb_alias;	// SQL alias name for this instance of relation
		jrd_prc* csb_procedure;
		jrd_rel* csb_view;				// parent view

		IndexDescAlloc* csb_idx;		// Packed description of indices
		jrd_nod* csb_message;			// Msg for send/receive
		const Format* csb_format;		// Default Format for stream
		Format* csb_internal_format;	// Statement internal format
		UInt32Bitmap* csb_fields;		// Fields referenced
		double csb_cardinality;			// Cardinality of relation
		PlanNode* csb_plan;				// user-specified plan for this relation
		UCHAR* csb_map;					// Stream map for views
		RecordSource** csb_rsb_ptr;		// point to rsb for nod_stream
	};

	typedef csb_repeat* rpt_itr;
	typedef const csb_repeat* rpt_const_itr;
	Firebird::HalfStaticArray<csb_repeat, 5> csb_rpt;
};

const int csb_internal			= 1;	// "csb_g_flag" switch
const int csb_get_dependencies	= 2;	// we are retrieving dependencies
const int csb_ignore_perm		= 4;	// ignore permissions checks
const int csb_blr_version4		= 8;	// the BLR is of version 4
const int csb_pre_trigger		= 16;	// this is a BEFORE trigger
const int csb_post_trigger		= 32;	// this is an AFTER trigger
const int csb_validation		= 64;	// we're in a validation expression (RDB hack)
const int csb_reuse_context		= 128;	// allow context reusage

const int csb_active		= 1;		// stream is active
const int csb_used			= 2;		// context has already been defined (BLR parsing only)
const int csb_view_update	= 4;		// view update w/wo trigger is in progress
const int csb_trigger		= 8;		// NEW or OLD context in trigger
const int csb_no_dbkey		= 16;		// stream doesn't have a dbkey
const int csb_store			= 32;		// we are processing a store statement
const int csb_modify		= 64;		// we are processing a modify
const int csb_sub_stream	= 128;		// a sub-stream of the RSE being processed
const int csb_erase			= 256;		// we are processing an erase
const int csb_unmatched		= 512;		// stream has conjuncts unmatched by any index
const int csb_update		= 1024;		// erase or modify for relation

// Exception condition list

struct xcp_repeat
{
	SSHORT xcp_type;
	SLONG xcp_code;
};

class PsqlException : public pool_alloc_rpt<xcp_repeat, type_xcp>
{
public:
	SLONG xcp_count;
	xcp_repeat xcp_rpt[1];
};

const int xcp_sql_code	= 1;
const int xcp_gds_code	= 2;
const int xcp_xcp_code	= 3;
const int xcp_default	= 4;

class StatusXcp
{
	ISC_STATUS_ARRAY status;

public:
	StatusXcp();

	void clear();
	void init(const ISC_STATUS*);
	void copyTo(ISC_STATUS*) const;
	bool success() const;
	SLONG as_gdscode() const;
	SLONG as_sqlcode() const;
	void as_sqlstate(char*) const;
};

// must correspond to the size of RDB$EXCEPTIONS.RDB$MESSAGE
// minus size of vary::vary_length (USHORT) since RDB$MESSAGE
// declared as varchar
const int XCP_MESSAGE_LENGTH = 1023 - sizeof(USHORT);

} // namespace Jrd

#endif // JRD_EXE_H
