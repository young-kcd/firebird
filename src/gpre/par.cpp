//____________________________________________________________
//  
//		PROGRAM:	C preprocessor
//		MODULE:		par.cpp
//		DESCRIPTION:	Parser
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
//  $Id: par.cpp,v 1.26 2003-09-06 00:52:10 brodsom Exp $
//  Revision 1.2  2000/11/27 09:26:13  fsg
//  Fixed bugs in gpre to handle PYXIS forms
//  and allow edit.e and fred.e to go through
//  gpre without errors (and correct result).
//  
//  This is a partial fix until all
//  PYXIS datatypes are adjusted in frm_trn.c
//  
//  removed some compiler warnings too
//  
//  TMN (Mike Nordell) 11.APR.2001 - Reduce compiler warnings
//  TMN (Mike Nordell) APR-MAY.2001 - Conversion to C++
//  
//
//____________________________________________________________
//
//	$Id: par.cpp,v 1.26 2003-09-06 00:52:10 brodsom Exp $
//

#include "firebird.h"
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include "../jrd/gds.h"
#include "../gpre/gpre.h"
#include "../gpre/parse.h"
#include "../gpre/form.h"
#include "../gpre/cmp_proto.h"
#include "../gpre/exp_proto.h"
#include "../gpre/form_proto.h"
#include "../gpre/gpre_proto.h"
#include "../gpre/hsh_proto.h"
#include "../gpre/gpre_meta.h"
#include "../gpre/msc_proto.h"
#include "../gpre/par_proto.h"
#include "../gpre/sql_proto.h"


ACT			cur_routine;

extern TEXT*	module_lc_ctype;

static jmp_buf*	PAR_jmp_buf;

#ifdef FTN_BLK_DATA
static void		block_data_list(DBB);
#endif
static BOOLEAN	match_parentheses();
static ACT		par_any();
static ACT		par_array_element();
static ACT		par_at();
static ACT		par_based();
static ACT		par_begin();
static BLB		par_blob();
static ACT		par_blob_action(ACT_T);
static ACT		par_blob_field();
static ACT		par_case();
static ACT		par_clear_handles();
static ACT		par_derived_from();
static ACT		par_end_block();
static ACT		par_end_error();
static ACT		par_end_fetch();
static ACT		par_end_for();
#ifdef PYXIS
static ACT		par_end_form();
static ACT		par_end_menu();
#endif
static ACT		par_end_modify();
static ACT		par_end_stream();
static ACT		par_end_store(bool);
#ifdef PYXIS
static ACT		par_entree();
#endif
static ACT		par_erase();
static ACT		par_fetch();
static ACT		par_finish();
static ACT		par_for();
#ifdef PYXIS
static GPRE_CTX		par_form_menu(enum sym_t);
static ACT		par_form_display();
static ACT		par_form_field();
static void		par_form_fields(GPRE_REQ, LLS *);
static ACT		par_form_for();
#endif
static ACT		par_function();
#ifdef PYXIS
static ACT		par_item_end();
static ACT		par_item_for(ACT_T);
#endif
static ACT		par_left_brace();
#ifdef PYXIS
static ACT		par_menu_att();
static ACT		par_menu_case();
static ACT		par_menu_display(GPRE_CTX);
static ACT		par_menu_entree_att();
static ACT		par_menu_for();
static ACT		par_menu_item_for(SYM, GPRE_CTX, ACT_T);
#endif
static ACT		par_modify();
static ACT		par_on();
static ACT		par_on_error();
static ACT		par_open_blob(ACT_T, SYM);
static BOOLEAN	par_options(GPRE_REQ, BOOLEAN);
static ACT		par_procedure();
#ifdef PYXIS
static TEXT*	par_quoted_string();
#endif
static ACT		par_ready();
static ACT		par_returning_values();
static ACT		par_right_brace();
static ACT		par_release();
static ACT		par_slice(ACT_T);
static ACT		par_store();
static ACT		par_start_stream();
static ACT		par_start_transaction();
static ACT		par_subroutine();
static ACT		par_trans(ACT_T);
static ACT		par_type();
static ACT		par_variable();
#ifdef PYXIS
static ACT		par_window_create();
static ACT		par_window_delete();
static ACT		par_window_scope();
static ACT		par_window_suspend();
#endif
static ACT		scan_routine_header();
static void		set_external_flag();
static BOOLEAN	terminator();

static int		brace_count;
static int		routine_decl;
static BOOLEAN	bas_extern_flag;
static ACT		cur_statement;
static ACT		cur_item;
static LLS		cur_for;
static LLS		cur_store;
static LLS		cur_fetch;
static LLS		cur_modify;
static LLS		cur_form;
static LLS		cur_error;
static LLS		cur_menu;
static LLS		routine_stack;
static GPRE_FLD		flag_field;


//____________________________________________________________
//  
//		We have a token with a symbolic meaning.  If appropriate,
//		parse an action segment.  If not, return NULL.
//  

ACT PAR_action(TEXT* base_dir)
{
	ACT action;
	SYM symbol;
	enum kwwords keyword;
	jmp_buf env;

	symbol = token.tok_symbol;

	if (!symbol) {
		cur_statement = NULL;
		return NULL;
	}

	if ((USHORT) token.tok_keyword >= (USHORT) KW_start_actions &&
		(USHORT) token.tok_keyword <= (USHORT) KW_end_actions)
	{
		keyword = token.tok_keyword;
		switch (keyword)
		{
		case KW_READY:
		case KW_START_TRANSACTION:
		case KW_FINISH:
		case KW_COMMIT:
		case KW_PREPARE:
		case KW_RELEASE_REQUESTS:
		case KW_ROLLBACK:
		case KW_FUNCTION:
		case KW_SAVE:
		case KW_SUB:
		case KW_SUBROUTINE:
			CPR_eol_token();
			break;

		case KW_EXTERNAL:
			set_external_flag();
			return NULL;

		case KW_FOR:
		/** Get the next token as it is without upcasing **/
			override_case = 1;
			CPR_token();
			break;

		default:
			CPR_token();
		}

		try {

		PAR_jmp_buf = &env;

		switch (keyword)
		{
		case KW_INT:
		case KW_LONG:
		case KW_SHORT:
		case KW_CHAR:
		case KW_FLOAT:
		case KW_DOUBLE:
// ***
//    par_var_c (keyword);
//** 
			return NULL;

		case KW_ANY:
			return par_any();
		case KW_AT:
			return par_at();
		case KW_BASED:
			return par_based();
		case KW_CLEAR_HANDLES:
			return par_clear_handles();
#ifdef PYXIS
		case KW_CREATE_WINDOW:
			return par_window_create();
#endif
		case KW_DATABASE:
			return PAR_database(FALSE, base_dir);
#ifdef PYXIS
		case KW_DELETE_WINDOW:
			return par_window_delete();
#endif
		case KW_DERIVED_FROM:
			return par_derived_from();
#ifdef PYXIS
		case KW_DISPLAY:
			return par_form_display();
#endif
		case KW_END_ERROR:
			return par_end_error();
		case KW_END_FOR:
			return cur_statement = par_end_for();
#ifdef PYXIS
		case KW_END_FORM:
			return par_end_form();
		case KW_END_ITEM:
			return par_item_end();
		case KW_END_MENU:
			return par_end_menu();
#endif
		case KW_END_MODIFY:
			return cur_statement = par_end_modify();
		case KW_END_STREAM:
			return cur_statement = par_end_stream();
		case KW_END_STORE:
			return cur_statement = par_end_store(false);
		case KW_END_STORE_SPECIAL:
			return cur_statement = par_end_store(true);
#ifdef PYXIS
		case KW_MENU_ENTREE:
			return par_entree();
#endif
		case KW_ELEMENT:
			return par_array_element();
		case KW_ERASE:
			return cur_statement = par_erase();
		case KW_EVENT_INIT:
			return cur_statement = PAR_event_init(FALSE);
		case KW_EVENT_WAIT:
			return cur_statement = PAR_event_wait(FALSE);
		case KW_FETCH:
			return cur_statement = par_fetch();
		case KW_FINISH:
			return cur_statement = par_finish();
		case KW_FOR:
			return par_for();
#ifdef PYXIS
		case KW_FOR_FORM:
			return par_form_for();
		case KW_FOR_ITEM:
			return par_item_for(ACT_item_for);
		case KW_FOR_MENU:
			return par_menu_for();
#endif
		case KW_END_FETCH:
			return cur_statement = par_end_fetch();
#ifdef PYXIS
		case KW_CASE_MENU:
			return par_menu_case();
#endif
		case KW_MODIFY:
			return par_modify();
		case KW_ON:
			return par_on();
		case KW_ON_ERROR:
			return par_on_error();
#ifdef PYXIS
		case KW_PUT_ITEM:
			return par_item_for(ACT_item_put);
#endif
		case KW_READY:
			return cur_statement = par_ready();
		case KW_RELEASE_REQUESTS:
			return cur_statement = par_release();
		case KW_RETURNING:
			return par_returning_values();
		case KW_START_STREAM:
			return cur_statement = par_start_stream();
		case KW_STORE:
			return par_store();
		case KW_START_TRANSACTION:
			return cur_statement = par_start_transaction();
#ifdef PYXIS
		case KW_SUSPEND_WINDOW:
			return par_window_suspend();
#endif
		case KW_FUNCTION:
			return par_function();
#ifdef PYXIS
		case KW_GDS_WINDOWS:
			return par_window_scope();
#endif
		case KW_PROCEDURE:
			return par_procedure();

		case KW_PROC:
			break;

		case KW_SUBROUTINE:
			return par_subroutine();
		case KW_SUB:
			break;

		case KW_OPEN_BLOB:
			return cur_statement = par_open_blob(ACT_blob_open, 0);
		case KW_CREATE_BLOB:
			return cur_statement = par_open_blob(ACT_blob_create, 0);
		case KW_GET_SLICE:
			return cur_statement = par_slice(ACT_get_slice);
		case KW_PUT_SLICE:
			return cur_statement = par_slice(ACT_put_slice);
		case KW_GET_SEGMENT:
			return cur_statement = par_blob_action(ACT_get_segment);
		case KW_PUT_SEGMENT:
			return cur_statement = par_blob_action(ACT_put_segment);
		case KW_CLOSE_BLOB:
			return cur_statement = par_blob_action(ACT_blob_close);
		case KW_CANCEL_BLOB:
			return cur_statement = par_blob_action(ACT_blob_cancel);

		case KW_COMMIT:
			return cur_statement = par_trans(ACT_commit);
		case KW_SAVE:
			return cur_statement = par_trans(ACT_commit_retain_context);
		case KW_ROLLBACK:
			return cur_statement = par_trans(ACT_rollback);
		case KW_PREPARE:
			return cur_statement = par_trans(ACT_prepare);

		case KW_L_BRACE:
			return par_left_brace();
		case KW_R_BRACE:
			return par_right_brace();
		case KW_END:
			return par_end_block();
		case KW_BEGIN:
			return par_begin();
		case KW_CASE:
			return par_case();

		case KW_EXEC:
			if (!MATCH(KW_SQL))
				break;
			sw_sql = TRUE;
			action = SQL_action(base_dir);
			sw_sql = FALSE;
			return action;
		default:
			break;
		}

		}	// try
		catch (const std::exception&) {
			sw_sql = FALSE;
			/* This is to force GPRE to get the next symbol. Fix for bug #274. DROOT */
			token.tok_symbol = NULL;
			return NULL;
		}

		cur_statement = NULL;
		return NULL;
	}

	for (; symbol; symbol = symbol->sym_homonym)
	{
		switch (symbol->sym_type)
		{
		case SYM_context:
			try {
				PAR_jmp_buf = &env;
				cur_statement = NULL;
				return par_variable();
			}
			catch (const std::exception&) {
				return 0;
			}
#ifdef PYXIS
		case SYM_form_map:
			try {
				PAR_jmp_buf = &env;
				cur_statement = NULL;
				return par_form_field();
			}
			catch (const std::exception&) {
				return 0;
			}
#endif
		case SYM_blob:
			try {
				PAR_jmp_buf = &env;
				cur_statement = NULL;
				return par_blob_field();
			}
			catch (const std::exception&) {
				return 0;
			}
		case SYM_relation:
			try {
				PAR_jmp_buf = &env;
				cur_statement = NULL;
				return par_type();
			}
			catch (const std::exception&) {
				return 0;
			}
#ifdef PYXIS
		case SYM_menu:
			try {
				PAR_jmp_buf = &env;
				cur_statement = NULL;
				return par_menu_att();
			}
			catch (const std::exception&) {
				return 0;
			}
		case SYM_menu_map:
			try {
				PAR_jmp_buf = &env;
				cur_statement = NULL;
				return par_menu_entree_att();
			}
			catch (const std::exception&) {
				return 0;
			}
#endif
		default:
			break;
		}
	}

	cur_statement = NULL;
	CPR_token();

	return NULL;
}


//____________________________________________________________
//  
//		Parse a blob subtype -- either a signed number or a symbolic name.
//  

