/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		btr.c
 *	DESCRIPTION:	B-tree management code
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
 * 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
 *
 */
/*
$Id: btr.cpp,v 1.33.2.4 2005-07-26 13:06:30 arnobrinkman Exp $
*/

#include "firebird.h"
#include <string.h>
#include <stdlib.h>
#include "../jrd/ib_stdio.h"
#include "../jrd/jrd.h"
#include "../jrd/ods.h"
#include "../jrd/val.h"
#include "../jrd/btr.h"
#include "../jrd/req.h"
#include "../jrd/tra.h"
#include "../jrd/intl.h"
#include "gen/codes.h"
#include "../jrd/common.h"
#include "../jrd/jrn.h"
#include "../jrd/lck.h"
#include "../jrd/cch.h"
#include "../jrd/sbm.h"
#include "../jrd/sort.h"
#include "../jrd/gdsassert.h"
#include "../jrd/all_proto.h"
#include "../jrd/btr_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/dpm_proto.h"
#include "../jrd/dbg_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/jrd_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/nav_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/pcmet_proto.h"
#include "../jrd/sbm_proto.h"
#include "../jrd/sort_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/tra_proto.h"

extern "C" {


/*********************************************
      eliminate this conversion - kk
#ifdef VMS
extern double	MTH$CVT_G_D();
#endif
**********************************************/

#define MAX_LEVELS	16

inline void MOVE_BYTE(UCHAR*& x_from, UCHAR*& x_to)
{
	*x_to++ = *x_from++;
}

#define OVERSIZE	(MAX_PAGE_SIZE + BTN_SIZE + MAX_KEY + sizeof (SLONG) - 1) / sizeof (SLONG)

typedef union {
	SLONG n;
	SCHAR c[4];
} LONGCHAR;

#define GARBAGE_COLLECTION_THRESHOLD	(dbb->dbb_page_size / 4)


typedef struct {
	double d_part;
	SSHORT s_part;
} INT64_KEY;

#define INT64_KEY_LENGTH	(sizeof (double) + sizeof (SSHORT))

static const double pow10[] =
	{ 1.e00, 1.e01, 1.e02, 1.e03, 1.e04, 1.e05, 1.e06, 1.e07, 1.e08, 1.e09,
	1.e10, 1.e11, 1.e12, 1.e13, 1.e14, 1.e15, 1.e16, 1.e17, 1.e18, 1.e19,
	1.e20, 1.e21, 1.e22, 1.e23, 1.e24, 1.e25, 1.e26, 1.e27, 1.e28, 1.e29,
	1.e30, 1.e31, 1.e32, 1.e33, 1.e34, 1.e35, 1.e36
};

#define powerof10(s) ((s) <= 0 ? pow10[-(s)] : 1./pow10[-(s)])

static const struct {			/* Used in make_int64_key() */
	UINT64 limit;
	SINT64 factor;
	SSHORT scale_change;
} int64_scale_control[] = {
	{
	QUADCONST(922337203685470000), QUADCONST(1), 0}, {
	QUADCONST(92233720368547000), QUADCONST(10), 1}, {
	QUADCONST(9223372036854700), QUADCONST(100), 2}, {
	QUADCONST(922337203685470), QUADCONST(1000), 3}, {
	QUADCONST(92233720368548), QUADCONST(10000), 4}, {
	QUADCONST(9223372036855), QUADCONST(100000), 5}, {
	QUADCONST(922337203686), QUADCONST(1000000), 6}, {
	QUADCONST(92233720369), QUADCONST(10000000), 7}, {
	QUADCONST(9223372035), QUADCONST(100000000), 8}, {
	QUADCONST(922337204), QUADCONST(1000000000), 9}, {
	QUADCONST(92233721), QUADCONST(10000000000), 10}, {
	QUADCONST(9223373), QUADCONST(100000000000), 11}, {
	QUADCONST(922338), QUADCONST(1000000000000), 12}, {
	QUADCONST(92234), QUADCONST(10000000000000), 13}, {
	QUADCONST(9224), QUADCONST(100000000000000), 14}, {
	QUADCONST(923), QUADCONST(1000000000000000), 15}, {
	QUADCONST(93), QUADCONST(10000000000000000), 16}, {
	QUADCONST(10), QUADCONST(100000000000000000), 17}, {
	QUADCONST(1), QUADCONST(1000000000000000000), 18}, {
QUADCONST(0), QUADCONST(0), 0}};

/* The first four entries in the array int64_scale_control[] ends with the
 * limit having 0's in the end. This is to inhibit any rounding off that
 * DOUBLE precision can introduce. DOUBLE can easily store upto 92233720368547
 * uniquely. Values after this tend to round off to the upper limit during
 * division. Hence the ending with 0's so that values will be bunched together
 * in the same limit range and scale control for INT64 index KEY calculation.
 * 
 * This part was changed as a fix for bug 10267. - bsriram 04-Mar-1999 
 */

/* enumerate the possible outcomes of deleting a node */

typedef enum contents {
	contents_empty = 0,
	contents_single,
	contents_below_threshold,
	contents_above_threshold
} CONTENTS;

static SLONG add_node(TDBB, WIN *, IIB *, KEY *, SLONG *, SLONG *);
static void compress(TDBB, DSC *, KEY *, USHORT, USHORT, USHORT, USHORT);
static USHORT compress_root(TDBB, IRT);
static USHORT compute_prefix(KEY *, UCHAR *, USHORT);
static void copy_key(KEY *, KEY *);
static CONTENTS delete_node(TDBB, WIN *, BTN);
static void delete_tree(TDBB, USHORT, USHORT, SLONG, SLONG);
static DSC *eval(TDBB, JRD_NOD, DSC *, int *);
static SLONG fast_load(TDBB, JRD_REL, IDX *, USHORT, SCB, float *);
static IRT fetch_root(TDBB, WIN *, JRD_REL);
static BTN find_node(BTR, KEY *, USHORT);
static CONTENTS garbage_collect(TDBB, WIN *, SLONG);
static SLONG insert_node(TDBB, WIN *, IIB *, KEY *, SLONG *, SLONG *);
static void journal_btree_segment(TDBB, WIN *, BTR);
static BOOLEAN key_equality(KEY *, BTN);
static INT64_KEY make_int64_key(SINT64, SSHORT);
#ifdef DEBUG_INDEXKEY
static void print_int64_key(SINT64, SSHORT, INT64_KEY);
#endif
static void quad_put(SLONG, SCHAR *);
static void quad_move(UCHAR*, UCHAR*);
static CONTENTS remove_node(TDBB, IIB *, WIN *);
static CONTENTS remove_leaf_node(TDBB, IIB *, WIN *);
static BOOLEAN scan(TDBB, BTN, SBM *, UCHAR, KEY *, USHORT);


}                             // extern "C"


//
// TMN: Ease C -> C++ conversion. These functions must be outside the
// extern "C" block since it uses function overloading.
//
inline SLONG BTR_get_quad(const UCHAR* p)
{
#if defined(i386) || defined(I386) || defined(_M_IX86) || defined(AMD64)
	// For IA32 (little-endian) this optimization is a _very_ large speed-up!
	return *reinterpret_cast<const SLONG*>(p);
#else
	// Non-IA32

	// Copied from BTR_get_quad(const SCHAR*);
	LONGCHAR value;

	value.c[0] = p[0];
	value.c[1] = p[1];
	value.c[2] = p[2];
	value.c[3] = p[3];

	return value.n;

#endif	// endianness
}

inline void quad_put(SLONG value, UCHAR * data)
{
	quad_put(value, reinterpret_cast<SCHAR*>(data));
}


extern "C" {

USHORT BTR_all(TDBB    tdbb,
			   JRD_REL relation,
			   IDX**   start_buffer,
			   IDX**   csb_idx,
			   STR*    csb_idx_allocation,
			   SLONG*  idx_size)
{
/**************************************
 *
 *	B T R _ a l l
 *
 **************************************
 *
 * Functional description
 *	Return descriptions of all indices for relation.  If there isn't
 *	a known index root, assume we were called during optimization
 *	and return no indices.
 *
 **************************************/
	DBB dbb;
	WIN window;
	IRT root;
	STR new_buffer;
	USHORT count, i;
	IDX *buffer;
	SLONG size;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);
	window.win_flags = 0;

	buffer = *start_buffer;
	if (!(root = fetch_root(tdbb, &window, relation)))
		return 0;

	if ((SLONG) (root->irt_count * sizeof(IDX)) > *idx_size) {
		size = (sizeof(IDX) * dbb->dbb_max_idx) + ALIGNMENT;
		*csb_idx_allocation = new_buffer = FB_NEW_RPT(*dbb->dbb_permanent, size) str();
		buffer = *start_buffer =
			(IDX *) FB_ALIGN((U_IPTR) new_buffer->str_data, ALIGNMENT);
		*idx_size = size - ALIGNMENT;
	}
	count = 0;
	for (i = 0; i < root->irt_count; i++)
		if (BTR_description(relation, root, buffer, i)) {
			count++;
			buffer = NEXT_IDX(buffer->idx_rpt, buffer->idx_count);
		}
	*csb_idx = *start_buffer;
	*idx_size = *idx_size - ((UCHAR *) buffer - (UCHAR *) * start_buffer);
	*start_buffer = buffer;

	CCH_RELEASE(tdbb, &window);
	return count;
}


void BTR_create(TDBB tdbb,
				JRD_REL relation,
				IDX * idx,
				USHORT key_length, SCB sort_handle, float *selectivity)
{
/**************************************
 *
 *	B T R _ c r e a t e
 *
 **************************************
 *
 * Functional description
 *	Create a new index.
 *
 **************************************/
	DBB dbb;
	WIN window;
	IRT root;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* Now that the index id has been checked out, create the index. */

	idx->idx_root =
		fast_load(tdbb, relation, idx, key_length, sort_handle, selectivity);

/* Index is created.  Go back to the index root page and update it to
   point to the index. */

	window.win_page = relation->rel_index_root;
	window.win_flags = 0;
	root = (IRT) CCH_FETCH(tdbb, &window, LCK_write, pag_root);
	CCH_MARK(tdbb, &window);
	root->irt_rpt[idx->idx_id].irt_root = idx->idx_root;
	root->irt_rpt[idx->idx_id].irt_stuff.irt_selectivity = *selectivity;
	root->irt_rpt[idx->idx_id].irt_flags &= ~irt_in_progress;

	if (dbb->dbb_wal)
		CCH_journal_page(tdbb, &window);

	CCH_RELEASE(tdbb, &window);
}


void BTR_delete_index(TDBB tdbb, WIN * window, USHORT id)
{
/**************************************
 *
 *	B T R _ d e l e t e _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Delete an index if it exists.
 *
 **************************************/
	DBB dbb;
	IRT root;
	USHORT relation_id;
	SLONG prior, next;
	irt::irt_repeat * irt_desc;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* Get index descriptor.  If index doesn't exist, just leave.  */

	root = (IRT) window->win_buffer;

	if (id >= root->irt_count)
		CCH_RELEASE(tdbb, window);
	else {
		irt_desc = root->irt_rpt + id;
		CCH_MARK(tdbb, window);
		next = irt_desc->irt_root;

		/* remove the pointer to the top-level index page before we delete it */

		irt_desc->irt_root = 0;
		irt_desc->irt_flags = 0;
		prior = window->win_page;
		relation_id = root->irt_relation;

		/* Journal update of index root page */

		if (dbb->dbb_wal)
			CCH_journal_page(tdbb, window);

		CCH_RELEASE(tdbb, window);
		delete_tree(tdbb, relation_id, id, next, prior);
	}
}


BOOLEAN BTR_description(JRD_REL relation,
						IRT root, IDX * idx, SSHORT id)
{
/**************************************
 *
 *	B T R _ d e s c r i p t i o n
 *
 **************************************
 *
 * Functional description
 *	See if index exists, and if so, pick up its description.
 *  Index id's must fit in a short - formerly a UCHAR.
 *
 **************************************/
	irt::irt_repeat * irt_desc;
	idx::idx_repeat * idx_desc;
	struct irtd *irtd;
	USHORT i;

	if (id >= root->irt_count)
		return FALSE;

	irt_desc = &root->irt_rpt[id];

	if (irt_desc->irt_root == 0)
		return FALSE;

	//assert(id <= MAX_USHORT);
	idx->idx_id = (USHORT)id;
	idx->idx_root = irt_desc->irt_root;
	idx->idx_selectivity = irt_desc->irt_stuff.irt_selectivity;
	idx->idx_count = irt_desc->irt_keys;
	idx->idx_flags = irt_desc->irt_flags;
	idx->idx_runtime_flags = 0;
	idx->idx_foreign_primaries = NULL;
	idx->idx_foreign_relations = NULL;
	idx->idx_foreign_indexes = NULL;
	idx->idx_primary_relation = 0;
	idx->idx_primary_index = 0;
	idx->idx_expression = NULL;
	idx->idx_expression_request = NULL;

/* pick up field ids and type descriptions for each of the fields */

	irtd = (IRTD *) ((UCHAR *) root + irt_desc->irt_desc);
	idx_desc = idx->idx_rpt;
	for (i = 0; i < idx->idx_count; i++, irtd++, idx_desc++) {
		idx_desc->idx_field = irtd->irtd_field;
		idx_desc->idx_itype = irtd->irtd_itype;
	}
#ifdef EXPRESSION_INDICES
	if (idx->idx_flags & idx_expressn)
		PCMET_lookup_index(relation, idx);
#endif

	return TRUE;
}


void BTR_evaluate(TDBB tdbb, IRB retrieval, SBM * bitmap)
{
/**************************************
 *
 *	B T R _ e v a l u a t e
 *
 **************************************
 *
 * Functional description
 *	Do an index scan and return a bitmap 
 * 	of all candidate record numbers.
 *
 **************************************/
	KEY lower, upper;
	WIN window;
	BTR page;
	UCHAR prefix;
	BTN node;
	IDX idx;
	SLONG number;

	SET_TDBB(tdbb);
	DEBUG;
	SBM_reset(bitmap);
	window.win_flags = 0;

	page =
		BTR_find_page(tdbb, retrieval, &window, &idx, &lower, &upper, FALSE);

/* If there is a starting descriptor, search down index to starting position.
   This may involve sibling buckets if splits are in progress.  If there 
   isn't a starting descriptor, walk down the left side of the index. */

	if (retrieval->irb_lower_count) {
		while (!
			   (node =
				BTR_find_leaf(page, &lower, 0, &prefix,
							  idx.idx_flags & idx_descending, TRUE)))
				page =
				(BTR) CCH_HANDOFF(tdbb, &window, page->btr_sibling,
								  LCK_read, pag_index);

		/* Compute the number of matching characters in lower and upper bounds */

		if (retrieval->irb_upper_count) {
			/* TMN: Watch out, possibility for UCHAR overflow! */
			prefix =
				(UCHAR) compute_prefix(&upper, lower.key_data,
									   lower.key_length);
		}
	}
	else {
		node = page->btr_nodes;
		prefix = 0;
	}

/* if there is an upper bound, scan the index pages looking for it */

	if (retrieval->irb_upper_count) {
		while (scan(tdbb, node, bitmap, prefix, &upper,
					(USHORT) (retrieval->irb_generic &
							  (irb_partial | irb_descending | irb_starting
							   | irb_equality)))) {
			page =
				(BTR) CCH_HANDOFF(tdbb, &window, page->btr_sibling,
								  LCK_read, pag_index);
			node = page->btr_nodes;
			prefix = 0;
		}
	}
	else {
		/* if there isn't an upper bound, just walk the index to the end of the level */
		while (TRUE) {
			number = BTR_get_quad(BTN_NUMBER(node));

			if (number == END_LEVEL)
				break;

#ifdef IGNORE_NULL_IDX_KEY
			if (number == END_NON_NULL) {
				/* break if we have reached the end of non-null values */
				if (retrieval->irb_generic & irb_ignore_null_value_key)
					break;
				/* Else, go to the next node if we want to look at all of them */
				node = NEXT_NODE(node);
				continue;
			}
#endif /* IGNORE_NULL_IDX_KEY */

			if (number != END_BUCKET) {
				SBM_set(tdbb, bitmap, number);
				node = NEXT_NODE(node);
				continue;
			}

			page =
				(BTR) CCH_HANDOFF(tdbb, &window, page->btr_sibling,
								  LCK_read, pag_index);
			node = page->btr_nodes;
		}
	}

	CCH_RELEASE(tdbb, &window);
}


