//____________________________________________________________
//  
//		PROGRAM:	C preprocess
//		MODULE:		int.cpp
//		DESCRIPTION:	Code generate for internal JRD modules
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
//  TMN (Mike Nordell) 11.APR.2001 - Reduce compiler warnings in generated code
//  
//
//____________________________________________________________
//
//	$Id: int_cxx.cpp,v 1.23 2003-11-08 16:31:40 brodsom Exp $
//

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include "../jrd/common.h"
#include <stdarg.h>
#include "../jrd/y_ref.h"
#include "../jrd/ibase.h"
#include "../gpre/gpre.h"
#include "../gpre/gpre_proto.h"
#include "../gpre/lang_proto.h"
#include "../jrd/gds_proto.h"

static void align(const int);
static void asgn_from(REF, int);
#ifdef NOT_USED_OR_REPLACED
static void asgn_to(REF);
#endif
static void gen_at_end(const act*, int);
static void gen_blr(void*, SSHORT, const char*);
static void gen_compile(const gpre_req*, int);
static void gen_database(const act*, int);
static void gen_emodify(const act*, int);
static void gen_estore(const act*, int, bool);
static void gen_endfor(const act*, int);
static void gen_erase(const act*, int);
static void gen_for(const act*, int);
static char* gen_name(char*, const ref*);
static void gen_raw(const gpre_req*);
static void gen_receive(const gpre_req*, const por*);
static void gen_request(const gpre_req*);
static void gen_routine(const act*, int);
static void gen_s_end(const act*, int);
static void gen_s_fetch(const act*, int);
static void gen_s_start(const act*, int);
static void gen_send(const gpre_req*, const por*, int, bool);
static void gen_start(const gpre_req*, const por*, int, bool);
static void gen_type(const act*, int);
static void gen_variable(const act*, int);
static void make_port(POR, int);
static void printa(const int, const TEXT*, ...);

static bool global_first_flag = false;

const int INDENT = 3;

static const char* const GDS_VTOV = "gds__vtov";
static const char* const JRD_VTOF = "jrd_vtof";
static const char* const VTO_CALL = "%s ((const char*)%s, (char*)%s, %d);";

static inline void begin(const int column)
{
	printa(column, "{");
}

static inline void endp(const int column)
{
	printa(column, "}");
}

//____________________________________________________________
//  
//  

void INT_CXX_action( const act* action, int column)
{

//  Put leading braces where required 

	switch (action->act_type) {
	case ACT_for:
	case ACT_insert:
	case ACT_modify:
	case ACT_store:
	case ACT_s_fetch:
	case ACT_s_start:
		begin(column);
		align(column);
	}

	switch (action->act_type) {
	case ACT_at_end:
		gen_at_end(action, column);
		return;
	case ACT_b_declare:
	case ACT_database:
		gen_database(action, column);
		return;
	case ACT_endfor:
		gen_endfor(action, column);
		break;
	case ACT_endmodify:
		gen_emodify(action, column);
		break;
	case ACT_endstore:
		gen_estore(action, column, false);
		break;
	case ACT_endstore_special:
		gen_estore(action, column, true);
		break;
	case ACT_erase:
		gen_erase(action, column);
		return;
	case ACT_for:
		gen_for(action, column);
		return;
	case ACT_hctef:
		break;
	case ACT_insert:
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
	case ACT_type_number:
		gen_type(action, column);
		return;
	case ACT_variable:
		gen_variable(action, column);
		return;
	default:
		return;
	}

//  Put in a trailing brace for those actions still with us 

	endp(column);
}


//____________________________________________________________
//  
//		Align output to a specific column for output.
//  

static void align(const int column)
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

static void asgn_from( REF reference, int column)
{
	TEXT* value;
	TEXT variable[20];
	TEXT temp[20];

	for (; reference; reference = reference->ref_next)
	{
		const GPRE_FLD field = reference->ref_field;
		align(column);
		gen_name(variable, reference);
		if (reference->ref_source) {
			value = gen_name(temp, reference->ref_source);
		} else {
			value = reference->ref_value;
		}

		/* To avoid chopping off a double byte kanji character in between
		   the two bytes, generate calls to gds__ftof2 gds$_vtof2,
		   gds$_vtov2 and jrd_vtof2 wherever necessary */

		if (!field || field->fld_dtype == dtype_text)
			ib_fprintf(out_file, VTO_CALL,
					   JRD_VTOF,
					   value, variable,
					   field ? field->fld_length : 0);
		else if (!field || field->fld_dtype == dtype_cstring)
			ib_fprintf(out_file, VTO_CALL,
					   GDS_VTOV,
					   value, variable,
					   field ? field->fld_length : 0);
		else
			ib_fprintf(out_file, "%s = %s;", variable, value);
	}

}


