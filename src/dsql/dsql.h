/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		dsql.h
 *	DESCRIPTION:	General Definitions for V4 DSQL module
 *
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
 * 2001.11.26 Claudio Valderrama: include udf_arguments and udf_flags
 *   in the udf struct, so we can load the arguments and check for
 *   collisions between dropping and redefining the udf concurrently.
 *   This closes SF Bug# 409769.
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 * 2004.01.16 Vlad Horsun: added support for default parameters and
 *   EXECUTE BLOCK statement
 * Adriano dos Santos Fernandes
 */

#ifndef DSQL_DSQL_H
#define DSQL_DSQL_H

#include "../common/common.h"
#include "../jrd/RuntimeStatistics.h"
#include "../jrd/val.h"  // Get rid of duplicated FUN_T enum.
#include "../jrd/Database.h"
#include "../dsql/BlrWriter.h"
#include "../common/classes/array.h"
#include "../common/classes/GenericMap.h"
#include "../common/classes/MetaName.h"
#include "../common/classes/stack.h"
#include "../common/classes/auto.h"

#ifdef DEV_BUILD
// This macro enables DSQL tracing code
#define DSQL_DEBUG
#endif

#ifdef DSQL_DEBUG
DEFINE_TRACE_ROUTINE(dsql_trace);
#endif

// generic block used as header to all allocated structures
#include "../include/fb_blk.h"

#include "../dsql/sym.h"

// Context aliases used in triggers
const char* const OLD_CONTEXT		= "OLD";
const char* const NEW_CONTEXT		= "NEW";
const char* const TEMP_CONTEXT		= "TEMP";

namespace Jrd
{
	class Attachment;
	class Database;
	class DsqlCompilerScratch;
	class jrd_tra;
	class jrd_req;
	class blb;
	struct bid;

	class dsql_blb;
	class dsql_ctx;
	class dsql_msg;
	class dsql_par;
	class dsql_str;
	class dsql_map;
	class dsql_nod;
	class dsql_intlsym;

	typedef Firebird::Stack<dsql_ctx*> DsqlContextStack;
	typedef Firebird::Stack<const dsql_str*> DsqlStrStack;
	typedef Firebird::Stack<dsql_nod*> DsqlNodStack;
}

namespace Firebird
{
	class MetaName;
}

//======================================================================
// remaining node definitions for local processing
//

/// Include definition of descriptor

#include "../common/dsc.h"

namespace Jrd {

//! generic data type used to store strings
class dsql_str : public pool_alloc_rpt<char, dsql_type_str>
{
public:
	enum Type
	{
		TYPE_SIMPLE = 0,	// '...'
		TYPE_ALTERNATE,		// q'{...}'
		TYPE_HEXA,			// x'...'
		TYPE_DELIMITED		// "..."
	};

public:
	const char* str_charset;	// ASCIIZ Character set identifier for string
	Type	type;
	ULONG	str_length;		// length of string in BYTES
	char	str_data[2];	// one for ALLOC and one for the NULL
};

// blocks used to cache metadata

// Database Block
class dsql_dbb : public pool_alloc<dsql_type_dbb>
{
public:
	Firebird::GenericMap<Firebird::Pair<Firebird::Left<
		Firebird::MetaName, class dsql_rel*> > > dbb_relations;			// known relations in database
	Firebird::GenericMap<Firebird::Pair<Firebird::Left<
		Firebird::QualifiedName, class dsql_prc*> > > dbb_procedures;	// known procedures in database
	Firebird::GenericMap<Firebird::Pair<Firebird::Left<
		Firebird::QualifiedName, class dsql_udf*> > > dbb_functions;	// known functions in database
	Firebird::GenericMap<Firebird::Pair<Firebird::Left<
		Firebird::MetaName, class dsql_intlsym*> > > dbb_charsets;		// known charsets in database
	Firebird::GenericMap<Firebird::Pair<Firebird::Left<
		Firebird::MetaName, class dsql_intlsym*> > > dbb_collations;	// known collations in database
	Firebird::GenericMap<Firebird::Pair<Firebird::NonPooled<
		SSHORT, dsql_intlsym*> > > dbb_charsets_by_id;	// charsets sorted by charset_id
	Firebird::GenericMap<Firebird::Pair<Firebird::Left<
		Firebird::string, class dsql_req*> > > dbb_cursors;			// known cursors in database

