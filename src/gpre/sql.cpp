//____________________________________________________________
//  
//		PROGRAM:	C Preprocessor
//		MODULE:		sql.cpp
//		DESCRIPTION:	SQL parser
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
//  TMN (Mike Nordell) 11.APR.2001 - Reduce compiler warnings
//  
//
//____________________________________________________________
//
//	$Id: sql.cpp,v 1.17 2003-09-06 00:52:10 brodsom Exp $
//

#include "firebird.h"
#include <stdlib.h>
#include <string.h>
#include "../gpre/gpre.h"
#include "../jrd/gds.h"
#include "../gpre/parse.h"
#include "../jrd/intl.h"
#include "../wal/wal.h"
#include "../jrd/constants.h"
#include "../gpre/cme_proto.h"
#include "../gpre/cmp_proto.h"
#include "../gpre/exp_proto.h"
#include "../gpre/gpre_proto.h"
#include "../gpre/hsh_proto.h"
#include "../gpre/gpre_meta.h"
#include "../gpre/msc_proto.h"
#include "../gpre/par_proto.h"
#include "../gpre/sqe_proto.h"
#include "../gpre/sql_proto.h"


#define ERROR_LENGTH			128
#ifdef FLINT_CACHE
#define MIN_CACHE_BUFFERS		250
#define DEF_CACHE_BUFFERS		1000
#endif
#define DEFAULT_BLOB_SEGMENT_LENGTH	80	/* bytes */

extern ACT cur_routine;
extern TEXT *database_name;

TEXT *module_lc_ctype = NULL;

static ACT act_alter(void);
static ACT act_alter_database(void);
static ACT act_alter_domain(void);
static ACT act_alter_index(void);
static ACT act_alter_table(void);
static ACT act_comment(void);
static ACT act_connect(void);
static ACT act_create(void);
static ACT act_create_database(void);
static ACT act_create_domain(void);
static ACT act_create_generator(void);
static ACT act_create_index(SSHORT, BOOLEAN);
static ACT act_create_shadow(void);
static ACT act_create_table(void);
static ACT act_create_view(void);
static ACT act_d_section(enum act_t);
static ACT act_declare(void);
static ACT act_declare_filter(void);
static ACT act_declare_table(SYM, DBB);
static ACT act_declare_udf(void);
static ACT act_delete(void);
static ACT act_describe(void);
static ACT act_disconnect(void);
static ACT act_drop(void);
static ACT act_event(void);
static ACT act_execute(void);
static ACT act_fetch(void);
static ACT act_grant_revoke(enum act_t);
static ACT act_include(void);
static ACT act_insert(void);
static ACT act_insert_blob(TEXT *);
static ACT act_lock(void);
static ACT act_openclose(enum act_t);
static ACT act_open_blob(ACT_T, SYM);
static ACT act_prepare(void);
static ACT act_procedure(void);
static ACT act_select(void);
static ACT act_set(TEXT*);
static ACT act_set_dialect(void);
static ACT act_set_generator(void);
static ACT act_set_names(void);
static ACT act_set_statistics(void);
static ACT act_set_transaction(void);
static ACT act_transaction(enum act_t);
static ACT act_update(void);
static ACT act_whenever(void);
static BOOLEAN check_filename(TEXT *);
static void connect_opts(TEXT **, TEXT **, TEXT **, TEXT **, USHORT *);
#ifdef FLINT_CACHE
static FIL define_cache(void);
#endif
static FIL define_file(void);
static FIL define_log_file(BOOLEAN);
static DBB dup_dbb(DBB);
static void error(TEXT *, TEXT *);
static TEXT *extract_string(BOOLEAN);
static SWE gen_whenever(void);
static void into(GPRE_REQ, GPRE_NOD, GPRE_NOD);
static GPRE_FLD make_field(GPRE_REL);
static IND make_index(GPRE_REQ, TEXT *);
static GPRE_REL make_relation(GPRE_REQ, TEXT *);
static void pair(GPRE_NOD, GPRE_NOD);
static void par_array(GPRE_FLD);
static SSHORT par_char_set(void);
static void par_computed(GPRE_REQ, GPRE_FLD);
static GPRE_REQ par_cursor(SYM *);
static DYN par_dynamic_cursor(void);
static GPRE_FLD par_field(GPRE_REQ, GPRE_REL);
static CNSTRT par_field_constraint(GPRE_REQ, GPRE_FLD, GPRE_REL);
static void par_fkey_extension(CNSTRT);
static BOOLEAN par_into(DYN);
static void par_options(TEXT **);
static int par_page_size(void);
static GPRE_REL par_relation(GPRE_REQ);
static DYN par_statement(void);
static CNSTRT par_table_constraint(GPRE_REQ, GPRE_REL);
static BOOLEAN par_transaction_modes(GPRE_TRA, BOOLEAN);
static BOOLEAN par_using(DYN);
static USHORT resolve_dtypes(KWWORDS, BOOLEAN);
static BOOLEAN tail_database(enum act_t, DBB);
static void to_upcase(TEXT *, TEXT *);
static void dialect1_bad_type(USHORT);

static SWE whenever[SWE_max], whenever_list;

#define END_OF_COMMAND											\
	(((sw_language != lang_cobol) &&							\
		((int) token.tok_keyword == (int) KW_SEMI_COLON)) ||	\
		 ((sw_language == lang_cobol) &&						\
		 ((int) token.tok_keyword == (int) KW_END_EXEC)))


//____________________________________________________________
//  
//		Parse and return a sequel action.
//  

ACT SQL_action(TEXT * base_directory)
{
	ACT action;
	enum kwwords keyword;

	sw_gen_sql = TRUE;

	switch (keyword = token.tok_keyword) {
	case KW_ALTER:
	case KW_COMMENT:
	case KW_CONNECT:
	case KW_CREATE:
	case KW_DROP:
	case KW_EVENT:
	case KW_GRANT:
	case KW_REVOKE:
	case KW_BEGIN:
	case KW_CLOSE:
	case KW_COMMIT:
	case KW_DECLARE:
	case KW_DELETE:
	case KW_DESCRIBE:
	case KW_DISCONNECT:
	case KW_END:
	case KW_EXECUTE:
	case KW_FETCH:
	case KW_INCLUDE:
	case KW_INSERT:
	case KW_LOCK:
	case KW_OPEN:
	case KW_PREPARE:
	case KW_ROLLBACK:
	case KW_SELECT:
	case KW_SET:
	case KW_UPDATE:
	case KW_WHENEVER:
	case KW_DATABASE:
		ADVANCE_TOKEN;
		break;

	default:
		SYNTAX_ERROR("SQL operation");
	}
	switch (keyword) {
	case KW_ALTER:
		action = act_alter();
		break;

	case KW_BEGIN:
		action = act_d_section(ACT_b_declare);
		break;

	case KW_CLOSE:
		action = act_openclose(ACT_close);
		break;

	case KW_CONNECT:
		action = act_connect();
		break;

	case KW_COMMENT:
		action = act_comment();
		break;

	case KW_COMMIT:
		action = act_transaction(ACT_commit);
		break;

	case KW_CREATE:
		action = act_create();
		break;

	case KW_DATABASE:
		action = PAR_database((USHORT) TRUE, base_directory);
		break;

	case KW_DROP:
		action = act_drop();
		break;

	case KW_DECLARE:
		action = act_declare();
		break;

	case KW_DELETE:
		action = act_delete();
		break;

	case KW_DESCRIBE:
		action = act_describe();
		break;

	case KW_DISCONNECT:
		action = act_disconnect();
		break;

	case KW_EVENT:
		action = act_event();
		break;

	case KW_END:
		action = act_d_section(ACT_e_declare);
		break;

	case KW_EXECUTE:
		action = act_execute();
		break;

	case KW_FETCH:
		action = act_fetch();
		break;

	case KW_GRANT:
		action = act_grant_revoke(ACT_dyn_grant);
		break;

	case KW_INCLUDE:
		action = act_include();
		break;

	case KW_INSERT:
		action = act_insert();
		break;

	case KW_LOCK:
		action = act_lock();
		break;

	case KW_OPEN:
		action = act_openclose(ACT_open);
		break;

	case KW_PREPARE:
		action = act_prepare();
		break;

	case KW_REVOKE:
		action = act_grant_revoke(ACT_dyn_revoke);
		break;

	case KW_ROLLBACK:
		action = act_transaction(ACT_rollback);
		break;

	case KW_SELECT:
		action = act_select();
		break;

	case KW_SET:
		action = act_set(base_directory);
		break;

	case KW_UPDATE:
		action = act_update();
		break;

	case KW_WHENEVER:
		action = act_whenever();
		break;
	}

	MATCH(KW_END_EXEC);
	PAR_end();
	action->act_flags |= ACT_sql;

	return action;
}


//____________________________________________________________
//  
//		Given a field datatype, remap it as needed to
//		a user datatype, and set the length field.
//  

void SQL_adjust_field_dtype( GPRE_FLD field)
{
	ULONG field_length;


	if (field->fld_dtype <= dtype_any_text) {
		/* Adjust the string data types and their lengths */
		if (field->fld_collate) {
			if (field->fld_char_length)
				field_length = (ULONG) field->fld_char_length *
					field->fld_collate->intlsym_bytes_per_char;
			else
				field_length = field->fld_length;
			field->fld_collate_id = field->fld_collate->intlsym_collate_id;
			field->fld_charset_id = field->fld_collate->intlsym_charset_id;
			field->fld_ttype = field->fld_collate->intlsym_ttype;
		}
		else if (field->fld_character_set) {
			if (field->fld_char_length)
				field_length = (ULONG) field->fld_char_length *
					field->fld_character_set->intlsym_bytes_per_char;
			else
				field_length = field->fld_length;
			field->fld_collate_id =
				field->fld_character_set->intlsym_collate_id;
			field->fld_charset_id =
				field->fld_character_set->intlsym_charset_id;
			field->fld_ttype = field->fld_character_set->intlsym_ttype;
		}
		else {
			if (field->fld_char_length)
				field_length = (ULONG) field->fld_char_length * 1;
			else
				field_length = field->fld_length;
			field->fld_collate_id = 0;
			field->fld_charset_id = 0;
			field->fld_ttype = 0;
		}

		if (!(field->fld_flags & FLD_meta)) {	/* field for meta operation? */
			/* Field isn't for meta-data operation, so adjust it's
			 * type definition for local use
			 */
			if (field->fld_dtype != dtype_cstring)
				field->fld_dtype = (sw_cstring
									&& !FIELD_ALLOWS_NULLS(field)) ?
					dtype_cstring : dtype_text;
			if (field->fld_dtype == dtype_cstring)
				field_length++;
			field->fld_length = (USHORT) field_length;
		}
		else {
			field->fld_length = (USHORT) field_length;
			if (field->fld_dtype == dtype_varying)
				field_length += sizeof(USHORT);

			if (field_length > MAX_COLUMN_SIZE)
				error("Size of column %s exceeds implementation limit",
					  field->fld_symbol->sym_string);
		}
	}
	else
		switch (field->fld_dtype) {
		case dtype_short:
			field->fld_length = sizeof(SSHORT);
			break;

		case dtype_long:
			field->fld_length = sizeof(SLONG);
			break;

		case dtype_float:
			field->fld_length = sizeof(float);
			break;

		case dtype_double:
			field->fld_length = sizeof(double);
			break;

// ** Begin sql/date/time/timestamp *
		case dtype_sql_date:
			field->fld_length = sizeof(ISC_DATE);
			break;

		case dtype_sql_time:
			field->fld_length = sizeof(ISC_TIME);
			break;

		case dtype_timestamp:
			field->fld_length = sizeof(ISC_TIMESTAMP);
			break;
// ** End sql/date/time/timestamp *

		case dtype_int64:
			field->fld_length = sizeof(ISC_INT64);
			break;

		case dtype_blob:
			field->fld_length = sizeof(GDS_QUAD);
			field->fld_flags |= FLD_blob;
			if (field->fld_character_set) {
				field->fld_charset_id =
					field->fld_character_set->intlsym_charset_id;
				field->fld_ttype = field->fld_character_set->intlsym_ttype;
			}
			break;

		default:
			CPR_bugcheck("datatype not recognized");
			break;
		}
}


//____________________________________________________________
//  
//		Initialize (or re-initialize) to process a module.
//  

void SQL_init(void)
{
	USHORT i;

	whenever_list = NULL;

	for (i = 0; i < SWE_max; i++)
		whenever[i] = NULL;

	module_lc_ctype = default_lc_ctype;
}


//____________________________________________________________
//  
//  

void SQL_par_field_collate( GPRE_REQ request, GPRE_FLD field)
{
	SYM symbol;

	if (MATCH(KW_COLLATE)) {

		if ((field->fld_dtype != dtype_text) &&
			(field->fld_dtype != dtype_cstring) &&
			(field->fld_dtype != dtype_varying))
				PAR_error("COLLATE applies only to character columns");
		if (token.tok_type != tok_ident)
			SYNTAX_ERROR("<collation name>");
		if (!(symbol = MSC_find_symbol(token.tok_symbol, SYM_collate)))
			PAR_error("The named COLLATION was not found");
		field->fld_collate = (INTLSYM) symbol->sym_object;

		/* Is the collation valid for declared character set? 
		 * The character set is either declared (fld_character_set) or inferered
		 * from the global domain (fld_global & fld_charset_id)
		 */

		if ((field->fld_character_set &&
			 (field->fld_character_set->intlsym_charset_id !=
			  field->fld_collate->intlsym_charset_id))
			|| (field->fld_global &&
				(field->fld_charset_id !=
				 field->fld_collate->
				 intlsym_charset_id)))
				PAR_error
				("Specified COLLATION is incompatible with column CHARACTER SET");
		ADVANCE_TOKEN;
	}
}


//____________________________________________________________
//  
//		Handle an SQL field datatype definition for
//		field CREATE, DECLARE or ALTER TABLE statement.
//		Also for CAST statement
//  

void SQL_par_field_dtype( GPRE_REQ request, GPRE_FLD field, BOOLEAN udf)
{
	int l, p, q;
	enum kwwords keyword;
	SYM symbol;
	char s[ERROR_LENGTH];
	BOOLEAN sql_date = FALSE;

	switch (keyword = token.tok_keyword) {
	case KW_SMALLINT:
	case KW_INT:
	case KW_INTEGER:
	case KW_FLOAT:
	case KW_REAL:
	case KW_DOUBLE:
	case KW_LONG:
// ** Begin sql/date/time/timestamp *
	case KW_TIMESTAMP:
// ** End sql/date/time/timestamp *
	case KW_CHAR:
	case KW_NCHAR:
	case KW_VARCHAR:
	case KW_DECIMAL:
	case KW_NUMERIC:
	case KW_BLOB:
	case KW_NATIONAL:
		ADVANCE_TOKEN;
		break;

	case KW_DATE:
		if (sw_sql_dialect == 2)
			PAR_error
				("DATE is ambiguous in dialect 2 use SQL DATE or TIMESTAMP");
		ADVANCE_TOKEN;
		break;

	case KW_TIME:
		if (sw_sql_dialect == 1)
			SYNTAX_ERROR("<data type>");
		ADVANCE_TOKEN;
		break;

	case KW_SQL:
		if (sw_sql_dialect == 1)
			SYNTAX_ERROR("<data type>");
		ADVANCE_TOKEN;
		if (token.tok_keyword == KW_DATE)
			ADVANCE_TOKEN;
		else
			SYNTAX_ERROR("<data type>");
		keyword = KW_DATE;
		sql_date = TRUE;
		break;

	case KW_COMPUTED:
		if (udf)
			SYNTAX_ERROR("<data type>");
		/* just return - actual parse is done later */
		return;

	default:
		if (udf) {
			if (keyword == KW_CSTRING)
				ADVANCE_TOKEN;
			else
				SYNTAX_ERROR("<data type>");
		}
		else {
			SQL_resolve_identifier("<domain name>", s);
			field->fld_global = symbol =
				MSC_symbol(SYM_field, s, (USHORT) strlen(s), (GPRE_CTX) field);
			if (!MET_domain_lookup(request, field, s))
				PAR_error("Specified DOMAIN or source column not found");
			ADVANCE_TOKEN;
			return;
		}
	}

	switch (keyword) {
	case KW_SMALLINT:
		field->fld_dtype = dtype_short;
		break;

	case KW_INT:
	case KW_INTEGER:
		field->fld_dtype = dtype_long;
		break;

	case KW_REAL:
		field->fld_dtype = dtype_float;
		break;

	case KW_FLOAT:
		if (MATCH(KW_LEFT_PAREN)) {
			l = EXP_USHORT_ordinal(TRUE);
			EXP_match_paren();
			if (l < 17)
				field->fld_dtype = dtype_float;
			else
				field->fld_dtype = dtype_double;
		}
		else
			field->fld_dtype = dtype_float;
		break;

	case KW_LONG:
		if (!KEYWORD(KW_FLOAT))
			SYNTAX_ERROR("FLOAT");
		ADVANCE_TOKEN;
		field->fld_dtype = dtype_double;
		break;

	case KW_DOUBLE:
		if (!KEYWORD(KW_PRECISION))
			SYNTAX_ERROR("PRECISION");
		ADVANCE_TOKEN;
		field->fld_dtype = dtype_double;
		break;

// ** Begin sql/date/time/timestamp *
	case KW_DATE:
	case KW_TIME:
	case KW_TIMESTAMP:
		field->fld_dtype = resolve_dtypes(keyword, sql_date);
		break;
// ** End sql/date/time/timestamp *

	case KW_NCHAR:
		field->fld_flags |= FLD_national;
		field->fld_dtype = dtype_text;
		if (MATCH(KW_LEFT_PAREN)) {
			field->fld_char_length = EXP_pos_USHORT_ordinal(TRUE);
			EXP_match_paren();
		}
		else
			field->fld_char_length = 1;
		break;

	case KW_NATIONAL:
		if (!KEYWORD(KW_CHAR))
			SYNTAX_ERROR("CHARACTER");
		ADVANCE_TOKEN;
		field->fld_flags |= FLD_national;
		/* Fall into KW_CHAR */
	case KW_CHAR:
		if (MATCH(KW_VARYING)) {
			field->fld_dtype = dtype_varying;
			EXP_left_paren(0);
			field->fld_char_length = EXP_pos_USHORT_ordinal(TRUE);
			EXP_match_paren();
			break;
		}
	case KW_CSTRING:
		if (keyword == KW_CSTRING) {
			field->fld_dtype = dtype_cstring;
			field->fld_flags |= FLD_meta_cstring;
		}
		else
			field->fld_dtype = dtype_text;
		if (MATCH(KW_LEFT_PAREN)) {
			field->fld_char_length = EXP_pos_USHORT_ordinal(TRUE);
			EXP_match_paren();
		}
		else
			field->fld_char_length = 1;
		break;

	case KW_VARCHAR:
		field->fld_dtype = dtype_varying;
		EXP_left_paren(0);
		field->fld_char_length = EXP_pos_USHORT_ordinal(TRUE);
		EXP_match_paren();
		break;


	case KW_NUMERIC:
	case KW_DECIMAL:
		field->fld_dtype = dtype_long;
		field->fld_scale = 0;
		field->fld_precision = 9;
		field->fld_sub_type = (keyword == KW_NUMERIC) ? 1 : 2;
		if (MATCH(KW_LEFT_PAREN)) {
			p = EXP_USHORT_ordinal(TRUE);
			if ((p <= 0) || (p > 18))
				PAR_error("Precision must be from 1 to 18");

			if ((keyword == KW_NUMERIC) && (p < 5))
				field->fld_dtype = dtype_short;
			else if (p > 9) {
				if (sw_sql_dialect == SQL_DIALECT_V5)
					field->fld_dtype = dtype_double;
				else
					field->fld_dtype = dtype_int64;
			}

			if (MATCH(KW_COMMA)) {
				q = EXP_USHORT_ordinal(TRUE);

				if (q > p)
					PAR_error("Scale can not be greater than precision");
				field->fld_scale = -q;
			}
			field->fld_precision = p;
			EXP_match_paren();
		}
		break;

	case KW_BLOB:
		field->fld_dtype = dtype_blob;
		field->fld_seg_length = DEFAULT_BLOB_SEGMENT_LENGTH;
		if (MATCH(KW_LEFT_PAREN)) {
			if (MATCH(KW_COMMA))
				field->fld_sub_type = EXP_SSHORT_ordinal(TRUE);
			else {
				field->fld_seg_length = EXP_USHORT_ordinal(TRUE);
				if (MATCH(KW_COMMA))
					field->fld_sub_type = EXP_SSHORT_ordinal(TRUE);
			}
			EXP_match_paren();
		}
		else {
			if (MATCH(KW_SUB_TYPE)) {
				field->fld_sub_type = PAR_blob_subtype(request->req_database);
			}
			if (MATCH(KW_SEGMENT)) {
				MATCH(KW_SIZE);
				field->fld_seg_length = EXP_USHORT_ordinal(TRUE);
			}
		}
		break;

	default:
		SYNTAX_ERROR("<data type>");
	}

//   Check for array declaration 

	if ((keyword != KW_BLOB) && !udf && (MATCH(KW_L_BRCKET))) {
		field->fld_array_info = (ary*) ALLOC(sizeof(ary));
		par_array(field);
	}

	if (MATCH(KW_CHAR)) {
		SYM symbol;
		if ((field->fld_dtype != dtype_text) &&
			(field->fld_dtype != dtype_cstring) &&
			(field->fld_dtype != dtype_varying) &&
			(field->fld_dtype != dtype_blob))
				PAR_error("CHARACTER SET applies only to character columns");

		if (field->fld_dtype == dtype_blob
			&& field->fld_sub_type == BLOB_untyped) field->fld_sub_type =
				BLOB_text;

		if (field->fld_dtype == dtype_blob
			&& field->fld_sub_type !=
			BLOB_text)
PAR_error("CHARACTER SET applies only to character columns");

		if (field->fld_flags & FLD_national)
			PAR_error("cannot specify CHARACTER SET with NATIONAL");

		if (!MATCH(KW_SET))
			SYNTAX_ERROR("SET");
		if (token.tok_type != tok_ident)
			SYNTAX_ERROR("<character set name>");
		if (!(symbol = MSC_find_symbol(token.tok_symbol, SYM_charset)))
			PAR_error("The named CHARACTER SET was not found");
		field->fld_character_set = (INTLSYM) symbol->sym_object;
		ADVANCE_TOKEN;
	}

	if (field->fld_flags & FLD_national) {
		if (!
			(symbol =
			 MSC_find_symbol(HSH_lookup(DEFAULT_CHARACTER_SET_NAME),
							 SYM_charset)))
	   PAR_error("NATIONAL character set missing");
		field->fld_character_set = (INTLSYM) symbol->sym_object;
	}
	else if ((field->fld_dtype <= dtype_any_text ||
			  (field->fld_dtype == dtype_blob
			   && field->fld_sub_type == BLOB_text))
			 && !field->fld_character_set && !field->fld_collate && request
			 && request->req_database
			 && request->req_database->dbb_def_charset) {
		/* Use database default character set */
		if (symbol =
			MSC_find_symbol(HSH_lookup
							(request->req_database->dbb_def_charset),
							SYM_charset)) field->fld_character_set =
				(INTLSYM) symbol->sym_object;
		else
			PAR_error("Could not find database default character set");
	}
}


//____________________________________________________________
//  
//		Find procedure for request.  If request already has a database,
//		find the procedure in that database only.
//  

