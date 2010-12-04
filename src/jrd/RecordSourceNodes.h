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
 * Adriano dos Santos Fernandes
 */

#ifndef JRD_RECORD_SOURCE_NODES_H
#define JRD_RECORD_SOURCE_NODES_H

#include "../common/common.h"
#include "../common/classes/alloc.h"
#include "../common/classes/array.h"
#include "../common/classes/objects_array.h"
#include "../common/classes/NestConst.h"
#include "../jrd/jrd.h"
#include "../jrd/exe.h"
#include "../dsql/Visitors.h"

namespace Jrd {

class IndexRetrieval;
class OptimizerRetrieval;
class ProcedureScan;
class BoolExprNode;
class MessageNode;
class RelationSourceNode;
class RseNode;
class ValueListNode;


class SortNode : public Firebird::PermanentStorage
{
public:
	explicit SortNode(MemoryPool& pool)
		: PermanentStorage(pool),
		  unique(false),
		  expressions(pool),
		  descending(pool),
		  nullOrder(pool)
	{
	}

	SortNode* copy(thread_db* tdbb, NodeCopier& copier);
	SortNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	SortNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	bool computable(CompilerScratch* csb, SSHORT stream, bool idx_use, bool allowOnlyCurrentStream);
	void findDependentFromStreams(const OptimizerRetrieval* optRet, SortedStreamList* streamList);

	bool unique;						// sorts using unique key - for distinct and group by
	NestValueArray expressions;			// sort expressions
	Firebird::Array<bool> descending;	// true = descending / false = ascending
	Firebird::Array<int> nullOrder;		// rse_nulls_*
};

class MapNode : public Firebird::PermanentStorage
{
public:
	explicit MapNode(MemoryPool& pool)
		: PermanentStorage(pool),
		  sourceList(pool),
		  targetList(pool)
	{
	}

	MapNode* copy(thread_db* tdbb, NodeCopier& copier);
	MapNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	MapNode* pass2(thread_db* tdbb, CompilerScratch* csb);

	NestValueArray sourceList;
	NestValueArray targetList;
};

class PlanNode : public Firebird::PermanentStorage
{
public:
	enum Type
	{
		TYPE_JOIN,
		TYPE_RETRIEVE
	};

	struct AccessItem
	{
		explicit AccessItem(MemoryPool& pool)
			: relationId(0),
			  indexId(0),
			  indexName(pool)
		{
		}

		SLONG relationId;
		SLONG indexId;
		Firebird::MetaName indexName;
	};

	struct AccessType
	{
		enum Type
		{
			TYPE_SEQUENTIAL,
			TYPE_NAVIGATIONAL,
			TYPE_INDICES
		};

		AccessType(MemoryPool& pool, Type aType)
			: type(aType),
			  items(pool)
		{
		}

		Type const type;
		Firebird::ObjectsArray<AccessItem> items;
	};

	PlanNode(MemoryPool& pool, Type aType)
		: PermanentStorage(pool),
		  type(aType),
		  accessType(NULL),
		  relationNode(NULL),
		  subNodes(pool)
	{
	}

	Type const type;
	AccessType* accessType;
	RelationSourceNode* relationNode;
	Firebird::Array<NestConst<PlanNode> > subNodes;
};

class InversionNode
{
public:
	enum Type
	{
		TYPE_AND,
		TYPE_OR,
		TYPE_IN,
		TYPE_DBKEY,
		TYPE_INDEX
	};

	InversionNode(Type aType, InversionNode* aNode1, InversionNode* aNode2)
		: type(aType),
		  impure(0),
		  retrieval(NULL),
		  node1(aNode1),
		  node2(aNode2),
		  value(NULL),
		  id(0)
	{
	}

	explicit InversionNode(IndexRetrieval* aRetrieval)
		: type(TYPE_INDEX),
		  impure(0),
		  retrieval(aRetrieval),
		  node1(NULL),
		  node2(NULL),
		  value(NULL),
		  id(0)
	{
	}

	InversionNode(ValueExprNode* aValue, USHORT aId)
		: type(TYPE_DBKEY),
		  impure(0),
		  retrieval(NULL),
		  node1(NULL),
		  node2(NULL),
		  value(aValue),
		  id(aId)
	{
	}

	Type type;
	ULONG impure;
	NestConst<IndexRetrieval> retrieval;
	NestConst<InversionNode> node1;
	NestConst<InversionNode> node2;
	NestConst<ValueExprNode> value;
	USHORT id;
};


class RecordSourceNode : public ExprNode
{
public:
	RecordSourceNode(Type aType, MemoryPool& pool)
		: ExprNode(aType, pool, KIND_REC_SOURCE),
		  stream(MAX_USHORT)
	{
	}

	virtual USHORT getStream() const
	{
		return stream;
	}