	MemoryPool&		dbb_pool;			// The current pool for the dbb
	Database*		dbb_database;
	Attachment*		dbb_attachment;
	dsql_str*		dbb_dfl_charset;
	bool			dbb_no_charset;
	bool			dbb_read_only;
	USHORT			dbb_db_SQL_dialect;
	USHORT			dbb_ods_version;	// major ODS version number
	USHORT			dbb_minor_version;	// minor ODS version number

	explicit dsql_dbb(MemoryPool& p)
		: dbb_relations(p),
		  dbb_procedures(p),
		  dbb_functions(p),
		  dbb_charsets(p),
		  dbb_collations(p),
		  dbb_charsets_by_id(p),
		  dbb_cursors(p),
		  dbb_pool(p)
	{}

	~dsql_dbb();

	MemoryPool* createPool()
	{
		return dbb_database->createPool();
	}

	void deletePool(MemoryPool* pool)
	{
		dbb_database->deletePool(pool);
	}
};

//! Relation block
class dsql_rel : public pool_alloc<dsql_type_rel>
{
public:
	explicit dsql_rel(MemoryPool& p)
		: rel_name(p),
		  rel_owner(p)
	{
	}

	class dsql_fld*	rel_fields;		// Field block
	//dsql_rel*	rel_base_relation;	// base relation for an updatable view
	Firebird::MetaName rel_name;	// Name of relation
	Firebird::MetaName rel_owner;	// Owner of relation
	USHORT		rel_id;				// Relation id
	USHORT		rel_dbkey_length;
	USHORT		rel_flags;
};

// rel_flags bits
enum rel_flags_vals {
	REL_new_relation	= 1, // relation exists in sys tables, not committed yet
	REL_dropped			= 2, // relation has been dropped
	REL_view			= 4, // relation is a view
	REL_external		= 8, // relation is an external table
	REL_creating		= 16 // we are creating the bare relation in memory
};

class dsql_fld : public pool_alloc<dsql_type_fld>
{
public:
	explicit dsql_fld(MemoryPool& p)
		: fld_next(NULL),
		  fld_relation(NULL),
		  fld_procedure(NULL),
		  fld_ranges(NULL),
		  fld_character_set(NULL),
		  fld_sub_type_name(NULL),
		  fld_flags(0),
		  fld_id(0),
		  fld_dtype(0),
		  fld_length(0),
		  fld_element_dtype(0),
		  fld_element_length(0),
		  fld_scale(0),
		  fld_sub_type(0),
		  fld_precision(0),
		  fld_character_length(0),
		  fld_seg_length(0),
		  fld_dimensions(0),
		  fld_character_set_id(0),
		  fld_collation_id(0),
		  fld_ttype(0),
		  fld_type_of_name(p),
		  fld_type_of_table(NULL),
		  fld_explicit_collation(false),
		  fld_not_nullable(false),
		  fld_full_domain(false),
		  fld_name(p),
		  fld_source(p)
	{
	}

	dsql_fld*	fld_next;				// Next field in relation
	dsql_rel*	fld_relation;			// Parent relation
	dsql_prc*	fld_procedure;			// Parent procedure
	dsql_nod*	fld_ranges;				// ranges for multi dimension array
	dsql_nod*	fld_character_set;		// null means not specified
	dsql_nod*	fld_sub_type_name;		// Subtype name for later resolution
	USHORT		fld_flags;
	USHORT		fld_id;					// Field in in database
	USHORT		fld_dtype;				// Data type of field
	FLD_LENGTH	fld_length;				// Length of field
	USHORT		fld_element_dtype;		// Data type of array element
	USHORT		fld_element_length;		// Length of array element
	SSHORT		fld_scale;				// Scale factor of field
	SSHORT		fld_sub_type;			// Subtype for text & blob fields
	USHORT		fld_precision;			// Precision for exact numeric types
	USHORT		fld_character_length;	// length of field in characters
	USHORT		fld_seg_length;			// Segment length for blobs
	SSHORT		fld_dimensions;			// Non-zero means array
	SSHORT		fld_character_set_id;	// ID of field's character set
	SSHORT		fld_collation_id;		// ID of field's collation
	SSHORT		fld_ttype;				// ID of field's language_driver
	Firebird::string fld_type_of_name;	// TYPE OF
	dsql_str*	fld_type_of_table;		// TYPE OF table name
	bool		fld_explicit_collation;	// COLLATE was explicit specified
	bool		fld_not_nullable;		// NOT NULL was explicit specified
	bool		fld_full_domain;		// Domain name without TYPE OF prefix
	Firebird::string fld_name;
	Firebird::MetaName fld_source;
};

// values used in fld_flags

enum fld_flags_vals {
	FLD_computed	= 1,
	FLD_national	= 2, // field uses NATIONAL character set
	FLD_nullable	= 4,
	FLD_system		= 8
};

//! database/log/cache file block
class dsql_fil : public pool_alloc<dsql_type_fil>
{
public:
	SLONG	fil_length;			// File length in pages
	SLONG	fil_start;			// Starting page
	dsql_str*	fil_name;			// File name
	//dsql_fil*	fil_next;			// next file
	//SSHORT	fil_shadow_number;	// shadow number if part of shadow
	//SSHORT	fil_manual;			// flag to indicate manual shadow
	//SSHORT	fil_partitions;		// number of log file partitions
	//USHORT	fil_flags;
};

//! Stored Procedure block
class dsql_prc : public pool_alloc<dsql_type_prc>
{
public:
	explicit dsql_prc(MemoryPool& p)
		: prc_name(p),
		  prc_owner(p)
	{
	}

