/*
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
 * Adriano dos Santos Fernandes - refactored from pass1.cpp
 */

#ifndef DSQL_VISITORS_H
#define DSQL_VISITORS_H

#include "../common/common.h"
#include "../dsql/dsql.h"
#include "../dsql/node.h"
#include "../common/classes/array.h"
#include "../common/classes/auto.h"
#include "../common/classes/NestConst.h"

namespace Jrd {

class CompilerScratch;
class dsql_nod;
class ExprNode;
class FieldNode;
class MapNode;
class MessageNode;


enum FieldMatchType
{
	FIELD_MATCH_TYPE_EQUAL = 0,
	FIELD_MATCH_TYPE_LOWER = 1,
	FIELD_MATCH_TYPE_LOWER_EQUAL = 2
	/***
	,
	FIELD_MATCH_TYPE_HIGHER = 3,
	FIELD_MATCH_TYPE_HIGHER_EQUAL = 4
	***/
};


// Base class to call a visitor of an ExprNode.
class VisitorCaller
{
public:
	virtual bool call(ExprNode* exprNode) = 0;
};

// Implementation class to call a visitor of an ExprNode.
template <typename T>
class VisitorCallerImpl : public VisitorCaller
{
public:
	VisitorCallerImpl(T& aVisitor, bool (ExprNode::*aVisitFunction)(T&))
		: visitor(aVisitor),
		  visitFunction(aVisitFunction)
	{
	}

	virtual bool call(ExprNode* exprNode)
	{
		return (exprNode->*visitFunction)(visitor);
	}

private:
	T& visitor;
	bool (ExprNode::*visitFunction)(T&);
};


template <typename T, typename T2>
class DsqlNodeVisitor
{
public:
	template <typename Self>
	DsqlNodeVisitor(bool aAssertOnOthers, bool aReturnOnOthers, bool (ExprNode::*aVisitFunction)(Self&))
		: assertOnOthers(aAssertOnOthers),
		  returnOnOthers(aReturnOnOthers),
		  currentNode(NULL)
	{
		caller = FB_NEW(*getDefaultMemoryPool()) VisitorCallerImpl<Self>(
			static_cast<Self&>(*this), aVisitFunction);
	}

public:
	T getCurrentNode()
	{
		return *currentNode;
	}

	void replaceNode(T node)
	{
		*currentNode = node;
	}

	bool visit(T2 nodePtr)
	{
		Firebird::AutoSetRestore<T2> autoInsideHigherMap(&currentNode, nodePtr);
		return internalVisit(*nodePtr);
	}

	bool visitChildren(T node);

	virtual bool internalVisit(T node) = 0;

private:
	bool call(ExprNode* exprNode)
	{
		return caller->call(exprNode);
	}

private:
	const bool assertOnOthers;
	const bool returnOnOthers;
	Firebird::AutoPtr<VisitorCaller> caller;
	T2 currentNode;
};

typedef DsqlNodeVisitor<const Jrd::dsql_nod*, const Jrd::dsql_nod* const*> ConstDsqlNodeVisitor;
typedef DsqlNodeVisitor<Jrd::dsql_nod*, Jrd::dsql_nod**> NonConstDsqlNodeVisitor;


// Check for an aggregate expression in an expression. It could be buried in an expression
// tree and therefore call itselfs again. The level parameters (currentLevel & deepestLevel)
// are used to see how deep we are with passing sub-queries (= scope_level).
class AggregateFinder : public ConstDsqlNodeVisitor
{
public:
	AggregateFinder(const DsqlCompilerScratch* aDsqlScratch, bool aWindow);

	static bool find(const DsqlCompilerScratch* dsqlScratch, bool window, const dsql_nod* node)
	{
		return AggregateFinder(dsqlScratch, window).visit(&node);
	}

	virtual bool internalVisit(const dsql_nod* node);

public:
	const DsqlCompilerScratch* const dsqlScratch;
	bool window;
	USHORT currentLevel;
	USHORT deepestLevel;
	bool ignoreSubSelects;
};

// Check the fields inside an aggregate and check if the field scope_level meets the specified
// conditions.
//
// The SQL 2008 standard says:
// <where clause> ::=
//   WHERE <search condition>
// Syntax Rules
// 1) If a <value expression> directly contained in the <search condition> is a
// <set function specification>, then the <where clause> shall be contained in a <having clause>
// or <select list>, the <set function specification> shall contain a column reference, and
// every column reference contained in an aggregated argument of the <set function specification>
// shall be an outer reference.
//    NOTE 160 - outer reference is defined in Subclause 6.7, "<column reference>".
// 2) The <search condition> shall not contain a <window function> without an intervening
// <query expression>.
class Aggregate2Finder : public ConstDsqlNodeVisitor
{
public:
	Aggregate2Finder(USHORT aCheckScopeLevel, FieldMatchType aMatchType, bool aWindowOnly);

	static bool find(USHORT checkScopeLevel, FieldMatchType matchType, bool windowOnly,
		const dsql_nod* node)
	{
		return Aggregate2Finder(checkScopeLevel, matchType, windowOnly).visit(&node);
	}

public:
	virtual bool internalVisit(const dsql_nod* node);

public:
	const USHORT checkScopeLevel;
	const FieldMatchType matchType;
	bool windowOnly;
	bool currentScopeLevelEqual;
};

// Check the fields inside an aggregate and check if the field scope_level meets the specified
// conditions.
class FieldFinder : public ConstDsqlNodeVisitor
{
public:
	FieldFinder(USHORT aCheckScopeLevel, FieldMatchType aMatchType);