	void setStream(USHORT value)
	{
		stream = value;
	}

	// Identify the streams that make up an RseNode.
	virtual void getStreams(StreamsArray& list) const
	{
		list.add(getStream());
	}

	virtual RecordSourceNode* copy(thread_db* tdbb, NodeCopier& copier) = 0;
	virtual void ignoreDbKey(thread_db* tdbb, CompilerScratch* csb) const = 0;
	virtual RecordSourceNode* pass1(thread_db* tdbb, CompilerScratch* csb) = 0;
	virtual void pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
		BoolExprNode** boolean, RecordSourceNodeStack& stack) = 0;
	virtual RecordSourceNode* pass2(thread_db* tdbb, CompilerScratch* csb) = 0;
	virtual void pass2Rse(thread_db* tdbb, CompilerScratch* csb) = 0;
	virtual bool containsStream(USHORT checkStream) const = 0;

	virtual void genBlr(DsqlCompilerScratch* dsqlScratch)
	{
		fb_assert(false);
	}

	virtual bool jrdPossibleUnknownFinder()
	{
		return true;
	}

	virtual bool jrdStreamFinder(CompilerScratch* /*csb*/, UCHAR /*findStream*/)
	{
		return true;
	}

	virtual bool jrdUnmappableNode(const MapNode* /*mapNode*/, UCHAR /*shellStream*/)
	{
		return false;
	}

	virtual bool expressionEqual(thread_db* tdbb, CompilerScratch* csb, /*const*/ ExprNode* other,
		USHORT stream) /*const*/
	{
		return false;
	}

	// Identify all of the streams for which a dbkey may need to be carried through a sort.
	virtual void computeDbKeyStreams(UCHAR* streams) const = 0;
	virtual RecordSource* compile(thread_db* tdbb, OptimizerBlk* opt, bool innerSubStream) = 0;

protected:
	USHORT stream;
};

class RelationSourceNode : public TypedNode<RecordSourceNode, RecordSourceNode::TYPE_RELATION>
{
public:
	explicit RelationSourceNode(MemoryPool& pool)
		: TypedNode<RecordSourceNode, RecordSourceNode::TYPE_RELATION>(pool),
		  relation(NULL),
		  context(0),
		  alias(NULL),
		  view(NULL)
	{
	}

	static RelationSourceNode* parse(thread_db* tdbb, CompilerScratch* csb, SSHORT blrOp,
		bool parseContext);

	virtual RelationSourceNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual void ignoreDbKey(thread_db* tdbb, CompilerScratch* csb) const;

	virtual RecordSourceNode* pass1(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
	{
		return this;
	}

	virtual void pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
		BoolExprNode** boolean, RecordSourceNodeStack& stack);

	virtual RecordSourceNode* pass2(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
	{
		return this;
	}

	virtual void pass2Rse(thread_db* tdbb, CompilerScratch* csb);

	virtual bool containsStream(USHORT checkStream) const
	{
		return checkStream == stream;
	}

	virtual void computeDbKeyStreams(UCHAR* streams) const
	{
		fb_assert(streams[0] < MAX_STREAMS && streams[0] < MAX_UCHAR);
		streams[++streams[0]] = getStream();
	}

	virtual bool computable(CompilerScratch* /*csb*/, SSHORT /*stream*/, bool /*idx_use*/,
		bool /*allowOnlyCurrentStream*/, ValueExprNode* /*value*/)
	{
		return true;
	}

	virtual void findDependentFromStreams(const OptimizerRetrieval* /*optRet*/,
		SortedStreamList* /*streamList*/)
	{
	}

	virtual RecordSource* compile(thread_db* tdbb, OptimizerBlk* opt, bool innerSubStream);

public:
	jrd_rel* relation;
	SSHORT context;		// user-specified context number for the relation reference
	const char* alias;	// SQL alias for the relation

private:
	jrd_rel* view;		// parent view for posting access
};

class ProcedureSourceNode : public TypedNode<RecordSourceNode, RecordSourceNode::TYPE_PROCEDURE>
{
public:
	explicit ProcedureSourceNode(MemoryPool& pool)
		: TypedNode<RecordSourceNode, RecordSourceNode::TYPE_PROCEDURE>(pool),
		  sourceList(NULL),
		  targetList(NULL),
		  in_msg(NULL),
		  procedure(0),
		  view(NULL),
		  context(0)
	{
	}

	static ProcedureSourceNode* parse(thread_db* tdbb, CompilerScratch* csb, SSHORT blrOp);

	virtual ProcedureSourceNode* copy(thread_db* tdbb, NodeCopier& copier);

	virtual void ignoreDbKey(thread_db* /*tdbb*/, CompilerScratch* /*csb*/) const
	{
	}