	dsql_fld*	prc_inputs;		// Input parameters
	dsql_fld*	prc_outputs;	// Output parameters
	Firebird::QualifiedName prc_name;	// Name of procedure
	Firebird::MetaName prc_owner;	// Owner of procedure
	SSHORT		prc_in_count;
	SSHORT		prc_def_count;	// number of inputs with default values
	SSHORT		prc_out_count;
	USHORT		prc_id;			// Procedure id
	USHORT		prc_flags;
	bool		prc_private;	// Packaged private procedure
};

// prc_flags bits

enum prc_flags_vals {
	PRC_new_procedure	= 1, // procedure is newly defined, not committed yet
	PRC_dropped			= 2  // procedure has been dropped
};

//! User defined function block
class dsql_udf : public pool_alloc<dsql_type_udf>
{
public:
	explicit dsql_udf(MemoryPool& p)
		: udf_name(p), udf_arguments(p)
	{
	}

	USHORT		udf_dtype;
	SSHORT		udf_scale;
	SSHORT		udf_sub_type;
	USHORT		udf_length;
	SSHORT		udf_character_set_id;
	//USHORT		udf_character_length;
    USHORT      udf_flags;
	Firebird::QualifiedName udf_name;
	Firebird::Array<dsc> udf_arguments;
	bool		udf_private;	// Packaged private function
};

// udf_flags bits

enum udf_flags_vals {
	UDF_new_udf		= 1, // udf is newly declared, not committed yet
	UDF_dropped		= 2  // udf has been dropped
};

// Variables - input, output & local

//! Variable block
class dsql_var : public Firebird::PermanentStorage
{
public:
	enum Type
	{
		TYPE_INPUT,
		TYPE_OUTPUT,
		TYPE_LOCAL,
		TYPE_HIDDEN
	};

public:
	explicit dsql_var(MemoryPool& p)
		: PermanentStorage(p),
		  name(p),
		  field(NULL),
		  type(TYPE_INPUT),
		  msgNumber(0),
		  msgItem(0),
		  number(0)
	{
		desc.clear();
	}

	Firebird::MetaName name;
	dsql_fld* field;	// Field on which variable is based
	Type type;			// Input, output, local or hidden variable
	USHORT msgNumber;	// Message number containing variable
	USHORT msgItem;		// Item number in message
	USHORT number;		// Local variable number
	dsc desc;
};


// Symbolic names for international text types
// (either collation or character set name)

//! International symbol
class dsql_intlsym : public pool_alloc<dsql_type_intlsym>
{
public:
	explicit dsql_intlsym(MemoryPool& p)
		: intlsym_name(p)
	{
	}