//____________________________________________________________
//  
//		Build an assignment to a host language variable from
//		a port variable.
//  
#ifdef NOT_USED_OR_REPLACED
static void asgn_to( REF reference)
{
	TEXT s[20];

	REF source = reference->ref_friend;
	GPRE_FLD field = source->ref_field;
	gen_name(s, source);

	// Repeated later down in function gen_emodify, but then
	// emitting jrd_ftof call.

	if (!field || field->fld_dtype == dtype_text)
		ib_fprintf(out_file, "gds__ftov (%s, %d, %s, sizeof (%s));",
				   s,
				   field ? field->fld_length : 0,
				   reference->ref_value, reference->ref_value);
	else if (!field || field->fld_dtype == dtype_cstring)
		ib_fprintf(out_file, "gds__vtov((const char*)%s, (char*)%s, sizeof (%s));",
				   s, reference->ref_value, reference->ref_value);
	else
		ib_fprintf(out_file, "%s = %s;", reference->ref_value, s);
}
#endif

//____________________________________________________________
//  
//		Generate code for AT END clause of FETCH.
//  

static void gen_at_end( const act* action, int column)
{
	TEXT s[20];

	const gpre_req* request = action->act_request;
	printa(column, "if (!%s) ", gen_name(s, request->req_eof));
}


//____________________________________________________________
//  
//		Callback routine for BLR pretty printer.
//  

static void gen_blr(void* user_arg, SSHORT offset, const char* string)
{
	ib_fprintf(out_file, "%s\n", string);
}


//____________________________________________________________
//  
//		Generate text to compile a request.
//  

static void gen_compile( const gpre_req* request, int column)
{
	column += INDENT;
	const dbb* db = request->req_database;
	const sym* symbol = db->dbb_name;
	ib_fprintf(out_file, "if (!%s)", request->req_handle);
	align(column);
	ib_fprintf(out_file,
			   "%s = (BLK) CMP_compile2 (tdbb, (UCHAR*)jrd_%d, TRUE);",
			   request->req_handle, request->req_ident);
}


//____________________________________________________________
//  
//		Generate insertion text for the database statement.
//  

static void gen_database( const act* action, int column)
{
	if (global_first_flag)
		return;
	global_first_flag = true;

	align(0);
	for (const gpre_req* request = requests; request; request = request->req_next)
		gen_request(request);
}


//____________________________________________________________
//  
//		Generate substitution text for END_MODIFY.
//  

static void gen_emodify( const act* action, int column)
{
	TEXT s1[20], s2[20];

	const upd* modify = (UPD) action->act_object;

	for (const ref* reference = modify->upd_port->por_references; reference;
		 reference = reference->ref_next)
	{
		const ref* source = reference->ref_source;
		if (!source) {
			continue;
		}
		const gpre_fld* field = reference->ref_field;
		align(column);

		if (field->fld_dtype == dtype_text)
			ib_fprintf(out_file, "jrd_ftof (%s, %d, %s, %d);",
					   gen_name(s1, source),
					   field->fld_length,
					   gen_name(s2, reference), field->fld_length);
		else if (field->fld_dtype == dtype_cstring)
			ib_fprintf(out_file, "gds__vtov((const char*)%s, (char*)%s, %d);",
					   gen_name(s1, source),
					   gen_name(s2, reference), field->fld_length);
		else
			ib_fprintf(out_file, "%s = %s;",
					   gen_name(s1, reference), gen_name(s2, source));
	}

	gen_send(action->act_request, modify->upd_port, column, false);
}


//____________________________________________________________
//  
//		Generate substitution text for END_STORE.
//  

static void gen_estore( const act* action, int column, bool special)
{
	const gpre_req* request = action->act_request;
	align(column);
	gen_compile(request, column);
	gen_start(request, request->req_primary, column, special);
}


//____________________________________________________________
//  
//		Generate definitions associated with a single request.
//  

