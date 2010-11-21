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
 *  Copyright (c) 2010 Adriano dos Santos Fernandes <adrianosf@gmail.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef DSQL_BOOL_NODES_H
#define DSQL_BOOL_NODES_H

#include "../common/common.h"
#include "../jrd/blr.h"
#include "../dsql/Nodes.h"

namespace Jrd {

class RecordSource;


class BinaryBoolNode : public TypedNode<BoolExprNode, ExprNode::TYPE_BINARY_BOOL>
{
public:
	BinaryBoolNode(MemoryPool& pool, UCHAR aBlrOp, dsql_nod* aArg1 = NULL, dsql_nod* aArg2 = NULL);

	static BoolExprNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual BoolExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

	virtual BoolExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/;
	virtual bool execute(thread_db* tdbb, jrd_req* request) const;

private:
	virtual bool executeAnd(thread_db* tdbb, jrd_req* request) const;
	virtual bool executeOr(thread_db* tdbb, jrd_req* request) const;

public:
	UCHAR blrOp;
	dsql_nod* dsqlArg1;
	dsql_nod* dsqlArg2;
	NestConst<BoolExprNode> arg1;
	NestConst<BoolExprNode> arg2;
};


class ComparativeBoolNode : public TypedNode<BoolExprNode, ExprNode::TYPE_COMPARATIVE_BOOL>
{
public:
	enum DsqlFlag
	{
		DFLAG_NONE,
		DFLAG_ANSI_ALL,
		DFLAG_ANSI_ANY
	};

	ComparativeBoolNode(MemoryPool& pool, UCHAR aBlrOp, dsql_nod* aArg1 = NULL,
		dsql_nod* aArg2 = NULL, dsql_nod* aArg3 = NULL);

	static BoolExprNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual BoolExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

	virtual bool jrdPossibleUnknownFinder()
	{
		return blrOp == blr_equiv ? true : BoolExprNode::jrdPossibleUnknownFinder();
	}

	virtual BoolExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/;
	virtual BoolExprNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass2Boolean1(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass2Boolean2(thread_db* tdbb, CompilerScratch* csb);
	virtual bool execute(thread_db* tdbb, jrd_req* request) const;

private:
	bool stringBoolean(thread_db* tdbb, jrd_req* request, dsc* desc1, dsc* desc2,
		bool computedInvariant) const;
	bool stringFunction(thread_db* tdbb, jrd_req* request, SLONG l1, const UCHAR* p1,
		SLONG l2, const UCHAR* p2, USHORT ttype, bool computedInvariant) const;
	bool sleuth(thread_db* tdbb, jrd_req* request, const dsc* desc1, const dsc* desc2) const;

	BoolExprNode* createRseNode(DsqlCompilerScratch* dsqlScratch, UCHAR rseBlrOp);

public:
	UCHAR blrOp;
	dsql_nod* dsqlArg1;
	dsql_nod* dsqlArg2;
	dsql_nod* dsqlArg3;
	DsqlFlag dsqlFlag;
	NestConst<ValueExprNode> arg1;
	NestConst<ValueExprNode> arg2;
	NestConst<ValueExprNode> arg3;

};


class MissingBoolNode : public TypedNode<BoolExprNode, ExprNode::TYPE_MISSING_BOOL>
{
public:
	MissingBoolNode(MemoryPool& pool, dsql_nod* aArg = NULL);

	static BoolExprNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual BoolExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

	virtual bool jrdPossibleUnknownFinder()
	{
		return true;
	}

	virtual BoolExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual BoolExprNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass2Boolean2(thread_db* tdbb, CompilerScratch* csb);
	virtual bool execute(thread_db* tdbb, jrd_req* request) const;

public:
	dsql_nod* dsqlArg;
	NestConst<ValueExprNode> arg;
};


class NotBoolNode : public TypedNode<BoolExprNode, ExprNode::TYPE_NOT_BOOL>
{
public:
	NotBoolNode(MemoryPool& pool, dsql_nod* aArg = NULL);

	static BoolExprNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual BoolExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

	virtual bool jrdPossibleUnknownFinder()
	{
		return true;
	}

	virtual BoolExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual BoolExprNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual bool execute(thread_db* tdbb, jrd_req* request) const;

private:
	BoolExprNode* process(DsqlCompilerScratch* dsqlScratch, bool invert);

public:
	dsql_nod* dsqlArg;
	NestConst<BoolExprNode> arg;
};


class RseBoolNode : public TypedNode<BoolExprNode, ExprNode::TYPE_RSE_BOOL>
{
public:
	RseBoolNode(MemoryPool& pool, UCHAR aBlrOp, dsql_nod* aDsqlRse = NULL);

	static BoolExprNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual BoolExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

	virtual bool dsqlAggregateFinder(AggregateFinder& visitor)
	{
		fb_assert(blrOp != blr_any && blrOp != blr_ansi_any && blrOp != blr_ansi_all);
		return visitor.ignoreSubSelects ? false : BoolExprNode::dsqlVisit(visitor);
	}

	virtual bool jrdPossibleUnknownFinder()
	{
		return true;
	}

	virtual BoolExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/;
	virtual BoolExprNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass2Boolean1(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass2Boolean2(thread_db* tdbb, CompilerScratch* csb);
	virtual bool execute(thread_db* tdbb, jrd_req* request) const;

private:
	BoolExprNode* convertNeqAllToNotAny(thread_db* tdbb, CompilerScratch* csb);

public:
	UCHAR blrOp;
	dsql_nod* dsqlRse;
	NestConst<RseNode> rse;
	NestConst<RecordSource> rsb;
};


} // namespace

#endif // DSQL_BOOL_NODES_H
