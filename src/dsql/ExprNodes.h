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

#include "../common/common.h"
#include "../jrd/blr.h"
#include "../dsql/Nodes.h"

class SysFunction;

namespace Jrd {

struct ItemInfo;
class RecordSource;


class ArithmeticNode : public TypedNode<ValueExprNode, ExprNode::TYPE_ARITHMETIC>
{
public:
	ArithmeticNode(MemoryPool& pool, UCHAR aBlrOp, bool aDialect1,
		dsql_nod* aArg1 = NULL, dsql_nod* aArg2 = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/;
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

	// add and add2 are used in somewhat obscure way in aggregation.
	static dsc* add(const dsc* desc, impure_value* value, const jrd_nod* node, UCHAR blrOp);
	static dsc* add2(const dsc* desc, impure_value* value, const jrd_nod* node, UCHAR blrOp);

private:
	dsc* multiply(const dsc* desc, impure_value* value) const;
	dsc* multiply2(const dsc* desc, impure_value* value) const;
	dsc* divide2(const dsc* desc, impure_value* value) const;
	dsc* addDateTime(const dsc* desc, impure_value* value) const;
	dsc* addSqlDate(const dsc* desc, impure_value* value) const;
	dsc* addSqlTime(const dsc* desc, impure_value* value) const;
	dsc* addTimeStamp(const dsc* desc, impure_value* value) const;

private:
	void makeDialect1(dsc* desc, dsc& desc1, dsc& desc2);
	void makeDialect3(dsc* desc, dsc& desc1, dsc& desc2);

	void getDescDialect1(thread_db* tdbb, dsc* desc, dsc& desc1, dsc& desc2);
	void getDescDialect3(thread_db* tdbb, dsc* desc, dsc& desc1, dsc& desc2);

public:
	UCHAR blrOp;
	bool dialect1;
	Firebird::string label;
	dsql_nod* dsqlArg1;
	dsql_nod* dsqlArg2;
	NestConst<jrd_nod> arg1;
	NestConst<jrd_nod> arg2;
};


class CastNode : public TypedNode<ValueExprNode, ExprNode::TYPE_CAST>
{
public:
	CastNode(MemoryPool& pool, dsql_nod* aDsqlSource = NULL, dsql_fld* aDsqlField = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/;
	virtual ExprNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	dsql_nod* dsqlSource;
	dsql_fld* dsqlField;
	dsc castDesc;
	NestConst<jrd_nod> source;
	NestConst<Format> format;
	NestConst<ItemInfo> itemInfo;
};


class ConcatenateNode : public TypedNode<ValueExprNode, ExprNode::TYPE_CONCATENATE>
{
public:
	explicit ConcatenateNode(MemoryPool& pool, dsql_nod* aArg1 = NULL, dsql_nod* aArg2 = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	dsql_nod* dsqlArg1;
	dsql_nod* dsqlArg2;
	NestConst<jrd_nod> arg1;
	NestConst<jrd_nod> arg2;
};


class CurrentDateNode : public TypedNode<ValueExprNode, ExprNode::TYPE_CURRENT_DATE>
{
public:
	explicit CurrentDateNode(MemoryPool& pool)
		: TypedNode<ValueExprNode, ExprNode::TYPE_CURRENT_DATE>(pool)
	{
	}

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;
};


class CurrentTimeNode : public TypedNode<ValueExprNode, ExprNode::TYPE_CURRENT_TIME>
{
public:
	CurrentTimeNode(MemoryPool& pool, unsigned aPrecision)
		: TypedNode<ValueExprNode, ExprNode::TYPE_CURRENT_TIME>(pool),
		  precision(aPrecision)
	{
	}

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	unsigned precision;
};


class CurrentTimeStampNode : public TypedNode<ValueExprNode, ExprNode::TYPE_CURRENT_TIMESTAMP>
{
public:
	CurrentTimeStampNode(MemoryPool& pool, unsigned aPrecision)
		: TypedNode<ValueExprNode, ExprNode::TYPE_CURRENT_TIMESTAMP>(pool),
		  precision(aPrecision)
	{
	}

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	unsigned precision;
};


class CurrentRoleNode : public TypedNode<ValueExprNode, ExprNode::TYPE_CURRENT_ROLE>
{
public:
	explicit CurrentRoleNode(MemoryPool& pool)
		: TypedNode<ValueExprNode, ExprNode::TYPE_CURRENT_ROLE>(pool)
	{
	}

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;
};


class CurrentUserNode : public TypedNode<ValueExprNode, ExprNode::TYPE_CURRENT_USER>
{
public:
	explicit CurrentUserNode(MemoryPool& pool)
		: TypedNode<ValueExprNode, ExprNode::TYPE_CURRENT_USER>(pool)
	{
	}

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;
};


class DerivedExprNode : public TypedNode<ValueExprNode, ExprNode::TYPE_DERIVED_EXPR>
{
public:
	DerivedExprNode(MemoryPool& pool, dsql_nod* aArg = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	// This is a non-DSQL node.

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const
	{
		fb_assert(false);
	}

	virtual void setParameterName(dsql_par* parameter) const
	{
		fb_assert(false);
	}

	virtual void genBlr(DsqlCompilerScratch* dsqlScratch)
	{
		fb_assert(false);
	}

	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
	{
		fb_assert(false);
	}

	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idxUse,
		bool allowOnlyCurrentStream);

	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	dsql_nod* dsqlArg;
	NestConst<jrd_nod> arg;
	Firebird::Array<USHORT> streamList;
};


class DomainValidationNode : public TypedNode<ValueExprNode, ExprNode::TYPE_DOMAIN_VALIDATION>
{
public:
	explicit DomainValidationNode(MemoryPool& pool)
		: TypedNode<ValueExprNode, ExprNode::TYPE_DOMAIN_VALIDATION>(pool)
	{
		domDesc.clear();
	}