	Firebird::MetaName intlsym_name;
	USHORT		intlsym_type;		// what type of name
	USHORT		intlsym_flags;
	SSHORT		intlsym_ttype;		// id of implementation
	SSHORT		intlsym_charset_id;
	SSHORT		intlsym_collate_id;
	USHORT		intlsym_bytes_per_char;
};

// values used in intlsym_flags

enum intlsym_flags_vals {
	INTLSYM_dropped	= 1  // intlsym has been dropped
};


// Compiled statement - shared by multiple requests.
class DsqlCompiledStatement : public Firebird::PermanentStorage
{
public:
	enum Type	// statement type
	{
		TYPE_SELECT, TYPE_SELECT_UPD, TYPE_INSERT, TYPE_DELETE, TYPE_UPDATE, TYPE_UPDATE_CURSOR,
		TYPE_DELETE_CURSOR, TYPE_COMMIT, TYPE_ROLLBACK, TYPE_CREATE_DB, TYPE_DDL, TYPE_START_TRANS,
		TYPE_GET_SEGMENT, TYPE_PUT_SEGMENT, TYPE_EXEC_PROCEDURE, TYPE_COMMIT_RETAIN,
		TYPE_ROLLBACK_RETAIN, TYPE_SET_GENERATOR, TYPE_SAVEPOINT, TYPE_EXEC_BLOCK, TYPE_SELECT_BLOCK
	};

	// Statement flags.
	static const unsigned FLAG_ORPHAN		= 0x01;
	static const unsigned FLAG_NO_BATCH		= 0x02;
	static const unsigned FLAG_BLR_VERSION4	= 0x04;
	static const unsigned FLAG_BLR_VERSION5	= 0x08;
	static const unsigned FLAG_SELECTABLE	= 0x10;

public:
	explicit DsqlCompiledStatement(MemoryPool& p)
		: PermanentStorage(p),
		  type(TYPE_SELECT),
		  flags(0),
		  ddlData(p),
		  ddlNode(NULL),
		  blob(NULL),
		  sendMsg(NULL),
		  receiveMsg(NULL),
		  eof(NULL),
		  dbKey(NULL),
		  recVersion(NULL),
		  parentRecVersion(NULL),
		  parentDbKey(NULL),
		  parentRequest(NULL),
		  ddlScratch(NULL)
	{
	}

public:
	MemoryPool& getPool() { return PermanentStorage::getPool(); }

	Type getType() const { return type; }
	void setType(Type value) { type = value; }

	ULONG getFlags() const { return flags; }
	void setFlags(ULONG value) { flags = value; }
	void addFlags(ULONG value) { flags |= value; }

	Firebird::RefStrPtr& getSqlText() { return sqlText; }
	const Firebird::RefStrPtr& getSqlText() const { return sqlText; }
	void setSqlText(Firebird::RefString* value) { sqlText = value; }

	Firebird::HalfStaticArray<UCHAR, 1024>& getDdlData() { return ddlData; }
	const Firebird::HalfStaticArray<UCHAR, 1024>& getDdlData() const { return ddlData; }
	void setDdlData(Firebird::HalfStaticArray<UCHAR, 1024>& value) { ddlData = value; }

	dsql_nod* getDdlNode() { return ddlNode; }
	const dsql_nod* getDdlNode() const { return ddlNode; }
	void setDdlNode(dsql_nod* value) { ddlNode = value; }

	dsql_blb* getBlob() { return blob; }
	const dsql_blb* getBlob() const { return blob; }
	void setBlob(dsql_blb* value) { blob = value; }

	dsql_msg* getSendMsg() { return sendMsg; }
	const dsql_msg* getSendMsg() const { return sendMsg; }
	void setSendMsg(dsql_msg* value) { sendMsg = value; }

	dsql_msg* getReceiveMsg() { return receiveMsg; }
	const dsql_msg* getReceiveMsg() const { return receiveMsg; }
	void setReceiveMsg(dsql_msg* value) { receiveMsg = value; }

	dsql_par* getEof() { return eof; }
	const dsql_par* getEof() const { return eof; }
	void setEof(dsql_par* value) { eof = value; }

	dsql_par* getDbKey() { return dbKey; }
	const dsql_par* getDbKey() const { return dbKey; }
	void setDbKey(dsql_par* value) { dbKey = value; }

	dsql_par* getRecVersion() { return recVersion; }
	const dsql_par* getRecVersion() const { return recVersion; }
	void setRecVersion(dsql_par* value) { recVersion = value; }

	dsql_par* getParentRecVersion() { return parentRecVersion; }
	const dsql_par* getParentRecVersion() const { return parentRecVersion; }
	void setParentRecVersion(dsql_par* value) { parentRecVersion = value; }

	dsql_par* getParentDbKey() { return parentDbKey; }
	const dsql_par* getParentDbKey() const { return parentDbKey; }
	void setParentDbKey(dsql_par* value) { parentDbKey = value; }

	dsql_req* getParentRequest() const { return parentRequest; }
	void setParentRequest(dsql_req* value) { parentRequest = value; }

