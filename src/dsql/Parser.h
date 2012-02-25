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

#ifndef DSQL_PARSER_H
#define DSQL_PARSER_H

#include "../common/common.h"
#include "../dsql/dsql.h"
#include "../dsql/node.h"
#include "../dsql/DdlNodes.h"
#include "../dsql/BoolNodes.h"
#include "../dsql/ExprNodes.h"
#include "../dsql/AggNodes.h"
#include "../dsql/WinNodes.h"
#include "../dsql/PackageNodes.h"
#include "../dsql/StmtNodes.h"
#include "../jrd/RecordSourceNodes.h"
#include "../common/classes/Nullable.h"
#include "../common/classes/stack.h"

#define _yacc_defines_keywords
#include "gen/parse.h"

namespace Jrd {

class dsql_nod;

class Parser : public Firebird::PermanentStorage
{
private:
	// User-defined text position type.
	struct Position
	{
		USHORT firstLine;
		USHORT firstColumn;
		USHORT lastLine;
		USHORT lastColumn;
		const char* firstPos;
		const char* lastPos;
	};

	typedef Position YYPOSN;
	typedef int Yshort;

	struct yyparsestate
	{
	  yyparsestate* save;		// Previously saved parser state
	  int state;
	  int errflag;
	  Yshort* ssp;				// state stack pointer
	  YYSTYPE* vsp;				// value stack pointer
	  YYPOSN* psp;				// position stack pointer
	  YYSTYPE val;				// value as returned by actions
	  YYPOSN pos;				// position as returned by universal action
	  Yshort* ss;				// state stack base
	  YYSTYPE* vs;				// values stack base
	  YYPOSN* ps;				// position stack base
	  int lexeme;				// index of the conflict lexeme in the lexical queue
	  unsigned int stacksize;	// current maximum stack size
	  Yshort ctry;				// index in yyctable[] for this conflict
	};

	struct LexerState
	{
		// This is, in fact, parser state. Not used in lexer itself
		dsql_fld* g_field;
		dsql_nod* g_field_name;
		int dsql_debug;

		// Actual lexer state begins from here

		const TEXT* ptr;
		const TEXT* end;
		const TEXT* last_token;
		const TEXT* start;
		const TEXT* line_start;
		const TEXT* last_token_bk;
		const TEXT* line_start_bk;
		SSHORT lines, att_charset;
		SSHORT lines_bk;
		int prev_keyword;
		USHORT param_number;
	};

	struct StrMark
	{
		StrMark()
			: introduced(false),
			  pos(0),
			  length(0),
			  str(NULL)
		{
		}

		bool operator >(const StrMark& o) const
		{
			return pos > o.pos;
		}

		bool introduced;
		unsigned pos;
		unsigned length;
		dsql_str* str;
	};

public:
	Parser(MemoryPool& pool, USHORT aClientDialect, USHORT aDbDialect, USHORT aParserVersion,
		const TEXT* string, size_t length, SSHORT characterSet);
	~Parser();

public:
	dsql_req* parse();

	const Firebird::string& getTransformedString() const
	{
		return transformedString;
	}

	bool isStmtAmbiguous() const
	{
		return stmt_ambiguous;
	}

	// newNode overloads

	template <typename T>
	T* newNode()
	{
		return FB_NEW(getPool()) T(getPool());
	}

	template <typename T, typename T1>
	T* newNode(T1 a1)
	{
		return FB_NEW(getPool()) T(getPool(), a1);
	}

	template <typename T, typename T1, typename T2>
	T* newNode(T1 a1, T2 a2)
	{
		return FB_NEW(getPool()) T(getPool(), a1, a2);
	}

	template <typename T, typename T1, typename T2, typename T3>
	T* newNode(T1 a1, T2 a2, T3 a3)
	{
		return FB_NEW(getPool()) T(getPool(), a1, a2, a3);
	}

	template <typename T, typename T1, typename T2, typename T3, typename T4>
	T* newNode(T1 a1, T2 a2, T3 a3, T4 a4)
	{
		return FB_NEW(getPool()) T(getPool(), a1, a2, a3, a4);
	}

	template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
	T* newNode(T1 a1, T2 a2, T3 a3, T4 a4, T5 a5)
	{
		return FB_NEW(getPool()) T(getPool(), a1, a2, a3, a4, a5);
	}

private:
	void transformString(const char* start, unsigned length, Firebird::string& dest);

	// Set the value of a clause, checking if it was already specified.