	virtual RecordSourceNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
		BoolExprNode** boolean, RecordSourceNodeStack& stack);
	virtual RecordSourceNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass2Rse(thread_db* tdbb, CompilerScratch* csb);

	virtual bool containsStream(USHORT /*checkStream*/) const
	{
		return false;
	}

	virtual void computeDbKeyStreams(UCHAR* /*streams*/) const
	{
	}

	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
		bool allowOnlyCurrentStream, ValueExprNode* value);
	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList);

	virtual bool jrdStreamFinder(CompilerScratch* /*csb*/, UCHAR findStream);
	virtual void jrdStreamsCollector(Firebird::SortedArray<int>& streamList);

	virtual RecordSource* compile(thread_db* tdbb, OptimizerBlk* opt, bool innerSubStream);

private:
	ProcedureScan* generate(thread_db* tdbb, OptimizerBlk* opt);

public:
	NestConst<ValueListNode> sourceList;
	NestConst<ValueListNode> targetList;

private:
	NestConst<MessageNode> in_msg;
	USHORT procedure;
	jrd_rel* view;
	SSHORT context;
};

class AggregateSourceNode : public TypedNode<RecordSourceNode, RecordSourceNode::TYPE_AGGREGATE_SOURCE>
{
public:
	explicit AggregateSourceNode(MemoryPool& pool)
		: TypedNode<RecordSourceNode, RecordSourceNode::TYPE_AGGREGATE_SOURCE>(pool),
		  group(NULL),
		  map(NULL),
		  rse(NULL)
	{
	}

	static AggregateSourceNode* parse(thread_db* tdbb, CompilerScratch* csb);

	virtual AggregateSourceNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual void ignoreDbKey(thread_db* tdbb, CompilerScratch* csb) const;
	virtual RecordSourceNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
		BoolExprNode** boolean, RecordSourceNodeStack& stack);
	virtual RecordSourceNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass2Rse(thread_db* tdbb, CompilerScratch* csb);
	virtual bool containsStream(USHORT checkStream) const;

	virtual void computeDbKeyStreams(UCHAR* /*streams*/) const
	{
	}

	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
		bool allowOnlyCurrentStream, ValueExprNode* value);
	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList);
	virtual RecordSource* compile(thread_db* tdbb, OptimizerBlk* opt, bool innerSubStream);

private:
	RecordSource* generate(thread_db* tdbb, OptimizerBlk* opt, BoolExprNodeStack* parentStack,
		UCHAR shellStream);

public:
	NestConst<SortNode> group;
	NestConst<MapNode> map;

private:
	NestConst<RseNode> rse;
};

class UnionSourceNode : public TypedNode<RecordSourceNode, RecordSourceNode::TYPE_UNION>
{
public:
	explicit UnionSourceNode(MemoryPool& pool)
		: TypedNode<RecordSourceNode, RecordSourceNode::TYPE_UNION>(pool),
		  clauses(pool),
		  maps(pool),
		  mapStream(0),
		  recursive(false)
	{
	}

	static UnionSourceNode* parse(thread_db* tdbb, CompilerScratch* csb, SSHORT blrOp);

	virtual UnionSourceNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual void ignoreDbKey(thread_db* tdbb, CompilerScratch* csb) const;

	virtual RecordSourceNode* pass1(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
	{
		return this;
	}

	virtual void pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
		BoolExprNode** boolean, RecordSourceNodeStack& stack);
	virtual RecordSourceNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass2Rse(thread_db* tdbb, CompilerScratch* csb);
	virtual bool containsStream(USHORT checkStream) const;
	virtual void computeDbKeyStreams(UCHAR* streams) const;
	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
		bool allowOnlyCurrentStream, ValueExprNode* value);
	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList);
	virtual RecordSource* compile(thread_db* tdbb, OptimizerBlk* opt, bool innerSubStream);

private:
	RecordSource* generate(thread_db* tdbb, OptimizerBlk* opt, UCHAR* streams, USHORT nstreams,
		BoolExprNodeStack* parentStack, UCHAR shellStream);

private:
	Firebird::Array<NestConst<RseNode> > clauses;	// RseNode's for union
	Firebird::Array<NestConst<MapNode> > maps;		// RseNode's maps
	USHORT mapStream;	// stream for next level record of recursive union
	bool recursive;		// union node is a recursive union
};

class WindowSourceNode : public TypedNode<RecordSourceNode, RecordSourceNode::TYPE_WINDOW>
{
public:
	struct Partition
	{
		explicit Partition(MemoryPool&)
			: stream(MAX_USHORT)
		{
		}

		USHORT stream;
		NestConst<SortNode> group;
		NestConst<SortNode> regroup;
		NestConst<SortNode> order;
		NestConst<MapNode> map;
	};

