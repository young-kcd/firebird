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
#include "../dsql/node.h"
#include "../dsql/Visitors.h"
#include "../common/classes/array.h"

namespace Jrd {

class AggregateSort;
class CompilerScratch;
class Cursor;
class dsql_nod;
class ExprNode;
class jrd_nod;
class RecordSelExpr;
class SlidingWindow;
class TypeClause;


template <typename T>
class RegisterNode
{
public:
	explicit RegisterNode(UCHAR blr)
	{
		PAR_register(blr, T::parse);
	}
};


class Node : public Firebird::PermanentStorage
{
public:
	explicit Node(MemoryPool& pool)
		: PermanentStorage(pool),
		  dsqlScratch(NULL)
	{
	}

	virtual ~Node()
	{
	}

public:
	Node* dsqlPass(DsqlCompilerScratch* aDsqlScratch)
	{
		dsqlScratch = aDsqlScratch;
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
	DsqlCompilerScratch* dsqlScratch;
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

	// Set the scratch's transaction when executing a node. Fact of accessing the scratch during
	// execution is a hack.
	void executeDdl(thread_db* tdbb, jrd_tra* transaction)
	{
		dsqlScratch->setTransaction(transaction);
		execute(tdbb, transaction);
	}

public:
	enum DdlTriggerWhen { DTW_BEFORE, DTW_AFTER };
	static void executeDdlTrigger(thread_db* tdbb, jrd_tra* transaction,
		DdlTriggerWhen when, int action, const Firebird::MetaName& objectName,
		const Firebird::string& sqlText);

protected:
	void executeDdlTrigger(thread_db* tdbb, jrd_tra* transaction,
		DdlTriggerWhen when, int action, const Firebird::MetaName& objectName);
	void putType(const TypeClause& type, bool useSubType);
	void resetContextStack();
	Firebird::MetaName storeGlobalField(thread_db* tdbb, jrd_tra* transaction,
		const TypeClause& field, Firebird::MetaName& name);

protected:
	virtual DdlNode* internalDsqlPass()
	{
		dsqlScratch->getStatement()->setType(DsqlCompiledStatement::TYPE_DDL);
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
	virtual DmlNode* pass1(thread_db* tdbb, CompilerScratch* csb, jrd_nod* aNode);
	virtual DmlNode* pass2(thread_db* tdbb, CompilerScratch* csb, jrd_nod* aNode);

public:
	virtual void genBlr() = 0;
	virtual DmlNode* pass1(thread_db* tdbb, CompilerScratch* csb) = 0;
	virtual DmlNode* pass2(thread_db* tdbb, CompilerScratch* csb) = 0;

protected:
	jrd_nod* node;
};


template <typename T, typename T::Type typeConst>
class TypedNode : public T
{
public:
	explicit TypedNode(MemoryPool& pool)
		: T(typeConst, pool)
	{
	}

public:
	const static typename T::Type TYPE = typeConst;
};


class ExprNode : public DmlNode
{
public:
	enum Type
	{
		TYPE_AGGREGATE,
		TYPE_CONCATENATE,
		TYPE_OVER,
		TYPE_SUBSTRING_SIMILAR,
		TYPE_SYSFUNC_CALL,
		TYPE_UDF_CALL
	};

	explicit ExprNode(Type aType, MemoryPool& pool)
		: DmlNode(pool),
		  type(aType),
		  dsqlCompatDialectVerb(NULL),
		  dsqlChildNodes(pool),
		  jrdChildNodes(pool)
	{
	}

	template <typename T> T* as()
	{
		return type == T::TYPE ? (T*) this : NULL;
	}

	template <typename T> const T* as() const
	{
		return type == T::TYPE ? (T*) this : NULL;
	}

	template <typename T> bool is() const
	{
		return type == T::TYPE;
	}

	template <typename T, typename LegacyType> static T* as(LegacyType* node)
	{
		ExprNode* obj = T::fromLegacy(node);
		return obj ? obj->as<T>() : NULL;
	}

	template <typename T, typename LegacyType> static const T* as(const LegacyType* node)
	{
		const ExprNode* obj = T::fromLegacy(node);
		return obj ? obj->as<T>() : NULL;
	}

	template <typename T, typename LegacyType> static bool is(const LegacyType* node)
	{
		const ExprNode* obj = T::fromLegacy(node);
		return obj ? obj->is<T>() : false;
	}

	static ExprNode* fromLegacy(const dsql_nod* node);
	static ExprNode* fromLegacy(const jrd_nod* node);

	virtual bool dsqlAggregateFinder(AggregateFinder& visitor)
	{
		return dsqlVisit(visitor);
	}

	virtual bool dsqlAggregate2Finder(Aggregate2Finder& visitor)
	{
		return dsqlVisit(visitor);
	}

	virtual bool dsqlFieldFinder(FieldFinder& visitor)
	{
		return dsqlVisit(visitor);
	}

	virtual bool dsqlInvalidReferenceFinder(InvalidReferenceFinder& visitor)
	{
		return dsqlVisit(visitor);
	}

	virtual bool dsqlSubSelectFinder(SubSelectFinder& visitor)
	{
		return dsqlVisit(visitor);
	}

	virtual bool dsqlFieldRemapper(FieldRemapper& visitor)
	{
		return dsqlVisit(visitor);
	}

	virtual bool jrdPossibleUnknownFinder(PossibleUnknownFinder& visitor)
	{
		return jrdVisit(visitor);
	}

	virtual bool jrdStreamFinder(StreamFinder& visitor)
	{
		return jrdVisit(visitor);
	}

	virtual bool jrdStreamsCollector(StreamsCollector& visitor)
	{
		return jrdVisit(visitor);
	}

	virtual bool jrdUnmappedNodeGetter(UnmappedNodeGetter& visitor)
	{
		visitor.nodeFound = node;
		return jrdVisit(visitor);
	}

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual ExprNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);

	virtual bool setParameterType(DsqlCompilerScratch* /*dsqlScratch*/,
		dsql_nod* /*node*/, bool /*forceVarChar*/) const
	{
		return false;
	}

	virtual void setParameterName(dsql_par* parameter) const = 0;
	virtual void make(dsc* desc, dsql_nod* nullReplacement) = 0;

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc) = 0;
	virtual ExprNode* copy(thread_db* tdbb, NodeCopier& copier) const = 0;
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const = 0;

protected:
	virtual bool dsqlVisit(ConstDsqlNodeVisitor& visitor);
	virtual bool dsqlVisit(NonConstDsqlNodeVisitor& visitor);
	virtual bool jrdVisit(JrdNodeVisitor& visitor);