GPRE_PRC SQL_procedure(GPRE_REQ request,
				  TEXT * prc_string,
				  TEXT * db_string, TEXT * owner_string, BOOLEAN err_flag)
{
	DBB db;
	GPRE_PRC procedure, tmp_procedure;
	SYM symbol;
	SCHAR s[ERROR_LENGTH];

	procedure = NULL;

	if (db_string && db_string[0]) {
		/* a database was specified for the procedure
		   search the known symbols for the database name */

		if (!(symbol = MSC_find_symbol(HSH_lookup(db_string), SYM_database)))
			PAR_error("Unknown database specifier.");
		if (request->req_database) {
			if ((DBB) symbol->sym_object != request->req_database)
				PAR_error("Inconsistent database specifier");
		}
		else
			request->req_database = (DBB) symbol->sym_object;
	}

	if (request->req_database)
		procedure =
			MET_get_procedure(request->req_database, prc_string,
							  owner_string);
	else {
		/* no database was specified, check the metadata for all the databases
		   for the existence of the procedure */

		procedure = NULL;
		for (db = isc_databases; db; db = db->dbb_next)
			if (tmp_procedure =
				MET_get_procedure(db, prc_string,
								  owner_string)) if (procedure) {
					/* relation was found in more than one database */

					sprintf(s, "PROCEDURE %s is ambiguous", prc_string);
					PAR_error(s);
				}
				else {
					procedure = tmp_procedure;
					request->req_database = db;
				}
	}

	if (!procedure) {
		if (!err_flag)
			return NULL;
		if (owner_string[0])
			sprintf(s, "PROCEDURE %s.%s not defined", owner_string,
					prc_string);
		else
			sprintf(s, "PROCEDURE %s not defined", prc_string);
		PAR_error(s);
	}

	return procedure;
}


//____________________________________________________________
//  
//		Find relation for request.  If request already has a database,
//		find the relation in that database only.
//  

GPRE_REL SQL_relation(GPRE_REQ request,
				 TEXT * rel_string,
				 TEXT * db_string, TEXT * owner_string, BOOLEAN err_flag)
{
	DBB db;
	GPRE_REL relation, tmp_relation;
	SYM symbol;
	SCHAR s[ERROR_LENGTH];

	relation = NULL;

	if (db_string && db_string[0]) {
		/* a database was specified for the relation,
		   search the known symbols for the database name */

		if (!(symbol = MSC_find_symbol(HSH_lookup(db_string), SYM_database)))
			PAR_error("Unknown database specifier.");
		if (request->req_database) {
			if ((DBB) symbol->sym_object != request->req_database)
				PAR_error("Inconsistent database specifier");
		}
		else
			request->req_database = (DBB) symbol->sym_object;
	}

	if (request->req_database)
		relation =
			MET_get_relation(request->req_database, rel_string, owner_string);
	else {
		/* no database was specified, check the metadata for all the databases
		   for the existence of the relation */

		relation = NULL;
		for (db = isc_databases; db; db = db->dbb_next)
			if (tmp_relation = MET_get_relation(db, rel_string, owner_string))
				if (relation) {
					/* relation was found in more than one database */

					sprintf(s, "TABLE %s is ambiguous", rel_string);
					PAR_error(s);
				}
				else {
					relation = tmp_relation;
					request->req_database = db;
				}
	}

	if (!relation) {
		if (!err_flag)
			return (NULL);
		if (owner_string[0])
			sprintf(s, "TABLE %s.%s not defined", owner_string, rel_string);
		else
			sprintf(s, "TABLE %s not defined", rel_string);
		PAR_error(s);
	}

	return relation;
}


//____________________________________________________________
//  
//		Get a relation name (checking for database specifier)
//  

void SQL_relation_name( TEXT * r_name, TEXT * db_name, TEXT * owner_name)
{
	SYM symbol;
	TEXT *t_str;

	db_name[0] = 0;
	owner_name[0] = 0;

	t_str = (TEXT *) ALLOC(NAME_SIZE + 1);
	SQL_resolve_identifier("<Table name>", t_str);

	if (symbol = MSC_find_symbol(token.tok_symbol, SYM_database)) {
		strcpy(db_name, symbol->sym_name);
		ADVANCE_TOKEN;
		if (!MATCH(KW_DOT))
			SYNTAX_ERROR(". (period)");
	}

	SQL_resolve_identifier("<Table name>", t_str);
	if (token.tok_length > NAME_SIZE)
		PAR_error("Table, owner, or database name too long");

	strcpy(r_name, token.tok_string);
	ADVANCE_TOKEN;

	if (MATCH(KW_DOT)) {
		/* the table name was really a owner specifier */

		if (token.tok_length > NAME_SIZE)
			PAR_error("TABLE name too long");
		strcpy(owner_name, r_name);
		SQL_resolve_identifier("<Table name>", t_str);
		strcpy(r_name, token.tok_string);
		ADVANCE_TOKEN;
	}
}


//____________________________________________________________
//  
//		Extract SQL va
//  

TEXT *SQL_var_or_string(BOOLEAN string_only)
{

	if ((!SINGLE_QUOTED(token.tok_type) && sw_sql_dialect == 3) ||
		(!QUOTED(token.tok_type) && sw_sql_dialect == 1)) {
		if (string_only)
			SYNTAX_ERROR("<quoted string>");
		if (!MATCH(KW_COLON))
			SYNTAX_ERROR("<colon> or <quoted string>");
	}
	return PAR_native_value(FALSE, FALSE);
}


//____________________________________________________________
//  
//		Handle an SQL alter statement.
//  

static ACT act_alter(void)
{

	switch (token.tok_keyword) {

	case KW_DATABASE:
	case KW_SCHEMA:
		return act_alter_database();

	case KW_DOMAIN:
		return act_alter_domain();

	case KW_INDEX:
		ADVANCE_TOKEN;
		return act_alter_index();

	case KW_STOGROUP:
		PAR_error("ALTER STOGROUP not supported");

	case KW_TABLE:
		ADVANCE_TOKEN;
		return act_alter_table();

	case KW_TABLESPACE:
		PAR_error("ALTER TABLESPACE not supported");

	default:
		PAR_error("Invalid ALTER request");
	}
	return NULL;				/* silence compiler */
}


//____________________________________________________________
//  
//		Handle an SQL alter database statement
//  

static ACT act_alter_database(void)
{
	ACT action;
	GPRE_REQ request;
	DBB database;
	FIL file;
	SSHORT logdefined;

	request = MAKE_REQUEST(REQ_ddl);
	if (isc_databases && !isc_databases->dbb_next)
		request->req_database = isc_databases;
	else
		PAR_error("Can only alter database in context of single database");

	ADVANCE_TOKEN;

//  create action block 

	action = MAKE_ACTION(request, ACT_alter_database);
	action->act_whenever = gen_whenever();
	database = (DBB) ALLOC(DBB_LEN);
	database->dbb_grp_cmt_wait = -1;
	action->act_object = (REF) database;

	while (TRUE) {
		if (MATCH(KW_DROP)) {
			if (MATCH(KW_LOG_FILE))
				database->dbb_flags |= DBB_drop_log;
			else if (MATCH(KW_CASCADE))
				database->dbb_flags |= DBB_cascade;
#ifdef FLINT_CACHE
			else if (MATCH(KW_CACHE))
				database->dbb_flags |= DBB_drop_cache;
			else
				PAR_error("only log or cache can be dropped");
#else
			else
				PAR_error("only log file can be dropped");
#endif /* FLINT_CACHE */
		}
		else if (MATCH(KW_ADD)) {
			if (MATCH(KW_FILE)) {
				do {
					file = define_file();
					file->fil_next = database->dbb_files;
					database->dbb_files = file;
				}
				while (MATCH(KW_FILE));
			}
			else if (MATCH(KW_LOG_FILE)) {
				if (logdefined)
					PAR_error("log redefinition");
				logdefined = TRUE;
				if (MATCH(KW_LEFT_PAREN)) {
					while (TRUE) {
						file = define_log_file(FALSE);
						file->fil_next = database->dbb_logfiles;
						database->dbb_logfiles = file;
						if (!MATCH(KW_COMMA)) {
							EXP_match_paren();
							break;
						}
					}

					if (MATCH(KW_OVERFLOW))
						database->dbb_overflow = define_log_file(TRUE);
					else
						PAR_error
							("Overflow log specification required for this configuration");

				}
				else if (MATCH(KW_BASE_NAME)) {
					database->dbb_flags |= DBB_log_serial;
					database->dbb_logfiles = file = define_log_file(TRUE);
				}
				else
					database->dbb_flags |= DBB_log_default;
			}
#ifdef FLINT_CACHE
			else if (MATCH(KW_CACHE))
				database->dbb_cache_file = define_cache();
#endif /* FLINT_CACHE */
		}
		else if (MATCH(KW_SET)) {
			while (TRUE) {
				if (MATCH(KW_CHECK_POINT_LEN)) {
					MATCH(KW_EQUALS);
					database->dbb_chkptlen = EXP_ULONG_ordinal(TRUE);
					MATCH(KW_COMMA);
				}
				else if (MATCH(KW_NUM_LOG_BUFS)) {
					MATCH(KW_EQUALS);
					database->dbb_numbufs = EXP_USHORT_ordinal(TRUE);
					MATCH(KW_COMMA);
				}
				else if (MATCH(KW_LOG_BUF_SIZE)) {
					MATCH(KW_EQUALS);
					database->dbb_bufsize = EXP_USHORT_ordinal(TRUE);
					MATCH(KW_COMMA);
				}
				else if (MATCH(KW_GROUP_COMMIT_WAIT)) {
					MATCH(KW_EQUALS);
					database->dbb_grp_cmt_wait = EXP_ULONG_ordinal(TRUE);
					MATCH(KW_COMMA);
				}
				else
					break;
			}
		}
		else
			break;
	}
	return action;
}


//____________________________________________________________
//  
//		Handle altering of a domain (global field). 
//  

static ACT act_alter_domain(void)
{
	ACT action;
	GPRE_REQ request;
	GPRE_FLD field;
	CNSTRT * cnstrt_ptr;
	CNSTRT cnstrt_str;
	GPRE_NOD literal_node;

//  create request block 

	request = MAKE_REQUEST(REQ_ddl);
	if (isc_databases && !isc_databases->dbb_next)
		request->req_database = isc_databases;
	else
		PAR_error("Can only ALTER a domain in context of single database");

//  create action block 

	action = MAKE_ACTION(request, ACT_alter_domain);

	ADVANCE_TOKEN;
	field = make_field(0);
	cnstrt_ptr = &field->fld_constraints;

//  Check if default value was specified 

	while (!END_OF_COMMAND) {
		if (MATCH(KW_SET)) {
			if (token.tok_keyword == KW_DEFAULT) {
				field->fld_default_source = CPR_start_text();
				ADVANCE_TOKEN;
			}
			else
				SYNTAX_ERROR("DEFAULT");

			if (MATCH(KW_USER))
				field->fld_default_value = MAKE_NODE(nod_user_name, 0);
			else if (MATCH(KW_NULL))
				field->fld_default_value = MAKE_NODE(nod_null, 0);
			else {
				if (MATCH(KW_MINUS)) {
					if (token.tok_type != tok_number)
						SYNTAX_ERROR("<number>");

					literal_node = EXP_literal();
					field->fld_default_value = MSC_unary(nod_negate,
														 literal_node);
				}
				else if ((field->fld_default_value = EXP_literal()) == NULL)
					SYNTAX_ERROR("<constant>");
			}
			CPR_end_text(field->fld_default_source);
		}
		else if (MATCH(KW_ADD)) {
			MATCH(KW_CONSTRAINT);
			if (token.tok_keyword == KW_CHECK) {
				cnstrt_str = par_field_constraint(request, field, 0);
				*cnstrt_ptr = cnstrt_str;
				cnstrt_ptr = &cnstrt_str->cnstrt_next;
			}
			else
				PAR_error("Invalid constraint.");
		}
		else if (MATCH(KW_DROP)) {
			if (MATCH(KW_CONSTRAINT)) {
				cnstrt_str = (CNSTRT) ALLOC(CNSTRT_LEN);
				cnstrt_str->cnstrt_flags |= CNSTRT_delete;
				*cnstrt_ptr = cnstrt_str;
				cnstrt_ptr = &cnstrt_str->cnstrt_next;
			}
			else if (MATCH(KW_DEFAULT)) {
				field->fld_default_value = MAKE_NODE(nod_erase, 0);
			}
			else
				PAR_error("Invalid attribute for DROP");
		}
		else
			SYNTAX_ERROR("SET, ADD, or DROP");

	}

	action->act_whenever = gen_whenever();
	action->act_object = (REF) field;

	return action;
}


//____________________________________________________________
//  
//		Handle an SQL alter index statement.
//  

static ACT act_alter_index(void)
{
	ACT action;
	GPRE_REQ request;
	IND index;
	SCHAR i_name[NAME_SIZE + 1];

//  create request block 

	request = MAKE_REQUEST(REQ_ddl);

	if (token.tok_length > NAME_SIZE)
		PAR_error("Index name too long");

	SQL_resolve_identifier("<column name>", i_name);

	ADVANCE_TOKEN;

	index = make_index(request, i_name);

	if (MATCH(KW_ACTIVE))
		index->ind_flags |= IND_active;
	else if (MATCH(KW_INACTIVE))
		index->ind_flags |= IND_inactive;
	else
		PAR_error("Unsupported ALTER INDEX option");

	action = MAKE_ACTION(request, ACT_alter_index);
	action->act_object = (REF) index;
	action->act_whenever = gen_whenever();

	return action;
}


//____________________________________________________________
//  
//		Handle an SQL alter table statement.
//  

static ACT act_alter_table(void)
{
	ACT action;
	GPRE_FLD field, *ptr;
	GPRE_REQ request;
	GPRE_REL relation;
	CNSTRT cnstrt_str;
	CNSTRT *cnstrt_ptr;
	GPRE_CTX context;

//  create request block 

	request = MAKE_REQUEST(REQ_ddl);

//  get table name and create relation block 

	relation = par_relation(request);

//  CHECK Constraints require the context to be set to the
//  current relation 

	request->req_contexts = context = MAKE_CONTEXT(request);
	context->ctx_relation = relation;

//  Reserve context 1 for relation on which constraint is
//  being defined 

	context->ctx_internal++;
	request->req_internal++;

//  create action block 

	action = MAKE_ACTION(request, ACT_alter_table);
	action->act_whenever = gen_whenever();
	action->act_object = (REF) relation;

//  parse action list and create corresponding field blocks 

	ptr = &relation->rel_fields;
	cnstrt_ptr = &relation->rel_constraints;

	while (!END_OF_COMMAND) {
		if (MATCH(KW_ADD)) {
			switch (token.tok_keyword) {
			case KW_CONSTRAINT:
			case KW_PRIMARY:
			case KW_UNIQUE:
			case KW_FOREIGN:
			case KW_CHECK:
				cnstrt_str = par_table_constraint(request, relation);
				*cnstrt_ptr = cnstrt_str;
				cnstrt_ptr = &cnstrt_str->cnstrt_next;
				break;

			default:
				field = par_field(request, relation);
				*ptr = field;
				ptr = &field->fld_next;
			}
		}
		else if (MATCH(KW_DROP)) {
			if (token.tok_keyword == KW_CONSTRAINT) {
				ADVANCE_TOKEN;
				cnstrt_str = (CNSTRT) ALLOC(CNSTRT_LEN);
				cnstrt_str->cnstrt_flags |= CNSTRT_delete;
				cnstrt_str->cnstrt_name = (STR) ALLOC(NAME_SIZE + 1);
				SQL_resolve_identifier("<constraint name>",
									   (TEXT *) cnstrt_str->cnstrt_name);
				if (token.tok_length > NAME_SIZE)
					PAR_error("Constraint name too long");
				*cnstrt_ptr = cnstrt_str;
				cnstrt_ptr = &cnstrt_str->cnstrt_next;
				ADVANCE_TOKEN;
			}
			else {
				field = make_field(relation);
				field->fld_flags |= FLD_delete;
				*ptr = field;
				ptr = &field->fld_next;
				/* Fix for bug 8054:

				   [CASCADE | RESTRICT] syntax is available in IB4.5, but not
				   required until v5.0.

				   Option CASCADE causes an error :
				   unsupported construct

				   Option RESTRICT is default behaviour.
				 */
				if (token.tok_keyword == KW_CASCADE) {
					PAR_error("Unsupported construct CASCADE");
					ADVANCE_TOKEN;
				}
				else if (token.tok_keyword == KW_RESTRICT) {
					ADVANCE_TOKEN;
				}
			}
		}
		else
			SYNTAX_ERROR("ADD or DROP");
		if (!MATCH(KW_COMMA))
			break;
	}

	return action;
}


//____________________________________________________________
//  
//		Handle an SQL comment statement.
//			Reject
//  

static ACT act_comment(void)
{

	PAR_error("SQL COMMENT ON request not allowed");
	return NULL;				/* silence compiler */
}


//____________________________________________________________
//  
//		Parse a CONNECT statement.
//  

static ACT act_connect(void)
{
	ACT action;
	GPRE_REQ request;
	SYM symbol;
	RDY ready;
	DBB db;
	BOOLEAN need_handle;
	USHORT default_buffers, buffers = 0;
	TEXT *user = NULL, *password = NULL, *sql_role = NULL;
	TEXT *lc_messages = NULL;

	action = MAKE_ACTION(0, ACT_ready);
	action->act_whenever = gen_whenever();
	need_handle = FALSE;
	default_buffers = 0;

	MATCH(KW_TO);

	if (!MATCH(KW_ALL) && !MATCH(KW_DEFAULT))
		while (TRUE) {
			ready = (RDY) ALLOC(RDY_LEN);
			ready->rdy_next = (RDY) action->act_object;
			action->act_object = (REF) ready;

			if (!(symbol = token.tok_symbol)
				|| symbol->sym_type != SYM_database) {
				ready->rdy_filename = SQL_var_or_string(FALSE);
				if (MATCH(KW_AS))
					need_handle = TRUE;
			}

			if (!(symbol = token.tok_symbol)
				|| symbol->sym_type != SYM_database) {
				if (!isc_databases || isc_databases->dbb_next || need_handle) {
					need_handle = FALSE;
					SYNTAX_ERROR("<database handle>");
				}
				ready->rdy_database = dup_dbb(isc_databases);
			}

			need_handle = FALSE;
			if (!ready->rdy_database) {
				ready->rdy_database = dup_dbb((DBB) symbol->sym_object);
				ADVANCE_TOKEN;
			}

			/* pick up the possible parameters, in any order */

			buffers = 0;
			db = ready->rdy_database;
			connect_opts(&db->dbb_r_user, &db->dbb_r_password,
						 &db->dbb_r_sql_role, &db->dbb_r_lc_messages,
						 &buffers);

			request = NULL;
			if (buffers)
				request = PAR_set_up_dpb_info(ready, action, buffers);

			/* if there are any options that take host variables as arguments, 
			   make sure that we generate variables for the request so that the 
			   dpb can be extended at runtime */

			if (db->dbb_r_user || db->dbb_r_password || db->dbb_r_sql_role
				|| db->dbb_r_lc_messages || db->dbb_r_lc_ctype) {
				if (!request)
					request =
						PAR_set_up_dpb_info(ready, action, default_buffers);
				request->req_flags |= REQ_extend_dpb;
			}

			/* ...and if there are compile time user or password specified,
			   make sure there will be a dpb generated for them */

			if (!request && (db->dbb_c_user || db->dbb_c_password ||
							 db->dbb_c_sql_role ||
							 db->dbb_c_lc_messages || db->dbb_c_lc_ctype))
					request =
					PAR_set_up_dpb_info(ready, action, default_buffers);

			if (!MATCH(KW_COMMA))
				break;
		}

	if (action->act_object)
		return action;

//  No explicit databases -- pick up all known 

	connect_opts(&user, &password, &sql_role, &lc_messages, &buffers);

	for (db = isc_databases; db; db = db->dbb_next)
		if (db->dbb_runtime || !(db->dbb_flags & DBB_sqlca)) {
			ready = (RDY) ALLOC(RDY_LEN);
			ready->rdy_next = (RDY) action->act_object;
			action->act_object = (REF) ready;
			ready->rdy_database = dup_dbb(db);
		}

	if (!action->act_object)
		PAR_error("no database available to CONNECT");
	else
		for (ready = (RDY) action->act_object; ready; ready = ready->rdy_next) {
			if (buffers)
				request = PAR_set_up_dpb_info(ready, action, buffers);
			else
				request = ready->rdy_request;

			/* if there are any options that take host variables as arguments, 
			   make sure that we generate variables for the request so that the 
			   dpb can be extended at runtime */

			db = ready->rdy_database;
			if (user || password || lc_messages || db->dbb_r_lc_ctype) {
				db->dbb_r_user = user;
				db->dbb_r_password = password;
				db->dbb_r_lc_messages = lc_messages;
				if (!request)
					request =
						PAR_set_up_dpb_info(ready, action, default_buffers);
				request->req_flags |= REQ_extend_dpb;
			}

			/* ...and if there are compile time user or password specified,
			   make sure there will be a dpb generated for them */

			if (!request && (db->dbb_c_user || db->dbb_c_password ||
							 db->dbb_c_sql_role ||
							 db->dbb_c_lc_ctype || db->dbb_c_lc_messages))
					request =
					PAR_set_up_dpb_info(ready, action, default_buffers);
		}

	return action;
}


//____________________________________________________________
//  
//		Handle an SQL create statement.
//  

static ACT act_create(void)
{
	BOOLEAN descending;
	BOOLEAN unique;

	if (MATCH(KW_DATABASE) || MATCH(KW_SCHEMA))
		return act_create_database();

	if (MATCH(KW_DOMAIN))
		return act_create_domain();

	if (MATCH(KW_GENERATOR))
		return act_create_generator();

	if (MATCH(KW_SHADOW))
		return act_create_shadow();

	if (MATCH(KW_STOGROUP))
		PAR_error("CREATE STOGROUP not supported");

	if (MATCH(KW_SYNONYM))
		PAR_error("CREATE SYNONYM not supported");

	if (MATCH(KW_TABLE))
		return act_create_table();

	if (MATCH(KW_TABLESPACE))
		PAR_error("CREATE TABLESPACE not supported");

	if (MATCH(KW_VIEW))
		return (act_create_view());

	if (KEYWORD(KW_UNIQUE) || KEYWORD(KW_ASCENDING) ||
		KEYWORD(KW_DESCENDING) || KEYWORD(KW_INDEX)) {
		descending = FALSE;
		unique = FALSE;
		while (TRUE) {
			if (MATCH(KW_UNIQUE))
				unique = TRUE;
			else if (MATCH(KW_ASCENDING))
				descending = FALSE;
			else if (MATCH(KW_DESCENDING))
				descending = TRUE;
			else if (MATCH(KW_INDEX))
				return act_create_index(unique, descending);
			else
				break;
		}
	}

	PAR_error("Invalid CREATE request");
	return NULL;				/* silence compiler */
}


//____________________________________________________________
//  
//		Handle an SQLish create database statement.
//  