	DsqlCompilerScratch* getDdlScratch() const { return ddlScratch; }
	void setDdlScratch(DsqlCompilerScratch* value) { ddlScratch = value; }

private:
	Type type;					// Type of statement
	ULONG flags;				// generic flag
	Firebird::RefStrPtr sqlText;
	Firebird::HalfStaticArray<UCHAR, 1024> ddlData;
	dsql_nod* ddlNode;			// Store metadata statement
	dsql_blb* blob;				// Blob info for blob statements
	dsql_msg* sendMsg;			// Message to be sent to start request
	dsql_msg* receiveMsg;		// Per record message to be received
	dsql_par* eof;				// End of file parameter
	dsql_par* dbKey;			// Database key for current of
	dsql_par* recVersion;		// Record Version for current of
	dsql_par* parentRecVersion;	// parent record version
	dsql_par* parentDbKey;		// Parent database key for current of
	dsql_req* parentRequest;	// Source request, if cursor update
	DsqlCompilerScratch* ddlScratch;	// DSQL scratch for DDL statements
};


class dsql_req : public Firebird::StdIface<Firebird::IStatement, FB_I_STATEMENT_VERSION, pool_alloc<dsql_type_req> >
{
public:
	static const unsigned FLAG_OPENED_CURSOR	= 0x01;
	static const unsigned FLAG_EMBEDDED			= 0x02;

public:
	explicit dsql_req(DsqlCompiledStatement* aStatement)
		: req_pool(aStatement->getPool()),
		  statement(aStatement),
		  cursors(req_pool),
		  req_msg_buffers(req_pool),
		  req_cursor(req_pool),
		  req_user_descs(req_pool)
	{
	}

public:
	MemoryPool& getPool()
	{
		return req_pool;
	}

	jrd_tra* getTransaction()
	{
		return req_transaction;
	}

	const DsqlCompiledStatement* getStatement() const
	{
		return statement;
	}

	static void destroy(thread_db* tdbb, dsql_req* request, bool drop);

private:
	MemoryPool&	req_pool;
	const DsqlCompiledStatement* statement;

public:
	Firebird::Array<DsqlCompiledStatement*> cursors;	// Cursor update statements

	dsql_dbb* req_dbb;			// DSQL attachment
	jrd_tra* req_transaction;	// JRD transaction
	jrd_req* req_request;		// JRD request

	unsigned req_flags;			// flags

	Firebird::Array<UCHAR*>	req_msg_buffers;
	Firebird::string req_cursor;	// Cursor name, if any
	blb* req_blb;			// JRD blob
	Firebird::GenericMap<Firebird::NonPooled<const dsql_par*, dsc> > req_user_descs; // SQLDA data type

	Firebird::AutoPtr<Jrd::RuntimeStatistics> req_fetch_baseline; // State of request performance counters when we reported it last time
	SINT64 req_fetch_elapsed;		// Number of clock ticks spent while fetching rows for this request since we reported it last time
	SINT64 req_fetch_rowcount;		// Total number of rows returned by this request
	bool req_traced;				// request is traced via TraceAPI

protected:
	// Request should never be destroyed using delete.
	// It dies together with it's pool in release_request().
	~dsql_req();