	void addChildNode(dsql_nod*& dsqlNode, jrd_nod*& jrdNode)
	{
		dsqlChildNodes.add(&dsqlNode);
		jrdChildNodes.add(&jrdNode);
	}

	void addChildNode(dsql_nod*& dsqlNode)
	{
		dsqlChildNodes.add(&dsqlNode);
	}

public:
	const Type type;
	const char* dsqlCompatDialectVerb;
	Firebird::Array<dsql_nod**> dsqlChildNodes;
	Firebird::Array<jrd_nod**> jrdChildNodes;
};

class AggNode : public TypedNode<ExprNode, ExprNode::TYPE_AGGREGATE>
{
protected:
	struct AggInfo
	{
		AggInfo(const char* aName, UCHAR aBlr, UCHAR aDistinctBlr)
			: name(aName),
			  blr(aBlr),
			  distinctBlr(aDistinctBlr)
		{
		}

		const char* const name;
		const UCHAR blr;
		const UCHAR distinctBlr;
	};

public:
	template <typename T>
	class Register : public AggInfo
	{
	public:
		explicit Register(const char* aName, UCHAR blr, UCHAR blrDistinct)
			: AggInfo(aName, blr, blrDistinct),
			  registerNode1(blr),
			  registerNode2(blrDistinct)
		{
		}

		explicit Register(const char* aName, UCHAR blr)
			: AggInfo(aName, blr, blr),
			  registerNode1(blr),
			  registerNode2(blr)
		{
		}

	private:
		RegisterNode<T> registerNode1, registerNode2;
	};

	explicit AggNode(MemoryPool& pool, const AggInfo& aAggInfo, bool aDistinct, bool aDialect1,
		dsql_nod* aArg = NULL);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;

	virtual bool dsqlAggregateFinder(AggregateFinder& visitor);
	virtual bool dsqlAggregate2Finder(Aggregate2Finder& visitor);
	virtual bool dsqlInvalidReferenceFinder(InvalidReferenceFinder& visitor);
	virtual bool dsqlSubSelectFinder(SubSelectFinder& visitor);
	virtual bool dsqlFieldRemapper(FieldRemapper& visitor);

	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;
	virtual void setParameterName(dsql_par* parameter) const;
	virtual void genBlr();

	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);

	virtual bool jrdPossibleUnknownFinder(PossibleUnknownFinder& /*visitor*/)
	{
		return true;
	}

	virtual bool jrdStreamFinder(StreamFinder& /*visitor*/)
	{
		// ASF: Although in v2.5 the visitor happens normally for the node childs, nod_count has
		// been set to 0 in CMP_pass2, so that doesn't happens.
		return false;
	}

	virtual bool jrdStreamsCollector(StreamsCollector& /*visitor*/)
	{
		// ASF: Although in v2.5 the visitor happens normally for the node childs, nod_count has
		// been set to 0 in CMP_pass2, so that doesn't happens.
		return false;
	}

