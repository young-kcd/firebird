//____________________________________________________________
//  
//		PROGRAM:	ADA preprocesser
//		MODULE:		ada.cpp
//		DESCRIPTION:	Inserted text generator for ADA
//  
//  The contents of this file are subject to the Interbase Public
//  License Version 1.0 (the "License"); you may not use this file
//  except in compliance with the License. You may obtain a copy
//  of the License at http://www.Inprise.com/IPL.html
//  
//  Software distributed under the License is distributed on an
//  "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
//  or implied. See the License for the specific language governing
//  rights and limitations under the License.
//  
//  The Original Code was created by Inprise Corporation
//  and its predecessors. Portions created by Inprise Corporation are
//  Copyright (C) Inprise Corporation.
//  
//  All Rights Reserved.
//  Contributor(s): ______________________________________.
//  
//
//____________________________________________________________
//
//	$Id: ada.cpp,v 1.30 2003-10-14 22:21:49 brodsom Exp $
//

#include "firebird.h"

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "../jrd/ib_stdio.h"
#include "../jrd/common.h"
#include <stdarg.h>
#include "../jrd/gds.h"
#include "../gpre/gpre.h"
#include "../gpre/pat.h"
#include "../gpre/cmp_proto.h"
#include "../gpre/gpre_proto.h"
#include "../gpre/lang_proto.h"
#include "../gpre/pat_proto.h"
#include "../gpre/prett_proto.h"
#include "../jrd/gds_proto.h"

static void	align (int);
static void	asgn_from (const act*, REF, int);
static void	asgn_to (const act*, REF, int);
static void	asgn_to_proc (REF, int);
static void	gen_any (const act*, int);
static void	gen_at_end (const act*, int);
static void	gen_based (const act*, int);
static void	gen_blob_close (const act*, USHORT);
static void	gen_blob_end (const act*, USHORT);
static void	gen_blob_for (const act*, USHORT);
static void	gen_blob_open (const act*, USHORT);
static void	gen_blr (void*, SSHORT, const char*);
static void	gen_clear_handles (const act*, int);
static void	gen_compile (const act*, int);
static void	gen_create_database (const act*, int);
static int	gen_cursor_close (const act*, GPRE_REQ, int);
static void	gen_cursor_init (const act*, int);
static void	gen_database (const act*, int);
static void	gen_ddl (const act*, int);
static void	gen_drop_database (const act*, int);
static void	gen_dyn_close (const act*, int);
static void	gen_dyn_declare (const act*, int);
static void	gen_dyn_describe(const act*, int, bool);
static void	gen_dyn_execute (const act*, int);
static void	gen_dyn_fetch (const act*, int);
static void	gen_dyn_immediate (const act*, int);
static void	gen_dyn_insert (const act*, int);
static void	gen_dyn_open (const act*, int);
static void	gen_dyn_prepare (const act*, int);
static void	gen_emodify (const act*, int);
static void	gen_estore (const act*, int);
static void	gen_endfor (const act*, int);
static void	gen_erase (const act*, int);
static SSHORT	gen_event_block (const act*);
static void	gen_event_init (const act*, int);
static void	gen_event_wait (const act*, int);
static void	gen_fetch (const act*, int);
static void	gen_finish (const act*, int);
static void	gen_for (const act*, int);
static void	gen_function (const act*, int);
static void	gen_get_or_put_slice(const act*, REF, bool, int);
static void	gen_get_segment (const act*, int);
static void	gen_loop (const act*, int);
static TEXT	*gen_name(TEXT *, REF, bool);
static void	gen_on_error (const act*, USHORT);
static void	gen_procedure (const act*, int);
static void	gen_put_segment (const act*, int);
static void	gen_raw (UCHAR *, enum req_t, int, int);
static void	gen_ready (const act*, int);
static void	gen_receive (const act*, int, POR);
static void	gen_release (const act*, int);
static void	gen_request (GPRE_REQ, int);
static void	gen_return_value (const act*, int);
static void	gen_routine (const act*, int);
static void	gen_s_end (const act*, int);
static void	gen_s_fetch (const act*, int);
static void	gen_s_start (const act*, int);
static void	gen_segment (const act*, int);
static void	gen_select (const act*, int);
static void	gen_send (const act*, POR, int);
static void	gen_slice (const act*, int);
static void	gen_start (const act*, POR, int);
static void	gen_store (const act*, int);
static void	gen_t_start (const act*, int);
static void	gen_tpb (TPB, int);
static void	gen_trans (const act*, int);
static void	gen_type (const act*, int);
static void	gen_update (const act*, int);
static void	gen_variable (const act*, int);
static void	gen_whenever (SWE, int);
static void	make_array_declaration (REF, int);
static void	make_cursor_open_test (enum act_t, GPRE_REQ, int);
static TEXT	*make_name (TEXT *, SYM);
static void	make_ok_test (const act*, GPRE_REQ, int);
static void	make_port (POR, int);
static void	make_ready (DBB, TEXT *, TEXT *, USHORT, GPRE_REQ);
static void	printa (int, TEXT *, ...);
static const TEXT* request_trans (const act*, GPRE_REQ);
static TEXT	*status_vector (const act*);
static void	t_start_auto (const act*, GPRE_REQ, TEXT *, int, bool);

static TEXT output_buffer[512];
static int first_flag;

static const char* COMMENT = "--- ";
const int INDENT	= 3;

static const char* BYTE_DCL			= "interbase.isc_byte";
static const char* BYTE_VECTOR_DCL	= "interbase.isc_vector_byte";
static const char* SHORT_DCL		= "interbase.isc_short";
static const char* USHORT_DCL		= "interbase.isc_ushort";
static const char* LONG_DCL			= "interbase.isc_long";
static const char* LONG_VECTOR_DCL	= "interbase.isc_vector_long";
static const char* EVENT_LIST_DCL	= "interbase.event_list";
static const char* REAL_DCL			= "interbase.isc_float";
static const char* DOUBLE_DCL		= "interbase.isc_double";

static inline void endif(const int column)
{
	printa(column, "end if;");
}

static inline void begin(const int column)
{
	printa(column, "begin");
}

static inline void endp(const int column)
{
	printa(column, "end");
}

static inline void set_sqlcode(const act* action, const int column)
{
	if (action->act_flags & ACT_sql)
		printa(column, "SQLCODE := interbase.sqlcode(isc_status);");
}

//____________________________________________________________
//  
//		Code generator for ADA.  Not to be confused with
//  
//  

void ADA_action( const act* action, int column)
{

	switch (action->act_type) {
	case ACT_alter_database:
	case ACT_alter_domain:
	case ACT_alter_table:
	case ACT_alter_index:
	case ACT_create_domain:
	case ACT_create_generator:
	case ACT_create_index:
	case ACT_create_shadow:
	case ACT_create_table:
	case ACT_create_view:
	case ACT_declare_filter:
	case ACT_declare_udf:
	case ACT_drop_domain:
	case ACT_drop_filter:
	case ACT_drop_index:
	case ACT_drop_shadow:
	case ACT_drop_table:
	case ACT_drop_udf:
	case ACT_statistics:
	case ACT_drop_view:
		gen_ddl(action, column);
		break;
	case ACT_any:
		gen_any(action, column);
		return;
	case ACT_at_end:
		gen_at_end(action, column);
		return;
	case ACT_commit:
		gen_trans(action, column);
		break;
	case ACT_commit_retain_context:
		gen_trans(action, column);
		break;
	case ACT_b_declare:
		gen_database(action, column);
		gen_routine(action, column);
		return;
	case ACT_basedon:
		gen_based(action, column);
		return;
	case ACT_blob_cancel:
		gen_blob_close(action, column);
		return;
	case ACT_blob_close:
		gen_blob_close(action, column);
		return;
	case ACT_blob_create:
		gen_blob_open(action, column);
		return;
	case ACT_blob_for:
		gen_blob_for(action, column);
		return;
	case ACT_blob_handle:
		gen_segment(action, column);
		return;
	case ACT_blob_open:
		gen_blob_open(action, column);
		return;
	case ACT_clear_handles:
		gen_clear_handles(action, column);
		break;
	case ACT_close:
		gen_s_end(action, column);
		break;
	case ACT_create_database:
		gen_create_database(action, column);
		break;
	case ACT_cursor:
		gen_cursor_init(action, column);
		return;
	case ACT_database:
		gen_database(action, column);
		return;
	case ACT_disconnect:
		gen_finish(action, column);
		break;
	case ACT_drop_database:
		gen_drop_database(action, column);
		break;
	case ACT_dyn_close:
		gen_dyn_close(action, column);
		break;
	case ACT_dyn_cursor:
		gen_dyn_declare(action, column);
		break;
	case ACT_dyn_describe:
		gen_dyn_describe(action, column, false);
		break;
	case ACT_dyn_describe_input:
		gen_dyn_describe(action, column, true);
		break;
	case ACT_dyn_execute:
		gen_dyn_execute(action, column);
		break;
	case ACT_dyn_fetch:
		gen_dyn_fetch(action, column);
		break;
	case ACT_dyn_grant:
		gen_ddl(action, column);
		break;
	case ACT_dyn_immediate:
		gen_dyn_immediate(action, column);
		break;
	case ACT_dyn_insert:
		gen_dyn_insert(action, column);
		break;
	case ACT_dyn_open:
		gen_dyn_open(action, column);
		break;
	case ACT_dyn_prepare:
		gen_dyn_prepare(action, column);
		break;
	case ACT_procedure:
		gen_procedure(action, column);
		break;
	case ACT_dyn_revoke:
		gen_ddl(action, column);
		break;
	case ACT_endblob:
		gen_blob_end(action, column);
		return;
	case ACT_enderror:
		endif(column);
		break;
	case ACT_endfor:
		gen_endfor(action, column);
		break;
	case ACT_endmodify:
		gen_emodify(action, column);
		break;
	case ACT_endstore:
		gen_estore(action, column);
		break;
	case ACT_erase:
		gen_erase(action, column);
		return;
	case ACT_event_init:
		gen_event_init(action, column);
		break;
	case ACT_event_wait:
		gen_event_wait(action, column);
		break;
	case ACT_fetch:
		gen_fetch(action, column);
		break;
	case ACT_finish:
		gen_finish(action, column);
		break;
	case ACT_for:
		gen_for(action, column);
		return;
	case ACT_function:
		gen_function(action, column);
		return;
	case ACT_get_segment:
		gen_get_segment(action, column);
		return;
	case ACT_get_slice:
		gen_slice(action, column);
		return;
	case ACT_hctef:
		endif(column);
		break;
	case ACT_insert:
		gen_s_start(action, column);
		break;
	case ACT_loop:
		gen_loop(action, column);
		break;
	case ACT_open:
		gen_s_start(action, column);
		break;
	case ACT_on_error:
		gen_on_error(action, column);
		return;
	case ACT_ready:
		gen_ready(action, column);
		break;
	case ACT_prepare:
		gen_trans(action, column);
		break;
	case ACT_put_segment:
		gen_put_segment(action, column);
		return;
	case ACT_put_slice:
		gen_slice(action, column);
		return;
	case ACT_release:
		gen_release(action, column);
		break;
	case ACT_rfinish:
		gen_finish(action, column);
		break;
	case ACT_rollback:
		gen_trans(action, column);
		break;
	case ACT_routine:
		gen_routine(action, column);
		return;
	case ACT_s_end:
		gen_s_end(action, column);
		return;
	case ACT_s_fetch:
		gen_s_fetch(action, column);
		return;
	case ACT_s_start:
		gen_s_start(action, column);
		break;
	case ACT_select:
		gen_select(action, column);
		break;
	case ACT_segment_length:
		gen_segment(action, column);
		return;
	case ACT_segment:
		gen_segment(action, column);
		return;
	case ACT_sql_dialect:
		sw_sql_dialect = ((SDT) action->act_object)->sdt_dialect;
		return;
	case ACT_start:
		gen_t_start(action, column);
		break;
	case ACT_store:
		gen_store(action, column);
		return;
	case ACT_store2:
		gen_return_value(action, column);
		return;
	case ACT_type_number:
		gen_type(action, column);
		return;
	case ACT_update:
		gen_update(action, column);
		break;
	case ACT_variable:
		gen_variable(action, column);
		return;
	default:
		return;
	}

//  Put in a trailing brace for those actions still with us 

	if (action->act_flags & ACT_sql)
		gen_whenever(action->act_whenever, column);
}


//____________________________________________________________
//  
//		Print a statment, breaking it into
//       reasonable 120 character hunks.
//  

void ADA_print_buffer( TEXT * output_buffer, int column)
{
	TEXT s[121], *p, *q;
	int i;
	bool multiline = false;

	p = s;

	for (q = output_buffer; *q; q++) {
		*p++ = *q;
		if (((p - s) + column) > 119) {
			for (p--; (*p != ',') && (*p != ' '); p--)
				q--;
			*++p = 0;

			if (multiline) {
				for (i = column / 8; i; --i)
					ib_putc('\t', out_file);

				for (i = column % 8; i; --i)
					ib_putc(' ', out_file);
			}
			ib_fprintf(out_file, "%s\n", s);
			s[0] = 0;
			p = s;
			multiline = true;
		}
	}
	*p = 0;
	if (multiline) {
		for (i = column / 8; i; --i)
			ib_putc('\t', out_file);
		for (i = column % 8; i; --i)
			ib_putc(' ', out_file);
	}
	ib_fprintf(out_file, "%s", s);
	output_buffer[0] = 0;
}


//____________________________________________________________
//  
//		Align output to a specific column for output.
//  