BTN BTR_find_leaf(BTR bucket,
				  KEY * key,
				  UCHAR * value,
				  UCHAR * return_value, int descending, BOOLEAN retrieval)
{
/**************************************
 *
 *	B T R _ f i n d _ l e a f
 *
 **************************************
 *
 * Functional description
 *	Locate and return a pointer to the insertion point.
 *	If the key doesn't belong in this bucket, return NULL.
 *	A flag indicates the index is descending.
 *
 **************************************/
	BTN node;
	UCHAR prefix, *key_end, *node_end;
	UCHAR *p, *q, *r;
	USHORT l;
	SLONG number;

	DEBUG;
	node = bucket->btr_nodes;
	prefix = 0;
	p = key->key_data;
	key_end = p + key->key_length;

/* If this is an non-leaf bucket of a descending index, the dummy node on the
   front will trip us up.  NOTE: This code may be apocryphal.  I don't see 
   anywhere that a dummy node is stored for a descending index.  - deej */

	if (bucket->btr_level && descending && !BTN_LENGTH(node))
		node = NEXT_NODE(node);

	while (TRUE) {
		/* Pick up data from node */

		if (value && (l = BTN_LENGTH(node))) {
			r = value + BTN_PREFIX(node);
			q = BTN_DATA(node);
			do
				*r++ = *q++;
			while (--l);
		}
		/* If the page/record number is -1, the node is the last in the level
		   and, by definition, is the insertion point.  Otherwise, if the
		   prefix of the current node is less than the running prefix, the 
		   node must have a value greater than the key, so it is the insertion
		   point. */ number = BTR_get_quad(BTN_NUMBER(node));

#ifdef IGNORE_NULL_IDX_KEY
		if (number == END_BUCKET)
			return NULL;

		if ((number == END_NON_NULL)
			&& (key->key_flags & KEY_first_segment_is_null)) {
			/* reset running prefix length. */
			prefix = 0;
			p = key->key_data;
			/* We are looking for a key with initial NULL segment. Go past the
			 * END_NON_NULL marker. */
			node = NEXT_NODE(node);
			continue;
		}
#endif /* IGNORE_NULL_IDX_KEY */

#ifdef IGNORE_NULL_IDX_KEY
		if (
			(number == END_NON_NULL
			 && !(key->key_flags & KEY_first_segment_is_null))
			|| number == END_LEVEL || BTN_PREFIX(node) < prefix)
#else
		if (number == END_LEVEL || BTN_PREFIX(node) < prefix)
#endif /* IGNORE_NULL_IDX_KEY */
		{
			if (return_value)
				*return_value = prefix;
			return node;
		}

		/* If the node prefix is greater than current prefix , it must be less 
		   than the key, so we can skip it.  If it has zero length, then
		   it is a duplicate, and can also be skipped. */

		if (BTN_PREFIX(node) == prefix) {
			q = BTN_DATA(node);
			node_end = q + BTN_LENGTH(node);
			if (descending) {
				while (TRUE)
					if (q == node_end || retrieval && p == key_end)
						goto done;
					else if (p == key_end || *p > *q)
						break;
					else if (*p++ < *q++)
						goto done;
			}
			else if (BTN_LENGTH(node) > 0)
				while (TRUE)
					if (p == key_end)
						goto done;
					else if (q == node_end || *p > *q)
						break;
					else if (*p++ < *q++)
						goto done;
			prefix = (UCHAR) (p - key->key_data);
		}
#ifndef IGNORE_NULL_IDX_KEY
		/* this part of the code moved up for IGNORE_NULL... */
		if (number == END_BUCKET)
			return NULL;
#endif /* IGNORE_NULL_IDX_KEY */
		node = NEXT_NODE(node);
	}

  done:
	if (return_value)
		*return_value = prefix;
	return node;
}


BTR BTR_find_page(TDBB tdbb,
				  IRB retrieval,
				  WIN * window,
				  IDX * idx, KEY * lower, KEY * upper, BOOLEAN backwards)
{
/**************************************
 *
 *	B T R _ f i n d _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Initialize for an index retrieval.
 *
 **************************************/
	IRT rpage;
	BTR page;
	SLONG number;
	BTN node;

	SET_TDBB(tdbb);

/* Generate keys before we get any pages locked to avoid unwind
   problems --  if we already have a key, assume that we 
   are looking for an equality */

	if (retrieval->irb_key) {
		copy_key(retrieval->irb_key, lower);
		copy_key(retrieval->irb_key, upper);
	}
	else {
		if (retrieval->irb_upper_count)
			BTR_make_key(tdbb, retrieval->irb_upper_count,
						 retrieval->irb_value +
						 retrieval->irb_desc.idx_count,
						 &retrieval->irb_desc, upper,
						 (USHORT) (retrieval->irb_generic & irb_starting));

		if (retrieval->irb_lower_count)
			BTR_make_key(tdbb, retrieval->irb_lower_count,
						 retrieval->irb_value,
						 &retrieval->irb_desc, lower,
						 (USHORT) (retrieval->irb_generic & irb_starting));
	}

	window->win_page = retrieval->irb_relation->rel_index_root;
	rpage = (IRT) CCH_FETCH(tdbb, window, LCK_read, pag_root);

	if (!BTR_description
		(retrieval->irb_relation, rpage, idx, retrieval->irb_index)) {
		CCH_RELEASE(tdbb, window);
		IBERROR(260);			/* msg 260 index unexpectedly deleted */
	}

	page =
		(BTR) CCH_HANDOFF(tdbb, window, idx->idx_root, LCK_read, pag_index);

/* If there is a starting descriptor, search down index to starting position.
   This may involve sibling buckets if splits are in progress.  If there 
   isn't a starting descriptor, walk down the left side of the index (right
   side if we are going backwards). */

	if ((!backwards && retrieval->irb_lower_count) ||
		(backwards && retrieval->irb_upper_count)) {
		while (page->btr_level > 0)
			while (TRUE) {
				node =
					find_node(page, backwards ? upper : lower,
							  (USHORT) (idx->idx_flags & idx_descending));
				number = BTR_get_quad(BTN_NUMBER(node));
				if (number != END_BUCKET) {
					page =
						(BTR) CCH_HANDOFF(tdbb, window, number, LCK_read,
										  pag_index);
					break;
				}

				page =
					(BTR) CCH_HANDOFF(tdbb, window, page->btr_sibling,
									  LCK_read, pag_index);
			}
	}
	else {
		while (page->btr_level > 0) {
#ifdef SCROLLABLE_CURSORS
			if (backwards)
				node = BTR_last_node(page, NAV_expand_index(window, 0), 0);
			else
#endif
				node = page->btr_nodes;

			number = BTR_get_quad(BTN_NUMBER(node));
			page =
				(BTR) CCH_HANDOFF(tdbb, window, number, LCK_read, pag_index);

			/* make sure that we are actually on the last page on this
			   level when scanning in the backward direction */

			if (backwards)
				while (page->btr_sibling)
					page =
						(BTR) CCH_HANDOFF(tdbb, window, page->btr_sibling,
										  LCK_read, pag_index);
		}
	}

	return page;
}


SLONG BTR_get_quad(const SCHAR* data)
{
/**************************************
 *
 *	B T R _ g e t _ q u a d
 *
 **************************************
 *
 * Functional description
 *	Get a four byte binary number.
 *
 **************************************/
	LONGCHAR value;

	value.c[0] = data[0];
	value.c[1] = data[1];
	value.c[2] = data[2];
	value.c[3] = data[3];

	return value.n;
}


void BTR_insert(TDBB tdbb, WIN * root_window, IIB * insertion)
{
/**************************************
 *
 *	B T R _ i n s e r t
 *
 **************************************
 *
 * Functional description
 *	Insert a node into an index.
 *
 **************************************/
	DBB dbb;
	IDX *idx;
	WIN window, new_window;
	BTR bucket, new_bucket;
	KEY key;
	IRT root;
	BTN node;
	UCHAR *p, *q;
	USHORT l;
	SLONG split_page;

	dbb = tdbb->tdbb_database;

	DEBUG;
	idx = insertion->iib_descriptor;
	window.win_page = idx->idx_root;
	window.win_flags = 0;
	bucket = (BTR) CCH_FETCH(tdbb, &window, LCK_read, pag_index);

	if (bucket->btr_level == 0) {
		CCH_RELEASE(tdbb, &window);
		CCH_FETCH(tdbb, &window, LCK_write, pag_index);
	}
	CCH_RELEASE(tdbb, root_window);

	if (
		(split_page =
		 add_node(tdbb, &window, insertion, &key, NULL, NULL)) == 0) return;

/* The top of the index has split.  We need to make a new level and
   update the index root page.  Oh boy. */

	root = (IRT) CCH_FETCH(tdbb, root_window, LCK_write, pag_root);

	window.win_page = root->irt_rpt[idx->idx_id].irt_root;
	bucket = (BTR) CCH_FETCH(tdbb, &window, LCK_write, pag_index);

/* the original page was marked as not garbage-collectable, but 
   since it is the root page it won't be garbage-collected anyway, 
   so go ahead and mark it as garbage-collectable now */

	CCH_MARK(tdbb, &window);
	bucket->btr_header.pag_flags &= ~btr_dont_gc;

	new_window.win_page = split_page;
	new_window.win_flags = 0;
	new_bucket = (BTR) CCH_FETCH(tdbb, &new_window, LCK_read, pag_index);

	if (bucket->btr_level != new_bucket->btr_level) {
		CCH_RELEASE(tdbb, &new_window);
		CCH_RELEASE(tdbb, &window);
		CORRUPT(204);			/* msg 204 index inconsistent */
	}

	CCH_RELEASE(tdbb, &new_window);
	CCH_RELEASE(tdbb, &window);

/* Allocate and format new bucket */

	new_bucket = (BTR) DPM_allocate(tdbb, &new_window);
	CCH_precedence(tdbb, &new_window, window.win_page);
	new_bucket->btr_header.pag_type = pag_index;
	new_bucket->btr_relation = bucket->btr_relation;
	new_bucket->btr_level = bucket->btr_level + 1;
	new_bucket->btr_id = bucket->btr_id;
	new_bucket->btr_header.pag_flags |=
		(bucket->btr_header.pag_flags & btr_descending);

/* Set up first node as degenerate, but pointing to first bucket on
   next level. */

	node = new_bucket->btr_nodes;
	quad_put(window.win_page, BTN_NUMBER(node));
	BTN_PREFIX(node) = 0;
	BTN_LENGTH(node) = 0;
	node = NEXT_NODE(node);

/* Move in the split node */

	quad_put(split_page, BTN_NUMBER(node));
	BTN_PREFIX(node) = 0;
	assert(key.key_length <= MAX_UCHAR);
	l = BTN_LENGTH(node) = (UCHAR) key.key_length;
	q = BTN_DATA(node);
	p = key.key_data;
	if (l) {
		do {
			MOVE_BYTE(p, q);
		} while (--l);
	}
	node = NEXT_NODE(node);

/* mark end of level */

	BTN_PREFIX(node) = 0;
	BTN_LENGTH(node) = 0;
	quad_put((SLONG) END_LEVEL, BTN_NUMBER(node));
	node = NEXT_NODE(node);

	new_bucket->btr_length = (UCHAR *) node - (UCHAR *) new_bucket;

/* update the root page to point to the new top-level page, 
   and make sure the new page has higher precedence so that 
   it will be written out first--this will make sure that the 
   root page doesn't point into space */

	CCH_RELEASE(tdbb, &new_window);
	CCH_precedence(tdbb, root_window, new_window.win_page);
	CCH_MARK(tdbb, root_window);
	root->irt_rpt[idx->idx_id].irt_root = new_window.win_page;

/* journal root page change */

	if (dbb->dbb_wal) {
		JRNRP journal;
		journal.jrnrp_type = JRNP_ROOT_PAGE;
		journal.jrnrp_id = idx->idx_id;
		journal.jrnrp_page = new_window.win_page;
		CCH_journal_record(tdbb, root_window, (UCHAR *) & journal,
						   JRNRP_SIZE, 0, 0);
	}

	CCH_RELEASE(tdbb, root_window);
}


IDX_E BTR_key(TDBB tdbb, JRD_REL relation, REC record, IDX * idx, KEY * key, idx_null_state * null_state)
{
/**************************************
 *
 *	B T R _ k e y
 *
 **************************************
 *
 * Functional description
 *	Compute a key from an record and an index descriptor. 
 *	Note that compound keys are expanded by 25%.  If this
 *	changes, both BTR_key_length and GDEF exe.e have to
 *	change.
 *
 **************************************/
	KEY temp;
	DSC desc, *desc_ptr;
	SSHORT stuff_count;
	USHORT n, l;
	UCHAR *p, *q;
	IDX_E result;
	idx::idx_repeat * tail;
	int not_missing;
	int missing_unique_segments = 0;

	result = idx_e_ok;
	tail = idx->idx_rpt;

	try {

#ifdef IGNORE_NULL_IDX_KEY
/* Initialize KEY flags */
	key->key_flags = 0;
#endif /* IGNORE_NULL_IDX_KEY */

/* Special case single segment indices */

	if (idx->idx_count == 1) {
#ifdef EXPRESSION_INDICES
		/* for expression indices, compute the value of the expression */

		if (idx->idx_expression) {
			JRD_REQ current_request;
			current_request = tdbb->tdbb_request;
			tdbb->tdbb_request = idx->idx_expression_request;
			tdbb->tdbb_request->req_rpb[0].rpb_record = record;

			if (!(desc_ptr = EVL_expr(tdbb, idx->idx_expression)))
				desc_ptr = &idx->idx_expression_desc;

			not_missing =
				tdbb->tdbb_request->req_flags & req_null ? FALSE : TRUE;
			tdbb->tdbb_request = current_request;
		}
		else
#endif
		{
			desc_ptr = &desc;
			/* In order to "map a null to a default" value (in EVL_field()), 
			 * the relation block is referenced. 
			 * Reference: Bug 10116, 10424 
			 */
			not_missing =
				EVL_field(relation, record, tail->idx_field, desc_ptr);
		}

		if (!not_missing && (idx->idx_flags & idx_unique))
			missing_unique_segments++;

		compress(tdbb, desc_ptr, key, tail->idx_itype,
				 (USHORT) ((not_missing) ? FALSE : TRUE),
				 (USHORT) (idx->idx_flags & idx_descending), (USHORT) FALSE);
#ifdef IGNORE_NULL_IDX_KEY
		if (!not_missing) {
			key->key_flags |= KEY_first_segment_is_null;
		}
#endif /* IGNORE_NULL_IDX_KEY */
	}
	else {
		p = key->key_data;
		stuff_count = 0;
		for (n = 0; n < idx->idx_count; n++, tail++) {
			for (; stuff_count; --stuff_count)
				*p++ = 0;

			desc_ptr = &desc;
			/* In order to "map a null to a default" value (in EVL_field()), 
			 * the relation block is referenced. 
			 * Reference: Bug 10116, 10424 
			 */
			not_missing =
				EVL_field(relation, record, tail->idx_field, desc_ptr);
			if (!not_missing && (idx->idx_flags & idx_unique))
				missing_unique_segments++;

			compress(tdbb, desc_ptr, &temp, tail->idx_itype,
					 (USHORT) ((not_missing) ? FALSE : TRUE),
					 (USHORT) (idx->idx_flags & idx_descending),
					 (USHORT) FALSE);
#ifdef IGNORE_NULL_IDX_KEY
			if (n == 0 && !not_missing) {
				key->key_flags |= KEY_first_segment_is_null;
			}
#endif /* IGNORE_NULL_IDX_KEY */

			for (q = temp.key_data, l = temp.key_length; l;
				 --l, --stuff_count) {
				if (stuff_count == 0) {
					*p++ = idx->idx_count - n;
					stuff_count = STUFF_COUNT;
				}
				*p++ = *q++;
			}
		}
		key->key_length = p - key->key_data;
	}

	if (key->key_length >= MAX_KEY)
		result = idx_e_keytoobig;

	if (idx->idx_flags & idx_descending)
		BTR_complement_key(key);

	if (null_state) {
		*null_state = !missing_unique_segments ? idx_nulls_none :
			(missing_unique_segments == idx->idx_count) ? 
			    idx_nulls_all : 
				idx_nulls_some;
	}

	return result;

	}	// try
	catch(const std::exception&) {
		key->key_length = 0;
		return idx_e_conversion;
	}
}


USHORT BTR_key_length(JRD_REL relation, IDX * idx)
{
/**************************************
 *
 *	B T R _ k e y _ l e n g t h
 *
 **************************************
 *
 * Functional description
 *	Compute the maximum key length for an index.
 *
 **************************************/
	FMT format;
	USHORT n, key_length, length;
	idx::idx_repeat * tail;
	TDBB tdbb;

	tdbb = GET_THREAD_DATA;

	format = MET_current(tdbb, relation);
	tail = idx->idx_rpt;

/* If there is only a single key, the computation is straightforward. */

	if (idx->idx_count == 1) {
		if (tail->idx_itype == idx_numeric ||
			tail->idx_itype == idx_timestamp1) return sizeof(double);

		if (tail->idx_itype == idx_sql_time)
			return sizeof(ULONG);

		if (tail->idx_itype == idx_sql_date)
			return sizeof(SLONG);

		if (tail->idx_itype == idx_timestamp2)
			return sizeof(SINT64);

		if (tail->idx_itype == idx_numeric2)
			return INT64_KEY_LENGTH;

#ifdef EXPRESSION_INDICES
		if (idx->idx_expression) {
			length = idx->idx_expression_desc.dsc_length;
			if (idx->idx_expression_desc.dsc_dtype == dtype_varying)
				length = length - sizeof(SSHORT);
		}
		else
#endif
		{
			length = format->fmt_desc[tail->idx_field].dsc_length;
			if (format->fmt_desc[tail->idx_field].dsc_dtype == dtype_varying)
				length = length - sizeof(SSHORT);
		}

		if (tail->idx_itype >= idx_first_intl_string)
			return INTL_key_length(tdbb, tail->idx_itype, length);
		else
			return length;
	}

/* Compute length of key for segmented indices. */

	key_length = 0;

	for (n = 0; n < idx->idx_count; n++, tail++) {
		if (tail->idx_itype == idx_numeric ||
			tail->idx_itype == idx_timestamp1) length = sizeof(double);
		else if (tail->idx_itype == idx_sql_time)
			length = sizeof(ULONG);
		else if (tail->idx_itype == idx_sql_date)
			length = sizeof(ULONG);
		else if (tail->idx_itype == idx_timestamp2)
			length = sizeof(SINT64);
		else if (tail->idx_itype == idx_numeric2)
			length = INT64_KEY_LENGTH;
		else {
			length = format->fmt_desc[tail->idx_field].dsc_length;
			if (format->fmt_desc[tail->idx_field].dsc_dtype == dtype_varying)
				length -= sizeof(SSHORT);
			if (tail->idx_itype >= idx_first_intl_string)
				length = INTL_key_length(tdbb, tail->idx_itype, length);
		}
		key_length +=
			((length + STUFF_COUNT - 1) / STUFF_COUNT) * (STUFF_COUNT + 1);
	}

	return key_length;
}


