/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		eval.c
 *	DESCRIPTION:	Value and boolean expression evaluator
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
#include "../jrd/ib_stdio.h"
#include <string.h>
#include "../jrd/gds.h"
#include "../jrd/common.h"
#include "../qli/dtr.h"
#include "../qli/exe.h"
#include "../qli/err_proto.h"
#include "../qli/eval_proto.h"
#include "../qli/exe_proto.h"
#include "../qli/form_proto.h"
#include "../qli/lex_proto.h"
#include "../qli/mov_proto.h"
#include "../qli/picst_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/utl_proto.h"

typedef vary VARY;


extern USHORT QLI_prompt_count, QLI_reprompt;
#ifdef PYXIS
extern USHORT sw_forms;
#endif
static SLONG execute_any(QLI_NOD);
static DSC *execute_concatenate(QLI_NOD, DSC *, DSC *);
static DSC *execute_edit(QLI_NOD);
static DSC *execute_function(QLI_NOD);
static DSC *execute_prompt(QLI_NOD);
static DSC *execute_statistical(QLI_NOD);
static int like(UCHAR *, SSHORT, UCHAR *, SSHORT, UCHAR);
static TEXT *make_blob_buffer(FRBRD *, USHORT *);
static int matches(TEXT *, SSHORT, TEXT *, SSHORT);
static int sleuth(QLI_NOD, DSC *, DSC *, DSC *);
static int sleuth_check(USHORT, UCHAR *, UCHAR *, UCHAR *, UCHAR *);
static int sleuth_class(USHORT, UCHAR *, UCHAR *, UCHAR);
static int sleuth_merge(UCHAR *, UCHAR *, UCHAR *, UCHAR *, UCHAR *);
static int string_boolean(QLI_NOD);
static int string_function(QLI_NOD, SSHORT, TEXT *, SSHORT, TEXT *);

#define	TEMP_LENGTH		128
#define SLEUTH_insensitive	1
#define COND_UPPER(c)		((flags & SLEUTH_insensitive) ? UPPER(c) : c)

static UCHAR special[127] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
		0, 1, 1, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
};

static TEXT prompt[2][128] = { "", "" };


int EVAL_boolean( QLI_NOD node)
{
/**************************************
 *
 *	E V A L _ b o o l e a n
 *
 **************************************
 *
 * Functional description
 *	Evaluate a boolean expression.
 *
 **************************************/
	DSC *value1, *value2;
	int result;

	switch (node->nod_type) {
	case nod_eql:
	case nod_neq:
	case nod_gtr:
	case nod_geq:
	case nod_leq:
	case nod_lss:
	case nod_between:
		if (!(value1 = EVAL_value(node->nod_arg[0])) ||
			(value1->dsc_missing & DSC_missing) ||
			!(value2 = EVAL_value(node->nod_arg[1])) ||
			(value2->dsc_missing & DSC_missing)) return FALSE;
		if (node->nod_flags & nod_comparison)
			result = MOVQ_compare(value1, value2);
		break;

	case nod_and:
	case nod_or:
	case nod_not:
		result = EVAL_boolean(node->nod_arg[0]);
		break;

	case nod_containing:
	case nod_matches:
	case nod_like:
	case nod_starts:
	case nod_sleuth:
		result = string_boolean(node);
	}

	switch (node->nod_type) {
	case nod_eql:
		return (result == 0);
	case nod_neq:
		return (result != 0);
	case nod_gtr:
		return (result > 0);
	case nod_geq:
		return (result >= 0);
	case nod_leq:
		return (result <= 0);
	case nod_lss:
		return (result < 0);

	case nod_between:
		if (result < 0)
			return FALSE;
		if (!(value2 = EVAL_value(node->nod_arg[2])) ||
			(value2->dsc_missing & DSC_missing)) return FALSE;
		result = MOVQ_compare(value1, value2);
		if (result > 0)
			return FALSE;
		return TRUE;

	case nod_missing:
		value1 = EVAL_value(node->nod_arg[0]);
		if (value1)
			return (value1->dsc_missing & DSC_missing);
		return TRUE;

	case nod_and:
		if (!result)
			return FALSE;
		return EVAL_boolean(node->nod_arg[1]);

	case nod_or:
		if (result)
			return TRUE;
		return EVAL_boolean(node->nod_arg[1]);

	case nod_not:
		return !result;

	case nod_any:
	case nod_unique:
		return execute_any(node);

	case nod_sleuth:
	case nod_containing:
	case nod_matches:
	case nod_like:
	case nod_starts:
		return result;

	default:
		BUGCHECK(28);			/* Msg28 EVAL_boolean: not finished */
		return FALSE;
	}
}