	static bool find(USHORT checkScopeLevel, FieldMatchType matchType, const dsql_nod* node)
	{
		return FieldFinder(checkScopeLevel, matchType).visit(&node);
	}

	virtual bool internalVisit(const dsql_nod* node);

public:
	bool getField() const
	{
		return field;
	}

public:
	const USHORT checkScopeLevel;
	const FieldMatchType matchType;
	bool field;
};

// Validate that an expanded field / context pair is in a specified list. This is used in one
// instance to check that a simple field selected through a grouping rse is a grouping field -
// thus a valid field reference. For the sake of argument, we'll match qualified to unqualified
// reference, but qualified reference must match completely.
// A list element containing a simple CAST for collation purposes is allowed.
class InvalidReferenceFinder : public ConstDsqlNodeVisitor
{
public:
	InvalidReferenceFinder(const dsql_ctx* aContext, const dsql_nod* aList);

	static bool find(const dsql_ctx* context, const dsql_nod* list, const dsql_nod* node)
	{
		return InvalidReferenceFinder(context, list).visit(&node);
	}

	virtual bool internalVisit(const dsql_nod* node);

public:
	const dsql_ctx* const context;
	const dsql_nod* const list;
	bool insideOwnMap;
	bool insideHigherMap;
};

// Called to map fields used in an aggregate-context after all pass1 calls
// (SELECT-, ORDER BY-lists). Walk completly through the given node 'field' and map the fields
// with same scope_level as the given context to the given context with the post_map function.
class FieldRemapper : public NonConstDsqlNodeVisitor
{
public:
	FieldRemapper(DsqlCompilerScratch* aDsqlScratch, dsql_ctx* aContext, bool aWindow,
				dsql_nod* aPartitionNode, dsql_nod* aOrderNode);

	static dsql_nod* remap(DsqlCompilerScratch* dsqlScratch, dsql_ctx* context, bool window,
		dsql_nod* field, dsql_nod* partitionNode = NULL, dsql_nod* orderNode = NULL)
	{
		// The bool value returned by the visitor is completely discarded in this class.
		FieldRemapper(dsqlScratch, context, window, partitionNode, orderNode).visit(&field);
		return field;
	}

	virtual bool internalVisit(dsql_nod* node);

public:
	DsqlCompilerScratch* const dsqlScratch;
	dsql_ctx* const context;
	const bool window;
	dsql_nod* partitionNode;
	dsql_nod* orderNode;
	USHORT currentLevel;
};

// Search if a sub select is buried inside a select list from a query expression.
class SubSelectFinder : public ConstDsqlNodeVisitor
{
public:
	SubSelectFinder();

	static bool find(const dsql_nod* node)
	{
		return SubSelectFinder().visit(&node);
	}

	virtual bool internalVisit(const dsql_nod* node);
};


// Generic node copier.
class NodeCopier
{
public:
	NodeCopier(CompilerScratch* aCsb, UCHAR* aRemap)
		: csb(aCsb),
		  remap(aRemap),
		  message(NULL)
	{
	}

	virtual ~NodeCopier()
	{
	}

public:
	// Copy an expression tree remapping field streams. If the map isn't present, don't remap.
	template <typename T>
	T* copy(thread_db* tdbb, T* input)
	{
		if (!input)
			return NULL;

		T* copy = input->copy(tdbb, *this);
		postCopy(input, copy, copy);

		return copy;
	}

	template <typename T>
	T* copy(thread_db* tdbb, NestConst<T>& input)
	{
		return copy(tdbb, input.getObject());
	}

	template <typename T>
	static T* copy(thread_db* tdbb, CompilerScratch* csb, T* input, UCHAR* remap)
	{
		return NodeCopier(csb, remap).copy(tdbb, input);
	}

	template <typename T>
	static T* copy(thread_db* tdbb, CompilerScratch* csb, NestConst<T>& input, UCHAR* remap)
	{
		return NodeCopier(csb, remap).copy(tdbb, input.getObject());
	}

	virtual bool remapArgument()
	{
		return false;
	}

	virtual USHORT remapField(USHORT /*stream*/, USHORT fldId)
	{
		return fldId;
	}

	virtual USHORT getFieldId(FieldNode* input);

private:
	template <typename T1, typename T2> void postCopy(T1* source, T2* target, ExprNode* /*dummy*/)
	{
		target->nodFlags = source->nodFlags;
	}

	void postCopy(StmtNode* /*source*/, StmtNode* /*target*/, StmtNode* /*dummy*/)
	{
	}

public:
	CompilerScratch* csb;
	UCHAR* remap;
	MessageNode* message;
};


template <typename T, typename T2>
inline bool DsqlNodeVisitor<T, T2>::visitChildren(T node)
{
	using namespace Firebird;
	using namespace Dsql;

	bool ret = false;

	if (!node)
		return ret;

	switch (node->nod_type)
	{
		case nod_class_exprnode:
		{
			Jrd::ExprNode* exprNode = reinterpret_cast<Jrd::ExprNode*>(node->nod_arg[0]);
			ret |= call(exprNode);
			break;
		}

		case nod_dom_value:
			break;

		case nod_alias:
			ret |= visit(&node->nod_arg[e_alias_value]);
			break;

		case nod_order:
			ret |= visit(&node->nod_arg[e_order_field]);
			break;

		case nod_searched_case:
		case nod_list:
		{
			T2 ptr = node->nod_arg;
			for (T2 end = ptr + node->nod_count; ptr < end; ++ptr)
				ret |= visit(ptr);
			break;
		}

		default:
			fb_assert(!assertOnOthers);
			return returnOnOthers;
	}

	return ret;
}


} // namespace

#endif // DSQL_VISITORS_H
