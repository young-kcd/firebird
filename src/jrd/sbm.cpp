/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sbm.cpp
 *	DESCRIPTION:	Sparse Bit Map manager
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
 */

#include "firebird.h"
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/req.h"
#include "../jrd/sbm.h"
#include "../jrd/all.h"
#include "../jrd/rse.h"
#include "../jrd/all_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/sbm_proto.h"
#include "../jrd/thd.h"

using namespace Jrd;

static void bucket_reset(SparseBitmap*);
static void clear_bucket(SparseBitmap*);
static void clear_segment(BitmapSegment*);

/* Stores a constant determined during initialization */
static ULONG SBM_max_tail;


SparseBitmap** SBM_and(SparseBitmap** bitmap1, SparseBitmap** bitmap2)
{
/**************************************
 *
 *	S B M _ a n d
 *
 **************************************
 *
 * Functional description
 *	Compute the intersection of two bitmaps.  Note: this may
 *	destroy one or the other of the bitmaps!
 *
 **************************************/
	SparseBitmap* map1 = (bitmap1) ? *bitmap1 : NULL;
	SparseBitmap* map2 = (bitmap2) ? *bitmap2 : NULL;

/* If either bitmap is null, so is the intersection.  */

	if (!map1 || !map2 ||
		map1->sbm_state == SBM_EMPTY || map2->sbm_state == SBM_EMPTY)
	{
			return NULL;
	}

	if (map1->sbm_state == SBM_SINGULAR) {
		if (SBM_test(map2, map1->sbm_number))
			return bitmap1;
		else
			return NULL;
	}

	if (map2->sbm_state == SBM_SINGULAR) {
		if (SBM_test(map1, map2->sbm_number))
			return bitmap2;
		else
			return NULL;
	}

/* Both bitmaps exist.  Make sure that the shorter of the two is
   bitmap1.  If not, flip them. */

	SparseBitmap** result = bitmap1;

	if (map1->sbm_high_water > map2->sbm_high_water) {
		map2 = *bitmap1;
		map1 = *bitmap2;
		result = bitmap2;
	}

	if (map1->sbm_type == SBM_ROOT) {
		SparseBitmap::iterator bucket1 = map1->sbm_segments.begin();
		SparseBitmap::iterator bucket2 = map2->sbm_segments.begin();
		const SparseBitmap::const_iterator end_buckets = bucket1 + map1->sbm_high_water + 1;

		for (; bucket1 < end_buckets; bucket1++, bucket2++) {
			SparseBitmap** result_bucket;
			if (*bucket1) {
				if (!*bucket2) {
					*bucket2 = *bucket1;
					*bucket1 = NULL;
				}
				else if (!(result_bucket =
					SBM_and((SparseBitmap**) &*bucket1, (SparseBitmap**) &*bucket2))) {
					bucket_reset((SparseBitmap*) *bucket1);
					*bucket1 = NULL;
				}
				else if (result_bucket == (SparseBitmap**) &*bucket2) {
					SparseBitmap* const temp = (SparseBitmap*)*bucket2;
					*bucket2 = *bucket1;
					*bucket1 = (BitmapSegment*)temp;
				}
			}
		}
	}
	else {
		/* AND the bitmaps segment-wise.  If each bucket has a segment
		   is a given position, AND the segment bit-wise. */

		SparseBitmap::iterator segment1 = map1->sbm_segments.begin();
		SparseBitmap::const_iterator segment2 = map2->sbm_segments.begin();
		const SparseBitmap::const_iterator end_segments = segment1 + map1->sbm_high_water + 1;

		for (; segment1 < end_segments; segment1++, segment2++) {
			if (*segment1) {
				if (!*segment2) {
					JrdMemoryPool* pool = (*segment1)->bms_pool;
					(*segment1)->bms_next = pool->plb_segments;
					pool->plb_segments = *segment1;
					*segment1 = NULL;
					continue;
				}
				BUNCH* b1 = (*segment1)->bms_bits;
				const BUNCH* b2 = (*segment2)->bms_bits;
				for (USHORT j = 0; j < BUNCH_SEGMENT; j++) {
					*b1++ &= *b2++;
				}
			}
		}
	}

	return result;
}


