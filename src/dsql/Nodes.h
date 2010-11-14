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

#include "../common/common.h"
#include "../dsql/DsqlCompilerScratch.h"
#include "../dsql/node.h"
#include "../dsql/Visitors.h"
#include "../common/classes/array.h"
#include "../common/classes/NestConst.h"

namespace Jrd {

class AggregateSort;
class CompilerScratch;
class Cursor;
class dsql_nod;
class ExprNode;
class jrd_nod;
class OptimizerRetrieval;
class RseNode;
class SlidingWindow;
class TypeClause;


// Must be less then MAX_SSHORT. Not used for static arrays.
const int MAX_CONJUNCTS	= 32000;

// Note that MAX_STREAMS currently MUST be <= MAX_UCHAR.
// Here we should really have a compile-time fb_assert, since this hard-coded
// limit is NOT negotiable so long as we use an array of UCHAR, where index 0
// tells how many streams are in the array (and the streams themselves are
// identified by a UCHAR).
const int MAX_STREAMS = 255;

// This is number of ULONG's needed to store bit-mapped flags for all streams
// OPT_STREAM_BITS = (MAX_STREAMS + 1) / sizeof(ULONG)
// This value cannot be increased simple way. Decrease is possible, but it is also
// hardcoded in several places such as TEST_DEP_ARRAYS macro
const int OPT_STREAM_BITS = 8;

// Number of streams, conjuncts, indices that will be statically allocated
// in various arrays. Larger numbers will have to be allocated dynamically
const int OPT_STATIC_ITEMS = 16;


typedef Firebird::HalfStaticArray<UCHAR, OPT_STATIC_ITEMS> StreamsArray;
typedef Firebird::SortedArray<int> SortedStreamList;
typedef UCHAR stream_array_t[MAX_STREAMS + 1];


template <typename T>
class RegisterNode
{
public:
	explicit RegisterNode(UCHAR blr)
	{
		PAR_register(blr, T::parse);
	}
};

template <typename T>
class RegisterBoolNode
{
public:
	explicit RegisterBoolNode(UCHAR blr)
	{
		PAR_register(blr, T::parse);
	}
};


class Node : public Firebird::PermanentStorage
{
public:
	explicit Node(MemoryPool& pool)
		: PermanentStorage(pool)
	{
	}

	virtual ~Node()
	{
	}

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const = 0;

	virtual Node* dsqlPass(DsqlCompilerScratch* /*dsqlScratch*/)
	{
		return this;
	}
};


class DdlNode : public Node
{
public:
	explicit DdlNode(MemoryPool& pool)
		: Node(pool)
	{
	}

	static bool deleteSecurityClass(thread_db* tdbb, jrd_tra* transaction,
		const Firebird::MetaName& secClass);

public:
	// Set the scratch's transaction when executing a node. Fact of accessing the scratch during
	// execution is a hack.
	void executeDdl(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction)
	{
		using namespace Firebird;

		dsqlScratch->setTransaction(transaction);

		try
		{
			execute(tdbb, dsqlScratch, transaction);
		}
		catch (status_exception& ex)
		{
			// Rethrow an exception with isc_no_meta_update and prefix codes.

			Arg::StatusVector newVector;
			newVector << Arg::Gds(isc_no_meta_update);
			putErrorPrefix(newVector);

			const ISC_STATUS* status = ex.value();

			if (status[1] == isc_no_meta_update)
				status += 2;

			newVector.append(Arg::StatusVector(status));

			status_exception::raise(newVector);
		}
	}

	virtual DdlNode* dsqlPass(DsqlCompilerScratch* dsqlScratch)
	{
		dsqlScratch->getStatement()->setType(DsqlCompiledStatement::TYPE_DDL);
		return this;
	}

public:
	enum DdlTriggerWhen { DTW_BEFORE, DTW_AFTER };

	static void executeDdlTrigger(thread_db* tdbb, jrd_tra* transaction,
		DdlTriggerWhen when, int action, const Firebird::MetaName& objectName,
		const Firebird::string& sqlText);

protected:
	// Return exception code based on combination of create and alter clauses.
	static ISC_STATUS createAlterCode(bool create, bool alter, ISC_STATUS createCode,
		ISC_STATUS alterCode, ISC_STATUS createOrAlterCode)
	{
		if (create && alter)
			return createOrAlterCode;

		if (create)
			return createCode;

		if (alter)
			return alterCode;

		fb_assert(false);
		return 0;
	}

