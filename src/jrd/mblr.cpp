/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		MBLR.C
 *	DESCRIPTION:	RDB DDL Call translator
 *			Translate MBLR byte strings to DYN byte strings.
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
#include "../jrd/gds.h"
#include "../jrd/mblr.h"
#include "../jrd/mblr_proto.h"

#define GET_BYTE(op)	op = (*mblr->met_mblr++); mblr->met_mblr_length--
#define GET_WORD(op)	op = get_word (mblr)
#define PEEK_BYTE	(*mblr->met_mblr)
#define STUFF(dyn)	(*mblr->met_dyn++) = (dyn)
#define STUFF_END	STUFF (gds_dyn_end)
#define STUFF_LITERAL	get_literal (mblr)
#define STUFF_LITERAL_W	get_literal_w (mblr)
#define STUFF_WORD(dyn)	STUFF (dyn); STUFF (dyn >> 8)
#define STUFF_INT(dyn)	STUFF (dyn); STUFF (dyn >> 8); STUFF (dyn >> 16); STUFF (dyn >> 24)

#define IN_FIELD	1
#define IN_INDEX	2
#define IN_RELATION	3
#define IN_VIEW		4

typedef struct met {
	UCHAR met_op;
	UCHAR *met_mblr;
	UCHAR *met_dyn;
	USHORT met_mblr_length;
	USHORT met_dyn_length;
} *MET;

static int area, level, duplicates;

static int field_def(MET);
static int field_descriptor(MET);
static void get_literal(MET);
static void get_literal_w(MET);
static int get_word(MET);
static int index_def(MET);
static int parse_mblr(MET);
static int relation_def(MET);
static int view_def(MET);


int MBLR_translate_to_dyn(
						  USHORT ddl_operation,
						  USHORT mblr_buffer_length,
						  UCHAR * mblr_buffer,
USHORT * dyn_length, UCHAR * dyn_buffer)
{
/************************************************
 *
 *	M B L R _ t r a n s l a t e _ t o _ d y n
 *
 ************************************************
 *
 * Functional description
 *	Translate Rdb/VMS RDB$DDL call MBLR byte-string
 *	to a DYN byte-string.  DYN string is created in
 *	a buffer passed by the caller.  This routine
 *	returns the length of the DYN string, or zero
 *	for any failure.
 *
 ************************************************/
	struct met mblr;
	UCHAR op;
	int result;
	UCHAR *dyn_start;

	level = FALSE;

	if ((ddl_operation != mblr$k_define) &&
		(ddl_operation != mblr$k_modify) && (ddl_operation != mblr$k_delete))
		return FALSE;

	mblr.met_op = ddl_operation;

	mblr.met_mblr = mblr_buffer;
	mblr.met_mblr_length = mblr_buffer_length;

	mblr.met_dyn = dyn_start = dyn_buffer;
	mblr.met_dyn_length = 0;

	op = *mblr.met_mblr++;
	mblr.met_mblr_length--;

	if (op != mblr$k_version_1)
		return NULL;

	*mblr.met_dyn++ = (gds_dyn_version_1);
	*mblr.met_dyn++ = (gds_dyn_begin);

	op = *mblr.met_mblr++;
	mblr.met_mblr_length--;

	switch (op) {
	case mblr$k_relation_def:
		area = IN_RELATION;
		result = relation_def(&mblr);
		break;
	case mblr$k_field_def:
		area = IN_FIELD;
		result = field_def(&mblr);
		break;
	case mblr$k_view_def:
		area = IN_VIEW;
		result = view_def(&mblr);
		break;
	case mblr$k_index_def:
		area = IN_INDEX;
		result = index_def(&mblr);
		break;
	default:
		return FALSE;
	}

	*mblr.met_dyn++ = (gds_dyn_end);
	*mblr.met_dyn++ = (gds_dyn_eoc);

	*dyn_length = mblr.met_dyn - dyn_start;

	return result;

}