	// This is a non-DSQL node.

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const
	{
		fb_assert(false);
	}

	virtual void setParameterName(dsql_par* parameter) const
	{
		fb_assert(false);
	}

	virtual void genBlr(DsqlCompilerScratch* dsqlScratch)
	{
		fb_assert(false);
	}

	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc)
	{
		fb_assert(false);
	}

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	dsc domDesc;
};


class ExtractNode : public TypedNode<ValueExprNode, ExprNode::TYPE_EXTRACT>
{
public:
	ExtractNode(MemoryPool& pool, UCHAR aBlrSubOp, dsql_nod* aArg = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/;
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	UCHAR blrSubOp;
	dsql_nod* dsqlArg;
	NestConst<jrd_nod> arg;
};


class GenIdNode : public TypedNode<ValueExprNode, ExprNode::TYPE_GEN_ID>
{
public:
	GenIdNode(MemoryPool& pool, bool aDialect1, const Firebird::MetaName& aName,
		dsql_nod* aArg = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar) const;
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual bool jrdVisit(JrdNodeVisitor& visitor)
	{
		return false;
	}

	virtual bool jrdUnmappedNodeGetter(UnmappedNodeGetter& visitor)
	{
		return false;
	}

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/;
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	bool dialect1;
	Firebird::MetaName name;
	dsql_nod* dsqlArg;
	NestConst<jrd_nod> arg;
	SLONG id;
};


class InternalInfoNode : public TypedNode<ValueExprNode, ExprNode::TYPE_INTERNAL_INFO>
{
public:
	// Constants stored in BLR.
	enum InfoType
	{
		INFO_TYPE_UNKNOWN = 0,
		INFO_TYPE_CONNECTION_ID = 1,
		INFO_TYPE_TRANSACTION_ID = 2,
		INFO_TYPE_GDSCODE = 3,
		INFO_TYPE_SQLCODE = 4,
		INFO_TYPE_ROWS_AFFECTED = 5,
		INFO_TYPE_TRIGGER_ACTION = 6,
		INFO_TYPE_SQLSTATE = 7,
		MAX_INFO_TYPE
	};

	struct InfoAttr
	{
		const char* alias;
		unsigned mask;
	};

	static const InfoAttr INFO_TYPE_ATTRIBUTES[MAX_INFO_TYPE];

	explicit InternalInfoNode(MemoryPool& pool, dsql_nod* aArg = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	dsql_nod* dsqlArg;
	NestConst<jrd_nod> arg;
};


class LiteralNode : public TypedNode<ValueExprNode, ExprNode::TYPE_LITERAL>
{
public:
	LiteralNode(MemoryPool& pool);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);
	static void genConstant(DsqlCompilerScratch* dsqlScratch, const dsc* desc, bool negateValue);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/;
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

