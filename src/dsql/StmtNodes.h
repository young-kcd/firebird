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
#include "../dsql/DdlNodes.h"
#include "../common/classes/MetaName.h"

namespace Jrd {

class PsqlException;


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

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual IfNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual IfNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual IfNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const jrd_nod* execute(thread_db* tdbb, jrd_req* request) const;

public:
	dsql_nod* dsqlCondition;
	dsql_nod* dsqlTrueAction;
	dsql_nod* dsqlFalseAction;
	NestConst<jrd_nod> condition;
	NestConst<jrd_nod> trueAction;
	NestConst<jrd_nod> falseAction;
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

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual InAutonomousTransactionNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual InAutonomousTransactionNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual InAutonomousTransactionNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const jrd_nod* execute(thread_db* tdbb, jrd_req* request) const;

public:
	dsql_nod* dsqlAction;
	NestConst<jrd_nod> action;
	SLONG savNumberOffset;
};


class ExecBlockNode : public DsqlOnlyStmtNode
{
public:
	explicit ExecBlockNode(MemoryPool& pool)
		: DsqlOnlyStmtNode(pool),
		  parameters(pool),
		  returns(pool),
		  localDeclList(NULL),
		  body(NULL)
	{
	}

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ExecBlockNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

private:
	static void revertParametersOrder(Firebird::Array<dsql_par*>& parameters);

public:
	Firebird::Array<ParameterClause> parameters;
	Firebird::Array<ParameterClause> returns;
	dsql_nod* localDeclList;
	dsql_nod* body;
};


class ExceptionNode : public StmtNode
{
public:
	explicit ExceptionNode(MemoryPool& pool, const Firebird::MetaName& aName = "",
				dsql_nod* aDsqlMessageExpr = NULL, dsql_nod* aDsqlParameters = NULL)
		: StmtNode(pool),
		  name(pool, aName),
		  dsqlMessageExpr(aDsqlMessageExpr),
		  dsqlParameters(aDsqlParameters),
		  messageExpr(NULL),
		  parameters(NULL),
		  exception(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual StmtNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual ExceptionNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ExceptionNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const jrd_nod* execute(thread_db* tdbb, jrd_req* request) const;

private:
	void setError(thread_db* tdbb) const;

public:
	Firebird::MetaName name;
	dsql_nod* dsqlMessageExpr;
	dsql_nod* dsqlParameters;
	NestConst<jrd_nod> messageExpr;
	NestConst<jrd_nod> parameters;
	NestConst<PsqlException> exception;
};


class ExitNode : public DsqlOnlyStmtNode
{
public:
	explicit ExitNode(MemoryPool& pool)
		: DsqlOnlyStmtNode(pool)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
};


class ForNode : public StmtNode
{
public:
	explicit ForNode(MemoryPool& pool)
		: StmtNode(pool),
		  dsqlSelect(NULL),
		  dsqlInto(NULL),
		  dsqlCursor(NULL),
		  dsqlAction(NULL),
		  dsqlLabel(NULL),
		  dsqlForceSingular(false),
		  stall(NULL),
		  rse(NULL),
		  statement(NULL),
		  cursor(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual StmtNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual StmtNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual StmtNode* pass2(thread_db* tdbb, CompilerScratch* csb);

	virtual void pass2Cursor(RseNode*& rsePtr, Cursor**& cursorPtr)
	{
		rsePtr = rse;
		cursorPtr = cursor.getAddress();
	}

	virtual const jrd_nod* execute(thread_db* tdbb, jrd_req* request) const;

public:
	dsql_nod* dsqlSelect;
	dsql_nod* dsqlInto;
	dsql_nod* dsqlCursor;
	dsql_nod* dsqlAction;
	dsql_nod* dsqlLabel;
	bool dsqlForceSingular;
	NestConst<jrd_nod> stall;
	NestConst<RseNode> rse;
	NestConst<jrd_nod> statement;
	NestConst<Cursor> cursor;
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

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual PostEventNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual PostEventNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual PostEventNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const jrd_nod* execute(thread_db* tdbb, jrd_req* request) const;

public:
	dsql_nod* dsqlEvent;
	dsql_nod* dsqlArgument;
	NestConst<jrd_nod> event;
	NestConst<jrd_nod> argument;
};


class SavepointNode : public StmtNode
{
public:
	enum Command
	{
		CMD_NOTHING = -1,
		CMD_SET = blr_savepoint_set,
		CMD_RELEASE = blr_savepoint_release,
		CMD_RELEASE_ONLY = blr_savepoint_release_single,
		CMD_ROLLBACK = blr_savepoint_undo
	};

public:
	explicit SavepointNode(MemoryPool& pool)
		: StmtNode(pool),
		  command(CMD_NOTHING),
		  name(pool)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual SavepointNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual SavepointNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual SavepointNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const jrd_nod* execute(thread_db* tdbb, jrd_req* request) const;

public:
	Command command;
	Firebird::MetaName name;
};


class SuspendNode : public StmtNode
{
public:
	explicit SuspendNode(MemoryPool& pool)
		: StmtNode(pool),
		  message(NULL),
		  statement(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual SuspendNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual SuspendNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual SuspendNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const jrd_nod* execute(thread_db* tdbb, jrd_req* request) const;

public:
	NestConst<jrd_nod> message;
	NestConst<jrd_nod> statement;
};


class ReturnNode : public DsqlOnlyStmtNode
{
public:
	explicit ReturnNode(MemoryPool& pool, dsql_nod* val = NULL)
		: DsqlOnlyStmtNode(pool), value(val)
	{
	}

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ReturnNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

public:
	dsql_nod* value;
};


} // namespace

#endif // DSQL_STMT_NODES_H