bool SBM_clear(SparseBitmap* bitmap, SLONG number)
{
/**************************************
 *
 *	S B M _ c l e a r
 *
 **************************************
 *
 * Functional description
 *	Clear a bit in a sparse bitmap.
 *	Return true if the bit was found set, else return false.
 *
 **************************************/

/* If the bitmap is completely missing, or is known to be empty,
   give up immediately.  */

	if (!bitmap || bitmap->sbm_state == SBM_EMPTY)
		return false;

/* If the bitmap is singular (represents a single value), return it
   if appropriate, else indicate bitmap exhausted. */

	if (bitmap->sbm_state == SBM_SINGULAR) {
		if (number == bitmap->sbm_number) {
			bitmap->sbm_state = SBM_EMPTY;
			return true;
		}
		else
			return false;
	}

	if (bitmap->sbm_type == SBM_ROOT) {
		const USHORT slot = number >> BUCKET_BITS;
// USHORT slot is never < 0
		SparseBitmap* bucket;
		if (slot > bitmap->sbm_high_water ||
			!(bucket = (SparseBitmap*) bitmap->sbm_segments[slot]))
		{
			return false;
		}

		const SLONG relative = number & ((1 << BUCKET_BITS) - 1);
		return SBM_clear(bucket, relative);
	}
	else {
		const USHORT slot = number >> SEGMENT_BITS;
// USHORT slot is never < 0
		BitmapSegment* segment;
		if (slot > bitmap->sbm_high_water ||
			!(segment = bitmap->sbm_segments[slot]))
		{
			return false;
		}

		const SLONG relative = number & ((1 << SEGMENT_BITS) - 1);
		const SSHORT bunch =
			(relative >> BUNCH_BITS) & ((1 << (SEGMENT_BITS - BUNCH_BITS)) -
										1);

		BUNCH test = segment->bms_bits[bunch];
		if (!test)
			return false;

		const SSHORT bit = relative & ((1 << BUNCH_BITS) - 1);

		if (!(test & (1 << bit)))
			return false;

		test &= ~(1 << bit);

		/* store value back in bit map */

		segment->bms_bits[bunch] = test;
		return true;
	}
}


#ifdef DEV_BUILD
void SBM_dump(FILE* f, SparseBitmap* bitmap1)
{
/**************************************
 *
 *	S B M _ d u m p
 *
 **************************************
 *
 * Functional description
 *	Print out a dump of a bitmap.
 **************************************/
	SLONG bit1, last_bit;
	USHORT counter = 0;
	bit1 = -1;
	last_bit = bit1 - 1;

	fprintf(f, "State: %s", (bitmap1->sbm_state == SBM_EMPTY) ? "EMPTY" :
			   (bitmap1->sbm_state == SBM_SINGULAR) ? "Singular" :
			   (bitmap1->sbm_state == SBM_PLURAL) ? "Plural" : "Bogus");
	fprintf(f, " Type: %s", (bitmap1->sbm_type == SBM_BUCKET) ? "Bucket" :
			   (bitmap1->sbm_type == SBM_ROOT) ? "root" : "Bogus");
	fprintf(f, " Count %d Used %d high %d\n",
			   bitmap1->sbm_count, bitmap1->sbm_used,
			   bitmap1->sbm_high_water);

	bool in_range = false;
	while (true) {
		if (!SBM_next(bitmap1, &bit1, RSE_get_forward))
			break;
		if (bit1 == last_bit + 1) {
			last_bit = bit1;
			in_range = true;
			continue;
		}
		if (in_range)
			fprintf(f, "-%"SLONGFORMAT, last_bit);
		if ((++counter % 8) == 0)
			fprintf(f, "\n");
		fprintf(f, " %"SLONGFORMAT, bit1);
		last_bit = bit1;
		in_range = false;
	}

	if (in_range)
		fprintf(f, "-%"SLONGFORMAT, last_bit);
	fprintf(f, "\n");
}
#endif