static ACT act_create_database(void)
{
	ACT action;
	SYM symbol;
	DBB db;
	MDBB mdb;
	GPRE_REQ request;
	SCHAR *string;
	BOOLEAN dummy, extend_dpb;

	if (isc_databases && isc_databases->dbb_next)
		PAR_error
			("CREATE DATABASE only allowed in context of a single database");

	if (!isc_databases) {
		/* generate a dummy db */

		dummy = TRUE;
		isc_databases = (DBB) MSC_alloc_permanent(DBB_LEN);

		/* allocate symbol block */

		symbol = (SYM) MSC_alloc_permanent(SYM_LEN);

		/* make it the default database */

		symbol->sym_type = SYM_database;
		symbol->sym_object = (GPRE_CTX) isc_databases;
		symbol->sym_string = database_name;

		/* database block points to the symbol block */

		isc_databases->dbb_name = symbol;
		isc_databases->dbb_filename = NULL;
		isc_databases->dbb_c_lc_ctype = module_lc_ctype;
	}
	else
		dummy = FALSE;

//  get database name 

	if (QUOTED(token.tok_type)) {
		db = dup_dbb(isc_databases);
		db->dbb_filename = string = (TEXT *) ALLOC(token.tok_length + 1);
		COPY(token.tok_string, token.tok_length, string);
		if (!isc_databases->dbb_filename)
			isc_databases->dbb_filename = string;
		ADVANCE_TOKEN;
	}
	else
		SYNTAX_ERROR("<quoted database name>");

//  Create a request for generating DYN to add files to database. 

//  request = MAKE_REQUEST (REQ_create_database); 
	request = MAKE_REQUEST(REQ_ddl);
	request->req_flags |= REQ_sql_database_dyn;
	request->req_database = db;

//  create action block 

	action = MAKE_ACTION(request, ACT_create_database);

	mdb = (MDBB) ALLOC(sizeof(mdbb));
	mdb->mdbb_database = db;
	action->act_object = (REF) mdb;
	action->act_whenever = gen_whenever();

	if (dummy) {
		/* Create a ACT_database action */

		action->act_rest = MAKE_ACTION(0, ACT_database);
		action->act_rest->act_flags |= ACT_mark;
	}

//  Get optional specifications 

	extend_dpb = tail_database(ACT_create_database, db);

//  Create a request to generate dpb 

	ada_flags |= ADA_create_database;

	request = MAKE_REQUEST(REQ_create_database);
	request->req_actions = action;
	mdb->mdbb_dpb_request = request;
	request->req_database = db;
	if (extend_dpb)
		request->req_flags |= REQ_extend_dpb;

	return action;
}


//____________________________________________________________
//  
//		Handle creation of a domain (global field). 
//  

static ACT act_create_domain(void)
{
	ACT action;
	GPRE_REQ request;
	GPRE_FLD field;
	CNSTRT *cnstrt_ptr;
	int in_constraints;
	GPRE_NOD literal_node;

//  create request block 

	request = MAKE_REQUEST(REQ_ddl);
	if (isc_databases && !isc_databases->dbb_next)
		request->req_database = isc_databases;
	else
		PAR_error("Can only CREATE DOMAIN in context of single database");

//  create action block 

	action = MAKE_ACTION(request, ACT_create_domain);

	field = make_field(0);
	MATCH(KW_AS);
	SQL_par_field_dtype(request, field, FALSE);

//  Check if default value was specified 

	if (token.tok_keyword == KW_DEFAULT) {
		field->fld_default_source = CPR_start_text();
		ADVANCE_TOKEN;

		if (MATCH(KW_USER))
			field->fld_default_value = MAKE_NODE(nod_user_name, 0);
		else if (MATCH(KW_NULL))
			field->fld_default_value = MAKE_NODE(nod_null, 0);
		else {
			if (MATCH(KW_MINUS)) {
				if (token.tok_type != tok_number)
					SYNTAX_ERROR("<number>");

				literal_node = EXP_literal();
				field->fld_default_value = MSC_unary(nod_negate,
													 literal_node);
			}
			else if ((field->fld_default_value = EXP_literal()) == NULL)
				SYNTAX_ERROR("<constant>");
		}
		CPR_end_text(field->fld_default_source);
	}


//  Check for any column level constraints 

	cnstrt_ptr = &field->fld_constraints;
	in_constraints = TRUE;

	while (in_constraints) {
		switch (token.tok_keyword) {
		case KW_CONSTRAINT:
		case KW_CHECK:
		case KW_NOT:
			*cnstrt_ptr = par_field_constraint(request, field, 0);
			cnstrt_ptr = &(*cnstrt_ptr)->cnstrt_next;
			break;

		default:
			in_constraints = FALSE;
			break;
		}
	}

	SQL_par_field_collate(request, field);
	SQL_adjust_field_dtype(field);

	action->act_whenever = gen_whenever();
	action->act_object = (REF) field;

	return action;
}


//____________________________________________________________
//  
//		Handle an SQL create generator statement
//  

static ACT act_create_generator(void)
{

	ACT action;
	TEXT *generator_name;
	GPRE_REQ request;


	generator_name = (TEXT *) ALLOC(NAME_SIZE + 1);
	SQL_resolve_identifier("<identifier>", generator_name);

	request = MAKE_REQUEST(REQ_ddl);
	if (isc_databases && !isc_databases->dbb_next)
		request->req_database = isc_databases;
	else
		PAR_error("Can only CREATE GENERATOR in context of single database");

	if (token.tok_length > NAME_SIZE)
		PAR_error("Generator name too long");


//  create action block 
	action = MAKE_ACTION(request, ACT_create_generator);
	action->act_whenever = gen_whenever();
	action->act_object = (REF) generator_name;

	ADVANCE_TOKEN;
	return action;
}


//____________________________________________________________
//  
//		Handle an SQL create index statement.
//  

static ACT act_create_index( SSHORT dups, BOOLEAN descending)
{
	ACT action;
	GPRE_FLD *ptr;
	IND index;
	GPRE_REQ request;
	GPRE_REL relation;
	SCHAR i_name[NAME_SIZE + 1];

//  create request block 

	request = MAKE_REQUEST(REQ_ddl);

//  get index and table names and create index and relation blocks 

	if (token.tok_length > NAME_SIZE)
		PAR_error("Index name too long");


	SQL_resolve_identifier("<column name>", i_name);

	ADVANCE_TOKEN;

	if (!MATCH(KW_ON))
		SYNTAX_ERROR("ON");

	relation = par_relation(request);
	index = make_index(request, i_name);
	index->ind_relation = relation;
	index->ind_flags |= (dups) ? IND_dup_flag : 0;
	index->ind_flags |= (descending) ? IND_descend : 0;

//  create action block 

	action = MAKE_ACTION(request, ACT_create_index);
	action->act_whenever = gen_whenever();
	action->act_object = (REF) index;

//  parse field list and create corresponding field blocks 

	EXP_left_paren(0);
	ptr = &index->ind_fields;

	for (;;) {
		*ptr = make_field(relation);
		ptr = &(*ptr)->fld_next;
		if (!MATCH(KW_COMMA))
			break;
	}

	EXP_match_paren();

	return action;
}


//____________________________________________________________
//  
//		Handle an SQL create shadow statement
//  

static ACT act_create_shadow(void)
{

	ACT action;
	GPRE_REQ request;
	FIL file_list, file;
	SLONG shadow_number;
	USHORT file_flags = 0;
	SLONG length;
	TEXT err_string[1024];

	request = MAKE_REQUEST(REQ_ddl);
	if (isc_databases && !isc_databases->dbb_next)
		request->req_database = isc_databases;
	else
		PAR_error("Can only CREATE SHADOW in context of single database");

	action = MAKE_ACTION(request, ACT_create_shadow);
	action->act_whenever = gen_whenever();
	shadow_number = EXP_USHORT_ordinal((USHORT) TRUE);

	if (!RANGE_POSITIVE_SHORT_INTEGER(shadow_number))
		PAR_error("Shadow number out of range");

	if (MATCH(KW_MANUAL))
		file_flags |= FIL_manual;
	else
		MATCH(KW_AUTO);

	if (MATCH(KW_CONDITIONAL))
		file_flags |= FIL_conditional;

	file_list = file = define_file();
	if (file->fil_start)
		PAR_error("Can not specify file start for first file");
	length = file->fil_length;
	file->fil_flags = file_flags;
	file->fil_shadow_number = (SSHORT) shadow_number;
	while (MATCH(KW_FILE)) {
		file = define_file();
		if (!length && !file->fil_start) {
			sprintf(err_string,
					"Preceding file did not specify length, so %s must include starting page number",
					file->fil_name);
			PAR_error(err_string);
		}
		length = file->fil_length;
		file->fil_flags = file_flags;
		file->fil_next = file_list;
		file_list = file;
	}
	action->act_object = (REF) file_list;
	return action;
}


//____________________________________________________________
//  
//		Handle an SQL create table statement.
//  

static ACT act_create_table(void)
{
	ACT action;
	GPRE_FLD *ptr;
	GPRE_REQ request;
	GPRE_REL relation;
	cnstrt** constraint_ptr;
	GPRE_CTX context;
	TEXT *string;

	request = MAKE_REQUEST(REQ_ddl);
	relation = par_relation(request);

	if (MATCH(KW_EXTERNAL)) {
		MATCH(KW_FILE);
		if (QUOTED(token.tok_type)) {
			relation->rel_ext_file = string =
				(TEXT *) ALLOC(token.tok_length + 1);
			COPY(token.tok_string, token.tok_length, string);
			ADVANCE_TOKEN;
		}
		else
			SYNTAX_ERROR("<quoted filename>");

		if (!check_filename(string))
			PAR_error("node name not permitted");	/* a node name is not permitted in external file name */
	}

//  CHECK Constraints require the context to be set to the
//  current relation 

	request->req_contexts = context = MAKE_CONTEXT(request);
	context->ctx_relation = relation;

//  Reserve context 1 for relation on which constraint is
//  being defined 
	context->ctx_internal++;
	request->req_internal++;

//  create action block 

	action = MAKE_ACTION(request, ACT_create_table);
	action->act_whenever = gen_whenever();
	action->act_object = (REF) relation;

	EXP_left_paren(0);
	ptr = &relation->rel_fields;
	constraint_ptr = &relation->rel_constraints;

	for (;;) {
		switch (token.tok_keyword) {
		case KW_CONSTRAINT:
		case KW_PRIMARY:
		case KW_UNIQUE:
		case KW_FOREIGN:
		case KW_CHECK:
			*constraint_ptr = par_table_constraint(request, relation);
			constraint_ptr = &(*constraint_ptr)->cnstrt_next;
			break;

		default:
			/* parse field list and create corresponding field blocks */

			*ptr = par_field(request, relation);
			ptr = &(*ptr)->fld_next;
		}
		if (!MATCH(KW_COMMA))
			break;
	}
	EXP_match_paren();

	return action;
}


//____________________________________________________________
//  
//		Handle an SQL create view statement.
//  

static ACT act_create_view(void)
{
	ACT action;
	GPRE_FLD *ptr;
	GPRE_REQ request;
	GPRE_REL relation;
	RSE select;

	request = MAKE_REQUEST(REQ_ddl);
	relation = par_relation(request);

//  create action block 

	action = MAKE_ACTION(request, ACT_create_view);
	action->act_whenever = gen_whenever();
	action->act_object = (REF) relation;

//  if field list is present parse it and create corresponding field blocks 

	if (MATCH(KW_LEFT_PAREN)) {
		ptr = &relation->rel_fields;
		for (;;) {
			*ptr = make_field(relation);
			ptr = &(*ptr)->fld_next;
			if (MATCH(KW_RIGHT_PAREN))
				break;
			MATCH(KW_COMMA);
		}
	}

//  skip 'AS SELECT' 

	if (!MATCH(KW_AS))
		SYNTAX_ERROR("AS");

	relation->rel_view_text = CPR_start_text();
	if (!MATCH(KW_SELECT))
		SYNTAX_ERROR("SELECT");

//  reserve context variable 0 for view 

	request->req_internal++;

//  parse the view SELECT 

	relation->rel_view_rse = select = SQE_select(request, TRUE);
	EXP_rse_cleanup(select);

	if (MATCH(KW_WITH)) {
		if (!MATCH(KW_CHECK))
			SYNTAX_ERROR("CHECK");
		if (!MATCH(KW_OPTION))
			SYNTAX_ERROR("OPTION");
		relation->rel_flags |= REL_view_check;
	}

	CPR_end_text(relation->rel_view_text);

	return action;
}


//____________________________________________________________
//  
//		Recognize BEGIN/END DECLARE SECTION, 
//     and mark it as a good place to put miscellaneous
//     global routine stuff.
//  

static ACT act_d_section( enum act_t type)
{
	ACT action;
	SYM symbol;

	if (!MATCH(KW_DECLARE))
		SYNTAX_ERROR("DECLARE SECTION");

	if (!MATCH(KW_SECTION))
		SYNTAX_ERROR("SECTION");

	MATCH(KW_SEMI_COLON);

	action = (ACT) ALLOC(ACT_LEN);
	action->act_type = type;

	if (type == ACT_b_declare)
		cur_routine = action;

	if (!isc_databases) {
		/* allocate database block and link to db chain */

		isc_databases = (DBB) MSC_alloc_permanent(DBB_LEN);

		/* allocate symbol block */

		symbol = (SYM) MSC_alloc_permanent(SYM_LEN);

		/* make it a database, specifically this one */

		symbol->sym_type = SYM_database;
		symbol->sym_object = (GPRE_CTX) isc_databases;
		symbol->sym_string = database_name;

		/* database block points to the symbol block */

		isc_databases->dbb_name = symbol;
		isc_databases->dbb_filename = NULL;
		isc_databases->dbb_flags = DBB_sqlca;
		isc_databases->dbb_c_lc_ctype = module_lc_ctype;
		if (sw_external)
			isc_databases->dbb_scope = DBB_EXTERN;
	}
	else {
		/* Load up the symbol (hash) table with relation names from this database */

		MET_load_hash_table(isc_databases);
	}

	HSH_insert(isc_databases->dbb_name);

	if (MATCH(KW_SQL)) {
		if (!MATCH(KW_NAMES))
			SYNTAX_ERROR("NAMES ARE");
		if (!MATCH(KW_ARE))
			SYNTAX_ERROR("NAMES ARE");
		if (!(symbol = MSC_find_symbol(token.tok_symbol, SYM_charset)))
			PAR_error("The named CHARACTER SET was not found");
		if (module_lc_ctype && !strcmp(module_lc_ctype, symbol->sym_string))
			PAR_error("Duplicate specification of module CHARACTER SET.");
		module_lc_ctype = symbol->sym_string;
		isc_databases->dbb_c_lc_ctype = symbol->sym_string;
		CPR_token();
	}

	return action;
}


//____________________________________________________________
//  
//		Parse the SQL cursor declaration.
//  

static ACT act_declare(void)
{
	DBB db;
	GPRE_REQ request;
	ACT action;
	SYM symbol;
	DYN statement;
	TEXT t_str[132];
	BOOLEAN delimited = FALSE;
#ifdef SCROLLABLE_CURSORS
	BOOLEAN scroll = FALSE;
#endif

	db = NULL;

	if (token.tok_symbol && (token.tok_symbol->sym_type == SYM_database)) {
		/* must be a database specifier in a DECLARE TABLE statement */

		db = (DBB) token.tok_symbol->sym_object;
		ADVANCE_TOKEN;
		if (!MATCH(KW_DOT))
			SYNTAX_ERROR(". (period)");
		symbol = PAR_symbol(SYM_dummy);
		if (token.tok_keyword != KW_TABLE)
			SYNTAX_ERROR("TABLE");
		return (act_declare_table(symbol, db));
	}

	switch (token.tok_keyword) {
	case KW_FILTER:
		return (act_declare_filter());
		break;

	case KW_EXTERNAL:
		ADVANCE_TOKEN;
		if (MATCH(KW_FUNCTION))
			return (act_declare_udf());
		else
			SYNTAX_ERROR("FUNCTION");
		break;
	}

	SQL_resolve_identifier("<Cursor Name>", t_str);
	if (token.tok_type == tok_dblquoted)
		delimited = TRUE;
	else {
		SYM symb;
		if ((symb = HSH_lookup2(token.tok_string)) &&
			(symb->sym_type == SYM_cursor ||
			 symb->sym_type == SYM_delimited_cursor)) {
			char s[64];
			sprintf(s, "symbol %s is already in use", t_str);
			PAR_error(s);
		}
	}
	symbol = PAR_symbol(SYM_cursor);

	switch (token.tok_keyword) {
	case KW_TABLE:
		return (act_declare_table(symbol, 0));

#ifdef SCROLLABLE_CURSORS
	case KW_SCROLL:
		ADVANCE_TOKEN;
		scroll = TRUE;
		if (token.tok_keyword != KW_CURSOR)
			SYNTAX_ERROR("CURSOR");
#endif
	case KW_CURSOR:
		ADVANCE_TOKEN;
		if (!MATCH(KW_FOR))
			SYNTAX_ERROR("FOR");
		if (MATCH(KW_SELECT)) {
			request = MAKE_REQUEST(REQ_cursor);
			request->req_flags |= REQ_sql_cursor | REQ_sql_declare_cursor;
#ifdef SCROLLABLE_CURSORS
			if (scroll)
				request->req_flags |= REQ_scroll;
#endif
			symbol->sym_object = (GPRE_CTX) request;
			action = MAKE_ACTION(request, ACT_cursor);
			action->act_object = (REF) symbol;
			symbol->sym_type =
				(delimited) ? SYM_delimited_cursor : SYM_cursor;
			request->req_rse = SQE_select(request, FALSE);
			if (MATCH(KW_FOR)) {
				if (!MATCH(KW_UPDATE))
					SYNTAX_ERROR("UPDATE");
				if (!MATCH(KW_OF))
					SYNTAX_ERROR("OF");

				do
					CPR_token();
				while (MATCH(KW_COMMA));
			}
			EXP_rse_cleanup(request->req_rse);
		}
		else if (MATCH(KW_READ)) {
			action = act_open_blob(ACT_blob_open, symbol);
			symbol->sym_type =
				(delimited) ? SYM_delimited_cursor : SYM_cursor;
		}
		else if (MATCH(KW_INSERT)) {
			action = act_open_blob(ACT_blob_create, symbol);
			symbol->sym_type =
				(delimited) ? SYM_delimited_cursor : SYM_cursor;
		}
		else {
			statement = par_statement();
			symbol->sym_object = (GPRE_CTX) statement;
			if (MATCH(KW_FOR)) {
				if (!MATCH(KW_UPDATE))
					SYNTAX_ERROR("UPDATE");
				if (!MATCH(KW_OF))
					SYNTAX_ERROR("OF");

				do
					CPR_token();
				while (MATCH(KW_COMMA));
			}
			symbol->sym_type = SYM_dyn_cursor;
			statement->dyn_cursor_name = symbol;
			action = (ACT) ALLOC(ACT_LEN);
			action->act_type = ACT_dyn_cursor;
			action->act_object = (REF) statement;
			action->act_whenever = gen_whenever();
		}
		HSH_insert(symbol);
		return action;

	default:
		while (MATCH(KW_COMMA));
		if (MATCH(KW_STATEMENT)) {
			action = (ACT) ALLOC(ACT_LEN);
			action->act_type = ACT_dyn_statement;
			return action;
		}
		SYNTAX_ERROR("CURSOR, STATEMENT or TABLE");
	}
	return NULL;				/* silence compiler */
}


//____________________________________________________________
//  
//		Handle an SQL declare filter statement
//  

static ACT act_declare_filter(void)
{
	ACT action;
	GPRE_REQ request;
	FLTR filter;
	SLONG input_type, output_type;

	request = MAKE_REQUEST(REQ_ddl);

	if (isc_databases && !isc_databases->dbb_next)
		request->req_database = isc_databases;
	else
		PAR_error("Can only DECLARE FILTER in context of single database");

	ADVANCE_TOKEN;
	filter = (FLTR) ALLOC(FLTR_LEN);
	filter->fltr_name = (TEXT *) ALLOC(NAME_SIZE + 1);
	SQL_resolve_identifier("<identifier>", filter->fltr_name);
	if (token.tok_length > NAME_SIZE)
		PAR_error("Filter name too long");

	ADVANCE_TOKEN;

//  create action block 
	action = MAKE_ACTION(request, ACT_declare_filter);
	action->act_whenever = gen_whenever();
	action->act_object = (REF) filter;

	if (MATCH(KW_INPUT_TYPE))
		input_type = EXP_SSHORT_ordinal((USHORT) TRUE);
	else
		SYNTAX_ERROR("INPUT_TYPE");

	if (MATCH(KW_OUTPUT_TYPE))
		output_type = EXP_SSHORT_ordinal((USHORT) TRUE);
	else
		SYNTAX_ERROR("OUTPUT_TYPE");

	if (!RANGE_SHORT_INTEGER(input_type)
		|| !RANGE_SHORT_INTEGER(output_type))
		PAR_error("Blob sub_type out of range");

	filter->fltr_input_type = (SSHORT) input_type;
	filter->fltr_output_type = (SSHORT) output_type;

	if (MATCH(KW_ENTRY_POINT))
		filter->fltr_entry_point = extract_string(TRUE);
	else
		SYNTAX_ERROR("ENTRY_POINT");

	if (MATCH(KW_MODULE_NAME))
		filter->fltr_module_name = extract_string(TRUE);
	else
		SYNTAX_ERROR("MODULE_NAME");

	return action;
}


//____________________________________________________________
//  
//		Handle an SQL declare table statement.
//  

static ACT act_declare_table( SYM symbol, DBB db)
{
	ACT action;
	GPRE_FLD field, dbkey, *ptr;
	GPRE_REL relation, tmp_relation;
	SYM old_symbol, tmp_symbol;
	USHORT count;
	GPRE_REQ request;

//  create a local request block 

	request = (GPRE_REQ) ALLOC(REQ_LEN);
	request->req_type = REQ_ddl;

//  create relation block 

	relation = make_relation(0, symbol->sym_string);

	if (!db)
		db = relation->rel_database;

	request->req_database = db;

	relation->rel_next = db->dbb_relations;
	db->dbb_relations = relation;
	relation->rel_dbkey = dbkey =
		MET_make_field("rdb$db_key", dtype_text, 8, FALSE);
	dbkey->fld_flags |= FLD_dbkey | FLD_text | FLD_charset;
	dbkey->fld_ttype = ttype_binary;

//  if relation name already in incore metadata, remove it & its fields 

	old_symbol = HSH_lookup(relation->rel_symbol->sym_string);

	while (old_symbol)
		if (old_symbol->sym_type == SYM_relation) {
			tmp_relation = (GPRE_REL) old_symbol->sym_object;
			if (tmp_relation->rel_meta)
				PAR_error("Multiple DECLARE TABLE statements for table");
			tmp_symbol = old_symbol->sym_homonym;
			HSH_remove(old_symbol);
			for (field = tmp_relation->rel_fields; field;
				 field = field->fld_next) HSH_remove(field->fld_symbol);
			old_symbol = tmp_symbol;
		}
		else
			old_symbol = old_symbol->sym_homonym;

//  add new symbol to incore metadata 

	HSH_insert(relation->rel_symbol);

//  create action block 

	action = (ACT) ALLOC(ACT_LEN);
	action->act_type = ACT_noop;
	action->act_object = (REF) relation;

//  parse field list and create corresponding field blocks 
//  include size information for message length calculations 

	ADVANCE_TOKEN;
	EXP_left_paren(0);
	count = 0;
	ptr = &relation->rel_fields;

	for (;;) {
		*ptr = field = par_field(request, relation);
		ptr = &field->fld_next;
		HSH_insert(field->fld_symbol);
		field->fld_position = count++;
		field->fld_flags &= ~FLD_meta;
		SQL_adjust_field_dtype(field);

		if (!MATCH(KW_COMMA))
			break;
	}

	EXP_match_paren();

	return action;
}


//____________________________________________________________
//  
//		Handle an SQL declare external statement
//  