static void gen_endfor( const act* action, int column)
{
	const gpre_req* request = action->act_request;
	column += INDENT;

	if (request->req_sync)
		gen_send(request, request->req_sync, column, false);

	endp(column);
}


//____________________________________________________________
//  
//		Generate substitution text for ERASE.
//  

static void gen_erase( const act* action, int column)
{
	UPD erase = (UPD) action->act_object;
	gen_send(erase->upd_request, erase->upd_port, column, false);
}


//____________________________________________________________
//  
//		Generate substitution text for FOR statement.
//  

static void gen_for( const act* action, int column)
{
	TEXT s[20];

	gen_s_start(action, column);

	align(column);
	const gpre_req* request = action->act_request;
	ib_fprintf(out_file, "while (1)");
	column += INDENT;
	begin(column);
	align(column);
	gen_receive(action->act_request, request->req_primary);
	align(column);
	ib_fprintf(out_file, "if (!%s) break;", gen_name(s, request->req_eof));
}


//____________________________________________________________
//  
//		Generate a name for a reference.  Name is constructed from
//		port and parameter idents.
//  

static char* gen_name(char* string, const ref* reference)
{

	sprintf(string, "jrd_%d.jrd_%d",
			reference->ref_port->por_ident, reference->ref_ident);

	return string;
}


//____________________________________________________________
//  
//		Generate BLR in raw, numeric form.  Ugly but dense.
//  

static void gen_raw( const gpre_req* request)
{
	TEXT buffer[80];

	const UCHAR* blr = request->req_blr;
	int blr_length = request->req_length;
	TEXT* p = buffer;
	align(0);

	while (--blr_length) {
		const UCHAR c = *blr++;
		if ((c >= 'A' && c <= 'Z') || c == '$' || c == '_')
			sprintf(p, "'%c',", c);
		else
			sprintf(p, "%d,", c);
		while (*p)
			p++;
		if (p - buffer > 60) {
			ib_fprintf(out_file, "%s\n", buffer);
			p = buffer;
			*p = 0;
		}
	}

	ib_fprintf(out_file, "%s%d", buffer, blr_eoc);
}


//____________________________________________________________
//  
//		Generate a send or receive call for a port.
//  

static void gen_receive( const gpre_req* request, const por* port)
{

	ib_fprintf(out_file,
			   "EXE_receive (tdbb, (JRD_REQ)%s, %d, %d, (UCHAR*)&jrd_%d);",
			   request->req_handle, port->por_msg_number, port->por_length,
			   port->por_ident);
}


//____________________________________________________________
//  
//		Generate definitions associated with a single request.
//  

static void gen_request( const gpre_req* request)
{

	if (!(request->req_flags & REQ_exp_hand))
		ib_fprintf(out_file, "static void\t*%s;\t/* request handle */\n",
				   request->req_handle);

	ib_fprintf(out_file, "static const UCHAR\tjrd_%d [%d] =",
			   request->req_ident, request->req_length);
	align(INDENT);
	ib_fprintf(out_file, "{\t/* blr string */\n");

	if (sw_raw)
		gen_raw(request);
	else
		gds__print_blr(request->req_blr, gen_blr, 0, 0);

	printa(INDENT, "};\t/* end of blr string */\n");
}


//____________________________________________________________
//  
//		Process routine head.  If there are requests in the
//		routine, insert local definitions.
//  

static void gen_routine( const act* action, int column)
{
	for (const gpre_req* request = (GPRE_REQ) action->act_object; request;
		 request = request->req_routine) 
	{
		for (POR port = request->req_ports; port; port = port->por_next)
			make_port(port, column + INDENT);
	}
}


//____________________________________________________________
//  
//		Generate substitution text for END_STREAM.
//  

static void gen_s_end( const act* action, int column)
{
	const gpre_req* request = action->act_request;
	printa(column, "EXE_unwind (tdbb, %s);", request->req_handle);
}


//____________________________________________________________
//  
//		Generate substitution text for FETCH.
//  

static void gen_s_fetch( const act* action, int column)
{
	const gpre_req* request = action->act_request;
	if (request->req_sync)
		gen_send(request, request->req_sync, column, false);

	gen_receive(action->act_request, request->req_primary);
}


//____________________________________________________________
//  
//		Generate text to compile and start a stream.  This is
//		used both by START_STREAM and FOR
//  

static void gen_s_start( const act* action, int column)
{
	const gpre_req* request = action->act_request;
	gen_compile(request, column);

    const por* port = request->req_vport;
	if (port)
		asgn_from(port->por_references, column);

	gen_start(request, port, column, false);
}


