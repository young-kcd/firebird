/*
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
 * Adriano dos Santos Fernandes
 */

#ifndef DSQL_COMPILER_SCRATCH_H
#define DSQL_COMPILER_SCRATCH_H

#include "../common/common.h"
#include "../jrd/jrd.h"
#include "../dsql/dsql.h"
#include "../dsql/BlrWriter.h"
#include "../common/classes/array.h"
#include "../common/classes/MetaName.h"
#include "../common/classes/stack.h"
#include "../common/classes/alloc.h"

namespace Jrd
{

class TypeClause;
class VariableNode;


// DSQL Compiler scratch block - may be discarded after compilation in the future.
class DsqlCompilerScratch : public BlrWriter
{
public:
	static const unsigned FLAG_IN_AUTO_TRANS_BLOCK	= 0x001;
	static const unsigned FLAG_RETURNING_INTO		= 0x002;
	static const unsigned FLAG_METADATA_SAVED		= 0x004;
	static const unsigned FLAG_PROCEDURE			= 0x008;
	static const unsigned FLAG_TRIGGER				= 0x010;
	static const unsigned FLAG_BLOCK				= 0x020;
	static const unsigned FLAG_RECURSIVE_CTE		= 0x040;
	static const unsigned FLAG_UPDATE_OR_INSERT		= 0x080;
	static const unsigned FLAG_MERGE				= 0x100;
	static const unsigned FLAG_FUNCTION				= 0x200;

public:
	DsqlCompilerScratch(MemoryPool& p, dsql_dbb* aDbb, jrd_tra* aTransaction,
				DsqlCompiledStatement* aStatement)
		: BlrWriter(p),
		  dbb(aDbb),
		  transaction(aTransaction),
		  statement(aStatement),
		  flags(0),
		  ports(p),
		  relation(NULL),
		  procedure(NULL),
		  mainContext(p),
		  context(&mainContext),
		  unionContext(p),
		  derivedContext(p),
		  outerAggContext(NULL),
		  contextNumber(0),
		  derivedContextNumber(0),
		  scopeLevel(0),
		  loopLevel(0),
		  labels(p),
		  cursorNumber(0),
		  cursors(p),
		  inSelectList(0),
		  inWhereClause(0),
		  inGroupByClause(0),
		  inHavingClause(0),
		  inOrderByClause(0),
		  errorHandlers(0),
		  clientDialect(0),
		  inOuterJoin(0),
		  aliasRelationPrefix(NULL),
		  hiddenVars(p),
		  hiddenVarsNumber(0),
		  package(p),
		  currCtes(p),
		  recursiveCtx(0),
		  recursiveCtxId(0),
		  processingWindow(false),
		  checkConstraintTrigger(false),
		  variables(p),
		  outputVariables(p),
		  ctes(p),
		  cteAliases(p),
		  currCteAlias(NULL),
		  psql(false)
	{
		domainValue.clear();
	}

protected:
	// DsqlCompilerScratch should never be destroyed using delete.
	// It dies together with it's pool in release_request().
	~DsqlCompilerScratch()
	{
	}

	virtual bool isDdlDyn()
	{
		return (statement->getType() == DsqlCompiledStatement::TYPE_DDL || statement->getDdlNode()) &&
			!(flags & FLAG_BLOCK);
	}

public:
	virtual bool isVersion4()
	{
		return statement->getFlags() & DsqlCompiledStatement::FLAG_BLR_VERSION4;
	}

	MemoryPool& getPool()
	{
		return PermanentStorage::getPool();
	}

	dsql_dbb* getAttachment()
	{
		return dbb;
	}

	jrd_tra* getTransaction()
	{
		return transaction;
	}

	void setTransaction(jrd_tra* value)
	{
		transaction = value;
	}

	DsqlCompiledStatement* getStatement()
	{
		return statement;
	}

	DsqlCompiledStatement* getStatement() const
	{
		return statement;
	}

	void putDtype(const dsql_fld* field, bool useSubType);
	void putType(const TypeClause& type, bool useSubType);
	void putLocalVariables(const dsql_nod* parameters, SSHORT locals);
	void putLocalVariable(dsql_var* variable, dsql_nod* hostParam, const dsql_str* collationName);
	VariableNode* resolveVariable(const dsql_str* varName);
	void genReturn(bool eosFlag = false);

	void resetContextStack()
	{
		context->clear();
		contextNumber = 0;
	}

	void addCTEs(dsql_nod* list);
	dsql_nod* findCTE(const Firebird::MetaName& name);
	void clearCTEs();
	void checkUnusedCTEs() const;

