/*
 *	PROGRAM:	Client/Server Common Code
 *	MODULE:		Optimizer.h
 *	DESCRIPTION:	Optimizer
 *
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
 *  The Original Code was created by Arno Brinkman
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Arno Brinkman <firebird@abvisie.nl>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#ifndef OPTIMIZER_H
#define OPTIMIZER_H

//#define OPT_DEBUG

#ifdef OPT_DEBUG
#define OPTIMIZER_DEBUG_FILE "opt_debug.out"
#endif


#include "../common/classes/alloc.h"
#include "../common/classes/array.h"

#include "../jrd/rse.h"
#include "../jrd/exe.h"

namespace Jrd {

class jrd_nod;
struct index_desc;
class OptimizerBlk;
class jrd_rel;

bool computable(CompilerScratch*, jrd_nod*, SSHORT, bool, bool);
#ifdef EXPRESSION_INDICES
bool expression_equal(thread_db*, OptimizerBlk*, const index_desc*,
							 jrd_nod*, USHORT);
bool expression_equal2(thread_db*, OptimizerBlk*, jrd_nod*,
							  jrd_nod*, USHORT);
#endif
double getRelationCardinality(thread_db*, jrd_rel*,const Format*);
str* make_alias(thread_db*, CompilerScratch*, CompilerScratch::csb_repeat*);
jrd_nod* make_binary_node(NOD_T, jrd_nod*, jrd_nod*, bool);
USHORT nav_rsb_size(RecordSource*, USHORT, USHORT);

inline int STREAM_INDEX(const jrd_nod* node)
{
	switch (node->nod_type)
	{
		case nod_relation:
			return e_rel_stream;
		case nod_procedure:
			return e_prc_stream;
		case nod_union:
			return e_uni_stream;
		default:
			return e_agg_stream;
	}
}

enum segmentScanType {
	segmentScanNone,
	segmentScanGreater,
	segmentScanLess,
	segmentScanBetween,
	segmentScanEqual,
	segmentScanEquivalent,
	segmentScanMissing,
	segmentScanStarting
};

class IndexScratchSegment 
{
public:
	IndexScratchSegment(MemoryPool& p);
	IndexScratchSegment(MemoryPool& p, IndexScratchSegment* segment);

	jrd_nod* lowerValue;		// lower bound on index value
	jrd_nod* upperValue;		// upper bound on index value
	int scope;					// highest scope level
	segmentScanType scanType;	// scan type

	Firebird::Array<jrd_nod*> matches;
};

class IndexScratch 
{
public:
	IndexScratch(MemoryPool& p, index_desc* idx);
	IndexScratch(MemoryPool& p, IndexScratch* scratch);
	~IndexScratch();

	index_desc*	idx;				// index descriptor
	double selectivity;				// calculated selectivity for this index
	bool candidate;					// used when deciding which indices to use
	bool scopeCandidate;			// used when making inversion based on scope
	int lowerCount;					//
	int upperCount;					//
	int nonFullMatchedSegments;		//

	Firebird::Array<IndexScratchSegment*> segments;
};

typedef Firebird::SortedArray<int> SortedStreamList;

class InversionCandidate
{
public:
	InversionCandidate(MemoryPool& p);

	double			selectivity;
	USHORT			nonFullMatchedSegments;
	USHORT			matchedSegments;
	int				indexes;
	jrd_nod*		boolean;
	jrd_nod*		inversion;
	IndexScratch*	scratch;
	bool			used;
	bool			unique;

	Firebird::Array<jrd_nod*> matches;
	SortedStreamList dependentFromStreams;
};

typedef Firebird::HalfStaticArray<InversionCandidate*, 16> InversionCandidateList;
typedef Firebird::Array<IndexScratch*> IndexScratchList;

class OptimizerRetrieval
{
public:
	OptimizerRetrieval(MemoryPool& p, OptimizerBlk* opt, SSHORT streamNumber, 
		bool outer, bool inner, jrd_nod** sortNode);
	~OptimizerRetrieval();

	InversionCandidate* getCost();
	InversionCandidate* getInversion(RecordSource** rsb);

protected:
	jrd_nod* composeInversion(jrd_nod* node1, jrd_nod* node2, NOD_T node_type);
	void findDependentFromStreams(jrd_nod* node, SortedStreamList* streamList);
	str* getAlias();
	InversionCandidate* generateInversion(RecordSource** rsb);
	RecordSource* generateNavigation();
	bool getInversionCandidates(InversionCandidateList* inversions, 
		IndexScratchList* indexScratches, USHORT scope);
	jrd_nod* makeIndexNode(const index_desc* idx);
	jrd_nod* makeIndexScanNode(IndexScratch* indexScratch);
	InversionCandidate* makeInversion(InversionCandidateList* inversions);
	bool matchBoolean(IndexScratch* indexScratch, jrd_nod* boolean, USHORT scope);
	InversionCandidate* matchOnIndexes(IndexScratchList* indexScratches,
		jrd_nod* boolean, USHORT scope);
	bool validateStarts(IndexScratch* indexScratch, jrd_nod* boolean, USHORT segment);
private:
	MemoryPool& pool;
	thread_db* tdbb;
	SSHORT stream;
	str* alias;
	jrd_nod** sort;
	jrd_rel* relation;
	CompilerScratch* csb;
	Database* database;
	OptimizerBlk* optimizer;
	IndexScratchList indexScratches;
	InversionCandidateList inversionCandidates;
	bool innerFlag;
	bool outerFlag; 
	bool createIndexScanNodes;
	bool setConjunctionsMatched;
};

class IndexRelationship
{
public:
	IndexRelationship();

	int		stream;
	bool	unique;
	double	cost;
	double	cardinality;
};

typedef Firebird::Array<IndexRelationship*> IndexedRelationships;

class InnerJoinStreamInfo
{
public:
	InnerJoinStreamInfo(MemoryPool& p);
	bool independent();

	int		stream;
	bool	baseUnique;
	double	baseCost;
	int		baseIndexes;
	int		baseConjunctionMatches;
	bool	used;

	IndexedRelationships indexedRelationships;
	Firebird::Array<int> previousExpectedStreams;
};

typedef Firebird::HalfStaticArray<InnerJoinStreamInfo*, 8> StreamInfoList;

class OptimizerInnerJoin
{
public:
	OptimizerInnerJoin(MemoryPool& p, OptimizerBlk* opt, UCHAR*	streams,
		RiverStack& river_stack, jrd_nod** sort_clause, 
		jrd_nod** project_clause, jrd_nod* plan_clause);
	~OptimizerInnerJoin();

	int findJoinOrder();

protected:
	void calculateCardinalities();
	void calculateStreamInfo();
	bool cheaperRelationship(IndexRelationship* checkRelationship,
		IndexRelationship* withRelationship);
	bool estimateCost(USHORT stream, double *cost, 
		double *resulting_cardinality);
	void findBestOrder(int position, InnerJoinStreamInfo* stream, 
		IndexedRelationships* processList, double cost, double cardinality);
	void getIndexedRelationship(InnerJoinStreamInfo* baseStream, 
		InnerJoinStreamInfo* testStream);
	InnerJoinStreamInfo* getStreamInfo(int stream);
#ifdef OPT_DEBUG
	void printFoundOrder(int position, double cost, double cardinality);
	void printProcessList(IndexedRelationships* processList, int stream);
#endif

private:
	MemoryPool& pool;
	thread_db* tdbb;
	jrd_nod** sort;
	jrd_nod** project;
	jrd_nod* plan;
	CompilerScratch* csb;
	Database* database;
	OptimizerBlk* optimizer;
	StreamInfoList innerStreams;
	int remainingStreams;
};


} // namespace Jrd

#endif // OPTIMIZER_H

