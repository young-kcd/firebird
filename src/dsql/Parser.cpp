/*
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../dsql/Parser.h"
#include "../jrd/jrd.h"

using namespace Jrd;


Parser::Parser(MemoryPool& pool, USHORT aClientDialect, USHORT aDbDialect, USHORT aParserVersion,
			const TEXT* string, USHORT length, SSHORT characterSet)
	: PermanentStorage(pool),
	  client_dialect(aClientDialect),
	  db_dialect(aDbDialect),
	  parser_version(aParserVersion),
	  stmt_ambiguous(false)
{
	yyps = 0;
	yypath = 0;
	yylvals = 0;
	yylvp = 0;
	yylve = 0;
	yylvlim = 0;
	yylpsns = 0;
	yylpp = 0;
	yylpe = 0;
	yylplim = 0;
	yylexp = 0;
	yylexemes = 0;

	lex.line_start = lex.ptr = string;
	lex.end = string + length;
	lex.lines = 1;
	lex.att_charset = characterSet;
	lex.line_start_bk = lex.line_start;
	lex.lines_bk = lex.lines;
	lex.param_number = 1;
	lex.prev_keyword = -1;
#ifdef DSQL_DEBUG
	if (DSQL_debug & 32)
		dsql_trace("Source DSQL string:\n%.*s", (int) length, string);
#endif
}


Parser::~Parser()
{
	while (yyps)
	{
		yyparsestate* p = yyps;
		yyps = p->save;
		yyFreeState(p);
	}

	while (yypath)
	{
		yyparsestate* p = yypath;
		yypath = p->save;
		yyFreeState(p);
	}

	delete[] yylvals;
	delete[] yylpsns;
	delete[] yylexemes;
}


Parser::YYSTYPE Parser::parse()
{
	if (parseAux() != 0)
		return NULL;
	else
		return DSQL_parse;
}