SSHORT PAR_blob_subtype(DBB db)
{
	GPRE_REL relation;
	GPRE_FLD field;
	SSHORT const_subtype;

//  Check for symbol type name 

	if (token.tok_type == tok_ident) {
		if (!(relation = MET_get_relation(db, "RDB$FIELDS", "")) ||
			!(field = MET_field(relation, "RDB$FIELD_SUB_TYPE")))
		{
			PAR_error("error during BLOB SUB_TYPE lookup");
		}
		if (!MET_type(field, token.tok_string, &const_subtype))
			SYNTAX_ERROR("blob sub_type");
		ADVANCE_TOKEN;
		return const_subtype;
	}

	return EXP_SSHORT_ordinal(TRUE);
}


//____________________________________________________________
//  
//		Parse a DATABASE declaration.  If successful, return
//		an action block.
//  

ACT PAR_database(USHORT sql, TEXT* base_directory)
{
	ACT action;
	SYM symbol;
	DBB db, *db_ptr;
	TEXT s[256], *string;

	action = MAKE_ACTION(0, ACT_database);
	db = (DBB) ALLOC(DBB_LEN);

//  Get handle name token, make symbol for handle, and
//  insert symbol into hash table 

	symbol = PAR_symbol(SYM_dummy);
	db->dbb_name = symbol;
	symbol->sym_type = SYM_database;
	symbol->sym_object = (GPRE_CTX) db;

	if (!MATCH(KW_EQUALS))
		SYNTAX_ERROR("\"=\" in database declaration");

	if (MATCH(KW_STATIC))
		db->dbb_scope = DBB_STATIC;
	else if (MATCH(KW_EXTERN))
		db->dbb_scope = DBB_EXTERN;

	MATCH(KW_COMPILETIME);

//  parse the compiletime options 

	for (;;) {
		if (MATCH(KW_FILENAME) && (!QUOTED(token.tok_type)))
			SYNTAX_ERROR("quoted file name");

		if (QUOTED(token.tok_type)) {
			if (base_directory){
				db->dbb_filename = string = (TEXT *) ALLOC(token.tok_length + 
													strlen(base_directory) + 1);
				COPY_CAT(base_directory, strlen(base_directory), 
						 token.tok_string, token.tok_length, string);
			}
			else {
				db->dbb_filename = string = (TEXT *) ALLOC(token.tok_length + 1);
				COPY(token.tok_string, token.tok_length, string);
			}
			token.tok_length += 2;
		}
		else if (MATCH(KW_PASSWORD)) {
			if (!QUOTED(token.tok_type))
				SYNTAX_ERROR("quoted password");
			db->dbb_c_password = string =
				(TEXT *) ALLOC(token.tok_length + 1);
			COPY(token.tok_string, token.tok_length, string);
		}
		else if (MATCH(KW_USER)) {
			if (!QUOTED(token.tok_type))
				SYNTAX_ERROR("quoted user name");
			db->dbb_c_user = string = (TEXT *) ALLOC(token.tok_length + 1);
			COPY(token.tok_string, token.tok_length, string);
		}
		else if (MATCH(KW_LC_MESSAGES)) {
			if (!QUOTED(token.tok_type))
				SYNTAX_ERROR("quoted language name");
			db->dbb_c_lc_messages = string =
				(TEXT *) ALLOC(token.tok_length + 1);
			COPY(token.tok_string, token.tok_length, string);
		}
		else if (!sql && MATCH(KW_LC_CTYPE)) {
			if (!QUOTED(token.tok_type))
				SYNTAX_ERROR("quoted character set name");
			db->dbb_c_lc_ctype = string =
				(TEXT *) ALLOC(token.tok_length + 1);
			COPY(token.tok_string, token.tok_length, string);
		}
		else
			break;

		ADVANCE_TOKEN;
	}

	if ((sw_auto) && (db->dbb_c_password || db->dbb_c_user || db->dbb_c_lc_ctype
			|| db->dbb_c_lc_messages))
	{
			CPR_warn
			("PASSWORD, USER and NAMES options require -manual switch to gpre.");
	}

	if (!db->dbb_filename)
		SYNTAX_ERROR("quoted file name");

	if (default_user && !db->dbb_c_user)
		db->dbb_c_user = default_user;

	if (default_password && !db->dbb_c_password)
		db->dbb_c_password = default_password;

	if (module_lc_ctype && !db->dbb_c_lc_ctype)
		db->dbb_c_lc_ctype = module_lc_ctype;

	if (default_lc_ctype && !db->dbb_c_lc_ctype)
		db->dbb_c_lc_ctype = default_lc_ctype;

//  parse the runtime options 

	if (MATCH(KW_RUNTIME)) {
		if (MATCH(KW_FILENAME))
			db->dbb_runtime = sql ? SQL_var_or_string((USHORT) FALSE)
				: PAR_native_value(FALSE, FALSE);
		else if (MATCH(KW_PASSWORD))
			db->dbb_r_password = sql ? SQL_var_or_string((USHORT) FALSE)
				: PAR_native_value(FALSE, FALSE);
		else if (MATCH(KW_USER))
			db->dbb_r_user = sql ? SQL_var_or_string((USHORT) FALSE)
				: PAR_native_value(FALSE, FALSE);
		else if (MATCH(KW_LC_MESSAGES))
			db->dbb_r_lc_messages = sql ? SQL_var_or_string((USHORT) FALSE)
				: PAR_native_value(FALSE, FALSE);
		else if (!sql && MATCH(KW_LC_CTYPE))
			db->dbb_r_lc_ctype = sql ? SQL_var_or_string((USHORT) FALSE)
				: PAR_native_value(FALSE, FALSE);
		else
			db->dbb_runtime = sql ? SQL_var_or_string((USHORT) FALSE)
				: PAR_native_value(FALSE, FALSE);
	}

	if ((sw_language == lang_ada) && (KEYWORD(KW_HANDLES))) {
		ADVANCE_TOKEN;
		if (QUOTED(token.tok_type))
			SYNTAX_ERROR("quoted file name");
		COPY(token.tok_string, token.tok_length, s);
		strcat(s, ".");
		if (!ada_package[0] || !strcmp(ada_package, s))
			strcpy(ada_package, s);
		else {
			sprintf(s,
					"Ada handle package \"%s\" already in use, ignoring package %s",
					ada_package, token.tok_string);
			CPR_warn(s);
		}
		ADVANCE_TOKEN;
	}

	if (sw_language != lang_fortran)
		MATCH(KW_SEMI_COLON);

	if (!MET_database(db, TRUE)) {
		sprintf(s, "Couldn't access database %s = '%s'",
				db->dbb_name->sym_string, db->dbb_filename);
		CPR_error(s);
		CPR_abort();
	}

	db->dbb_next = isc_databases;
	isc_databases = db;
	HSH_insert(symbol);

//   Load up the symbol (hash) table with relation names from this databases.  
	MET_load_hash_table(db);

#ifdef FTN_BLK_DATA
	if ((sw_language == lang_fortran) && (db->dbb_scope != DBB_EXTERN))
		block_data_list(db);
#endif

//  Since we have a real DATABASE statement, get rid of any artificial
//  databases that were created because of an INCLUDE SQLCA statement. 

	for (db_ptr = &isc_databases; *db_ptr;)
		if ((*db_ptr)->dbb_flags & DBB_sqlca)
			*db_ptr = (*db_ptr)->dbb_next;
		else
			db_ptr = &(*db_ptr)->dbb_next;

	return action;
}


//____________________________________________________________
//  
//		Parse end of statement.  All languages except ADA leave
//		the trailing semi-colon dangling.   ADA, however, must
//		eat the semi-colon as part of the statement.  In any case,
//		return TRUE is a semi-colon is/was there, otherwise return
//		FALSE.
//  

BOOLEAN PAR_end()
{

	if ((sw_language == lang_ada) ||
		(sw_language == lang_c) ||
		(isLangCpp(sw_language))) {
        return MATCH(KW_SEMI_COLON);
    }

	return KEYWORD(KW_SEMI_COLON);
}


//____________________________________________________________
//  
//		Report an error during parse and unwind.
//  

void PAR_error( TEXT * string)
{

	IBERROR(string);
	PAR_unwind();
}


//____________________________________________________________
//  
//		Parse an event init statement, preparing
//		to wait on a number of named events.
//  

ACT PAR_event_init( USHORT sql)
{
	GPRE_NOD init, event_list, node, *ptr;
	ACT action;
	LLS stack = NULL;
	SYM symbol;
	GPRE_FLD field;
	GPRE_CTX context;
	REF reference;
	int count = 0;
	char req_name[128];

//  make up statement node 

	SQL_resolve_identifier("<identifier>", req_name);
	strcpy(token.tok_string, req_name);
	init = MAKE_NODE(nod_event_init, 4);
	init->nod_arg[0] = (GPRE_NOD) PAR_symbol(SYM_dummy);
	init->nod_arg[3] = (GPRE_NOD) isc_databases;

	action = MAKE_ACTION(0, ACT_event_init);
	action->act_object = (REF) init;

//  parse optional database handle 

	if (!MATCH(KW_LEFT_PAREN)) {
		if ((symbol = token.tok_symbol) && (symbol->sym_type == SYM_database))
			init->nod_arg[3] = (GPRE_NOD) symbol->sym_object;
		else
			SYNTAX_ERROR("left parenthesis or database handle");

		ADVANCE_TOKEN;
		if (!MATCH(KW_LEFT_PAREN))
			SYNTAX_ERROR("left parenthesis");
	}

//  eat any number of event strings until a right paren is found,
//  pushing the events onto a stack 

	while (TRUE) {
		if (MATCH(KW_RIGHT_PAREN))
			break;

		if (!sql && (symbol = token.tok_symbol)
			&& symbol->sym_type == SYM_context) {
			field = EXP_field(&context);
			reference = EXP_post_field(field, context, FALSE);
			node = MAKE_NODE(nod_field, 1);
			node->nod_arg[0] = (GPRE_NOD) reference;
		}
		else {
			node = MAKE_NODE(nod_null, 1);
			if (sql)
				node->nod_arg[0] = (GPRE_NOD) SQL_var_or_string((USHORT) FALSE);
			else
				node->nod_arg[0] = (GPRE_NOD) PAR_native_value(FALSE, FALSE);
		}
		PUSH(node, &stack);
		count++;

		MATCH(KW_COMMA);
	}

//  pop the event strings off the stack 

	event_list = init->nod_arg[1] = MAKE_NODE(nod_list, (SSHORT) count);
	ptr = event_list->nod_arg + count;
	while (stack)
		*--ptr = (GPRE_NOD) POP(&stack);

	PUSH((GPRE_NOD) action, &events);

	if (!sql)
		PAR_end();
	return action;
}


//____________________________________________________________
//  
//		Parse an event wait statement, preparing
//		to wait on a number of named events.
//  

ACT PAR_event_wait(USHORT sql)
{
	ACT action;
	char req_name[132];

//  this is a simple statement, just add a handle 

	action = MAKE_ACTION(0, ACT_event_wait);
	SQL_resolve_identifier("<identifier>", req_name);
	strcpy(token.tok_string, req_name);
	action->act_object = (REF) PAR_symbol(SYM_dummy);
	if (!sql)
		PAR_end();

	return action;
}


//____________________________________________________________
//  
//		Perform any last minute stuff necessary at the end of pass1.
//  

void PAR_fini()
{

	if (cur_for)
		IBERROR("unterminated FOR statement");

	if (cur_modify)
		IBERROR("unterminated MODIFY statement");

	if (cur_store)
		IBERROR("unterminated STORE statement");

	if (cur_error)
		IBERROR("unterminated ON_ERROR clause");

	if (cur_menu)
		IBERROR("unterminated MENU statement");

	if (cur_form)
		IBERROR("unterminated FOR_FORM statement");

	if (cur_item)
		IBERROR("unterminated ITEM statement");
}


//____________________________________________________________
//  
//		Get a token or unwind the parse
//		if we hit end of file
//  

TOK PAR_get_token()
{

	if (CPR_token() == NULL) {
		CPR_error("unexpected EOF");
		PAR_unwind();
	}
	return NULL;
}


//____________________________________________________________
//  
//		Do any initialization necessary.
//		For one thing, set all current indicators
//		to null, since nothing is current.  Also,
//		set up a block to hold the current routine,
//  
//		(The 'routine' indicator tells the code
//		generator where to put ports to support
//		recursive routines and Fortran's strange idea
//		of separate sub-modules.  For PASCAL only, we
//		keep a stack of routines, and pay special attention
//		to the main routine.)
//  

void PAR_init()
{

	SQL_init();

	cur_error = cur_fetch = cur_for = cur_modify = cur_store = cur_form =
		cur_menu = NULL;
	cur_statement = cur_item = NULL;
	bas_extern_flag = FALSE;

	cur_routine = MAKE_ACTION(0, ACT_routine);
	cur_routine->act_flags |= ACT_main;
	PUSH((GPRE_NOD) cur_routine, &routine_stack);
	routine_decl = TRUE;

	flag_field = NULL;
	brace_count = 0;
}