static void align( int column)
{
	int i;

	if (column < 0)
		return;

	ib_putc('\n', out_file);

	for (i = column / 8; i; --i)
		ib_putc('\t', out_file);

	for (i = column % 8; i; --i)
		ib_putc(' ', out_file);
}


//____________________________________________________________
//  
//		Build an assignment from a host language variable to
//		a port variable.
//  

static void asgn_from( const act* action, REF reference, int column)
{
	GPRE_FLD field;
	TEXT *value, name[64], variable[20], temp[20];

	for (; reference; reference = reference->ref_next) {
		field = reference->ref_field;
		if (field->fld_array_info)
			if (!(reference->ref_flags & REF_array_elem)) {
				printa(column, "%s := interbase.null_blob;",
					   gen_name(name, reference, true));
				gen_get_or_put_slice(action, reference, false, column);
				continue;
			}

		if (!reference->ref_source && !reference->ref_value)
			continue;
		gen_name(variable, reference, true);
		if (reference->ref_source)
			value = gen_name(temp, reference->ref_source, true);
		else
			value = reference->ref_value;

		if (!reference->ref_master)
			printa(column, "%s := %s;", variable, value);
		else {
			printa(column, "if %s < 0 then", value);
			printa(column + INDENT, "%s := -1;", variable);
			printa(column, "else");
			printa(column + INDENT, "%s := 0;", variable);
			endif(column);
		}
	}
}

//____________________________________________________________
//  
//		Build an assignment to a host language variable from
//		a port variable.
//  

static void asgn_to( const act* action, REF reference, int column)
{
	GPRE_FLD field;
	REF source;
	TEXT s[20];

	source = reference->ref_friend;
	field = source->ref_field;

	if (field->fld_array_info) {
		source->ref_value = reference->ref_value;
		gen_get_or_put_slice(action, source, true, column);
		return;
	}
//  
//if (field && (field->fld_flags & FLD_text))
//   printa (column, "interbase.isc_ftof (%s, %d, %s, sizeof (%s));", 
// gen_name (s, source, true),
// field->fld_length,
// reference->ref_value,
// reference->ref_value);
//else
//  
	printa(column, "%s := %s;", reference->ref_value,
		   gen_name(s, source, true));

//  Pick up NULL value if one is there 
	if (reference = reference->ref_null)
		printa(column, "%s := %s;", reference->ref_value,
			   gen_name(s, reference, true));
}


//____________________________________________________________
//  
//		Build an assignment to a host language variable from
//		a port variable.
//  

static void asgn_to_proc( REF reference, int column)
{
	SCHAR s[64];

	for (; reference; reference = reference->ref_next) {
		if (!reference->ref_value)
			continue;
		printa(column, "%s := %s;",
			   reference->ref_value, gen_name(s, reference, true));
	}
}


//____________________________________________________________
//  
//       Generate a function call for free standing ANY.  Somebody else
//       will need to generate the actual function.
//  

static void gen_any( const act* action, int column)
{
	GPRE_REQ request;
	POR port;
	REF reference;

	align(column);
	request = action->act_request;

	ib_fprintf(out_file, "%s_r (&%s, &%s",
			   request->req_handle, request->req_handle, request->req_trans);

	if (port = request->req_vport)
		for (reference = port->por_references; reference;
			 reference = reference->ref_next)
				ib_fprintf(out_file, ", %s", reference->ref_value);

	ib_fprintf(out_file, ")");
}


//____________________________________________________________
//  
//       Generate code for AT END clause of FETCH.
//  

static void gen_at_end( const act* action, int column)
{
	GPRE_REQ request;
	SCHAR s[20];

	request = action->act_request;
	printa(column, "if %s = 0 then", gen_name(s, request->req_eof, true));
}


//____________________________________________________________
//  
//		Substitute for a BASED ON <field name> clause.
//  

static void gen_based( const act* action, int column)
{
	BAS based_on;
	GPRE_FLD field;
	SSHORT datatype, i, length;
	DIM dimension;
	SCHAR s[512], s2[128], *p, *q;

	based_on = (BAS) action->act_object;
	field = based_on->bas_field;
	q = s;

	if (field->fld_array_info) {
		datatype = field->fld_array->fld_dtype;
		sprintf(s, "array (");
		for (q = s; *q; q++);

		/*  Print out the dimension part of the declaration  */
		for (dimension = field->fld_array_info->ary_dimension, i = 1;
			 i < field->fld_array_info->ary_dimension_count;
			 dimension = dimension->dim_next, i++) {
			sprintf(s2, "%s range %"SLONGFORMAT"..%"SLONGFORMAT
					",\n                            ",
					LONG_DCL, dimension->dim_lower, dimension->dim_upper);
			for (p = s2; *p; p++, q++)
				*q = *p;
		}

		sprintf(s2, "%s range %"SLONGFORMAT"..%"SLONGFORMAT") of ",
				LONG_DCL, dimension->dim_lower, dimension->dim_upper);
		for (p = s2; *p; p++, q++)
			*q = *p;
	}
	else
		datatype = field->fld_dtype;

	switch (datatype) {
	case dtype_short:
		sprintf(s2, "%s", SHORT_DCL);
		break;

	case dtype_long:
		sprintf(s2, "%s", LONG_DCL);
		break;

	case dtype_date:
	case dtype_blob:
	case dtype_quad:
		sprintf(s2, "interbase.quad");
		break;

	case dtype_text:
		length =
			(field->fld_array_info) ? field->fld_array->fld_length : field->
			fld_length;
		if (field->fld_sub_type == 1) {
			if (length == 1)
				sprintf(s2, "%s", BYTE_DCL);
			else
				sprintf(s2, "%s (1..%d)", BYTE_VECTOR_DCL, length);
		}
		else if (length == 1)
			sprintf(s2, "interbase.isc_character");
		else
			sprintf(s2, "string (1..%d)", length);
		break;

	case dtype_real:
		sprintf(s2, "%s", REAL_DCL);
		break;

	case dtype_double:
		sprintf(s2, "%s", DOUBLE_DCL);
		break;

	default:
		sprintf(s2, "datatype %d unknown\n", field->fld_dtype);
		return ;
	}
	for (p = s2; *p; p++, q++)
		*q = *p;
	if (!strcmp(based_on->bas_terminator, ";"))
		*q++ = ';';
	*q = 0;
	printa(column, s);
}


//____________________________________________________________
//  
//		Make a blob FOR loop.
//  