void EVAL_break_compute( QLI_NOD node)
{
/**************************************
 *
 *	E V A L _ b r e a k _ c o m p u t e
 *
 **************************************
 *
 * Functional description
 *	Compute the value of a statistical expression at control
 *	break.  The only function that needs any work is average.
 *
 **************************************/

	if (node->nod_type == nod_rpt_average && node->nod_arg[e_stt_default])
		if (node->nod_desc.dsc_dtype == dtype_long)
			*(SLONG *) node->nod_desc.dsc_address /= (SLONG) node->
				nod_arg[e_stt_default];
		else
			*(double *) node->nod_desc.dsc_address /= (SLONG) node->
				nod_arg[e_stt_default];
}


void EVAL_break_increment( QLI_NOD node)
{
/**************************************
 *
 *	E V A L _ b r e a k _ i n c r e m e n t
 *
 **************************************
 *
 * Functional description
 *	Initialize a report-local statistical function.
 *
 **************************************/
	DSC *desc1, *desc2;
	SLONG count;
	SSHORT comparison;

	desc1 = &node->nod_desc;

/* Knock off count as trivial */

	if (node->nod_type == nod_rpt_count) {
		*(SLONG *) node->nod_desc.dsc_address += 1;
		return;
	}

/* Evaluate the sub-expression.  If null, don't bother to do anything
   more.  If not, bump the number of records involved */

	if (!(desc2 = EVAL_value(node->nod_arg[e_stt_value])))
		return;

/* If this is the first value, just move it in. */

	count = (SLONG) node->nod_arg[e_stt_default] + 1;
	if (count == 1) {
		if (desc2->dsc_missing)
			desc1->dsc_missing = DSC_missing;
		else {
			desc1->dsc_missing = FALSE;
			MOVQ_move(desc2, desc1);
			node->nod_arg[e_stt_default] = (QLI_NOD) (SLONG) count;
		}
		return;
	}
	else if (desc2->dsc_missing)
		return;

	node->nod_arg[e_stt_default] = (QLI_NOD) (SLONG) count;
	desc1->dsc_missing = FALSE;

/* Finish off as per operator */

	switch (node->nod_type) {
	case nod_rpt_min:
	case nod_rpt_max:
		if (!(comparison = MOVQ_compare(desc2, desc1)))
			break;
		if ((comparison > 0 && node->nod_type == nod_rpt_max) ||
			(comparison < 0 && node->nod_type == nod_rpt_min))
			MOVQ_move(desc2, desc1);
		break;

	case nod_rpt_total:
	case nod_rpt_average:
		if (desc1->dsc_dtype == dtype_long)
			*(SLONG *) desc1->dsc_address +=
				MOVQ_get_long(desc2, desc1->dsc_scale);
		else
			*(double *) desc1->dsc_address += MOVQ_get_double(desc2);
		break;
	}
}


void EVAL_break_init( QLI_NOD node)
{
/**************************************
 *
 *	E V A L _ b r e a k _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize a report-local statistical function.
 *
 **************************************/

	node->nod_arg[e_stt_default] = 0;
	node->nod_desc.dsc_missing = FALSE;

	if (node->nod_type == nod_rpt_count)
		*(SLONG *) (node->nod_desc.dsc_address) = 0;
}


DSC *EVAL_parameter(PAR parameter)
{
/**************************************
 *
 *	E V A L _ p a r a m e t e r
 *
 **************************************
 *
 * Functional description
 *	Compute the descriptor for a parameter.
 *
 **************************************/
	DSC *desc;
	QLI_MSG message;
	PAR missing_parameter;
	USHORT *missing_flag;

	desc = &parameter->par_desc;
	desc->dsc_missing = FALSE;
	message = parameter->par_message;

	if (missing_parameter = parameter->par_missing) {
		missing_flag =
			(USHORT *) (message->msg_buffer + missing_parameter->par_offset);
		desc->dsc_missing = (*missing_flag) ? DSC_missing : 0;
	}

	desc->dsc_address = message->msg_buffer + parameter->par_offset;

	return desc;
}


