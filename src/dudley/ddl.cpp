/*
 *	PROGRAM:	Data Definition Utility
 *	MODULE:		ddl.c
 *	DESCRIPTION:	Main line routine
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

#define DDL_MAIN

#include "../jrd/ib_stdio.h"
#include <stdlib.h>
#include <string.h>
#include "../dudley/ddl.h"
#include "../dudley/parse.h"
#include "../jrd/gds.h"
#include "../jrd/license.h"
#include "../dudley/ddl_proto.h"
#include "../dudley/exe_proto.h"
#include "../dudley/expan_proto.h"
#include "../dudley/extra_proto.h"
#include "../dudley/hsh_proto.h"
#include "../dudley/lex_proto.h"
#include "../dudley/parse_proto.h"
#include "../dudley/trn_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/why_proto.h"

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#if defined(WIN_NT)
#include <io.h>
#endif

const TEXT *DDL_prompt;

static LLS free_stack;
static TEXT DDL_message[256];

#ifndef FOPEN_INPUT_TYPE
#define FOPEN_INPUT_TYPE	"r"
#endif

const char* DDL_EXT		= ".gdl";	// normal extension for a ddl file
const int MAX_ERRORS	= 50;

enum in_sw_values {
	IN_SW_GDEF_0 = 0,		// null switch value
	IN_SW_GDEF_G,			// generate DDL from a database file
	IN_SW_GDEF_R,			// replace existing database
	IN_SW_GDEF_D,			// generate dynamic DDL
	IN_SW_GDEF_Z,			// print version number
	IN_SW_GDEF_T,			// print tokens as they are read
	IN_SW_GDEF_C = 7,		// source is C
	IN_SW_GDEF_F,			// source is FORTRAN
	IN_SW_GDEF_P, 			// source is PASCAL
	IN_SW_GDEF_COB, 		// source is (shudder) cobol
	IN_SW_GDEF_ANSI,		// source is (worse and worse!) ansi format
	IN_SW_GDEF_ADA = 14,	// source is ada
	IN_SW_GDEF_CXX,			// source is C++
	IN_SW_GDEF_USER = 17,	// user name for PC security
	IN_SW_GDEF_PASSWORD		// password for PC security
};

static in_sw_tab_t gdef_in_sw_table[] = {
	{ IN_SW_GDEF_G, 0, "EXTRACT", 0, 0, 0, FALSE, 0, 0,
		"\t\textract definition from database"}, 	/* extract DDL from database */
	{ IN_SW_GDEF_R, 0, "REPLACE", 0, 0, 0, FALSE, 0, 0,
		"\t\treplace existing database"},	/* replace database */
	{ IN_SW_GDEF_D, 0, "DYNAMIC", 0, 0, 0, FALSE, 0, 0,
		"\t\tgenerate dynamic DDL"},
	{ IN_SW_GDEF_T, 0, "T", 0, 0, 0, FALSE, 0, 0, NULL },
	{ IN_SW_GDEF_C, 0, "C", 0, 0, 0, FALSE, 0, 0, "\t\tDYN for C" },
	{ IN_SW_GDEF_F, 0, "FORTRAN", 0, 0, 0, FALSE, 0, 0, "\t\tDYN for FORTRAN" },
	{ IN_SW_GDEF_P, 0, "PASCAL", 0, 0, 0, FALSE, 0, 0, "\t\tDYN for PASCAL" },
	{ IN_SW_GDEF_COB, 0, "COB", 0, 0, 0, FALSE, 0, 0, "\t\tDYN for COBOL" },
	{ IN_SW_GDEF_ANSI, 0, "ANSI", 0, 0, 0, FALSE, 0, 0,
		"\t\tDYN for ANSI COBOL" },
	{ IN_SW_GDEF_ADA, 0, "ADA", 0, 0, 0, FALSE, 0, 0, "\t\tDYN for ADA" },
	{ IN_SW_GDEF_CXX, 0, "CXX", 0, 0, 0, FALSE, 0, 0, "\t\tDYN for C++" },
	{ IN_SW_GDEF_USER, 0, "USER", 0, 0, 0, FALSE, 0, 0,
		"\t\tuser name to use in attaching database" },
	{ IN_SW_GDEF_PASSWORD, 0, "PASSWORD", 0, 0, 0, FALSE, 0, 0,
		"\t\tpassword to use with user name" },
	{ IN_SW_GDEF_Z, 0, "Z", 0, 0, 0, FALSE, 0, 0, "\t\tprint version number" },
	{ IN_SW_GDEF_0, 0, NULL, 0, 0, 0, FALSE, 0, 0, NULL }
};