static void gen_blob_close( const act* action, USHORT column)
{
	TEXT *command;
	BLB blob;

	if (action->act_flags & ACT_sql) {
		column = gen_cursor_close(action, action->act_request, column);
		blob = (BLB) action->act_request->req_blobs;
	}
	else
		blob = (BLB) action->act_object;

	command = (action->act_type == ACT_blob_cancel) ? (TEXT*)"CANCEL" :
                                                                 (TEXT*)"CLOSE";
	printa(column, "interbase.%s_BLOB (%s isc_%d);",
		   command, status_vector(action), blob->blb_ident);

	if (action->act_flags & ACT_sql) {
		endif(column);
		column -= INDENT;
	}

	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		End a blob FOR loop.
//  

static void gen_blob_end( const act* action, USHORT column)
{
	BLB blob;

	blob = (BLB) action->act_object;
	printa(column, "end loop;");

	if (action->act_error)
		printa(column, "interbase.CLOSE_BLOB (isc_status2, isc_%d);",
			   blob->blb_ident);
	else
		printa(column, "interbase.CLOSE_BLOB (%s isc_%d);",
			   status_vector(0), blob->blb_ident);
}


//____________________________________________________________
//  
//		Make a blob FOR loop.
//  

static void gen_blob_for( const act* action, USHORT column)
{

	gen_blob_open(action, column);
	if (action->act_error)
		printa(column, "if (isc_status(1) = 0) then");
	printa(column, "loop");
	gen_get_segment(action, column + INDENT);
	printa(column + INDENT,
		   "exit when (isc_status(1) /= 0 and isc_status(1) /= interbase.isc_segment);");
}


#ifdef NOT_USED_OR_REPLACED
//  This is the V4.0 version of the function prior to 18-January-95.
//  for unknown reasons it contains a core dump.  The V3.3 version of
//  this function appears to work fine, so we are using it instead.
//  Ravi Kumar
//  
//____________________________________________________________
//  
//		Generate the call to open (or create) a blob.
//  

static gen_blob_open( const act* action, USHORT column)
{
	TEXT *command;
	BLB blob;
	PAT args;
	REF reference;
	TEXT s[20];
	TEXT *pattern1 =
		"interbase.%IFCREATE%ELOPEN%EN_BLOB2 (%V1 %RF%DH, %RF%RT, %RF%BH, %RF%FR, %N1, %I1);",
		*pattern2 =
		"interbase.%IFCREATE%ELOPEN%EN_BLOB2 (%V1 %RF%DH, %RF%RT, %RF%BH, %RF%FR, 0, isc_null_bpb);";

	if (action->act_flags & ACT_sql) {
		column = gen_cursor_open(action, action->act_request, column);
		blob = (BLB) action->act_request->req_blobs;
		reference = ((OPN) action->act_object)->opn_using;
		gen_name(s, reference, true);
	}
	else
		blob = (BLB) action->act_object;

	args.pat_condition = (action->act_type == ACT_blob_create);	//  open or create blob
	args.pat_vector1 = status_vector(action);	//  status vector
	args.pat_database = blob->blb_request->req_database;	//  database handle
	args.pat_request = blob->blb_request;	// transaction handle
	args.pat_blob = blob;		// blob handle
	args.pat_reference = reference;	//  blob identifier
	args.pat_ident1 = blob->blb_bpb_ident;

	if ((action->act_flags & ACT_sql) && action->act_type == ACT_blob_open)
		printa(column, "%s := %s;", s, reference->ref_value);

	if (args.pat_value1 = blob->blb_bpb_length)
		PATTERN_expand(column, pattern1, &args);
	else
		PATTERN_expand(column, pattern2, &args);

	if (action->act_flags & ACT_sql) {
		endif(column);
		column -= INDENT;
		endif(column);
		column -= INDENT;
		set_sqlcode(action, column);
		if (action->act_type == ACT_blob_create) {
			printa(column, "if SQLCODE = 0 then");
			printa(column + INDENT, "%s := %s;", reference->ref_value, s);
			endif(column);
		}
		endif(column);
	}
}
#else
//  This is the version 3.3 of this routine - the original V4.0 version
//  has a core drop.
//  Ravi Kumar
//  
//____________________________________________________________
//  
//       Generate the call to open (or create) a blob.
//  

static void gen_blob_open( const act* action, USHORT column)
{
	BLB blob;
	PAT args;
	TEXT *pattern1 =
		"interbase.%IFCREATE%ELOPEN%EN_BLOB2 (%V1 %RF%DH, %RF%RT, %RF%BH, %RF%FR, %N1, %I1);";
	TEXT *pattern2 =
		"interbase.%IFCREATE%ELOPEN%EN_BLOB2 (%V1 %RF%DH, %RF%RT, %RF%BH, %RF%FR, 0, isc_null_bpb);";

	blob = (BLB) action->act_object;
	args.pat_condition = (action->act_type == ACT_blob_create);	// open or create blob
	args.pat_vector1 = status_vector(action);	// status vector
	args.pat_database = blob->blb_request->req_database;	// database handle
	args.pat_request = blob->blb_request;	// transaction handle
	args.pat_blob = blob;		// blob handle
	args.pat_reference = blob->blb_reference;	// blob identifier
	args.pat_ident1 = blob->blb_bpb_ident;

	if (args.pat_value1 = blob->blb_bpb_length)
		PATTERN_expand(column, pattern1, &args);
	else
		PATTERN_expand(column, pattern2, &args);
}
#endif


//____________________________________________________________
//  
//		Callback routine for BLR pretty printer.
//  

static void gen_blr(void* user_arg, SSHORT offset, const char* string)
{
	int from, to, len, c_len;

	c_len = strlen(COMMENT);
	len = strlen(string);
	from = 0;
	to = 120 - c_len;

	while (from < len) {
		if (to < len) {
			char buffer[121];
			strncpy(buffer, string + from, 120 - c_len);
			buffer[120 - c_len] = 0;
			ib_fprintf(out_file, "%s%s\n", COMMENT, buffer);
		} else
			ib_fprintf(out_file, "%s%s\n", COMMENT, string + from);
		from = to;
		to = to + 120 - c_len;
	}
}


//____________________________________________________________
//  
//       Zap all know handles.
//  

static void gen_clear_handles( const act* action, int column)
{
	GPRE_REQ request;

	for (request = requests; request; request = request->req_next) {
		if (!(request->req_flags & REQ_exp_hand))
			printa(column, "%s = 0;", request->req_handle);
	}
}


#ifdef NOT_USED_OR_REPLACED
//____________________________________________________________
//  
//       Generate a symbol to ease compatibility with V3.
//  

static void gen_compatibility_symbol(
									 TEXT * symbol,
									 TEXT * v4_prefix, TEXT * trailer)
{
	TEXT *v3_prefix;

	v3_prefix = (isLangCpp(sw_language)) ? "gds_" : "gds__";

	ib_fprintf(out_file, "#define %s%s\t%s%s%s\n", v3_prefix, symbol,
			   v4_prefix, symbol, trailer);
}
#endif


//____________________________________________________________
//  
//		Generate text to compile a request.
//  

static void gen_compile( const act* action, int column)
{
	GPRE_REQ request;
	DBB db;
	SYM symbol;
	BLB blob;

	request = action->act_request;
	column += INDENT;
	db = request->req_database;
	symbol = db->dbb_name;

	if (sw_auto)
		t_start_auto(action, request, status_vector(action), column, true);

	if ((action->act_error || (action->act_flags & ACT_sql)) && sw_auto)
		printa(column, "if (%s = 0) and (%s%s /= 0) then", request->req_handle,
				ada_package, request_trans(action, request));
	else
		printa(column, "if %s = 0 then", request->req_handle);

	column += INDENT;
#ifdef NOT_USED_OR_REPLACED
	printa(column, "interbase.COMPILE_REQUEST%s (%s %s%s, %s%s, %d, isc_%d);",
		   (request->req_flags & REQ_exp_hand) ? "" : "2",
		   status_vector(action),
		   ada_package, symbol->sym_string,
		   request_trans(action, request),
		   (request->req_flags & REQ_exp_hand) ? "" : "'address",
		   request->req_length, request->req_ident);
#endif
	printa(column, "interbase.COMPILE_REQUEST%s (%s %s%s, %s%s, %d, isc_%d);",
		   (request->req_flags & REQ_exp_hand) ? "" : "2",
		   status_vector(action),
		   ada_package, symbol->sym_string,
		   request->req_handle,
		   (request->req_flags & REQ_exp_hand) ? "" : "'address",
		   request->req_length, request->req_ident);

	set_sqlcode(action, column);
	column -= INDENT;
	endif(column);


	if (blob = request->req_blobs)
		for (; blob; blob = blob->blb_next)
			printa(column - INDENT, "isc_%d := 0;", blob->blb_ident);
}


//____________________________________________________________
//  
//		Generate a call to create a database.
//  

static void gen_create_database( const act* action, int column)
{
	GPRE_REQ request;
	DBB db;
	bool save_sw_auto;
	TEXT s1[32], s2[32];

	request = ((MDBB) action->act_object)->mdbb_dpb_request;
	db = (DBB) request->req_database;

	sprintf(s1, "isc_%dl", request->req_ident);

	if (request->req_flags & REQ_extend_dpb) {
		sprintf(s2, "isc_%dp", request->req_ident);
		if (request->req_length)
			printa(column, "%s = isc_to_dpb_ptr (isc_%d'address);", s2,
				   request->req_ident);
		if (db->dbb_r_user)
			printa(column,
				   "interbase.MODIFY_DPB (%s, %s, interbase.isc_dpb_user_name, %s, %d);\n",
				   s2, s1, db->dbb_r_user, (strlen(db->dbb_r_user) - 2));
		if (db->dbb_r_password)
			printa(column,
				   "interbase.MODIFY_DPB (%s, %s, interbase.isc_dpb_password, %s, %d);\n",
				   s2, s1, db->dbb_r_password,
				   (strlen(db->dbb_r_password) - 2));
		/*
		   ** ===========================================================
		   ** ==
		   ** ==   Process Role Name
		   ** ==
		   ** ===========================================================
		 */
		if (db->dbb_r_sql_role)
			printa(column,
				   "interbase.MODIFY_DPB (%s, %s, interbase.isc_dpb_sql_role_name, %s, %d);\n",
				   s2, s1, db->dbb_r_sql_role,
				   (strlen(db->dbb_r_sql_role) - 2));

		if (db->dbb_r_lc_messages)
			printa(column,
				   "interbase.MODIFY_DPB (%s, %s, interbase.isc_dpb_lc_messages, %s, %d);\n",
				   s2, s1, db->dbb_r_lc_messages,
				   (strlen(db->dbb_r_lc_messages) - 2));
		if (db->dbb_r_lc_ctype)
			printa(column,
				   "interbase.MODIFY_DPB (%s, %s, interbase.isc_dpb_lc_ctype, %s, %d);\n",
				   s2, s1, db->dbb_r_lc_ctype,
				   (strlen(db->dbb_r_lc_ctype) - 2));

	}
	else
		sprintf(s2, "isc_to_dpb_ptr (isc_%d'address)", request->req_ident);

	if (request->req_length)
		printa(column,
			   "interbase.CREATE_DATABASE (%s %d, \"%s\", %s%s, %s, %s, 0);",
			   status_vector(action),
			   strlen(db->dbb_filename),
			   db->dbb_filename,
			   ada_package, db->dbb_name->sym_string, s1, s2);
	else
		printa(column,
			   "interbase.CREATE_DATABASE (%s %d, \"%s\", %s%s, 0, interbase.null_dpb, 0);",
			   status_vector(action),
			   strlen(db->dbb_filename),
			   db->dbb_filename, ada_package, db->dbb_name->sym_string);

	if (request && request->req_flags & REQ_extend_dpb) {
		if (request->req_length)
			printa(column, "if (%s != isc_%d)", s2, request->req_ident);
		printa(column + (request->req_length ? 4 : 0),
			   "interbase.FREE (%s);", s2);

		/* reset the length of the dpb */
		if (request->req_length)
			printa(column, "%s = %d;", s1, request->req_length);
	}

	printa(column, "if %sisc_status(1) = 0 then", ada_package);
	column += INDENT;
	save_sw_auto = sw_auto;
	sw_auto = true;
	gen_ddl(action, column);
	sw_auto = save_sw_auto;
	column -= INDENT;
	printa(column, "else");
	column += INDENT;
	set_sqlcode(action, column);
	column -= INDENT;
	endif(column);
}


//____________________________________________________________
//  
//		Generate substitution text for END_STREAM.
//  

static int gen_cursor_close( const act* action, GPRE_REQ request, int column)
{

	return column;
}


//____________________________________________________________
//  
//		Generate text to initialize a cursor.
//  

static void gen_cursor_init( const act* action, int column)
{

//  If blobs are present, zero out all of the blob handles.  After this
//int, the handles are the user's responsibility 

	if (action->act_request->
		req_flags & (REQ_sql_blob_open | REQ_sql_blob_create)) printa(column,
																	  "gds_%d := 0;",
																	  action->
																	  act_request->
																	  req_blobs->
																	  blb_ident);
}

#ifdef NOT_USED_OR_REPLACED
//____________________________________________________________
//  
//		Generate text to open an embedded SQL cursor.
//  

static int gen_cursor_open( const act* action, GPRE_REQ request, int column)
{

	return column;
}
#endif

//____________________________________________________________
//  
//		Generate insertion text for the database statement,
//		including the definitions of all requests, and blob
//		and port declarations for requests in the main routine.
//  

static void gen_database( const act* action, int column)
{
	DBB db;
	GPRE_REQ request;
	POR port;
	BLB blob;
	USHORT count;
	SSHORT blob_subtype;
	USHORT event_count, max_count;
	LLS stack_ptr;
	TPB tpb_val;
	REF reference;
	GPRE_FLD field;
	bool array_flag = false;

	if (first_flag++ != 0)
		return;

	ib_fprintf(out_file, "\n----- GPRE Preprocessor Definitions -----\n");

	for (request = requests; request; request = request->req_next) {
		if (request->req_flags & REQ_local)
			continue;
		for (port = request->req_ports; port; port = port->por_next)
			make_port(port, column);
	}
	ib_fprintf(out_file, "\n");
	for (request = requests; request; request = request->req_routine) {
		if (request->req_flags & REQ_local)
			continue;
		for (port = request->req_ports; port; port = port->por_next)
			printa(column, "isc_%d\t: isc_%dt;\t\t\t-- message --",
				   port->por_ident, port->por_ident);

		for (blob = request->req_blobs; blob; blob = blob->blb_next) {
			printa(column,
				   "isc_%d\t: interbase.blob_handle;\t-- blob handle --",
				   blob->blb_ident);
			if (!(blob_subtype = blob->blb_const_to_type))
				if (reference = blob->blb_reference)
					if (field = reference->ref_field)
						blob_subtype = field->fld_sub_type;
			if (blob_subtype != 0 && blob_subtype != 1)
				printa(column,
					   "isc_%d\t: %s (1 .. %d);\t\t-- blob segment --",
					   blob->blb_buff_ident, BYTE_VECTOR_DCL,
					   blob->blb_seg_length);
			else
				printa(column,
					   "isc_%d\t: string (1 .. %d);\t\t-- blob segment --",
					   blob->blb_buff_ident, blob->blb_seg_length);
			printa(column, "isc_%d\t: %s;\t\t-- segment length --",
				   blob->blb_len_ident, USHORT_DCL);
		}
	}

//  generate event parameter block for each event in module 

	max_count = 0;
	for (stack_ptr = events; stack_ptr; stack_ptr = stack_ptr->lls_next) {
		event_count = gen_event_block((const act*) stack_ptr->lls_object);
		max_count = MAX(event_count, max_count);
	}

	if (max_count)
		printa(column, "isc_events: %s(1..%d);\t-- event vector --",
			   EVENT_LIST_DCL, max_count);

	for (request = requests; request; request = request->req_next) {
		gen_request(request, column);

		/*  Array declarations  */
		if (port = request->req_primary)
			for (reference = port->por_references; reference;
				 reference = reference->ref_next)
			{
				if (reference->ref_flags & REF_fetch_array) {
					make_array_declaration(reference, column);
					array_flag = true;
				}
			}
	}
	if (array_flag) {
		printa(column, "isc_array_length\t: %s;\t-- slice return value --",
			   LONG_DCL);
		printa(column,
			   "isc_null_vector_l\t: %s (1..1);\t-- null long vector --",
			   LONG_VECTOR_DCL);
	}

	printa(column,
		   "isc_null_bpb\t: interbase.blr (1..1);\t-- null blob parameter block --");

	if (!ada_package[0])
		printa(column,
			   "gds_trans\t: interbase.transaction_handle := 0;\t-- default transaction --");
	printa(column,
		   "isc_status\t: interbase.status_vector;\t-- status vector --");
	printa(column,
		   "isc_status2\t: interbase.status_vector;\t-- status vector --");
	printa(column, "SQLCODE\t: %s := 0;\t\t\t-- SQL status code --",
		   LONG_DCL);
	if (!ada_package[0])
		for (db = isc_databases; db; db = db->dbb_next)
			printa(column,
				   "%s\t\t: interbase.database_handle%s;-- database handle --",
				   db->dbb_name->sym_string,
				   (db->dbb_scope == DBB_EXTERN) ? "" : " := 0");

	printa(column, " ");

	count = 0;
	for (db = isc_databases; db; db = db->dbb_next) {
		count++;
		for (tpb_val = db->dbb_tpbs; tpb_val; tpb_val = tpb_val->tpb_dbb_next)
			gen_tpb(tpb_val, column + INDENT);
	}

	printa(column,
		   "isc_teb\t: array (1..%d) of interbase.isc_teb_t;\t-- transaction vector ",
		   count);

	printa(column, "----- end of GPRE definitions ");
}


//____________________________________________________________
//  
//		Generate a call to update metadata.
//  

static void gen_ddl( const act* action, int column)
{
	GPRE_REQ request;

//  Set up command type for call to RDB$DDL 

	request = action->act_request;

//  Generate a call to RDB$DDL to perform action 

	if (sw_auto) {
		t_start_auto(action, 0, status_vector(action), column, true);
		printa(column, "if %sgds_trans /= 0 then", ada_package);
		column += INDENT;
	}

	printa(column, "interbase.DDL (%s %s%s, %s%s, %d, isc_%d'address);",
		   status_vector(action),
		   ada_package, request->req_database->dbb_name->sym_string,
		   ada_package, request->req_trans,
		   request->req_length, request->req_ident);

	if (sw_auto) {
		printa(column, "if %sisc_status(1) = 0 then", ada_package);
		printa(column + INDENT,
			   "interbase.commit_TRANSACTION (%s %sgds_trans);",
			   status_vector(action), ada_package);
		printa(column, "else", ada_package);
		printa(column + INDENT,
			   "interbase.rollback_TRANSACTION (%sgds_trans);", ada_package);
		endif(column);
		column -= INDENT;
		endif(column);
	}

	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate a call to create a database.
//  

static void gen_drop_database( const act* action, int column)
{
	GPRE_REQ request;
	DBB db;

	db = (DBB) action->act_object;
	request = action->act_request;

	printa(column,
		   "interbase.DROP_DATABASE (%s %d, \"%s\", RDBK_DB_TYPE_GDS);",
		   status_vector(action), strlen(db->dbb_filename), db->dbb_filename);

	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_close( const act* action, int column)
{
	DYN statement;
	TEXT s[64];

	statement = (DYN) action->act_object;
	printa(column,
		   "interbase.embed_dsql_close (%s %s);",
		   status_vector(action), make_name(s, statement->dyn_cursor_name));
	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_declare( const act* action, int column)
{
	DYN statement;
	TEXT s1[64], s2[64];

	statement = (DYN) action->act_object;
	printa(column,
		   "interbase.embed_dsql_declare (%s %s, %s);",
		   status_vector(action),
		   make_name(s1, statement->dyn_statement_name),
		   make_name(s2, statement->dyn_cursor_name));
	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_describe(const act* action,
							 int column,
							 bool input_flag)
{
	DYN statement;
	TEXT s[64];

	statement = (DYN) action->act_object;
	printa(column,
		   "interbase.embed_dsql_describe%s (%s %s, %d, %s'address);",
		   input_flag ? "_bind" : "",
		   status_vector(action),
		   make_name(s, statement->dyn_statement_name),
		   sw_sql_dialect, statement->dyn_sqlda);
	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_execute( const act* action, int column)
{
	TEXT s[64];
	gpre_req* request;
	gpre_req req_const;

	DYN statement = (DYN) action->act_object;
	const TEXT* transaction;
	if (statement->dyn_trans) {
		transaction = statement->dyn_trans;
		request = &req_const;
		request->req_trans = transaction;
	}
	else {
		transaction = "gds_trans";
		request = NULL;
	}

	if (sw_auto) {
		t_start_auto(action, request, status_vector(action), column, true);
		printa(column, "if %s%s /= 0 then", ada_package, transaction);
		column += INDENT;
	}

	if (statement->dyn_sqlda2)
		printa(column,
			   "interbase.embed_dsql_execute2 (isc_status, %s%s, %s, %d, %s'address);",
			   ada_package, transaction,
			   make_name(s, statement->dyn_statement_name),
			   sw_sql_dialect,
			   (statement->dyn_sqlda) ? ada_null_address : statement->
			   dyn_sqlda, statement->dyn_sqlda2);
	else if (statement->dyn_sqlda)
		printa(column,
			   "interbase.embed_dsql_execute (isc_status, %s%s, %s, %d, %s'address);",
			   ada_package, transaction, make_name(s,
												   statement->
												   dyn_statement_name),
			   sw_sql_dialect, statement->dyn_sqlda);
	else
		printa(column,
			   "interbase.embed_dsql_execute (isc_status, %s%s, %s, %d);",
			   ada_package, transaction, make_name(s,
												   statement->
												   dyn_statement_name),
			   sw_sql_dialect);

	if (sw_auto) {
		column -= INDENT;
		endif(column);
	}

	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_fetch( const act* action, int column)
{
	DYN statement;
	TEXT s[64];

	statement = (DYN) action->act_object;
	if (statement->dyn_sqlda)
		printa(column,
			   "interbase.embed_dsql_fetch (isc_status, SQLCODE, %s, %d, %s'address);",
			   make_name(s, statement->dyn_cursor_name),
			   sw_sql_dialect, statement->dyn_sqlda);
	else
		printa(column,
			   "interbase.embed_dsql_fetch (isc_status, SQLCODE, %s, %d);",
			   make_name(s, statement->dyn_cursor_name), sw_sql_dialect);

	printa(column, "if SQLCODE /= 100 then");
	printa(column + INDENT, "SQLCODE := interbase.sqlcode (isc_status);");
	endif(column);
}


//____________________________________________________________
//  
//		Generate code for an EXECUTE IMMEDIATE dynamic SQL statement.
//  

static void gen_dyn_immediate( const act* action, int column)
{
	DBB database;
	gpre_req* request;
	gpre_req req_const;

	DYN statement = (DYN) action->act_object;
	const TEXT* transaction;
	if (statement->dyn_trans) {
		transaction = statement->dyn_trans;
		request = &req_const;
		request->req_trans = transaction;
	}
	else {
		transaction = "gds_trans";
		request = NULL;
	}

	database = statement->dyn_database;

	if (sw_auto) {
		t_start_auto(action, request, status_vector(action), column, true);
		printa(column, "if %s%s /= 0 then", ada_package, transaction);
		column += INDENT;
	}

	printa(column,
		   "interbase.embed_dsql_execute_immed (isc_status, %s%s, %s%s, %s'length(1), %s, %d);",
		   ada_package, database->dbb_name->sym_string,
		   ada_package, transaction,
		   statement->dyn_string, statement->dyn_string, sw_sql_dialect);

	if (sw_auto) {
		column -= INDENT;
		endif(column);
	}

	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_insert( const act* action, int column)
{
	DYN statement;
	TEXT s[64];

	statement = (DYN) action->act_object;
	if (statement->dyn_sqlda)
		printa(column,
			   "interbase.embed_dsql_insert (isc_status, %s, %d, %s'address);",
			   make_name(s, statement->dyn_cursor_name),
			   sw_sql_dialect, statement->dyn_sqlda);
	else
		printa(column,
			   "interbase.embed_dsql_insert (isc_status, %s, %d);",
			   make_name(s, statement->dyn_cursor_name), sw_sql_dialect);

	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_open( const act* action, int column)
{
	TEXT s[64];
	gpre_req* request;
	gpre_req req_const;

	DYN statement = (DYN) action->act_object;
	const TEXT* transaction;
	if (statement->dyn_trans) {
		transaction = statement->dyn_trans;
		request = &req_const;
		request->req_trans = transaction;
	}
	else {
		transaction = "gds_trans";
		request = NULL;
	}

	make_name(s, statement->dyn_cursor_name);

	if (sw_auto) {
		t_start_auto(action, request, status_vector(action), column, true);
		printa(column, "if %s%s /= 0 then", ada_package, transaction);
		column += INDENT;
	}

	if (statement->dyn_sqlda)
		printa(column,
			   "interbase.embed_dsql_open (isc_status, %s%s, %s, %d, %s'address);",
			   ada_package, transaction,
			   s, sw_sql_dialect, statement->dyn_sqlda);
	else
		printa(column,
			   "interbase.embed_dsql_open (isc_status, %s%s, %s, %d);",
			   ada_package, transaction, s, sw_sql_dialect);

	if (sw_auto) {
		column -= INDENT;
		endif(column);
	}

	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate a dynamic SQL statement.
//  

static void gen_dyn_prepare( const act* action, int column)
{
	TEXT s[64];
	gpre_req* request;
	gpre_req req_const;

	DYN statement = (DYN) action->act_object;
	DBB database = statement->dyn_database;

	const TEXT* transaction;
	if (statement->dyn_trans) {
		transaction = statement->dyn_trans;
		request = &req_const;
		request->req_trans = transaction;
	}
	else {
		transaction = "gds_trans";
		request = NULL;
	}

	if (sw_auto) {
		t_start_auto(action, request, status_vector(action), column, true);
		printa(column, "if %s%s /= 0 then", ada_package, transaction);
		column += INDENT;
	}

	if (statement->dyn_sqlda)
		printa(column,
			   "interbase.embed_dsql_prepare (isc_status, %s%s, %s%s, %s, %s'length(1), %s, %d, %s'address);",
			   ada_package, database->dbb_name->sym_string, ada_package,
			   transaction, make_name(s, statement->dyn_statement_name),
			   statement->dyn_string, statement->dyn_string, sw_sql_dialect,
			   statement->dyn_sqlda);
	else
		printa(column,
			   "interbase.embed_dsql_prepare (isc_status, %s%s, %s%s, %s, %s'length(1), %s, %d);",
			   ada_package, database->dbb_name->sym_string, ada_package,
			   transaction, make_name(s, statement->dyn_statement_name),
			   statement->dyn_string, statement->dyn_string, sw_sql_dialect);

	if (sw_auto) {
		column -= INDENT;
		endif(column);
	}

	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate substitution text for END_MODIFY.
//  

static void gen_emodify( const act* action, int column)
{
	UPD modify;
	REF reference, source;
	GPRE_FLD field;
	TEXT s1[20], s2[20];

	modify = (UPD) action->act_object;

	for (reference = modify->upd_port->por_references; reference;
		 reference = reference->ref_next) {
		if (!(source = reference->ref_source))
			continue;
		field = reference->ref_field;
		printa(column, "%s := %s;",
			   gen_name(s1, reference, true), gen_name(s2, source, true));
		if (field->fld_array_info)
			gen_get_or_put_slice(action, reference, false, column);
	}

	gen_send(action, modify->upd_port, column);
}


//____________________________________________________________
//  
//		Generate substitution text for END_STORE.
//  

static void gen_estore( const act* action, int column)
{
	GPRE_REQ request;

	request = action->act_request;

//  if we did a store ... returning_values aka store2
//  just wrap up any dangling error handling 
	if (request->req_type == REQ_store2) {
		if (action->act_error)
			endif(column);
		return;
	}

	gen_start(action, request->req_primary, column);
	if (action->act_error)
		endif(column);
}


//____________________________________________________________
//  
//		Generate definitions associated with a single request.
//  

static void gen_endfor( const act* action, int column)
{
	GPRE_REQ request;

	request = action->act_request;

	if (request->req_sync)
		gen_send(action, request->req_sync, column + INDENT);

	printa(column, "end loop;");

	if (action->act_error || (action->act_flags & ACT_sql))
		endif(column);
}


//____________________________________________________________
//  
//		Generate substitution text for ERASE.
//  

static void gen_erase( const act* action, int column)
{
	UPD erase;

	erase = (UPD) action->act_object;
	gen_send(action, erase->upd_port, column);
}


//____________________________________________________________
//  
//		Generate event parameter blocks for use
//		with a particular call to isc_event_wait.
//		For languages too dim to deal with variable
//		arg lists, set up a vector for the event names.
//  

static SSHORT gen_event_block( const act* action)
{
	GPRE_NOD init, list;
	SYM event_name;
	int ident;

	init = (GPRE_NOD) action->act_object;
	event_name = (SYM) init->nod_arg[0];

	ident = CMP_next_ident();
	init->nod_arg[2] = (GPRE_NOD) ident;
	list = init->nod_arg[1];

	printa(0, "isc_%da\t\t: system.address;\t\t-- event parameter block --\n",
		   ident);
	printa(0,
		   "isc_%db\t\t: system.address;\t\t-- result parameter block --\n",
		   ident);
	printa(0, "isc_%dc\t\t: %s;\t\t-- count of events --\n", ident,
		   USHORT_DCL);
	printa(0, "isc_%dl\t\t: %s;\t\t-- length of event parameter block --\n",
		   ident, USHORT_DCL);
	printa(0,
		   "isc_%dv\t\t: interbase.isc_el_t (1..%d);\t\t-- vector for initialization --\n",
		   ident, list->nod_count);

	return list->nod_count;
}


//____________________________________________________________
//  
//		Generate substitution text for EVENT_INIT.
//  

static void gen_event_init( const act* action, int column)
{
	GPRE_NOD init, event_list, *ptr, *end, node;
	REF reference;
	PAT args;
	TEXT variable[32];
	USHORT count;
	TEXT
		* pattern1 =
		"isc_%N1l := interbase.event_block (%RFisc_%N1a'address, %RFisc_%N1b'address, isc_%N1c, %RFisc_%N1v);",
		*pattern2 =
		"interbase.event_wait (%V1 %RF%DH, isc_%N1l, isc_%N1a, isc_%N1b);",
		*pattern3 =
		"interbase.event_counts (isc_events, isc_%N1l, isc_%N1a, isc_%N1b);";

	if (action->act_error)
		begin(column);
	begin(column);

	init = (GPRE_NOD) action->act_object;
	event_list = init->nod_arg[1];

	args.pat_database = (DBB) init->nod_arg[3];
	args.pat_vector1 = status_vector(action);
	args.pat_value1 = (int) init->nod_arg[2];

//  generate call to dynamically generate event blocks 

	for (ptr = event_list->nod_arg, count = 0, end =
		 ptr + event_list->nod_count; ptr < end; ptr++) {
		count++;
		node = *ptr;
		if (node->nod_type == nod_field) {
			reference = (REF) node->nod_arg[0];
			gen_name(variable, reference, true);
			printa(column, "isc_%dv(%d) := %s'address;",
				   args.pat_value1, count, variable);
		}
		else
			printa(column, "isc_%dv(%d) := %s'address;",
				   args.pat_value1, count, node->nod_arg[0]);
	}

	printa(column, "isc_%dc := %d;", args.pat_value1,
		   (int) event_list->nod_count);
	PATTERN_expand(column, pattern1, &args);

//  generate actual call to event_wait 

	PATTERN_expand(column, pattern2, &args);

//  get change in event counts, copying event parameter block for reuse 

	PATTERN_expand(column, pattern3, &args);

	endp(column);
	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate substitution text for EVENT_WAIT.
//  

static void gen_event_wait( const act* action, int column)
{
	PAT args;
	GPRE_NOD event_init;
	SYM event_name, stack_name;
	DBB database;
	LLS stack_ptr;
	const act* event_action;
	int ident;
	TEXT s[64];
	TEXT
		* pattern1 =
		"interbase.event_wait (%V1 %RF%DH, isc_%N1l, isc_%N1a, isc_%N1b);",
		*pattern2 =
		"interbase.event_counts (isc_events, isc_%N1l, isc_%N1a, isc_%N1b);";

	if (action->act_error)
		begin(column);
	begin(column);

	event_name = (SYM) action->act_object;

//  go through the stack of events, checking to see if the
//  event has been initialized and getting the event identifier 

	ident = -1;
	for (stack_ptr = events; stack_ptr; stack_ptr = stack_ptr->lls_next) {
		event_action = (const act*) stack_ptr->lls_object;
		event_init = (GPRE_NOD) event_action->act_object;
		stack_name = (SYM) event_init->nod_arg[0];
		if (!strcmp(event_name->sym_string, stack_name->sym_string)) {
			ident = (int) event_init->nod_arg[2];
			database = (DBB) event_init->nod_arg[3];
		}
	}

	if (ident < 0) {
		sprintf(s, "event handle \"%s\" not found", event_name->sym_string);
		CPR_error(s);
		return;
	}

	args.pat_database = database;
	args.pat_vector1 = status_vector(action);
	args.pat_value1 = (int) ident;

//  generate calls to wait on the event and to fill out the events array 

	PATTERN_expand(column, pattern1, &args);
	PATTERN_expand(column, pattern2, &args);

	endp(column);
	set_sqlcode(action, column);
}



//____________________________________________________________
//  
//		Generate replacement text for the SQL FETCH statement.  The
//		epilog FETCH statement is handled by GEN_S_FETCH (generate
//		stream fetch).
//  

static void gen_fetch( const act* action, int column)
{
	GPRE_REQ request;
	GPRE_NOD var_list;
	int i;
	SCHAR s[20];
#ifdef SCROLLABLE_CURSORS
	POR port;
	REF reference;
	VAL value;
	SCHAR *direction, *offset;
#endif

	request = action->act_request;

#ifdef SCROLLABLE_CURSORS
	if (port = request->req_aport) {
		/* set up the reference to point to the correct value 
		   in the linked list of values, and prepare for the 
		   next FETCH statement if applicable */

		for (reference = port->por_references; reference;
			 reference = reference->ref_next) {
			value = reference->ref_values;
			reference->ref_value = value->val_value;
			reference->ref_values = value->val_next;
		}

		/* find the direction and offset parameters */

		reference = port->por_references;
		offset = reference->ref_value;
		reference = reference->ref_next;
		direction = reference->ref_value;

		/* the direction in which the engine will scroll is sticky, so check to see 
		   the last direction passed to the engine; if the direction is the same and 
		   the offset is 1, then there is no need to pass the message; this prevents 
		   extra packets and allows for batch fetches in either direction */

		printa(column, "if isc_%ddirection MOD 2 /= %s or %s /= 1 then",
			   request->req_ident, direction, offset);
		column += INDENT;

		/* assign the direction and offset parameters to the appropriate message, 
		   then send the message to the engine */

		asgn_from(action, port->por_references, column);
		gen_send(action, port, column);
		printa(column, "isc_%ddirection := %s;", request->req_ident,
			   direction);
		column -= INDENT;
		endif(column);

		printa(column, "if SQLCODE = 0 then");
		column += INDENT;
	}
#endif

	if (request->req_sync) {
		gen_send(action, request->req_sync, column);
		printa(column, "if SQLCODE = 0 then");
		column += INDENT;
	}

	gen_receive(action, column, request->req_primary);
	printa(column, "if SQLCODE = 0 then");
	column += INDENT;
	printa(column, "if %s /= 0 then", gen_name(s, request->req_eof, true));
	column += INDENT;

	if (var_list = (GPRE_NOD) action->act_object) {
		for (i = 0; i < var_list->nod_count; i++)
			asgn_to(action, (REF) var_list->nod_arg[i], column);
	}
	else {						/*  No WITH clause on the FETCH, so no assignments generated;  fix
								   for bug#940.  mao 6/20/89  */
		printa(column, "NULL;");
	}

	column -= INDENT;
	printa(column, "else");
	printa(column + INDENT, "SQLCODE := 100;");
	endif(column);
	column -= INDENT;
	endif(column);
	column -= INDENT;

	if (request->req_sync)
		endif(column);

#ifdef SCROLLABLE_CURSORS
	if (port) {
		column -= INDENT;
		endif(column);
	}
#endif
}


//____________________________________________________________
//  
//		Generate substitution text for FINISH.
//  

static void gen_finish( const act* action, int column)
{
	DBB db;
	GPRE_REQ request;
	RDY ready;

	db = NULL;

	if (sw_auto || ((action->act_flags & ACT_sql) &&
					(action->act_type != ACT_disconnect))) {
		printa(column, "if %sgds_trans /= 0 then", ada_package);
		printa(column + INDENT, "interbase.%s_TRANSACTION (%s %sgds_trans);",
			   (action->act_type != ACT_rfinish) ? "COMMIT" : "ROLLBACK",
			   status_vector(action), ada_package);
		endif(column);
	}

//  the user supplied one or more db_handles 

	for (ready = (RDY) action->act_object; ready; ready = ready->rdy_next) {
		db = ready->rdy_database;
		if (action->act_error || (action->act_flags & ACT_sql))
			printa(column, "if %s%s /= 0 then", ada_package,
				   db->dbb_name->sym_string);
		printa(column, "interbase.DETACH_DATABASE (%s %s%s);",
			   status_vector(action), ada_package, db->dbb_name->sym_string);
		if (action->act_error || (action->act_flags & ACT_sql))
			endif(column);
	}

//  no hanbdles, so we finish all known databases 

	if (!db) {
		if (action->act_error || (action->act_flags & ACT_sql))
			printa(column, "if %sgds_trans = 0 then", ada_package);
		for (db = isc_databases; db; db = db->dbb_next) {
			if ((action->act_error || (action->act_flags & ACT_sql))
				&& (db != isc_databases)) printa(column,
												 "if %s%s /= 0 and isc_status(1) = 0 then",
												 ada_package,
												 db->dbb_name->sym_string);
			else
				printa(column, "if %s%s /= 0 then", ada_package,
					   db->dbb_name->sym_string);
			column += INDENT;
			printa(column, "interbase.DETACH_DATABASE (%s %s%s);",
				   status_vector(action),
				   ada_package, db->dbb_name->sym_string);
			for (request = requests; request; request = request->req_next)
				if (!(request->req_flags & REQ_exp_hand) &&
					request->req_type != REQ_slice &&
					request->req_type != REQ_procedure &&
					request->req_database == db)
					printa(column, "%s := 0;", request->req_handle);
			column -= INDENT;
			endif(column);
		}
		if (action->act_error || (action->act_flags & ACT_sql))
			endif(column);
	}

	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate substitution text for FOR statement.
//  

static void gen_for( const act* action, int column)
{
	POR port;
	GPRE_REQ request;
	TEXT s[20];
	REF reference;

	gen_s_start(action, column);
	request = action->act_request;

	if (action->act_error || (action->act_flags & ACT_sql))
		printa(column, "if isc_status(1) = 0 then");

	printa(column, "loop");
	column += INDENT;
	gen_receive(action, column, request->req_primary);

	if (action->act_error || (action->act_flags & ACT_sql))
		printa(column, "exit when (%s = 0) or (isc_status(1) /= 0);",
			   gen_name(s, request->req_eof, true));
	else
		printa(column, "exit when %s = 0;",
			   gen_name(s, request->req_eof, true));

	if (port = action->act_request->req_primary)
		for (reference = port->por_references; reference;
			 reference = reference->ref_next)
		{
			if (reference->ref_field->fld_array_info)
				gen_get_or_put_slice(action, reference, true, column);
		}
}


//____________________________________________________________
//  
//       Generate a function for free standing ANY or statistical.
//  

static void gen_function( const act* function, int column)
{
	GPRE_REQ request;
	POR port;
	REF reference;
	GPRE_FLD field;
	const act* action;
	TEXT *dtype, s[64];

	action = (const act*) function->act_object;

	if (action->act_type != ACT_any) {
		CPR_error("can't generate function");
		return;
	}

	request = action->act_request;

	ib_fprintf(out_file, "static %s_r (request, transaction", request->req_handle);

	if (port = request->req_vport)
		for (reference = port->por_references; reference;
			 reference = reference->ref_next)
				ib_fprintf(out_file, ", %s",
						   gen_name(s, reference->ref_source, true));
	ib_fprintf(out_file,
			   ")\n    isc_req_handle\trequest;\n    isc_tr_handle\ttransaction;\n");

	if (port)
		for (reference = port->por_references; reference;
			 reference = reference->ref_next) {
			field = reference->ref_field;
			switch (field->fld_dtype) {
			case dtype_short:
				dtype = "short";
				break;

			case dtype_long:
				dtype = "long";
				break;

			case dtype_cstring:
			case dtype_text:
				dtype = "char*";
				break;

			case dtype_quad:
				dtype = "long";
				break;

			case dtype_date:
			case dtype_blob:
				dtype = "ISC_QUAD";
				break;

			case dtype_real:
				dtype = "float";
				break;

			case dtype_double:
				dtype = "double";
				break;

			default:
				CPR_error("gen_function: unsupported datatype");
				return;
			}
			ib_fprintf(out_file, "    %s\t%s;\n", dtype,
					   gen_name(s, reference->ref_source, true));
		}

	ib_fprintf(out_file, "{\n");
	for (port = request->req_ports; port; port = port->por_next)
		make_port(port, column);

	ib_fprintf(out_file, "\n\n");
	gen_s_start(action, 0);
	gen_receive(action, column, request->req_primary);

	for (port = request->req_ports; port; port = port->por_next)
		for (reference = port->por_references; reference;
			 reference = reference->ref_next)
		{
			if (reference->ref_field->fld_array_info)
				gen_get_or_put_slice(action, reference, true, column);
		}

	port = request->req_primary;
	ib_fprintf(out_file, "\nreturn %s;\n}\n",
			   gen_name(s, port->por_references, true)
		);
}

//____________________________________________________________
//  
//		Generate a call to isc_get_slice
//       or isc_put_slice for an array.
//  

static void gen_get_or_put_slice(const act* action,
								 REF reference,
								 bool get,
								 int column)
{
	PAT args;
	TEXT s1[25], s2[25], s3[25], s4[25];
	TEXT *pattern1 =
		"interbase.GET_SLICE (%V1 %RF%DH%RE, %RF%S1%RE, %S2, %N1, %S3, %N2, %S4, %L1, %S5, %S6);";
	TEXT *pattern2 =
		"interbase.PUT_SLICE (%V1 %RF%DH%RE, %RF%S1%RE, %S2, %N1, %S3, %N2, %S4, %L1, %S5);";

	if (!(reference->ref_flags & REF_fetch_array))
		return;

	args.pat_vector1 = status_vector(action);	/* status vector */
	args.pat_database = action->act_request->req_database;	/* database handle */
	args.pat_string1 = action->act_request->req_trans;	/* transaction handle */

	gen_name(s1, reference, true);	// blob handle
	args.pat_string2 = s1;

	args.pat_value1 = reference->ref_sdl_length;	/* slice descr. length */

	sprintf(s2, "isc_%d", reference->ref_sdl_ident);	/* slice description */
	args.pat_string3 = s2;

	args.pat_value2 = 0;		/* parameter length */

	sprintf(s3, "isc_null_vector_l");	/* parameter block init */
	args.pat_string4 = s3;

	args.pat_long1 = reference->ref_field->fld_array_info->ary_size;
	/*  slice size */
	if (action->act_flags & ACT_sql) {
		sprintf(s4, "%s'address", reference->ref_value);
	}
	else {
		sprintf(s4, "isc_%d'address",
				reference->ref_field->fld_array_info->ary_ident);

	}
	args.pat_string5 = s4;		/* array name */

	args.pat_string6 = "isc_array_length";	/* return length */

	if (get)
		PATTERN_expand(column, pattern1, &args);
	else
		PATTERN_expand(column, pattern2, &args);
}


//____________________________________________________________
//  
//		Generate the code to do a get segment.
//  

static void gen_get_segment( const act* action, int column)
{
	BLB blob;
	REF into;

	if (action->act_flags & ACT_sql)
		blob = (BLB) action->act_request->req_blobs;
	else
		blob = (BLB) action->act_object;

	if (action->act_error || (action->act_flags & ACT_sql))
		printa(column,
			   "interbase.ISC_GET_SEGMENT (isc_status, isc_%d, isc_%d, %d, isc_%d'address);",
			   blob->blb_ident,
			   blob->blb_len_ident,
			   blob->blb_seg_length, blob->blb_buff_ident);
	else
		printa(column,
			   "interbase.ISC_GET_SEGMENT (isc_status(1), isc_%d, isc_%d, %d, isc_%d'address);",
			   blob->blb_ident,
			   blob->blb_len_ident,
			   blob->blb_seg_length, blob->blb_buff_ident);

	if (action->act_flags & ACT_sql) {
		into = action->act_object;
		set_sqlcode(action, column);
		printa(column, "if (SQLCODE = 0) or (SQLCODE = 101) then");
		printa(column + INDENT, "%s := isc_%d;",
			   into->ref_value, blob->blb_buff_ident);
		if (into->ref_null_value)
			printa(column + INDENT, "%s := isc_%d;",
				   into->ref_null_value, blob->blb_len_ident);
		endif(column);
	}
}


//____________________________________________________________
//  
//		Generate code to drive a mass update.  Just start a request
//		and get the result.
//  

static void gen_loop( const act* action, int column)
{
	GPRE_REQ request;
	POR port;
	TEXT name[20];

	gen_s_start(action, column);
	request = action->act_request;
	port = request->req_primary;
	printa(column, "if SQLCODE = 0 then");
	column += INDENT;
	gen_receive(action, column, port);
	gen_name(name, port->por_references, true);
	printa(column, "if (SQLCODE = 0) and (%s = 0) then", name);
	printa(column + INDENT, "SQLCODE := 100;");
	endif(column);
	column -= INDENT;
	endif(column);
}


//____________________________________________________________
//  
//		Generate a name for a reference.  Name is constructed from
//		port and parameter idents.
//  

static TEXT *gen_name(TEXT * string,
					  REF reference,
					  bool as_blob)
{

	if (reference->ref_field->fld_array_info && !as_blob)
		sprintf(string, "isc_%d",
				reference->ref_field->fld_array_info->ary_ident);
	else
		sprintf(string, "isc_%d.isc_%d",
				reference->ref_port->por_ident, reference->ref_ident);

	return string;
}


//____________________________________________________________
//  
//		Generate a block to handle errors.
//  

static void gen_on_error( const act* action, USHORT column)
{
	const act* err_action;

	err_action = (const act*) action->act_object;
	if ((err_action->act_type == ACT_get_segment) ||
		(err_action->act_type == ACT_put_segment) ||
		(err_action->act_type == ACT_endblob))
			printa(column,
				   "if (isc_status (1) /= 0) and (isc_status(1) /= interbase.isc_segment) and (isc_status(1) /= interbase.isc_segstr_eof) then");
	else
		printa(column, "if (isc_status (1) /= 0) then");
}


//____________________________________________________________
//  
//		Generate code for an EXECUTE PROCEDURE.
//  

static void gen_procedure( const act* action, int column)
{
	PAT args;
	TEXT *pattern;
	GPRE_REQ request;
	POR in_port, out_port;
	dbb* database;

	request = action->act_request;
	in_port = request->req_vport;
	out_port = request->req_primary;

	database = request->req_database;
	args.pat_database = database;
	args.pat_request = action->act_request;
	args.pat_vector1 = status_vector(action);
	args.pat_string2 = ada_null_address;
	args.pat_request = request;
	args.pat_port = in_port;
	args.pat_port2 = out_port;
	if (in_port && in_port->por_length)
		pattern =
			"interbase.gds_transact_request (%V1 %RF%DH%RE, %RF%RT%RE, %VF%RS%VE, %RF%RI%RE, %VF%PL%VE, %PI'address, %VF%QL%VE, %QI'address);\n";
	else
		pattern =
			"interbase.gds_transact_request (%V1 %RF%DH%RE, %RF%RT%RE, %VF%RS%VE, %RF%RI%RE, %VF0%VE, %S2, %VF%QL%VE, %QI'address);\n";


//  Get database attach and transaction started 

	if (sw_auto)
		t_start_auto(action, 0, status_vector(action), column, true);

//  Move in input values 

	asgn_from(action, request->req_values, column);

//  Execute the procedure 

	PATTERN_expand(column, pattern, &args);

	set_sqlcode(action, column);

	printa(column, "if SQLCODE = 0 then");

//  Move out output values 

	asgn_to_proc(request->req_references, column);
	endif(column);
}


//____________________________________________________________
//  
//		Generate the code to do a put segment.
//  

static void gen_put_segment( const act* action, int column)
{
	BLB blob;
	REF from;

	if (action->act_flags & ACT_sql) {
		blob = (BLB) action->act_request->req_blobs;
		from = action->act_object;
		printa(column, "isc_%d := %s;",
			   blob->blb_len_ident, from->ref_null_value);
		printa(column, "isc_%d := %s;",
			   blob->blb_buff_ident, from->ref_value);
	}
	else
		blob = (BLB) action->act_object;

	printa(column,
		   "interbase.ISC_PUT_SEGMENT (%s isc_%d, isc_%d, isc_%d'address);",
		   status_vector(action),
		   blob->blb_ident, blob->blb_len_ident, blob->blb_buff_ident);

	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate BLR in raw, numeric form.  Ughly but dense.
//  

static void gen_raw(
			   UCHAR * blr,
			   enum req_t request_type, int request_length, int column)
{
	UCHAR *end;
	UCHAR c;
	TEXT buffer[80], *p, *limit;

	p = buffer;
	limit = buffer + 60;

	for (end = blr + request_length - 1; blr <= end; blr++) {
		c = *blr;
		sprintf(p, "%d", c);
		while (*p)
			p++;
		if (blr != end)
			*p++ = ',';
		if (p < limit)
			continue;
		*p = 0;
		printa(INDENT, buffer);
		p = buffer;
	}

	*p = 0;
	printa(INDENT, buffer);
}


//____________________________________________________________
//  
//		Generate substitution text for READY
//  

static void gen_ready( const act* action, int column)
{
	RDY ready;
	DBB db;
	TEXT *filename, *vector;

	vector = status_vector(action);

	for (ready = (RDY) action->act_object; ready; ready = ready->rdy_next) {
		db = ready->rdy_database;
		if (!(filename = ready->rdy_filename))
			filename = db->dbb_runtime;
		if ((action->act_error || (action->act_flags & ACT_sql)) &&
			ready != (RDY) action->act_object)
			printa(column, "if isc_status(1) = 0 then");
		make_ready(db, filename, vector, column, ready->rdy_request);
		if ((action->act_error || (action->act_flags & ACT_sql)) &&
			ready != (RDY) action->act_object)
			endif(column);
	}
	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate receive call for a port.
//  

static void gen_receive( const act* action, int column, POR port)
{
	GPRE_REQ request;

	request = action->act_request;
	printa(column, "interbase.RECEIVE (%s %s, %d, %d, isc_%d'address, %s);",
		   status_vector(action),
		   request->req_handle,
		   port->por_msg_number,
		   port->por_length, port->por_ident, request->req_request_level);

	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate substitution text for RELEASE_REQUESTS
//       For active databases, call isc_release_request.
//       for all others, just zero the handle.  For the
//       release request calls, ignore error returns, which
//       are likely if the request was compiled on a database
//       which has been released and re-readied.  If there is
//       a serious error, it will be caught on the next statement.
//  

static void gen_release( const act* action, int column)
{
	DBB db, exp_db;
	GPRE_REQ request;

	exp_db = (DBB) action->act_object;

	for (request = requests; request; request = request->req_next) {
		db = request->req_database;
		if (exp_db && db != exp_db)
			continue;
		if (!(request->req_flags & REQ_exp_hand)) {
			printa(column, "if %s%s /= 0 then", ada_package,
				   db->dbb_name->sym_string);
			printa(column + INDENT,
				   "interbase.release_request (isc_status, %s);",
				   request->req_handle);
			endif(column);
			printa(column, "%s := 0;", request->req_handle);
		}
	}
}


//____________________________________________________________
//  
//		Generate definitions associated with a single request.
//  

static void gen_request( GPRE_REQ request, int column)
{
	REF reference;
	BLB blob;
	POR port;
	SSHORT length;
	TEXT *string_type;

//  generate request handle, blob handles, and ports 

	if (!(request->req_flags &
		  (REQ_exp_hand  | REQ_sql_blob_open | REQ_sql_blob_create)) &&
		request->req_type != REQ_slice && request->req_type != REQ_procedure) 
	{
		printa(column, "%s\t: interbase.request_handle := 0;-- request handle --",
			   request->req_handle);
	}

	if (request->req_flags & (REQ_sql_blob_open | REQ_sql_blob_create))
		printa(column, "isc_%do\t: %s;\t\t-- SQL CURSOR FLAG --",
			   request->req_ident, SHORT_DCL);

//  check the case where we need to extend the dpb dynamically at runtime,
//  in which case we need dpb length and a pointer to be defined even if 
//  there is no static dpb defined 

	if (request->req_flags & REQ_extend_dpb) {
		printa(column,
			   "isc_%dp\t: interbase.dpb_ptr := 0;\t-- db parameter block --",
			   request->req_ident);
		if (!request->req_length)
			printa(column,
				   "isc_%dl\t: interbase.isc_ushort := 0;\t-- db parameter block --",
				   request->req_ident);
	}

//  generate actual BLR string 

	if (request->req_length) {
		length = request->req_length;
		switch (request->req_type) {
		case REQ_create_database:
			if (ada_flags & ADA_create_database) {
				printa(column,
					   "function isc_to_dpb_ptr is new unchecked_conversion (system.address, interbase.dpb_ptr);");
				ada_flags &= ~ADA_create_database;
			}
			/* fall into ... */
		case REQ_ready:
			printa(column,
				   "isc_%dl\t: interbase.isc_ushort := %d;\t-- db parameter block --",
				   request->req_ident, length);
			printa(column, "isc_%d\t: CONSTANT interbase.dpb (1..%d) := (",
				   request->req_ident, length);
			break;

		default:
			if (request->req_flags & REQ_sql_cursor)
				printa(column, "isc_%do\t: %s;\t\t-- SQL CURSOR FLAG --",
					   request->req_ident, SHORT_DCL);
#ifdef SCROLLABLE_CURSORS
			if (request->req_flags & REQ_scroll)
				printa(column,
					   "isc_%ddirection\t: interbase.isc_ushort := 0;\t-- last direction sent to engine --",
					   request->req_ident);
#endif
			printa(column, "isc_%d\t: CONSTANT interbase.blr (1..%d) := (",
				   request->req_ident, length);
		}
		gen_raw(request->req_blr, request->req_type, request->req_length,
				column);
		printa(column, ");\n");
		if (!sw_raw) {
			printa(column, "---");
			printa(column, "--- FORMATTED REQUEST BLR FOR GDS_%d = \n",
				   request->req_ident);
			switch (request->req_type) {
			case REQ_create_database:
			case REQ_ready:
				string_type = "DPB";
				if (PRETTY_print_cdb(request->req_blr,
									 gen_blr, 0, 0))
					CPR_error("internal error during parameter generation");
				break;

			case REQ_ddl:
				string_type = "DYN";
				if (PRETTY_print_dyn(request->req_blr,
									 gen_blr, 0, 0))
					CPR_error("internal error during dynamic DDL generation");
				break;
			case REQ_slice:
				string_type = "SDL";
				if (PRETTY_print_sdl(request->req_blr,
									 gen_blr, 0, 0))
					CPR_error("internal error during SDL generation");
				break;

			default:
				string_type = "BLR";
				if (gds__print_blr(request->req_blr,
								   gen_blr, 0, 0))
					CPR_error("internal error during BLR generation");
			}
		}
		else {
			switch (request->req_type) {
			case REQ_create_database:
			case REQ_ready:
				string_type = "DPB";
				break;
			case REQ_ddl:
				string_type = "DYN";
				break;
			case REQ_slice:
				string_type = "SDL";
				break;

			default:
				string_type = "BLR";
			}
		}
		printa(column, "\t-- end of %s string for request isc_%d --\n",
			   string_type, request->req_ident, request->req_length);
	}

//   Print out slice description language if there are arrays associated with request  
	for (port = request->req_ports; port; port = port->por_next)
		for (reference = port->por_references; reference;
			 reference = reference->ref_next)
		{
			if (reference->ref_sdl) {
				printa(column,
					   "isc_%d\t: CONSTANT interbase.blr (1..%d) := (",
					   reference->ref_sdl_ident, reference->ref_sdl_length);
				gen_raw(reference->ref_sdl, REQ_slice,
						reference->ref_sdl_length, column);
				printa(column, "); \t--- end of SDL string for isc_%d\n",
					   reference->ref_sdl_ident);
				if (!sw_raw) {
					printa(column, "---");
					printa(column,
						   "--- FORMATTED REQUEST SDL FOR GDS_%d = \n",
						   reference->ref_sdl_ident);
					if (PRETTY_print_sdl(reference->ref_sdl,
										 gen_blr, 0, 1))
						CPR_error("internal error during SDL generation");
				}
			}
		}

//  Print out any blob parameter blocks required 

	for (blob = request->req_blobs; blob; blob = blob->blb_next)
		if (blob->blb_bpb_length) {
			printa(column, "isc_%d\t: CONSTANT interbase.blr (1..%d) := (",
				   blob->blb_bpb_ident, blob->blb_bpb_length);
			gen_raw(blob->blb_bpb, request->req_type, blob->blb_bpb_length,
					column);
			printa(INDENT, ");\n");
		}

//  If this is a GET_SLICE/PUT_slice, allocate some variables 

	if (request->req_type == REQ_slice) {
		printa(INDENT, "isc_%ds\t: %s;\t\t", request->req_ident, LONG_DCL);
		printa(INDENT, "isc_%dv: %s (1..%d);", request->req_ident,
			   LONG_VECTOR_DCL, MAX(request->req_slice->slc_parameters, 1));
	}
}


//____________________________________________________________
//  
//		Generate receive call for a port
//		in a store2 statement.
//  

static void gen_return_value( const act* action, int column)
{
	UPD update;
	REF reference;
	GPRE_REQ request;

	request = action->act_request;
	if (action->act_pair->act_error)
		column += INDENT;
	align(column);

	gen_start(action, request->req_primary, column);
	update = (UPD) action->act_object;
	reference = update->upd_references;
	gen_receive(action, column, reference->ref_port);
}


//____________________________________________________________
//  
//		Process routine head.  If there are requests in the
//		routine, insert local definitions.
//  

static void gen_routine( const act* action, int column)
{
	BLB blob;
	GPRE_REQ request;
	POR port;
	SSHORT blob_subtype;
	REF reference;
	GPRE_FLD field;

	column += INDENT;

	for (request = (GPRE_REQ) action->act_object; request;
		 request = request->req_routine) {
		for (port = request->req_ports; port; port = port->por_next)
			make_port(port, column);
		for (port = request->req_ports; port; port = port->por_next)
			printa(column, "isc_%d\t: isc_%dt;\t\t\t-- message --",
				   port->por_ident, port->por_ident);

		for (blob = request->req_blobs; blob; blob = blob->blb_next) {
			printa(column,
				   "isc_%d\t: interbase.blob_handle;\t-- blob handle --",
				   blob->blb_ident);
			if (!(blob_subtype = blob->blb_const_to_type))
				if (reference = blob->blb_reference)
					if (field = reference->ref_field)
						blob_subtype = field->fld_sub_type;
			if (blob_subtype != 0 && blob_subtype != 1)
				printa(column, "isc_%d\t: %s (1..%d);\t-- blob segment --",
					   blob->blb_buff_ident, BYTE_VECTOR_DCL,
					   blob->blb_seg_length);
			else
				printa(column,
					   "isc_%d\t: string (1..%d);\t-- blob segment --",
					   blob->blb_buff_ident, blob->blb_seg_length);
			printa(column, "isc_%d\t: %s;\t\t\t-- segment length --",
				   blob->blb_len_ident, USHORT_DCL);
		}
	}
	column -= INDENT;
}


//____________________________________________________________
//  
//		Generate substitution text for END_STREAM.
//  

static void gen_s_end( const act* action, int column)
{
	GPRE_REQ request;

	request = action->act_request;

	if (action->act_type == ACT_close) {
		make_cursor_open_test(ACT_close, request, column);
		column += INDENT;
	}

	printa(column, "interbase.UNWIND_REQUEST (%s %s, %s);",
		   status_vector(action),
		   request->req_handle, request->req_request_level);

	set_sqlcode(action, column);

	if (action->act_type == ACT_close) {
		printa(column, "if (SQLCODE = 0) then");
		printa(column + INDENT, "isc_%do := 0;", request->req_ident);
		endif(column);
		column -= INDENT;
		endif(column);
	}
}


//____________________________________________________________
//  
//		Generate substitution text for FETCH.
//  

static void gen_s_fetch( const act* action, int column)
{
	GPRE_REQ request;

	request = action->act_request;
	if (request->req_sync)
		gen_send(action, request->req_sync, column);

	gen_receive(action, column, request->req_primary);
}


//____________________________________________________________
//  
//		Generate text to compile and start a stream.  This is
//		used both by START_STREAM and FOR
//  

static void gen_s_start( const act* action, int column)
{
	GPRE_REQ request;
	POR port;

	request = action->act_request;

	if ((action->act_type == ACT_open)) {
		make_cursor_open_test(ACT_open, request, column);
		column += INDENT;
	}

	gen_compile(action, column);

	if (port = request->req_vport)
		asgn_from(action, port->por_references, column);

	if (action->act_error || (action->act_flags & ACT_sql)) {
		make_ok_test(action, request, column);
		gen_start(action, port, column + INDENT);
		endif(column);
	}
	else
		gen_start(action, port, column);

	if (action->act_type == ACT_open) {
		printa(column, "if (SQLCODE = 0) then");
		printa(column + INDENT, "isc_%do := 1;", request->req_ident);
		endif(column);
		column -= INDENT;
		endif(column);
	}
}


//____________________________________________________________
//  
//		Substitute for a segment, segment length, or blob handle.
//  

static void gen_segment( const act* action, int column)
{
	BLB blob;

	blob = (BLB) action->act_object;

	printa(column, "isc_%d",
		   (action->act_type == ACT_segment) ? blob->blb_buff_ident :
		   (action->act_type == ACT_segment_length) ? blob->blb_len_ident :
		   blob->blb_ident);
}


//____________________________________________________________
//  
//		Generate code for standalone SELECT statement.
//  

static void gen_select( const act* action, int column)
{
	GPRE_REQ request;
	POR port;
	GPRE_NOD var_list;
	int i;
	TEXT name[20];

	request = action->act_request;
	port = request->req_primary;
	gen_name(name, request->req_eof, true);

	gen_s_start(action, column);
	gen_receive(action, column, port);
	printa(column, "if SQLCODE = 0 then", name);
	column += INDENT;
	printa(column, "if %s /= 0 then", name);
	column += INDENT;

	if (var_list = (GPRE_NOD) action->act_object)
		for (i = 0; i < var_list->nod_count; i++)
			asgn_to(action, (REF) var_list->nod_arg[i], column);
	if (request->req_database->dbb_flags & DBB_v3) {
		gen_receive(action, column, port);
		printa(column, "if (SQLCODE = 0) AND (%s /= 0) then", name);
		printa(column + INDENT, "SQLCODE := -1;");
		endif(column);
	}

	printa(column - INDENT, "else");
	printa(column, "SQLCODE := 100;");
	column -= INDENT;
	endif(column);
	column -= INDENT;
	endif(column);
}


//____________________________________________________________
//  
//		Generate a send or receive call for a port.
//  

static void gen_send( const act* action, POR port, int column)
{
	GPRE_REQ request;

	request = action->act_request;
	printa(column, "interbase.SEND (%s %s, %d, %d, isc_%d'address, %s);",
		   status_vector(action),
		   request->req_handle,
		   port->por_msg_number,
		   port->por_length, port->por_ident, request->req_request_level);

	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate support for get/put slice statement.
//  

static void gen_slice( const act* action, int column)
{
	GPRE_REQ request, parent_request;
	REF reference, upper, lower;
	SLC slice;
	PAT args;
	slc::slc_repeat *tail, *end;
	TEXT *pattern1 =
		"interbase.GET_SLICE (%V1 %RF%DH%RE, %RF%RT%RE, %RF%FR%RE, %N1, \
%I1, %N2, %I1v, %I1s, %RF%S5'address%RE, %RF%S6%RE);";
	TEXT *pattern2 =
		"interbase.PUT_SLICE (%V1 %RF%DH%RE, %RF%RT%RE, %RF%FR%RE, %N1, \
%I1, %N2, %I1v, %I1s, %RF%S5'address%RE);";

	request = action->act_request;
	slice = (SLC) action->act_object;
	parent_request = slice->slc_parent_request;

//  Compute array size 

	printa(column, "isc_%ds := %d", request->req_ident,
		   slice->slc_field->fld_array->fld_length);

	for (tail = slice->slc_rpt, end = tail + slice->slc_dimensions;
		 tail < end; ++tail)
		if (tail->slc_upper != tail->slc_lower) {
			lower = (REF) tail->slc_lower->nod_arg[0];
			upper = (REF) tail->slc_upper->nod_arg[0];
			if (lower->ref_value)
				ib_fprintf(out_file, " * ( %s - %s + 1)", upper->ref_value,
						   lower->ref_value);
			else
				ib_fprintf(out_file, " * ( %s + 1)", upper->ref_value);
		}

	ib_fprintf(out_file, ";");

//  Make assignments to variable vector 

	for (reference = request->req_values; reference;
		 reference =
		 reference->ref_next) printa(column, "isc_%dv (%d) := %s;",
									 request->req_ident,
									 reference->ref_id + 1,
									 reference->ref_value);

	args.pat_reference = slice->slc_field_ref;
	args.pat_request = parent_request;	/* blob id request */
	args.pat_vector1 = status_vector(action);	/* status vector */
	args.pat_database = parent_request->req_database;	/* database handle */
	args.pat_value1 = request->req_length;	/* slice descr. length */
	args.pat_ident1 = request->req_ident;	/* request name */
	args.pat_value2 = slice->slc_parameters * sizeof(SLONG);	/* parameter length */

	reference = (REF) slice->slc_array->nod_arg[0];
	args.pat_string5 = reference->ref_value;	/* array name */
	args.pat_string6 = "isc_array_length";

	PATTERN_expand(column,
				   (action->act_type == ACT_get_slice) ? pattern1 : pattern2,
				   &args);
}


//____________________________________________________________
//  
//		Generate either a START or START_AND_SEND depending
//		on whether or a not a port is present.
//  

static void gen_start( const act* action, POR port, int column)
{
	GPRE_REQ request;
	TEXT *vector;
	REF reference;

	request = action->act_request;
	vector = status_vector(action);
	column += INDENT;

	if (port) {
		for (reference = port->por_references; reference;
			 reference = reference->ref_next)
		{
			if (reference->ref_field->fld_array_info)
					gen_get_or_put_slice(action, reference, false, column);
		}

		printa(column,
			   "interbase.START_AND_SEND (%s %s, %s%s, %d, %d, isc_%d'address, %s);",
			   vector, request->req_handle, ada_package, request_trans(action,
																	   request),
			   port->por_msg_number, port->por_length, port->por_ident,
			   request->req_request_level);
	}
	else
		printa(column, "interbase.START_REQUEST (%s %s, %s%s, %s);",
			   vector,
			   request->req_handle,
			   ada_package, request_trans(action, request),
			   request->req_request_level);

	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate text for STORE statement.  This includes the compile
//		call and any variable initialization required.
//  

static void gen_store( const act* action, int column)
{
	GPRE_REQ request;
	REF reference;
	GPRE_FLD field;
	POR port;
	TEXT name[64];

	request = action->act_request;
	gen_compile(action, column);
	if (action->act_error || (action->act_flags & ACT_sql))
		make_ok_test(action, request, column);

//  Initialize any blob fields 

	port = request->req_primary;
	for (reference = port->por_references; reference;
		 reference = reference->ref_next) {
		field = reference->ref_field;
		if (field->fld_flags & FLD_blob)
			printa(column, "%s := interbase.null_blob;",
				   gen_name(name, reference, true));
	}
}


//____________________________________________________________
//  
//		Generate substitution text for START_TRANSACTION.
//  

static void gen_t_start( const act* action, int column)
{
	DBB db;
	GPRE_TRA trans;
	tpb* tpb_iterator;
	int count;
	TEXT *filename;

//  for automatically generated transactions, and transactions that are
//  explicitly started, but don't have any arguments so don't get a TPB,
//  generate something plausible. 

	if (!action || !(trans = (GPRE_TRA) action->act_object)) {
		t_start_auto(action, 0, status_vector(action), column, false);
		return;
	}

//  build a complete statement, including tpb's.
//  first generate any appropriate ready statements,
//  and fill in the tpb vector (aka TEB). 

	count = 0;
	for (tpb_iterator = trans->tra_tpb;
		 tpb_iterator;
		 tpb_iterator = tpb_iterator->tpb_tra_next)
	{
		count++;
		db = tpb_iterator->tpb_database;
		if (sw_auto)
			if ((filename = db->dbb_runtime) || !(db->dbb_flags & DBB_sqlca)) {
				printa(column, "if (%s%s = 0) then", ada_package,
					   db->dbb_name->sym_string);
				align(column + INDENT);
				make_ready(db, filename, status_vector(action),
						   column + INDENT, 0);
				endif(column);
			}

		printa(column, "isc_teb(%d).tpb_len := %d;", count, tpb_iterator->tpb_length);
		printa(column, "isc_teb(%d).tpb_ptr := isc_tpb_%d'address;",
			   count, tpb_iterator->tpb_ident);
		printa(column, "isc_teb(%d).dbb_ptr := %s%s'address;",
			   count, ada_package, db->dbb_name->sym_string);
	}

	printa(column, "interbase.start_multiple (%s %s%s, %d, isc_teb'address);",
		   status_vector(action),
		   ada_package,
		   (trans->tra_handle) ? (SCHAR *) trans->tra_handle : "gds_trans",
		   trans->tra_db_count);

	set_sqlcode(action, column);
}


//____________________________________________________________
//  
//		Generate a TPB in the output file
//  

static void gen_tpb(tpb* tpb_buffer, int column)
{
	TEXT *text, buffer[80], c, *p;
	int length;

	printa(column, "isc_tpb_%d\t: CONSTANT interbase.tpb (1..%d) := (",
		   tpb_buffer->tpb_ident, tpb_buffer->tpb_length);

	length = tpb_buffer->tpb_length;
	text = (TEXT *) tpb_buffer->tpb_string;
	p = buffer;

	while (--length) {
		c = *text++;
		sprintf(p, "%d, ", c);
		while (*p)
			p++;
		if (p - buffer > 60) {
			printa(column + INDENT, " %s", buffer);
			p = buffer;
			*p = 0;
		}
	}

//  handle the last character 

	c = *text++;
	sprintf(p, "%d", c);
	printa(column + INDENT, "%s", buffer);
	printa(column, ");");
}


//____________________________________________________________
//  
//		Generate substitution text for COMMIT, ROLLBACK, PREPARE, and SAVE
//  

static void gen_trans( const act* action, int column)
{

	if (action->act_type == ACT_commit_retain_context)
		printa(column, "interbase.COMMIT_RETAINING (%s %s%s);",
			   status_vector(action),
			   ada_package,
			   (action->act_object) ? (TEXT *) action->
			   act_object : "gds_trans");
	else
		printa(column, "interbase.%s_TRANSACTION (%s %s%s);",
			   (action->act_type ==
				ACT_commit) ? "COMMIT" : (action->act_type ==
										  ACT_rollback) ? "ROLLBACK" :
			   "PREPARE", status_vector(action), ada_package,
			   (action->act_object) ? (TEXT *) action->
			   act_object : "gds_trans");

	set_sqlcode(action, column);
}



//____________________________________________________________
//  
//       Substitute for a variable reference.
//  

static void gen_type( const act* action, int column)
{

	printa(column, "%ld", action->act_object);
}









//____________________________________________________________
//  
//		Generate substitution text for UPDATE ... WHERE CURRENT OF ...
//  

static void gen_update( const act* action, int column)
{
	POR port;
	UPD modify;

	modify = (UPD) action->act_object;
	port = modify->upd_port;
	asgn_from(action, port->por_references, column);
	gen_send(action, port, column);
}


//____________________________________________________________
//  
//		Substitute for a variable reference.
//  

static void gen_variable( const act* action, int column)
{
	TEXT s[20];

	printa(column, gen_name(s, (REF) action->act_object, false));
}


//____________________________________________________________
//  
//		Generate tests for any WHENEVER clauses that may have been declared.
//  

static void gen_whenever( SWE label, int column)
{
	TEXT *condition;

	while (label) {
		switch (label->swe_condition) {
		case SWE_error:
			condition = "SQLCODE < 0";
			break;

		case SWE_warning:
			condition = "(SQLCODE > 0) AND (SQLCODE /= 100)";
			break;

		case SWE_not_found:
			condition = "SQLCODE = 100";
			break;
		}
		printa(column, "if %s then goto %s; end if;", condition,
			   label->swe_label);
		label = label->swe_next;
	}
}


//____________________________________________________________
//  
//		Generate a declaration of an array in the
//       output file.
//  

static void make_array_declaration( REF reference, int column)
{
	GPRE_FLD field;
	DIM dimension;
	int i;

	field = reference->ref_field;
	const TEXT* name = field->fld_symbol->sym_string;

//  Don't generate multiple declarations for the array.  V3 Bug 569.  

	if (field->fld_array_info->ary_declared)
		return;

	field->fld_array_info->ary_declared = true;

	if ((field->fld_array->fld_dtype <= dtype_varying)
		&& (field->fld_array->fld_length != 1)) {
		if (field->fld_array->fld_sub_type == 1)
			ib_fprintf(out_file, "subtype isc_%d_byte is %s(1..%d);\n",
					   field->fld_array_info->ary_ident, BYTE_VECTOR_DCL,
					   field->fld_array->fld_length);
		else
			ib_fprintf(out_file, "subtype isc_%d_string is string(1..%d);\n",
					   field->fld_array_info->ary_ident,
					   field->fld_array->fld_length);
	}

	ib_fprintf(out_file, "type isc_%dt is array (",
			   field->fld_array_info->ary_ident);

//   Print out the dimension part of the declaration  
	for (dimension = field->fld_array_info->ary_dimension, i = 1;
		 i < field->fld_array_info->ary_dimension_count;
		 dimension = dimension->dim_next, i++)
		ib_fprintf(out_file, "%s range %"SLONGFORMAT"..%"SLONGFORMAT
				   ",\n                        ",
				   LONG_DCL, dimension->dim_lower, dimension->dim_upper);

	ib_fprintf(out_file, "%s range %"SLONGFORMAT"..%"SLONGFORMAT") of ",
			   LONG_DCL, dimension->dim_lower, dimension->dim_upper);

	switch (field->fld_array_info->ary_dtype) {
	case dtype_short:
		ib_fprintf(out_file, "%s", SHORT_DCL);
		break;

	case dtype_long:
		ib_fprintf(out_file, "%s", LONG_DCL);
		break;

	case dtype_cstring:
	case dtype_text:
	case dtype_varying:
		if (field->fld_array->fld_length == 1) {
			if (field->fld_array->fld_sub_type == 1)
				ib_fprintf(out_file, BYTE_DCL);
			else
				ib_fprintf(out_file, "interbase.isc_character");
		}
		else {
			if (field->fld_array->fld_sub_type == 1)
				ib_fprintf(out_file, "isc_%d_byte",
						   field->fld_array_info->ary_ident);
			else
				ib_fprintf(out_file, "isc_%d_string",
						   field->fld_array_info->ary_ident);
		}
		break;

	case dtype_date:
	case dtype_quad:
		ib_fprintf(out_file, "interbase.quad");
		break;

	case dtype_real:
		ib_fprintf(out_file, "%s", REAL_DCL);
		break;

	case dtype_double:
		ib_fprintf(out_file, "%s", DOUBLE_DCL);
		break;

	default:
		printa(column, "datatype %d unknown for field %s",
			   field->fld_array_info->ary_dtype, name);
		return;
	}

	ib_fprintf(out_file, ";\n");

//   Print out the database field  

	ib_fprintf(out_file, "isc_%d : isc_%dt;\t--- %s\n\n",
			   field->fld_array_info->ary_ident,
			   field->fld_array_info->ary_ident, name);
}


//____________________________________________________________
//  
//     Generate code to test existence 
//     of open cursor and do the right thing:
//     if type == ACT_open && isc_nl, error
//     if type == ACT_close && !isc_nl, error
//  

static void make_cursor_open_test( enum act_t type, GPRE_REQ request, int column)
{

	if (type == ACT_open) {
		printa(column, "if (isc_%do = 1) then", request->req_ident);
		printa(column + INDENT, "SQLCODE := -502;");
	}
	else if (type == ACT_close) {
		printa(column, "if (isc_%do = 0) then", request->req_ident);
		printa(column + INDENT, "SQLCODE := -501;");
	}

	printa(column, "else");
}


//____________________________________________________________
//  
//		Turn a symbol into a varying string.
//  

static TEXT *make_name( TEXT * string, SYM symbol)
{

	sprintf(string, "\"%s \"", symbol->sym_string);

	return string;
}


//____________________________________________________________
//  
//		Generate code to test existence of
//		compiled request with active transaction.
//  

static void make_ok_test( const act* action, GPRE_REQ request, int column)
{

	if (sw_auto)
		printa(column, "if (%s%s /= 0) and (%s /= 0) then",
			   ada_package, request_trans(action, request),
			   request->req_handle);
	else
		printa(column, "if (%s /= 0) then", request->req_handle);
}


//____________________________________________________________
//  
//		Insert a port record description in output.
//  

static void make_port( POR port, int column)
{
	GPRE_FLD field;
	REF reference;
	SYM symbol;
	TEXT s[80];
	int pos;

	printa(column, "type isc_%dt is record", port->por_ident);

	for (reference = port->por_references; reference;
		 reference = reference->ref_next) 
	{
		field = reference->ref_field;
		const TEXT* name;
		if (symbol = field->fld_symbol)
			name = symbol->sym_string;
		else
			name = "<expression>";
		if (reference->ref_value && (reference->ref_flags & REF_array_elem))
			field = field->fld_array;

		switch (field->fld_dtype) {
		case dtype_real:
			printa(column + INDENT, "isc_%d\t: %s;\t-- %s --",
				   reference->ref_ident, REAL_DCL, name);
			break;

		case dtype_double:
			printa(column + INDENT, "isc_%d\t: %s;\t-- %s --",
				   reference->ref_ident, DOUBLE_DCL, name);
			break;

		case dtype_short:
			printa(column + INDENT, "isc_%d\t: %s;\t-- %s --",
				   reference->ref_ident, SHORT_DCL, name);
			break;

		case dtype_long:
			printa(column + INDENT, "isc_%d\t: %s;\t-- %s --",
				   reference->ref_ident, LONG_DCL, name);
			break;

		case dtype_date:
		case dtype_quad:
		case dtype_blob:
			printa(column + INDENT, "isc_%d\t: interbase.quad;\t-- %s --",
				   reference->ref_ident, name);
			break;

		case dtype_text:
			if (strcmp(name, "isc_slack")) {
				if (field->fld_sub_type == 1) {
					if (field->fld_length == 1)
						printa(column + INDENT, "isc_%d\t: %s;\t-- %s --",
							   reference->ref_ident, BYTE_DCL, name);
					else
						printa(column + INDENT,
							   "isc_%d\t: %s (1..%d);\t-- %s --",
							   reference->ref_ident, BYTE_VECTOR_DCL,
							   field->fld_length, name);
				}
				else {
					if (field->fld_length == 1)
						printa(column + INDENT,
							   "isc_%d\t: interbase.isc_character;\t-- %s --",
							   reference->ref_ident, name);
					else
						printa(column + INDENT,
							   "isc_%d\t: string (1..%d);\t-- %s --",
							   reference->ref_ident, field->fld_length, name);
				}
			}
			break;

		default:
			sprintf(s, "datatype %d unknown for field %s, msg %d",
					field->fld_dtype, name, port->por_msg_number);
			CPR_error(s);
			return;
		}
	}

	printa(column, "end record;");

	printa(column, "for isc_%dt use record at mod 4;", port->por_ident);

	pos = 0;
	for (reference = port->por_references; reference;
		 reference = reference->ref_next) 
	{
		field = reference->ref_field;
		if (reference->ref_value && field->fld_array_info)
			field = field->fld_array;
		printa(column + INDENT, "isc_%d at %d range 0..%d;",
			   reference->ref_ident, pos, 8 * field->fld_length - 1);
		pos += field->fld_length;
	}

	printa(column, "end record;");
}


//____________________________________________________________
//  
//		Generate the actual insertion text for a
//       ready;
//  

static void make_ready(DBB db,
					   TEXT * filename,
					   TEXT * vector,
					   USHORT column,
					   GPRE_REQ request)
{
	TEXT s1[32], s2[32];

	if (request) {
		sprintf(s1, "isc_%dl", request->req_ident);
		sprintf(s2, "isc_%d", request->req_ident);

		/* if the dpb needs to be extended at runtime to include items
		   in host variables, do so here; this assumes that there is 
		   always a request generated for runtime variables */

		if (request->req_flags & REQ_extend_dpb) {
			sprintf(s2, "isc_%dp", request->req_ident);
			if (request->req_length)
				printa(column, "%s = isc_%d;", s2, request->req_ident);
			if (db->dbb_r_user)
				printa(column,
					   "interbase.MODIFY_DPB (%s, %s, interbase.isc_dpb_user_name, %s, %d);\n",
					   s2, s1, db->dbb_r_user, (strlen(db->dbb_r_user) - 2));
			if (db->dbb_r_password)
				printa(column,
					   "interbase.MODIFY_DPB (%s, %s, interbase.isc_dpb_password, %s, %d);\n",
					   s2, s1, db->dbb_r_password,
					   (strlen(db->dbb_r_password) - 2));
			/*
			   ** ===========================================================
			   ** ==
			   ** ==   Process Role Name
			   ** ==
			   ** ===========================================================
			 */
			if (db->dbb_r_sql_role)
				printa(column,
					   "interbase.MODIFY_DPB (%s, %s, interbase.isc_dpb_sql_role_name, %s, %d);\n",
					   s2, s1, db->dbb_r_sql_role,
					   (strlen(db->dbb_r_sql_role) - 2));

			if (db->dbb_r_lc_messages)
				printa(column,
					   "interbase.MODIFY_DPB (%s, %s, interbase.isc_dpb_lc_messages, %s, %d);\n",
					   s2, s1, db->dbb_r_lc_messages,
					   (strlen(db->dbb_r_lc_messages) - 2));
			if (db->dbb_r_lc_ctype)
				printa(column,
					   "interbase.MODIFY_DPB (%s, %s, interbase.isc_dpb_lc_ctype, %s, %d);\n",
					   s2, s1, db->dbb_r_lc_ctype,
					   (strlen(db->dbb_r_lc_ctype) - 2));
		}
	}

	if (filename)
		if (*filename == '"')
			printa(column,
				   "interbase.ATTACH_DATABASE (%s %d, %s, %s%s, %s, %s);",
				   vector, strlen(filename) - 2, filename, ada_package,
				   db->dbb_name->sym_string, (request ? s1 : "0"),
				   (request ? s2 : "interbase.null_dpb"));
		else
			printa(column,
				   "interbase.ATTACH_DATABASE (%s %s'length(1), %s, %s%s, %s, %s);",
				   vector, filename, filename, ada_package,
				   db->dbb_name->sym_string, (request ? s1 : "0"),
				   (request ? s2 : "interbase.null_dpb"));
	else
		printa(column,
			   "interbase.ATTACH_DATABASE (%s %d, \"%s\", %s%s, %s, %s);",
			   vector, strlen(db->dbb_filename), db->dbb_filename,
			   ada_package, db->dbb_name->sym_string, (request ? s1 : "0"),
			   (request ? s2 : "interbase.null_dpb"));

	if (request && request->req_flags & REQ_extend_dpb) {
		if (request->req_length)
			printa(column, "if (%s != isc_%d)", s2, request->req_ident);
		printa(column + (request->req_length ? 4 : 0),
			   "interbase.FREE (%s);", s2);

		/* reset the length of the dpb */
		if (request->req_length)
			printa(column, "%s = %d;", s1, request->req_length);
	}
}


//____________________________________________________________
//  
//		Print a fixed string at a particular column.
//  

static void printa( int column, TEXT * string, ...)
{
	va_list ptr;

	VA_START(ptr, string);
	align(column);
	vsprintf(output_buffer, string, ptr);
	ADA_print_buffer(output_buffer, column);
}


//____________________________________________________________
//  
//		Generate the appropriate transaction handle.
//  

static const TEXT* request_trans( const act* action, GPRE_REQ request)
{
	if (action->act_type == ACT_open) {
		const TEXT* trname = ((OPN) action->act_object)->opn_trans;
		if (!trname)
			trname = "gds_trans";
		return trname;
	}
	else
		return (request) ? request->req_trans : (TEXT*) "gds_trans";
}


//____________________________________________________________
//  
//		Generate the appropriate status vector parameter for a gds
//		call depending on where or not the action has an error clause.
//  

static TEXT* status_vector( const act* action)
{
	if (action && (action->act_error || (action->act_flags & ACT_sql)))
		return "isc_status,";

	return "";
}


//____________________________________________________________
//  
//		Generate substitution text for START_TRANSACTION.
//  

static void t_start_auto(const act* action,
						 GPRE_REQ request,
						 TEXT * vector,
						 int column,
						 bool test)
{
	DBB db;
	int count, stat;
	TEXT *filename, buffer[256], temp[40];

	const TEXT* trname = request_trans(action, request);

	stat = !strcmp(vector, "isc_status");

	begin(column);

	if (sw_auto) {
		buffer[0] = 0;
		for (count = 0, db = isc_databases; db; db = db->dbb_next, count++)
			if ((filename = db->dbb_runtime) || !(db->dbb_flags & DBB_sqlca)) {
				align(column);
				ib_fprintf(out_file, "if (%s%s = 0", ada_package,
						   db->dbb_name->sym_string);
				if (stat && buffer[0])
					ib_fprintf(out_file, "and %s(1) = 0", vector);
				ib_fprintf(out_file, ") then");
				make_ready(db, filename, vector, column + INDENT, 0);
				endif(column);
				if (buffer[0])
					strcat(buffer, ") and (");
				sprintf(temp, "%s%s /= 0", ada_package,
						db->dbb_name->sym_string);
				strcat(buffer, temp);
			}
		if (!buffer[0])
			strcpy(buffer, "True");
		if (test)
			printa(column, "if (%s) and (%s%s = 0) then", buffer, ada_package,
				   trname);
		else
			printa(column, "if (%s) then", buffer);
		column += INDENT;
	}

	for (count = 0, db = isc_databases; db; db = db->dbb_next) {
		count++;
		printa(column, "isc_teb(%d).tpb_len:= 0;", count);
		printa(column, "isc_teb(%d).tpb_ptr := interbase.null_tpb'address;",
			   count);
		printa(column, "isc_teb(%d).dbb_ptr := %s%s'address;", count,
			   ada_package, db->dbb_name->sym_string);
	}

	printa(column, "interbase.start_multiple (%s %s%s, %d, isc_teb'address);",
		   vector, ada_package, trname, count);

	if (sw_auto) {
		endif(column);
		column -= INDENT;
	}

	endp(column);
}
