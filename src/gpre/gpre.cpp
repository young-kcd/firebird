//____________________________________________________________
//  
//		PROGRAM:	Preprocessor
//		MODULE:		gpre.cpp
//		DESCRIPTION:	Main line routine
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
//  $Id: gpre.cpp,v 1.39 2003-10-14 22:21:49 brodsom Exp $
//  Revision 1.2  2000/11/16 15:54:29  fsg
//  Added new switch -verbose to gpre that will dump
//  parsed lines to stderr
//  
//  Fixed gpre bug in handling row names in WHERE clauses
//  that are reserved words now (DATE etc)
//  (this caused gpre to dump core when parsing tan.e)
//  
//  Fixed gpre bug in handling lower case table aliases
//  in WHERE clauses for sql dialect 2 and 3.
//  (cause a core dump in a test case from C.R. Zamana)
//  
//  TMN (Mike Nordell) 11.APR.2001 - Reduce compiler warnings
//  
//  FSG (Frank Schlottmann-Gödde) 8.Mar.2002 - tiny cobol support
//       fixed Bug No. 526204 
//
// 2002.10.30 Sean Leyne - Removed support for obsolete "PC_PLATFORM" define
//
//____________________________________________________________
//
//

#define GPRE_MAIN
#define PARSER_MAIN
#include "firebird.h"
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include "../gpre/gpre.h"
#include "../jrd/license.h"
#include "../gpre/parse.h"
#include "../jrd/intl.h"
#include "../gpre/cmp_proto.h"
#include "../gpre/hsh_proto.h"
#include "../gpre/gpre_proto.h"
#include "../gpre/lang_proto.h"
#include "../gpre/gpre_meta.h"
#include "../gpre/msc_proto.h"
#include "../gpre/par_proto.h"
#include "../jrd/gds_proto.h"
#include "../gpre/gpreswi.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef VMS
#include <descrip.h>
extern "C" {
	int lib$get_foreign();
} // extern "C"
#endif

// needed for unlink in MinGW 3.0 rc2
#ifdef MINGW 
#include <io.h>
#endif

#ifdef SMALL_FILE_NAMES
const char* SCRATCH		= "fb_q";
#else
const char* SCRATCH		= "fb_query_";
#endif

const char* FOPEN_READ_TYPE		= "r";
const char* FOPEN_WRITE_TYPE	= "w";

static bool			all_digits(const char*);
static bool			arg_is_string(SLONG, TEXT**, const TEXT*);
static SSHORT		compare_ASCII7z(const char*, const char*);
static SLONG		compile_module(SLONG, const TEXT*);
static bool			file_rename(TEXT*, const TEXT*, const TEXT*);
#ifdef GPRE_FORTRAN
static void			finish_based(ACT);
#endif
static int			get_char(IB_FILE *);
static bool			get_switches(int, TEXT **, const in_sw_tab_t*, SW_TAB, TEXT **);
static TOK			get_token();
static int			nextchar();
static SLONG		pass1(const TEXT*);
static void			pass2(SLONG);
static void			print_switches();
static void			remember_label(TEXT *);
//static IB_FILE*		reposition_file(IB_FILE *, SLONG);
static void			return_char(SSHORT);
static SSHORT		skip_white();

/* Program wide globals */

IB_FILE *input_file, *trace_file;
TEXT*	file_name;
TEXT*	out_file_name;
SLONG position, last_position, line_position, first_position,
	prior_line_position;
ACT global_last_action, global_first_action;
UCHAR classes[256], fortran_labels[1024];
TEXT *ident_pattern, *utility_name, *count_name, *slack_name,
	*transaction_name, *database_name;

static TEXT input_buffer[512], *input_char;

static DBB sw_databases;
static USHORT sw_first;
//static jmp_buf fatal_env;
tok prior_token;
static TEXT *comment_start, *comment_stop;

typedef void (*pfn_gen_routine) (const act*, int);
static pfn_gen_routine gen_routine;

static TEXT trace_file_name[128];
static SLONG traced_position = 0;

//___________________________________________________________________
// Test if input language is cpp based.
//
bool isLangCpp(LANG_T lang) {
    if (lang == lang_cxx || lang == lang_internal) {
        return true;
    }
    return false;
}

/*
 * Type and table definition for the extension tables.  Tells GPRE
 * the default extensions for DML and host languages.
 */

struct ext_table_t
{
	lang_t			ext_language;
	gpre_cmd_switch	ext_in_sw;
	TEXT*			in;
	TEXT*			out;
};


static const ext_table_t dml_ext_table[] =
{
	{ lang_c, IN_SW_GPRE_C, ".e", ".c" },

#ifndef VMS
#ifndef WIN_NT
	{ lang_scxx, IN_SW_GPRE_SCXX, ".E", ".C" },
#endif
#endif

	{ lang_cxx, IN_SW_GPRE_CXX, ".exx", ".cxx" },
	{ lang_cpp, IN_SW_GPRE_CXX, ".epp", ".cpp" },
	{ lang_internal, IN_SW_GPRE_G, ".e", ".c" },
	{ lang_internal_cxx, IN_SW_GPRE_0, ".epp", ".cpp" },
	{ lang_pascal, IN_SW_GPRE_P, ".epas", ".pas" },

#ifdef GPRE_FORTRAN
#ifdef VMS
	{ lang_fortran, IN_SW_GPRE_F, ".efor", ".for" },
#else
	{ lang_fortran, IN_SW_GPRE_F, ".ef", ".f" },
#endif
#endif // GPRE_FORTRAN

#ifdef GPRE_COBOL
#ifdef VMS
	{ lang_cobol, IN_SW_GPRE_COB, ".ecob", ".cob" },
#else 
	{ lang_cobol, IN_SW_GPRE_COB, ".ecbl", ".cbl" },
#endif
#endif // GPRE_COBOL

#ifdef GPRE_ADA
#ifdef VMS
	{ lang_ada, IN_SW_GPRE_ADA, ".eada", ".ada" },
#elif hpux
	{ lang_ada, IN_SW_GPRE_ADA, ".eada", ".ada" },
#else
	{ lang_ada, IN_SW_GPRE_ADA, ".ea", ".a" },
#endif

#ifdef ALSYS_ADA
	{ lang_ada, IN_SW_GPRE_ALSYS, ".eada", ".ada" },
#endif
#endif // GPRE_ADA

#if (defined( WIN_NT))
	{ lang_cplusplus, IN_SW_GPRE_CPLUSPLUS, ".epp", ".cpp" },
#else
	{ lang_cplusplus, IN_SW_GPRE_CPLUSPLUS, ".exx", ".cxx" },
#endif
	{ lang_undef, IN_SW_GPRE_0, NULL, NULL }
};

enum char_types {
	CHR_LETTER	= 1,
	CHR_DIGIT	= 2,
	CHR_IDENT	= 4,
	CHR_QUOTE	= 8,
	CHR_WHITE	= 16,
	CHR_INTRODUCER	= 32,
	CHR_DBLQUOTE	= 64
};

//  macro compares chars; case sensitive for some platforms 

#ifdef EITHER_CASE
#define SAME(p,q)	UPPER7 (*p) == UPPER7 (*q)
#else
#define SAME(p,q)	*p == *q
#endif


//____________________________________________________________
//
//	Main line routine for C preprocessor.  Initializes
//	system, performs pass 1 and pass 2.  Interprets
//	command line.
//