DSC *EVAL_value(QLI_NOD node)
{
/**************************************
 *
 *	E V A L _ v a l u e
 *
 **************************************
 *
 * Functional description
 *	Evaluate a value node.
 *
 **************************************/
	QLI_NOD *ptr, *end_ptr;
	QLI_FLD field;
	DSC *values[4], **value, *desc, *desc2;
	UCHAR *p;
	double d1;

/* Start by evaluating sub-expressions (where appropriate) */

	desc = &node->nod_desc;
	ptr = node->nod_arg;
	end_ptr = ptr + node->nod_count;

	for (value = values; ptr < end_ptr; ptr++, value++) {
		*value = EVAL_value(*ptr);
		if (node->nod_flags & nod_partial)
			break;
	}

	switch (node->nod_type) {
	case nod_edit_blob:
		return execute_edit(node);

	case nod_rpt_max:
	case nod_rpt_min:
	case nod_rpt_total:
	case nod_rpt_average:
		if (!(SLONG) node->nod_arg[e_stt_default])
			desc->dsc_missing = DSC_missing;

	case nod_rpt_count:
	case nod_constant:
		return desc;

	case nod_variable:
		field = (QLI_FLD) node->nod_arg[e_fld_field];
		desc->dsc_missing =
			(field->fld_flags & FLD_missing) ? DSC_missing : 0;
		return desc;

	case nod_field:
		return EVAL_value(node->nod_arg[e_fld_reference]);

	case nod_reference:
		return EVAL_parameter(node->nod_import);

	case nod_null:
		return desc;

	case nod_add:
		if ((values[0]->dsc_missing & DSC_missing) ||
			(values[1]->dsc_missing & DSC_missing)) {
			desc->dsc_missing = DSC_missing;
			return desc;
		}
		desc->dsc_missing = FALSE;
		if (node->nod_flags & nod_date) {
			d1 = MOVQ_date_to_double(values[0]) + MOVQ_get_double(values[1]);
			MOVQ_double_to_date(d1, (long*) desc->dsc_address);
		}
		else if (desc->dsc_dtype == dtype_long)
			*((SLONG *) desc->dsc_address) =
				MOVQ_get_long(values[0], desc->dsc_scale) +
				MOVQ_get_long(values[1], desc->dsc_scale);
		else
			*((double *) desc->dsc_address) =
				MOVQ_get_double(values[0]) + MOVQ_get_double(values[1]);
		return desc;

	case nod_subtract:
		if ((values[0]->dsc_missing & DSC_missing) ||
			(values[1]->dsc_missing & DSC_missing)) {
			desc->dsc_missing = DSC_missing;
			return desc;
		}
		desc->dsc_missing = FALSE;
		if (node->nod_flags & nod_date) {
			*((double *) desc->dsc_address) =
				MOVQ_date_to_double(values[0]) -
				MOVQ_date_to_double(values[1]);
		}
		else if (desc->dsc_dtype == dtype_long)
			*((SLONG *) desc->dsc_address) =
				MOVQ_get_long(values[0], desc->dsc_scale) -
				MOVQ_get_long(values[1], desc->dsc_scale);
		else
			*((double *) desc->dsc_address) =
				MOVQ_get_double(values[0]) - MOVQ_get_double(values[1]);
		return desc;

	case nod_divide:
		if ((values[0]->dsc_missing & DSC_missing) ||
			(values[1]->dsc_missing & DSC_missing)) {
			desc->dsc_missing = DSC_missing;
			return desc;
		}
		desc->dsc_missing = FALSE;
		*((double *) desc->dsc_address) =
			MOVQ_get_double(values[0]) / MOVQ_get_double(values[1]);
		return desc;

	case nod_multiply:
		if ((values[0]->dsc_missing & DSC_missing) ||
			(values[1]->dsc_missing & DSC_missing)) {
			desc->dsc_missing = DSC_missing;
			return desc;
		}
		desc->dsc_missing = FALSE;
		if (desc->dsc_dtype == dtype_long)
			*((SLONG *) desc->dsc_address) =
				MOVQ_get_long(values[0], values[0]->dsc_scale) *
				MOVQ_get_long(values[1], values[1]->dsc_scale);
		else
			*((double *) desc->dsc_address) =
				MOVQ_get_double(values[0]) * MOVQ_get_double(values[1]);
		return desc;

	case nod_negate:
		if (values[0]->dsc_missing & DSC_missing) {
			desc->dsc_missing = DSC_missing;
			return desc;
		}
		desc->dsc_missing = FALSE;
		switch (desc->dsc_dtype) {
		case dtype_short:
			*((SSHORT *) desc->dsc_address) =
				-MOVQ_get_long(values[0], desc->dsc_scale);
			break;

		case dtype_long:
			*((SLONG *) desc->dsc_address) =
				-MOVQ_get_long(values[0], desc->dsc_scale);
			break;

		case dtype_real:
			*((float *) desc->dsc_address) = -MOVQ_get_double(values[0]);
			break;

		case dtype_double:
			*((double *) desc->dsc_address) = -MOVQ_get_double(values[0]);
			break;

		default:
			IBERROR(30);		/* Msg30 data type not supported for arithmetic */
		}
		return desc;

	case nod_prompt:
		if (!prompt[0][0]) {
			ERRQ_msg_get(499, prompt[0]);	/* Msg499 Re-enter */
			ERRQ_msg_get(500, prompt[1]);	/* Msg500 Enter    */
		}
		return execute_prompt(node);

	case nod_concatenate:
		return execute_concatenate(node, values[0], values[1]);

	case nod_function:
		return execute_function(node);

	case nod_max:
	case nod_min:
	case nod_count:
	case nod_average:
	case nod_total:
	case nod_from:
		return execute_statistical(node);

	case nod_running_count:
		*(SLONG *) (desc->dsc_address) += 1;
		return desc;

	case nod_running_total:
		if (desc2 = EVAL_value(node->nod_arg[e_stt_value])) {
			if (desc2->dsc_missing & DSC_missing)
				return desc;
			if (desc->dsc_dtype == dtype_long)
				*(SLONG *) desc->dsc_address +=
					MOVQ_get_long(desc2, desc->dsc_scale);
			else
				*(double *) desc->dsc_address += MOVQ_get_double(desc2);
		}
		return desc;

	case nod_format:
		p = desc->dsc_address;
		PIC_edit(values[0], (pics*) node->nod_arg[e_fmt_picture], (TEXT**) &p,
				 desc->dsc_length);
		desc->dsc_length = p - desc->dsc_address;
		return desc;
#ifdef PYXIS
	case nod_form_field:
		return FORM_get_field(node);
#endif
	case nod_user_name:
		IBERROR(31);			/* Msg31 user name is supported only in RSEs temporarily */

	case nod_parameter:
	case nod_position:

	case nod_substr:

	case nod_via:

	default:
		BUGCHECK(29);			/* Msg29 EVAL_value: not finished */
		return NULL;
	}
}