static int field_def(MET mblr)
{
/**************************************
 *
 *	f i e l d _ d e f
 *
 **************************************
 *
 * Functional description
 *	Translate a field modify / update / delete.
 *
 **************************************/
	int op, result;

	result = TRUE;

	switch (mblr->met_op) {
	case mblr$k_modify:
		STUFF(gds_dyn_mod_global_fld);
		STUFF_LITERAL;
		while ((PEEK_BYTE != mblr$k_field_end) && result)
			result = parse_mblr(mblr);
		break;

	case mblr$k_define:
		if (level)
			STUFF(gds_dyn_def_local_fld);
		else
			STUFF(gds_dyn_def_global_fld);
		STUFF_LITERAL;
		while ((PEEK_BYTE != mblr$k_field_end) && result)
			result = parse_mblr(mblr);
		break;

	case mblr$k_delete:
		if (area == IN_FIELD)
			STUFF(gds_dyn_delete_global_fld);
		else
			STUFF(gds_dyn_delete_local_fld);
		STUFF_LITERAL;
		break;
	}

	if (level)
		GET_BYTE(op);

	STUFF_END;

	return result;

}



static int index_def(MET mblr)
{
/**************************************
 *
 *	i n d e x _ d e f
 *
 **************************************
 *
 * Functional description
 *	Translate a index modify / update / delete.
 *
 **************************************/
	int result;

	result = TRUE;

	switch (mblr->met_op) {
	case mblr$k_modify:
		result = FALSE;
		break;

	case mblr$k_define:
		duplicates = FALSE;
		STUFF(gds_dyn_def_idx);
		STUFF_LITERAL;
		while ((PEEK_BYTE != mblr$k_index_end) && result)
			result = parse_mblr(mblr);
		STUFF(gds_dyn_idx_inactive);
		STUFF(FALSE);
		if (duplicates == FALSE) {
			STUFF(gds_dyn_idx_unique);
			STUFF_WORD(2);
			STUFF_WORD(TRUE);
		}
		break;

	case mblr$k_delete:
		STUFF(gds_dyn_delete_idx);
		STUFF_LITERAL;
		break;
	}

	STUFF_END;

	return result;

}



static int relation_def(MET mblr)
{
/**************************************
 *
 *	r e l a t i o n _ d e f
 *
 **************************************
 *
 * Functional description
 *	Translate a relation modify / update / delete.
 *
 **************************************/
	int result;

	level = result = TRUE;

	switch (mblr->met_op) {
	case mblr$k_modify:
		STUFF(gds_dyn_mod_rel);
		break;

	case mblr$k_define:
		STUFF(gds_dyn_def_rel);
		break;

	case mblr$k_delete:
		STUFF(gds_dyn_delete_rel);
		break;
	}

	STUFF_LITERAL;

	while ((PEEK_BYTE != mblr$k_relation_end) && result)
		result = parse_mblr(mblr);

	STUFF_END;

	return result;

}



static int view_def(MET mblr)
{
/**************************************
 *
 *	v i e w _ d e f
 *
 **************************************
 *
 * Functional description
 *	Translate a view modify / update / delete.
 *
 **************************************/
	int result;

	result = TRUE;

	switch (mblr->met_op) {
	case mblr$k_modify:
		result = FALSE;
		break;

	case mblr$k_define:
		STUFF(gds_dyn_def_view);
		STUFF_LITERAL;
		while ((PEEK_BYTE != mblr$k_view_end) && result)
			result = parse_mblr(mblr);
		break;

	case mblr$k_delete:
		STUFF(gds_dyn_delete_rel);
		STUFF_LITERAL;
		break;
	}

	STUFF_END;

	return result;
}