#ifndef SUPERSERVER
int CLIB_ROUTINE main( int argc, char *argv[])
{
/**************************************
 *
 *	m a i n
 *
 **************************************
 *
 * Functional description
 *	Main line routine for C preprocessor.  Initializes
 *	system, performs pass 1 and pass 2.  Interpretes
 *	command line.
 *
 **************************************/
	IB_FILE *input_file;
	TEXT *p, *q, *string, file_name_1[256], file_name_2[256],
		buffer[256];
	USHORT in_sw;
	IN_SW_TAB in_sw_tab;
	ACT temp, stack;
	FIL file;
#ifdef SERVICE_REDIRECT
	SLONG redir_in;
	SLONG redir_out;
	SLONG redir_err;
#endif

#ifdef VMS
	argc = VMS_parse(&argv, argc);
#endif

/* Perform some special handling when run as an Interbase service.  The
   first switch can be "-svc" (lower case!) or it can be "-svc_re" followed
   by 3 file descriptors to use in re-directing ib_stdin, ib_stdout, and ib_stderr. */

	DDL_service = false;

	if (argc > 1 && !strcmp(argv[1], "-svc")) {
		DDL_service = true;
		argv++;
		argc--;
	}
//
// BRS: 15-Sep-2003
// This code could not be used actually (see SVC_attach, comment by Dmitry)
// Until a more detailed analysis is made it is preserved under an ifdef
//
#ifdef SERVICE_REDIRECT
	else if (argc > 4 && !strcmp(argv[1], "-svc_re")) {
		DDL_service = true;
		redir_in = atol(argv[2]);
		redir_out = atol(argv[3]);
		redir_err = atol(argv[4]);
#ifdef WIN_NT
		redir_in = _open_osfhandle(redir_in, 0);
		redir_out = _open_osfhandle(redir_out, 0);
		redir_err = _open_osfhandle(redir_err, 0);
#endif
		if (redir_in != 0)
			if (dup2((int) redir_in, 0))
				close((int) redir_in);
		if (redir_out != 1)
			if (dup2((int) redir_out, 1))
				close((int) redir_out);
		if (redir_err != 2)
			if (dup2((int) redir_err, 2))
				close((int) redir_err);
		argv += 4;
		argc -= 4;
	}
#endif

	DDL_file_name = NULL;
	DB_file_name = NULL;
	DDL_drop_database = DDL_quit = DDL_extract = DDL_dynamic = DDL_trace =
		DDL_version = false;
	DDL_default_user = DDL_default_password = NULL;

	file_name_1[0] = file_name_2[0] = 0;

	for (--argc; argc; argc--) {
		string = *++argv;
		if ((*string != '-') && (*string != '?')) {
			if (!*file_name_1)
				strcpy(file_name_1, string);
			else
				strcpy(file_name_2, string);
			continue;
		}
		else if (*string == '-') {
			/* iterate through the switch table, looking for matches */

			in_sw = IN_SW_GDEF_0;
			for (in_sw_tab = gdef_in_sw_table; q = in_sw_tab->in_sw_name;
				 in_sw_tab++) {
				p = string + 1;

				/* handle orphaned hyphen case */

				if (!*p--)
					break;

				/* compare switch to switch name in table */

				while (*p) {
					if (!*++p)
						in_sw = in_sw_tab->in_sw;
					if (UPPER(*p) != *q++)
						break;
				}

				/* end of input means we got a match.  stop looking */

				if (!*p)
					break;
			}
		}
		switch (in_sw) {
		case IN_SW_GDEF_D:
			DDL_dynamic = true;
			DYN_file_name[0] = 0;
			if (argc == 1)
				break;
			string = *++argv;
			if (*string != '-') {
				argc--;
				strcpy(DYN_file_name, string);
			}
			else
				argv--;
			break;

		case IN_SW_GDEF_C:
			language = lan_c;
			break;

		case IN_SW_GDEF_P:
			language = lan_pascal;
			break;

		case IN_SW_GDEF_COB:
			language = lan_cobol;
			break;

		case IN_SW_GDEF_ANSI:
			language = lan_ansi_cobol;
			break;
		case IN_SW_GDEF_F:
			language = lan_fortran;
			break;

		case IN_SW_GDEF_ADA:
			language = lan_ada;
			break;

		case IN_SW_GDEF_CXX:
			language = lan_cxx;
			break;

		case IN_SW_GDEF_G:
			DDL_extract = true;
			break;

		case IN_SW_GDEF_R:
			DDL_replace = true;
			break;

		case IN_SW_GDEF_T:
			DDL_trace = true;
			break;

		case IN_SW_GDEF_Z:
			DDL_msg_put(0, GDS_VERSION, 0, 0, 0, 0);	/* msg 0: gdef version %s\n */
			DDL_version = true;
			break;

		case IN_SW_GDEF_PASSWORD:
			if (argc > 1) {
				DDL_default_password = *++argv;
				argc--;
			}
			break;

		case IN_SW_GDEF_USER:
			if (argc > 1) {
				DDL_default_user = *++argv;
				argc--;
			}
			break;

		case IN_SW_GDEF_0:
			if (*string != '?')
				DDL_msg_put(1, string, 0, 0, 0, 0);	/* msg 1: gdef: unknown switch %s */
			DDL_msg_put(2, 0, 0, 0, 0, 0);	/* msg 2: \tlegal switches are: */
			for (in_sw_tab = gdef_in_sw_table; in_sw_tab->in_sw; in_sw_tab++)
				if (in_sw_tab->in_sw_text)
					DDL_msg_put(3, in_sw_tab->in_sw_name,
								in_sw_tab->in_sw_text, 0, 0, 0);	/* msg 3: %s%s */
			DDL_exit(FINI_ERROR);
		}
	}

	if (DDL_extract) {
		strcpy(DB_file_string, file_name_1);
		strcpy(DDL_file_string, file_name_2);
		if (!*DB_file_string) {
			DDL_msg_put(4, 0, 0, 0, 0, 0);	/* msg 4: gdef: Database name is required for extract */
			DDL_exit(FINI_ERROR);
		}
		DB_file_name = DB_file_string;
		DDL_file_name = DDL_file_string;
		DDL_ext();
		DDL_exit(FINI_OK);
	}
	else if (*file_name_1) {
		strcpy(DDL_file_string, file_name_1);
		DDL_file_name = DDL_file_string;
	}
	if (DDL_file_name == NULL) {
		DDL_file_name = "standard input";
		input_file = ib_stdin;
		DDL_interactive = DDL_service || isatty(0);
	}
	else {
		/* 
		   * try to open the input DDL file.  
		   * If it already has a .GDL extension, just try to open it.
		   * Otherwise, add the extension, try, remove the extension,
		   * and try again.
		 */

		input_file = NULL;

		/* first find the extension by going to the end and backing up */

		for (p = DDL_file_name; *p; p++);
		while ((p != DDL_file_name) && (*p != '.') && (*p != '/'))
			p--;

		/* then handle the case where the input already ends in .GDL */

		if (*p == '.') {
			for (q = const_cast<TEXT*>(DDL_EXT); UPPER(*p) == UPPER(*q); p++, q++)
				if (!*p) {
					input_file = ib_fopen(DDL_file_name, FOPEN_INPUT_TYPE);
					if (!input_file) {
						DDL_msg_put(5, DDL_file_name, 0, 0, 0, 0);	/* msg 5: gdef: can't open %s */
						DDL_exit(FINI_ERROR);
					}
				}
		}

		/* if we got this far without opening it, it's time to add the new extension */

		if (!input_file) {
			sprintf(file_name_1, "%s%s", DDL_file_name, DDL_EXT);
			input_file = ib_fopen(file_name_1, FOPEN_INPUT_TYPE);
			if (input_file)
				DDL_file_name = file_name_1;
			else {
				input_file = ib_fopen(DDL_file_name, FOPEN_INPUT_TYPE);
				if (!input_file)
					DDL_msg_put(6, DDL_file_name, file_name_1, 0, 0, 0);
				/* msg 6: gdef: can't open %s or %s */
			}
		}
		if (!input_file)
			DDL_exit(FINI_ERROR);
	}

	LEX_init(input_file);
	HSH_init();
	PARSE_actions();

	if (input_file != ib_stdin)
		ib_fclose(input_file);

	if (DDL_actions && ((DDL_errors && DDL_interactive) || DDL_quit)) {
		ib_rewind(ib_stdin);
		*buffer = 0;
		if (DDL_errors > 1)
			DDL_msg_partial(7, (TEXT *) (ULONG) DDL_errors, 0, 0, 0, 0);	/* msg 7: \n%d errors during input. */
		else if (DDL_errors)
			DDL_msg_partial(9, 0, 0, 0, 0, 0);	/* msg 9: \n1 error during input. */
		else
			DDL_msg_partial(8, 0, 0, 0, 0, 0);	/* msg 8: \nNo errors. */
		if (DDL_yes_no(10)){ // msg 10 : save changes before exiting?
			DDL_quit = false;
			DDL_errors = 0;
		}
	}

/* Reverse the set of actions */

	stack = NULL;
	while (DDL_actions) {
		temp = DDL_actions;
		DDL_actions = temp->act_next;
		temp->act_next = stack;
		stack = temp;
	}
	DDL_actions = stack;

	if (!DDL_errors && !DDL_quit) {
		EXP_actions();
		if (!DDL_errors && !DDL_quit) {
			EXE_execute();
			if (DDL_dynamic)
				TRN_translate();
		}
	}

	if (DDL_actions && (DDL_errors || DDL_quit))
		if (DDL_errors)
			DDL_msg_put(307, 0, 0, 0, 0, 0);	/* msg 307: Ceasing processing because of errors. */
		else
			DDL_msg_put(308, 0, 0, 0, 0, 0);	/* msg 308: Ceasing processing. */

	EXE_fini(database);

	if (DDL_errors) {
		if (database && (database->dbb_flags & DBB_create_database)) {
			for (file = database->dbb_files; file; file = file->fil_next)
				unlink(file->fil_name->sym_name);
			unlink(database->dbb_name->sym_string);
		}
		DDL_exit(FINI_ERROR);
	}

	DDL_exit(FINI_OK);
	// This will never execute, see exit() in DDL_exit. Make the compiler happy.
	return 0;
}
#endif