	SLONG getSlong() const
	{
		fb_assert(litDesc.dsc_dtype == dtype_long);
		return *reinterpret_cast<SLONG*>(litDesc.dsc_address);
	}

public:
	dsql_str* dsqlStr;
	dsc litDesc;
};


class NegateNode : public TypedNode<ValueExprNode, ExprNode::TYPE_NEGATE>
{
public:
	explicit NegateNode(MemoryPool& pool, dsql_nod* aArg = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	dsql_nod* dsqlArg;
	NestConst<jrd_nod> arg;
};


class NullNode : public TypedNode<ValueExprNode, ExprNode::TYPE_NULL>
{
public:
	explicit NullNode(MemoryPool& pool)
		: TypedNode<ValueExprNode, ExprNode::TYPE_NULL>(pool)
	{
	}

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;
};


// OVER is used only in DSQL. In the engine, normal aggregate functions are used in partitioned
// maps.
class OverNode : public TypedNode<ValueExprNode, ExprNode::TYPE_OVER>
{
public:
	explicit OverNode(MemoryPool& pool, dsql_nod* aAggExpr = NULL, dsql_nod* aPartition = NULL,
		dsql_nod* aOrder = NULL);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);

	virtual bool dsqlAggregateFinder(AggregateFinder& visitor);
	virtual bool dsqlAggregate2Finder(Aggregate2Finder& visitor);
	virtual bool dsqlInvalidReferenceFinder(InvalidReferenceFinder& visitor);
	virtual bool dsqlSubSelectFinder(SubSelectFinder& visitor);
	virtual bool dsqlFieldRemapper(FieldRemapper& visitor);

	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	dsql_nod* dsqlAggExpr;
	dsql_nod* dsqlPartition;
	dsql_nod* dsqlOrder;
};


class ParameterNode : public TypedNode<ValueExprNode, ExprNode::TYPE_PARAMETER>
{
private:
	// CVC: This is a guess for the length of the parameter for LIKE and others, when the
	// original dtype isn't string and force_varchar is true.
	static const int LIKE_PARAM_LEN = 30;

public:
	ParameterNode(MemoryPool& pool);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);

	virtual void setParameterName(dsql_par* parameter) const
	{
	}

	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;

	virtual bool jrdVisit(JrdNodeVisitor& visitor)
	{
		return false;
	}

