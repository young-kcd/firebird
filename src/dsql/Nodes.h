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
#include "../jrd/jrd.h"
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
class OptimizerRetrieval;
class RseNode;
class SlidingWindow;
class TypeClause;
class ValueExprNode;


// Must be less then MAX_SSHORT. Not used for static arrays.
const int MAX_CONJUNCTS	= 32000;

// Note that MAX_STREAMS currently MUST be <= MAX_UCHAR.
// Here we should really have a compile-time fb_assert, since this hard-coded
// limit is NOT negotiable so long as we use an array of UCHAR, where index 0
// tells how many streams are in the array (and the streams themselves are
// identified by a UCHAR).
const unsigned int MAX_STREAMS = 255;

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

typedef Firebird::Array<NestConst<ValueExprNode> > NestValueArray;


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
	// DML node kinds
	enum Kind
	{
		KIND_STATEMENT,
		KIND_VALUE,
		KIND_BOOLEAN,
		KIND_REC_SOURCE
	};

	explicit DmlNode(MemoryPool& pool, Kind aKind)
		: Node(pool),
		kind(aKind)
	{
	}

	// Merge missing values, computed values, validation expressions, and views into a parsed request.
	template <typename T> static void doPass1(thread_db* tdbb, CompilerScratch* csb, T** node)
	{
		if (!*node)
			return;

		*node = (*node)->pass1(tdbb, csb);
	}

public:
	virtual void genBlr(DsqlCompilerScratch* dsqlScratch) = 0;
	virtual DmlNode* pass1(thread_db* tdbb, CompilerScratch* csb) = 0;
	virtual DmlNode* pass2(thread_db* tdbb, CompilerScratch* csb) = 0;
	virtual DmlNode* copy(thread_db* tdbb, NodeCopier& copier) const = 0;

public:
	const Kind kind;
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


// Stores a reference to a specialized ExprNode.
// This class and NodeRefImpl exists to nodes replace themselfs (eg. pass1) in a type-safe way.
class NodeRef
{
public:
	virtual ~NodeRef()
	{
	}

	bool operator !() const
	{
		return !getExpr();
	}

	operator bool() const
	{
		return getExpr() != NULL;
	}

	virtual ExprNode* getExpr() = 0;
	virtual const ExprNode* getExpr() const = 0;

	virtual void pass1(thread_db* tdbb, CompilerScratch* csb) = 0;
	void pass2(thread_db* tdbb, CompilerScratch* csb);

protected:
	virtual void internalPass2(thread_db* tdbb, CompilerScratch* csb) = 0;
};

template <typename T> class NodeRefImpl : public NodeRef
{
public:
	NodeRefImpl(T** aPtr)
		: ptr(aPtr)
	{
		fb_assert(aPtr);
	}

	virtual ExprNode* getExpr()
	{
		return *ptr;
	}

	virtual const ExprNode* getExpr() const
	{
		return *ptr;
	}

	virtual void pass1(thread_db* tdbb, CompilerScratch* csb)
	{
		DmlNode::doPass1(tdbb, csb, ptr);
	}

protected:
	virtual inline void internalPass2(thread_db* tdbb, CompilerScratch* csb);

private:
	T** ptr;
};


class ExprNode : public DmlNode
{
public:
	enum Type
	{
		// Value types
		TYPE_AGGREGATE,
		TYPE_ARITHMETIC,
		TYPE_BOOL_AS_VALUE,
		TYPE_CAST,
		TYPE_COALESCE,
		TYPE_CONCATENATE,
		TYPE_CURRENT_DATE,
		TYPE_CURRENT_TIME,
		TYPE_CURRENT_TIMESTAMP,
		TYPE_CURRENT_ROLE,
		TYPE_CURRENT_USER,
		TYPE_DERIVED_EXPR,
		TYPE_DECODE,
		TYPE_DERIVED_FIELD,
		TYPE_DOMAIN_VALIDATION,
		TYPE_EXTRACT,
		TYPE_FIELD,
		TYPE_GEN_ID,
		TYPE_INTERNAL_INFO,
		TYPE_LITERAL,
		TYPE_MAP,
		TYPE_NEGATE,
		TYPE_NULL,
		TYPE_OVER,
		TYPE_PARAMETER,
		TYPE_RECORD_KEY,
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
		TYPE_VALUE_LIST,
		TYPE_VARIABLE,

		// Bool types
		TYPE_BINARY_BOOL,
		TYPE_COMPARATIVE_BOOL,
		TYPE_MISSING_BOOL,
		TYPE_NOT_BOOL,
		TYPE_RSE_BOOL,