UCHAR *DDL_alloc(int size)
{
/**************************************
 *
 *	D D L _ a l l o c
 *
 **************************************
 *
 * Functional description
 *
 **************************************/
	UCHAR *block, *p;

	p = block = (UCHAR*) gds__alloc((SLONG) size);

#ifdef DEBUG_GDS_ALLOC
/* For V4.0 we don't care about gdef specific memory leaks */
	gds_alloc_flag_unfreed(block);
#endif

	if (!p)
		DDL_err(14, 0, 0, 0, 0, 0);	/* msg 14: memory exhausted */
	else
		do
			*p++ = 0;
		while (--size);

	return block;
}


int DDL_db_error(
				 ISC_STATUS * status_vector,
				 USHORT number,
				 TEXT * arg1,
				 TEXT * arg2, TEXT * arg3, TEXT * arg4, TEXT * arg5)
{
/**************************************
 *
 *	D D L _ d b _ e r r o r
 *
 **************************************
 *
 * Functional description
 *	Issue an error message.
 *
 **************************************/

	gds__print_status(status_vector);

	return DDL_err(number, arg1, arg2, arg3, arg4, arg5);
}


int DDL_err(
			USHORT number,
			TEXT * arg1, TEXT * arg2, TEXT * arg3, TEXT * arg4, TEXT * arg5)
{
/**************************************
 *
 *	D D L _ e r r
 *
 **************************************
 *
 * Functional description
 *	Issue an error message. Quit if >MAX_ERRORS
 *
 **************************************/

	DDL_msg_partial(15, DDL_file_name, (TEXT *) (ULONG) DDL_line, 0, 0, 0);	/*msg 15: %s:%d: */
	DDL_msg_put(number, arg1, arg2, arg3, arg4, arg5);
	if (DDL_errors++ > MAX_ERRORS) {
		DDL_msg_put(16, (TEXT *) (SLONG) MAX_ERRORS, 0, 0, 0, 0);	/* msg 16: error count exceeds limit (%d) */
		DDL_msg_put(17, 0, 0, 0, 0, 0);	/* msg 17: what we have here is a failure to communicate! */
		if (database && (database->dbb_flags & DBB_create_database))
			unlink(DB_file_name);
		DDL_exit(FINI_ERROR);
	}

	return 0;
}


