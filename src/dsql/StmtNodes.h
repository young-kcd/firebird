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

#ifndef DSQL_STMT_NODES_H
#define DSQL_STMT_NODES_H

#include "../jrd/common.h"
#include "../jrd/blr.h"
#include "../dsql/Nodes.h"
#include "../common/classes/MetaName.h"

namespace Jrd {


class IfNode : public StmtNode
{
public:
	explicit IfNode(MemoryPool& pool)
		: StmtNode(pool),
		  dsqlCondition(NULL),
		  dsqlTrueAction(NULL),
		  dsqlFalseAction(NULL),
		  condition(NULL),
		  trueAction(NULL),
		  falseAction(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

protected:
	virtual IfNode* internalDsqlPass();

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void genBlr();
	virtual IfNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual IfNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual jrd_nod* execute(thread_db* tdbb, jrd_req* request);

public:
	dsql_nod* dsqlCondition;
	dsql_nod* dsqlTrueAction;
	dsql_nod* dsqlFalseAction;
	jrd_nod* condition;
	jrd_nod* trueAction;
	jrd_nod* falseAction;
};


class InAutonomousTransactionNode : public StmtNode
{
public:
	explicit InAutonomousTransactionNode(MemoryPool& pool)
		: StmtNode(pool),
		  dsqlAction(NULL),
		  action(NULL),
		  savNumberOffset(0)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

protected:
	virtual InAutonomousTransactionNode* internalDsqlPass();

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void genBlr();
	virtual InAutonomousTransactionNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual InAutonomousTransactionNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual jrd_nod* execute(thread_db* tdbb, jrd_req* request);

public:
	dsql_nod* dsqlAction;
	jrd_nod* action;
	SLONG savNumberOffset;
};


class ExecBlockNode : public DsqlOnlyStmtNode, public BlockNode
{
public:
	explicit ExecBlockNode(MemoryPool& pool)
		: DsqlOnlyStmtNode(pool),
		  legacyParameters(NULL),
		  legacyReturns(NULL),
		  localDeclList(NULL),
		  body(NULL)
	{
	}

protected:
	virtual ExecBlockNode* internalDsqlPass();

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void genBlr();

public:
	virtual void genReturn();
	virtual dsql_nod* resolveVariable(const dsql_str* varName);

private:
	static dsql_par* revertParametersOrder(dsql_par* parameter, dsql_par* prev);

public:
	dsql_nod* legacyParameters;
	dsql_nod* legacyReturns;
	dsql_nod* localDeclList;
	dsql_nod* body;
};


class ExitNode : public DsqlOnlyStmtNode
{
public:
	explicit ExitNode(MemoryPool& pool)
		: DsqlOnlyStmtNode(pool)
	{
	}

protected:
	virtual ExitNode* internalDsqlPass();

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void genBlr();
};


class PostEventNode : public StmtNode
{
public:
	explicit PostEventNode(MemoryPool& pool)
		: StmtNode(pool),
		  dsqlEvent(NULL),
		  dsqlArgument(NULL),
		  event(NULL),
		  argument(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

protected:
	virtual PostEventNode* internalDsqlPass();

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void genBlr();
	virtual PostEventNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual PostEventNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual jrd_nod* execute(thread_db* tdbb, jrd_req* request);

public:
	dsql_nod* dsqlEvent;
	dsql_nod* dsqlArgument;
	jrd_nod* event;
	jrd_nod* argument;
};


class SavepointNode : public StmtNode
{
public:
	enum Command
	{
		CMD_SET = blr_savepoint_set,
		CMD_RELEASE = blr_savepoint_release,
		CMD_RELEASE_ONLY = blr_savepoint_release_single,
		CMD_ROLLBACK = blr_savepoint_undo
	};

public:
	explicit SavepointNode(MemoryPool& pool)
		: StmtNode(pool),
		  command((Command) 0),
		  name(pool)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

protected:
	virtual SavepointNode* internalDsqlPass();

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void genBlr();
	virtual SavepointNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual SavepointNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual jrd_nod* execute(thread_db* tdbb, jrd_req* request);

public:
	Command command;
	Firebird::MetaName name;
};


class SuspendNode : public StmtNode
{
public:
	explicit SuspendNode(MemoryPool& pool)
		: StmtNode(pool),
		  blockNode(NULL),
		  message(NULL),
		  statement(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

protected:
	virtual SuspendNode* internalDsqlPass();

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void genBlr();
	virtual SuspendNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual SuspendNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual jrd_nod* execute(thread_db* tdbb, jrd_req* request);

public:
	BlockNode* blockNode;
	jrd_nod* message;
	jrd_nod* statement;
};


} // namespace

#endif // DSQL_STMT_NODES_H