int main(int argc, char* argv[])
{
	SYM symbol;
	SLONG end_position;
	int i;
	TEXT*	p;
	TEXT	spare_file_name[256];
	TEXT	spare_out_file_name[256];
	bool renamed;
	bool explicitt;
	const ext_table_t* ext_tab;
	sw_tab_t sw_table[IN_SW_GPRE_COUNT];
#ifdef VMS
	IB_FILE *temp;
	TEXT temp_name[256];
	SSHORT c;
#endif

    errors = warnings = fatals = 0;

	bool use_lang_internal_gxx_output;

	use_lang_internal_gxx_output = false;
	strcpy(ada_package, "");
	ada_flags = 0;
	input_char = input_buffer;

#ifdef VMS
	argc = VMS_parse(&argv, argc);
#endif

	//  Initialize character class table 
	for (i = 0; i <= 127; ++i) {
		classes[i] = 0;
	}
	for (i = 128; i <= 255; ++i) {
		classes[i] = CHR_LETTER | CHR_IDENT;
	}
	for (i = 'a'; i <= 'z'; ++i) {
		classes[i] = CHR_LETTER | CHR_IDENT;
	}
	for (i = 'A'; i <= 'Z'; ++i) {
		classes[i] = CHR_LETTER | CHR_IDENT;
	}
	for (i = '0'; i <= '9'; ++i) {
		classes[i] = CHR_DIGIT | CHR_IDENT;
	}

	classes['_']	= CHR_LETTER | CHR_IDENT | CHR_INTRODUCER;
	classes['$']	= CHR_IDENT;
	classes[' ']	= CHR_WHITE;
	classes['\t']	= CHR_WHITE;
	classes['\n']	= CHR_WHITE;
	classes['\r']	= CHR_WHITE;
	classes['\'']	= CHR_QUOTE;
	classes['\"']	= CHR_DBLQUOTE;
	classes['#']	= CHR_IDENT;

//  zorch 0 through 7 in the fortran label vector 

	fortran_labels[0] = 255;

//  set switches and so on to default (C) values 

	DBB db = NULL;

	sw_language			= lang_undef;
	sw_lines			= true;
	sw_auto				= true;
	sw_cstring			= true;
	sw_alsys			= false;
	sw_external			= false;
	sw_standard_out		= false;
	sw_ansi				= false;
	sw_version			= false;
	sw_d_float			= false;
	sw_sql_dialect		= SQL_DIALECT_V5;
	dialect_specified	= false;
	sw_window_scope		= DBB_GLOBAL;
	gen_routine			= C_CXX_action;
	comment_start		= "/*";
	comment_stop		= "*/";
	ident_pattern		= "gds__%d";
	transaction_name	= "gds__trans";
	database_name		= "gds__database";
	utility_name		= "gds__utility";
	count_name			= "gds__count";
	slack_name			= "gds__slack";
	global_db_count		= 0;
	default_user		= NULL;
	default_password	= NULL;
	default_lc_ctype	= NULL;
	default_lc_messages	= NULL;
	text_subtypes		= NULL;
	override_case		= false;

	sw_know_interp = FALSE;
	sw_interp = 0;
//  FSG 14.Nov.2000 
	sw_verbose = false;
	sw_sql_dialect = compiletime_db_dialect = SQL_DIALECT_V5;

//  
//  Call a subroutine to process the input line 
//  

	TEXT* filename_array[4] = { 0 };

	if (!get_switches(argc, argv, gpre_in_sw_table, sw_table, filename_array)) {
		CPR_exit(FINI_ERROR);
	}

	file_name		= filename_array[0];
	out_file_name	= filename_array[1];

	TEXT* db_filename = filename_array[2];
	TEXT* db_base_directory = filename_array[3];

	if (!file_name) {
		ib_fprintf(ib_stderr, "gpre:  no source file named.\n");
		CPR_exit(FINI_ERROR);
	}

//  
//  Try to open the input file.  
//  If the language wasn't supplied, maybe the kind user included a language
//  specific extension, and the file name fixer will find it.  The file name
//  fixer returns FALSE if it can't add an extension, which means there's already
//  one of the right type there.
//  

	if (sw_language == lang_undef)
		for (ext_tab = dml_ext_table; sw_language = ext_tab->ext_language;
			 ext_tab++) {
			strcpy(spare_file_name, file_name);
			if (!(file_rename(spare_file_name, ext_tab->in, NULL)))
				break;
		}

//  
//   Sigh.  No such luck. Maybe there's a file lying around with a plausible
//   extension and we can use that.
//  

	if (sw_language == lang_undef)
		for (ext_tab = dml_ext_table; sw_language = ext_tab->ext_language;
			 ext_tab++) {
			strcpy(spare_file_name, file_name);
			if (file_rename(spare_file_name, ext_tab->in, NULL) &&
				(input_file = ib_fopen(spare_file_name, FOPEN_READ_TYPE))) {
				file_name = spare_file_name;
				break;
			}
		}

//  
//   Well, if he won't tell me what language it is, or even give me a hint, I'm
//   not going to spend all day figuring out what he wants done.
//  

	if (sw_language == lang_undef) {
		ib_fprintf(ib_stderr,
				   "gpre: can't find %s with any known extension.  Giving up.\n",
				   file_name);
		CPR_exit(FINI_ERROR);
	}

//  
//  Having got here, we know the language, and might even have the file open.
//  Better check before reopening it on ourselves.  Try the file with the
//  extension first (even if we have to add the extension).   If we add an
//  extension, and find a file with that extension, we make the file name
//  point to the expanded file name string in a private buffer.
//  

	if (!input_file) {
		strcpy(spare_file_name, file_name);
		for (ext_tab = dml_ext_table;
			 ext_tab->ext_language != sw_language;
			 ext_tab++)
		{
				 ;	// empty loop body
		}
		renamed = file_rename(spare_file_name, ext_tab->in, NULL);
		if (renamed &&
			(input_file = ib_fopen(spare_file_name, FOPEN_READ_TYPE)))
		{
			file_name = spare_file_name;
		}
		else if (!(input_file = ib_fopen(file_name, FOPEN_READ_TYPE))) {
			if (renamed) {
				ib_fprintf(ib_stderr, "gpre: can't open %s or %s\n",
						   file_name, spare_file_name);
			} else {
				ib_fprintf(ib_stderr, "gpre: can't open %s\n", file_name);
			}
			CPR_exit(FINI_ERROR);
		}
	}

//  
//  Now, apply the switches and defaults we've so painfully acquired;
//  adding in the language switch in case we inferred it rather than parsing it.
//  

	const ext_table_t* src_ext_tab = dml_ext_table;

	while (src_ext_tab->ext_language != sw_language) {
		++src_ext_tab;
	}
	sw_table[0].sw_in_sw = src_ext_tab->ext_in_sw;

	for (SW_TAB sw_tab = sw_table; sw_tab->sw_in_sw; sw_tab++)
	{
		switch (sw_tab->sw_in_sw)
		{
		case IN_SW_GPRE_C:
			sw_language		= lang_c;
			ident_pattern	= "isc_%d";
			utility_name	= "isc_utility";
			count_name		= "isc_count";
			slack_name		= "isc_slack";
			break;

		case IN_SW_GPRE_SCXX:
		case IN_SW_GPRE_CXX:
		case IN_SW_GPRE_CPLUSPLUS:
			sw_language		= lang_cxx;
			ident_pattern	= "isc_%d";
			utility_name	= "isc_utility";
			count_name		= "isc_count";
			slack_name		= "isc_slack";
			transaction_name = "gds_trans";
			database_name	= "gds_database";
			break;

		case IN_SW_GPRE_D:
			/* allocate database block and link to db chain */

			db = (DBB) MSC_alloc_permanent(DBB_LEN);
			db->dbb_next = isc_databases;

			/* put this one in line to be next */

			isc_databases = db;

			/* allocate symbol block */

			symbol = (SYM) MSC_alloc_permanent(SYM_LEN);

			/* make it a database, specifically this one */

			symbol->sym_type	= SYM_database;
			symbol->sym_object	= (GPRE_CTX) db;
			symbol->sym_string	= database_name;

			/* database block points to the symbol block */

			db->dbb_name = symbol;

			/* give it the file name and try to open it */

			db->dbb_filename = db_filename;
			if (!MET_database(db, true))
				CPR_exit(FINI_ERROR);
			if (sw_external)
				db->dbb_scope = DBB_EXTERN;
#ifdef FTN_BLK_DATA
			else {
				global_db_count = 1;
				strcpy(global_db_list[0].dbb_name, db->dbb_name->sym_string);
			}
#endif
			break;

		case IN_SW_GPRE_E:
			sw_case = true;
			break;

#ifndef BOOT_BUILD
#ifdef GPRE_ADA
		case IN_SW_GPRE_ADA:
#ifdef VMS
			ada_null_address = "system.address_zero";
#else
			ada_null_address = "0";
#endif
			sw_case = true;
			sw_language = lang_ada;
			sw_lines = false;
			sw_cstring = false;
			gen_routine = ADA_action;
			utility_name = "isc_utility";
			count_name = "isc_count";
			slack_name = "isc_slack";
			transaction_name = "gds_trans";
			database_name = "isc_database";
			ident_pattern = "isc_%d";
			comment_start = "--";
			if (db)
				db->dbb_name->sym_string = "isc_database";
			comment_stop = "--";
			break;

		case IN_SW_GPRE_ALSYS:
			sw_alsys = true;
			sw_case = true;
			sw_language = lang_ada;
			sw_lines = false;
			sw_cstring = false;
			gen_routine = ADA_action;
			utility_name = "isc_utility";
			count_name = "isc_count";
			slack_name = "isc_slack";
			transaction_name = "gds_trans";
			database_name = "isc_database";
			ident_pattern = "isc_%d";
			comment_start = "--";
			if (db)
				db->dbb_name->sym_string = "isc_database";
			comment_stop = "--";
			break;
#endif // GPRE_ADA


#ifdef GPRE_FORTRAN
		case IN_SW_GPRE_F:
			sw_case = true;
			sw_language = lang_fortran;
			sw_lines = false;
			sw_cstring = false;
			gen_routine = FTN_action;
#ifdef sun
			comment_start = "*      ";
#else
			comment_start = "C      ";
#endif
			comment_stop = " ";

			/* Change the patterns for v4.0 */

			ident_pattern = "isc_%d";
			utility_name = "isc_utility";
			count_name = "isc_count";
			slack_name = "isc_slack";
			break;
#endif // GPRE_FORTRAN

#ifdef GPRE_COBOL
		case IN_SW_GPRE_ANSI:
			sw_ansi = true;
			break;

		case IN_SW_GPRE_COB:
			sw_case = true;
			sw_language = lang_cobol;
			comment_stop = " ";
			sw_lines = false;
			sw_cstring = false;
			gen_routine = COB_action;
			break;
#endif // GPRE_COBOL

#ifdef GPRE_PASCAL
		case IN_SW_GPRE_P:
			sw_case			= true;
			sw_language		= lang_pascal;
			sw_lines		= false;
			sw_cstring		= false;
			gen_routine		= PAS_action;
			comment_start	= "(*";
			comment_stop	= "*)";
			break;
#endif // GPRE_PASCAL
#endif // !BOOT_BUILD

		case IN_SW_GPRE_D_FLOAT:
			sw_d_float = true;
			break;

		case IN_SW_GPRE_G:
			sw_language			= lang_internal;
			gen_routine			= INT_action;
			sw_cstring			= false;
			transaction_name	= "dbb->dbb_sys_trans";
			sw_know_interp		= TRUE;
			sw_interp			= ttype_metadata;
			break;

		case IN_SW_GPRE_GXX:
			/* When we get executed here the IN_SW_GPRE_G case
			 * has already been executed.  So we just set the
			 * gen_routine to point to the C++ action, and be
			 * done with it.
			 */
			gen_routine			= INT_CXX_action;
			use_lang_internal_gxx_output = true;
			break;

		case IN_SW_GPRE_LANG_INTERNAL:
			/* We need to reset all the variables (except sw_language) to the
 			* default values because the IN_SW_GPRE_G case was already
 			* executed in the for the very first switch.
 			**/
			sw_language = lang_internal;
			gen_routine = C_CXX_action;
			sw_cstring = true;
			transaction_name = "gds_trans";
			sw_know_interp = FALSE;
			sw_interp = 0;
			ident_pattern = "isc_%d"; 
			utility_name = "isc_utility";
			count_name = "isc_count";
			slack_name = "isc_slack";
			database_name	= "gds_database";
			break;

		case IN_SW_GPRE_I:
			sw_ids = true;
			break;

		case IN_SW_GPRE_M:
			sw_auto = false;
			break;

		case IN_SW_GPRE_N:
			sw_lines = false;
			break;

		case IN_SW_GPRE_O:
			sw_standard_out = true;
			out_file = ib_stdout;
			break;

		case IN_SW_GPRE_R:
			sw_raw = true;
			break;

		case IN_SW_GPRE_S:
			sw_cstring = false;
			break;

		case IN_SW_GPRE_T:
			sw_trace = true;
			break;
//  FSG 14.Nov.2000 
		case IN_SW_GPRE_VERBOSE:
			sw_verbose = true;
			break;
		default:
			break;
		}
	}	// for (...)

	if ((sw_auto) && (default_user || default_password || default_lc_ctype)) {
		CPR_warn("gpre: -user, -password and -charset switches require -manual");
	}
//  
//  If one of the C++ variants was used/discovered, convert to C++ for
//  further internal use.
//  

	if (sw_language == lang_cpp || sw_language == lang_cplusplus)
		sw_language = lang_cxx;

#ifdef ALSYS_ADA
	if (sw_alsys) {
		for (src_ext_tab = dml_ext_table;; src_ext_tab++)
			if (src_ext_tab->ext_in_sw == IN_SW_GPRE_ALSYS)
				break;
	}
#endif

#ifdef VMS
//  
//  If we're on VMS, we may have an RMS sequential file rather than
//  a stream file, and keeping track of our place will be harder.
//  So... for VMS, copy the input to a stream file.
//  
//  If this is Alpha OpenVMS, then we also have to do some more
//  work, since RMS is a little different...
//  

#ifndef __ALPHA
	temp = (IB_FILE *) gds__temp_file(TRUE, "temp", 0);
	strcpy(temp_name, "temporary file");
#else
	temp = (IB_FILE *) gds__temp_file(TRUE, "temp", temp_name);
#endif
	if (temp != (IB_FILE *) - 1) {
		while ((c = get_char(input_file)) != EOF)
			ib_putc(c, temp);
		ib_fclose(input_file);
#ifdef __ALPHA
		ib_fclose(temp);
		temp = ib_fopen(temp_name, FOPEN_READ_TYPE);
#endif
	}
	else {
		ib_fprintf(ib_stderr, "gpre: can't open %s\n", temp_name);
		CPR_exit(FINI_ERROR);
	}

	input_file = temp;
#endif

#if defined(GPRE_COBOL) && !defined(BOOT_BUILD)
//  if cobol is defined we need both sw_cobol and sw_ansi to
//  determine how the string substitution table is set up
//  

	if (sw_language == lang_cobol)
		if (sw_ansi) {
			if (db)
				db->dbb_name->sym_string = "isc-database";
			comment_start = "      *  ";
			ident_pattern = "isc-%d";
			transaction_name = "isc-trans";
			database_name = "isc-database";
			utility_name = "isc-utility";
			count_name = "isc-count";
			slack_name = "isc-slack";
		}
        else { // just to be sure :-)
            comment_start = "*      ";
            transaction_name = "ISC_TRANS";
        }

	COB_name_init(sw_ansi);
#endif

//  
//  See if user has specified an interpretation on the command line,
//  as might be used for SQL access.
//  

	if (default_lc_ctype) {
		if (all_digits(default_lc_ctype)) {
			/* Numeric name? if so assume user has hard-coded a subtype number */

			sw_interp = atoi(default_lc_ctype);
			sw_know_interp = TRUE;
		}
		else if (compare_ASCII7z(default_lc_ctype, "DYNAMIC") == 0) {
			/* Dynamic means use the interpretation declared at runtime */

			sw_interp = ttype_dynamic;
			sw_know_interp = TRUE;
		}
		else if (isc_databases) {
			/* Name resolution done by MET_load_hash_table */

			isc_databases->dbb_c_lc_ctype = default_lc_ctype;
		}
	}

//  
//  Finally, open the output file, if we're not using standard out.
//  If only one file name was given, make sure it has the preprocessor
//  extension, then back up to that extension, zorch it, and add
//  the language extension. Then you've got an output name.  Take it
//  and add the correct extension.  If got an explicit output file
//  name, use it as is unless it doesn't have an extension in which
//  case use the default language extension.  Finally, open the file.
//  What could be easier?
//  

	if (!sw_standard_out) {
		const ext_table_t* out_src_ext_tab = src_ext_tab;
		if (use_lang_internal_gxx_output) {
			out_src_ext_tab = dml_ext_table;
    		while (out_src_ext_tab->ext_language != lang_internal_cxx) {
        		++out_src_ext_tab;
    		}
		}

		renamed = explicitt = true;
		if (!out_file_name) {
			out_file_name = spare_out_file_name;
			strcpy(spare_out_file_name, file_name);
			if (renamed = file_rename(spare_out_file_name, out_src_ext_tab->in,
							out_src_ext_tab->out))
			{
				explicitt = false;
			}
		}

		if (renamed) {
			for (p = out_file_name; *p; p++);
#ifdef VMS
			while (p != out_file_name && *p != '.' && *p != ']')
#else
			while (p != out_file_name && *p != '.' && *p != '/')
#endif
				p--;
			if (!explicitt)
				*p = 0;

			if (*p != '.') {
				strcpy(spare_out_file_name, out_file_name);
				out_file_name = spare_out_file_name;
				file_rename(out_file_name, out_src_ext_tab->out, NULL);
			}
		}

		if (!(strcmp(out_file_name, file_name))) {
			ib_fprintf(ib_stderr,
					   "gpre: output file %s would duplicate input\n",
					   out_file_name);
			CPR_exit(FINI_ERROR);
		}
		if ((out_file = ib_fopen(out_file_name, FOPEN_WRITE_TYPE)) == NULL) {
			ib_fprintf(ib_stderr, "gpre: can't open output file %s\n",
					   out_file_name);
			CPR_exit(FINI_ERROR);
		}
	}

//  Compile modules until end of file 

	sw_databases = isc_databases;

	try {
		for (end_position = 0; end_position = compile_module(end_position,filename_array[3]););  // empty loop
	}	// try
	catch (const std::exception&) {}  // fall through to the cleanup code

#ifdef FTN_BLK_DATA
	if (sw_language == lang_fortran)
		FTN_fini();
#endif

	MET_fini(0);
	ib_fclose(input_file);
#ifdef VMS
#ifdef __ALPHA
	delete(temp_name);
#endif
#endif

	if (!sw_standard_out) {
		ib_fclose(out_file);
		if (errors)
			unlink(out_file_name);
	}

	if (errors || warnings) {
		if (!errors)
			ib_fprintf(ib_stderr, "No errors, ");
		else if (errors == 1)
			ib_fprintf(ib_stderr, "1 error, ");
		else
			ib_fprintf(ib_stderr, "%3d errors, ", errors);
		if (!warnings)
			ib_fprintf(ib_stderr, "no warnings\n");
		else if (warnings == 1)
			ib_fprintf(ib_stderr, "1 warning\n");
		else
			ib_fprintf(ib_stderr, "%3d warnings\n", warnings);
	}

	CPR_exit((errors) ? FINI_ERROR : FINI_OK);
	return 0;

}