void DDL_error_abort(
					 ISC_STATUS * status_vector,
					 USHORT number,
					 TEXT * arg1,
					 TEXT * arg2, TEXT * arg3, TEXT * arg4, TEXT * arg5)
{
/**************************************
 *
 *	D D L _ e r r o r _ a b o r t
 *
 **************************************
 *
 * Functional description
 *	Things are going very poorly, so put out an error message
 *	and give up.
 *
 **************************************/

	if (status_vector)
		gds__print_status(status_vector);

	DDL_err(number, arg1, arg2, arg3, arg4, arg5);
	DDL_exit(FINI_ERROR);
}


void DDL_exit( int stat)
{
/**************************************
 *
 *	D D L _ e x i t
 *
 **************************************
 *
 * Functional description
 *	Exit with status.
 *
 **************************************/

	LEX_fini();
	exit(stat);
}


void DDL_msg_partial(
					 USHORT number,
					 TEXT * arg1,
					 TEXT * arg2, TEXT * arg3, TEXT * arg4, TEXT * arg5)
{
/**************************************
 *
 *	D D L _ m s g _ p a r t i a l
 *
 **************************************
 *
 * Functional description
 *	Retrieve a message from the error file, format it, and print it
 *	without a newline.
 *
 **************************************/

	gds__msg_format(0, DDL_MSG_FAC, number, sizeof(DDL_message), DDL_message,
					arg1, arg2, arg3, arg4, arg5);
	ib_printf("%s", DDL_message);
}