	virtual bool jrdUnmappedNodeGetter(UnmappedNodeGetter& visitor)
	{
		return false;
	}

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	USHORT dsqlParameterIndex;
	dsql_par* dsqlParameter;
	NestConst<jrd_nod> message;
	USHORT argNumber;
	NestConst<jrd_nod> argFlag;
	NestConst<jrd_nod> argIndicator;
	NestConst<ItemInfo> argInfo;
};


class StrCaseNode : public TypedNode<ValueExprNode, ExprNode::TYPE_STR_CASE>
{
public:
	StrCaseNode(MemoryPool& pool, UCHAR aBlrOp, dsql_nod* aArg = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/;
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	UCHAR blrOp;
	dsql_nod* dsqlArg;
	NestConst<jrd_nod> arg;
};


class StrLenNode : public TypedNode<ValueExprNode, ExprNode::TYPE_STR_LEN>
{
public:
	StrLenNode(MemoryPool& pool, UCHAR aBlrSubOp, dsql_nod* aArg = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/;
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	UCHAR blrSubOp;
	dsql_nod* dsqlArg;
	NestConst<jrd_nod> arg;
};


// This node is used for DSQL subqueries and for legacy (BLR-only) functionality.
class SubQueryNode : public TypedNode<ValueExprNode, ExprNode::TYPE_SUBQUERY>
{
public:
	explicit SubQueryNode(MemoryPool& pool, UCHAR aBlrOp, dsql_nod* aDsqlRse = NULL,
		dsql_nod* aValue1 = NULL, dsql_nod* aValue2 = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual bool dsqlAggregateFinder(AggregateFinder& visitor);
	virtual bool dsqlAggregate2Finder(Aggregate2Finder& visitor);
	virtual bool dsqlSubSelectFinder(SubSelectFinder& visitor);
	virtual bool dsqlFieldFinder(FieldFinder& visitor);
	virtual bool dsqlFieldRemapper(FieldRemapper& visitor);

	virtual bool jrdVisit(JrdNodeVisitor& visitor)
	{
		return false;
	}

	virtual bool jrdUnmappedNodeGetter(UnmappedNodeGetter& visitor)
	{
		return false;
	}

	virtual bool jrdPossibleUnknownFinder(PossibleUnknownFinder& /*visitor*/)
	{
		return true;
	}

	virtual bool jrdStreamFinder(StreamFinder& visitor);
	virtual bool jrdStreamsCollector(StreamsCollector& visitor);

	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idxUse,
		bool allowOnlyCurrentStream);

	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/;
	virtual ExprNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	UCHAR blrOp;
	dsql_nod* dsqlRse;
	dsql_nod* dsqlValue1;
	dsql_nod* dsqlValue2;
	NestConst<jrd_nod> rse;
	NestConst<jrd_nod> value1;
	NestConst<jrd_nod> value2;
	NestConst<RecordSource> rsb;
};


class SubstringNode : public TypedNode<ValueExprNode, ExprNode::TYPE_SUBSTRING>
{
public:
	explicit SubstringNode(MemoryPool& pool, dsql_nod* aExpr = NULL,
		dsql_nod* aStart = NULL, dsql_nod* aLength = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

	static dsc* perform(thread_db* tdbb, impure_value* impure, const dsc* valueDsc,
		const dsc* startDsc, const dsc* lengthDsc);

public:
	dsql_nod* dsqlExpr;
	dsql_nod* dsqlStart;
	dsql_nod* dsqlLength;
	NestConst<jrd_nod> expr;
	NestConst<jrd_nod> start;
	NestConst<jrd_nod> length;
};


class SubstringSimilarNode : public TypedNode<ValueExprNode, ExprNode::TYPE_SUBSTRING_SIMILAR>
{
public:
	explicit SubstringSimilarNode(MemoryPool& pool, dsql_nod* aExpr = NULL,
		dsql_nod* aPattern = NULL, dsql_nod* aEscape = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	dsql_nod* dsqlExpr;
	dsql_nod* dsqlPattern;
	dsql_nod* dsqlEscape;
	NestConst<jrd_nod> expr;
	NestConst<jrd_nod> pattern;
	NestConst<jrd_nod> escape;
};


class SysFuncCallNode : public TypedNode<ValueExprNode, ExprNode::TYPE_SYSFUNC_CALL>
{
public:
	explicit SysFuncCallNode(MemoryPool& pool, const Firebird::MetaName& aName,
		dsql_nod* aArgs = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/;
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	Firebird::MetaName name;
	dsql_nod* dsqlArgs;
	bool dsqlSpecialSyntax;
	NestConst<jrd_nod> args;
	const SysFunction* function;
};


class TrimNode : public TypedNode<ValueExprNode, ExprNode::TYPE_TRIM>
{
public:
	explicit TrimNode(MemoryPool& pool, UCHAR aWhere,
		dsql_nod* aValue = NULL, dsql_nod* aTrimChars = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/;
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	UCHAR where;
	dsql_nod* dsqlValue;
	dsql_nod* dsqlTrimChars;		// may be NULL
	NestConst<jrd_nod> value;
	NestConst<jrd_nod> trimChars;	// may be NULL
};


class UdfCallNode : public TypedNode<ValueExprNode, ExprNode::TYPE_UDF_CALL>
{
public:
	explicit UdfCallNode(MemoryPool& pool, const Firebird::QualifiedName& aName,
		dsql_nod* aArgs = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/;
	virtual ExprNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	Firebird::QualifiedName name;
	dsql_nod* dsqlArgs;
	NestConst<jrd_nod> args;
	NestConst<Function> function;

private:
	dsql_udf* dsqlFunction;
};


class ValueIfNode : public TypedNode<ValueExprNode, ExprNode::TYPE_VALUE_IF>
{
public:
	explicit ValueIfNode(MemoryPool& pool, dsql_nod* aCondition = NULL, dsql_nod* aTrueValue = NULL,
		dsql_nod* aFalseValue = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual bool setParameterType(DsqlCompilerScratch* dsqlScratch,
		dsql_nod* node, bool forceVarChar);
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);

	virtual bool jrdPossibleUnknownFinder(PossibleUnknownFinder& /*visitor*/)
	{
		return true;
	}

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	dsql_nod* dsqlCondition;
	dsql_nod* dsqlTrueValue;
	dsql_nod* dsqlFalseValue;
	NestConst<BoolExprNode> condition;
	NestConst<jrd_nod> trueValue;
	NestConst<jrd_nod> falseValue;
};


class VariableNode : public TypedNode<ValueExprNode, ExprNode::TYPE_VARIABLE>
{
public:
	VariableNode(MemoryPool& pool);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);
	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc);
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;

	virtual bool jrdVisit(JrdNodeVisitor& visitor)
	{
		return false;
	}

	virtual bool jrdUnmappedNodeGetter(UnmappedNodeGetter& visitor)
	{
		return false;
	}

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc);
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual ExprNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

public:
	dsql_var* dsqlVar;
	dsc varDesc;
	USHORT varId;
	NestConst<jrd_nod> varDecl;
	NestConst<ItemInfo> varInfo;
};


} // namespace

#endif // DSQL_EXPR_NODES_H