#ifdef SCROLLABLE_CURSORS
BTN BTR_last_node(BTR page, EXP expanded_page, BTX * expanded_node)
{
/**************************************
 *
 *	B T R _ l a s t _ n o d e
 *
 **************************************
 *
 * Functional description                               
 *	Find the last node on a page.  Used when walking
 *	down the right side of an index tree.  
 *
 **************************************/
	BTN node, prior;
	SLONG number;
	BTX enode;

/* the last expanded node is always at the end of the page 
   minus the size of a BTX, since there is always an extra
   BTX node with zero-length tail at the end of the page */



	enode =
		(BTX) ((UCHAR *) expanded_page + expanded_page->exp_length -
			   BTX_SIZE);
	node = (BTN) ((UCHAR *) page + page->btr_length);

/* starting at the end of the page, find the
   first node that is not an end marker */

	while (TRUE) {
		node = BTR_previous_node(node, &enode);

		number = BTR_get_quad(BTN_NUMBER(node));
#ifdef IGNORE_NULL_IDX_KEY
		if (number != END_NON_NULL && number != END_BUCKET
			&& number != END_LEVEL)
#else
		if (number != END_BUCKET && number != END_LEVEL)
#endif /* IGNORE_NULL_IDX_KEY */
		{
			if (expanded_node)
				*expanded_node = enode;
			return node;
		}
	}
}
#endif
#ifdef SCROLLABLE_CURSORS
BTR BTR_left_handoff(TDBB tdbb, WIN * window, BTR page, SSHORT lock_level)
{
/**************************************
 *
 *	B T R _ l e f t _ h a n d o f f
 *
 **************************************
 *
 * Functional description
 *	Handoff a btree page to the left.  This is more difficult than a 
 *	right handoff because we have to traverse pages without handing 
 *	off locks.  (A lock handoff to the left while someone was handing
 *	off to the right could result in deadlock.)
 *
 **************************************/
	SLONG original_page, sibling, left_sibling;
	WIN fix_win;
	BTR fix_page;
	DBB dbb;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	original_page = window->win_page;
	left_sibling = page->btr_left_sibling;

	CCH_RELEASE(tdbb, window);
	window->win_page = left_sibling;
	page = (BTR) CCH_FETCH(tdbb, window, lock_level, pag_index);

	if ((sibling = page->btr_sibling) == original_page)
		return page;

/* Since we are not handing off pages, a page could split before we get to it.
 * To detect this case, fetch the left sibling pointer and then handoff right
 * sibling pointers until we reach the page to the left of the page passed 
 * to us. 
 */

	while (sibling != original_page) {
		page =
			(BTR) CCH_HANDOFF(tdbb, window, page->btr_sibling,
							  lock_level, pag_index);
		sibling = page->btr_sibling;
	}
	fix_win.win_page = original_page;
	fix_win.win_flags = 0;
	fix_page = (BTR) CCH_FETCH(tdbb, &fix_win, LCK_write, pag_index);

/* if someone else already fixed it, just return */

	if (fix_page->btr_left_sibling == window->win_page) {
		CCH_RELEASE(tdbb, &fix_win);
		return page;
	}

	CCH_MARK(tdbb, &fix_win);
	fix_page->btr_left_sibling = window->win_page;

	if (dbb->dbb_journal)
		CCH_journal_page(tdbb, &fix_win);

	CCH_RELEASE(tdbb, &fix_win);

	return page;
}
#endif


USHORT BTR_lookup(TDBB tdbb, JRD_REL relation, USHORT id, IDX * buffer)
{
/**************************************
 *
 *	B T R _ l o o k u p
 *
 **************************************
 *
 * Functional description
 *	Return a description of the specified index.
 *
 **************************************/
	WIN window;
	IRT root;

	SET_TDBB(tdbb);
	window.win_flags = 0;

	if (!(root = fetch_root(tdbb, &window, relation)))
		return FB_FAILURE;

	if ((id >= root->irt_count)
		|| !BTR_description(relation, root, buffer, id)) {
		CCH_RELEASE(tdbb, &window);
		return FB_FAILURE;
	}
	CCH_RELEASE(tdbb, &window);
	return FB_SUCCESS;
}


void BTR_make_key(TDBB tdbb,
				  USHORT count,
				  JRD_NOD * exprs, IDX * idx, KEY * key, USHORT fuzzy)
{
/**************************************
 *
 *	B T R _ m a k e _ k e y
 *
 **************************************
 *
 * Functional description
 *	Construct a (possibly) compound search key given a key count,
 *	a vector of value expressions, and a place to put the key.
 *
 **************************************/
	DSC *desc, temp_desc;
	SSHORT stuff_count;
	USHORT n, l;
	UCHAR *p, *q;
	KEY temp;
	int missing;
	idx::idx_repeat * tail;

	SET_TDBB(tdbb);
	assert(count > 0);
	assert(idx != NULL);
	assert(exprs != NULL);
	assert(key != NULL);

	tail = idx->idx_rpt;

#ifdef IGNORE_NULL_IDX_KEY
/* Initialize KEY flags */
	key->key_flags = 0;
#endif /* IGNORE_NULL_IDX_KEY */

/* If the index is a single segment index, don't sweat the compound
   stuff. */

	if (idx->idx_count == 1) {
		desc = eval(tdbb, *exprs, &temp_desc, &missing);
		compress(tdbb, desc, key, tail->idx_itype, (USHORT) missing,
				 (USHORT) (idx->idx_flags & idx_descending), fuzzy);
#ifdef IGNORE_NULL_IDX_KEY
		if (missing) {
			key->key_flags |= KEY_first_segment_is_null;
		}
#endif /* IGNORE_NULL_IDX_KEY */
	}
	else {
		/* Make a compound key */

		p = key->key_data;
		stuff_count = 0;

		for (n = 0; n < count; n++, tail++) {
			for (; stuff_count; --stuff_count)
				*p++ = 0;
			desc = eval(tdbb, *exprs++, &temp_desc, &missing);
			compress(tdbb, desc, &temp, tail->idx_itype,
					 (USHORT) missing,
					 (USHORT) (idx->idx_flags & idx_descending),
					 (USHORT) ((n == count - 1) ? fuzzy : FALSE));
#ifdef IGNORE_NULL_IDX_KEY
			if (n == 0 && missing) {
				key->key_flags |= KEY_first_segment_is_null;
			}
#endif /* IGNORE_NULL_IDX_KEY */
			for (q = temp.key_data, l = temp.key_length; l;
				 --l, --stuff_count) {
				if (stuff_count == 0) {
					*p++ = idx->idx_count - n;
					stuff_count = STUFF_COUNT;
				}
				*p++ = *q++;
			}
		}

		// AB: Fix bug SF #1242982
		// Equality search on first segment (integer) in compound indexes resulted 
		// in more scans on specific values (2^n, f.e. 131072) then needed.
		if (!fuzzy && (n != idx->idx_count)) {
			for (; stuff_count; --stuff_count) {
				*p++ = 0;
			}
		}

		key->key_length = p - key->key_data;
	}

	if (idx->idx_flags & idx_descending)
		BTR_complement_key(key);
}


BOOLEAN BTR_next_index(TDBB tdbb,
					   JRD_REL relation, JRD_TRA transaction, IDX * idx, WIN * window)
{
/**************************************
 *
 *	B T R _ n e x t _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Get next index for relation.  Index ids
 *  recently change from UCHAR to SHORT
 *
 **************************************/
	IRT root;
	SSHORT id;
	SLONG trans;
	int trans_state;
	irt::irt_repeat * irt_desc;

	SET_TDBB(tdbb);

	if ((USHORT)idx->idx_id == (USHORT)-1) {
		id = 0;
		window->win_bdb = NULL;
	}
	else
		id = idx->idx_id + 1;

	if (window->win_bdb)
		root = (IRT) window->win_buffer;
	else if (!(root = fetch_root(tdbb, window, relation)))
		return 0;

	for (; id < root->irt_count; ++id) {
		irt_desc = root->irt_rpt + id;
		if (!irt_desc->irt_root &&
			(irt_desc->irt_flags & irt_in_progress) && transaction) {
			trans = irt_desc->irt_stuff.irt_transaction;
			CCH_RELEASE(tdbb, window);
			trans_state = TRA_wait(tdbb, transaction, trans, TRUE);
			if ((trans_state == tra_dead)
				|| (trans_state == tra_committed)) {
				/* clean up this left-over index */
				root = (IRT) CCH_FETCH(tdbb, window, LCK_write, pag_root);
				irt_desc = root->irt_rpt + id;
				if (!irt_desc->irt_root &&
					irt_desc->irt_stuff.irt_transaction == trans &&
					(irt_desc->irt_flags & irt_in_progress))
					BTR_delete_index(tdbb, window, id);
				else
					CCH_RELEASE(tdbb, window);
				root = (IRT) CCH_FETCH(tdbb, window, LCK_read, pag_root);
				continue;
			}
			else
				root = (IRT) CCH_FETCH(tdbb, window, LCK_read, pag_root);
		}
		if (BTR_description(relation, root, idx, id))
			return TRUE;
	}

	CCH_RELEASE(tdbb, window);

	return FALSE;
}


BTN BTR_next_node(BTN node, BTX * expanded_node)
{
/**************************************
 *
 *	B T R _ n e x t _ n o d e
 *
 **************************************
 *
 * Functional description                               
 *	Find the next node on both the index page
 *	and its associated expanded buffer.
 *
 **************************************/

	if (*expanded_node)
		*expanded_node = NEXT_EXPANDED((*expanded_node), node);

	return NEXT_NODE(node);
}


BTN BTR_previous_node(BTN node, BTX * expanded_node)
{
/**************************************
 *
 *	B T R _ p r e v i o u s _ n o d e
 *
 **************************************
 *
 * Functional description                               
 *	Find the previous node on a page.  Used when walking
 *	an index backwards.  
 *
 **************************************/

	node =
		(BTN) ((UCHAR *) node -
			   (*expanded_node)->btx_btr_previous_length - BTN_SIZE);
	*expanded_node =
		(BTX) ((UCHAR *) * expanded_node -
			   (*expanded_node)->btx_previous_length - BTX_SIZE);

	return node;
}


void BTR_remove(TDBB tdbb, WIN * root_window, IIB * insertion)
{
/**************************************
 *
 *	B T R _ r e m o v e
 *
 **************************************
 *
 * Functional description
 *	Remove an index node from a b-tree.  
 *	If the node doesn't exist, don't get overly excited.
 *
 **************************************/
	DBB dbb;
	IDX *idx;
	WIN window;
	BTR page;
	BTN node;
	SLONG number;
	CONTENTS result;
	UCHAR level;
	IRT root;
	JRNRP journal;

	DEBUG;
	dbb = tdbb->tdbb_database;
	idx = insertion->iib_descriptor;
	window.win_page = idx->idx_root;
	window.win_flags = 0;
	page = (BTR) CCH_FETCH(tdbb, &window, LCK_read, pag_index);

/* If the page is level 0, re-fetch it for write */

	level = page->btr_level;
	if (level == 0) {
		CCH_RELEASE(tdbb, &window);
		CCH_FETCH(tdbb, &window, LCK_write, pag_index);
	}
/* remove the node from the index tree via recursive descent */
	result = remove_node(tdbb, insertion, &window);

/* if the root page points at only one lower page, remove this 
   level to prevent the tree from being deeper than necessary-- 
   do this only if the level is greater than 1 to prevent 
   excessive thrashing in the case where a small table is 
   constantly being loaded and deleted */

	if ((result == contents_single) && (level > 1)) {
		/* we must first release the windows to obtain the root for write 
		   without getting deadlocked */

		CCH_RELEASE(tdbb, &window);
		CCH_RELEASE(tdbb, root_window);

		root = (IRT) CCH_FETCH(tdbb, root_window, LCK_write, pag_root);
		page = (BTR) CCH_FETCH(tdbb, &window, LCK_write, pag_index);

		/* get the page number of the child, and check to make sure 
		   the page still has only one node on it */

		node = page->btr_nodes;
		number = BTR_get_quad(BTN_NUMBER(node));
		node = NEXT_NODE(node);
		if (BTR_get_quad(BTN_NUMBER(node)) >= 0) {
			CCH_RELEASE(tdbb, &window);
			CCH_RELEASE(tdbb, root_window);
			return;
		}

		CCH_MARK(tdbb, root_window);
		root->irt_rpt[idx->idx_id].irt_root = number;

		/* journal root page change */

		if (dbb->dbb_wal) {
			journal.jrnrp_type = JRNP_ROOT_PAGE;
			journal.jrnrp_id = idx->idx_id;
			journal.jrnrp_page = number;
			CCH_journal_record(tdbb, root_window, (UCHAR *) & journal,
							   JRNRP_SIZE, 0, 0);
		}

		/* release the pages, and place the page formerly at the top level 
		   on the free list, making sure the root page is written out first 
		   so that we're not pointing to a released page */

		CCH_RELEASE(tdbb, root_window);
		CCH_RELEASE(tdbb, &window);
		PAG_release_page(window.win_page, root_window->win_page);
	}

	if (window.win_bdb)
		CCH_RELEASE(tdbb, &window);
	if (root_window->win_bdb)
		CCH_RELEASE(tdbb, root_window);
}


void BTR_reserve_slot(TDBB tdbb, JRD_REL relation, JRD_TRA transaction, IDX * idx)
{
/**************************************
 *
 *	B T R _ r e s e r v e _ s l o t
 *
 **************************************
 *
 * Functional description
 *	Reserve a slot on an index root page 
 *	in preparation to index creation.
 *
 **************************************/
	DBB dbb;
	WIN window;
	IRT root;
	IRTD *desc;
	USHORT l, space;
	irt::irt_repeat * root_idx, *end, *slot;
	BOOLEAN maybe_no_room = FALSE;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

/* Get root page, assign an index id, and store the index descriptor.
   Leave the root pointer null for the time being.  */

	window.win_page = relation->rel_index_root;
	window.win_flags = 0;
	root = (IRT) CCH_FETCH(tdbb, &window, LCK_write, pag_root);
	CCH_MARK(tdbb, &window);

/* check that we create no more indexes than will fit on a single root page */

	if (root->irt_count > dbb->dbb_max_idx) {
		CCH_RELEASE(tdbb, &window);
		ERR_post(gds_no_meta_update, gds_arg_gds, gds_max_idx,
				 gds_arg_number, (SLONG) dbb->dbb_max_idx, 0);
	}
/* Scan the index page looking for the high water mark of the descriptions and,
         perhaps, an empty index slot */ 

retry:
	l = idx->idx_count * sizeof(IRTD);
	space = dbb->dbb_page_size;
	slot = NULL;

	for (root_idx = root->irt_rpt, end = root_idx + root->irt_count;
		 root_idx < end; root_idx++) {
		if (root_idx->irt_root || (root_idx->irt_flags & irt_in_progress))
			space = MIN(space, root_idx->irt_desc);
		if (!root_idx->irt_root && !slot
			&& !(root_idx->irt_flags & irt_in_progress)) slot = root_idx;
	}

	space -= l;
	desc = (IRTD *) ((UCHAR *) root + space);

/* Verify that there is enough room on the Index root page. */

	if (desc < (IRTD *) (end + 1)) {
		/* Not enough room:  Attempt to compress the index root page and try again.
		   If this is the second try already, then there really is no more room. */
		if (maybe_no_room) {
			CCH_RELEASE(tdbb, &window);
			ERR_post(gds_no_meta_update, gds_arg_gds,
					 gds_index_root_page_full, 0);
		}
		compress_root(tdbb, root);
		maybe_no_room = TRUE;
		goto retry;
	}

/* If we didn't pick up an empty slot, allocate a new one */

	if (!slot) {
		slot = end;
		root->irt_count++;
	}

	idx->idx_id = slot - root->irt_rpt;
	slot->irt_desc = space;
	assert(idx->idx_count <= MAX_UCHAR);
	slot->irt_keys = (UCHAR) idx->idx_count;
	slot->irt_flags = idx->idx_flags | irt_in_progress;

	if (transaction)
		slot->irt_stuff.irt_transaction = transaction->tra_number;

	slot->irt_root = 0;
	MOVE_FASTER(idx->idx_rpt, desc, l);

	if (dbb->dbb_wal)
		CCH_journal_page(tdbb, &window);

	CCH_RELEASE(tdbb, &window);
}