	template <typename T>
	void setClause(T& clause, const char* duplicateMsg, const T& value)
	{
		using namespace Firebird;
		if (isDuplicateClause(clause))
		{
			status_exception::raise(
				Arg::Gds(isc_sqlerr) << Arg::Num(-637) <<
				Arg::Gds(isc_dsql_duplicate_spec) << duplicateMsg);
		}

		clause = value;
	}

	template <typename T, typename Delete>
	void setClause(Firebird::AutoPtr<T, Delete>& clause, const char* duplicateMsg, T* value)
	{
		using namespace Firebird;
		if (isDuplicateClause(clause))
		{
			status_exception::raise(
				Arg::Gds(isc_sqlerr) << Arg::Num(-637) <<
				Arg::Gds(isc_dsql_duplicate_spec) << duplicateMsg);
		}

		clause = value;
	}

	template <typename T>
	void setClause(Nullable<T>& clause, const char* duplicateMsg, const T& value)
	{
		using namespace Firebird;
		if (clause.specified)
		{
			status_exception::raise(
				Arg::Gds(isc_sqlerr) << Arg::Num(-637) <<
				Arg::Gds(isc_dsql_duplicate_spec) << duplicateMsg);
		}

		clause = value;
	}

	void setClause(bool& clause, const char* duplicateMsg)
	{
		setClause(clause, duplicateMsg, true);
	}

	template <typename T>
	bool isDuplicateClause(const T& clause)
	{
		return clause != 0;
	}

	bool isDuplicateClause(const Firebird::MetaName& clause)
	{
		return clause.hasData();
	}

// start - defined in btyacc_fb.ske
private:
	static void yySCopy(YYSTYPE* to, YYSTYPE* from, int size);
	static void yyPCopy(YYPOSN* to, YYPOSN* from, int size);
	static void yyMoreStack(yyparsestate* yyps);
	static yyparsestate* yyNewState(int size);
	static void yyFreeState(yyparsestate* p);

private:
	int parseAux();
	int yylex1();
	int yyexpand();
// end - defined in btyacc_fb.ske

// start - defined in parse.y
private:
	void yyReducePosn(YYPOSN& ret, YYPOSN* termPosns, YYSTYPE* termVals,
		int termNo, int stkPos, int yychar, YYPOSN& yyposn, void*);

	int yylex();
	int yylexAux();

	void yyerror(const TEXT* error_string);
	void yyerror_detailed(const TEXT* error_string, int yychar, YYSTYPE&, YYPOSN&);

	dsql_str* makeParseStr(const Position& p1, const Position& p2);
	dsql_nod* make_list (dsql_nod* node);
	ParameterNode* make_parameter();
	dsql_nod* make_node(Dsql::nod_t type, int count, ...);
	dsql_nod* makeClassNode(ExprNode* node);
	dsql_nod* make_flag_node(Dsql::nod_t type, SSHORT flag, int count, ...);
// end - defined in parse.y

private:
	Firebird::string compilingText;
	USHORT client_dialect;
	USHORT db_dialect;
	USHORT parser_version;

	Firebird::string transformedString;
	Firebird::GenericMap<Firebird::NonPooled<dsql_str*, StrMark> > strMarks;
	bool stmt_ambiguous;
	dsql_req* DSQL_parse;

	// These value/posn are taken from the lexer
	YYSTYPE yylval;
	YYPOSN yyposn;

	// These value/posn of the root non-terminal are returned to the caller
	YYSTYPE yyretlval;
	Position yyretposn;

	int yynerrs;

	// Current parser state
	yyparsestate* yyps;
	// yypath!=NULL: do the full parse, starting at *yypath parser state.
	yyparsestate* yypath;
	// Base of the lexical value queue
	YYSTYPE* yylvals;
	// Current posistion at lexical value queue
	YYSTYPE* yylvp;
	// End position of lexical value queue
	YYSTYPE* yylve;
	// The last allocated position at the lexical value queue
	YYSTYPE* yylvlim;
	// Base of the lexical position queue
	Position* yylpsns;
	// Current posistion at lexical position queue
	Position* yylpp;
	// End position of lexical position queue
	Position* yylpe;
	// The last allocated position at the lexical position queue
	Position* yylplim;
	// Current position at lexical token queue
	Yshort* yylexp;
	Yshort* yylexemes;

public:
	LexerState lex;
};

} // namespace

#endif	// DSQL_PARSER_H
