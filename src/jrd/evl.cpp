/*
 *      PROGRAM:        JRD Access Method
 *      MODULE:         evl.c
 *      DESCRIPTION:    Expression evaluation
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
  * $Id: evl.cpp,v 1.34.2.8 2004-10-12 15:34:39 hvlad Exp $ 
 */

/*
 * Modified by: Patrick J. P. Griffin
 * Date: 11/24/2000
 * Problem:   select count(0)+1 from rdb$relations where 0=1; returns 0
 *            In the EVL_group processing, the internal assigment for 
 *            the literal in the computation is being done on every 
 *            statement fetch, so if there are no statements fetched
 *            then the internal field never gets set.
 * Change:    Added an assignment process for the literal
 *            before the first fetch.
 *
 * Modified by: Neil McCalden
 * Date: 05 Jan 2001
 * Problem:   Firebird bug: 127375
 *            Group by on a calculated expression would cause segv
 *            when it encountered a NULL value as the calculation
 *            was trying reference a null pointer.
 * Change:    Test the null flag before trying to expand the value.
 *
 * 2001.6.17 Claudio Valderrama: Fix the annoying behavior that causes silent
 *	overflow in dialect 1. If you define the macro FIREBIRD_AVOID_DIALECT1_OVERFLOW
 *	it will work with double should an overflow happen. Otherwise, an error will be
 *	issued to the user if the overflow happens. The multiplication is done using
 *	SINT64 quantities. I had the impression that casting this SINT64 result to double
 *	when we detect overflow was faster than achieving the failed full multiplication
 *	with double operands again. Usage will tell the truth.
 *	For now, the aforementioned macro is enabled.
 * 2001.6.18 Claudio Valderrama: substring() is working with international charsets,
 *	thanks to Dave Schnepper's directions.
 * 2002.2.15 Claudio Valderrama: divide2() should not mangle negative values.
 * 2002.04.16 Paul Beach HP10 Port - (UCHAR*) desc.dsc_address = p; modified for HP 
 *	Compiler
 * 2002.09.28 Dmitry Yemanov: Reworked internal_info stuff, enhanced
 *                            exception handling in SPs/triggers,
 *                            implemented ROWS_AFFECTED system variable
 */

#include "firebird.h"
#include <string.h>
#include "../jrd/jrd.h"
#include "../jrd/val.h"
#include "../jrd/req.h"
#include "../jrd/exe.h"
#include "../jrd/sbm.h"
#include "../jrd/blb.h"
#include "gen/codes.h"
#include "../jrd/scl.h"
#include "../jrd/lck.h"
#include "../jrd/lls.h"
#include "../jrd/intl.h"
#include "../jrd/intl_classes.h"
#include "../jrd/rse.h"
#include "../jrd/quad.h"
#include "../jrd/sort.h"
#include "../jrd/blr.h"
#include "../jrd/tra.h"
#include "../jrd/constants.h"
#include "../jrd/gdsassert.h"
#include "../jrd/jrd_time.h"
#include "../jrd/all_proto.h"
#include "../jrd/bookmark.h"
#include "../jrd/blb_proto.h"
#include "../jrd/btr_proto.h"
#include "../jrd/cvt_proto.h"
#include "../jrd/dpm_proto.h"
#include "../jrd/dsc_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/evl_proto.h"
#include "../jrd/exe_proto.h"
#include "../jrd/fun_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/lck_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/rlck_proto.h"
#include "../jrd/rng_proto.h"
#include "../jrd/rse_proto.h"
#include "../jrd/sbm_proto.h"
#include "../jrd/scl_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/sort_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/align.h"
#include "../jrd/met_proto.h"
#include "../jrd/cvt_proto.h"
#include "../jrd/misc_func_ids.h"
//#include "../jrd/authenticate.h"
#include "../common/config/config.h"

#define TEMP_LENGTH     128

#define TEMP_SIZE(x)	sizeof (x)

#define MAX_INT64_LIMIT	(MAX_SINT64 / 10)
#define MIN_INT64_LIMIT	(MIN_SINT64 / 10)

#ifdef VMS
extern double MTH$CVT_D_G(), MTH$CVT_G_D();
#endif

/*  *** DANGER DANGER WILL ROBINSON ***
 *  add(), multiply(), and divide() all take the same three arguments, but
 *  they don't all take them in the same order.  Be careful out there.
 *  The order should be made to agree as part of the next code cleanup.
 */

static DSC *add(DSC *, JRD_NOD, VLU);
static DSC *add2(DSC *, JRD_NOD, VLU);
static DSC *add_datetime(DSC *, JRD_NOD, VLU);
static DSC *add_sql_date(DSC *, JRD_NOD, VLU);
static DSC *add_sql_time(DSC *, JRD_NOD, VLU);
static DSC *add_timestamp(DSC *, JRD_NOD, VLU);
static DSC *binary_value(TDBB, JRD_NOD, VLU);
static DSC *cast(TDBB, DSC *, JRD_NOD, VLU);
static SSHORT compute_agg_distinct(TDBB, JRD_NOD);
static DSC *concatenate(TDBB, JRD_NOD, VLU);
static DSC *dbkey(TDBB, JRD_NOD, VLU);
static DSC *eval_statistical(TDBB, JRD_NOD, VLU);
static void fini_agg_distinct(TDBB, JRD_NOD);
static SINT64 get_day_fraction(DSC * d);
static DSC *get_mask(TDBB, JRD_NOD, VLU);
static SINT64 get_timestamp_to_isc_ticks(DSC * d);
static SSHORT init_agg_distinct(TDBB, JRD_NOD);
#ifdef PC_ENGINE
static DSC *lock_record(TDBB, JRD_NOD, VLU);
static DSC *lock_relation(TDBB, JRD_NOD, VLU);
#endif
static DSC *lock_state(TDBB, JRD_NOD, VLU);
static DSC *multiply(DSC *, VLU, JRD_NOD);
static DSC *multiply2(DSC *, VLU, JRD_NOD);
static DSC *divide2(DSC *, VLU, JRD_NOD);
static DSC *negate_dsc(TDBB, DSC *, VLU);
static DSC *record_version(TDBB, JRD_NOD, VLU);
static BOOLEAN reject_duplicate(UCHAR *, UCHAR *, int);
static DSC *scalar(TDBB, JRD_NOD, VLU);
static SSHORT sleuth(TDBB, JRD_NOD, DSC *, DSC *);
static BOOLEAN nc_sleuth_check(class TextType*, USHORT, UCHAR *, UCHAR *, UCHAR *,
							   UCHAR *);
static BOOLEAN nc_sleuth_class(class TextType*, USHORT, UCHAR *, UCHAR *, UCHAR);
static BOOLEAN wc_sleuth_check(class TextType*, USHORT, UCS2_CHAR *, UCS2_CHAR *,
						UCS2_CHAR *, UCS2_CHAR *);
static BOOLEAN wc_sleuth_class(class TextType*, USHORT, UCS2_CHAR *, UCS2_CHAR *,
						UCS2_CHAR);
static SSHORT string_boolean(TDBB, JRD_NOD, DSC *, DSC *);
static SSHORT string_function(TDBB, JRD_NOD, SSHORT, UCHAR *, SSHORT, UCHAR *, USHORT);
static DSC *substring(TDBB, VLU, DSC *, USHORT, USHORT);
static DSC *upcase(TDBB, DSC *, VLU);
static DSC *internal_info(TDBB, DSC *, VLU);


static const UCHAR special[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0,	/* $%*+- (dollar, percent, star, plus, minus) */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,	/* ?     (question) */
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* @     (at-sign) */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,	/* [     (open square) */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,	/* ~     (tilde) */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};



#define SECONDS_PER_DAY				(24*60*60)
#define ISC_TICKS_PER_DAY			(((SINT64)SECONDS_PER_DAY)*ISC_TIME_SECONDS_PRECISION)
#define DIALECT_3_TIMESTAMP_SCALE	-9
#define DIALECT_1_TIMESTAMP_SCALE	0

#ifdef SCROLLABLE_CURSORS
static const RSE_GET_MODE g_RSE_get_mode = RSE_get_next;
#else
static const RSE_GET_MODE g_RSE_get_mode = RSE_get_forward;
#endif




DSC *DLL_EXPORT EVL_assign_to(TDBB tdbb, JRD_NOD node)
{
/**************************************
 *
 *      E V L _ a s s i g n _ t o
 *
 **************************************
 *
 * Functional description
 *      Evaluate the descriptor of the
 *      destination node of an assignment.
 *
 **************************************/
	JRD_REQ request;
	VLU impure;
	DSC *desc;
	FMT format;
	JRD_NOD message;
	REC record;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	request = tdbb->tdbb_request;
	impure = (VLU) ((SCHAR *) request + node->nod_impure);

/* The only nodes that can be assigned to are: argument, field and variable. */

	VarInvariantArray *var_invariants;
	MsgInvariantArray *msg_invariants;
	int arg_number;

	switch (node->nod_type) {
	case nod_argument:
		message = node->nod_arg[e_arg_message];
		format = (FMT) message->nod_arg[e_msg_format];
		arg_number = (int)(IPTR)node->nod_arg[e_arg_number];
		desc = &format->fmt_desc[arg_number];
		impure->vlu_desc.dsc_address =
			(UCHAR *) request + message->nod_impure + (int) desc->dsc_address;
		impure->vlu_desc.dsc_dtype = desc->dsc_dtype;
		impure->vlu_desc.dsc_length = desc->dsc_length;
		impure->vlu_desc.dsc_scale = desc->dsc_scale;
		impure->vlu_desc.dsc_sub_type = desc->dsc_sub_type;
		if (IS_DTYPE_ANY_TEXT(desc->dsc_dtype) &&
			((INTL_TTYPE(desc) == ttype_dynamic) ||
			 (INTL_GET_CHARSET(desc) == CS_dynamic))) {
			/* Value is a text value, we're assigning it back to the user
			   process, user process has not specified a subtype, user
			   process specified dynamic translation and the DSC isn't from
			   a 3.3 type request (blr_cstring2 instead of blr_cstring) so
			   convert the charset to the declared charset of the process. */

			INTL_ASSIGN_DSC(&impure->vlu_desc,
							tdbb->tdbb_attachment->att_charset, COLLATE_NONE);
		}
		// Clear out dependent invariants
		msg_invariants = reinterpret_cast<MsgInvariantArray*>(
			message->nod_arg[e_msg_invariants]);
		if (msg_invariants && msg_invariants->getCount() > arg_number) {
			var_invariants = (*msg_invariants)[arg_number];
			if (var_invariants) {
				SLONG *ptr, *end;
				for (ptr = var_invariants->begin(), end = var_invariants->end();
					ptr < end; ptr++)
				{
					reinterpret_cast<VLU>((SCHAR *) request + *ptr)->vlu_flags = 0;
				}
			}
		}
		return &impure->vlu_desc;

	case nod_field:
		// 23-Nov-2003, Nickolay Samofatov. In theory, we should track
		// dependent invariants for fields too, but current engine
		// doesn't seem to be able to produce statements where anomalies 
		// may happen. I checked triggers and insert/update statements.
		// All seem to work fine.
		record =
			request->req_rpb[(int) node->nod_arg[e_fld_stream]].rpb_record;
		EVL_field(0, record, (USHORT) node->nod_arg[e_fld_id],
				  &impure->vlu_desc);
		if (!impure->vlu_desc.dsc_address)
			ERR_post(gds_read_only_field, 0);
		return &impure->vlu_desc;

	case nod_null:
		return NULL;

	case nod_variable:
		// Calculate descriptor
		node = node->nod_arg[e_var_variable];
		impure = (VLU) ((SCHAR *) request + node->nod_impure);
		// Clear out dependent invariants
		var_invariants = reinterpret_cast<VarInvariantArray*>(
			node->nod_arg[e_dcl_invariants]);
		if (var_invariants) {
			SLONG *ptr, *end;
			for (ptr = var_invariants->begin(), end = var_invariants->end();
				ptr < end; ptr++)
			{
				reinterpret_cast<VLU>((SCHAR *) request + *ptr)->vlu_flags = 0;
			}
		}
		return &impure->vlu_desc;

	default:
		BUGCHECK(229);			/* msg 229 EVL_assign_to: invalid operation */
	}
	return NULL;
}


SBM *DLL_EXPORT EVL_bitmap(TDBB tdbb, JRD_NOD node)
{
/**************************************
 *
 *      E V L _ b i t m a p
 *
 **************************************
 *
 * Functional description
 *      Evaluate bitmap valued expression.
 *
 **************************************/

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	switch (node->nod_type) {
	case nod_bit_and:
		return SBM_and(EVL_bitmap(tdbb, node->nod_arg[0]),
					   EVL_bitmap(tdbb, node->nod_arg[1]));

	case nod_bit_or:
		return SBM_or(EVL_bitmap(tdbb, node->nod_arg[0]),
					  EVL_bitmap(tdbb, node->nod_arg[1]));

	case nod_bit_dbkey:
		{
			DSC *desc;
			USHORT id;
			UCHAR *numbers;
			SLONG rel_dbkey;
			INV impure;

			impure = (INV) ((SCHAR *) tdbb->tdbb_request + node->nod_impure);
			SBM_reset(&impure->inv_bitmap);
			desc = EVL_expr(tdbb, node->nod_arg[0]);
			id = 1 + 2 * (USHORT) node->nod_arg[1];
			numbers = desc->dsc_address;
			numbers += id * sizeof(SLONG);
			MOVE_FAST(numbers, &rel_dbkey, sizeof(SLONG));
			rel_dbkey -= 1;
			SBM_set(tdbb, &impure->inv_bitmap, rel_dbkey);
			return &impure->inv_bitmap;
		}

	case nod_index:
		{
			INV impure;

			impure = (INV) ((SCHAR *) tdbb->tdbb_request + node->nod_impure);
			BTR_evaluate(tdbb,
						 reinterpret_cast <
						 irb * >(node->nod_arg[e_idx_retrieval]),
						 &impure->inv_bitmap);
			return &impure->inv_bitmap;
		}

	default:
		BUGCHECK(230);			/* msg 230 EVL_bitmap: invalid operation */
	}
	return NULL;
}


BOOLEAN DLL_EXPORT EVL_boolean(TDBB tdbb, JRD_NOD node)
{
/**************************************
 *
 *      E V L _ b o o l e a n
 *
 **************************************
 *
 * Functional description
 *      Evaluate a boolean.
 *
 **************************************/

	DSC*   desc[2];
	USHORT value;
	SSHORT comparison;
	VLU    impure;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

/* Handle and pre-processing possible for various nodes.  This includes
   evaluating argument and checking NULL flags */

	JRD_REQ request = tdbb->tdbb_request;
	JRD_NOD* ptr = node->nod_arg;

	switch (node->nod_type)
	{
	case nod_contains:
	case nod_starts:
	case nod_matches:
	case nod_like:
	case nod_eql:
	case nod_neq:
	case nod_gtr:
	case nod_geq:
	case nod_lss:
	case nod_leq:
	case nod_between:
	case nod_sleuth:
		{
			JRD_NOD rec_version;
			ULONG flags;
			SSHORT force_equal;

			request->req_flags &= ~req_same_tx_upd;
			force_equal = 0;

			/* Evaluate arguments.  If either is null, result is null, but in
			   any case, evaluate both, since some expressions may later depend
			   on mappings which are developed here */

			rec_version = *ptr;
			desc[0] = EVL_expr(tdbb, *ptr++);
			flags = request->req_flags;
			request->req_flags &= ~req_null;
			request->req_flags &= ~req_clone_data_from_default_clause;
			force_equal |= request->req_flags & req_same_tx_upd;

			desc[1] = EVL_expr(tdbb, *ptr++);

			/* restore preserved NULL state */

			if (flags & req_null)
				request->req_flags |= req_null;

			if (request->req_flags & req_null)
				return FALSE;

			force_equal |= request->req_flags & req_same_tx_upd;

			if (node->nod_flags & nod_comparison) {
				comparison = MOV_compare(desc[0], desc[1]);
			}

			/* If we are checking equality of record_version
			 * and same transaction updated the record,
			 * force equality.
			 */

			if ((rec_version->nod_type == nod_rec_version) && (force_equal)) {
				comparison = 0;
			}

			request->req_flags &= ~(req_null | req_same_tx_upd);
		}
		break;

	case nod_and:
		value = EVL_boolean(tdbb, *ptr++);
		break;

	case nod_or:
		value = EVL_boolean(tdbb, *ptr++);
		break;

	case nod_not:
		if ((*ptr)->nod_type == nod_ansi_any ||
		    (*ptr)->nod_type == nod_ansi_all)
		{
			request->req_flags |= req_ansi_not;
		}
		value = EVL_boolean(tdbb, *ptr++);
		break;

	default:   /* Shut up some compiler warnings */
		break;
	}

/* Evaluate node */

	// TODO: Verify and remove this flag once FB1.5beta3 is out.
	// Default to not eval complete expression (i.e. do short-circuit
	// optimizied evaluation). Both to get possible early warnings from
	// users, and to default to the faster of the two options.
	static bool bEvalCompleteExpression = Config::getCompleteBooleanEvaluation();

	switch (node->nod_type)
	{
	case nod_and:
		{
			/* for and,
			   if either operand is FALSE, then the result is FALSE;
			   if both are TRUE, the result is TRUE;
			   otherwise, the result is NULL

			   op 1            op 2            result
			   ----            ----            ------
			   F               F                F
			   F               T                F
			   F               N                F
			   T               F                F
			   T               T                T
			   T               N                N
			   N               F                F
			   N               T                N
			   N               N                N

			 */

			/* save null state and get other operand */

			const USHORT firstnull = request->req_flags & req_null;
			request->req_flags &= ~req_null;

			if (!bEvalCompleteExpression && !value && !firstnull)
			{
				// First term is FALSE, why the whole expression is false.
				// NULL flag is already turned off a few lines above.
				return FALSE;
			}

			const USHORT value2 = EVL_boolean(tdbb, *ptr);
			const USHORT secondnull = request->req_flags & req_null;
			request->req_flags &= ~req_null;

			if ((!value && !firstnull) || (!value2 && !secondnull)) {
				return FALSE;	/* at least one operand was FALSE */
			}
			else if (value && value2) {
				return TRUE;	/* both true */
			}
			request->req_flags |= req_null;
			return FALSE;		/* otherwise, return null */
		}

	case nod_any:
	case nod_ansi_any:
	case nod_ansi_all:
		{
			USHORT *invariant_flags;
			RSB rsb;
			RSE rse;

			if (node->nod_flags & nod_invariant)
			{
				impure = (VLU) ((SCHAR *) request + node->nod_impure);
				invariant_flags = & impure->vlu_flags;
				if (*invariant_flags & VLU_computed) {
					/* An invariant node has already been computed. */

					if ((node->nod_type == nod_ansi_any) &&
						(*invariant_flags & VLU_null))
					{
						request->req_flags |= req_null;
					}
					else
					{
						request->req_flags &= ~req_null;
					}
					return impure->vlu_misc.vlu_short;
				}
			}

			/* for ansi ANY clauses (and ALL's, which are negated ANY's)
			   the unoptimized boolean expression must be used, since the
			   processing of these clauses is order dependant (see rse.c) */

			rse = (RSE) (node->nod_arg[e_any_rse]);
			rsb = (RSB) (node->nod_arg[e_any_rsb]);
			if (node->nod_type != nod_any)
			{
				rsb->rsb_any_boolean = (JRD_NOD) rse->rse_boolean;
				if (node->nod_type == nod_ansi_any)
					request->req_flags |= req_ansi_any;
				else
					request->req_flags |= req_ansi_all;
			}
			RSE_open(tdbb,
					 reinterpret_cast<struct Rsb*>(node->nod_arg[e_any_rsb]));
			value =
				RSE_get_record(tdbb,
							   reinterpret_cast<struct Rsb*>(node->nod_arg[e_any_rsb]),
							   g_RSE_get_mode);
			RSE_close(tdbb,
					  reinterpret_cast<struct Rsb*>(node->nod_arg[e_any_rsb]));
			if (node->nod_type == nod_any)
				request->req_flags &= ~req_null;

			/* If this is an invariant node, save the return value. */

			if (node->nod_flags & nod_invariant)
			{
				*invariant_flags |= VLU_computed;
				if ((node->nod_type != nod_any) &&
				    (request->req_flags & req_null))
				{
					*invariant_flags |= VLU_null;
				}
				impure->vlu_misc.vlu_short = value;
			}
			return value;
		}

	case nod_contains:
	case nod_starts:
	case nod_matches:
	case nod_like:
		return string_boolean(tdbb, node, desc[0], desc[1]);

	case nod_sleuth:
		return sleuth(tdbb, node, desc[0], desc[1]);

	case nod_missing:
		EVL_expr(tdbb, *ptr);
		if (request->req_flags & req_null) {
			value = TRUE;
			request->req_flags &= ~req_null;
		}
		else {
			if (request->req_flags & req_clone_data_from_default_clause) {
				value = TRUE;
				request->req_flags &= ~req_clone_data_from_default_clause;
			}
			else
				value = FALSE;
		}
		return value;

	case nod_not:
		if (request->req_flags & req_null)
			return FALSE;
		return !value;

	case nod_or:
		{
			/* for or, if either operand is TRUE, then the result is TRUE; if
			   both are FALSE, the result is FALSE; otherwise, the result is NULL

			   op 1            op 2            result
			   ----            ----            ------
			   F               F                F
			   F               T                T
			   F               N                N
			   T               F                T
			   T               T                T
			   T               N                T
			   N               F                N
			   N               T                T
			   N               N                N


			   also, preserve first operand's value and null state, but still
			   evaluate second operand, since latter field mappings may
			   depend on the evaluation */

			const ULONG flags = request->req_flags;
			request->req_flags &= ~req_null;

			if (!bEvalCompleteExpression && value)
			{
				// First term is TRUE, why the whole expression is true.
				// NULL flag is already turned off a few lines above.
				return TRUE;
			}
			
			const USHORT value2 = EVL_boolean(tdbb, *ptr);
			if (value || value2) {
				request->req_flags &= ~req_null;
				return TRUE;
			}

			/* restore saved NULL state */

			if (flags & req_null) {
				request->req_flags |= req_null;
			}
			return FALSE;
		}

	case nod_unique:
		{
			USHORT* invariant_flags;

			if (node->nod_flags & nod_invariant)
			{
				impure = (VLU) ((SCHAR *) request + node->nod_impure);
				invariant_flags = & impure->vlu_flags;
				if (*invariant_flags & VLU_computed)
				{
					/* An invariant node has already been computed. */

					if (*invariant_flags & VLU_null)
						request->req_flags |= req_null;
					else
						request->req_flags &= ~req_null;
					return impure->vlu_misc.vlu_short;
				}
			}

			RSE_open(tdbb, reinterpret_cast<Rsb*>(node->nod_arg[e_any_rsb]));
			value =
				RSE_get_record(tdbb,
							   reinterpret_cast<Rsb*>(node->nod_arg[e_any_rsb]),
							   g_RSE_get_mode);
			if (value)
			{
				value =
				!RSE_get_record(tdbb,
								reinterpret_cast<Rsb*>(node->nod_arg[e_any_rsb]),
								g_RSE_get_mode);
			}
			RSE_close(tdbb, reinterpret_cast<Rsb*>(node->nod_arg[e_any_rsb]));

			/* If this is an invariant node, save the return value. */

			if (node->nod_flags & nod_invariant)
			{
				*invariant_flags |= VLU_computed;
				if (request->req_flags & req_null) {
					*invariant_flags |= VLU_null;
				}
				impure->vlu_misc.vlu_short = value;
			}
			return value;
		}

	case nod_eql:
		return ((comparison == 0) ? TRUE : FALSE);
	case nod_neq:
		return ((comparison != 0) ? TRUE : FALSE);
	case nod_gtr:
		return ((comparison > 0) ? TRUE : FALSE);
	case nod_geq:
		return ((comparison >= 0) ? TRUE : FALSE);
	case nod_lss:
		return ((comparison < 0) ? TRUE : FALSE);
	case nod_leq:
		return ((comparison <= 0) ? TRUE : FALSE);

	case nod_between:
		desc[1] = EVL_expr(tdbb, node->nod_arg[2]);
		if (request->req_flags & req_null)
			return FALSE;
		return (comparison >= 0 && MOV_compare(desc[0], desc[1]) <= 0) ?
			TRUE : FALSE;

	default:
		BUGCHECK(231);			/* msg 231 EVL_boolean: invalid operation */
	}
	return FALSE;
}