float BTR_selectivity(TDBB tdbb, JRD_REL relation, USHORT id)
{
/**************************************
 *
 *	B T R _ s e l e c t i v i t y
 *
 **************************************
 *
 * Functional description
 *	Update index selectivity on the fly.
 *	Note that index leaf pages are walked
 *	without visiting data pages. Thus the
 *	effects of uncommitted transactions
 *	will be included in the calculation.
 *
 **************************************/
	BTR bucket;
	IRT root;
	BTN node;
	SSHORT l, dup;
	UCHAR *p, *q;
	SLONG page, nodes, duplicates;
	KEY key;
	WIN window;
	float selectivity;

	SET_TDBB(tdbb);
	window.win_flags = 0;

	if (!(root = fetch_root(tdbb, &window, relation)))
		return 0.0;

	if (root->irt_count <= id || !(page = root->irt_rpt[id].irt_root)) {
		CCH_RELEASE(tdbb, &window);
		return 0.0;
	}
	window.win_flags = WIN_large_scan;
	window.win_scans = 1;
	bucket = (BTR) CCH_HANDOFF(tdbb, &window, page, LCK_read, pag_index);

/* go down the left side of the index to leaf level */

	while (bucket->btr_level) {
		node = bucket->btr_nodes;
		page = BTR_get_quad(BTN_NUMBER(node));
		bucket = (BTR) CCH_HANDOFF(tdbb, &window, page, LCK_read, pag_index);
	}

	duplicates = nodes = 0;
	key.key_length = 0;

/* go through all the leaf nodes and count them; 
   also count how many of them are duplicates */

	while (page) {
		for (node = bucket->btr_nodes;; node = NEXT_NODE(node)) {
			page = BTR_get_quad(BTN_NUMBER(node));
#ifdef IGNORE_NULL_IDX_KEY
			if (page == END_BUCKET || page == END_LEVEL)
				break;
			if (page == END_NON_NULL) {
				/* reset saved key. New comparisons to start for keys
				 * with initial segment NULL
				 */
				key.key_length = 0;
				continue;
			}
#else
			if (page < 0)
				break;
#endif /* IGNORE_NULL_IDX_KEY */
			++nodes;
			l = node->btn_length + node->btn_prefix;

			/* figure out if this is a duplicate */

			if (node == bucket->btr_nodes)
				dup = key_equality(&key, node);
			else
				dup = !node->btn_length && l == key.key_length;
			if (dup)
				++duplicates;

			/* keep the key value current for comparison with the next key */

			key.key_length = l;
			if ( (l = node->btn_length) ) {
				p = key.key_data + node->btn_prefix;
				q = node->btn_data;
				do
					*p++ = *q++;
				while (--l);
			}
		}

		if (page == END_LEVEL || !(page = bucket->btr_sibling))
			break;
		bucket =
			(BTR) CCH_HANDOFF_TAIL(tdbb, &window, page, LCK_read, pag_index);
	}

	CCH_RELEASE_TAIL(tdbb, &window);

/* calculate the selectivity and store it on the root page */

	selectivity =
		(float) ((nodes) ? 1.0 / (float) (nodes - duplicates) : 0.0);
	window.win_page = relation->rel_index_root;
	window.win_flags = 0;
	root = (IRT) CCH_FETCH(tdbb, &window, LCK_write, pag_root);
	CCH_MARK(tdbb, &window);
	root->irt_rpt[id].irt_stuff.irt_selectivity = selectivity;
	CCH_RELEASE(tdbb, &window);

	return selectivity;
}


static SLONG add_node(TDBB tdbb,
					  WIN * window,
					  IIB * insertion,
					  KEY * new_key,
					  SLONG * original_page, SLONG * sibling_page)
{
/**************************************
 *
 *	a d d _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Insert a node in an index.  This recurses to the leaf level.
 *	If a split occurs, return the new index page number and its
 *	leading string.
 *
 **************************************/
	BTR bucket;
	BTN node;
	IIB propogate;
	SLONG split, page, index;
	SLONG original_page2, sibling_page2;

	DEBUG;
	bucket = (BTR) window->win_buffer;

/* For leaf level guys, loop thru the leaf buckets until insertion
   point is found (should be instant) */

	if (bucket->btr_level == 0)
		while (TRUE)
			if (
				(split =
				 insert_node(tdbb, window, insertion, new_key,
							 original_page, sibling_page)) >= 0)
				return split;
			else


				bucket =
					(BTR) CCH_HANDOFF(tdbb, window,
									  bucket->btr_sibling, LCK_write,
									  pag_index);

/* If we're above the leaf level, find the appropriate node in the chain of sibling pages.
   Hold on to this position while we recurse down to the next level, in case there's a 
   split at the lower level, in which case we need to insert the new page at this level. */

#ifdef IGNORE_NULL_IDX_KEY
	assert(bucket->btr_level != 0);
#endif /* IGNORE_NULL_IDX_KEY */
	while (TRUE) {
		node =
			find_node(bucket, insertion->iib_key,
					  (USHORT) (insertion->iib_descriptor->idx_flags &
								idx_descending));
		page = BTR_get_quad(BTN_NUMBER(node));
#ifdef IGNORE_NULL_IDX_KEY
		assert(page != END_NON_NULL);
#endif /* IGNORE_NULL_IDX_KEY */
		if (page != END_BUCKET)
			break;
		bucket =
			(BTR) CCH_HANDOFF(tdbb, window, bucket->btr_sibling,
							  LCK_read, pag_index);
	}
/* Fetch the page at the next level down.  If the next level is leaf level, 
   fetch for write since we know we are going to write to the page (most likely). */
	index = window->win_page;
	CCH_HANDOFF(tdbb, window, page,
				(SSHORT) ((bucket->btr_level == 1) ? LCK_write :
						  LCK_read), pag_index);

/* now recursively try to insert the node at the next level down */

	split =
		add_node(tdbb, window, insertion, new_key, &page,
				 &propogate.iib_sibling);
	if (split == 0)
		return 0;

/* The page at the lower level split, so we need to insert a pointer 
   to the new page to the page at this level. */

	window->win_page = index;
	bucket = (BTR) CCH_FETCH(tdbb, window, LCK_write, pag_index);

	propogate.iib_number = split;
	propogate.iib_descriptor = insertion->iib_descriptor;
	propogate.iib_relation = insertion->iib_relation;
	propogate.iib_duplicates = NULL;
	propogate.iib_key = new_key;

/* now loop through the sibling pages trying to find the appropriate 
   place to put the pointer to the lower level page--remember that the 
   page we were on could have split while we weren't looking */

	while (TRUE)
		if (
			(split =
			 insert_node(tdbb, window, &propogate, new_key,
						 &original_page2, &sibling_page2)) >= 0)
			break;
		else
			bucket =
				(BTR) CCH_HANDOFF(tdbb, window, bucket->btr_sibling,
								  LCK_write, pag_index);

/* the split page on the lower level has been propogated, so we can go back to 
   the page it was split from, and mark it as garbage-collectable now */

	window->win_page = page;
	bucket = (BTR) CCH_FETCH(tdbb, window, LCK_write, pag_index);
	CCH_MARK(tdbb, window);
	bucket->btr_header.pag_flags &= ~btr_dont_gc;
	CCH_RELEASE(tdbb, window);

	if (original_page)
		*original_page = original_page2;
	if (sibling_page)
		*sibling_page = sibling_page2;
	return split;
}


void BTR_complement_key(KEY * key)
{
/**************************************
 *
 *	c o m p l e m e n t _ k e y
 *
 **************************************
 *
 * Functional description
 *	Negate a key for descending index.
 *
 **************************************/
	UCHAR *p, *end;

	for (p = key->key_data, end = p + key->key_length; p < end; p++)
		*p ^= -1;
}

static void compress(TDBB tdbb,
					 DSC * desc,
					 KEY * key,
					 USHORT itype,
					 USHORT missing, USHORT descending, USHORT fuzzy)
{
/**************************************
 *
 *	c o m p r e s s
 *
 **************************************
 *
 * Functional description
 *	Compress a data value into an index key.
 *
 **************************************/
	DBB dbb;
	UCHAR *q, *p;
	USHORT length;
	UCHAR pad, *ptr, temp1[MAX_KEY];
	union {
		INT64_KEY temp_int64_key;
		double temp_double;
		ULONG temp_ulong;
		SLONG temp_slong;
		SINT64 temp_sint64;
		UCHAR temp_char[sizeof(INT64_KEY)];
	} temp;
	USHORT temp_copy_length;
	BOOLEAN temp_is_negative = FALSE;
	BOOLEAN int64_key_op = FALSE;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	p = key->key_data;

	if (missing && dbb->dbb_ods_version >= ODS_VERSION7) {
		pad = 0;
		if (!descending)
			pad ^= -1;
		if (itype == idx_numeric || itype == idx_timestamp1)
			length = sizeof(double);
		else if (itype == idx_sql_time)
			length = sizeof(ULONG);
		else if (itype == idx_sql_date)
			length = sizeof(SLONG);
		else if (itype == idx_timestamp2)
			length = sizeof(SINT64);
		else if (itype == idx_numeric2)
			length = INT64_KEY_LENGTH;
		else {
			length = desc->dsc_length;
			if (desc->dsc_dtype == dtype_varying)
				length -= sizeof(SSHORT);
			if (itype >= idx_first_intl_string)
				length = INTL_key_length(tdbb, itype, length);
		}
		length =
			(length > sizeof(key->key_data)) ? sizeof(key->key_data) : length;
		while (length--)
			*p++ = pad;
		key->key_length = p - key->key_data;
		return;
	}


	if (itype == idx_string ||
		itype == idx_byte_array ||
		itype == idx_metadata || itype >= idx_first_intl_string) {
		pad = (itype == idx_string) ? ' ' : 0;

		if (missing)
			length = 0;
		else if (itype >= idx_first_intl_string || itype == idx_metadata) {
			DSC to;

			/* convert to an international byte array */

			to.dsc_dtype = dtype_text;
			to.dsc_flags = 0;
			to.dsc_sub_type = 0;
			to.dsc_scale = 0;
			to.dsc_ttype = ttype_sort_key;
			to.dsc_length = sizeof(temp1);
			ptr = to.dsc_address = temp1;
			length = INTL_string_to_key(tdbb, itype, desc, &to, fuzzy);
		}
		else {
			USHORT ttype;
			length =
				MOV_get_string_ptr(desc, &ttype, &ptr, (VARY *) temp1,
								   MAX_KEY);
		}

		if (length) {
			if (length > sizeof(key->key_data)) {
				length = sizeof(key->key_data);
			}
			do {
				*p++ = *ptr++;
			} while (--length);
		}
		else
			*p++ = pad;
		while (p > key->key_data)
			if (*--p != pad)
				break;
		key->key_length = p + 1 - key->key_data;

		return;
	}

/* The index is numeric.  
   For idx_numeric...
       Convert the value to a double precision number, 
       then zap it to compare in a byte-wise order. 
   For idx_numeric2...
       Convert the value to a INT64_KEY struct,
       then zap it to compare in a byte-wise order. 
 */

	temp_copy_length = sizeof(double);
	if (missing)
		memset(&temp, 0, sizeof(temp));
	if (itype == idx_timestamp1) {
		temp.temp_double = MOV_date_to_double(desc);
		temp_is_negative = (temp.temp_double < 0);
#ifdef DEBUG_INDEXKEY
		ib_fprintf(ib_stderr, "TIMESTAMP1 %lf ", temp.temp_double);
#endif
	}
	else if (itype == idx_timestamp2) {
		GDS_TIMESTAMP timestamp;
		timestamp = MOV_get_timestamp(desc);
#define SECONDS_PER_DAY	((ULONG) 24 * 60 * 60)
		temp.temp_sint64 = ((SINT64) (timestamp.timestamp_date) *
							(SINT64) (SECONDS_PER_DAY *
									  ISC_TIME_SECONDS_PRECISION)) +
			(SINT64) (timestamp.timestamp_time);
		temp_copy_length = sizeof(SINT64);
		temp_is_negative = (temp.temp_sint64 < 0);
#ifdef DEBUG_INDEXKEY
		ib_fprintf(ib_stderr, "TIMESTAMP2: %d:%u ",
				   ((SLONG *) desc->dsc_address)[0],
				   ((ULONG *) desc->dsc_address)[1]);
		ib_fprintf(ib_stderr, "TIMESTAMP2: %20" QUADFORMAT "d ",
				   temp.temp_sint64);
#endif
	}
	else if (itype == idx_sql_date) {
		temp.temp_slong = MOV_get_sql_date(desc);
		temp_copy_length = sizeof(SLONG);
		temp_is_negative = (temp.temp_slong < 0);
#ifdef DEBUG_INDEXKEY
		ib_fprintf(ib_stderr, "DATE %d ", temp.temp_slong);
#endif
	}
	else if (itype == idx_sql_time) {
		temp.temp_ulong = MOV_get_sql_time(desc);
		temp_copy_length = sizeof(ULONG);
		temp_is_negative = FALSE;
#ifdef DEBUG_INDEXKEY
		ib_fprintf(ib_stderr, "TIME %u ", temp.temp_ulong);
#endif
	}
	else if (itype == idx_numeric2) {
		int64_key_op = TRUE;
		temp.temp_int64_key =
			make_int64_key(MOV_get_int64(desc, desc->dsc_scale),
						   desc->dsc_scale);
		temp_copy_length = sizeof(temp.temp_int64_key.d_part);
		temp_is_negative = (temp.temp_int64_key.d_part < 0);
#ifdef DEBUG_INDEXKEY
		print_int64_key(*(SINT64 *) desc->dsc_address,
						desc->dsc_scale, temp.temp_int64_key);
#endif
	}
	else if (desc->dsc_dtype == dtype_timestamp) {
		/* This is the same as the pre v6 behavior.  Basically, the
		   customer has created a NUMERIC index, and is probing into that
		   index using a TIMESTAMP value.  
		   eg:  WHERE anInteger = TIMESTAMP '1998-9-16' */
		temp.temp_double = MOV_date_to_double(desc);
		temp_is_negative = (temp.temp_double < 0);
#ifdef DEBUG_INDEXKEY
		ib_fprintf(ib_stderr, "TIMESTAMP1 special %lg ", temp.temp_double);
#endif
	}
	else {
		temp.temp_double = MOV_get_double(desc);
		temp_is_negative = (temp.temp_double < 0);
#ifdef DEBUG_INDEXKEY
		ib_fprintf(ib_stderr, "NUMERIC %lg ", temp.temp_double);
#endif
	}

#ifdef IEEE

#ifndef WORDS_BIGENDIAN
/* For little-endian machines, reverse the order of bytes for the key */
/* Copy the first set of bytes into key_data */
	for (q = temp.temp_char + temp_copy_length, length =
		 temp_copy_length; length; --length)
		*p++ = *--q;

/* Copy the next 2 bytes into key_data, if key is of an int64 type */
	if (int64_key_op == TRUE)
		for (q = temp.temp_char + sizeof(double) + sizeof(SSHORT),
			 length = sizeof(SSHORT); length; --length)
			*p++ = *--q;
#else
/* For big-endian machines, copy the bytes as laid down */
/* Copy the first set of bytes into key_data */
	for (q = temp.temp_char, length = temp_copy_length; length; --length)
		*p++ = *q++;

/* Copy the next 2 bytes into key_data, if key is of an int64 type */
	if (int64_key_op == TRUE)
		for (q = temp.temp_char + sizeof(double),
			 length = sizeof(SSHORT); length; --length)
			*p++ = *q++;
#endif /* !WORDS_BIGENDIAN */


#else /* IEEE */

/*
   The conversion from G_FLOAT to D_FLOAT made below was removed because
   it prevented users from entering otherwise valid numbers into a field
   which was in an index.   A D_FLOAT has the sign and 7 of 8 exponent
   bits in the first byte and the remaining exponent bit plus the first
   7 bits of the mantissa in the second byte.   For G_FLOATS, the sign
   and 7 of 11 exponent bits go into the first byte, with the remaining
   4 exponent bits going into the second byte, with the first 4 bits of
   the mantissa.   Why this conversion was done is unknown, but it is
   of limited utility, being useful for reducing the compressed field
   length only for those values which have 0 for the last 6 bytes and
   a nonzero value for the 5-7 bits of the mantissa.
*/

/****************************************************************
#ifdef VMS
temp.temp_double = MTH$CVT_G_D (&temp.temp_double);
#endif
 ****************************************************************/

	*p++ = temp.temp_char[1];
	*p++ = temp.temp_char[0];
	*p++ = temp.temp_char[3];
	*p++ = temp.temp_char[2];
	*p++ = temp.temp_char[5];
	*p++ = temp.temp_char[4];
	*p++ = temp.temp_char[7];
	*p++ = temp.temp_char[6];

#error compile_time_failure:
#error Code needs to be written in the non - IEEE floating point case
#error to handle the following:
#error 	a) idx_sql_date, idx_sql_time, idx_timestamp2 b) idx_numeric2
#endif /* IEEE */
/* Test the sign of the double precision number.  Just to be sure, don't
   rely on the byte comparison being signed.  If the number is negative,
   complement the whole thing.  Otherwise just zap the sign bit. */
	if (temp_is_negative) {

		((SSHORT *) key->key_data)[0] = -((SSHORT *) key->key_data)[0] - 1;
		((SSHORT *) key->key_data)[1] = -((SSHORT *) key->key_data)[1] - 1;
		((SSHORT *) key->key_data)[2] = -((SSHORT *) key->key_data)[2] - 1;
		((SSHORT *) key->key_data)[3] = -((SSHORT *) key->key_data)[3] - 1;
	}
	else
		key->key_data[0] ^= 1 << 7;

/* Complement the s_part for an int64 key.
 * If we just flip the sign bit, which is equivalent to adding 32768, the
 * short part will unsigned-compare correctly.
 */
	if (int64_key_op == TRUE) {
		key->key_data[8] ^= 1 << 7;
	}

/* Finally, chop off trailing binary zeros */

	for (p = &key->key_data[(int64_key_op == FALSE) ?
							temp_copy_length - 1 : INT64_KEY_LENGTH -
							1]; p > key->key_data; --p) {
		if (*p)
			break;
	}

	key->key_length = p - key->key_data + 1;
#ifdef DEBUG_INDEXKEY
	{
		USHORT i;
		ib_fprintf(ib_stderr, "KEY: length: %d Bytes: ", key->key_length);
		for (i = 0; i < key->key_length; i++)
			ib_fprintf(ib_stderr, "%02x ", key->key_data[i]);
		ib_fprintf(ib_stderr, "\n");
	}
#endif
}