static int field_descriptor(MET mblr)
{
/**************************************
 *
 *	f i e l d _ d e s c r i p t o r
 *
 **************************************
 *
 * Functional description
 *	Translate an mblr field description string
 *	from within a define relation statement.
 *
 **************************************/
	int op, type, result;

	result = TRUE;

	GET_BYTE(op);
	STUFF(gds_dyn_fld_type);
	GET_BYTE(type);

	switch (type) {
	case dsc$k_dtype_adt:
		STUFF_WORD(2);
		STUFF_WORD(blr_timestamp);
		break;
	case dsc$k_dtype_b:
		STUFF_WORD(2);
		STUFF_WORD(blr_text);
		STUFF(gds_dyn_fld_length);
		STUFF_WORD(2);
		STUFF_WORD(1);
		break;
	case dsc$k_dtype_f:
	case dsc$k_dtype_g:
		STUFF_WORD(2);
		STUFF_WORD(type);
		break;
	case dsc$k_dtype_l:
	case dsc$k_dtype_q:
	case dsc$k_dtype_w:
		STUFF_WORD(2);
		STUFF_WORD(type);
		STUFF(gds_dyn_fld_scale);
		STUFF_WORD(2);
		GET_BYTE(type);
		STUFF_WORD(type);
		break;
	case dsc$k_dtype_qu:
		STUFF_WORD(2);
		STUFF_WORD(blr_blob);
		break;
	case dsc$k_dtype_t:
		STUFF_WORD(2);
		STUFF_WORD(blr_text);
		STUFF(gds_dyn_fld_length);
		STUFF_WORD(2);
		GET_WORD(type);
		STUFF_WORD(type);
		break;
	case dsc$k_dtype_vt:
		STUFF_WORD(2);
		STUFF_WORD(blr_varying);
		STUFF(gds_dyn_fld_length);
		STUFF_WORD(2);
		GET_WORD(type);
		STUFF_WORD(type);
		break;
	default:
		STUFF_WORD(2);
		STUFF_WORD(blr_short);
		break;
	}

	return result;
}


static void get_literal(MET mblr)
{
/**************************************
 *
 *	g e t _ l i t e r a l
 *
 **************************************
 *
 * Functional description
 *	Get a length prefixed literal (byte)
 *	Note: most MBLR literals use a byte as a length prefix,
 *            DYN literals use a word.
 *
 **************************************/

	int length, op, n;

	GET_BYTE(length);
	n = length;
	STUFF_WORD(length);

	while (--n >= 0) {
		GET_BYTE(op);
		STUFF(op);
	}

	return;
}



static void get_literal_w(MET mblr)
{
/**************************************
 *
 *	g e t _ l i t e r a l_w
 *
 **************************************
 *
 * Functional description
 *	Get a length prefixed literal (word)
 *
 **************************************/
	int length, op, n;

	GET_WORD(length);
	n = length;
	STUFF_WORD(length);

	while (--n >= 0) {
		GET_BYTE(op);
		STUFF(op);
	}

	return;
}




static int get_word(MET mblr)
{
/**************************************
 *
 *	g e t _ w o r d
 *
 **************************************
 *
 * Functional description
 *	Pick up a VAX style word.
 *
 **************************************/
	int op, i;

	GET_BYTE(op);
	GET_BYTE(i);
	op |= i << 8;

	return op;
}



