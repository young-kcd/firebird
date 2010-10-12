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

#include "../common/dsc.h"
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
typedef Firebird::SortedArray<ULONG> VarInvariantArray;

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


// General optimizer block

class OptimizerBlk : public pool_alloc<type_opt>
{
public:
	CompilerScratch*	opt_csb;					// compiler scratch block
	double opt_best_cost;					// cost of best join order
	USHORT opt_best_count;					// longest length of indexable streams
	USHORT opt_base_conjuncts;				// number of conjuncts in our rse, next conjuncts are distributed parent
	USHORT opt_base_parent_conjuncts;		// number of conjuncts in our rse + distributed with parent, next are parent
	USHORT opt_base_missing_conjuncts;		// number of conjuncts in our and parent rse, but without missing
	struct opt_conjunct
	{
		// Conjunctions and their options
		jrd_nod* opt_conjunct_node;	// conjunction
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
	OptimizerBlk(MemoryPool* pool) : opt_conjuncts(*pool), opt_streams(*pool) {}
};

// values for opt_conjunct_flags

const USHORT opt_conjunct_used = 1;			// conjunct is used
const USHORT opt_conjunct_matched = 2;		// conjunct matches an index segment

} //namespace Jrd

#endif // JRD_RSE_H
