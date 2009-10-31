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
 *  Copyright (c) 2008 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef DSQL_NODES_H
#define DSQL_NODES_H

#include "../jrd/common.h"
#include "../dsql/dsql.h"

namespace Jrd {

class CompilerScratch;
class TypeClause;


class Node : public Firebird::PermanentStorage
{
public:
	explicit Node(MemoryPool& pool)
		: PermanentStorage(pool),
		  compiledStatement(NULL)
	{
	}

	virtual ~Node()
	{
	}

public:
	Node* dsqlPass(CompiledStatement* aCompiledStatement)
	{
		compiledStatement = aCompiledStatement;
		return internalDsqlPass();
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const = 0;

protected:
	virtual Node* internalDsqlPass()
	{
		return this;
	}

protected:
	CompiledStatement* compiledStatement;
};


class DdlNode : public Node
{
public:
	explicit DdlNode(MemoryPool& pool, const Firebird::string& aSqlText)
		: Node(pool),
		  sqlText(pool, aSqlText)
	{
	}

public:
	const Firebird::string& getSqlText()
	{
		return sqlText;
	}

public:
	enum DdlTriggerWhen { DTW_BEFORE, DTW_AFTER };
	static void executeDdlTrigger(thread_db* tdbb, jrd_tra* transaction,
		DdlTriggerWhen when, int action, const Firebird::MetaName& objectName,
		const Firebird::string& sqlText);

public:
	static void checkEmptyName(const Firebird::MetaName& name);
	static Firebird::MetaName nameInMetaCharSet(thread_db* tdbb, const Firebird::MetaName& name);
	static Firebird::MetaName nameInUserCharSet(thread_db* tdbb, const Firebird::MetaName& name);
	static void storeTextBlob(thread_db* tdbb, jrd_tra* transaction, bid* blobId,
		const Firebird::string& text);
	static void storeBlob(thread_db* tdbb, jrd_tra* transaction, bid* blobId,
		const UCHAR* data, unsigned length);

protected:
	void executeDdlTrigger(thread_db* tdbb, jrd_tra* transaction,
		DdlTriggerWhen when, int action, const Firebird::MetaName& objectName);
	void putType(const TypeClause& type, bool useSubType);
	void resetContextStack();

protected:
	virtual Node* internalDsqlPass()
	{
		compiledStatement->req_type = REQ_DDL;
		return this;
	}

public:
	virtual void execute(thread_db* tdbb, jrd_tra* transaction) = 0;

private:
	Firebird::string sqlText;
};


class DmlNode : public Node
{
public:
	explicit DmlNode(MemoryPool& pool)
		: Node(pool),
		  node(NULL)
	{
	}

public:
	virtual DmlNode* pass2(thread_db* tdbb, CompilerScratch* csb, jrd_nod* aNode);

public:
	virtual void genBlr() = 0;
	virtual DmlNode* pass1(thread_db* tdbb, CompilerScratch* csb) = 0;
	virtual DmlNode* pass2(thread_db* tdbb, CompilerScratch* csb) = 0;
	virtual jrd_nod* execute(thread_db* tdbb, jrd_req* request) = 0;

protected:
	jrd_nod* node;
};


class StmtNode : public DmlNode
{
public:
	explicit StmtNode(MemoryPool& pool)
		: DmlNode(pool)
	{
	}
};


// Used to represent nodes that don't have a specific BLR verb, i.e.,
// do not use RegisterNode.
class DsqlOnlyStmtNode : public StmtNode
{
public:
	explicit DsqlOnlyStmtNode(MemoryPool& pool)
		: StmtNode(pool)
	{
	}

public:
	DsqlOnlyStmtNode* pass1(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
	{
		fb_assert(false);
		return this;
	}


	DsqlOnlyStmtNode* pass2(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
	{
		fb_assert(false);
		return this;
	}


	jrd_nod* execute(thread_db* /*tdbb*/, jrd_req* /*request*/)
	{
		fb_assert(false);
		return NULL;
	}
};


// Common node for all "code blocks" (i.e.: procedures, triggers and execute block)
class BlockNode
{
public:
	virtual ~BlockNode()
	{
	}

public:
	virtual void genReturn() = 0;
	virtual dsql_nod* resolveVariable(const dsql_str* varName) = 0;
};


} // namespace

#endif // DSQL_NODES_H