DSC* DLL_EXPORT EVL_expr(TDBB tdbb, JRD_NOD node)
{
/**************************************
 *
 *      E V L _ e x p r
 *
 **************************************
 *
 * Functional description
 *      Evaluate a value expression.
 *
 **************************************/
	JRD_REQ request;
	VLU impure;

	DEV_BLKCHK(node, type_nod);

	if (!node)
		BUGCHECK(303);			/* msg 303 Invalid expression for evaluation */

	SET_TDBB(tdbb);

	request = tdbb->tdbb_request;
	impure = (VLU) ((SCHAR *) request + node->nod_impure);
	request->req_flags &= ~req_null;
	request->req_flags &= ~req_clone_data_from_default_clause;

/* Do a preliminary screen for either simple nodes or nodes that
   are special cased elsewhere */

	switch (node->nod_type) {
	case nod_add:
	case nod_subtract:
	case nod_divide:
	case nod_multiply:
	case nod_add2:
	case nod_subtract2:
	case nod_divide2:
	case nod_multiply2:
		return binary_value(tdbb, node, impure);

	case nod_argument:
		{
			FMT format;
			JRD_NOD message;
			DSC *desc;

			if (node->nod_arg[e_arg_flag]) {
				desc = EVL_expr(tdbb, node->nod_arg[e_arg_flag]);
				if (MOV_get_long(desc, 0)) {
					request->req_flags |= req_null;
				}
			}
			message = node->nod_arg[e_arg_message];
			format = (FMT) message->nod_arg[e_msg_format];
			desc = &format->fmt_desc[(int) node->nod_arg[e_arg_number]];

			impure->vlu_desc.dsc_address = (UCHAR *) request +
				message->nod_impure + (int) desc->dsc_address;
			impure->vlu_desc.dsc_dtype = desc->dsc_dtype;
			impure->vlu_desc.dsc_length = desc->dsc_length;
			impure->vlu_desc.dsc_scale = desc->dsc_scale;
			impure->vlu_desc.dsc_sub_type = desc->dsc_sub_type;
			return &impure->vlu_desc;
		}

	case nod_concatenate:
		return concatenate(tdbb, node, impure);

	case nod_dbkey:
		return dbkey(tdbb, node, impure);

	case nod_rec_version:
		return record_version(tdbb, node, impure);

	case nod_field:
		{
			REC record =
				request->req_rpb[(int)node->nod_arg[e_fld_stream]].rpb_record;
			/* In order to "map a null to a default" value (in EVL_field()), 
			 * the relation block is referenced. 
			 * Reference: Bug 10116, 10424 
			 */
			if (!EVL_field(request->req_rpb[(USHORT) node->nod_arg[e_fld_stream]].rpb_relation,
							record,
							(USHORT) node->nod_arg[e_fld_id],
							&impure->vlu_desc))
			{
				request->req_flags |= req_null;
			}
			return &impure->vlu_desc;
		}

	case nod_function:
		FUN_evaluate(reinterpret_cast<fun*>(node->nod_arg[e_fun_function]),
				     node->nod_arg[e_fun_args], impure);
		/*request->req_flags |= req_null; THIS IS A TEST ONLY.
		return NULL;*/
		return &impure->vlu_desc;

	case nod_literal:
		return &((LIT) node)->lit_desc;

	case nod_lock_state:
		return lock_state(tdbb, node, impure);
#ifdef PC_ENGINE
	case nod_lock_record:
		return lock_record(tdbb, node, impure);

	case nod_lock_relation:
		return lock_relation(tdbb, node, impure);

	case nod_begin_range:
		return RNG_begin(node, impure);
#endif
	case nod_null:
		request->req_flags |= req_null;
		return NULL;

	case nod_prot_mask:
		return get_mask(tdbb, node, impure);

	case nod_current_time:
	case nod_current_date:
	case nod_current_timestamp:
		{
			time_t clock;
			struct tm times;
			GDS_TIMESTAMP enc_times;

			/* Use the request timestamp, if there is one.  Otherwise
			   fetch the current clock in order to keep running */

			if (request->req_timestamp)
				clock = request->req_timestamp;
			else {
				assert(FALSE);
				clock = time(0);
			}
			times = *localtime(&clock);

			memset(&impure->vlu_desc, 0, sizeof(impure->vlu_desc));
			impure->vlu_desc.dsc_address =
				(UCHAR *) & impure->vlu_misc.vlu_timestamp;
			if (node->nod_type == nod_current_time)
				times.tm_year = times.tm_mon = times.tm_mday = 0;
			else if (node->nod_type == nod_current_date)
				times.tm_hour = times.tm_min = times.tm_sec = 0;
			isc_encode_timestamp(&times, &enc_times);
			switch (node->nod_type) {
			case nod_current_time:
				impure->vlu_desc.dsc_dtype = dtype_sql_time;
				impure->vlu_desc.dsc_length = type_lengths[dtype_sql_time];
				*(ULONG *) impure->vlu_desc.dsc_address =
					enc_times.timestamp_time;
				break;
			case nod_current_date:
				impure->vlu_desc.dsc_dtype = dtype_sql_date;
				impure->vlu_desc.dsc_length = type_lengths[dtype_sql_date];
				*(ULONG *) impure->vlu_desc.dsc_address =
					enc_times.timestamp_date;
				break;
			case nod_current_timestamp:
				impure->vlu_desc.dsc_dtype = dtype_timestamp;
				impure->vlu_desc.dsc_length = type_lengths[dtype_timestamp];
				*((GDS_TIMESTAMP *) impure->vlu_desc.dsc_address) = enc_times;
				break;
			default:
				assert(FALSE);
				break;
			}
		}
		return &impure->vlu_desc;

	case nod_user_name:
		impure->vlu_desc.dsc_dtype = dtype_text;
		impure->vlu_desc.dsc_sub_type = 0;
		impure->vlu_desc.dsc_scale = 0;
		INTL_ASSIGN_TTYPE(&impure->vlu_desc, ttype_metadata);
		if (tdbb->tdbb_attachment->att_user)
			impure->vlu_desc.dsc_address =
				(UCHAR *) tdbb->tdbb_attachment->att_user->usr_user_name;
		if (impure->vlu_desc.dsc_address != NULL)
		{
			impure->vlu_desc.dsc_length =
				strlen(reinterpret_cast<const char*>(
					impure->vlu_desc.dsc_address));
		}
		else {
			impure->vlu_desc.dsc_length = 0;
		}
		return &impure->vlu_desc;

	/* CVC: Current role will get a validated role; IE one that exists. */
	case nod_current_role:
		impure->vlu_desc.dsc_dtype = dtype_text;
		impure->vlu_desc.dsc_sub_type = 0;
		impure->vlu_desc.dsc_scale = 0;
		INTL_ASSIGN_TTYPE(&impure->vlu_desc, ttype_metadata);
		if (tdbb->tdbb_attachment->att_user)
		{
			impure->vlu_desc.dsc_address =
				(UCHAR*) tdbb->tdbb_attachment->att_user->usr_sql_role_name;
		}
		if (impure->vlu_desc.dsc_address != NULL)
		{
			impure->vlu_desc.dsc_length =
				strlen(reinterpret_cast<const char*>(
					impure->vlu_desc.dsc_address));
		}
		else {
			impure->vlu_desc.dsc_length = 0;
		}
		return &impure->vlu_desc;

	case nod_extract:
		{
			DSC *value;
			struct tm times;
			USHORT part;
			GDS_TIMESTAMP timestamp;
			ULONG extract_part;

			impure = (VLU) ((SCHAR *) request + node->nod_impure);
			extract_part = (ULONG) node->nod_arg[e_extract_part];
			value = EVL_expr(tdbb, node->nod_arg[e_extract_value]);

			impure->vlu_desc.dsc_dtype = dtype_short;
			impure->vlu_desc.dsc_scale = 0;
			impure->vlu_desc.dsc_address =
				reinterpret_cast<UCHAR*>(&impure->vlu_misc.vlu_short);
			impure->vlu_desc.dsc_length = sizeof(SSHORT);
			if (!value || (request->req_flags & req_null)) {
				request->req_flags |= req_null;
				impure->vlu_misc.vlu_short = 0;
				return &impure->vlu_desc;
			}

			switch (value->dsc_dtype) {
			case dtype_sql_time:
				timestamp.timestamp_time = *(GDS_TIME *) value->dsc_address;
				timestamp.timestamp_date = 0;
				isc_decode_timestamp(&timestamp, &times);
				if (extract_part != blr_extract_hour &&
					extract_part != blr_extract_minute &&
					extract_part != blr_extract_second)
						ERR_post(gds_expression_eval_err, 0);
				break;
			case dtype_sql_date:
				timestamp.timestamp_date = *(GDS_DATE *) value->dsc_address;
				timestamp.timestamp_time = 0;
				isc_decode_timestamp(&timestamp, &times);
				if (extract_part == blr_extract_hour ||
					extract_part == blr_extract_minute ||
					extract_part == blr_extract_second)
						ERR_post(gds_expression_eval_err, 0);
				break;
			case dtype_timestamp:
				timestamp = *((GDS_TIMESTAMP *) value->dsc_address);
				isc_decode_timestamp(&timestamp, &times);
				break;
			default:
				ERR_post(gds_expression_eval_err, 0);
				break;
			}

			switch (extract_part) {
			case blr_extract_year:
				part = times.tm_year + 1900;
				break;
			case blr_extract_month:
				part = times.tm_mon + 1;
				break;
			case blr_extract_day:
				part = times.tm_mday;
				break;
			case blr_extract_hour:
				part = times.tm_hour;
				break;
			case blr_extract_minute:
				part = times.tm_min;
				break;
			case blr_extract_second:
				impure->vlu_desc.dsc_dtype = dtype_long;
				impure->vlu_desc.dsc_scale = ISC_TIME_SECONDS_PRECISION_SCALE;
				impure->vlu_desc.dsc_address =
					reinterpret_cast < UCHAR * >(&impure->vlu_misc.vlu_long);
				impure->vlu_desc.dsc_length = sizeof(ULONG);
				*(ULONG *) impure->vlu_desc.dsc_address =
					times.tm_sec * ISC_TIME_SECONDS_PRECISION +
					(timestamp.timestamp_time % ISC_TIME_SECONDS_PRECISION);
				return &impure->vlu_desc;
			case blr_extract_yearday:
				part = times.tm_yday;
				break;
			case blr_extract_weekday:
				part = times.tm_wday;
				break;
			default:
				assert(FALSE);
				part = 0;
			}
			*(USHORT *) impure->vlu_desc.dsc_address = part;
		}
		return &impure->vlu_desc;

	case nod_max:
	case nod_min:
	case nod_count:
	case nod_count2:
	case nod_average:
	case nod_average2:
	case nod_total:
	case nod_from:
		return eval_statistical(tdbb, node, impure);

	case nod_scalar:
		return scalar(tdbb, node, impure);

	case nod_variable:
		node = node->nod_arg[e_var_variable];
		impure = (VLU) ((SCHAR *) request + node->nod_impure);
		if (impure->vlu_desc.dsc_flags & DSC_null)
			request->req_flags |= req_null;
		return &impure->vlu_desc;

	case nod_value_if:
		return EVL_expr(tdbb, (EVL_boolean(tdbb, node->nod_arg[0])) ?
						node->nod_arg[1] : node->nod_arg[2]);

#ifdef PC_ENGINE
	case nod_crack:
		{
			RSB rsb;
			IRSB irsb;

			rsb = *(RSB *) node->nod_arg[1];
			if (rsb->rsb_type == rsb_boolean)
				rsb = rsb->rsb_next;
			irsb = (IRSB) ((UCHAR *) request + rsb->rsb_impure);
			impure->vlu_desc.dsc_address =
				(UCHAR *) & impure->vlu_misc.vlu_long;
			impure->vlu_desc.dsc_dtype = dtype_long;
			impure->vlu_desc.dsc_length = sizeof(ULONG);
			impure->vlu_desc.dsc_scale = 0;
			impure->vlu_misc.vlu_long =
				irsb->irsb_flags & (irsb_bof | irsb_eof | irsb_crack);
			return &impure->vlu_desc;
		}

	case nod_get_bookmark:
		{
			BKM bookmark;

			bookmark =
				RSE_get_bookmark(tdbb, *(RSB *) node->nod_arg[e_getmark_rsb]);
			return &bookmark->bkm_desc;
		}

	case nod_bookmark:
		{
			BKM bookmark;

			bookmark = BKM_lookup(node->nod_arg[e_bookmark_id]);
			return &bookmark->bkm_key_desc;
		}

	case nod_cardinality:
		impure->vlu_misc.vlu_long =
			(*(RSB *) node->nod_arg[e_card_rsb])->rsb_cardinality;
		impure->vlu_desc.dsc_dtype = dtype_long;
		impure->vlu_desc.dsc_length = sizeof(ULONG);
		impure->vlu_desc.dsc_scale = 0;
		impure->vlu_desc.dsc_sub_type = 0;
		impure->vlu_desc.dsc_address = (UCHAR *) & impure->vlu_misc.vlu_long;
		return &impure->vlu_desc;
#endif
        default:   /* Shut up some compiler warnings */
            break;
	}

	{
/* Evaluate arguments */

		DSC *values[3];

		if (node->nod_count) {
			JRD_NOD *ptr, *end;
			DSC **v;

			for (ptr = node->nod_arg, end = ptr + node->nod_count, v = values;
				 ptr < end;) {
				*v++ = EVL_expr(tdbb, *ptr++);
				if (request->req_flags & req_null)
					return NULL;
			}
		}

		switch (node->nod_type) {
		case nod_gen_id:		/* return a 32-bit generator value */
			impure->vlu_misc.vlu_long = (SLONG) DPM_gen_id(tdbb,
														   (SLONG)
														   node->nod_arg
														   [e_gen_id], 0,
														   MOV_get_int64
														   (values[0], 0));
			impure->vlu_desc.dsc_dtype = dtype_long;
			impure->vlu_desc.dsc_length = sizeof(SLONG);
			impure->vlu_desc.dsc_scale = 0;
			impure->vlu_desc.dsc_sub_type = 0;
			impure->vlu_desc.dsc_address =
				(UCHAR *) & impure->vlu_misc.vlu_long;
			return &impure->vlu_desc;

		case nod_gen_id2:
			impure->vlu_misc.vlu_int64 = DPM_gen_id(tdbb,
													(SLONG)
													node->nod_arg[e_gen_id],
													0,
													MOV_get_int64(values[0],
																  0));
			impure->vlu_desc.dsc_dtype = dtype_int64;
			impure->vlu_desc.dsc_length = sizeof(SINT64);
			impure->vlu_desc.dsc_scale = 0;
			impure->vlu_desc.dsc_sub_type = 0;
			impure->vlu_desc.dsc_address =
				(UCHAR *) & impure->vlu_misc.vlu_int64;
			return &impure->vlu_desc;

		case nod_negate:
			return negate_dsc(tdbb, values[0], impure);

		case nod_substr:
			return substring(tdbb, impure,
							 values[0],
							 (SSHORT) MOV_get_long(values[1], 0),
							 (SSHORT) MOV_get_long(values[2], 0));

		case nod_upcase:
			return upcase(tdbb, values[0], impure);

		case nod_cast:
			return cast(tdbb, values[0], node, impure);

		case nod_internal_info:
			return internal_info(tdbb, values[0], impure);

		default:
			BUGCHECK(232);		/* msg 232 EVL_expr: invalid operation */
		}
	}
	return NULL;
}


