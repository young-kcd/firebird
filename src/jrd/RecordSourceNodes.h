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

#include "../jrd/common.h"
#include "../common/classes/alloc.h"
#include "../common/classes/array.h"
#include "../common/classes/NestConst.h"
#include "../jrd/jrd.h"
#include "../jrd/exe.h"
#include "../dsql/Visitors.h"

namespace Jrd {

class OptimizerRetrieval;
class ProcedureScan;
class RseNode;


class RecordSourceNode : public Firebird::PermanentStorage
{
public:
	enum Type
	{
		TYPE_RELATION,
		TYPE_PROCEDURE,
		TYPE_AGGREGATE,
		TYPE_UNION,
		TYPE_WINDOW,
		TYPE_RSE
	};

	RecordSourceNode(Type aType, MemoryPool& pool)
		: PermanentStorage(pool),
		  type(aType),
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

	// Identify the streams that make up a RseNode.
	virtual void getStreams(StreamsArray& list) const
	{
		list.add(getStream());
	}

	virtual RecordSourceNode* copy(thread_db* tdbb, NodeCopier& copier) = 0;
	virtual void ignoreDbKey(thread_db* tdbb, CompilerScratch* csb, const jrd_rel* view) const = 0;
	virtual void pass1(thread_db* tdbb, CompilerScratch* csb, jrd_rel* view) = 0;
	virtual void pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
		jrd_nod** boolean, RecordSourceNodeStack& stack) = 0;
	virtual void pass2(thread_db* tdbb, CompilerScratch* csb) = 0;
	virtual void pass2Rse(thread_db* tdbb, CompilerScratch* csb) = 0;
	virtual bool containsStream(USHORT checkStream) const = 0;

	// Identify all of the streams for which a dbkey may need to be carried through a sort.
	virtual void computeDbKeyStreams(UCHAR* streams) const = 0;

	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
		bool allowOnlyCurrentStream, jrd_nod* value) = 0;
	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList) = 0;
	virtual RecordSource* compile(thread_db* tdbb, OptimizerBlk* opt, bool innerSubStream) = 0;

public:
	const Type type;

protected:
	USHORT stream;
};

class RelationSourceNode : public TypedNode<RecordSourceNode, RecordSourceNode::TYPE_RELATION>
{
public:
	explicit RelationSourceNode(MemoryPool& pool)
		: TypedNode<RecordSourceNode, RecordSourceNode::TYPE_RELATION>(pool),
		  relation(NULL),
		  context(NULL),
		  alias(NULL),
		  view(NULL)
	{
	}

	static RelationSourceNode* parse(thread_db* tdbb, CompilerScratch* csb, SSHORT blrOp,
		bool parseContext);

	virtual RelationSourceNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual void ignoreDbKey(thread_db* tdbb, CompilerScratch* csb, const jrd_rel* view) const;

	virtual void pass1(thread_db* tdbb, CompilerScratch* csb, jrd_rel* view)
	{
	}

	virtual void pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
		jrd_nod** boolean, RecordSourceNodeStack& stack);

	virtual void pass2(thread_db* tdbb, CompilerScratch* csb)
	{
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

	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
		bool allowOnlyCurrentStream, jrd_nod* value)
	{
		return true;
	}

	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList)
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
		  inputs(NULL),
		  in_msg(NULL),
		  procedure(NULL),
		  view(NULL),
		  context(NULL)
	{
	}

	static ProcedureSourceNode* parse(thread_db* tdbb, CompilerScratch* csb, SSHORT blrOp);

	virtual ProcedureSourceNode* copy(thread_db* tdbb, NodeCopier& copier);

	virtual void ignoreDbKey(thread_db* tdbb, CompilerScratch* csb, const jrd_rel* view) const
	{
	}

	virtual void pass1(thread_db* tdbb, CompilerScratch* csb, jrd_rel* view);
	virtual void pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
		jrd_nod** boolean, RecordSourceNodeStack& stack);
	virtual void pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass2Rse(thread_db* tdbb, CompilerScratch* csb);

	virtual bool containsStream(USHORT checkStream) const
	{
		return false;
	}

	virtual void computeDbKeyStreams(UCHAR* streams) const
	{
	}

	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
		bool allowOnlyCurrentStream, jrd_nod* value);
	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList);
	virtual RecordSource* compile(thread_db* tdbb, OptimizerBlk* opt, bool innerSubStream);

private:
	ProcedureScan* generate(thread_db* tdbb, OptimizerBlk* opt);

public:
	NestConst<jrd_nod> inputs;

private:
	NestConst<jrd_nod> in_msg;
	USHORT procedure;
	jrd_rel* view;
	SSHORT context;
};

class AggregateSourceNode : public TypedNode<RecordSourceNode, RecordSourceNode::TYPE_AGGREGATE>
{
public:
	explicit AggregateSourceNode(MemoryPool& pool)
		: TypedNode<RecordSourceNode, RecordSourceNode::TYPE_AGGREGATE>(pool),
		  group(NULL),
		  map(NULL),
		  rse(NULL)
	{
	}

	static AggregateSourceNode* parse(thread_db* tdbb, CompilerScratch* csb);

	virtual AggregateSourceNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual void ignoreDbKey(thread_db* tdbb, CompilerScratch* csb, const jrd_rel* view) const;
	virtual void pass1(thread_db* tdbb, CompilerScratch* csb, jrd_rel* view);
	virtual void pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
		jrd_nod** boolean, RecordSourceNodeStack& stack);
	virtual void pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass2Rse(thread_db* tdbb, CompilerScratch* csb);
	virtual bool containsStream(USHORT checkStream) const;

	virtual void computeDbKeyStreams(UCHAR* streams) const
	{
	}

	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
		bool allowOnlyCurrentStream, jrd_nod* value);
	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList);
	virtual RecordSource* compile(thread_db* tdbb, OptimizerBlk* opt, bool innerSubStream);

