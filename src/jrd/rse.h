/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		rse.h
 *	DESCRIPTION:	Record source block definitions
 *
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
 *
 * 2001.07.28: Added rsb_t.rsb_skip to support LIMIT functionality.
 */

#ifndef JRD_RSE_H
#define JRD_RSE_H

#include "../include/fb_blk.h"
#include "../common/classes/array.h"
#include "../jrd/constants.h"
#include "../common/dsc.h"
#include "../jrd/lls.h"
#include "../jrd/sbm.h"
#include "../jrd/ExtEngineManager.h"
#include "../jrd/RecordBuffer.h"
#include "../dsql/Nodes.h"

struct dsc;

namespace Jrd {

class jrd_nod;
struct sort_key_def;
class CompilerScratch;
class BoolExprNode;

// Array which stores relative pointers to impure areas of invariant nodes
typedef Firebird::SortedArray<ULONG> VarInvariantArray;

class RseNode;

// General optimizer block

class OptimizerBlk : public pool_alloc<type_opt>
{
public:
	OptimizerBlk(MemoryPool* pool, RseNode* aRse, BoolExprNodeStack* aParentStack)
		: opt_conjuncts(*pool),
		  opt_streams(*pool),
		  rse(aRse),
		  parentStack(aParentStack),
		  outerStreams(*pool),
		  subStreams(*pool),
		  conjunctStack(*pool),
		  conjunctCount(0)
	{
		compileStreams[0] = beds[0] = localStreams[0] = keyStreams[0] = 0;
	}

	CompilerScratch* opt_csb;				// compiler scratch block
	double opt_best_cost;					// cost of best join order
	USHORT opt_best_count;					// longest length of indexable streams
	USHORT opt_base_conjuncts;				// number of conjuncts in our rse, next conjuncts are distributed parent
	USHORT opt_base_parent_conjuncts;		// number of conjuncts in our rse + distributed with parent, next are parent
	USHORT opt_base_missing_conjuncts;		// number of conjuncts in our and parent rse, but without missing

	struct opt_conjunct
	{
		// Conjunctions and their options
		BoolExprNode* opt_conjunct_node;	// conjunction
		UCHAR opt_conjunct_flags;
	};

	struct opt_stream
	{
		// Streams and their options
		USHORT opt_best_stream;					// stream in best join order seen so far
		USHORT opt_stream_number;				// stream in position of join order
	};

	Firebird::HalfStaticArray<opt_conjunct, OPT_STATIC_ITEMS> opt_conjuncts;
	Firebird::HalfStaticArray<opt_stream, OPT_STATIC_ITEMS> opt_streams;

	RseNode* const rse;
	BoolExprNodeStack* parentStack;
	StreamsArray outerStreams, subStreams;
	BoolExprNodeStack conjunctStack;
	SLONG conjunctCount;
	stream_array_t compileStreams, beds, localStreams, keyStreams;
};

// values for opt_conjunct_flags

const USHORT opt_conjunct_used = 1;			// conjunct is used
const USHORT opt_conjunct_matched = 2;		// conjunct matches an index segment

} //namespace Jrd

#endif // JRD_RSE_H