static SLONG execute_any( QLI_NOD node)
{
/**************************************
 *
 *	e x e c u t e _ a n y
 *
 **************************************
 *
 * Functional description
 *	Execute an ANY expression.  This may require that a request get
 *	started, a message sent, and a message received for each
 *	record.  At the other end of the spectrum, there may be
 *	absolutely nothing to do.
 *
 **************************************/
	QLI_REQ request;
	QLI_MSG message;

/* If there is a request associated  with the node, start it and possibly
   send a message along with it. */

	if (request = (QLI_REQ) node->nod_arg[e_any_request])
		EXEC_start_request(request, (QLI_MSG) node->nod_arg[e_any_send]);
	else if (message = (QLI_MSG) node->nod_arg[e_any_send])
		EXEC_send(message);

	message = (QLI_MSG) node->nod_arg[e_any_receive];
	EXEC_receive(message, 0);

	return MOVQ_get_long(EVAL_parameter(node->nod_import), 0);
}


static DSC *execute_concatenate( QLI_NOD node, DSC * value1, DSC * value2)
{
/**************************************
 *
 *	e x e c u t e _ c o n c a t e n a t e
 *
 **************************************
 *
 * Functional description
 *	Concatenate two strings.
 *
 **************************************/
	DSC *desc;
	VARY *vary;
	TEXT *p, temp1[32], temp2[32], *address1, *address2;
	USHORT length1, length2;

	length1 = MOVQ_get_string(value1, &address1, (VARY*)temp1, sizeof(temp1));
	length2 = MOVQ_get_string(value2, &address2, (VARY*)temp2, sizeof(temp2));
	desc = &node->nod_desc;
	vary = (VARY *) desc->dsc_address;
	p = vary->vary_string;
	length1 = MIN(length1, desc->dsc_length - 2);
	length2 = MAX(MIN(length2, desc->dsc_length - 2 - length1), 0);

	if (length1)
		do
			*p++ = *address1++;
		while (--length1);

	if (length2)
		do
			*p++ = *address2++;
		while (--length2);

	vary->vary_length = p - vary->vary_string;

	return desc;
}


static DSC *execute_edit( QLI_NOD node)
{
/**************************************
 *
 *	e x e c u t e _ e d i t
 *
 **************************************
 *
 * Functional description
 *	Edit a blob.  If there's an existing
 *	blob for input, force the descriptor
 *	to quad to get move the id.
 *
 **************************************/
	SLONG *id;
	DSC *desc;
	DBB dbb;
	TEXT *field_name;

	dbb = (DBB) node->nod_arg[e_edt_dbb];
	id = (SLONG *) & node->nod_arg[e_edt_id1];

	desc = NULL;

	if (node->nod_arg[e_edt_input]) {
		desc = EVAL_value(node->nod_arg[e_edt_input]);
		if (desc && (desc->dsc_dtype == dtype_blob)) {
			desc->dsc_dtype = dtype_quad;
			MOVQ_move(desc, &node->nod_desc);
			desc->dsc_dtype = dtype_blob;
		}
	}
	if (!desc)
		id[0] = id[1] = 0;

	field_name = (TEXT *) node->nod_arg[e_edt_name];
	BLOB_edit((GDS_QUAD_t*)id, dbb->dbb_handle, dbb->dbb_transaction, field_name);

	node->nod_desc.dsc_missing = (id[0] || id[1]) ? 0 : DSC_missing;

	return &node->nod_desc;
}