BOOLEAN DLL_EXPORT EVL_field(JRD_REL relation,
							 REC record,
							 USHORT id, DSC * desc)
{
/**************************************
 *
 *      E V L _ f i e l d
 *
 **************************************
 *
 * Functional description
 *      Evaluate a field by filling out a descriptor.
 *
 **************************************/

	DEV_BLKCHK(record, type_rec);

	if (!record) {
		ERR_warning(gds_no_cur_rec, 0);
		return FALSE;
	}

	FMT format = record->rec_format;

	if (format) {
		*desc = format->fmt_desc[id];
	}

/*
	dimitr: fixed bug SF #562417

	AFAIU, there was an assumption here that if a field descriptor is not
	filled, then a field doesn't exist. This is not true, because in fact
	an empty string has dsc_length = 0, and being part of an aggregate it
	becomes nod_field with zero length, hence we had NULL as a result.

	if (!format || id >= format->fmt_count || !desc->dsc_length)
*/
	if (!format || id >= format->fmt_count || !desc->dsc_dtype)
	{
	/* Map a non-existent field to a default value, if available.
		 * This enables automatic format upgrade for data rows.
		 * Handle Outer Joins and such specially!
		 * Reference: Bug 10424, 10116
		 */

		/* rec_format == NULL indicates we're performing a 
		   join-to-null operation for outer joins */

		if (record && record->rec_format && relation)
		{
			/* A database sweep does not scan a relation's metadata. However
			 * the change to substitute a default value for a missing "not null" 
			 * field makes it necessary to reference the field block. 
			 */
			if (!relation->rel_fields)
			{
				TDBB tdbb = NULL;

				SET_TDBB(tdbb);
				MET_scan_relation(tdbb, relation);
			}

			JRD_FLD temp_field =
				reinterpret_cast<jrd_fld*>((jrd_fld*)(*relation->rel_fields)[id]);

			if (temp_field)
			{
				if (temp_field->fld_default_value && temp_field->fld_not_null)
				{
					const NOD_T temp_nod_type =
						temp_field->fld_default_value->nod_type;

					if (temp_nod_type == nod_user_name ||
					    temp_nod_type == nod_current_role)
					{
						desc->dsc_dtype    = dtype_text;
						desc->dsc_sub_type = 0;
						desc->dsc_scale    = 0;
						INTL_ASSIGN_TTYPE(desc, ttype_metadata);
						desc->dsc_address =
							(UCHAR *) relation->rel_owner_name;
						desc->dsc_length =
							strlen(reinterpret_cast<const char*>(desc->dsc_address));
						return TRUE;
					}

					if (temp_nod_type == nod_current_date ||
					    temp_nod_type == nod_current_time ||
						temp_nod_type == nod_current_timestamp)
					{
						static const GDS_TIMESTAMP temp_timestamp = { 0, 0 };
						desc->dsc_dtype = dtype_timestamp;
						desc->dsc_scale = 0;
						desc->dsc_flags = 0;
						desc->dsc_address =
							reinterpret_cast<UCHAR*>(
								const_cast<ISC_TIMESTAMP*>(&temp_timestamp));
						desc->dsc_length = sizeof(temp_timestamp);
						return TRUE;
					}
					else
					{
						LIT default_literal =
							reinterpret_cast<lit*>(temp_field->fld_default_value);

						if (default_literal->nod_type == nod_null)
						{
							ERR_post(gds_not_valid,
									 gds_arg_string, temp_field->fld_name,
									 gds_arg_string, "*** null ***", 0);
						}

						assert(default_literal->nod_type == nod_literal);

						DSC* default_desc = &default_literal->lit_desc;
						desc->dsc_dtype    = default_desc->dsc_dtype;
						desc->dsc_scale    = default_desc->dsc_scale;
						desc->dsc_length   = default_desc->dsc_length;
						desc->dsc_sub_type = default_desc->dsc_sub_type;
						desc->dsc_flags    = default_desc->dsc_flags;
						desc->dsc_address  = default_desc->dsc_address;
						return TRUE;
					}
				}
				else
				{
					desc->dsc_dtype = dtype_text;
					desc->dsc_length = 1;
					desc->dsc_sub_type = 0;
					desc->dsc_scale = 0;
					desc->dsc_ttype = ttype_ascii;
					desc->dsc_address = (UCHAR *) " ";
					return FALSE;
				}
			}
		}
		else
		{
			desc->dsc_dtype = dtype_text;
			desc->dsc_length = 1;
			desc->dsc_sub_type = 0;
			desc->dsc_scale = 0;
			desc->dsc_ttype = ttype_ascii;
			desc->dsc_address = (UCHAR *) " ";
			return FALSE;
		}
	}

/* If the offset of the field is 0, the field can't possible exist */

	if (!desc->dsc_address) {
		return FALSE;
	}

	desc->dsc_address = record->rec_data + (int) desc->dsc_address;

	if (TEST_NULL(record, id)) {
		desc->dsc_flags |= DSC_null;
		return FALSE;
	}
	else {
		desc->dsc_flags &= ~DSC_null;
		return TRUE;
	}
}


USHORT DLL_EXPORT EVL_group(TDBB tdbb, BLK rsb, JRD_NOD node, USHORT state)
{
/**************************************
 *
 *      E V L _ g r o u p
 *
 **************************************
 *
 * Functional description
 *      Compute the next aggregated record of a value group.  EVL_group
 *      is driven by, and returns, a state variable.  The values of the
 *      state are:
 *
 *              3       Entering EVL group beforing fetching the first record.
 *              1       Values are pending from a prior fetch
 *              2       We encountered EOF from the last attempted fetch
 *              0       We processed everything now process (EOF)
 *
 **************************************/
	JRD_REQ request;
	JRD_NOD group, map, *ptr, *end, from, field;
	DSC temp, *desc;
	VLUX impure;
	struct vlu vtemp;
	REC record;
	USHORT id;
	SLONG result;
	double d;
	SINT64 i;
	ASB asb;
	IASB asb_impure;
	UCHAR *data;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

/* if we found the last record last time, we're all done  */

	if (state == 2)
		return 0;

	vtemp.vlu_string = NULL;
	request = tdbb->tdbb_request;
	map = node->nod_arg[e_agg_map];
	group = node->nod_arg[e_agg_group];

	try {

/* Initialize the aggregate record */

	for (ptr = map->nod_arg, end = ptr + map->nod_count; ptr < end; ptr++) {
		from = (*ptr)->nod_arg[e_asgn_from];
		impure = (VLUX) ((SCHAR *) request + from->nod_impure);
		impure->vlux_count = 0;
		switch (from->nod_type) {
		case nod_agg_average:
		case nod_agg_average_distinct:
			impure->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
			impure->vlu_desc.dsc_length = sizeof(double);
			impure->vlu_desc.dsc_sub_type = 0;
			impure->vlu_desc.dsc_scale = 0;
			impure->vlu_desc.dsc_address =
				(UCHAR *) & impure->vlu_misc.vlu_double;
			impure->vlu_misc.vlu_double = 0;
			if (from->nod_type == nod_agg_average_distinct)
				/* Initialize a sort to reject duplicate values */
				init_agg_distinct(tdbb, from);
			break;

		case nod_agg_average2:
		case nod_agg_average_distinct2:
			/* Initialize the result area as an int64.  If the field being
			   averaged is approximate numeric, the first call to add2 will
			   convert the descriptor to double. */
			impure->vlu_desc.dsc_dtype = dtype_int64;
			impure->vlu_desc.dsc_length = sizeof(SINT64);
			impure->vlu_desc.dsc_sub_type = 0;
			impure->vlu_desc.dsc_scale = from->nod_scale;
			impure->vlu_desc.dsc_address =
				(UCHAR *) & impure->vlu_misc.vlu_int64;
			impure->vlu_misc.vlu_int64 = 0;
			if (from->nod_type == nod_agg_average_distinct2)
				/* Initialize a sort to reject duplicate values */
				init_agg_distinct(tdbb, from);
			break;

		case nod_agg_total:
		case nod_agg_total_distinct:
			impure->vlu_desc.dsc_dtype = dtype_long;
			impure->vlu_desc.dsc_length = sizeof(SLONG);
			impure->vlu_desc.dsc_sub_type = 0;
			impure->vlu_desc.dsc_scale = 0;
			impure->vlu_desc.dsc_address =
				(UCHAR *) & impure->vlu_misc.vlu_long;
			impure->vlu_misc.vlu_long = 0;
			if (from->nod_type == nod_agg_total_distinct)
				/* Initialize a sort to reject duplicate values */
				init_agg_distinct(tdbb, from);
			break;

		case nod_agg_total2:
		case nod_agg_total_distinct2:
			/* Initialize the result area as an int64.  If the field being
			   averaged is approximate numeric, the first call to add2 will
			   convert the descriptor to double. */
			impure->vlu_desc.dsc_dtype = dtype_int64;
			impure->vlu_desc.dsc_length = sizeof(SINT64);
			impure->vlu_desc.dsc_sub_type = 0;
			impure->vlu_desc.dsc_scale = from->nod_scale;
			impure->vlu_desc.dsc_address =
				(UCHAR *) & impure->vlu_misc.vlu_int64;
			impure->vlu_misc.vlu_int64 = 0;
			if (from->nod_type == nod_agg_total_distinct2)
				/* Initialize a sort to reject duplicate values */
				init_agg_distinct(tdbb, from);
			break;

		case nod_agg_min:
		case nod_agg_min_indexed:
		case nod_agg_max:
		case nod_agg_max_indexed:
			impure->vlu_desc.dsc_dtype = 0;
			break;

		case nod_agg_count:
		case nod_agg_count2:
		case nod_agg_count_distinct:
			impure->vlu_desc.dsc_dtype = dtype_long;
			impure->vlu_desc.dsc_length = sizeof(SLONG);
			impure->vlu_desc.dsc_sub_type = 0;
			impure->vlu_desc.dsc_scale = 0;
			impure->vlu_desc.dsc_address =
				(UCHAR *) & impure->vlu_misc.vlu_long;
			impure->vlu_misc.vlu_long = 0;
			if (from->nod_type == nod_agg_count_distinct)
				/* Initialize a sort to reject duplicate values */
				init_agg_distinct(tdbb, from);
			break;

		case nod_literal:		/* pjpg 20001124 */
			EXE_assignment(tdbb, *ptr);
			break;

                default:    /* Shut up some compiler warnings */
                    break;
		}
	}

/* If there isn't a record pending, open the stream and get one */

	if ((state == 0) || (state == 3))
	{
		RSE_open(tdbb, reinterpret_cast<struct Rsb*>(rsb));
		if (!RSE_get_record(tdbb, reinterpret_cast<struct Rsb*>(rsb), g_RSE_get_mode))
		{
			if (group) {
				fini_agg_distinct(tdbb, node);
				return 0;
			}
			state = 2;
		}
	}

	if (group)
		for (ptr = group->nod_arg, end = ptr + group->nod_count; ptr < end;
			 ptr++) {
			from = *ptr;
			impure = (VLUX) ((SCHAR *) request + from->nod_impure);
			desc = EVL_expr(tdbb, from);
			if (request->req_flags & req_null)
				impure->vlu_desc.dsc_address = NULL;
			else
				EVL_make_value(tdbb, desc,
							   reinterpret_cast < vlu * >(impure));
		}

/* Loop thru records until either a value change or EOF */

	while (state != 2) {
		state = 1;

		/* in the case of a group by, look for a change in value of any of 
		   the columns; if we find one, stop aggregating and return what we have */

		if (group)
		{
			for (ptr = group->nod_arg, end = ptr + group->nod_count;
				 ptr < end; ptr++)
			{
				from = *ptr;
				impure = (VLUX) ((SCHAR *) request + from->nod_impure);
				if (impure->vlu_desc.dsc_address)
					EVL_make_value(tdbb, &impure->vlu_desc, &vtemp);
				else
					vtemp.vlu_desc.dsc_address = NULL;
				desc = EVL_expr(tdbb, from);
				if (request->req_flags & req_null) {
					impure->vlu_desc.dsc_address = NULL;
					if (vtemp.vlu_desc.dsc_address)
						goto break_out;
				}
				else {
					EVL_make_value(tdbb, desc,
								   reinterpret_cast < vlu * >(impure));
					if (!vtemp.vlu_desc.dsc_address
						|| MOV_compare(&vtemp.vlu_desc, desc))
						goto break_out;
				}
			}
		}

		/* go through and compute all the aggregates on this record */

		for (ptr = map->nod_arg, end = ptr + map->nod_count; ptr < end; ptr++)
		{
			from = (*ptr)->nod_arg[e_asgn_from];
			impure = (VLUX) ((SCHAR *) request + from->nod_impure);
			switch (from->nod_type)
			{
			case nod_agg_min:
			case nod_agg_min_indexed:
			case nod_agg_max:
			case nod_agg_max_indexed:
				desc = EVL_expr(tdbb, from->nod_arg[0]);
				if (request->req_flags & req_null) {
					break;
				}
				++impure->vlux_count;
				if (!impure->vlu_desc.dsc_dtype)
				{
					EVL_make_value(tdbb, desc,
								   reinterpret_cast<vlu*>(&impure->vlu_desc));
					break;
				}
				result =
					MOV_compare(desc, reinterpret_cast<dsc*>(impure));

				if ((result > 0 &&
				     (from->nod_type == nod_agg_max ||
				      from->nod_type == nod_agg_max_indexed)) ||
				    (result < 0 &&
				     (from->nod_type == nod_agg_min ||
				      from->nod_type == nod_agg_min_indexed)))
				{
					EVL_make_value(tdbb, desc, reinterpret_cast<vlu*>(impure));
				}

				/* if a max or min has been mapped to an index, then the first record is the eof */

				if (from->nod_type == nod_agg_max_indexed ||
					from->nod_type == nod_agg_min_indexed) state = 2;
				break;

			case nod_agg_total:
			case nod_agg_average:
				desc = EVL_expr(tdbb, from->nod_arg[0]);
				if (request->req_flags & req_null)
					break;
				++impure->vlux_count;
				add(desc, from, reinterpret_cast < vlu * >(impure));
				break;

			case nod_agg_total2:
			case nod_agg_average2:
				desc = EVL_expr(tdbb, from->nod_arg[0]);
				if (request->req_flags & req_null)
					break;
				++impure->vlux_count;
				add2(desc, from, reinterpret_cast < vlu * >(impure));
				break;

			case nod_agg_count2:
				++impure->vlux_count;
				desc = EVL_expr(tdbb, from->nod_arg[0]);
				if (request->req_flags & req_null)
					break;

			case nod_agg_count:
				++impure->vlux_count;
				++impure->vlu_misc.vlu_long;
				break;

			case nod_agg_count_distinct:
			case nod_agg_total_distinct:
			case nod_agg_average_distinct:
			case nod_agg_average_distinct2:
			case nod_agg_total_distinct2:
				desc = EVL_expr(tdbb, from->nod_arg[0]);
				if (request->req_flags & req_null)
					break;
				/* "Put" the value to sort. */
				asb = (ASB) from->nod_arg[1];
				asb_impure = (IASB) ((SCHAR *) request + asb->nod_impure);
				if (SORT_put(tdbb->tdbb_status_vector,
							 reinterpret_cast<scb*>(asb_impure->iasb_sort_handle),
							 reinterpret_cast<ULONG**>(&data)))
				{
					  ERR_punt();
				}
				MOVE_CLEAR(data, ROUNDUP_LONG(asb->asb_key_desc->skd_length));
				asb->asb_desc.dsc_address = data;
				MOV_move(desc, &asb->asb_desc);
				break;

			default:
				EXE_assignment(tdbb, *ptr);
			}
		}

		if (!RSE_get_record(tdbb,
		                    reinterpret_cast<struct Rsb*>(rsb),
		                    g_RSE_get_mode))
		{
			  state = 2;
		}
	}

  break_out:

/* Finish up any residual computations and get out */

	if (vtemp.vlu_string)
		delete vtemp.vlu_string;

	for (ptr = map->nod_arg, end = ptr + map->nod_count; ptr < end; ptr++)
	{
		from = (*ptr)->nod_arg[e_asgn_from];
		field = (*ptr)->nod_arg[e_asgn_to];
		id = (USHORT) field->nod_arg[e_fld_id];
		record =
			request->req_rpb[(int) field->nod_arg[e_fld_stream]].rpb_record;
		impure = (VLUX) ((SCHAR *) request + from->nod_impure);
		switch (from->nod_type)
		{
		case nod_agg_min:
		case nod_agg_min_indexed:
		case nod_agg_max:
		case nod_agg_max_indexed:
		case nod_agg_total:
		case nod_agg_total_distinct:
		case nod_agg_total2:
		case nod_agg_total_distinct2:
			if ((from->nod_type == nod_agg_total_distinct) ||
				(from->nod_type == nod_agg_total_distinct2))
			{
				compute_agg_distinct(tdbb, from);
			}
			if (!impure->vlux_count)
			{
				SET_NULL(record, id);
				break;
			}
			/* If vlux_count is non-zero, we need to fall through. */
		case nod_agg_count:
		case nod_agg_count2:
		case nod_agg_count_distinct:
			if (from->nod_type == nod_agg_count_distinct) {
				compute_agg_distinct(tdbb, from);
			}
			if (!impure->vlu_desc.dsc_dtype) {
				SET_NULL(record, id);
			}
			else {
				MOV_move(&impure->vlu_desc, EVL_expr(tdbb, field));
				CLEAR_NULL(record, id);
			}
			break;

		case nod_agg_average_distinct:
			compute_agg_distinct(tdbb, from);
			/* fall through */
		case nod_agg_average:
			if (!impure->vlux_count) {
				SET_NULL(record, id);
				break;
			}
			CLEAR_NULL(record, id);
			temp.dsc_dtype = DEFAULT_DOUBLE;
			temp.dsc_length = sizeof(double);
			temp.dsc_scale = 0;
			temp.dsc_sub_type = 0;
			temp.dsc_address = (UCHAR *) & d;
			d = MOV_get_double(&impure->vlu_desc) / impure->vlux_count;
			MOV_move(&temp, EVL_expr(tdbb, field));
			break;

		case nod_agg_average_distinct2:
			compute_agg_distinct(tdbb, from);
			/* fall through */
		case nod_agg_average2:
			if (!impure->vlux_count) {
				SET_NULL(record, id);
				break;
			}
			CLEAR_NULL(record, id);
			temp.dsc_sub_type = 0;
			if (dtype_int64 == impure->vlu_desc.dsc_dtype) {
				temp.dsc_dtype = dtype_int64;
				temp.dsc_length = sizeof(SINT64);
				temp.dsc_scale = impure->vlu_desc.dsc_scale;
				temp.dsc_address = (UCHAR *) & i;
				i = *((SINT64 *) impure->vlu_desc.dsc_address) /
					impure->vlux_count;
			}
			else {
				temp.dsc_dtype = DEFAULT_DOUBLE;
				temp.dsc_length = sizeof(double);
				temp.dsc_scale = 0;
				temp.dsc_address = (UCHAR *) & d;
				d = MOV_get_double(&impure->vlu_desc) / impure->vlux_count;
			}
			MOV_move(&temp, EVL_expr(tdbb, field));
			break;

                default: /* Shut up some compiler warnings */
                    break;
		}
	}

	}
	catch (const std::exception&) {
		fini_agg_distinct(tdbb, node);
		ERR_punt();
	}
	return state;
}


void DLL_EXPORT EVL_make_value(TDBB tdbb, DSC * desc, VLU value)
{
/**************************************
 *
 *      E V L _ m a k e _ v a l u e
 *
 **************************************
 *
 * Functional description
 *      Make a value block reflect the value of a descriptor. 
 *
 **************************************/
	DSC from;

	SET_TDBB(tdbb);

/* Handle the fixed length data types first.  They're easy. */

	from = *desc;
	value->vlu_desc = *desc;
	value->vlu_desc.dsc_address = (UCHAR *) & value->vlu_misc;

	switch (from.dsc_dtype) {
	case dtype_short:
		value->vlu_misc.vlu_short = *((SSHORT *) from.dsc_address);
		return;

	case dtype_long:
	case dtype_real:
	case dtype_sql_time:
	case dtype_sql_date:
		value->vlu_misc.vlu_long = *((SLONG *) from.dsc_address);
		return;

	case dtype_int64:
		value->vlu_misc.vlu_int64 = *((SINT64 *) from.dsc_address);
		return;

	case dtype_double:
#ifdef VMS
	case dtype_d_float:
#endif
		value->vlu_misc.vlu_double = *((double *) from.dsc_address);
		return;

	case dtype_timestamp:
	case dtype_quad:
		value->vlu_misc.vlu_dbkey[0] = ((SLONG *) from.dsc_address)[0];
		value->vlu_misc.vlu_dbkey[1] = ((SLONG *) from.dsc_address)[1];
		return;

	case dtype_text:
	case dtype_varying:
	case dtype_cstring:
		break;
	default:
		assert(FALSE);
		break;
	}

	{
		STR string;
		UCHAR temp[128], *address, *p;
		USHORT length;
		USHORT ttype;

/* Get string.  If necessary, get_string will convert the string into a
   temporary buffer.  Since this will always be the result of a conversion,
   this isn't a serious problem. */

		length =
			MOV_get_string_ptr(&from, &ttype, &address,
							   reinterpret_cast < vary * >(temp),
							   sizeof(temp));

/* Allocate a string block of sufficient size. */

		if ((string = value->vlu_string) && string->str_length < length) {
			delete string;
			string = NULL;
		}

		if (!string) {
			string = value->vlu_string = FB_NEW_RPT(*tdbb->tdbb_default, length) str();
			string->str_length = length;
		}

		value->vlu_desc.dsc_dtype = dtype_text;
		value->vlu_desc.dsc_length = length;
		value->vlu_desc.dsc_address = p = string->str_data;
		value->vlu_desc.dsc_sub_type = 0;
		value->vlu_desc.dsc_scale = 0;
		INTL_ASSIGN_TTYPE(&value->vlu_desc, ttype);

		if (address && length)
			MOVE_FAST(address, p, length);
	}
}