//____________________________________________________________
//  
//		Abort this silly program.
//  

void CPR_abort()
{

	++fatals;
	Firebird::status_exception::raise(1);
}


#ifdef DEV_BUILD
//____________________________________________________________
//  
//		Report an assertion failure and abort this silly program.
//  

void CPR_assert(const TEXT* file, int line)
{
	TEXT buffer[200];

	sprintf(buffer, "GPRE assertion failure file '%s' line '%d'", file, line);
	CPR_bugcheck(buffer);
}
#endif


//____________________________________________________________
//  
//		Issue an error message.
//  

void CPR_bugcheck(const TEXT* string)
{

	ib_fprintf(ib_stderr, "*** INTERNAL BUGCHECK: %s ***\n", string);
	MET_fini(0);
	CPR_abort();
}


//____________________________________________________________
//  
//       Mark end of a text description.
//  

void CPR_end_text( TXT text)
{

	text->txt_length = (USHORT) (token.tok_position - text->txt_position - 1);
}


//____________________________________________________________
//  
//		Issue an error message.
//  

int CPR_error(const TEXT* string)
{

	ib_fprintf(ib_stderr, "(E) %s:%d: %s\n", file_name, line + 1, string);
	errors++;

	return 0;
}


//____________________________________________________________
//  
//		Exit with status.
//  