static DSC *execute_function( QLI_NOD node)
{
/**************************************
 *
 *	e x e c u t e _ f u n c t i o n
 *
 **************************************
 *
 * Functional description
 *	Execute a statistical expression.
 *
 **************************************/
	QLI_REQ request;
	QLI_MSG message;

/* If there is a request associated  with the node, start it and possibly
   send a message along with it. */

	if (request = (QLI_REQ) node->nod_arg[e_fun_request])
		EXEC_start_request(request, (QLI_MSG) node->nod_arg[e_fun_send]);
	else if (message = (QLI_MSG) node->nod_arg[e_fun_send])
		EXEC_send(message);

	return EXEC_receive((QLI_MSG) node->nod_arg[e_fun_receive], node->nod_import);
}


static DSC *execute_prompt( QLI_NOD node)
{
/**************************************
 *
 *	e x e c u t e _ p r o m p t
 *
 **************************************
 *
 * Functional description
 *	Execute a prompt and return a descriptor.  If end of file,
 *	abort request; if a singleton tab, return the previous value,
 *	and if a null string, return MISSING.
 *
 **************************************/
	TEXT *p, string[128], *value, buffer[256];
	DSC *desc;
	USHORT reprompt;
	VARY *data;
	int length, l;
#ifdef PYXIS
	if (sw_forms)
		FORM_reset();
#endif
	ERRQ_pending();
	reprompt = QLI_reprompt;
	desc = &node->nod_desc;
	data = (VARY *) desc->dsc_address;

	value =
		(desc->dsc_length - 2 <= sizeof(buffer)) ? buffer : data->vary_string;
	length =
		(desc->dsc_length - 2 <=
		 sizeof(buffer)) ? sizeof(buffer) : desc->dsc_length - 2;

	for (;;) {
		++QLI_prompt_count;
		if (node->nod_arg[e_prm_prompt]) {
			if (reprompt)
				sprintf(string, "\07%s %s: ", prompt[0],
						node->nod_arg[e_prm_prompt]);
			else
				sprintf(string, "%s %s: ", prompt[1],
						node->nod_arg[e_prm_prompt]);
		}
		else {
			if (reprompt)
				sprintf(string, "\07%s: ", prompt[0]);	/* Msg497 Re-enter */
			else
				sprintf(string, "%s: ", prompt[1]);	/* Msg498 Enter    */
		}

		if (!LEX_get_line(string, value, length)) {
			QLI_abort = TRUE;
			EXEC_poll_abort();
		}

		if (value[0] == '\t' && value[1] == '\n')
			return desc;

		p = value + strlen(value);
		if (p > value && p[-1] == '\n')
			*--p = 0;

		/* Get rid of trailing blanks on non-text data types */

		if (desc->dsc_dtype > dtype_varying) {
			while (p > value && p[-1] == ' ')
				--p;
			*p = 0;
		}

		if ((l = p - value) <= desc->dsc_length - 2) {
			if (value != data->vary_string)
				memcpy(data->vary_string, value, l);
			desc->dsc_missing = (data->vary_length = l) ? 0 : DSC_missing;
			return desc;
		}

		ERRQ_msg_put(32, NULL, NULL, NULL, NULL, NULL);	/* Msg32 Input value is too long */
		reprompt = TRUE;
	}
}


static DSC *execute_statistical( QLI_NOD node)
{
/**************************************
 *
 *	e x e c u t e _ s t a t i s t i c a l
 *
 **************************************
 *
 * Functional description
 *	Execute a statistical expression.
 *
 **************************************/
	QLI_REQ request;
	QLI_MSG message;

/* If there is a request associated  with the node, start it and possibly
   send a message along with it. */

	if (request = (QLI_REQ) node->nod_arg[e_stt_request])
		EXEC_start_request(request, (QLI_MSG) node->nod_arg[e_stt_send]);
	else if (message = (QLI_MSG) node->nod_arg[e_stt_send])
		EXEC_send(message);

	return EXEC_receive((QLI_MSG) node->nod_arg[e_stt_receive], node->nod_import);
}

static int like(
				   UCHAR * p1,
				   SSHORT l1, UCHAR * p2, SSHORT l2, UCHAR escape_char)
{
/**************************************
 *
 *	l i k e
 *
 **************************************
 *
 * Functional description
 *	Return true if a string (p1, l1) matches a given pattern (p2, l2).
 *	The character '_' in the pattern may match any single character
 *	in the the string, and the character '%' may match any sequence
 *	of characters.
 *      Japanese version operates on short-based buffer,
 *      instead of char-based.
 *
 **************************************/
	UCHAR c;
	int escape;

	escape = FALSE;

	while (--l2 >= 0) {
		c = *p2++;
		if (escape_char && !escape && c == escape_char) {
			escape = TRUE;
			continue;
		}
		if (!escape && c == '%') {
			if (l2 == 0)
				return TRUE;
			while (l1)
				if (like(p1++, l1--, p2, l2, escape_char))
					return TRUE;
			return FALSE;
		}
		if (--l1 < 0)
			return FALSE;
		if ((escape || c != '_') && c != *p1)
			return FALSE;
		escape = FALSE;
		p1++;
	}

	return (l1) ? FALSE : TRUE;
}