static USHORT compress_root(TDBB tdbb, IRT page)
{
/**************************************
 *
 *	c o m p r e s s _ r o o t
 *
 **************************************
 *
 * Functional description
 *	Compress an index root page.
 *
 **************************************/
	DBB dbb;
	UCHAR *temp, *p;
	USHORT l;
	irt::irt_repeat * root_idx, *end;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	temp = (UCHAR *) tdbb->tdbb_default->allocate((SLONG) dbb->dbb_page_size, 0
#ifdef DEBUG_GDS_ALLOC
	  ,__FILE__,__LINE__
#endif
	);
	MOVE_FASTER(page, temp, dbb->dbb_page_size);
	p = temp + dbb->dbb_page_size;

	for (root_idx = page->irt_rpt, end = root_idx + page->irt_count;
		 root_idx < end; root_idx++)
		if (root_idx->irt_root) {
			l = root_idx->irt_keys * sizeof(IRTD);
			p -= l;
			MOVE_FAST((SCHAR *) page + root_idx->irt_desc, p, l);
			root_idx->irt_desc = p - temp;
		}
	l = p - temp;
	tdbb->tdbb_default->deallocate(temp);

	return l;
}


static USHORT compute_prefix(KEY * key, UCHAR * string, USHORT length)
{
/**************************************
 *
 *	c o m p u t e _ p r e f i x
 *
 **************************************
 *
 * Functional description
 *	Compute and return prefix common to two strings.
 *
 **************************************/
	UCHAR *p;
	USHORT l;

	if (!(l = MIN(key->key_length, length)))
		return 0;

	p = key->key_data;

	while (*p == *string) {
		p++;
		string++;
		if (!--l)
			break;
	}
	return p - key->key_data;
}


static void copy_key(KEY * in, KEY * out)
{
/**************************************
 *
 *	c o p y _ k e y
 *
 **************************************
 *
 * Functional description
 *	Copy a key.
 *
 **************************************/
	UCHAR *p, *q;
	USHORT l;

#ifdef IGNORE_NULL_IDX_KEY
	out->key_flags = in->key_flags;
#endif /* IGNORE_NULL_IDX_KEY */

	if ( (l = out->key_length = in->key_length) ) {
		p = out->key_data;
		q = in->key_data;
		do
			*p++ = *q++;
		while (--l);
	}
}


static CONTENTS delete_node(TDBB tdbb, WIN * window, BTN node)
{
/**************************************
 *
 *	d e l e t e _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Delete a node from a page and return whether it 
 *	empty, if there is a single node on it, or if it 
 * 	is above or below the threshold for garbage collection. 
 *
 **************************************/
	DBB dbb;
	BTN next;
	BTR page;
	USHORT l;
	UCHAR *p, *q;
	SLONG number;
	SLONG node_offset;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	page = (BTR) window->win_buffer;
	node_offset = (UCHAR *) node - (UCHAR *) page;

	CCH_MARK(tdbb, window);

/* move the rest of the page to the left to cover over this node */

	next = (BTN) (BTN_DATA(node) + BTN_LENGTH(node));
	QUAD_MOVE(BTN_NUMBER(next), BTN_NUMBER(node));

	p = BTN_DATA(node);
	q = BTN_DATA(next);
	l = BTN_LENGTH(next);
	if (BTN_PREFIX(node) < BTN_PREFIX(next)) {
		BTN_LENGTH(node) =
			BTN_LENGTH(next) + BTN_PREFIX(next) - BTN_PREFIX(node);
		p += BTN_PREFIX(next) - BTN_PREFIX(node);
	}
	else {
		page->btr_prefix_total -= BTN_PREFIX(node);
		assert(l <= MAX_UCHAR);
		BTN_LENGTH(node) = (UCHAR) l;
		BTN_PREFIX(node) = BTN_PREFIX(next);
	}

	if (l)
		do
			*p++ = *q++;
		while (--l);

/* Compute length of rest of bucket and move it down. */

	l = page->btr_length - (q - (UCHAR *) page);

	if (l) {
		/* Could be overlapping buffers. 
		   Use MEMMOVE macro which is memmove() in most platforms, instead 
		   of MOVE_FAST which is memcpy() in most platforms. 
		   memmove() is guaranteed to work non-destructivly on overlapping buffers. 
		 */
		MEMMOVE(q, p, l);
		p += l;
		q += l;
		l = 0;
	}

	page->btr_length = p - (UCHAR *) page;

/* Journal b-tree page - logical log of delete */

	if (dbb->dbb_wal) {
		JRNB journal;
		assert(node_offset <= MAX_USHORT);
		journal.jrnb_type = JRNP_BTREE_DELETE;
		journal.jrnb_prefix_total = page->btr_prefix_total;
		journal.jrnb_offset = (USHORT) node_offset;
		journal.jrnb_delta = BTN_PREFIX(node);	/* DEBUG ONLY */
		journal.jrnb_length = page->btr_length;	/* DEBUG ONLY */
		CCH_journal_record(tdbb, window, (UCHAR *) & journal,
						   JRNB_SIZE, 0, 0);
	}

/* check to see if the page is now empty */

#ifdef IGNORE_NULL_IDX_KEY
/* do not use 'node' here. It is being passed back to the caller */
	next = page->btr_nodes;
	number = BTR_get_quad(BTN_NUMBER(next));
	if (number == END_LEVEL || number == END_BUCKET)
#else
	node = page->btr_nodes;
	number = BTR_get_quad(BTN_NUMBER(node));
	if (number < 0)
#endif /* IGNORE_NULL_IDX_KEY */
		return contents_empty;

/* check to see if there is just one node */

#ifdef IGNORE_NULL_IDX_KEY
	next = NEXT_NODE(next);
	number = BTR_get_quad(BTN_NUMBER(next));
	if (number == END_LEVEL || number == END_BUCKET)
#else
	node = NEXT_NODE(node);
	number = BTR_get_quad(BTN_NUMBER(node));
	if (number < 0)
#endif /* IGNORE_NULL_IDX_KEY */
		return contents_single;

/* check to see if the size of the page is below the garbage collection threshold, 
   meaning below the size at which it should be merged with its left sibling if possible */

	if (page->btr_length < GARBAGE_COLLECTION_THRESHOLD)
		return contents_below_threshold;

	return contents_above_threshold;
}


static void delete_tree(TDBB tdbb,
						USHORT rel_id, USHORT idx_id, SLONG next, SLONG prior)
{
/**************************************
 *
 *	d e l e t e _ t r e e
 *
 **************************************
 *
 * Functional description
 *	Release index pages back to free list.
 *
 **************************************/
	BTR page;
	BTN node;
	SLONG down;
	WIN window;

	SET_TDBB(tdbb);
	window.win_flags = WIN_large_scan;
	window.win_scans = 1;

	down = next;

/* Delete the index tree from the top down. */

	while (next) {
		window.win_page = next;
		page = (BTR) CCH_FETCH(tdbb, &window, LCK_write, 0);

		/* do a little defensive programming--if any of these conditions 
		   are true we have a damaged pointer, so just stop deleting. At
		   the same time, allow updates of indexes with id > 255 even though
		   the page header uses a byte for its index id.  This requires relaxing
		   the check slightly introducing a risk that we'll pick up a page belonging
		   to some other index that is ours +/- (256*n).  On the whole, unlikely.*/

		if (page->btr_header.pag_type != pag_index ||
			page->btr_id != (UCHAR)(idx_id % 256) || page->btr_relation != rel_id) {
			CCH_RELEASE(tdbb, &window);
			return;
		}
		/* if we are at the beginning of a non-leaf level, position 
		   "down" to the beginning of the next level down */
		if (next == down)
			if (page->btr_level) {
				node = page->btr_nodes;
				down = BTR_get_quad(BTN_NUMBER(node));
			}
			else
				down = 0;

		/* go through all the sibling pages on this level and release them */

		next = page->btr_sibling;
		CCH_RELEASE_TAIL(tdbb, &window);
		PAG_release_page(window.win_page, prior);
		prior = window.win_page;

		/* if we are at end of level, go down to the next level */

		if (!next)
			next = down;
	}
}


static DSC *eval(TDBB tdbb, JRD_NOD node, DSC * temp, int *missing)
{
/**************************************
 *
 *	e v a l
 *
 **************************************
 *
 * Functional description
 *	Evaluate an expression returning a descriptor, and
 *	a flag to indicate a null value.  
 *
 **************************************/
	DSC *desc;

	SET_TDBB(tdbb);

	desc = EVL_expr(tdbb, node);
	*missing = FALSE;

	if (desc && !(tdbb->tdbb_request->req_flags & req_null))
		return desc;
	else
		*missing = TRUE;

	temp->dsc_dtype = dtype_text;
	temp->dsc_flags = 0;
	temp->dsc_sub_type = 0;
	temp->dsc_scale = 0;
	temp->dsc_length = 1;
	temp->dsc_ttype = ttype_ascii;
	temp->dsc_address = (UCHAR *) " ";

	return temp;
}


static SLONG fast_load(TDBB tdbb,
					   JRD_REL relation,
					   IDX * idx,
					   USHORT key_length, SCB sort_handle, float *selectivity)
{
/**************************************
 *
 *	f a s t _ l o a d
 *
 **************************************
 *
 * Functional description
 *	Do a fast load.  The indices have already been passed into sort, and
 *	are ripe for the plucking.  This beast is complicated, but, I hope,
 *	comprehendable.
 *
 **************************************/
	DBB dbb;
	ULONG count, duplicates, split_pages[MAX_LEVELS];
	USHORT level, prefix, i, l, lp_fill_limit, pp_fill_limit;
	BTR buckets[MAX_LEVELS], bucket, split;
	BTN nodes[MAX_LEVELS], node, split_node, next_node;
	WIN windows[MAX_LEVELS], *window, split_window;
	KEY keys[MAX_LEVELS], *key, split_key, temp_key;
	UCHAR *record, *p, *q;
	ISR isr;
	BOOLEAN error, duplicate;
#ifdef IGNORE_NULL_IDX_KEY
	BOOLEAN processed_first_null_idx_key = FALSE;
	BOOLEAN first_null_idx_key = FALSE;
#endif /* IGNORE_NULL_IDX_KEY */

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	count = duplicates = 0;
	buckets[1] = NULL;
#ifdef IGNORE_NULL_IDX_KEY
/* Define fill limits. Pointer page does not have END_NON_NULL marker. Hence 1
 * less BTN */
	lp_fill_limit = dbb->dbb_page_size - SPECIAL_BTN_NUMBER_COUNT * BTN_SIZE;
	pp_fill_limit = dbb->dbb_page_size - 2 * BTN_SIZE;
#else
	lp_fill_limit = pp_fill_limit = dbb->dbb_page_size - 2 * BTN_SIZE;
#endif /* IGNORE_NULL_IDX_KEY */
	keys[0].key_length = 0;

/* Allocate and format the first leaf level bucket.  Awkwardly,
   the bucket header has room for only a byte of index id and that's
   part of the ODS.  So, for now, we'll just record the first byte
   of the id and hope for the best.  Index buckets are (almost) always
   located through the index structure (dmp being an exception used 
   only for debug) so the id is actually redundant.*/

	bucket = buckets[0] = (BTR) DPM_allocate(tdbb, &windows[0]);
	bucket->btr_header.pag_type = pag_index;
	bucket->btr_relation = relation->rel_id;
	bucket->btr_id = (UCHAR)(idx->idx_id % 256);
	bucket->btr_level = 0;
	bucket->btr_length = OFFSETA(BTR, btr_nodes);
	if (idx->idx_flags & idx_descending)
		bucket->btr_header.pag_flags |= btr_descending;

	nodes[0] = bucket->btr_nodes;
	error = duplicate = FALSE;

	tdbb->tdbb_flags |= TDBB_no_cache_unwind;

	try {

/* If there's an error during index construction, fall
   thru to release the last index bucket at each level
   of the index. This will prepare for a single attempt
   to deallocate the index pages for reuse. */

	while (!error)
	{
		/* Get the next record in sorted order. */

		DEBUG;
		SORT_get(tdbb->tdbb_status_vector, sort_handle,
				 /* TMN: cast */ (ULONG **) & record
#ifdef SCROLLABLE_CURSORS
				 , RSE_get_forward
#endif
			);

		if (!record)
			break;
		isr = (ISR) (record + key_length);
		count++;

		bucket = buckets[0];
		node = nodes[0];
		split_pages[0] = 0;
		key = &keys[0];

#ifdef IGNORE_NULL_IDX_KEY
		/* Are we encountering the first index key to have 
		   its first segment for a NULL value? */
		if (SORTP_VAL_IS_NULL == *((SORTP *) record) &&
			processed_first_null_idx_key == FALSE) {
			processed_first_null_idx_key = TRUE;
			first_null_idx_key = TRUE;
		}
		/* skip the first longword which contains the BOOLEAN used for sorting
		 * NULLs higher than similar valid keys */
		record += sizeof(SORTP);
#endif /* IGNORE_NULL_IDX_KEY */

		/* Compute the prefix as the length in common with the previous record's key. */

#ifdef IGNORE_NULL_IDX_KEY
		/* do not compute prefix between the last NON-NULL and the first NULL
		 * nodes. Let them be independent of each other. This is because the
		 * END_NON_NULL marker is in between the two.
		 */
		if (first_null_idx_key == TRUE)
			prefix = 0;
		else
#endif /* IGNORE_NULL_IDX_KEY */
			prefix = compute_prefix(key, record, isr->isr_key_length);

		/* If the length of the new node will cause us to overflow the bucket, 
		   form a new bucket. */

		if (bucket->btr_length + isr->isr_key_length - prefix > lp_fill_limit) {
			split = (BTR) DPM_allocate(tdbb, &split_window);
			bucket->btr_sibling = split_window.win_page;
			split->btr_left_sibling = windows[0].win_page;
			split->btr_header.pag_type = pag_index;
			split->btr_relation = bucket->btr_relation;
			split->btr_level = bucket->btr_level;
			split->btr_id = bucket->btr_id;
			split->btr_header.pag_flags |=
				(bucket->btr_header.pag_flags & btr_descending);

			/* store the first node on the split page */

			split_node = split->btr_nodes;
			QUAD_MOVE(BTN_NUMBER(node), BTN_NUMBER(split_node));
			BTN_PREFIX(split_node) = 0;
			p = BTN_DATA(split_node);
			q = key->key_data;
			assert(key->key_length <= MAX_UCHAR);
				if ( (l = BTN_LENGTH(split_node) = (UCHAR) key->key_length) )
				do
					*p++ = *q++;
				while (--l);

			/* mark the end of the previous page */

			quad_put((SLONG) END_BUCKET, BTN_NUMBER(node));

			/* save the page number of the previous page and release it */

			split_pages[0] = windows[0].win_page;
			CCH_RELEASE(tdbb, &windows[0]);

			/* set up the new page as the "current" page */

			windows[0] = split_window;
			node = split_node;
			buckets[0] = bucket = split;

			/* save the first key on page as the page to be propogated */

			copy_key(key, &split_key);

			DEBUG;
		}

		if (bucket->btr_length != OFFSETA(BTR, btr_nodes))
			node = NEXT_NODE(node);

#ifdef IGNORE_NULL_IDX_KEY
		/* mark end of NON_NULL first segment, and update the length of the page */
		if (first_null_idx_key == TRUE) {
			first_null_idx_key = FALSE;
			BTN_PREFIX(node) = 0;
			BTN_LENGTH(node) = 0;
			quad_put((SLONG) END_NON_NULL, BTN_NUMBER(node));
			node = NEXT_NODE(node);
			bucket->btr_length = (UCHAR *) (node) - (UCHAR *) bucket;
			if (bucket->btr_length > dbb->dbb_page_size)
				BUGCHECK(205);	/* msg 205 index bucket overfilled */
		}
#endif /* IGNORE_NULL_IDX_KEY */

		/* Insert the new node in the now current bucket */

		assert(prefix <= MAX_UCHAR);
		BTN_PREFIX(node) = (UCHAR) prefix;
		bucket->btr_prefix_total += prefix;
		quad_put(isr->isr_record_number, BTN_NUMBER(node));
		p = BTN_DATA(node);
		q = record + prefix;
		if ( (l = BTN_LENGTH(node) = isr->isr_key_length - prefix) )
			do
				*p++ = *q++;
			while (--l);

		/* check if this is a duplicate node */

		duplicate = (!BTN_LENGTH(node) && prefix == key->key_length);
		if (duplicate)
			++duplicates;

		/* set this node as the current node, and update the length of the page */

		nodes[0] = node;
		next_node = NEXT_NODE(node);
		bucket->btr_length = (UCHAR *) (next_node) - (UCHAR *) bucket;
		if (bucket->btr_length > dbb->dbb_page_size)
			BUGCHECK(205);		/* msg 205 index bucket overfilled */

		/* Remember the last key inserted to compress the next one. */

		p = key->key_data;
		q = record;
		if ( (l = key->key_length = isr->isr_key_length) )
			do
				*p++ = *q++;
			while (--l);

		/* If there wasn't a split, we're done.  If there was, propogate the
		   split upward */

		for (level = 1; split_pages[level - 1]; level++) {
			DEBUG;
			/* initialize the current pointers for this level */
			window = &windows[level];
			key = &keys[level];
			split_pages[level] = 0;
			node = nodes[level];

		/* If there isn't already a bucket at this level, make one.  Remember to 
		   shorten the index id to a byte */

			if (!(bucket = buckets[level])) {
				buckets[level + 1] = NULL;
				buckets[level] = bucket = (BTR) DPM_allocate(tdbb, window);
				bucket->btr_header.pag_type = pag_index;
				bucket->btr_relation = relation->rel_id;
				bucket->btr_id = (UCHAR)(idx->idx_id % 256);
				assert(level <= MAX_UCHAR);
					bucket->btr_level = (UCHAR) level;
				if (idx->idx_flags & idx_descending)
					bucket->btr_header.pag_flags |= btr_descending;
				bucket->btr_length = OFFSETA(BTR, btr_nodes) + BTN_SIZE;

				/* since this is the beginning of the level, we propogate the lower-level 
				   page with a "degenerate" zero-length node indicating that this page holds 
				   any key value less than the next node */

				node = bucket->btr_nodes;
				BTN_LENGTH(node) = BTN_PREFIX(node) = 0;
				quad_put(split_pages[level - 1], BTN_NUMBER(node));
				key->key_length = 0;
			}

			/* Compute the prefix in preparation of insertion */

			prefix =
				compute_prefix(key, split_key.key_data, split_key.key_length);

			/* Remember the last key inserted to compress the next one. */

			copy_key(&split_key, &temp_key);

			/* See if the new node fits in the current bucket.  If not, split
			   the bucket. */

			if (bucket->btr_length + temp_key.key_length - prefix >
				pp_fill_limit) {
				split = (BTR) DPM_allocate(tdbb, &split_window);
				bucket->btr_sibling = split_window.win_page;
				split->btr_left_sibling = window->win_page;
				split->btr_header.pag_type = pag_index;
				split->btr_relation = bucket->btr_relation;
				split->btr_level = bucket->btr_level;
				split->btr_id = bucket->btr_id;
				split->btr_header.pag_flags |=
					(bucket->btr_header.pag_flags & btr_descending);
				split_node = split->btr_nodes;

				/* insert the new node in the new bucket */

				QUAD_MOVE(BTN_NUMBER(node), BTN_NUMBER(split_node));
				BTN_PREFIX(split_node) = 0;
				p = BTN_DATA(split_node);
				q = key->key_data;
				assert(key->key_length <= MAX_UCHAR);
					if ( (l = BTN_LENGTH(split_node) = (UCHAR) key->key_length) )
					do
						MOVE_BYTE(q, p);
					while (--l);

				/* mark the end of the page; note that the end_bucket marker must 
				   contain info about the first node on the next page */

				quad_put((SLONG) END_BUCKET, BTN_NUMBER(node));

				/* indicate to propogate the page we just split from */

				split_pages[level] = window->win_page;
				CCH_RELEASE(tdbb, window);

				/* and make the new page the current page */

				*window = split_window;
				node = split_node;
				buckets[level] = bucket = split;
				copy_key(key, &split_key);

				DEBUG;
			}

			/* Now propogate up the lower-level bucket by storing a "pointer" to it. */

			node = NEXT_NODE(node);
			assert(prefix <= MAX_UCHAR);
			BTN_PREFIX(node) = (UCHAR) prefix;
			bucket->btr_prefix_total += prefix;
			quad_put(windows[level - 1].win_page, BTN_NUMBER(node));

			/* Store the key associated with the page as the first unique key value 
			   on the page. */

			p = BTN_DATA(node);
			q = temp_key.key_data + prefix;
			if ( (l = BTN_LENGTH(node) = temp_key.key_length - prefix) )
				do
					MOVE_BYTE(q, p);
				while (--l);

			/* Now restore the current key value and save this node as the current 
			   node on this level; also calculate the new page length. */

			copy_key(&temp_key, key);
			nodes[level] = node;
			next_node = NEXT_NODE(node);
			bucket->btr_length = (UCHAR *) next_node - (UCHAR *) bucket;
			if (bucket->btr_length > dbb->dbb_page_size)
				BUGCHECK(205);	/* msg 205 index bucket overfilled */

			DEBUG;
		}

#ifdef SUPERSERVER
		if (--tdbb->tdbb_quantum < 0 && !tdbb->tdbb_inhibit)
			error = JRD_reschedule(tdbb, 0, FALSE);
#endif
	}

/* To finish up, put an end of level marker on the last bucket 
   of each level. */

	DEBUG;
	for (i = 0; (bucket = buckets[i]); i++) {
		/* retain the top level window for returning to the calling routine */

		window = &windows[i];

#ifdef IGNORE_NULL_IDX_KEY
		/* For level 0 (leaf pages), insert an END_NON_NULL marker before the 
		   END_LEVEL marker, if we have not as yet processed any 
		   first-segment NULL record. */

		if (i == 0 && processed_first_null_idx_key == FALSE) {
			processed_first_null_idx_key = TRUE;
			/* There does not seem to be any END_BUCKET in the last page.
			   * Naturally, the bucket is not full, right? */
			/* Insert END_NON_NULL at that point */
			node = LAST_NODE(bucket);
			BTN_LENGTH(node) = BTN_PREFIX(node) = 0;
			quad_put((SLONG) END_NON_NULL, BTN_NUMBER(node));
			bucket->btr_length += BTN_SIZE;
		}
#endif /* IGNORE_NULL_IDX_KEY */

		/* store the end of level marker */

		node = LAST_NODE(bucket);
		BTN_LENGTH(node) = BTN_PREFIX(node) = 0;
		quad_put((SLONG) END_LEVEL, BTN_NUMBER(node));

		/* and update the final page length */

		bucket->btr_length += BTN_SIZE;
		if (bucket->btr_length > dbb->dbb_page_size)
			BUGCHECK(205);		/* msg 205 index bucket overfilled */

		CCH_RELEASE(tdbb, &windows[i]);
	}
	DEBUG;
	tdbb->tdbb_flags &= ~TDBB_no_cache_unwind;

/* do some final housekeeping */

	SORT_fini(sort_handle, tdbb->tdbb_attachment);

	}	// try
	catch (const std::exception&) {
		error = TRUE;
	}


/* If index flush fails, try to delete the index tree.
   If the index delete fails, just go ahead and punt. */

	try {

	if (error) {
		delete_tree(tdbb, relation->rel_id, idx->idx_id, window->win_page, 0);
		ERR_punt();
	}

	CCH_flush(tdbb, (USHORT) FLUSH_ALL, 0);
	*selectivity = (float) ((count)
							? (1. / (double) (count - duplicates))
							: 0);

	return window->win_page;

	}	// try
	catch(const std::exception&) {
		if (!error)
			error = TRUE;
		else {
			ERR_punt();
		}
	}

	return -1L; /* lint */
}