bool SBM_equal(SparseBitmap* bitmap1, SparseBitmap* bitmap2)
{
/**************************************
 *
 *	S B M _ e q u a l
 *
 **************************************
 *
 * Functional description
 *	Return true if two sparse bitmaps are equal (eg: have the
 *	exact same set of bits set).
 *	The test is non-destructive of both bitmaps.
 *
 *	This implementation is not optimal - as it just probes
 *	each bit individually rather than using the inards
 *	for a quicker probe.
 *
 **************************************/
/* Check the highest bit in the bitmap - just as
   a quick optimization */
	SLONG bit1, bit2;
	bit1 = bit2 = -1;
	if ((SBM_next(bitmap1, &bit1, RSE_get_backward) !=
		 SBM_next(bitmap2, &bit2, RSE_get_backward))
		|| (bit1 != bit2))
	{
		return false;
	}

/* Now check each bit */
	bit1 = bit2 = -1;
	while (true) {
		const bool res1 = SBM_next(bitmap1, &bit1, RSE_get_forward);
		const bool res2 = SBM_next(bitmap2, &bit2, RSE_get_forward);

		/* Reached end of one bitmap, but not the other? */
		if (res1 != res2)
			return false;

		/* Have we reached the end of both bitmaps? */
		if (!res1)
			return true;

		/* Is the next-bit-set the same for each? */
		if (bit1 != bit2)
			return false;
	}
}


void SBM_init(void)
{
/**************************************
 *
 *	S B M _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Init sparse bit map constant
 *
 **************************************/

	/* JMB:
	 * There is no more ALL_tail function to call, so the following line:
	 *   SBM_max_tail = plb::ALL_tail(type_sbm);
	 * gets removed.  But what do we set SBM_max_tail to?  We need to know
	 * how it it used.  Here is the scoop:
	 * When extending a bitmap, if the needed capacity is less then SBM_max_tail
	 * we _double_ the current size until we have enough capacity.
	 * If the needed size is greator then SBM_max_tail we increase capacity
	 * to 5 more than needed, instead of doubling.
	 *
	 * The bottom line is SBM_max_tail is not a max, it is just a threshold
	 * the allocation scheme changes around.  The FB1 value is kept for
	 * a lack of a better value.
	 */
	SBM_max_tail = ((MAX_USHORT - 32 - sizeof(SparseBitmap)) / sizeof(BitmapSegment*)) + 1;
}