static TEXT *make_blob_buffer( FRBRD * blob, USHORT * length)
{
/**************************************
 *
 *	m a k e _ b l o b _ b u f f e r
 *
 **************************************
 *
 * Functional description
 *	For a blob, get the max segment length,
 *	check that the buffer allocated is at least
 *	that length, and, if not, allocate a bigger one.
 *
 **************************************/
	SLONG size, segment_count, max_segment;

	gds__blob_size(&blob, &size, &segment_count, &max_segment);

	if (max_segment >= *length) {
		TEXT *buffer;
		*length = max_segment;
		buffer = (TEXT *) gds__alloc((SLONG) * length);
#ifdef DEBUG_GDS_ALLOC
		/* We don't care about QLI specific memory leaks for V4.0 */
		gds_alloc_flag_unfreed((void *) buffer);	/* QLI: don't care */
#endif
		return buffer;
	}

	return 0;
}

static int matches(
					  TEXT * p1, SSHORT l1, TEXT * p2, SSHORT l2)
{
/**************************************
 *
 *	m a t c h e s
 *
 **************************************
 *
 * Functional description
 *	Return true if a string (p1, l1) matches a given pattern (p2, l2).
 *	The character '?' in the pattern may match any single character
 *	in the the string, and the character '*' may match any sequence
 *	of characters.
 *
 **************************************/
	TEXT c;

	while (--l2 >= 0) {
		c = *p2++;
		if (c == '*') {
			if (l2 == 0)
				return TRUE;
			while (l1)
				if (matches(p1++, l1--, p2, l2))
					return TRUE;
			return FALSE;
		}
		if (--l1 < 0 || (c != '?' && UPPER(c) != UPPER(*p1)))
			return FALSE;
		p1++;
	}

	return (l1) ? FALSE : TRUE;
}


static int sleuth( QLI_NOD node, DSC * desc1, DSC * desc2, DSC * desc3)
{
/**************************************
 *
 *	s l e u t h
 *
 **************************************
 *
 * Functional description
 *	Return true if a string (p1, l1) matches a given pattern (p2, l2),
 *	using a pattern language defined in p3, l3.
 *
 **************************************/
	TEXT *p1, *p2, *buffer, temp1[TEMP_LENGTH], temp2[TEMP_LENGTH],
		fixed_buffer[512], control[256];
	FRBRD *blob;
	USHORT buffer_length;
	ISC_STATUS_ARRAY status_vector;
	SSHORT l1, l2, result;
	QLI_CTX context;
	QLI_REQ request;
	DBB dbb;

/* Get operator definition string (control string) */

	l1 = MOVQ_get_string(desc3, &p1, (VARY*) temp1, TEMP_LENGTH);

/* Get address and length of search string */

	l2 = MOVQ_get_string(desc2, &p2, (VARY*) temp2, TEMP_LENGTH);

/* Merge search and control strings */

	l2 = sleuth_merge((UCHAR*) p2, (UCHAR*) (p2 + l2), (UCHAR*) p1, (UCHAR*) (p1 + l1), (UCHAR*) control);

/* If source is not a blob, do a simple search */

	if (desc1->dsc_dtype != dtype_blob) {
		l1 = MOVQ_get_string(desc1, &p1, (VARY*) temp1, TEMP_LENGTH);
		return sleuth_check(0, (UCHAR*) p1, (UCHAR*) (p1 + l1), (UCHAR*) control, (UCHAR*) (control + l2));
	}

/* Source string is a blob, things get interesting */

	result = FALSE;

	blob = EXEC_open_blob(node->nod_arg[0]);

	buffer_length = sizeof(fixed_buffer);

	if (!(buffer = make_blob_buffer( blob, &buffer_length)))
		buffer = fixed_buffer;

	while (!gds__get_segment(status_vector,
							 GDS_REF(blob),
							 (USHORT*) GDS_REF(l1), buffer_length, GDS_VAL(buffer)))
		if (sleuth_check(0, (UCHAR*) buffer, (UCHAR*) (buffer + l1), (UCHAR*) control, (UCHAR*) (control + l2)))
		{
			result = TRUE;
			break;
		}

	if (buffer != fixed_buffer)
		gds__free(buffer);

	if (gds__close_blob(status_vector, GDS_REF(blob))) {
		context = (QLI_CTX) node->nod_arg[e_fld_context];
		request = context->ctx_request;
		dbb = request->req_database;
		ERRQ_database_error(dbb, status_vector);
	}

	return result;
}