	void executeDdlTrigger(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction,
		DdlTriggerWhen when, int action, const Firebird::MetaName& objectName);
	void storeGlobalField(thread_db* tdbb, jrd_tra* transaction, Firebird::MetaName& name,
		const TypeClause& field,
		const Firebird::string& computedSource = "",
		const BlrWriter::BlrData& computedValue = BlrWriter::BlrData());

protected:
	// Prefix DDL exceptions. To be implemented in each command.
	virtual void putErrorPrefix(Firebird::Arg::StatusVector& statusVector) = 0;

public:
	virtual void execute(thread_db* tdbb, DsqlCompilerScratch* dsqlScratch, jrd_tra* transaction) = 0;
};


class DmlNode : public Node
{
public:
	explicit DmlNode(MemoryPool& pool)
		: Node(pool)
	{
	}

public:
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch) = 0;
	virtual DmlNode* pass1(thread_db* tdbb, CompilerScratch* csb) = 0;
	virtual DmlNode* pass2(thread_db* tdbb, CompilerScratch* csb) = 0;
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
		TYPE_ARITHMETIC,
		TYPE_BINARY_BOOL,
		TYPE_CAST,
		TYPE_COMPARATIVE_BOOL,
		TYPE_CONCATENATE,
		TYPE_CURRENT_DATE,
		TYPE_CURRENT_TIME,
		TYPE_CURRENT_TIMESTAMP,
		TYPE_CURRENT_ROLE,
		TYPE_CURRENT_USER,
		TYPE_DERIVED_EXPR,
		TYPE_DERIVED_FIELD,
		TYPE_DOMAIN_VALIDATION,
		TYPE_EXTRACT,
		TYPE_FIELD,
		TYPE_GEN_ID,
		TYPE_INTERNAL_INFO,
		TYPE_LITERAL,
		TYPE_MAP,
		TYPE_MISSING_BOOL,
		TYPE_NEGATE,
		TYPE_NOT_BOOL,
		TYPE_NULL,
		TYPE_OVER,
		TYPE_PARAMETER,
		TYPE_RECORD_KEY,
		TYPE_RSE_BOOL,
		TYPE_SCALAR,
		TYPE_STMT_EXPR,
		TYPE_STR_CASE,
		TYPE_STR_LEN,
		TYPE_SUBQUERY,
		TYPE_SUBSTRING,
		TYPE_SUBSTRING_SIMILAR,
		TYPE_SYSFUNC_CALL,
		TYPE_TRIM,
		TYPE_UDF_CALL,
		TYPE_VALUE_IF,
		TYPE_VARIABLE
	};

	// Generic flags.
	static const unsigned FLAG_INVARIANT	= 0x01;	// Node is recognized as being invariant.

	// Boolean flags.
	static const unsigned FLAG_DEOPTIMIZE	= 0x02;	// Boolean which requires deoptimization.
	static const unsigned FLAG_ANSI_NOT		= 0x04;	// ANY/ALL predicate is prefixed with a NOT one.

	// Value flags.
	static const unsigned FLAG_QUAD			= 0x08;	// Compute in quad (default is long).
	static const unsigned FLAG_DOUBLE		= 0x10;
	static const unsigned FLAG_DATE			= 0x20;
	static const unsigned FLAG_VALUE		= 0x40;	// Full value area required in impure space.

	explicit ExprNode(Type aType, MemoryPool& pool)
		: DmlNode(pool),
		  type(aType),
		  nodFlags(0),
		  impureOffset(0),
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
	static const ExprNode* fromLegacy(const jrd_nod* node);
	static ExprNode* fromLegacy(jrd_nod* node);

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
		for (JrdNode* i = jrdChildNodes.begin(); i != jrdChildNodes.end(); ++i)
		{
			if (!visitor.visit(*i))
				return false;
		}

		return true;
	}

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;

	virtual ExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch)
	{
		DmlNode::dsqlPass(dsqlScratch);
		return this;
	}

	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/;
	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idxUse,
		bool allowOnlyCurrentStream);
	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList);
	virtual ExprNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual ExprNode* copy(thread_db* tdbb, NodeCopier& copier) = 0;