static ACT act_declare_udf(void)
{
	ACT action;
	GPRE_REQ request;
	DECL_UDF udf_declaration;
	GPRE_FLD *ptr, field;
	SLONG return_parameter;

	request = MAKE_REQUEST(REQ_ddl);

	if (isc_databases && !isc_databases->dbb_next)
		request->req_database = isc_databases;
	else
		PAR_error
			("Can only DECLARE EXTERNAL FUNCTION in context of single database");

	udf_declaration = (DECL_UDF) ALLOC(DECL_UDF_LEN);
	udf_declaration->decl_udf_name = (TEXT *) ALLOC(NAME_SIZE + 1);
	SQL_resolve_identifier("<identifier>", udf_declaration->decl_udf_name);
	if (token.tok_length > NAME_SIZE)
		PAR_error("external function name too long");
	ADVANCE_TOKEN;

//  create action block 
	action = MAKE_ACTION(request, ACT_declare_udf);
	action->act_whenever = gen_whenever();
	action->act_object = (REF) udf_declaration;

	ptr = &udf_declaration->decl_udf_arg_list;
	while (TRUE) {
		if (MATCH(KW_RETURNS)) {
			if (MATCH(KW_PARAMETER)) {
				return_parameter = EXP_pos_USHORT_ordinal((USHORT) TRUE);
				if (return_parameter > 10)
					PAR_error("return parameter not in range");
				assert(return_parameter <= MAX_SSHORT);
				udf_declaration->decl_udf_return_parameter = (SSHORT) return_parameter;
			}
			else {
				field = (GPRE_FLD) ALLOC(FLD_LEN);
				field->fld_flags |= (FLD_meta | FLD_meta_cstring);
				SQL_par_field_dtype(request, field, TRUE);
				SQL_adjust_field_dtype(field);
				udf_declaration->decl_udf_return_type = field;
				MATCH(KW_BY);
				if (MATCH(KW_VALUE))
					udf_declaration->decl_udf_return_mode = FUN_value;
				else
					udf_declaration->decl_udf_return_mode = FUN_reference;
			}
			break;
		}
		else {
			field = (GPRE_FLD) ALLOC(FLD_LEN);
			field->fld_flags |= (FLD_meta | FLD_meta_cstring);
			SQL_par_field_dtype(request, field, TRUE);
			SQL_adjust_field_dtype(field);
			*ptr = field;
			ptr = &(field->fld_next);
			MATCH(KW_COMMA);
		}
	}

	if (MATCH(KW_ENTRY_POINT))
		udf_declaration->decl_udf_entry_point = extract_string(TRUE);
	else
		SYNTAX_ERROR("ENTRY_POINT");

	if (MATCH(KW_MODULE_NAME))
		udf_declaration->decl_udf_module_name = extract_string(TRUE);
	else
		SYNTAX_ERROR("MODULE_NAME");

	return action;
}


//____________________________________________________________
//  
//		Parse an update action.  This is a little more complicated
//		because SQL confuses the update of a cursor with a mass update.
//		The syntax, and therefor the code, I fear, is a mess.
//  

static ACT act_delete(void)
{
	ACT action;
	GPRE_REQ request;
	GPRE_REL relation;
	rse* selection;
	GPRE_CTX context;
	UPD update;
	SYM alias;
	TEXT *transaction, r_name[NAME_SIZE + 1], db_name[NAME_SIZE + 1],
		owner_name[NAME_SIZE + 1], *str_1, *str_2;
	SSHORT where, i, trans_nm_len;
	SSHORT hsh_rm = 0;

	par_options(&transaction);

	if (!MATCH(KW_FROM))
		SYNTAX_ERROR("FROM");

//  First comes the relation.  Unfortunately, there is no way to identify
//  its database until the cursor is known.  Sigh.  Save the token. 

	SQL_relation_name(r_name, db_name, owner_name);

//  Parse the optional alias (context variable) 

	alias = (token.tok_symbol) ? NULL : PAR_symbol(SYM_dummy);

//  Now the moment of truth.  If the next few tokens are WHERE CURRENT OF
//  then this is a sub-action of an existing request.  If not, then it is
//  a free standing request 

	request = MAKE_REQUEST(REQ_mass_update);
	update = (UPD) ALLOC(UPD_LEN);

	if ((where = MATCH(KW_WITH)) && MATCH(KW_CURRENT)) {
		if (!MATCH(KW_OF))
			SYNTAX_ERROR("OF <cursor>");
		requests = request->req_next;
		cur_routine->act_object = (REF) request->req_routine;
		FREE((UCHAR *) request);
		request = par_cursor(NULL);
		if ((transaction || request->req_trans) &&
			(!transaction || !request->req_trans ||
			 strcmp(transaction, request->req_trans))) {
			if (transaction)
				PAR_error("different transaction for select and delete");
			else {				/* does not specify transaction clause in      */
				/*   "delete ... where cuurent of cursor" stmt */
				trans_nm_len = strlen(request->req_trans);
				str_2 = transaction = (SCHAR *) ALLOC(trans_nm_len + 1);
				str_1 = request->req_trans;
				do
					*str_2++ = *str_1++;
				while (--trans_nm_len);
			}
		}
		request->req_trans = transaction;
		relation = SQL_relation(request, r_name, db_name, owner_name, TRUE);
		selection = request->req_rse;
		for (i = 0; i < selection->rse_count; i++) {
			context = selection->rse_context[i];
			if (context->ctx_relation == relation)
				break;
		}
		if (i == selection->rse_count)
			PAR_error("table not in request");
		update->upd_request = request;
		update->upd_source = context;
		action = MAKE_ACTION(request, ACT_erase);
		action->act_object = (REF) update;
		action->act_whenever = gen_whenever();
		return action;
	}

	request->req_trans = transaction;

//  How amusing.  After all that work, it wasn't a sub-action at all.
//  Neat.  Take the pieces and build a complete request.  Start by
//  figuring out what database is involved.  

	relation = SQL_relation(request, r_name, db_name, owner_name, TRUE);

	request->req_rse = selection = (rse*) ALLOC(RSE_LEN(1));
	selection->rse_count = 1;
	selection->rse_context[0] = context = MAKE_CONTEXT(request);
	context->ctx_relation = relation;

	if (alias && !token.tok_symbol) {
		alias->sym_type = SYM_context;
		alias->sym_object = context;
		context->ctx_symbol = alias;
		token.tok_symbol = alias;
		HSH_insert(alias);
		hsh_rm = 1;
	}

	if (where)
		selection->rse_boolean = SQE_boolean(request, 0);

	request->req_node = MAKE_NODE(nod_erase, 0);
	action = MAKE_ACTION(request, ACT_loop);
	action->act_whenever = gen_whenever();

	if (hsh_rm)
		HSH_remove(alias);

	return action;
}


//____________________________________________________________
//  
//		Handle an SQL describe statement.
//			Reject
//  

static ACT act_describe(void)
{
	DYN statement;
	ACT action;
	BOOLEAN in_sqlda;

	if (MATCH(KW_INPUT))
		in_sqlda = TRUE;
	else {
		MATCH(KW_OUTPUT);
		in_sqlda = FALSE;
	}

	statement = par_statement();

	if (!MATCH(KW_INTO)) {
		/* check for SQL2 syntax 
		   "USING SQL DESCRIPTOR sqlda" */

		if (!MATCH(KW_USING) || !MATCH(KW_SQL) || !MATCH(KW_DESCRIPTOR))
			SYNTAX_ERROR("INTO or USING SQL DESCRIPTOR sqlda");
	}
	else if (MATCH(KW_SQL) && !MATCH(KW_DESCRIPTOR))
		SYNTAX_ERROR("INTO SQL DESCRIPTOR sqlda");

	statement->dyn_sqlda = PAR_native_value(FALSE, FALSE);
	action = (ACT) ALLOC(ACT_LEN);
	if (in_sqlda)
		action->act_type = ACT_dyn_describe_input;
	else
		action->act_type = ACT_dyn_describe;
	action->act_object = (REF) statement;
	action->act_whenever = gen_whenever();

	return action;
}


//____________________________________________________________
//  
//		Parse a FINISH statement.
//  

static ACT act_disconnect(void)
{
	ACT action;
	SYM symbol;
	RDY ready;
	USHORT all;

	action = MAKE_ACTION(0, ACT_disconnect);
	action->act_whenever = gen_whenever();
	all = MATCH(KW_ALL) || MATCH(KW_DEFAULT);

	if (!all) {
		if (MATCH(KW_CURRENT))
			PAR_error("DISCONNECT CURRENT not supported");
		if (!
			((symbol = token.tok_symbol)
			 && (symbol->sym_type ==
				 SYM_database)))
SYNTAX_ERROR("ALL, DEFAULT, or <database handle>");
		while (TRUE) {
			if ((symbol = token.tok_symbol)
				&& (symbol->sym_type == SYM_database)) {
				ready = (RDY) ALLOC(RDY_LEN);
				ready->rdy_next = (RDY) action->act_object;
				action->act_object = (REF) ready;
				ready->rdy_database = (DBB) symbol->sym_object;
				ADVANCE_TOKEN;
			}
			else
				SYNTAX_ERROR("<database handle>");
			if (!MATCH(KW_COMMA))
				break;
		}
	}

	if (sw_language == lang_ada)
		MATCH(KW_SEMI_COLON);
	return action;
}


//____________________________________________________________
//  
//		Handle an SQL drop statement.
//  

static ACT act_drop(void)
{
	ACT action;
	DBB db;
	IND index;
	GPRE_REQ request;
	GPRE_REL relation;
	SYM symbol;
	SCHAR *db_string;
	TEXT *identifier_name;
	SLONG num;

	switch (token.tok_keyword) {
	case KW_DATABASE:
		PAR_error("DROP DATABASE not supported");

		request = MAKE_REQUEST(REQ_ddl);
		ADVANCE_TOKEN;
		if (!QUOTED(token.tok_type))
			SYNTAX_ERROR("<quoted database name>");
		db = (DBB) ALLOC(DBB_LEN);
		db->dbb_filename = db_string = (TEXT *) ALLOC(token.tok_length + 1);
		COPY(token.tok_string, token.tok_length, db_string);
		symbol = PAR_symbol(SYM_dummy);
		db->dbb_name = symbol;
		symbol->sym_type = SYM_database;
		symbol->sym_object = (GPRE_CTX) db;
		action = MAKE_ACTION(request, ACT_drop_database);
		action->act_whenever = gen_whenever();
		action->act_object = (REF) db;
		ADVANCE_TOKEN;
		return action;

	case KW_DOMAIN:
		request = MAKE_REQUEST(REQ_ddl);
		if (isc_databases && !isc_databases->dbb_next)
			request->req_database = isc_databases;
		else
			PAR_error("Can only DROP DOMAIN in context of single database");
		ADVANCE_TOKEN;
		identifier_name = (TEXT *) ALLOC(NAME_SIZE + 1);
		SQL_resolve_identifier("<identifier>", identifier_name);
		action = MAKE_ACTION(request, ACT_drop_domain);
		action->act_whenever = gen_whenever();
		action->act_object = (REF) identifier_name;
		ADVANCE_TOKEN;
		return action;

	case KW_FILTER:
		request = MAKE_REQUEST(REQ_ddl);
		if (isc_databases && !isc_databases->dbb_next)
			request->req_database = isc_databases;
		else
			PAR_error("Can only DROP FILTER in context of single database");
		ADVANCE_TOKEN;
		identifier_name = (TEXT *) ALLOC(NAME_SIZE + 1);
		SQL_resolve_identifier("<identifier>", identifier_name);
		action = MAKE_ACTION(request, ACT_drop_filter);
		action->act_whenever = gen_whenever();
		action->act_object = (REF) identifier_name;
		ADVANCE_TOKEN;
		return action;

	case KW_EXTERNAL:

		ADVANCE_TOKEN;
		if (!MATCH(KW_FUNCTION))
			SYNTAX_ERROR("FUNCTION");
		request = MAKE_REQUEST(REQ_ddl);
		if (isc_databases && !isc_databases->dbb_next)
			request->req_database = isc_databases;
		else
			PAR_error
				("Can only DROP EXTERNAL FUNCTION in context of a single database");

		identifier_name = (TEXT *) ALLOC(NAME_SIZE + 1);
		SQL_resolve_identifier("<identifier>", identifier_name);
		action = MAKE_ACTION(request, ACT_drop_udf);
		action->act_whenever = gen_whenever();
		action->act_object = (REF) identifier_name;
		ADVANCE_TOKEN;
		return action;

	case KW_INDEX:
		request = MAKE_REQUEST(REQ_ddl);
		ADVANCE_TOKEN;
		identifier_name = (TEXT *) ALLOC(NAME_SIZE + 1);
		SQL_resolve_identifier("<identifier>", identifier_name);
		index = make_index(request, token.tok_string);
		action = MAKE_ACTION(request, ACT_drop_index);
		action->act_whenever = gen_whenever();
		action->act_object = (REF) index;
		ADVANCE_TOKEN;
		return action;

	case KW_STOGROUP:
		PAR_error("DROP STOGROUP not supported");

	case KW_TABLESPACE:
		PAR_error("DROP TABLESPACE not supported");

	case KW_SYNONYM:
		PAR_error("DROP SYNONYM request not allowed");

	case KW_SHADOW:
		request = MAKE_REQUEST(REQ_ddl);
		if (isc_databases && !isc_databases->dbb_next)
			request->req_database = isc_databases;
		else
			PAR_error("Can only DROP SHADOW in context of a single database");
		ADVANCE_TOKEN;
		action = MAKE_ACTION(request, ACT_drop_shadow);
		action->act_whenever = gen_whenever();
		num = EXP_USHORT_ordinal(TRUE);
		if (!RANGE_POSITIVE_SHORT_INTEGER(num))
			PAR_error("Shadow number out of range");
		action->act_object = (REF) num;
		return action;

	case KW_TABLE:
		request = MAKE_REQUEST(REQ_ddl);
		ADVANCE_TOKEN;
		relation = par_relation(request);
		action = MAKE_ACTION(request, ACT_drop_table);
		action->act_whenever = gen_whenever();
		action->act_object = (REF) relation;
		return action;

	case KW_VIEW:
		request = MAKE_REQUEST(REQ_ddl);
		ADVANCE_TOKEN;
		relation = par_relation(request);
		action = MAKE_ACTION(request, ACT_drop_view);
		action->act_whenever = gen_whenever();
		action->act_object = (REF) relation;
		return action;

	default:
		PAR_error("Invalid DROP request");
	}
	return NULL;				/* silence compiler */
}


//____________________________________________________________
//  
//		Handle an SQL event statement
//  

static ACT act_event(void)
{
	ACT action;

	if (MATCH(KW_INIT))
		action = PAR_event_init((USHORT) TRUE);
	else if (MATCH(KW_WAIT))
		action = PAR_event_wait((USHORT) TRUE);
	else
		SYNTAX_ERROR("INIT or WAIT");
	action->act_whenever = gen_whenever();
	return action;
}


//____________________________________________________________
//  
//		Handle an SQL execute statement.
//			Reject
//  

static ACT act_execute(void)
{
	DYN statement;
	ACT action;
	TEXT *transaction, s[ERROR_LENGTH];

	if (MATCH(KW_PROCEDURE))
		return act_procedure();

//  EXECUTE IMMEDIATE is a different sort of duck 

	if (MATCH(KW_IMMEDIATE)) {
		if (isc_databases && isc_databases->dbb_next) {
			sprintf(s,
					"Executing dynamic SQL statement in context of database %s",
					isc_databases->dbb_name->sym_string);
			CPR_warn(s);
		}
		statement = (DYN) ALLOC(DYN_LEN);
		par_options(&statement->dyn_trans);

		switch (sw_sql_dialect) {
		case 1:
			if ((!QUOTED(token.tok_type)) && (!MATCH(KW_COLON)))
				SYNTAX_ERROR(": <string expression>");
			break;

		default:
			if ((!SINGLE_QUOTED(token.tok_type)) && (!MATCH(KW_COLON)))
				SYNTAX_ERROR(": <string expression>");
			break;
		}
		statement->dyn_string = PAR_native_value(FALSE, FALSE);
		par_using(statement);
		if (statement->dyn_using)
			PAR_error("Using host-variable list not supported.");
		par_into(statement);
		statement->dyn_database = isc_databases;
		action = (ACT) ALLOC(ACT_LEN);
		action->act_type = ACT_dyn_immediate;
		action->act_object = (REF) statement;
		action->act_whenever = gen_whenever();
		return action;
	}

//  Ordinary form of EXECUTE 

	par_options(&transaction);
	statement = par_statement();
	statement->dyn_trans = transaction;

	par_using(statement);
	if (statement->dyn_using)
		PAR_error("Using host-variable list not supported.");
	par_into(statement);

	action = (ACT) ALLOC(ACT_LEN);
	action->act_type = ACT_dyn_execute;
	action->act_object = (REF) statement;
	action->act_whenever = gen_whenever();

	return action;
}


//____________________________________________________________
//  
//		Parse the SQL fetch statement.
//  

static ACT act_fetch(void)
{
	GPRE_REQ request;
	ACT action;
	RSE select;
	DYN statement, cursor;
#ifdef SCROLLABLE_CURSORS
	USHORT direction;
	REF reference;
	VAL value;
	TEXT *direction_string, *offset_string, *string;
	GPRE_NOD offset_node = NULL;
#endif

//  Handle dynamic SQL statement, if appropriate 

	if (cursor = par_dynamic_cursor()) {
		statement = (DYN) ALLOC(DYN_LEN);
		statement->dyn_statement_name = cursor->dyn_statement_name;
		statement->dyn_cursor_name = cursor->dyn_cursor_name;
		if (MATCH(KW_USING) || MATCH(KW_INTO)) {
			MATCH(KW_SQL);		/* optional for backward compatibility */

#pragma FB_COMPILER_MESSAGE("Fix! Wrong function ptr type!")
//
// Please search for "reinterpret_cast<pfn_SQE_list_cb>",
// there are more in this file.
//
			if (MATCH(KW_DESCRIPTOR))
				statement->dyn_sqlda = PAR_native_value(FALSE, FALSE);
			else
				statement->dyn_using =
					(GPRE_NOD) SQE_list(reinterpret_cast < pfn_SQE_list_cb >
								   (SQE_variable), 0, FALSE);
			if (statement->dyn_using)
				PAR_error("Using host-variable list not supported.");
		}
		action = (ACT) ALLOC(ACT_LEN);
		action->act_type = ACT_dyn_fetch;
		action->act_object = (REF) statement;
		action->act_whenever = gen_whenever();
		return action;
	}

//  Statement is static SQL 

#ifdef SCROLLABLE_CURSORS
//  parse the fetch orientation 

	direction = blr_forward;

	if (!MATCH(KW_NEXT)) {
		if (MATCH(KW_PRIOR)) {
			direction = blr_backward;
			direction_string = "1";
		}
		else if (MATCH(KW_FIRST)) {
			direction = blr_bof_forward;
			direction_string = "2";
		}
		else if (MATCH(KW_LAST)) {
			direction = blr_eof_backward;
			direction_string = "3";
		}
		else if (MATCH(KW_RELATIVE)) {
			direction = blr_forward;
			direction_string = "0";
			offset_node = SQE_value(0, FALSE, 0, 0);
			ADVANCE_TOKEN;
		}
		else if (MATCH(KW_ABSOLUTE)) {
			direction = blr_bof_forward;
			direction_string = "2";
			offset_node = SQE_value(0, FALSE, 0, 0);
			ADVANCE_TOKEN;
		}
	}

	MATCH(KW_FROM);
#endif

	request = par_cursor(NULL);

#ifdef SCROLLABLE_CURSORS
//  if scrolling is required, set up the offset and direction parameters 
//  to be passed to the running request via the asynchronous message-- 
//  there could be multiple FETCH statements, so we need to store multiple 
//  value blocks, one for each FETCH statement 

	if (direction != blr_forward) {
		if (!(request->req_flags & REQ_scroll))
			PAR_error
				("Must use SCROLL modifier for DECLARE CURSOR to enable scrolling.");

		/* create a literal for the direction parameter */

		if (!(reference = request->req_avalues))
			reference = request->req_avalues = (REF) ALLOC(REF_LEN);

		if (!(value = reference->ref_values))
			reference->ref_values = value = (VAL) ALLOC(VAL_LEN);
		else {
			while (value->val_next)
				value = value->val_next;
			value->val_next = (VAL) ALLOC(VAL_LEN);
			value = value->val_next;
		}

		value->val_value = string = (TEXT *) ALLOC(2);
		COPY(direction_string, 1, string);

		/* create a reference to the offset variable or literal */

		if (!reference->ref_next)
			reference->ref_next = (REF) ALLOC(REF_LEN);
		reference = reference->ref_next;

		if (!(value = reference->ref_values))
			reference->ref_values = value = (VAL) ALLOC(VAL_LEN);
		else {
			while (value->val_next)
				value = value->val_next;
			value->val_next = (VAL) ALLOC(VAL_LEN);
			value = value->val_next;
		}

		if (offset_node)
			value->val_value = ((REF) offset_node->nod_arg[0])->ref_value;
		else {
			offset_string = "1";
			value->val_value = string = (TEXT *) ALLOC(2);
			COPY(offset_string, 1, string);
		}
	}
#endif

	if (request->req_flags & REQ_sql_blob_create)
		PAR_error("Fetching from a blob being created is not allowed.");

	action = MAKE_ACTION(request, ACT_fetch);
	action->act_whenever = gen_whenever();

	if (request->req_flags & REQ_sql_blob_open) {
		if (!MATCH(KW_INTO))
			SYNTAX_ERROR("INTO");
		action->act_object = SQE_variable(0, FALSE);
		action->act_type = ACT_get_segment;
	}
	else if (MATCH(KW_INTO)) {
		action->act_object =
			(REF) SQE_list(reinterpret_cast < pfn_SQE_list_cb >
						   (SQE_variable), request, FALSE);
		select = request->req_rse;
		into(request, select->rse_fields, (GPRE_NOD) action->act_object);
	}

	return action;
}


//____________________________________________________________
//  
//		Parse an SQL grant or revoke statement.  Set up grant/revoke
//		blocks, fill in all of the privilege information, and
//		attach them to an action block of type GRANT or REVOKE.
//  