		// RecordSource types
		TYPE_AGGREGATE_SOURCE,
		TYPE_PROCEDURE,
		TYPE_RELATION,
		TYPE_RSE,
		TYPE_UNION,
		TYPE_WINDOW
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

	explicit ExprNode(Type aType, MemoryPool& pool, Kind aKind)
		: DmlNode(pool, aKind),
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
		return type == T::TYPE ? static_cast<T*>(this) : NULL;
	}

	template <typename T> const T* as() const
	{
		return type == T::TYPE ? static_cast<const T*>(this) : NULL;
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

	static const ExprNode* fromLegacy(const ExprNode* node)
	{
		return node;
	}

	static ExprNode* fromLegacy(ExprNode* node)
	{
		return node;
	}

	static ExprNode* fromLegacy(const dsql_nod* node);

	// Allocate and assign impure space for various nodes.
	template <typename T> static void doPass2(thread_db* tdbb, CompilerScratch* csb, T** node)
	{
		if (!*node)
			return;

		*node = (*node)->pass2(tdbb, csb);
	}

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

	virtual bool jrdPossibleUnknownFinder();
	virtual bool jrdStreamFinder(CompilerScratch* csb, UCHAR findStream);
	virtual void jrdStreamsCollector(Firebird::SortedArray<int>& streamList);
	virtual bool jrdUnmappableNode(const MapNode* mapNode, UCHAR shellStream);

	virtual void print(Firebird::string& text, Firebird::Array<dsql_nod*>& nodes) const;
	virtual bool dsqlMatch(const ExprNode* other, bool ignoreMapCast) const;

	virtual ExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch)
	{
		DmlNode::dsqlPass(dsqlScratch);
		return this;
	}

	virtual bool sameAs(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other) /*const*/;

	// See if node is presently computable.
	// Note that a field is not computable
	// with respect to its own stream.
	//
	// There are two different uses of OPT_computable().
	// (a) idx_use == false: when an unused conjunct is to be picked for making
	//     into a boolean and in making a db_key.
	//     In this case, a node is said to be computable, if all the streams
	//     involved in that node are csb_active. The csb_active flag
	//     defines all the streams available in the current scope of the
	//     query.
	// (b) idx_use == true: to determine if we can use an
	//     index on the conjunct we have already chosen.
	//     In order to use an index on a conjunct, it is required that the
	//     all the streams involved in the conjunct are currently active
	//     or have been already processed before and made into rivers.
	//     Because, here we want to differentiate between streams we have
	//     not yet worked on and those that we have worked on or are currently
	//     working on.
	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idxUse,
		bool allowOnlyCurrentStream, ValueExprNode* value = NULL);

	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList);
	virtual ExprNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual ExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual ExprNode* copy(thread_db* tdbb, NodeCopier& copier) const = 0;

protected:
	virtual bool dsqlVisit(ConstDsqlNodeVisitor& visitor);
	virtual bool dsqlVisit(NonConstDsqlNodeVisitor& visitor);

	template <typename T>
	void addChildNode(dsql_nod*& dsqlNode, NestConst<T>& jrdNode)
	{
		dsqlChildNodes.add(&dsqlNode);
		jrdChildNodes.add(FB_NEW(getPool()) NodeRefImpl<T>(jrdNode.getAddress()));
	}

	void addChildNode(dsql_nod*& dsqlNode)
	{
		dsqlChildNodes.add(&dsqlNode);
	}

	template <typename T>
	void addChildNode(NestConst<T>& jrdNode)
	{
		jrdChildNodes.add(FB_NEW(getPool()) NodeRefImpl<T>(jrdNode.getAddress()));
	}

public:
	const Type type;
	unsigned nodFlags;
	ULONG impureOffset;
	const char* dsqlCompatDialectVerb;
	Firebird::Array<dsql_nod**> dsqlChildNodes;
	Firebird::Array<NodeRef*> jrdChildNodes;
};


template <typename T>
inline void NodeRefImpl<T>::internalPass2(thread_db* tdbb, CompilerScratch* csb)
{
	ExprNode::doPass2(tdbb, csb, ptr);
}


class BoolExprNode : public ExprNode
{
public:
	BoolExprNode(Type aType, MemoryPool& pool)
		: ExprNode(aType, pool, KIND_BOOLEAN)
	{
	}

