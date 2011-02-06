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

#include "../common/common.h"
#include "../common/classes/MetaName.h"
#include "../jrd/blr.h"
#include "../jrd/extds/ExtDS.h"
#include "../dsql/Nodes.h"
#include "../dsql/DdlNodes.h"

namespace Jrd {

class PsqlException;
class CompoundStmtNode;
class RelationSourceNode;


struct ValidateInfo
{
	NestConst<StmtNode> stmt;
	NestConst<BoolExprNode> boolean;
	NestConst<ValueExprNode> value;
};


class AssignmentNode : public TypedNode<StmtNode, StmtNode::TYPE_ASSIGNMENT>
{
public:
	explicit AssignmentNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_ASSIGNMENT>(pool),
		  asgnFrom(NULL),
		  asgnTo(NULL),
		  missing(NULL),
		  missing2(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual AssignmentNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual AssignmentNode* copy(thread_db* tdbb, NodeCopier& copier) const;
	virtual AssignmentNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual AssignmentNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	NestConst<ValueExprNode> asgnFrom;
	NestConst<ValueExprNode> asgnTo;
	NestConst<ValueExprNode> missing;
	NestConst<ValueExprNode> missing2;
};


class BlockNode : public TypedNode<StmtNode, StmtNode::TYPE_BLOCK>
{
public:
	explicit BlockNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_BLOCK>(pool),
		  action(NULL),
		  handlers(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual BlockNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual BlockNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual BlockNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

private:
	static bool testAndFixupError(thread_db* tdbb, jrd_req* request, const PsqlException* conditions);

public:
	NestConst<StmtNode> action;
	NestConst<CompoundStmtNode> handlers;
};


class CompoundStmtNode : public TypedNode<StmtNode, StmtNode::TYPE_COMPOUND_STMT>	// blr_begin
{
public:
	explicit CompoundStmtNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_COMPOUND_STMT>(pool),
		  statements(pool),
		  onlyAssignments(false)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual CompoundStmtNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual CompoundStmtNode* copy(thread_db* tdbb, NodeCopier& copier) const;
	virtual CompoundStmtNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual CompoundStmtNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	Firebird::Array<NestConst<StmtNode> > statements;
	bool onlyAssignments;
};


class ContinueLeaveNode : public TypedNode<StmtNode, StmtNode::TYPE_CONTINUE_LEAVE>
{
public:
	explicit ContinueLeaveNode(MemoryPool& pool, UCHAR aBlrOp)
		: TypedNode<StmtNode, StmtNode::TYPE_CONTINUE_LEAVE>(pool),
		  blrOp(aBlrOp),
		  labelNumber(0),
		  dsqlLabel(NULL)
	{
		fb_assert(blrOp == blr_continue_loop || blrOp == blr_leave);
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ContinueLeaveNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

	virtual ContinueLeaveNode* pass1(thread_db* tdbb, CompilerScratch* csb)
	{
		return this;
	}

	virtual ContinueLeaveNode* pass2(thread_db* tdbb, CompilerScratch* csb)
	{
		return this;
	}

	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	UCHAR blrOp;
	USHORT labelNumber;
	dsql_nod* dsqlLabel;
};


class CursorStmtNode : public TypedNode<StmtNode, StmtNode::TYPE_CURSOR_STMT>
{
public:
	explicit CursorStmtNode(MemoryPool& pool, UCHAR aCursorOp, const Firebird::MetaName& aDsqlName = "",
				dsql_nod* aDsqlIntoStmt = NULL)
		: TypedNode<StmtNode, StmtNode::TYPE_CURSOR_STMT>(pool),
		  dsqlName(pool, aDsqlName),
		  dsqlIntoStmt(aDsqlIntoStmt),
		  dsqlScrollExpr(NULL),
		  cursorOp(aCursorOp),
		  cursorNumber(0),
		  scrollOp(0),
		  scrollExpr(NULL),
		  intoStmt(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual CursorStmtNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual CursorStmtNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual CursorStmtNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	Firebird::MetaName dsqlName;
	dsql_nod* dsqlIntoStmt;
	dsql_nod* dsqlScrollExpr;
	UCHAR cursorOp;
	USHORT cursorNumber;
	UCHAR scrollOp;
	NestConst<ValueExprNode> scrollExpr;
	NestConst<StmtNode> intoStmt;
};


class DeclareCursorNode : public TypedNode<StmtNode, StmtNode::TYPE_DECLARE_CURSOR>
{
public:
	explicit DeclareCursorNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_DECLARE_CURSOR>(pool),
		  rse(NULL),
		  refs(NULL),
		  cursorNumber(0),
		  cursor(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual DeclareCursorNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual DeclareCursorNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual DeclareCursorNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	NestConst<RseNode> rse;
	NestConst<ValueListNode> refs;
	USHORT cursorNumber;
	NestConst<Cursor> cursor;
};


class DeclareVariableNode : public TypedNode<StmtNode, StmtNode::TYPE_DECLARE_VARIABLE>
{
public:
	explicit DeclareVariableNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_DECLARE_VARIABLE>(pool),
		  varId(0)
	{
		varDesc.clear();
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual DeclareVariableNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual DeclareVariableNode* copy(thread_db* tdbb, NodeCopier& copier) const;
	virtual DeclareVariableNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual DeclareVariableNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	USHORT varId;
	dsc varDesc;
};


class EraseNode : public TypedNode<StmtNode, StmtNode::TYPE_ERASE>
{
public:
	explicit EraseNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_ERASE>(pool),
		  statement(NULL),
		  subStatement(NULL),
		  stream(0)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual EraseNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual EraseNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual EraseNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

private:
	static void pass1Erase(thread_db* tdbb, CompilerScratch* csb, EraseNode* node);
	const StmtNode* erase(thread_db* tdbb, jrd_req* request, WhichTrigger whichTrig) const;

public:
	NestConst<StmtNode> statement;
	NestConst<StmtNode> subStatement;
	USHORT stream;
};


class ErrorHandlerNode : public TypedNode<StmtNode, StmtNode::TYPE_ERROR_HANDLER>
{
public:
	explicit ErrorHandlerNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_ERROR_HANDLER>(pool),
		  action(NULL),
		  conditions(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ErrorHandlerNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual ErrorHandlerNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ErrorHandlerNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	NestConst<StmtNode> action;
	NestConst<PsqlException> conditions;
};


class ExecProcedureNode : public TypedNode<StmtNode, StmtNode::TYPE_EXEC_PROCEDURE>
{
public:
	explicit ExecProcedureNode(MemoryPool& pool,
				const Firebird::QualifiedName& aDsqlName = Firebird::QualifiedName(),
				dsql_nod* aDsqlInputs = NULL, dsql_nod* aDsqlOutputs = NULL)
		: TypedNode<StmtNode, StmtNode::TYPE_EXEC_PROCEDURE>(pool),
		  dsqlName(pool, aDsqlName),
		  dsqlInputs(aDsqlInputs),
		  dsqlOutputs(aDsqlOutputs),
		  inputSources(NULL),
		  inputTargets(NULL),
		  inputMessage(NULL),
		  outputSources(NULL),
		  outputTargets(NULL),
		  outputMessage(NULL),
		  procedure(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ExecProcedureNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual ExecProcedureNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ExecProcedureNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

private:
	dsql_nod* explodeOutputs(DsqlCompilerScratch* dsqlScratch, const dsql_prc* procedure);
	void executeProcedure(thread_db* tdbb, jrd_req* request) const;

public:
	Firebird::QualifiedName dsqlName;
	dsql_nod* dsqlInputs;
	dsql_nod* dsqlOutputs;
	NestConst<ValueListNode> inputSources;
	NestConst<ValueListNode> inputTargets;
	NestConst<MessageNode> inputMessage;
	NestConst<ValueListNode> outputSources;
	NestConst<ValueListNode> outputTargets;
	NestConst<MessageNode> outputMessage;
	NestConst<jrd_prc> procedure;
};


class ExecStatementNode : public TypedNode<StmtNode, StmtNode::TYPE_EXEC_STATEMENT>
{
public:
	explicit ExecStatementNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_EXEC_STATEMENT>(pool),
		  dsqlSql(NULL),
		  dsqlDataSource(NULL),
		  dsqlUserName(NULL),
		  dsqlPassword(NULL),
		  dsqlRole(NULL),
		  dsqlInnerStmt(NULL),
		  dsqlInputs(NULL),
		  dsqlOutputs(NULL),
		  dsqlLabel(NULL),
		  sql(NULL),
		  dataSource(NULL),
		  userName(NULL),
		  password(NULL),
		  role(NULL),
		  innerStmt(NULL),
		  inputs(NULL),
		  outputs(NULL),
		  useCallerPrivs(false),
		  traScope(EDS::TraScope(0)),	// not defined
		  inputNames(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual StmtNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual ExecStatementNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ExecStatementNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

private:
	static void genOptionalExpr(DsqlCompilerScratch* dsqlScratch, const UCHAR code, dsql_nod* node);

	void getString(thread_db* tdbb, jrd_req* request, const ValueExprNode* node,
		Firebird::string& str, bool useAttCS = false) const;

public:
	dsql_nod* dsqlSql;
	dsql_nod* dsqlDataSource;
	dsql_nod* dsqlUserName;
	dsql_nod* dsqlPassword;
	dsql_nod* dsqlRole;
	dsql_nod* dsqlInnerStmt;
	dsql_nod* dsqlInputs;
	dsql_nod* dsqlOutputs;
	dsql_nod* dsqlLabel;
	NestConst<ValueExprNode> sql;
	NestConst<ValueExprNode> dataSource;
	NestConst<ValueExprNode> userName;
	NestConst<ValueExprNode> password;
	NestConst<ValueExprNode> role;
	NestConst<StmtNode> innerStmt;
	NestConst<ValueListNode> inputs;
	NestConst<ValueListNode> outputs;
	bool useCallerPrivs;
	EDS::TraScope traScope;
	EDS::ParamNames* inputNames;
};


class IfNode : public TypedNode<StmtNode, StmtNode::TYPE_IF>
{
public:
	explicit IfNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_IF>(pool),
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
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	dsql_nod* dsqlCondition;
	dsql_nod* dsqlTrueAction;
	dsql_nod* dsqlFalseAction;
	NestConst<BoolExprNode> condition;
	NestConst<StmtNode> trueAction;
	NestConst<StmtNode> falseAction;
};


class InAutonomousTransactionNode : public TypedNode<StmtNode, StmtNode::TYPE_IN_AUTO_TRANS>
{
public:
	explicit InAutonomousTransactionNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_IN_AUTO_TRANS>(pool),
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
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	dsql_nod* dsqlAction;
	NestConst<StmtNode> action;
	SLONG savNumberOffset;
};


class InitVariableNode : public TypedNode<StmtNode, StmtNode::TYPE_INIT_VARIABLE>
{
public:
	explicit InitVariableNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_INIT_VARIABLE>(pool),
		  varId(0),
		  varDecl(NULL),
		  varInfo(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual InitVariableNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual InitVariableNode* copy(thread_db* tdbb, NodeCopier& copier) const;
	virtual InitVariableNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual InitVariableNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	USHORT varId;
	NestConst<DeclareVariableNode> varDecl;
	NestConst<ItemInfo> varInfo;
};


class ExecBlockNode : public TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_EXEC_BLOCK>
{
public:
	explicit ExecBlockNode(MemoryPool& pool)
		: TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_EXEC_BLOCK>(pool),
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


class ExceptionNode : public TypedNode<StmtNode, StmtNode::TYPE_EXCEPTION>
{
public:
	explicit ExceptionNode(MemoryPool& pool, const Firebird::MetaName& aName = "",
				dsql_nod* aDsqlMessageExpr = NULL, dsql_nod* aDsqlParameters = NULL)
		: TypedNode<StmtNode, StmtNode::TYPE_EXCEPTION>(pool),
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
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

private:
	void setError(thread_db* tdbb) const;

public:
	Firebird::MetaName name;
	dsql_nod* dsqlMessageExpr;
	dsql_nod* dsqlParameters;
	NestConst<ValueExprNode> messageExpr;
	NestConst<ValueListNode> parameters;
	NestConst<PsqlException> exception;
};


class ExitNode : public TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_EXIT>
{
public:
	explicit ExitNode(MemoryPool& pool)
		: TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_EXIT>(pool)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
};


class ForNode : public TypedNode<StmtNode, StmtNode::TYPE_FOR>
{
public:
	explicit ForNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_FOR>(pool),
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
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	dsql_nod* dsqlSelect;
	dsql_nod* dsqlInto;
	dsql_nod* dsqlCursor;
	dsql_nod* dsqlAction;
	dsql_nod* dsqlLabel;
	bool dsqlForceSingular;
	NestConst<StmtNode> stall;
	NestConst<RseNode> rse;
	NestConst<StmtNode> statement;
	NestConst<Cursor> cursor;
};


class HandlerNode : public TypedNode<StmtNode, StmtNode::TYPE_HANDLER>
{
public:
	explicit HandlerNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_HANDLER>(pool),
		  statement(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual HandlerNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual HandlerNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual HandlerNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	NestConst<StmtNode> statement;
};


class LabelNode : public TypedNode<StmtNode, StmtNode::TYPE_LABEL>
{
public:
	explicit LabelNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_LABEL>(pool),
		  statement(NULL),
		  labelNumber(0)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual LabelNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual LabelNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual LabelNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	NestConst<StmtNode> statement;
	USHORT labelNumber;
};


class LoopNode : public TypedNode<StmtNode, StmtNode::TYPE_LOOP>
{
public:
	explicit LoopNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_LOOP>(pool),
		  statement(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual LoopNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual LoopNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual LoopNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	NestConst<StmtNode> statement;
};


class MessageNode : public TypedNode<StmtNode, StmtNode::TYPE_MESSAGE>
{
public:
	explicit MessageNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_MESSAGE>(pool),
		  messageNumber(0),
		  format(NULL),
		  impureFlags(0)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual MessageNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual MessageNode* copy(thread_db* tdbb, NodeCopier& copier) const;
	virtual MessageNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual MessageNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	USHORT messageNumber;
	NestConst<Format> format;
	ULONG impureFlags;
};


class ModifyNode : public TypedNode<StmtNode, StmtNode::TYPE_MODIFY>
{
public:
	explicit ModifyNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_MODIFY>(pool),
		  statement(NULL),
		  statement2(NULL),
		  subMod(NULL),
		  validations(pool),
		  mapView(NULL),
		  orgStream(0),
		  newStream(0)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ModifyNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual ModifyNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ModifyNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

private:
	static void pass1Modify(thread_db* tdbb, CompilerScratch* csb, ModifyNode* node);
	const StmtNode* modify(thread_db* tdbb, jrd_req* request, WhichTrigger whichTrig) const;

public:
	NestConst<StmtNode> statement;
	NestConst<StmtNode> statement2;
	NestConst<StmtNode> subMod;
	Firebird::Array<ValidateInfo> validations;
	NestConst<StmtNode> mapView;
	USHORT orgStream;
	USHORT newStream;
};


class PostEventNode : public TypedNode<StmtNode, StmtNode::TYPE_POST_EVENT>
{
public:
	explicit PostEventNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_POST_EVENT>(pool),
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
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	dsql_nod* dsqlEvent;
	dsql_nod* dsqlArgument;
	NestConst<ValueExprNode> event;
	NestConst<ValueExprNode> argument;
};


class ReceiveNode : public TypedNode<StmtNode, StmtNode::TYPE_RECEIVE>
{
public:
	explicit ReceiveNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_RECEIVE>(pool),
		  statement(NULL),
		  message(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ReceiveNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual ReceiveNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ReceiveNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	NestConst<StmtNode> statement;
	NestConst<MessageNode> message;
};


class StoreNode : public TypedNode<StmtNode, StmtNode::TYPE_STORE>
{
public:
	explicit StoreNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_STORE>(pool),
		  statement(NULL),
		  statement2(NULL),
		  subStore(NULL),
		  validations(pool),
		  relationSource(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual StoreNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual StoreNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual StoreNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

private:
	static bool pass1Store(thread_db* tdbb, CompilerScratch* csb, StoreNode* node);
	void makeDefaults(thread_db* tdbb, CompilerScratch* csb);
	const StmtNode* store(thread_db* tdbb, jrd_req* request, WhichTrigger whichTrig) const;

public:
	NestConst<StmtNode> statement;
	NestConst<StmtNode> statement2;
	NestConst<StmtNode> subStore;
	Firebird::Array<ValidateInfo> validations;
	NestConst<RelationSourceNode> relationSource;
};


class UserSavepointNode : public TypedNode<StmtNode, StmtNode::TYPE_USER_SAVEPOINT>
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
	explicit UserSavepointNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_USER_SAVEPOINT>(pool),
		  command(CMD_NOTHING),
		  name(pool)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual UserSavepointNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual UserSavepointNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual UserSavepointNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	Command command;
	Firebird::MetaName name;
};


class SelectNode : public TypedNode<StmtNode, StmtNode::TYPE_SELECT>
{
public:
	explicit SelectNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_SELECT>(pool),
		  statements(pool)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual SelectNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual SelectNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual SelectNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	Firebird::Array<NestConst<StmtNode> > statements;
};


class SetGeneratorNode : public TypedNode<StmtNode, StmtNode::TYPE_SET_GENERATOR>
{
public:
	explicit SetGeneratorNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_SET_GENERATOR>(pool),
		  value(NULL),
		  genId(0)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual SetGeneratorNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual SetGeneratorNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual SetGeneratorNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	NestConst<ValueExprNode> value;
	USHORT genId;
};


class StallNode : public TypedNode<StmtNode, StmtNode::TYPE_STALL>
{
public:
	explicit StallNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_STALL>(pool)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual StallNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual StallNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual StallNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;
};


class SuspendNode : public TypedNode<StmtNode, StmtNode::TYPE_SUSPEND>
{
public:
	explicit SuspendNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_SUSPEND>(pool),
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
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	NestConst<MessageNode> message;
	NestConst<StmtNode> statement;
};


class ReturnNode : public TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_RETURN>
{
public:
	explicit ReturnNode(MemoryPool& pool, dsql_nod* val = NULL)
		: TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_RETURN>(pool),
		  value(val)
	{
	}

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ReturnNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

public:
	dsql_nod* value;
};


class SavePointNode : public TypedNode<StmtNode, StmtNode::TYPE_SAVEPOINT>
{
public:
	explicit SavePointNode(MemoryPool& pool, UCHAR aBlrOp)
		: TypedNode<StmtNode, StmtNode::TYPE_SAVEPOINT>(pool),
		  blrOp(aBlrOp)
	{
		fb_assert(blrOp == blr_start_savepoint || blrOp == blr_end_savepoint);
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual SavePointNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

	virtual SavePointNode* pass1(thread_db* tdbb, CompilerScratch* csb)
	{
		return this;
	}

	virtual SavePointNode* pass2(thread_db* tdbb, CompilerScratch* csb)
	{
		return this;
	}

	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	UCHAR blrOp;
};


} // namespace

#endif // DSQL_STMT_NODES_H