static int parse_mblr(MET mblr)
{
/***************************************************
 *
 *	p a r s e _ m b l r
 *
 ***************************************************
 *
 * Functional description
 *	Primary recursive routine to translate mblr.
 *
 ***************************************************/
	int op, result, save_area;
	UCHAR *save_dyn;

	if (!mblr->met_mblr_length)
		return FALSE;

	op = PEEK_BYTE;

	result = TRUE;

	switch (op) {
	case mblr$k_change_attributes:
	case mblr$k_descriptor:
		result = field_descriptor(mblr);
		break;

	case mblr$k_change_field:
	case mblr$k_delete_field:
	case mblr$k_define_field:
		save_area = area;
		area = IN_FIELD;
		GET_BYTE(op);
		result = field_def(mblr);
		area = save_area;
		break;

	case mblr$k_dtr_def_val:
		GET_BYTE(op);
		STUFF(gds_dyn_fld_default_value);
		STUFF_LITERAL;
		break;

	case mblr$k_dtr_edit_str:
		GET_BYTE(op);
		STUFF(gds_dyn_fld_edit_string);
		STUFF_LITERAL;
		break;

	case mblr$k_dtr_qry_hdr:
		GET_BYTE(op);
		if (PEEK_BYTE == mblr$k_dtr_qry_hdr_line) {
			GET_BYTE(op);
			GET_BYTE(op);
			mblr->met_mblr += op;
			if (PEEK_BYTE != mblr$k_dtr_qry_hdr_end)
				result = FALSE;
		}
		else
			result = FALSE;
		break;

	case mblr$k_duplicates:
		GET_BYTE(op);
		duplicates = TRUE;
		break;

	case mblr$k_field_def:
		GET_BYTE(op);
		result = field_def(mblr);
		break;

	case mblr$k_field_expression:
		GET_BYTE(op);
		STUFF(gds_dyn_fld_computed_blr);
		save_dyn = mblr->met_dyn;
		STUFF_LITERAL_W;
		STUFF(blr_eoc);
		(*save_dyn)++;
		break;

	case mblr$k_field_name:
		GET_BYTE(op);
		if (area == IN_INDEX)
			STUFF(gds_dyn_fld_name);
		else
			STUFF(gds_dyn_fld_source);
		STUFF_LITERAL;
		break;

	case mblr$k_missing_value:
		GET_BYTE(op);
		STUFF(gds_dyn_fld_missing_value);
		save_dyn = mblr->met_dyn;
		STUFF_LITERAL_W;
		STUFF(blr_eoc);
		(*save_dyn)++;
		break;

	case mblr$k_relation_name:
		GET_BYTE(op);
		STUFF(gds_dyn_rel_name);
		STUFF_LITERAL;
		break;

	case mblr$k_rse:
		GET_BYTE(op);
		STUFF(gds_dyn_view_blr);
		save_dyn = mblr->met_dyn;
		STUFF_LITERAL_W;
		STUFF(blr_eoc);
		(*save_dyn)++;
		break;

	case mblr$k_valid_if:
		GET_BYTE(op);
		STUFF(gds_dyn_fld_validation_blr);
		save_dyn = mblr->met_dyn;
		STUFF_LITERAL_W;
		STUFF(blr_eoc);
		(*save_dyn)++;
		break;

	case mblr$k_dtr_qry_nam:
		GET_BYTE(op);
		STUFF(gds_dyn_fld_query_name);
		STUFF_LITERAL;
		break;

	case mblr$k_valid_if_source:
		GET_BYTE(op);
		STUFF(gds_dyn_fld_validation_source);
		STUFF_LITERAL_W;
		break;

	case mblr$k_computed_by_source:
		GET_BYTE(op);
		STUFF(gds_dyn_fld_computed_source);
		STUFF_LITERAL_W;
		break;

	case mblr$k_rse_source:
		GET_BYTE(op);
		STUFF(gds_dyn_view_source);
		STUFF_LITERAL_W;
		break;

	case mblr$k_description_source:
		GET_BYTE(op);
		STUFF(gds_dyn_description);
		STUFF_LITERAL_W;
		break;

	case mblr$k_sub_type:
		GET_BYTE(op);
		STUFF(gds_dyn_fld_sub_type);
		STUFF_WORD(2);
		GET_WORD(op);
		STUFF_WORD(op);
		break;

	case mblr$k_segment_length:
		GET_BYTE(op);
		STUFF(gds_dyn_fld_segment_length);
		STUFF_WORD(2);
		GET_WORD(op);
		STUFF_WORD(op);
		break;

	case mblr$k_field_noupdate:
	case mblr$k_field_update:
		GET_BYTE(op);
		break;

	case mblr$k_index_unknown1:
		GET_BYTE(op);
		break;

	case mblr$k_index_unknown2:
		GET_BYTE(op);
		GET_BYTE(op);
		GET_BYTE(op);
		break;

	case mblr$k_field_end:
	case mblr$k_index_end:
	case mblr$k_relation_end:
	case mblr$k_view_end:
		break;

	case mblr_null:
	case mblr$k_cdd_path:
	case mblr$k_commit:
	case mblr$k_constraint:
	case mblr$k_constraint_count:
	case mblr$k_constraint_def:
	case mblr$k_constraint_end:
	case mblr$k_delete_attributes:
	case mblr$k_dtr_qry_hdr_line:
	case mblr$k_dtr_qry_hdr_end:
	case mblr$k_update:
	default:
		result = FALSE;
		break;

	}

	return result;
}