	virtual BoolExprNode* dsqlPass(DsqlCompilerScratch* dsqlScratch)
	{
		ExprNode::dsqlPass(dsqlScratch);
		return this;
	}

	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idxUse,
		bool allowOnlyCurrentStream, ValueExprNode* value = NULL);

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

	virtual BoolExprNode* copy(thread_db* tdbb, NodeCopier& copier) const = 0;
	virtual bool execute(thread_db* tdbb, jrd_req* request) const = 0;
};

class ValueExprNode : public ExprNode
{
public:
	ValueExprNode(Type aType, MemoryPool& pool)
		: ExprNode(aType, pool, KIND_VALUE),
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

	virtual ValueExprNode* pass1(thread_db* tdbb, CompilerScratch* csb)
	{
		ExprNode::pass1(tdbb, csb);
		return this;
	}

	virtual ValueExprNode* pass2(thread_db* tdbb, CompilerScratch* csb)
	{
		ExprNode::pass2(tdbb, csb);
		return this;
	}

	// Compute descriptor for value expression.
	virtual void getDesc(thread_db* tdbb, CompilerScratch* csb, dsc* desc) = 0;

	virtual ValueExprNode* copy(thread_db* tdbb, NodeCopier& copier) const = 0;
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

	virtual ValueExprNode* pass2(thread_db* tdbb, CompilerScratch* csb);

	virtual bool jrdPossibleUnknownFinder()
	{
		return true;
	}

	virtual bool jrdStreamFinder(CompilerScratch* csb, UCHAR findStream)
	{
		// ASF: Although in v2.5 the visitor happens normally for the node childs, nod_count has
		// been set to 0 in CMP_pass2, so that doesn't happens.
		return false;
	}

	virtual void jrdStreamsCollector(Firebird::SortedArray<int>& /*streamList*/)
	{
		// ASF: Although in v2.5 the visitor happens normally for the node childs, nod_count has
		// been set to 0 in CMP_pass2, so that doesn't happens.
		return;
	}

	virtual bool jrdUnmappableNode(const MapNode* mapNode, UCHAR shellStream)
	{
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
	NestConst<ValueExprNode> arg;
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

protected:
	virtual void parseArgs(thread_db* tdbb, CompilerScratch* csb, unsigned count)
	{
		fb_assert(count == 0);
	}

private:
	static Factory* factories;
};


class StmtNode : public DmlNode
{
public:
	enum Type
	{
		TYPE_ASSIGNMENT,
		TYPE_BLOCK,
		TYPE_COMPOUND_STMT,
		TYPE_CONTINUE_LEAVE,
		TYPE_CURSOR_STMT,
		TYPE_DECLARE_CURSOR,
		TYPE_DECLARE_VARIABLE,
		TYPE_ERASE,
		TYPE_ERROR_HANDLER,
		TYPE_EXCEPTION,
		TYPE_EXEC_BLOCK,
		TYPE_EXEC_PROCEDURE,
		TYPE_EXEC_STATEMENT,
		TYPE_EXIT,
		TYPE_IF,
		TYPE_IN_AUTO_TRANS,
		TYPE_INIT_VARIABLE,
		TYPE_FOR,
		TYPE_HANDLER,
		TYPE_LABEL,
		TYPE_LOOP,
		TYPE_MERGE,
		TYPE_MESSAGE,
		TYPE_MODIFY,
		TYPE_POST_EVENT,
		TYPE_RECEIVE,
		TYPE_RETURN,
		TYPE_SAVEPOINT,
		TYPE_SAVEPOINT_ENCLOSE,
		TYPE_SELECT,
		TYPE_SET_GENERATOR,
		TYPE_STALL,
		TYPE_STORE,
		TYPE_SUSPEND,
		TYPE_USER_SAVEPOINT
	};

	enum WhichTrigger
	{
		ALL_TRIGS = 0,
		PRE_TRIG = 1,
		POST_TRIG = 2
	};

	struct ExeState
	{
		ExeState(thread_db* tdbb)
			: oldPool(tdbb->getDefaultPool()),
			  oldRequest(tdbb->getRequest()),
			  oldTransaction(tdbb->getTransaction()),
			  errorPending(false),
			  catchDisabled(false),
			  whichEraseTrig(ALL_TRIGS),
			  whichStoTrig(ALL_TRIGS),
			  whichModTrig(ALL_TRIGS),
			  topNode(NULL),
			  prevNode(NULL),
			  exit(false)
		{
		}