USHORT DLL_EXPORT EVL_mb_contains(TDBB tdbb,
								  class TextType* obj,
								  UCHAR * p1,
								  USHORT l1, UCHAR * p2, USHORT l2)
{
/**************************************
 *
 *      E V L _ m b _ c o n t a i n s
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	UCS2_CHAR buffer1[100], buffer2[100];	/* arbitrary size for optimization */
	UCS2_CHAR *pp1 = buffer1;
	UCS2_CHAR *pp2 = buffer2;
	USHORT len1, len2;			/* byte counts */
	STR buf1, buf2;
	USHORT ret_val;
	SSHORT err_code;
	USHORT err_pos;

	SET_TDBB(tdbb);

	len1 = obj->to_wc(NULL, 0, p1, l1, &err_code, &err_pos);
	len2 = obj->to_wc(NULL, 0, p2, l2, &err_code, &err_pos);

	if (len1 > sizeof(buffer1)) {
		buf1 = FB_NEW_RPT(*tdbb->tdbb_default, len1) str();
		pp1 = (UCS2_CHAR *) buf1->str_data;
	}
	if (len2 > sizeof(buffer2)) {
		buf2 = FB_NEW_RPT(*tdbb->tdbb_default, len2) str();
		pp2 = (UCS2_CHAR *) buf2->str_data;
	}

	len1 = obj->to_wc(pp1, len1, p1, l1, &err_code, &err_pos);
	len2 = obj->to_wc(pp2, len2, p2, l2, &err_code, &err_pos);

	ret_val = EVL_wc_contains(tdbb, obj, pp1, len1, pp2, len2);

	if (pp1 != buffer1)
		delete buf1;
	if (pp2 != buffer2)
		delete buf2;

	return ret_val;
}


USHORT DLL_EXPORT EVL_mb_like(TDBB tdbb,
							  class TextType* obj,
							  UCHAR * p1,
							  SSHORT l1,
							  UCHAR * p2, SSHORT l2, UCS2_CHAR escape_char)
{
/**************************************
 *
 *      E V L _ m b _ l i k e 
 *
 **************************************
 *
 * Functional description
 *      Multi-Byte version of Like().
 *      Front-end of like() in Japanese version.
 *      
 *      Prepare buffer of short, then "copy" char-based data
 *      into the new ucs2-based buffer. Use the new buffer for
 *      later processing with wc_like().
 *
 **************************************/
	UCS2_CHAR buffer1[100], buffer2[100];	/* arbitrary size for optimization */
	UCS2_CHAR *pp1 = buffer1;
	UCS2_CHAR *pp2 = buffer2;
	USHORT len1, len2;
	STR buf1, buf2;
	USHORT ret_val;
	SSHORT err_code;
	USHORT err_pos;

	SET_TDBB(tdbb);

	len1 = obj->to_wc(NULL, 0, p1, l1, &err_code, &err_pos);
	len2 = obj->to_wc(NULL, 0, p2, l2, &err_code, &err_pos);
	if (len1 > sizeof(buffer1)) {
		buf1 = FB_NEW_RPT(*tdbb->tdbb_default, len1) str();
		pp1 = (UCS2_CHAR *) buf1->str_data;
	}
	if (len2 > sizeof(buffer2)) {
		buf2 = FB_NEW_RPT(*tdbb->tdbb_default, len2) str();
		pp2 = (UCS2_CHAR *) buf2->str_data;
	}

	len1 = obj->to_wc(pp1, len1, p1, l1, &err_code, &err_pos);
	len2 = obj->to_wc(pp2, len2, p2, l2, &err_code, &err_pos);
// CHECK ME: Shouldn't errors to be handled?

	ret_val = EVL_wc_like(tdbb, obj, pp1, len1, pp2, len2, escape_char);

	if (pp1 != buffer1)
		delete buf1;
	if (pp2 != buffer2)
		delete buf2;

	return ret_val;
}


USHORT DLL_EXPORT EVL_mb_matches(TDBB tdbb,
								 class TextType* obj,
								 UCHAR * p1, SSHORT l1, UCHAR * p2, SSHORT l2)
{
/**************************************
 *
 *      E V L _ m b _ m a t c h e s 
 *
 **************************************
 *
 * Functional description
 *      Front-end of matches() in Japanese version.
 *      Prepare buffer of short, then "copy" char-based data
 *      into the new short-based buffer. Use the new buffer for
 *      later processing with EVL_wc_matches().
 *
 **************************************/
	UCS2_CHAR buffer1[100], buffer2[100];	/* arbitrary size for optimization */
	UCS2_CHAR *pp1 = buffer1;
	UCS2_CHAR *pp2 = buffer2;
	USHORT len1, len2;
	STR buf1, buf2;
	USHORT ret_val;
	SSHORT err_code;
	USHORT err_pos;

	SET_TDBB(tdbb);

	len1 = obj->to_wc(NULL, 0, p1, l1, &err_code, &err_pos);
	len2 = obj->to_wc(NULL, 0, p2, l2, &err_code, &err_pos);
	if (len1 > sizeof(buffer1)) {
		buf1 = FB_NEW_RPT(*tdbb->tdbb_default, len1) str();
		pp1 = (UCS2_CHAR *) buf1->str_data;
	}
	if (len2 > sizeof(buffer2)) {
		buf2 = FB_NEW_RPT(*tdbb->tdbb_default, len2) str();
		pp2 = (UCS2_CHAR *) buf2->str_data;
	}

	len1 = obj->to_wc(pp1, len1, p1, l1, &err_code, &err_pos);
	len2 = obj->to_wc(pp2, len2, p2, l2, &err_code, &err_pos);

	ret_val = EVL_wc_matches(tdbb, obj, pp1, len1, pp2, len2);

	if (pp1 != buffer1)
		delete buf1;
	if (pp2 != buffer2)
		delete buf2;

	return ret_val;
}


USHORT DLL_EXPORT EVL_mb_sleuth_check(TDBB tdbb,
									  class TextType* obj,
									  USHORT flags,
									  UCHAR * search,
									  USHORT search_bytes,
									  UCHAR * match, USHORT match_bytes)
{
/**************************************
 *
 *      E V L _ m b _ s l e u t h _ c h e c k 
 *
 **************************************
 *
 * Functional description
 *      Front-end of sleuth_check() in Japanese version.
 *      Prepare buffer of short, then "copy" char-based data
 *      into the new short-based buffer. Use the new buffer for
 *      later processing with sleuth_check().
 *
 **************************************/
	UCS2_CHAR buffer1[100];		/* arbitrary size for optimization */
	UCS2_CHAR *pp1 = buffer1;
	USHORT len1;
	STR buf1;
	USHORT ret_val;
	SSHORT err_code;
	USHORT err_pos;

	assert(search != NULL);
	assert(match != NULL);

/* Note: search_merge has already converted the match
   string to wide character (see note in sleuth()) */

	SET_TDBB(tdbb);

	len1 = obj->to_wc(NULL, 0, search, search_bytes, &err_code, &err_pos);
	if (len1 > sizeof(buffer1)) {
		buf1 = FB_NEW_RPT(*tdbb->tdbb_default, len1) str();
		pp1 = (UCS2_CHAR *) buf1->str_data;
	}

	len1 = obj->to_wc(pp1, len1, search, search_bytes, &err_code, &err_pos);

	ret_val =
		EVL_wc_sleuth_check(tdbb, obj, 0, pp1, len1,
							reinterpret_cast < UCS2_CHAR * >(match),
							match_bytes);

	if (pp1 != buffer1)
		delete buf1;

	return ret_val;
}


USHORT DLL_EXPORT EVL_mb_sleuth_merge(TDBB tdbb,
									  class TextType* obj,
									  UCHAR * match,
									  USHORT match_bytes,
									  UCHAR * control,
									  USHORT control_bytes,
									  UCHAR * combined, USHORT combined_bytes)
{
/**************************************
 *
 *      E V L _ m b _ s l e u t h _ m e r g e 
 *
 **************************************
 *
 * Functional description
 *      Front-end of sleuth_merge() in Japanese version.
 *
 **************************************/
	UCS2_CHAR buffer1[100], buffer2[100];	/* arbitrary size for optimization */
	UCS2_CHAR *pp1 = buffer1;
	UCS2_CHAR *pp2 = buffer2;
	USHORT len1, len2;
	STR buf1, buf2;
	USHORT ret_val;
	SSHORT err_code;
	USHORT err_pos;

	assert(control != NULL);
	assert(match != NULL);
	assert(combined != NULL);

	SET_TDBB(tdbb);

	len1 = obj->to_wc(NULL, 0, match, match_bytes, &err_code, &err_pos);
	len2 = obj->to_wc(NULL, 0, control, control_bytes, &err_code, &err_pos);
	if (len1 > sizeof(buffer1)) {
		buf1 = FB_NEW_RPT(*tdbb->tdbb_default, len1) str();
		pp1 = (UCS2_CHAR *) buf1->str_data;
	}
	if (len2 > sizeof(buffer2)) {
		buf2 = FB_NEW_RPT(*tdbb->tdbb_default, len2) str();
		pp2 = (UCS2_CHAR *) buf2->str_data;
	}

	len1 = obj->to_wc(pp1, len1, match, match_bytes, &err_code, &err_pos);
	len2 = obj->to_wc(pp2, len2, control, control_bytes, &err_code, &err_pos);

	ret_val = EVL_wc_sleuth_merge(tdbb, obj, pp1, len1, pp2, len2,
				      reinterpret_cast < UCS2_CHAR * >(combined),
				      combined_bytes);

	if (pp1 != buffer1)
		delete buf1;
	if (pp2 != buffer2)
		delete buf2;

	return ret_val;
}