static int sleuth_check(
						   USHORT flags,
						   UCHAR * search,
						   UCHAR * end_search,
						   UCHAR * match, UCHAR * end_match)
{
/**************************************
 *
 *	s l e u t h _ c h e c k
 *
 **************************************
 *
 * Functional description
 *	Evaluate the "sleuth" search operator.
 *
 **************************************/
	UCHAR c, d, *class_, *end_class;

	while (match < end_match) {
		c = *match++;
		if ((c == '@' && (c = *match++)) || !special[c])
		{
			c = COND_UPPER(c);
			if (match >= end_match || *match != '*') {
				if (search >= end_search)
					return FALSE;
				d = *search++;
				if (c != COND_UPPER(d))
					return FALSE;
			}
			else {
				++match;
				for (;;)
					if (sleuth_check
						(flags, search, end_search, match,
						 end_match)) return TRUE;
					else if (search < end_search) {
						d = *search++;
						if (c != COND_UPPER(d))
							return FALSE;
					}
					else
						return FALSE;
			}
		}
		else if (c == '?')
			if (match >= end_match || *match != '*') {
				if (search >= end_search)
					return FALSE;
				search++;
			}
			else {
				if (++match >= end_match)
					return TRUE;
				for (;;)
					if (sleuth_check
						(flags, search, end_search, match,
						 end_match)) return TRUE;
					else if (++search >= end_search)
						return FALSE;
			}
		else if (c == '[') {
			class_ = match;
			while (*match++ != ']')
				if (match >= end_match)
					return FALSE;
			end_class = match - 1;
			if (match >= end_match || *match != '*') {
				if (!sleuth_class(flags, class_, end_class, *search++))
					return FALSE;
			}
			else {
				++match;
				for (;;)
					if (sleuth_check
						(flags, search, end_search, match,
						 end_match)) return TRUE;
					else if (search < end_search) {
						if (!sleuth_class
							(flags, class_, end_class,
							 *search++)) return FALSE;
					}
					else
						return FALSE;
			}
		}
		else if (c == '+') {
			c = *match++;
			if (c == 'S' || c == 's')
				flags &= ~SLEUTH_insensitive;
		}
		else if (c == '-') {
			c = *match++;
			if (c == 'S' || c == 's')
				flags |= SLEUTH_insensitive;
		}
	}

	if (search < end_search)
		return FALSE;

	return TRUE;
}


static int sleuth_class( USHORT flags,
						UCHAR * class_, UCHAR * end_class, UCHAR character)
{
/**************************************
 *
 *	s l e u t h _ c l a s s
 *
 **************************************
 *
 * Functional description
 *	See if a character is a member of a class.
 *
 **************************************/
	UCHAR c;
	USHORT result;

	result = TRUE;
	character = COND_UPPER(character);

	if (*class_ == '~') {
		++class_;
		result = FALSE;
	}

	while (class_ < end_class) {
		c = *class_++;
		if (c == '@') {
			if (*class_++ == character)
				return TRUE;
		}
		else if (*class_ == '-') {
			class_ += 2;
			if (character >= c && character <= class_[-1])
				return result;
		}
		else if (character == c)
			return result;
	}

	return (result) ? FALSE : TRUE;
}


static int sleuth_merge(
						UCHAR * match,
						UCHAR * end_match,
						UCHAR * control,
						UCHAR * end_control, UCHAR * combined)
{
/**************************************
 *
 *	s l e u t h _ m e r g e
 *
 **************************************
 *
 * Functional description
 *	Merge the matching pattern and control strings to give a cannonical
 *	matching pattern.  Return the length of the combined string. 
 *
 * 	What this routine does is to take the language template, strip off 
 *	the prefix and put it in the output string, then parse the definitions
 *	into an array of character pointers.  The index array is the defined
 *	character.   The routine then takes the actual match pattern and uses
 *	the characters in it to index into the definitions to produce an equivalent
 *	pattern in the cannonical language.
 *
 *	The silly loop setting *v++ to zero initializes the array up to the
 *	highest character defined (also max_op).  Believe it or not, that part
 *	is not a bug.
 *
 **************************************/
	UCHAR c, *comb, **v, *vector[128], **end_vector, *p, max_op, temp[256],
		*t;

	comb = combined;
	v = vector;
	t = temp;

/* Parse control string into substitution strings and initializing string */

	while (control < end_control) {
		c = *control++;
		if (*control == '=') {
			end_vector = vector + c;
			while (v <= end_vector)
				*v++ = 0;
			*end_vector = t;
			++control;
			while (control < end_control) {
				c = *control++;
				if ((t[-1] == '@') || ((c != ',') && (c != ')')))
					*t++ = c;
				else
					break;
			}
			*t++ = 0;
		}
		else if (c == '@' && control < end_control)
			*comb++ = *control++;
		else if (c == ')')
			break;
		else if (c != '(')
			*comb++ = c;
	}

	max_op = v - vector;

/* Interpret matching string, substituting where appropriate */

	while (c = *match++) {

		/* if we've got a defined character, slurp the definition */
		if (c <= max_op && (p = vector[c])) {
			while (*p)
				*comb++ = *p++;
			/* if we've got the definition of a quote character, slurp the next character too */
			if (comb[-1] == '@' && *match)
				*comb++ = *match++;
		}
		/* at this point we've got a non-match, but as it might be one of ours, quote it. */
		else {
			if (special[c] && comb[-1] != '@')
				*comb++ = '@';
			*comb++ = c;
		}
	}

/* Put in trailing stuff */

	while (control < end_control)
		*comb++ = *control++;

	return comb - combined;
}