bool SBM_next(SparseBitmap* bitmap, SLONG* number, RSE_GET_MODE mode)
{
/**************************************
 *
 *	S B M _ n e x t
 *
 **************************************
 *
 * Functional description
 *	Find the next bit set in a bitmap after a given position.
 *	Return true if a fit is found, else return false.
 *	Handle forwards or backwards traversal of the bitmap,
 *	or just return the currently set bit.
 *
 **************************************/

/* If the bitmap is completely missing, or is known to be empty,
   give up immediately.  */

	if (!bitmap || bitmap->sbm_state == SBM_EMPTY)
		return false;

/* If the bitmap is singular (represents a single value), return it
   if appropriate, else indicate bitmap exhausted. */

	if (bitmap->sbm_state == SBM_SINGULAR) {
		if (mode == RSE_get_forward && *number >= bitmap->sbm_number)
			return false;
		if (mode == RSE_get_backward &&
			*number <= bitmap->sbm_number && *number != -1)
			return false;
#ifdef PC_ENGINE
		if (mode == RSE_get_current && *number != bitmap->sbm_number)
			return false;
#endif

		*number = bitmap->sbm_number;
		return true;
	}

/* Advance the number and search for the next bit set */

	SLONG relative, slot;
	if (bitmap->sbm_type == SBM_ROOT) {
		if (mode == RSE_get_forward)
			(*number)++;
		else if (mode == RSE_get_backward)
			if (!*number)
				return false;
			else if (*number > 0)
				(*number)--;

		if (*number == -1) {
			/* if the number is -1, indicate to get the last bit in the bucket */

			slot = bitmap->sbm_high_water;
			relative = -1;
		}
		else {
			/* find the bucket this number fits into, and the relative
			   position from the beginning of the bucket via a fast modulo */

			slot = *number >> BUCKET_BITS;
			relative = *number & ((1 << BUCKET_BITS) - 1);

			if (mode == RSE_get_forward && !relative)
				relative = -1;
		}

		/* garbage collection occurs when a bucket is completely empty from beginning 
		   to end; flag that we started at the beginning of the bucket in this pass */

		bool garbage_collect = (relative == -1);

		for (;;) {
			if (slot < 0 || slot > (SLONG) bitmap->sbm_high_water)
				break;

			/* recursively find the next bit set within this bucket */

			SparseBitmap* bucket = (SparseBitmap*) bitmap->sbm_segments[slot];
			if (bucket) {
				if (SBM_next(bucket, &relative, mode)) {
					*number = ((SLONG) slot << BUCKET_BITS) + relative;
					return true;
				}
				else if (garbage_collect && mode == RSE_get_forward) {
					bucket_reset(bucket);
					bitmap->sbm_segments[slot] = 0;
					--bitmap->sbm_used;
					if (slot == bitmap->sbm_high_water) {
						for (; slot > 0; --slot) {
							if (bitmap->sbm_segments[slot])
								break;
						}
						bitmap->sbm_high_water = slot;
					}
				}
			}

			/* when one bucket is exhausted, try the next one in either direction */

			if (mode == RSE_get_forward)
				slot++;
			else if (mode == RSE_get_backward)
				slot--;
			else
				break;

			/* signify that we are at the beginning of the bucket */

			relative = -1;
			garbage_collect = true;
		}

		return false;
	}
	else {
		/* -1 signifies beginning of bucket in either direction, so
		   adjust the actual number as appropriate */

		if (*number == -1) {
			if (mode == RSE_get_forward)
				*number = 0;
			else if (mode == RSE_get_backward)
				*number =
					((SLONG) bitmap->sbm_high_water << SEGMENT_BITS) +
					BITS_SEGMENT - 1;
#ifdef PC_ENGINE
			else if (mode == RSE_get_current)
				return false;
#endif
		}

		/* find the slot within the bucket, and the relative offset from 
		   the beginning of the bucket */

		slot = *number >> SEGMENT_BITS;
		relative = *number & ((1 << SEGMENT_BITS) - 1);

		/* within the offset longword, find the exact bit */

		SSHORT bunch =
			(relative >> BUNCH_BITS) & ((1 << (SEGMENT_BITS - BUNCH_BITS)) -
										1);
		SSHORT bit = relative & ((1 << BUNCH_BITS) - 1);

		bool garbage_collect = (!bunch && !bit) != 0;

		/* go through all the longwords in the segment, and all the bits
		   in the longword, in either direction */

		if (mode == RSE_get_forward) {
			for (; slot <= (SLONG) bitmap->sbm_high_water;
				 slot++, bunch = 0, bit = 0) 
			{
				BitmapSegment* segment = bitmap->sbm_segments[slot];
				if (segment) {
					for (; bunch < BUNCH_SEGMENT; bunch++, bit = 0) {
						BUNCH test = segment->bms_bits[bunch];
						if (test) {
							for (; bit < BITS_BUNCH; bit++) {
								if (test & (1 << bit)) {
									*number = ((SLONG) slot << SEGMENT_BITS) +
										(bunch << BUNCH_BITS) + bit;
									return true;
								}
							}
						}
					}

					/* didn't find any bits in this bucket, so release it */

					if (garbage_collect) {
						JrdMemoryPool* pool = segment->bms_pool;
						segment->bms_next = pool->plb_segments;
						pool->plb_segments = segment;
						bitmap->sbm_segments[slot] = 0;
						--bitmap->sbm_used;
						if (slot == bitmap->sbm_high_water) {
							for (; slot > 0; --slot) {
								if (bitmap->sbm_segments[slot])
									break;
							}
							bitmap->sbm_high_water = slot;
						}
					}
				}
				garbage_collect = true;
			}
		}
		else if (mode == RSE_get_backward) {
			for (; slot >= 0;
				 slot--, bunch = BUNCH_SEGMENT - 1, bit = BITS_BUNCH - 1)
			{
				BitmapSegment* segment = bitmap->sbm_segments[slot];
				if (segment) {
					for (; bunch >= 0; bunch--, bit = BITS_BUNCH - 1) {
						BUNCH test = segment->bms_bits[bunch];
						if (test) {
							for (; bit >= 0; bit--) {
								if (test & (1 << bit)) {
									*number = ((SLONG) slot << SEGMENT_BITS) +
										(bunch << BUNCH_BITS) + bit;
									return true;
								}
							}
						}
					}
				}
			}
		}
#ifdef PC_ENGINE
		else if (mode == RSE_get_current) {
			if (slot < 0 || slot > bitmap->sbm_high_water)
				return false;
			if (!(segment = bitmap->sbm_segments[slot]))
				return false;
			if (bunch < 0 || bunch >= BUNCH_SEGMENT)
				return false;
			if (!(test = segment->bms_bits[bunch]))
				return false;
			if (bit < 0 || bit >= BITS_BUNCH)
				return false;
			if (test & (1 << bit)) {
				*number = ((SLONG) slot << SEGMENT_BITS) +
					(bunch << BUNCH_BITS) + bit;
				return true;
			}
		}
#endif

		return false;
	}
}


