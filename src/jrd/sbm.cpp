/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sbm.c
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
#include "../jrd/jrd.h"
#include "../jrd/req.h"
#include "../jrd/sbm.h"
#include "../jrd/all.h"
#include "../jrd/rse.h"
#include "../jrd/all_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/sbm_proto.h"
#include "../jrd/thd_proto.h"

static void bucket_reset(SBM);
static void clear_bucket(SBM);
static void clear_segment(BMS);

/* Stores a constant determined during initialization */
static ULONG SBM_max_tail;


SBM *SBM_and(SBM * bitmap1, SBM * bitmap2)
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
	SBM map1, map2;
	SBM *result;

	map1 = (bitmap1) ? *bitmap1 : NULL;
	map2 = (bitmap2) ? *bitmap2 : NULL;

/* If either bitmap is null, so is the intersection.  */

	if (!map1 || !map2 ||
		map1->sbm_state == SBM_EMPTY || map2->sbm_state == SBM_EMPTY)
			return NULL;

	if (map1->sbm_state == SBM_SINGULAR)
		if (SBM_test(map2, map1->sbm_number))
			return bitmap1;
		else
			return NULL;

	if (map2->sbm_state == SBM_SINGULAR)
		if (SBM_test(map1, map2->sbm_number))
			return bitmap2;
		else
			return NULL;

/* Both bitmaps exist.  Make sure that the shorter of the two is
   bitmap1.  If not, flip them. */

	result = bitmap1;

	if (map1->sbm_high_water > map2->sbm_high_water) {
		map2 = *bitmap1;
		map1 = *bitmap2;
		result = bitmap2;
	}

	if (map1->sbm_type == SBM_ROOT) {
		sbm::iterator bucket1, bucket2, end_buckets;
		SBM *result_bucket, temp;

		bucket1 = map1->sbm_segments.begin();
		bucket2 = map2->sbm_segments.begin();
		end_buckets = bucket1 + map1->sbm_high_water + 1;

		for (; bucket1 < end_buckets; bucket1++, bucket2++)
			if (*bucket1) {
				if (!*bucket2) {
					*bucket2 = *bucket1;
					*bucket1 = NULL;
				}
				else if (!(result_bucket =
					SBM_and((SBM*) &*bucket1, (SBM*) &*bucket2))) {
					bucket_reset((SBM) *bucket1);
					*bucket1 = NULL;
				}
				else if (result_bucket == (SBM*) &*bucket2) {
					temp = (SBM)*bucket2;
					*bucket2 = *bucket1;
					*bucket1 = (BMS)temp;
				}
			}
	}
	else {
		sbm::iterator segment1, segment2, end_segments;
		JrdMemoryPool *pool;
		BUNCH *b1, *b2;
		SSHORT j;

		/* AND the bitmaps segment-wise.  If each bucket has a segment
		   is a given position, AND the segment bit-wise. */

		segment1 = map1->sbm_segments.begin();
		segment2 = map2->sbm_segments.begin();
		end_segments = segment1 + map1->sbm_high_water + 1;

		for (; segment1 < end_segments; segment1++, segment2++)
			if (*segment1) {
				if (!*segment2) {
					pool = ((BMS)(*segment1))->bms_pool;
					((BMS)(*segment1))->bms_next = pool->plb_segments;
					pool->plb_segments = *segment1;
					*segment1 = NULL;
					continue;
				}
				b1 = ((BMS)(*segment1))->bms_bits;
				b2 = ((BMS)(*segment2))->bms_bits;
				for (j = 0; j < BUNCH_SEGMENT; j++)
					*b1++ &= *b2++;
			}
	}

	return result;
}