static int string_boolean( QLI_NOD node)
{
/**************************************
 *
 *	s t r i n g _ b o o l e a n
 *
 **************************************
 *
 * Functional description
 *	Perform one of the complex string functions CONTAINING, MATCHES,
 *	or STARTS WITH.
 *
 **************************************/
	DSC *desc1, *desc2, *desc3;
	TEXT *p1, *p2, *buffer, fixed_buffer[512];
	TEXT temp1[TEMP_LENGTH], temp2[TEMP_LENGTH];
	FRBRD *blob;
	SSHORT l1, l2, result;
	USHORT buffer_length;
	ISC_STATUS_ARRAY status_vector;
	QLI_CTX context;
	QLI_REQ request;
	DBB dbb;

	if (!(desc1 = EVAL_value(node->nod_arg[0])) ||
		(desc1->dsc_missing & DSC_missing) ||
		!(desc2 = EVAL_value(node->nod_arg[1])) ||
		(desc2->dsc_missing & DSC_missing) ||
		(node->nod_arg[2] && (!(desc3 = EVAL_value(node->nod_arg[2])) ||
							  (desc3->dsc_missing & DSC_missing))))
		return FALSE;

	if (node->nod_type == nod_sleuth)
		return sleuth(node, desc1, desc2, desc3);

/* Get address and length of strings */

	l2 = MOVQ_get_string(desc2, &p2, (VARY*) temp2, TEMP_LENGTH);

/* If source is not a blob, do a simple search */

	if (desc1->dsc_dtype != dtype_blob) {
		l1 = MOVQ_get_string(desc1, &p1, (VARY*) temp1, TEMP_LENGTH);
		return string_function(node, l1, p1, l2, p2);
	}

/* Source string is a blob, things get interesting */

	result = FALSE;
	blob = EXEC_open_blob(node->nod_arg[0]);

	buffer_length = sizeof(fixed_buffer);

	if (!(buffer = make_blob_buffer( blob, &buffer_length)))
		buffer = fixed_buffer;

	while (!gds__get_segment(status_vector,
							 GDS_REF(blob),
							 (USHORT*) GDS_REF(l1),
							 buffer_length,
							 GDS_VAL(buffer)))
			if (string_function(node, l1, buffer, l2, p2)) {
			result = TRUE;
			break;
		}

	if (buffer != fixed_buffer)
		gds__free(buffer);

	if (gds__close_blob(status_vector, GDS_REF(blob))) {
		context = (QLI_CTX) node->nod_arg[e_fld_context];
		request = context->ctx_request;
		dbb = request->req_database;
		ERRQ_database_error(dbb, status_vector);
	}

	return result;
}


static int string_function(
						   QLI_NOD node,
						   SSHORT l1, TEXT * p1, SSHORT l2, TEXT * p2)
{
/**************************************
 *
 *	s t r i n g _ f u n c t i o n
 *
 **************************************
 *
 * Functional description
 *	Perform one of the complex string functions CONTAINING, MATCHES,
 *	or STARTS WITH.
 *
 **************************************/
	TEXT *q1, *q2, c1, c2, temp[16];
	SSHORT l;

/* Handle "STARTS WITH" */

	if (node->nod_type == nod_starts) {
		if (l1 < l2)
			return FALSE;
		while (--l2 >= 0)
			if (*p1++ != *p2++)
				return FALSE;
		return TRUE;
	}

/* Handle CONTAINS */

	if (node->nod_type == nod_containing) {
		while (l1 >= l2) {
			--l1;
			q1 = p1++;
			q2 = p2;
			l = l2;
			do {
				if (--l < 0)
					return TRUE;
				c1 = *q1++;
				c2 = *q2++;
			} while (UPPER(c1) == UPPER(c2));
		}
		return FALSE;
	}

/* Handle LIKE */

	if (node->nod_type == nod_like) {
		c1 = 0;
		if (node->nod_count > 2 &&
			MOVQ_get_string(EVAL_value(node->nod_arg[2]), &q1, (VARY*) temp,
							sizeof(temp))) c1 = *q1;
		if (like((UCHAR*) p1, l1, (UCHAR*) p2, l2, c1))
			return TRUE;
		return FALSE;
	}

/* Handle MATCHES */

	if (node->nod_type == nod_matches)
		if (matches(p1, l1, p2, l2))
			return TRUE;

	return FALSE;
}
