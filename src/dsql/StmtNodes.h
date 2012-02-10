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
#include "../jrd/Function.h"
#include "../jrd/extds/ExtDS.h"
#include "../dsql/Nodes.h"
#include "../dsql/DdlNodes.h"

namespace Jrd {

class CompoundStmtNode;
class ExecBlockNode;
class RelationSourceNode;
class SelectNode;

typedef Firebird::Pair<Firebird::NonPooled<dsql_nod*, Firebird::Array<dsql_nod*>*> > ReturningClause;


class ExceptionItem : public Firebird::PermanentStorage
{
public:
	enum Type
	{
		SQL_CODE = 1,
		GDS_CODE = 2,
		XCP_CODE = 3,
		XCP_DEFAULT = 4
	};

	ExceptionItem(MemoryPool& pool, const ExceptionItem& o)
		: PermanentStorage(pool),
		  type(o.type),
		  code(o.code),
		  name(pool, o.name)
	{
	}

	explicit ExceptionItem(MemoryPool& pool)
		: PermanentStorage(pool),
		  type(Type(0)),
		  code(0),
		  name(pool)
	{
	}

	ExceptionItem& operator =(const ExceptionItem& o)
	{
		type = o.type;
		code = o.code;
		name = o.name;
		return *this;
	}

	Type type;
	SLONG code;
	// ASF: There are some inconsistencies in the type of 'name'. Metanames have maximum of 31 chars,
	// while there are system exceptions with 32 chars. The parser always expects metanames, but
	// I'm following the legacy code and making this a string.
	Firebird::string name;
};

typedef Firebird::ObjectsArray<ExceptionItem> ExceptionArray;


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
		  dsqlAsgnFrom(NULL),
		  dsqlAsgnTo(NULL),
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
	dsql_nod* dsqlAsgnFrom;
	dsql_nod* dsqlAsgnTo;
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
	virtual StmtNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual BlockNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual BlockNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

private:
	static bool testAndFixupError(thread_db* tdbb, jrd_req* request, const ExceptionArray& conditions);

public:
	NestConst<StmtNode> action;
	NestConst<CompoundStmtNode> handlers;
};


class CommitRollbackNode : public TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_COMMIT_ROLLBACK>
{
public:
	enum Command
	{
		CMD_COMMIT,
		CMD_ROLLBACK
	};

public:
	explicit CommitRollbackNode(MemoryPool& pool, Command aCommand, bool aRetain)
		: TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_COMMIT_ROLLBACK>(pool),
		  command(aCommand),
		  retain(aRetain)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const
	{
		text = "CommitRollbackNode";
	}

	virtual CommitRollbackNode* dsqlPass(DsqlCompilerScratch* dsqlScratch)
	{
		switch (command)
		{
			case CMD_COMMIT:
				dsqlScratch->getStatement()->setType(retain ?
					DsqlCompiledStatement::TYPE_COMMIT_RETAIN : DsqlCompiledStatement::TYPE_COMMIT);
				break;

			case CMD_ROLLBACK:
				dsqlScratch->getStatement()->setType(retain ?
					DsqlCompiledStatement::TYPE_ROLLBACK_RETAIN : DsqlCompiledStatement::TYPE_ROLLBACK);
				break;
		}

		return this;
	}

	virtual void genBlr(DsqlCompilerScratch* dsqlScratch)
	{
	}

private:
	Command command;
	bool retain;
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

