/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		idx.c
 *	DESCRIPTION:	Index manager
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
 * 2003.03.04 Dmitry Yemanov: Added support for NULLs in unique indices.
 *							  Done in two stages:
 *								1. Restored old behaviour of having only _one_
 *								   NULL key allowed (i.e. two NULLs are considered
 *								   duplicates). idx_e_nullunique error was removed.
 *								2. Changed algorithms in IDX_create_index() and
 *								   check_duplicates() to ignore NULL key duplicates.
 */

#include "firebird.h"
#include <string.h>
#include "../jrd/jrd.h"
#include "../jrd/val.h"
#include "../jrd/intl.h"
#include "../jrd/req.h"
#include "../jrd/ods.h"
#include "../jrd/btr.h"
#include "../jrd/sort.h"
#include "../jrd/lls.h"
#include "../jrd/tra.h"
#include "gen/codes.h"
#include "../jrd/sbm.h"
#include "../jrd/exe.h"
#include "../jrd/scl.h"
#include "../jrd/lck.h"
#include "../jrd/rse.h"
#include "../jrd/cch.h"
#include "../jrd/gdsassert.h"
#include "../jrd/all_proto.h"
#include "../jrd/btr_proto.h"
#include "../jrd/cch_proto.h"
#include "../jrd/cmp_proto.h"
#include "../jrd/dpm_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/idx_proto.h"
#include "../jrd/jrd_proto.h"
#include "../jrd/lck_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/sbm_proto.h"
#include "../jrd/sort_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/vio_proto.h"