void CPR_exit( int stat)
{
#ifdef LINUX

	if (trace_file_name[0])
	 {
		if (trace_file)
			ib_fclose(trace_file);
		unlink(trace_file_name);
	}

#else
	if (trace_file)
		ib_fclose(trace_file);
	if (trace_file_name[0])
		unlink(trace_file_name);
#endif

	exit(stat);
}


//____________________________________________________________
//  
//		Issue an warning message.
//  

void CPR_warn(const TEXT* string)
{

	ib_fprintf(ib_stderr, "(W) %s:%d: %s\n", file_name, line + 1, string);
	warnings++;
}


//____________________________________________________________
//  
//		Fortran, being a line oriented language, sometimes needs
//		to know when it is at end of line to avoid parsing into the
//		next statement.  CPR_eol_token normally gets the next token,
//		but if the language is FORTRAN and there isn't anything else
//		on the line, it fakes a dummy token to indicate end of line.
//  

TOK CPR_eol_token()
{
	if (sw_language != lang_fortran)
		return CPR_token();

//  Save the information from the previous token 

	prior_token = token;
	prior_token.tok_position = last_position;

	last_position =
		token.tok_position + token.tok_length + token.tok_white_space - 1;
	TEXT* p = token.tok_string;
	SSHORT num_chars = 0;

//  skip spaces 

	SSHORT c;
	for (c = nextchar(); c == ' '; c = nextchar()) {
		num_chars++;
		*p++ = (TEXT) c;
	}

//  in-line comments are equivalent to end of line 

	if (c == '!')
		while (c != '\n' && c != EOF) {
			c = nextchar();
			num_chars++;
		}

//  in-line SQL comments are equivalent to end of line 

	if (sw_sql && (c == '-')) {
		const SSHORT peek = nextchar();
		if (peek != '-')
			return_char(peek);
		else {
			while (c != '\n' && c != EOF) {
				c = nextchar();
				num_chars++;
			}
			last_position = position - 1;
		}
	}

	if (c == EOF) {
		token.tok_symbol = NULL;
		token.tok_keyword = KW_none;
		return NULL;
	}

//  Not EOL so back up to the begining and try again 

	if (c != '\n') {
		return_char(c);
		while (--num_chars > 0)
			return_char(*--p);
		return CPR_token();
	}

//  if we've got EOL, treat it like a semi-colon 
//  NOTE: the fact that the length of this token is set to 0, is used as an
//  indicator elsewhere that it was a faked token 

	token.tok_string[0] = ';';
	token.tok_string[1] = 0;
	token.tok_type = tok_punct;
	token.tok_length = 0;
	token.tok_white_space = 0;
	token.tok_position = position;
	token.tok_symbol = HSH_lookup(token.tok_string);
	token.tok_keyword = (KWWORDS) token.tok_symbol->sym_keyword;

	if (sw_trace)
		ib_puts(token.tok_string);

	return &token;
}


//____________________________________________________________
//  
//       Write text from the scratch trace file into a buffer.
//  

void CPR_get_text( TEXT* buffer, TXT text)
{
	SLONG start = text->txt_position;
	int length = text->txt_length;

//  On PC-like platforms, '\n' will be 2 bytes.  The txt_position
//  will be incorrect for ib_fseek.  The position is not adjusted
//  just for PC-like platforms because, we use ib_fseek () and
//  ib_getc to position ourselves at the token position.
//  We should keep both character position and byte position
//  and use them appropriately. for now use ib_getc ()
//  

#if (defined WIN_NT)
	if (ib_fseek(trace_file, 0L, 0))
#else
	if (ib_fseek(trace_file, start, 0))
#endif
	{
		ib_fseek(trace_file, 0L, 2);
		CPR_error("ib_fseek failed for trace file");
	}
#if (defined WIN_NT)
//  move forward to actual position 

	while (start--)
		ib_getc(trace_file);
#endif

	TEXT* p = buffer;
	while (length--)
		*p++ = ib_getc(trace_file);
	ib_fseek(trace_file, (SLONG) 0, 2);
}


//____________________________________________________________
//  
//       A BASIC-specific function which resides here since it reads from
//       the input file.  Look for a '\n' with no continuation character (&).
//       Eat tokens until previous condition is satisfied.
//       This function is used to "eat" an BASIC external function definition.
//  

void CPR_raw_read()
{
	SSHORT c;
	SCHAR token_string[MAXSYMLEN];
	bool continue_char = false;

	SCHAR* p = token_string;

	while (c = get_char(input_file)) {
		position++;
		if ((classes[c] == CHR_WHITE) && sw_trace && token_string) {
			*p = 0;
			ib_puts(token_string);
			token_string[0] = 0;
			p = token_string;
		}
		else
			*p++ = (SCHAR) c;

		if (c = '\n') {
			line++;
			line_position = 0;
			if (!continue_char)
				return;
			continue_char = false;
		}
		else {
			line_position++;
			if (classes[c] != CHR_WHITE)
				continue_char = (token.tok_keyword == KW_AMPERSAND);
		}
	}
}


//____________________________________________________________
//  
//		Generate a syntax error.
//  

void CPR_s_error(const TEXT* string)
{
	TEXT s[512];

	sprintf(s, "expected %s, encountered \"%s\"", string, token.tok_string);
	CPR_error(s);
	PAR_unwind();
}


//____________________________________________________________
//  
//       Make the current position to save description text.
//  

TXT CPR_start_text()
{
	TXT text = (TXT) ALLOC(TXT_LEN);
	text->txt_position = token.tok_position - 1;

	return text;
}


//____________________________________________________________
//  
//		Parse and return the next token.
//		If the token is a charset introducer, gobble it, grab the
//		next token, and flag that token as being in a non-default
//		character set.
//  

TOK CPR_token()
{
	TOK token = get_token();
	if (!token)
		return NULL;

	if (token->tok_type == tok_introducer) {
		SYM symbol = MSC_find_symbol(HSH_lookup(token->tok_string + 1), SYM_charset);
		if (!symbol) {
			TEXT err_buffer[100];

			sprintf(err_buffer, "Character set not recognized: '%.50s'",
					token->tok_string);
			CPR_error(err_buffer);
		}
		token = get_token();

		switch (sw_sql_dialect) {
		case SQL_DIALECT_V5:
			if (!(isQuoted(token->tok_type)))
				CPR_error("Can only tag quoted strings with character set");
			else
				token->tok_charset = symbol;
			break;

		default:
			if (token->tok_type != tok_sglquoted)
				CPR_error("Can only tag quoted strings with character set");
			else
				token->tok_charset = symbol;
			break;

		}
	}
	// use -charset switch if there is one for quoted strings
	// only after a database declaration is loaded and MET_load_hash_table run.
	else if (default_lc_ctype && text_subtypes) {
		switch (sw_sql_dialect) {
		case SQL_DIALECT_V5:
			if (isQuoted(token->tok_type)){
				token->tok_charset = MSC_find_symbol(HSH_lookup(default_lc_ctype), 
												   SYM_charset);
			}
			break;
		default:
			if (token->tok_type == tok_sglquoted){
				token->tok_charset = MSC_find_symbol(HSH_lookup(default_lc_ctype),
												   SYM_charset);
			}
			break;
		}
	}
	return token;
}


//____________________________________________________________
//  
//		Return true if the string consists entirely of digits.
//  

static bool all_digits(const char* str1)
{
	for (; *str1; str1++)
		if (!(classes[*str1] & CHR_DIGIT))
			return false;

	return true;
}


//____________________________________________________________
//  
//       Check the command line argument which follows
//       a switch which requires a string argument.
//       If there is a problem, explain and return.
//  

static bool arg_is_string(SLONG argc,
						  TEXT** argvstring,
						  const TEXT* errstring)
{
	const TEXT* str = *++argvstring;

	if (!argc || *str == '-') {
		ib_fprintf(ib_stderr, "%s", errstring);
		print_switches();
		return false;
	}

	return true;
}


//____________________________________________________________
//  
//		Compare two ASCII 7-bit strings, case insensitive.
//		Strings are null-byte terminated.
//		Return 0 if strings are equal,
//		(negative) if str1 < str2 
//		(positive) if str1 > str2
//  

static SSHORT compare_ASCII7z(const char* str1, const char* str2)
{

	for (; *str1; str1++, str2++)
		if (UPPER7(*str1) != UPPER7(*str2))
			return (UPPER7(*str1) - UPPER7(*str2));

	return 0;
}


//____________________________________________________________
//  
//		Switches have been processed and files have been opened.
//		Process a module and generate output.
//  