	// To avoid posix warning about missing public destructor declare
	// MemoryPool as friend class. In fact IT releases request memory!
	friend class Firebird::MemoryPool;

public:
	virtual int FB_CARG release();
	virtual Firebird::IStatement* FB_CARG prepare(Status* status, Firebird::ITransaction* tra,
									  unsigned int stmtLength, const char* sqlStmt, unsigned int dialect,
									  unsigned int item_length, const unsigned char* items,
	    		                      unsigned int buffer_length, unsigned char* buffer);
	virtual void FB_CARG getInfo(Status* status,
						 unsigned int itemsLength, const unsigned char* items,
						 unsigned int bufferLength, unsigned char* buffer);
	virtual void FB_CARG setCursor(Status* status, const char* name, unsigned int type);
	virtual Firebird::ITransaction* FB_CARG execute(Status* status, Firebird::ITransaction* tra,
										unsigned int in_msg_type,
										const Firebird::MessageBuffer* inMsgBuffer,
										const Firebird::MessageBuffer* outMsgBuffer);
	virtual int FB_CARG fetch(Status* status, const Firebird::MessageBuffer* msgBuffer);	// returns 100 if EOF, 101 if fragmented
	virtual void FB_CARG insert(Status* status, const Firebird::MessageBuffer* msgBuffer);
	virtual void FB_CARG free(Status* status, unsigned int option);
};

// Blob
class dsql_blb : public pool_alloc<dsql_type_blb>
{
public:
	dsql_par*	blb_blob_id;		// Parameter to hold blob id
	dsql_par*	blb_segment;		// Parameter for segments
	dsql_nod*	blb_from;
	dsql_nod*	blb_to;
	dsql_msg*	blb_open_in_msg;	// Input message to open cursor
	dsql_msg*	blb_open_out_msg;	// Output message from open cursor
	dsql_msg*	blb_segment_msg;	// Segment message
};

//! Implicit (NATURAL and USING) joins
class ImplicitJoin : public pool_alloc<dsql_type_imp_join>
{
public:
	dsql_nod* value;
	dsql_ctx* visibleInContext;
};

struct PartitionMap
{
	PartitionMap(dsql_nod* aPartition, dsql_nod* aOrder)
		: partition(aPartition),
		  partitionRemapped(NULL),
		  order(aOrder),
		  map(NULL),
		  context(0)
	{
	}

	dsql_nod* partition;
	dsql_nod* partitionRemapped;
	dsql_nod* order;
	dsql_map* map;
	USHORT context;
};

//! Context block used to create an instance of a relation reference
class dsql_ctx : public pool_alloc<dsql_type_ctx>
{
public:
	explicit dsql_ctx(MemoryPool& p)
		: ctx_alias(p),
		  ctx_internal_alias(p),
		  ctx_main_derived_contexts(p),
		  ctx_childs_derived_table(p),
	      ctx_imp_join(p),
	      ctx_win_maps(p)
	{
	}

	dsql_rel*			ctx_relation;		// Relation for context
	dsql_prc*			ctx_procedure;		// Procedure for context
	dsql_nod*			ctx_proc_inputs;	// Procedure input parameters
	dsql_map*			ctx_map;			// Maps for aggregates and unions
	dsql_nod*			ctx_rse;			// Sub-rse for aggregates
	dsql_ctx*			ctx_parent;			// Parent context for aggregates
	USHORT				ctx_context;		// Context id
	USHORT				ctx_recursive;		// Secondary context id for recursive UNION (nobody referred to this context)
	USHORT				ctx_scope_level;	// Subquery level within this request
	USHORT				ctx_flags;			// Various flag values
	USHORT				ctx_in_outer_join;	// inOuterJoin when context was created
	Firebird::string	ctx_alias;			// Context alias (can include concatenated derived table alias)
	Firebird::string	ctx_internal_alias;	// Alias as specified in query
	DsqlContextStack	ctx_main_derived_contexts;	// contexts used for blr_derived_expr
	DsqlContextStack	ctx_childs_derived_table;	// Childs derived table context
	Firebird::GenericMap<Firebird::Pair<Firebird::Left<
		Firebird::MetaName, ImplicitJoin*> > > ctx_imp_join;	// Map of USING fieldname to ImplicitJoin
	Firebird::Array<PartitionMap*> ctx_win_maps;	// Maps for window functions

	dsql_ctx& operator=(dsql_ctx& v)
	{
		ctx_relation = v.ctx_relation;
		ctx_procedure = v.ctx_procedure;
		ctx_proc_inputs = v.ctx_proc_inputs;
		ctx_map = v.ctx_map;
		ctx_rse = v.ctx_rse;
		ctx_parent = v.ctx_parent;
		ctx_alias = v.ctx_alias;
		ctx_context = v.ctx_context;
		ctx_recursive = v.ctx_recursive;
		ctx_scope_level = v.ctx_scope_level;
		ctx_flags = v.ctx_flags;
		ctx_in_outer_join = v.ctx_in_outer_join;
		ctx_main_derived_contexts.assign(v.ctx_main_derived_contexts);
		ctx_childs_derived_table.assign(v.ctx_childs_derived_table);
		ctx_imp_join.assign(v.ctx_imp_join);
		ctx_win_maps.assign(v.ctx_win_maps);

		return *this;
	}