	virtual ContinueLeaveNode* pass1(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
	{
		return this;
	}

	virtual ContinueLeaveNode* pass2(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
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
				Firebird::Array<dsql_nod*>* aDsqlIntoStmt = NULL)
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
	Firebird::Array<dsql_nod*>* dsqlIntoStmt;
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
	static const USHORT CUR_TYPE_NONE = 0;
	static const USHORT CUR_TYPE_EXPLICIT = 1;
	static const USHORT CUR_TYPE_FOR = 2;
	static const USHORT CUR_TYPE_ALL = (CUR_TYPE_EXPLICIT | CUR_TYPE_FOR);

	explicit DeclareCursorNode(MemoryPool& pool, const Firebird::MetaName& aDsqlName = NULL,
				USHORT aDsqlCursorType = CUR_TYPE_NONE)
		: TypedNode<StmtNode, StmtNode::TYPE_DECLARE_CURSOR>(pool),
		  dsqlCursorType(aDsqlCursorType),
		  dsqlScroll(false),
		  dsqlName(aDsqlName),
		  dsqlSelect(NULL),
		  dsqlRse(NULL),
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
	USHORT dsqlCursorType;
	bool dsqlScroll;
	Firebird::MetaName dsqlName;
	SelectNode* dsqlSelect;
	dsql_nod* dsqlRse;
	NestConst<RseNode> rse;
	NestConst<ValueListNode> refs;
	USHORT cursorNumber;
	NestConst<Cursor> cursor;
};


class DeclareSubFuncNode : public TypedNode<StmtNode, StmtNode::TYPE_DECLARE_SUBFUNC>
{
public:
	explicit DeclareSubFuncNode(MemoryPool& pool, const Firebird::MetaName& aName)
		: TypedNode<StmtNode, StmtNode::TYPE_DECLARE_SUBFUNC>(pool),
		  name(pool, aName),
		  dsqlDeterministic(false),
		  dsqlBlock(NULL),
		  blockScratch(NULL),
		  dsqlFunction(NULL),
		  blrStart(NULL),
		  blrLength(0),
		  subCsb(NULL),
		  routine(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual DeclareSubFuncNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

	virtual DeclareSubFuncNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual DeclareSubFuncNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

private:
	static void parseParameters(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb,
		Firebird::Array<NestConst<Parameter> >& paramArray, USHORT* defaultCount = NULL);

	void genParameters(DsqlCompilerScratch* dsqlScratch,
		const Firebird::Array<ParameterClause>& paramArray);

public:
	Firebird::MetaName name;
	bool dsqlDeterministic;
	ExecBlockNode* dsqlBlock;
	DsqlCompilerScratch* blockScratch;
	dsql_udf* dsqlFunction;
	const UCHAR* blrStart;
	ULONG blrLength;
	CompilerScratch* subCsb;
	Function* routine;
};


class DeclareSubProcNode : public TypedNode<StmtNode, StmtNode::TYPE_DECLARE_SUBPROC>
{
public:
	explicit DeclareSubProcNode(MemoryPool& pool, const Firebird::MetaName& aName)
		: TypedNode<StmtNode, StmtNode::TYPE_DECLARE_SUBPROC>(pool),
		  name(pool, aName),
		  dsqlBlock(NULL),
		  blockScratch(NULL),
		  dsqlProcedure(NULL),
		  blrStart(NULL),
		  blrLength(0),
		  subCsb(NULL),
		  routine(NULL)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual DeclareSubProcNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

	virtual DeclareSubProcNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual DeclareSubProcNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

private:
	static void parseParameters(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb,
		Firebird::Array<NestConst<Parameter> >& paramArray, USHORT* defaultCount = NULL);