static ACT act_grant_revoke( enum act_t type)
{
	GPRE_REQ request;
	ACT action;
	USN user, usernames;
	PRV priv_block, last_priv_block;
	GPRE_REL relation;
	GPRE_PRC procedure;
	USHORT user_dyn;
	SCHAR col_name[NAME_SIZE + 1], r_name[NAME_SIZE + 1],
		db_name[NAME_SIZE + 1], owner_name[NAME_SIZE + 1];
	LLS *fields;
	STR field_name, relation_name;
	BOOLEAN first;
	BOOLEAN grant_option_legal = TRUE;
	BOOLEAN execute_priv = FALSE;
	SCHAR s[ERROR_LENGTH];

	request = MAKE_REQUEST(REQ_ddl);

	priv_block = MAKE_PRIVILEGE_BLOCK();
	fields = &priv_block->prv_fields;

//  if it is revoke action, parse the optional grant option for 

	if (type == ACT_dyn_revoke)
		if (MATCH(KW_GRANT)) {
			if (!MATCH(KW_OPTION))
				SYNTAX_ERROR("OPTION");
			if (!MATCH(KW_FOR))
				SYNTAX_ERROR("FOR");
			priv_block->prv_privileges |= PRV_grant_option;
		}

	if (MATCH(KW_ALL)) {
		MATCH(KW_PRIVILEGES);	/*  Keyword 'privileges' is optional  */
		priv_block->prv_privileges = PRV_all;
	}
	else if (MATCH(KW_EXECUTE)) {
		priv_block->prv_privileges |= PRV_execute;
		execute_priv = TRUE;
	}
	else
		while (TRUE) {
			if (MATCH(KW_SELECT))
				priv_block->prv_privileges |= PRV_select;
			else if (MATCH(KW_INSERT))
				priv_block->prv_privileges |= PRV_insert;
			else if (MATCH(KW_DELETE))
				priv_block->prv_privileges |= PRV_delete;
			else if (MATCH(KW_UPDATE)) {
				priv_block->prv_privileges |= PRV_update;
				if (MATCH(KW_LEFT_PAREN)) {
					do {
						SQL_resolve_identifier("<column name>", col_name);
						field_name = (STR) MAKE_STRING(col_name);
						PUSH((GPRE_NOD) field_name, fields);
						fields = &(*fields)->lls_next;
						CPR_token();
					} while (MATCH(KW_COMMA));

					if (!MATCH(KW_RIGHT_PAREN))
						SYNTAX_ERROR("<right parenthesis>");
				}
			}

			if (!MATCH(KW_COMMA))
				break;
		}

	if (!MATCH(KW_ON))
		SYNTAX_ERROR("ON");

	if (execute_priv) {
		if (!MATCH(KW_PROCEDURE))
			SYNTAX_ERROR("PROCEDURE");
		SQL_relation_name(r_name, db_name, owner_name);
		procedure = SQL_procedure(request, r_name, db_name, owner_name, TRUE);
		relation_name = (STR) MAKE_STRING(r_name);
		priv_block->prv_relation = relation_name;
		priv_block->prv_object_dyn = gds_dyn_prc_name;
	}
	else {
		MATCH(KW_TABLE);		/* filler word */
		SQL_relation_name(r_name, db_name, owner_name);
		relation = SQL_relation(request, r_name, db_name, owner_name, TRUE);
		relation_name = (STR) MAKE_STRING(r_name);
		priv_block->prv_relation = relation_name;
		priv_block->prv_object_dyn = gds_dyn_rel_name;
	}

	if (type == ACT_dyn_grant) {
		if (!MATCH(KW_TO))
			SYNTAX_ERROR("TO");
	}
	else {						/*  type == ACT_dyn_revoke  */

		if (!MATCH(KW_FROM))
			SYNTAX_ERROR("FROM");
	}

	usernames = 0;
	user = 0;
	while (TRUE) {
		if (MATCH(KW_PROCEDURE)) {
			SQL_relation_name(r_name, db_name, owner_name);
			procedure =
				SQL_procedure(request, r_name, db_name, owner_name, TRUE);
			user_dyn = gds_dyn_grant_proc;
			grant_option_legal = FALSE;
		}
		else if (MATCH(KW_TRIGGER)) {
			SQL_relation_name(r_name, db_name, owner_name);
			if (!MET_trigger_exists(request->req_database, r_name)) {
				sprintf(s, "TRIGGER %s not defined", r_name);
				PAR_error(s);
			}
			user_dyn = gds_dyn_grant_trig;
			grant_option_legal = FALSE;
		}
		else if (MATCH(KW_VIEW)) {
			SQL_relation_name(r_name, db_name, owner_name);
			if (!MET_get_view_relation
				(request, r_name, (char *) relation_name, 0)) {
				sprintf(s, "VIEW %s not defined on table %s", r_name,
						relation_name->str_string);
				PAR_error(s);
			}
			user_dyn = gds_dyn_grant_view;
			grant_option_legal = FALSE;
		}
		else if (MATCH(KW_PUBLIC)) {
			strcpy(r_name, "PUBLIC");
			user_dyn = gds_dyn_grant_user;
		}
		else {
			MATCH(KW_USER);
			if (token.tok_type != tok_ident)
				SYNTAX_ERROR("<user name identifier>");
			else
				to_upcase(token.tok_string, r_name);
			user_dyn = gds_dyn_grant_user;
			CPR_token();
		}

		if (!usernames) {
			usernames = MAKE_USERNAME(r_name, user_dyn);
			user = usernames;
		}
		else {
			user->usn_next = MAKE_USERNAME(r_name, user_dyn);
			user = user->usn_next;
		}

		if (!MATCH(KW_COMMA))
			break;
	}

//  If this is a grant, do we have the optional WITH GRANT OPTION specification? 

	if ((type == ACT_dyn_grant) && grant_option_legal)
		if (MATCH(KW_WITH)) {
			if (!MATCH(KW_GRANT))
				SYNTAX_ERROR("GRANT");
			if (!MATCH(KW_OPTION))
				SYNTAX_ERROR("OPTION");
			priv_block->prv_privileges |= PRV_grant_option;
		}

//  create action block 

	action = MAKE_ACTION(request, type);
	action->act_next = 0;

	last_priv_block = priv_block;
	first = TRUE;

	for (user = usernames; user; user = user->usn_next) {
		/* create and fill privilege block */

		priv_block = MAKE_PRIVILEGE_BLOCK();
		priv_block->prv_username = user->usn_name;
		priv_block->prv_user_dyn = user->usn_dyn;
		priv_block->prv_privileges = last_priv_block->prv_privileges;
		priv_block->prv_relation = last_priv_block->prv_relation;
		priv_block->prv_object_dyn = last_priv_block->prv_object_dyn;
		priv_block->prv_fields = last_priv_block->prv_fields;
		if (first) {
			action->act_object = (REF) priv_block;
			first = FALSE;
		}
		else
			last_priv_block->prv_next = priv_block;
		last_priv_block = priv_block;
	}
	action->act_request = request;
	action->act_whenever = gen_whenever();

	return action;
}


//____________________________________________________________
//  
//  

static ACT act_include(void)
{
	ACT action;
	SYM symbol;

	ADVANCE_TOKEN;
	MATCH(KW_SEMI_COLON);

	action = (ACT) ALLOC(ACT_LEN);
	action->act_type = ACT_b_declare;
	cur_routine = action;

	if (!isc_databases) {
		/* allocate database block and link to db chain */

		isc_databases = (DBB) MSC_alloc_permanent(DBB_LEN);

		/* allocate symbol block */

		symbol = (SYM) MSC_alloc_permanent(SYM_LEN);

		/* make it a database, specifically this one */

		symbol->sym_type = SYM_database;
		symbol->sym_object = (GPRE_CTX) isc_databases;
		symbol->sym_string = database_name;

		/* database block points to the symbol block */

		isc_databases->dbb_name = symbol;
		isc_databases->dbb_filename = NULL;
		isc_databases->dbb_flags = DBB_sqlca;
		isc_databases->dbb_c_lc_ctype = module_lc_ctype;
		if (sw_external)
			isc_databases->dbb_scope = DBB_EXTERN;
	}
	else
		/* Load the symbol (hash) table with relation names from this database. */
		MET_load_hash_table(isc_databases);

	HSH_insert(isc_databases->dbb_name);

	return action;
}


//____________________________________________________________
//  
//		Process SQL INSERT statement.
//  

static ACT act_insert(void)
{
	GPRE_REQ request;
	ACT action;
	GPRE_CTX context;
	LLS fields, values;
	RSE select;
	GPRE_NOD node, store, assignment, list, select_list, *ptr, *ptr2;
	TEXT *transaction;
	int count, count2, i;

	par_options(&transaction);

	if (MATCH(KW_CURSOR))
		return act_insert_blob(transaction);

	if (!MATCH(KW_INTO))
		SYNTAX_ERROR("INTO");

	request = MAKE_REQUEST(REQ_insert);
	request->req_trans = transaction;
	context = SQE_context(request);
	count = count2 = 0;
	fields = values = NULL;

//  Pick up a field list 

	if (!MATCH(KW_LEFT_PAREN)) {
		list = MET_fields(context);
		count = list->nod_count;
		for (i = 0; i < count; i++)
			PUSH(list->nod_arg[i], &fields);
	}
	else {
		do {
			node = SQE_field(request, FALSE);
			if (node->nod_type == nod_array) {
				node->nod_type = nod_field;

				/* Make sure no subscripts are specified  */

				if (node->nod_arg[1]) {
					PAR_error("Partial insert of arrays not permitted");
				}
			}

			/* Dialect 1 program may not insert new datatypes */
			if ((SQL_DIALECT_V5 == sw_sql_dialect) &&
				(nod_field == node->nod_type)) {
				USHORT field_dtype;

				field_dtype =
					((REF) (node->nod_arg[0]))->ref_field->fld_dtype;
				if ((dtype_sql_date == field_dtype)
					|| (dtype_sql_time == field_dtype)
					|| (dtype_int64 == field_dtype)) {
					dialect1_bad_type(field_dtype);
				}
			}

			PUSH(node, &fields);
			count++;
		} while (MATCH(KW_COMMA));

		EXP_match_paren();
	}

	if (MATCH(KW_VALUES)) {
		/* Now pick up a value list */

		EXP_left_paren(0);
		for (;;) {
			if (MATCH(KW_NULL))
				PUSH(MAKE_NODE(nod_null, 0), &values);
			else
				PUSH(SQE_value(request, FALSE, 0, 0), &values);
			count2++;
			if (!(MATCH(KW_COMMA)))
				break;
		}
		EXP_match_paren();

		/* Make an assignment list */

		if (count != count2)
			PAR_error("count of values doesn't match count of columns");

		request->req_node = list = MAKE_NODE(nod_list, (SSHORT) count);
		ptr = &list->nod_arg[count];

		while (values) {
			assignment = MAKE_NODE(nod_assignment, 2);
			assignment->nod_arg[0] = (GPRE_NOD) POP(&values);
			assignment->nod_arg[1] = (GPRE_NOD) POP(&fields);
			pair(assignment->nod_arg[0], assignment->nod_arg[1]);
			*--ptr = assignment;
		}

		if (context->ctx_symbol)
			HSH_remove(context->ctx_symbol);
		action = MAKE_ACTION(request, ACT_insert);
		action->act_whenever = gen_whenever();
		return action;
	}

	if (!MATCH(KW_SELECT))
		SYNTAX_ERROR("VALUES or SELECT");

//  OK, we've got a mass insert on our hands.  Start by picking
//  up the select statement.  First, however, remove the INSERT
//  context to avoid resolving SELECT fields to the insert relation. 

	request->req_type = REQ_mass_update;
	request->req_contexts = NULL;
	request->req_update = context;
	request->req_rse = select = SQE_select(request, FALSE);
	context->ctx_next = request->req_contexts;
	request->req_contexts = context;

//  Build an assignment list from select expressions into target list 

	select_list = select->rse_fields;

	if (count != select_list->nod_count)
		PAR_error("count of values doesn't match count of columns");

	request->req_node = list = MAKE_NODE(nod_list, (SSHORT) count);
	ptr = &list->nod_arg[count];
	ptr2 = &select_list->nod_arg[count];

	while (fields) {
		assignment = MAKE_NODE(nod_assignment, 2);
		assignment->nod_arg[0] = *--ptr2;
		assignment->nod_arg[1] = (GPRE_NOD) POP(&fields);
		pair(assignment->nod_arg[0], assignment->nod_arg[1]);
		*--ptr = assignment;
	}

	store = MSC_binary(nod_store, (GPRE_NOD) context, list);
	request->req_node = store;
	EXP_rse_cleanup(select);
	if (context->ctx_symbol)
		HSH_remove(context->ctx_symbol);

	action = MAKE_ACTION(request, ACT_loop);
	action->act_whenever = gen_whenever();

	return action;
}


//____________________________________________________________
//  
//		Process SQL INSERT statement.
//  

static ACT act_insert_blob( TEXT * transaction)
{
	GPRE_REQ request;
	ACT action;
	DYN statement, cursor;

//  Handle dynamic SQL statement, if appropriate 

	if (cursor = par_dynamic_cursor()) {
		statement = (DYN) ALLOC(DYN_LEN);
		statement->dyn_statement_name = cursor->dyn_statement_name;
		statement->dyn_cursor_name = cursor->dyn_cursor_name;
		par_using(statement);
		if (statement->dyn_using)
			PAR_error("Using host-variable list not supported.");
		action = (ACT) ALLOC(ACT_LEN);
		action->act_type = ACT_dyn_insert;
		action->act_object = (REF) statement;
		action->act_whenever = gen_whenever();
		return action;
	}

//  Statement is static SQL 

	request = par_cursor(NULL);
	if (request->req_flags & REQ_sql_blob_open)
		PAR_error("Inserting into a blob being opened is not allowed.");
	action = MAKE_ACTION(request, ACT_put_segment);
	action->act_whenever = gen_whenever();

	if (!MATCH(KW_VALUES))
		SYNTAX_ERROR("VALUES");

	EXP_left_paren(0);
	action->act_object = SQE_variable(0, FALSE);
	if (!action->act_object->ref_null_value)
		PAR_error("A segment length is required.");
	EXP_match_paren();

	return action;
}


//____________________________________________________________
//  
//		Handle an SQL lock statement.
//			Reject
//  

static ACT act_lock(void)
{

	PAR_error("SQL LOCK TABLE request not allowed");
	return NULL;				/* silence compiler */
}


//____________________________________________________________
//  
//		Handle the SQL actions OPEN and CLOSE cursors.
//  

static ACT act_openclose( enum act_t type)
{
	ACT action;
	GPRE_REQ request;
	DYN statement, cursor;
	SYM symbol;
	OPN open;
	REF using_;
	TEXT *transaction;

	if (type == ACT_open)
		par_options(&transaction);

//  Handle dynamic SQL statement, if appropriate 

	if (cursor = par_dynamic_cursor()) {
		statement = (DYN) ALLOC(DYN_LEN);
		statement->dyn_statement_name = cursor->dyn_statement_name;
		statement->dyn_cursor_name = cursor->dyn_cursor_name;
		action = (ACT) ALLOC(ACT_LEN);
		action->act_object = (REF) statement;
		action->act_whenever = gen_whenever();
		if (type == ACT_open) {
			action->act_type = ACT_dyn_open;
			statement->dyn_trans = transaction;
			par_using(statement);
			if (statement->dyn_using)
				PAR_error("Using host-variable list not supported.");
			par_into(statement);
		}
		else
			action->act_type = ACT_dyn_close;
		return action;
	}

//  Statement is static SQL 

	request = par_cursor(&symbol);

	action = MAKE_ACTION(request, type);
	if (type == ACT_open) {
		open = (OPN) ALLOC(OPN_LEN);
		open->opn_trans = transaction;
		open->opn_cursor = symbol;
		action->act_object = (REF) open;
		if (transaction != NULL)
			request->req_trans = transaction;
		if (request->req_flags & (REQ_sql_blob_open | REQ_sql_blob_create)) {
			if (request->req_flags & REQ_sql_blob_open) {
				if (!MATCH(KW_USING))
					SYNTAX_ERROR("USING");
			}
			else if (!MATCH(KW_INTO))
				SYNTAX_ERROR("INTO");
			open->opn_using = using_ = SQE_variable(0, FALSE);
			using_->ref_next = request->req_blobs->blb_reference;
			request->req_blobs->blb_reference = using_;
			using_->ref_context = request->req_contexts;
			using_->ref_field = request->req_references->ref_field;
		}
	}
	else
		action->act_object = (REF) symbol;
	action->act_whenever = gen_whenever();

	if (request->req_flags & (REQ_sql_blob_open | REQ_sql_blob_create))
		action->act_type = (type == ACT_close) ? ACT_blob_close :
			(request->
			 req_flags & REQ_sql_blob_open) ? ACT_blob_open : ACT_blob_create;

	return action;
}


//____________________________________________________________
//  
//		Parse an "open blob" type statement.
//		These include READ BLOB and INSERT BLOB.
//  

static ACT act_open_blob( ACT_T act_op, SYM symbol)
{
	TOK f_token;
	SCHAR r_name[NAME_SIZE + 1], db_name[NAME_SIZE + 1],
		owner_name[NAME_SIZE + 1];
	GPRE_REQ request;
	GPRE_REL relation;
	GPRE_FLD field;
	REF reference;
	BLB blob;
	GPRE_CTX context;
	SCHAR s[128];
	ACT action;

	if (!MATCH(KW_BLOB))
		SYNTAX_ERROR("BLOB");

//  if the token isn't an identifier, complain 

	f_token = (TOK) ALLOC(TOK_LEN);
	f_token->tok_length = token.tok_length;

	SQL_resolve_identifier("<column_name>", f_token->tok_string);
	CPR_token();

	if (act_op == ACT_blob_open) {
		if (!MATCH(KW_FROM))
			SYNTAX_ERROR("FROM");
	}
	else if (!MATCH(KW_INTO))
		SYNTAX_ERROR("INTO");

	request = MAKE_REQUEST(REQ_cursor);
	request->req_flags =
		(act_op == ACT_blob_open) ? REQ_sql_blob_open : REQ_sql_blob_create;

	SQL_relation_name(r_name, db_name, owner_name);
	relation = SQL_relation(request, r_name, db_name, owner_name, TRUE);

	if (!(field = MET_field(relation, f_token->tok_string))) {
		sprintf(s, "column \"%s\" not in context", f_token->tok_string);
		PAR_error(s);
	}

	if (!(field->fld_flags & FLD_blob)) {
		sprintf(s, "column %s is not a BLOB", field->fld_symbol->sym_string);
		PAR_error(s);
	}

	if (field->fld_array_info) {
		sprintf(s, "column %s is an array and can not be opened as a BLOB",
				field->fld_symbol->sym_string);
		PAR_error(s);
	}

	reference = MAKE_REFERENCE(0);
	reference->ref_field = field;

	context = MAKE_CONTEXT(request);
	context->ctx_relation = relation;

	blob = (BLB) ALLOC(BLB_LEN);
	blob->blb_symbol = symbol;
	blob->blb_request = request;
	blob->blb_next = request->req_blobs;
	symbol->sym_object = (GPRE_CTX) request;
	symbol->sym_type = SYM_cursor;

	request->req_references = reference;
	request->req_blobs = blob;

	if (MATCH(KW_FILTER)) {
		if (MATCH(KW_FROM)) {
			blob->blb_const_from_type =
				PAR_blob_subtype(request->req_database);
			if (token.tok_keyword == KW_CHAR)
				if (blob->blb_const_from_type == BLOB_text) {
					blob->blb_from_charset = par_char_set();
					if (act_op == ACT_blob_open
						&& blob->blb_from_charset != field->fld_charset_id)
							PAR_error
							("Specified CHARACTER SET does not match BLOB column declaration.");
				}
				else
					PAR_error("Only text BLOBS can specify CHARACTER SET");
			else if (blob->blb_const_from_type == BLOB_text)
				if (act_op == ACT_blob_create)
					blob->blb_from_charset = CS_dynamic;
				else
					blob->blb_from_charset = field->fld_charset_id;
		}
		else {
			blob->blb_const_from_type = field->fld_sub_type;
			if (blob->blb_const_from_type == BLOB_text)
				if (act_op == ACT_blob_create)
					blob->blb_from_charset = CS_dynamic;
				else
					blob->blb_from_charset = field->fld_charset_id;
		}
		if (!MATCH(KW_TO))
			SYNTAX_ERROR("TO");
		blob->blb_const_to_type = PAR_blob_subtype(request->req_database);
		if (token.tok_keyword == KW_CHAR)
			if (blob->blb_const_to_type == BLOB_text) {
				blob->blb_to_charset = par_char_set();
				if (act_op == ACT_blob_create
					&& blob->blb_to_charset != field->fld_charset_id)
						PAR_error
						("Specified CHARACTER SET does not match BLOB column declaration.");
			}
			else
				PAR_error("Only text BLOBS can specify CHARACTER SET");
		else if (blob->blb_const_to_type == BLOB_text)
			if (act_op == ACT_blob_create)
				blob->blb_to_charset = field->fld_charset_id;
			else
				blob->blb_to_charset = CS_dynamic;
	}
	else {						/* No FILTER keyword seen */

		/*
		 *  Even if no FILTER was specified, we set one up for the special
		 *  case of InterBase TEXT blobs, in order to do character set
		 *  transliteration from the column-declared character set of the
		 *  blob to the process character set (CS_dynamic).
		 *
		 *  It is necessary to pass this information in the bpb as blob
		 *  operations are done using blob_ids, and we cannot determine
		 *  this information from the blob_id within the engine.
		 */
		if (field->fld_sub_type == BLOB_text
			&& (field->fld_charset_id != CS_NONE)) {
			blob->blb_const_from_type = BLOB_text;
			blob->blb_const_to_type = BLOB_text;
			if (act_op == ACT_blob_create) {
				blob->blb_from_charset = CS_dynamic;
				blob->blb_to_charset = field->fld_charset_id;
			}
			else {
				blob->blb_from_charset = field->fld_charset_id;
				blob->blb_to_charset = CS_dynamic;
			}
		}
	}

	if (MATCH(KW_MAX_SEGMENT))
		blob->blb_seg_length = EXP_USHORT_ordinal(TRUE);
	else
		blob->blb_seg_length = field->fld_seg_length;

	if (!blob->blb_seg_length)
		blob->blb_seg_length = 512;

	action = MAKE_ACTION(request, ACT_cursor);
	action->act_object = (REF) blob;

	return action;
}


//____________________________________________________________
//  
//		Handle an SQL prepare statement.
//  

static ACT act_prepare(void)
{
	DYN statement;
	ACT action;
	TEXT s[ERROR_LENGTH];
	TEXT *transaction;

	if (isc_databases && isc_databases->dbb_next) {
		sprintf(s,
				"Executing dynamic SQL statement in context of database %s",
				isc_databases->dbb_name->sym_string);
		CPR_warn(s);
	}

	par_options(&transaction);
	statement = par_statement();
	statement->dyn_database = isc_databases;
	statement->dyn_trans = transaction;

	if (MATCH(KW_INTO)) {
		if (MATCH(KW_SQL) && !MATCH(KW_DESCRIPTOR))
			SYNTAX_ERROR("INTO SQL DESCRIPTOR sqlda");
		statement->dyn_sqlda = PAR_native_value(FALSE, FALSE);
	}

	if (!MATCH(KW_FROM))
		SYNTAX_ERROR("FROM");

	switch (sw_sql_dialect) {
	case 1:
		if ((!QUOTED(token.tok_type)) && (!MATCH(KW_COLON)))
			SYNTAX_ERROR(": <string expression>");
		break;

	default:
		if ((!SINGLE_QUOTED(token.tok_type)) && (!MATCH(KW_COLON)))
			SYNTAX_ERROR(": <string expression>");
		break;
	}

	statement->dyn_string = PAR_native_value(FALSE, FALSE);
	action = (ACT) ALLOC(ACT_LEN);
	action->act_type = ACT_dyn_prepare;
	action->act_object = (REF) statement;
	action->act_whenever = gen_whenever();
	sw_dsql = TRUE;

	return action;
}


//____________________________________________________________
//  
//		Handle the EXECUTE PROCEDURE statement.
//  