int SBM_clear(SBM bitmap, SLONG number)
{
/**************************************
 *
 *	S B M _ c l e a r
 *
 **************************************
 *
 * Functional description
 *	Clear a bit in a sparse bitmap.
 *	Return TRUE if the bit was found set, else return FALSE.
 *
 **************************************/
	SLONG relative;
	USHORT slot;

/* If the bitmap is completely missing, or is known to be empty,
   give up immediately.  */

	if (!bitmap || bitmap->sbm_state == SBM_EMPTY)
		return FALSE;

/* If the bitmap is singular (represents a single value), return it
   if appropriate, else indicate bitmap exhausted. */

	if (bitmap->sbm_state == SBM_SINGULAR)
		if (number == bitmap->sbm_number) {
			bitmap->sbm_state = SBM_EMPTY;
			return TRUE;
		}
		else
			return FALSE;

	if (bitmap->sbm_type == SBM_ROOT) {
		SBM bucket;

		slot = number >> BUCKET_BITS;

// USHORT slot is never < 0
		if (slot > bitmap->sbm_high_water ||
			!(bucket = (SBM) bitmap->sbm_segments[slot]))
			return FALSE;

		relative = number & ((1 << BUCKET_BITS) - 1);
		return SBM_clear(bucket, relative);
	}
	else {
		BMS segment;
		BUNCH test;
		SSHORT bit, bunch;

		slot = number >> SEGMENT_BITS;

// USHORT slot is never < 0
		if (slot > bitmap->sbm_high_water ||
			!(segment = bitmap->sbm_segments[slot]))
			return FALSE;

		relative = number & ((1 << SEGMENT_BITS) - 1);
		bunch =
			(relative >> BUNCH_BITS) & ((1 << (SEGMENT_BITS - BUNCH_BITS)) -
										1);

		if (!(test = segment->bms_bits[bunch]))
			return FALSE;

		bit = relative & ((1 << BUNCH_BITS) - 1);

		if (!(test & (1 << bit)))
			return FALSE;

		test &= ~(1 << bit);

		/* store value back in bit map */

		segment->bms_bits[bunch] = test;
		return TRUE;
	}
}


#ifdef DEV_BUILD
void SBM_dump(IB_FILE * f, SBM bitmap1)
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
	BOOLEAN in_range = FALSE;
	USHORT counter = 0;
	bit1 = -1;
	last_bit = bit1 - 1;

	ib_fprintf(f, "State: %s", (bitmap1->sbm_state == SBM_EMPTY) ? "EMPTY" :
			   (bitmap1->sbm_state == SBM_SINGULAR) ? "Singular" :
			   (bitmap1->sbm_state == SBM_PLURAL) ? "Plural" : "Bogus");
	ib_fprintf(f, " Type: %s", (bitmap1->sbm_type == SBM_BUCKET) ? "Bucket" :
			   (bitmap1->sbm_type == SBM_ROOT) ? "root" : "Bogus");
	ib_fprintf(f, " Count %d Used %d high %d\n",
			   bitmap1->sbm_count, bitmap1->sbm_used,
			   bitmap1->sbm_high_water);

	while (TRUE) {
		if (!SBM_next(bitmap1, &bit1, RSE_get_forward))
			break;
		if (bit1 == last_bit + 1) {
			last_bit = bit1;
			in_range = TRUE;
			continue;
		}
		if (in_range)
			ib_fprintf(f, "-%"SLONGFORMAT, last_bit);
		if ((++counter % 8) == 0)
			ib_fprintf(f, "\n");
		ib_fprintf(f, " %"SLONGFORMAT, bit1);
		last_bit = bit1;
		in_range = FALSE;
	}

	if (in_range)
		ib_fprintf(f, "-%"SLONGFORMAT, last_bit);
	ib_fprintf(f, "\n");
}
#endif