SparseBitmap** SBM_or(SparseBitmap** bitmap1, SparseBitmap** bitmap2)
{
/**************************************
 *
 *	S B M _ o r
 *
 **************************************
 *
 * Functional description
 *	Compute the bitwise OR (better known as union) of two sparse
 *	bitmaps.  Note: this may result in the destruction of either
 *	or both bitmaps.
 *
 **************************************/
	SparseBitmap* map1 = (bitmap1) ? *bitmap1 : NULL;
	SparseBitmap* map2 = (bitmap2) ? *bitmap2 : NULL;

/* If either bitmap is empty, return the other. */

	if ((!map1 || map1->sbm_state == SBM_EMPTY))
		return bitmap2;

	if ((!map2 || map2->sbm_state == SBM_EMPTY))
		return bitmap1;

/* Both bitmaps are known to exit.  More work.  If either is singular
   add the bit to the other and return. */

	if (map1->sbm_state == SBM_SINGULAR) {
		SBM_set(NULL, bitmap2, map1->sbm_number);
		return bitmap2;
	}

	if (map2->sbm_state == SBM_SINGULAR) {
		SBM_set(NULL, bitmap1, map2->sbm_number);
		return bitmap1;
	}

/* We need to loop thru the segments.  Sigh.  Make sure the first
   is the SLONGer of the two.  If not, switch them. */

	SparseBitmap** result = bitmap1;

	if (map1->sbm_high_water < map2->sbm_high_water) {
		map2 = map1;
		map1 = *bitmap2;
		result = bitmap2;
	}

	if (map1->sbm_type == SBM_ROOT) {
        //bucket1 = (SparseBitmap**) &*(map1->sbm_segments.begin());
		//bucket2 = (SparseBitmap**) &*(map2->sbm_segments.begin());
		SparseBitmap** bucket1 = (SparseBitmap**) &(map1->sbm_segments[0]);
		SparseBitmap** bucket2 = (SparseBitmap**) &(map2->sbm_segments[0]);
		const SparseBitmap* const* const end_buckets = bucket2 + map2->sbm_high_water + 1;

		for (; bucket2 < end_buckets; bucket1++, bucket2++) {
			if (!*bucket2)
				continue;
			if (!*bucket1) {
				*bucket1 = *bucket2;
				*bucket2 = NULL;
				continue;
			}
			SparseBitmap* const temp = *bucket1;
			*bucket1 = *(SBM_or(bucket1, bucket2));
			if (*bucket1 == *bucket2)
				*bucket2 = temp;
		}
	}
	else {
		/* Both bitmaps exist.  Form the bitwise union in the first bitmap */

		SparseBitmap::iterator segment1 = map1->sbm_segments.begin();
		SparseBitmap::iterator segment2 = map2->sbm_segments.begin();
		const SparseBitmap::const_iterator end_segments = segment2 + map2->sbm_high_water + 1;

		for (; segment2 < end_segments; segment1++, segment2++) {
			if (!*segment2)
				continue;
			if (!*segment1) {
				*segment1 = *segment2;
				*segment2 = NULL;
				continue;
			}
			BUNCH* b1 = (*segment1)->bms_bits;
			const BUNCH* b2 = (*segment2)->bms_bits;
			for (size_t j = 0; j < BUNCH_SEGMENT; j++) {
				*b1++ |= *b2++;
			}
		}
	}

	return result;
}