	// hvlad: each member of recursive CTE can refer to CTE itself (only once) via
	// CTE name or via alias. We need to substitute this aliases when processing CTE
	// member to resolve field names. Therefore we store all aliases in order of
	// occurrence and later use it in backward order (since our parser is right-to-left).
	// Also we put CTE name after all such aliases to distinguish aliases for
	// different CTE's.
	// We also need to repeat this process if main select expression contains union with
	// recursive CTE
	void addCTEAlias(const Firebird::string& alias)
	{
		thread_db* tdbb = JRD_get_thread_data();
		cteAliases.add(FB_NEW(*tdbb->getDefaultPool()) Firebird::string(*tdbb->getDefaultPool(), alias));
	}

	const Firebird::string* getNextCTEAlias()
	{
		return *(--currCteAlias);
	}

	void resetCTEAlias(const Firebird::string& alias)
	{
		const Firebird::string* const* begin = cteAliases.begin();

		currCteAlias = cteAliases.end() - 1;
		fb_assert(currCteAlias >= begin);

		const Firebird::string* curr = *(currCteAlias);

		while (strcmp(curr->c_str(), alias.c_str()))
		{
			currCteAlias--;
			fb_assert(currCteAlias >= begin);

			curr = *(currCteAlias);
		}
	}

	bool isPsql() const { return psql; }
	void setPsql(bool value) { psql = value; }

private:
	dsql_nod* pass1RecursiveCte(dsql_nod* input);
	dsql_nod* pass1RseIsRecursive(dsql_nod* input);
	bool pass1RelProcIsRecursive(dsql_nod* input);
	dsql_nod* pass1JoinIsRecursive(dsql_nod*& inputNod);

private:
	dsql_dbb* dbb;						// DSQL attachment
	jrd_tra* transaction;				// Transaction
	DsqlCompiledStatement* statement;	// Compiled statement

public:
	unsigned flags;						// flags
	Firebird::Array<dsql_msg*> ports;	// Port messages
	dsql_rel* relation;					// relation created by this request (for DDL)
	dsql_prc* procedure;				// procedure created by this request (for DDL)
	DsqlContextStack mainContext;
	DsqlContextStack* context;
	DsqlContextStack unionContext;		// Save contexts for views of unions
	DsqlContextStack derivedContext;	// Save contexts for views of derived tables
	dsql_ctx* outerAggContext;			// agg context for outer ref
	USHORT contextNumber;				// Next available context number
	USHORT derivedContextNumber;		// Next available context number for derived tables
	USHORT scopeLevel;					// Scope level for parsing aliases in subqueries
	USHORT loopLevel;					// Loop level
	DsqlStrStack labels;				// Loop labels
	USHORT cursorNumber;				// Cursor number
	DsqlNodStack cursors;				// Cursors
	USHORT inSelectList;				// now processing "select list"
	USHORT inWhereClause;				// processing "where clause"
	USHORT inGroupByClause;				// processing "group by clause"
	USHORT inHavingClause;				// processing "having clause"
	USHORT inOrderByClause;				// processing "order by clause"
	USHORT errorHandlers;				// count of active error handlers
	USHORT clientDialect;				// dialect passed into the API call
	USHORT inOuterJoin;					// processing inside outer-join part
	dsql_str* aliasRelationPrefix;		// prefix for every relation-alias.
	DsqlNodStack hiddenVars;			// hidden variables
	USHORT hiddenVarsNumber;			// next hidden variable number
	Firebird::MetaName package;			// package being defined
	DsqlNodStack currCtes;				// current processing CTE's
	class dsql_ctx* recursiveCtx;		// context of recursive CTE
	USHORT recursiveCtxId;				// id of recursive union stream context
	bool processingWindow;				// processing window functions
	bool checkConstraintTrigger;		// compiling a check constraint trigger
	dsc domainValue;					// VALUE in the context of domain's check constraint
	Firebird::Array<VariableNode*> variables;
	Firebird::Array<VariableNode*> outputVariables;

private:
	Firebird::HalfStaticArray<dsql_nod*, 4> ctes; // common table expressions
	Firebird::HalfStaticArray<const Firebird::string*, 4> cteAliases; // CTE aliases in recursive members
	const Firebird::string* const* currCteAlias;
	bool psql;
};

class PsqlChanger
{
public:
	PsqlChanger(DsqlCompilerScratch* aDsqlScratch, bool value)
		: dsqlScratch(aDsqlScratch),
		  oldValue(dsqlScratch->isPsql())
	{
		dsqlScratch->setPsql(value);
	}

	~PsqlChanger()
	{
		dsqlScratch->setPsql(oldValue);
	}

private:
	// copying is prohibited
	PsqlChanger(const PsqlChanger&);
	PsqlChanger& operator =(const PsqlChanger&);

	DsqlCompilerScratch* dsqlScratch;
	const bool oldValue;
};

}	// namespace Jrd

#endif // DSQL_COMPILER_SCRATCH_H