static IRT fetch_root(TDBB tdbb, WIN * window, JRD_REL relation)
{
/**************************************
 *
 *	f e t c h _ r o o t
 *
 **************************************
 *
 * Functional description
 *	Return descriptions of all indices for relation.  If there isn't
 *	a known index root, assume we were called during optimization
 *	and return no indices.
 *
 **************************************/
	SET_TDBB(tdbb);

	if ((window->win_page = relation->rel_index_root) == 0)
		if (relation->rel_id == 0)
			return NULL;
		else {
			DPM_scan_pages(tdbb);
			window->win_page = relation->rel_index_root;
		}
	return (IRT) CCH_FETCH(tdbb, window, LCK_read, pag_root);
}


static BTN find_node(BTR bucket, KEY * key, USHORT descending)
{
/**************************************
 *
 *	f i n d _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Find a node in an index level.  Return either the 
 *	node equal to the key or the last node less than the key.
 *	Note that this routine can be called only for non-leaf 
 *	pages, because it assumes the first node on page is 
 *	a degenerate, zero-length node.
 *
 **************************************/
	BTN node;
	BTN prior;
	UCHAR prefix, *key_end, *node_end;
	UCHAR *p, *q;
	SLONG number;

	DEBUG;
#ifdef IGNORE_NULL_IDX_KEY
	assert(bucket->btr_level != 0);
#endif /* IGNORE_NULL_IDX_KEY */
	node = bucket->btr_nodes;

/* Compute common prefix of key and first node */

/* TMN: Watch out, possibility for UCHAR overflow! */
	prefix = (UCHAR) compute_prefix(key, BTN_DATA(node), BTN_LENGTH(node));
	p = key->key_data + prefix;
	key_end = key->key_data + key->key_length;
	number = BTR_get_quad(BTN_NUMBER(node));

	if (number == END_LEVEL)
		BUGCHECK(206);			/* msg 206 exceeded index level */

	if (key->key_length == 0)
		return node;

	while (TRUE) {
		/* If this is the end of bucket, return node.  Somebody else can
		   deal with this */

		if (number == END_BUCKET)
			return node;

		prior = node;
		node = NEXT_NODE(node);
		number = BTR_get_quad(BTN_NUMBER(node));

		/* If the page/record number is -1, the node is the last in the level
		   and, by definition, is the target node.  Otherwise, if the
		   prefix of the current node is less than the running prefix, its
		   node must have a value greater than the key, which is the insertion
		   point. */

		if (number == END_LEVEL || BTN_PREFIX(node) < prefix)
			return prior;

		/* If the node prefix is greater than current prefix , it must be less 
		   than the key, so we can skip it.  If it has zero length, then
		   it is a duplicate, and can also be skipped. */

		q = BTN_DATA(node);
		node_end = q + BTN_LENGTH(node);
		if (descending) {
			if (BTN_PREFIX(node) == prefix) {
				while (TRUE)
					if (q == node_end || p == key_end)
						return prior;
					else if (*p > *q)
						break;
					else if (*p++ < *q++)
						return prior;
			}
		}
		else if (BTN_PREFIX(node) == prefix && BTN_LENGTH(node) > 0)
			while (TRUE)
				if (p == key_end)
					return prior;
				else if (q == node_end || *p > *q)
					break;
				else if (*p++ < *q++)
					return prior;

		prefix = p - key->key_data;
	}
/* NOTREACHED */
	return NULL;				/* superfluous return to shut lint up */
}


static CONTENTS garbage_collect(TDBB tdbb, WIN * window, SLONG parent_number)
{
/**************************************
 *
 *	g a r b a g e _ c o l l e c t
 *
 **************************************
 *
 * Functional description
 *	Garbage collect an index page.  This requires 
 * 	care so that we don't step on other processes 
 * 	that might be traversing the tree forwards, 
 *	backwards, or top to bottom.  We must also 
 *	keep in mind that someone might be adding a node 
 *	at the same time we are deleting.  Therefore we 
 *	must lock all the pages involved to prevent 
 *	such operations while we are garbage collecting.
 *
 **************************************/
	DBB dbb;
	WIN parent_window, left_window, right_window;
	BTR gc_page, parent_page, left_page, right_page = NULL;
	BTN node, parent_node, last_node;
	SLONG number, left_number;
#ifdef DEBUG_BTR
	SLONG previous_number;
#endif
	USHORT relation_number, l;
	UCHAR index_id, index_level, prefix;
	CONTENTS result = contents_above_threshold;
	KEY last_key;
	UCHAR *p, *q;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	gc_page = (BTR) window->win_buffer;

/* check to see if the page was marked not to be garbage collected */

	if (gc_page->btr_header.pag_flags & btr_dont_gc) {
		CCH_RELEASE(tdbb, window);
		return contents_above_threshold;
	}
/* record the left sibling now since this is the only way to 
   get to it quickly; don't worry if it's not accurate now or 
   is changed after we release the page, since we will fetch 
   it in a fault-tolerant way anyway */
	left_number = gc_page->btr_left_sibling;

/* if the left sibling is blank, that indicates we are the leftmost page, 
   so don't garbage-collect the page; do this for several reasons:
   1.  The leftmost page needs a degenerate zero length node as its first node 
       (for a non-leaf, non-top-level page).
   2.  The parent page would need to be fixed up to have a degenerate node 
       pointing to the right sibling. 
   3.  If we remove all pages on the level, we would need to re-add it next 
       time a record is inserted, so why constantly garbage-collect and re-create 
       this page? */

	if (!left_number) {
		CCH_RELEASE(tdbb, window);
		return contents_above_threshold;
	}

/* record some facts for later validation */

	relation_number = gc_page->btr_relation;
	index_id = gc_page->btr_id;
	index_level = gc_page->btr_level;

/* we must release the page we are attempting to garbage collect; 
   this is necessary to avoid deadlocks when we fetch the parent page */

	CCH_RELEASE(tdbb, window);

/* fetch the parent page, but we have to be careful, because it could have 
   been garbage-collected when we released it--make checks so that we know it 
   is the parent page; there is a minute possibility that it could have been 
   released and reused already as another page on this level, but if so, it 
   won't really matter because we won't find the node on it */

	parent_window.win_page = parent_number;
	parent_window.win_flags = 0;
	parent_page =
		(BTR) CCH_FETCH(tdbb, &parent_window, LCK_write, pag_undefined);
	if ((parent_page->btr_header.pag_type != pag_index)
		|| (parent_page->btr_relation != relation_number)
		|| (parent_page->btr_id != (UCHAR)(index_id % 256))
		|| (parent_page->btr_level != index_level + 1)) 
	{
		CCH_RELEASE(tdbb, &parent_window);
		return contents_above_threshold;
	}

/* find the left sibling page by going one page to the left, 
   but if it does not recognize us as its right sibling, keep 
   going to the right until we find the page that is our real 
   left sibling */

	left_window.win_page = left_number;
	left_window.win_flags = 0;
	left_page = (BTR) CCH_FETCH(tdbb, &left_window, LCK_write, pag_index);
	while (left_page->btr_sibling != window->win_page) {
#ifdef DEBUG_BTR
		CCH_RELEASE(tdbb, &parent_window);
		CCH_RELEASE(tdbb, &left_window);
		CORRUPT(204);			/* msg 204 index inconsistent */
#endif
		/* If someone garbage collects the index page before we can, it
		   won't be found by traversing the right sibling chain. This means
		   scanning index pages until the end-of-level bucket is hit. */

		if (!left_page->btr_sibling) {
			CCH_RELEASE(tdbb, &parent_window);
			CCH_RELEASE(tdbb, &left_window);
			return contents_above_threshold;
		}
		left_page =
			(BTR) CCH_HANDOFF(tdbb,
							  &left_window,
							  left_page->btr_sibling, LCK_write, pag_index);
	}

/* now refetch the original page and make sure it is still 
   below the threshold for garbage collection. */

	gc_page = (BTR) CCH_FETCH(tdbb, window, LCK_write, pag_index);
	if ((gc_page->btr_length >= GARBAGE_COLLECTION_THRESHOLD)
		|| (gc_page->btr_header.pag_flags & btr_dont_gc)) {
		CCH_RELEASE(tdbb, &parent_window);
		CCH_RELEASE(tdbb, &left_window);
		CCH_RELEASE(tdbb, window);
		return contents_above_threshold;
	}

/* fetch the right sibling page */

	if ( (right_window.win_page = gc_page->btr_sibling) ) {
		right_window.win_flags = 0;
		right_page =
			(BTR) CCH_FETCH(tdbb, &right_window, LCK_write, pag_index);
		if (right_page->btr_left_sibling != window->win_page) {
			CCH_RELEASE(tdbb, &parent_window);
			if (left_page)
				CCH_RELEASE(tdbb, &left_window);
			CCH_RELEASE(tdbb, window);
			CCH_RELEASE(tdbb, &right_window);
#ifdef DEBUG_BTR
			CORRUPT(204);		/* msg 204 index inconsistent */
#endif
			return contents_above_threshold;
		}
	}

/* Find the node on the parent's level--the parent page could 
   have split while we didn't have it locked */

#ifdef DEBUG_BTR
	previous_number = 0;
#endif
	for (parent_node = parent_page->btr_nodes;;) {
		number = BTR_get_quad(BTN_NUMBER(parent_node));

		if (number == END_BUCKET) {
			parent_page =
				(BTR) CCH_HANDOFF(tdbb,
								  &parent_window,
								  parent_page->btr_sibling,
								  LCK_write, pag_index);
			parent_node = parent_page->btr_nodes;
			continue;
		}

		if (number == window->win_page || number == END_LEVEL)
			break;

#ifdef DEBUG_BTR
		previous_number = number;
#endif
		parent_node = NEXT_NODE(parent_node);
	}

/* we should always find the node, but just in case we don't, bow out gracefully */

	if (number == END_LEVEL) {
		CCH_RELEASE(tdbb, &left_window);
		if (right_page)
			CCH_RELEASE(tdbb, &right_window);
		CCH_RELEASE(tdbb, &parent_window);
		CCH_RELEASE(tdbb, window);
#ifdef DEBUG_BTR
		CORRUPT(204);			/* msg 204 index inconsistent */
#endif
		return contents_above_threshold;
	}

/* Fix for ARINC database corruption bug: in most cases we update the END_BUCKET 
   marker of the left sibling page to contain the END_BUCKET of the garbage-collected 
   page.  However, when this page is the first page on its parent, then the left 
   sibling page is the last page on its parent.  That means if we update its END_BUCKET 
   marker, its bucket of values will extend past that of its parent, causing trouble 
   down the line.  
   
   So we never garbage-collect a page which is the first one on its parent.  This page 
   will have to wait until the parent page gets collapsed with the page to its left, 
   in which case this page itself will then be garbage-collectable.  Since there are 
   no more keys on this page, it will not be garbage-collected itself.  When the page 
   to the right falls below the threshold for garbage collection, it will be merged with 
   this page. */

	if (parent_node == parent_page->btr_nodes) {
		CCH_RELEASE(tdbb, &left_window);
		if (right_page)
			CCH_RELEASE(tdbb, &right_window);
		CCH_RELEASE(tdbb, &parent_window);
		CCH_RELEASE(tdbb, window);
		return contents_above_threshold;
	}

/* find the last node on the left sibling and save its key value */

	p = last_key.key_data;
#ifdef IGNORE_NULL_IDX_KEY
	for (last_node =
		 left_page->btr_nodes, number =
		 BTR_get_quad(BTN_NUMBER(last_node)); (number != END_LEVEL)
		 && (number != END_BUCKET);
		 last_node =
		 NEXT_NODE(last_node), number = BTR_get_quad(BTN_NUMBER(last_node))) {
		if (number == END_NON_NULL) {
			/* this will help in negotiating for enough space later 
			   This will make key_length calculation = 0; */
			p = last_key.key_data;
			continue;
		}
		if (l = BTN_LENGTH(last_node)) {
			p = last_key.key_data + BTN_PREFIX(last_node);
			q = BTN_DATA(last_node);
			do
				*p++ = *q++;
			while (--l);
		}
	}
	assert(number != END_NON_NULL);
#else
	for (last_node = left_page->btr_nodes;
		 (number = BTR_get_quad(BTN_NUMBER(last_node))
		 >= 0); last_node = NEXT_NODE(last_node))
		if ( (l = BTN_LENGTH(last_node)) ) {
			p = last_key.key_data + BTN_PREFIX(last_node);
			q = BTN_DATA(last_node);
			do
				*p++ = *q++;
			while (--l);
		}
#endif /* IGNORE_NULL_IDX_KEY */
	last_key.key_length = p - last_key.key_data;

/* see if there's enough space on the left page to move all the nodes to it
   and leave some extra space for expansion (at least one key length) */

	node = gc_page->btr_nodes;

/* TMN: Watch out, possibility for UCHAR overflow! */
	prefix =
		(UCHAR) compute_prefix(&last_key, BTN_DATA(node), BTN_LENGTH(node));

	if (left_page->btr_length +
		gc_page->btr_length - prefix -
		BTN_LENGTH(last_node) - BTN_SIZE -
		((UCHAR *) gc_page->btr_nodes -
		 (UCHAR *) gc_page) > dbb->dbb_page_size - MAX_KEY) {
		CCH_RELEASE(tdbb, &parent_window);
		CCH_RELEASE(tdbb, &left_window);
		CCH_RELEASE(tdbb, window);
		if (right_page)
			CCH_RELEASE(tdbb, &right_window);
		return contents_above_threshold;
	}

#ifdef DEBUG_BTR
	{
		SLONG next_number;
		BTN next_parent_node;
/* do a consistency check to be sure that the parent page has the proper 
   nodes to the left and to the right--this assumes single-user, because 
   it's possible that leaf pages in a duplicate chain could be out of 
   order when two different processes split pages at the same time */

		next_parent_node = NEXT_NODE(parent_node);
		next_number = BTR_get_quad(BTN_NUMBER(next_parent_node));

		if (
			(left_page && previous_number
			 && (previous_number !=
				 left_window.win_page)) || (right_page && (next_number > 0)
											&& (next_number !=
												right_window.win_page))) {
			CCH_RELEASE(tdbb, &parent_window);
			CCH_RELEASE(tdbb, &left_window);
			CCH_RELEASE(tdbb, window);
			if (right_page)
				CCH_RELEASE(tdbb, &right_window);
			CORRUPT(204);		/* msg 204 index inconsistent */
			return contents_above_threshold;
		}
	}
#endif

/* Now begin updating the pages.  We must write them out in such 
   a way as to maintain on-disk integrity at all times.  That means 
   not having pointers to released pages, and not leaving things in 
   an inconsistent state for navigation through the pages. */

/* Update the parent first.  If the parent is not written out first, 
   we will be pointing to a page which is not in the doubly linked 
   sibling list, and therefore navigation back and forth won't work. */

	result = delete_node(tdbb, &parent_window, parent_node);
	CCH_RELEASE(tdbb, &parent_window);

/* Update the right sibling page next, since it does not really 
   matter that the left sibling pointer points to the page directly 
   to the left, only that it point to some page to the left.
   Set up the precedence so that the parent will be written first. */

	if (right_page) {
		if (parent_page)
			CCH_precedence(tdbb, &right_window, parent_window.win_page);
		CCH_MARK(tdbb, &right_window);
		right_page->btr_left_sibling = left_window.win_page;

		if (dbb->dbb_journal)
			CCH_journal_page(tdbb, &right_window);
		CCH_RELEASE(tdbb, &right_window);
	}

/* Now update the left sibling, effectively removing the garbage-collected page 
   from the tree.  Set the precedence so the right sibling will be written first. */

	if (right_page)
		CCH_precedence(tdbb, &left_window, right_window.win_page);
	else if (parent_page)
		CCH_precedence(tdbb, &left_window, parent_window.win_page);

	CCH_MARK(tdbb, &left_window);

	if (right_page)
		left_page->btr_sibling = right_window.win_page;
	else
		left_page->btr_sibling = 0;

/* move all the nodes from the garbage-collected page to the left sibling, 
   overwriting the END_BUCKET of the left sibling */

	node = gc_page->btr_nodes;

/* calculate the total amount of compression on page as the combined totals 
   of the two pages, plus the compression of the first node on the g-c'ed page,
   minus the prefix of the END_BUCKET node to be deleted */

	left_page->btr_prefix_total +=
		gc_page->btr_prefix_total + prefix - BTN_PREFIX(last_node);

/* fix up the last node of the left page to contain the compressed first node */

	BTN_PREFIX(last_node) = prefix;
	BTN_LENGTH(last_node) = BTN_LENGTH(node) - prefix;
	p = BTN_NUMBER(last_node);
	q = BTN_NUMBER(node);
	l = 4;
	do
		*p++ = *q++;
	while (--l);

/* copy over the remainder of the page to be garbage-collected */

	p = BTN_DATA(last_node);
	q = BTN_DATA(node) + prefix;
	l = gc_page->btr_length - (q - (UCHAR *)
							   gc_page);

	if (l)
		do
			*p++ = *q++;
		while (--l);

	left_page->btr_length = p - (UCHAR *) left_page;

#ifdef DEBUG_BTR
	if (left_page->btr_length > dbb->dbb_page_size) {
		CCH_RELEASE(tdbb, &left_window);
		CCH_RELEASE(tdbb, window);
		CORRUPT(204);			/* msg 204 index inconsistent */
		return contents_above_threshold;
	}
#endif

	if (dbb->dbb_journal)
		CCH_journal_page(tdbb, &left_window);
	CCH_RELEASE(tdbb, &left_window);

/* finally, release the page, and indicate that we should write the 
   previous page out before we write the TIP page out */

	CCH_RELEASE(tdbb, window);
	PAG_release_page(window->win_page,
					 left_page ?
					 left_window.win_page :
					 right_page ?
					 right_window.win_page : parent_window.win_page);

/* if the parent page needs to be garbage collected, that means we need to 
   re-fetch the parent and check to see whether it is still garbage-collectable; 
   make sure that the page is still a btree page in this index and in this level--
   there is a miniscule chance that it was already reallocated as another page 
   on this level which is already below the threshold, in which case it doesn't 
   hurt anything to garbage-collect it anyway */

	if (result != contents_above_threshold) {
		window->win_page = parent_window.win_page;
		parent_page = (BTR) CCH_FETCH(tdbb, window, LCK_write, pag_undefined);

		if ((parent_page->btr_header.pag_type != pag_index)
			|| (parent_page->btr_relation != relation_number)
			|| (parent_page->btr_id != index_id)
			|| (parent_page->btr_level != index_level + 1)) {
			CCH_RELEASE(tdbb, window);
			return contents_above_threshold;
		}

		/* check whether it is empty */

		parent_node = parent_page->btr_nodes;
		number = BTR_get_quad(BTN_NUMBER(parent_node));
#ifdef IGNORE_NULL_IDX_KEY
		if (number == END_LEVEL || number == END_BUCKET)
#else
		if (number < 0)
#endif /* IGNORE_NULL_IDX_KEY */
			return contents_empty;

		/* check whether there is just one node */

		parent_node = NEXT_NODE(parent_node);
		number = BTR_get_quad(BTN_NUMBER(parent_node));
#ifdef IGNORE_NULL_IDX_KEY
		if (number == END_LEVEL || number == END_BUCKET)
#else
		if (number < 0)
#endif /* IGNORE_NULL_IDX_KEY */
			return contents_single;

		/* check to see if the size of the page is below the garbage collection threshold */

		if (parent_page->btr_length < GARBAGE_COLLECTION_THRESHOLD)
			return contents_below_threshold;

		/* the page must have risen above the threshold; release the window since 
		   someone else added a node while the page was released */

		CCH_RELEASE(tdbb, window);
		return contents_above_threshold;
	}

	return result;
}