void SBM_release(SparseBitmap* bitmap)
{
/**************************************
 *
 *	S B M _ r e l e a s e
 *
 **************************************
 *
 * Functional description
 *	Release a (possibly null) bitmap.
 *
 **************************************/

	if (!bitmap)
		return;

	SBM_reset(&bitmap);
	delete bitmap;
}


void SBM_reset(SparseBitmap** bitmap)
{
/**************************************
 *
 *	S B M _ r e s e t
 *
 **************************************
 *
 * Functional description
 *	Clear a sparse bit map.  So save time, don't release the
 *	vector, just the segments.
 *
 **************************************/
	SparseBitmap* vector = *bitmap;
	if (!vector || vector->sbm_state == SBM_EMPTY)
		return;

	size_t i = 0;
	for (SparseBitmap::iterator tail = vector->sbm_segments.begin(); i < vector->sbm_count;
		i++, tail++) 
	{
		SparseBitmap* bucket = (SparseBitmap*) *tail;
		if (bucket) {
			bucket_reset(bucket);
			*tail = NULL;
		}
	}

	vector->sbm_state = SBM_EMPTY;
	vector->sbm_used = vector->sbm_high_water = 0;
}


void SBM_set(thread_db* tdbb, SparseBitmap** bitmap, SLONG number)
{
/**************************************
 *
 *	S B M _ s e t
 *
 **************************************
 *
 * Functional description
 *	Set a bit in a sparse bit map.
 *
 **************************************/
	SET_TDBB(tdbb);

	SparseBitmap* vector = *bitmap;
	if (!vector) {
		*bitmap = vector = FB_NEW(*tdbb->getDefaultPool()) SparseBitmap(*tdbb->getDefaultPool(), 5);
		vector->sbm_type = SBM_ROOT;
		vector->sbm_count = 5;
		vector->sbm_state = SBM_SINGULAR;
		vector->sbm_number = number;
		return;
	}

	if (vector->sbm_state == SBM_EMPTY) {
		vector->sbm_state = SBM_SINGULAR;
		vector->sbm_number = number;
		return;
	}

	if (vector->sbm_type == SBM_ROOT) {
		const USHORT slot = number >> BUCKET_BITS;
		const SLONG relative = number & ((1 << BUCKET_BITS) - 1);

		/* Make sure a vector is allocated and sufficiently large */

		if (vector->sbm_count <= slot) {
			ULONG end;
			if (SBM_max_tail <= slot)
				end = slot + 5;
			else {
				end = vector->sbm_count;
				do {
					end <<= 1;
				} while (end <= slot);
				if (end > SBM_max_tail)
					end = SBM_max_tail;
			}
			//vector = (SparseBitmap*) plb::ALL_extend((BLK *) bitmap, end);
			vector->sbm_segments.resize(end);
			vector->sbm_count = end;
		}

		/* Get bucket */

		SparseBitmap* bucket = (SparseBitmap*) vector->sbm_segments[slot];
		if (!bucket) {
			if ( (bucket = tdbb->getDefaultPool()->plb_buckets) )
				tdbb->getDefaultPool()->plb_buckets = bucket->sbm_next;
			else {
				bucket = FB_NEW(*tdbb->getDefaultPool())
						SparseBitmap(*tdbb->getDefaultPool(), BUNCH_BUCKET);
				bucket->sbm_pool = tdbb->getDefaultPool();
			}
			clear_bucket(bucket);
			vector->sbm_segments[slot] = (BitmapSegment*) bucket;
			vector->sbm_used++;
			if (vector->sbm_high_water < slot)
				vector->sbm_high_water = slot;
		}

		/* Set bit, etc. */

		SBM_set(tdbb, &bucket, relative);
	}
	else {
		const USHORT slot = number >> SEGMENT_BITS;
		const SLONG relative = number & ((1 << SEGMENT_BITS) - 1);
		const SSHORT bunch =
			(relative >> BUNCH_BITS) & ((1 << (SEGMENT_BITS - BUNCH_BITS)) -
										1);
		const SSHORT bit = relative & ((1 << BUNCH_BITS) - 1);

		/* Get segment */

		BitmapSegment* segment = vector->sbm_segments[slot];
		if (!segment) {
			if ( (segment = tdbb->getDefaultPool()->plb_segments) ) {
				tdbb->getDefaultPool()->plb_segments = segment->bms_next;
				clear_segment(segment);
			}
			else {
				segment = FB_NEW(*tdbb->getDefaultPool()) BitmapSegment();
				segment->bms_pool = tdbb->getDefaultPool();
			}
			vector->sbm_segments[slot] = segment;
			vector->sbm_used++;
			if (vector->sbm_high_water < slot)
				vector->sbm_high_water = slot;
		}

		/* Set bit, etc. */

		segment->bms_bits[bunch] |= 1 << bit;
		if (relative < segment->bms_min)
			segment->bms_min = relative;
		if (relative > segment->bms_max)
			segment->bms_max = relative;
	}

/* If the bitmap was singular, go back add the single bit */

	if (vector->sbm_state == SBM_PLURAL)
		return;

	vector->sbm_state = SBM_PLURAL;
	SBM_set(tdbb, bitmap, vector->sbm_number);
}