static ACT act_procedure(void)
{
	GPRE_PRC procedure;
	GPRE_REQ request;
	ACT action;
	REF reference, *ref_ptr;
	GPRE_FLD field;
	SSHORT inputs, outputs;
	LLS values;
	GPRE_NOD list, *ptr;
	SCHAR p_name[NAME_SIZE + 1], db_name[NAME_SIZE + 1],
		owner_name[NAME_SIZE + 1];
	BOOLEAN paren;

	request = MAKE_REQUEST(REQ_procedure);
	par_options(&request->req_trans);
	SQL_relation_name(p_name, db_name, owner_name);
	procedure = SQL_procedure(request, p_name, db_name, owner_name, TRUE);
	inputs = outputs = 0;
	values = NULL;

	if (!KEYWORD(KW_RETURNING) && !KEYWORD(KW_SEMI_COLON)) {
		/* parse input references */

		paren = MATCH(KW_LEFT_PAREN);
		field = procedure->prc_inputs;
		ref_ptr = &request->req_values;
		do {
			if (MATCH(KW_NULL))
				PUSH(MAKE_NODE(nod_null, 0), &values);
			else {
				*ref_ptr = reference = SQE_parameter(request, FALSE);
				reference->ref_field = field;
				PUSH(MSC_unary(nod_value, (GPRE_NOD) reference), &values);
				ref_ptr = &reference->ref_next;
			}
			if (field)
				field = field->fld_next;
			inputs++;
		} while (MATCH(KW_COMMA));
		if (paren)
			EXP_match_paren();
	}

	if (MATCH(KW_RETURNING)) {
		/* parse output references */

		paren = MATCH(KW_LEFT_PAREN);
		field = procedure->prc_outputs;
		ref_ptr = &request->req_references;
		do {
			*ref_ptr = reference = SQE_variable(request, FALSE);
			if (reference->ref_field = field)
				field = field->fld_next;
			ref_ptr = &reference->ref_next;
			outputs++;
		} while (MATCH(KW_COMMA));
		if (paren)
			EXP_match_paren();
	}

	if (procedure->prc_in_count != inputs)
		PAR_error("count of input values doesn't match count of parameters");
	if (procedure->prc_out_count != outputs)
		PAR_error("count of output values doesn't match count of parameters");

	request->req_node = list = MAKE_NODE(nod_list, inputs);
	ptr = &list->nod_arg[inputs];
	while (values)
		*--ptr = (GPRE_NOD) POP(&values);

	action = MAKE_ACTION(request, ACT_procedure);
	action->act_object = (REF) procedure;
	action->act_whenever = gen_whenever();

	return action;
}


//____________________________________________________________
//  
//		Handle the stand alone SQL select statement.
//  

static ACT act_select(void)
{
	GPRE_REQ request;
	ACT action;
	RSE select;
	TEXT s[ERROR_LENGTH];

	request = MAKE_REQUEST(REQ_for);
	par_options(&request->req_trans);
	request->req_rse = select = SQE_select(request, FALSE);

	if (!MATCH(KW_SEMI_COLON)) {
		sprintf(s, "Expected ';', got %s.", token.tok_string);
		CPR_warn(s);
	}

	if (select->rse_into)
		into(request, select->rse_fields, select->rse_into);

	action = MAKE_ACTION(request, ACT_select);
	action->act_object = (REF) select->rse_into;
	action->act_whenever = gen_whenever();
	EXP_rse_cleanup(select);
	return action;
}


//____________________________________________________________
//  
//		Parse a SET <something>
//  

static ACT act_set(TEXT* base_directory)
{

	if (MATCH(KW_TRANSACTION))
		return act_set_transaction();

	if (MATCH(KW_NAMES))
		return act_set_names();

	if (MATCH(KW_STATISTICS))
		return act_set_statistics();

	if (MATCH(KW_SCHEMA) || MATCH(KW_DATABASE))
		return PAR_database((USHORT) TRUE, base_directory);

	if (MATCH(KW_GENERATOR))
		return act_set_generator();

	if (MATCH(KW_SQL)) {
		if (MATCH(KW_DIALECT))
			return act_set_dialect();
	}

	SYNTAX_ERROR
		("TRANSACTION, NAMES, SCHEMA, DATABASE, GENERATOR, DIALECT or STATISTICS");
	return NULL;				/* silence compiler */
}


//____________________________________________________________
//  
//		Parse a SET SQL DIALECT
//  

static ACT act_set_dialect(void)
{
	ACT action;
	USHORT dialect;

	action = (ACT) ALLOC(ACT_LEN);
	action->act_type = ACT_sql_dialect;

	dialect = EXP_USHORT_ordinal(0 /*FALSE*/);
	if ((dialect < 1) || (dialect > 3))
		SYNTAX_ERROR("SQL DIALECT 1,2 or 3");

	if (isc_databases && dialect != compiletime_db_dialect
		&& sw_ods_version < 10) {
		char warn_mesg[100];
		sprintf(warn_mesg,
				"Pre 6.0 database. Cannot use dialect %d, Resetting to %d\n",
				dialect, SQL_DIALECT_V5);
		dialect = SQL_DIALECT_V5;
		CPR_warn(warn_mesg);
	}
	else if (isc_databases && dialect != compiletime_db_dialect) {
		char warn_mesg[100];
		sprintf(warn_mesg,
				"Client dialect set to %d. Compiletime database dialect is %d\n",
				dialect, compiletime_db_dialect);
		CPR_warn(warn_mesg);
	}

	action->act_object = (REF) ALLOC(SDT_LEN);
	((SDT) action->act_object)->sdt_dialect = dialect;

//  Needed because subsequent parsing pass1 looks at sw_Sql_dialect value 
	sw_sql_dialect = dialect;
	dialect_specified = 1;

	ADVANCE_TOKEN;
	return action;
}


//____________________________________________________________
//  
//		Parse a SET generator
//  

static ACT act_set_generator(void)
{
	ACT action;
	SGEN setgen;
	GPRE_REQ request;
	SCHAR s[128];

	request = MAKE_REQUEST(REQ_set_generator);
	action = (ACT) MAKE_ACTION(request, ACT_s_start);
	setgen = (SGEN) ALLOC(SGEN_LEN);

	if (isc_databases && !isc_databases->dbb_next)
		request->req_database = isc_databases;
	else
		PAR_error("Can SET GENERATOR in context of single database only");

	if (token.tok_length > NAME_SIZE)
		PAR_error("Generator name too long");

	setgen->sgen_name = (TEXT *) ALLOC(token.tok_length + 1);
	SQL_resolve_identifier("<identifier>", setgen->sgen_name);
	if (!MET_generator(setgen->sgen_name, request->req_database)) {
		sprintf(s, "generator %s not found", token.tok_string);
		PAR_error(s);
	}
	ADVANCE_TOKEN;
	MATCH(KW_TO);
	if ((sw_sql_dialect == SQL_DIALECT_V5) || (sw_server_version < 6)) {
		setgen->sgen_value = EXP_SLONG_ordinal(TRUE);
		setgen->sgen_dialect = SQL_DIALECT_V5;
	}
	else {
// ** dialect is > 1. Server can handle int64 *

		setgen->sgen_int64value = EXP_SINT64_ordinal(TRUE);
		setgen->sgen_dialect = SQL_DIALECT_V5 + 1;
	}
	action->act_whenever = gen_whenever();
	action->act_object = (REF) setgen;
	return action;
}


//____________________________________________________________
//  
//		Parse a SET NAMES <charset>;
//  

static ACT act_set_names(void)
{
	ACT action;
	TEXT *value;
	DBB db;
	char buffer[256];

	if (sw_auto)
		CPR_warn("SET NAMES requires -manual switch to gpre.");

	action = (ACT) ALLOC(ACT_LEN);
	action->act_type = ACT_noop;

	if (MATCH(KW_COLON)) {
		/* User is specifying a host variable or string as
		 * the character set.  Make this the run-time set.
		 */
		value = PAR_native_value(FALSE, FALSE);
		for (db = isc_databases; db; db = db->dbb_next) {
			if (db->dbb_r_lc_ctype) {
				sprintf(buffer,
						"Supersedes runtime character set for database %s",
						(db->dbb_filename) ? db->dbb_filename : db->dbb_name->
						sym_string);
				CPR_warn(buffer);
			}
			db->dbb_r_lc_ctype = value;
		}
	}
	else if (token.tok_type == tok_ident) {
		/* User is specifying the name of a character set */
		/* Make this the compile time character set */

		value = (TEXT *) ALLOC(token.tok_length + 1);
		COPY(token.tok_string, token.tok_length, value);
		value[token.tok_length] = '\0';

		/* Due to the ambiguities involved in having modules expressed
		 * in multiple compile-time character sets, we disallow it.
		 */
		if (module_lc_ctype && strcmp(module_lc_ctype, value) != 0)
			PAR_error("Duplicate declaration of module CHARACTER SET");

		module_lc_ctype = value;
		for (db = isc_databases; db; db = db->dbb_next) {
			if (db->dbb_c_lc_ctype) {
				sprintf(buffer, "Supersedes character set for database %s",
						(db->dbb_filename) ? db->dbb_filename : db->dbb_name->
						sym_string);
				CPR_warn(buffer);
			}

			db->dbb_c_lc_ctype = value;
			if (!(db->dbb_flags & DBB_sqlca)) {
				/* Verify that character set name is valid,
				 * this requires a database to be previously declared
				 * so we can resolve against it.
				 * So what if we go through this code once for each database...
				 */
				if (!(MSC_find_symbol(token.tok_symbol, SYM_charset)))
					PAR_error("The named CHARACTER SET was not found");
			}
		}
		CPR_token();
	}
	else
		SYNTAX_ERROR("<character set name> or :<hostvar>");

	return action;
}


//____________________________________________________________
//  
//		Parse a SET statistics
//  

static ACT act_set_statistics(void)
{
	ACT action;
	STS stats;
	GPRE_REQ request;

	request = MAKE_REQUEST(REQ_ddl);
	action = (ACT) MAKE_ACTION(request, ACT_statistics);
	stats = (STS) ALLOC(STS_LEN);

	if (isc_databases && !isc_databases->dbb_next)
		request->req_database = isc_databases;
	else
		PAR_error("Can SET STATISTICS in context of single database only");

	if (MATCH(KW_INDEX)) {
		stats->sts_flags = STS_index;
		stats->sts_name = (STR) ALLOC(NAME_SIZE + 1);
		SQL_resolve_identifier("<index name>", (TEXT *) stats->sts_name);
		if (token.tok_length > NAME_SIZE)
			PAR_error("Index name too long");
		ADVANCE_TOKEN;
	}
	else
		SYNTAX_ERROR("INDEX");

	action->act_object = (REF) stats;
	return action;
}


//____________________________________________________________
//  
//		Generate a set transaction
//  

static ACT act_set_transaction(void)
{
	ACT action;
	GPRE_TRA trans;

	action = (ACT) ALLOC(ACT_LEN);
	action->act_type = ACT_start;

	trans = (GPRE_TRA) ALLOC(TRA_LEN);

	if (MATCH(KW_NAME))
		trans->tra_handle = PAR_native_value(FALSE, TRUE);

//  Get all the transaction parameters 

	while (TRUE) {
		if (MATCH(KW_ISOLATION)) {
			MATCH(KW_LEVEL);
			if (!par_transaction_modes(trans, TRUE))
				SYNTAX_ERROR("SNAPSHOT");

			continue;
		}

		if (par_transaction_modes(trans, FALSE))
			continue;

		if (MATCH(KW_NO)) {
			if (MATCH(KW_WAIT)) {
				trans->tra_flags |= TRA_nw;
				continue;
			}
			SYNTAX_ERROR("WAIT");
		}

		if (MATCH(KW_WAIT))
			continue;

#ifdef DEV_BUILD
		if (MATCH(KW_AUTOCOMMIT)) {
			trans->tra_flags |= TRA_autocommit;
			continue;
		}
#endif

		if (MATCH(KW_NO_AUTO_UNDO)) {
			trans->tra_flags |= TRA_no_auto_undo;
			continue;
		}

		break;
	}

//  send out for the list of reserved relations 

	if (MATCH(KW_RESERVING)) {
		trans->tra_flags |= TRA_rrl;
		PAR_reserving(trans->tra_flags, 1);
	}
	else if (MATCH(KW_USING)) {
		trans->tra_flags |= TRA_inc;
		PAR_using_db();
	}

	CMP_t_start(trans);
	action->act_object = (REF) trans;
	action->act_whenever = gen_whenever();

	return action;
}


//____________________________________________________________
//  
//		Generate a COMMIT, FINISH, or ROLLBACK.
//  

static ACT act_transaction( enum act_t type)
{
	ACT action;
	TEXT *transaction;

	par_options(&transaction);
	MATCH(KW_WORK);
	action = (ACT) ALLOC(ACT_LEN);
	action->act_type = type;
	action->act_whenever = gen_whenever();
	action->act_object = (REF) transaction;

	if (MATCH(KW_RELEASE)) {
		type = (type == ACT_commit) ? ACT_finish : ACT_rfinish;
		if (transaction) {
			action->act_rest = (ACT) ALLOC(ACT_LEN);
			action->act_rest->act_type = type;
		}
		else
			action->act_type = type;
	}
	else if ((type == ACT_commit) && (MATCH(KW_RETAIN))) {
		MATCH(KW_SNAPSHOT);
		action->act_type = type = ACT_commit_retain_context;
	}

	return action;
}


//____________________________________________________________
//  
//		Parse an update action.  This is a little more complicated
//		because SQL confuses the update of a cursor with a mass update.
//		The syntax, and therefor the code, I fear, is a mess.
//  

static ACT act_update(void)
{
	ACT action, slice_action;
	GPRE_REQ request, slice_request;
	GPRE_REL relation;
	rse* selection;
	GPRE_CTX input_context, update_context;
	GPRE_NOD set_list, *end_list, set_item, modify, *ptr;
	UPD update;
	LLS stack;
	SYM alias;
	REF field_ref, req_ref;
	SLC slice;
	SCHAR *transaction, r_name[NAME_SIZE + 1], db_name[NAME_SIZE + 1],
		owner_name[NAME_SIZE + 1], *str_1, *str_2;
	SSHORT where, count, i, found = FALSE, trans_nm_len;

	par_options(&transaction);

//  First comes the relation.  Unfortunately, there is no way to identify
//  its database until the cursor is known.  Sigh.  Save the token. 

	SQL_relation_name(r_name, db_name, owner_name);

//  Parse the optional alias (context variable) 

	alias = (token.tok_symbol) ? NULL : PAR_symbol(SYM_dummy);

//  Now we need the SET list list.  Do this thru a linked list stack 

	if (!MATCH(KW_SET))
		SYNTAX_ERROR("SET");

	request = MAKE_REQUEST(REQ_mass_update);
	relation = SQL_relation(request, r_name, db_name, owner_name, TRUE);
	input_context = MAKE_CONTEXT(request);
	input_context->ctx_relation = relation;
	if (alias) {
		alias->sym_type = SYM_context;
		alias->sym_object = input_context;
		HSH_insert(alias);
		token.tok_symbol = HSH_lookup(token.tok_string);
	}

	stack = NULL;
	count = 0;

	do {
		set_item = MAKE_NODE(nod_assignment, 2);
		set_item->nod_arg[1] = SQE_field(NULL, FALSE);
		if (!MATCH(KW_EQUALS))
			SYNTAX_ERROR("assignment operator");
		if (MATCH(KW_NULL))
			set_item->nod_arg[0] = MAKE_NODE(nod_null, 0);
		else
			set_item->nod_arg[0] = SQE_value(request, FALSE, 0, 0);
		PUSH(set_item, &stack);
		count++;
	} while (MATCH(KW_COMMA));

	set_list = MAKE_NODE(nod_list, count);
	ptr = end_list = set_list->nod_arg + count;

	while (stack)
		*--ptr = (GPRE_NOD) POP(&stack);

//  Now the moment of truth.  If the next few tokens are WHERE CURRENT OF
//  then this is a sub-action of an existing request.  If not, then it is
//  a free standing request 

	if ((where = MATCH(KW_WITH)) && MATCH(KW_CURRENT)) {
		if (!MATCH(KW_OF))
			SYNTAX_ERROR("OF cursor");

		/* Allocate update block, flush old request block, then
		   find the right request block, and the target relation */

		update = (UPD) ALLOC(UPD_LEN);
		update->upd_references = request->req_values;
		MSC_free_request(request);

		request = par_cursor(NULL);
		if (request->req_flags == REQ_sql_blob_create)
			PAR_error("expected a TABLE cursor, got a BLOB cursor");
		if ((transaction || request->req_trans) &&
			(!transaction || !request->req_trans ||
			 strcmp(transaction, request->req_trans))) {
			if (transaction)
				PAR_error("different transaction for select and update");
			else {				/* does not specify transaction clause in      */
				/*   "update ... where cuurent of cursor" stmt */
				trans_nm_len = strlen(request->req_trans);
				str_2 = transaction = (SCHAR *) ALLOC(trans_nm_len + 1);
				str_1 = request->req_trans;
				do
					*str_2++ = *str_1++;
				while (--trans_nm_len);
			}
		}
		request->req_trans = transaction;
		relation = SQL_relation(request, r_name, db_name, owner_name, TRUE);

		/* Given the target relation, find the input context for the modify */

		selection = request->req_rse;
		for (i = 0; i < selection->rse_count; i++) {
			input_context = selection->rse_context[i];
			if (input_context->ctx_relation == relation)
				break;
		}

		if (i == selection->rse_count)
			PAR_error("table not in request");

		/* Resolve input fields first */

		if (alias) {
			alias->sym_type = SYM_context;
			alias->sym_object = input_context;
			HSH_insert(alias);
		}

		for (ptr = set_list->nod_arg; ptr < end_list; ptr++) {
			set_item = *ptr;
			SQE_resolve(set_item->nod_arg[0], request, 0);
			pair(set_item->nod_arg[0], set_item->nod_arg[1]);
		}

		update->upd_request = request;
		update->upd_source = input_context;
		update->upd_update = update_context = MAKE_CONTEXT(request);
		update_context->ctx_relation = relation;
		update->upd_assignments = set_list;

		action = MAKE_ACTION(request, ACT_update);

		/* Resolve update fields next */

		if (alias)
			alias->sym_object = update_context;

		for (ptr = set_list->nod_arg; ptr < end_list; ptr++) {
			set_item = *ptr;
			SQE_resolve(set_item->nod_arg[1], request, 0);
			field_ref = (REF) ((set_item->nod_arg[1])->nod_arg[0]);
			if ((slice_action = (ACT) field_ref->ref_slice) &&
				(slice = (SLC) slice_action->act_object)) {
				/* These requests got lost in freeing the main request  */

				slice_request = slice_action->act_request;
				slice_request->req_next = requests;
				requests = slice_request;
				slice->slc_field_ref = field_ref;
				slice->slc_array = (GPRE_NOD) set_item->nod_arg[0];
				slice->slc_parent_request = request;
				slice_action->act_type = ACT_put_slice;

				/* If slice ref is not yet posted, post it.
				   This is required to receive the handle for the
				   array being updated */

				found = FALSE;
				for (req_ref = request->req_references; req_ref;
					 req_ref = req_ref->ref_next) {
					if (req_ref == field_ref) {
						set_item->nod_arg[1]->nod_arg[0] = (GPRE_NOD) req_ref;
						found = TRUE;
						break;
					}
					if ((req_ref->ref_field == field_ref->ref_field) &&
						(req_ref->ref_context == field_ref->ref_context))
						PAR_error
							("Can't update multiple slices of same column");
				}
				if (!found) {
					field_ref->ref_next = request->req_references;
					request->req_references = field_ref;
				}
			}
			pair(set_item->nod_arg[0], set_item->nod_arg[1]);
		}

		action->act_whenever = gen_whenever();
		action->act_object = (REF) update;
		if (alias)
			HSH_remove(alias);
		return action;
	}

	request->req_trans = transaction;

//  How amusing.  After all that work, it wasn't a sub-action at all.
//  Neat.  Take the pieces and build a complete request.  Start by
//  figuring out what database is involved.  

//  Generate record select expression, then resolve input values 

	request->req_rse = selection = (rse*) ALLOC(RSE_LEN(1));
	selection->rse_count = 1;
	selection->rse_context[0] = input_context;

	if (!alias && !token.tok_symbol)
		/* may have a relation name put parser didn't know it when it parsed it */
		token.tok_symbol = HSH_lookup(token.tok_string);

	for (ptr = set_list->nod_arg; ptr < end_list; ptr++) {
		set_item = *ptr;
		SQE_resolve(set_item->nod_arg[0], request, selection);
	}

//  Process boolean, if any 

	if (where)
		selection->rse_boolean = SQE_boolean(request, 0);

//  Resolve update fields to update context 

	request->req_update = update_context = MAKE_CONTEXT(request);
	update_context->ctx_relation = relation;

	if (alias)
		alias->sym_object = update_context;

	for (ptr = set_list->nod_arg; ptr < end_list; ptr++) {
		set_item = *ptr;
		SQE_resolve(set_item->nod_arg[1], request, 0);
		field_ref = (REF) ((set_item->nod_arg[1])->nod_arg[0]);
		if (slice_action = (ACT) field_ref->ref_slice) {
			/* Slices not allowed in searched updates  */

			PAR_error("Updates of slices not allowed in searched updates");
		}

		/* In dialect 1, neither the value being assigned (nod_arg[0])
		   nor the field to which it is being assigned (nod_arg[1]) may
		   be of a data type added in V6. */
		if (SQL_DIALECT_V5 == sw_sql_dialect) {
			USHORT field_dtype;
			int arg_num;
			for (arg_num = 0; arg_num <= 1; arg_num++)
				if (nod_field == set_item->nod_arg[arg_num]->nod_type) {
					field_dtype =
						((REF)
						 (set_item->nod_arg[arg_num]->
						  nod_arg[0]))->ref_field->fld_dtype;
					if ((dtype_sql_date == field_dtype)
						|| (dtype_sql_time == field_dtype)
						|| (dtype_int64 == field_dtype)) {
						dialect1_bad_type(field_dtype);
					}
				}
		}

		pair(set_item->nod_arg[0], set_item->nod_arg[1]);
	}

	request->req_node = modify = MAKE_NODE(nod_modify, 1);
	modify->nod_arg[0] = set_list;

	action = MAKE_ACTION(request, ACT_loop);
	action->act_whenever = gen_whenever();

	if (alias)
		HSH_remove(alias);

	return action;
}


//____________________________________________________________
//  
//		Handle an SQL whenever statement.  This is declaratory,
//		rather than a significant action.
//  

static ACT act_whenever(void)
{
	USHORT condition, l;
	TEXT *p, *q;
	SWE label;
	ACT action;

	whenever_list = NULL;

//  Pick up condition 

	if (MATCH(KW_SQLERROR))
		condition = SWE_error;
	else if (MATCH(KW_SQLWARNING))
		condition = SWE_warning;
	else if (MATCH(KW_NOT) && MATCH(KW_FOUND))
		condition = SWE_not_found;
	else
		SYNTAX_ERROR("WHENEVER condition");

//  Pick up action 

	if (MATCH(KW_CONTINUE))
		label = NULL;
	else if ((MATCH(KW_GO) && MATCH(KW_TO)) || MATCH(KW_GOTO)) {
		MATCH(KW_COLON);
		l = token.tok_length;
		label = (SWE) ALLOC(sizeof(swe) + l);
		label->swe_condition = condition;
		if (label->swe_length = l) {
			p = label->swe_label;
			q = token.tok_string;
			do
				*p++ = *q++;
			while (--l);
		}
		ADVANCE_TOKEN;
		label->swe_condition = condition;
	}
	else
		SYNTAX_ERROR("GO TO or CONTINUE");

//  Set up condition vector 

	whenever[condition] = label;

	action = (ACT) ALLOC(ACT_LEN);
	action->act_type = ACT_noop;

	MATCH(KW_SEMI_COLON);
	return action;
}


//____________________________________________________________
//  
//		Make sure that a file path doesn't contain a
//		Decnet node name.
//  

static BOOLEAN check_filename( TEXT * name)
{
	USHORT l;
	TEXT *p, file_name[256];

	if (!(l = strlen(name)))
		return TRUE;

	for (p = name; *p; p++)
		if (p[0] == ':' && p[1] == ':')
			return FALSE;

	l = MIN(l, sizeof(file_name) - 1);
	strncpy(file_name, name, l);
	file_name[l] = '\0';

	return TRUE;
}


//____________________________________________________________
//  
//		Parse connect options
//  