BOOLEAN SBM_equal(SBM bitmap1, SBM bitmap2)
{
/**************************************
 *
 *	S B M _ e q u a l
 *
 **************************************
 *
 * Functional description
 *	Return TRUE if two sparse bitmaps are equal (eg: have the
 *	exact same set of bits set).
 *	The test is non-destructive of both bitmaps.
 *
 *	This implementation is not optimal - as it just probes
 *	each bit individually rather than using the inards
 *	for a quicker probe.
 *
 **************************************/
	SLONG bit1, bit2;

/* Check the highest bit in the bitmap - just as
   a quick optimization */
	bit1 = bit2 = -1;
	if ((SBM_next(bitmap1, &bit1, RSE_get_backward) !=
		 SBM_next(bitmap2, &bit2, RSE_get_backward))
		|| (bit1 != bit2))
		return FALSE;

/* Now check each bit */
	bit1 = bit2 = -1;
	while (TRUE) {
		int res1, res2;

		res1 = SBM_next(bitmap1, &bit1, RSE_get_forward);
		res2 = SBM_next(bitmap2, &bit2, RSE_get_forward);

		/* Reached end of one bitmap, but not the other? */
		if (res1 != res2)
			return FALSE;

		/* Have we reached the end of both bitmaps? */
		if (!res1)
			return TRUE;

		/* Is the next-bit-set the same for each? */
		if (bit1 != bit2)
			return FALSE;
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
	 * The bottom line is SBM_max_tail is not a max, it is just a threashold
	 * the allocation scheme changes around.  The FB1 value is kept for
	 * a lack of a better value.
	 */
	SBM_max_tail = ((MAX_USHORT - 32 - sizeof(sbm)) / sizeof(class bms*)) + 1;
}


int SBM_next(SBM bitmap, SLONG * number, RSE_GET_MODE mode)
{
/**************************************
 *
 *	S B M _ n e x t
 *
 **************************************
 *
 * Functional description
 *	Find the next bit set in a bitmap after a given position.
 *	Return TRUE if a fit is found, else return FALSE.
 *	Handle forwards or backwards traversal of the bitmap,
 *	or just return the currently set bit.
 *
 **************************************/
	SLONG relative, slot;
	BOOLEAN garbage_collect;

/* If the bitmap is completely missing, or is known to be empty,
   give up immediately.  */

	if (!bitmap || bitmap->sbm_state == SBM_EMPTY)
		return FALSE;

/* If the bitmap is singular (represents a single value), return it
   if appropriate, else indicate bitmap exhausted. */

	if (bitmap->sbm_state == SBM_SINGULAR) {
		if (mode == RSE_get_forward && *number >= bitmap->sbm_number)
			return FALSE;
		if (mode == RSE_get_backward &&
			*number <= bitmap->sbm_number && *number != -1)
			return FALSE;
#ifdef PC_ENGINE
		if (mode == RSE_get_current && *number != bitmap->sbm_number)
			return FALSE;
#endif

		*number = bitmap->sbm_number;
		return TRUE;
	}

/* Advance the number and search for the next bit set */

	if (bitmap->sbm_type == SBM_ROOT) {
		SBM bucket;

		if (mode == RSE_get_forward)
			(*number)++;
		else if (mode == RSE_get_backward)
			if (!*number)
				return FALSE;
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

		garbage_collect = (relative == -1) ? TRUE : FALSE;

		for (;;) {
			if (slot < 0 || slot > (SLONG) bitmap->sbm_high_water)
				break;

			/* recursively find the next bit set within this bucket */

			if ( (bucket = (SBM) bitmap->sbm_segments[slot]) ) {
				if (SBM_next(bucket, &relative, mode)) {
					*number = ((SLONG) slot << BUCKET_BITS) + relative;
					return TRUE;
				}
				else if (garbage_collect && mode == RSE_get_forward) {
					bucket_reset(bucket);
					bitmap->sbm_segments[slot] = 0;
					--bitmap->sbm_used;
					if (slot == bitmap->sbm_high_water) {
						for (; slot > 0; --slot)
							if (bitmap->sbm_segments[slot])
								break;
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
			garbage_collect = TRUE;
		}

		return FALSE;
	}
	else {
		BMS segment;
		BUNCH test;
		SSHORT bit, bunch;
		JrdMemoryPool *pool;

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
				return FALSE;
#endif
		}

		/* find the slot within the bucket, and the relative offset from 
		   the beginning of the bucket */

		slot = *number >> SEGMENT_BITS;
		relative = *number & ((1 << SEGMENT_BITS) - 1);

		/* within the offset longword, find the exact bit */

		bunch =
			(relative >> BUNCH_BITS) & ((1 << (SEGMENT_BITS - BUNCH_BITS)) -
										1);
		bit = relative & ((1 << BUNCH_BITS) - 1);

		garbage_collect = (!bunch && !bit) ? TRUE : FALSE;

		/* go through all the longwords in the segment, and all the bits
		   in the longword, in either direction */

		if (mode == RSE_get_forward) {
			for (; slot <= (SLONG) bitmap->sbm_high_water;
				 slot++, bunch = 0, bit = 0) {
				if ( (segment = bitmap->sbm_segments[slot]) ) {
					for (; bunch < BUNCH_SEGMENT; bunch++, bit = 0)
						if ( (test = segment->bms_bits[bunch]) )
							for (; bit < BITS_BUNCH; bit++)
								if (test & (1 << bit)) {
									*number = ((SLONG) slot << SEGMENT_BITS) +
										(bunch << BUNCH_BITS) + bit;
									return TRUE;
								}

					/* didn't find any bits in this bucket, so release it */

					if (garbage_collect) {
						pool = segment->bms_pool;
						segment->bms_next = pool->plb_segments;
						pool->plb_segments = segment;
						bitmap->sbm_segments[slot] = 0;
						--bitmap->sbm_used;
						if (slot == bitmap->sbm_high_water) {
							for (; slot > 0; --slot)
								if (bitmap->sbm_segments[slot])
									break;
							bitmap->sbm_high_water = slot;
						}
					}
				}
				garbage_collect = TRUE;
			}
		}
		else if (mode == RSE_get_backward) {
			for (; slot >= 0;
				 slot--, bunch = BUNCH_SEGMENT - 1, bit = BITS_BUNCH - 1)
				if ( (segment = bitmap->sbm_segments[slot]) )
					for (; bunch >= 0; bunch--, bit = BITS_BUNCH - 1)
						if ( (test = segment->bms_bits[bunch]) )
							for (; bit >= 0; bit--)
								if (test & (1 << bit)) {
									*number = ((SLONG) slot << SEGMENT_BITS) +
										(bunch << BUNCH_BITS) + bit;
									return TRUE;
								}
		}
#ifdef PC_ENGINE
		else if (mode == RSE_get_current) {
			if (slot < 0 || slot > bitmap->sbm_high_water)
				return FALSE;
			if (!(segment = bitmap->sbm_segments[slot]))
				return FALSE;
			if (bunch < 0 || bunch >= BUNCH_SEGMENT)
				return FALSE;
			if (!(test = segment->bms_bits[bunch]))
				return FALSE;
			if (bit < 0 || bit >= BITS_BUNCH)
				return FALSE;
			if (test & (1 << bit)) {
				*number = ((SLONG) slot << SEGMENT_BITS) +
					(bunch << BUNCH_BITS) + bit;
				return TRUE;
			}
		}
#endif

		return FALSE;
	}
}


SBM *SBM_or(SBM * bitmap1, SBM * bitmap2)
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
	SBM map1, map2;
	SBM *result;

	map1 = (bitmap1) ? *bitmap1 : NULL;
	map2 = (bitmap2) ? *bitmap2 : NULL;

/* If either bitmap is empty, return the other. */

	if ((!map1 || map1->sbm_state == SBM_EMPTY))
		return bitmap2;

	if ((!map2 || map2->sbm_state == SBM_EMPTY))
		return bitmap1;

/* Both bitmaps are known to exit.  More work.  If either is singular
   add the bit to the other and return. */

	if (map1->sbm_state == SBM_SINGULAR) {
		SBM_set(NULL_TDBB, bitmap2, map1->sbm_number);
		return bitmap2;
	}

	if (map2->sbm_state == SBM_SINGULAR) {
		SBM_set(NULL_TDBB, bitmap1, map2->sbm_number);
		return bitmap1;
	}

/* We need to loop thru the segments.  Sigh.  Make sure the first
   is the SLONGer of the two.  If not, switch them. */

	result = bitmap1;

	if (map1->sbm_high_water < map2->sbm_high_water) {
		map2 = map1;
		map1 = *bitmap2;
		result = bitmap2;
	}

	if (map1->sbm_type == SBM_ROOT) {
		SBM *bucket1, *bucket2, *end_buckets;
		SBM temp;

        //bucket1 = (SBM *) &*(map1->sbm_segments.begin());
		//bucket2 = (SBM *) &*(map2->sbm_segments.begin());
		bucket1 = (SBM *) &(map1->sbm_segments[0]);
		bucket2 = (SBM *) &(map2->sbm_segments[0]);
		end_buckets = bucket2 + map2->sbm_high_water + 1;

		for (; bucket2 < end_buckets; bucket1++, bucket2++) {
			if (!*bucket2)
				continue;
			if (!*bucket1) {
				*bucket1 = *bucket2;
				*bucket2 = NULL;
				continue;
			}
			temp = *bucket1;
			*bucket1 = *(SBM_or(bucket1, bucket2));
			if (*bucket1 == *bucket2)
				*bucket2 = temp;
		}
	}
	else {
		sbm::iterator segment1, segment2, end_segments;
		BUNCH *b1, *b2;
		USHORT j;

		/* Both bitmaps exist.  Form the bitwise union in the first bitmap */

		segment1 = map1->sbm_segments.begin();
		segment2 = map2->sbm_segments.begin();
		end_segments = segment2 + map2->sbm_high_water + 1;

		for (; segment2 < end_segments; segment1++, segment2++) {
			if (!*segment2)
				continue;
			if (!*segment1) {
				*segment1 = *segment2;
				*segment2 = NULL;
				continue;
			}
			b1 = ((BMS)(*segment1))->bms_bits;
			b2 = ((BMS)(*segment2))->bms_bits;
			for (j = 0; j < BUNCH_SEGMENT; j++)
				*b1++ |= *b2++;
		}
	}

	return result;
}


void SBM_release(SBM bitmap)
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


void SBM_reset(SBM * bitmap)
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
	sbm::iterator tail;
	SBM vector, bucket;
	USHORT i;

	if (!(vector = *bitmap) || vector->sbm_state == SBM_EMPTY)
		return;

	for (i = 0, tail = vector->sbm_segments.begin(); i < vector->sbm_count;
		 i++, tail++)
		if ( (bucket = (SBM)*tail) ) {
			bucket_reset(bucket);
			*tail = NULL;
		}

	vector->sbm_state = SBM_EMPTY;
	vector->sbm_used = vector->sbm_high_water = 0;
}


void SBM_set(TDBB tdbb, SBM * bitmap, SLONG number)
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
	SBM vector;
	SLONG relative;
	USHORT slot;

	SET_TDBB(tdbb);

	if (!(vector = *bitmap)) {
		*bitmap = vector = FB_NEW(*tdbb->tdbb_default) sbm(*tdbb->tdbb_default, 5);
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
		SBM bucket;
		ULONG end;

		slot = number >> BUCKET_BITS;
		relative = number & ((1 << BUCKET_BITS) - 1);

		/* Make sure a vector is allocated and sufficiently large */

		if (vector->sbm_count <= slot) {
			if (SBM_max_tail <= slot)
				end = slot + 5;
			else {
				end = vector->sbm_count;
				do
					end <<= 1;
				while (end <= slot);
				if (end > SBM_max_tail)
					end = SBM_max_tail;
			}
			//vector = (SBM) plb::ALL_extend((BLK *) bitmap, end);
			vector->sbm_segments.resize(end);
			vector->sbm_count = end;
		}

		/* Get bucket */

		if (!(bucket = (SBM) vector->sbm_segments[slot])) {
			if ( (bucket = tdbb->tdbb_default->plb_buckets) )
				tdbb->tdbb_default->plb_buckets = bucket->sbm_next;
			else {
				bucket = FB_NEW(*tdbb->tdbb_default)
						sbm(*tdbb->tdbb_default, BUNCH_BUCKET);
				bucket->sbm_pool = tdbb->tdbb_default;
			}
			clear_bucket(bucket);
			vector->sbm_segments[slot] = (BMS) bucket;
			vector->sbm_used++;
			if (vector->sbm_high_water < slot)
				vector->sbm_high_water = slot;
		}

		/* Set bit, etc. */

		SBM_set(tdbb, &bucket, relative);
	}
	else {
		BMS segment;
		SSHORT bit, bunch;

		slot = number >> SEGMENT_BITS;
		relative = number & ((1 << SEGMENT_BITS) - 1);
		bunch =
			(relative >> BUNCH_BITS) & ((1 << (SEGMENT_BITS - BUNCH_BITS)) -
										1);
		bit = relative & ((1 << BUNCH_BITS) - 1);

		/* Get segment */

		if (!(segment = vector->sbm_segments[slot])) {
			if ( (segment = tdbb->tdbb_default->plb_segments) ) {
				tdbb->tdbb_default->plb_segments = segment->bms_next;
				clear_segment(segment);
			}
			else {
				segment = FB_NEW(*tdbb->tdbb_default) bms();
				segment->bms_pool = tdbb->tdbb_default;
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


int SBM_test(SBM bitmap, SLONG number)
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
	SLONG relative;
	USHORT slot;

/* If the bitmap is completely missing, or is known to be empty,
   give up immediately.  */

	if (!bitmap || bitmap->sbm_state == SBM_EMPTY)
		return FALSE;

/* If the bitmap is singular (represents a single value), return it
   if appropriate, else indicate bitmap exhausted. */

	if (bitmap->sbm_state == SBM_SINGULAR)
		return (number == bitmap->sbm_number);

	if (bitmap->sbm_type == SBM_ROOT) {
		SBM bucket;

		slot = number >> BUCKET_BITS;

// USHORT slot is never < 0
		if (slot > bitmap->sbm_high_water ||
			!(bucket = (SBM) bitmap->sbm_segments[slot]))
			return FALSE;

		relative = number & ((1 << BUCKET_BITS) - 1);
		return SBM_test(bucket, relative);
	}
	else {
		BMS segment;
		BUNCH test;
		SSHORT bit, bunch;

		slot = number >> SEGMENT_BITS;

// USHORT slot is never < 0
		if (slot > bitmap->sbm_high_water ||
			!(segment = bitmap->sbm_segments[slot]))
			return FALSE;

		relative = number & ((1 << SEGMENT_BITS) - 1);
		bunch =
			(relative >> BUNCH_BITS) & ((1 << (SEGMENT_BITS - BUNCH_BITS)) -
										1);

		if (!(test = segment->bms_bits[bunch]))
			return FALSE;

		bit = relative & ((1 << BUNCH_BITS) - 1);

		if (!(test & (1 << bit)))
			return FALSE;

		return TRUE;
	}
}


SLONG SBM_size(SBM * bitmap)
{
/**************************************
 *
 *	S B M _ s i z e  
 *
 **************************************
 *
 * Functional description
 *	Returns the number of "sbm" and "bms" structs are allocated
 *	for this sparce bitmap.  (This is a measure of the memory
 *      used for this sparce bitmap)
 *
 **************************************/
	sbm::iterator tail;
	SBM vector, bucket;
	sbm::iterator node;
	BMS segment;
	USHORT i, j;
	SLONG count;

	vector = *bitmap;
	if (!vector)
		return 0;				/* not even the root sbm is allocated */

	if (vector->sbm_state == SBM_EMPTY)
		return 1;				/* only the the root sbm is allocated */

	count = 1;					/* one for the root sbm */
	for (i = 0, tail = vector->sbm_segments.begin(); i < vector->sbm_count;
		 i++, tail++) {
		bucket = (SBM)*tail;
		if (bucket) {
			for (j = 0, node = bucket->sbm_segments.begin();
				 j < (USHORT) BUNCH_BUCKET; j++, node++) {
				segment = *node;
				if (segment)
					count++;	/* one for the bms */
			}
			count++;			/* one for the bucket sbm */
		}
	}

	return (count);

}


static void bucket_reset(SBM bucket)
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
	sbm::iterator node;
	BMS segment;
	SSHORT i;
	JrdMemoryPool *pool;

	if (!bucket)
		return;

	for (i = 0, node = bucket->sbm_segments.begin();
				i < BUNCH_BUCKET; i++, node++)
		if ( (segment = *node) ) {
			pool = segment->bms_pool;
			segment->bms_next = pool->plb_segments;
			pool->plb_segments = segment;
			*node = NULL;
		}

	pool = bucket->sbm_pool;
	bucket->sbm_next = pool->plb_buckets;
	pool->plb_buckets = bucket;
}


static void clear_bucket(SBM bucket)
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
	sbm::iterator p;
	SSHORT l;

	bucket->sbm_next = NULL;
	bucket->sbm_type = SBM_BUCKET;
	bucket->sbm_state = SBM_EMPTY;
	bucket->sbm_count = BUNCH_BUCKET;
	bucket->sbm_used = 0;
	bucket->sbm_high_water = 0;
	bucket->sbm_number = 0;
	p = bucket->sbm_segments.begin();
	l = BUNCH_BUCKET;

	do {
		*p = 0;
		p++;
	} while (--l);
}


static void clear_segment(BMS segment)
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
	BUNCH *p;
	SSHORT l;

	segment->bms_min = segment->bms_max = 0;
	p = segment->bms_bits;
	l = BUNCH_SEGMENT;

	do {
		*p = 0;
		p++;
	} while (--l);
}