static SLONG insert_node(TDBB tdbb,
						 WIN * window,
						 IIB * insertion,
						 KEY * new_key,
						 SLONG * original_page, SLONG * sibling_page)
{
/**************************************
 *
 *	i n s e r t _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Insert a node in a bucket.  If this isn't the right bucket, 
 *	return -1.  If it splits, return the split page number and 
 *	leading string.  This is the workhorse for add_node.
 *
 **************************************/
	DBB dbb;
	KEY *key;
	BTR bucket, split;
	WIN split_window;
	BTN node, new_node, next_node;
	UCHAR prefix, old_prefix, old_length;
	USHORT delta, l, node_offset;
	SLONG old_number, split_page, right_sibling;
	SLONG overflow_page[OVERSIZE];
	UCHAR *p, *q;
	UCHAR *midpoint;
	SLONG prefix_total;
	JRNB journal;
	BOOLEAN end_of_page;
#ifdef IGNORE_NULL_IDX_KEY
	BOOLEAN midpoint_is_end_non_null = FALSE;
#endif /* IGNORE_NULL_IDX_KEY */

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	DEBUG;
/* find the insertion point for the specified key */
	bucket = (BTR) window->win_buffer;
	key = insertion->iib_key;

	if (!
		(node =
		 BTR_find_leaf(bucket, key, 0,
					   &prefix,
					   insertion->iib_descriptor->idx_flags &
					   idx_descending, FALSE))) return -1;

/* loop through the equivalent nodes until the correct insertion 
   point is found; for leaf level this will be the first node */

	for (;;) {
		node_offset = (UCHAR *) node - (UCHAR *) bucket;

		old_number = BTR_get_quad(BTN_NUMBER(node));
		old_prefix = BTN_PREFIX(node);
		old_length = BTN_LENGTH(node);

#ifdef IGNORE_NULL_IDX_KEY
		if (old_number == END_BUCKET)
			return -1;
		if (old_number == END_LEVEL)
			break;
		if (old_number == END_NON_NULL) {
			assert(bucket->btr_level == 0);
			break;
		}
#endif /* IGNORE_NULL_IDX_KEY */
		p = key->key_data + old_prefix;
		q = BTN_DATA(node);
		l = MIN(key->key_length - old_prefix, old_length);
		if (l)
			do {
				if (*p++ != *q++)
					break;
				--old_length;
				old_prefix++;
			} while (--l);

		/* check if the inserted node has the same value as the next node */

		if (old_prefix != key->key_length ||
			old_prefix != BTN_LENGTH(node) + BTN_PREFIX(node))
			break;

#ifndef IGNORE_NULL_IDX_KEY
		/* This block of code moved up for IGNORE_NULL_IDX_KEY */
		if (old_number == END_BUCKET)
			return -1;
		if (old_number == END_LEVEL)
			break;
#endif /* IGNORE_NULL_IDX_KEY */

		/* if this is a non-leaf page, we need to find 
		   the correct insertion point in the duplicate chain */
		if (!bucket->btr_level)
			break;
		if (old_number == insertion->iib_sibling)
			break;

		/* since the node is equivalent and we are about to skip past it, 
		   the prefix of the inserted node is now the same */

		prefix = old_prefix;

		while (old_number != insertion->iib_sibling) {
			node = NEXT_NODE(node);
			old_number = BTR_get_quad(BTN_NUMBER(node));

			if (BTN_LENGTH(node))
				break;
			if (old_number == END_BUCKET)
				return -1;
			if (old_number == END_LEVEL)
				break;
		}
	}

/* Compute the length of the updated page.  This is a function of the
   new string length minus prefix and recompression done to the string
   following the insertion. */

	delta =
		BTN_SIZE + key->key_length - prefix + BTN_PREFIX(node) - old_prefix;

/* Prepare to slide down tail of bucket.  If we're going to split,
   move the initialized hunk of the bucket to an overflow area big
   enough to hold the split.  If the bucket isn't going to split,
   mark the buffer as dirty. */

	if (bucket->btr_length + delta > dbb->dbb_page_size) {
		MOVE_FASTER(bucket, overflow_page, bucket->btr_length);
		node = (BTN) ((UCHAR *) overflow_page + node_offset);
		bucket = (BTR) overflow_page;
	}
	else {
		/* if we are a pointer page, make sure that the page we are 
		   pointing to gets written before we do for on-disk integrity */

		if (bucket->btr_level != 0)
			CCH_precedence(tdbb, window, insertion->iib_number);
		CCH_MARK(tdbb, window);
	}

	new_node = node;

/* Slide down the upper hunk of the bucket to make room for the
   insertion.  */

	l = bucket->btr_length - node_offset;
	p = (UCHAR *) bucket + bucket->btr_length;
	q = p + delta;
	do
		*--q = *--p;
	while (--l);

/* Insert the new node.  */

	bucket->btr_length += delta;
	bucket->btr_prefix_total += prefix - BTN_PREFIX(node);
	BTN_PREFIX(node) = prefix;
	quad_put(insertion->iib_number, BTN_NUMBER(node));
	p = BTN_DATA(node);
	q = key->key_data + prefix;
	if ( (l = BTN_LENGTH(node) = key->key_length - prefix) ) {
		do
			MOVE_BYTE(q, p);
		while (--l);
	}

/* Recompress and rebuild the next node. */

	node = (BTN) p;
	bucket->btr_prefix_total += old_prefix;
	BTN_PREFIX(node) = old_prefix;
	BTN_LENGTH(node) = old_length;
	quad_put(old_number, BTN_NUMBER(node));
/* We don't need to rebuild BTN_DATA of first pushed node here because,
 * if old_prefix has increased we only move down part of the node anyway */

/* figure out whether this node was inserted at the end of the page */

#ifdef IGNORE_NULL_IDX_KEY
/* A END_NON_NULL marker does not signify end_of_page. There could be more
 * valid BTN nodes after the END_NON_NULL in the same page */
	end_of_page =
		(old_number == END_BUCKET || old_number == END_LEVEL) ? TRUE : FALSE;
#else
	end_of_page = (old_number < 0) ? TRUE : FALSE;
#endif /* IGNORE_NULL_IDX_KEY */

/* If the index is unique, look for duplicates in this bucket. */

	if (insertion->iib_descriptor->idx_flags & idx_unique)
		while (BTN_LENGTH(node) == 0 && BTN_PREFIX(node) == key->key_length) {
			old_number = BTR_get_quad(BTN_NUMBER(node));
			if (old_number < 0)
				break;
			SBM_set(tdbb, &insertion->iib_duplicates, old_number);
			node = (BTN) BTN_DATA(node);
		}

/* If the bucket still fits on a page, we're almost done. */

	if (bucket->btr_length <= dbb->dbb_page_size) {
		/*
		 * Journal new node added. The node is journalled as the compressed
		 * new node and the BTN of the re compressed next node.
		 */

		if (dbb->dbb_wal) {
			journal.jrnb_type = JRNP_BTREE_NODE;
			journal.jrnb_prefix_total = bucket->btr_prefix_total;
			journal.jrnb_offset = node_offset;
			journal.jrnb_delta = delta;
			journal.jrnb_length = BTN_SIZE + BTN_SIZE + BTN_LENGTH(new_node);
			CCH_journal_record(tdbb,
							   window,
							   (UCHAR *) & journal, JRNB_SIZE, (UCHAR *)
							   bucket + node_offset, journal.jrnb_length);
		}

		CCH_RELEASE(tdbb, window);

		return 0;
	}

/* We've a bucket split in progress.  We need to determine the split point.
   Set it halfway through the page, unless we are at the end of the page, 
   in which case put only the new node on the new page.  This will ensure 
   that pages get filled in the case of a monotonically increasing key. 
   Make sure that the original page has room, in case the END_BUCKET marker 
   is now longer because it is pointing at the new node.
*/

	DEBUG;
	if (end_of_page &&
		((UCHAR *) NEXT_NODE(new_node) <=
		 (UCHAR *) bucket + dbb->dbb_page_size))
midpoint = (UCHAR *) new_node;
	else
		midpoint =
			(UCHAR *) bucket +
			(dbb->dbb_page_size - OFFSETA(BTR, btr_nodes)) / 2;

/* Copy the bucket up to the midpoint, restructing the full midpoint key */

	prefix_total = 0;
#ifdef IGNORE_NULL_IDX_KEY
	new_key->key_flags = 0;
	midpoint_is_end_non_null = FALSE;
	for (p = (UCHAR *) bucket->btr_nodes;
		 p < midpoint || TRUE == midpoint_is_end_non_null;)
#else
	for (p = (UCHAR *) bucket->btr_nodes; p < midpoint;)
#endif /* IGNORE_NULL_IDX_KEY */
	{
		node = (BTN) p;
		prefix_total += BTN_PREFIX(node);
		p = BTN_DATA(node);
		q = new_key->key_data + BTN_PREFIX(node);
		new_key->key_length = BTN_PREFIX(node) + BTN_LENGTH(node);
		if ( (l = BTN_LENGTH(node)) )
			do
				*q++ = *p++;
			while (--l);
#ifdef IGNORE_NULL_IDX_KEY
		/* We do not want END_NON_NULL marker as the first node in the split page.
		 * Move one extra node down in the original bucket */
		if (END_NON_NULL == BTR_get_quad(BTN_NUMBER(node))) {
			/* assert: cannot have multiple END_NON_NULL markers */
			assert(midpoint_is_end_non_null == FALSE);
			midpoint_is_end_non_null = TRUE;
		}
		else
			midpoint_is_end_non_null = FALSE;
#endif /* IGNORE_NULL_IDX_KEY */
	}

/* Allocate and format the overflow page */

	split = (BTR) DPM_allocate(tdbb, &split_window);

/* if we're a pointer page, make sure the child page is written first */

	if (bucket->btr_level != 0)
		if ((UCHAR *) new_node < midpoint)
			CCH_precedence(tdbb, window, insertion->iib_number);
		else
			CCH_precedence(tdbb, &split_window, insertion->iib_number);

/* format the new page to look like the old page */

	split->btr_header.pag_type = bucket->btr_header.pag_type;
	split->btr_relation = bucket->btr_relation;
	split->btr_id = bucket->btr_id;
	split->btr_level = bucket->btr_level;
	split->btr_sibling = right_sibling = bucket->btr_sibling;
	split->btr_left_sibling = window->win_page;
	split->btr_header.pag_flags |=
		(bucket->btr_header.pag_flags & btr_descending);

/* Format the first node on the overflow page */

	new_node = split->btr_nodes;
	BTN_PREFIX(new_node) = 0;
	QUAD_MOVE(BTN_NUMBER(node), BTN_NUMBER(new_node));
	p = BTN_DATA(new_node);
	q = new_key->key_data;
	assert(new_key->key_length <= MAX_UCHAR);
        if ( (l = BTN_LENGTH(new_node) = (UCHAR) new_key->key_length) )
		do
			MOVE_BYTE(q, p);
		while (--l);

/* Copy down the remaining half of the original bucket on the overflow page */

	q = (UCHAR *) (NEXT_NODE(node));
	l = bucket->btr_length - (q - (UCHAR *) bucket);

	if (((U_IPTR) p & (ALIGNMENT - 1))
		|| ((U_IPTR) q & (ALIGNMENT - 1)))
		MOVE_FAST(q, p, l);
	else
		MOVE_FASTER(q, p, l);

	split->btr_length = p + l - (UCHAR *) split;

/* the sum of the prefixes on the split page is the previous total minus
   the prefixes found on the original page; the sum of the prefixes on the 
   original page must exclude the split node */

	split->btr_prefix_total = bucket->btr_prefix_total - prefix_total;
	bucket->btr_prefix_total = prefix_total - BTN_PREFIX(node);
	split_page = split_window.win_page;

	CCH_RELEASE(tdbb, &split_window);
	CCH_precedence(tdbb, window, split_window.win_page);
	CCH_mark_must_write(tdbb, window);

/* The split bucket is still residing in the overflow area.  Copy it
   back to the original buffer.  After cleaning up the last node,
   we're done! */

	bucket->btr_sibling = split_window.win_page;

/* mark the end of the page; note that the end_bucket marker must 
   contain info about the first node on the next page */

	quad_put((SLONG) END_BUCKET, BTN_NUMBER(node));
#ifdef IGNORE_NULL_IDX_KEY
/* Why should the END_BUCKET marker contain info about the first node on the
 * next page??? -bsriram, 13-Sep-1999
 */
	BTN_PREFIX(node) = 0;
	BTN_LENGTH(node) = 0;
#endif /* IGNORE_NULL_IDX_KEY */
	next_node = NEXT_NODE(node);
	bucket->btr_length = (UCHAR *) next_node - (UCHAR *) bucket;
	MOVE_FASTER(bucket, window->win_buffer, bucket->btr_length);

/* mark the bucket as non garbage-collectable until we can propogate 
   the split page up to the parent; otherwise its possible that the 
   split page we just created will be lost */

	bucket->btr_header.pag_flags |= btr_dont_gc;

/* journal the split page */

	if (dbb->dbb_wal)
		journal_btree_segment(tdbb, window, bucket);

	if (original_page)
		*original_page = window->win_page;

/* now we need to go to the right sibling page and update its 
   left sibling pointer to point to the newly split page */

	if (right_sibling) {
		bucket =
			(BTR) CCH_HANDOFF(tdbb,
							  window, right_sibling, LCK_write, pag_index);
		CCH_MARK(tdbb, window);
		bucket->btr_left_sibling = split_window.win_page;
		if (dbb->dbb_journal)
			CCH_journal_page(tdbb, window);
	}
	CCH_RELEASE(tdbb, window);

/* return the page number of the right sibling page */

	if (sibling_page)
		*sibling_page = right_sibling;

	return split_page;
}