static void connect_opts(
						 TEXT ** user,
						 TEXT ** password,
						 TEXT ** sql_role,
						 TEXT ** lc_messages, USHORT * buffers)
{
	TEXT *s;

	for (;;) {
		if (MATCH(KW_CACHE)) {
			*buffers = atoi(token.tok_string);
			ADVANCE_TOKEN;
			MATCH(KW_BUFFERS);
		}
		else if (MATCH(KW_USER))
			*user = SQL_var_or_string(FALSE);
		else if (MATCH(KW_PASSWORD))
			*password = SQL_var_or_string(FALSE);
		else if (MATCH(KW_ROLE)) {
			if (token.tok_type == tok_ident) {
				s = (TEXT *) ALLOC(token.tok_length + 3);
											   /** extra bytes for quotes and NULL **/
				SQL_resolve_identifier("<Role Name>", s);
				s[0] = '\"';
				strcpy(s + 1, token.tok_string);
				strcat(s, "\"");
				*sql_role = s;
			}
			else
				*sql_role = SQL_var_or_string(FALSE);
		}
		else if (MATCH(KW_LC_MESSAGES))
			*lc_messages = SQL_var_or_string(FALSE);
		else
			break;
	}

}

#ifdef FLINT_CACHE
//____________________________________________________________
//  
//		Add a shared cache to an existing database.
//  

static FIL define_cache(void)
{
	FIL file;
	TEXT *string;
	TEXT err_string[256];

	file = (FIL) ALLOC(sizeof(fil));
	if (QUOTED(token.tok_type)) {
		file->fil_name = string = (TEXT *) ALLOC(token.tok_length + 1);
		COPY(token.tok_string, token.tok_length, string);
		ADVANCE_TOKEN;
	}
	else
		SYNTAX_ERROR("<quoted filename>");
	if (!check_filename(file->fil_name))
		PAR_error("node name not permitted");	/* a node name is not permitted in a shared cache file name */

	if (MATCH(KW_LENGTH)) {
		file->fil_length = EXP_ULONG_ordinal(TRUE);
		if (file->fil_length < MIN_CACHE_BUFFERS) {
			sprintf(err_string, "Minimum of %d cache pages required",
					MIN_CACHE_BUFFERS);
			PAR_error(err_string);
		}
		MATCH(KW_PAGES);
	}
	else
		file->fil_length = DEF_CACHE_BUFFERS;	/* default cache buffers */

	return file;
}
#endif

//____________________________________________________________
//  
//		Add a new file to an existing database.
//  

static FIL define_file(void)
{
	FIL file;
	TEXT *string;

	file = (FIL) ALLOC(sizeof(fil));
	if (QUOTED(token.tok_type)) {
		file->fil_name = string = (TEXT *) ALLOC(token.tok_length + 1);
		COPY(token.tok_string, token.tok_length, string);
		token.tok_length += 2;
		ADVANCE_TOKEN;
	}
	else
		SYNTAX_ERROR("<quoted filename>");

	if (!check_filename(file->fil_name))
		PAR_error("node name not permitted");	/* A node name is not permitted in a shadow or secondary file name */

	while (TRUE) {
		if (MATCH(KW_LENGTH)) {
			MATCH(KW_EQUALS);
			file->fil_length = EXP_ULONG_ordinal(TRUE);
			MATCH(KW_PAGES);
			MATCH(KW_PAGE);
		}
		else if (MATCH(KW_STARTS) || MATCH(KW_STARTING)) {
			MATCH(KW_AT);
			MATCH(KW_PAGE);
			file->fil_start = EXP_ULONG_ordinal(TRUE);
		}
		else
			break;
	}

	return file;
}


//____________________________________________________________
//  
//		define a log file 
//  

static FIL define_log_file( BOOLEAN log_serial)
{
	FIL file;
	TEXT *string;
	TEXT err_string[256];

	file = (FIL) ALLOC(sizeof(fil));
	if (QUOTED(token.tok_type)) {
		file->fil_name = string = (TEXT *) ALLOC(token.tok_length + 1);
		COPY(token.tok_string, token.tok_length, string);
		ADVANCE_TOKEN;
	}
	else
		SYNTAX_ERROR("<quoted filename>");

	if (!check_filename(file->fil_name))
		PAR_error("node name not permitted");	/* A node name is not permitted in a shadow or secondary file name */

	while (TRUE) {
		if (MATCH(KW_SIZE)) {
			MATCH(KW_EQUALS);
			file->fil_length = EXP_ULONG_ordinal(TRUE);
		}
// **** REMOVED for now from the product.
//   else if (MATCH (KW_RAW_PARTITIONS))
// { 
// if (log_serial)
//    PAR_error ("Partitions not supported in series of log file specification");
// MATCH (KW_EQUALS);
// file->fil_partitions = EXP_USHORT_ordinal (TRUE);
// file->fil_flags |= FIL_raw;
// } 
//***
		else
			break;
	}

	if (file->fil_partitions) {
		if (!file->fil_length) {
			sprintf(err_string,
					"Total length of the partitioned log %s must be specified",
					file->fil_name);
			PAR_error(err_string);
		}

		if (PARTITION_SIZE(OneK * file->fil_length, file->fil_partitions) <
			OneK * MIN_LOG_LENGTH) {
			sprintf(err_string, "log partition size too small for %s",
					file->fil_name);
			PAR_error(err_string);
		}
	}
	else {
		if ((file->fil_length) && (file->fil_length < MIN_LOG_LENGTH)) {
			/* msg 336: Minimum log length should be MIN_LOG_LENGTH Kbytes */
			sprintf(err_string,
					"Minimum log length should be %d Kbytes", MIN_LOG_LENGTH);
			PAR_error(err_string);
		}
	}

	return file;
}


static DBB dup_dbb( DBB db)
{

// ****************************************
//  
//		d u p _ d b b
//  
// ****************************************
//  
//		dirty duplication of a dbb.
//		just memcpy as no memory 
//		is freed in gpre.
//  
// *************************************
	DBB newdb;

	if (!db)
		return NULL;
	newdb = (DBB) ALLOC(DBB_LEN);
	memcpy((SCHAR *) newdb, (SCHAR *) db, (size_t) DBB_LEN);

	return newdb;
}


//____________________________________________________________
//  
//       Report an error with parameter
//  

static void error( TEXT * string, TEXT * string2)
{
	TEXT buffer[256];

	sprintf(buffer, string, string2);
	PAR_error(buffer);
}


//____________________________________________________________
//  
//       Extract string from "string" in
//		token.
//  

static TEXT *extract_string( BOOLEAN advance_token)
{
	TEXT *string = NULL;

	switch (sw_sql_dialect) {
	case 1:
		if (!QUOTED(token.tok_type))
			SYNTAX_ERROR("<string>");
		break;

	default:
		if (!SINGLE_QUOTED(token.tok_type))
			SYNTAX_ERROR("<string>");
		break;
	}

	string = (TEXT *) ALLOC(token.tok_length + 1);
	COPY(token.tok_string, token.tok_length, string);
	if (advance_token)
		ADVANCE_TOKEN;
	return string;
}



//____________________________________________________________
//  
//		Generate a linked list of SQL WHENEVER items for error
//		handling.
//  

static SWE gen_whenever(void)
{
	SWE label, prior, proto;
	USHORT i, l;
	TEXT *p, *q;

	if (whenever_list)
		return whenever_list;

	whenever_list = label = NULL;

	for (i = 0; i < SWE_max; i++)
		if (proto = whenever[i]) {
			prior = label;
			l = proto->swe_length;
			label = (SWE) ALLOC(sizeof(swe) + l);
			label->swe_next = prior;
			label->swe_condition = proto->swe_condition;
			if (l) {
				p = label->swe_label;
				q = proto->swe_label;
				do
					*p++ = *q++;
				while (--l);
			}
		}

	return label;
}


//____________________________________________________________
//  
//		Correlate the into list with the select expression list
//		to form full references (post same against request).
//  

static void into( GPRE_REQ request, GPRE_NOD field_list, GPRE_NOD var_list)
{
	REF var_ref, field_ref, reference;
	GPRE_NOD *var_ptr, *fld_ptr, *end;
	GPRE_FLD field;
	GPRE_REQ slice_req;
	SSHORT found = FALSE;

	if (!var_list)
		PAR_error("INTO list is required");

	if (!field_list || field_list->nod_count != var_list->nod_count)
		PAR_error
			("column count and number of INTO list host-variables unequal");

	var_ptr = var_list->nod_arg;
	fld_ptr = field_list->nod_arg;

	for (end = fld_ptr + var_list->nod_count; fld_ptr < end;
		 fld_ptr++, var_ptr++) {
		var_ref = (REF) * var_ptr;
		if (((*fld_ptr)->nod_type == nod_field)
			|| ((*fld_ptr)->nod_type == nod_array)) {
			field_ref = (REF) (*fld_ptr)->nod_arg[0];
			slice_req = (GPRE_REQ) (*fld_ptr)->nod_arg[2];
		}
		else
			field_ref = 0;
		if (field_ref && slice_req && (field = field_ref->ref_field)
			&& field->fld_array) {
			EXP_post_array(field_ref);

			/* If field ref not posted yet, post it   */

			found = FALSE;
			for (reference = request->req_references; reference;
				 reference = reference->ref_next) {
				if (reference == field_ref) {
					found = TRUE;
					break;
				}
			}
			if (!found) {
				field_ref->ref_next = request->req_references;
				request->req_references = field_ref;
			}
			reference = field_ref;
		}
		else
			reference = SQE_post_reference(request, 0, 0, *fld_ptr);
		var_ref->ref_friend = reference;
		reference->ref_null_value = var_ref->ref_null_value;
	}
}


//____________________________________________________________
//  
//		Create field in a relation for a metadata request.
//  

static GPRE_FLD make_field( GPRE_REL relation)
{
	GPRE_FLD field;
	char s[ERROR_LENGTH];

	SQL_resolve_identifier("<column name>", s);
	field = MET_make_field(s, 0, 0, TRUE);
	field->fld_relation = relation;
	field->fld_flags |= FLD_meta;
	ADVANCE_TOKEN;

	return field;
}


//____________________________________________________________
//  
//		Create index for metadata request.
//  

static IND make_index( GPRE_REQ request, TEXT * string)
{
	IND index;
	TEXT s[ERROR_LENGTH];

	if ((isc_databases) && (!(isc_databases->dbb_next))) {
		strcpy(s, string);
		index = MET_make_index(s);
		if (request)
			request->req_database = isc_databases;
		index->ind_flags |= IND_meta;
	}
	else
		PAR_error("Can only reference INDEX in context of single database");

	return index;
}


//____________________________________________________________
//  
//		Create relation for a metadata request.
//  

static GPRE_REL make_relation( GPRE_REQ request, TEXT * relation_name)
{
	GPRE_REL relation;
	TEXT r[ERROR_LENGTH];

	if (isc_databases && !isc_databases->dbb_next) {
		strcpy(r, relation_name);

		relation = MET_make_relation(r);
		relation->rel_database = isc_databases;
		relation->rel_meta = TRUE;

		if (request)
			request->req_database = isc_databases;
	}
	else
		PAR_error("Can only reference TABLE in context of single database");

	return relation;
}


//____________________________________________________________
//  
//		Given two value expressions associated in a relational
//		expression, see if one is a field reference and the other
//		is a host language variable..  If so, match the field to the
//		host language variable.
//		In other words, here we are guessing what the datatype is
//		of a host language variable.
//  

static void pair( GPRE_NOD expr, GPRE_NOD field_expr)
{
	GPRE_NOD *ptr, *end_ptr;
	REF ref1, ref2;
	MEL element;

	if (field_expr->nod_type != nod_field)
		return;

	switch (expr->nod_type) {
	case nod_value:
		ref1 = (REF) field_expr->nod_arg[0];
		ref2 = (REF) expr->nod_arg[0];

		/* We're done if we've already determined the type of this reference
		 * (if, for instance, it's a parameter to a UDF or PROCEDURE)
		 */
		if (ref2->ref_field)
			return;

		ref2->ref_field = ref1->ref_field;
		if (ref1->ref_field->fld_array) {
			ref2->ref_context = ref1->ref_context;
			ref2->ref_friend = ref1;
			EXP_post_array(ref1);
			EXP_post_array(ref2);
		}
		return;

	case nod_map_ref:
		element = (MEL) expr->nod_arg[0];
		pair(element->mel_expr, field_expr);
		return;

	case nod_field:
	case nod_literal:
		return;
	}

	ptr = expr->nod_arg;
	end_ptr = ptr + expr->nod_count;

	for (; ptr < end_ptr; ptr++)
		pair(*ptr, field_expr);
}


//____________________________________________________________
//  
//		Parse the multi-dimensional array specification.
//  

static void par_array( GPRE_FLD field)
{
	SLONG rangeh, rangel;
	SSHORT i = 0;
	ARY array_info;

//  Pick up ranges 

	array_info = field->fld_array_info;

	while (TRUE) {
		rangel = EXP_SSHORT_ordinal(TRUE);
		if (MATCH(KW_COLON))
			rangeh = EXP_SSHORT_ordinal(TRUE);
		else {
			if (rangel < 1)
				rangeh = 1;
			else {
				rangeh = rangel;
				rangel = 1;
			}
		}

		if (rangel >= rangeh)
			PAR_error("Start of array range must be less than end of range");

		if (i > MAX_ARRAY_DIMENSIONS)
			PAR_error("Array has too many dimensions");

		array_info->ary_rpt[i].ary_lower = rangel;
		array_info->ary_rpt[i++].ary_upper = rangeh;

		if (MATCH(KW_R_BRCKET))
			break;

		if (!MATCH(KW_COMMA))
			SYNTAX_ERROR(", (comma)");
	}

	array_info->ary_dimension_count = i;
}


//____________________________________________________________
//  
//  
//		Read in the specified character set on 
//		a READ BLOB or an INSERT BLOB.
//  

static SSHORT par_char_set(void)
{
	SYM symbol;

	if (!MATCH(KW_CHAR))
		SYNTAX_ERROR("CHARACTER SET");

	if (!MATCH(KW_SET))
		SYNTAX_ERROR("CHARACTER SET");

	if (token.tok_type != tok_ident)
		SYNTAX_ERROR("<character set name>");

	if (!(symbol = MSC_find_symbol(token.tok_symbol, SYM_charset)))
		PAR_error("The named CHARACTER SET was not found");

	ADVANCE_TOKEN;

	return (((INTLSYM) symbol->sym_object)->intlsym_charset_id);
}


//____________________________________________________________
//  
//		Create a computed field
//  

static void par_computed( GPRE_REQ request, GPRE_FLD field)
{
	CMPF cmp;
	gpre_fld save_fld;

	MATCH(KW_BY);

//  If field size has been specified, save it.  Then NULL it
//  till the new size is calculated from the specified expression.
//  This will catch self references.
//  
//  The user specified values will be restored to override calculated
//  values.
//  

	save_fld = *field;

	cmp = (CMPF) ALLOC(CMPF_LEN);
	cmp->cmpf_text = CPR_start_text();
	cmp->cmpf_boolean = SQE_value(request, FALSE, 0, 0);
	CPR_end_text(cmp->cmpf_text);

	field->fld_computed = cmp;
	field->fld_flags |= FLD_computed;

	CME_get_dtype(field->fld_computed->cmpf_boolean, field);

//  If there was user specified data type/size, restore it 

	if (save_fld.fld_dtype) {
		field->fld_dtype = save_fld.fld_dtype;
		field->fld_length = save_fld.fld_length;
		field->fld_scale = save_fld.fld_scale;
		field->fld_sub_type = save_fld.fld_sub_type;
		field->fld_ttype = save_fld.fld_ttype;
		field->fld_charset_id = save_fld.fld_charset_id;
		field->fld_collate_id = save_fld.fld_collate_id;
		field->fld_char_length = save_fld.fld_char_length;
	}
}


//____________________________________________________________
//  
//		Parse the next token as a cursor name.
//		If it is, return the request associated with the cursor.  If
//		not, produce an error and return NULL.
//  

static GPRE_REQ par_cursor( SYM * symbol_ptr)
{
	SYM symbol;
	TEXT t_cur[128];

// ** 
//   par_cursor() is called to use a previously declared cursor.
//   tok_symbol == NULL means one of the two things.
//   a) The name does not belong to a cursor. OR
//   b) get_token() function in gpre.c was not able to find the cursor
//      in hash table.
//  
//   case a) is an error condition.
//   case b) Could have resulted because the cursor name was upcased and
//    inserted into hash table since it was not quoted and it is 
//    now being refered as it was declared.
//  
//   Hence, Try and lookup the cursor name after resolving it once more. If
//   it still cannot be located, Its an error
//*  

	SQL_resolve_identifier("<cursor name>", t_cur);
	token.tok_symbol = symbol = HSH_lookup(token.tok_string);
	if (symbol && symbol->sym_type == SYM_keyword)
		token.tok_keyword = (KWWORDS) symbol->sym_keyword;
	else
		token.tok_keyword = KW_none;

	symbol = MSC_find_symbol(token.tok_symbol, SYM_cursor);
	if (!symbol)
		symbol = MSC_find_symbol(token.tok_symbol, SYM_delimited_cursor);
	if (symbol) {
		ADVANCE_TOKEN;
		if (symbol_ptr)
			*symbol_ptr = symbol;
		return (GPRE_REQ) symbol->sym_object;
	}
	else if (symbol = MSC_find_symbol(token.tok_symbol, SYM_dyn_cursor))
		PAR_error("DSQL cursors require DSQL update & delete statements");
	SYNTAX_ERROR("<cursor name>");
	return NULL;				/* silence compiler */
}


//____________________________________________________________
//  
//		If this is a dynamic curser, return dynamic statement block.
//  

static DYN par_dynamic_cursor(void)
{
	SYM symbol;
	TEXT t_cur[128];

	if (token.tok_symbol == NULL) {
		SQL_resolve_identifier("<cursor name>", t_cur);
		token.tok_symbol = symbol = HSH_lookup(token.tok_string);
		if (symbol && symbol->sym_type == SYM_keyword)
			token.tok_keyword = (KWWORDS) symbol->sym_keyword;
		else
			token.tok_keyword = KW_none;
	}
	if (symbol = MSC_find_symbol(token.tok_symbol, SYM_dyn_cursor)) {
		ADVANCE_TOKEN;
		return (DYN) symbol->sym_object;
	}

	return NULL;
}


//____________________________________________________________
//  
//		Handle an SQL field definition in CREATE, DECLARE or 
//		ALTER TABLE statement.
//  

static GPRE_FLD par_field( GPRE_REQ request, GPRE_REL relation)
{
	GPRE_FLD field;
// *IND		index; 
	cnstrt** constraint_ref;
	int in_constraints;
	GPRE_NOD literal_node;

	field = make_field(relation);

	SQL_par_field_dtype(request, field, FALSE);

	if (MATCH(KW_COMPUTED)) {
		if (field->fld_global)
			PAR_error("Cannot use domains to override computed column size");
		if (field->fld_array_info)
			PAR_error("Computed columns cannot be arrays");

		par_computed(request, field);
	}

//  Check if default value was specified 

	if (token.tok_keyword == KW_DEFAULT) {
		field->fld_default_source = CPR_start_text();
		ADVANCE_TOKEN;

		if (MATCH(KW_USER))
			field->fld_default_value = MAKE_NODE(nod_user_name, 0);
// ** Begin sql/date/time/timestamp *
		else if (MATCH(KW_CURRENT_DATE))
			field->fld_default_value = MAKE_NODE(nod_current_date, 0);
		else if (MATCH(KW_CURRENT_TIME))
			field->fld_default_value = MAKE_NODE(nod_current_time, 0);
		else if (MATCH(KW_CURRENT_TIMESTAMP))
			field->fld_default_value = MAKE_NODE(nod_current_timestamp, 0);
// ** End sql/date/time/timestamp *
		else if (MATCH(KW_NULL))
			field->fld_default_value = MAKE_NODE(nod_null, 0);
		else {
			if (MATCH(KW_MINUS)) {
				if (token.tok_type != tok_number)
					SYNTAX_ERROR("<number>");

				literal_node = EXP_literal();
				field->fld_default_value = MSC_unary(nod_negate,
													 literal_node);
			}
			else if ((field->fld_default_value = EXP_literal()) == NULL)
				SYNTAX_ERROR("<constant>");
		}
		CPR_end_text(field->fld_default_source);
	}


//  Check for any column level constraints 

	constraint_ref = &field->fld_constraints;
	in_constraints = TRUE;

	while (in_constraints) {
		switch (token.tok_keyword) {
		case KW_CONSTRAINT:
		case KW_PRIMARY:
		case KW_UNIQUE:
		case KW_REFERENCES:
		case KW_CHECK:
		case KW_NOT:
			*constraint_ref = par_field_constraint(request, field, relation);
			constraint_ref = &(*constraint_ref)->cnstrt_next;
			break;

		default:
			in_constraints = FALSE;
			break;
		}
	}
//    if (MATCH (KW_NOT))
//   if (MATCH (KW_NULL))
// { 
// field->fld_flags |= FLD_not_null;
// if (MATCH (KW_UNIQUE))
//    {
//    index = (IND) ALLOC (IND_LEN);
//    index->ind_relation = relation;
//    index->ind_fields = field;
//    index->ind_flags |= IND_dup_flag | IND_meta;
//    index->ind_flags &= ~IND_descend;
//    field->fld_index = index;
//    }
// } 
//   else
// SYNTAX_ERROR ("NULL");    

	SQL_par_field_collate(request, field);
	SQL_adjust_field_dtype(field);

	return field;
}


//____________________________________________________________
//  
//		Create a field level constraint as part of CREATE TABLE or
//		ALTER TABLE statement. Constraint maybe table or column level.
//  

static CNSTRT par_field_constraint( GPRE_REQ request, GPRE_FLD for_field, GPRE_REL relation)
{
	enum kwwords keyword;
	cnstrt* new_constraint = (cnstrt*) ALLOC(CNSTRT_LEN);
	STR field_name;

	if (token.tok_keyword == KW_CONSTRAINT) {
		ADVANCE_TOKEN;
		new_constraint->cnstrt_name = (STR) ALLOC(NAME_SIZE + 1);
		SQL_resolve_identifier("<constraint name>",
							   (TEXT *) new_constraint->cnstrt_name);
		if (token.tok_length > NAME_SIZE)
			PAR_error("Constraint name too long");
		ADVANCE_TOKEN;
	}

	switch (keyword = token.tok_keyword) {
	case KW_NOT:
		ADVANCE_TOKEN;
		if (!MATCH(KW_NULL))
			SYNTAX_ERROR("NULL");
		new_constraint->cnstrt_type = CNSTRT_NOT_NULL;
		for_field->fld_flags |= FLD_not_null;
		break;

	case KW_PRIMARY:
	case KW_UNIQUE:
	case KW_REFERENCES:
		ADVANCE_TOKEN;
		if (keyword == KW_PRIMARY) {
			if (!MATCH(KW_KEY))
				SYNTAX_ERROR("KEY");
			new_constraint->cnstrt_type = CNSTRT_PRIMARY_KEY;
		}
		else if (keyword == KW_REFERENCES) {
			new_constraint->cnstrt_type = CNSTRT_FOREIGN_KEY;
		}
		else
			new_constraint->cnstrt_type = CNSTRT_UNIQUE;

		/* Set field for PRIMARY KEY or FOREIGN KEY or UNIQUE constraint  */

		field_name = (STR) ALLOC(NAME_SIZE + 1);
		strcpy((char *) field_name, for_field->fld_symbol->sym_string);
		PUSH((GPRE_NOD) field_name, &new_constraint->cnstrt_fields);

		if (keyword == KW_REFERENCES) {
			/* Relation name for foreign key  */

			new_constraint->cnstrt_referred_rel = (STR) ALLOC(NAME_SIZE + 1);
			SQL_resolve_identifier("referred <table name>",
								   (TEXT *) new_constraint->cnstrt_referred_rel);
			if (token.tok_length > NAME_SIZE)
				PAR_error("Referred table name too long");
			ADVANCE_TOKEN;

			if (MATCH(KW_LEFT_PAREN)) {
				/* Field specified for referred relation  */

				field_name = (STR) ALLOC(NAME_SIZE + 1);
				SQL_resolve_identifier("<column name>", (TEXT *) field_name);
				PUSH((GPRE_NOD) field_name, &new_constraint->cnstrt_referred_fields);
				CPR_token();
				EXP_match_paren();
			}

			if (token.tok_keyword == KW_ON) {
				par_fkey_extension(new_constraint);
				ADVANCE_TOKEN;
				if (token.tok_keyword == KW_ON) {
					par_fkey_extension(new_constraint);
					ADVANCE_TOKEN;
				}
			}
		}
		break;

	case KW_CHECK:
		ADVANCE_TOKEN;
		new_constraint->cnstrt_type = CNSTRT_CHECK;
		new_constraint->cnstrt_text = CPR_start_text();
		new_constraint->cnstrt_boolean = SQE_boolean(request, 0);
		CPR_end_text(new_constraint->cnstrt_text);
		break;

	default:
		PAR_error("Invalid constraint type");
	}

	return new_constraint;
}