private:
	RecordSource* generate(thread_db* tdbb, OptimizerBlk* opt, NodeStack* parentStack,
		UCHAR shellStream);

public:
	NestConst<jrd_nod> group;
	NestConst<jrd_nod> map;

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
		  mapStream(NULL),
		  recursive(false)
	{
	}

	static UnionSourceNode* parse(thread_db* tdbb, CompilerScratch* csb, SSHORT blrOp);

	virtual UnionSourceNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual void ignoreDbKey(thread_db* tdbb, CompilerScratch* csb, const jrd_rel* view) const;

	virtual void pass1(thread_db* tdbb, CompilerScratch* csb, jrd_rel* view)
	{
	}

	virtual void pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
		jrd_nod** boolean, RecordSourceNodeStack& stack);
	virtual void pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass2Rse(thread_db* tdbb, CompilerScratch* csb);
	virtual bool containsStream(USHORT checkStream) const;
	virtual void computeDbKeyStreams(UCHAR* streams) const;
	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
		bool allowOnlyCurrentStream, jrd_nod* value);
	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList);
	virtual RecordSource* compile(thread_db* tdbb, OptimizerBlk* opt, bool innerSubStream);

private:
	RecordSource* generate(thread_db* tdbb, OptimizerBlk* opt, UCHAR* streams, USHORT nstreams,
		NodeStack* parentStack, UCHAR shellStream);

private:
	Firebird::Array<NestConst<RseNode> > clauses;	// RseNode's for union
	Firebird::Array<NestConst<jrd_nod> > maps;		// RseNode's maps
	USHORT mapStream;	// stream for next level record of recursive union
	bool recursive;		// union node is a recursive union
};

class WindowSourceNode : public TypedNode<RecordSourceNode, RecordSourceNode::TYPE_WINDOW>
{
public:
	struct Partition
	{
		Partition()
			: stream(MAX_USHORT)
		{
		}

		USHORT stream;
		NestConst<jrd_nod> group;
		NestConst<jrd_nod> regroup;
		NestConst<jrd_nod> order;
		NestConst<jrd_nod> map;
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
	virtual void ignoreDbKey(thread_db* tdbb, CompilerScratch* csb, const jrd_rel* view) const;
	virtual void pass1(thread_db* tdbb, CompilerScratch* csb, jrd_rel* view);
	virtual void pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
		jrd_nod** boolean, RecordSourceNodeStack& stack);
	virtual void pass2(thread_db* tdbb, CompilerScratch* csb);
	virtual void pass2Rse(thread_db* tdbb, CompilerScratch* csb);
	virtual bool containsStream(USHORT checkStream) const;

	virtual void computeDbKeyStreams(UCHAR* streams) const
	{
	}

	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
		bool allowOnlyCurrentStream, jrd_nod* value);
	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList);
	virtual RecordSource* compile(thread_db* tdbb, OptimizerBlk* opt, bool innerSubStream);

private:
	NestConst<RseNode> rse;
	Firebird::Array<Partition> partitions;
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

	static RseNode* getFrom(jrd_nod* node)
	{
		fb_assert(node->nod_type == nod_class_recsrcnode_jrd);
		return reinterpret_cast<RseNode*>(node->nod_arg[0]);
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

	virtual void getStreams(StreamsArray& list) const
	{
	}

	virtual RseNode* copy(thread_db* tdbb, NodeCopier& copier);
	virtual void ignoreDbKey(thread_db* tdbb, CompilerScratch* csb, const jrd_rel* view) const;
	virtual void pass1(thread_db* tdbb, CompilerScratch* csb, jrd_rel* view);
	virtual void pass1Source(thread_db* tdbb, CompilerScratch* csb, RseNode* rse,
		jrd_nod** boolean, RecordSourceNodeStack& stack);

	virtual void pass2(thread_db* tdbb, CompilerScratch* csb)
	{
	}

	virtual void pass2Rse(thread_db* tdbb, CompilerScratch* csb);
	virtual bool containsStream(USHORT checkStream) const;
	virtual void computeDbKeyStreams(UCHAR* streams) const;
	virtual bool computable(CompilerScratch* csb, SSHORT stream, bool idx_use,
		bool allowOnlyCurrentStream, jrd_nod* value);
	virtual void findDependentFromStreams(const OptimizerRetrieval* optRet,
		SortedStreamList* streamList);
	virtual RecordSource* compile(thread_db* tdbb, OptimizerBlk* opt, bool innerSubStream);

private:
	void computeRseStreams(const CompilerScratch* csb, UCHAR* streams) const;
	void planCheck(const CompilerScratch* csb) const;
	static void planSet(CompilerScratch* csb, jrd_nod* plan);

public:
	USHORT rse_jointype;		// inner, left, full
	NestConst<jrd_nod> rse_first;
	NestConst<jrd_nod> rse_skip;
	NestConst<jrd_nod> rse_boolean;
	NestConst<jrd_nod> rse_sorted;
	NestConst<jrd_nod> rse_projection;
	NestConst<jrd_nod> rse_aggregate;	// singleton aggregate for optimizing to index
	NestConst<jrd_nod> rse_plan;		// user-specified access plan
	NestConst<VarInvariantArray> rse_invariants; // Invariant nodes bound to top-level RSE
	Firebird::Array<NestConst<RecordSourceNode> > rse_relations;
	unsigned flags;
};


} //namespace Jrd

#endif	// JRD_RECORD_SOURCE_NODES_H