static SLONG compile_module( SLONG start_position, const TEXT* base_directory)
{
//  Reset miscellaneous pointers 

	isc_databases = sw_databases;
	requests = NULL;
	events = NULL;
	global_last_action = global_first_action = global_functions = NULL;

//  Position the input file and initialize various modules 

	ib_fseek(input_file, start_position, 0);
	input_char = input_buffer;

#if !(defined WIN_NT)
	trace_file = (IB_FILE *) gds__temp_file(TRUE, SCRATCH, 0);
#else
//  PC-like platforms can't delete a file that is open.  Therefore
//  we will save the name of the temp file for later deletion. 

	trace_file = (IB_FILE *) gds__temp_file(TRUE,  const_cast<char*>(SCRATCH),
											trace_file_name);
#endif

	if (trace_file == (IB_FILE *) - 1) {
		trace_file = NULL;
		CPR_error("Couldn't open scratch file");
		return 0;
	}

	position = start_position;
	MSC_init();
	HSH_init();
	PAR_init();
	CMP_init();

//  Take a first pass at the module 

	SLONG end_position = pass1(base_directory);

//  finish up any based_ons that got deferred 

#ifdef GPRE_FORTRAN
	if (sw_language == lang_fortran)
		finish_based(global_first_action);
#endif

	MET_fini(NULL);
	PAR_fini();

	if (errors)
		return end_position;

	for (GPRE_REQ request = requests; request; request = request->req_next)
		CMP_compile_request(request);

	ib_fseek(input_file, start_position, 0);
	input_char = input_buffer;

	if (!errors)
		pass2(start_position);

	return end_position;
}


//____________________________________________________________
//  
//		Add the appropriate extension to a file
//		name, if there's not one already.  If
//		the "appropriate" one is there and a
//		new extension is given, use it.
//  

static bool file_rename(TEXT* file_name,
						const TEXT* extension,
						const TEXT* new_extension)
{
	TEXT *p;

//  go to the end of the file name 

	for (p = file_name; *p; p++);

	TEXT* terminator = p;

//  back up to the last extension (if any) 

#if defined(VMS)
	while ((p != file_name) && (*p != '.') && (*p != ']'))
#elif defined(WIN_NT)
	while ((p != file_name) && (*p != '.') && (*p != '/') && (*p != '\\'))
#else
	while ((p != file_name) && (*p != '.') && (*p != '/'))
#endif

	--p;

//  
//  There's a match and the file spec has no extension, 
//  so add extension.  
//  

	if (*p != '.') {
		while (*terminator++ = *extension++);
		return true;
	}

//  
//  There's a match and an extension.  If the extension in 
//  the table matches the one on the file, we don't want 
//  to add a duplicate.  Otherwise add it.
//  

	TEXT* ext = p;
	for (const TEXT* q = extension; SAME(p, q); p++, q++)
		if (!*p) {
			if (new_extension)
				while (*ext++ = *new_extension++);
			return false;
		}

#ifndef VMS
//  Didn't match extension, so add the extension 
	while (*terminator++ = *extension++);
#endif

	return true;
}


#ifdef GPRE_FORTRAN
//____________________________________________________________
//  
//		Scan through the based_on actions
//		looking for ones that were deferred
//       because we didn't have a database yet.
//  
//		Look at each action in turn, and if it's
//		a based_on with a field name rather than a
//		field block pointer, complete the name parse.
//		If there's a database name, find the database,
//		then the relation within the database, then
//     	the field.  Otherwise, look through all databases
//       for the relation.
//  

static void finish_based( ACT action)
{
	GPRE_REL relation;
	GPRE_FLD field;
	TEXT s[128];

	for (; action; action = action->act_rest) {
		if (action->act_type != ACT_basedon)
			continue;

		/* If there are no databases either on the command line or in
		   this subroutine or main program, can't do a BASED_ON. */

		if (!isc_databases) {
			CPR_error
				("No database defined.  Needed for a BASED_ON operation");
			continue;
		}

		BAS based_on = (BAS) action->act_object;
		if (!based_on->bas_fld_name)
			continue;

		DBB db = NULL;
		if (based_on->bas_db_name) {
			SYM symbol = HSH_lookup((SCHAR *) based_on->bas_db_name);
			for (; symbol; symbol = symbol->sym_homonym)
				if (symbol->sym_type == SYM_database)
					break;

			if (symbol) {
				db = (DBB) symbol->sym_object;
				relation =
					MET_get_relation(db, (TEXT *) based_on->bas_rel_name, "");
				if (!relation) {
					sprintf(s, "relation %s is not defined in database %s",
							based_on->bas_rel_name->str_string, based_on->bas_db_name->str_string);
					CPR_error(s);
					continue;
				}
				field = MET_field(relation, (char *) based_on->bas_fld_name);
			}
			else {
				if (based_on->bas_flags & BAS_ambiguous) {
					/* The reference could have been DB.RELATION.FIELD or
					   RELATION.FIELD.SEGMENT.  It's not the former.  Try
					   the latter. */

					based_on->bas_fld_name = based_on->bas_rel_name;
					based_on->bas_rel_name = based_on->bas_db_name;
					based_on->bas_db_name = NULL;
					based_on->bas_flags |= BAS_segment;
				}
				else {
					sprintf(s, "database %s is not defined",
							based_on->bas_db_name->str_string);
					CPR_error(s);
					continue;
				}
			}
		}

		if (!db) {
			field = NULL;
			for (db = isc_databases; db; db = db->dbb_next)
				if (relation =
					MET_get_relation(db, (TEXT *) based_on->bas_rel_name, "")) {
					if (field) {
						/* The field reference is ambiguous.  It exists in more
						   than one database. */

						sprintf(s, "field %s in relation %s ambiguous",
								based_on->bas_fld_name->str_string,
								based_on->bas_rel_name->str_string);
						CPR_error(s);
						break;
					}
					field =
						MET_field(relation, (char *) based_on->bas_fld_name);
				}

			if (db)
				continue;
			if (!relation && !field) {
				sprintf(s, "relation %s is not defined",
						based_on->bas_rel_name->str_string);
				CPR_error(s);
				continue;
			}
		}

		if (!field) {
			sprintf(s, "field %s is not defined in relation %s",
					based_on->bas_fld_name->str_string,
					based_on->bas_rel_name->str_string);
			CPR_error(s);
			continue;
		}

		if ((based_on->bas_flags & BAS_segment)
			&& !(field->fld_flags & FLD_blob)) {
			sprintf(s, "field %s is not a blob",
					field->fld_symbol->sym_string);
			CPR_error(s);
			continue;
		}

		based_on->bas_field = field;
	}
}
#endif

//____________________________________________________________
//  
//		Return a character to the input stream.
//  

static int get_char( IB_FILE * file)
{
	if (input_char != input_buffer) {
		return (int) *--input_char;

	}
	else
	{
		const USHORT pc = ib_getc(file);

//  Dump this char to stderr, so we can see
//  what input line will cause this ugly
//  core dump.
//  FSG 14.Nov.2000 
		if (sw_verbose) {
			ib_fprintf(ib_stderr, "%c", pc);
		}
		return pc;
	}


}


//____________________________________________________________
//  
//  
//  Parse the input line arguments, saving
//  interesting switches in a switch table.
//  The first entry in the switch table is 
//  reserved for the language, and is set
//  later, even if specified here.
//  