		MemoryPool* oldPool;		// Save the old pool to restore on exit.
		jrd_req* oldRequest;		// Save the old request to restore on exit.
		jrd_tra* oldTransaction;	// Save the old transcation to restore on exit.
		bool errorPending;			// Is there an error pending to be handled?
		bool catchDisabled;			// Catch errors so we can unwind cleanly.
		WhichTrigger whichEraseTrig;
		WhichTrigger whichStoTrig;
		WhichTrigger whichModTrig;
		const StmtNode* topNode;
		const StmtNode* prevNode;
		bool exit;
	};

public:
	explicit StmtNode(Type aType, MemoryPool& pool)
		: DmlNode(pool, KIND_STATEMENT),
		  type(aType),
		  parentStmt(NULL),
		  impureOffset(0),
		  hasLineColumn(false),
		  line(0),
		  column(0)
	{
	}

	template <typename T> T* as()
	{
		return type == T::TYPE ? static_cast<T*>(this) : NULL;
	}

	template <typename T> const T* as() const
	{
		return type == T::TYPE ? static_cast<const T*>(this) : NULL;
	}

	template <typename T> bool is() const
	{
		return type == T::TYPE;
	}

	template <typename T, typename LegacyType> static T* as(LegacyType* node)
	{
		StmtNode* obj = T::fromLegacy(node);
		return obj ? obj->as<T>() : NULL;
	}

	template <typename T, typename LegacyType> static const T* as(const LegacyType* node)
	{
		const StmtNode* obj = T::fromLegacy(node);
		return obj ? obj->as<T>() : NULL;
	}

	template <typename T, typename LegacyType> static bool is(const LegacyType* node)
	{
		const StmtNode* obj = T::fromLegacy(node);
		return obj ? obj->is<T>() : false;
	}

	static const StmtNode* fromLegacy(const StmtNode* node)
	{
		return node;
	}

	static StmtNode* fromLegacy(StmtNode* node)
	{
		return node;
	}

	static StmtNode* fromLegacy(const dsql_nod* node);

	// Allocate and assign impure space for various nodes.
	template <typename T> static void doPass2(thread_db* tdbb, CompilerScratch* csb, T** node,
		StmtNode* parentStmt)
	{
		if (!*node)
			return;

		if (parentStmt)
			(*node)->parentStmt = parentStmt;

		*node = (*node)->pass2(tdbb, csb);
	}

	virtual StmtNode* pass1(thread_db* tdbb, CompilerScratch* csb) = 0;
	virtual StmtNode* pass2(thread_db* tdbb, CompilerScratch* csb) = 0;

	virtual StmtNode* copy(thread_db* tdbb, NodeCopier& copier) const
	{
		fb_assert(false);
		Firebird::status_exception::raise(
			Firebird::Arg::Gds(isc_cannot_copy_stmt) <<
			Firebird::Arg::Num(int(type)));

		return NULL;
	}

	virtual const StmtNode* execute(thread_db* tdbb, jrd_req* request, ExeState* exeState) const = 0;

public:
	const Type type;
	NestConst<StmtNode> parentStmt;
	ULONG impureOffset;	// Inpure offset from request block.
	bool hasLineColumn;
	USHORT line;
	USHORT column;
};


// Used to represent nodes that don't have a specific BLR verb, i.e.,
// do not use RegisterNode.
class DsqlOnlyStmtNode : public StmtNode
{
public:
	explicit DsqlOnlyStmtNode(Type aType, MemoryPool& pool)
		: StmtNode(aType, pool)
	{
	}

public:
	virtual DsqlOnlyStmtNode* pass1(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
	{
		fb_assert(false);
		return this;
	}

	virtual DsqlOnlyStmtNode* pass2(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
	{
		fb_assert(false);
		return this;
	}

	virtual DsqlOnlyStmtNode* copy(thread_db* /*tdbb*/, NodeCopier& /*copier*/) const
	{
		fb_assert(false);
		return NULL;
	}

	const StmtNode* execute(thread_db* /*tdbb*/, jrd_req* /*request*/, ExeState* /*exeState*/) const
	{
		fb_assert(false);
		return NULL;
	}
};


// This class (via the make method) does the job that pass1_savepoint does for the legacy nodes.
class SavepointEncloseNode : public TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_SAVEPOINT_ENCLOSE>
{
public:
	explicit SavepointEncloseNode(MemoryPool& pool, StmtNode* aStmt)
		: TypedNode<DsqlOnlyStmtNode, StmtNode::TYPE_SAVEPOINT_ENCLOSE>(pool),
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