bool SBM_test(const SparseBitmap* bitmap, SLONG number)
{
/**************************************
 *
 *	S B M _ t e s t
 *
 **************************************
 *
 * Functional description
 *	Test whether or not a particular bit is set.
 *
 **************************************/

/* If the bitmap is completely missing, or is known to be empty,
   give up immediately.  */

	if (!bitmap || bitmap->sbm_state == SBM_EMPTY)
		return false;

/* If the bitmap is singular (represents a single value), return it
   if appropriate, else indicate bitmap exhausted. */

	if (bitmap->sbm_state == SBM_SINGULAR)
		return (number == bitmap->sbm_number);

	if (bitmap->sbm_type == SBM_ROOT) {
		const USHORT slot = number >> BUCKET_BITS;

// USHORT slot is never < 0
		SparseBitmap* bucket;
		if (slot > bitmap->sbm_high_water ||
			!(bucket = (SparseBitmap*) bitmap->sbm_segments[slot]))
		{
			return false;
		}

		const SLONG relative = number & ((1 << BUCKET_BITS) - 1);
		return SBM_test(bucket, relative);
	}
	else {
		const USHORT slot = number >> SEGMENT_BITS;

		BitmapSegment* segment;
// USHORT slot is never < 0
		if (slot > bitmap->sbm_high_water ||
			!(segment = bitmap->sbm_segments[slot]))
		{
			return false;
		}

		const SLONG relative = number & ((1 << SEGMENT_BITS) - 1);
		const SSHORT bunch =
			(relative >> BUNCH_BITS) & ((1 << (SEGMENT_BITS - BUNCH_BITS)) -
										1);

		BUNCH test = segment->bms_bits[bunch];
		if (!test)
			return false;

		const SSHORT bit = relative & ((1 << BUNCH_BITS) - 1);

		if (!(test & (1 << bit)))
			return false;

		return true;
	}
}