static bool get_switches(int			argc,
						 TEXT**		argv,
						 const in_sw_tab_t*	in_sw_table,
						 SW_TAB		sw_table,
						 TEXT**		file_array)
{
	TEXT *p, *q, *string;
	const in_sw_tab_t* in_sw_table_iterator;
	SW_TAB sw_table_iterator;
	USHORT in_sw;

//  
//  Read all the switches and arguments, acting only on those
//  that apply immediately, since we may find out more when
//  we try to open the file. 
//  

	sw_table_iterator = sw_table;

	for (--argc; argc; argc--)
	{
		string = *++argv;
		if (*string != '?')
		{
			if (*string != '-')
			{
				if (!file_array[1])
				{
					if (!file_array[0]) {
						file_array[0] = string;
					} else {
						file_array[1] = string;
					}
					continue;
				}
				else
				{
					// both input and output files have been defined, hence
					// there is an unknown switch
					in_sw = IN_SW_GPRE_0;
				}
			}
			else
			{
				/* iterate through the switch table, looking for matches */

				sw_table_iterator++;
				sw_table_iterator->sw_in_sw = IN_SW_GPRE_0;
				for (in_sw_table_iterator = in_sw_table;
					 q = in_sw_table_iterator->in_sw_name;
					 in_sw_table_iterator++)
				{
					p = string + 1;

					/* handle orphaned hyphen case */

					if (!*p--)
						break;

					/* compare switch to switch name in table */

					while (*p) {
						if (!*++p) {
							sw_table_iterator->sw_in_sw = (gpre_cmd_switch)in_sw_table_iterator->in_sw;
						}
						if (UPPER7(*p) != *q++) {
							break;
						}
					}
					/* end of input means we got a match.  stop looking */

					if (!*p)
						break;
				}
				in_sw = sw_table_iterator->sw_in_sw;
			}
		}

		/*
		 * Check here for switches that affect file look ups
		 * and -D so we don't lose their arguments.
		 * Give up here if we find a bad switch.
		 */

		if (*string == '?') {
			in_sw = IN_SW_GPRE_0;
		}

		switch (in_sw) {
		case IN_SW_GPRE_C:
			sw_language = lang_c;
			sw_table_iterator--;
			break;

		case IN_SW_GPRE_CXX:
			sw_language = lang_cxx;
			sw_table_iterator--;
			break;

		case IN_SW_GPRE_CPLUSPLUS:
			sw_language = lang_cplusplus;
			sw_table_iterator--;
			break;

		case IN_SW_GPRE_GXX:
			/* If we decrement sw_tab the switch is removed
			 * from the table and not processed in the main
			 * switch statement.  Since IN_SW_GPRE_G will always
			 * be processed for lang_internal, we leave our
			 * switch in so we can clean up the mess left behind
			 * by IN_SW_GPRE_G
			 */
			sw_language = lang_internal;
			break;

		case IN_SW_GPRE_G:
			sw_language = lang_internal;
			sw_table_iterator--;
			break;

#ifdef GPRE_FORTRAN
		case IN_SW_GPRE_F:
			sw_language = lang_fortran;
			sw_table_iterator--;
			break;
#endif
#ifdef GPRE_PASCAL
		case IN_SW_GPRE_P:
			sw_language = lang_pascal;
			sw_table_iterator--;
			break;
#endif
		case IN_SW_GPRE_X:
			sw_external = true;
			sw_table_iterator--;
			break;
#ifdef GPRE_COBOL
		case IN_SW_GPRE_COB:
			sw_language = lang_cobol;
			sw_table_iterator--;
			break;
#endif
		case IN_SW_GPRE_LANG_INTERNAL : 
			sw_language = lang_internal;
			/*sw_tab--;*/
			break;

		case IN_SW_GPRE_D:
			if (!arg_is_string
				(--argc, argv,
				 "Command line syntax: -d requires database name:\n ")) 
			{
				return false;
			}

			file_array[2] = *++argv;
			string = *argv;
			if (*string == '=')
				if (!arg_is_string
					(--argc, argv,
					 "Command line syntax: -d requires database name:\n "))
				{
					return false;
				}
				else
					file_array[2] = *++argv;
			break;

		case IN_SW_GPRE_BASE:
			if (!arg_is_string
				(--argc, argv,
				 "Command line syntax: -b requires database base directory:\n ")) 
			{
				return false;
			}

			file_array[3] = *++argv;
			string = *argv;
			if (*string == '=')
				if (!arg_is_string
					(--argc, argv,
					 "Command line syntax: -b requires database base directory:\n "))
				{
					return false;
				}
				else
					file_array[3] = *++argv;
			break;

		case IN_SW_GPRE_HANDLES:
			if (!arg_is_string(
					--argc,
					argv,
					"Command line syntax: -h requires handle package name\n"))
			{
					return false;
			}
			strcpy(ada_package, *++argv);
			strcat(ada_package, ".");
			break;

		case IN_SW_GPRE_SQLDA:
			if (!arg_is_string(
					--argc,
					argv,
					"Command line syntax: -sqlda requires NEW\n "))
			{
				return false;
			}

			if (**argv != 'n' || **argv != 'N') {
				ib_fprintf(ib_stderr,
						   "-sqlda :  Deprecated Feature: you must use XSQLDA\n ");
				print_switches();
				return false;
			}
			break;

		case IN_SW_GPRE_SQLDIALECT:
			{
				int inp;
				if (!arg_is_string(
						--argc,
						argv,
						"Command line syntax: -SQL_DIALECT requires value 1, 2 or 3 \n "))
				{
					return false;
				}
				++argv;
				inp = atoi(*argv);
				if (inp < 1 || inp > 3) {
					ib_fprintf(ib_stderr,
							   "Command line syntax: -SQL_DIALECT requires value 1, 2 or 3 \n ");
					print_switches();
					return false;
				}
				else {
					sw_sql_dialect = inp;
				}
				dialect_specified = true;
				break;
			}

		case IN_SW_GPRE_Z:
			if (!sw_version) {
				ib_printf("gpre version %s\n", GDS_VERSION);
			}
			sw_version = true;
			break;

		case IN_SW_GPRE_0:
			if (*string != '?') {
				ib_fprintf(ib_stderr, "gpre: unknown switch %s\n", string);
			}
			print_switches();
			return false;

		case IN_SW_GPRE_USER:
			if (!arg_is_string(
					--argc,
					argv,
					"Command line syntax: -user requires user name string:\n "))
			{
				return false;
			}
			default_user = *++argv;
			break;

		case IN_SW_GPRE_PASSWORD:
			if (!arg_is_string(
					--argc,
					argv,
					"Command line syntax: -password requires password string:\n "))
			{
				return false;
			}
			default_password = *++argv;
			break;

		case IN_SW_GPRE_INTERP:
			if (!arg_is_string(
					--argc,
					argv,
					"Command line syntax: -charset requires character set name:\n "))
			{
				return false;
			}
			default_lc_ctype = (TEXT *) * ++argv;
			break;

		}
	}

	sw_table_iterator++;
	sw_table_iterator->sw_in_sw = IN_SW_GPRE_0;

	return true;
}


//____________________________________________________________
//  
//		Parse and return the next token.
//  

static TOK get_token()
{
	SSHORT next;
	USHORT peek;
	TEXT *p, *end;
	UCHAR class_;

//  Save the information from the previous token 

	prior_token = token;
	prior_token.tok_position = last_position;

	last_position =
		token.tok_position + token.tok_length + token.tok_white_space - 1;
	int start_line = line;
	SLONG start_position = position;
	token.tok_charset = NULL;

	SSHORT c = skip_white();

#ifdef GPRE_COBOL
//  Skip over cobol line continuation characters 
	if (sw_language == lang_cobol && !sw_ansi)
		while (line_position == 1) {
			c = skip_white();
			start_line = line;
		}
#endif

//  Skip fortran line continuation characters 

#ifdef GPRE_FORTRAN
	if (sw_language == lang_fortran) {
		while (line_position == 6) {
			c = skip_white();
			start_line = line;
		}
		if (sw_sql && line != start_line) {
			return_char(c);
			token.tok_string[0] = ';';
			token.tok_string[1] = 0;
			token.tok_type = tok_punct;
			token.tok_length = 0;
			token.tok_white_space = 0;
			token.tok_position = start_position + 1;
			token.tok_symbol = HSH_lookup(token.tok_string);
			token.tok_keyword = (KWWORDS) token.tok_symbol->sym_keyword;
			return &token;
		}
	}
#endif
//  Get token rolling 

	p = token.tok_string;
	end = p + sizeof(token.tok_string);
	*p++ = (TEXT) c;

	if (c == EOF) {
		token.tok_symbol = NULL;
		token.tok_keyword = KW_none;
		return NULL;
	}

	token.tok_position = position;
	token.tok_white_space = 0;
	class_ = classes[c];

#ifdef GPRE_ADA
	if ((sw_language == lang_ada) && (c == '\'')) {
		SSHORT c1, c2;
		c1 = nextchar();
		c2 = nextchar();
		if (c2 != '\'')
			class_ = CHR_LETTER;
		return_char(c2);
		return_char(c1);
	}
#endif

	bool label = false;

	if (sw_sql && (class_ & CHR_INTRODUCER)) {
		while (classes[c = nextchar()] & CHR_IDENT)
			if (p < end)
				*p++ = (TEXT) c;
		return_char(c);
		token.tok_type = tok_introducer;
	}
	else if (class_ & CHR_LETTER) {
		while (true) {
			while (classes[c = nextchar()] & CHR_IDENT)
				*p++ = (TEXT) c;
			if (c != '-' || sw_language != lang_cobol)
				break;
			if (sw_language == lang_cobol && sw_ansi)
				*p++ = (TEXT) c;
			else
				*p++ = '_';
		}
		return_char(c);
		token.tok_type = tok_ident;
	}
	else if (class_ & CHR_DIGIT) {
#ifdef GPRE_FORTRAN
		if (sw_language == lang_fortran && line_position < 7)
			label = true;
#endif
		while (classes[c = nextchar()] & CHR_DIGIT)
			*p++ = (TEXT) c;
		if (label) {
			*p = 0;
			remember_label(token.tok_string);
		}
		if (c == '.') {
			*p++ = (TEXT) c;
			while (classes[c = nextchar()] & CHR_DIGIT)
				*p++ = (TEXT) c;
		}
		if (!label && (c == 'E' || c == 'e')) {
			*p++ = (TEXT) c;
			c = nextchar();
			if (c == '+' || c == '-')
				*p++ = (TEXT) c;
			else
				return_char(c);
			while (classes[c = nextchar()] & CHR_DIGIT)
				*p++ = (TEXT) c;
		}
		return_char(c);
		token.tok_type = tok_number;
	}
	else if ((class_ & CHR_QUOTE) || (class_ & CHR_DBLQUOTE)) {
		token.tok_type = (class_ & CHR_QUOTE) ? tok_sglquoted : tok_dblquoted;
		for (;;) {
			next = nextchar();
			if (sw_language == lang_cobol && sw_ansi && next == '\n') {
				if (prior_line_position == 73) {
					/* should be a split literal */
					next = skip_white();
					if (next != '-' || line_position != 7) {
						CPR_error("unterminated quoted string");
						break;
					}
					next = skip_white();
					if (next != c) {
						CPR_error("unterminated quoted string");
						break;
					}
					next = nextchar();
					token.tok_white_space += line_position - 1;
				}
				else {
					CPR_error("unterminated quoted string");
					break;
				}
			}
			else if (next == EOF
					 || (next == '\n' && (p[-1] != '\\' || sw_sql))) {
				return_char(*p);

				/*  Decrement, then increment line counter, for accuracy of 
				   the error message for an unterminated quoted string. */

				line--;
				CPR_error("unterminated quoted string");
				line++;
				break;
			}

			/* If we can hold the literal do so, else assume it is in part
			   of program we do not care about */

			if (next == '\\' && !sw_sql &&
				((sw_language == lang_c) || (isLangCpp(sw_language))))
			{
				peek = nextchar();
				if (peek == '\n') {
					token.tok_white_space += 2;
				} else if (p < end) {
					*p++ = (TEXT) next;
					if (p < end) {
						*p++ = (TEXT) peek;
					}
				}
				continue;
			}
			if (p < end)
				*p++ = (TEXT) next;
			if (next == c)
				/* If 2 quotes in a row, treat 2nd as literal - bug #1530 */
			{
				peek = nextchar();
				if (peek != c) {
					return_char(peek);
					break;
				}
				else
					token.tok_white_space++;
			}
		}
	}
	else if (c == '.') {
		if (classes[c = nextchar()] & CHR_DIGIT) {
			*p++ = (TEXT) c;
			while (classes[c = nextchar()] & CHR_DIGIT)
				*p++ = (TEXT) c;
			if ((c == 'E' || c == 'e')) {
				*p++ = (TEXT) c;
				c = nextchar();
				if (c == '+' || c == '-')
					*p++ = (TEXT) c;
				else
					return_char(c);
				while (classes[c = nextchar()] & CHR_DIGIT)
					*p++ = (TEXT) c;
			}
			return_char(c);
			token.tok_type = tok_number;
		}
		else {
			return_char(c);
			token.tok_type = tok_punct;
			*p++ = nextchar();
			*p = 0;
			if (!HSH_lookup(token.tok_string))
				return_char(*--p);
		}
	}
	else {
		token.tok_type = tok_punct;
		*p++ = nextchar();
		*p = 0;
		if (!HSH_lookup(token.tok_string))
			return_char(*--p);
	}

	SYM symbol;

	token.tok_length = p - token.tok_string;
	*p++ = 0;
	if (isQuoted(token.tok_type)) {
		strip_quotes(token);
	/** If the dialect is 1 then anything that is quoted is
	a string. Don not lookup in the hash table to prevent 
	parsing confusion. 
    **/
		if (sw_sql_dialect != SQL_DIALECT_V5)
			token.tok_symbol = symbol = HSH_lookup(token.tok_string);
		else
			token.tok_symbol = symbol = NULL;
		if (symbol && symbol->sym_type == SYM_keyword)
			token.tok_keyword = (KWWORDS) symbol->sym_keyword;
		else
			token.tok_keyword = KW_none;
	}
	else if (sw_case) {
		if (!override_case) {
			token.tok_symbol = symbol = HSH_lookup2(token.tok_string);
			if (symbol && symbol->sym_type == SYM_keyword)
				token.tok_keyword = (KWWORDS) symbol->sym_keyword;
			else
				token.tok_keyword = KW_none;
		}
		else {
			token.tok_symbol = symbol = HSH_lookup(token.tok_string);
			if (symbol && symbol->sym_type == SYM_keyword)
				token.tok_keyword = (KWWORDS) symbol->sym_keyword;
			else
				token.tok_keyword = KW_none;
			override_case = false;
		}
	}
	else {
		token.tok_symbol = symbol = HSH_lookup(token.tok_string);
		if (symbol && symbol->sym_type == SYM_keyword)
			token.tok_keyword = (KWWORDS) symbol->sym_keyword;
		else
			token.tok_keyword = KW_none;
	}

// ** Take care of GDML context variables. Context variables are inserted 
//into the hash table as it is. There is no upper casing of the variable 
//name done. Hence in all likelyhood we might have missed it while looking it
//up if -e switch was specified. Hence
//IF symbol is null AND it is not a quoted string AND -e switch was specified
//THEN search again using HSH_lookup2().
//*  
	if ((token.tok_symbol == NULL) && (!isQuoted(token.tok_type)) && sw_case) {
		token.tok_symbol = symbol = HSH_lookup2(token.tok_string);
		if (symbol && symbol->sym_type == SYM_keyword)
			token.tok_keyword = (KWWORDS) symbol->sym_keyword;
		else
			token.tok_keyword = KW_none;
	}

//  for FORTRAN, make note of the first token in a statement 

	assert(first_position <= MAX_USHORT);
	token.tok_first = (USHORT) first_position;
	first_position = FALSE;

	if (sw_trace)
		ib_puts(token.tok_string);

	return &token;
}