//____________________________________________________________
//  
//		Parse a native expression as a string.
//  

TEXT *PAR_native_value(USHORT array_ref, USHORT handle_ref)
{
	SCHAR *s2, buffer[512];
	SCHAR *string, *s1;
	enum kwwords keyword;
	int length;
	USHORT parens, brackets;

#define GOBBLE {for (s1= token.tok_string; *s1;) *string++ = *s1++; ADVANCE_TOKEN;}

	string = buffer;

	while (TRUE) {
	/** PAR_native_values copies the string constants. These are 
	passed to api calls. Make sure to enclose these with
	double quotes.
    **/
		if (sw_sql_dialect == 1) {
			if (QUOTED(token.tok_type)) {
				enum tok_t typ;
				typ = token.tok_type;
				token.tok_length += 2;
				*string++ = '\"';
				GOBBLE;
				*string++ = '\"';
				break;
			}
		}
		else if (sw_sql_dialect == 2) {
			if (DOUBLE_QUOTED(token.tok_type))
				PAR_error("Ambiguous use of double quotes in dialect 2");
			else if (SINGLE_QUOTED(token.tok_type)) {
				token.tok_length += 2;
				*string++ = '\"';
				GOBBLE;
				*string++ = '\"';
				break;
			}
		}
		else if (sw_sql_dialect == 3) {
			if (SINGLE_QUOTED(token.tok_type)) {
				token.tok_length += 2;
				*string++ = '\"';
				GOBBLE;
				*string++ = '\"';
				break;
			}
		}

		if (KEYWORD(KW_AMPERSAND) || KEYWORD(KW_ASTERISK))
			GOBBLE;
		if (token.tok_type != tok_ident)
			SYNTAX_ERROR("identifier");
		GOBBLE;

		/* For ADA, gobble '<attribute> */

		if ((sw_language == lang_ada) && (token.tok_string[0] == '\'')) {
			GOBBLE;
		}
		keyword = token.tok_keyword;
		if (keyword == KW_LEFT_PAREN) {
			parens = 1;
			while (parens) {
				enum tok_t typ;
				typ = token.tok_type;
				if (QUOTED(typ))
					*string++ = (SINGLE_QUOTED(typ)) ? '\'' : '\"';
				GOBBLE;
				if (QUOTED(typ))
					*string++ = (SINGLE_QUOTED(typ)) ? '\'' : '\"';
				keyword = token.tok_keyword;
				if (keyword == KW_RIGHT_PAREN)
					parens--;
				else if (keyword == KW_LEFT_PAREN)
					parens++;
			}
			GOBBLE;
			keyword = token.tok_keyword;
		}
		while (keyword == KW_L_BRCKET) {
			brackets = 1;
			while (brackets) {
				GOBBLE;
				keyword = token.tok_keyword;
				if (keyword == KW_R_BRCKET)
					brackets--;
				else if (keyword == KW_L_BRCKET)
					brackets++;
			}
			GOBBLE;
			keyword = token.tok_keyword;
		}

		while ((keyword == KW_CARAT) && (sw_language == lang_pascal)) {
			GOBBLE;
			keyword = token.tok_keyword;
		}

		if (
			(keyword == KW_DOT
			 && (!handle_ref || sw_language == lang_c
				 || sw_language == lang_ada)) || keyword == KW_POINTS
			|| (keyword == KW_COLON && !sw_sql && !array_ref)) {
			GOBBLE;
		}
		else
			break;
	}

	length = string - buffer;
	s2 = string = (SCHAR *) ALLOC(length + 1);
	s1 = buffer;

	if (length)
		do
			*string++ = *s1++;
		while (--length);

	return s2;
}


//____________________________________________________________
//  
//		Find a pseudo-field for null.  If there isn't one,
//		make one.
//  

GPRE_FLD PAR_null_field()
{

	if (flag_field)
		return flag_field;

	flag_field =
		MET_make_field("gds__null_flag", dtype_short, sizeof(SSHORT), FALSE);

	return flag_field;
}


//____________________________________________________________
//  
//		Parse the RESERVING clause of the start_transaction & set transaction
//  	statements, creating a partial TPB in the process.  The
//  	TPB just hangs off the end of the transaction block.
//  

void PAR_reserving( USHORT flags, SSHORT parse_sql)
{
	RRL lock_block;
	GPRE_REL relation;
	DBB database;
	USHORT lock_level, lock_mode;

	while (TRUE) {
		/* find a relation name, or maybe a list of them */

		if ((!parse_sql) && terminator())
			break;

		do {
			if (!(relation = EXP_relation()))
				SYNTAX_ERROR("relation name");
			database = relation->rel_database;
			lock_block = (RRL) ALLOC(RRL_LEN);
			lock_block->rrl_next = database->dbb_rrls;
			lock_block->rrl_relation = relation;
			database->dbb_rrls = lock_block;
		} while (MATCH(KW_COMMA));

		/*
		 * get the lock level and mode and apply them to all the
		 * relations in the list
		 */

		MATCH(KW_FOR);
		lock_level = (flags & TRA_con) ? gds_tpb_protected : gds_tpb_shared;
		lock_mode = gds_tpb_lock_read;

		if (MATCH(KW_PROTECTED))
			lock_level = gds_tpb_protected;
		else if (MATCH(KW_EXCLUSIVE))
			lock_level = gds_tpb_exclusive;
		else if (MATCH(KW_SHARED))
			lock_level = gds_tpb_shared;

		if (MATCH(KW_WRITE)) {
			if (flags & TRA_ro)
				IBERROR("write lock requested for a read_only transaction");
			lock_mode = gds_tpb_lock_write;
		}
		else
			MATCH(KW_READ);

		for (database = isc_databases; database;
			 database = database->dbb_next) for (lock_block =
												 database->dbb_rrls;
												 lock_block;
												 lock_block =
												 lock_block->
												 rrl_next) if (!lock_block->
															   rrl_lock_level)
				{
					assert(lock_level <= MAX_UCHAR);
					assert(lock_mode <= MAX_UCHAR);
					lock_block->rrl_lock_level = (UCHAR) lock_level;
					lock_block->rrl_lock_mode = (UCHAR) lock_mode;
				}
		if (!(MATCH(KW_COMMA)))
			break;
	}
}


//____________________________________________________________
//  
//		Initialize the request and the ready.
//  

GPRE_REQ PAR_set_up_dpb_info(RDY ready, ACT action, USHORT buffercount)
{
	GPRE_REQ request;

	ready->rdy_database->dbb_buffercount = buffercount;
	request = MAKE_REQUEST(REQ_ready);
	request->req_database = ready->rdy_database;
	request->req_actions = action;
	ready->rdy_request = request;

	return request;
}


//____________________________________________________________
//  
//		Make a symbol from the current token, and advance
//		to the next token.  If a symbol type other than
//		SYM_dummy, the symbol can be overloaded, but not
//		redefined.
//  

SYM PAR_symbol(enum sym_t type)
{
	SYM symbol;
	TEXT s[128];

	for (symbol = token.tok_symbol; symbol; symbol = symbol->sym_homonym)
		if (type == SYM_dummy || symbol->sym_type == type) {
			sprintf(s, "symbol %s is already in use", token.tok_string);
			PAR_error(s);
		}

	symbol = MSC_symbol(SYM_cursor, token.tok_string, token.tok_length, 0);
	ADVANCE_TOKEN;

	return symbol;
}


//____________________________________________________________
//  
//		There's been a parse error, so unwind out.
//  

void PAR_unwind()
{
	Firebird::status_exception::raise(1);
}


//____________________________________________________________
//  
//		mark databases specified in start_transaction and set transaction
//		statements.
//  

void PAR_using_db()
{
	DBB db;
	SYM symbol;

	while (TRUE) {
		if ((symbol = MSC_find_symbol(token.tok_symbol, SYM_database))) {
			db = (DBB) symbol->sym_object;
			db->dbb_flags |= DBB_in_trans;
		}
		else
			SYNTAX_ERROR("database handle");
		ADVANCE_TOKEN;
		if (!MATCH(KW_COMMA))
			break;
	}
}


#ifdef FTN_BLK_DATA
//____________________________________________________________
//  
//  
//		Damn fortran sometimes only allows global
//		initializations in block data.  This collects
//		names of dbs to be so handled.  
//  

static void block_data_list( DBB db)
{
	DBD list, end;
	TEXT *name;

	if (db->dbb_scope == DBB_EXTERN)
		return;
	name = db->dbb_name->sym_string;
	list = global_db_list;

	if (global_db_count)
		if (global_db_count > 32)
			PAR_error
				("Database limit exceeded: 32 databases per source file.");
		else
			for (end = global_db_list + global_db_count; list < end; list++)
				if (!(strcmp(name, list->dbb_name)))
					return;

	if (global_db_count > 32)
		return;

	strcpy(list->dbb_name, name);
	global_db_count++;
}
#endif


//____________________________________________________________
//  
//  
//		For reasons best left unnamed, we need
//		to skip the contents of a parenthesized
//		list	
//  

static BOOLEAN match_parentheses()
{
	USHORT paren_count;

	paren_count = 0;

	if (MATCH(KW_LEFT_PAREN)) {
		paren_count++;
		while (paren_count) {
			if (MATCH(KW_RIGHT_PAREN))
				paren_count--;
			else if (MATCH(KW_LEFT_PAREN))
				paren_count++;
			else
				ADVANCE_TOKEN;
		}
		return TRUE;
	}
	else
		return FALSE;
}


//____________________________________________________________
//  
//		Parse a free standing ANY expression.
//  

static ACT par_any()
{
	SYM symbol;
	ACT action, function;
	GPRE_REQ request;
	RSE rec_expr;
	GPRE_CTX context;
	GPRE_REL relation;

//  For time being flag as an error 

	PAR_error("Free standing any not supported");

	symbol = NULL;

//  Make up request block.  Since this might not be a database statement,
//  stay ready to back out if necessay. 

	request = MAKE_REQUEST(REQ_any);

	par_options(request, TRUE);
	rec_expr = EXP_rse(request, symbol);
	EXP_rse_cleanup(rec_expr);
	action = MAKE_ACTION(request, ACT_any);

	request->req_rse = rec_expr;
	context = rec_expr->rse_context[0];
	relation = context->ctx_relation;
	request->req_database = relation->rel_database;

	function = MAKE_ACTION(0, ACT_function);
	function->act_object = (REF) action;
	function->act_next = functions;
	functions = function;

	return action;
}


//____________________________________________________________
//  
//		Parse a free reference to a database field in general
//		program context.  If the next keyword isn't a context
//		varying, this isn't an array element reference.
//  

static ACT par_array_element()
{
	GPRE_FLD field, element;
	ACT action;
	REF reference;
	GPRE_REQ request;
	GPRE_CTX context;
	GPRE_NOD node;

	if (!MSC_find_symbol(token.tok_symbol, SYM_context))
		return NULL;

	field = EXP_field(&context);
	request = context->ctx_request;
	node = EXP_array(request, field, FALSE, FALSE);
	reference = MAKE_REFERENCE(&request->req_references);
	reference->ref_expr = node;
	reference->ref_field = element = field->fld_array;
	element->fld_symbol = field->fld_symbol;
	reference->ref_context = context;
	node->nod_arg[0] = (GPRE_NOD) reference;

	action = MAKE_ACTION(request, ACT_variable);
	action->act_object = reference;

	return action;
}


//____________________________________________________________
//  
//		Parse an AT END clause.
//  

static ACT par_at()
{
	ACT action;

	if (!MATCH(KW_END) || !cur_fetch)
		return NULL;

	action = (ACT) cur_fetch->lls_object;

	return MAKE_ACTION(action->act_request, ACT_at_end);
}


//____________________________________________________________
//  
//		Parse a BASED ON clause.  If this
//		is fortran and we don't have a database
//		declared yet, don't parse it completely.
//		If this is PLI look for a left paren or
//		a semi colon to avoid stomping a
//		DECLARE i FIXED BIN BASED (X);
//		or DECLARE LIST (10) FIXED BINARY BASED;
//  