	void genParameters(DsqlCompilerScratch* dsqlScratch,
		const Firebird::Array<ParameterClause>& paramArray);

public:
	Firebird::MetaName name;
	ExecBlockNode* dsqlBlock;
	DsqlCompilerScratch* blockScratch;
	dsql_prc* dsqlProcedure;
	const UCHAR* blrStart;
	ULONG blrLength;
	CompilerScratch* subCsb;
	jrd_prc* routine;
};


class DeclareVariableNode : public TypedNode<StmtNode, StmtNode::TYPE_DECLARE_VARIABLE>
{
public:
	explicit DeclareVariableNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_DECLARE_VARIABLE>(pool),
		  dsqlDef(NULL),
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
	NestConst<ParameterClause> dsqlDef;
	USHORT varId;
	dsc varDesc;
};


class EraseNode : public TypedNode<StmtNode, StmtNode::TYPE_ERASE>
{
public:
	explicit EraseNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_ERASE>(pool),
		  dsqlRelation(NULL),
		  dsqlBoolean(NULL),
		  dsqlPlan(NULL),
		  dsqlSort(NULL),
		  dsqlRows(NULL),
		  dsqlCursor(NULL),
		  dsqlReturning(NULL),
		  dsqlRse(NULL),
		  dsqlContext(NULL),
		  statement(NULL),
		  subStatement(NULL),
		  stream(0)
	{
	}

public:
	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual StmtNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual EraseNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual EraseNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

private:
	static void pass1Erase(thread_db* tdbb, CompilerScratch* csb, EraseNode* node);
	const StmtNode* erase(thread_db* tdbb, jrd_req* request, WhichTrigger whichTrig) const;

public:
	dsql_nod* dsqlRelation;
	dsql_nod* dsqlBoolean;
	dsql_nod* dsqlPlan;
	dsql_nod* dsqlSort;
	dsql_nod* dsqlRows;
	dsql_nod* dsqlCursor;
	ReturningClause* dsqlReturning;
	dsql_nod* dsqlRse;
	dsql_ctx* dsqlContext;
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
		  conditions(pool)
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
	ExceptionArray conditions;
};


class ExecProcedureNode : public TypedNode<StmtNode, StmtNode::TYPE_EXEC_PROCEDURE>
{
public:
	explicit ExecProcedureNode(MemoryPool& pool,
				const Firebird::QualifiedName& aDsqlName = Firebird::QualifiedName(),
				dsql_nod* aDsqlInputs = NULL, Firebird::Array<dsql_nod*>* aDsqlOutputs = NULL)
		: TypedNode<StmtNode, StmtNode::TYPE_EXEC_PROCEDURE>(pool),
		  dsqlName(pool, aDsqlName),
		  dsqlInputs(aDsqlInputs),
		  dsqlOutputs(aDsqlOutputs),
		  dsqlProcedure(NULL),
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
	Firebird::Array<dsql_nod*>* explodeOutputs(DsqlCompilerScratch* dsqlScratch, const dsql_prc* procedure);
	void executeProcedure(thread_db* tdbb, jrd_req* request) const;

public:
	Firebird::QualifiedName dsqlName;
	dsql_nod* dsqlInputs;
	Firebird::Array<dsql_nod*>* dsqlOutputs;
	dsql_prc* dsqlProcedure;
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
	dsql_nod* dsqlInputs;
	Firebird::Array<dsql_nod*>* dsqlOutputs;
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
	NestConst<BoolExprNode> condition;
	NestConst<StmtNode> trueAction;
	NestConst<StmtNode> falseAction;
};


class InAutonomousTransactionNode : public TypedNode<StmtNode, StmtNode::TYPE_IN_AUTO_TRANS>
{
public:
	explicit InAutonomousTransactionNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_IN_AUTO_TRANS>(pool),
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
	NestConst<CompoundStmtNode> localDeclList;
	NestConst<StmtNode> body;
};


class ExceptionNode : public TypedNode<StmtNode, StmtNode::TYPE_EXCEPTION>
{
public:
	ExceptionNode(MemoryPool& pool, const Firebird::MetaName& name,
				dsql_nod* aDsqlMessageExpr = NULL, dsql_nod* aDsqlParameters = NULL)
		: TypedNode<StmtNode, StmtNode::TYPE_EXCEPTION>(pool),
		  dsqlMessageExpr(aDsqlMessageExpr),
		  dsqlParameters(aDsqlParameters),
		  messageExpr(NULL),
		  parameters(NULL)
	{
		exception = FB_NEW(pool) ExceptionItem(pool);
		exception->type = ExceptionItem::XCP_CODE;
		exception->name = name.c_str();
	}