static void journal_btree_segment(TDBB tdbb, WIN * window, BTR bucket)
{
/**************************************
 *
 *    j o u r n a l _ b t r e e _ s e g m e n t
 *
 **************************************
 *
 * Functional description
 *    Journal valid part of btree segment.
 *
 **************************************/
	JRNB journal;

	SET_TDBB(tdbb);

	journal.jrnb_type = JRNP_BTREE_SEGMENT;
	journal.jrnb_offset = 0;
	journal.jrnb_delta = 0;
	journal.jrnb_length = bucket->btr_length;

	CCH_journal_record(tdbb, window, (UCHAR *) & journal, JRNB_SIZE, (UCHAR *)
					   bucket, journal.jrnb_length);
} static BOOLEAN key_equality(KEY * key, BTN node)
{
/**************************************
 *
 *	k e y _ e q u a l i t y
 *
 **************************************
 *
 * Functional description
 *	Check a B-tree node against a key for equality.
 *
 **************************************/
	SSHORT l;
	UCHAR *p, *q;

	if (key->key_length != node->btn_length + node->btn_prefix)
		return FALSE;

	if (!(l = node->btn_length))
		return TRUE;

	p = node->btn_data;
	q = key->key_data + node->btn_prefix;

	do
		if (*p++ != *q++)
			return FALSE;
	while (--l);

	return TRUE;
}

static INT64_KEY make_int64_key(SINT64 q, SSHORT scale)
{
/**************************************
 *
 *	m a k e _ i n t 6 4 _ k e y
 *
 **************************************
 *
 * Functional description
 *	Make an Index key for a 64-bit Integer value.
 *
 **************************************/
	UINT64 uq;
	INT64_KEY key;
	int n;

/* Following structure declared above in the modules global section
 *
 * static const struct {
 *     UINT64 limit;		--- if abs(q) is >= this, ...
 *     SINT64 factor;		--- then multiply by this, ...
 *     SSHORT scale_change;	--- and add this to the scale.
 * } int64_scale_control[];
 */

/* Before converting the scaled int64 to a double, multiply it by the
 * largest power of 10 which will NOT cause an overflow, and adjust
 * the scale accordingly.  This ensures that two different
 * representations of the same value, entered at times when the
 * declared scale of the column was different, actually wind up
 * being mapped to the same key.
 */
	n = 0;
	uq = (UINT64) ((q >= 0) ? q : -q);	/* absolute value */
	while (uq < int64_scale_control[n].limit)
		n++;
	q *= int64_scale_control[n].factor;
	scale -= int64_scale_control[n].scale_change;

	key.d_part = ((double) (q / 10000)) / powerof10(scale);
	key.s_part = (SSHORT) (q % 10000);

	return key;
}

#ifdef DEBUG_INDEXKEY
static void print_int64_key(SINT64 value, SSHORT scale, INT64_KEY key)
{
/**************************************
 *
 *	p r i n t _ i n t 6 4 _ k e y
 *
 **************************************
 *
 * Functional description
 *	Debugging function to print a key created out of an int64 
 *	quantify.
 *
 **************************************/
	UCHAR *p;
	USHORT n;

	ib_fprintf(ib_stderr,
			   "%20" QUADFORMAT
			   "d  %4d  %.15e  %6d  ", value, scale, key.d_part, key.s_part);

	p = (UCHAR *) & key;
	for (n = 10; n--; n > 0)
		ib_fprintf(ib_stderr, "%02x ", *p++);

	ib_fprintf(ib_stderr, "\n");
	return;
}
#endif /* DEBUG_INDEXKEY */


static void quad_put(SLONG value, SCHAR* data)
{
/**************************************
 *
 *	q u a d _ p u t
 *
 **************************************
 *
 * Functional description
 *	Move SLONG to a four byte vector.
 *
 **************************************/

	const SCHAR* p = (SCHAR*) &value;

	data[0] = p[0];
	data[1] = p[1];
	data[2] = p[2];
	data[3] = p[3];
}


static void quad_move(UCHAR* a, UCHAR* b)
{
/**************************************
 *
 *	q u a d _ m o v e
 *
 **************************************
 *
 * Functional description
 *	Move an unaligned longword (4 bytes).
 *
 **************************************/

	MOVE_BYTE(a, b);
	MOVE_BYTE(a, b);
	MOVE_BYTE(a, b);
	MOVE_BYTE(a, b);
}


static CONTENTS remove_node(TDBB tdbb, IIB * insertion, WIN * window)
{
/**************************************
 *
 *	r e m o v e _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Remove an index node from a b-tree, 
 * 	recursing down through the levels in case 
 * 	we need to garbage collect pages.
 *
 **************************************/
	DBB dbb;
	IDX *idx;
	BTR page;
	BTN node;
	SLONG number, parent_number;
	CONTENTS result;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	idx = insertion->iib_descriptor;
	page = (BTR) window->win_buffer;

/* if we are on a leaf page, remove the leaf node */

	if (page->btr_level == 0)
		return remove_leaf_node(tdbb, insertion, window);

#ifdef IGNORE_NULL_IDX_KEY
	assert(page->btr_level != 0);
#endif /* IGNORE_NULL_IDX_KEY */
	while (TRUE) {
		node = find_node(page, insertion->iib_key, (USHORT)
						 (idx->idx_flags & idx_descending));
		number = BTR_get_quad(BTN_NUMBER(node));

		/* we should always find the node, but let's make sure */

		if (number == END_LEVEL) {
			CCH_RELEASE(tdbb, window);
#ifdef DEBUG_BTR
			CORRUPT(204);		/* msg 204 index inconsistent */
#endif
			return contents_above_threshold;
		}
		/* recurse to the next level down; if we are about to fetch a 
		   level 0 page, make sure we fetch it for write */
		if (number != END_BUCKET) {
			/* handoff down to the next level, retaining the parent page number */

			parent_number = window->win_page;
			page = (BTR)
				CCH_HANDOFF(tdbb, window, number, (SSHORT)
							(
							 (page->btr_level
							  == 1) ? LCK_write : LCK_read), pag_index);

			/* if the removed node caused the page to go below the garbage collection 
			   threshold, and the database was created by a version of the engine greater 
			   than 8.2, then we can garbage-collect the page */

			result = remove_node(tdbb, insertion, window);

			if ((result != contents_above_threshold)
				&& (dbb->dbb_ods_version >= ODS_VERSION9))
				return garbage_collect(tdbb, window, parent_number);

			if (window->win_bdb)
				CCH_RELEASE(tdbb, window);
			return contents_above_threshold;
		}

		/* we've hit end of bucket, so go to the sibling looking for the node */

		page = (BTR)
			CCH_HANDOFF(tdbb, window, page->btr_sibling, LCK_read, pag_index);
	}

/* NOTREACHED */
	return contents_empty;		/* superfluous return to shut lint up */
}


static CONTENTS remove_leaf_node(TDBB tdbb, IIB * insertion, WIN * window)
{
/**************************************
 *
 *	r e m o v e _ l e a f _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Remove an index node from the leaf level.
 *
 **************************************/
	BTN node;
	BTR page;
	USHORT l;
	UCHAR prefix, *p, *q;
	KEY *key;
	ULONG pages;
	SLONG number;

	SET_TDBB(tdbb);
	page = (BTR) window->win_buffer;
	key = insertion->iib_key;

/* Look for the first node with the value to be removed. */

	while (!
		   (node =
			BTR_find_leaf(page,
						  key, 0,
						  &prefix,
						  insertion->iib_descriptor->idx_flags
						  &
						  idx_descending,
						  FALSE))) page =
			(BTR) CCH_HANDOFF(tdbb,
							  window,
							  page->btr_sibling, LCK_write, pag_index);

/* Make sure first node looks ok */

#ifdef IGNORE_NULL_IDX_KEY
	if (key->key_length != BTN_LENGTH(node) + BTN_PREFIX(node))
#else
	if (prefix > BTN_PREFIX(node)
		|| key->key_length != BTN_LENGTH(node) + BTN_PREFIX(node))
#endif /* IGNORE_NULL_IDX_KEY */
	{
#ifdef DEBUG_BTR
		CCH_RELEASE(tdbb, window);
		CORRUPT(204);			/* msg 204 index inconsistent */
#endif
		return contents_above_threshold;
	}
/* check to make sure the node has the same value */
	p = BTN_DATA(node);
	q = key->key_data + BTN_PREFIX(node);
	if ( (l = BTN_LENGTH(node)) )
		do
			if (*p++ != *q++) {
#ifdef DEBUG_BTR
				CCH_RELEASE(tdbb, window);
				CORRUPT(204);	/* msg 204 index inconsistent */
#endif
				return contents_above_threshold;
			}
		while (--l);

/* now look through the duplicate nodes to find the one 
   with matching record number */

	pages = 0;
	while (TRUE) {
		/* if we find the right one, quit */

		number = BTR_get_quad(BTN_NUMBER(node));
		if (insertion->iib_number == number)
			break;

#ifdef IGNORE_NULL_IDX_KEY
		if (number == END_LEVEL || number == END_NON_NULL)
#else
		if (number == END_LEVEL)
#endif /* IGNORE_NULL_IDX_KEY */
		{
#ifdef DEBUG_BTR
			CCH_RELEASE(tdbb, window);
			CORRUPT(204);		/* msg 204 index inconsistent */
#endif
			return contents_above_threshold;
		}

		/* go to the next node and check that it is a duplicate */

		if (number != END_BUCKET) {
#ifdef IGNORE_NULL_IDX_KEY
			/* The next node could be (erroneously) a special END_xxx marker. 
			 * What if it is a END_BUCKET ? It is fine, and we should go to the
			 * next page, right?
			 * so do not check for comparison between BTN_PREFIX and key_length. 
			 * Just check whether the next node has any BTN_LENGTH. If it does,
			 * then it is not a duplicate */
			node = NEXT_NODE(node);
			if (BTN_LENGTH(node) != 0)
#else
			node = (BTN) (BTN_DATA(node) + BTN_LENGTH(node));
			if (BTN_LENGTH(node) != 0 || BTN_PREFIX(node) != key->key_length)
#endif /* IGNORE_NULL_IDX_KEY */
			{
#ifdef DEBUG_BTR
				CCH_RELEASE(tdbb, window);
				CORRUPT(204);	/* msg 204 index inconsistent */
#endif
				return contents_above_threshold;
			}
			continue;
		}

		/* if we hit the end of bucket, go to the right sibling page, 
		   and check that the first node is a duplicate */

		++pages;
		page = (BTR)
			CCH_HANDOFF(tdbb, window, page->btr_sibling, LCK_write, pag_index);
		node = page->btr_nodes;
		if ((l = BTN_LENGTH(node))
			!= key->key_length) {
#ifdef DEBUG_BTR
			CCH_RELEASE(tdbb, window);
			CORRUPT(204);		/* msg 204 index inconsistent */
#endif
			return contents_above_threshold;
		}

		if (l) {
			p = BTN_DATA(node);
			q = key->key_data;
			do
				if (*p++ != *q++) {
#ifdef DEBUG_BTR
					CCH_RELEASE(tdbb, window);
					CORRUPT(204);	/* msg 204 index inconsistent */
#endif
					return contents_above_threshold;
				}
			while (--l);
		}

#ifdef SUPERSERVER
		/* Until deletion of duplicate nodes becomes efficient, limit
		   leaf level traversal by rescheduling. */

		if (--tdbb->tdbb_quantum < 0 && !tdbb->tdbb_inhibit)
			if (JRD_reschedule(tdbb, 0, FALSE)) {
				CCH_RELEASE(tdbb, window);
				ERR_punt();
			}
#endif
	}

/* If we've needed to search thru a significant number of pages, warn the
   cache manager in case we come back this way */

	if (pages > 75)
		CCH_expand(tdbb, pages + 25);

	return delete_node(tdbb, window, node);
}


static BOOLEAN scan(TDBB tdbb,
					BTN node,
					SBM * bitmap,
					UCHAR prefix, KEY * key, USHORT flag)
{
/**************************************
 *
 *	s c a n
 *
 **************************************
 *
 * Functional description
 *	Do an index scan.  If we run over the bucket, return TRUE.  If
 *	we're completely done, return FALSE.
 *
 **************************************/
	USHORT l;
	SLONG number;
	UCHAR *end_key;
	UCHAR *p = NULL, *q = NULL;
	USHORT i, count;

	SET_TDBB(tdbb);

	DEBUG;
/* if the search key is flagged to indicate a multi-segment index
   stuff the key to the stuff boundary */
	if ((flag & irb_partial)
		&& (flag & irb_equality)
		&& !(flag & irb_starting)
		&& !(flag & irb_descending)) {
		count =
			STUFF_COUNT -
			((key->key_length + STUFF_COUNT) % (STUFF_COUNT + 1));
		for (i = 0; i < count; i++)
			key->key_data[key->key_length + i] = 0;
		count += key->key_length;
	}
	else
		count = key->key_length;

	end_key = key->key_data + count;
	count -= key->key_length;

/* reset irb_equality flag passed for optimization */

	flag &= ~irb_equality;

	while (true) {
		number = BTR_get_quad(BTN_NUMBER(node));

		if (number == END_LEVEL) {
			return FALSE;
		}

#ifdef IGNORE_NULL_IDX_KEY
		if (number == END_NON_NULL) {
			/* skip this node and go to the next one */
			node = NEXT_NODE(node);
			continue;
		}
#endif /* IGNORE_NULL_IDX_KEY */

		if (BTN_PREFIX(node) <= prefix) {
			prefix = BTN_PREFIX(node);
			p = key->key_data + prefix;
			q = BTN_DATA(node);
			for (l = BTN_LENGTH(node); l; --l, prefix++) {
				if (p >= end_key)
					if (flag)
						break;
					else
						return FALSE;
				if (p > (end_key - count))
					if (*p++ == *q++)
						break;
					else
						continue;
				if (*p < *q)
					return FALSE;
				if (*p++ > *q++)
					break;
			}
		}

		if (number == END_BUCKET)
			return TRUE;

		if ((flag & irb_starting) || !count)
			SBM_set(tdbb, bitmap, number);
		else if (p > (end_key - count))
			SBM_set(tdbb, bitmap, number);

		node = NEXT_NODE(node);
	}
/* NOTREACHED */
	return 0;					/* superfluous return to shut lint up */
}


} // extern "C"