static ACT par_based()
{
	GPRE_FLD field;
	BAS based_on;
	GPRE_REL relation;
	ACT action;
	TEXT s[64];
	TEXT t_str[NAME_SIZE + 1];
	BOOLEAN ambiguous_flag;
	LLS t1, t2, hold;
	int notSegment = 0;			/* a COBOL specific patch */
	char tmpChar[2];			/* a COBOL specific patch */

	MATCH(KW_ON);
	action = MAKE_ACTION(0, ACT_basedon);
	based_on = (BAS) ALLOC(BAS_LEN);
	action->act_object = (REF) based_on;

	if ((sw_language != lang_fortran) || isc_databases) {
		relation = EXP_relation();
		if (!MATCH(KW_DOT))
			SYNTAX_ERROR("dot in qualified field reference");
		SQL_resolve_identifier("<fieldname>", t_str);
		if (!(field = MET_field(relation, token.tok_string))) {
			sprintf(s, "undefined field %s", token.tok_string);
			PAR_error(s);
		}
		if (SQL_DIALECT_V5 == sw_sql_dialect) {
			USHORT field_dtype;
			field_dtype = field->fld_dtype;
			if ((dtype_sql_date == field_dtype) ||
				(dtype_sql_time == field_dtype) ||
				(dtype_int64 == field_dtype)) {
				PAR_error
					("BASED ON impermissible datatype for a dialect-1 program");
			}
		}
		ADVANCE_TOKEN;
		if (sw_language == lang_cobol && KEYWORD(KW_DOT)) {
			strcpy(tmpChar, token.tok_string);
		}
		if (MATCH(KW_DOT)) {
			if (!MATCH(KW_SEGMENT)) {
				if (sw_language != lang_cobol)
					PAR_error
						("only .SEGMENT allowed after qualified field name");
				else {
					strcpy(based_on->bas_terminator, tmpChar);
					notSegment = 1;
				}
			}
			else if (!(field->fld_flags & FLD_blob)) {
				sprintf(s, "field %s is not a blob",
						field->fld_symbol->sym_string);
				PAR_error(s);
			}
			if (notSegment == 0)	/* this is flag is to solve KW_DOT problem
									   in COBOL. should be 0 for all other lang */
				based_on->bas_flags |= BAS_segment;
		}
		based_on->bas_field = field;
	}
	else {
		based_on->bas_rel_name = (STR) ALLOC(token.tok_length + 1);
		COPY(token.tok_string, token.tok_length,
			 (SCHAR *) based_on->bas_rel_name);
		ADVANCE_TOKEN;
		if (!MATCH(KW_DOT))
			PAR_error("expected qualified field name");
		else {
			based_on->bas_fld_name = (STR) ALLOC(token.tok_length + 1);
			COPY(token.tok_string, token.tok_length,
				 (SCHAR *) based_on->bas_fld_name);
			ambiguous_flag = FALSE;
			ADVANCE_TOKEN;
			if (MATCH(KW_DOT)) {
				based_on->bas_db_name = based_on->bas_rel_name;
				based_on->bas_rel_name = based_on->bas_fld_name;
				based_on->bas_fld_name = (STR) ALLOC(token.tok_length + 1);
				COPY(token.tok_string, token.tok_length,
					 (SCHAR *) based_on->bas_fld_name);
				if (KEYWORD(KW_SEGMENT))
					ambiguous_flag = TRUE;
				ADVANCE_TOKEN;
				if (MATCH(KW_DOT)) {
					if (!MATCH(KW_SEGMENT))
						PAR_error("too many qualifiers on field name");
					based_on->bas_flags |= BAS_segment;
					ambiguous_flag = FALSE;
				}
			}
			if (ambiguous_flag)
				based_on->bas_flags |= BAS_ambiguous;
		}

	}

	switch (sw_language) {
	case lang_internal:
	case lang_fortran:
	case lang_epascal:
	case lang_c:
	case lang_cxx:
		do {
			PUSH((GPRE_NOD) PAR_native_value(FALSE, FALSE),
				 &based_on->bas_variables);
		} while (MATCH(KW_COMMA));
		/* 
		   ** bug_4031.  based_on->bas_variables are now in reverse order.
		   ** we must reverse the order so we can output them to the .c 
		   ** file correctly.
		 */
		if (based_on->bas_variables->lls_next) {
			t1 = based_on->bas_variables;	/* last one in the old list */
			t2 = NULL;			/* last one in the new list */
			hold = t2;			/* beginning of new list */

			/* while we still have a next one, keep going thru */
			while (t1->lls_next) {
				/* now find the last one in the list */
				while (t1->lls_next->lls_next)
					t1 = t1->lls_next;

				/* if this is the first time thru, set hold */
				if (hold == NULL) {
					hold = t1->lls_next;
					t2 = hold;
				}
				else {
					/* not first time thru, add this one to the end 
					   ** of the new list */

					t2->lls_next = t1->lls_next;
					t2 = t2->lls_next;
				}
				/* now null out the last one, and start again */
				t1->lls_next = NULL;
				t1 = based_on->bas_variables;
			}
			/* ok, we're done, tack the original lls onto the very
			   ** end of the new list. */

			t2->lls_next = t1;
			if (hold)
				based_on->bas_variables = hold;
		}
	default:
		break;
	}

	if (notSegment)
		return action;
	if (KEYWORD(KW_SEMI_COLON) ||
		(sw_language == lang_cobol && KEYWORD(KW_DOT))) {
		strcpy(based_on->bas_terminator, token.tok_string);
		ADVANCE_TOKEN;
	}

	return action;
}


//____________________________________________________________
//  
//		If this is a PASCAL program, and we're
//  	in a code block, then increment the 
//		brace count.  If we're in a routine
//		declaration, then we've reached the start
//		of the code block and should mark it as
//		a new routine.
//  

static ACT par_begin()
{

	if (sw_language == lang_pascal) {
		routine_decl = FALSE;
		cur_routine->act_count++;
	}
	return NULL;
}


//____________________________________________________________
//  
//		Parse a blob handle and return the blob.
//  

static BLB par_blob()
{
	SYM symbol;

	if (!(symbol = MSC_find_symbol(token.tok_symbol, SYM_blob)))
		SYNTAX_ERROR("blob handle");

	ADVANCE_TOKEN;

	return (BLB) symbol->sym_object;
}


//____________________________________________________________
//  
//		Parse a GET_SEGMENT, PUT_SEGMENT, CLOSE_BLOB or CANCEL_BLOB.
//  

static ACT par_blob_action( ACT_T type)
{
	BLB blob;
	ACT action;

	blob = par_blob();
	action = MAKE_ACTION(blob->blb_request, type);
	action->act_object = (REF) blob;

//   Need to eat the semicolon if present  

	if (sw_language == lang_c)
		MATCH(KW_SEMI_COLON);
	else
		PAR_end();

	return action;
}


//____________________________________________________________
//  
//		Parse a blob segment or blob field reference. 
//  

static ACT par_blob_field()
{
	ACT_T type;
	ACT action;
	BLB blob;
	SSHORT first;

	first = token.tok_first;

	blob = par_blob();

	if (MATCH(KW_DOT)) {
		if (MATCH(KW_SEGMENT))
			type = ACT_segment;
		else if (MATCH(KW_LENGTH))
			type = ACT_segment_length;
		else
			SYNTAX_ERROR("SEGMENT or LENGTH");
	}
	else
		type = ACT_blob_handle;

	action = MAKE_ACTION(blob->blb_request, type);

	if (first)
		action->act_flags |= ACT_first;

	action->act_object = (REF) blob;

	return action;
}


//____________________________________________________________
//  
//		If this is a PASCAL program, and we're
//  	in a code block, then a case statement
//		will end with an END, so it adds to the
//		begin count.
//  

static ACT par_case()
{

	if ((sw_language == lang_pascal) && (!routine_decl))
		cur_routine->act_count++;

	return NULL;
}


//____________________________________________________________
//  
//		Parse degenerate CLEAR_HANDLES command.
//  

static ACT par_clear_handles()
{

	return MAKE_ACTION(0, ACT_clear_handles);
}


//____________________________________________________________
//  
//		Parse a DERIVED_FROM clause.  Like
//		BASED ON but for C/C++ prototypes.
//  

static ACT par_derived_from()
{
	GPRE_FLD field;
	BAS based_on;
	GPRE_REL relation;
	ACT action;
	TEXT s[64];

	if ((sw_language != lang_c) && (!isLangCpp(sw_language))) {
		return (NULL);
    }

	action = MAKE_ACTION(0, ACT_basedon);
	based_on = (BAS) ALLOC(BAS_LEN);
	action->act_object = (REF) based_on;

	relation = EXP_relation();
	if (!MATCH(KW_DOT))
		SYNTAX_ERROR("dot in qualified field reference");
	SQL_resolve_identifier("<Field Name>", s);
	if (!(field = MET_field(relation, token.tok_string))) {
		sprintf(s, "undefined field %s", token.tok_string);
		PAR_error(s);
	}
	ADVANCE_TOKEN;
	based_on->bas_field = field;


	based_on->bas_variables = (LLS) ALLOC(LLS_LEN);;
	based_on->bas_variables->lls_next = NULL;
	based_on->bas_variables->lls_object =
		(GPRE_NOD) PAR_native_value(FALSE, FALSE);

	strcpy(based_on->bas_terminator, token.tok_string);
	ADVANCE_TOKEN;

	return action;
}


//____________________________________________________________
//  
//  
//     If the language is PASCAL, and if we're
//     the body of a routine,  every END counts
//     against the number of BEGIN's and CASE's
//     and when the count comes to zero, we SHOULD
//     be at the end of the current routine, so
//     pop it off the routine stack.
//  

static ACT par_end_block()
{

	if (sw_language == lang_pascal &&
		!routine_decl && --cur_routine->act_count == 0 && routine_stack)
		cur_routine = (ACT) POP(&routine_stack);

	return NULL;
}


//____________________________________________________________
//  
//		Parse an END_ERROR statement.  Piece of cake.
//  

static ACT par_end_error()
{

//  avoid parsing an ada exception end_error -
//  check for a semicolon 

	if (!PAR_end() && sw_language == lang_ada)
		return NULL;

	if (!cur_error)
		PAR_error("END_ERROR used out of context");

	if (!((ACT) POP(&cur_error)))
		return NULL;

//   Need to eat the semicolon for c if present  

	if (sw_language == lang_c)
		MATCH(KW_SEMI_COLON);

	return MAKE_ACTION(0, ACT_enderror);
}


//____________________________________________________________
//  
//		Parse END_FETCH statement (clause?).
//  

static ACT par_end_fetch()
{
	ACT begin_action, action;

	if (!cur_fetch)
		PAR_error("END_FETCH used out of context");

	begin_action = (ACT) POP(&cur_fetch);

	action = MAKE_ACTION(begin_action->act_request, ACT_hctef);
	begin_action->act_pair = action;
	action->act_pair = begin_action;

	PAR_end();
	return action;
}


//____________________________________________________________
//  
//		Parse a FOR loop terminator.
//  

static ACT par_end_for()
{
	ACT begin_action, action;
	BLB blob;
	GPRE_REQ request;

	if (!cur_for)
		PAR_error("unmatched END_FOR");

	if (!(begin_action = (ACT) POP(&cur_for)))
		return NULL;

	PAR_end();
	request = begin_action->act_request;

//  If the action is a blob for, make up a blob end. 

	if (begin_action->act_type == ACT_blob_for) {
		blob = (BLB) begin_action->act_object;
		action = MAKE_ACTION(request, ACT_endblob);
		action->act_object = (REF) blob;
		begin_action->act_pair = action;
		action->act_pair = begin_action;
		HSH_remove(blob->blb_symbol);
		blob->blb_flags |= BLB_symbol_released;
		return action;
	}

//  If there isn't a database assigned, the FOR statement itself
//  failed.  Since an error has been given, just return quietly. 

	if (!request->req_database)
		return NULL;

	action = MAKE_ACTION(request, ACT_endfor);
	begin_action->act_pair = action;
	action->act_pair = begin_action;
	EXP_rse_cleanup(request->req_rse);

	for (blob = request->req_blobs; blob; blob = blob->blb_next)
		if (!(blob->blb_flags & BLB_symbol_released))
			HSH_remove(blob->blb_symbol);

	return action;
}

#ifdef PYXIS
//____________________________________________________________
//  
//		Parse an END_FORM statement.
//  