//____________________________________________________________
//  
//		Get the next character from the input stream.
//       Also, for Fortran, mark the beginning of a statement
//  

static int nextchar()
{
	SSHORT c;

	position++;
	line_position++;
	if ((c = get_char(input_file)) == '\n') {
		line++;
		prior_line_position = line_position;
		line_position = 0;
	}

//  For silly fortran, mark the first token in a statement so
//  we can decide to start the database field substitution string
//  with a continuation indicator if appropriate. 

	if (line_position == 1) {
		first_position = TRUE;

		/* If the first character on a Fortran line is a tab, bump up the
		   position indicator. */

#ifdef GPRE_FORTRAN
		if (sw_language == lang_fortran && c == '\t')
			line_position = 7;
#endif
	}

//  if this is a continuation line, the next token is not
//  the start of a statement. 

#ifdef GPRE_FORTRAN
	if (sw_language == lang_fortran && line_position == 6 && c != ' '
		&& c != '0')
	{
		first_position = FALSE;
	}
#endif

#ifdef GPRE_COBOL
	if (sw_language == lang_cobol &&
		(!sw_ansi && line_position == 1 && c == '-') ||
		(sw_ansi && line_position == 7 && c == '-'))
	{
		first_position = FALSE;
	}
#endif

	if (position > traced_position) {
		traced_position = position;
		ib_fputc(c, trace_file);
	}

	return c;
}


//____________________________________________________________
//  
//		Make first pass at input file.  This involves
//		passing thru tokens looking for keywords.  When
//		a keyword is found, try to parse an action.  If
//		the parse is successful (an action block is returned)
//		link the new action into the system data structures
//		for processing on pass 2.
//  

static SLONG pass1(const TEXT* base_directory)
{
	ACT action;
	SLONG start;

//  FSG 14.Nov.2000 
	if (sw_verbose) {
		ib_fprintf(ib_stderr,
				   "*********************** PASS 1 ***************************\n");
	}

	while (CPR_token())
	{
		while (token.tok_symbol)
		{
			start = token.tok_position;
			if (action = PAR_action(base_directory))
			{
				action->act_position = start;
				if (!(action->act_flags & ACT_back_token)) {
					action->act_length = last_position - start;
				} else {
					action->act_length =
						prior_token.tok_position +
						prior_token.tok_length - 1 - start;
				}
				if (global_first_action) {
					global_last_action->act_rest = action;
				} else {
					global_first_action = action;
				}

				/* Allow for more than one action to be generated by a token. */

				do
				{
					global_last_action = action;
					if (action = action->act_rest)
					{
						if (action->act_type == ACT_database)
						{
							/* CREATE DATABASE has two actions the second one
							   is do generate global decl at the start of the
							   program file. */

							global_last_action->act_rest = NULL;
							action->act_rest = global_first_action;
							global_first_action = action;
							action->act_position = -1;
							action->act_length = -1;
							break;
						}
						else
						{
							action->act_position = global_last_action->act_position;
							action->act_length = 0;
						}
					}
				} while (action);

				if (global_last_action->act_flags & ACT_break) {
					return last_position;
				}

				if (!token.tok_length &&
					((int) token.tok_keyword == (int) KW_SEMI_COLON))
				{
					break;
				}
			}
		}
	}

	if (isc_databases &&
		(isc_databases->dbb_flags & DBB_sqlca) &&
		!isc_databases->dbb_filename)
	{
		CPR_error("No database specified");
	}

	return 0;
}


//____________________________________________________________
//  
//		Make a second pass thru the input file turning actions into
//		comments, substituting text for actions, and generating the
//		output file.
//  