USHORT DLL_EXPORT EVL_nc_contains(TDBB tdbb_dummy,
								  class TextType* obj,
								  UCHAR * p1,
								  USHORT l1, UCHAR * p2, USHORT l2)
{
/**************************************
 *
 *      E V L _ n c _ c o n t a i n s
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	UCHAR *q1, *q2, c1, c2;
	SSHORT l;

	while (l1 >= l2)
	{
		--l1;
		q1 = p1++;
		q2 = p2;
		l = l2;
		do {
			if (--l < 0)
				return TRUE;
			c1 = *q1++;
			c2 = *q2++;
		} while (obj->to_upper(c1) == obj->to_upper(c2));
	}

	return FALSE;
}


/**************************************
 *
 *      E V L _ n c _ l i k e
 *
 **************************************
 */
/**************************************
 *
 *      E V L _ n c _ m a t c h e s
 *
 **************************************
 */
/**************************************
 *
 *      E V L _ n c _ s l e u t h _ c h e c k
 *
 **************************************
 */
#define LIKENAME                EVL_nc_like
#define LIKETYPE                UCHAR
#define MATCHESNAME             EVL_nc_matches
#define MATCHESTYPE             UCHAR
#define SLEUTHNAME              EVL_nc_sleuth_check
#define SLEUTH_MERGE_NAME       EVL_nc_sleuth_merge
#define SLEUTH_AUX              nc_sleuth_check
#define SLEUTH_CLASS_NAME       nc_sleuth_class
#define SLEUTHTYPE              UCHAR

#define EVL_LIKE_INCLUDED_BY_EVL_CPP
#include "../jrd/evl_like.cpp"
#undef EVL_LIKE_INCLUDED_BY_EVL_CPP

#undef LIKENAME
#undef LIKETYPE
#undef MATCHESNAME
#undef MATCHESTYPE
#undef SLEUTHNAME
#undef SLEUTH_MERGE_NAME
#undef SLEUTH_AUX
#undef SLEUTH_CLASS_NAME
#undef SLEUTHTYPE


USHORT DLL_EXPORT EVL_wc_contains(TDBB tdbb_dumm,
								  class TextType* obj, UCS2_CHAR * p1, USHORT l1,	/* byte count */
								  UCS2_CHAR * p2, USHORT l2)
{
/**************************************
 *
 *      E V L _ w c _ c o n t a i n s
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	UCS2_CHAR *q1, *q2, c1, c2;
	SSHORT l;

	while (l1 >= l2) {
		l1 -= 2;
		q1 = p1++;
		q2 = p2;
		l = l2;
		do {
			l -= 2;
			if (l < 0)
				return TRUE;
			c1 = *q1++;
			c2 = *q2++;
		} while (obj->to_upper(c1) == obj->to_upper(c2));
	}

	return FALSE;
}

/**************************************
 *
 *      E V L _ w c _ l i k e
 *
 **************************************
 */
/**************************************
 *
 *      E V L _ w c _ m a t c h e s
 *
 **************************************
 */
/**************************************
 *
 *      E V L _ w c _ s l e u t h _ c h e c k
 *
 **************************************
 */
#define LIKENAME                EVL_wc_like
#define LIKETYPE                UCS2_CHAR
#define MATCHESNAME             EVL_wc_matches
#define MATCHESTYPE             UCS2_CHAR
#define SLEUTHNAME              EVL_wc_sleuth_check
#define SLEUTH_MERGE_NAME       EVL_wc_sleuth_merge
#define SLEUTH_AUX              wc_sleuth_check
#define SLEUTH_CLASS_NAME       wc_sleuth_class
#define SLEUTHTYPE              UCS2_CHAR

#define EVL_LIKE_INCLUDED_BY_EVL_CPP
#include "../jrd/evl_like.cpp"
#undef EVL_LIKE_INCLUDED_BY_EVL_CPP

#undef LIKENAME
#undef LIKETYPE
#undef MATCHESNAME
#undef MATCHESTYPE
#undef SLEUTHNAME
#undef SLEUTH_MERGE_NAME
#undef SLEUTH_AUX
#undef SLEUTH_CLASS_NAME
#undef SLEUTHTYPE



static DSC *add(DSC * desc, JRD_NOD node, VLU value)
{
/**************************************
 *
 *      a d d
 *
 **************************************
 *
 * Functional description
 *      Add (or subtract) the contents of a descriptor to value
 *      block, with dialect-1 semantics.
 *      This function can be removed when dialect-3 becomes
 *      the lowest supported dialect.  (Version 7.0?)
 *
 **************************************/
	DSC *result;
	double d1, d2;
	SLONG l1, l2;

	DEV_BLKCHK(node, type_nod);
	assert(node->nod_type == nod_add ||
		   node->nod_type == nod_subtract ||
		   node->nod_type == nod_total ||
		   node->nod_type == nod_average ||
		   node->nod_type == nod_agg_total ||
		   node->nod_type == nod_agg_average ||
		   node->nod_type == nod_agg_total_distinct ||
		   node->nod_type == nod_agg_average_distinct);

	result = &value->vlu_desc;

/* Handle date arithmetic */

	if (node->nod_flags & nod_date) {
		return add_datetime(desc, node, value);
	}

/* Handle floating arithmetic */

	if (node->nod_flags & nod_double) {
		d1 = MOV_get_double(desc);
		d2 = MOV_get_double(&value->vlu_desc);
		value->vlu_misc.vlu_double = (node->nod_type == nod_subtract) ?
			d2 - d1 : d1 + d2;
		result->dsc_dtype = DEFAULT_DOUBLE;
		result->dsc_length = sizeof(double);
		result->dsc_scale = 0;
		result->dsc_sub_type = 0;
		result->dsc_address = (UCHAR *) & value->vlu_misc.vlu_double;
		return result;
	}

/* Handle (oh, ugh) quad arithmetic */

	if (node->nod_flags & nod_quad) {
		SQUAD q1, q2;

		q1 = MOV_get_quad(desc, node->nod_scale);
		q2 = MOV_get_quad(&value->vlu_desc, node->nod_scale);
		result->dsc_dtype = dtype_quad;
		result->dsc_length = sizeof(SQUAD);
		result->dsc_scale = node->nod_scale;
		value->vlu_misc.vlu_quad = (node->nod_type == nod_subtract) ?
			QUAD_SUBTRACT(q2, q1, (FPTR_VOID) ERR_post) :
			QUAD_ADD(q1, q2, (FPTR_VOID) ERR_post);
		result->dsc_address = (UCHAR *) & value->vlu_misc.vlu_quad;

		return result;
	}

/* Everything else defaults to longword */

	l1 = MOV_get_long(desc, node->nod_scale);
	l2 = MOV_get_long(&value->vlu_desc, node->nod_scale);
	result->dsc_dtype = dtype_long;
	result->dsc_length = sizeof(SLONG);
	result->dsc_scale = node->nod_scale;
	value->vlu_misc.vlu_long =
		(node->nod_type == nod_subtract) ? l2 - l1 : l1 + l2;
	result->dsc_address = (UCHAR *) & value->vlu_misc.vlu_long;

	result->dsc_sub_type = 0;
	return result;
}


static DSC *add2(DSC * desc, JRD_NOD node, VLU value)
{
/**************************************
 *
 *      a d d 2
 *
 **************************************
 *
 * Functional description
 *      Add (or subtract) the contents of a descriptor to value
 *      block, with dialect-3 semantics, as in the blr_add,
 *      blr_subtract, and blr_agg_total verbs following a
 *      blr_version5
 *
 **************************************/
	DSC *result;
	double d1, d2;
	SINT64 i1, i2;

	DEV_BLKCHK(node, type_nod);
	assert(node->nod_type == nod_add2 ||
		   node->nod_type == nod_subtract2 ||
		   node->nod_type == nod_average2 ||
		   node->nod_type == nod_agg_total2 ||
		   node->nod_type == nod_agg_average2 ||
		   node->nod_type == nod_agg_total_distinct2 ||
		   node->nod_type == nod_agg_average_distinct2);

	result = &value->vlu_desc;

/* Handle date arithmetic */

	if (node->nod_flags & nod_date) {
		return add_datetime(desc, node, value);
	}

/* Handle floating arithmetic */

	if (node->nod_flags & nod_double) {
		d1 = MOV_get_double(desc);
		d2 = MOV_get_double(&value->vlu_desc);
		value->vlu_misc.vlu_double = (node->nod_type == nod_subtract2) ?
			d2 - d1 : d1 + d2;
		result->dsc_dtype = DEFAULT_DOUBLE;
		result->dsc_length = sizeof(double);
		result->dsc_scale = 0;
		result->dsc_sub_type = 0;
		result->dsc_address = (UCHAR *) & value->vlu_misc.vlu_double;
		return result;
	}

/* Handle (oh, ugh) quad arithmetic */

	if (node->nod_flags & nod_quad) {
		SQUAD q1, q2;

		q1 = MOV_get_quad(desc, node->nod_scale);
		q2 = MOV_get_quad(&value->vlu_desc, node->nod_scale);
		result->dsc_dtype = dtype_quad;
		result->dsc_length = sizeof(SQUAD);
		result->dsc_scale = node->nod_scale;
		value->vlu_misc.vlu_quad = (node->nod_type == nod_subtract2) ?
			QUAD_SUBTRACT(q2, q1, (FPTR_VOID) ERR_post) :
			QUAD_ADD(q1, q2, (FPTR_VOID) ERR_post);
		result->dsc_address = (UCHAR *) & value->vlu_misc.vlu_quad;

		return result;
	}

/* Everything else defaults to int64 */

	i1 = MOV_get_int64(desc, node->nod_scale);
	i2 = MOV_get_int64(&value->vlu_desc, node->nod_scale);
	result->dsc_dtype = dtype_int64;
	result->dsc_length = sizeof(SINT64);
	result->dsc_scale = node->nod_scale;
	value->vlu_misc.vlu_int64 =
		(node->nod_type == nod_subtract2) ? i2 - i1 : i1 + i2;
	result->dsc_address = (UCHAR *) & value->vlu_misc.vlu_int64;

	result->dsc_sub_type = MAX(desc->dsc_sub_type,
							   value->vlu_desc.dsc_sub_type);

/* If the operands of an addition have the same sign, and their sum has
   the opposite sign, then overflow occurred.  If the two addends have
   opposite signs, then the result will lie between the two addends, and
   overflow cannot occur.
   If this is a subtraction, note that we invert the sign bit, rather than
   negating the argument, so that subtraction of MIN_SINT64, which is
   unchanged by negation, will be correctly treated like the addition of
   a positive number for the purposes of this test.

   Test cases for a Gedankenexperiment, considering the sign bits of the
   operands and result after the inversion below:                L  Rt  Sum

        MIN_SINT64 - MIN_SINT64 ==          0, with no overflow  1   0   0
       -MAX_SINT64 - MIN_SINT64 ==          1, with no overflow  1   0   0
        1          - MIN_SINT64 == overflow                      0   0   1
       -1          - MIN_SINT64 == MAX_SINT64, no overflow       1   0   0
*/

	if (node->nod_type == nod_subtract2)
		i1 ^= MIN_SINT64;		/* invert the sign bit */
	if (((i1 ^ i2) >= 0) && ((i1 ^ value->vlu_misc.vlu_int64) < 0))
		ERR_post(isc_exception_integer_overflow, 0);

	return result;
}


static DSC *add_datetime(DSC * desc, JRD_NOD node, VLU value)
{
/**************************************
 *
 *      a d d _ d a t e t i m e
 *
 **************************************
 *
 * Functional description
 *	Vector out to one of the actual datetime addition routines
 *
 **************************************/
	BYTE dtype;					/* Which addition routine to use? */

	assert(node->nod_flags & nod_date);

/* Value is the LHS of the operand.  desc is the RHS */

	if ((node->nod_type == nod_add) || (node->nod_type == nod_add2)) {
		dtype = DSC_add_result[value->vlu_desc.dsc_dtype][desc->dsc_dtype];
	}
	else {
		assert((node->nod_type == nod_subtract)
			   || (node->nod_type == nod_subtract2));
		dtype = DSC_sub_result[value->vlu_desc.dsc_dtype][desc->dsc_dtype];

		/* Is this a <date type> - <date type> construct? 
		   chose the proper routine to do the subtract from the 
		   LHS of expression
		   Thus:   <TIME> - <TIMESTAMP> uses TIME arithmetic
		   <DATE> - <TIMESTAMP> uses DATE arithmetic
		   <TIMESTAMP> - <DATE> uses TIMESTAMP arithmetic */
		if (DTYPE_IS_NUMERIC(dtype))
			dtype = value->vlu_desc.dsc_dtype;

		/* Handle historical <timestamp> = <string> - <value> case */
		if (!DTYPE_IS_DATE(dtype) &&
			(IS_DTYPE_ANY_TEXT(value->vlu_desc.dsc_dtype) ||
			 IS_DTYPE_ANY_TEXT(desc->dsc_dtype))) dtype = dtype_timestamp;
	}

	switch (dtype) {
	case dtype_timestamp:
	default:
		/* This needs to handle a dtype_sql_date + dtype_sql_time */
		/* For historical reasons prior to V6 - handle any types for 
		   timestamp arithmetic */
		return add_timestamp(desc, node, value);

	case dtype_sql_time:
		return add_sql_time(desc, node, value);

	case dtype_sql_date:
		return add_sql_date(desc, node, value);

	case DTYPE_CANNOT:
		ERR_post(gds_expression_eval_err, 0);
		return NULL;
	};
}


static DSC *add_sql_date(DSC * desc, JRD_NOD node, VLU value)
{
/**************************************
 *
 *      a d d _ s q l _ d a t e
 *
 **************************************
 *
 * Functional description
 *	Perform date arithmetic
 *
 *	DATE -   DATE	   Result is SLONG
 *	DATE +/- NUMERIC   Numeric is interpreted as days DECIMAL(*,0).
 *	NUMERIC +/- TIME   Numeric is interpreted as days DECIMAL(*,0).
 *
 **************************************/
	DSC *result;
	SINT64 d1, d2;
	BOOLEAN op1_is_date = FALSE;
	BOOLEAN op2_is_date = FALSE;
	ISC_TIMESTAMP new_date;
	struct tm times;

	DEV_BLKCHK(node, type_nod);
	assert(node->nod_type == nod_add ||
		   node->nod_type == nod_subtract ||
		   node->nod_type == nod_add2 || node->nod_type == nod_subtract2);

	result = &value->vlu_desc;

	assert(value->vlu_desc.dsc_dtype == dtype_sql_date ||
		   desc->dsc_dtype == dtype_sql_date);

/* Coerce operand1 to a count of days */
	if (value->vlu_desc.dsc_dtype == dtype_sql_date) {
		d1 = *((GDS_DATE *) value->vlu_desc.dsc_address);
		op1_is_date++;
	}
	else
		d1 = MOV_get_int64(&value->vlu_desc, 0);

/* Coerce operand2 to a count of days */
	if (desc->dsc_dtype == dtype_sql_date) {
		d2 = *((GDS_DATE *) desc->dsc_address);
		op2_is_date++;
	}
	else
		d2 = MOV_get_int64(desc, 0);

	if (((node->nod_type == nod_subtract)
		 || (node->nod_type == nod_subtract2)) && op1_is_date && op2_is_date) {
		d2 = d1 - d2;
		value->vlu_misc.vlu_int64 = d2;
		result->dsc_dtype = dtype_int64;
		result->dsc_length = sizeof(SINT64);
		result->dsc_scale = 0;
		result->dsc_sub_type = 0;
		result->dsc_address = (UCHAR *) & value->vlu_misc.vlu_int64;
		return result;
	}

	assert(op1_is_date || op2_is_date);
	assert(!(op1_is_date && op2_is_date));

/* Perform the operation */

	if ((node->nod_type == nod_subtract) || (node->nod_type == nod_subtract2)) {
		assert(op1_is_date);
		d2 = d1 - d2;
	}
	else
		d2 = d1 + d2;

/*
   BUG: 68427
   d2 should not be out of range  (0001 - 9999) for the year

   The valid range for dates is 0001-01-01 to 9999-12-31.  If the
   calculation extends this range, return an error
*/

	new_date.timestamp_time = 0;
	new_date.timestamp_date = d2;
	isc_decode_timestamp(&new_date, &times);

	if ((times.tm_year + 1900) < MIN_YEAR
		|| (times.tm_year) + 1900 >
		MAX_YEAR) ERR_post(gds_expression_eval_err, gds_arg_gds,
						   isc_date_range_exceeded, 0);

	value->vlu_misc.vlu_sql_date = d2;

	result->dsc_dtype = dtype_sql_date;
	result->dsc_length = type_lengths[result->dsc_dtype];
	result->dsc_scale = 0;
	result->dsc_sub_type = 0;
	result->dsc_address = (UCHAR *) & value->vlu_misc.vlu_sql_date;
	return result;

}


static DSC *add_sql_time(DSC * desc, JRD_NOD node, VLU value)
{
/**************************************
 *
 *      a d d _ s q l _ t i m e
 *
 **************************************
 *
 * Functional description
 *	Perform time arithmetic
 *
 *	TIME -   TIME	   Result is SLONG, scale -4
 *	TIME +/- NUMERIC   Numeric is interpreted as seconds DECIMAL(*,4).
 *	NUMERIC +/- TIME   Numeric is interpreted as seconds DECIMAL(*,4).
 *
 **************************************/
	DSC *result;
	SINT64 d1, d2;
	BOOLEAN op1_is_time = FALSE;
	BOOLEAN op2_is_time = FALSE;

	DEV_BLKCHK(node, type_nod);
	assert(node->nod_type == nod_add ||
		   node->nod_type == nod_subtract ||
		   node->nod_type == nod_add2 || node->nod_type == nod_subtract2);

	result = &value->vlu_desc;

	assert(value->vlu_desc.dsc_dtype == dtype_sql_time ||
		   desc->dsc_dtype == dtype_sql_time);

/* Coerce operand1 to a count of seconds */
	if (value->vlu_desc.dsc_dtype == dtype_sql_time) {
		d1 = *(GDS_TIME *) value->vlu_desc.dsc_address;
		op1_is_time++;
		assert(d1 >= 0 && d1 < ISC_TICKS_PER_DAY);
	}
	else
		d1 =
			MOV_get_int64(&value->vlu_desc, ISC_TIME_SECONDS_PRECISION_SCALE);

/* Coerce operand2 to a count of seconds */
	if (desc->dsc_dtype == dtype_sql_time) {
		d2 = *(GDS_TIME *) desc->dsc_address;
		op2_is_time++;
		assert(d2 >= 0 && d2 < ISC_TICKS_PER_DAY);
	}
	else
		d2 = MOV_get_int64(desc, ISC_TIME_SECONDS_PRECISION_SCALE);

	if (((node->nod_type == nod_subtract)
		 || (node->nod_type == nod_subtract2)) && op1_is_time && op2_is_time) {
		d2 = d1 - d2;
		/* Overflow cannot occur as the range of supported TIME values
		   is less than the range of INTEGER */
		value->vlu_misc.vlu_long = d2;
		result->dsc_dtype = dtype_long;
		result->dsc_length = sizeof(SLONG);
		result->dsc_scale = ISC_TIME_SECONDS_PRECISION_SCALE;
		result->dsc_address = (UCHAR *) & value->vlu_misc.vlu_long;
		return result;
	}

	assert(op1_is_time || op2_is_time);
	assert(!(op1_is_time && op2_is_time));

/* Perform the operation */

	if ((node->nod_type == nod_subtract) || (node->nod_type == nod_subtract2)) {
		assert(op1_is_time);
		d2 = d1 - d2;
	}
	else
		d2 = d1 + d2;

/* Make sure to use modulo 24 hour arithmetic */

/* Make the result positive */
	while (d2 < 0)
		d2 += (ISC_TICKS_PER_DAY);

/* And make it in the range of values for a day */
	d2 %= (ISC_TICKS_PER_DAY);

	assert(d2 >= 0 && d2 < ISC_TICKS_PER_DAY);

	value->vlu_misc.vlu_sql_time = d2;

	result->dsc_dtype = dtype_sql_time;
	result->dsc_length = type_lengths[result->dsc_dtype];
	result->dsc_scale = 0;
	result->dsc_sub_type = 0;
	result->dsc_address = (UCHAR *) & value->vlu_misc.vlu_sql_time;
	return result;
}


static DSC *add_timestamp(DSC * desc, JRD_NOD node, VLU value)
{
/**************************************
 *
 *      a d d _ t i m e s t a m p
 *
 **************************************
 *
 * Functional description
 *	Perform date&time arithmetic
 *
 *	TIMESTAMP - TIMESTAMP	Result is INT64
 *	TIMESTAMP +/- NUMERIC   Numeric is interpreted as days DECIMAL(*,*).
 *	NUMERIC +/- TIMESTAMP   Numeric is interpreted as days DECIMAL(*,*).
 *	DATE	+   TIME
 *	TIME	+   DATE
 *
 **************************************/
	DSC *result;
	SINT64 d1, d2;
	BOOLEAN op1_is_timestamp = FALSE;
	BOOLEAN op2_is_timestamp = FALSE;
	ISC_TIMESTAMP new_date;
	struct tm times;

	DEV_BLKCHK(node, type_nod);
	assert(node->nod_type == nod_add ||
		   node->nod_type == nod_subtract ||
		   node->nod_type == nod_add2 || node->nod_type == nod_subtract2);

	result = &value->vlu_desc;

/* Operand 1 is Value -- Operand 2 is desc */

	if (value->vlu_desc.dsc_dtype == dtype_sql_date) {
		/* DATE + TIME */
		if ((desc->dsc_dtype == dtype_sql_time) &&
			((node->nod_type == nod_add) || (node->nod_type == nod_add2))) {
			value->vlu_misc.vlu_timestamp.timestamp_date =
				value->vlu_misc.vlu_sql_date;
			value->vlu_misc.vlu_timestamp.timestamp_time =
				*(GDS_TIME *) desc->dsc_address;
			goto return_result;
		}
		ERR_post(gds_expression_eval_err, 0);
	}
	else if (desc->dsc_dtype == dtype_sql_date) {
		/* TIME + DATE */
		if ((value->vlu_desc.dsc_dtype == dtype_sql_time) &&
			((node->nod_type == nod_add) || (node->nod_type == nod_add2))) {
			value->vlu_misc.vlu_timestamp.timestamp_time =
				value->vlu_misc.vlu_sql_time;
			value->vlu_misc.vlu_timestamp.timestamp_date =
				*(GDS_DATE *) desc->dsc_address;
			goto return_result;
		}
		ERR_post(gds_expression_eval_err, 0);
	}

/* For historical reasons (behavior prior to V6), 
   there are times we will do timestamp arithmetic without a 
   timestamp being involved.  
   In such an event we need to convert a text type to a timestamp when
   we don't already have one.
   We assume any text string must represent a timestamp value.  */

/* If we're subtracting, and the 2nd operand is a timestamp, or
   something that looks & smells like it could be a timestamp, then
   we must be doing <timestamp> - <timestamp> subtraction.
   Notes that this COULD be as strange as <string> - <string>, but 
   because nod_date is set in the nod_flags we know we're supposed 
   to use some form of date arithmetic */

	if (((node->nod_type == nod_subtract)
		 || (node->nod_type == nod_subtract2))
		&& ((desc->dsc_dtype == dtype_timestamp)
			|| DTYPE_IS_TEXT(desc->dsc_dtype))) {

		/* Handle cases of
		   <string>    - <string>
		   <string>    - <timestamp>
		   <timestamp> - <string>
		   <timestamp> - <timestamp>
		   in which cases we assume the string represents a timestamp value */

		/* If the first operand couldn't represent a timestamp, bomb out */

		if (!((value->vlu_desc.dsc_dtype == dtype_timestamp) ||
			  DTYPE_IS_TEXT(value->vlu_desc.dsc_dtype)))
				ERR_post(gds_expression_eval_err, 0);

		d1 = get_timestamp_to_isc_ticks(&value->vlu_desc);
		d2 = get_timestamp_to_isc_ticks(desc);

		d2 = d1 - d2;

		if (node->nod_type == nod_subtract2) {

			/* mutlipy by 100,000 so that we can have the result as decimal (18,9) 
			 * We have 10 ^-4; to convert this to 10^-9 we need to multiply by
			 * 100,000. Of course all this is true only because we are dividing
			 * by SECONDS_PER_DAY 
			 * now divide by the number of seconds per day, this will give us the
			 * result as a int64 of type decimal (18, 9) in days instead of
			 * seconds.
			 *
			 * But SECONDS_PER_DAY has 2 trailing zeroes (because it is 24 * 60 *
			 * 60), so instead of calculating (X * 100000) / SECONDS_PER_DAY,
			 * use (X * (100000 / 100)) / (SECONDS_PER_DAY / 100), which can be
			 * simplified to (X * 1000) / (SECONDS_PER_DAY / 100)
			 * Since the largest possible difference in timestamps is about 3E11
			 * seconds or 3E15 isc_ticks, the product won't exceed approximately
			 * 3E18, which fits into an INT64.  
			 */
			d2 = (d2 * 1000) / (SINT64) (SECONDS_PER_DAY / 100);
			value->vlu_misc.vlu_int64 = (SINT64) d2;
			result->dsc_dtype = dtype_int64;
			result->dsc_length = sizeof(SINT64);
			result->dsc_scale = DIALECT_3_TIMESTAMP_SCALE;
			result->dsc_address = (UCHAR *) & value->vlu_misc.vlu_int64;
			return result;
		}
		else {
			/* This is dialect 1 subtraction returning double as before */
			value->vlu_misc.vlu_double =
				(double) d2 / ((double) ISC_TICKS_PER_DAY);
			result->dsc_dtype = dtype_double;
			result->dsc_length = sizeof(double);
			result->dsc_scale = DIALECT_1_TIMESTAMP_SCALE;
			result->dsc_address = (UCHAR *) & value->vlu_misc.vlu_double;
			return result;
		}
	}

/* From here we know our result must be a <timestamp>.  The only
   legal cases are:
	<timestamp> +/-  <numeric>
	<numeric>   +    <timestamp>
   However, the nod_date flag might have been set on any type of
   nod_add / nod_subtract equation -- so we must detect any invalid
   operands.   Any <string> value is assumed to be convertable to
   a timestamp */

/* Coerce operand1 to a count of microseconds */
	if ((value->vlu_desc.dsc_dtype == dtype_timestamp)
		|| (DTYPE_IS_TEXT(value->vlu_desc.dsc_dtype))) {
		op1_is_timestamp = TRUE;
	};

/* Coerce operand2 to a count of microseconds */
	if ((desc->dsc_dtype == dtype_timestamp)
		|| (DTYPE_IS_TEXT(desc->dsc_dtype))) {
		op2_is_timestamp = TRUE;
	};

/* Exactly one of the operands must be a timestamp or
   convertable into a timestamp, otherwise it's one of 
	   <numeric>   +/- <numeric>
	or <timestamp> +/- <timestamp>
	or <string>    +/- <string>
   which are errors */

	if (op1_is_timestamp == op2_is_timestamp)
		ERR_post(gds_expression_eval_err, 0);

	if (op1_is_timestamp) {
		d1 = get_timestamp_to_isc_ticks(&value->vlu_desc);
		d2 = get_day_fraction(desc);
	}
	else {
		assert((node->nod_type == nod_add) || (node->nod_type == nod_add2));
		assert(op2_is_timestamp);
		d1 = get_day_fraction(&value->vlu_desc);
		d2 = get_timestamp_to_isc_ticks(desc);
	};

/* Perform the operation */

	if ((node->nod_type == nod_subtract) || (node->nod_type == nod_subtract2)) {
		assert(op1_is_timestamp);
		d2 = d1 - d2;
	}
	else
		d2 = d1 + d2;

/* Convert the count of microseconds back to a date / time format */

	value->vlu_misc.vlu_timestamp.timestamp_date = d2 / (ISC_TICKS_PER_DAY);
	value->vlu_misc.vlu_timestamp.timestamp_time = (d2 % ISC_TICKS_PER_DAY);

/*
   BUG: 68427
   d2 should not be out of range  (0001 - 9999) for the year

   The valid range for dates is 0001-01-01 to 9999-12-31.  If the
   calculation extends this range, return an error
*/

	new_date.timestamp_time = 0;
	new_date.timestamp_date = value->vlu_misc.vlu_timestamp.timestamp_date;
	isc_decode_timestamp(&new_date, &times);

	if ((times.tm_year + 1900) < MIN_YEAR
		|| (times.tm_year) + 1900 >
		MAX_YEAR) ERR_post(gds_expression_eval_err, gds_arg_gds,
						   isc_date_range_exceeded, 0);


/* Make sure the TIME portion is non-negative */

	if ((SLONG) value->vlu_misc.vlu_timestamp.timestamp_time < 0) {
		value->vlu_misc.vlu_timestamp.timestamp_time =
			((SLONG) value->vlu_misc.vlu_timestamp.timestamp_time) +
			ISC_TICKS_PER_DAY;
		value->vlu_misc.vlu_timestamp.timestamp_date -= 1;
	}

  return_result:
/* Caution: target of GOTO */

	assert(value->vlu_misc.vlu_timestamp.timestamp_time >= 0 &&
		   value->vlu_misc.vlu_timestamp.timestamp_time < ISC_TICKS_PER_DAY);

	result->dsc_dtype = dtype_timestamp;
	result->dsc_length = type_lengths[result->dsc_dtype];
	result->dsc_scale = 0;
	result->dsc_sub_type = 0;
	result->dsc_address = (UCHAR *) & value->vlu_misc.vlu_timestamp;
	return result;
}


static DSC *binary_value(TDBB tdbb, JRD_NOD node, VLU impure)
{
/**************************************
 *
 *      b i n a r y _ v a l u e
 *
 **************************************
 *
 * Functional description
 *      Evaluate a binary value expression.
 *
 **************************************/
	JRD_REQ request;
	DSC *desc1, *desc2;
	double divisor;
	ULONG flags;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	request = tdbb->tdbb_request;
	impure = (VLU) ((SCHAR *) request + node->nod_impure);

/* Evaluate arguments.  If either is null, result is null, but in
   any case, evaluate both, since some expressions may later depend
   on mappings which are developed here */

	desc1 = EVL_expr(tdbb, node->nod_arg[0]);
	flags = request->req_flags;
	request->req_flags &= ~req_null;

	desc2 = EVL_expr(tdbb, node->nod_arg[1]);

/* restore saved NULL state */

	if (flags & req_null)
		request->req_flags |= req_null;

	if (request->req_flags & req_null)
		return NULL;

	EVL_make_value(tdbb, desc1, impure);

	switch (node->nod_type) {
	case nod_add:				/* with dialect-1 semantics */
	case nod_subtract:
		return add(desc2, node, impure);

	case nod_divide:			/* dialect-1 semantics */
		divisor = MOV_get_double(desc2);
		if (divisor == 0)
			ERR_post(gds_arith_except, 0);
		impure->vlu_misc.vlu_double =
			DOUBLE_DIVIDE(MOV_get_double(desc1), divisor);
		impure->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
		impure->vlu_desc.dsc_length = sizeof(double);
		impure->vlu_desc.dsc_address = (UCHAR *) & impure->vlu_misc;
		return &impure->vlu_desc;

	case nod_multiply:			/* dialect-1 semantics */
		return multiply(desc2, impure, node);

	case nod_add2:				/* with dialect-3 semantics */
	case nod_subtract2:
		return add2(desc2, node, impure);

	case nod_multiply2:		/* dialect-3 semantics */
		return multiply2(desc2, impure, node);

	case nod_divide2:			/* dialect-3 semantics */
		return divide2(desc2, impure, node);

	default:
		BUGCHECK(232);			/* msg 232 EVL_expr: invalid operation */
	}
	return NULL;
}


static DSC *cast(TDBB tdbb, DSC * value, JRD_NOD node, VLU impure)
{
/**************************************
 *
 *      c a s t
 *
 **************************************
 *
 * Functional description
 *      Cast from one datatype to another.
 *
 **************************************/
	FMT format;
	STR string;
	USHORT length;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

/* value is present; make the conversion */

	format = (FMT) node->nod_arg[e_cast_fmt];
	impure->vlu_desc = format->fmt_desc[0];
	impure->vlu_desc.dsc_address = (UCHAR *) & impure->vlu_misc;
	if (DTYPE_IS_TEXT(impure->vlu_desc.dsc_dtype)) {
		length = DSC_string_length(&impure->vlu_desc);
		if (length <= 0) {
			/* cast is a subtype cast only */

			length = DSC_string_length(value);
			if (impure->vlu_desc.dsc_dtype == dtype_cstring)
				length++;		/* for NULL byte */
			else if (impure->vlu_desc.dsc_dtype == dtype_varying)
				length += sizeof(USHORT);
			impure->vlu_desc.dsc_length = length;
		}
		length = impure->vlu_desc.dsc_length;

		/* Allocate a string block of sufficient size. */

		if ((string = impure->vlu_string) && string->str_length < length) {
			delete string;
			string = NULL;
		}

		if (!string) {
			string = impure->vlu_string = FB_NEW_RPT(*tdbb->tdbb_default, length) str();
			string->str_length = length;
		}

		impure->vlu_desc.dsc_address = string->str_data;
	}

	MOV_move(value, &impure->vlu_desc);

	return &impure->vlu_desc;
}


static SSHORT compute_agg_distinct(TDBB tdbb, JRD_NOD node)
{
/**************************************
 *
 *      c o m p u t e _ a g g _ d i s t i n c t
 *
 **************************************
 *
 * Functional description
 *      Sort/project the values and compute      
 *      the aggregate.
 *
 **************************************/
	JRD_REQ request;
	DSC *desc;
	VLUX impure;
	ASB asb;
	IASB asb_impure;
	SSHORT ret_val = 0;
	UCHAR *data;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	request = tdbb->tdbb_request;
	asb = (ASB) node->nod_arg[1];
	asb_impure = (IASB) ((SCHAR *) request + asb->nod_impure);
	desc = &asb->asb_desc;
	impure = (VLUX) ((SCHAR *) request + node->nod_impure);

/* Sort the values already "put" to sort */

	if (SORT_sort(	tdbb->tdbb_status_vector,
					reinterpret_cast<SCB> (asb_impure->iasb_sort_handle)))
	{
		ERR_punt();
	}

/* Now get the sorted/projected values and compute the aggregate */

	while (TRUE) {
		SORT_get(tdbb->tdbb_status_vector,
				 reinterpret_cast < SCB > (asb_impure->iasb_sort_handle),
				 reinterpret_cast < ULONG ** >(&data)
#ifdef SCROLLABLE_CURSORS
				 , RSE_get_forward
#endif
			);

		if (data == NULL) {
			/* we are done, close the sort */
			SORT_fini(reinterpret_cast < SCB > (asb_impure->iasb_sort_handle),
					  tdbb->tdbb_attachment);
			asb_impure->iasb_sort_handle = NULL;
			break;
		}

		desc->dsc_address = data;
		switch (node->nod_type) {
		case nod_agg_total_distinct:
		case nod_agg_average_distinct:
			++impure->vlux_count;
			add(desc, node, reinterpret_cast < vlu * >(impure));
			break;

		case nod_agg_total_distinct2:
		case nod_agg_average_distinct2:
			++impure->vlux_count;
			add2(desc, node, reinterpret_cast < vlu * >(impure));
			break;

		case nod_agg_count_distinct:
			++impure->vlux_count;
			++impure->vlu_misc.vlu_long;
			break;

                default:  /* Shut up some warnings */
                    break;
		}
	}

	return ret_val;

}


static DSC *concatenate(TDBB tdbb, JRD_NOD node, VLU impure)
{
/**************************************
 *
 *      c o n c a t e n a t e
 *
 **************************************
 *
 * Functional description
 *      Concatenate two strings.  
 *
 **************************************/
	JRD_REQ request;
	DSC *value1, *value2;
	ULONG flags;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

/* Evaluate sub expressions.  If either is missing, return NULL result. */

	request = tdbb->tdbb_request;

/* Evaluate arguments.  If either is null, result is null, but in
   any case, evaluate both, since some expressions may later depend
   on mappings which are developed here */

	value1 = EVL_expr(tdbb, node->nod_arg[0]);
	flags = request->req_flags;
	request->req_flags &= ~req_null;

	value2 = EVL_expr(tdbb, node->nod_arg[1]);

/* restore saved NULL state */

	if (flags & req_null) {
		request->req_flags |= req_null;
		return value1;
	}

	if (request->req_flags & req_null)
		return value2;

	{
		DSC desc;
		UCHAR *p, temp1[32], temp2[256], *address1, *address2;
		USHORT length1, length2;
		USHORT ttype1;
		STR temp3 = NULL;

/* Both values are present; build the concatenation */

		length1 =
			MOV_get_string_ptr(value1, &ttype1, &address1,
							   reinterpret_cast < vary * >(temp1),
							   sizeof(temp1));

/* value2 will be converted to the same text type as value1 */

		if ((value2->dsc_sub_type != CS_NONE) &&
			((ttype1 == CS_NONE) || (ttype1 == CS_ASCII)))
			ttype1 = value2->dsc_sub_type;

		length2 =
			MOV_make_string2(value2, ttype1, &address2,
							 reinterpret_cast < vary * >(temp2),
							 sizeof(temp2), &temp3);

		desc.dsc_dtype = dtype_text;
		desc.dsc_sub_type = 0;
		desc.dsc_scale = 0;
		desc.dsc_length = length1 + length2;
		desc.dsc_address = NULL;
		INTL_ASSIGN_TTYPE(&desc, ttype1);

		EVL_make_value(tdbb, &desc, impure);
		p = impure->vlu_desc.dsc_address;

		if (length1)
			do
				*p++ = *address1++;
			while (--length1);

		if (length2)
			do
				*p++ = *address2++;
			while (--length2);

		if (temp3)
			delete temp3;
	}
	return &impure->vlu_desc;
}


static DSC *dbkey(TDBB tdbb, JRD_NOD node, VLU impure)
{
/**************************************
 *
 *      d b k e y       ( J R D )
 *
 **************************************
 *
 * Functional description
 *      Make up a dbkey for a record stream.  A dbkey is expressed
 *      as an 8 byte character string.
 *
 **************************************/
	RPB *rpb;
	JRD_REQ request;
	JRD_REL relation;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

/* Get request, record parameter block, and relation for stream */

	request = tdbb->tdbb_request;
	impure = (VLU) ((SCHAR *) request + node->nod_impure);
	rpb = &request->req_rpb[(int) node->nod_arg[0]];
	relation = rpb->rpb_relation;

/* Format dbkey as vector of relation id, record number */

	if (relation->rel_file) {
		impure->vlu_misc.vlu_dbkey[0] = rpb->rpb_b_page;
		impure->vlu_misc.vlu_dbkey[1] = rpb->rpb_b_line;
	}
	else {
		impure->vlu_misc.vlu_dbkey[0] = relation->rel_id;
		impure->vlu_misc.vlu_dbkey[1] = rpb->rpb_number + 1;
	}

/* Initialize descriptor */

	impure->vlu_desc.dsc_address = (UCHAR *) impure->vlu_misc.vlu_dbkey;
	impure->vlu_desc.dsc_dtype = dtype_text;
	impure->vlu_desc.dsc_length = 8;
	impure->vlu_desc.dsc_ttype = ttype_binary;

	return &impure->vlu_desc;
}


static DSC *eval_statistical(TDBB tdbb, JRD_NOD node, VLU impure)
{
/**************************************
 *
 *      e v a l _ s t a t i s t i c a l
 *
 **************************************
 *
 * Functional description
 *      Evaluate a statistical expression.
 *
 **************************************/
	JRD_REQ request;
	DSC *desc, *value;
	ULONG flag;
	USHORT *invariant_flags;
	BLK rsb;
	SSHORT result;
	SLONG count;
	double d;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

/* Get started by opening stream */

	request = tdbb->tdbb_request;
	desc = &impure->vlu_desc;

	if (node->nod_flags & nod_invariant) {
		invariant_flags = & impure->vlu_flags;
		if (*invariant_flags & VLU_computed) {
			/* An invariant node has already been computed. */

			if (*invariant_flags & VLU_null)
				request->req_flags |= req_null;
			else
				request->req_flags &= ~req_null;
			return desc;
		}
	}

	flag = req_null;
	if ((nod_average2 == node->nod_type)) {
		impure->vlu_misc.vlu_int64 = 0;
		impure->vlu_desc.dsc_dtype = dtype_int64;
		impure->vlu_desc.dsc_length = sizeof(SINT64);
		impure->vlu_desc.dsc_address = (UCHAR *) & impure->vlu_misc.vlu_int64;
		impure->vlu_desc.dsc_scale = 0;
	}
	else {
		impure->vlu_misc.vlu_long = 0;
		impure->vlu_desc.dsc_dtype = dtype_long;
		impure->vlu_desc.dsc_length = sizeof(SLONG);
		impure->vlu_desc.dsc_address = (UCHAR *) & impure->vlu_misc.vlu_long;
	}
	count = 0;

	rsb = (BLK) node->nod_arg[e_stat_rsb];
	RSE_open(tdbb, reinterpret_cast < struct Rsb *>(rsb));

/* Handle each variety separately */

	switch (node->nod_type)
	{
	case nod_count:
		flag = 0;
		while (RSE_get_record(tdbb, reinterpret_cast<struct Rsb*>(rsb),
							  g_RSE_get_mode))
		{
			++ impure->vlu_misc.vlu_long;
		}
		break;

	case nod_count2:
		flag = 0;
		while (RSE_get_record(tdbb, reinterpret_cast<struct Rsb*>(rsb),
							  g_RSE_get_mode))
		{
			value = EVL_expr(tdbb, node->nod_arg[e_stat_value]);
			if (!(request->req_flags & req_null)) {
				++impure->vlu_misc.vlu_long;
			}
		}
		break;

	case nod_min:
	case nod_max:
		while (RSE_get_record(tdbb, reinterpret_cast<struct Rsb*>(rsb),
							  g_RSE_get_mode))
		{
			value = EVL_expr(tdbb, node->nod_arg[e_stat_value]);
			if (request->req_flags & req_null) {
				continue;
			}
			if (flag ||
				((result = MOV_compare(value, desc)) < 0 &&
				 node->nod_type == nod_min) ||
				(node->nod_type != nod_min && result > 0))
			{
				flag = 0;
				EVL_make_value(tdbb, value, impure);
			}
		}
		break;

	case nod_from:
		if (RSE_get_record(tdbb, reinterpret_cast<struct Rsb*>(rsb),
						   g_RSE_get_mode))
		{
			desc = EVL_expr(tdbb, node->nod_arg[e_stat_value]);
		}
		else
		{
			if (node->nod_arg[e_stat_default])
				desc = EVL_expr(tdbb, node->nod_arg[e_stat_default]);
			else
				ERR_post(gds_from_no_match, 0);
		}
		flag = request->req_flags;
		break;

	case nod_average:			/* total or average with dialect-1 semantics */
	case nod_total:
		while (RSE_get_record(tdbb, reinterpret_cast<struct Rsb*>(rsb),
								g_RSE_get_mode))
		{
			desc = EVL_expr(tdbb, node->nod_arg[e_stat_value]);
			if (request->req_flags & req_null) {
				continue;
			}
			/* Note: if the field being SUMed or AVERAGEd is short or long,
			   impure will stay long, and the first add() will
			   set the correct scale; if it is approximate numeric,
			   the first add() will convert impure to double. */
			add(desc, node, impure);
			count++;
		}
		desc = &impure->vlu_desc;
		if (node->nod_type == nod_total) {
			flag = 0;
			break;
		}
		if (!count)
			break;
		d = MOV_get_double(&impure->vlu_desc);
		impure->vlu_misc.vlu_double = d / count;
		impure->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
		impure->vlu_desc.dsc_length = sizeof(double);
		impure->vlu_desc.dsc_scale = 0;
		flag = 0;
		break;

	case nod_average2:			/* average with dialect-3 semantics */
		while (RSE_get_record(tdbb, reinterpret_cast<struct Rsb*>(rsb),
							  g_RSE_get_mode))
		{
			desc = EVL_expr(tdbb, node->nod_arg[e_stat_value]);
			if (request->req_flags & req_null)
				continue;
			/* Note: if the field being SUMed or AVERAGEd is exact
			   numeric, impure will stay int64, and the first add() will
			   set the correct scale; if it is approximate numeric,
			   the first add() will convert impure to double. */
			add(desc, node, impure);
			count++;
		}
		desc = &impure->vlu_desc;
		if (!count)
			break;
		/* We know the sum, but we want the average.  To get it, divide
		   the sum by the count.  Since count is exact, dividing an int64
		   sum by count should leave an int64 average, while dividing a
		   double sum by count should leave a double average. */
		if (dtype_int64 == impure->vlu_desc.dsc_dtype)
			impure->vlu_misc.vlu_int64 /= count;
		else
			impure->vlu_misc.vlu_double /= count;
		flag = 0;
		break;

#ifndef AIX_PPC
	default:
		BUGCHECK(233);			/* msg 233 eval_statistical: invalid operation */
#endif
	}

#ifdef AIX_PPC
	if (node->nod_type != nod_count &&
		node->nod_type != nod_count2 &&
		node->nod_type != nod_min &&
		node->nod_type != nod_max &&
		node->nod_type != nod_from &&
		node->nod_type != nod_average &&
		node->nod_type != nod_average2 && node->nod_type != nod_total)
		BUGCHECK(233);			/* msg 233 eval_statistical: invalid operation */
#endif

/* Close stream and return value */

	RSE_close(tdbb, reinterpret_cast < struct Rsb *>(rsb));
	request->req_flags &= ~req_null;
	request->req_flags |= flag;

/* If this is an invariant node, save the return value.  If the
   descriptor does not point to the impure area for this node then
   point this node's descriptor to the correct place; copy the whole
   structure to be absolutely sure */

	if (node->nod_flags & nod_invariant) {
		*invariant_flags |= VLU_computed;
		if (request->req_flags & req_null)
			*invariant_flags |= VLU_null;
		if (desc && (desc != &impure->vlu_desc))
			impure->vlu_desc = *desc;
	}

	return desc;
}


static void fini_agg_distinct(TDBB tdbb, JRD_NOD node)
{
/**************************************
 *
 *      f i n i _ a g g _ d i s t i n c t
 *
 **************************************
 *
 * Functional description
 *      Finalize a sort for distinct aggregate.
 *
 **************************************/
	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	JRD_REQ request = tdbb->tdbb_request;
	JRD_NOD map = node->nod_arg[e_agg_map];

	JRD_NOD *ptr, *end;

	for (ptr = map->nod_arg, end = ptr + map->nod_count; ptr < end; ptr++)
	{
		const jrd_nod* from = (*ptr)->nod_arg[e_asgn_from];
		switch (from->nod_type)
		{
		case nod_agg_count_distinct:
		case nod_agg_total_distinct:
		case nod_agg_average_distinct:
		case nod_agg_average_distinct2:
		case nod_agg_total_distinct2:
			{
			const ASB asb = (ASB) from->nod_arg[1];
			iasb* asb_impure = (iasb*) ((SCHAR *) request + asb->nod_impure);
			SORT_fini(reinterpret_cast<scb*>(asb_impure->iasb_sort_handle),
					  tdbb->tdbb_attachment);
			asb_impure->iasb_sort_handle = NULL;
			}
		}
	}
}

static SINT64 get_day_fraction(DSC * d)
{
/**************************************
 *
 *      g e t _ d a y _ f r a c t i o n
 *
 **************************************
 *
 * Functional description
 *	Take the input number, assume it represents a fractional
 *	count of days.  Convert it to a count of microseconds.
 *
 **************************************/
	DSC result;
	double result_days;

	result.dsc_dtype = dtype_double;
	result.dsc_scale = 0;
	result.dsc_flags = 0;
	result.dsc_sub_type = 0;
	result.dsc_length = sizeof(double);
	result.dsc_address = reinterpret_cast < UCHAR * >(&result_days);

/* Convert the input number to a double */
	CVT_move(d, &result, reinterpret_cast < void (*)() > (ERR_post));

/* There's likely some loss of precision here due to rounding of number */

	return (SINT64) (result_days * ISC_TICKS_PER_DAY);
}



static DSC *get_mask(TDBB tdbb, JRD_NOD node, VLU impure)
{
/**************************************
 *
 *      g e t _ m a s k
 *
 **************************************
 *
 * Functional description
 *      Compute protection mask.
 *
 **************************************/
	TEXT *p1, *p2, relation_name[32], field_name[32];
	DSC *value;
	JRD_REQ request;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	request = tdbb->tdbb_request;
	p1 = p2 = NULL;
	value = EVL_expr(tdbb, node->nod_arg[0]);

	if (!(request->req_flags & req_null)) {
		p1 = relation_name;
		MOV_get_name(value, p1);
		value = EVL_expr(tdbb, node->nod_arg[1]);
		if (!(request->req_flags & req_null)) {
			p2 = field_name;
			MOV_get_name(value, p2);
		}
	}

	request->req_flags &= ~req_null;

	impure->vlu_misc.vlu_long = SCL_get_mask(p1, p2);
	impure->vlu_desc.dsc_dtype = dtype_long;
	impure->vlu_desc.dsc_length = sizeof(SLONG);
	impure->vlu_desc.dsc_scale = 0;
	impure->vlu_desc.dsc_address = (UCHAR *) & impure->vlu_misc.vlu_long;

	return &impure->vlu_desc;
}


static SINT64 get_timestamp_to_isc_ticks(DSC * d)
{
/**************************************
 *
 *	g e t _ t i m e s t a m p _ t o _ i s c _ t i c k s
 *
 **************************************
 *
 * Functional description
 *	Take the input value, which is either a timestamp
 *	or a string representing a timestamp.  Convert it to
 *	a timestamp, and then return that timestamp as a
 *	count of isc_ticks since the base date and time
 *	in MJD time arithmetic.
 *	ISC_TICKS or isc_ticks are actually deci - milli seconds or
 *	tenthousandth of seconds per day. This is derived from the 
 *	ISC_TIME_SECONDS_PRECISION.
 *
 **************************************/
	DSC result;
	GDS_TIMESTAMP result_timestamp;

	result.dsc_dtype = dtype_timestamp;
	result.dsc_scale = 0;
	result.dsc_flags = 0;
	result.dsc_sub_type = 0;
	result.dsc_length = sizeof(GDS_TIMESTAMP);
	result.dsc_address = reinterpret_cast < UCHAR * >(&result_timestamp);

	CVT_move(d, &result, reinterpret_cast < void (*)() > (ERR_post));

	return ((SINT64) result_timestamp.timestamp_date) * ISC_TICKS_PER_DAY
		+ (SINT64) result_timestamp.timestamp_time;
}


static SSHORT init_agg_distinct(TDBB tdbb, JRD_NOD node)
{
/**************************************
 *
 *      i n i t _ a g g _ d i s t i n c t
 *
 **************************************
 *
 * Functional description
 *      Initialize a sort for distinct aggregate.
 *
 **************************************/
	JRD_REQ request;
	ASB asb;
	SCB handle;
	SKD *sort_key;
	IASB asb_impure;
	SSHORT ret_val = 0;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	request = tdbb->tdbb_request;

	asb = (ASB) node->nod_arg[1];
	asb_impure = (IASB) ((SCHAR *) request + asb->nod_impure);
	sort_key = asb->asb_key_desc;

	handle =
		SORT_init(tdbb->tdbb_status_vector,
				  ROUNDUP_LONG(sort_key->skd_length), 1, sort_key,
				  reinterpret_cast < BOOLEAN(*)() > (reject_duplicate), 0,
				  tdbb->tdbb_attachment, 0);

	if (!(asb_impure->iasb_sort_handle = (SLONG *) handle))
		ERR_punt();

	return ret_val;
}


#ifdef PC_ENGINE
static DSC *lock_record(TDBB tdbb, JRD_NOD node, VLU impure)
{
/**************************************
 *
 *      l o c k _ r e c o r d
 *
 **************************************
 *
 * Functional description
 *      Lock a record and return a descriptor
 *      pointing to the lock handle.
 *
 **************************************/
	JRD_REQ request;
	DSC *desc;
	USHORT lock_level;
	RSB rsb;
	RPB *rpb;
	LCK lock = NULL;

	SET_TDBB(tdbb);

	request = tdbb->tdbb_request;

	DEV_BLKCHK(node, type_nod);

/* Initialize descriptor */

	impure->vlu_desc.dsc_address = (UCHAR *) & impure->vlu_misc.vlu_long;
	impure->vlu_desc.dsc_dtype = dtype_long;
	impure->vlu_desc.dsc_length = sizeof(ULONG);
	impure->vlu_desc.dsc_scale = 0;

/* get the locking level */

	desc = EVL_expr(tdbb, node->nod_arg[e_lockrec_level]);
	lock_level = (USHORT) MOV_get_long(desc, 0);
	if (lock_level > LCK_EX)
		ERR_post(gds_bad_lock_level, gds_arg_number, (SLONG) lock_level, 0);

/* perform the actual lock (or unlock) */

	rsb = *(RSB *) node->nod_arg[e_lockrec_rsb];
	rpb = request->req_rpb + rsb->rsb_stream;
	if (!lock_level)
		RLCK_unlock_record(0, rpb);
	else if (!(lock = RLCK_lock_record(rpb, lock_level, 0, 0)))
		ERR_warning(gds_record_lock, 0);

/* return the lock handle (actually the pointer to the lock block) */

#if SIZEOF_VOID_P != 8
	impure->vlu_misc.vlu_long = (ULONG) lock;
#else
	{
		ATT att;
		ULONG slot;

		/* The lock pointer can't be stored in a ULONG.  Therefore we must
		   generate a ULONG value that can be used to retrieve the pointer.
		   Basically we will keep a vector of user locks and give the user
		   an index into this vector.  When the user releases a lock, its
		   slot in the vector is zeroed and it becomes available for reuse. */

		att = tdbb->tdbb_attachment;
		slot = ALL_get_free_object(tdbb->tdbb_database->dbb_permanent,
								   &att->att_lck_quick_ref, 50);
		att->att_lck_quick_ref->vec_object[slot] = lock;
		impure->vlu_misc.vlu_long = slot;
	}
#endif

	return &impure->vlu_desc;
}
#endif


#ifdef PC_ENGINE
static DSC *lock_relation(TDBB tdbb, JRD_NOD node, VLU impure)
{
/**************************************
 *
 *      l o c k _ r e l a t i o n 
 *
 **************************************
 *
 * Functional description
 *      Lock a relation and return a descriptor
 *      pointing to the lock handle.
 *
 **************************************/
	DSC *desc;
	USHORT lock_level;
	JRD_NOD relation_node;
	JRD_REL relation;
	LCK lock = NULL;

	SET_TDBB(tdbb);
	DEV_BLKCHK(node, type_nod);

/* Initialize descriptor */

	impure->vlu_desc.dsc_address = (UCHAR *) & impure->vlu_misc.vlu_long;
	impure->vlu_desc.dsc_dtype = dtype_long;
	impure->vlu_desc.dsc_length = sizeof(ULONG);
	impure->vlu_desc.dsc_scale = 0;

/* get the locking level */

	desc = EVL_expr(tdbb, node->nod_arg[e_lockrel_level]);
	lock_level = (USHORT) MOV_get_long(desc, 0);
	if (lock_level > LCK_EX)
		ERR_post(gds_bad_lock_level, gds_arg_number, (SLONG) lock_level, 0);

/* perform the actual lock (or unlock) */

	relation_node = node->nod_arg[e_lockrel_relation];
	relation = (JRD_REL) relation_node->nod_arg[e_rel_relation];
	if (!lock_level)
		RLCK_unlock_relation(0, relation);
	else
		lock = RLCK_lock_relation(relation, lock_level, 0, relation);

/* return the lock handle (actually the pointer to the lock block) */

#if SIZEOF_VOID_P != 8
	impure->vlu_misc.vlu_long = (ULONG) lock;
#else
	{
		ATT att;
		ULONG slot;

		SET_TDBB(tdbb);

		/* The lock pointer can't be stored in a ULONG.  Therefore we must
		   generate a ULONG value that can be used to retrieve the pointer.
		   Basically we will keep a vector of user locks and give the user
		   an index into this vector.  When the user releases a lock, its
		   slot in the vector is zeroed and it becomes available for reuse. */

		att = tdbb->tdbb_attachment;
		slot = ALL_get_free_object(tdbb->tdbb_database->dbb_permanent,
								   &att->att_lck_quick_ref, 50);
		att->att_lck_quick_ref->vec_object[slot] = lock;
		impure->vlu_misc.vlu_long = slot;
	}
#endif

	return &impure->vlu_desc;
}
#endif


static DSC *lock_state(TDBB tdbb, JRD_NOD node, VLU impure)
{
/**************************************
 *
 *      l o c k _ s t a t e
 *
 **************************************
 *
 * Functional description
 *      Compute state of an attachment id.  The values
 *      returned are:
 *
 *          0 - value is null
 *          1 - attachment is gone
 *          2 - we are that attachment
 *          3 - attachment is active
 *
 **************************************/
	DBB dbb;
	JRD_REQ request;
	DSC *desc;
	SLONG id;

	SET_TDBB(tdbb);

	dbb = tdbb->tdbb_database;

	DEV_BLKCHK(node, type_nod);

/* Initialize descriptor */

	impure->vlu_desc.dsc_address = (UCHAR *) & impure->vlu_misc.vlu_long;
	impure->vlu_desc.dsc_dtype = dtype_long;
	impure->vlu_desc.dsc_length = sizeof(SLONG);
	impure->vlu_desc.dsc_scale = 0;

/* Evaluate attachment id */

	request = tdbb->tdbb_request;
	desc = EVL_expr(tdbb, node->nod_arg[0]);

	if (request->req_flags & req_null)
		impure->vlu_misc.vlu_long = 0;
	else {
		id = MOV_get_long(desc, 0);
		if (id == PAG_attachment_id())
			impure->vlu_misc.vlu_long = 2;
		else {
			struct lck temp_lock;
			/* fill out a lock block, zeroing it out first */

			temp_lock.lck_parent = dbb->dbb_lock;
			temp_lock.lck_type = LCK_attachment;
			temp_lock.lck_owner_handle =
				LCK_get_owner_handle(tdbb, temp_lock.lck_type);
			temp_lock.lck_length = sizeof(SLONG);
			temp_lock.lck_key.lck_long = id;

			if (LCK_lock(tdbb, &temp_lock, LCK_write, FALSE)) {
				impure->vlu_misc.vlu_long = 1;
				LCK_release(tdbb, &temp_lock);
			}
			else
				impure->vlu_misc.vlu_long = 3;
		}
	}

	return &impure->vlu_desc;
}


static DSC *multiply(DSC * desc, VLU value, JRD_NOD node)
{
/**************************************
 *
 *      m u l t i p l y
 *
 **************************************
 *
 * Functional description
 *      Multiply two numbers, with SQL dialect-1 semantics
 *      This function can be removed when dialect-3 becomes
 *      the lowest supported dialect.  (Version 7.0?)
 *
 **************************************/
	double d1, d2;
//	SLONG l1, l2;
	SSHORT scale;

	DEV_BLKCHK(node, type_nod);

/* Handle floating arithmetic */

	if (node->nod_flags & nod_double) {
		d1 = MOV_get_double(desc);
		d2 = MOV_get_double(&value->vlu_desc);
		value->vlu_misc.vlu_double = DOUBLE_MULTIPLY(d1, d2);
		value->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
		value->vlu_desc.dsc_length = sizeof(double);
		value->vlu_desc.dsc_scale = 0;
		value->vlu_desc.dsc_address = (UCHAR *) & value->vlu_misc.vlu_double;
		return &value->vlu_desc;
	}

/* Handle (oh, ugh) quad arithmetic */

	if (node->nod_flags & nod_quad) {
		SQUAD q1, q2;

		scale = NUMERIC_SCALE(value->vlu_desc);
		q1 = MOV_get_quad(desc, node->nod_scale - scale);
		q2 = MOV_get_quad(&value->vlu_desc, scale);
		value->vlu_desc.dsc_dtype = dtype_quad;
		value->vlu_desc.dsc_length = sizeof(SQUAD);
		value->vlu_desc.dsc_scale = node->nod_scale;
		value->vlu_misc.vlu_quad =
			QUAD_MULTIPLY(q1, q2, (FPTR_VOID) ERR_post);
		value->vlu_desc.dsc_address = (UCHAR *) & value->vlu_misc.vlu_quad;

		return &value->vlu_desc;
	}

/* Everything else defaults to longword */

	/* CVC: With so many problems cropping with dialect 1 and multiplication,
			I decided to close this Pandora box by incurring in INT64 performance
			overhead (if noticeable) and try to get the best result. When I read it,
			this function didn't bother even to check for overflow! */

#define FIREBIRD_AVOID_DIALECT1_OVERFLOW

	{
	SINT64 i1, i2, rc;
	scale = NUMERIC_SCALE(value->vlu_desc);
	i1 = MOV_get_long(desc, node->nod_scale - scale);
	i2 = MOV_get_long(&value->vlu_desc, scale);
	value->vlu_desc.dsc_dtype = dtype_long;
	value->vlu_desc.dsc_length = sizeof(SLONG);
	value->vlu_desc.dsc_scale = node->nod_scale;
	rc = i1 * i2;
	if (rc < MIN_SLONG || rc > MAX_SLONG)
	{
#ifdef FIREBIRD_AVOID_DIALECT1_OVERFLOW
		value->vlu_misc.vlu_int64 = rc;
		value->vlu_desc.dsc_address = (UCHAR*) &value->vlu_misc.vlu_int64;
		value->vlu_desc.dsc_dtype = dtype_int64;
		value->vlu_desc.dsc_length = sizeof(SINT64);
		value->vlu_misc.vlu_double = MOV_get_double(&value->vlu_desc);
		/* This is the Borland solution instead of the five lines above.
		d1 = MOV_get_double (desc); 
        d2 = MOV_get_double (&value->vlu_desc); 
        value->vlu_misc.vlu_double = DOUBLE_MULTIPLY (d1, d2); */
		value->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
		value->vlu_desc.dsc_length = sizeof(double);
		value->vlu_desc.dsc_scale = 0;
		value->vlu_desc.dsc_address = (UCHAR*) &value->vlu_misc.vlu_double;
#else
		ERR_post(isc_exception_integer_overflow, 0);
#endif
	}
	else
	{
		value->vlu_misc.vlu_long = (SLONG) rc; /* l1 * l2;*/
		value->vlu_desc.dsc_address = (UCHAR*) &value->vlu_misc.vlu_long;
	}
	}

	return &value->vlu_desc;
}


static DSC *multiply2(DSC * desc, VLU value, JRD_NOD node)
{
/**************************************
 *
 *      m u l t i p l y 2
 *
 **************************************
 *
 * Functional description
 *      Multiply two numbers, with dialect-3 semantics,
 *      implementing blr_version5 ... blr_multiply.
 *
 **************************************/

	SSHORT scale;

	DEV_BLKCHK(node, type_nod);

/* Handle floating arithmetic */

	if (node->nod_flags & nod_double)
	{
		double d1 = MOV_get_double(desc);
		double d2 = MOV_get_double(&value->vlu_desc);
		value->vlu_misc.vlu_double = DOUBLE_MULTIPLY(d1, d2);
		value->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
		value->vlu_desc.dsc_length = sizeof(double);
		value->vlu_desc.dsc_scale = 0;
		value->vlu_desc.dsc_address = (UCHAR *) & value->vlu_misc.vlu_double;
		return &value->vlu_desc;
	}

/* Handle (oh, ugh) quad arithmetic */

	if (node->nod_flags & nod_quad)
	{
		scale = NUMERIC_SCALE(value->vlu_desc);
		SQUAD q1 = MOV_get_quad(desc, node->nod_scale - scale);
		SQUAD q2 = MOV_get_quad(&value->vlu_desc, scale);
		value->vlu_desc.dsc_dtype = dtype_quad;
		value->vlu_desc.dsc_length = sizeof(SQUAD);
		value->vlu_desc.dsc_scale = node->nod_scale;
		value->vlu_misc.vlu_quad =
			QUAD_MULTIPLY(q1, q2, (FPTR_VOID) ERR_post);
		value->vlu_desc.dsc_address = (UCHAR *) & value->vlu_misc.vlu_quad;

		return &value->vlu_desc;
	}

/* Everything else defaults to int64 */

	scale = NUMERIC_SCALE(value->vlu_desc);
	const SINT64 i1 = MOV_get_int64(desc, node->nod_scale - scale);
	const SINT64 i2 = MOV_get_int64(&value->vlu_desc, scale);

/*
   We need to report an overflow if
       (i1 * i2 < MIN_SINT64) || (i1 * i2 > MAX_SINT64)
   which is equivalent to
       (i1 < MIN_SINT64 / i2) || (i1 > MAX_SINT64 / i2)
   
   Unfortunately, a trial division to see whether the multiplication will
   overflow is expensive: fortunately, we only need perform one division and
   test for one of the two cases, depending on whether the factors have the
   same or opposite signs.

   Unfortunately, in C it is unspecified which way division rounds
   when one or both arguments are negative.  (ldiv() is guaranteed to
   round towards 0, but the standard does not yet require an lldiv()
   or whatever for 64-bit operands.  This makes the problem messy.
   We use UINT64s for the checking, thus ensuring that our division rounds
   down.  This means that we have to check the sign of the product first
   in order to know whether the maximum abs(i1*i2) is MAX_SINT64 or
   (MAX_SINT64+1).

   Of course, if a factor is 0, the product will also be 0, and we don't
   need a trial-division to be sure the multiply won't overflow.
 */

	const UINT64 u1 = (i1 >= 0) ? i1 : -i1;	/* abs(i1) */
	const UINT64 u2 = (i2 >= 0) ? i2 : -i2;	/* abs(i2) */
	const UINT64 u_limit = ((i1 ^ i2) >= 0) ? MAX_SINT64 : MAX_SINT64 + 1;	/* largest product */

	if ((u1 != 0) && ((u_limit / u1) < u2)) {
		ERR_post(isc_exception_integer_overflow, 0);
	}

	value->vlu_desc.dsc_dtype   = dtype_int64;
	value->vlu_desc.dsc_length  = sizeof(SINT64);
	value->vlu_desc.dsc_scale   = node->nod_scale;
	value->vlu_misc.vlu_int64   = i1 * i2;
	value->vlu_desc.dsc_address = (UCHAR *) & value->vlu_misc.vlu_int64;

	return &value->vlu_desc;
}


static DSC *divide2(DSC * desc, VLU value, JRD_NOD node)
{
/**************************************
 *
 *      d i v i d e 2
 *
 **************************************
 *
 * Functional description
 *      Divide two numbers, with SQL dialect-3 semantics,
 *      as in the blr_version5 ... blr_divide or
 *                blr_version5 ... blr_average ....
 *
 **************************************/
	double d1, d2;
	SINT64 i1, i2;
	SSHORT addl_scale;

	DEV_BLKCHK(node, type_nod);

/* Handle floating arithmetic */

	if (node->nod_flags & nod_double) {
		d2 = MOV_get_double(desc);
		if (d2 == 0.0)
			ERR_post(gds_arith_except, 0);
		d1 = MOV_get_double(&value->vlu_desc);
		value->vlu_misc.vlu_double = DOUBLE_DIVIDE(d1, d2);
		value->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
		value->vlu_desc.dsc_length = sizeof(double);
		value->vlu_desc.dsc_scale = 0;
		value->vlu_desc.dsc_address = (UCHAR *) & value->vlu_misc.vlu_double;
		return &value->vlu_desc;
	}

/* Everything else defaults to int64 */

/*
 * In the SQL standard, the precision and scale of the quotient of exact
 * numeric dividend and divisor are implementation-defined: we have defined
 * the precision as 18 (in other words, an SINT64), and the scale as the
 * sum of the scales of the two operands.  To make this work, we have to
 * multiply by pow(10, -2* (scale of divisor)).
 *
 * To see this, consider the operation n1 / n2, and represent the numbers
 * by ordered pairs (v1, s1) and (v2, s2), representing respectively the
 * integer value and the scale of each operation, so that
 *     n1 = v1 * pow(10, s1), and
 *     n2 = v2 * pow(10, s2)
 * Then the quotient is ...
 * 
 *     v1 * pow(10,s1)
 *     ----------------- = (v1/v2) * pow(10, s1-s2)
 *     v2 * pow(10,s2)
 * 
 * But we want the scale of the result to be (s1+s2), not (s1-s2)
 * so we need to multiply by 1 in the form
 *         pow(10, -2 * s2) * pow(20, 2 * s2)
 * which, after regrouping, gives us ...
 *   =  ((v1 * pow(10, -2*s2))/v2) * pow(10, 2*s2) * pow(10, s1-s2)
 *   =  ((v1 * pow(10, -2*s2))/v2) * pow(10, 2*s2 + s1 - s2)
 *   =  ((v1 * pow(10, -2*s2))/v2) * pow(10, s1 + s2)
 * or in our ordered-pair notation, 
 *      ( v1 * pow(10, -2*s2) / v2, s1 + s2 )
 *
 * To maximize the amount of information in the result, we scale up the
 * the dividend as far as we can without causing overflow, then we perform
 * the division, then do any additional required scaling.
 *
 * Who'da thunk that 9th-grade algebra would prove so useful.
 *                                      -- Chris Jewell, December 1998
 */
	i2 = MOV_get_int64(desc, desc->dsc_scale);
	if (i2 == 0)
		ERR_post(gds_arith_except, 0);

	i1 = MOV_get_int64(&value->vlu_desc, value->vlu_desc.dsc_scale);

/* MIN_SINT64 / -1 = (MAX_SINT64 + 1), which overflows in SINT64. */
	if ((i1 == MIN_SINT64) && (i2 == -1))
		ERR_post(isc_exception_integer_overflow, 0);

/* Scale the dividend by as many of the needed powers of 10 as possible
   without causing an overflow. */
	addl_scale = 2 * desc->dsc_scale;
	if (i1 >= 0)
	{
		while ((addl_scale < 0) && (i1 <= MAX_INT64_LIMIT))
		{
			i1 *= 10;
			++addl_scale;
		}
	}
	else
	{
		while ((addl_scale < 0) && (i1 >= MIN_INT64_LIMIT))
		{
			i1 *= 10;
			++addl_scale;
		}
	}

/* If we couldn't use up all the additional scaling by multiplying the
   dividend by 10, but there are trailing zeroes in the divisor, we can
   get the same effect by dividing the divisor by 10 instead. */
	while ((addl_scale < 0) && (0 == (i2 % 10))) {
		i2 /= 10;
		++addl_scale;
	}

	value->vlu_desc.dsc_dtype = dtype_int64;
	value->vlu_desc.dsc_length = sizeof(SINT64);
	value->vlu_desc.dsc_scale = node->nod_scale;
	value->vlu_misc.vlu_int64 = i1 / i2;
	value->vlu_desc.dsc_address = (UCHAR *) & value->vlu_misc.vlu_int64;

/* If we couldn't do all the required scaling beforehand without causing
   an overflow, do the rest of it now.  If we get an overflow now, then
   the result is really too big to store in a properly-scaled SINT64,
   so report the error. For example, MAX_SINT64 / 1.00 overflows. */
	if (value->vlu_misc.vlu_int64 >= 0)
	{
		while ((addl_scale < 0) && (value->vlu_misc.vlu_int64 <= MAX_INT64_LIMIT))
		{
			value->vlu_misc.vlu_int64 *= 10;
			addl_scale++;
		}
	}
	else
	{
		while ((addl_scale < 0) && (value->vlu_misc.vlu_int64 >= MIN_INT64_LIMIT))
		{
			value->vlu_misc.vlu_int64 *= 10;
			addl_scale++;
		}
	}
	if (addl_scale < 0)
		ERR_post(gds_arith_except, 0);

	return &value->vlu_desc;
}


static DSC *negate_dsc(TDBB tdbb, DSC * desc, VLU value)
{
/**************************************
 *
 *      n e g a t e
 *
 **************************************
 *
 * Functional description
 *      Negate a single value
 *
 **************************************/

	SET_TDBB(tdbb);
	EVL_make_value(tdbb, desc, value);

	switch (value->vlu_desc.dsc_dtype) {
	case dtype_short:
		if (value->vlu_misc.vlu_short == MIN_SSHORT)
			ERR_post(isc_exception_integer_overflow, 0);
		value->vlu_misc.vlu_short = -value->vlu_misc.vlu_short;
		break;

	case dtype_long:
		if (value->vlu_misc.vlu_long == MIN_SLONG)
			ERR_post(isc_exception_integer_overflow, 0);
		value->vlu_misc.vlu_long = -value->vlu_misc.vlu_long;
		break;

	case dtype_real:
		value->vlu_misc.vlu_float = -value->vlu_misc.vlu_float;
		break;

	case DEFAULT_DOUBLE:
		value->vlu_misc.vlu_double = -value->vlu_misc.vlu_double;
		break;

#ifdef VMS
	case SPECIAL_DOUBLE:
		{
			double d1 = -CNVT_TO_DFLT(&value->vlu_misc.vlu_double);
			value->vlu_misc.vlu_double = CNVT_FROM_DFLT(&d1);
		}
		break;
#endif

	case dtype_quad:
		value->vlu_misc.vlu_quad =
			QUAD_NEGATE(value->vlu_misc.vlu_quad, (FPTR_VOID) ERR_post);
		break;

	case dtype_int64:
		if (value->vlu_misc.vlu_int64 == MIN_SINT64)
			ERR_post(isc_exception_integer_overflow, 0);
		value->vlu_misc.vlu_int64 = -value->vlu_misc.vlu_int64;
		break;

	default:
		value->vlu_misc.vlu_double = -MOV_get_double(&value->vlu_desc);
		value->vlu_desc.dsc_dtype = DEFAULT_DOUBLE;
		value->vlu_desc.dsc_length = sizeof(double);
		value->vlu_desc.dsc_scale = 0;
		value->vlu_desc.dsc_address = (UCHAR *) & value->vlu_misc.vlu_double;
	}

	return &value->vlu_desc;
}


static DSC* record_version(TDBB tdbb, JRD_NOD node, VLU impure)
{
/**************************************
 *
 *      r e c o r d _ v e r s i o n
 *
 **************************************
 *
 * Functional description
 *      Make up a record version for a record stream.
 *      The tid of the record will be used.  This will be returned 
 *      as a 4 byte character string.
 *
 **************************************/
	RPB *rpb;
	JRD_REQ request;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

/* Get request, record parameter block for stream */

	request = tdbb->tdbb_request;
	impure = (VLU) ((SCHAR *) request + node->nod_impure);
	rpb = &request->req_rpb[(int) node->nod_arg[0]];

/* If the current transaction has updated the record, the record version
 * coming in from DSQL will have the original transaction # (or current
 * transaction if the current transaction updated the record in a different
 * request.).  In these cases, mark the request so that the boolean
 * to check equality of record version will be forced to evaluate to true.
 */

	if (tdbb->tdbb_request->req_transaction->tra_number ==
		rpb->rpb_transaction)
	{
		request->req_flags |= req_same_tx_upd;
	}
	else
	{
		/* If the transaction is a commit retain, check if the record was
		 * last updated in one of its own prior transactions
		 */

		if (request->req_transaction->tra_commit_sub_trans)
		{
			if (SBM_test(request->req_transaction->tra_commit_sub_trans,
			             rpb->rpb_transaction))
			{
				 request->req_flags |= req_same_tx_upd;
			}
		}
	}

/* Initialize descriptor */

	impure->vlu_misc.vlu_long    = rpb->rpb_transaction;
	impure->vlu_desc.dsc_address = (UCHAR *) & impure->vlu_misc.vlu_long;
	impure->vlu_desc.dsc_dtype   = dtype_text;
	impure->vlu_desc.dsc_length  = 4;
	impure->vlu_desc.dsc_ttype   = ttype_binary;

	return &impure->vlu_desc;
}


static BOOLEAN reject_duplicate(UCHAR * data1, UCHAR * data2, int user_arg)
{
/**************************************
 *
 *      r e j e c t _ d u p l i c a t e
 *
 **************************************
 *
 * Functional description
 *      Callback routine used by sort/project to reject duplicate values.
 *      Particularly dumb routine -- always returns TRUE;
 *
 **************************************/

	return TRUE;
}


static DSC* scalar(TDBB tdbb, JRD_NOD node, VLU impure)
{
/**************************************
 *
 *      s c a l a r
 *
 **************************************
 *
 * Functional description
 *      Evaluate a scalar item from an array.
 *
 **************************************/
	SLONG subscripts[16], *p;
	JRD_NOD list, *ptr, *end;
	DSC *desc;
	JRD_REQ request;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	desc = EVL_expr(tdbb, node->nod_arg[e_scl_field]);
	request = tdbb->tdbb_request;

	if (request->req_flags & req_null)
		return NULL;

	if (desc->dsc_dtype != dtype_array)
		IBERROR(261);			/* msg 261 scalar operator used on field which is not an array */

	list = node->nod_arg[e_scl_subscripts];

	for (ptr = list->nod_arg, end = ptr + list->nod_count, p = subscripts;
		 ptr < end;)
	{
		*p++ = MOV_get_long(EVL_expr(tdbb, *ptr++), 0);
		if (request->req_flags & req_null)
			return NULL;
	}

	BLB_scalar(tdbb,
			   request->req_transaction,
			   reinterpret_cast < bid * >(desc->dsc_address),
			   list->nod_count,
			   subscripts, reinterpret_cast < vlu * >(&impure->vlu_desc));

	return &impure->vlu_desc;
}


static SSHORT sleuth(TDBB tdbb, JRD_NOD node, DSC * desc1, DSC * desc2)
{
/**************************************
 *
 *      s l e u t h
 *
 **************************************
 *
 * Functional description
 *      Execute SLEUTH operator.
 *
 **************************************/
	BLB blob;
	DSC *desc3;
	UCHAR *p1, *p2, temp1[TEMP_LENGTH], temp2[TEMP_LENGTH],
		buffer[BUFFER_LARGE], control[BUFFER_SMALL];
	SSHORT l1, l2;
	USHORT ttype;
	STR data_str = NULL, match_str = NULL, sleuth_str = NULL;
	SSHORT ret_val;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

/* Choose interpretation for the operation */

	if (desc1->dsc_dtype == dtype_blob) {
		if (desc1->dsc_sub_type == BLOB_text)
			ttype = desc1->dsc_scale;	/* Load blob character set */
		else
			ttype = INTL_TTYPE(desc2);
	}
	else
		ttype = INTL_TTYPE(desc1);

	TextType* obj =
		INTL_texttype_lookup(tdbb, ttype, (FPTR_VOID) ERR_post, NULL);

/* Get operator definition string (control string) */

	desc3 = EVL_expr(tdbb, node->nod_arg[2]);
	l1 =
		MOV_make_string2(desc3, ttype, &p1,
						 reinterpret_cast<vary*>(temp1), TEMP_SIZE(temp1),
						 &sleuth_str);
/* Get address and length of search string */
	l2 =
		MOV_make_string2(desc2, ttype, &p2,
						 reinterpret_cast<vary*>(temp2), TEMP_SIZE(temp2),
						 &match_str);
/* Merge search and control strings */
	l2 = obj->sleuth_merge(tdbb, p2, l2, p1, l1, control,
										 BUFFER_SMALL);

/* l2 is result's byte-count */

/* Note: resulting string from sleuth_merge is either UCS2_CHAR or NCHAR 
   and never Multibyte (see note in EVL_mb_sleuth_check) */

	if (desc1->dsc_dtype != dtype_blob) {
		/* Source is not a blob, do a simple search */

		l1 =
			MOV_make_string2(desc1, ttype, &p1,
							 reinterpret_cast<vary*>(temp1),
							 TEMP_SIZE(temp1), &data_str);
		ret_val = obj->sleuth_check(tdbb, 0, p1, l1, control, l2);
	}
	else {
		/* Source string is a blob, things get interesting */

		blob =
			BLB_open(tdbb, tdbb->tdbb_request->req_transaction,
					 reinterpret_cast < bid * >(desc1->dsc_address));

		ret_val = FALSE;
		while (!(blob->blb_flags & BLB_eof))
		{
			l1 = BLB_get_segment(tdbb, blob, buffer, sizeof(buffer));
			if (obj->sleuth_check(tdbb, 0, buffer, l1, control, l2))
			{
				ret_val = TRUE;
				break;
			}
		}

		BLB_close(tdbb, blob);
	}

	if (data_str)
		delete data_str;
	if (sleuth_str)
		delete sleuth_str;
	if (match_str)
		delete match_str;

	return ret_val;
}


static SSHORT string_boolean(TDBB tdbb, JRD_NOD node, DSC * desc1, DSC * desc2)
{
/**************************************
 *
 *      s t r i n g _ b o o l e a n
 *
 **************************************
 *
 * Functional description
 *      Perform one of the complex string functions CONTAINING, MATCHES,
 *      or STARTS WITH.
 *
 **************************************/
	BLB blob;

	UCHAR *p1, *p2, temp1[TEMP_LENGTH], temp2[TEMP_LENGTH],
		buffer[BUFFER_LARGE];
	SSHORT l1, l2;
	USHORT type1, xtype1;
	STR match_str = NULL;
	SSHORT ret_val;

	SET_TDBB(tdbb);

	DEV_BLKCHK(node, type_nod);

	if (desc1->dsc_dtype != dtype_blob) {
		/* Source is not a blob, do a simple search */

		/* Get text type of data string */

		type1 = INTL_TEXT_TYPE(*desc1);

		/* Get address and length of search string - convert to datatype of data */

		l2 =
			MOV_make_string2(desc2, type1, &p2,
							 reinterpret_cast < vary * >(temp2),
							 TEMP_SIZE(temp2), &match_str);
		l1 =
			MOV_get_string_ptr(desc1, &xtype1, &p1,
							   reinterpret_cast < vary * >(temp1),
							   TEMP_SIZE(temp1));

		assert(xtype1 == type1);
		ret_val = string_function(tdbb, node, l1, p1, l2, p2, type1);
	}
	else {
		/* Source string is a blob, things get interesting */

		/* Get address and length of search string - make it string if neccessary
		 * but don't transliterate character set if the source blob is binary
		 */

		if (desc1->dsc_sub_type == BLOB_text) {
			type1 = desc1->dsc_scale;	/* pick up character set of blob */
			l2 =
				MOV_make_string2(desc2, type1, &p2,
								 reinterpret_cast < vary * >(temp2),
								 TEMP_SIZE(temp2), &match_str);
		}
		else {
			type1 = ttype_none;	/* Do byte matching */
			l2 =
				MOV_get_string(desc2, &p2, reinterpret_cast < vary * >(temp2),
							   TEMP_SIZE(temp2));
		}

		blob =
			BLB_open(tdbb, tdbb->tdbb_request->req_transaction,
					 reinterpret_cast < bid * >(desc1->dsc_address));

		/* Performs the string_function on each segment of the blob until
		   a positive result is obtained */

		ret_val = FALSE;
		while (!(blob->blb_flags & BLB_eof)) {
			l1 = BLB_get_segment(tdbb, blob, buffer, sizeof(buffer));
			if (string_function(tdbb, node, l1, buffer, l2, p2, type1)) {
				ret_val = TRUE;
				break;
			}
			if (node->nod_type == nod_starts)
				break;
		}

		BLB_close(tdbb, blob);
	}

	if (match_str)
		delete match_str;

	return ret_val;
}


static SSHORT string_function(
							  TDBB tdbb,
							  JRD_NOD node,
							  SSHORT l1,
							  UCHAR * p1, SSHORT l2, UCHAR * p2, USHORT ttype)
{
/**************************************
 *
 *      s t r i n g _ f u n c t i o n
 *
 **************************************
 *
 * Functional description
 *      Perform one of the complex string functions CONTAINING, MATCHES,
 *      or STARTS WITH.
 *
 **************************************/
	class TextType* obj;

	SET_TDBB(tdbb);
	DEV_BLKCHK(node, type_nod);

/* Handle "STARTS WITH" */

	if (node->nod_type == nod_starts) {
		if (l1 < l2)
			return FALSE;
		while (--l2 >= 0)
			if (*p1++ != *p2++)
				return FALSE;
		return TRUE;
	}

	obj = INTL_texttype_lookup(tdbb, ttype, (FPTR_VOID) ERR_post, NULL);

/* Handle contains */

	if (node->nod_type == nod_contains) {
		return obj->contains(tdbb, p1, l1, p2, l2);
	}

/* Handle LIKE and MATCHES*/

	if (node->nod_type == nod_like) {
		UCS2_CHAR escape;

		escape = 0;
		/* ensure 3rd argument (escape char) is in operation text type */
		if (node->nod_count == 3) {
			DSC* dsc = EVL_expr(tdbb, node->nod_arg[2]);
			if (tdbb->tdbb_request->req_flags & req_null) {
				return FALSE;
			}

			const char* q1;
			USHORT l3, consumed;
			UCHAR temp3[TEMP_LENGTH];

			/* Convert ESCAPE to operation character set */
			l3 = MOV_make_string(dsc,
								 ttype, &q1, (VARY *) temp3,
								 TEMP_SIZE(temp3));
			if (!l3)
				ERR_post(gds_like_escape_invalid, 0);
			/* Grab the first character from the string */
			consumed = obj->mbtowc(&escape, reinterpret_cast<unsigned char*>(const_cast<char*>(q1)), l3);

			/* If characters left, or null byte character, return error */
			if (consumed <= 0 || consumed != l3 || (escape == 0))
				ERR_post(gds_like_escape_invalid, 0);

		}
		return obj->like(tdbb, p1, l1, p2, l2, escape);
	}

	return obj->matches(tdbb, p1, l1, p2, l2);
}


static DSC *substring(
					  TDBB tdbb,
					  VLU impure, DSC * value, USHORT offset, USHORT length)
{
/**************************************
 *
 *      s u b s t r i n g
 *
 **************************************
 *
 * Functional description
 *      Perform substring function.
 *
 **************************************/
	DSC desc;
	UCHAR temp[32];
	USHORT ttype;
	TextType *obj1 = 0;
	/* CVC: I didn't bother to define a larger buffer because:
			- Native types when converted to string don't reach 31 bytes plus terminator.
			- String types do not need and do not use the buffer ("temp") to be pulled.
			- The types that can cause an error() issued inside the low level MOV/CVT
			routines because the "temp" is not enough are blob and array but at this time
			they aren't accepted, so they will cause error() to be called anyway.
	*/

	SET_TDBB(tdbb);
	desc.dsc_dtype = dtype_text;
	desc.dsc_scale = 0;

	if (dtype_blob == value->dsc_dtype && (BLOB_text != value->dsc_sub_type
		|| (ttype = value->dsc_scale) == ttype_ascii || ttype == ttype_none || ttype == ttype_binary
		|| ((obj1 = INTL_texttype_lookup(tdbb, ttype, (FPTR_VOID) ERR_post, NULL)) != 0
			&& 1 == obj1->getBytesPerChar())))
	{
		/* Source string is a blob, things get interesting. */

		BLB blob = BLB_open(tdbb, tdbb->tdbb_request->req_transaction,
							reinterpret_cast<BID>(value->dsc_address));
		if (!blob->blb_length || blob->blb_length <= offset)
		{
			desc.dsc_length = 0;
			INTL_ASSIGN_TTYPE(&desc, value->dsc_scale);
			BLB_close(tdbb, blob);
			EVL_make_value(tdbb, &desc, impure);
		}
		else
		{
			USHORT bufflen = MAX(BUFFER_LARGE, length);
			STR temp_str = FB_NEW_RPT(*tdbb->tdbb_default, sizeof(UCHAR) * bufflen) str();
			UCHAR *buffer = temp_str->str_data;
		
			USHORT datalen = 0;
			while (!(blob->blb_flags & BLB_eof) && offset)
			{
				/* Both cases are the same for now. Let's see if we can optimize in the future. */
				USHORT waste = MIN(bufflen, offset);
				USHORT l1 = BLB_get_segment(tdbb, blob, buffer, waste);
				offset -= l1;
			}
			assert(!offset && !(blob->blb_flags & BLB_eof));
			datalen = BLB_get_data(tdbb, blob, buffer, length);
			assert(datalen && datalen <= length);
			desc.dsc_length = datalen;
			desc.dsc_address = buffer;
			INTL_ASSIGN_TTYPE(&desc, value->dsc_scale);
			EVL_make_value(tdbb, &desc, impure);
			delete temp_str;
		}
	
		return &impure->vlu_desc;
	}

	desc.dsc_length =
		MOV_get_string_ptr(value, &ttype, &desc.dsc_address,
						   reinterpret_cast < vary * >(temp), sizeof(temp));
	INTL_ASSIGN_TTYPE(&desc, ttype);

	/* CVC: Why bother? If the offset is greater or equal than the length in bytes,
			it's impossible that the offset be less than the length in an international charset. */
	if (offset >= desc.dsc_length || !length)
	{
		desc.dsc_length = 0;
		EVL_make_value(tdbb, &desc, impure);
	}
	/* CVC: God save the king if the engine doesn't protect itself against buffer overruns,
			because intl.h defines UNICODE as the type of most system relations' string fields.
			Also, the field charset can come as 127 (dynamic) when it comes from system triggers,
			but it's resolved by INTL_obj_lookup() to UNICODE_FSS in the cases I observed. Here I cannot
			distinguish between user calls and system calls. Unlike the original ASCII substring(),
			this one will get correctly the amount of UNICODE characters requested. */
	else if (desc.dsc_ttype == ttype_ascii || desc.dsc_ttype == ttype_none
		|| ttype == ttype_binary
		/*|| desc.dsc_ttype == ttype_metadata) */)
	{
		/* Redundant.
		if (offset >= desc.dsc_length)
			desc.dsc_length = 0;
		else */
		desc.dsc_address += offset;
		desc.dsc_length -= offset;
		if (length < desc.dsc_length)
			desc.dsc_length = length;
		EVL_make_value(tdbb, &desc, impure);
	}
	else
	{
		/* CVC: ATTENTION:
				I couldn't find an appropriate message for this failure among current registered
				messages, so I will return empty.
				Finally I decided to use arithmetic exception or numeric overflow. */
		TextType *text_obj = 0;
		UCHAR *p = (UCHAR*) desc.dsc_address;
		USHORT pcount = desc.dsc_length;
		BOOLEAN failure = FALSE;
		while (offset && pcount)
		{
			if (!INTL_getch(tdbb, &text_obj, INTL_TTYPE(&desc), &p, &pcount))
			{
				failure = TRUE;
				break;
			}
			--offset;
		}
		/* If we failed or we exhausted our available characters before finding the
		starting position, nothing more to do. */
		if (failure || !pcount) {
			desc.dsc_length = 0;
        }
		else {
			/* Keep our starting pos safe. */
			desc.dsc_address = p;
			while (length && pcount) {
				if (!INTL_getch(tdbb, &text_obj, INTL_TTYPE(&desc), &p, &pcount))
				{
					failure = TRUE;
					break;
				}
				--length;
			}
			/* We can't use length or pcount as error conditions here, since the
			target string can be shorter, longer or equal length than the requested. */
			if (failure)
				desc.dsc_length = 0;
			else
			{
				/* I thought I would need more operations here, but... */
				desc.dsc_length = p - (UCHAR*) desc.dsc_address;
			}
		}
		if (failure)
			ERR_post(gds_arith_except, 0);

		EVL_make_value(tdbb, &desc, impure);
	}
	
	return &impure->vlu_desc;
}


static DSC *upcase(TDBB tdbb, DSC * value, VLU impure)
{
/**************************************
 *
 *      u p c a s e
 *
 **************************************
 *
 * Functional description
 *      Upcase a string.
 *
 **************************************/
	USHORT temp[16];
	DSC desc;
	UCHAR *p, *end;
	USHORT ttype;

	SET_TDBB(tdbb);
	desc.dsc_length =
		MOV_get_string_ptr(value, &ttype, &desc.dsc_address,
						   reinterpret_cast < vary * >(temp), sizeof(temp));
	desc.dsc_dtype = dtype_text;
	INTL_ASSIGN_TTYPE(&desc, ttype);
	EVL_make_value(tdbb, &desc, impure);

	if ((desc.dsc_ttype == ttype_ascii) ||
		(desc.dsc_ttype == ttype_none) || (desc.dsc_ttype == ttype_metadata)) {
		for (p = (UCHAR *) impure->vlu_desc.dsc_address, end =
			 p + impure->vlu_desc.dsc_length; p < end; p++)
			*p = UPPER7(*p);
	}
	else
		INTL_str_to_upper(tdbb, &impure->vlu_desc);

	return &impure->vlu_desc;
}


static DSC *internal_info(TDBB tdbb, DSC * value, VLU impure)
{
/**************************************
 *
 *      i n t e r n a l _ i n f o
 *
 **************************************
 *
 * Functional description
 *      Return a given element
 *      of the internal engine data.
 *
 **************************************/
	EVL_make_value(tdbb, value, impure);

	internal_info_id id =
		*reinterpret_cast<internal_info_id*>(impure->vlu_desc.dsc_address);

	switch (id)
	{
	case internal_connection_id:
		impure->vlu_misc.vlu_long = PAG_attachment_id();
		break;
	case internal_transaction_id:
		impure->vlu_misc.vlu_long = tdbb->tdbb_transaction->tra_number;
		break;
	case internal_gdscode:
		impure->vlu_misc.vlu_long =
			(tdbb->tdbb_request->req_last_xcp.xcp_type == xcp_gds_code) ?
				tdbb->tdbb_request->req_last_xcp.xcp_code : 0;
		break;
	case internal_sqlcode:
		impure->vlu_misc.vlu_long =
			(tdbb->tdbb_request->req_last_xcp.xcp_type == xcp_sql_code) ?
				tdbb->tdbb_request->req_last_xcp.xcp_code : 0;
		break;
	case internal_rows_affected:
		impure->vlu_misc.vlu_long = tdbb->tdbb_request->req_records_affected;
		break;
	case internal_trigger_action:
		impure->vlu_misc.vlu_long = tdbb->tdbb_request->req_trigger_action;
		break;
	default:
		BUGCHECK(232);	/* msg 232 EVL_expr: invalid operation */
	}
	impure->vlu_desc.dsc_address = (UCHAR *) &impure->vlu_misc.vlu_long;

	return &impure->vlu_desc;
}