void DDL_msg_put(
				 USHORT number,
				 TEXT * arg1,
				 TEXT * arg2, TEXT * arg3, TEXT * arg4, TEXT * arg5)
{
/**************************************
 *
 *	D D L _ m s g _ p u t
 *
 **************************************
 *
 * Functional description
 *	Retrieve a message from the error file, format it, and print it.
 *
 **************************************/

	gds__msg_format(0, DDL_MSG_FAC, number, sizeof(DDL_message), DDL_message,
					arg1, arg2, arg3, arg4, arg5);
	ib_printf("%s\n", DDL_message);
}


DUDLEY_NOD DDL_pop(LLS * pointer)
{
/**************************************
 *
 *	D D L _ p o p
 *
 **************************************
 *
 * Functional description
 *	Pop an item off a linked list stack.  Free the stack node.
 *
 **************************************/
	LLS stack;
	DUDLEY_NOD node;

	stack = *pointer;
	node = stack->lls_object;
	*pointer = stack->lls_next;
	stack->lls_next = free_stack;
	free_stack = stack;

	return node;
}


void DDL_push( DUDLEY_NOD object, LLS * pointer)
{
/**************************************
 *
 *	D D L _ p u s h
 *
 **************************************
 *
 * Functional description
 *	Push an arbitrary object onto a linked list stack.
 *
 **************************************/
	LLS stack;

	if (free_stack) {
		stack = free_stack;
		free_stack = stack->lls_next;
	}
	else
		stack = (LLS) DDL_alloc(sizeof(lls));

	stack->lls_object = object;
	stack->lls_next = *pointer;
	*pointer = stack;
}


bool DDL_yes_no( USHORT number)
{
/**************************************
 *
 *	D D L _ y e s _ n o
 *
 **************************************
 *
 * Functional description
 *	Ask a yes/no question.
 *
 **************************************/
	int c, d;
	USHORT count, yes_num, no_num, re_num;
	TEXT prompt[128], reprompt[128], yes_ans[128], no_ans[128];

	gds__msg_format(0, DDL_MSG_FAC, number, sizeof(prompt), prompt, NULL,
					NULL, NULL, NULL, NULL);

	yes_num = 342;				/* Msg342 YES   */
	no_num = 343;				/* Msg343 NO    */
	re_num = 344;				/* Msg344 Please respond with YES or NO. */
	reprompt[0] = '\0';

	if (gds__msg_format
		(0, DDL_MSG_FAC, no_num, sizeof(no_ans), no_ans, NULL, NULL, NULL,
		 NULL, NULL) <= 0)
		strcpy(no_ans, "NO");	/* default if msg_format fails */
	if (gds__msg_format
		(0, DDL_MSG_FAC, yes_num, sizeof(yes_ans), yes_ans, NULL, NULL, NULL,
		 NULL, NULL) <= 0)
		strcpy(yes_ans, "YES");

	for (;;) {
		ib_printf(prompt);
		if (DDL_service)
			ib_putc('\001', ib_stdout);
		ib_fflush(ib_stdout);
		count = 0;
		while ((c = ib_getc(ib_stdin)) == ' ')
			count++;
		if (c != '\n' && c != EOF)
			while ((d = ib_getc(ib_stdin)) != '\n' && d != EOF);
		if (!count && c == EOF)
			return false;
		if (UPPER(c) == UPPER(yes_ans[0]))
			return true;
		if (UPPER(c) == UPPER(no_ans[0]))
			return false;
		if (!reprompt
			&& gds__msg_format(0, DDL_MSG_FAC, re_num, sizeof(reprompt),
							   reprompt, NULL, NULL, NULL, NULL, NULL) <= 0)
			sprintf(reprompt, "Please respond with YES or NO.");	/* default if msg_format fails */
		ib_printf("%s\n", reprompt);
	}
}