//____________________________________________________________
//  
//		Generate a send or receive call for a port.
//  

static void gen_send( const gpre_req* request, const por* port, int column, bool special)
{
	if (special) {
		align(column);
		ib_fprintf(out_file, "if (ignore_perm)");
		align(column);
		ib_fprintf(out_file, "\t((JRD_REQ)request)->req_flags |= req_ignore_perm;"); 
	}
	align(column);

	ib_fprintf(out_file, "EXE_send (tdbb, (JRD_REQ)%s, %d, %d, (UCHAR*)&jrd_%d);",
			   request->req_handle,
			   port->por_msg_number, port->por_length, port->por_ident);
}


//____________________________________________________________
//  
//		Generate a START.
//  

static void gen_start( const gpre_req* request, const por* port, int column, bool special)
{
	align(column);

	ib_fprintf(out_file, "EXE_start (tdbb, (JRD_REQ)%s, %s);",
			   request->req_handle, request->req_trans);

	if (port)
		gen_send(request, port, column, special);
}


//____________________________________________________________
//  
//		Substitute for a variable reference.
//  

static void gen_type( const act* action, int column)
{

	printa(column, "%ld", action->act_object);
}


//____________________________________________________________
//  
//		Substitute for a variable reference.
//  

static void gen_variable( const act* action, int column)
{
	char s[20];

	align(column);
	ib_fprintf(out_file, gen_name(s, action->act_object));

}


//____________________________________________________________
//  
//		Insert a port record description in output.
//  

static void make_port( POR port, int column)
{
	printa(column, "struct {");

	for (REF reference = port->por_references; reference;
		 reference = reference->ref_next)
	{
		align(column + INDENT);
		GPRE_FLD field = reference->ref_field;
		SYM symbol = field->fld_symbol;
		const TEXT* name = symbol->sym_string;

		switch (field->fld_dtype) {
		case dtype_short:
			ib_fprintf(out_file, "    SSHORT jrd_%d;\t/* %s */",
					   reference->ref_ident, name);
			break;

		case dtype_long:
			ib_fprintf(out_file, "    SLONG  jrd_%d;\t/* %s */",
					   reference->ref_ident, name);
			break;

// ** Begin sql date/time/timestamp *
		case dtype_sql_date:
			ib_fprintf(out_file, "    ISC_DATE  jrd_%d;\t/* %s */",
					   reference->ref_ident, name);
			break;

		case dtype_sql_time:
			ib_fprintf(out_file, "    ISC_TIME  jrd_%d;\t/* %s */",
					   reference->ref_ident, name);
			break;

		case dtype_timestamp:
			ib_fprintf(out_file, "    ISC_TIMESTAMP  jrd_%d;\t/* %s */",
					   reference->ref_ident, name);
			break;
// ** End sql date/time/timestamp *

		case dtype_int64:
			ib_fprintf(out_file, "    ISC_INT64  jrd_%d;\t/* %s */",
					   reference->ref_ident, name);
			break;

		case dtype_quad:
		case dtype_blob:
			ib_fprintf(out_file, "    GDS_QUAD  jrd_%d;\t/* %s */",
					   reference->ref_ident, name);
			break;

		case dtype_cstring:
		case dtype_text:
			ib_fprintf(out_file, "    TEXT  jrd_%d [%d];\t/* %s */",
					   reference->ref_ident, field->fld_length, name);
			break;

		case dtype_real:
			ib_fprintf(out_file, "    float  jrd_%d;\t/* %s */",
					   reference->ref_ident, name);
			break;

		case dtype_double:
			ib_fprintf(out_file, "    double  jrd_%d;\t/* %s */",
					   reference->ref_ident, name);
			break;

		default:
			{
				TEXT s[80];
				sprintf(s, "datatype %d unknown for field %s, msg %d",
						field->fld_dtype, name, port->por_msg_number);
				CPR_error(s);
				return;
			}
		}
	}
	align(column);
	ib_fprintf(out_file, "} jrd_%d;", port->por_ident);
}


//____________________________________________________________
//  
//		Print a fixed string at a particular column.
//  

static void printa(const int column, const TEXT* string, ...)
{
	va_list ptr;

	VA_START(ptr, string);
	align(column);
	ib_vfprintf(out_file, string, ptr);
}

