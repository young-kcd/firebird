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

// This is really funny: class rse is not defined here but in exe.h!!!

#include "../include/fb_blk.h"

#include "../common/classes/array.h"
#include "../jrd/constants.h"

#include "../jrd/dsc.h"
#include "../jrd/lls.h"
#include "../jrd/sbm.h"

#include "../jrd/ExtEngineManager.h"
#include "../jrd/RecordBuffer.h"

struct dsc;

namespace Jrd {

class jrd_nod;
struct sort_key_def;
class CompilerScratch;

// Array which stores relative pointers to impure areas of invariant nodes
typedef Firebird::SortedArray<SLONG> VarInvariantArray;

// Blocks used to compute optimal join order:
// indexed relationships block (IRL) holds
// information about potential join orders

class IndexedRelationship : public pool_alloc<type_irl>
{
public:
	IndexedRelationship*	irl_next;		// next IRL block for stream
	USHORT					irl_stream;		// stream reachable by relation
	bool					irl_unique;		// is this stream reachable by unique index?
};

// Must be less then MAX_SSHORT. Not used for static arrays.
const int MAX_CONJUNCTS	= 32000;

// Note that MAX_STREAMS currently MUST be <= MAX_UCHAR.
// Here we should really have a compile-time fb_assert, since this hard-coded
// limit is NOT negotiable so long as we use an array of UCHAR, where index 0
// tells how many streams are in the array (and the streams themselves are
// identified by a UCHAR).
const int MAX_STREAMS	= 255;

// This is number of ULONG's needed to store bit-mapped flags for all streams
// OPT_STREAM_BITS = (MAX_STREAMS + 1) / sizeof(ULONG)
// This value cannot be increased simple way. Decrease is possible, but it is also
// hardcoded in several places such as TEST_DEP_ARRAYS macro
const int OPT_STREAM_BITS	= 8;

// Number of streams, conjuncts, indices that will be statically allocated
// in various arrays. Larger numbers will have to be allocated dynamically
const int OPT_STATIC_ITEMS = 16;


// General optimizer block

class OptimizerBlk : public pool_alloc<type_opt>
{
public:
	CompilerScratch*	opt_csb;					// compiler scratch block
	SLONG opt_combinations;					// number of partial orders considered
	double opt_best_cost;					// cost of best join order
	USHORT opt_best_count;					// longest length of indexable streams
	USHORT opt_base_conjuncts;				// number of conjuncts in our rse, next conjuncts are distributed parent
	USHORT opt_base_parent_conjuncts;		// number of conjuncts in our rse + distributed with parent, next are parent
	USHORT opt_base_missing_conjuncts;		// number of conjuncts in our and parent rse, but without missing
	// 01 Oct 2003. Nickolay Samofatov: this static array takes as much as 256 bytes.
	// This is nothing compared to original Firebird 1.5 OptimizerBlk structure size of ~180k
	// All other arrays had been converted to dynamic to preserve memory
	// and improve performance
	struct opt_segment
	{
		// Index segments and their options
		jrd_nod* opt_lower;			// lower bound on index value
		jrd_nod* opt_upper;			// upper bound on index value
		jrd_nod* opt_match;			// conjunct which matches index segment
	} opt_segments[MAX_INDEX_SEGMENTS];
	struct opt_conjunct
	{
		// Conjunctions and their options
		jrd_nod* opt_conjunct_node;	// conjunction
		// Stream dependencies to compute conjunct
		ULONG opt_dependencies[(MAX_STREAMS + 1) / 32];
		UCHAR opt_conjunct_flags;
	};
	struct opt_stream
	{
		// Streams and their options
		IndexedRelationship* opt_relationships;	// streams directly reachable by index
		double opt_best_stream_cost;			// best cost of retrieving first n = streams
		USHORT opt_best_stream;					// stream in best join order seen so far
		USHORT opt_stream_number;				// stream in position of join order
		UCHAR opt_stream_flags;
	};
	Firebird::HalfStaticArray<opt_conjunct, OPT_STATIC_ITEMS> opt_conjuncts;
	Firebird::HalfStaticArray<opt_stream, OPT_STATIC_ITEMS> opt_streams;
	OptimizerBlk(MemoryPool* pool) : opt_conjuncts(*pool), opt_streams(*pool) {}
};

// values for opt_stream_flags

const USHORT opt_stream_used = 1;			// stream is used

// values for opt_conjunct_flags

const USHORT opt_conjunct_used = 1;			// conjunct is used
const USHORT opt_conjunct_matched = 2;		// conjunct matches an index segment


// River block - used to hold temporary information about a group of streams
// CVC: River is a "secret" of opt.cpp, maybe a new opt.h would be adequate.

class River : public pool_alloc_rpt<SCHAR, type_riv>
{
public:
	RecordSource* riv_rsb;		// record source block for river
	USHORT riv_number;			// temporary number for river
	UCHAR riv_count;			// count of streams
	UCHAR riv_streams[1];		// actual streams
};

} //namespace Jrd

#endif // JRD_RSE_H