	bool getImplicitJoinField(const Firebird::MetaName& name, dsql_nod*& node);
	PartitionMap* getPartitionMap(DsqlCompilerScratch* dsqlScratch, dsql_nod* partitionNode,
		dsql_nod* orderNode);
};

// Flag values for ctx_flags

const USHORT CTX_outer_join = 0x01;	// reference is part of an outer join
const USHORT CTX_system		= 0x02;	// Context generated by system (NEW/OLD in triggers, check-constraint, RETURNING)
const USHORT CTX_null		= 0x04;	// Fields of the context should be resolved to NULL constant
const USHORT CTX_returning	= 0x08;	// Context generated by RETURNING
const USHORT CTX_recursive	= 0x10;	// Context has secondary number (ctx_recursive) generated for recursive UNION

//! Aggregate/union map block to map virtual fields to their base
//! TMN: NOTE! This datatype should definitely be renamed!
class dsql_map : public pool_alloc<dsql_type_map>
{
public:
	dsql_map* map_next;				// Next map in item
	dsql_nod* map_node;				// Value for map item
	USHORT map_position;			// Position in map
	PartitionMap* map_partition;	// Partition
};

// Message block used in communicating with a running request
class dsql_msg : public Firebird::PermanentStorage
{
public:
	explicit dsql_msg(MemoryPool& p)
		: PermanentStorage(p),
		  msg_parameters(p),
		  msg_number(0),
		  msg_buffer_number(0),
		  msg_length(0),
		  msg_parameter(0),
		  msg_index(0)
	{
	}

	Firebird::Array<dsql_par*> msg_parameters;	// Parameter list
	USHORT		msg_number;		// Message number
	USHORT		msg_buffer_number;	// Message buffer number (used instead of msg_number for blob msgs)
	ULONG		msg_length;		// Message length
	USHORT		msg_parameter;	// Next parameter number
	USHORT		msg_index;		// Next index into SQLDA
};

// Parameter block used to describe a parameter of a message
class dsql_par : public Firebird::PermanentStorage
{
public:
	explicit dsql_par(MemoryPool& p)
		: PermanentStorage(p),
		  par_message(NULL),
		  par_null(NULL),
		  par_node(NULL),
		  par_dbkey_relname(p),
		  par_rec_version_relname(p),
		  par_name(p),
		  par_rel_name(p),
		  par_owner_name(p),
		  par_rel_alias(p),
		  par_alias(p),
		  par_parameter(0),
		  par_index(0)
	{
		par_desc.clear();
	}

	dsql_msg*	par_message;		// Parent message
	dsql_par*	par_null;			// Null parameter, if used
	dsql_nod*	par_node;			// Associated value node, if any
	Firebird::MetaName par_dbkey_relname;		// Context of internally requested dbkey
	Firebird::MetaName par_rec_version_relname;	// Context of internally requested rec. version
	Firebird::MetaName par_name;				// Parameter name, if any
	Firebird::MetaName par_rel_name;			// Relation name, if any
	Firebird::MetaName par_owner_name;			// Owner name, if any
	Firebird::MetaName par_rel_alias;			// Relation alias, if any
	Firebird::MetaName par_alias;				// Alias, if any
	dsc			par_desc;			// Field data type
	USHORT		par_parameter;		// BLR parameter number
	USHORT		par_index;			// Index into SQLDA, if appropriate
};

// CVC: Enumeration used for the COMMENT command.
enum
{
	ddl_database, ddl_domain, ddl_relation, ddl_view, ddl_procedure, ddl_trigger,
	ddl_udf, ddl_blob_filter, ddl_exception, ddl_generator, ddl_index, ddl_role,
	ddl_charset, ddl_collation, ddl_package, ddl_schema,
	//, ddl_sec_class
};

class CStrCmp
{
public:
	static int greaterThan(const char* s1, const char* s2)
	{
		return strcmp(s1, s2) > 0;
	}
};

typedef Firebird::SortedArray<const char*,
			Firebird::EmptyStorage<const char*>, const char*,
			Firebird::DefaultKeyValue<const char*>,
			CStrCmp>
		StrArray;

} // namespace

/*! \var unsigned DSQL_debug
    \brief Debug level

    0       No output
    1       Display output tree in PASS1_statment
    2       Display input tree in PASS1_statment
    4       Display ddl BLR
    8       Display BLR
    16      Display PASS1_rse input tree
    32      Display SQL input string
    64      Display BLR in dsql/prepare
    > 256   Display yacc parser output level = DSQL_level>>8
*/

#ifdef DSQL_DEBUG
extern unsigned DSQL_debug;
#endif

#endif // DSQL_DSQL_H