SLONG SBM_size(const SparseBitmap* const* const bitmap)
{
/**************************************
 *
 *	S B M _ s i z e  
 *
 **************************************
 *
 * Functional description
 *	Returns the number of "SparseBitmap" and "BitmapSegment" structs are allocated
 *	for this sparce bitmap.  (This is a measure of the memory
 *      used for this sparce bitmap)
 *
 **************************************/
	const SparseBitmap* vector = *bitmap;
	if (!vector)
		return 0;				/* not even the root SparseBitmap is allocated */

	if (vector->sbm_state == SBM_EMPTY)
		return 1;				/* only the the root SparseBitmap is allocated */

	SLONG count = 1;					/* one for the root SparseBitmap */
	size_t i = 0;
	for (SparseBitmap::const_iterator tail = vector->sbm_segments.begin();
		i < vector->sbm_count; i++, tail++)
	{
		const SparseBitmap* bucket = (const SparseBitmap*) *tail;
		if (bucket) {
			size_t j = 0;
			for (SparseBitmap::const_iterator node = bucket->sbm_segments.begin();
				 j < (USHORT) BUNCH_BUCKET; j++, node++)
			{
				const BitmapSegment* segment = *node;
				if (segment)
					count++;	/* one for the BitmapSegment */
			}
			count++;			/* one for the bucket SparseBitmap */
		}
	}

	return (count);

}


static void bucket_reset(SparseBitmap* bucket)
{
/**************************************
 *
 *	b u c k e t _ r e s e t
 *
 **************************************
 *
 * Functional description
 *	Reset a bucket and all its segments..
 *
 **************************************/
	if (!bucket)
		return;

	size_t i = 0;
	for (SparseBitmap::iterator node = bucket->sbm_segments.begin();
				i < BUNCH_BUCKET; i++, node++)
	{
		BitmapSegment* segment = *node;
		if (segment) {
			JrdMemoryPool* pool = segment->bms_pool;
			segment->bms_next = pool->plb_segments;
			pool->plb_segments = segment;
			*node = NULL;
		}
	}

	JrdMemoryPool* pool = bucket->sbm_pool;
	bucket->sbm_next = pool->plb_buckets;
	pool->plb_buckets = bucket;
}


static void clear_bucket(SparseBitmap* bucket)
{
/**************************************
 *
 *	c l e a r _ b u c k e t
 *
 **************************************
 *
 * Functional description
 *	Clear out a bit map bucket.
 *
 **************************************/
	bucket->sbm_next = NULL;
	bucket->sbm_type = SBM_BUCKET;
	bucket->sbm_state = SBM_EMPTY;
	bucket->sbm_count = BUNCH_BUCKET;
	bucket->sbm_used = 0;
	bucket->sbm_high_water = 0;
	bucket->sbm_number = 0;
	SparseBitmap::iterator p = bucket->sbm_segments.begin();
	size_t l = BUNCH_BUCKET;

	do {
		*p = 0;
		p++;
	} while (--l);
}


static void clear_segment(BitmapSegment* segment)
{
/**************************************
 *
 *	c l e a r _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *	Clear out a used bit map segment.
 *
 **************************************/
	segment->bms_min = segment->bms_max = 0;
	BUNCH* p = segment->bms_bits;
	size_t l = BUNCH_SEGMENT;

	do {
		*p = 0;
		p++;
	} while (--l);
}