	explicit WindowSourceNode(MemoryPool& pool)
		: TypedNode<RecordSourceNode, RecordSourceNode::TYPE_WINDOW>(pool),
		  rse(NULL),
		  partitions(pool)
	{
	}

	static WindowSourceNode* parse(thread_db* tdbb, CompilerScratch* csb);

private:
	void parsePartitionBy(thread_db* tdbb, CompilerScratch* csb);

public:
	virtual USHORT getStream() const
	{
		fb_assert(false);
		return 0;
	}

	virtual void getStreams(StreamsArray& list) const;
	virtual WindowSourceNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual void ignoreDbKey(thread_db* tdbb, CompilerScratch* csb) const;
	virtual RecordSourceNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
		BoolExprNode** boolean, RecordSourceNodeStack& stack);
	virtual RecordSourceNode* pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass2Rse(thread_db* tdbb, CompilerScratch* csb);
	virtual bool containsStream(USHORT checkStream) const;

	virtual void computeDbKeyStreams(UCHAR* /*streams*/) const
	{
	}

	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
		bool allowOnlyCurrentStream, ValueExprNode* value);
	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList);
	virtual RecordSource* compile(thread_db* tdbb, OptimizerBlk* opt, bool innerSubStream);

private:
	NestConst<RseNode> rse;
	Firebird::ObjectsArray<Partition> partitions;
};

class RseNode : public TypedNode<RecordSourceNode, RecordSourceNode::TYPE_RSE>
{
public:
	static const unsigned FLAG_VARIANT		= 0x1;	// variant (not invariant?)
	static const unsigned FLAG_SINGULAR		= 0x2;	// singleton select
	static const unsigned FLAG_WRITELOCK	= 0x4;	// locked for write
	static const unsigned FLAG_SCROLLABLE	= 0x8;	// scrollable cursor

	explicit RseNode(MemoryPool& pool)
		: TypedNode<RecordSourceNode, RecordSourceNode::TYPE_RSE>(pool),
		  rse_jointype(0),
		  rse_invariants(NULL),
		  rse_relations(pool),
		  flags(0)
	{
	}

	RseNode* clone()
	{
		RseNode* obj = FB_NEW(getPool()) RseNode(getPool());

		obj->rse_jointype = rse_jointype;
		obj->rse_first = rse_first;
		obj->rse_skip = rse_skip;
		obj->rse_boolean = rse_boolean;
		obj->rse_sorted = rse_sorted;
		obj->rse_projection = rse_projection;
		obj->rse_aggregate = rse_aggregate;
		obj->rse_plan = rse_plan;
		obj->rse_invariants = rse_invariants;
		obj->flags = flags;
		obj->rse_relations = rse_relations;

		return obj;
	}

	virtual void getStreams(StreamsArray& /*list*/) const
	{
	}

	virtual RseNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual void ignoreDbKey(thread_db* tdbb, CompilerScratch* csb) const;
	virtual RseNode* pass1(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
		BoolExprNode** boolean, RecordSourceNodeStack& stack);

	virtual RseNode* pass2(thread_db* /*tdbb*/, CompilerScratch* /*csb*/)
	{
		return this;
	}

	virtual void pass2Rse(thread_db* tdbb, CompilerScratch* csb);
	virtual bool containsStream(USHORT checkStream) const;
	virtual void computeDbKeyStreams(UCHAR* streams) const;
	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
		bool allowOnlyCurrentStream, ValueExprNode* value);
	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList);

	virtual bool jrdStreamFinder(CompilerScratch* /*csb*/, UCHAR findStream);
	virtual void jrdStreamsCollector(Firebird::SortedArray<int>& streamList);

	virtual RecordSource* compile(thread_db* tdbb, OptimizerBlk* opt, bool innerSubStream);

private:
	void computeRseStreams(const CompilerScratch* csb, UCHAR* streams) const;
	void planCheck(const CompilerScratch* csb) const;
	static void planSet(CompilerScratch* csb, PlanNode* plan);

public:
	USHORT rse_jointype;		// inner, left, full
	NestConst<ValueExprNode> rse_first;
	NestConst<ValueExprNode> rse_skip;
	NestConst<BoolExprNode> rse_boolean;
	NestConst<SortNode> rse_sorted;
	NestConst<SortNode> rse_projection;
	NestConst<SortNode> rse_aggregate;	// singleton aggregate for optimizing to index
	NestConst<PlanNode> rse_plan;		// user-specified access plan
	NestConst<VarInvariantArray> rse_invariants; // Invariant nodes bound to top-level RSE
	Firebird::Array<NestConst<RecordSourceNode> > rse_relations;
	unsigned flags;
};


} // namespace Jrd

#endif	// JRD_RECORD_SOURCE_NODES_H
