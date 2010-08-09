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

#ifndef DSQL_EXPR_NODES_H
#define DSQL_EXPR_NODES_H

#include "../jrd/common.h"
#include "../jrd/blr.h"
#include "../dsql/Nodes.h"

class SysFunction;

namespace Jrd {


class ConcatenateNode : public TypedNode<ExprNode, ExprNode::TYPE_CONCATENATE>
{
public:
	explicit ConcatenateNode(MemoryPool& pool, dsql_nod* aArg1 = NULL, dsql_nod* aArg2 = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void setParameterName(dsql_par* parameter) const;
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar) const;
	virtual void genBlr();
	virtual void make(dsc* desc, dsql_nod* nullReplacement);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

protected:
	virtual ExprNode* internalDsqlPass();

public:
	dsql_nod* dsqlArg1;
	dsql_nod* dsqlArg2;
	NestConst<jrd_nod> arg1;
	NestConst<jrd_nod> arg2;
};


// OVER is used only in DSQL. In the engine, normal aggregate functions are used in partitioned
// maps.
class OverNode : public TypedNode<ExprNode, ExprNode::TYPE_OVER>
{
public:
	explicit OverNode(MemoryPool& pool, dsql_nod* aAggExpr = NULL, dsql_nod* aPartition = NULL,
		dsql_nod* aOrder = NULL);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;

	virtual bool dsqlAggregateFinder(AggregateFinder& visitor);
	virtual bool dsqlAggregate2Finder(Aggregate2Finder& visitor);
	virtual bool dsqlInvalidReferenceFinder(InvalidReferenceFinder& visitor);
	virtual bool dsqlSubSelectFinder(SubSelectFinder& visitor);
	virtual bool dsqlFieldRemapper(FieldRemapper& visitor);

	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr();
	virtual void make(dsc* desc, dsql_nod* nullReplacement);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

protected:
	virtual ExprNode* internalDsqlPass();

public:
	dsql_nod* dsqlAggExpr;
	dsql_nod* dsqlPartition;
	dsql_nod* dsqlOrder;
};


class SubstringSimilarNode : public TypedNode<ExprNode, ExprNode::TYPE_SUBSTRING_SIMILAR>
{
public:
	explicit SubstringSimilarNode(MemoryPool& pool, dsql_nod* aExpr = NULL,
		dsql_nod* aPattern = NULL, dsql_nod* aEscape = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void setParameterName(dsql_par* parameter) const;
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar) const;
	virtual void genBlr();
	virtual void make(dsc* desc, dsql_nod* nullReplacement);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

protected:
	virtual ExprNode* internalDsqlPass();

public:
	dsql_nod* dsqlExpr;
	dsql_nod* dsqlPattern;
	dsql_nod* dsqlEscape;
	NestConst<jrd_nod> expr;
	NestConst<jrd_nod> pattern;
	NestConst<jrd_nod> escape;
};


class SysFuncCallNode : public TypedNode<ExprNode, ExprNode::TYPE_SYSFUNC_CALL>
{
public:
	explicit SysFuncCallNode(MemoryPool& pool, const Firebird::MetaName& aName,
		dsql_nod* aArgs = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr();
	virtual void make(dsc* desc, dsql_nod* nullReplacement);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

protected:
	virtual ExprNode* internalDsqlPass();

public:
	Firebird::MetaName name;
	dsql_nod* dsqlArgs;
	bool dsqlSpecialSyntax;
	NestConst<jrd_nod> args;
	const SysFunction* function;
};


class UdfCallNode : public TypedNode<ExprNode, ExprNode::TYPE_UDF_CALL>
{
public:
	explicit UdfCallNode(MemoryPool& pool, const Firebird::QualifiedName& aName,
		dsql_nod* aArgs = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr();
	virtual void make(dsc* desc, dsql_nod* nullReplacement);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual ExprNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

protected:
	virtual ExprNode* internalDsqlPass();

public:
	Firebird::QualifiedName name;
	dsql_nod* dsqlArgs;
	NestConst<jrd_nod> args;
	/*const*/ Function* function;

private:
	dsql_udf* dsqlFunction;
};


} // namespace

#endif // DSQL_EXPR_NODES_H