//____________________________________________________________
//  
//		Parse the INTO clause for a dynamic SQL statement.
//  

static BOOLEAN par_into( DYN statement)
{

	if (!MATCH(KW_INTO))
		return FALSE;

	MATCH(KW_SQL);				/* "SQL" keyword is optional for backward compatibility */

	if (!MATCH(KW_DESCRIPTOR))
		SYNTAX_ERROR("DESCRIPTOR");

	statement->dyn_sqlda2 = PAR_native_value(FALSE, FALSE);

	return TRUE;
}


//____________________________________________________________
//  
//		Parse request options.
//  

static void par_options( TEXT ** transaction)
{

	*transaction = NULL;

	if (!MATCH(KW_TRANSACTION))
		return;

	*transaction = PAR_native_value(FALSE, TRUE);
}


//____________________________________________________________
//  
//		parse the page_size clause of a
//		create database statement
//  

static int par_page_size(void)
{
	int n1, n2;

	MATCH(KW_EQUALS);
	n2 = n1 = EXP_USHORT_ordinal(FALSE);

	if (n1 <= 1024)
		n2 = 1024;
	else if (n1 <= 2048)
		n2 = 2048;
	else if (n1 <= 4096)
		n2 = 4096;
	else if (n1 <= 8192)
		n2 = 8192;
	else
		SYNTAX_ERROR("<page size> in range 1024 to 8096");
	ADVANCE_TOKEN;

	return n2;
}


//____________________________________________________________
//  
//		Parse the next thing as a relation.
//  

static GPRE_REL par_relation( GPRE_REQ request)
{
	SCHAR r_name[NAME_SIZE + 1], db_name[NAME_SIZE + 1],
		owner_name[NAME_SIZE + 1];

	SQL_relation_name(r_name, db_name, owner_name);

	return make_relation(request, r_name);
}


//____________________________________________________________
//  
//		Parse a dynamic statement name returning a dynamic
//		statement block.
//  

static DYN par_statement(void)
{
	DYN statement;

	statement = (DYN) ALLOC(DYN_LEN);
	statement->dyn_statement_name = PAR_symbol(SYM_dummy);
	statement->dyn_database = isc_databases;

	return statement;
}


//____________________________________________________________
//  
//		Handle an extended foreign key definition as part of CREATE TABLE 
//		or ALTER TABLE statements.
//  

static void par_fkey_extension(CNSTRT cnstrt_val)
{
	enum kwwords keyword;

//  
//   Extended foreign key definition could be as follows :
//  
//   [ON DELETE { NO ACTION | CASCADE | SET DEFAULT | SET NULL } ]
//   [ON UPDATE { NO ACTION | CASCADE | SET DEFAULT | SET NULL } ]
//  

	assert(token.tok_keyword == KW_ON)
		assert(cnstrt_val != NULL);

	ADVANCE_TOKEN;

	switch (keyword = token.tok_keyword) {
	case KW_DELETE:
		/* NOTE: action must be defined only once */
		if (cnstrt_val->cnstrt_fkey_def_type & REF_DELETE_ACTION)
			SYNTAX_ERROR("UPDATE");
		else
			cnstrt_val->cnstrt_fkey_def_type |= REF_DELETE_ACTION;
		break;
	case KW_UPDATE:
		/* NOTE: action must be defined only once */
		if (cnstrt_val->cnstrt_fkey_def_type & REF_UPDATE_ACTION)
			SYNTAX_ERROR("DELETE");
		else
			cnstrt_val->cnstrt_fkey_def_type |= REF_UPDATE_ACTION;
		break;
	default:
		/* unexpected keyword */
		SYNTAX_ERROR("UPDATE or DELETE");
		break;
	}

	ADVANCE_TOKEN;

	switch (token.tok_keyword) {
	case KW_NO:
		ADVANCE_TOKEN;
		if (token.tok_keyword != KW_ACTION)
			SYNTAX_ERROR("ACTION");
		else if (keyword == KW_DELETE)
			cnstrt_val->cnstrt_fkey_def_type |= REF_DEL_NONE;
		else
			cnstrt_val->cnstrt_fkey_def_type |= REF_UPD_NONE;
		break;
	case KW_CASCADE:
		if (keyword == KW_DELETE)
			cnstrt_val->cnstrt_fkey_def_type |= REF_DEL_CASCADE;
		else
			cnstrt_val->cnstrt_fkey_def_type |= REF_UPD_CASCADE;
		break;
	case KW_SET:
		ADVANCE_TOKEN;
		switch (token.tok_keyword) {
		case KW_DEFAULT:
			if (keyword == KW_DELETE)
				cnstrt_val->cnstrt_fkey_def_type |= REF_DEL_SET_DEFAULT;
			else
				cnstrt_val->cnstrt_fkey_def_type |= REF_UPD_SET_DEFAULT;
			break;
		case KW_NULL:
			if (keyword == KW_DELETE)
				cnstrt_val->cnstrt_fkey_def_type |= REF_DEL_SET_NULL;
			else
				cnstrt_val->cnstrt_fkey_def_type |= REF_UPD_SET_NULL;
			break;
		default:
			/* unexpected keyword */
			SYNTAX_ERROR("NULL or DEFAULT");
			break;
		}
		break;
	default:
		/* unexpected keyword */
		SYNTAX_ERROR("NO ACTION or CASCADE or SET DEFAULT or SET NULL");
		break;
	}
}


//____________________________________________________________
//  
//		Handle a create constraint verb as part of CREATE TABLE or
//		ALTER TABLE statement. Constraint maybe table or column level.
//  

static CNSTRT par_table_constraint( GPRE_REQ request, GPRE_REL relation)
{
	enum kwwords keyword;
	cnstrt* constraint = (cnstrt*) ALLOC(CNSTRT_LEN);
	LLS *fields;
	STR field_name;
	USHORT num_for_key_flds = 0, num_prim_key_flds = 0;

	if (token.tok_keyword == KW_CONSTRAINT) {
		ADVANCE_TOKEN;
		constraint->cnstrt_name = (STR) ALLOC(NAME_SIZE + 1);
		SQL_resolve_identifier("<constraint name>",
							   (TEXT *) constraint->cnstrt_name);
		if (token.tok_length > NAME_SIZE)
			PAR_error("Constraint name too long");
		ADVANCE_TOKEN;
	}

	switch (keyword = token.tok_keyword) {
	case KW_PRIMARY:
	case KW_UNIQUE:
	case KW_FOREIGN:
		ADVANCE_TOKEN;
		if (keyword == KW_PRIMARY) {
			if (!MATCH(KW_KEY))
				SYNTAX_ERROR("KEY");
			constraint->cnstrt_type = CNSTRT_PRIMARY_KEY;
		}
		else if (keyword == KW_FOREIGN) {
			if (!MATCH(KW_KEY))
				SYNTAX_ERROR("KEY");
			constraint->cnstrt_type = CNSTRT_FOREIGN_KEY;
		}
		else
			constraint->cnstrt_type = CNSTRT_UNIQUE;

		EXP_left_paren(0);

		/* Get list of fields for PRIMARY KEY or FOREIGN KEY or UNIQUE
		   constraint */

		fields = &constraint->cnstrt_fields;
		do {
			field_name = (STR) ALLOC(NAME_SIZE + 1);
			SQL_resolve_identifier("<column name>", (TEXT *) field_name);
			PUSH((GPRE_NOD) field_name, fields);
			fields = &(*fields)->lls_next;
			++num_for_key_flds;
			CPR_token();
		} while (MATCH(KW_COMMA));

		if (keyword != KW_FOREIGN)
			num_for_key_flds = 0;

		EXP_match_paren();
		if (keyword == KW_FOREIGN) {
			if (!MATCH(KW_REFERENCES))
				SYNTAX_ERROR("REFERENCES");

			/* Relation name for foreign key  */

			constraint->cnstrt_referred_rel = (STR) ALLOC(NAME_SIZE + 1);
			SQL_resolve_identifier("referred <table name>",
								   (TEXT *) constraint->cnstrt_referred_rel);
			if (token.tok_length > NAME_SIZE)
				PAR_error("Referred table name too long");
			ADVANCE_TOKEN;

			constraint->cnstrt_referred_fields = NULL;

			if (MATCH(KW_LEFT_PAREN)) {
				/* Fields specified for referred relation  */

				fields = &constraint->cnstrt_referred_fields;
				do {
					field_name = (STR) ALLOC(NAME_SIZE + 1);
					SQL_resolve_identifier("<column name>",
										   (TEXT *) field_name);
					PUSH((GPRE_NOD) field_name, fields);
					fields = &(*fields)->lls_next;
					++num_prim_key_flds;
					CPR_token();
				} while (MATCH(KW_COMMA));

				EXP_match_paren();
			}

			/* Don't print error message in case if <referenced column list>
			   is not specified. Try to catch them in cmd.c[create_constraint]
			   later on */
			if (constraint->cnstrt_referred_fields != NULL &&
				num_prim_key_flds != num_for_key_flds)
					PAR_error
					("FOREIGN KEY column count does not match PRIMARY KEY");
			if (token.tok_keyword == KW_ON) {
				par_fkey_extension(constraint);
				ADVANCE_TOKEN;
				if (token.tok_keyword == KW_ON) {
					par_fkey_extension(constraint);
					ADVANCE_TOKEN;
				}
			}
		}						/* if KW_FOREIGN */
		break;

	case KW_CHECK:
		ADVANCE_TOKEN;
		constraint->cnstrt_type = CNSTRT_CHECK;
		constraint->cnstrt_text = CPR_start_text();
		constraint->cnstrt_boolean = SQE_boolean(request, 0);
		CPR_end_text(constraint->cnstrt_text);
		break;

	default:
		PAR_error("Invalid constraint type");
	}

	return constraint;
}


//____________________________________________________________
//  
//		Parse the isolation level part.
//		If expect_iso is true, an isolation level is required.
//		Returns TRUE if found a match else FALSE.
//  

static BOOLEAN par_transaction_modes( GPRE_TRA trans, BOOLEAN expect_iso)
{

	if (MATCH(KW_READ)) {
		if (MATCH(KW_ONLY)) {
			if (expect_iso)
				SYNTAX_ERROR("SNAPSHOT");

			trans->tra_flags |= TRA_ro;
			return TRUE;
		}
		else if (MATCH(KW_WRITE)) {
			if (expect_iso)
				SYNTAX_ERROR("SNAPSHOT");

			return TRUE;
		}

		if (!(MATCH(KW_COMMITTED) || MATCH(KW_UNCOMMITTED)))
			SYNTAX_ERROR("COMMITTED");

		trans->tra_flags |= TRA_read_committed;

		if (MATCH(KW_NO)) {
			if (MATCH(KW_VERSION))
				return TRUE;
			else if (MATCH(KW_WAIT)) {
				trans->tra_flags |= TRA_nw;
				return TRUE;
			}
			else
				SYNTAX_ERROR("WAIT or VERSION");
		}

		if (MATCH(KW_VERSION))
			trans->tra_flags |= TRA_rec_version;

		return TRUE;
	}
	else if (MATCH(KW_SNAPSHOT)) {
		if (MATCH(KW_TABLE)) {
			trans->tra_flags |= TRA_con;

			MATCH(KW_STABILITY);
			return TRUE;
		}
		else
			return TRUE;
	}
	return FALSE;
}


//____________________________________________________________
//  
//		Parse the USING clause for a dynamic SQL statement.
//  

static BOOLEAN par_using( DYN statement)
{

	if (!MATCH(KW_USING))
		return FALSE;

	MATCH(KW_SQL);				/* keyword "SQL" is optional for backward compatibility */

	if (MATCH(KW_DESCRIPTOR))
		statement->dyn_sqlda = PAR_native_value(FALSE, FALSE);
	else
		statement->dyn_using =
			(GPRE_NOD) SQE_list(reinterpret_cast < pfn_SQE_list_cb >
						   (SQE_variable), 0, FALSE);

	return TRUE;
}


//____________________________________________________________
//  
//		Figure out the correct dtypes
//  

static USHORT resolve_dtypes( KWWORDS typ, BOOLEAN sql_date)
{
	TEXT err_mesg[128];

	switch (typ) {
	case KW_DATE:
		if ((sw_ods_version < 10) || (sw_server_version < 6))
			switch (sw_sql_dialect) {
			case 1:
				return dtype_timestamp;
			case 2:
				sprintf(err_mesg,
						"Encountered column type DATE which is ambiguous in dialect %d\n",
						sw_sql_dialect);
				PAR_error(err_mesg);
				return dtype_null;	/* TMN: FIX FIX */
				/* return; */
			default:
				sprintf(err_mesg,
						"Encountered column type DATE which is not supported in ods version %d\n",
						sw_ods_version);
				PAR_error(err_mesg);
			}
		else {
		/** column definition SQL DATE is unambiguous in any dialect **/
			if (sql_date)
				return dtype_sql_date;
			switch (sw_sql_dialect) {
			case 1:
				return dtype_timestamp;
			case 2:
				sprintf(err_mesg,
						"Encountered column type DATE which is ambiguous in dialect %d\n",
						sw_sql_dialect);
				PAR_error(err_mesg);
				return dtype_null;	/* TMN: FIX FIX */
				/* return; */
			default:
				return dtype_sql_date;
			}
		}
		break;

	case KW_TIME:
		if ((sw_ods_version < 10) || (sw_server_version < 6)) {
			sprintf(err_mesg,
					"Encountered column type TIME which is not supported by pre 6.0 Servers\n");
			PAR_error(err_mesg);
			return dtype_null;	/* TMN: FIX FIX */
			/* return; */
		}
		else
			return dtype_sql_time;
		break;

	case KW_TIMESTAMP:
		return dtype_timestamp;
		break;

	default:
		sprintf(err_mesg, "resolve_dtypes(): Unknown dtype %d\n", typ);
		PAR_error(err_mesg);
		break;
	}
//  
//  TMN: FIX FIX Added "return dtype_null;" to silence compiler, but
//  this is really a logic error we have to fix.
//  
	return dtype_null;
}


//____________________________________________________________
//  
//		Parse the tail of a CREATE DATABASE statement.
//  

static BOOLEAN tail_database( enum act_t action_type, DBB database)
{
	FIL file;
	TEXT *string;
	BOOLEAN extend_dpb, logdefined;

//  parse options for the database parameter block 

	logdefined = FALSE;
	extend_dpb = FALSE;
	database->dbb_grp_cmt_wait = -1;

	while (TRUE) {
		if (MATCH(KW_LENGTH)) {
			database->dbb_length = EXP_ULONG_ordinal(TRUE);
			MATCH(KW_PAGES);
		}
		else if (MATCH(KW_USER)) {
			if (QUOTED(token.tok_type)) {
				database->dbb_c_user = string =
					(TEXT *) ALLOC(token.tok_length + 1);
				COPY(token.tok_string, token.tok_length, string);

				string[token.tok_length - 2] = '\0';
				ADVANCE_TOKEN;
			}
			else
				/* Some non-C languages (such as Fortran) have problems
				 * with extending the static DPB to add the runtime options
				 * needed for a runtime user name or password.
				 * 11 April 1995
				 */
			if (sw_language == lang_c) {
				if (!MATCH(KW_COLON))
					SYNTAX_ERROR("<colon> or <quoted string>");
		/** Should I bother about dialects here ?? **/
				if (QUOTED(token.tok_type))
					SYNTAX_ERROR("<host variable>");
				database->dbb_r_user = PAR_native_value(FALSE, FALSE);
				extend_dpb = TRUE;
			}
			else
				SYNTAX_ERROR("<quoted string>");

		}
		else if (MATCH(KW_PASSWORD)) {
			if (QUOTED(token.tok_type)) {
				database->dbb_c_password = string =
					(TEXT *) ALLOC(token.tok_length + 1);
				COPY(token.tok_string, token.tok_length, string);
				string[token.tok_length] = '\0';
				ADVANCE_TOKEN;
			}
			else
				/* Some non-C languages (such as Fortran) have problems
				 * with extending the static DPB to add the runtime options
				 * needed for a runtime user name or password.
				 * 11 April 1995
				 */
			if (sw_language == lang_c) {
				if (!MATCH(KW_COLON))
					SYNTAX_ERROR("<colon> or <quoted string>");
				if (QUOTED(token.tok_type))
					SYNTAX_ERROR("<host variable>");
				database->dbb_r_password = PAR_native_value(FALSE, FALSE);
				extend_dpb = TRUE;
			}
			else
				SYNTAX_ERROR("<quoted string>");
		}
		else if (MATCH(KW_DEFAULT)) {
			if (!MATCH(KW_CHAR) || !MATCH(KW_SET))
				SYNTAX_ERROR("CHARACTER SET");
			database->dbb_def_charset = PAR_native_value(FALSE, FALSE);
		}
		else
			break;
	}

	if (action_type == ACT_drop_database) {
		if (MATCH(KW_CASCADE))
			database->dbb_flags |= DBB_cascade;
		/* TMN: ERROR ERROR we cant return _nothing* from a function returning */
		/* a BOOLEAN. I changed this to FALSE to falg an error, but we have to */
		/* look into this. */
		return FALSE;
		/* return; */
	}

//  parse add/drop items 

	while (TRUE) {
		MATCH(KW_ADD);
		if (MATCH(KW_DROP)) {
			if (MATCH(KW_LOG_FILE))
				database->dbb_flags |= DBB_drop_log;
			else if (MATCH(KW_CASCADE))
				database->dbb_flags |= DBB_cascade;
#ifdef FLINT_CACHE
			else if (MATCH(KW_CACHE))
				database->dbb_flags |= DBB_drop_cache;
			else
				PAR_error("only log files or shared cache can be dropped");	/* msg 121 only SECURITY_CLASS, DESCRIPTION and CACHE can be dropped */
#else
			else
				PAR_error("only log files can be dropped");	/* msg 121 only SECURITY_CLASS, DESCRIPTION and CACHE can be dropped */
#endif /* FLINT_CACHE */

// ****
// else if (MATCH (KW_DESCRIP))
//    database->dbb_flags	|= DBB_null_description;
// else if (MATCH (KW_SECURITY_CLASS))
//    database->dbb_flags |= DBB_null_security_class;
//***
		}
// ****
//   else if (KEYWORD (KW_DESCRIPTION))
// database->dbb_description = parse_description();
//   else if (MATCH (KW_SECURITY_CLASS))
// database->dbb_security_class = PARSE_symbol (tok_ident); 
//***
		else if (MATCH(KW_FILE)) {
			file = define_file();
			file->fil_next = database->dbb_files;
			database->dbb_files = file;
		}
		else if (MATCH(KW_PAGE_SIZE) || MATCH(KW_PAGESIZE)) {
			if (action_type == ACT_alter_database)
				PAR_error("PAGE SIZE can not be modified");
			database->dbb_pagesize = par_page_size();
		}
		else if (MATCH(KW_CHECK_POINT_LEN)) {
			MATCH(KW_EQUALS);
			database->dbb_chkptlen = EXP_ULONG_ordinal(TRUE);
		}
		else if (MATCH(KW_NUM_LOG_BUFS)) {
			MATCH(KW_EQUALS);
			database->dbb_numbufs = EXP_USHORT_ordinal(TRUE);
		}
		else if (MATCH(KW_LOG_BUF_SIZE)) {
			MATCH(KW_EQUALS);
			database->dbb_bufsize = EXP_USHORT_ordinal(TRUE);
		}
		else if (MATCH(KW_GROUP_COMMIT_WAIT)) {
			MATCH(KW_EQUALS);
			database->dbb_grp_cmt_wait = EXP_ULONG_ordinal(TRUE);
		}
		else if (MATCH(KW_LOG_FILE)) {
			if (logdefined)
				PAR_error("log redefinition");
			logdefined = TRUE;
			if (MATCH(KW_LEFT_PAREN)) {
				while (TRUE) {
					file = define_log_file(FALSE);
					file->fil_next = database->dbb_logfiles;
					database->dbb_logfiles = file;
					if (!MATCH(KW_COMMA)) {
						EXP_match_paren();
						break;
					}
				}

				if (MATCH(KW_OVERFLOW))
					database->dbb_overflow = define_log_file(TRUE);
				else
					PAR_error
						("Overflow log specification required for this configuration");
			}
			else if (MATCH(KW_BASE_NAME)) {
				database->dbb_flags |= DBB_log_serial;
				database->dbb_logfiles = file = define_log_file(TRUE);
			}
			else
				database->dbb_flags |= DBB_log_default;
		}
#ifdef FLINT_CACHE
		else if (MATCH(KW_CACHE))
			database->dbb_cache_file = define_cache();
#endif /* FLINT_CACHE */
		else
			break;
	}

	return extend_dpb;
}


//____________________________________________________________
//  
//		Upcase a string into another string.
//  

static void to_upcase( TEXT * p, TEXT * q)
{
	UCHAR c;
	USHORT l;

	l = 0;
	while ((c = *p++) && (++l <= NAME_SIZE)) {
		*q++ = UPPER(c);
	}
	*q = 0;
}


//____________________________________________________________
//  
//  

void SQL_resolve_identifier( TEXT * err_mesg, TEXT * str)
{

	switch (sw_sql_dialect) {
	case 2:
		if (DOUBLE_QUOTED(token.tok_type))
			PAR_error("Ambiguous use of double quotes in dialect 2");
	case 1:
		if (token.tok_type != tok_ident)
			SYNTAX_ERROR(err_mesg);
		else
			to_upcase(token.tok_string, str);
		break;
	case 3:
		if (DOUBLE_QUOTED(token.tok_type)) {
			if (DOUBLE_QUOTES_ON(token.tok_string))
				STRIP_QUOTES(token)
					strcpy(str, token.tok_string);
		}
		else if (token.tok_type == tok_ident)
			to_upcase(token.tok_string, str);
		else
			SYNTAX_ERROR(err_mesg);
		break;
	}
	strcpy(token.tok_string, str);
}


static void dialect1_bad_type(USHORT field_dtype)
{
	char buffer[200];
	char *s;

	switch (field_dtype) {
	case dtype_sql_date:
		s = "SQL DATE";
		break;
	case dtype_sql_time:
		s = "SQL TIME";
		break;
	case dtype_int64:
		s = "64-bit numeric";
		break;
	}
	sprintf(buffer,
			"Client SQL dialect 1 does not support reference to the %s datatype",
			s);
	PAR_error(buffer);
}