	explicit ExceptionNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_EXCEPTION>(pool),
		  dsqlMessageExpr(NULL),
		  dsqlParameters(NULL),
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
	dsql_nod* dsqlMessageExpr;
	dsql_nod* dsqlParameters;
	NestConst<ValueExprNode> messageExpr;
	NestConst<ValueListNode> parameters;
	NestConst<ExceptionItem> exception;
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
	virtual ForNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual StmtNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual StmtNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	SelectNode* dsqlSelect;
	Firebird::Array<dsql_nod*>* dsqlInto;
	dsql_nod* dsqlCursor;
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


class LineColumnNode : public TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_LINE_COLUMN>
{
public:
	explicit LineColumnNode(MemoryPool& pool, USHORT aLine, USHORT aColumn, StmtNode* aStatement)
		: TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_LINE_COLUMN>(pool),
		  statement(aStatement)
	{
		line = aLine;
		column = aColumn;
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual LineColumnNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

private:
	NestConst<StmtNode> statement;
};


class LoopNode : public TypedNode<StmtNode, StmtNode::TYPE_LOOP>
{
public:
	explicit LoopNode(MemoryPool& pool)
		: TypedNode<StmtNode, StmtNode::TYPE_LOOP>(pool),
		  dsqlLabel(NULL),
		  dsqlExpr(NULL),
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
	dsql_nod* dsqlLabel;
	dsql_nod* dsqlExpr;
	NestConst<StmtNode> statement;
};


class MergeNode : public TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_MERGE>
{
public:
	struct Matched
	{
		Matched()
			: assignments(NULL),
			  condition(NULL)
		{
		}

		CompoundStmtNode* assignments;
		dsql_nod* condition;
	};

	struct NotMatched
	{
		NotMatched()
			: fields(NULL),
			  values(NULL),
			  condition(NULL)
		{
		}

		dsql_nod* fields;
		dsql_nod* values;
		dsql_nod* condition;
	};

	explicit MergeNode(MemoryPool& pool)
		: TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_MERGE>(pool),
		  dsqlRelation(NULL),
		  dsqlUsing(NULL),
		  dsqlCondition(NULL),
		  dsqlWhenMatched(pool),
		  dsqlWhenNotMatched(pool),
		  dsqlReturning(NULL)
	{
	}

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual StmtNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

public:
	dsql_nod* dsqlRelation;
	dsql_nod* dsqlUsing;
	dsql_nod* dsqlCondition;
	Firebird::Array<Matched> dsqlWhenMatched;
	Firebird::Array<NotMatched> dsqlWhenNotMatched;
	ReturningClause* dsqlReturning;
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
		  dsqlRelation(NULL),
		  dsqlBoolean(NULL),
		  dsqlPlan(NULL),
		  dsqlSort(NULL),
		  dsqlRows(NULL),
		  dsqlCursor(NULL),
		  dsqlReturning(NULL),
		  dsqlRseFlags(0),
		  dsqlRse(NULL),
		  dsqlContext(NULL),
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
	StmtNode* internalDsqlPass(DsqlCompilerScratch* dsqlScratch, bool updateOrInsert);
	virtual StmtNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual ModifyNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ModifyNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

private:
	static void pass1Modify(thread_db* tdbb, CompilerScratch* csb, ModifyNode* node);
	const StmtNode* modify(thread_db* tdbb, jrd_req* request, WhichTrigger whichTrig) const;

public:
	dsql_nod* dsqlRelation;
	dsql_nod* dsqlBoolean;
	dsql_nod* dsqlPlan;
	dsql_nod* dsqlSort;
	dsql_nod* dsqlRows;
	dsql_nod* dsqlCursor;
	ReturningClause* dsqlReturning;
	USHORT dsqlRseFlags;
	dsql_nod* dsqlRse;
	dsql_ctx* dsqlContext;
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
		  dsqlRelation(NULL),
		  dsqlFields(NULL),
		  dsqlValues(NULL),
		  dsqlReturning(NULL),
		  dsqlRse(NULL),
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
	StmtNode* internalDsqlPass(DsqlCompilerScratch* dsqlScratch, bool updateOrInsert);
	virtual StmtNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual StoreNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual StoreNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

private:
	static bool pass1Store(thread_db* tdbb, CompilerScratch* csb, StoreNode* node);
	void makeDefaults(thread_db* tdbb, CompilerScratch* csb);
	const StmtNode* store(thread_db* tdbb, jrd_req* request, WhichTrigger whichTrig) const;

public:
	dsql_nod* dsqlRelation;
	dsql_nod* dsqlFields;
	dsql_nod* dsqlValues;
	ReturningClause* dsqlReturning;
	dsql_nod* dsqlRse;
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
		  dsqlExpr(NULL),
		  dsqlForUpdate(false),
		  dsqlWithLock(false),
		  dsqlRse(NULL),
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
	dsql_nod* dsqlExpr;
	bool dsqlForUpdate;
	bool dsqlWithLock;
	dsql_nod* dsqlRse;
	Firebird::Array<NestConst<StmtNode> > statements;
};


class SetGeneratorNode : public TypedNode<StmtNode, StmtNode::TYPE_SET_GENERATOR>
{
public:
	SetGeneratorNode(MemoryPool& pool, const Firebird::MetaName& aName, dsql_nod* aDsqlValue = NULL)
		: TypedNode<StmtNode, StmtNode::TYPE_SET_GENERATOR>(pool),
		  name(aName),
		  dsqlValue(aDsqlValue),
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
	Firebird::MetaName name;
	dsql_nod* dsqlValue;
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