	virtual bool jrdUnmappedNodeGetter(UnmappedNodeGetter& visitor)
	{
		visitor.invalid = true;
		return false;
	}

	virtual void checkOrderedWindowCapable() const
	{
		if (distinct)
		{
			Firebird::status_exception::raise(
				Firebird::Arg::Gds(isc_wish_list) <<
				Firebird::Arg::Gds(isc_random) <<
					"DISTINCT is not supported in ordered windows");
		}
	}

	virtual bool shouldCallWinPass() const
	{
		return false;
	}

	virtual dsc* winPass(thread_db* /*tdbb*/, jrd_req* /*request*/, SlidingWindow* /*window*/) const
	{
		return NULL;
	}

	virtual void aggInit(thread_db* tdbb, jrd_req* request) const = 0;	// pure, but defined
	virtual void aggFinish(thread_db* tdbb, jrd_req* request) const;
	virtual void aggPass(thread_db* tdbb, jrd_req* request) const;
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const;

	virtual void aggPass(thread_db* tdbb, jrd_req* request, dsc* desc) const = 0;
	virtual dsc* aggExecute(thread_db* tdbb, jrd_req* request) const = 0;

protected:
	virtual ExprNode* internalDsqlPass();
	virtual AggNode* dsqlCopy() const = 0;

public:
	const AggInfo& aggInfo;
	bool distinct;
	bool dialect1;
	dsql_nod* dsqlArg;
	jrd_nod* arg;
	const AggregateSort* asb;
	bool indexed;
};


// Base class for window functions.
class WinFuncNode : public AggNode
{
private:
	// Base factory to create instance of subclasses.
	class Factory : public AggInfo
	{
	public:
		explicit Factory(const char* aName)
			: AggInfo(aName, 0, 0)
		{
		}

		virtual WinFuncNode* newInstance(MemoryPool& pool) const = 0;

	public:
		const Factory* next;
	};

public:
	// Concrete implementation of the factory.
	template <typename T>
	class Register : public Factory
	{
	public:
		explicit Register(const char* aName)
			: Factory(aName)
		{
			next = factories;
			factories = this;
		}

		WinFuncNode* newInstance(MemoryPool& pool) const
		{
			return new T(pool);
		}
	};

	explicit WinFuncNode(MemoryPool& pool, const AggInfo& aAggInfo, dsql_nod* aArg = NULL);

	static DmlNode* parse(thread_db* tdbb, MemoryPool& pool, CompilerScratch* csb, UCHAR blrOp);

private:
	static Factory* factories;
};


class StmtNode : public DmlNode
{
public:
	explicit StmtNode(MemoryPool& pool)
		: DmlNode(pool)
	{
	}

public:
	virtual void pass2Cursor(RecordSelExpr*& /*rsePtr*/, Cursor**& /*cursorPtr*/)
	{
	}

	virtual jrd_nod* execute(thread_db* tdbb, jrd_req* request) const = 0;
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

	jrd_nod* execute(thread_db* /*tdbb*/, jrd_req* /*request*/) const
	{
		fb_assert(false);
		return NULL;
	}
};


// This class (via the make method) does the job that pass1_savepoint does for the legacy nodes.
class SavepointEncloseNode : public DsqlOnlyStmtNode
{
public:
	explicit SavepointEncloseNode(MemoryPool& pool, StmtNode* aStmt)
		: DsqlOnlyStmtNode(pool),
		  stmt(aStmt)
	{
	}

public:
	static StmtNode* make(MemoryPool& pool, DsqlCompilerScratch* dsqlScratch, StmtNode* node);

public:
	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual void genBlr();

private:
	StmtNode* stmt;
};


// Common node for all "code blocks" (i.e.: procedures, triggers and execute block)
class BlockNode
{
public:
	explicit BlockNode(MemoryPool& pool, bool aHasEos)
		: hasEos(aHasEos),
		  variables(pool),
		  outputVariables(pool)
	{
	}

	virtual ~BlockNode()
	{
	}

	static void putDtype(DsqlCompilerScratch* dsqlScratch, const dsql_fld* field, bool useSubType);

	void putLocalVariables(DsqlCompilerScratch* dsqlScratch, const dsql_nod* parameters,
		SSHORT locals);
	void putLocalVariable(DsqlCompilerScratch* dsqlScratch, dsql_var* variable,
		dsql_nod* hostParam, const dsql_str* collationName);
	dsql_nod* resolveVariable(const dsql_str* varName);
	void genReturn(DsqlCompilerScratch* dsqlScratch, bool eosFlag = false);

private:
	bool hasEos;

protected:
	Firebird::Array<dsql_nod*> variables;
	Firebird::Array<dsql_nod*> outputVariables;
};


} // namespace

#endif // DSQL_NODES_H