protected:
	virtual bool dsqlVisit(ConstDsqlNodeVisitor& visitor);
	virtual bool dsqlVisit(NonConstDsqlNodeVisitor& visitor);
	virtual bool jrdVisit(JrdNodeVisitor& visitor);

	void addChildNode(dsql_nod*& dsqlNode, const JrdNode& jrdNode)
	{
		dsqlChildNodes.add(&dsqlNode);
		jrdChildNodes.add(jrdNode);
	}

	void addChildNode(dsql_nod*& dsqlNode)
	{
		dsqlChildNodes.add(&dsqlNode);
	}

	void addChildNode(const JrdNode& jrdNode)
	{
		jrdChildNodes.add(jrdNode);
	}

public:
	const Type type;
	unsigned nodFlags;
	ULONG impureOffset;
	const char* dsqlCompatDialectVerb;
	Firebird::Array<dsql_nod**> dsqlChildNodes;
	Firebird::Array<JrdNode> jrdChildNodes;
};

class BoolExprNode : public ExprNode
{
public:
	BoolExprNode(Type aType, MemoryPool& pool)
		: ExprNode(aType, pool)
	{
	}

	virtual BoolExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch)
	{
		ExprNode::dsqlPass(dsqlScratch);
		return this;
	}

	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idxUse,
		bool allowOnlyCurrentStream);

	virtual BoolExprNode* pass1(thread_db* tdbb, CompilerScratch* csb)
	{
		ExprNode::pass1(tdbb, csb);
		return this;
	}

	virtual BoolExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);

	virtual void pass2Boolean1(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
	{
	}

	virtual void pass2Boolean2(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
	{
	}

	virtual BoolExprNode* copy(thread_db* tdbb, NodeCopier& copier) = 0;
	virtual bool execute(thread_db* tdbb, jrd_req* request) const = 0;
};

class ValueExprNode : public ExprNode
{
public:
	ValueExprNode(Type aType, MemoryPool& pool)
		: ExprNode(aType, pool),
		  nodScale(0)
	{
	}

	virtual ValueExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch)
	{
		ExprNode::dsqlPass(dsqlScratch);
		return this;
	}

	virtual bool setParameterType(DsqlCompilerScratch* /*dsqlScratch*/,
		dsql_nod* /*node*/, bool /*forceVarChar*/)
	{
		return false;
	}

	virtual void setParameterName(dsql_par* parameter) const = 0;
	virtual void make(DsqlCompilerScratch* dsqlScratch, dsc* desc) = 0;

	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc) = 0;
	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier) = 0;
	virtual dsc* execute(thread_db* tdbb, jrd_req* request) const = 0;

public:
	SCHAR nodScale;
};

class AggNode : public TypedNode<ValueExprNode, ExprNode::TYPE_AGGREGATE>
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
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

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

	virtual AggNode* dsqlPass(DsqlCompilerScratch* dsqlScratch);

protected:
	virtual AggNode* dsqlCopy(DsqlCompilerScratch* dsqlScratch) const = 0;

public:
	const AggInfo& aggInfo;
	bool distinct;
	bool dialect1;
	dsql_nod* dsqlArg;
	NestConst<jrd_nod> arg;
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
		: DmlNode(pool),
		  node(NULL)
	{
	}

public:
	jrd_nod* getNode()
	{
		return node;
	}

	void setNode(jrd_nod* value)
	{
		node = value;
	}

	virtual void pass2Cursor(RseNode*& /*rsePtr*/, Cursor**& /*cursorPtr*/)
	{
	}

	virtual const jrd_nod* execute(thread_db* tdbb, jrd_req* request) const = 0;

protected:
	NestConst<jrd_nod> node;
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

	const jrd_nod* execute(thread_db* /*tdbb*/, jrd_req* /*request*/) const
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
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch);

private:
	StmtNode* stmt;
};


} // namespace

#endif // DSQL_NODES_H