	virtual SavePointNode* pass1(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
	{
		return this;
	}

	virtual SavePointNode* pass2(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
	{
		return this;
	}

	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const;

public:
	UCHAR blrOp;
};


class SetTransactionNode : public TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_SET_TRANSACTION>
{
public:
	enum
	{
		ISO_LEVEL_CONCURRENCY,
		ISO_LEVEL_CONSISTENCY,
		ISO_LEVEL_READ_COMMITTED_REC_VERSION,
		ISO_LEVEL_READ_COMMITTED_NO_REC_VERSION
	};

public:
	explicit SetTransactionNode(MemoryPool& pool)
		: TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_SET_TRANSACTION>(pool)
	{
	}

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const
	{
		text = "SetTransactionNode";
	}

	virtual SetTransactionNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);

	virtual void genBlr(DsqlCompilerScratch* dsqlScratch)
	{
	}

private:
	void genTableLock(DsqlCompilerScratch* dsqlScratch, const dsql_nod* tblLock, USHORT lockLevel);

public:
	Nullable<bool> readOnly;
	Nullable<bool> wait;
	Nullable<unsigned> isoLevel;
	Nullable<bool> noAutoUndo;
	Nullable<bool> ignoreLimbo;
	Nullable<bool> restartRequests;
	Nullable<USHORT> lockTimeout;
	Nullable<dsql_nod*> reserveList;
};


class UpdateOrInsertNode : public TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_UPDATE_OR_INSERT>
{
public:
	explicit UpdateOrInsertNode(MemoryPool& pool)
		: TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_UPDATE_OR_INSERT>(pool),
		  dsqlRelation(NULL),
		  dsqlFields(NULL),
		  dsqlValues(NULL),
		  dsqlMatching(NULL),
		  dsqlReturning(NULL)
	{
	}

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual StmtNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

public:
	dsql_nod* dsqlRelation;
	dsql_nod* dsqlFields;
	dsql_nod* dsqlValues;
	dsql_nod* dsqlMatching;
	ReturningClause* dsqlReturning;
};


} // namespace

#endif // DSQL_STMT_NODES_H