static void pass2( SLONG start_position)
{
	SSHORT d, prior, comment_start_len, to_skip;
	SLONG column, start;
	SLONG i;
	bool line_pending;

	SSHORT c = 0;

//  FSG 14.Nov.2000 
	if (sw_verbose) {
		ib_fprintf(ib_stderr,
				   "*********************** PASS 2 ***************************\n");
	}

	bool suppress_output = false;

	const bool sw_block_comments =
		sw_language == lang_c		||
		isLangCpp(sw_language)      ||
		sw_language == lang_pascal;

//  Put out a distintive module header 

	if (!sw_first++)
	{
		for (i = 0; i < 5; ++i)
		{
			ib_fprintf(out_file,
					   "%s********** Preprocessed module -- do not edit **************%s\n",
					   comment_start, comment_stop);
		}
		ib_fprintf(out_file,
				   "%s**************** gpre version %s *********************%s\n",
				   comment_start, GDS_VERSION, comment_stop);
	}

#ifdef GPRE_ADA
	if ((sw_language == lang_ada) && (ada_flags & ADA_create_database))
		ib_fprintf(out_file, "with unchecked_conversion;\nwith system;\n");
#endif
//  
//if (sw_lines)
//   ib_fprintf (out_file, "#line 1 \"%s\"\n", file_name);
//  

	SLONG line = 0;
	if (sw_lines)
		line_pending = true;
	else
		line_pending = false;
	SLONG current = 1 + start_position;
	column = 0;

	comment_start_len = strlen(comment_start);
	to_skip = 0;

//  Dump text until the start of the next action, then process the action. 

	for (const act* action = global_first_action; action; action = action->act_rest)
	{
		/* Dump text until the start of the next action.  If a line marker
		   is pending and we see an end of line, dump out the marker. */

		for (; current < action->act_position; current++)
		{
			c = get_char(input_file);
			if (c == EOF) {
				CPR_error("internal error -- unexpected EOF between actions");
				return;
			}
			if (c == '\n' || !line) {
				line++;
				if (line_pending) {
					if (line == 1)
						ib_fprintf(out_file, "#line %ld \"%s\"\n", line,
								   file_name);
					else
						ib_fprintf(out_file, "\n#line %ld \"%s\"", line,
								   file_name);

					line_pending = false;
				}
				if (line == 1 && c == '\n')
					line++;
				column = -1;
			}
			ib_putc(c, out_file);
			if (c == '\t') {
				column = (column + 8) & ~7;
			} else {
				++column;
			}
		}

		// Determine if this action is one which requires line continuation
		// handling in certain languages.

		const bool continue_flag =
			(action->act_type == ACT_variable)			||
			(action->act_type == ACT_segment)			||
			(action->act_type == ACT_segment_length) ;

		// Unless the action is purely a marker, insert a comment initiator
		// into the output stream.

		start = column;
		if (!(action->act_flags & ACT_mark)) {
			if (sw_language == lang_fortran) {
				ib_fputc('\n', out_file);
				ib_fputs(comment_start, out_file);
			}
			else if (sw_language == lang_cobol)
				if (continue_flag)
					suppress_output = true;
				else {
					ib_fputc('\n', out_file);
					ib_fputs(comment_start, out_file);
					to_skip = (column < 7) ? comment_start_len - column : 0;
					column = 0;
				}
			else
				ib_fputs(comment_start, out_file);
		}

		/* Next, dump the text of the action to the output stream. */

		for (i = 0; i <= action->act_length; ++i, ++current) {
			if (c == EOF) {
				CPR_error("internal error -- unexpected EOF in action");
				return;
			}
			prior = c;
			c = get_char(input_file);
			if (!suppress_output) {
				/* close current comment to avoid nesting comments */
				if (sw_block_comments && !(action->act_flags & ACT_mark) &&
					c == comment_start[0]) {
					return_char((d = get_char(input_file)));
					if (d == comment_start[1])
						ib_fputs(comment_stop, out_file);
				}
				if (sw_language != lang_cobol || !sw_ansi || c == '\n'
					|| to_skip-- <= 0)
					ib_putc(c, out_file);
				if (c == '\n') {
					line++;
					if ((sw_language == lang_fortran) ||
						(sw_language == lang_ada) ||
						(sw_language == lang_cobol)) {
						ib_fputs(comment_start, out_file);
						to_skip =
							(column < 7) ? comment_start_len - column : 0;
						column = 0;
					}
				}

				/* reopen our comment at end of user's comment */

				if (sw_block_comments && !(action->act_flags & ACT_mark) &&
					prior == comment_stop[0] && c == comment_stop[1])
					ib_fputs(comment_start, out_file);
			}
		}

		/* Unless action was purely a marker, insert a comment terminator. */

		if (!(action->act_flags & ACT_mark) && !suppress_output) {
			ib_fputs(comment_stop, out_file);
			if ((sw_language == lang_fortran) || (sw_language == lang_cobol))
				ib_fputc('\n', out_file);
		}

		suppress_output = false;
		(*gen_routine) (action, start);
		if (action->act_type == ACT_routine &&
			!action->act_object &&
			((sw_language == lang_c) || (isLangCpp(sw_language)))) continue;

		if (action->act_flags & ACT_break)
			return;

		if (sw_lines)
			line_pending = true;
		column = 0;
		to_skip = 0;
	}

//  We're out of actions -- dump the remaining text to the output stream. 

	if (!line && line_pending) {
		ib_fprintf(out_file, "#line 1 \"%s\"\n", file_name);
		line_pending = false;
	}


	while ((c = get_char(input_file)) != EOF) {
		if (c == '\n' && line_pending) {
			ib_fprintf(out_file, "\n#line %ld \"%s\"", line + 1, file_name);
			line_pending = false;
		}
		if (c == EOF) {
			CPR_error("internal error -- unexpected EOF in tail");
			return;
		}
		ib_putc(c, out_file);
	}

//  Last but not least, generate any remaining functions 

	for (; global_functions; global_functions = global_functions->act_next)
		(*gen_routine) (global_functions, 0);
}


//____________________________________________________________
//  
//       Print out the switch table as an
//       aid to those who have forgotten or are fishing
//       

static void print_switches()
{
	const in_sw_tab_t* in_sw_table_iterator;

	ib_fprintf(ib_stderr, "\tlegal switches are:\n");
	for (in_sw_table_iterator = gpre_in_sw_table;
		 in_sw_table_iterator->in_sw;
		 in_sw_table_iterator++)
	{
		if (in_sw_table_iterator->in_sw_text) {
			ib_fprintf(ib_stderr, "%s%s\n", in_sw_table_iterator->in_sw_name,
					   in_sw_table_iterator->in_sw_text);
        }
    }

	ib_fprintf(ib_stderr, "\n\tand the internal 'illegal' switches are:\n");
	for (in_sw_table_iterator = gpre_in_sw_table;
		 in_sw_table_iterator->in_sw;
		 in_sw_table_iterator++)
	{
		if (!in_sw_table_iterator->in_sw_text) {
			ib_fprintf(ib_stderr, "%s\n", in_sw_table_iterator->in_sw_name);
        }
    }

}


//____________________________________________________________
//  
//		Set a bit in the label vector indicating
//       that a label has been used.  If the label
//		is bigger than the vector, punt.
//       

static void remember_label( TEXT * label_string)
{
	UCHAR target_byte;
	SLONG label;

	label = atoi(label_string);
	if (label < 8192) {
		target_byte = label & 7;
		label >>= 3;
		fortran_labels[label] |= 1 << target_byte;
	}
}


//____________________________________________________________
//  
//		Return a character to the input stream.
//  

static void return_char( SSHORT c)
{

	--position;
	--line_position;

//  note putting back a new line results in incorrect line_position value 

	if (c == '\n') {
		--line;
	}

	*input_char++ = (TEXT) c;
}


//____________________________________________________________
//  
//		Skip over white space and comments in input stream
//  

static SSHORT skip_white()
{
	SSHORT c, c2, next;

	while (true) {
		if ((c = nextchar()) == EOF)
			return c;

		c = c & 0xff;

		/* skip Fortran comments */

#ifdef GPRE_FORTRAN
		if (sw_language == lang_fortran &&
			line_position == 1 && (c == 'C' || c == 'c' || c == '*')) {
			while ((c = nextchar()) != '\n' && c != EOF);
			continue;
		}
#endif

#ifdef GPRE_COBOL
		/* skip sequence numbers when ansi COBOL */

		if (sw_language == lang_cobol && sw_ansi) {
			while (line_position < 7 && (c = nextchar()) != '\n' && c != EOF);
		}

		/* skip COBOL comments and conditional compilation */

		if (sw_language == lang_cobol &&
			(!sw_ansi && line_position == 1 &&
			 (c == 'C' || c == 'c' || c == '*' || c == '/' || c == '\\') ||
			 (sw_ansi && line_position == 7 && c != '\t' && c != ' '
			  && c != '-'))) {
			while ((c = nextchar()) != '\n' && c != EOF);
			continue;
		}
#endif

		SSHORT class_ = classes[c];

		if (class_ & CHR_WHITE)
			continue;

		/* skip in-line SQL comments */

		if (sw_sql && (c == '-')) {
			c2 = nextchar();
			if (c2 != '-')
				return_char(c2);
			else {
				while ((c = nextchar()) != '\n' && c != EOF);
				last_position = position - 1;
				continue;
			}
		}

		/* skip C, C++ and PL/I comments */

		if (c == '/' &&
			(sw_language == lang_c ||
			 isLangCpp(sw_language)))
		{
			if ((next = nextchar()) != '*') {
				if (isLangCpp(sw_language) && next == '/') {
					while ((c = nextchar()) != '\n' && c != EOF);
					continue;
				}
				return_char(next);
				return c;
			}
			c = nextchar();
			while ((next = nextchar()) != EOF && !(c == '*' && next == '/'))
				c = next;
			continue;
		}

#if !defined(sun) && defined(GPRE_FORTRAN)
		/* skip fortran embedded comments on VMS or hpux or sgi */

		if (c == '!'
			&& (sw_language == lang_fortran)) {
			/* If this character is a '!' followed by a '=', this is an
			   Interbase 'not equal' operator, not a Fortran comment.
			   Bug #307.  mao 6/14/89  */

			if ((c2 = nextchar()) == '=') {
				return_char(c2);
				return c;
			}
			else {
				if ((c = c2) != '\n' && c != EOF)
					while ((c = nextchar()) != '\n' && c != EOF);
				continue;
			}
		}
#endif

		if (c == '-' && (sw_sql || sw_language == lang_ada)) {
			if ((next = nextchar()) != '-') {
				return_char(next);
				return c;
			}
			while ((c = nextchar()) != EOF && c != '\n');
			continue;
		}

		/* skip PASCAL comments - both types */

		if (c == '{' && sw_language == lang_pascal) {
			while ((c = nextchar()) != EOF && c != '}');
			continue;
		}

		if (c == '(' && sw_language == lang_pascal) {
			if ((next = nextchar()) != '*') {
				return_char(next);
				return c;
			}
			c = nextchar();
			while ((next = nextchar()) != EOF && !(c == '*' && next == ')'))
				c = next;
			continue;
		}

		break;
	}

	return c;
}