static ACT par_end_form()
{
	PAR_error("FORMs not supported");
	return NULL;				/* silence compiler */
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Handle a menu entree.
//  

static ACT par_end_menu()
{
	GPRE_REQ request;
	GPRE_CTX context;

	if (!cur_menu)
		PAR_error("END_MENU not in MENU context");

	request = (GPRE_REQ) POP(&cur_menu);

	if (request->req_flags & REQ_menu_for) {
		context = request->req_contexts;
		HSH_remove(context->ctx_symbol);
	}

	return MAKE_ACTION(request, ACT_menu_end);
}
#endif

//____________________________________________________________
//  
//		Parse and process END_MODIFY.  The processing mostly includes
//		copying field references to proper context at proper level.
//  

static ACT par_end_modify()
{
	ACT begin_action, action;
	GPRE_REQ request;
	REF change, reference, flag;
	UPD modify;
	GPRE_NOD assignments, item, *ptr;
	LLS stack;
	int count;

	if (!cur_modify)
		PAR_error("unmatched END_MODIFY");

	PAR_end();
	modify = (UPD) POP(&cur_modify);

	if (errors)
		return NULL;

	request = modify->upd_request;

	for (begin_action = request->req_actions;
		 (UPD) begin_action->act_object != modify;
		 begin_action = begin_action->act_next);

//  Build assignments for all fields and null flags referenced 

	stack = NULL;
	count = 0;

	for (reference = request->req_references; reference;
		 reference = reference->ref_next)
			if (reference->ref_context == modify->upd_source &&
				reference->ref_level >= modify->upd_level &&
				!reference->ref_master) {
			change = MAKE_REFERENCE(&modify->upd_references);
			change->ref_context = modify->upd_update;
			change->ref_field = reference->ref_field;
			change->ref_source = reference;
			change->ref_flags = reference->ref_flags;

			item = MAKE_NODE(nod_assignment, 2);
			item->nod_arg[0] = MSC_unary(nod_value, (GPRE_NOD) change);
			item->nod_arg[1] = MSC_unary(nod_field, (GPRE_NOD) change);
			PUSH((GPRE_NOD) item, &stack);
			count++;

			if (reference->ref_null) {
				flag = MAKE_REFERENCE(&modify->upd_references);
				flag->ref_context = change->ref_context;
				flag->ref_field = flag_field;
				flag->ref_master = change;
				flag->ref_source = reference->ref_null;
				change->ref_null = flag;

				item = MAKE_NODE(nod_assignment, 2);
				item->nod_arg[0] = MSC_unary(nod_value, (GPRE_NOD) flag);
				item->nod_arg[1] = MSC_unary(nod_field, (GPRE_NOD) flag);
				PUSH((GPRE_NOD) item, &stack);
				count++;
			}
		}

//  Build a list node of the assignments 

	modify->upd_assignments = assignments =
		MAKE_NODE(nod_list, (SSHORT) count);
	ptr = assignments->nod_arg + count;

	while (stack)
		*--ptr = (GPRE_NOD) POP(&stack);

	action = MAKE_ACTION(request, ACT_endmodify);
	action->act_object = (REF) modify;
	begin_action->act_pair = action;
	action->act_pair = begin_action;

	return action;
}


//____________________________________________________________
//  
//		Parse a stream END statement.
//  

static ACT par_end_stream()
{
	SYM symbol;
	GPRE_REQ request;

	if (!(symbol = token.tok_symbol) || symbol->sym_type != SYM_stream)
		SYNTAX_ERROR("stream cursor");

	request = (GPRE_REQ) symbol->sym_object;
	HSH_remove(symbol);

	EXP_rse_cleanup(request->req_rse);
	ADVANCE_TOKEN;
	PAR_end();

	return MAKE_ACTION(request, ACT_s_end);
}


//____________________________________________________________
//  
//		Process an END_STORE.
//  

static ACT par_end_store(bool special)
{
	ACT begin_action, action2, action;
	GPRE_REQ request;
	GPRE_CTX context;
	UPD return_values;
	REF reference, change;
	GPRE_NOD assignments, item;
	int count;
	GPRE_NOD *ptr;
	LLS stack;

	if (!cur_store)
		PAR_error("unmatched END_STORE");

	PAR_end();

	begin_action = (ACT) POP(&cur_store);
	request = begin_action->act_request;

	if (request->req_type == REQ_store) {
		if (errors)
			return NULL;

		/* Make up an assignment list for all field references */

		count = 0;
		for (reference = request->req_references; reference;
			 reference = reference->ref_next) if (!reference->ref_master)
				count++;

		request->req_node = assignments = MAKE_NODE(nod_list, (SSHORT) count);
		count = 0;

		for (reference = request->req_references; reference;
			 reference = reference->ref_next) {
			if (reference->ref_master)
				continue;
			item = MAKE_NODE(nod_assignment, 2);
			item->nod_arg[0] = MSC_unary(nod_value, (GPRE_NOD) reference);
			item->nod_arg[1] = MSC_unary(nod_field, (GPRE_NOD) reference);
			assignments->nod_arg[count++] = item;
		}
	}
	else {
		/* if the request type is store2, we have store ...returning_values.
		 * The next action on the cur_store stack points to a UPD structure
		 * which will give us the assignments for this one.
		 */

		action2 = (ACT) POP(&cur_store);
		return_values = (UPD) action2->act_object;

		/* Build assignments for all fields and null flags referenced */

		stack = NULL;
		count = 0;

		for (reference = request->req_references; reference;
			 reference = reference->ref_next)
				if (reference->ref_context == return_values->upd_update &&
					reference->ref_level >= return_values->upd_level &&
					!reference->ref_master) {
				change = MAKE_REFERENCE(&return_values->upd_references);
				change->ref_context = return_values->upd_update;
				change->ref_field = reference->ref_field;
				change->ref_source = reference;
				change->ref_flags = reference->ref_flags;

				item = MAKE_NODE(nod_assignment, 2);
				item->nod_arg[0] = MSC_unary(nod_field, (GPRE_NOD) change);
				item->nod_arg[1] = MSC_unary(nod_value, (GPRE_NOD) change);
				PUSH((GPRE_NOD) item, &stack);
				count++;
			}

		/* Build a list node of the assignments */

		return_values->upd_assignments = assignments =
			MAKE_NODE(nod_list, (SSHORT) count);
		ptr = assignments->nod_arg + count;

		while (stack)
			*--ptr = (GPRE_NOD) POP(&stack);
	}
	if ((context = request->req_contexts))
		HSH_remove(context->ctx_symbol);

	if (special)
		action = MAKE_ACTION(request, ACT_endstore_special);
	else
		action = MAKE_ACTION(request, ACT_endstore);
	begin_action->act_pair = action;
	action->act_pair = begin_action;
	return action;
}

#ifdef PYXIS
//____________________________________________________________
//  
//		Handle a menu entree.
//  

static ACT par_entree()
{
	GPRE_REQ request;
	ACT action;
	USHORT first;

	if (!cur_menu)
		return NULL;

	request = (GPRE_REQ) cur_menu->lls_object;

//  Check that this is a case menu, not a dynamic menu.  

	if (request->req_flags & REQ_menu_for)
		return NULL;

	first = TRUE;

	for (action = request->req_actions; action; action = action->act_next)
		if (action->act_type == ACT_menu_entree) {
			first = FALSE;
			break;
		}

	action = MAKE_ACTION(request, ACT_menu_entree);
	action->act_object = (REF) par_quoted_string();

	if (first)
		action->act_flags |= ACT_first_entree;

	if (!MATCH(KW_COLON))
		SYNTAX_ERROR("colon");

	return action;
}
#endif



//____________________________________________________________
//  
//		Parse a ERASE statement.
//  

static ACT par_erase()
{
	GPRE_CTX source;
	ACT action;
	UPD erase;
	SYM symbol;
	GPRE_REQ request;

	if (!(symbol = token.tok_symbol) || symbol->sym_type != SYM_context)
		SYNTAX_ERROR("context variable");

	source = symbol->sym_object;
	request = source->ctx_request;
	if (request->req_type != REQ_for && request->req_type != REQ_cursor)
		PAR_error("invalid context for modify");

	ADVANCE_TOKEN;
	PAR_end();

//  Make an update block to hold everything known about the modify 

	erase = (UPD) ALLOC(UPD_LEN);
	erase->upd_request = request;
	erase->upd_source = source;

	action = MAKE_ACTION(request, ACT_erase);
	action->act_object = (REF) erase;

	return action;
}


//____________________________________________________________
//  
//		Parse a stream FETCH statement.
//  

static ACT par_fetch()
{
	SYM symbol;
	GPRE_REQ request;
	ACT action;

	if (!(symbol = token.tok_symbol) || symbol->sym_type != SYM_stream)
		return NULL;

	request = (GPRE_REQ) symbol->sym_object;
	ADVANCE_TOKEN;
	PAR_end();

	action = MAKE_ACTION(request, ACT_s_fetch);
	PUSH((GPRE_NOD) action, &cur_fetch);

	return action;
}


//____________________________________________________________
//  
//		Parse a FINISH statement.
//  

static ACT par_finish()
{
	ACT action;
	SYM symbol;
	RDY ready;

	action = MAKE_ACTION(0, ACT_finish);

	if (!terminator())
		while (TRUE) {
			if ((symbol = token.tok_symbol)
				&& (symbol->sym_type == SYM_database)) {
				ready = (RDY) ALLOC(RDY_LEN);
				ready->rdy_next = (RDY) action->act_object;
				action->act_object = (REF) ready;
				ready->rdy_database = (DBB) symbol->sym_object;
				CPR_eol_token();
			}
			else
				SYNTAX_ERROR("database handle");
			if (terminator())
				break;
			if (!MATCH(KW_COMMA))
				break;
		}

	if (sw_language == lang_ada)
		MATCH(KW_SEMI_COLON);
	return action;
}


//____________________________________________________________
//  
//		Parse a FOR clause, returning an action.
//		We don't know where we are a host language FOR, a record looping
//		FOR, or a blob FOR.  Parse a little ahead and try to find out.
//		Avoid stepping on user routines that use GDML keywords
//  

static ACT par_for()
{
	SYM symbol, temp;
	ACT action;
	GPRE_REQ request;
	RSE rec_expr;
	GPRE_CTX context, *ptr, *end;
	GPRE_REL relation;
	TEXT s[128], dup_symbol;
#ifdef PYXIS
	if (MATCH(KW_FORM))
		return par_form_for();
#endif
	symbol = NULL;
	dup_symbol = FALSE;

	if (!KEYWORD(KW_FIRST) && !KEYWORD(KW_LEFT_PAREN)) {
		if (token.tok_symbol)
			dup_symbol = TRUE;

		symbol =
			MSC_symbol(SYM_cursor, token.tok_string, token.tok_length, 0);
		ADVANCE_TOKEN;

		if (!MATCH(KW_IN)) {
			MSC_free((UCHAR *) symbol);
			return NULL;
		}
		if (dup_symbol) {
			sprintf(s, "symbol %s is already in use", token.tok_string);
			PAR_error(s);
		}

		if ((temp = token.tok_symbol) && temp->sym_type == SYM_context)
			return par_open_blob(ACT_blob_for, symbol);
	}

//  Make up request block.  Since this might not be a database statement,
//  stay ready to back out if necessay. 

	request = MAKE_REQUEST(REQ_for);

	if (!par_options(request, TRUE) || !(rec_expr = EXP_rse(request, symbol))) {
		MSC_free_request(request);
		return NULL;
	}

	action = MAKE_ACTION(request, ACT_for);
	PUSH((GPRE_NOD) action, &cur_for);

	request->req_rse = rec_expr;
	context = rec_expr->rse_context[0];
	relation = context->ctx_relation;
	request->req_database = relation->rel_database;

	for (ptr = rec_expr->rse_context, end = ptr + rec_expr->rse_count;
		 ptr < end; ptr++) {
		context = *ptr;
		context->ctx_next = request->req_contexts;
		request->req_contexts = context;
	}

	return action;
}

#ifdef PYXIS
//____________________________________________________________
//  
//		Parse a form or menu name, if one is present.  Return form
//		or menu context.
//  

static GPRE_CTX par_form_menu( enum sym_t type)
{
	PAR_error("FORMs not supported");
	return NULL;				/* silence compiler */
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Parse a form display/interaction statement.
//  

static ACT par_form_display()
{
	PAR_error("FORMs not supported");
	return NULL;				/* silence compiler */
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Handle a reference to a form field.
//  

static ACT par_form_field()
{
	PAR_error("FORMs not supported");
	return NULL;				/* silence compiler */
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Parse a parenthesed list of form field names.
//  

static void par_form_fields( GPRE_REQ request, LLS * stack)
{
	PAR_error("FORMs not supported");
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//  

static ACT par_form_for()
{
	PAR_error("FORMs not supported");
	return NULL;				/* silence compiler */
}
#endif

//____________________________________________________________
//  
//  	A function declaration is interesting in
//		FORTRAN because it starts a new sub-module
//		and we have to begin everything all over.
//		In PASCAL it's interesting because it may
//		indicate a good place to put message declarations.
//		Unfortunately that requires a loose parse of the
//		routine header, but what the hell...
//  

static ACT par_function()
{

	if (sw_language == lang_fortran)
		return par_subroutine();

	if (sw_language == lang_pascal)
		return par_procedure();

	return NULL;
}

#ifdef PYXIS
//____________________________________________________________
//  
//		Parse/handle END_ITEM.
//  

static ACT par_item_end()
{
	ACT action, prior;
	GPRE_REQ request;
	GPRE_CTX context;

	if (!cur_item) {
		CPR_error("unmatched END_ITEM");
		return NULL;
	}

	prior = (ACT) POP((LLS *) & cur_item);
	request = prior->act_request;
	context = request->req_contexts;
	HSH_remove(context->ctx_symbol);
	action = MAKE_ACTION(request, ACT_item_end);
	action->act_pair = prior;

	return action;
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Handle FOR_ITEM and/or PUT_ITEM.
//  

static ACT par_item_for( ACT_T type)
{
	ACT action;
	FORM form;
	SYM symbol;
	GPRE_REQ request, parent;
	GPRE_CTX context;
	GPRE_FLD field;
	REF reference;
	TEXT *form_handle;

	sw_pyxis = TRUE;
	form_handle = NULL;
	symbol = PAR_symbol(SYM_dummy);

	if (!MATCH(KW_IN))
		SYNTAX_ERROR("IN");

//  Pick up parent form or menu context 

	if (!(context = par_form_menu(SYM_form_map))) {
		if (!(context = par_form_menu(SYM_menu)))
			SYNTAX_ERROR("form context or menu context");
		else
			return par_menu_item_for(symbol, context, type);
	}
	symbol->sym_type = SYM_form_map;

	parent = context->ctx_request;

	if (!(MATCH(KW_DOT)))
		SYNTAX_ERROR("period");

//  Pick up form 

	form = parent->req_form;

	if (!
		(field =
		 FORM_lookup_field(form, form->form_object,
						   token.tok_string))) SYNTAX_ERROR("sub-form name");

	ADVANCE_TOKEN;

	if (!(form = FORM_lookup_subform(parent->req_form, field)))
		SYNTAX_ERROR("sub-form name");

	form->form_parent = parent;

//  Set up various data structures 

	request = MAKE_REQUEST(REQ_form);
	request->req_form = form;
	request->req_form_handle = form_handle;
	request->req_database = parent->req_database;
	request->req_trans = parent->req_trans;
	context = MSC_context(request);
	context->ctx_symbol = symbol;
	symbol->sym_object = context;
	HSH_insert(symbol);
	action = MAKE_ACTION(request, type);
	PUSH((GPRE_NOD) action, (LLS *) & cur_item);

//  If this is a FOR_ITEM, generate an index variable 

	if (type == ACT_item_for) {
		field =
			MET_make_field("ITEM_INDEX", dtype_short, sizeof(SSHORT), FALSE);
		request->req_index = reference =
			EXP_post_field(field, context, FALSE);
		reference->ref_flags |= REF_pseudo;
	}

	return action;
}
#endif

//____________________________________________________________
//  
//		Check a left brace (or whatever) for start of a new
//		routine.
//  

static ACT par_left_brace()
{
	ACT action;

	if (brace_count++ > 0)
		return NULL;

	cur_routine = action = MAKE_ACTION(0, ACT_routine);
	action->act_flags |= ACT_mark;

	return action;
}

#ifdef PYXIS
//____________________________________________________________
//  
//		Handle a reference to a menu attribute.
//  

static ACT par_menu_att()
{
	ACT_T type;
	ACT action;
	GPRE_CTX context;
	SSHORT first;
	MENU menu;

	first = token.tok_first;
	context = par_form_menu(SYM_menu);

	if (MATCH(KW_DOT)) {
		if (MATCH(KW_TITLE_TEXT))
			type = ACT_title_text;
		else if (MATCH(KW_TITLE_LENGTH))
			type = ACT_title_length;
		else if (MATCH(KW_TERMINATOR))
			type = ACT_terminator;
		else if (MATCH(KW_ENTREE_VALUE))
			type = ACT_entree_value;
		else if (MATCH(KW_ENTREE_TEXT))
			type = ACT_entree_text;
		else if (MATCH(KW_ENTREE_LENGTH))
			type = ACT_entree_length;
		else
			SYNTAX_ERROR("menu attribute");
	}
	else
		SYNTAX_ERROR("period");

	menu = NULL;

	for (action = context->ctx_request->req_actions; action;
		 action = action->act_next) if (action->act_type == ACT_menu_for) {
			menu = (MENU) action->act_object;
			break;
		}

	if (!menu)
		return NULL;

	action = MAKE_ACTION(context->ctx_request, type);
	action->act_object = (REF) menu;

	if (first)
		action->act_flags |= ACT_first;

	return action;
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//  

static ACT par_menu_case()
{
	GPRE_REQ request;
	ACT action;

	sw_pyxis = TRUE;
	request = MAKE_REQUEST(REQ_menu);
	PUSH((GPRE_NOD) request, &cur_menu);
	action = MAKE_ACTION(request, ACT_menu);

	if (MATCH(KW_LEFT_PAREN)) {
		for (;;)
			if (MATCH(KW_VERTICAL))
				request->req_flags |= REQ_menu_pop_up;
			else if (MATCH(KW_HORIZONTAL))
				request->req_flags |= REQ_menu_tag;
			else if (MATCH(KW_TRANSPARENT))
				request->req_flags |= REQ_transparent;
			else if (MATCH(KW_MENU_HANDLE)) {
				request->req_handle = PAR_native_value(FALSE, TRUE);
				request->req_flags |= REQ_exp_hand;
			}
			else
				break;
		EXP_match_paren();
	}

	action->act_object = (REF) par_quoted_string();

//   We should eat semicolons at the end of case_menu statements.  
//   mao 3/29/89
//  
	MATCH(KW_SEMI_COLON);

	return action;
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Parse a menu display/interaction statement.
//  

static ACT par_menu_display( GPRE_CTX context)
{
	ACT action;
	GPRE_REQ display_request;

	action = MAKE_ACTION(context->ctx_request, ACT_menu_display);
	display_request = MAKE_REQUEST(REQ_menu);
	display_request->req_flags |= REQ_exp_hand;
	action->act_object = (REF) display_request;

	for (;;)
		if (MATCH(KW_VERTICAL)) {
			display_request->req_flags |= REQ_menu_pop_up;
			display_request->req_flags &= ~REQ_menu_tag;
		}
		else if (MATCH(KW_HORIZONTAL)) {
			display_request->req_flags |= REQ_menu_tag;
			display_request->req_flags &= ~REQ_menu_pop_up;
		}
		else if (MATCH(KW_TRANSPARENT))
			display_request->req_flags |= REQ_transparent;
		else if (MATCH(KW_OPAQUE))
			display_request->req_flags &= ~REQ_transparent;
		else
			break;

	PAR_end();

	return action;
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Handle a reference to a menu entree attribute.
//  

static ACT par_menu_entree_att()
{
	ACT_T type;
	ACT action;
	GPRE_CTX context;
	SSHORT first;
	ENTREE entree;

	first = token.tok_first;
	context = par_form_menu(SYM_menu_map);

	if (MATCH(KW_DOT)) {
		if (MATCH(KW_ENTREE_TEXT))
			type = ACT_entree_text;
		else if (MATCH(KW_ENTREE_LENGTH))
			type = ACT_entree_length;
		else if (MATCH(KW_ENTREE_VALUE))
			type = ACT_entree_value;
		else
			SYNTAX_ERROR("entree attribute");
	}
	else
		SYNTAX_ERROR("period");

	entree = NULL;
	for (action = context->ctx_request->req_actions; action;
		 action = action->act_next) if (action->act_type == ACT_item_for
										|| action->act_type == ACT_item_put) {
			entree = (ENTREE) action->act_object;
			break;
		}

	if (!entree)
		return NULL;

	action = MAKE_ACTION(context->ctx_request, type);

	if (first)
		action->act_flags |= ACT_first;

	action->act_object = (REF) entree;

	return action;
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//  

static ACT par_menu_for()
{
	SYM symbol;
	ACT action;
	GPRE_REQ request;
	GPRE_CTX context;
	MENU menu;

	sw_pyxis = TRUE;
	request = MAKE_REQUEST(REQ_menu);
	request->req_flags |= REQ_menu_for;

	if (MATCH(KW_LEFT_PAREN)) {
		for (;;)
			if (MATCH(KW_MENU_HANDLE)) {
				request->req_handle = PAR_native_value(FALSE, TRUE);
				request->req_flags |= REQ_exp_hand;
			}
			else
				break;
		EXP_match_paren();
	}

	symbol = PAR_symbol(SYM_dummy);
	symbol->sym_type = SYM_menu;

//  Set up various data structures 

	context = MSC_context(request);
	context->ctx_symbol = symbol;
	symbol->sym_object = context;
	HSH_insert(symbol);
	PUSH((GPRE_NOD) request, &cur_menu);

	action = MAKE_ACTION(request, ACT_menu_for);
	menu = (MENU) ALLOC(sizeof(menu));
	action->act_object = (REF) menu;
	menu->menu_request = request;

	return action;
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Handle FOR_ITEM and/or PUT_ITEM for menus.
//  

static ACT par_menu_item_for( SYM symbol, GPRE_CTX context, ACT_T type)
{
	ACT action;
	GPRE_REQ request, parent;
	ENTREE entree;

	sw_pyxis = TRUE;
	symbol->sym_type = SYM_menu_map;
	parent = context->ctx_request;

//  Set up various data structures 

	request = MAKE_REQUEST(REQ_menu);
	request->req_flags |= REQ_menu_for_item;

	context = MSC_context(request);
	context->ctx_symbol = symbol;
	symbol->sym_object = context;
	HSH_insert(symbol);
	action = MAKE_ACTION(request, type);
	PUSH((GPRE_NOD) action, (LLS *) & cur_item);

	entree = (ENTREE) ALLOC(sizeof(entree));
	action->act_object = (REF) entree;
	entree->entree_request = parent;

	return action;
}
#endif

//____________________________________________________________
//  
//		Parse a MODIFY statement.
//  

static ACT par_modify()
{
	GPRE_CTX source, update;
	ACT action;
	UPD modify;
	SYM symbol;
	GPRE_REQ request;
	SCHAR s[50];

//  Set up modify and action blocks.  This is done here to leave the
//  structure in place to cleanly handle END_MODIFY under error conditions. 

	modify = (UPD) ALLOC(UPD_LEN);
	PUSH((GPRE_NOD) modify, &cur_modify);

//  If the next token isn't a context variable, we can't continue 

	if (!(symbol = token.tok_symbol) || symbol->sym_type != SYM_context) {
		sprintf(s, "%s is not a valid context variable", token.tok_string);
		PAR_error(s);
	}

	source = symbol->sym_object;
	request = source->ctx_request;
	if (request->req_type != REQ_for && request->req_type != REQ_cursor)
		PAR_error("invalid context for modify");

	action = MAKE_ACTION(request, ACT_modify);
	action->act_object = (REF) modify;

	ADVANCE_TOKEN;
	MATCH(KW_USING);

//  Make an update context by cloning the source context 

	update = MAKE_CONTEXT(request);
	update->ctx_symbol = source->ctx_symbol;
	update->ctx_relation = source->ctx_relation;

//  Make an update block to hold everything known about the modify 

	modify->upd_request = request;
	modify->upd_source = source;
	modify->upd_update = update;
	modify->upd_level = ++request->req_level;

	return action;
}


//____________________________________________________________
//  
//		This rather degenerate routine exists to allow both:
//  
//			ON_ERROR
//			ON ERROR
//  
//		so the more dim of our users avoid mistakes.
//  

static ACT par_on()
{

	if (!(MATCH(KW_ERROR)))
		return NULL;

	return par_on_error();
}


//____________________________________________________________
//  

//		Parse a trailing ON_ERROR clause.
//  

static ACT par_on_error()
{
	ACT action;

	if (!cur_statement)
		PAR_error("ON_ERROR used out of context");

	PAR_end();
	cur_statement->act_error = action = MAKE_ACTION(0, ACT_on_error);
	action->act_object = (REF) cur_statement;

	PUSH((GPRE_NOD) action, &cur_error);

	if (cur_statement->act_pair)
		cur_statement->act_pair->act_error = action;

	return action;
}


//____________________________________________________________
//  
//		Parse an "open blob" type statement.  These include OPEN_BLOB,
//		CREATE_BLOB, and blob FOR.
//  

static ACT par_open_blob( ACT_T act_op, SYM symbol)
{
	GPRE_CTX context;
	GPRE_FLD field;
	REF reference;
	ACT action;
	BLB blob;
	GPRE_REQ request;
	TEXT s[128];
	USHORT filter_is_defined = FALSE;

//  If somebody hasn't already parsed up a symbol for us, parse the
//  symbol and the mandatory IN now. 

	if (!symbol) {
		symbol = PAR_symbol(SYM_dummy);
		if (!MATCH(KW_IN))
			SYNTAX_ERROR("IN");
	}

//  The next thing we should find is a field reference.  Get it. 

	if (!(field = EXP_field(&context)))
		return NULL;

	if (!(field->fld_flags & FLD_blob)) {
		sprintf(s, "Field %s is not a blob", field->fld_symbol->sym_string);
		PAR_error(s);
	}

	if (field->fld_array_info) {
		sprintf(s, "Field %s is an array and can not be opened as a blob",
				field->fld_symbol->sym_string);
		PAR_error(s);
	}

	request = context->ctx_request;
	reference = EXP_post_field(field, context, FALSE);

	blob = (BLB) ALLOC(BLB_LEN);
	blob->blb_symbol = symbol;
	blob->blb_reference = reference;

//   See if we need a blob filter (do we have a subtype to subtype clause?)  

	for (;;)
		if (MATCH(KW_FILTER)) {
			blob->blb_const_from_type = (MATCH(KW_FROM)) ?
				PAR_blob_subtype(request->req_database) : field->fld_sub_type;
			if (!MATCH(KW_TO))
				SYNTAX_ERROR("TO");
			blob->blb_const_to_type = PAR_blob_subtype(request->req_database);
			filter_is_defined = TRUE;
		}
		else if (MATCH(KW_STREAM))
			blob->blb_type = gds_bpb_type_stream;
		else
			break;

	if (!(blob->blb_seg_length = field->fld_seg_length))
		blob->blb_seg_length = 512;

	blob->blb_request = request;
	blob->blb_next = request->req_blobs;
	request->req_blobs = blob;

	symbol->sym_type = SYM_blob;
	symbol->sym_object = (GPRE_CTX) blob;
	HSH_insert(symbol);
// ** You just inserted the context variable into the hash table.
//The current token however might be the same context variable. 
//If so, get the symbol for it.
//*  
	if (token.tok_keyword == KW_none)
		token.tok_symbol = HSH_lookup(token.tok_string);

	action = MAKE_ACTION(request, act_op);
	action->act_object = (REF) blob;

	if (act_op == ACT_blob_for)
		PUSH((GPRE_NOD) action, &cur_for);

//   Need to eat the semicolon if present  

	if (sw_language == lang_c)
		MATCH(KW_SEMI_COLON);
	else
		PAR_end();

	return action;
}


//____________________________________________________________
//  
//		Parse request options.  Return TRUE if successful, otherwise
//		FALSE.  If a flag is set, don't give an error on FALSE.
//  

static BOOLEAN par_options( GPRE_REQ request, BOOLEAN flag)
{

	if (!MATCH(KW_LEFT_PAREN))
		return TRUE;

	while (TRUE) {
		if (MATCH(KW_RIGHT_PAREN))
			return TRUE;
		if (MATCH(KW_REQUEST_HANDLE)) {
			request->req_handle = PAR_native_value(FALSE, TRUE);
			request->req_flags |= REQ_exp_hand;
		}
		else if (MATCH(KW_TRANSACTION_HANDLE))
			request->req_trans = PAR_native_value(FALSE, TRUE);
		else if (MATCH(KW_LEVEL))
			request->req_request_level = PAR_native_value(FALSE, FALSE);
		else {
			if (!flag)
				SYNTAX_ERROR("request option");
			return FALSE;
		}
		MATCH(KW_COMMA);
	}
}


//____________________________________________________________
//  
//		If this is PLI, then we've got a new procedure.
//  
//		If this is PASCAL, then we've come upon
//		a program, module, function, or procedure header.  
//		Alas and alack, we have to decide if this is
//		a real header or a forward/external declaration.
//  
//		In either case, we make a mark-only action block,
//		because that's real cheap.  If it's a real routine,
//		we make the action the current routine.
//  

static ACT par_procedure()
{
	ACT action;

	if (sw_language == lang_pascal) {
		routine_decl = TRUE;
		action = scan_routine_header();
		if (!(action->act_flags & ACT_decl)) {
			PUSH((GPRE_NOD) cur_routine, &routine_stack);
			cur_routine = action;
		}
	}
	else
		action = NULL;
	return action;
}

#ifdef PYXIS
//____________________________________________________________
//  
//		Parse a quoted string.
//  

static TEXT *par_quoted_string()
{
	TEXT *string;

	if (!SINGLE_QUOTED(token.tok_type))
		SYNTAX_ERROR("quoted string");
	string = (TEXT *) ALLOC(token.tok_length + 1);
	COPY(token.tok_string, token.tok_length, string);

//  Adjust size to include quotes 
//  FSG 26.Nov.2000
//  
	token.tok_length = token.tok_length + 2;
	ADVANCE_TOKEN;
	return string;
}
#endif

//____________________________________________________________
//  
//		Parse a READY statement.
//  

static ACT par_ready()
{
	ACT action;
	GPRE_REQ request;
	SYM symbol;
	RDY ready;
	DBB db;
	BOOLEAN need_handle;
	USHORT default_buffers, buffers;

	action = MAKE_ACTION(0, ACT_ready);
	need_handle = FALSE;
	default_buffers = 0;

	if (KEYWORD(KW_CACHE))
		SYNTAX_ERROR("database name or handle");

	while (!terminator()) {
		/* this default mechanism is left here for backwards 
		   compatibility, but it is no longer documented and
		   is not something we should maintain for all ready 
		   options since it needlessly complicates the ready
		   statement without providing any extra functionality */

		if (MATCH(KW_DEFAULT)) {
			if (!MATCH(KW_CACHE))
				SYNTAX_ERROR("database name or handle");
			default_buffers = atoi(token.tok_string);
			CPR_eol_token();
			MATCH(KW_BUFFERS);
			continue;
		}

		ready = (RDY) ALLOC(RDY_LEN);
		ready->rdy_next = (RDY) action->act_object;
		action->act_object = (REF) ready;

		if (!(symbol = token.tok_symbol) || symbol->sym_type != SYM_database) {
			ready->rdy_filename = PAR_native_value(FALSE, FALSE);
			if (MATCH(KW_AS))
				need_handle = TRUE;
		}

		if (!(symbol = token.tok_symbol) || symbol->sym_type != SYM_database) {
			if (!isc_databases || isc_databases->dbb_next || need_handle) {
				need_handle = FALSE;
				SYNTAX_ERROR("database handle");
			}
			ready->rdy_database = isc_databases;
		}

		need_handle = FALSE;
		if (!ready->rdy_database)
			ready->rdy_database = (DBB) symbol->sym_object;
		if (terminator())
			break;
		CPR_eol_token();

		/* pick up the possible parameters, in any order */

		buffers = 0;
		db = ready->rdy_database;
		for (;;) {
			if (MATCH(KW_CACHE)) {
				buffers = atoi(token.tok_string);
				CPR_eol_token();
				MATCH(KW_BUFFERS);
			}
			else if (MATCH(KW_USER))
				db->dbb_r_user = PAR_native_value(FALSE, FALSE);
			else if (MATCH(KW_PASSWORD))
				db->dbb_r_password = PAR_native_value(FALSE, FALSE);
			else if (MATCH(KW_LC_MESSAGES))
				db->dbb_r_lc_messages = PAR_native_value(FALSE, FALSE);
			else if (MATCH(KW_LC_CTYPE)) {
				db->dbb_r_lc_ctype = PAR_native_value(FALSE, FALSE);
				db->dbb_know_subtype = 2;
			}
			else
				break;
		}

		request = NULL;
		if (buffers)
			request = PAR_set_up_dpb_info(ready, action, buffers);

		/* if there are any options that take host variables as arguments, 
		   make sure that we generate variables for the request so that the 
		   dpb can be extended at runtime */

		if (db->dbb_r_user || db->dbb_r_password ||
			db->dbb_r_lc_messages || db->dbb_r_lc_ctype)
		{
			if (!request)
				request = PAR_set_up_dpb_info(ready, action, default_buffers);
			request->req_flags |= REQ_extend_dpb;
		}

		/* ...and if there are compile time user or password specified,
		   make sure there will be a dpb generated for them */

		if (!request && (db->dbb_c_user || db->dbb_c_password ||
						 db->dbb_c_lc_messages || db->dbb_c_lc_ctype))
		{
			request = PAR_set_up_dpb_info(ready, action, default_buffers);
		}

		MATCH(KW_COMMA);
	}

	PAR_end();

	if (action->act_object) {
		if (default_buffers)
			for (ready = (RDY) action->act_object; ready;
				 ready = ready->rdy_next) if (!ready->rdy_request)
					request =
						PAR_set_up_dpb_info(ready, action, default_buffers);
		return action;
	}

//  No explicit databases -- pick up all known 

	for (db = isc_databases; db; db = db->dbb_next)
		if (db->dbb_runtime || !(db->dbb_flags & DBB_sqlca)) {
			ready = (RDY) ALLOC(RDY_LEN);
			ready->rdy_next = (RDY) action->act_object;
			action->act_object = (REF) ready;
			ready->rdy_database = db;
		}

	if (!action->act_object)
		PAR_error("no database available to READY");
	else
		for (ready = (RDY) action->act_object; ready; ready = ready->rdy_next) {
			request = ready->rdy_request;
			if (default_buffers && !ready->rdy_request)
				request = PAR_set_up_dpb_info(ready, action, default_buffers);

			/* if there are any options that take host variables as arguments, 
			   make sure that we generate variables for the request so that the 
			   dpb can be extended at runtime */

			db = ready->rdy_database;
			if (db->dbb_r_user || db->dbb_r_password ||
				db->dbb_r_lc_messages || db->dbb_r_lc_ctype) 
			{
				if (!request)
					request = PAR_set_up_dpb_info(ready, action, default_buffers);
				request->req_flags |= REQ_extend_dpb;
			}

			/* ...and if there are compile time user or password specified,
			   make sure there will be a dpb generated for them */

			if (!request && (db->dbb_c_user || db->dbb_c_password ||
							 db->dbb_c_lc_messages || db->dbb_c_lc_ctype))
			{
				request = PAR_set_up_dpb_info(ready, action, default_buffers);
			}
		}

	return action;
}


//____________________________________________________________
//  
//		Parse a returning values clause in a STORE
//		returning an action. 
//		Act as if we were at end_store, then set up
//		for a further set of fields for returned values.
//  

static ACT par_returning_values()
{

	REF reference;
	GPRE_NOD assignments;
	int count;

	if (!cur_store)
		PAR_error("STORE must precede RETURNING_VALUES");

	ACT begin_action = (ACT) POP(&cur_store);
	GPRE_REQ request = begin_action->act_request;

//  First take care of the impending store:
//  Make up an assignment list for all field references  and
//  clone the references while we are at it 

	count = 0;
	for (reference = request->req_references; reference;
		 reference = reference->ref_next) if (!reference->ref_master)
			count++;

	request->req_node = assignments = MAKE_NODE(nod_list, (SSHORT) count);
	count = 0;

	for (reference = request->req_references; reference;
		 reference = reference->ref_next) {
		REF save_ref = MAKE_REFERENCE(&begin_action->act_object);
		save_ref->ref_context = reference->ref_context;
		save_ref->ref_field = reference->ref_field;
		save_ref->ref_source = reference;
		save_ref->ref_flags = reference->ref_flags;

		if (reference->ref_master)
			continue;
		GPRE_NOD item = MAKE_NODE(nod_assignment, 2);
		item->nod_arg[0] = MSC_unary(nod_value, (GPRE_NOD) save_ref);
		item->nod_arg[1] = MSC_unary(nod_field, (GPRE_NOD) save_ref);
		assignments->nod_arg[count++] = item;
	}

//  Next make an updated context for post_store actions 

	UPD new_values = (UPD) ALLOC(UPD_LEN);
	GPRE_CTX source = request->req_contexts;
	request->req_type = REQ_store2;

	GPRE_CTX new_ = MAKE_CONTEXT(request);
	new_->ctx_symbol = source->ctx_symbol;
	new_->ctx_relation = source->ctx_relation;
	new_->ctx_symbol->sym_object = new_;

//  make an update block to hold everything known about referenced
//  fields 

	ACT action = MAKE_ACTION(request, ACT_store2);
	action->act_object = (REF) new_values;

	new_values->upd_request = request;
	new_values->upd_source = source;
	new_values->upd_update = new_;
	new_values->upd_level = ++request->req_level;

//  both actions go on the cur_store stack, the store topmost 

	PUSH((GPRE_NOD) action, &cur_store);
	PUSH((GPRE_NOD) begin_action, &cur_store);
	return action;
}


//____________________________________________________________
//  
//		Do something about a right brace.
//  

static ACT par_right_brace()
{

	if (--brace_count < 0)
		brace_count = 0;

	return NULL;
}


//____________________________________________________________
//  
//		Parse a RELEASE_REQUEST statement.
//  

static ACT par_release()
{
	ACT action;
	SYM symbol;

	action = MAKE_ACTION(0, ACT_release);

	MATCH(KW_FOR);

	if ((symbol = token.tok_symbol) && (symbol->sym_type == SYM_database)) {
		action->act_object = (REF) symbol->sym_object;
		ADVANCE_TOKEN;
	}

	PAR_end();

	return action;
}


//____________________________________________________________
//  
//		Handle a GET_SLICE or PUT_SLICE statement.
//  

static ACT par_slice( ACT_T type)
{

	ACT action;
	GPRE_FLD field;
	GPRE_CTX context;
	ARY info;
	SLC slice;
	GPRE_REQ request;
	USHORT n;
	slc::slc_repeat * tail;

	field = EXP_field(&context);

	if (!(info = field->fld_array_info))
		SYNTAX_ERROR("array field");

	request = MAKE_REQUEST(REQ_slice);
	request->req_slice = slice =
		(SLC) ALLOC(SLC_LEN(info->ary_dimension_count));
	slice->slc_dimensions = info->ary_dimension_count;
	slice->slc_field = field;
	slice->slc_field_ref = EXP_post_field(field, context, FALSE);
	slice->slc_parent_request = context->ctx_request;

	if (!MATCH(KW_L_BRCKET))
		SYNTAX_ERROR("left bracket");

	for (tail = slice->slc_rpt, n = 0; ++n <= slice->slc_dimensions; ++tail) {
		tail->slc_lower = tail->slc_upper = EXP_subscript(request);
		if (MATCH(KW_COLON))
			tail->slc_upper = EXP_subscript(request);
		if (!MATCH(KW_COMMA))
			break;
	}

	if (n != slice->slc_dimensions)
		PAR_error("subscript count mismatch");

	if (!MATCH(KW_R_BRCKET))
		SYNTAX_ERROR("right bracket");

	if (type == ACT_get_slice) {
		if (!MATCH(KW_INTO))
			SYNTAX_ERROR("INTO");
	}
	else if (!MATCH(KW_FROM))
		SYNTAX_ERROR("FROM");

	slice->slc_array = EXP_subscript(0);

	action = MAKE_ACTION(request, type);
	action->act_object = (REF) slice;

	if (sw_language == lang_c)
		MATCH(KW_SEMI_COLON);
	else
		PAR_end();

	return action;
}


//____________________________________________________________
//  
//		Parse a STORE clause, returning an action.
//  

static ACT par_store()
{
	ACT action;
	GPRE_REQ request;
	GPRE_CTX context;
	GPRE_REL relation;

	request = MAKE_REQUEST(REQ_store);
	par_options(request, FALSE);
	action = MAKE_ACTION(request, ACT_store);
	PUSH((GPRE_NOD) action, &cur_store);

	context = EXP_context(request, 0);
	relation = context->ctx_relation;
	request->req_database = relation->rel_database;
	HSH_insert(context->ctx_symbol);
// ** You just inserted the context variable into the hash table.
//The current token however might be the same context variable. 
//If so, get the symbol for it.
//*  
	if (token.tok_keyword == KW_none)
		token.tok_symbol = HSH_lookup(token.tok_string);
	MATCH(KW_USING);

	return action;
}


//____________________________________________________________
//  
//		Parse a start stream statement.
//  

static ACT par_start_stream()
{
	ACT action;
	GPRE_REQ request;
	RSE rec_expr;
	GPRE_CTX context, *ptr, *end;
	GPRE_REL relation;
	SYM cursor;

	request = MAKE_REQUEST(REQ_cursor);
	par_options(request, FALSE);
	action = MAKE_ACTION(request, ACT_s_start);

	cursor = PAR_symbol(SYM_dummy);
	cursor->sym_type = SYM_stream;
	cursor->sym_object = (GPRE_CTX) request;

	MATCH(KW_USING);
	rec_expr = EXP_rse(request, 0);
	request->req_rse = rec_expr;
	context = rec_expr->rse_context[0];
	relation = context->ctx_relation;
	request->req_database = relation->rel_database;

	for (ptr = rec_expr->rse_context, end = ptr + rec_expr->rse_count;
		 ptr < end; ptr++) {
		context = *ptr;
		context->ctx_next = request->req_contexts;
		request->req_contexts = context;
	}

	HSH_insert(cursor);
	PAR_end();

	return action;
}


//____________________________________________________________
//  
//		Parse a START_TRANSACTION statement, including
//  	transaction handle, transaction options, and
//		reserving list.
//  

static ACT par_start_transaction()
{
	ACT action;
	GPRE_TRA trans;

	action = MAKE_ACTION(0, ACT_start);

	if (terminator()) {
		PAR_end();
		return action;
	}

	trans = (GPRE_TRA) ALLOC(TRA_LEN);

//  get the transaction handle  

	if (!token.tok_symbol)
		trans->tra_handle = PAR_native_value(FALSE, TRUE);

//  loop reading the various transaction options 

	while (!KEYWORD(KW_RESERVING) && !KEYWORD(KW_USING) && !terminator()) {
		if (MATCH(KW_READ_ONLY)) {
			trans->tra_flags |= TRA_ro;
			continue;
		}
		else if (MATCH(KW_READ_WRITE))
			continue;

		if (MATCH(KW_CONSISTENCY)) {
			trans->tra_flags |= TRA_con;
			continue;
		}
// ***    else if (MATCH (KW_READ_COMMITTED))
// { 
// trans->tra_flags |= TRA_read_committed;
// continue;
// } **
		else if (MATCH(KW_CONCURRENCY))
			continue;

		if (MATCH(KW_NO_WAIT)) {
			trans->tra_flags |= TRA_nw;
			continue;
		}
		else if (MATCH(KW_WAIT))
			continue;

		if (MATCH(KW_AUTOCOMMIT)) {
			trans->tra_flags |= TRA_autocommit;
			continue;
		}

		if (sw_language == lang_cobol || sw_language == lang_fortran)
			break;
		else
			SYNTAX_ERROR("transaction keyword");
	}

//  send out for the list of reserved relations 

	if (MATCH(KW_RESERVING)) {
		trans->tra_flags |= TRA_rrl;
		PAR_reserving(trans->tra_flags, 0);
	}
	else if (MATCH(KW_USING)) {
		trans->tra_flags |= TRA_inc;
		PAR_using_db();
	}

	PAR_end();
	CMP_t_start(trans);
	action->act_object = (REF) trans;

	return action;
}


//____________________________________________________________
//  
//		We have hit either a function or subroutine declaration.
//		If the language is fortran, make the position with a break.
//  

static ACT par_subroutine()
{
	ACT action;

	if (sw_language != lang_fortran)
		return NULL;

	action = MAKE_ACTION(0, ACT_routine);
	action->act_flags |= ACT_mark | ACT_break;
	cur_routine = action;
	return action;
}


//____________________________________________________________
//  
//		Parse a transaction termination statement: commit,
//		prepare, rollback, or save (commit retaining context).
//  

static ACT par_trans( ACT_T act_op)
{
	ACT action;
	USHORT parens;

	action = MAKE_ACTION(0, act_op);

	if (!terminator()) {
		parens = MATCH(KW_LEFT_PAREN);
		if ((sw_language == lang_fortran)
			&& (act_op == ACT_commit_retain_context)) {
			if (!(MATCH(KW_TRANSACTION_HANDLE)))
				return NULL;
		}
		else
			MATCH(KW_TRANSACTION_HANDLE);
		action->act_object = (REF) PAR_native_value(FALSE, TRUE);
		if (parens)
			EXP_match_paren();
	}

	if ((sw_language != lang_fortran) && (sw_language != lang_pascal))
		MATCH(KW_SEMI_COLON);

	return action;
}


//____________________________________________________________
//  
//		Parse something of the form:
//  
//			<relation> . <field> . <something>
//  
//		where <something> is currently an enumerated type.
//  

static ACT par_type()
{
	GPRE_REL relation;
	GPRE_FLD field;
	ACT action;
	SSHORT type;
	TEXT s[64];

//  Pick up relation 

// ***
//SYM	symbol;
//symbol = token.tok_symbol;
//relation = (GPRE_REL) symbol->sym_object;
//ADVANCE_TOKEN;
//** 

	relation = EXP_relation();

//  No dot and we give up 

	if (!MATCH(KW_DOT))
		return NULL;

//  Look for field name.  No field name, punt 

	SQL_resolve_identifier("<Field Name>", s);
	if (!(field = MET_field(relation, token.tok_string)))
		return NULL;

	ADVANCE_TOKEN;

	if (!MATCH(KW_DOT))
		SYNTAX_ERROR("period");

//  Lookup type.  If we can't find it, complain bitterly 

	if (!MET_type(field, token.tok_string, &type)) {
		sprintf(s, "undefined type %s", token.tok_string);
		PAR_error(s);
	}

	ADVANCE_TOKEN;
	action = MAKE_ACTION(0, ACT_type);
	action->act_object = (REF) (ULONG) type;

	return action;
}

//____________________________________________________________
//  
//		Parse a free reference to a database field in general
//		program context.
//  

static ACT par_variable()
{
	GPRE_FLD field, cast;
	ACT action;
	REF reference, flag;
	GPRE_REQ request;
	GPRE_CTX context;
	USHORT first, dot, is_null;

//  
//  Since fortran is fussy about continuations and the like,
//  see if this variable token is the first thing in a statement.
//  

	is_null = FALSE;
	first = token.tok_first;
	field = EXP_field(&context);

	if ((dot = MATCH(KW_DOT)) && (cast = EXP_cast(field))) {
		field = cast;
		dot = MATCH(KW_DOT);
	}

	if (dot && MATCH(KW_NULL)) {
		is_null = TRUE;
		dot = FALSE;
	}

	request = context->ctx_request;
	reference = EXP_post_field(field, context, is_null);

	if (field->fld_array)
		EXP_post_array(reference);

	action = MAKE_ACTION(request, ACT_variable);

	if (first)
		action->act_flags |= ACT_first;
	if (dot)
		action->act_flags |= ACT_back_token;

	action->act_object = reference;

	if (!is_null)
		return action;

//  We've got a explicit null flag referernce rather than a field
//  reference.  If there's already a null reference for the field,
//  use it; otherwise make one up. 

	if (reference->ref_null) {
		action->act_object = reference->ref_null;
		return action;
	}

//  Check to see if the flag field has been allocated.  If not, sigh, allocate it 

	flag = MAKE_REFERENCE(&request->req_references);
	flag->ref_context = reference->ref_context;
	flag->ref_field = PAR_null_field();
	flag->ref_level = request->req_level;

	flag->ref_master = reference;
	reference->ref_null = flag;

	action->act_object = flag;

	return action;
}

#ifdef PYXIS
//____________________________________________________________
//  
//		Create a new window.
//  

static ACT par_window_create()
{
	PAR_error("FORMs not supported");
	return NULL;				/* silence compiler */
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Create a new window.
//  

static ACT par_window_delete()
{
	PAR_error("FORMs not supported");
	return NULL;				/* silence compiler */
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Establish the scope of window declarations,
//		whether local, external, or global.   This
//		is a purely declarative statement.
//  

static ACT par_window_scope()
{
	PAR_error("FORMs not supported");
	return NULL;				/* silence compiler */
}
#endif
#ifdef PYXIS
//____________________________________________________________
//  
//		Create a new window.
//  

static ACT par_window_suspend()
{
	PAR_error("FORMs not supported");
	return NULL;				/* silence compiler */
}
#endif

//____________________________________________________________
//  
//		This is PASCAL, and we've got a function, or procedure header.  
//		Alas and alack, we have to decide if this is a real header or 
//		a forward/external declaration.
//  
//		Basically we scan the thing, skipping parenthesized bits,
//		looking for a semi-colon.  We look at the next token, which may
//		be OPTIONS followed by a parenthesized list of options, or it
//		may be just some options, or it may be nothing.  If the options
//		are EXTERN or FORWARD, we've got a reference, otherwise its a real
//		routine (or possibly program or module). 
//  
//		Fortunately all of these are of the form:
//			<keyword> <name> [( blah, blah )] [: type] ; [<options>;] 
//  
//  

static ACT scan_routine_header()
{
	ACT action;

	action = MAKE_ACTION(0, ACT_routine);
	action->act_flags |= ACT_mark;

	while (!(MATCH(KW_SEMI_COLON)))
		if (!(match_parentheses()))
			ADVANCE_TOKEN;

	if (MATCH(KW_OPTIONS) && MATCH(KW_LEFT_PAREN)) {
		while (!(MATCH(KW_RIGHT_PAREN))) {
			if (MATCH(KW_EXTERN) || MATCH(KW_FORWARD))
				action->act_flags |= ACT_decl;
			else
				ADVANCE_TOKEN;
		}
		MATCH(KW_SEMI_COLON);
	}
	else
		for (;;) {
			if (MATCH(KW_EXTERN) || MATCH(KW_FORWARD)) {
				action->act_flags |= ACT_decl;
				MATCH(KW_SEMI_COLON);
			}
			else if (MATCH(KW_INTERNAL) || MATCH(KW_ABNORMAL) ||
					 MATCH(KW_VARIABLE) || MATCH(KW_VAL_PARAM))
				MATCH(KW_SEMI_COLON);
			else
				break;
		}

	return action;
}


//____________________________________________________________
//  
//		If this is a external declaration in
//		a BASIC program, set a flag to indicate
//		the situation.
//  

static void set_external_flag()
{
	CPR_token();
}


//____________________________________________________________
//  
//		Check the current token for a logical terminator.  Terminators
//		are semi-colon, ELSE, or ON_ERROR.
//  

static BOOLEAN terminator()
{

//  For C, changed KEYWORD (KW_SEMICOLON) to MATCH (KW_SEMICOLON) to eat a
//  semicolon if it is present so as to allow it to be there or not be there.
//  Bug#833.  mao 6/21/89 

//  For C, right brace ("}") must also be a terminator. 

	if (sw_language == lang_c) {
		if (MATCH(KW_SEMI_COLON) ||
			KEYWORD(KW_ELSE) || KEYWORD(KW_ON_ERROR) || KEYWORD(KW_R_BRACE))
			return TRUE;
	}
	else if (sw_language == lang_ada) {
		if (MATCH(KW_SEMI_COLON) || KEYWORD(KW_ELSE) || KEYWORD(KW_ON_ERROR))
			return TRUE;
	}
	else {
		if (KEYWORD(KW_SEMI_COLON) ||
			KEYWORD(KW_ELSE) ||
			KEYWORD(KW_ON_ERROR) ||
			(sw_language == lang_cobol && KEYWORD(KW_DOT))) return TRUE;
	}

	return FALSE;
}