extern "C" {


static const SCHAR NULL_STR = '\0';

/* Data to be passed to index fast load duplicates routine */

typedef struct ifl {
	SLONG ifl_duplicates;
	USHORT ifl_key_length;
} *IFL;

static IDX_E check_duplicates(TDBB, REC, IDX *, IIB *, JRD_REL);
static IDX_E check_foreign_key(TDBB, REC, JRD_REL, JRD_TRA, IDX *, JRD_REL *, USHORT *);
static IDX_E check_partner_index(TDBB, JRD_REL, REC, JRD_TRA, IDX *, JRD_REL, SSHORT);
static BOOLEAN duplicate_key(UCHAR *, UCHAR *, IFL);
static SLONG get_root_page(TDBB, JRD_REL);
static int index_block_flush(void *ast_object);
static IDX_E insert_key(TDBB, JRD_REL, REC, JRD_TRA, WIN *, IIB *, JRD_REL *, USHORT *);
static BOOLEAN key_equal(KEY *, KEY *);
static void signal_index_deletion(TDBB, JRD_REL, USHORT);


void IDX_check_access(TDBB tdbb, CSB csb, JRD_REL view, JRD_REL relation, JRD_FLD field)
{
/**************************************
 *
 *	I D X _ c h e c k _ a c c e s s
 *
 **************************************
 *
 * Functional description
 *	Check the various indices in a relation
 *	to see if we need REFERENCES access to fields
 *	in the primary key.   Don't call this routine for
 *	views or external relations, since the mechanism
 *	ain't there.
 *
 **************************************/
	WIN window, referenced_window;
	IDX idx, referenced_idx;
	JRD_REL referenced_relation;
	JRD_FLD referenced_field;
	IRT referenced_root;
	USHORT index_id, i;
	idx::idx_repeat * idx_desc;

	SET_TDBB(tdbb);

	idx.idx_id = (USHORT) -1;
	window.win_flags = 0;

	while (BTR_next_index(tdbb, relation, 0, &idx, &window))
		if (idx.idx_flags & idx_foreign) {
			/* find the corresponding primary key index */

			if (!MET_lookup_partner
				(tdbb, relation, &idx,
				 reinterpret_cast < UCHAR * >(const_cast <
											  SCHAR *
											  >(&NULL_STR)))) continue;
			referenced_relation =
				MET_relation(tdbb, idx.idx_primary_relation);
			MET_scan_relation(tdbb, referenced_relation);
			index_id = (USHORT) idx.idx_primary_index;

			/* get the description of the primary key index */

			referenced_window.win_page =
				get_root_page(tdbb, referenced_relation);
			referenced_window.win_flags = 0;
			referenced_root =
				(IRT) CCH_FETCH(tdbb, &referenced_window, LCK_read, pag_root);
			if (!BTR_description
				(referenced_relation, referenced_root, &referenced_idx,
				 index_id)) BUGCHECK(173);	/* msg 173 referenced index description not found */

			/* post references access to each field in the index */

			idx_desc = referenced_idx.idx_rpt;
			for (i = 0; i < referenced_idx.idx_count; i++, idx_desc++) {
				referenced_field =
					MET_get_field(referenced_relation, idx_desc->idx_field);
				CMP_post_access(tdbb, csb,
								referenced_relation->rel_security_name, 
									(view ? view->rel_id : 0),
								SCL_sql_references, "TABLE",
								referenced_relation->rel_name);
				CMP_post_access(tdbb, csb,
								referenced_field->fld_security_name, 0,
								SCL_sql_references, "COLUMN",
								referenced_field->fld_name);
			}

			CCH_RELEASE(tdbb, &referenced_window);
		}
}


void IDX_create_index(
					  TDBB tdbb,
					  JRD_REL relation,
					  IDX * idx,
					  UCHAR * index_name,
					  USHORT * index_id, JRD_TRA transaction, float *selectivity)
{
/**************************************
 *
 *	I D X _ c r e a t e _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Create and populate index.
 *
 **************************************/
	DBB dbb;
	ATT attachment;
	USHORT key_length, l;
	RPB primary, secondary;
	LLS stack;
	REC record, gc_record;
	ISR isr;
	KEY key;
	SCB sort_handle;
	SKD key_desc;
	UCHAR *p, *q, pad;
	JRD_REL partner_relation;
	USHORT partner_index_id;
	BOOLEAN cancel = FALSE;
	IDX_E result = idx_e_ok;
	struct ifl ifl_data;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;

	if (relation->rel_file)
		ERR_post(gds_no_meta_update, gds_arg_gds, gds_extfile_uns_op,
				 gds_arg_string, ERR_cstring(relation->rel_name), 0);

	BTR_reserve_slot(tdbb, relation, transaction, idx);

	if (index_id) {
		*index_id = idx->idx_id;
	}

	secondary.rpb_relation = relation;
	primary.rpb_relation   = relation;
	primary.rpb_number = -1;
	primary.rpb_window.win_flags = secondary.rpb_window.win_flags = 0;

	key_length = ROUNDUP(BTR_key_length(relation, idx), sizeof(SLONG));
	if (key_length >= MAX_KEY)
		ERR_post(gds_no_meta_update,
				 gds_arg_gds,
				 gds_keytoobig,
				 gds_arg_string,
				 ERR_cstring(reinterpret_cast < char *>(index_name)), 0);
#ifdef IGNORE_NULL_IDX_KEY
/* accomodate enough space to prepend sort key with a BOOLEAN as 
 * SORTP_VAL_IS_NULL for initial NULL segment records. */
	key_length += sizeof(SORTP);
#endif /* IGNORE_NULL_IDX_KEY */
	stack = NULL;
	pad = (idx->idx_flags & idx_descending) ? -1 : 0;

	ifl_data.ifl_duplicates = 0;
	ifl_data.ifl_key_length = key_length;

	bool key_is_null = false;

	key_desc.skd_dtype = SKD_bytes;
	key_desc.skd_flags = SKD_ascending;
	key_desc.skd_length = key_length;
	key_desc.skd_offset = 0;
	key_desc.skd_vary_offset = 0;

	sort_handle = SORT_init(tdbb->tdbb_status_vector,
							key_length + sizeof(struct isr),
							1,
							&key_desc,
							reinterpret_cast < BOOLEAN(*)() > (duplicate_key),
							&ifl_data, tdbb->tdbb_attachment, 0);

	if (!sort_handle)
		ERR_punt();

	if (idx->idx_flags & idx_foreign) {
		if (!MET_lookup_partner(tdbb, relation, idx, index_name))
			BUGCHECK(173);		/* msg 173 referenced index description not found */
		partner_relation = MET_relation(tdbb, idx->idx_primary_relation);
		partner_index_id = (USHORT) idx->idx_primary_index;
	}

/* Checkout a garbage collect record block for fetching data. */

	gc_record = VIO_gc_record(tdbb, relation);

/* Unless this is the only attachment or a database restore, worry about
   preserving the page working sets of other attachments. */

	if ((attachment = tdbb->tdbb_attachment) &&
		(attachment != dbb->dbb_attachments || attachment->att_next)) {
		if (attachment->att_flags & ATT_gbak_attachment ||
			DPM_data_pages(tdbb, relation) > dbb->dbb_bcb->bcb_count) {
			primary.rpb_window.win_flags = secondary.rpb_window.win_flags =
				WIN_large_scan;
			primary.rpb_org_scans = secondary.rpb_org_scans =
				relation->rel_scan_count++;
		}
	}

/* Loop thru the relation computing index keys.  If there are old versions,
   find them, too. */

	while (!cancel && DPM_next(tdbb, &primary, LCK_read, FALSE, FALSE)) {
		if (transaction && !VIO_garbage_collect(tdbb, &primary, transaction))
			continue;
		if (primary.rpb_flags & rpb_deleted)
			CCH_RELEASE(tdbb, &primary.rpb_window);
		else {
			primary.rpb_record = gc_record;
			VIO_data(tdbb, &primary,
					 reinterpret_cast < BLK > (dbb->dbb_permanent));
			gc_record = primary.rpb_record;
			LLS_PUSH(primary.rpb_record, &stack);
		}
		secondary.rpb_page = primary.rpb_b_page;
		secondary.rpb_line = primary.rpb_b_line;
		secondary.rpb_prior = primary.rpb_prior;
		while (secondary.rpb_page) {
			if (!DPM_fetch(tdbb, &secondary, LCK_read))
				break;			/* must be garbage collected */
			secondary.rpb_record = NULL;
			VIO_data(tdbb, &secondary,
					 reinterpret_cast < BLK > (tdbb->tdbb_default));
			LLS_PUSH(secondary.rpb_record, &stack);
			secondary.rpb_page = secondary.rpb_b_page;
			secondary.rpb_line = secondary.rpb_b_line;
		}

		while (stack) {
			record = (REC) LLS_POP(&stack);

			/* If foreign key index is being defined, make sure foreign
			   key definition will not be violated */

			if (idx->idx_flags & idx_foreign) {
				idx_null_state null_state;
				/* find out if there is a null segment by faking uniqueness --
				   if there is one, don't bother to check the primary key */

				if (!(idx->idx_flags & idx_unique)) {
					idx->idx_flags |= idx_unique;
					result = BTR_key(tdbb, relation, record, idx, &key, &null_state);
					idx->idx_flags &= ~idx_unique;
				} 
				else {
					result = BTR_key(tdbb, relation, record, idx, &key, &null_state);
				}
				if (null_state != idx_nulls_none) {
					result = idx_e_ok;
				}
				else {
					result =
						check_partner_index(tdbb, relation, record,
											transaction, idx,
											partner_relation,
											partner_index_id);
				}
			}

			if (result == idx_e_ok) {
				idx_null_state null_state;
				BTR_key(tdbb, relation, record, idx, &key, &null_state);
				key_is_null = (null_state == idx_nulls_all);
			}
			else {
				do {
					if (record != gc_record)
						delete record;
				} while (stack && (record = (REC) LLS_POP(&stack)));
				SORT_fini(sort_handle, tdbb->tdbb_attachment);
				gc_record->rec_flags &= ~REC_gc_active;
				if (primary.rpb_window.win_flags & WIN_large_scan)
					--relation->rel_scan_count;
				ERR_duplicate_error(result, partner_relation,
									partner_index_id);
			}

#ifdef IGNORE_NULL_IDX_KEY
			/* Offset BOOLEAN in the beginning of the sort record key */
			if (key.key_length > (key_length - sizeof(SORTP)))
#else
			if (key.key_length > key_length)
#endif /* IGNORE_NULL_IDX_KEY */
			{
				do {
					if (record != gc_record)
						delete record;
				} while (stack && (record = (REC) LLS_POP(&stack)));
				SORT_fini(sort_handle, tdbb->tdbb_attachment);
				gc_record->rec_flags &= ~REC_gc_active;
				if (primary.rpb_window.win_flags & WIN_large_scan)
					--relation->rel_scan_count;
				BUGCHECK(174);	/* msg 174 index key too big */
			}
			if (SORT_put
				(tdbb->tdbb_status_vector, sort_handle,
				 reinterpret_cast < ULONG ** >(&p))) {
				do {
					if (record != gc_record)
						delete record;
				} while (stack && (record = (REC) LLS_POP(&stack)));
				SORT_fini(sort_handle, tdbb->tdbb_attachment);
				gc_record->rec_flags &= ~REC_gc_active;
				if (primary.rpb_window.win_flags & WIN_large_scan)
					--relation->rel_scan_count;
				ERR_punt();
			}

			/* try to catch duplicates early */

			if ((idx->idx_flags & idx_unique) && ifl_data.ifl_duplicates > 0) {
				do {
					if (record != gc_record)
						delete record;
				} while (stack && (record = (REC) LLS_POP(&stack)));
				SORT_fini(sort_handle, tdbb->tdbb_attachment);
				gc_record->rec_flags &= ~REC_gc_active;
				if (primary.rpb_window.win_flags & WIN_large_scan)
					--relation->rel_scan_count;
				ERR_post(gds_no_dup, gds_arg_string,
						 ERR_cstring(reinterpret_cast < char *>(index_name)),
						 0);
			}

#ifdef IGNORE_NULL_IDX_KEY
			/* Initialize first longword of sort record key for NULL/non-NULL 
			 * values. When sorting and merge runs happen later, this BOOLEAN
			 * valued longword will be used to order NULL valued records high */
			if (key.key_flags & KEY_first_segment_is_null)
				*((SORTP *) p) = SORTP_VAL_IS_NULL;
			else
				*((SORTP *) p) = SORTP_VAL_IS_NOT_NULL;
			p += sizeof(SORTP);
#endif /* IGNORE_NULL_IDX_KEY */

			l = key.key_length;
			q = key.key_data;

			assert(l > 0);

			do
				*p++ = *q++;
			while (--l);
#ifdef IGNORE_NULL_IDX_KEY
			/* Offset BOOLEAN in the beginning of the sort record key */
			if (l = key_length - key.key_length - sizeof(SORTP))
#else
			if ( (l = key_length - key.key_length) )
#endif /* IGNORE_NULL_IDX_KEY */
				do
					*p++ = pad;
				while (--l);
			isr = (ISR) p;
			isr->isr_key_length = key.key_length;
			isr->isr_record_number = primary.rpb_number;
			isr->isr_flags = (stack ? ISR_secondary : 0) | (key_is_null ? ISR_null : 0);
			if (record != gc_record)
				delete record;
		}

#ifdef SUPERSERVER
		if (--tdbb->tdbb_quantum < 0 && !tdbb->tdbb_inhibit)
			cancel = JRD_reschedule(tdbb, 0, FALSE);
#endif
	}

	gc_record->rec_flags &= ~REC_gc_active;
	if (primary.rpb_window.win_flags & WIN_large_scan)
		--relation->rel_scan_count;

	if (cancel || SORT_sort(tdbb->tdbb_status_vector, sort_handle)) {
		SORT_fini(sort_handle, tdbb->tdbb_attachment);
		ERR_punt();
	}

	if ((idx->idx_flags & idx_unique) && ifl_data.ifl_duplicates > 0) {
		SORT_fini(sort_handle, tdbb->tdbb_attachment);
		ERR_post(gds_no_dup, gds_arg_string,
				 ERR_cstring(reinterpret_cast < char *>(index_name)), 0);
	}

	BTR_create(tdbb, relation, idx, key_length, sort_handle, selectivity);

	if ((idx->idx_flags & idx_unique) && ifl_data.ifl_duplicates > 0) {
		// we don't need SORT_fini() here, as it's called inside BTR_create()
		ERR_post(gds_no_dup, gds_arg_string,
				 ERR_cstring(reinterpret_cast < char *>(index_name)), 0);
	}
}


IDB IDX_create_index_block(TDBB tdbb, JRD_REL relation, USHORT id)
{
/**************************************
 *
 *	I D X _ c r e a t e _ i n d e x _ b l o c k
 *
 **************************************
 *
 * Functional description
 *	Create an index block and an associated
 *	lock block for the specified index.
 *
 **************************************/
	DBB dbb;
	IDB index_block;
	LCK lock;

	SET_TDBB(tdbb);
	dbb = tdbb->tdbb_database;
	CHECK_DBB(dbb);

	index_block = FB_NEW(*dbb->dbb_permanent) idb();
	index_block->idb_id = id;

/* link the block in with the relation linked list */

	index_block->idb_next = relation->rel_index_blocks;
	relation->rel_index_blocks = index_block;

/* create a shared lock for the index, to coordinate
   any modification to the index so that the cached information
   about the index will be discarded */

	index_block->idb_lock = lock = FB_NEW_RPT(*dbb->dbb_permanent, 0) lck;
	lock->lck_parent = dbb->dbb_lock;
	lock->lck_dbb = dbb;
	lock->lck_key.lck_long = index_block->idb_id;
	lock->lck_length = sizeof(lock->lck_key.lck_long);
	lock->lck_type = LCK_expression;
	lock->lck_owner_handle = LCK_get_owner_handle(tdbb, lock->lck_type);
	lock->lck_ast = index_block_flush;
	lock->lck_object = reinterpret_cast<blk*>(index_block);

	return index_block;
}


void IDX_delete_index(TDBB tdbb, JRD_REL relation, USHORT id)
{
/**************************************
 *
 *	I D X _ d e l e t e _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	Delete a single index.
 *
 **************************************/
	WIN window;

	SET_TDBB(tdbb);

	signal_index_deletion(tdbb, relation, id);

	window.win_page = relation->rel_index_root;
	window.win_flags = 0;
	CCH_FETCH(tdbb, &window, LCK_write, pag_root);

	BTR_delete_index(tdbb, &window, id);
}


void IDX_delete_indices(TDBB tdbb, JRD_REL relation)
{
/**************************************
 *
 *	I D X _ d e l e t e _ i n d i c e s
 *
 **************************************
 *
 * Functional description
 *	Delete all known indices in preparation for deleting a
 *	complete relation.
 *
 **************************************/
	WIN window;
	IRT root;
	SSHORT i;

	SET_TDBB(tdbb);

	window.win_page = relation->rel_index_root;
	window.win_flags = 0;
	root = (IRT) CCH_FETCH(tdbb, &window, LCK_write, pag_root);

	for (i = 0; i < root->irt_count; i++) {
		BTR_delete_index(tdbb, &window, i);
		root = (IRT) CCH_FETCH(tdbb, &window, LCK_write, pag_root);
	}

	CCH_RELEASE(tdbb, &window);
}


IDX_E IDX_erase(TDBB tdbb,
				RPB * rpb,
				JRD_TRA transaction, JRD_REL * bad_relation, USHORT * bad_index)
{
/**************************************
 *
 *	I D X _ e r a s e
 *
 **************************************
 *
 * Functional description
 *	Check the various indices prior to an ERASE operation.
 *	If one is a primary key, check its partner for 
 *	a duplicate record.
 *
 **************************************/
	WIN window;
	IDX idx;
	IDX_E error_code;

	SET_TDBB(tdbb);

	error_code = idx_e_ok;
	idx.idx_id = (USHORT) -1;
	window.win_flags = 0;

	while (BTR_next_index
		   (tdbb, rpb->rpb_relation, transaction, &idx,
			&window)) if (idx.idx_flags & (idx_primary | idx_unique)) {
			error_code =
				check_foreign_key(tdbb, rpb->rpb_record, rpb->rpb_relation,
								  transaction, &idx, bad_relation, bad_index);
			if (idx_e_ok != error_code) {
				CCH_RELEASE(tdbb, &window);
				break;
			}
		}

	return error_code;
}


void IDX_garbage_collect(TDBB tdbb, RPB * rpb, LLS going, LLS staying)
{
/**************************************
 *
 *	I D X _ g a r b a g e _ c o l l e c t
 *
 **************************************
 *
 * Functional description
 *	Perform garbage collection for a bunch of records.  Scan
 *	through the indices defined for a relation.  Garbage collect
 *	each.
 *
 **************************************/
	LLS stack1, stack2;
	REC rec1, rec2;
	IIB insertion;
	IDX idx;
	KEY key1, key2;
	WIN window;
	IRT root;
	USHORT i;

	SET_TDBB(tdbb);

	insertion.iib_descriptor = &idx;
	insertion.iib_number = rpb->rpb_number;
	insertion.iib_relation = rpb->rpb_relation;
	insertion.iib_key = &key1;

	window.win_page = rpb->rpb_relation->rel_index_root;
	window.win_flags = 0;
	root = (IRT) CCH_FETCH(tdbb, &window, LCK_read, pag_root);

	for (i = 0; i < root->irt_count; i++)
		if (BTR_description(rpb->rpb_relation, root, &idx, i)) {
			for (stack1 = going; stack1; stack1 = stack1->lls_next) {
				rec1 = (REC) stack1->lls_object;
				BTR_key(tdbb, rpb->rpb_relation, rec1, &idx, &key1, 0);

				/* Cancel index if there are duplicates in the remaining records */

				for (stack2 = stack1->lls_next; stack2; stack2 = stack2->lls_next) {
					rec2 = (REC) stack2->lls_object;
					if (rec2->rec_number == rec1->rec_number) {
						BTR_key(tdbb, rpb->rpb_relation, rec2, &idx, &key2, 0);
						if (key_equal(&key1, &key2))
							break;
					}
				}
				if (stack2)
					continue;

				/* Make sure the index doesn't exist in any record remaining */

				for (stack2 = staying; stack2; stack2 = stack2->lls_next) {
					rec2 = (REC) stack2->lls_object;
					BTR_key(tdbb, rpb->rpb_relation, rec2, &idx, &key2, 0);
					if (key_equal(&key1, &key2))
						break;
				}
				if (stack2)
					continue;

				/* Get rid of index node */

				BTR_remove(tdbb, &window, &insertion);
				root = (IRT) CCH_FETCH(tdbb, &window, LCK_read, pag_root);
				if (stack1->lls_next)
					BTR_description(rpb->rpb_relation, root, &idx, i);
			}
		}

	CCH_RELEASE(tdbb, &window);
}


IDX_E IDX_modify(TDBB tdbb,
				 RPB * org_rpb,
				 RPB * new_rpb,
				 JRD_TRA transaction, JRD_REL * bad_relation, USHORT * bad_index)
{
/**************************************
 *
 *	I D X _ m o d i f y
 *
 **************************************
 *
 * Functional description
 *	Update the various indices after a MODIFY operation.  If a duplicate
 *	index is violated, return the index number.  If successful, return
 *	-1.
 *
 **************************************/
	WIN window;
	IDX idx;
	IIB insertion;
	KEY key1, key2;
	IDX_E error_code;

	SET_TDBB(tdbb);

	insertion.iib_relation = org_rpb->rpb_relation;
	insertion.iib_number = org_rpb->rpb_number;
	insertion.iib_key = &key1;
	insertion.iib_descriptor = &idx;
	insertion.iib_transaction = transaction;
	error_code = idx_e_ok;
	idx.idx_id = (USHORT) -1;
	window.win_flags = 0;

	while (BTR_next_index
		   (tdbb, org_rpb->rpb_relation, transaction, &idx, &window)) {
		*bad_index = idx.idx_id;
		*bad_relation = new_rpb->rpb_relation;
		if ( (error_code =
			BTR_key(tdbb, new_rpb->rpb_relation, new_rpb->rpb_record, &idx,
					&key1, 0)) ) {
			CCH_RELEASE(tdbb, &window);
			break;
		}
		BTR_key(tdbb, org_rpb->rpb_relation, org_rpb->rpb_record, &idx,
				&key2, 0);
		if (!key_equal(&key1, &key2)) {
			if (( error_code =
				insert_key(tdbb, new_rpb->rpb_relation, new_rpb->rpb_record,
						   transaction, &window, &insertion, bad_relation,
						   bad_index)) ) return error_code;
		}
	}

	return error_code;
}


IDX_E IDX_modify_check_constraints(TDBB tdbb,
								   RPB * org_rpb,
								   RPB * new_rpb,
								   JRD_TRA transaction,
								   JRD_REL * bad_relation, USHORT * bad_index)
{
/**************************************
 *
 *	I D X _ m o d i f y _ c h e c k _ c o n s t r a i n t
 *
 **************************************
 *
 * Functional description
 *	Check for foreign key constraint after a modify statement
 *
 **************************************/
	WIN window;
	IDX idx;
	KEY key1, key2;
	IDX_E error_code;

	SET_TDBB(tdbb);

	error_code = idx_e_ok;
	idx.idx_id = (USHORT) -1;
	window.win_flags = 0;

/* If relation's primary/unique keys have no dependencies by other
   relations' foreign keys then don't bother cycling thru all index
   descriptions. */

	if (!(org_rpb->rpb_relation->rel_flags & REL_check_partners) &&
		!(org_rpb->rpb_relation->rel_primary_dpnds.prim_reference_ids))
			return error_code;

/* Now check all the foreign key constraints. Referential integrity relation
   could be established by primary key/foreign key or unique key/foreign key */

	while (BTR_next_index
		   (tdbb, org_rpb->rpb_relation, transaction, &idx, &window)) {
		if (!(idx.idx_flags & (idx_primary | idx_unique))
			|| !MET_lookup_partner(tdbb, org_rpb->rpb_relation, &idx,
								   reinterpret_cast < UCHAR * >(const_cast <
																SCHAR *
																>
																(&NULL_STR))))
				continue;
		*bad_index = idx.idx_id;
		*bad_relation = new_rpb->rpb_relation;
		if (
			(error_code =
			 BTR_key(tdbb, new_rpb->rpb_relation, new_rpb->rpb_record, &idx,
					 &key1, 0))
			|| (error_code =
				BTR_key(tdbb, org_rpb->rpb_relation, org_rpb->rpb_record,
						&idx, &key2, 0))) {
			CCH_RELEASE(tdbb, &window);
			break;
		}
		if (!key_equal(&key1, &key2)) {
			error_code =
				check_foreign_key(tdbb, org_rpb->rpb_record,
								  org_rpb->rpb_relation, transaction, &idx,
								  bad_relation, bad_index);
			if (idx_e_ok != error_code) {
				CCH_RELEASE(tdbb, &window);
				return error_code;
			}
		}
	}

	return error_code;
}


float IDX_statistics(TDBB tdbb, JRD_REL relation, USHORT id)
{
/**************************************
 *
 *	I D X _ s t a t i s t i c s
 *
 **************************************
 *
 * Functional description
 *	Scan index pages recomputing
 *	selectivity.
 *
 **************************************/

	SET_TDBB(tdbb);

	return BTR_selectivity(tdbb, relation, id);
}


IDX_E IDX_store(TDBB tdbb,
				RPB * rpb,
				JRD_TRA transaction, JRD_REL * bad_relation, USHORT * bad_index)
{
/**************************************
 *
 *	I D X _ s t o r e
 *
 **************************************
 *
 * Functional description
 *	Update the various indices after a STORE operation.  If a duplicate
 *	index is violated, return the index number.  If successful, return
 *	-1.
 *
 **************************************/
	WIN window;
	IDX idx;
	IIB insertion;
	KEY key;
	IDX_E error_code;

	SET_TDBB(tdbb);

	insertion.iib_relation = rpb->rpb_relation;
	insertion.iib_number = rpb->rpb_number;
	insertion.iib_key = &key;
	insertion.iib_descriptor = &idx;
	insertion.iib_transaction = transaction;

	error_code = idx_e_ok;
	idx.idx_id = (USHORT) -1;
	window.win_flags = 0;

	while (BTR_next_index
		   (tdbb, rpb->rpb_relation, transaction, &idx, &window)) {
		*bad_index = idx.idx_id;
		*bad_relation = rpb->rpb_relation;
		if ( (error_code =
			BTR_key(tdbb, rpb->rpb_relation, rpb->rpb_record, &idx, &key, 0)) ) {
			CCH_RELEASE(tdbb, &window);
			break;
		}
		if ( (error_code =
			insert_key(tdbb, rpb->rpb_relation, rpb->rpb_record, transaction,
					   &window, &insertion, bad_relation, bad_index)) )
			return error_code;
	}

	return error_code;
}


static IDX_E check_duplicates(
							  TDBB tdbb,
							  REC record,
							  IDX * record_idx,
							  IIB * insertion, JRD_REL relation_2)
{
/**************************************
 *
 *	c h e c k _ d u p l i c a t e s
 *
 **************************************
 *
 * Functional description
 *	Make sure there aren't any active duplicates for
 *	a unique index or a foreign key.
 *
 **************************************/
	IDX_E result;
	RPB rpb;
	IDX *insertion_idx;
	DSC desc1, desc2;
	USHORT field_id, flag, i, flag_2;
	JRD_REL relation_1;

	SET_TDBB(tdbb);

	result = idx_e_ok;
	insertion_idx = insertion->iib_descriptor;

	rpb.rpb_number = -1;
	rpb.rpb_relation = insertion->iib_relation;
	rpb.rpb_record = NULL;
	rpb.rpb_window.win_flags = 0;
	relation_1 = insertion->iib_relation;

	while (SBM_next
		   (insertion->iib_duplicates, &rpb.rpb_number, RSE_get_forward)) {
		if (rpb.rpb_number != insertion->iib_number
			&& VIO_get_current(tdbb, &rpb, insertion->iib_transaction,
							   reinterpret_cast < BLK > (tdbb->tdbb_default),
							   record_idx->idx_flags & idx_foreign)) {
			// dimitr: we shouldn't ignore status exceptions which take place
			//		   inside the lock manager. Namely, they are: gds_deadlock,
			//		   gds_lock_conflict, gds_lock_timeout. Otherwise we may
			//		   have logically corrupted database as a result. If any
			//		   of the mentioned errors appeared, it means that there's
			//		   an active transaction out there which has modified our
			//		   record. For "nowait" transaction, it means we have
			//		   an update conflict.
			//
			// was: if (rpb.rpb_flags & rpb_deleted) {
			//
			// P.S. I think the check for a status vector should be enough,
			//      but for sure let's keep the old one as well.
			//														2003.05.27

			bool lock_error =
				(tdbb->tdbb_status_vector[1] == gds_deadlock ||
				tdbb->tdbb_status_vector[1] == gds_lock_conflict ||
				tdbb->tdbb_status_vector[1] == gds_lock_timeout);
			// the above errors are not thrown but returned silently

			if (rpb.rpb_flags & rpb_deleted || lock_error) {
				result = idx_e_duplicate;
				break;
			}

			/* check the values of the fields in the record being inserted with the 
			   record retrieved -- for unique indexes the insertion index and the 
			   record index are the same, but for foreign keys they are different */

			bool all_nulls = true;

			for (i = 0; i < insertion_idx->idx_count; i++) {
				field_id = insertion_idx->idx_rpt[i].idx_field;
				/* In order to "map a null to a default" value (in EVL_field()), 
				 * the relation block is referenced. 
				 * Reference: Bug 10116, 10424 
				 */
				flag =
					EVL_field(relation_1, rpb.rpb_record, field_id, &desc1);

				field_id = record_idx->idx_rpt[i].idx_field;
				flag_2 = EVL_field(relation_2, record, field_id, &desc2);

				if (flag != flag_2 || MOV_compare(&desc1, &desc2) != 0)
					break;
				all_nulls = all_nulls && !flag && !flag_2;
			}

			if (i >= insertion_idx->idx_count && !all_nulls) {
				result = idx_e_duplicate;
				break;
			}
		}
	}

	if (rpb.rpb_record)
		delete rpb.rpb_record;

	return result;
}


static IDX_E check_foreign_key(
							   TDBB tdbb,
							   REC record,
							   JRD_REL relation,
							   JRD_TRA transaction,
							   IDX * idx,
JRD_REL * bad_relation, USHORT * bad_index)
{
/**************************************
 *
 *	c h e c k _ f o r e i g n _ k e y
 *
 **************************************
 *
 * Functional description
 *	The passed index participates in a foreign key.
 *	Check the passed record to see if a corresponding 
 *	record appears in the partner index.
 *
 **************************************/
	IDX_E result;
	int index_number;
	JRD_REL partner_relation;
	USHORT index_id;

	SET_TDBB(tdbb);

	result = idx_e_ok;

	if (!MET_lookup_partner
		(tdbb, relation, idx,
		 reinterpret_cast < UCHAR * >(const_cast < SCHAR * >(&NULL_STR))))
		return result;

	if (idx->idx_flags & idx_foreign) {
		partner_relation = MET_relation(tdbb, idx->idx_primary_relation);
		index_id = (USHORT) idx->idx_primary_index;
		result =
			check_partner_index(tdbb, relation, record, transaction, idx,
								partner_relation, index_id);
	}
	else if (idx->idx_flags & (idx_primary | idx_unique))
		for (index_number = 0;
			 index_number < idx->idx_foreign_primaries->count();
			 index_number++) {
			if (idx->idx_id !=
				(UCHAR) (*idx->idx_foreign_primaries)[index_number]) continue;
			partner_relation =
				MET_relation(tdbb,
							 (int) (*idx->idx_foreign_relations)[index_number]);
			index_id =
				(USHORT) (*idx->idx_foreign_indexes)[index_number];
			if ( (result =
				check_partner_index(tdbb, relation, record, transaction, idx,
									partner_relation, index_id)) )
				break;
		}

	if (result) {
		if (idx->idx_flags & idx_foreign) {
			*bad_relation = relation;
			*bad_index = idx->idx_id;
		}
		else {
			*bad_relation = partner_relation;
			*bad_index = index_id;
		}
	}

	return result;
}


static IDX_E check_partner_index(
								 TDBB tdbb,
								 JRD_REL relation,
								 REC record,
								 JRD_TRA transaction,
								 IDX * idx,
JRD_REL partner_relation, SSHORT index_id)
{
/**************************************
 *
 *	c h e c k _ p a r t n e r _ i n d e x
 *
 **************************************
 *
 * Functional description
 *	The passed index participates in a foreign key.
 *	Check the passed record to see if a corresponding 
 *	record appears in the partner index.
 *
 **************************************/
	IDX_E result;
	WIN window;
	IRT root;
	IDX partner_idx;
	IIB insertion;
	KEY key;
	SBM bitmap;
	struct irb retrieval;

	SET_TDBB(tdbb);

	result = idx_e_ok;

/* get the index root page for the partner relation */

	window.win_page = get_root_page(tdbb, partner_relation);
	window.win_flags = 0;
	root = (IRT) CCH_FETCH(tdbb, &window, LCK_read, pag_root);

/* get the description of the partner index */

	if (!BTR_description(partner_relation, root, &partner_idx, index_id))
		BUGCHECK(175);			/* msg 175 partner index description not found */

/* get the key in the original index */

	result = BTR_key(tdbb, relation, record, idx, &key, 0);
	CCH_RELEASE(tdbb, &window);

/* now check for current duplicates */

	if (result == idx_e_ok) {
		/* fill out a retrieval block for the purpose of 
		   generating a bitmap of duplicate records  */

		bitmap = NULL;
		MOVE_CLEAR(&retrieval, sizeof(struct irb));
		//retrieval.blk_type = type_irb;
		retrieval.irb_index = partner_idx.idx_id;
		MOVE_FAST(&partner_idx, &retrieval.irb_desc,
				  sizeof(retrieval.irb_desc));
		retrieval.irb_generic = irb_equality;
		retrieval.irb_relation = partner_relation;
		retrieval.irb_key = &key;
		retrieval.irb_upper_count = retrieval.irb_lower_count = idx->idx_count;
		if (partner_idx.idx_flags & idx_descending) {
			retrieval.irb_generic |= irb_descending;
		}
		if ((partner_idx.idx_flags & idx_descending) != 
					(idx->idx_flags & idx_descending)) {
			BTR_complement_key(&key);
		}
		BTR_evaluate(tdbb, &retrieval, &bitmap);

		/* if there is a bitmap, it means duplicates were found */

		if (bitmap) {
			insertion.iib_descriptor = &partner_idx;
			insertion.iib_relation = partner_relation;
			insertion.iib_number = -1;
			insertion.iib_duplicates = bitmap;
			insertion.iib_transaction = transaction;
			result =
				check_duplicates(tdbb, record, idx, &insertion, relation);
			if (idx->idx_flags & (idx_primary | idx_unique))
				result = result ? idx_e_foreign : idx_e_ok;
			if (idx->idx_flags & idx_foreign)
				result = result ? idx_e_ok : idx_e_foreign;
			SBM_release(bitmap);
		}
		else if (idx->idx_flags & idx_foreign)
			result = idx_e_foreign;
	}

	return result;
}



static BOOLEAN duplicate_key(UCHAR * record1, UCHAR * record2, IFL ifl_data)
{
/**************************************
 *
 *	d u p l i c a t e _ k e y
 *
 **************************************
 *
 * Functional description
 *	Callback routine for duplicate keys during index creation.  Just
 *	bump a counter.
 *
 **************************************/
	ISR rec1, rec2;

	rec1 = (ISR) (record1 + ifl_data->ifl_key_length);
	rec2 = (ISR) (record2 + ifl_data->ifl_key_length);

	if (!(rec1->isr_flags & (ISR_secondary | ISR_null)) &&
		!(rec2->isr_flags & (ISR_secondary | ISR_null))) {
		++ifl_data->ifl_duplicates;
	}

	return FALSE;
}


static SLONG get_root_page(TDBB tdbb, JRD_REL relation)
{
/**************************************
 *
 *	g e t _ r o o t _ p a g e
 *
 **************************************
 *
 * Functional description
 *	Find the root page for a relation.
 *
 **************************************/
	SLONG page;

	SET_TDBB(tdbb);

	if (!(page = relation->rel_index_root)) {
		DPM_scan_pages(tdbb);
		page = relation->rel_index_root;
	}

	return page;
}


static int index_block_flush(void *ast_object)
{
/**************************************
 *
 *	i n d e x _ b l o c k _ f l u s h
 *
 **************************************
 *
 * Functional description
 *	An exclusive lock has been requested on the
 *	index block.  The information in the cached
 *	index block is no longer valid, so clear it 
 *	out and release the lock.
 *
 **************************************/
	IDB index_block = reinterpret_cast<IDB>(ast_object);
	LCK lock;
	struct tdbb thd_context, *tdbb;

/* Since this routine will be called asynchronously, we must establish
   a thread context. */

	SET_THREAD_DATA;

	lock = index_block->idb_lock;

	if (lock->lck_attachment)
		tdbb->tdbb_database = lock->lck_attachment->att_database;
	tdbb->tdbb_attachment = lock->lck_attachment;
	tdbb->tdbb_quantum = QUANTUM;
	tdbb->tdbb_request = NULL;
	tdbb->tdbb_transaction = NULL;

/* release the index expression request, which also has
   the effect of releasing the expression tree */

	if (index_block->idb_expression_request)
		CMP_release(tdbb, index_block->idb_expression_request);

	index_block->idb_expression_request = NULL;
	index_block->idb_expression = NULL;
	MOVE_CLEAR(&index_block->idb_expression_desc, sizeof(struct dsc));

	LCK_release(tdbb, lock);

/* Restore the prior thread context */

	RESTORE_THREAD_DATA;

	return 0;
}


static IDX_E insert_key(
						TDBB tdbb,
						JRD_REL relation,
						REC record,
						JRD_TRA transaction,
						WIN * window_ptr,
						IIB * insertion,
						JRD_REL * bad_relation,
						USHORT * bad_index)
{
/**************************************
 *
 *	i n s e r t _ k e y
 *
 **************************************
 *
 * Functional description
 *	Insert a key in the index.  
 *	If this is a unique index, check for active duplicates.  
 *	If this is a foreign key, check for duplicates in the
 *	primary key index.
 *
 **************************************/
	IDX_E result;
	IDX *idx;
	KEY key;

	SET_TDBB(tdbb);

	result = idx_e_ok;
	idx = insertion->iib_descriptor;

/* Insert the key into the index.  If the index is unique, BTR
   will keep track of duplicates. */

	insertion->iib_duplicates = NULL;
	BTR_insert(tdbb, window_ptr, insertion);

	if (insertion->iib_duplicates) {
		result = check_duplicates(tdbb, record, idx, insertion, NULL);
		SBM_release(insertion->iib_duplicates);
	}

	if (result != idx_e_ok)
		return result;

/* if we are dealing with a foreign key index, 
   check for an insert into the corresponding 
   primary key index */

	if (idx->idx_flags & idx_foreign) {
		/* find out if there is a null segment by faking uniqueness --
		   if there is one, don't bother to check the primary key */

		idx_null_state null_state;
		idx->idx_flags |= idx_unique;
		CCH_FETCH(tdbb, window_ptr, LCK_read, pag_root);
		result = BTR_key(tdbb, relation, record, idx, &key, &null_state);
		CCH_RELEASE(tdbb, window_ptr);
		idx->idx_flags &= ~idx_unique;
		if (null_state == idx_nulls_none) {
			result =
				check_foreign_key(tdbb, record, insertion->iib_relation,
								  transaction, idx, bad_relation, bad_index);
		}
	}

	return result;
}


static BOOLEAN key_equal(KEY * key1, KEY * key2)
{
/**************************************
 *
 *	k e y _ e q u a l
 *
 **************************************
 *
 * Functional description
 *	Compare two keys for equality.
 *
 **************************************/
	SSHORT l;
	UCHAR *p, *q;

	if ((l = key1->key_length) != key2->key_length)
		return FALSE;

	p = key1->key_data;
	q = key2->key_data;

	if (l)
		do
			if (*p++ != *q++)
				return FALSE;
		while (--l);

	return TRUE;
}


static void signal_index_deletion(TDBB tdbb, JRD_REL relation, USHORT id)
{
/**************************************
 *
 *	s i g n a l _ i n d e x _ d e l e t i o n
 *
 **************************************
 *
 * Functional description
 *	On delete of an index, force all 
 *	processes to get rid of index info.
 *
 **************************************/
	IDB index_block;
	LCK lock = NULL;

	SET_TDBB(tdbb);

/* get an exclusive lock on the associated index
   block (if it exists) to make sure that all other
   processes flush their cached information about
   this index */

	for (index_block = relation->rel_index_blocks; index_block;
		 index_block = index_block->idb_next) if (index_block->idb_id == id) {
			lock = index_block->idb_lock;
			break;
		}

/* if one didn't exist, create it */

	if (!index_block) {
		index_block = IDX_create_index_block(tdbb, relation, id);
		lock = index_block->idb_lock;
	}

/* signal other processes to clear out the index block */

	if (lock->lck_physical == LCK_SR)
		LCK_convert_non_blocking(tdbb, lock, LCK_EX, TRUE);
	else
		LCK_lock_non_blocking(tdbb, lock, LCK_EX, TRUE);

/* and clear out our index block as well */

	index_block_flush(index_block);
}

} // extern "C"
