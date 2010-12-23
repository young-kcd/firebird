%{
/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		parse.y
 *	DESCRIPTION:	Dynamic SQL parser
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
 *
 * 2002-02-24 Sean Leyne - Code Cleanup of old Win 3.1 port (WINDOWS_ONLY)
 * 2001.05.20 Neil McCalden: Allow a udf to be used in a 'group by' clause.
 * 2001.05.30 Claudio Valderrama: DROP TABLE and DROP VIEW lead now to two
 *   different node types so DDL can tell which is which.
 * 2001.06.13 Claudio Valderrama: SUBSTRING is being surfaced.
 * 2001.06.30 Claudio valderrama: Feed (line,column) for each node. See node.h.
 * 2001.07.10 Claudio Valderrama: Better (line,column) report and "--" for comments.
 * 2001.07.28 John Bellardo: Changes to support parsing LIMIT and FIRST
 * 2001.08.03 John Bellardo: Finalized syntax for LIMIT, change LIMIT to SKIP
 * 2001.08.05 Claudio Valderrama: closed Bug #448062 and other spaces that appear
 *   in rdb$*_source fields when altering domains plus one unexpected null pointer.
 * 2001.08.12 Claudio Valderrama: adjust SUBSTRING's starting pos argument here
 *   and not in gen.c; this closes Bug #450301.
 * 2001.10.01 Claudio Valderrama: enable explicit GRANT...to ROLE role_name.
 * 2001.10.06 Claudio Valderrama: Honor explicit USER keyword in GRANTs and REVOKEs.
 * 2002.07.05 Mark O'Donohue: change keyword DEBUG to KW_DEBUG to avoid
 *			clashes with normal DEBUG macro.
 * 2002.07.30 Arno Brinkman:
 * 2002.07.30 	Let IN predicate handle value_expressions
 * 2002.07.30 	tokens CASE, NULLIF, COALESCE added
 * 2002.07.30 	See block < CASE expression > what is added to value as case_expression
 * 2002.07.30 	function is split up into aggregate_function, numeric_value_function, string_value_function, generate_value_function
 * 2002.07.30 	new group_by_function and added to grp_column_elem
 * 2002.07.30 	cast removed from function and added as cast_specification to value
 * 2002.08.04 Claudio Valderrama: allow declaring and defining variables at the same time
 * 2002.08.04 Dmitry Yemanov: ALTER VIEW
 * 2002.08.06 Arno Brinkman: ordinal added to grp_column_elem for using positions in group by
 * 2002.08.07 Dmitry Yemanov: INT64/LARGEINT are replaced with BIGINT and available in dialect 3 only
 * 2002.08.31 Dmitry Yemanov: allowed user-defined index names for PK/FK/UK constraints
 * 2002.09.01 Dmitry Yemanov: RECREATE VIEW
 * 2002.09.28 Dmitry Yemanov: Reworked internal_info stuff, enhanced
 *							exception handling in SPs/triggers,
 *							implemented ROWS_AFFECTED system variable
 * 2002.10.21 Nickolay Samofatov: Added support for explicit pessimistic locks
 * 2002.10.29 Nickolay Samofatov: Added support for savepoints
 * 2002.12.03 Dmitry Yemanov: Implemented ORDER BY clause in subqueries.
 * 2002.12.18 Dmitry Yemanov: Added support for SQL-compliant labels and LEAVE statement
 * 2002.12.28 Dmitry Yemanov: Added support for parametrized events.
 * 2003.01.14 Dmitry Yemanov: Fixed bug with cursors in triggers.
 * 2003.01.15 Dmitry Yemanov: Added support for runtime trigger action checks.
 * 2003.02.10 Mike Nordell  : Undefined Microsoft introduced macros to get a clean compile.
 * 2003.05.24 Nickolay Samofatov: Make SKIP and FIRST non-reserved keywords
 * 2003.06.13 Nickolay Samofatov: Make INSERTING/UPDATING/DELETING non-reserved keywords
 * 2003.07.01 Blas Rodriguez Somoza: Change DEBUG and IN to avoid conflicts in win32 build/bison
 * 2003.08.11 Arno Brinkman: Changed GROUP BY to support all expressions and added "AS" support
 *						   with table alias. Also removed group_by_function and ordinal.
 * 2003.08.14 Arno Brinkman: Added support for derived tables.
 * 2003.10.05 Dmitry Yemanov: Added support for explicit cursors in PSQL.
 * 2004.01.16 Vlad Horsun: added support for default parameters and
 *   EXECUTE BLOCK statement
 * Adriano dos Santos Fernandes
 */

#include "firebird.h"
#include "dyn_consts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/common.h"
#include <stdarg.h>

#include "gen/iberror.h"
#include "../dsql/dsql.h"
#include "../dsql/node.h"
#include "../jrd/ibase.h"
#include "../jrd/flags.h"
#include "../jrd/jrd.h"
#include "../dsql/errd_proto.h"
#include "../dsql/hsh_proto.h"
#include "../dsql/make_proto.h"
#include "../yvalve/keywords.h"
#include "../yvalve/gds_proto.h"
#include "../jrd/err_proto.h"
#include "../common/intlobj_new.h"
#include "../jrd/Attachment.h"
#include "../common/StatusArg.h"

// since UNIX isn't standard, we have to define
// stuff which is in <limits.h> (which isn't available on all UNIXes...

const long SHRT_POS_MAX			= 32767;
const long SHRT_UNSIGNED_MAX	= 65535;
const long SHRT_NEG_MAX			= 32768;
const long LONG_POS_MAX			= 2147483647;
const int POSITIVE	= 0;
const int NEGATIVE	= 1;
const int UNSIGNED	= 2;

//const int MIN_CACHE_BUFFERS	= 250;
//const int DEF_CACHE_BUFFERS	= 1000;

#define YYSTYPE YYSTYPE
#if defined(DEBUG) || defined(DEV_BUILD)
#define YYDEBUG		1
#endif

static const char INTERNAL_FIELD_NAME[] = "DSQL internal"; // NTX: placeholder

inline unsigned trigger_type_suffix(const unsigned slot1, const unsigned slot2, const unsigned slot3)
{
	return ((slot1 << 1) | (slot2 << 3) | (slot3 << 5));
}


#include "../dsql/chars.h"

const int MAX_TOKEN_LEN = 256;

using namespace Jrd;
using namespace Dsql;
using namespace Firebird;

#ifdef NOT_USED_OR_REPLACED
static bool		long_int(dsql_nod*, SLONG*);
#endif
static dsql_fld*	make_field (dsql_nod*);
static dsql_fil*	make_file();
#ifdef NOT_USED_OR_REPLACED
static bool	short_int(dsql_nod*, SLONG*, SSHORT);
#endif
static void	stack_nodes (dsql_nod*, DsqlNodStack&);
static Firebird::MetaName toName(dsql_nod* node);
static Firebird::string toString(dsql_str* node);

static void	yyabandon (SLONG, ISC_STATUS);

inline void check_bound(const char* const to, const char* const string)
{
	if ((to - string) >= MAX_TOKEN_LEN)
		yyabandon (-104, isc_token_too_long);
}

inline void check_copy_incr(char*& to, const char ch, const char* const string)
{
	check_bound(to, string);
	*to++ = ch;
}

%}


// token declarations

// Tokens are organized chronologically by date added.
// See yvalve/keywords.cpp for a list organized alphabetically

// Tokens in v4.0 -- not separated into v3 and v4 tokens

%token <legacyNode> ACTIVE
%token <legacyNode> ADD
%token <legacyNode> AFTER
%token <legacyNode> ALL
%token <legacyNode> ALTER
%token <legacyNode> AND
%token <legacyNode> ANY
%token <legacyNode> AS
%token <legacyNode> ASC
%token <legacyNode> AT
%token <legacyNode> AVG
%token <legacyNode> AUTO
%token <legacyNode> BEFORE
%token <legacyNode> BEGIN
%token <legacyNode> BETWEEN
%token <legacyNode> BLOB
%token <legacyNode> BY
%token <legacyNode> CAST
%token <legacyNode> CHARACTER
%token <legacyNode> CHECK
%token <legacyNode> COLLATE
%token <legacyNode> COMMA
%token <legacyNode> COMMIT
%token <legacyNode> COMMITTED
%token <legacyNode> COMPUTED
%token <legacyNode> CONCATENATE
%token <legacyNode> CONDITIONAL
%token <legacyNode> CONSTRAINT
%token <legacyNode> CONTAINING
%token <legacyNode> COUNT
%token <legacyNode> CREATE
%token <legacyNode> CSTRING
%token <legacyNode> CURRENT
%token <legacyNode> CURSOR
%token <legacyNode> DATABASE
%token <legacyNode> DATE
%token <legacyNode> DB_KEY
%token <legacyNode> DECIMAL
%token <legacyNode> DECLARE
%token <legacyNode> DEFAULT
%token <legacyNode> KW_DELETE
%token <legacyNode> DESC
%token <legacyNode> DISTINCT
%token <legacyNode> DO
%token <legacyNode> DOMAIN
%token <legacyNode> DROP
%token <legacyNode> ELSE
%token <legacyNode> END
%token <legacyNode> ENTRY_POINT
%token <legacyNode> EQL
%token <legacyNode> ESCAPE
%token <legacyNode> EXCEPTION
%token <legacyNode> EXECUTE
%token <legacyNode> EXISTS
%token <legacyNode> EXIT
%token <legacyNode> EXTERNAL
%token <legacyNode> FILTER
%token <legacyNode> FOR
%token <legacyNode> FOREIGN
%token <legacyNode> FROM
%token <legacyNode> FULL
%token <legacyNode> FUNCTION
%token <legacyNode> GDSCODE
%token <legacyNode> GEQ
%token <legacyNode> GENERATOR
%token <legacyNode> GEN_ID
%token <legacyNode> GRANT
%token <legacyNode> GROUP
%token <legacyNode> GTR
%token <legacyNode> HAVING
%token <legacyNode> IF
%token <legacyNode> KW_IN
%token <legacyNode> INACTIVE
%token <legacyNode> INNER
%token <legacyNode> INPUT_TYPE
%token <legacyNode> INDEX
%token <legacyNode> INSERT
%token <legacyNode> INTEGER
%token <legacyNode> INTO
%token <legacyNode> IS
%token <legacyNode> ISOLATION
%token <legacyNode> JOIN
%token <legacyNode> KEY
%token <legacyNode> KW_CHAR
%token <legacyNode> KW_DEC
%token <legacyNode> KW_DOUBLE
%token <legacyNode> KW_FILE
%token <legacyNode> KW_FLOAT
%token <legacyNode> KW_INT
%token <legacyNode> KW_LONG
%token <legacyNode> KW_NULL
%token <legacyNode> KW_NUMERIC
%token <legacyNode> KW_UPPER
%token <legacyNode> KW_VALUE
%token <legacyNode> LENGTH
%token <legacyNode> LPAREN
%token <legacyNode> LEFT
%token <legacyNode> LEQ
%token <legacyNode> LEVEL
%token <legacyNode> LIKE
%token <legacyNode> LSS
%token <legacyNode> MANUAL
%token <legacyNode> MAXIMUM
%token <legacyNode> MAX_SEGMENT
%token <legacyNode> MERGE
%token <legacyNode> MINIMUM
%token <legacyNode> MODULE_NAME
%token <legacyNode> NAMES
%token <legacyNode> NATIONAL
%token <legacyNode> NATURAL
%token <legacyNode> NCHAR
%token <legacyNode> NEQ
%token <legacyNode> NO
%token <legacyNode> NOT
%token <legacyNode> NOT_GTR
%token <legacyNode> NOT_LSS
%token <legacyNode> OF
%token <legacyNode> ON
%token <legacyNode> ONLY
%token <legacyNode> OPTION
%token <legacyNode> OR
%token <legacyNode> ORDER
%token <legacyNode> OUTER
%token <legacyNode> OUTPUT_TYPE
%token <legacyNode> OVERFLOW
%token <legacyNode> PAGE
%token <legacyNode> PAGES
%token <legacyNode> KW_PAGE_SIZE
%token <legacyNode> PARAMETER
%token <legacyNode> PASSWORD
%token <legacyNode> PLAN
%token <legacyNode> POSITION
%token <legacyNode> POST_EVENT
%token <legacyNode> PRECISION
%token <legacyNode> PRIMARY
%token <legacyNode> PRIVILEGES
%token <legacyNode> PROCEDURE
%token <legacyNode> PROTECTED
%token <legacyNode> READ
%token <legacyNode> REAL
%token <legacyNode> REFERENCES
%token <legacyNode> RESERVING
%token <legacyNode> RETAIN
%token <legacyNode> RETURNING_VALUES
%token <legacyNode> RETURNS
%token <legacyNode> REVOKE
%token <legacyNode> RIGHT
%token <legacyNode> RPAREN
%token <legacyNode> ROLLBACK
%token <legacyNode> SEGMENT
%token <legacyNode> SELECT
%token <legacyNode> SET
%token <legacyNode> SHADOW
%token <legacyNode> KW_SHARED
%token <legacyNode> SINGULAR
%token <legacyNode> KW_SIZE
%token <legacyNode> SMALLINT
%token <legacyNode> SNAPSHOT
%token <legacyNode> SOME
%token <legacyNode> SORT
%token <legacyNode> SQLCODE
%token <legacyNode> STABILITY
%token <legacyNode> STARTING
%token <legacyNode> STATISTICS
%token <legacyNode> SUB_TYPE
%token <legacyNode> SUSPEND
%token <legacyNode> SUM
%token <legacyNode> TABLE
%token <legacyNode> THEN
%token <legacyNode> TO
%token <legacyNode> TRANSACTION
%token <legacyNode> TRIGGER
%token <legacyNode> UNCOMMITTED
%token <legacyNode> UNION
%token <legacyNode> UNIQUE
%token <legacyNode> UPDATE
%token <legacyNode> USER
%token <legacyNode> VALUES
%token <legacyNode> VARCHAR
%token <legacyNode> VARIABLE
%token <legacyNode> VARYING
%token <legacyNode> VERSION
%token <legacyNode> VIEW
%token <legacyNode> WAIT
%token <legacyNode> WHEN
%token <legacyNode> WHERE
%token <legacyNode> WHILE
%token <legacyNode> WITH
%token <legacyNode> WORK
%token <legacyNode> WRITE

%token <legacyNode> FLOAT_NUMBER NUMERIC SYMBOL
%token <int32Val> NUMBER

%token <legacyStr>	STRING
%token <textPtr>	INTRODUCER

// New tokens added v5.0

%token <legacyNode> ACTION
%token <legacyNode> ADMIN
%token <legacyNode> CASCADE
%token <legacyNode> FREE_IT			// ISC SQL extension
%token <legacyNode> RESTRICT
%token <legacyNode> ROLE

// New tokens added v6.0

%token <legacyNode> COLUMN
%token <legacyNode> KW_TYPE
%token <legacyNode> EXTRACT
%token <legacyNode> YEAR
%token <legacyNode> MONTH
%token <legacyNode> DAY
%token <legacyNode> HOUR
%token <legacyNode> MINUTE
%token <legacyNode> SECOND
%token <legacyNode> WEEKDAY			// ISC SQL extension
%token <legacyNode> YEARDAY			// ISC SQL extension
%token <legacyNode> TIME
%token <legacyNode> TIMESTAMP
%token <legacyNode> CURRENT_DATE
%token <legacyNode> CURRENT_TIME
%token <legacyNode> CURRENT_TIMESTAMP

// special aggregate token types returned by lex in v6.0

%token <legacyNode> NUMBER64BIT SCALEDINT

// CVC: Special Firebird additions.

%token <legacyNode> CURRENT_USER
%token <legacyNode> CURRENT_ROLE
%token <legacyNode> KW_BREAK
%token <legacyNode> SUBSTRING
%token <legacyNode> RECREATE
%token <legacyNode> KW_DESCRIPTOR
%token <legacyNode> FIRST
%token <legacyNode> SKIP

// tokens added for Firebird 1.5

%token <legacyNode> CURRENT_CONNECTION
%token <legacyNode> CURRENT_TRANSACTION
%token <legacyNode> BIGINT
%token <legacyNode> CASE
%token <legacyNode> NULLIF
%token <legacyNode> COALESCE
%token <legacyNode> USING
%token <legacyNode> NULLS
%token <legacyNode> LAST
%token <legacyNode> ROW_COUNT
%token <legacyNode> LOCK
%token <legacyNode> SAVEPOINT
%token <legacyNode> RELEASE
%token <legacyNode> STATEMENT
%token <legacyNode> LEAVE
%token <legacyNode> INSERTING
%token <legacyNode> UPDATING
%token <legacyNode> DELETING

// tokens added for Firebird 2.0

%token <legacyNode> BACKUP
%token <legacyNode> KW_DIFFERENCE
%token <legacyNode> OPEN
%token <legacyNode> CLOSE
%token <legacyNode> FETCH
%token <legacyNode> ROWS
%token <legacyNode> BLOCK
%token <legacyNode> IIF
%token <legacyNode> SCALAR_ARRAY
%token <legacyNode> CROSS
%token <legacyNode> NEXT
%token <legacyNode> SEQUENCE
%token <legacyNode> RESTART
%token <legacyNode> BOTH
%token <legacyNode> COLLATION
%token <legacyNode> COMMENT
%token <legacyNode> BIT_LENGTH
%token <legacyNode> CHAR_LENGTH
%token <legacyNode> CHARACTER_LENGTH
%token <legacyNode> LEADING
%token <legacyNode> KW_LOWER
%token <legacyNode> OCTET_LENGTH
%token <legacyNode> TRAILING
%token <legacyNode> TRIM
%token <legacyNode> RETURNING
%token <legacyNode> KW_IGNORE
%token <legacyNode> LIMBO
%token <legacyNode> UNDO
%token <legacyNode> REQUESTS
%token <legacyNode> TIMEOUT

// tokens added for Firebird 2.1

%token <legacyNode> ABS
%token <legacyNode> ACCENT
%token <legacyNode> ACOS
%token <legacyNode> ALWAYS
%token <legacyNode> ASCII_CHAR
%token <legacyNode> ASCII_VAL
%token <legacyNode> ASIN
%token <legacyNode> ATAN
%token <legacyNode> ATAN2
%token <legacyNode> BIN_AND
%token <legacyNode> BIN_OR
%token <legacyNode> BIN_SHL
%token <legacyNode> BIN_SHR
%token <legacyNode> BIN_XOR
%token <legacyNode> CEIL
%token <legacyNode> CONNECT
%token <legacyNode> COS
%token <legacyNode> COSH
%token <legacyNode> COT
%token <legacyNode> DATEADD
%token <legacyNode> DATEDIFF
%token <legacyNode> DECODE
%token <legacyNode> DISCONNECT
%token <legacyNode> EXP
%token <legacyNode> FLOOR
%token <legacyNode> GEN_UUID
%token <legacyNode> GENERATED
%token <legacyNode> GLOBAL
%token <legacyNode> HASH
%token <legacyNode> INSENSITIVE
%token <legacyNode> LIST
%token <legacyNode> LN
%token <legacyNode> LOG
%token <legacyNode> LOG10
%token <legacyNode> LPAD
%token <legacyNode> MATCHED
%token <legacyNode> MATCHING
%token <legacyNode> MAXVALUE
%token <legacyNode> MILLISECOND
%token <legacyNode> MINVALUE
%token <legacyNode> MOD
%token <legacyNode> OVERLAY
%token <legacyNode> PAD
%token <legacyNode> PI
%token <legacyNode> PLACING
%token <legacyNode> POWER
%token <legacyNode> PRESERVE
%token <legacyNode> RAND
%token <legacyNode> RECURSIVE
%token <legacyNode> REPLACE
%token <legacyNode> REVERSE
%token <legacyNode> ROUND
%token <legacyNode> RPAD
%token <legacyNode> SENSITIVE
%token <legacyNode> SIGN
%token <legacyNode> SIN
%token <legacyNode> SINH
%token <legacyNode> SPACE
%token <legacyNode> SQRT
%token <legacyNode> START
%token <legacyNode> TAN
%token <legacyNode> TANH
%token <legacyNode> TEMPORARY
%token <legacyNode> TRUNC
%token <legacyNode> WEEK

// tokens added for Firebird 2.5

%token <legacyNode> AUTONOMOUS
%token <legacyNode> CHAR_TO_UUID
%token <legacyNode> FIRSTNAME
%token <legacyNode> GRANTED
%token <legacyNode> LASTNAME
%token <legacyNode> MIDDLENAME
%token <legacyNode> MAPPING
%token <legacyNode> OS_NAME
%token <legacyNode> SIMILAR
%token <legacyNode> UUID_TO_CHAR
// new execute statement
%token <legacyNode> CALLER
%token <legacyNode> COMMON
%token <legacyNode> DATA
%token <legacyNode> SOURCE
%token <legacyNode> TWO_PHASE
%token <legacyNode> BIND_PARAM
%token <legacyNode> BIN_NOT

// tokens added for Firebird 3.0

%token <legacyNode> BODY
%token <legacyNode> CONTINUE
%token <legacyNode> DDL
%token <legacyNode> ENGINE
%token <legacyNode> NAME
%token <legacyNode> OVER
%token <legacyNode> PACKAGE
%token <legacyNode> PARTITION
%token <legacyNode> RDB_GET_CONTEXT
%token <legacyNode> RDB_SET_CONTEXT
%token <legacyNode> SCROLL
%token <legacyNode> PRIOR
%token <legacyNode> KW_ABSOLUTE
%token <legacyNode> KW_RELATIVE
%token <legacyNode> ACOSH
%token <legacyNode> ASINH
%token <legacyNode> ATANH
%token <legacyNode> RETURN
%token <legacyNode> DETERMINISTIC
%token <legacyNode> IDENTITY
%token <legacyNode> DENSE_RANK
%token <legacyNode> LAG
%token <legacyNode> LEAD
%token <legacyNode> RANK
%token <legacyNode> ROW_NUMBER
%token <legacyNode> SQLSTATE

%token <legacyNode> KW_BOOLEAN
%token <legacyNode> KW_FALSE
%token <legacyNode> KW_TRUE
%token <legacyNode> UNKNOWN

// precedence declarations for expression evaluation

%left	OR
%left	AND
%left	NOT
%left	'=' '<' '>' LIKE CONTAINING STARTING SIMILAR KW_IN EQL NEQ GTR LSS GEQ LEQ NOT_GTR NOT_LSS
%left	'+' '-'
%left	'*' '/'
%left	UMINUS UPLUS
%left	CONCATENATE
%left	COLLATE

// Fix the dangling IF-THEN-ELSE problem
%nonassoc THEN
%nonassoc ELSE

/* The same issue exists with ALTER COLUMN now that keywords can be used
   in order to change their names.  The syntax which shows the issue is:
	 ALTER COLUMN where column is part of the alter statement
	   or
	 ALTER COLUMN where column is the name of the column in the relation
*/
%nonassoc ALTER
%nonassoc COLUMN

%union
{
	BaseNullable<int> nullableIntVal;
	BaseNullable<bool> nullableBoolVal;
	bool boolVal;
	int intVal;
	unsigned uintVal;
	SLONG int32Val;
	FB_UINT64 uint64Val;
	BaseNullable<FB_UINT64> nullableUint64Val;
	UCHAR blrOp;
	Jrd::ComparativeBoolNode::DsqlFlag cmpBoolFlag;
	Jrd::dsql_nod* legacyNode;
	Jrd::dsql_str* legacyStr;
	Jrd::dsql_fld* legacyField;
	Firebird::PathName* pathNamePtr;
	TEXT* textPtr;
	Jrd::ExternalClause* externalClause;
	Firebird::Array<Jrd::ParameterClause>* parametersClause;
	Jrd::ExprNode* exprNode;
	Jrd::BoolExprNode* boolExprNode;
	Jrd::StmtNode* stmtNode;
	Jrd::DdlNode* ddlNode;
	Jrd::CreateCollationNode* createCollationNode;
	Jrd::CreateDomainNode* createDomainNode;
	Jrd::AlterDomainNode* alterDomainNode;
	Jrd::CreateAlterFunctionNode* createAlterFunctionNode;
	Jrd::CreateAlterProcedureNode* createAlterProcedureNode;
	Jrd::CreateAlterTriggerNode* createAlterTriggerNode;
	Jrd::CreateAlterPackageNode* createAlterPackageNode;
	Firebird::Array<Jrd::CreateAlterPackageNode::Item>* packageItems;
	Jrd::CreateAlterPackageNode::Item packageItem;
	Jrd::CreatePackageBodyNode* createPackageBodyNode;
	Jrd::CreateRelationNode* createRelationNode;
	Jrd::CreateAlterViewNode* createAlterViewNode;
	Jrd::ExecBlockNode* execBlockNode;
	Jrd::AggNode* aggNode;
	Jrd::SysFuncCallNode* sysFuncCallNode;
	Jrd::CursorStmtNode* cursorStmtNode;
	Jrd::ExecStatementNode* execStatementNode;
}

%type <legacyNode> access_mode access_type alias_list
%type <legacyNode> alter alter_clause alter_column_name
%type <legacyNode> alter_data_type_or_domain alter_db
%type <legacyNode> alter_exception_clause alter_index_clause alter_op alter_ops
%type <legacyNode> alter_role_clause alter_role_enable alter_sequence_clause
%type <legacyNode> alter_udf_clause alter_user_clause alter_view_clause
%type <legacyNode> arg_desc arg_desc_list arg_desc_list1 array_element array_range
%type <legacyNode> array_spec array_type as_opt assignment assignments
%type <legacyStr>  admin_opt

%type <legacyNode> begin_string begin_trigger
%type <legacyNode> blob_filter_subtype blob_io blob_segsize blob_subtype blob_subtype_io
%type <legacyNode> blob_subtype_value_io blob_type breakleave

%type block_input_params(<parametersClause>) block_parameter(<parametersClause>)
%type block_parameters(<parametersClause>)

%type <legacyNode> case_abbreviation case_expression case_operand case_result case_specification
%type <legacyNode> cast_specification character_keyword character_type
%type <legacyNode> charset_clause check_constraint close_cursor col_opt collate_clause
%type <legacyNode> column_constraint column_constraint_clause
%type <legacyNode> column_constraint_def column_constraint_list column_def
%type <boolVal>	   check_opt

%type <legacyNode> column_list column_name column_parens column_parens_opt column_select
%type <legacyNode> column_singleton commit complex_proc_statement
%type <legacyNode> computed_by computed_clause conditional constant continue constraint_index_opt
%type <legacyNode> constraint_name_opt correlation_name create
%type <legacyNode> create_clause create_or_alter create_user_clause cross_join
%type <legacyNode> cursor_clause cursor_declaration_item cursor_def
%type <legacyNode> cursor_statement

%type <legacyNode> data_type data_type_or_domain
%type <legacyNode> db_alter_clause db_clause db_file db_file_list db_initial_desc db_initial_desc1
%type <legacyNode> db_initial_option db_rem_desc db_rem_desc1 db_rem_option ddl_subname
%type <legacyNode> decimal_keyword declare declare_clause
%type <legacyNode> decode_pairs def_computed default_par_opt default_value delete delete_positioned
%type <legacyNode> delete_rule delete_searched delimiter_opt derived_column_list derived_table
%type <legacyNode> deterministic_opt distinct_clause
%type <legacyNode> domain_default domain_default_opt domain_or_non_array_type
%type <legacyNode> domain_or_non_array_type_name domain_type drop drop_behaviour
%type <legacyNode> drop_clause drop_user_clause
%type <legacyStr>  db_name ddl_desc

%type <legacyNode> end_default err errors event_argument_opt exception_clause
%type <legacyNode> excp_hndl_statement excp_hndl_statements
%type <legacyNode> exec_function exec_procedure
%type <legacyNode> extra_indices_opt
%type <legacyNode> execute_privilege
%type <legacyStr>  end_trigger entry_op
%type <pathNamePtr> external_file
%type <execStatementNode> exec_into exec_sql
%type exec_stmt_option(<execStatementNode>) exec_stmt_options(<execStatementNode>)
%type exec_stmt_options_list(<execStatementNode>) exec_stmt_inputs(<execStatementNode>)

%type <legacyNode> fetch_cursor file1 file_clause file_desc file_desc1
%type <legacyNode> filter_clause_io filter_decl_clause first_clause
%type <legacyNode> float_type for_select for_update_clause for_update_list from_clause
%type <legacyNode> from_list full_proc_block full_proc_block_body
%type <legacyStr>  firstname_opt
%type fetch_scroll(<cursorStmtNode>)
%type <int32Val>   first_file_length
%type <execStatementNode> for_exec_into

%type <legacyNode> generated_always_clause grant grant_option granted_by granted_by_text grantee grantee_list
%type <legacyNode> grantor group_by_item group_by_list group_clause gtt_recreate_clause
%type <legacyStr>  grant_admin grant_admin_opt

%type <legacyNode> having_clause

%type <legacyNode> identity_clause in_predicate_value
%type <legacyNode> index_definition index_list init_alter_db
%type <legacyNode> ins_column_list ins_column_parens
%type <legacyNode> ins_column_parens_opt insert integer_keyword
%type <legacyNode> iso_mode isolation_mode
%type input_parameters(<parametersClause>) input_proc_parameter(<parametersClause>)
%type input_proc_parameters(<parametersClause>)

%type <legacyNode> join_condition join_specification join_type joined_table

%type <legacyNode> keyword_or_column

%type <legacyNode> label_opt limit_clause
%type <legacyNode> local_declaration local_declaration_item local_declaration_list local_declarations
%type <legacyNode> lock_clause lock_mode lock_type lock_wait
%type <legacyStr>  lastname_opt
%type <int32Val>   long_integer

%type <legacyNode> manual_auto merge merge_insert_specification merge_update_specification
%type <legacyNode> merge_when_clause merge_when_matched_clause merge_when_not_matched_clause
%type <legacyStr>  middlename_opt module_op

%type <legacyNode> named_columns_join national_character_keyword
%type <legacyNode> national_character_type natural_join
%type <legacyNode> non_array_type
%type <legacyNode> non_charset_simple_type non_reserved_word non_role_grantee_list
%type <legacyNode> null_constraint
%type <legacyNode> nulls_clause nulls_placement numeric_type
%type <int32Val>   neg_short_integer nonneg_short_integer
%type named_param(<execStatementNode>) named_params_list(<execStatementNode>)
%type not_named_param(<execStatementNode>) not_named_params_list(<execStatementNode>)

%type <legacyNode> open_cursor opt_snapshot optional_retain
%type <legacyNode> optional_savepoint optional_work order_clause order_direction order_item order_list
%type output_parameters(<parametersClause>) output_proc_parameter(<parametersClause>)
%type output_proc_parameters(<parametersClause>)

%type <legacyNode> param_mechanism parameter plan_clause
%type <legacyNode> plan_expression plan_item plan_item_list plan_type
%type <legacyNode> post_event prec_scale primary_constraint privilege
%type <legacyNode> privilege_list privileges proc_block proc_inputs proc_outputs_opt
%type <legacyNode> proc_statement proc_statements
%type <legacyStr>  passwd_clause passwd_opt
%type <int32Val>   pos_short_integer precision_opt

%type <legacyNode> qualified_join query_spec query_term

%type <legacyNode> recreate recreate_clause referential_action referential_constraint
%type <legacyNode> referential_trigger_action release_savepoint replace_clause
%type <legacyNode> replace_exception_clause
%type <legacyNode> replace_view_clause restr_list restr_option return_mechanism return_value
%type <legacyNode> return_value1 returning_clause rev_admin_option rev_grant_option revoke
%type <legacyNode> rexception_clause role_admin_option role_clause role_grantee role_grantee_list
%type <legacyNode> role_name role_name_list rollback rows_clause rtable_clause
%type <legacyNode> rview_clause
%type <legacyStr>  revoke_admin

%type <legacyNode> savepoint scroll_opt search_condition searched_case searched_when_clause
%type <legacyNode> sec_shadow_files segment_clause_io segment_length_io select select_expr
%type <legacyNode> select_expr_body select_item select_items select_list set set_generator
%type <legacyNode> set_savepoint set_statistics set_transaction shadow_clause
%type <legacyNode> simple_case simple_UDF_name
%type <legacyNode> simple_column_name simple_package_name simple_proc_name simple_proc_statement simple_table_name
%type <legacyNode> simple_type simple_when_clause singleton_select skip_clause
%type <legacyNode> snap_shot statement stmt_start_column
%type <legacyNode> stmt_start_line string_length_opt
%type <legacyNode> symbol_UDF_call_name symbol_UDF_name symbol_blob_subtype_name symbol_character_set_name
%type <legacyNode> symbol_collation_name symbol_column_name symbol_constraint_name symbol_cursor_name
%type <legacyNode> symbol_ddl_name symbol_domain_name symbol_exception_name symbol_filter_name
%type <legacyNode> symbol_gdscode_name symbol_generator_name symbol_index_name symbol_item_alias_name
%type <legacyNode> symbol_label_name symbol_procedure_name symbol_role_name symbol_savepoint_name
%type <legacyNode> symbol_table_alias_name symbol_table_name symbol_trigger_name symbol_user_name
%type <legacyNode> symbol_variable_name symbol_view_name
%type <legacyNode> system_function_std_syntax
%type <legacyStr>  sql_string
%type <int32Val>   signed_long_integer signed_short_integer

%type <createRelationNode> table_clause
%type <createRelationNode> gtt_table_clause
%type table_elements(<createRelationNode>)
%type table_element(<createRelationNode>)
%type <intVal> gtt_scope

%type <legacyNode> table_constraint table_constraint_definition
%type <legacyNode> table_list table_lock table_name table_or_alias_list table_primary
%type <legacyNode> table_proc table_proc_inputs table_reference table_subquery tbl_reserve_options
%type <legacyNode> top tra_misc_options tra_timeout tran_opt tran_opt_list tran_opt_list_m
%type <uintVal>	   time_precision_opt timestamp_precision_opt

%type <legacyNode> u_constant u_numeric_constant udf_data_type udf_decl_clause undo_savepoint
%type <legacyNode> unique_constraint unique_opt update update_column_name
%type <legacyNode> update_or_insert update_or_insert_matching_opt update_positioned update_rule
%type <legacyNode> update_searched user_grantee user_grantee_list
%type <int32Val>   unsigned_short_integer

%type <legacyNode> valid_symbol_name value common_value common_value_list common_value_list_opt value_list value_list_opt var_decl_opt var_declaration_item
%type <legacyNode> variable variable_list varying_keyword version_mode
%type <createAlterViewNode> view_clause

%type <legacyNode> when_operand where_clause while window_partition_opt
%type <legacyNode> with_clause with_item with_list

%type <legacyStr> external_body_clause_opt

%type <legacyField> alter_col_name column_def_name data_type_descriptor init_data_type simple_column_def_name

%type <legacyNode>	boolean_literal

// New nodes

%type <intVal> ddl_type0 ddl_type1 ddl_type2

%type <boolVal> release_only_opt

%type <ddlNode> alter_charset_clause comment generator_clause
%type <stmtNode> if_then_else in_autonomous_transaction excp_statement raise_statement
%type <execBlockNode> exec_block

%type <createAlterFunctionNode> alter_function_clause function_clause function_clause_start replace_function_clause
%type <createAlterProcedureNode> alter_procedure_clause procedure_clause procedure_clause_start replace_procedure_clause
%type <externalClause> external_clause

%type <createCollationNode> collation_clause
%type collation_sequence_definition(<createCollationNode>)
%type collation_accent_attribute(<createCollationNode>)
%type collation_attribute(<createCollationNode>)
%type collation_attribute_list(<createCollationNode>)
%type collation_attribute_list_opt(<createCollationNode>)
%type collation_case_attribute(<createCollationNode>)
%type collation_pad_attribute(<createCollationNode>)
%type collation_specific_attribute_opt(<createCollationNode>)

%type <createDomainNode> domain_clause
%type domain_constraints_opt(<createDomainNode>)
%type domain_constraints(<createDomainNode>)
%type domain_constraint(<createDomainNode>)

%type <alterDomainNode> alter_domain
%type alter_domain_ops(<alterDomainNode>)
%type alter_domain_op(<alterDomainNode>)

%type <nullableBoolVal> trigger_active
%type <uint64Val> trigger_db_type trigger_ddl_type trigger_ddl_type_items trigger_type
%type <uint64Val> trigger_type_prefix trigger_type_suffix
%type <nullableUint64Val> trigger_type_opt
%type <createAlterTriggerNode> alter_trigger_clause replace_trigger_clause trigger_clause
%type <nullableIntVal> trigger_position

%type <legacyNode> symbol_package_name
%type <createAlterPackageNode> alter_package_clause package_clause replace_package_clause
%type <createPackageBodyNode> package_body_clause
%type <packageItems> package_items_opt package_items package_body_items_opt package_body_items
%type <packageItem> package_item package_body_item

%type <aggNode> aggregate_function aggregate_window_function window_function

%type <sysFuncCallNode> system_function_special_syntax

%type <blrOp> timestamp_part trim_specification

%type <exprNode> bit_length_expression char_length_expression current_role current_user
%type <exprNode> datetime_value_expression extract_expression function internal_info
%type <exprNode> length_expression next_value_expression non_aggregate_function null_value
%type <exprNode> numeric_value_function octet_length_expression over_clause string_value_function
%type <exprNode> substring_function system_function_expression trim_function udf

// Predicates
%type <boolExprNode> between_predicate comparison_predicate distinct_predicate
%type <boolExprNode> exists_predicate in_predicate binary_pattern_predicate
%type <boolExprNode> null_predicate predicate quantified_predicate boolean_value_expression
%type <boolExprNode> singular_predicate ternary_pattern_predicate

%type <blrOp> binary_pattern_operator ternary_pattern_operator comparison_operator
%type <cmpBoolFlag> quantified_flag
%type <legacyNode> escape_opt

%%

// list of possible statements

top
	: statement
		{ DSQL_parse = $1; }
	| statement ';'
		{ DSQL_parse = $1; }
	;

statement
	: alter
	| blob_io
	| comment
		{ $$ = makeClassNode($1); }
	| commit
	| create
	| create_or_alter
	| declare
	| delete
	| drop
	| grant
	| insert
	| merge
	| exec_procedure
	| exec_block
		{ $$ = makeClassNode($1); }
	| recreate
	| revoke
	| rollback
	| savepoint
	| select
	| set
	| update
	| update_or_insert
	;


// GRANT statement

grant	: GRANT privileges ON table_noise simple_table_name
			TO non_role_grantee_list grant_option granted_by
			{ $$ = make_node (nod_grant, (int) e_grant_count,
					$2, $5, make_list($7), $8, $9); }
		| GRANT execute_privilege ON PROCEDURE simple_proc_name
			TO non_role_grantee_list grant_option granted_by
			{ $$ = make_node (nod_grant, (int) e_grant_count,
					$2, $5, make_list($7), $8, $9); }
		| GRANT execute_privilege ON FUNCTION simple_UDF_name
			TO non_role_grantee_list grant_option granted_by
			{ $$ = make_node (nod_grant, (int) e_grant_count,
					$2, $5, make_list($7), $8, $9); }
		| GRANT execute_privilege ON PACKAGE simple_package_name
			TO non_role_grantee_list grant_option granted_by
			{ $$ = make_node (nod_grant, (int) e_grant_count, $2, $5, make_list($7), $8, $9); }
		| GRANT role_name_list TO role_grantee_list role_admin_option granted_by
			{ $$ = make_node (nod_grant, (int) e_grant_count,
					make_list($2), make_list($4), NULL, $5, $6); }
		;

table_noise
	:
	| TABLE
	;

privileges	: ALL
			{ $$ = make_node (nod_all, (int) 0, NULL); }
		| ALL PRIVILEGES
			{ $$ = make_node (nod_all, (int) 0, NULL); }
		| privilege_list
			{ $$ = make_list ($1); }
		;

privilege_list	: privilege
		| privilege_list ',' privilege
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;

execute_privilege	: EXECUTE
			{ $$ = make_list (make_node (nod_execute, (int) 0, NULL)); }
		;

privilege	: SELECT
			{ $$ = make_node (nod_select, (int) 0, NULL); }
		| INSERT
			{ $$ = make_node (nod_insert, (int) 0, NULL); }
		| KW_DELETE
			{ $$ = make_node (nod_delete, (int) 0, NULL); }
		| UPDATE column_parens_opt
			{ $$ = make_node (nod_update, (int) 1, $2); }
		| REFERENCES column_parens_opt
			{ $$ = make_node (nod_references, (int) 1, $2); }
		;

grant_option	: WITH GRANT OPTION
			{ $$ = make_node (nod_grant, (int) 0, NULL); }
		|
			{ $$ = NULL; }
		;

role_admin_option   : WITH ADMIN OPTION
			{ $$ = make_node (nod_grant_admin, (int) 0, NULL); }
		|
			{ $$ = NULL; }
		;

granted_by	: granted_by_text grantor
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;

granted_by_text	: GRANTED BY
		|  AS
		;

grantor		: role_grantee
			{ $$ = $1; }
		;

simple_package_name
	: symbol_package_name
		{ $$ = make_node(nod_package_name, (int) 1, $1); }
	;

simple_proc_name
	: symbol_procedure_name
		{ $$ = make_node(nod_procedure_name, (int) 1, $1); }
	;

simple_UDF_name
	: symbol_UDF_name
		{ $$ = make_node(nod_function_name, (int) 1, $1); }
	;


// REVOKE statement

revoke	: REVOKE rev_grant_option privileges ON table_noise simple_table_name
			FROM non_role_grantee_list granted_by
			{ $$ = make_node (nod_revoke, (int) e_grant_count,
					$3, $6, make_list($8), $2, $9); }
		| REVOKE rev_grant_option execute_privilege ON PROCEDURE simple_proc_name
			FROM non_role_grantee_list granted_by
			{ $$ = make_node (nod_revoke, (int) e_grant_count,
					$3, $6, make_list($8), $2, $9); }
		| REVOKE rev_grant_option execute_privilege ON FUNCTION simple_UDF_name
			FROM non_role_grantee_list granted_by
			{ $$ = make_node (nod_revoke, (int) e_grant_count,
					$3, $6, make_list($8), $2, $9); }
		| REVOKE rev_grant_option execute_privilege ON PACKAGE simple_package_name
			FROM non_role_grantee_list granted_by
			{ $$ = make_node (nod_revoke, (int) e_grant_count, $3, $6, make_list($8), $2, $9); }
		| REVOKE rev_admin_option role_name_list FROM role_grantee_list granted_by
			{ $$ = make_node (nod_revoke, (int) e_grant_count,
					make_list($3), make_list($5), NULL, $2, $6); }
		| REVOKE ALL ON ALL FROM non_role_grantee_list
			{ $$ = make_node (nod_revoke, (int) e_grant_count,
					NULL, NULL, make_list($6), NULL, NULL); }
		;

rev_grant_option : GRANT OPTION FOR
			{ $$ = make_node (nod_grant, (int) 0, NULL); }
		|
			{ $$ = NULL; }
		;

rev_admin_option : ADMIN OPTION FOR
			{ $$ = make_node (nod_grant_admin, (int) 0, NULL); }
		|
			{ $$ = NULL; }
		;

non_role_grantee_list	: grantee_list
		| user_grantee_list
		;

grantee_list	: grantee
		| grantee_list ',' grantee
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		| grantee_list ',' user_grantee
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		| user_grantee_list ',' grantee
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;

grantee
	: PROCEDURE symbol_procedure_name
		{ $$ = make_node (nod_proc_obj, (int) 1, $2); }
	| FUNCTION symbol_UDF_name
		{ $$ = make_node (nod_func_obj, (int) 1, $2); }
	| PACKAGE symbol_package_name
		{ $$ = make_node (nod_package_obj, (int) 1, $2); }
	| TRIGGER symbol_trigger_name
		{ $$ = make_node (nod_trig_obj, (int) 1, $2); }
	| VIEW symbol_view_name
		{ $$ = make_node (nod_view_obj, (int) 1, $2); }
	| ROLE symbol_role_name
			{ $$ = make_node (nod_role_name, (int) 1, $2); }
	;

user_grantee_list : user_grantee
		| user_grantee_list ',' user_grantee
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;

// CVC: In the future we can deprecate the first implicit form since we'll support
// explicit grant/revoke for both USER and ROLE keywords & object types.

user_grantee	: symbol_user_name
		{ $$ = make_node (nod_user_name, (int) 1, $1); }
	| USER symbol_user_name
		{ $$ = make_node (nod_user_name, (int) 2, $2, NULL); }
	| GROUP symbol_user_name
		{ $$ = make_node (nod_user_group, (int) 1, $2); }
	;

role_name_list  : role_name
		| role_name_list ',' role_name
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;

role_name   : symbol_role_name
		{ $$ = make_node (nod_role_name, (int) 1, $1); }
		;

role_grantee_list  : role_grantee
		| role_grantee_list ',' role_grantee
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;

role_grantee   : symbol_user_name
		{ $$ = make_node (nod_user_name, (int) 1, $1); }
	| USER symbol_user_name
		{ $$ = make_node (nod_user_name, (int) 1, $2); }
		;


// DECLARE operations

declare		: DECLARE declare_clause
			{ $$ = $2;}
		;

declare_clause
	: FILTER filter_decl_clause
		{ $$ = $2; }
	| EXTERNAL FUNCTION udf_decl_clause
		{ $$ = $3; }
	;

udf_decl_clause
	: symbol_UDF_name arg_desc_list1 RETURNS return_value1
			ENTRY_POINT sql_string MODULE_NAME sql_string
		{
			$$ = make_node (nod_def_udf, (int) e_udf_count, $1, $6, $8, make_list ($2), $4);
		}
	;

udf_data_type
	: simple_type
	| BLOB
		{ lex.g_field->fld_dtype = dtype_blob; }
	| CSTRING '(' pos_short_integer ')' charset_clause
		{
			lex.g_field->fld_dtype = dtype_cstring;
			lex.g_field->fld_character_length = (USHORT) $3;
		}
	;

arg_desc_list1	:
		 	{ $$ = NULL; }
		| arg_desc_list
		| '(' arg_desc_list ')'
		 	{ $$ = $2; }
		;

arg_desc_list	: arg_desc
		| arg_desc_list ',' arg_desc
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;

/*arg_desc	: init_data_type udf_data_type
  { $$ = $1; } */
arg_desc	: init_data_type udf_data_type param_mechanism
			{ $$ = make_node (nod_udf_param, (int) e_udf_param_count, $1, $3); }
		;

param_mechanism :
			{ $$ = NULL; } // Beware: ddl.cpp converts this to mean FUN_reference.
		| BY KW_DESCRIPTOR
			{ $$ = MAKE_const_slong (FUN_descriptor); }
		| BY SCALAR_ARRAY
			{ $$ = MAKE_const_slong (FUN_scalar_array); }
		| KW_NULL
			{ $$ = MAKE_const_slong (FUN_ref_with_null); }
		;

return_value1	: return_value
		| '(' return_value ')'
			{ $$ = $2; }
		;

return_value
	: init_data_type udf_data_type return_mechanism
		{ $$ = make_node (nod_udf_return_value, (int) e_udf_param_count, $1, $3); }
	| PARAMETER pos_short_integer
		{
			$$ = make_node(nod_udf_return_value, (int) e_udf_param_count, NULL, MAKE_const_slong($2));
		}
	;

return_mechanism :
			{ $$ = MAKE_const_slong (FUN_reference); }
		| BY KW_VALUE
			{ $$ = MAKE_const_slong (FUN_value); }
		| BY KW_DESCRIPTOR
			{ $$ = MAKE_const_slong (FUN_descriptor); }
		| FREE_IT
			{ $$ = MAKE_const_slong (-1 * FUN_reference); }
										 // FUN_refrence with FREE_IT is -ve
		| BY KW_DESCRIPTOR FREE_IT
			{ $$ = MAKE_const_slong (-1 * FUN_descriptor); }
		;


filter_decl_clause : symbol_filter_name INPUT_TYPE blob_filter_subtype OUTPUT_TYPE blob_filter_subtype
			ENTRY_POINT sql_string MODULE_NAME sql_string
				{ $$ = make_node (nod_def_filter, (int) e_filter_count, $1, $3, $5, $7, $9); }
		;

blob_filter_subtype
	: symbol_blob_subtype_name
		{ $$ = MAKE_constant((dsql_str*) $1, CONSTANT_STRING); }
	| signed_short_integer
		{ $$ = MAKE_const_slong($1); }
	;

// CREATE metadata operations

create	 	: CREATE create_clause
			{ $$ = $2; }
		;

create_clause	: EXCEPTION exception_clause
			{ $$ = $2; }
		| unique_opt order_direction INDEX symbol_index_name ON simple_table_name index_definition
			{ $$ = make_node (nod_def_index, (int) e_idx_count, $1, $2, $4, $6, $7); }
		| FUNCTION function_clause
			{ $$ = makeClassNode($2); }
		| PROCEDURE procedure_clause
			{ $$ = makeClassNode($2); }
		| TABLE table_clause
			{ $$ = makeClassNode($2); }
		| GLOBAL TEMPORARY TABLE gtt_table_clause
			{ $$ = makeClassNode($4); }
		| TRIGGER trigger_clause
			{ $$ = makeClassNode($2); }
		| VIEW view_clause
			{ $$ = makeClassNode($2); }
		| GENERATOR generator_clause
			{ $$ = makeClassNode($2); }
		| SEQUENCE generator_clause
			{ $$ = makeClassNode($2); }
		| DATABASE db_clause
			{ $$ = $2; }
		| DOMAIN domain_clause
			{ $$ = makeClassNode($2); }
		| SHADOW shadow_clause
			{ $$ = $2; }
		| ROLE role_clause
			{ $$ = $2; }
		| COLLATION collation_clause
			{ $$ = makeClassNode($2); }
		| USER create_user_clause
			{ $$ = $2; }
		| PACKAGE package_clause
			{ $$ = makeClassNode($2); }
		| PACKAGE BODY package_body_clause
			{ $$ = makeClassNode($3); }
		;


recreate 	: RECREATE recreate_clause
			{ $$ = $2; }
		;

recreate_clause
	: PROCEDURE procedure_clause
		{ $$ = makeClassNode(newNode<RecreateProcedureNode>($2)); }
	| FUNCTION function_clause
		{ $$ = makeClassNode(newNode<RecreateFunctionNode>($2)); }
	| TABLE rtable_clause
		{ $$ = $2; }
	| GLOBAL TEMPORARY TABLE gtt_recreate_clause
		{ $$ = $4; }
	| VIEW rview_clause
		{ $$ = $2; }
	| TRIGGER trigger_clause
		{ $$ = makeClassNode(newNode<RecreateTriggerNode>($2)); }
	| PACKAGE package_clause
		{ $$ = makeClassNode(newNode<RecreatePackageNode>($2)); }
	| PACKAGE BODY package_body_clause
		{ $$ = makeClassNode(newNode<RecreatePackageBodyNode>($3)); }
	| EXCEPTION rexception_clause
		{ $$ = $2; }
	;

create_or_alter	: CREATE OR ALTER replace_clause
			{ $$ = $4; }
		;

replace_clause
	: PROCEDURE replace_procedure_clause
		{ $$ = makeClassNode($2); }
	| FUNCTION replace_function_clause
		{ $$ = makeClassNode($2); }
	| TRIGGER replace_trigger_clause
		{ $$ = makeClassNode($2); }
	| PACKAGE replace_package_clause
		{ $$ = makeClassNode($2); }
	| VIEW replace_view_clause
		{ $$ = $2; }
	| EXCEPTION replace_exception_clause
		{ $$ = $2; }
	;


// CREATE EXCEPTION

exception_clause
	: symbol_exception_name sql_string
		{ $$ = makeClassNode(newNode<CreateAlterExceptionNode>(toName($1), toString($2))); }
	;

rexception_clause
	: symbol_exception_name sql_string
		{
			CreateAlterExceptionNode* createNode = newNode<CreateAlterExceptionNode>(
				toName($1), toString($2));
			$$ = makeClassNode(newNode<RecreateExceptionNode>(createNode));
		}
	;

replace_exception_clause
	: symbol_exception_name sql_string
		{
			CreateAlterExceptionNode* node = newNode<CreateAlterExceptionNode>(
				toName($1), toString($2));
			node->alter = true;
			$$ = makeClassNode(node);
		}
	;

alter_exception_clause
	: symbol_exception_name sql_string
		{
			CreateAlterExceptionNode* node = newNode<CreateAlterExceptionNode>(
				toName($1), toString($2));
			node->create = false;
			node->alter = true;
			$$ = makeClassNode(node);
		}
	;


// CREATE INDEX

unique_opt	: UNIQUE
			{ $$ = make_node (nod_unique, 0, NULL); }
		|
			{ $$ = NULL; }
		;

index_definition : column_list
			{ $$ = make_list ($1); }
		| column_parens
		| computed_by '(' begin_trigger value end_trigger ')'
			{ $$ = make_node (nod_def_computed, 2, $4, $5); }
		;


// CREATE SHADOW
shadow_clause
	: pos_short_integer manual_auto conditional sql_string first_file_length sec_shadow_files
	 	{
	 		$$ = make_node (nod_def_shadow, (int) e_shadow_count,
				(dsql_nod*)(IPTR) $1, $2, $3, $4, (dsql_nod*)(IPTR) $5, make_list($6));
	 	}
	;

manual_auto	: MANUAL
			{ $$ = MAKE_const_slong (1); }
		| AUTO
			{ $$ = MAKE_const_slong (0); }
		|
			{ $$ = MAKE_const_slong (0); }
		;

conditional	:
			{ $$ = MAKE_const_slong (0); }
		| CONDITIONAL
			{ $$ = MAKE_const_slong (1); }
		;

first_file_length
	:
		{ $$ = 0;}
	| LENGTH equals long_integer page_noise
		{ $$ = $3; }
	;

sec_shadow_files :
		 	{ $$ = NULL; }
		| db_file_list
		;

db_file_list	: db_file
		| db_file_list db_file
			{ $$ = make_node (nod_list, (int) 2, $1, $2); }
		;


// CREATE DOMAIN

domain_clause
	: column_def_name as_opt data_type domain_default_opt
			{
				$<createDomainNode>$ = newNode<CreateDomainNode>(
					ParameterClause(getPool(), (dsql_fld*) $1, "", $4, NULL));
			}
		domain_constraints_opt($5) collate_clause
			{
				$$ = $5;
				$$->nameType.collate = toName($7);
			}
	;

domain_constraints_opt($createDomainNode)
	:
	| domain_constraints($createDomainNode)
	;

domain_constraints($createDomainNode)
	: domain_constraint($createDomainNode)
	| domain_constraints domain_constraint($createDomainNode)
	;

domain_constraint($createDomainNode)
	: null_constraint
		{ setClause($createDomainNode->notNull, "NOT NULL"); }
	| check_constraint
		{ setClause($createDomainNode->check, "DOMAIN CHECK CONSTRAINT", $1); }
	;

as_opt
	:
		{ $$ = NULL; }
	| AS
		{ $$ = NULL; }
	;

domain_default
	: DEFAULT begin_trigger default_value end_default
		{ $$ = make_node (nod_def_default, (int) e_dft_count, $3, $4); }
	;

domain_default_opt	
	:
		{ $$ = NULL; }
	| domain_default
	;

null_constraint
	: NOT KW_NULL
		{ $$ = make_node (nod_not_null, (int) 0, NULL); }
	;

check_constraint
	: CHECK begin_trigger '(' search_condition ')' end_trigger
		{ $$ = make_node (nod_def_constraint, (int) e_cnstr_count, NULL, NULL, $4, NULL, $6); }
	;


// CREATE SEQUENCE/GENERATOR

generator_clause
	: symbol_generator_name
		{ $$ = newNode<CreateSequenceNode>(toName($1)); }
	;


// CREATE ROLE

role_clause : symbol_role_name
			{ $$ = make_node (nod_def_role, (int) 1, $1); }
		;


// CREATE COLLATION

collation_clause
	: symbol_collation_name FOR symbol_character_set_name
			{ $<createCollationNode>$ = newNode<CreateCollationNode>(toName($1), toName($3)); }
		collation_sequence_definition($4)
		collation_attribute_list_opt($4) collation_specific_attribute_opt($4)
			{ $$ = $4; }
	;

collation_sequence_definition($createCollation)
	:
	| FROM symbol_collation_name
		{ $createCollation->fromName = toName($2); }
	| FROM EXTERNAL '(' sql_string ')'
		{ $createCollation->fromExternal = toString($4); }
	;

collation_attribute_list_opt($createCollation)
	:
	| collation_attribute_list($createCollation)
	;

collation_attribute_list($createCollation)
	: collation_attribute($createCollation)
	| collation_attribute_list collation_attribute($createCollation)
	;

collation_attribute($createCollation)
	: collation_pad_attribute($createCollation)
	| collation_case_attribute($createCollation)
	| collation_accent_attribute($createCollation)
	;

collation_pad_attribute($createCollation)
	: NO PAD
		{ $createCollation->unsetAttribute(TEXTTYPE_ATTR_PAD_SPACE); }
	| PAD SPACE
		{ $createCollation->setAttribute(TEXTTYPE_ATTR_PAD_SPACE); }
	;

collation_case_attribute($createCollation)
	: CASE SENSITIVE
		{ $createCollation->unsetAttribute(TEXTTYPE_ATTR_CASE_INSENSITIVE); }
	| CASE INSENSITIVE
		{ $createCollation->setAttribute(TEXTTYPE_ATTR_CASE_INSENSITIVE); }
	;

collation_accent_attribute($createCollation)
	: ACCENT SENSITIVE
		{ $createCollation->unsetAttribute(TEXTTYPE_ATTR_ACCENT_INSENSITIVE); }
	| ACCENT INSENSITIVE
		{ $createCollation->setAttribute(TEXTTYPE_ATTR_ACCENT_INSENSITIVE); }
	;

collation_specific_attribute_opt($createCollation)
	:
	| sql_string
		{
			string s(toString($1));
			$createCollation->specificAttributes.clear();
			$createCollation->specificAttributes.add((const UCHAR*) s.begin(), s.length());
		}
	;

// ALTER CHARACTER SET

alter_charset_clause
	: symbol_character_set_name SET DEFAULT COLLATION symbol_collation_name
		{ $$ = newNode<AlterCharSetNode>(toName($1), toName($5)); }
	;

// CREATE DATABASE
// ASF: CREATE DATABASE command is divided in three pieces: name, initial options and
// remote options.
// Initial options are basic properties of a database and should be handled here and
// in preparse.cpp.
// Remote options always come after initial options, so they don't need to be parsed
// in preparse.cpp. They are interpreted only in the server, using this grammar.

db_clause
	: db_name db_initial_desc1 db_rem_desc1
		{
			$$ = make_node(nod_def_database, (int) e_cdb_count, $1, make_list($2), make_list($3));
		}
	;

equals
	:
	| '='
	;

db_name
	: sql_string
	;

db_initial_desc1
	:
		{ $$ = NULL; }
	| db_initial_desc
	;

db_initial_desc
	: db_initial_option
	| db_initial_desc db_initial_option
		{ $$ = make_node(nod_list, 2, $1, $2); }
	;

db_initial_option
	: KW_PAGE_SIZE equals pos_short_integer
		{ $$ = make_node(nod_page_size, 1, (dsql_nod*)(IPTR) $3); }
	| LENGTH equals long_integer page_noise
		{ $$ = make_node(nod_file_length, 1, (dsql_nod*)(IPTR) $3); }
	| USER sql_string
		{ $$ = make_node(nod_user_name, 1, $2); }
	| PASSWORD sql_string
		{ $$ = make_node(nod_password, 1, $2); }
	| SET NAMES sql_string
		{ $$ = make_node(nod_lc_ctype, 1, $3); }
	;

db_rem_desc1
	:
		{ $$ = NULL; }
	| db_rem_desc
	;

db_rem_desc
	: db_rem_option
	| db_rem_desc db_rem_option
		{ $$ = make_node(nod_list, 2, $1, $2); }
	;

db_rem_option
	: db_file
	| DEFAULT CHARACTER SET symbol_character_set_name
		{ $$ = make_node(nod_dfl_charset, 1, $4); }
	| DEFAULT CHARACTER SET symbol_character_set_name COLLATION symbol_collation_name
		{
			$$ = make_node(nod_list, 2,
				make_node(nod_dfl_charset, 1, $4),
				make_node(nod_dfl_collate, 1, $6));
		}
	| KW_DIFFERENCE KW_FILE sql_string
		{ $$ = make_node(nod_difference_file, 1, $3); }
	;

db_file
	: file1 sql_string file_desc1
		{
			lex.g_file->fil_name = $2;
			$$ = make_node(nod_file_desc, (int) 1, (dsql_nod*) lex.g_file);
		}
	;

file1
	: KW_FILE
		{ lex.g_file = make_file();}
	;

file_desc1
	: { $$ = NULL; }
	| file_desc
	;

file_desc
	: file_clause
	| file_desc file_clause
	;

file_clause
	: STARTING file_clause_noise long_integer
		{ lex.g_file->fil_start = $3; }
	| LENGTH equals long_integer page_noise
		{ lex.g_file->fil_length = $3; }
	;

file_clause_noise
	:
	| AT
	| AT PAGE
	;

page_noise
	:
	| PAGE
	| PAGES
	;


// CREATE TABLE

table_clause
	: simple_table_name external_file
			{ $<createRelationNode>$ = newNode<CreateRelationNode>($1, $2); }
		'(' table_elements($3) ')'
			{ $$ = $3; }
	;

rtable_clause
	: table_clause
		{ $$ = makeClassNode(newNode<RecreateTableNode>($1)); }
	;

gtt_table_clause
	: simple_table_name
			{ $<createRelationNode>$ = newNode<CreateRelationNode>($1); }
		'(' table_elements($2) ')' gtt_scope
			{
				$$ = $2;
				$$->relationType = static_cast<rel_t>($6);
			}
	;

gtt_recreate_clause
	: gtt_table_clause
		{ $$ = makeClassNode(newNode<RecreateTableNode>($1)); }
	;

gtt_scope
	:
		{ $$ = rel_global_temp_delete; }
	| ON COMMIT KW_DELETE ROWS
		{ $$ = rel_global_temp_delete; }
	| ON COMMIT PRESERVE ROWS
		{ $$ = rel_global_temp_preserve; }
	;

external_file
	:
		{ $$ = NULL; }
	| EXTERNAL KW_FILE sql_string
		{ $$ = newNode<PathName>($3->str_data, $3->str_length); }
	| EXTERNAL sql_string
		{ $$ = newNode<PathName>($2->str_data, $2->str_length); }
	;

table_elements($createRelationNode)
	: table_element($createRelationNode)
	| table_elements ',' table_element($createRelationNode)
	;

table_element($createRelationNode)
	: column_def
		{ $createRelationNode->elements.add($1); }
	| table_constraint_definition
		{ $createRelationNode->elements.add($1); }
	;


// column definition

column_def
	: column_def_name data_type_or_domain domain_default_opt column_constraint_clause collate_clause
		{ $$ = make_node(nod_def_field, (int) e_dfl_count, $1, $3, make_list($4), $5, $2, NULL, NULL); }
	| column_def_name data_type_or_domain identity_clause column_constraint_clause collate_clause
		{ $$ = make_node(nod_def_field, (int) e_dfl_count, $1, NULL, make_list($4), $5, $2, NULL, $3); }
	| column_def_name non_array_type def_computed
		{ $$ = make_node(nod_def_field, (int) e_dfl_count, $1, NULL, NULL, NULL, NULL, $3, NULL); }
	| column_def_name def_computed
		{ $$ = make_node(nod_def_field, (int) e_dfl_count, $1, NULL, NULL, NULL, NULL, $2, NULL); }
	;

identity_clause
	: GENERATED BY DEFAULT AS IDENTITY
		{ $$ = make_node(nod_flag, 0, NULL); }
	;

// value does allow parens around it, but there is a problem getting the source text.

def_computed	: computed_clause '(' begin_trigger value end_trigger ')'
			{
				lex.g_field->fld_flags |= FLD_computed;
				$$ = make_node (nod_def_computed, 2, $4, $5);
			}
		;

computed_clause
	: computed_by
	| generated_always_clause
	;

generated_always_clause
	: GENERATED ALWAYS AS
	;

computed_by
	: COMPUTED BY
	| COMPUTED
	;

data_type_or_domain	: data_type
			  { $$ = NULL; }
		| simple_column_name
			  { $$ = make_node (nod_def_domain, (int) e_dom_count, $1, NULL, NULL, NULL); }
		;

collate_clause	: COLLATE symbol_collation_name
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;


column_def_name	: simple_column_name
			{
				lex.g_field_name = $1;
				lex.g_field = make_field ($1);
				$$ = lex.g_field;
			}
		;

simple_column_def_name  : simple_column_name
			{
				lex.g_field = make_field ($1);
				$$ = lex.g_field;
			}
		;


data_type_descriptor :	init_data_type data_type
			{ $$ = $1; }
		| KW_TYPE OF column_def_name
			{
				$3->fld_type_of_name = $3->fld_name;
				$$ = $3;
			}
		| KW_TYPE OF COLUMN symbol_column_name '.' symbol_column_name
			{
				lex.g_field = make_field(NULL);
				lex.g_field->fld_type_of_table = ((dsql_str*) $4);
				lex.g_field->fld_type_of_name = ((dsql_str*) $6)->str_data;
				$$ = lex.g_field;
			}
		| column_def_name
			{
				$1->fld_type_of_name = $1->fld_name;
				$1->fld_full_domain = true;
				$$ = $1;
			}
		;

init_data_type
	:
		{
			lex.g_field = make_field(NULL);
			$$ = lex.g_field;
		}
	;


default_value
	: constant
	| current_user					{ $$ = makeClassNode($1); }
	| current_role					{ $$ = makeClassNode($1); }
	| internal_info					{ $$ = makeClassNode($1); }
	| null_value					{ $$ = makeClassNode($1); }
	| datetime_value_expression		{ $$ = makeClassNode($1); }
	;

column_constraint_clause :
				{ $$ = NULL; }
			| column_constraint_list
			;

column_constraint_list	: column_constraint_def
				| column_constraint_list column_constraint_def
			{ $$ = make_node (nod_list, (int) 2, $1, $2); }
				;

column_constraint_def : constraint_name_opt column_constraint
			{ $$ = make_node (nod_rel_constraint, (int) 2, $1, $2);}
		;


column_constraint : null_constraint
				  | check_constraint
				  | REFERENCES simple_table_name column_parens_opt
			referential_trigger_action constraint_index_opt
						{ $$ = make_node (nod_foreign, (int) e_for_count,
						make_node (nod_list, (int) 1, lex.g_field_name), $2, $3, $4, $5); }

				  | UNIQUE constraint_index_opt
						{ $$ = make_node (nod_unique, 2, NULL, $2); }
				  | PRIMARY KEY constraint_index_opt
						{ $$ = make_node (nod_primary, (int) e_pri_count, NULL, $3); }
		;



// table constraints

table_constraint_definition : constraint_name_opt table_constraint
		   { $$ = make_node (nod_rel_constraint, (int) 2, $1, $2);}
		;

constraint_name_opt : CONSTRAINT symbol_constraint_name
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;

table_constraint : unique_constraint
			| primary_constraint
			| referential_constraint
			| check_constraint
		;

unique_constraint	: UNIQUE column_parens constraint_index_opt
			{ $$ = make_node (nod_unique, 2, $2, $3); }
		;

primary_constraint	: PRIMARY KEY column_parens constraint_index_opt
			{ $$ = make_node (nod_primary, (int) e_pri_count, $3, $4); }
		;

referential_constraint	: FOREIGN KEY column_parens REFERENCES
			  simple_table_name column_parens_opt
			  referential_trigger_action constraint_index_opt
			{ $$ = make_node (nod_foreign, (int) e_for_count, $3, $5, $6, $7, $8); }
		;

constraint_index_opt	: USING order_direction INDEX symbol_index_name
			{ $$ = make_node (nod_def_index, (int) e_idx_count, NULL, $2, $4, NULL, NULL); }
/*
		| NO INDEX
			{ $$ = NULL; }
*/
		|
			{ $$ = make_node (nod_def_index, (int) e_idx_count, NULL, NULL, NULL, NULL, NULL); }
		;

referential_trigger_action:
		  update_rule
		  { $$ = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, $1, NULL);}
		| delete_rule
		  { $$ = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, NULL, $1);}
		| delete_rule update_rule
		  { $$ = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, $2, $1); }
		| update_rule delete_rule
		  { $$ = make_node (nod_ref_upd_del, (int) e_ref_upd_del_count, $1, $2);}
		| // empty
		  { $$ = NULL;}
		;

update_rule	: ON UPDATE referential_action
		  { $$ = $3;}
		;
delete_rule	: ON KW_DELETE referential_action
		  { $$ = $3;}
		;

referential_action: CASCADE
		  { $$ = make_flag_node (nod_ref_trig_action,
			 REF_ACTION_CASCADE, (int) e_ref_trig_action_count, NULL);}
		| SET DEFAULT
		  { $$ = make_flag_node (nod_ref_trig_action,
			 REF_ACTION_SET_DEFAULT, (int) e_ref_trig_action_count, NULL);}
		| SET KW_NULL
		  { $$ = make_flag_node (nod_ref_trig_action,
			 REF_ACTION_SET_NULL, (int) e_ref_trig_action_count, NULL);}
		| NO ACTION
		  { $$ = make_flag_node (nod_ref_trig_action,
			 REF_ACTION_NONE, (int) e_ref_trig_action_count, NULL);}
		;


// PROCEDURE


procedure_clause
	: procedure_clause_start AS begin_string local_declaration_list full_proc_block end_trigger
		{
			$$ = $1;
			$$->source = toString($6);
			$$->localDeclList = $4;
			$$->body = $5;
		}
	| procedure_clause_start external_clause external_body_clause_opt
		{
			$$ = $1;
			$$->external = $2;
			if ($3)
				$$->source = toString($3);
		}
	;

procedure_clause_start
	: symbol_procedure_name
			{ $$ = newNode<CreateAlterProcedureNode>(toName($1)); }
		input_parameters(&$2->parameters) output_parameters(&$2->returns)
			{ $$ = $2; }
	;

alter_procedure_clause
	: procedure_clause
		{
			$$ = $1;
			$$->alter = true;
			$$->create = false;
		}
	;

replace_procedure_clause
	: procedure_clause
		{
			$$ = $1;
			$$->alter = true;
		}
	;

input_parameters($parameters)
	:
	| '(' ')'
	| '(' input_proc_parameters($parameters) ')'
	;

output_parameters($parameters)
	:
	| RETURNS '(' output_proc_parameters($parameters) ')'
	;

input_proc_parameters($parameters)
	: input_proc_parameter($parameters)
	| input_proc_parameters ',' input_proc_parameter($parameters)
	;

input_proc_parameter($parameters)
	: simple_column_def_name domain_or_non_array_type collate_clause default_par_opt
		{ $parameters->add(ParameterClause(getPool(), $1, toName($3), $4, NULL)); }
	;

output_proc_parameters($parameters)
	: output_proc_parameter
	| output_proc_parameters ',' output_proc_parameter($parameters)
	;

output_proc_parameter($parameters)
	: simple_column_def_name domain_or_non_array_type collate_clause
		{ $parameters->add(ParameterClause(getPool(), $1, toName($3), NULL, NULL)); }
	;

default_par_opt	: DEFAULT begin_trigger default_value end_default
			{ $$ = make_node (nod_def_default, (int) e_dft_count, $3, $4); }
		| '=' begin_trigger default_value end_default
			{ $$ = make_node (nod_def_default, (int) e_dft_count, $3, $4); }
		|
			{ $$ = NULL; }
		;


// FUNCTION

function_clause
	: function_clause_start AS begin_string local_declaration_list full_proc_block end_trigger
		{
			$$ = $1;
			$$->source = toString($6);
			$$->localDeclList = $4;
			$$->body = $5;
		}
	| function_clause_start external_clause external_body_clause_opt
		{
			$$ = $1;
			$$->external = $2;
			if ($3)
				$$->source = toString($3);
		}
	;

function_clause_start
	: symbol_UDF_name
			{ $$ = newNode<CreateAlterFunctionNode>(toName($1)); }
		input_parameters(&$2->parameters)
		RETURNS { $<legacyField>$ = lex.g_field = make_field(NULL); }
		domain_or_non_array_type collate_clause deterministic_opt
			{
				$$ = $2;
				$$->returnType = TypeClause(getPool(), $<legacyField>5, toName($7));
				$$->deterministic = ($8 != NULL);
			}
	;

deterministic_opt
	: DETERMINISTIC
		{ $$ = make_node (nod_flag, 0, NULL); }
	| NOT DETERMINISTIC
		{ $$ = NULL; }
	|
		{ $$ = NULL; }
	;

external_clause
	: EXTERNAL NAME sql_string ENGINE valid_symbol_name
		{
			$$ = newNode<ExternalClause>();
			$$->name = toString($3);
			$$->engine = toName($5);
		}
	| EXTERNAL ENGINE valid_symbol_name
		{
			$$ = newNode<ExternalClause>();
			$$->engine = toName($3);
		}
	;

external_body_clause_opt
	: AS sql_string
		{ $$ = $2; }
	|
		{ $$ = NULL; }
	;

alter_function_clause
	: function_clause
		{
			$$ = $1;
			$$->alter = true;
			$$->create = false;
		}
	;

replace_function_clause
	: function_clause
		{
			$$ = $1;
			$$->alter = true;
		}
	;


// PACKAGE

package_clause
	: symbol_package_name AS begin_string stmt_start_line stmt_start_column
			BEGIN package_items_opt END end_trigger
		{
			CreateAlterPackageNode* node = newNode<CreateAlterPackageNode>(toName($1));
			node->source = toString($9);
			node->items = $7;

			$$ = node;
		}
	;

package_items_opt
	: package_items
	|
		{ $$ = newNode<Array<CreateAlterPackageNode::Item> >(); }
	;

package_items
	: package_item
		{
			$$ = newNode<Array<CreateAlterPackageNode::Item> >();
			$$->add($1);
		}
	| package_items package_item
		{
			$$ = $1;
			$$->add($2);
		}
	;

package_item
	: FUNCTION function_clause_start ';'
		{
			$$ = CreateAlterPackageNode::Item::create($2);
		}
	| PROCEDURE procedure_clause_start ';'
		{
			$$ = CreateAlterPackageNode::Item::create($2);
		}
	;

alter_package_clause
	: package_clause
		{
			$$ = $1;
			$$->alter = true;
			$$->create = false;
		}
	;

replace_package_clause
	: package_clause
		{
			$$ = $1;
			$$->alter = true;
		}
	;


// PACKAGE BODY

package_body_clause
	: symbol_package_name AS begin_string stmt_start_line stmt_start_column
			BEGIN package_items package_body_items_opt END end_trigger
		{
			CreatePackageBodyNode* node = newNode<CreatePackageBodyNode>(toName($1));
			node->source = toString($10);
			node->declaredItems = $7;
			node->items = $8;

			$$ = node;
		}
	| symbol_package_name AS begin_string stmt_start_line stmt_start_column
			BEGIN package_body_items_opt END end_trigger
		{
			CreatePackageBodyNode* node = newNode<CreatePackageBodyNode>(toName($1));
			node->source = toString($9);
			node->items = $7;

			$$ = node;
		}
	;

package_body_items_opt
	:
		{ $$ = newNode<Array<CreateAlterPackageNode::Item> >(); }
	| package_body_items
	;

package_body_items
	: package_body_item
		{
			$$ = newNode<Array<CreateAlterPackageNode::Item> >();
			$$->add($1);
		}
	| package_body_items package_body_item
		{
			$$ = $1;
			$$->add($2);
		}
	;

package_body_item
	: FUNCTION function_clause ';'
		{
			$$ = CreateAlterPackageNode::Item::create($2);
		}
	| PROCEDURE procedure_clause ';'
		{
			$$ = CreateAlterPackageNode::Item::create($2);
		}
	;


local_declaration_list	: local_declarations
			{ $$ = make_list ($1); }
		|
			{ $$ = NULL; }
		;

local_declarations	: local_declaration
		| local_declarations local_declaration
			{ $$ = make_node (nod_list, 2, $1, $2); }
		;

local_declaration
	: stmt_start_line stmt_start_column DECLARE var_decl_opt local_declaration_item ';'
		{
			$$ = $5;
			$$->nod_line = (IPTR) $1;
			$$->nod_column = (IPTR) $2;
		}
	;

local_declaration_item	: var_declaration_item
		| cursor_declaration_item
		;

var_declaration_item	: column_def_name domain_or_non_array_type collate_clause default_par_opt
			{ $$ = make_node (nod_def_field, (int) e_dfl_count, $1, $4, NULL, $3, NULL, NULL, NULL); }
		;

var_decl_opt	: VARIABLE
			{ $$ = NULL; }
		|
			{ $$ = NULL; }
		;

cursor_declaration_item	: symbol_cursor_name scroll_opt CURSOR FOR '(' select ')'
			{ $$ = make_flag_node (nod_cursor, NOD_CURSOR_EXPLICIT,
				(int) e_cur_count, $1, $2, $6, NULL, NULL); }
 		;

scroll_opt	: SCROLL
			{ $$ = make_node (nod_flag, 0, NULL); }
		| NO SCROLL
			{ $$ = NULL; }
		|
			{ $$ = NULL; }
		;

proc_block	: proc_statement
		| full_proc_block
		;

full_proc_block	: stmt_start_line stmt_start_column BEGIN full_proc_block_body END
			{
				$$ = make_node (nod_src_info, e_src_info_count, $1, $2, $4);
			}
		;

full_proc_block_body	: proc_statements
			{ $$ = make_node (nod_block, (int) e_blk_count, make_list ($1), NULL); }
		| proc_statements excp_hndl_statements
			{ $$ = make_node (nod_block, (int) e_blk_count, make_list ($1), make_list ($2)); }
		|
			{ $$ = make_node (nod_block, (int) e_blk_count, NULL, NULL);}
		;

proc_statements	: proc_block
		| proc_statements proc_block
			{ $$ = make_node (nod_list, 2, $1, $2); }
		;

proc_statement	: stmt_start_line stmt_start_column simple_proc_statement ';'
			{
				$$ = make_node (nod_src_info, e_src_info_count, $1, $2, $3);
			}
		| stmt_start_line stmt_start_column complex_proc_statement
			{
				$$ = make_node (nod_src_info, e_src_info_count, $1, $2, $3);
			}
		;

stmt_start_line :
		{ $$ = (dsql_nod*) (IPTR) lex.lines_bk; }

stmt_start_column :
		{
			const USHORT column = (lex.last_token_bk - lex.line_start_bk + 1);
			$$ = (dsql_nod*) (IPTR) column;
		}

simple_proc_statement	: assignment
		| insert
		| merge
		| update
		| update_or_insert
		| delete
		| singleton_select
		| exec_procedure
		| exec_sql
			{ $$ = makeClassNode($1); }
		| exec_into
			{ $$ = makeClassNode($1); }
		| exec_function
		| excp_statement
			{ $$ = makeClassNode($1); }
		| raise_statement
			{ $$ = makeClassNode($1); }
		| post_event
		| cursor_statement
		| breakleave
		| continue
		| SUSPEND
			{ $$ = makeClassNode(newNode<SuspendNode>()); }
		| EXIT
			{ $$ = makeClassNode(newNode<ExitNode>()); }
		| RETURN value
			{ $$ = makeClassNode(newNode<ReturnNode>($2)); }
		;

complex_proc_statement
	: in_autonomous_transaction
		{ $$ = makeClassNode($1); }
	| if_then_else
		{ $$ = makeClassNode($1); }
	| while
	| for_select
	| for_exec_into
		{ $$ = makeClassNode($1); }
	;

in_autonomous_transaction
	: KW_IN AUTONOMOUS TRANSACTION DO proc_block
		{
			InAutonomousTransactionNode* node = newNode<InAutonomousTransactionNode>();
			node->dsqlAction = $5;
			$$ = node;
		}
	;

excp_statement
	: EXCEPTION symbol_exception_name
		{ $$ = newNode<ExceptionNode>(toName($2)); }
	| EXCEPTION symbol_exception_name value
		{ $$ = newNode<ExceptionNode>(toName($2), $3); }
	| EXCEPTION symbol_exception_name USING '(' value_list ')'
		{ $$ = newNode<ExceptionNode>(toName($2), (dsql_nod*) NULL, make_list($5)); }
	;

raise_statement
	: EXCEPTION
		{ $$ = newNode<ExceptionNode>(); }
	;

for_select
	: label_opt FOR select INTO variable_list cursor_def DO proc_block
		{
			ForNode* node = newNode<ForNode>();
			node->dsqlLabel = $1;
			node->dsqlSelect = $3;
			node->dsqlInto = make_list($5);
			node->dsqlCursor = $6;
			node->dsqlAction = $8;
			$$ = makeClassNode(node);
		}
	;

exec_sql
	: EXECUTE STATEMENT
			{ $<execStatementNode>$ = newNode<ExecStatementNode>(); }
			exec_stmt_inputs($<execStatementNode>3) exec_stmt_options($<execStatementNode>3)
		{
			$$ = $<execStatementNode>3;
		}
	;

exec_into
	: exec_sql INTO variable_list
		{
			$$ = $<execStatementNode>1;
			$$->dsqlOutputs = make_list($3);
		}
	;

for_exec_into
	: label_opt FOR exec_into DO proc_block
		{
			$$ = $<execStatementNode>3;
			$$->dsqlLabel = $1;
			$$->dsqlInnerStmt = $5;
		}
	;

exec_stmt_inputs($execStatementNode)
	: value
		{ $execStatementNode->dsqlSql = $1; }
	| '(' value ')' '(' named_params_list($execStatementNode) ')'
		{
			$execStatementNode->dsqlSql = $2;
			$execStatementNode->dsqlInputs = make_list($execStatementNode->dsqlInputs);
		}
	| '(' value ')' '(' not_named_params_list($execStatementNode) ')'
		{
			$execStatementNode->dsqlSql = $2;
			$execStatementNode->dsqlInputs = make_list($execStatementNode->dsqlInputs);
		}
	;

named_params_list($execStatementNode)
	: named_param($execStatementNode)
	| named_params_list ',' named_param($execStatementNode)
	;

named_param($execStatementNode)
	: symbol_variable_name BIND_PARAM value
		{
			if (!$execStatementNode->inputNames)
				$execStatementNode->inputNames = FB_NEW(getPool()) EDS::ParamNames(getPool());

			$execStatementNode->inputNames->add(
				FB_NEW(getPool()) string(getPool(), toString((dsql_str*) $1)));

			if ($execStatementNode->dsqlInputs)
			{
				$execStatementNode->dsqlInputs = make_node(nod_list, 2,
					$execStatementNode->dsqlInputs, $3);
			}
			else
				$execStatementNode->dsqlInputs = $3;
		}
	;

not_named_params_list($execStatementNode)
	: not_named_param($execStatementNode)
	| not_named_params_list ',' not_named_param($execStatementNode)
	;

not_named_param($execStatementNode)
	: value
		{
			if ($execStatementNode->dsqlInputs)
			{
				$execStatementNode->dsqlInputs = make_node(nod_list, 2,
					$execStatementNode->dsqlInputs, $1);
			}
			else
				$execStatementNode->dsqlInputs = $1;
		}
	;

exec_stmt_options($execStatementNode)
	:
	| exec_stmt_options_list($execStatementNode)
	;

exec_stmt_options_list($execStatementNode)
	: exec_stmt_options_list exec_stmt_option($execStatementNode)
	| exec_stmt_option($execStatementNode)
	;

exec_stmt_option($execStatementNode)
	: ON EXTERNAL DATA SOURCE value
		{ setClause($execStatementNode->dsqlDataSource, "EXTERNAL DATA SOURCE", $5); }
	| ON EXTERNAL value
		{ setClause($execStatementNode->dsqlDataSource, "EXTERNAL DATA SOURCE", $3); }
	| AS USER value
		{ setClause($execStatementNode->dsqlUserName, "USER", $3); }
	| PASSWORD value
		{ setClause($execStatementNode->dsqlPassword, "PASSWORD", $2); }
	| ROLE value
		{ setClause($execStatementNode->dsqlRole, "ROLE", $2); }
	| WITH AUTONOMOUS TRANSACTION
		{ setClause($execStatementNode->traScope, "TRANSACTION", EDS::traAutonomous); }
	| WITH COMMON TRANSACTION
		{ setClause($execStatementNode->traScope, "TRANSACTION", EDS::traCommon); }
	| WITH CALLER PRIVILEGES
		{ setClause($execStatementNode->useCallerPrivs, "CALLER PRIVILEGES"); }
	/*
	| WITH TWO_PHASE TRANSACTION
		{ setClause($execStatementNode->traScope, "TRANSACTION", EDS::traTwoPhase); }
	*/
	;

if_then_else
	: IF '(' search_condition ')' THEN proc_block ELSE proc_block
		{
			IfNode* node = newNode<IfNode>();
			node->dsqlCondition = $3;
			node->dsqlTrueAction = $6;
			node->dsqlFalseAction = $8;
			$$ = node;
		}
	| IF '(' search_condition ')' THEN proc_block
		{
			IfNode* node = newNode<IfNode>();
			node->dsqlCondition = $3;
			node->dsqlTrueAction = $6;
			$$ = node;
		}
	;

post_event
	: POST_EVENT value event_argument_opt
		{
			PostEventNode* node = newNode<PostEventNode>();
			node->dsqlEvent = $2;
			node->dsqlArgument = $3;
			$$ = makeClassNode(node);
		}
	;

event_argument_opt
	: /*',' value
		{ $$ = $2; }
	|*/
		{ $$ = NULL; }
	;

singleton_select
	: select INTO variable_list
		{
			ForNode* node = newNode<ForNode>();
			node->dsqlSelect = $1;
			node->dsqlInto = make_list($3);
			$$ = makeClassNode(node);
		}
	;

variable	: ':' symbol_variable_name
			{ $$ = make_node (nod_var_name, (int) e_vrn_count, $2); }
		;

variable_list	: variable
 		| column_name
		| variable_list ',' column_name
			{ $$ = make_node (nod_list, 2, $1, $3); }
		| variable_list ',' variable
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

while		: label_opt WHILE '(' search_condition ')' DO proc_block
			{ $$ = make_node (nod_while, (int) e_while_count, $4, $7, $1); }
		;

label_opt	: symbol_label_name ':'
			{ $$ = make_node (nod_label, (int) e_label_count, $1, NULL); }
		|
			{ $$ = NULL; }
		;

breakleave	: KW_BREAK
			{ $$ = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
		| LEAVE
			{ $$ = make_node (nod_breakleave, (int) e_breakleave_count, NULL); }
		| LEAVE symbol_label_name
			{ $$ = make_node (nod_breakleave, (int) e_breakleave_count,
				make_node (nod_label, (int) e_label_count, $2, NULL)); }
		;

continue
	: CONTINUE
		{ $$ = make_node(nod_continue, (int) e_continue_count, NULL); }
	| CONTINUE symbol_label_name
		{
			$$ = make_node(nod_continue, (int) e_continue_count,
					make_node(nod_label, (int) e_label_count, $2, NULL));
		}
	;

cursor_def	: AS CURSOR symbol_cursor_name
			{ $$ = make_flag_node (nod_cursor, NOD_CURSOR_FOR,
				(int) e_cur_count, $3, NULL, NULL, NULL, NULL); }
		|
			{ $$ = NULL; }
		;

excp_hndl_statements	: excp_hndl_statement
		| excp_hndl_statements excp_hndl_statement
			{ $$ = make_node (nod_list, 2, $1, $2); }
		;

excp_hndl_statement	: WHEN errors DO proc_block
			{ $$ = make_node (nod_on_error, (int) e_err_count, make_list ($2), $4); }
		;

errors	: err
	| errors ',' err
		{ $$ = make_node (nod_list, 2, $1, $3); }
	;

err	: SQLCODE signed_short_integer
		{ $$ = make_node (nod_sqlcode, 1, (dsql_nod*)(IPTR) $2); }
	| GDSCODE symbol_gdscode_name
		{ $$ = make_node (nod_gdscode, 1, $2); }
	| EXCEPTION symbol_exception_name
		{ $$ = make_node (nod_exception, 1, $2); }
	| ANY
		{ $$ = make_node (nod_default, 1, NULL); }
	;

cursor_statement	: open_cursor
	| fetch_cursor
	| close_cursor
	;

open_cursor
	: OPEN symbol_cursor_name
		{ $$ = makeClassNode(newNode<CursorStmtNode>(blr_cursor_open, toName($2))); }
	;

close_cursor
	: CLOSE symbol_cursor_name
		{ $$ = makeClassNode(newNode<CursorStmtNode>(blr_cursor_close, toName($2))); }
	;

fetch_cursor
	: FETCH
			{ $<cursorStmtNode>$ = newNode<CursorStmtNode>(blr_cursor_fetch_scroll); }
			fetch_scroll($<cursorStmtNode>2) FROM symbol_cursor_name INTO variable_list
		{
			CursorStmtNode* cursorStmt = $<cursorStmtNode>2;
			cursorStmt->dsqlName = toName($5);
			cursorStmt->dsqlIntoStmt = make_list($7);
			$$ = makeClassNode(cursorStmt);
		}
	| FETCH symbol_cursor_name INTO variable_list
		{ $$ = makeClassNode(newNode<CursorStmtNode>(blr_cursor_fetch, toName($2), make_list($4))); }
	;

fetch_scroll($cursorStmtNode)
	: FIRST
		{ $cursorStmtNode->scrollOp = blr_scroll_bof; }
	| LAST
		{ $cursorStmtNode->scrollOp = blr_scroll_eof; }
	| PRIOR
		{ $cursorStmtNode->scrollOp = blr_scroll_backward; }
	| NEXT
		{ $cursorStmtNode->scrollOp = blr_scroll_forward; }
	| KW_ABSOLUTE value
		{
			$cursorStmtNode->scrollOp = blr_scroll_absolute;
			$cursorStmtNode->dsqlScrollExpr = $2;
		}
	| KW_RELATIVE value
		{
			$cursorStmtNode->scrollOp = blr_scroll_relative;
			$cursorStmtNode->dsqlScrollExpr = $2;
		}
	;


// EXECUTE PROCEDURE

exec_procedure
	: EXECUTE PROCEDURE symbol_procedure_name proc_inputs proc_outputs_opt
		{ $$ = makeClassNode(newNode<ExecProcedureNode>(QualifiedName(toName($3)), $4, $5)); }
	| EXECUTE PROCEDURE symbol_package_name '.' symbol_procedure_name proc_inputs proc_outputs_opt
		{ $$ = makeClassNode(newNode<ExecProcedureNode>(QualifiedName(toName($5), toName($3)), $6, $7)); }
	;

proc_inputs	: value_list
			{ $$ = make_list ($1); }
		| '(' value_list ')'
			{ $$ = make_list ($2); }
		|
			{ $$ = NULL; }
		;

proc_outputs_opt	: RETURNING_VALUES variable_list
			{ $$ = make_list ($2); }
		| RETURNING_VALUES '(' variable_list  ')'
			{ $$ = make_list ($3); }
		|
			{ $$ = NULL; }
		;

// EXECUTE BLOCK

exec_block
	: EXECUTE BLOCK
			{ $<execBlockNode>$ = newNode<ExecBlockNode>(); }
			block_input_params(&$3->parameters)
			output_parameters(&$3->returns) AS
			local_declaration_list
			full_proc_block
		{
			ExecBlockNode* node = $3;
			node->localDeclList = $7;
			node->body = $8;
			$$ = node;
		}
	;

block_input_params($parameters)
	:
	| '(' block_parameters($parameters) ')'
	;

block_parameters($parameters)
	: block_parameter($parameters)
	| block_parameters ',' block_parameter($parameters)
	;

block_parameter($parameters)
	: simple_column_def_name domain_or_non_array_type collate_clause '=' parameter
		{ $parameters->add(ParameterClause(getPool(), $1, toName($3), NULL, $5)); }
	;

// CREATE VIEW

view_clause
	: simple_table_name column_parens_opt AS begin_string select_expr check_opt end_trigger
		{
			CreateAlterViewNode* node = newNode<CreateAlterViewNode>($1, $2, $5);
			node->source = toString($7);
			node->withCheckOption = $6;
			$$ = node;
		}
	;

rview_clause
	: view_clause
		{ $$ = makeClassNode(newNode<RecreateViewNode>($1)); }
	;

replace_view_clause
	: view_clause
		{
			$1->alter = true;
			$$ = makeClassNode($1);
		}
	;

alter_view_clause
	: view_clause
		{
			$1->alter = true;
			$1->create = false;
			$$ = makeClassNode($1);
		}
	;


// these rules will capture the input string for storage in metadata

begin_string	:
			{ lex.beginnings.push(lex_position()); }
		;
/*
end_string	:
			{
				const TEXT* start = lex.beginnings.pop();
				$$ = (dsql_nod*) MAKE_string(start,
					(lex_position() == lex.end) ? lex_position() - start : lex.last_token - start);
			}
		;
*/
begin_trigger	:
			{ lex.beginnings.push(lex.last_token); }
		;

end_trigger	:
			{
				const TEXT* start = lex.beginnings.pop();
				string str;
				transformString(start, lex_position() - start, str);
				$$ = MAKE_string(str.c_str(), str.length());
			}
		;

end_default	:
			{
				const TEXT* start = lex.beginnings.pop();
				$$ = (dsql_nod*) MAKE_string(start,
					(yychar <= 0 ? lex_position() : lex.last_token) - start);
			}
		;

check_opt
	:
		{ $$ = false; }
	| WITH CHECK OPTION
		{ $$ = true; }
	;



// CREATE TRIGGER

trigger_clause
	: symbol_trigger_name
	  trigger_active
	  trigger_type
	  trigger_position
	  AS begin_trigger
	  local_declaration_list
	  full_proc_block
	  end_trigger
		{
			$$ = newNode<CreateAlterTriggerNode>(toName($1));
			$$->active = $2;
			$$->type = $3;
			$$->position = $4;
			$$->source = toString($9);
			$$->localDeclList = $7;
			$$->body = $8;
		}
	| symbol_trigger_name
	  trigger_active
	  trigger_type
	  trigger_position
	  external_clause external_body_clause_opt
		{
			$$ = newNode<CreateAlterTriggerNode>(toName($1));
			$$->active = $2;
			$$->type = $3;
			$$->position = $4;
			$$->external = $5;
			if ($6)
				$$->source = toString($6);
		}
	| symbol_trigger_name
	  trigger_active
	  trigger_type
	  trigger_position
	  ON symbol_table_name
	  AS begin_trigger
	  local_declaration_list
	  full_proc_block
	  end_trigger
		{
			$$ = newNode<CreateAlterTriggerNode>(toName($1));
			$$->active = $2;
			$$->type = $3;
			$$->position = $4;
			$$->relationName = toName($6);
			$$->source = toString($11);
			$$->localDeclList = $9;
			$$->body = $10;
		}
	| symbol_trigger_name
	  trigger_active
	  trigger_type
	  trigger_position
	  ON symbol_table_name
	  external_clause external_body_clause_opt
		{
			$$ = newNode<CreateAlterTriggerNode>(toName($1));
			$$->active = $2;
			$$->type = $3;
			$$->position = $4;
			$$->relationName = toName($6);
			$$->external = $7;
			if ($8)
				$$->source = toString($8);
		}
	| symbol_trigger_name
	  FOR symbol_table_name
	  trigger_active
	  trigger_type
	  trigger_position
	  AS begin_trigger
	  local_declaration_list
	  full_proc_block
	  end_trigger
		{
			$$ = newNode<CreateAlterTriggerNode>(toName($1));
			$$->active = $4;
			$$->type = $5;
			$$->position = $6;
			$$->relationName = toName($3);
			$$->source = toString($11);
			$$->localDeclList = $9;
			$$->body = $10;
		}
	| symbol_trigger_name
	  FOR symbol_table_name
	  trigger_active
	  trigger_type
	  trigger_position
	  external_clause external_body_clause_opt
		{
			$$ = newNode<CreateAlterTriggerNode>(toName($1));
			$$->active = $4;
			$$->type = $5;
			$$->position = $6;
			$$->relationName = toName($3);
			$$->external = $7;
			if ($8)
				$$->source = toString($8);
		}
	;

replace_trigger_clause
	: trigger_clause
		{
			$$ = $1;
			$$->alter = true;
		}
	;

trigger_active
	: ACTIVE
		{ $$ = Nullable<bool>::val(true); }
	| INACTIVE
		{ $$ = Nullable<bool>::val(false); }
	|
		{ $$ = Nullable<bool>::empty(); }
	;

trigger_type
	: trigger_type_prefix trigger_type_suffix
		{ $$ = $1 + $2 - 1; }
	| ON trigger_db_type
		{ $$ = $2; }
	| trigger_type_prefix trigger_ddl_type
		{ $$ = $1 + $2; }
	;

trigger_db_type
	: CONNECT
		{ $$ = TRIGGER_TYPE_DB | DB_TRIGGER_CONNECT; }
	| DISCONNECT
		{ $$ = TRIGGER_TYPE_DB | DB_TRIGGER_DISCONNECT; }
	| TRANSACTION START
		{ $$ = TRIGGER_TYPE_DB | DB_TRIGGER_TRANS_START; }
	| TRANSACTION COMMIT
		{ $$ = TRIGGER_TYPE_DB | DB_TRIGGER_TRANS_COMMIT; }
	| TRANSACTION ROLLBACK
		{ $$ = TRIGGER_TYPE_DB | DB_TRIGGER_TRANS_ROLLBACK; }
	;

trigger_ddl_type
	: trigger_ddl_type_items
	| ANY DDL STATEMENT
		{
			$$ = TRIGGER_TYPE_DDL | (0x7FFFFFFFFFFFFFFFULL & ~(FB_UINT64) TRIGGER_TYPE_MASK & ~1ULL);
		}
	;

trigger_ddl_type_items
	: CREATE TABLE			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_CREATE_TABLE); }
	| ALTER TABLE			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_ALTER_TABLE); }
	| DROP TABLE			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_DROP_TABLE); }
	| CREATE PROCEDURE		{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_CREATE_PROCEDURE); }
	| ALTER PROCEDURE		{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_ALTER_PROCEDURE); }
	| DROP PROCEDURE		{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_DROP_PROCEDURE); }
	| CREATE FUNCTION		{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_CREATE_FUNCTION); }
	| ALTER FUNCTION		{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_ALTER_FUNCTION); }
	| DROP FUNCTION			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_DROP_FUNCTION); }
	| CREATE TRIGGER		{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_CREATE_TRIGGER); }
	| ALTER TRIGGER			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_ALTER_TRIGGER); }
	| DROP TRIGGER			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_DROP_TRIGGER); }
	| CREATE EXCEPTION		{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_CREATE_EXCEPTION); }
	| ALTER EXCEPTION		{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_ALTER_EXCEPTION); }
	| DROP EXCEPTION		{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_DROP_EXCEPTION); }
	| CREATE VIEW			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_CREATE_VIEW); }
	| ALTER VIEW			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_ALTER_VIEW); }
	| DROP VIEW				{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_DROP_VIEW); }
	| CREATE DOMAIN			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_CREATE_DOMAIN); }
	| ALTER DOMAIN			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_ALTER_DOMAIN); }
	| DROP DOMAIN			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_DROP_DOMAIN); }
	| CREATE ROLE			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_CREATE_ROLE); }
	| ALTER ROLE			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_ALTER_ROLE); }
	| DROP ROLE				{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_DROP_ROLE); }
	| CREATE SEQUENCE		{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_CREATE_SEQUENCE); }
	| ALTER SEQUENCE		{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_ALTER_SEQUENCE); }
	| DROP SEQUENCE			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_DROP_SEQUENCE); }
	| CREATE USER			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_CREATE_USER); }
	| ALTER USER			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_ALTER_USER); }
	| DROP USER				{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_DROP_USER); }
	| CREATE INDEX			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_CREATE_INDEX); }
	| ALTER INDEX			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_ALTER_INDEX); }
	| DROP INDEX			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_DROP_INDEX); }
	| CREATE COLLATION		{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_CREATE_COLLATION); }
	| DROP COLLATION		{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_DROP_COLLATION); }
	| ALTER CHARACTER SET	{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_ALTER_CHARACTER_SET); }
	| CREATE PACKAGE		{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_CREATE_PACKAGE); }
	| ALTER PACKAGE			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_ALTER_PACKAGE); }
	| DROP PACKAGE			{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_DROP_PACKAGE); }
	| CREATE PACKAGE BODY	{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_CREATE_PACKAGE_BODY); }
	| DROP PACKAGE BODY		{ $$ = TRIGGER_TYPE_DDL | (1LL << DDL_TRIGGER_DROP_PACKAGE_BODY); }
	| trigger_ddl_type OR
		trigger_ddl_type	{ $$ = $1 | $3; }
	;

trigger_type_prefix
	: BEFORE
		{ $$ = 0; }
	| AFTER
		{ $$ = 1; }
	;

trigger_type_suffix
	: INSERT
		{ $$ = trigger_type_suffix(1, 0, 0); }
	| UPDATE
		{ $$ = trigger_type_suffix(2, 0, 0); }
	| KW_DELETE
		{ $$ = trigger_type_suffix(3, 0, 0); }
	| INSERT OR UPDATE
		{ $$ = trigger_type_suffix(1, 2, 0); }
	| INSERT OR KW_DELETE
		{ $$ = trigger_type_suffix(1, 3, 0); }
	| UPDATE OR INSERT
		{ $$ = trigger_type_suffix(2, 1, 0); }
	| UPDATE OR KW_DELETE
		{ $$ = trigger_type_suffix(2, 3, 0); }
	| KW_DELETE OR INSERT
		{ $$ = trigger_type_suffix(3, 1, 0); }
	| KW_DELETE OR UPDATE
		{ $$ = trigger_type_suffix(3, 2, 0); }
	| INSERT OR UPDATE OR KW_DELETE
		{ $$ = trigger_type_suffix(1, 2, 3); }
	| INSERT OR KW_DELETE OR UPDATE
		{ $$ = trigger_type_suffix(1, 3, 2); }
	| UPDATE OR INSERT OR KW_DELETE
		{ $$ = trigger_type_suffix(2, 1, 3); }
	| UPDATE OR KW_DELETE OR INSERT
		{ $$ = trigger_type_suffix(2, 3, 1); }
	| KW_DELETE OR INSERT OR UPDATE
		{ $$ = trigger_type_suffix(3, 1, 2); }
	| KW_DELETE OR UPDATE OR INSERT
		{ $$ = trigger_type_suffix(3, 2, 1); }
	;

trigger_position
	: POSITION nonneg_short_integer
		{ $$ = Nullable<int>::val($2); }
	|
		{ $$ = Nullable<int>::empty(); }
	;

// ALTER statement

alter	: ALTER alter_clause
			{ $$ = $2; }
		;

alter_clause
	: EXCEPTION alter_exception_clause
		{ $$ = $2; }
	| TABLE simple_table_name alter_ops
		{
			AlterRelationNode* node = newNode<AlterRelationNode>($2);

			dsql_nod* list = make_list($3);
			for (dsql_nod** ptr = list->nod_arg; ptr != list->nod_arg + list->nod_count; ++ptr)
				node->elements.add(*ptr);

			$$ = makeClassNode(node);
		}
	| VIEW alter_view_clause
		{ $$ = $2; }
	| TRIGGER alter_trigger_clause
		{ $$ = makeClassNode($2); }
	| PROCEDURE alter_procedure_clause
		{ $$ = makeClassNode($2); }
	| PACKAGE alter_package_clause
		{ $$ = makeClassNode($2); }
	| DATABASE init_alter_db alter_db
		{ $$ = make_node (nod_mod_database, (int) e_adb_count, make_list ($3)); }
	| DOMAIN alter_domain
		{ $$ = makeClassNode($2); }
	| INDEX alter_index_clause
		{ $$ = make_node (nod_mod_index, (int) e_mod_idx_count, $2); }
	| SEQUENCE alter_sequence_clause
		{ $$ = $2; }
	| EXTERNAL FUNCTION alter_udf_clause
		{ $$ = $3; }
	| FUNCTION alter_function_clause
		{ $$ = makeClassNode($2); }
	| ROLE alter_role_clause
		{ $$ = $2; }
	| USER alter_user_clause
		{ $$ = $2; }
	| CHARACTER SET alter_charset_clause
		{ $$ = makeClassNode($3); }
	;

alter_domain
	: keyword_or_column
			{ $<alterDomainNode>$ = newNode<AlterDomainNode>(toName($1)); }
		alter_domain_ops($2)
			{ $$ = $2; }
	;

alter_domain_ops($alterDomainNode)
	: alter_domain_op($alterDomainNode)
	| alter_domain_ops alter_domain_op($alterDomainNode)
	;

alter_domain_op($alterDomainNode)
	: SET domain_default
		{ setClause($alterDomainNode->setDefault, "DOMAIN DEFAULT", $2); }
	| ADD CONSTRAINT check_constraint
		{ setClause($alterDomainNode->setConstraint, "DOMAIN CONSTRAINT", $3); }
	| ADD check_constraint
		{ setClause($alterDomainNode->setConstraint, "DOMAIN CONSTRAINT", $2); }
	| DROP DEFAULT
		{ setClause($alterDomainNode->dropDefault, "DOMAIN DROP DEFAULT"); }
	| DROP CONSTRAINT
		{ setClause($alterDomainNode->dropConstraint, "DOMAIN DROP CONSTRAINT"); }
	| KW_NULL
		{ setClause($alterDomainNode->notNullFlag, "[NOT] NULL", false); }
	| NOT KW_NULL
		{ setClause($alterDomainNode->notNullFlag, "[NOT] NULL", true); }
	| TO symbol_column_name
		{ setClause($alterDomainNode->renameTo, "DOMAIN NAME", toName($2)); }
	| KW_TYPE init_data_type non_array_type
		{
			//// FIXME: ALTER DOMAIN doesn't support collations, and altered domain's
			//// collation is always lost.
			TypeClause* type = newNode<TypeClause>($2, MetaName());
			setClause($alterDomainNode->type, "DOMAIN TYPE", type);
		}
	;

alter_ops
	: alter_op
	| alter_ops ',' alter_op
		{ $$ = make_node(nod_list, 2, $1, $3); }
	;

alter_op	: DROP simple_column_name drop_behaviour
			{ $$ = make_node (nod_del_field, 2, $2, $3); }
		| DROP CONSTRAINT symbol_constraint_name
			{ $$ = make_node (nod_delete_rel_constraint, (int) 1, $3);}
		| ADD column_def
			{ $$ = $2; }
		| ADD table_constraint_definition
			{ $$ = $2; }
		/* CVC: From SQL, field positions start at 1, not zero. Think in ORDER BY, for example
		| col_opt simple_column_name POSITION nonneg_short_integer
			{ $$ = make_node (nod_mod_field_pos, 2, $2,
			MAKE_const_slong ((IPTR) $4)); } */
		| col_opt simple_column_name POSITION pos_short_integer
			{ $$ = make_node(nod_mod_field_pos, 2, $2, MAKE_const_slong($4)); }
		| col_opt alter_column_name TO simple_column_name
			{ $$ = make_node(nod_mod_field_name, 2, $2, $4); }
		| col_opt alter_column_name KW_NULL
			{
				$$ = make_node(nod_mod_field_null_flag, e_mod_fld_null_flag_count,
					$2, MAKE_const_slong(0));
			}
		| col_opt alter_column_name NOT KW_NULL
			{
				$$ = make_node(nod_mod_field_null_flag, e_mod_fld_null_flag_count,
					$2, MAKE_const_slong(1));
			}
		| col_opt alter_col_name KW_TYPE alter_data_type_or_domain
			{ $$ = make_node(nod_mod_field_type, e_mod_fld_type_count, $2, $4, NULL, NULL); }
		| col_opt alter_col_name KW_TYPE non_array_type def_computed
			{
				// Due to parser hacks, we should not pass $4 (non_array_type) to make_node.
				$$ = make_node(nod_mod_field_type, e_mod_fld_type_count, $2, NULL, NULL, $5);
			}
		| col_opt alter_col_name def_computed
			{ $$ = make_node(nod_mod_field_type, e_mod_fld_type_count, $2, NULL, NULL, $3); }
		| col_opt alter_col_name SET domain_default
			{ $$ = make_node(nod_mod_field_type, e_mod_fld_type_count, $2, NULL, $4, NULL); }
		| col_opt alter_col_name DROP DEFAULT
			{ $$ = make_node(nod_mod_field_type, e_mod_fld_type_count, $2, NULL,
					make_node(nod_del_default, (int) 0, NULL), NULL); }
		;

alter_column_name  : keyword_or_column
		   { $$ = make_node (nod_field_name, (int) e_fln_count, NULL, $1); }
	   ;

// below are reserved words that could be used as column identifiers
// in the previous versions

keyword_or_column	: valid_symbol_name
		| ADMIN					// added in IB 5.0
		| COLUMN				// added in IB 6.0
		| EXTRACT
		| YEAR
		| MONTH
		| DAY
		| HOUR
		| MINUTE
		| SECOND
		| TIME
		| TIMESTAMP
		| CURRENT_DATE
		| CURRENT_TIME
		| CURRENT_TIMESTAMP
		| CURRENT_USER			// added in FB 1.0
		| CURRENT_ROLE
		| RECREATE
		| CURRENT_CONNECTION	// added in FB 1.5
		| CURRENT_TRANSACTION
		| BIGINT
		| CASE
		| RELEASE
		| ROW_COUNT
		| SAVEPOINT
		| OPEN					// added in FB 2.0
		| CLOSE
		| FETCH
		| ROWS
		| USING
		| CROSS
		| BIT_LENGTH
		| BOTH
		| CHAR_LENGTH
		| CHARACTER_LENGTH
		| COMMENT
		| LEADING
		| KW_LOWER
		| OCTET_LENGTH
		| TRAILING
		| TRIM
		| CONNECT				// added in FB 2.1
		| DISCONNECT
		| GLOBAL
		| INSENSITIVE
		| RECURSIVE
		| SENSITIVE
		| START
		| SIMILAR				// added in FB 2.5
		| OVER					// added in FB 3.0
		| SCROLL
		| RETURN
		| DETERMINISTIC
		| SQLSTATE
		;

col_opt	: ALTER
			{ $$ = NULL; }
		| ALTER COLUMN
			{ $$ = NULL; }
		;

alter_data_type_or_domain	: non_array_type
			{ $$ = NULL; }
		| simple_column_name
			{ $$ = make_node (nod_def_domain, (int) e_dom_count, $1, NULL, NULL, NULL); }
		;

alter_col_name	: simple_column_name
			{
				lex.g_field_name = $1;
				lex.g_field = make_field ($1);
				$$ = lex.g_field;
			}
		;

drop_behaviour	: RESTRICT
			{ $$ = make_node (nod_restrict, 0, NULL); }
		| CASCADE
			{ $$ = make_node (nod_cascade, 0, NULL); }
		|
			{ $$ = make_node (nod_restrict, 0, NULL); }
		;

alter_index_clause	: symbol_index_name ACTIVE
				{ $$ = make_node (nod_idx_active, 1, $1); }
			| symbol_index_name INACTIVE
				{ $$ = make_node (nod_idx_inactive, 1, $1); }
			;

alter_sequence_clause
	: symbol_generator_name RESTART WITH signed_long_integer
		{ $$ = make_node(nod_set_generator2, e_gen_id_count, $1, MAKE_const_slong($4)); }
	| symbol_generator_name RESTART WITH NUMBER64BIT
		{
			$$ = make_node(nod_set_generator2, e_gen_id_count,
				$1, MAKE_constant((dsql_str*) $4, CONSTANT_SINT64));
		}
	| symbol_generator_name RESTART WITH '-' NUMBER64BIT
		{
			$$ = make_node(nod_set_generator2, e_gen_id_count, $1,
				makeClassNode(newNode<NegateNode>(MAKE_constant((dsql_str*) $5, CONSTANT_SINT64))));
		}
	;

alter_udf_clause
	: symbol_UDF_name entry_op module_op
		{ $$ = make_node(nod_mod_udf, e_mod_udf_count, $1, $2, $3); }
	;

/*
alter_role_clause	: symbol_role_name alter_role_action OS_NAME os_security_name
			{ $$ = make_node(nod_mod_role, e_mod_role_count, $4, $1, $2); }
			;

alter_role_action	: ADD
			{ $$ = MAKE_const_slong (isc_dyn_map_role); }
		| DROP
			{ $$ = MAKE_const_slong (isc_dyn_unmap_role); }
		;
 */

alter_role_clause	: symbol_role_name alter_role_enable AUTO ADMIN MAPPING
			{ $$ = make_node(nod_mod_role, e_mod_role_count, NULL, $1, $2); }
			;

alter_role_enable	: SET
			{ $$ = MAKE_const_slong (isc_dyn_automap_role); }
		| DROP
			{ $$ = MAKE_const_slong (isc_dyn_autounmap_role); }
		;
/*
os_security_name	: STRING
			{ $$ = $1; }
		;
*/
entry_op	: ENTRY_POINT sql_string
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;

module_op	: MODULE_NAME sql_string
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;


// ALTER DATABASE

init_alter_db	:
			{ $$ = NULL; }
		;

alter_db	: db_alter_clause
		| alter_db db_alter_clause
				{ $$ = make_node (nod_list, (int) 2, $1, $2); }
		;

db_alter_clause
	: ADD db_file_list
		{ $$ = $2; }
	| ADD KW_DIFFERENCE KW_FILE sql_string
		{ $$ = make_node(nod_difference_file, (int) 1, $4); }
	| DROP KW_DIFFERENCE KW_FILE
		{ $$ = make_node(nod_drop_difference, (int) 0, NULL); }
	| BEGIN BACKUP
		{ $$ = make_node(nod_begin_backup, (int) 0, NULL); }
	| END BACKUP
		{ $$ = make_node(nod_end_backup, (int) 0, NULL); }
	| SET DEFAULT CHARACTER SET symbol_character_set_name
		{ $$ = make_node(nod_dfl_charset, 1, $5); }
	;


// ALTER TRIGGER

alter_trigger_clause
	: symbol_trigger_name
	  trigger_active
	  trigger_type_opt
	  trigger_position
	  AS begin_trigger
	  local_declaration_list
	  full_proc_block
	  end_trigger
		{
			$$ = newNode<CreateAlterTriggerNode>(toName($1));
			$$->alter = true;
			$$->create = false;
			$$->active = $2;
			$$->type = $3;
			$$->position = $4;
			$$->source = toString($9);
			$$->localDeclList = $7;
			$$->body = $8;
		}
	| symbol_trigger_name
	  trigger_active
	  trigger_type_opt
	  trigger_position
	  external_clause external_body_clause_opt
		{
			$$ = newNode<CreateAlterTriggerNode>(toName($1));
			$$->alter = true;
			$$->create = false;
			$$->active = $2;
			$$->type = $3;
			$$->position = $4;
			$$->external = $5;
			if ($6)
				$$->source = toString($6);
		}
	| symbol_trigger_name
	  trigger_active
	  trigger_type_opt
	  trigger_position
		{
			$$ = newNode<CreateAlterTriggerNode>(toName($1));
			$$->alter = true;
			$$->create = false;
			$$->active = $2;
			$$->type = $3;
			$$->position = $4;
		}
	;

trigger_type_opt	// we do not allow alter database triggers, hence we do not use trigger_type here
	: trigger_type_prefix trigger_type_suffix
		{ $$ = Nullable<FB_UINT64>::val($1 + $2 - 1); }
	|
		{ $$ = Nullable<FB_UINT64>::empty(); }
	;


// DROP metadata operations

drop		: DROP drop_clause
			{ $$ = $2; }
				;

drop_clause
	: EXCEPTION symbol_exception_name
		{ $$ = makeClassNode(newNode<DropExceptionNode>(toName($2))); }
	| INDEX symbol_index_name
		{ $$ = make_node (nod_del_index, (int) 1, $2); }
	| PROCEDURE symbol_procedure_name
		{ $$ = makeClassNode(newNode<DropProcedureNode>(toName($2))); }
	| TABLE symbol_table_name
		{ $$ = makeClassNode(newNode<DropRelationNode>(toName($2), false)); }
	| TRIGGER symbol_trigger_name
		{ $$ = makeClassNode(newNode<DropTriggerNode>(toName($2))); }
	| VIEW symbol_view_name
		{ $$ = makeClassNode(newNode<DropRelationNode>(toName($2), true)); }
	| FILTER symbol_filter_name
		{ $$ = make_node (nod_del_filter, (int) 1, $2); }
	| DOMAIN symbol_domain_name
		{ $$ = makeClassNode(newNode<DropDomainNode>(toName($2))); }
	| EXTERNAL FUNCTION symbol_UDF_name
		{ $$ = makeClassNode(newNode<DropFunctionNode>(toName($3))); }
	| FUNCTION symbol_UDF_name
		{ $$ = makeClassNode(newNode<DropFunctionNode>(toName($2))); }
	| SHADOW pos_short_integer
		{ $$ = make_node (nod_del_shadow, (int) 1, (dsql_nod*)(IPTR) $2); }
	| ROLE symbol_role_name
		{ $$ = make_node (nod_del_role, (int) 1, $2); }
	| GENERATOR symbol_generator_name
		{ $$ = make_node (nod_del_generator, (int) 1, $2); }
	| SEQUENCE symbol_generator_name
		{ $$ = make_node (nod_del_generator, (int) 1, $2); }
	| COLLATION symbol_collation_name
		{ $$ = makeClassNode(newNode<DropCollationNode>(toName($2))); }
	| USER drop_user_clause
		{ $$ = $2; }
	| PACKAGE symbol_package_name
		{ $$ = makeClassNode(newNode<DropPackageNode>(toName($2))); }
	| PACKAGE BODY symbol_package_name
		{ $$ = makeClassNode(newNode<DropPackageBodyNode>(toName($3))); }
	;


// these are the allowable datatypes

data_type	: non_array_type
		| array_type
		;

domain_or_non_array_type
	:	domain_or_non_array_type_name
	|	domain_or_non_array_type_name NOT KW_NULL
			{ lex.g_field->fld_not_nullable = true; }
	;

domain_or_non_array_type_name
	:	non_array_type
	|	domain_type
	;

domain_type
	:	KW_TYPE OF symbol_column_name
			{ lex.g_field->fld_type_of_name = ((dsql_str*) $3)->str_data; }
	|	KW_TYPE OF COLUMN symbol_column_name '.' symbol_column_name
			{
				lex.g_field->fld_type_of_name = ((dsql_str*) $6)->str_data;
				lex.g_field->fld_type_of_table = ((dsql_str*) $4);
			}
	|	symbol_column_name
			{
				lex.g_field->fld_type_of_name = ((dsql_str*) $1)->str_data;
				lex.g_field->fld_full_domain = true;
			}
	;


non_array_type	: simple_type
		| blob_type
		;

array_type	: non_charset_simple_type '[' array_spec ']'
			{
				lex.g_field->fld_ranges = make_list ($3);
				lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
				lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
				$$ = $1;
			}
		| character_type '[' array_spec ']' charset_clause
			{
				lex.g_field->fld_ranges = make_list ($3);
				lex.g_field->fld_dimensions = lex.g_field->fld_ranges->nod_count / 2;
				lex.g_field->fld_element_dtype = lex.g_field->fld_dtype;
				$$ = $1;
			}
		;

array_spec	: array_range
		| array_spec ',' array_range
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

array_range
	: signed_long_integer
		{
			if ($1 < 1)
		 		$$ = make_node(nod_list, 2, MAKE_const_slong($1), MAKE_const_slong(1));
			else
				$$ = make_node(nod_list, 2, MAKE_const_slong(1), MAKE_const_slong($1) );
		}
	| signed_long_integer ':' signed_long_integer
		{ $$ = make_node(nod_list, 2, MAKE_const_slong($1), MAKE_const_slong($3)); }
	;

simple_type	: non_charset_simple_type
		| character_type charset_clause
		;

non_charset_simple_type	: national_character_type
		| numeric_type
		| float_type
		| BIGINT
			{
				if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				{
					ERRD_post (Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						Arg::Gds(isc_sql_dialect_datatype_unsupport) << Arg::Num(client_dialect) <<
																		Arg::Str("BIGINT"));
				}
				if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				{
					ERRD_post (Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						Arg::Gds(isc_sql_db_dialect_dtype_unsupport) << Arg::Num(db_dialect) <<
																	Arg::Str("BIGINT"));
				}
				lex.g_field->fld_dtype = dtype_int64;
				lex.g_field->fld_length = sizeof (SINT64);
			}
		| integer_keyword
			{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
			}
		| SMALLINT
			{
				lex.g_field->fld_dtype = dtype_short;
				lex.g_field->fld_length = sizeof (SSHORT);
			}
		| DATE
			{
				stmt_ambiguous = true;
				if (client_dialect <= SQL_DIALECT_V5)
				{
					// Post warning saying that DATE is equivalent to TIMESTAMP
					ERRD_post_warning(Arg::Warning(isc_sqlwarn) << Arg::Num(301) <<
									  Arg::Warning(isc_dtype_renamed));
					lex.g_field->fld_dtype = dtype_timestamp;
					lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
				}
				else if (client_dialect == SQL_DIALECT_V6_TRANSITION)
					yyabandon (-104, isc_transitional_date);
				else
				{
					lex.g_field->fld_dtype = dtype_sql_date;
					lex.g_field->fld_length = sizeof (ULONG);
				}
			}
		| TIME
			{
				if (client_dialect < SQL_DIALECT_V6_TRANSITION)
				{
					ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
							  Arg::Gds(isc_sql_dialect_datatype_unsupport) << Arg::Num(client_dialect) <<
																			  Arg::Str("TIME"));
				}
				if (db_dialect < SQL_DIALECT_V6_TRANSITION)
				{
					ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
							  Arg::Gds(isc_sql_db_dialect_dtype_unsupport) << Arg::Num(db_dialect) <<
																			  Arg::Str("TIME"));
				}
				lex.g_field->fld_dtype = dtype_sql_time;
				lex.g_field->fld_length = sizeof (SLONG);
			}
		| TIMESTAMP
			{
				lex.g_field->fld_dtype = dtype_timestamp;
				lex.g_field->fld_length = sizeof (GDS_TIMESTAMP);
			}
		| KW_BOOLEAN
			{
				lex.g_field->fld_dtype = dtype_boolean;
				lex.g_field->fld_length = sizeof(UCHAR);
			}
		;

integer_keyword	: INTEGER
		| KW_INT
		;


// allow a blob to be specified with any combination of segment length and subtype

blob_type
	: BLOB blob_subtype blob_segsize charset_clause
		{
			lex.g_field->fld_dtype = dtype_blob;
			lex.g_field->fld_length = sizeof(ISC_QUAD);
		}
	| BLOB '(' unsigned_short_integer ')'
		{
			lex.g_field->fld_dtype = dtype_blob;
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT) $3;
			lex.g_field->fld_sub_type = 0;
		}
	| BLOB '(' unsigned_short_integer ',' signed_short_integer ')'
		{
			lex.g_field->fld_dtype = dtype_blob;
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = (USHORT) $3;
			lex.g_field->fld_sub_type = (USHORT) $5;
		}
	| BLOB '(' ',' signed_short_integer ')'
		{
			lex.g_field->fld_dtype = dtype_blob;
			lex.g_field->fld_length = sizeof(ISC_QUAD);
			lex.g_field->fld_seg_length = 80;
			lex.g_field->fld_sub_type = (USHORT) $4;
		}
	;

blob_segsize
	: SEGMENT KW_SIZE unsigned_short_integer
	  	{
			lex.g_field->fld_seg_length = (USHORT) $3;
	  	}
	|
	  	{
			lex.g_field->fld_seg_length = (USHORT) 80;
	  	}
	;

blob_subtype
	: SUB_TYPE signed_short_integer
		{
			lex.g_field->fld_sub_type = (USHORT) $2;
		}
	| SUB_TYPE symbol_blob_subtype_name
		{
			lex.g_field->fld_sub_type_name = $2;
		}
	|
		{
			lex.g_field->fld_sub_type = (USHORT) 0;
		}
	;

charset_clause
	:
		{ $$ = NULL; }
	| CHARACTER SET symbol_character_set_name
		{
			lex.g_field->fld_character_set = $3;
		}
	;


// character type


national_character_type
	: national_character_keyword '(' pos_short_integer ')'
		{
			lex.g_field->fld_dtype = dtype_text;
			lex.g_field->fld_character_length = (USHORT) $3;
			lex.g_field->fld_flags |= FLD_national;
			$$ = NULL;
		}
	| national_character_keyword
		{
			lex.g_field->fld_dtype = dtype_text;
			lex.g_field->fld_character_length = 1;
			lex.g_field->fld_flags |= FLD_national;
			$$ = NULL;
		}
	| national_character_keyword VARYING '(' pos_short_integer ')'
		{
			lex.g_field->fld_dtype = dtype_varying;
			lex.g_field->fld_character_length = (USHORT) $4;
			lex.g_field->fld_flags |= FLD_national;
			$$ = NULL;
		}
	;

character_type
	: character_keyword '(' pos_short_integer ')'
		{
			lex.g_field->fld_dtype = dtype_text;
			lex.g_field->fld_character_length = (USHORT) $3;
		}
	| character_keyword
		{
			lex.g_field->fld_dtype = dtype_text;
			lex.g_field->fld_character_length = 1;
		}
	| varying_keyword '(' pos_short_integer ')'
		{
			lex.g_field->fld_dtype = dtype_varying;
			lex.g_field->fld_character_length = (USHORT) $3;
		}
	;

varying_keyword 	   : VARCHAR
			   | CHARACTER VARYING
			   | KW_CHAR VARYING
			   ;

character_keyword 	   : CHARACTER
			   | KW_CHAR
			   ;

national_character_keyword : NCHAR
			   | NATIONAL CHARACTER
			   | NATIONAL KW_CHAR
			   ;



// numeric type

numeric_type	: KW_NUMERIC prec_scale
			{ lex.g_field->fld_sub_type = dsc_num_type_numeric; }
		| decimal_keyword prec_scale
			{
				lex.g_field->fld_sub_type = dsc_num_type_decimal;
				if (lex.g_field->fld_dtype == dtype_short)
				{
					lex.g_field->fld_dtype = dtype_long;
					lex.g_field->fld_length = sizeof (SLONG);
				}
			}
		;

prec_scale	:
			{
				lex.g_field->fld_dtype = dtype_long;
				lex.g_field->fld_length = sizeof (SLONG);
				lex.g_field->fld_precision = 9;
			}
		| '(' signed_long_integer ')'
			{
				if ($2 < 1 || $2 > 18)
					yyabandon(-842, isc_precision_err);	// Precision must be between 1 and 18.
				if ($2 > 9)
				{
					if ( ( (client_dialect <= SQL_DIALECT_V5) && (db_dialect > SQL_DIALECT_V5) ) ||
						( (client_dialect > SQL_DIALECT_V5) && (db_dialect <= SQL_DIALECT_V5) ) )
					{
						ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-817) <<
								  Arg::Gds(isc_ddl_not_allowed_by_db_sql_dial) << Arg::Num(db_dialect));
					}
					if (client_dialect <= SQL_DIALECT_V5)
					{
						lex.g_field->fld_dtype = dtype_double;
						lex.g_field->fld_length = sizeof (double);
					}
					else
					{
						if (client_dialect == SQL_DIALECT_V6_TRANSITION)
						{
							ERRD_post_warning(Arg::Warning(isc_dsql_warn_precision_ambiguous));
							ERRD_post_warning(Arg::Warning(isc_dsql_warn_precision_ambiguous1));
							ERRD_post_warning(Arg::Warning(isc_dsql_warn_precision_ambiguous2));
						}
						lex.g_field->fld_dtype = dtype_int64;
						lex.g_field->fld_length = sizeof (SINT64);
					}
				}
				else
				{
					if ($2 < 5)
					{
						lex.g_field->fld_dtype = dtype_short;
						lex.g_field->fld_length = sizeof (SSHORT);
					}
					else
					{
						lex.g_field->fld_dtype = dtype_long;
						lex.g_field->fld_length = sizeof (SLONG);
					}
				}
				lex.g_field->fld_precision = (USHORT) $2;
			}
		| '(' signed_long_integer ',' signed_long_integer ')'
			{
				if ($2 < 1 || $2 > 18)
					yyabandon (-842, isc_precision_err);	// Precision should be between 1 and 18
				if ($4 > $2 || $4 < 0)
					yyabandon (-842, isc_scale_nogt);	// Scale must be between 0 and precision
				if ($2 > 9)
				{
					if ( ( (client_dialect <= SQL_DIALECT_V5) && (db_dialect > SQL_DIALECT_V5) ) ||
						( (client_dialect > SQL_DIALECT_V5) && (db_dialect <= SQL_DIALECT_V5) ) )
					{
						ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-817) <<
								  Arg::Gds(isc_ddl_not_allowed_by_db_sql_dial) << Arg::Num(db_dialect));
					}
					if (client_dialect <= SQL_DIALECT_V5)
					{
						lex.g_field->fld_dtype = dtype_double;
						lex.g_field->fld_length = sizeof (double);
					}
					else
					{
						if (client_dialect == SQL_DIALECT_V6_TRANSITION)
						{
							ERRD_post_warning(Arg::Warning(isc_dsql_warn_precision_ambiguous));
							ERRD_post_warning(Arg::Warning(isc_dsql_warn_precision_ambiguous1));
							ERRD_post_warning(Arg::Warning(isc_dsql_warn_precision_ambiguous2));
						}
						// client_dialect >= SQL_DIALECT_V6
						lex.g_field->fld_dtype = dtype_int64;
						lex.g_field->fld_length = sizeof (SINT64);
					}
				}
				else
				{
					if ($2 < 5)
					{
						lex.g_field->fld_dtype = dtype_short;
						lex.g_field->fld_length = sizeof (SSHORT);
					}
					else
					{
						lex.g_field->fld_dtype = dtype_long;
						lex.g_field->fld_length = sizeof (SLONG);
					}
				}
				lex.g_field->fld_precision = (USHORT) $2;
				lex.g_field->fld_scale = - (SSHORT) $4;
			}
		;

decimal_keyword	: DECIMAL
		| KW_DEC
		;



// floating point type

float_type
	: KW_FLOAT precision_opt
		{
			if ($2 > 7)
			{
				lex.g_field->fld_dtype = dtype_double;
				lex.g_field->fld_length = sizeof(double);
			}
			else
			{
				lex.g_field->fld_dtype = dtype_real;
				lex.g_field->fld_length = sizeof(float);
			}
		}
	| KW_LONG KW_FLOAT precision_opt
		{
			lex.g_field->fld_dtype = dtype_double;
			lex.g_field->fld_length = sizeof(double);
		}
	| REAL
		{
			lex.g_field->fld_dtype = dtype_real;
			lex.g_field->fld_length = sizeof(float);
		}
	| KW_DOUBLE PRECISION
		{
			lex.g_field->fld_dtype = dtype_double;
			lex.g_field->fld_length = sizeof(double);
		}
	;

precision_opt
	:
		{ $$ = 0; }
	| '(' nonneg_short_integer ')'
		{ $$ = $2; }
	;



// SET statements
set		: set_transaction
		| set_generator
		| set_statistics
		;


set_generator
	: SET GENERATOR symbol_generator_name TO signed_long_integer
		{ $$ = make_node(nod_set_generator2, e_gen_id_count, $3, MAKE_const_slong($5)); }
	| SET GENERATOR symbol_generator_name TO NUMBER64BIT
		{
			$$ = make_node(nod_set_generator2, e_gen_id_count,
				$3, MAKE_constant((dsql_str*) $5, CONSTANT_SINT64));
		}
	| SET GENERATOR symbol_generator_name TO '-' NUMBER64BIT
		{
			$$ = make_node(nod_set_generator2, e_gen_id_count, $3,
				makeClassNode(newNode<NegateNode>(MAKE_constant((dsql_str*) $6, CONSTANT_SINT64))));
		}
	;


// transaction statements

savepoint	: set_savepoint
		| release_savepoint
		| undo_savepoint
		;

set_savepoint
	: SAVEPOINT symbol_savepoint_name
		{
			UserSavepointNode* node = newNode<UserSavepointNode>();
			node->command = UserSavepointNode::CMD_SET;
			node->name = toName($2);
			$$ = makeClassNode(node);
		}
	;

release_savepoint
	: RELEASE SAVEPOINT symbol_savepoint_name release_only_opt
		{
			UserSavepointNode* node = newNode<UserSavepointNode>();
			node->command = ($4 ? UserSavepointNode::CMD_RELEASE_ONLY : UserSavepointNode::CMD_RELEASE);
			node->name = toName($3);
			$$ = makeClassNode(node);
		}
	;

release_only_opt
	:
		{ $$ = false; }
	| ONLY
		{ $$ = true; }
	;

undo_savepoint
	: ROLLBACK optional_work TO optional_savepoint symbol_savepoint_name
		{
			UserSavepointNode* node = newNode<UserSavepointNode>();
			node->command = UserSavepointNode::CMD_ROLLBACK;
			node->name = toName($5);
			$$ = makeClassNode(node);
		}
	;

optional_savepoint
	: SAVEPOINT
	| { $$ = NULL; }
	;

commit		: COMMIT optional_work optional_retain
			{ $$ = make_node (nod_commit, e_commit_count, $3); }
		;

rollback	: ROLLBACK optional_work optional_retain
			{ $$ = make_node (nod_rollback, e_rollback_count, $3); }
		;

optional_work
	: WORK
	| { $$ = NULL; }
	;

optional_retain	: RETAIN opt_snapshot
			{ $$ = make_node (nod_retain, 0, NULL); }
		|
		 	{ $$ = NULL; }
		;

opt_snapshot	: SNAPSHOT
		|
		 	{ $$ = NULL; }
		;

set_transaction	: SET TRANSACTION tran_opt_list_m
			{$$ = make_node (nod_trans, 1, make_list ($3)); }
		;

tran_opt_list_m	: tran_opt_list
		|
		 	{ $$ = NULL; }
		;

tran_opt_list	: tran_opt
		| tran_opt_list tran_opt
			{ $$ = make_node (nod_list, (int) 2, $1, $2); }
		;

tran_opt	: access_mode
		| lock_wait
		| isolation_mode
		| tra_misc_options
		| tra_timeout
		| tbl_reserve_options
		;

access_mode	: READ ONLY
			{ $$ = make_flag_node (nod_access, NOD_READ_ONLY, (int) 0, NULL); }
		| READ WRITE
			{ $$ = make_flag_node (nod_access, NOD_READ_WRITE, (int) 0, NULL); }
		;

lock_wait	: WAIT
			{ $$ = make_flag_node (nod_wait, NOD_WAIT, (int) 0, NULL); }
		| NO WAIT
			{ $$ = make_flag_node (nod_wait, NOD_NO_WAIT, (int) 0, NULL); }
		;

isolation_mode	: ISOLATION LEVEL iso_mode
			{ $$ = $3;}
		| iso_mode
		;

iso_mode	: snap_shot
			{ $$ = $1;}
		| READ UNCOMMITTED version_mode
			{ $$ = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, $3); }
		| READ COMMITTED version_mode
			{ $$ = make_flag_node (nod_isolation, NOD_READ_COMMITTED, 1, $3); }
		;

snap_shot	: SNAPSHOT
			{ $$ = make_flag_node (nod_isolation, NOD_CONCURRENCY, 0, NULL); }
		| SNAPSHOT TABLE
			{ $$ = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
		| SNAPSHOT TABLE STABILITY
			{ $$ = make_flag_node (nod_isolation, NOD_CONSISTENCY, 0, NULL); }
		;

version_mode	: VERSION
			{ $$ = make_flag_node (nod_version, NOD_VERSION, 0, NULL); }
		| NO VERSION
			{ $$ = make_flag_node (nod_version, NOD_NO_VERSION, 0, NULL); }
		|
			{ $$ = 0; }
		;

tra_misc_options: NO AUTO UNDO
			{ $$ = make_flag_node(nod_tra_misc, NOD_NO_AUTO_UNDO, 0, NULL); }
		| KW_IGNORE LIMBO
			{ $$ = make_flag_node(nod_tra_misc, NOD_IGNORE_LIMBO, 0, NULL); }
		| RESTART REQUESTS
			{ $$ = make_flag_node(nod_tra_misc, NOD_RESTART_REQUESTS, 0, NULL); }
		;

tra_timeout
	: LOCK TIMEOUT nonneg_short_integer
		{ $$ = make_node(nod_lock_timeout, 1, MAKE_const_slong($3)); }
	;

tbl_reserve_options: RESERVING restr_list
			{ $$ = make_node (nod_reserve, 1, make_list ($2)); }
		;

lock_type	: KW_SHARED
			{ $$ = (dsql_nod*) NOD_SHARED; }
		| PROTECTED
			{ $$ = (dsql_nod*) NOD_PROTECTED; }
		|
			{ $$ = (dsql_nod*) 0; }
		;

lock_mode	: READ
			{ $$ = (dsql_nod*) NOD_READ; }
		| WRITE
			{ $$ = (dsql_nod*) NOD_WRITE; }
		;

restr_list	: restr_option
		| restr_list ',' restr_option
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;

restr_option	: table_list table_lock
			{ $$ = make_node (nod_table_lock, (int) 2, make_list ($1), $2); }
		;

table_lock	: FOR lock_type lock_mode
			{ $$ = make_flag_node (nod_lock_mode, (SSHORT) ((SSHORT)(IPTR) $2 | (SSHORT)(IPTR) $3), (SSHORT) 0, NULL); }
		|
			{ $$ = 0; }
		;

table_list	: simple_table_name
		| table_list ',' simple_table_name
			{ $$ = make_node (nod_list, (int) 2, $1, $3); }
		;


set_statistics	: SET STATISTICS INDEX symbol_index_name
				{ $$ = make_node (nod_set_statistics, (int) e_stat_count, $4); }
			;

comment
	: COMMENT ON ddl_type0 IS ddl_desc
		{
			$$ = newNode<CommentOnNode>($3,
				"", "", ($5 ? toString($5) : ""), ($5 ? $5->str_charset : NULL));
		}
	| COMMENT ON ddl_type1 symbol_ddl_name IS ddl_desc
		{
			$$ = newNode<CommentOnNode>($3,
				toName($4), "", ($6 ? toString($6) : ""), ($6 ? $6->str_charset : NULL));
		}
	| COMMENT ON ddl_type2 symbol_ddl_name ddl_subname IS ddl_desc
		{
			$$ = newNode<CommentOnNode>($3,
				toName($4), toName($5), ($7 ? toString($7) : ""), ($7 ? $7->str_charset : NULL));
		}
	;

ddl_type0
	: DATABASE
		{ $$ = ddl_database; }
	;

ddl_type1
	: DOMAIN
		{ $$ = ddl_domain; }
	| TABLE
		{ $$ = ddl_relation; }
	| VIEW
		{ $$ = ddl_view; }
	| PROCEDURE
		{ $$ = ddl_procedure; }
	| TRIGGER
		{ $$ = ddl_trigger; }
	| EXTERNAL FUNCTION
		{ $$ = ddl_udf; }
	| FUNCTION
		{ $$ = ddl_udf; }
	| FILTER
		{ $$ = ddl_blob_filter; }
	| EXCEPTION
		{ $$ = ddl_exception; }
	| GENERATOR
		{ $$ = ddl_generator; }
	| SEQUENCE
		{ $$ = ddl_generator; }
	| INDEX
		{ $$ = ddl_index; }
	| ROLE
		{ $$ = ddl_role; }
	| CHARACTER SET
		{ $$ = ddl_charset; }
	| COLLATION
		{ $$ = ddl_collation; }
	| PACKAGE
		{ $$ = ddl_package; }
/*
	| SECURITY CLASS
		{ $$ = ddl_sec_class; }
*/
	;

ddl_type2
	: COLUMN
		{ $$ = ddl_relation; }
	| PARAMETER
		{ $$ = ddl_procedure; }
	;

ddl_subname
	: '.' symbol_ddl_name
		{ $$ = $2; }
	;

ddl_desc
    : sql_string
	| KW_NULL
	    { $$ = NULL; }
	;


// SELECT statement

select		: select_expr for_update_clause lock_clause
			{ $$ = make_node (nod_select, (int) e_select_count, $1, $2, $3); }
		;

for_update_clause : FOR UPDATE for_update_list
			{ $$ = make_node (nod_for_update, (int) e_fpd_count, $3); }
		|
			{ $$ = NULL; }
		;

for_update_list	: OF column_list
			{ $$ = $2; }
		|
			{ $$ = make_node (nod_flag, 0, NULL); }
		;

lock_clause : WITH LOCK
			{ $$ = make_node (nod_flag, 0, NULL); }
		|
			{ $$ = NULL; }
		;


// SELECT expression

select_expr
	: with_clause select_expr_body order_clause rows_clause
		{ $$ = make_node (nod_select_expr, (int) e_sel_count, $2, $3, $4, $1); }
	;

with_clause
    :
		{ $$ = NULL; }
	| WITH RECURSIVE with_list
		{ $$ = make_flag_node (nod_with, NOD_UNION_RECURSIVE, 1, make_list($3)); }
	| WITH with_list
		{ $$ = make_node (nod_with, 1, make_list($2)); }
	;

with_list
	: with_item
	| with_item ',' with_list
		{ $$ = make_node (nod_list, 2, $1, $3); }
	;

with_item
	: symbol_table_alias_name derived_column_list AS '(' select_expr ')'
		{ $$ = make_node (nod_derived_table, (int) e_derived_table_count, $5, $1, $2, NULL); }
	;

column_select
	: select_expr
		{
			$$ = $1;
			$$->nod_flags |= NOD_SELECT_EXPR_VALUE;
		}
	;

column_singleton
	: column_select
		{
			$$ = $1;
			$$->nod_flags |= NOD_SELECT_EXPR_SINGLETON;
		}
	;

select_expr_body
	: query_term
	| select_expr_body UNION distinct_noise query_term
		{ $$ = make_node (nod_list, 2, $1, $4); }
	| select_expr_body UNION ALL query_term
		{ $$ = make_flag_node (nod_list, NOD_UNION_ALL, 2, $1, $4); }
	;

query_term
	: query_spec
	;

query_spec
	: SELECT limit_clause
			 distinct_clause
			 select_list
			 from_clause
			 where_clause
			 group_clause
			 having_clause
			 plan_clause
		{ $$ = make_node (nod_query_spec, (int) e_qry_count, $2, $3, $4, $5, $6, $7, $8, $9); }
	;

limit_clause
	:
		{ $$ = NULL; }
	| first_clause skip_clause
		{ $$ = make_node (nod_limit, (int) e_limit_count, $2, $1); }
	| first_clause
		{ $$ = make_node (nod_limit, (int) e_limit_count, NULL, $1); }
	| skip_clause
		{ $$ = make_node (nod_limit, (int) e_limit_count, $1, NULL); }
	;

first_clause
	: FIRST long_integer
		{ $$ = MAKE_const_slong($2); }
	| FIRST '(' value ')'
		{ $$ = $3; }
	| FIRST parameter
		{ $$ = $2; }
	;

skip_clause
	: SKIP long_integer
		{ $$ = MAKE_const_slong($2); }
	| SKIP '(' value ')'
		{ $$ = $3; }
	| SKIP parameter
		{ $$ = $2; }
	;

distinct_clause	: DISTINCT
			{ $$ = make_node (nod_flag, 0, NULL); }
		| all_noise
			{ $$ = 0; }
		;

select_list	: select_items
			{ $$ = make_list ($1); }
		| '*'
			{ $$ = 0; }
		;

select_items	: select_item
		| select_items ',' select_item
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

select_item	: value
		| value as_noise symbol_item_alias_name
			{ $$ = make_node (nod_alias, 2, $1, $3); }
		;

as_noise
	:
	| AS
	;

// FROM clause

from_clause
	: FROM from_list
		{ $$ = make_list ($2); }
	;

from_list
	: table_reference
	| from_list ',' table_reference
		{ $$ = make_node (nod_list, 2, $1, $3); }
	;

table_reference	: joined_table
		| table_primary
		;

table_primary	: table_proc
		| derived_table
		| '(' joined_table ')'
			{ $$ = $2; }
		;

// AB: derived table support
derived_table :
		'(' select_expr ')' as_noise correlation_name derived_column_list
			{ $$ = make_node(nod_derived_table, (int) e_derived_table_count, $2, $5, $6, NULL); }
		;

correlation_name : symbol_table_alias_name
		|
			{ $$ = NULL; }
		;

derived_column_list : '(' alias_list ')'
			{ $$ = make_list ($2); }
		|
			{ $$ = NULL; }
		;

alias_list : symbol_item_alias_name
		| alias_list ',' symbol_item_alias_name
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

joined_table	: cross_join
		| natural_join
		| qualified_join
		;

cross_join	: table_reference CROSS JOIN table_primary
			{ $$ = make_node (nod_join, (int) e_join_count, $1,
				make_node (nod_join_inner, (int) 0, NULL), $4, NULL); }
		;

natural_join	: table_reference NATURAL join_type JOIN table_primary
			{ $$ = make_node (nod_join, (int) e_join_count, $1, $3, $5,
					make_node (nod_flag, 0, NULL)); }
		;

qualified_join	: table_reference join_type JOIN table_reference join_specification
			{ $$ = make_node (nod_join, (int) e_join_count, $1, $2, $4, $5); }
		;

join_specification	: join_condition
		| named_columns_join
		;

join_condition	: ON search_condition
			{ $$ = $2; }
		;

named_columns_join	: USING '(' column_list ')'
			{ $$ = make_list ($3); }
		;

table_proc
	: symbol_procedure_name table_proc_inputs as_noise symbol_table_alias_name
		{ $$ = make_node (nod_rel_proc_name, (int) e_rpn_count, $1, $4, $2, NULL); }
	| symbol_procedure_name table_proc_inputs
		{ $$ = make_node (nod_rel_proc_name, (int) e_rpn_count, $1, NULL, $2, NULL); }
	| symbol_package_name '.' symbol_procedure_name table_proc_inputs as_noise symbol_table_alias_name
		{ $$ = make_node (nod_rel_proc_name, (int) e_rpn_count, $3, $6, $4, $1); }
	| symbol_package_name '.' symbol_procedure_name table_proc_inputs
		{ $$ = make_node (nod_rel_proc_name, (int) e_rpn_count, $3, NULL, $4, $1); }
	;

table_proc_inputs	: '(' value_list ')'
				{ $$ = make_list ($2); }
			|
				{ $$ = NULL; }
			;

table_name	: simple_table_name
		| symbol_table_name as_noise symbol_table_alias_name
			{ $$ = make_node (nod_relation_name, (int) e_rln_count, $1, $3); }
		;

simple_table_name: symbol_table_name
			{ $$ = make_node (nod_relation_name, (int) e_rln_count, $1, NULL); }
		;

join_type	: INNER
			{ $$ = make_node (nod_join_inner, (int) 0, NULL); }
		| LEFT outer_noise
			{ $$ = make_node (nod_join_left, (int) 0, NULL); }
		| RIGHT outer_noise
			{ $$ = make_node (nod_join_right, (int) 0, NULL); }
		| FULL outer_noise
			{ $$ = make_node (nod_join_full, (int) 0, NULL); }
		|
			{ $$ = make_node (nod_join_inner, (int) 0, NULL); }
		;

outer_noise
	:
	| OUTER
	;


// other clauses in the select expression

group_clause	: GROUP BY group_by_list
			{ $$ = make_list ($3); }
		|
			{ $$ = NULL; }
		;

group_by_list	: group_by_item
		| group_by_list ',' group_by_item
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

// Except aggregate-functions are all expressions supported in group_by_item,
// they are caught inside pass1.cpp
group_by_item : value
		;

having_clause	: HAVING search_condition
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;

where_clause	: WHERE search_condition
		 	{ $$ = $2; }
		|
			{ $$ = NULL; }
		;


// PLAN clause to specify an access plan for a query

plan_clause	: PLAN plan_expression
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;

plan_expression	: plan_type '(' plan_item_list ')'
			{ $$ = make_node (nod_plan_expr, 1, make_list ($3)); }
		;

plan_type
	:
		{ $$ = NULL; }
	| JOIN
	| SORT MERGE
	| MERGE
	| HASH
	| SORT
	;

plan_item_list	: plan_item
		| plan_item ',' plan_item_list
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

plan_item	: table_or_alias_list access_type
			{ $$ = make_node (nod_plan_item, 2, make_list ($1), $2); }
		| plan_expression
		;

table_or_alias_list : symbol_table_name
		| symbol_table_name table_or_alias_list
			{ $$ = make_node (nod_list, 2, $1, $2); }
		;

access_type	: NATURAL
			{ $$ = make_node (nod_natural, (int) 0, NULL); }
		| INDEX '(' index_list ')'
			{ $$ = make_node (nod_index, 1, make_list ($3)); }
		| ORDER symbol_index_name extra_indices_opt
			{ $$ = make_node (nod_index_order, 2, $2, $3); }
		;

index_list	: symbol_index_name
		| symbol_index_name ',' index_list
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

extra_indices_opt	: INDEX '(' index_list ')'
			{ $$ = make_list ($3); }
		|
			{ $$ = 0; }
		;

// ORDER BY clause

order_clause	: ORDER BY order_list
			{ $$ = make_list ($3); }
		|
			{ $$ = 0; }
		;

order_list	: order_item
		| order_list ',' order_item
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

order_item	: value order_direction nulls_clause
			{ $$ = make_node (nod_order, (int) e_order_count, $1, $2, $3); }
		;

order_direction	: ASC
			{ $$ = 0; }
		| DESC
			{ $$ = make_node (nod_flag, 0, NULL); }
		|
			{ $$ = 0; }
		;

nulls_clause : NULLS nulls_placement
			{ $$ = $2; }
		|
			{ $$ = 0; }
		;

nulls_placement
	: FIRST	{ $$ = MAKE_const_slong(NOD_NULLS_FIRST); }
	| LAST	{ $$ = MAKE_const_slong(NOD_NULLS_LAST); }
	;

// ROWS clause

rows_clause	: ROWS value
			// equivalent to FIRST value
			{ $$ = make_node (nod_rows, (int) e_rows_count, NULL, $2); }
		| ROWS value TO value
			// equivalent to FIRST (upper_value - lower_value + 1) SKIP (lower_value - 1)
			{
				$$ = make_node (nod_rows, (int) e_rows_count,
					makeClassNode(newNode<ArithmeticNode>(blr_subtract, true,
						$2, MAKE_const_slong(1))),
					makeClassNode(newNode<ArithmeticNode>(blr_add, true,
						makeClassNode(newNode<ArithmeticNode>(blr_subtract, true,
							$4, $2)),
						MAKE_const_slong(1))));
			}
		|
			{ $$ = NULL; }
		;


// INSERT statement
// IBO hack: replace column_parens_opt by ins_column_parens_opt.
insert		: INSERT INTO simple_table_name ins_column_parens_opt
				VALUES '(' value_list ')' returning_clause
			{ $$ = make_node (nod_insert, (int) e_ins_count, $3, $4, make_list ($7), NULL, $9); }
		| INSERT INTO simple_table_name ins_column_parens_opt select_expr returning_clause
			{ $$ = make_node (nod_insert, (int) e_ins_count, $3, $4, NULL, $5, $6); }
		| INSERT INTO simple_table_name DEFAULT VALUES returning_clause
			{ $$ = make_node (nod_insert, (int) e_ins_count, $3, NULL, NULL, NULL, $6); }
		;


// MERGE statement
merge
	: MERGE INTO table_name USING table_reference ON search_condition
			merge_when_clause returning_clause
		{
			$$ = make_node(nod_merge, e_mrg_count, $3, $5, $7, $8, $9);
		}
	;

merge_when_clause
	: merge_when_matched_clause merge_when_not_matched_clause
		{ $$ = make_node(nod_merge_when, e_mrg_when_count, $1, $2); }
	| merge_when_not_matched_clause merge_when_matched_clause
		{ $$ = make_node(nod_merge_when, e_mrg_when_count, $2, $1); }
	| merge_when_matched_clause
		{ $$ = make_node(nod_merge_when, e_mrg_when_count, $1, NULL); }
	| merge_when_not_matched_clause
		{ $$ = make_node(nod_merge_when, e_mrg_when_count, NULL, $1); }
	;

merge_when_matched_clause
	: WHEN MATCHED merge_update_specification
		{ $$ = $3; }
	;

merge_when_not_matched_clause
	: WHEN NOT MATCHED merge_insert_specification
		{ $$ = $4; }
	;

merge_update_specification
	: THEN UPDATE SET assignments
		{ $$ = make_node(nod_merge_update, e_mrg_update_count, NULL, make_list($4)); }
	| AND search_condition THEN UPDATE SET assignments
		{ $$ = make_node(nod_merge_update, e_mrg_update_count, $2, make_list($6)); }
	| THEN KW_DELETE
		{ $$ = make_node(nod_merge_delete, e_mrg_delete_count, NULL); }
	| AND search_condition THEN KW_DELETE
		{ $$ = make_node(nod_merge_delete, e_mrg_delete_count, $2); }
	;

merge_insert_specification
	: THEN INSERT ins_column_parens_opt VALUES '(' value_list ')'
		{ $$ = make_node(nod_merge_insert, e_mrg_insert_count, NULL, make_list($3), make_list($6)); }
	| AND search_condition THEN INSERT ins_column_parens_opt VALUES '(' value_list ')'
		{ $$ = make_node(nod_merge_insert, e_mrg_insert_count, $2, make_list($5), make_list($8)); }
	;


// DELETE statement

delete		: delete_searched
		| delete_positioned
		;

delete_searched	: KW_DELETE FROM table_name where_clause
		plan_clause order_clause rows_clause returning_clause
			{ $$ = make_node (nod_delete, (int) e_del_count, $3, $4, $5, $6, $7, NULL, $8); }
		;

delete_positioned : KW_DELETE FROM table_name cursor_clause
			{ $$ = make_node (nod_delete, (int) e_del_count, $3, NULL, NULL, NULL, NULL, $4, NULL); }
		;


// UPDATE statement

update		: update_searched
		| update_positioned
		;

update_searched	: UPDATE table_name SET assignments where_clause
		plan_clause order_clause rows_clause returning_clause
			{ $$ = make_node (nod_update, (int) e_upd_count,
				$2, make_list ($4), $5, $6, $7, $8, NULL, $9, NULL); }
		  	;

update_positioned : UPDATE table_name SET assignments cursor_clause
			{ $$ = make_node (nod_update, (int) e_upd_count,
				$2, make_list ($4), NULL, NULL, NULL, NULL, $5, NULL, NULL); }
		;


// UPDATE OR INSERT statement

update_or_insert
	:	UPDATE OR INSERT INTO simple_table_name ins_column_parens_opt
			VALUES '(' value_list ')'
			update_or_insert_matching_opt
			returning_clause
		{
			$$ = make_node (nod_update_or_insert, (int) e_upi_count,
				$5, make_list ($6), make_list ($9), $11, $12);
		}
	;

update_or_insert_matching_opt
	:	MATCHING ins_column_parens
		{ $$ = $2; }
	|
		{ $$ = NULL; }
	;


returning_clause	: RETURNING value_list
			{ $$ = make_node (nod_returning, (int) e_ret_count, make_list ($2), NULL); }
		| RETURNING value_list INTO variable_list
			{ $$ = make_node (nod_returning, (int) e_ret_count, make_list ($2), make_list ($4)); }
		|
			{ $$ = NULL; }
		;

cursor_clause	: WHERE CURRENT OF symbol_cursor_name
			{ $$ = make_node (nod_cursor, (int) e_cur_count, $4, NULL, NULL, NULL, NULL); }
		;


// Assignments

assignments	: assignment
		| assignments ',' assignment
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

assignment	: update_column_name '=' value
			{ $$ = make_node (nod_assign, e_asgn_count, $3, $1); }
		;

exec_function
	: udf
		{
			$$ = make_node (nod_assign, e_asgn_count, makeClassNode($1),
				makeClassNode(newNode<NullNode>()));
		}
	| non_aggregate_function
		{
			$$ = make_node (nod_assign, e_asgn_count, makeClassNode($1),
				makeClassNode(newNode<NullNode>()));
		}
	;


// BLOB get and put

blob_io			: READ BLOB simple_column_name FROM simple_table_name filter_clause_io segment_clause_io
			{ $$ = make_node (nod_get_segment, (int) e_blb_count, $3, $5, $6, $7); }
				| INSERT BLOB simple_column_name INTO simple_table_name filter_clause_io segment_clause_io
			{ $$ = make_node (nod_put_segment, (int) e_blb_count, $3, $5, $6, $7); }
		;

filter_clause_io	: FILTER FROM blob_subtype_value_io TO blob_subtype_value_io
			{ $$ = make_node (nod_list, 2, $3, $5); }
		| FILTER TO blob_subtype_value_io
			{ $$ = make_node (nod_list, 2, NULL, $3); }
		|
			{ $$ = NULL; }
		;

blob_subtype_value_io : blob_subtype_io
		| parameter
		;

blob_subtype_io
	: signed_short_integer
		{ $$ = MAKE_const_slong($1); }
	;

segment_clause_io	: MAX_SEGMENT segment_length_io
			{ $$ = $2; }
		|
			{ $$ = NULL; }
		;

segment_length_io
	: unsigned_short_integer
		{ $$ = MAKE_const_slong($1); }
	| parameter
	;


// column specifications

column_parens_opt : column_parens
		|
			{ $$ = NULL; }
		;

column_parens	: '(' column_list ')'
			{ $$ = make_list ($2); }
		;

column_list	: simple_column_name
		| column_list ',' simple_column_name
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;

// begin IBO hack
ins_column_parens_opt : ins_column_parens
		|
			{ $$ = NULL; }
		;

ins_column_parens	: '(' ins_column_list ')'
			{ $$ = make_list ($2); }
		;

ins_column_list	: update_column_name
		| ins_column_list ',' update_column_name
			{ $$ = make_node (nod_list, 2, $1, $3); }
		;
// end IBO hack

column_name	 : simple_column_name
		| symbol_table_alias_name '.' symbol_column_name
			{ $$ = make_node (nod_field_name, (int) e_fln_count, $1, $3); }
		| symbol_table_alias_name '.' '*'
			{ $$ = make_node (nod_field_name, (int) e_fln_count, $1, NULL); }
		;

simple_column_name : symbol_column_name
			{ $$ = make_node (nod_field_name, (int) e_fln_count, NULL, $1); }
		;

update_column_name : simple_column_name
// CVC: This option should be deprecated! The only allowed syntax should be
// Update...set column = expr, without qualifier for the column.
		| symbol_table_alias_name '.' symbol_column_name
			{ $$ = make_node (nod_field_name, (int) e_fln_count, $1, $3); }
		;

// boolean expressions

search_condition
	: value
		{
			BoolAsValueNode* node = ExprNode::as<BoolAsValueNode>($1);
			if (node)
				$$ = node->dsqlBoolean;
			else
			{
				ComparativeBoolNode* cmpNode = newNode<ComparativeBoolNode>(
					blr_eql, $1, MAKE_constant(MAKE_string("1", 1), CONSTANT_BOOLEAN));
				cmpNode->dsqlWasValue = true;
				$$ = makeClassNode(cmpNode);
			}
		}
	;

boolean_value_expression
	: predicate
	| search_condition OR search_condition
		{ $$ = newNode<BinaryBoolNode>(blr_or, $1, $3); }
	| search_condition AND search_condition
		{ $$ = newNode<BinaryBoolNode>(blr_and, $1, $3); }
	| NOT search_condition
		{ $$ = newNode<NotBoolNode>($2); }
	| '(' boolean_value_expression ')'
		{ $$ = $2; }
	;

predicate
	: comparison_predicate
	| distinct_predicate
	| between_predicate
	| binary_pattern_predicate
	| ternary_pattern_predicate
	| in_predicate
	| null_predicate
	| quantified_predicate
	| exists_predicate
	| singular_predicate
	| common_value IS boolean_literal
		{ $$ = newNode<ComparativeBoolNode>(blr_equiv, $1, $3); }
	| common_value IS NOT boolean_literal
		{
			ComparativeBoolNode* node = newNode<ComparativeBoolNode>(blr_equiv, $1, $4);
			$$ = newNode<NotBoolNode>(makeClassNode(node));
		}
	;


// comparisons

comparison_predicate
	: common_value comparison_operator common_value
		{ $$ = newNode<ComparativeBoolNode>($2, $1, $3); }
	;

comparison_operator
	: '='		{ $$ = blr_eql; }
	| '<'		{ $$ = blr_lss; }
	| '>'		{ $$ = blr_gtr; }
	| GEQ		{ $$ = blr_geq; }
	| LEQ		{ $$ = blr_leq; }
	| NOT_GTR	{ $$ = blr_leq; }
	| NOT_LSS	{ $$ = blr_geq; }
	| NEQ		{ $$ = blr_neq; }

// quantified comparisons

quantified_predicate
	: common_value comparison_operator quantified_flag '(' column_select ')'
		{
			ComparativeBoolNode* node = newNode<ComparativeBoolNode>($2, $1, $5);
			node->dsqlFlag = $3;
			$$ = node;
		}
	;

quantified_flag
	: ALL	{ $$ = ComparativeBoolNode::DFLAG_ANSI_ALL; }
	| SOME	{ $$ = ComparativeBoolNode::DFLAG_ANSI_ANY; }
	| ANY	{ $$ = ComparativeBoolNode::DFLAG_ANSI_ANY; }
	;


// other predicates

distinct_predicate
	: common_value IS DISTINCT FROM common_value
		{
			ComparativeBoolNode* node = newNode<ComparativeBoolNode>(blr_equiv, $1, $5);
			$$ = newNode<NotBoolNode>(makeClassNode(node));
		}
	| common_value IS NOT DISTINCT FROM common_value
		{ $$ = newNode<ComparativeBoolNode>(blr_equiv, $1, $6); }
	;

between_predicate
	: common_value BETWEEN common_value AND common_value
		{ $$ = newNode<ComparativeBoolNode>(blr_between, $1, $3, $5); }
	| common_value NOT BETWEEN common_value AND common_value
		{
			ComparativeBoolNode* node = newNode<ComparativeBoolNode>(blr_between, $1, $4, $6);
			$$ = newNode<NotBoolNode>(makeClassNode(node));
		}
	;

binary_pattern_predicate
	: common_value binary_pattern_operator common_value
		{ $$ = newNode<ComparativeBoolNode>($2, $1, $3); }
	| common_value NOT binary_pattern_operator common_value
		{
			ComparativeBoolNode* cmpNode = newNode<ComparativeBoolNode>($3, $1, $4);
			$$ = newNode<NotBoolNode>(makeClassNode(cmpNode));
		}
	;

binary_pattern_operator
	: CONTAINING	{ $$ = blr_containing; }
	| STARTING		{ $$ = blr_starting; }
	| STARTING WITH	{ $$ = blr_starting; }
	;

ternary_pattern_predicate
	: common_value ternary_pattern_operator common_value escape_opt
		{ $$ = newNode<ComparativeBoolNode>($2, $1, $3, $4); }
	| common_value NOT ternary_pattern_operator common_value escape_opt
		{
			ComparativeBoolNode* likeNode = newNode<ComparativeBoolNode>($3, $1, $4, $5);
			$$ = newNode<NotBoolNode>(makeClassNode(likeNode));
		}
	;

ternary_pattern_operator
	: LIKE			{ $$ = blr_like; }
	| SIMILAR TO	{ $$ = blr_similar; }
	;

escape_opt
	:				{ $$ = NULL; }
	| ESCAPE common_value	{ $$ = $2; }
	;

in_predicate
	: common_value KW_IN in_predicate_value
		{
			ComparativeBoolNode* node = newNode<ComparativeBoolNode>(blr_eql, $1, $3);
			node->dsqlFlag = ComparativeBoolNode::DFLAG_ANSI_ANY;
			$$ = node;
		}
	| common_value NOT KW_IN in_predicate_value
		{
			ComparativeBoolNode* node = newNode<ComparativeBoolNode>(blr_eql, $1, $4);
			node->dsqlFlag = ComparativeBoolNode::DFLAG_ANSI_ANY;
			$$ = newNode<NotBoolNode>(makeClassNode(node));
		}
	;

exists_predicate
	: EXISTS '(' select_expr ')'
		{ $$ = newNode<RseBoolNode>(blr_any, $3); }
	;

singular_predicate
	: SINGULAR '(' select_expr ')'
		{ $$ = newNode<RseBoolNode>(blr_unique, $3); }
	;

null_predicate
	: common_value IS KW_NULL
		{ $$ = newNode<MissingBoolNode>($1); }
	| common_value IS UNKNOWN
		{ $$ = newNode<MissingBoolNode>($1); }
	| common_value IS NOT KW_NULL
		{ $$ = newNode<NotBoolNode>(makeClassNode(newNode<MissingBoolNode>($1))); }
	| common_value IS NOT UNKNOWN
		{ $$ = newNode<NotBoolNode>(makeClassNode(newNode<MissingBoolNode>($1))); }
	;


// set values

in_predicate_value	: table_subquery
	| '(' value_list ')'
		{ $$ = make_list ($2); }
	;

table_subquery	: '(' column_select ')'
			{ $$ = $2; }
		;

// USER control SQL interface

create_user_clause : symbol_user_name passwd_clause firstname_opt middlename_opt lastname_opt grant_admin_opt
		{ $$ = make_node(nod_add_user, (int) e_user_count, $1, $2, $3, $4, $5, $6); }
	;

alter_user_clause : symbol_user_name passwd_opt firstname_opt middlename_opt lastname_opt admin_opt
		{ $$ = make_node(nod_mod_user, (int) e_user_count, $1, $2, $3, $4, $5, $6); }
	| symbol_user_name SET passwd_opt firstname_opt middlename_opt lastname_opt admin_opt
		{ $$ = make_node(nod_mod_user, (int) e_user_count, $1, $3, $4, $5, $6, $7); }
	;

drop_user_clause : symbol_user_name
		{ $$ = make_node(nod_del_user, (int) e_del_user_count, $1); }

passwd_clause : PASSWORD sql_string
		{ $$ = $2; }
	;

passwd_opt : passwd_clause
		{ $$ = $1; }
	|
		{ $$ = NULL; }
	;

firstname_opt : FIRSTNAME sql_string
		{ $$ = $2; }
	|
		{ $$ = NULL; }
	;

middlename_opt : MIDDLENAME sql_string
		{ $$ = $2; }
	|
		{ $$ = NULL; }
	;

lastname_opt : LASTNAME sql_string
		{ $$ = $2; }
	|
		{ $$ = NULL; }
	;

admin_opt
	: revoke_admin
		{ $$ = $1; }
	| grant_admin
		{ $$ = $1; }
	|
		{ $$ = NULL; }
	;

grant_admin_opt
	: grant_admin
		{ $$ = $1; }
	|
		{ $$ = NULL; }
	;

revoke_admin
	: REVOKE ADMIN ROLE
		{ $$ = MAKE_cstring("0"); }
	;

grant_admin
	: GRANT ADMIN ROLE
		{ $$ = MAKE_cstring("1"); }
	;

// value types

value
	: common_value
	| boolean_value_expression
		{ $$ = makeClassNode(newNode<BoolAsValueNode>(makeClassNode($1))); }
	;

common_value
		: column_name
		| array_element
		| function
			{ $$ = makeClassNode($1); }
		| u_constant
		| boolean_literal
		| parameter
		| variable
		| cast_specification
		| case_expression
		| next_value_expression
			{ $$ = makeClassNode($1); }
		| udf
			{ $$ = makeClassNode($1); }
		| '-' common_value %prec UMINUS
			{ $$ = makeClassNode(newNode<NegateNode>($2)); }
		| '+' common_value %prec UPLUS
			{ $$ = $2; }
		| common_value '+' common_value
			{
				$$ = makeClassNode(newNode<ArithmeticNode>(blr_add,
					(client_dialect < SQL_DIALECT_V6_TRANSITION), $1, $3));
			}
		| common_value CONCATENATE common_value
			{ $$ = makeClassNode(newNode<ConcatenateNode>($1, $3)); }
		| common_value COLLATE symbol_collation_name
			{ $$ = make_node (nod_collate, (int) e_coll_count, (dsql_nod*) $3, $1); }
		| common_value '-' common_value
			{
				$$ = makeClassNode(newNode<ArithmeticNode>(blr_subtract,
					(client_dialect < SQL_DIALECT_V6_TRANSITION), $1, $3));
			}
		| common_value '*' common_value
			{
				$$ = makeClassNode(newNode<ArithmeticNode>(blr_multiply,
					(client_dialect < SQL_DIALECT_V6_TRANSITION), $1, $3));
			}
		| common_value '/' common_value
			{
				$$ = makeClassNode(newNode<ArithmeticNode>(blr_divide,
					(client_dialect < SQL_DIALECT_V6_TRANSITION), $1, $3));
			}
		| '(' common_value ')'
			{ $$ = $2; }
		| '(' column_singleton ')'
			{ $$ = $2; }
		| current_user
			{ $$ = makeClassNode($1); }
		| current_role
			{ $$ = makeClassNode($1); }
		| internal_info
			{ $$ = makeClassNode($1); }
		| DB_KEY
			{ $$ = makeClassNode(newNode<RecordKeyNode>(blr_dbkey)); }
		| symbol_table_alias_name '.' DB_KEY
			{ $$ = makeClassNode(newNode<RecordKeyNode>(blr_dbkey, toName($1))); }
		| KW_VALUE
			{ $$ = make_node (nod_dom_value, 0, NULL); }
		| datetime_value_expression
			{ $$ = makeClassNode($1); }
		| null_value
			{ $$ = makeClassNode($1); }
		;

datetime_value_expression : CURRENT_DATE
		{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_dialect_datatype_unsupport) << Arg::Num(client_dialect) <<
						  												  Arg::Str("DATE"));
			}

			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_db_dialect_dtype_unsupport) << Arg::Num(db_dialect) <<
						  												  Arg::Str("DATE"));
			}

			$$ = newNode<CurrentDateNode>();
		}
	| CURRENT_TIME time_precision_opt
		{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_dialect_datatype_unsupport) << Arg::Num(client_dialect) <<
						  												  Arg::Str("TIME"));
			}

			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_db_dialect_dtype_unsupport) << Arg::Num(db_dialect) <<
						  												  Arg::Str("TIME"));
			}

			$$ = newNode<CurrentTimeNode>($2);
		}
	| CURRENT_TIMESTAMP timestamp_precision_opt
		{ $$ = newNode<CurrentTimeStampNode>($2); }
	;

time_precision_opt
	:
		{ $$ = DEFAULT_TIME_PRECISION; }
	| '(' nonneg_short_integer ')'
		{ $$ = $2; }
	;

timestamp_precision_opt
	:
		{ $$ = DEFAULT_TIMESTAMP_PRECISION; }
	| '(' nonneg_short_integer ')'
		{ $$ = $2; }
	;

array_element   : column_name '[' value_list ']'
			{ $$ = make_node (nod_array, (int) e_ary_count, $1, make_list ($3)); }
		;

common_value_list_opt
	:
		{ $$ = make_node(nod_list, 0, NULL); }
	| common_value_list
		{ $$ = make_list($1); }
	;

common_value_list
	: common_value
	| common_value_list ',' common_value
		{ $$ = make_node(nod_list, 2, $1, $3); }
	;

value_list_opt
	:
		{ $$ = make_node(nod_list, 0, NULL); }
	| value_list
		{ $$ = make_list($1); }
	;

value_list
	: value
	| value_list ',' value
		{ $$ = make_node(nod_list, 2, $1, $3); }
	;

constant
	: u_constant
	| '-' u_numeric_constant
		{ $$ = makeClassNode(newNode<NegateNode>($2)); }
	| boolean_literal
	;

u_numeric_constant : NUMERIC
			{ $$ = MAKE_constant ((dsql_str*) $1, CONSTANT_STRING); }
		| NUMBER
			{ $$ = MAKE_const_slong($1); }
		| FLOAT_NUMBER
			{ $$ = MAKE_constant ((dsql_str*) $1, CONSTANT_DOUBLE); }
		| NUMBER64BIT
			{ $$ = MAKE_constant ((dsql_str*) $1, CONSTANT_SINT64); }
		| SCALEDINT
			{ $$ = MAKE_constant ((dsql_str*) $1, CONSTANT_SINT64); }
		;

u_constant	: u_numeric_constant
	| sql_string
		{ $$ = MAKE_str_constant ($1, lex.att_charset); }
	| DATE STRING
		{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_dialect_datatype_unsupport) << Arg::Num(client_dialect) <<
						  												  Arg::Str("DATE"));
			}
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_db_dialect_dtype_unsupport) << Arg::Num(db_dialect) <<
						  												  Arg::Str("DATE"));
			}
			$$ = MAKE_constant ($2, CONSTANT_DATE);
		}
	| TIME STRING
		{
			if (client_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_dialect_datatype_unsupport) << Arg::Num(client_dialect) <<
						  												  Arg::Str("TIME"));
			}
			if (db_dialect < SQL_DIALECT_V6_TRANSITION)
			{
				ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
						  Arg::Gds(isc_sql_db_dialect_dtype_unsupport) << Arg::Num(db_dialect) <<
						  												  Arg::Str("TIME"));
			}
			$$ = MAKE_constant ($2, CONSTANT_TIME);
		}
	| TIMESTAMP STRING
		{ $$ = MAKE_constant ($2, CONSTANT_TIMESTAMP); }
		;

boolean_literal
	: KW_FALSE	{ $$ = MAKE_constant(MAKE_string("", 0), CONSTANT_BOOLEAN); }
	| KW_TRUE	{ $$ = MAKE_constant(MAKE_string("1", 1), CONSTANT_BOOLEAN); }
	;

parameter
	: '?'	{ $$ = makeClassNode(make_parameter()); }
	;

current_user
	: USER			{ $$ = newNode<CurrentUserNode>(); }
	| CURRENT_USER	{ $$ = newNode<CurrentUserNode>(); }
	;

current_role
	: CURRENT_ROLE	{ $$ = newNode<CurrentRoleNode>(); }
	;

internal_info
	: CURRENT_CONNECTION
		{
			$$ = newNode<InternalInfoNode>(
				MAKE_const_slong(SLONG(InternalInfoNode::INFO_TYPE_CONNECTION_ID)));
		}
	| CURRENT_TRANSACTION
		{
			$$ = newNode<InternalInfoNode>(
				MAKE_const_slong(SLONG(InternalInfoNode::INFO_TYPE_TRANSACTION_ID)));
		}
	| GDSCODE
		{
			$$ = newNode<InternalInfoNode>(
				MAKE_const_slong(SLONG(InternalInfoNode::INFO_TYPE_GDSCODE)));
		}
	| SQLCODE
		{
			$$ = newNode<InternalInfoNode>(
				MAKE_const_slong(SLONG(InternalInfoNode::INFO_TYPE_SQLCODE)));
		}
	| SQLSTATE
		{
			$$ = newNode<InternalInfoNode>(
				MAKE_const_slong(SLONG(InternalInfoNode::INFO_TYPE_SQLSTATE)));
		}
	| ROW_COUNT
		{
			$$ = newNode<InternalInfoNode>(
				MAKE_const_slong(SLONG(InternalInfoNode::INFO_TYPE_ROWS_AFFECTED)));
		}
	;

sql_string
	: STRING			// string in current charset
		{ $$ = $1; }
	| INTRODUCER STRING	// string in specific charset
		{
			dsql_str* str = $2;
			str->str_charset = $1;
			if (str->type == dsql_str::TYPE_SIMPLE || str->type == dsql_str::TYPE_ALTERNATE)
			{
				StrMark* mark = strMarks.get(str);
				fb_assert(mark);
				if (mark)
					mark->introduced = true;
			}
			$$ = str;
		}
	;

signed_short_integer
	: nonneg_short_integer
	| '-' neg_short_integer
		{ $$ = -$2; }
	;

nonneg_short_integer
	: NUMBER
		{
			if ($1 > SHRT_POS_MAX)
				yyabandon(-842, isc_expec_short);	// Short integer expected

			$$ = $1;
		}
	;

neg_short_integer
	: NUMBER
		{
			if ($1 > SHRT_NEG_MAX)
				yyabandon(-842, isc_expec_short);	// Short integer expected

			$$ = $1;
		}
	;

pos_short_integer
	: nonneg_short_integer
		{
			if ($1 == 0)
				yyabandon(-842, isc_expec_positive);	// Positive number expected

			$$ = $1;
		}
	;

unsigned_short_integer
	: NUMBER
		{
			if ($1 > SHRT_UNSIGNED_MAX)
				yyabandon(-842, isc_expec_ushort);	// Unsigned short integer expected

			$$ = $1;
		}
	;

signed_long_integer
	: long_integer
	| '-' long_integer
		{ $$ = -$2; }
	;

long_integer
	: NUMBER
		{ $$ = $1;}
	;


// functions

function
	: aggregate_function		{ $$ = $1; }
	| non_aggregate_function
	| over_clause
	;

non_aggregate_function
	: numeric_value_function
	| string_value_function
	| system_function_expression
	;

aggregate_function
	: COUNT '(' '*' ')'
		{ $$ = newNode<CountAggNode>(false); }
	| COUNT '(' all_noise value ')'
		{ $$ = newNode<CountAggNode>(false, $4); }
	| COUNT '(' DISTINCT value ')'
		{ $$ = newNode<CountAggNode>(true, $4); }
	| SUM '(' all_noise value ')'
		{
			$$ = newNode<SumAggNode>(false,
				(client_dialect < SQL_DIALECT_V6_TRANSITION), $4);
		}
	| SUM '(' DISTINCT value ')'
		{
			$$ = newNode<SumAggNode>(true,
				(client_dialect < SQL_DIALECT_V6_TRANSITION), $4);
		}
	| AVG '(' all_noise value ')'
		{
			$$ = newNode<AvgAggNode>(false,
				(client_dialect < SQL_DIALECT_V6_TRANSITION), $4);
		}
	| AVG '(' DISTINCT value ')'
		{
			$$ = newNode<AvgAggNode>(true,
				(client_dialect < SQL_DIALECT_V6_TRANSITION), $4);
		}
	| MINIMUM '(' all_noise value ')'
		{ $$ = newNode<MaxMinAggNode>(MaxMinAggNode::TYPE_MIN, $4); }
	| MINIMUM '(' DISTINCT value ')'
		{ $$ = newNode<MaxMinAggNode>(MaxMinAggNode::TYPE_MIN, $4); }
	| MAXIMUM '(' all_noise value ')'
		{ $$ = newNode<MaxMinAggNode>(MaxMinAggNode::TYPE_MAX, $4); }
	| MAXIMUM '(' DISTINCT value ')'
		{ $$ = newNode<MaxMinAggNode>(MaxMinAggNode::TYPE_MAX, $4); }
	| LIST '(' all_noise value delimiter_opt ')'
		{ $$ = newNode<ListAggNode>(false, $4, $5); }
	| LIST '(' DISTINCT value delimiter_opt ')'
		{ $$ = newNode<ListAggNode>(true, $4, $5); }
	;

window_function
	: DENSE_RANK '(' ')'
		{ $$ = newNode<DenseRankWinNode>(); }
	| RANK '(' ')'
		{ $$ = newNode<RankWinNode>(); }
	| ROW_NUMBER '(' ')'
		{ $$ = newNode<RowNumberWinNode>(); }
	| LAG '(' value ',' value ',' value ')'
		{ $$ = newNode<LagWinNode>($3, $5, $7); }
	| LAG '(' value ',' value ')'
		{
			$$ = newNode<LagWinNode>($3, $5,
				makeClassNode(newNode<NullNode>()));
		}
	| LAG '(' value ')'
		{
			$$ = newNode<LagWinNode>($3, MAKE_const_slong(1),
				makeClassNode(newNode<NullNode>()));
		}
	| LEAD '(' value ',' value ',' value ')'
		{ $$ = newNode<LeadWinNode>($3, $5, $7); }
	| LEAD '(' value ',' value ')'
		{
			$$ = newNode<LeadWinNode>($3, $5,
				makeClassNode(newNode<NullNode>()));
		}
	| LEAD '(' value ')'
		{
			$$ = newNode<LeadWinNode>($3, MAKE_const_slong(1),
				makeClassNode(newNode<NullNode>()));
		}
	;

aggregate_window_function
	: aggregate_function
	| window_function
	;

over_clause
	: aggregate_window_function OVER '(' window_partition_opt order_clause ')'
		{ $$ = newNode<OverNode>(makeClassNode($1), make_list($4), $5); }
	;

window_partition_opt
	:
		{ $$ = NULL; }
	| PARTITION BY value_list
		{ $$ = $3; }
	;

delimiter_opt
	: ',' value
		{ $$ = $2; }
	|
		{ $$ = MAKE_str_constant (MAKE_cstring(","), lex.att_charset); }
	;

numeric_value_function
	: extract_expression
	| length_expression
	;

extract_expression
	: EXTRACT '(' timestamp_part FROM value ')'
		{ $$ = newNode<ExtractNode>($3, $5); }
	;

length_expression
	: bit_length_expression
	| char_length_expression
	| octet_length_expression
	;

bit_length_expression
	: BIT_LENGTH '(' value ')'
		{ $$ = newNode<StrLenNode>(blr_strlen_bit, $3); }
	;

char_length_expression
	: CHAR_LENGTH '(' value ')'
		{ $$ = newNode<StrLenNode>(blr_strlen_char, $3); }
	| CHARACTER_LENGTH '(' value ')'
		{ $$ = newNode<StrLenNode>(blr_strlen_char, $3); }
	;

octet_length_expression
	: OCTET_LENGTH '(' value ')'
		{ $$ = newNode<StrLenNode>(blr_strlen_octet, $3); }
	;

system_function_expression
	: system_function_std_syntax '(' value_list_opt ')'
		{ $$ = newNode<SysFuncCallNode>(toName($1), $3); }
	| system_function_special_syntax
		{ $$ = $1; }
	;

system_function_std_syntax
	: ABS
	| ACOS
	| ACOSH
	| ASCII_CHAR
	| ASCII_VAL
	| ASIN
	| ASINH
	| ATAN
	| ATAN2
	| ATANH
	| BIN_AND
	| BIN_NOT
	| BIN_OR
	| BIN_SHL
	| BIN_SHR
	| BIN_XOR
	| CEIL
	| CHAR_TO_UUID
	| COS
	| COSH
	| COT
	| EXP
	| FLOOR
	| GEN_UUID
	| HASH
	| LEFT
	| LN
	| LOG
	| LOG10
	| LPAD
	| MAXVALUE
	| MINVALUE
	| MOD
	| PI
	| POWER
	| RAND
	| RDB_GET_CONTEXT
	| RDB_SET_CONTEXT
	| REPLACE
	| REVERSE
	| RIGHT
	| ROUND
	| RPAD
	| SIGN
	| SIN
	| SINH
	| SQRT
	| TAN
	| TANH
	| TRUNC
	| UUID_TO_CHAR
	;

system_function_special_syntax
	: DATEADD '(' value timestamp_part TO value ')'
		{
			$$ = newNode<SysFuncCallNode>(toName($1),
				make_node(nod_list, 3, $3, MAKE_const_slong($4), $6));
			$$->dsqlSpecialSyntax = true;
		}
	| DATEADD '(' timestamp_part ',' value ',' value ')'
		{
			$$ = newNode<SysFuncCallNode>(toName($1),
				make_node(nod_list, 3, $5, MAKE_const_slong($3), $7));
			$$->dsqlSpecialSyntax = true;
		}
	| DATEDIFF '(' timestamp_part FROM value TO value ')'
		{
			$$ = newNode<SysFuncCallNode>(toName($1),
				make_node(nod_list, 3, MAKE_const_slong($3), $5, $7));
			$$->dsqlSpecialSyntax = true;
		}
	| DATEDIFF '(' timestamp_part ',' value ',' value ')'
		{
			$$ = newNode<SysFuncCallNode>(toName($1),
				make_node(nod_list, 3, MAKE_const_slong($3), $5, $7));
			$$->dsqlSpecialSyntax = true;
		}
	| OVERLAY '(' value PLACING value FROM value FOR value ')'
		{
			$$ = newNode<SysFuncCallNode>(toName($1),
				make_node(nod_list, 4, $3, $5, $7, $9));
			$$->dsqlSpecialSyntax = true;
		}
	| OVERLAY '(' value PLACING value FROM value ')'
		{
			$$ = newNode<SysFuncCallNode>(toName($1),
				make_node(nod_list, 3, $3, $5, $7));
			$$->dsqlSpecialSyntax = true;
		}
	| POSITION '(' common_value KW_IN common_value ')'
		{
			$$ = newNode<SysFuncCallNode>(toName($1),
				make_node(nod_list, 2, $3, $5));
			$$->dsqlSpecialSyntax = true;
		}
	| POSITION '(' common_value_list_opt  ')'
		{ $$ = newNode<SysFuncCallNode>(toName($1), $3); }
	;

string_value_function
	: substring_function
	| trim_function
	| KW_UPPER '(' value ')'
		{ $$ = newNode<StrCaseNode>(blr_upcase, $3); }
	| KW_LOWER '(' value ')'
		{ $$ = newNode<StrCaseNode>(blr_lowcase, $3); }
	;

substring_function
	: SUBSTRING '(' value FROM value string_length_opt ')'
		{
			// SQL spec requires numbering to start with 1,
			// hence we decrement the first parameter to make it
			// compatible with the engine's implementation
			dsql_nod* subtractNode = makeClassNode(newNode<ArithmeticNode>(
				blr_subtract, true, $5, MAKE_const_slong(1)));

			$$ = newNode<SubstringNode>($3, subtractNode, $6);
		}
	| SUBSTRING '(' common_value SIMILAR common_value ESCAPE common_value ')'
		{ $$ = newNode<SubstringSimilarNode>($3, $5, $7); }
	;

string_length_opt
	:			{ $$ = MAKE_const_slong(SHRT_POS_MAX); }
	| FOR value { $$ = $2; }
	;

trim_function
	: TRIM '(' trim_specification value FROM value ')'
		{ $$ = newNode<TrimNode>($3, $6, $4); }
	| TRIM '(' value FROM value ')'
		{ $$ = newNode<TrimNode>(blr_trim_both, $5, $3); }
	| TRIM '(' trim_specification FROM value ')'
		{ $$ = newNode<TrimNode>($3, $5); }
	| TRIM '(' value ')'
		{ $$ = newNode<TrimNode>(blr_trim_both, $3); }
	;

trim_specification
	: BOTH		{ $$ = blr_trim_both; }
	| TRAILING	{ $$ = blr_trim_trailing; }
	| LEADING	{ $$ = blr_trim_leading; }
	;

udf
	: symbol_UDF_call_name '(' value_list ')'
		{ $$ = newNode<UdfCallNode>(QualifiedName(toName($1), ""), make_list($3)); }
	| symbol_UDF_call_name '(' ')'
		{ $$ = newNode<UdfCallNode>(QualifiedName(toName($1), ""), make_node(nod_list, 0)); }
	| symbol_package_name '.' symbol_UDF_name '(' value_list ')'
		{ $$ = newNode<UdfCallNode>(QualifiedName(toName($3), toName($1)), make_list($5)); }
	| symbol_package_name '.' symbol_UDF_name '(' ')'
		{ $$ = newNode<UdfCallNode>(QualifiedName(toName($3), toName($1)), make_node(nod_list, 0)); }
	;

cast_specification
	: CAST '(' value AS data_type_descriptor ')'
		{ $$ = makeClassNode(newNode<CastNode>($3, $5)); }
	;

// case expressions

case_expression	: case_abbreviation
		| case_specification
		;

case_abbreviation
	: NULLIF '(' value ',' value ')'
		{
			$$ = make_node(nod_searched_case, 2,
					make_node(nod_list, 2,
						makeClassNode(newNode<ComparativeBoolNode>(blr_eql, $3, $5)),
						makeClassNode(newNode<NullNode>())),
					$3);
		}
	| IIF '(' search_condition ',' value ',' value ')'
		{ $$ = makeClassNode(newNode<ValueIfNode>($3, $5, $7)); }
	| COALESCE '(' value ',' value_list ')'
		{ $$ = make_node (nod_coalesce, 2, $3, $5); }
	| DECODE '(' value ',' decode_pairs ')'
		{
			$$ = make_node(nod_simple_case, 3, $3, make_list($5),
				makeClassNode(newNode<NullNode>()));
		}
	| DECODE '(' value ',' decode_pairs ',' value ')'
		{ $$ = make_node(nod_simple_case, 3, $3, make_list($5), $7); }
	;

case_specification	: simple_case
		| searched_case
		;

simple_case
	: CASE case_operand simple_when_clause END
		{
			$$ = make_node (nod_simple_case, 3, $2, make_list($3),
				makeClassNode(newNode<NullNode>()));
		}
	| CASE case_operand simple_when_clause ELSE case_result END
		{ $$ = make_node (nod_simple_case, 3, $2, make_list($3), $5); }
	;

simple_when_clause	: WHEN when_operand THEN case_result
				{ $$ = make_node (nod_list, 2, $2, $4); }
			| simple_when_clause WHEN when_operand THEN case_result
				{ $$ = make_node (nod_list, 2, $1, make_node (nod_list, 2, $3, $5)); }
			;

searched_case
	: CASE searched_when_clause END
		{
			$$ = make_node (nod_searched_case, 2, make_list($2),
				makeClassNode(newNode<NullNode>()));
		}
	| CASE searched_when_clause ELSE case_result END
		{ $$ = make_node (nod_searched_case, 2, make_list($2), $4); }
	;

searched_when_clause	: WHEN search_condition THEN case_result
			{ $$ = make_node (nod_list, 2, $2, $4); }
		| searched_when_clause WHEN search_condition THEN case_result
			{ $$ = make_node (nod_list, 2, $1, make_node (nod_list, 2, $3, $5)); }
		;

when_operand	: value
		;

case_operand	: value
		;

case_result	: value
		;

decode_pairs
	: value ',' value
		{ $$ = make_node(nod_list, 2, $1, $3); }
	| decode_pairs ',' value ',' value
		{ $$ = make_node(nod_list, 2, $1, make_node(nod_list, 2, $3, $5)); }
	;

// next value expression

next_value_expression
	: NEXT KW_VALUE FOR symbol_generator_name
		{
			$$ = newNode<GenIdNode>((client_dialect < SQL_DIALECT_V6_TRANSITION),
				toName($4), MAKE_const_slong(1));
		}
	| GEN_ID '(' symbol_generator_name ',' value ')'
		{ $$ = newNode<GenIdNode>((client_dialect < SQL_DIALECT_V6_TRANSITION), toName($3), $5); }
	;


timestamp_part
	: YEAR			{ $$ = blr_extract_year; }
	| MONTH			{ $$ = blr_extract_month; }
	| DAY			{ $$ = blr_extract_day; }
	| HOUR			{ $$ = blr_extract_hour; }
	| MINUTE		{ $$ = blr_extract_minute; }
	| SECOND		{ $$ = blr_extract_second; }
	| MILLISECOND	{ $$ = blr_extract_millisecond; }
	| WEEK			{ $$ = blr_extract_week; }
	| WEEKDAY		{ $$ = blr_extract_weekday; }
	| YEARDAY		{ $$ = blr_extract_yearday; }
	;

all_noise
	:
	| ALL
	;

distinct_noise
	:
	| DISTINCT
	;

null_value
	: KW_NULL	{ $$ = newNode<NullNode>(); }
	| UNKNOWN	{ $$ = newNode<NullNode>(); }
	;


// Performs special mapping of keywords into symbols

symbol_UDF_call_name	: SYMBOL
	;

symbol_UDF_name	: valid_symbol_name
	;

symbol_blob_subtype_name	: valid_symbol_name
	;

symbol_character_set_name	: valid_symbol_name
	;

symbol_collation_name	: valid_symbol_name
	;

symbol_column_name	: valid_symbol_name
	;

symbol_constraint_name	: valid_symbol_name
	;

symbol_cursor_name	: valid_symbol_name
	;

symbol_domain_name	: valid_symbol_name
	;

symbol_exception_name	: valid_symbol_name
	;

symbol_filter_name	: valid_symbol_name
	;

symbol_gdscode_name	: valid_symbol_name
	;

symbol_generator_name	: valid_symbol_name
	;

symbol_index_name	: valid_symbol_name
	;

symbol_item_alias_name	: valid_symbol_name
	;

symbol_label_name	: valid_symbol_name
	;

symbol_ddl_name	: valid_symbol_name
	;

symbol_procedure_name	: valid_symbol_name
	;

symbol_role_name	: valid_symbol_name
	;

symbol_table_alias_name	: valid_symbol_name
	;

symbol_table_name	: valid_symbol_name
	;

symbol_trigger_name	: valid_symbol_name
	;

symbol_user_name	: valid_symbol_name
	;

symbol_variable_name	: valid_symbol_name
	;

symbol_view_name	: valid_symbol_name
	;

symbol_savepoint_name	: valid_symbol_name
	;

symbol_package_name
	: valid_symbol_name
	;

// symbols

valid_symbol_name	: SYMBOL
	| non_reserved_word
	;

// list of non-reserved words

non_reserved_word :
	ACTION					// added in IB 5.0/
	| CASCADE
	| FREE_IT
	| RESTRICT
	| ROLE
	| KW_TYPE				// added in IB 6.0
	| KW_BREAK				// added in FB 1.0
	| KW_DESCRIPTOR
	| SUBSTRING
	| COALESCE				// added in FB 1.5
	| LAST
	| LEAVE
	| LOCK
	| NULLIF
	| NULLS
	| STATEMENT
	| INSERTING
	| UPDATING
	| DELETING
	| FIRST
	| SKIP
	| BLOCK					// added in FB 2.0
	| BACKUP
	| KW_DIFFERENCE
	| IIF
	| SCALAR_ARRAY
	| WEEKDAY
	| YEARDAY
	| SEQUENCE
	| NEXT
	| RESTART
	| COLLATION
	| RETURNING
	| KW_IGNORE
	| LIMBO
	| UNDO
	| REQUESTS
	| TIMEOUT
	| ABS					// added in FB 2.1
	| ACCENT
	| ACOS
	| ALWAYS
	| ASCII_CHAR
	| ASCII_VAL
	| ASIN
	| ATAN
	| ATAN2
	| BIN_AND
	| BIN_OR
	| BIN_SHL
	| BIN_SHR
	| BIN_XOR
	| CEIL
	| COS
	| COSH
	| COT
	| DATEADD
	| DATEDIFF
	| DECODE
	| EXP
	| FLOOR
	| GEN_UUID
	| GENERATED
	| HASH
	| LIST
	| LN
	| LOG
	| LOG10
	| LPAD
	| MATCHED
	| MATCHING
	| MAXVALUE
	| MILLISECOND
	| MINVALUE
	| MOD
	| OVERLAY
	| PAD
	| PI
	| PLACING
	| POWER
	| PRESERVE
	| RAND
	| REPLACE
	| REVERSE
	| ROUND
	| RPAD
	| SIGN
	| SIN
	| SINH
	| SPACE
	| SQRT
	| TAN
	| TANH
	| TEMPORARY
	| TRUNC
	| WEEK
	| AUTONOMOUS			// added in FB 2.5
	| CHAR_TO_UUID
	| FIRSTNAME
	| MIDDLENAME
	| LASTNAME
	| MAPPING
	| OS_NAME
	| UUID_TO_CHAR
	| GRANTED
	| CALLER				// new execute statement
	| COMMON
	| DATA
	| SOURCE
	| TWO_PHASE
	| BIN_NOT
	| ACTIVE				// old keywords, that were reserved pre-Firebird.2.5
//	| ADD					// words commented it this list remain reserved due to conflicts
	| AFTER
	| ASC
	| AUTO
	| BEFORE
	| COMMITTED
	| COMPUTED
	| CONDITIONAL
	| CONTAINING
	| CSTRING
	| DATABASE
//	| DB_KEY
	| DESC
	| DO
	| DOMAIN
	| ENTRY_POINT
	| EXCEPTION
	| EXIT
	| KW_FILE
//	| GDSCODE
	| GENERATOR
	| GEN_ID
	| IF
	| INACTIVE
//	| INDEX
	| INPUT_TYPE
	| ISOLATION
	| KEY
	| LENGTH
	| LEVEL
//	| KW_LONG
	| MANUAL
	| MODULE_NAME
	| NAMES
	| OPTION
	| OUTPUT_TYPE
	| OVERFLOW
	| PAGE
	| PAGES
	| KW_PAGE_SIZE
	| PASSWORD
//	| PLAN
//	| POST_EVENT
	| PRIVILEGES
	| PROTECTED
	| READ
	| RESERVING
	| RETAIN
//	| RETURNING_VALUES
	| SEGMENT
	| SHADOW
	| KW_SHARED
	| SINGULAR
	| KW_SIZE
	| SNAPSHOT
	| SORT
//	| SQLCODE
	| STABILITY
	| STARTING
	| STATISTICS
	| SUB_TYPE
	| SUSPEND
	| TRANSACTION
	| UNCOMMITTED
//	| VARIABLE
//	| VIEW
	| WAIT
//	| WEEK
//	| WHILE
	| WORK
	| WRITE				// end of old keywords, that were reserved pre-Firebird.2.5
	| KW_ABSOLUTE		// added in FB 3.0
	| ACOSH
	| ASINH
	| ATANH
	| BODY
	| CONTINUE
	| DDL
	| ENGINE
	| IDENTITY
	| NAME
	| PACKAGE
	| PARTITION
	| PRIOR
	| RDB_GET_CONTEXT
	| RDB_SET_CONTEXT
	| KW_RELATIVE
	| DENSE_RANK
	| LAG
	| LEAD
	| RANK
	| ROW_NUMBER
	;

%%


/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		lex.c
 *	DESCRIPTION:	Lexical routine
 *
 */


void LEX_dsql_init(MemoryPool& pool)
{
/**************************************
 *
 *	L E X _ d s q l _ i n i t
 *
 **************************************
 *
 * Functional description
 *	Initialize LEX for processing.  This is called only once
 *	per session.
 *
 **************************************/
	for (const TOK* token = KEYWORD_getTokens(); token->tok_string; ++token)
	{
		dsql_sym* symbol = FB_NEW_RPT(pool, 0) dsql_sym;
		symbol->sym_string = token->tok_string;
		symbol->sym_length = strlen(token->tok_string);
		symbol->sym_type = SYM_keyword;
		symbol->sym_keyword = token->tok_ident;
		symbol->sym_version = token->tok_version;
		dsql_str* str = FB_NEW_RPT(pool, symbol->sym_length) dsql_str;
		str->str_length = symbol->sym_length;
		strncpy(str->str_data, symbol->sym_string, symbol->sym_length);
		//str->str_data[str->str_length] = 0; Is it necessary?
		symbol->sym_object = (void *) str;
		HSHD_insert(symbol);
	}
}


const TEXT* Parser::lex_position()
{
/**************************************
 *
 *	l e x _ p o s i t i o n
 *
 **************************************
 *
 * Functional description
 *	Return the current position of LEX
 *	in the input string.
 *
 **************************************/

	return lex.ptr;
}


#ifdef NOT_USED_OR_REPLACED
static bool long_int(dsql_nod* string, SLONG* long_value)
{
/*************************************
 *
 *	l o n g _ i n t
 *
 *************************************
 *
 * Functional description
 *	checks for all digits in the
 *	number and return an atol().
 *
 *************************************/

	const char* data = ((dsql_str*) string)->str_data;
	for (const UCHAR* p = (UCHAR*) data; true; p++)
	{
		if (!(classes(*p) & CHR_DIGIT)) {
			return false;
		}
	}

	*long_value = atol(data);

	return true;
}
#endif


static dsql_fld* make_field (dsql_nod* field_name)
{
/**************************************
 *
 *	m a k e _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Make a field block of given name.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	if (field_name == NULL)
	{
		dsql_fld* field = FB_NEW(*tdbb->getDefaultPool()) dsql_fld(*tdbb->getDefaultPool());
		field->fld_name = INTERNAL_FIELD_NAME;
		return field;
	}
	const dsql_str* string = (dsql_str*) field_name->nod_arg[1];
	dsql_fld* field = FB_NEW(*tdbb->getDefaultPool()) dsql_fld(*tdbb->getDefaultPool());
	field->fld_name = string->str_data;
	field->fld_explicit_collation = false;
	field->fld_not_nullable = false;
	field->fld_full_domain = false;

	return field;
}


static dsql_fil* make_file()
{
/**************************************
 *
 *	m a k e _ f i l e
 *
 **************************************
 *
 * Functional description
 *	Make a file block
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	dsql_fil* temp_file = FB_NEW(*tdbb->getDefaultPool()) dsql_fil;

	return temp_file;
}


dsql_nod* Parser::make_list (dsql_nod* node)
{
/**************************************
 *
 *	m a k e _ l i s t
 *
 **************************************
 *
 * Functional description
 *	Collapse nested list nodes into single list.
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	if (node)
	{
		DsqlNodStack stack;
		stack_nodes(node, stack);
		USHORT l = stack.getCount();

		const dsql_nod* old = node;
		node = FB_NEW_RPT(*tdbb->getDefaultPool(), l) dsql_nod;
		node->nod_count = l;
		node->nod_type = nod_list;
		node->nod_line = (USHORT) lex.lines_bk;
		node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
		if (old->getType() == dsql_type_nod)
		{
			node->nod_flags = old->nod_flags;
		}
		dsql_nod** ptr = node->nod_arg + node->nod_count;

		while (stack.hasData())
			*--ptr = stack.pop();
	}

	return node;
}


// Make parameter node.
ParameterNode* Parser::make_parameter()
{
	thread_db* tdbb = JRD_get_thread_data();

	ParameterNode* node = FB_NEW(*tdbb->getDefaultPool()) ParameterNode(*tdbb->getDefaultPool());
	node->dsqlParameterIndex = lex.param_number++;

	return node;
}


dsql_nod* Parser::make_node(NOD_TYPE type, int count, ...)
{
/**************************************
 *
 *	m a k e _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Make a node of given type.
 *	Any change should also be made to function below
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	dsql_nod* node = FB_NEW_RPT(*tdbb->getDefaultPool(), count) dsql_nod;
	node->nod_type = type;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	dsql_nod** p = node->nod_arg;
	va_list	ptr;
	va_start (ptr, count);

	while (--count >= 0)
		*p++ = va_arg (ptr, dsql_nod*);

	va_end(ptr);
	return node;
}


dsql_nod* Parser::makeClassNode(ExprNode* node)
{
	return make_node(nod_class_exprnode, 1, reinterpret_cast<dsql_nod*>(node));
}

dsql_nod* Parser::makeClassNode(DdlNode* node)
{
	return make_node(nod_class_stmtnode, 1, reinterpret_cast<dsql_nod*>(node));
}

dsql_nod* Parser::makeClassNode(StmtNode* node)
{
	return make_node(nod_class_stmtnode, 1, reinterpret_cast<dsql_nod*>(node));
}


dsql_nod* Parser::make_flag_node(NOD_TYPE type, SSHORT flag, int count, ...)
{
/**************************************
 *
 *	m a k e _ f l a g _ n o d e
 *
 **************************************
 *
 * Functional description
 *	Make a node of given type. Set flag field
 *
 **************************************/
	thread_db* tdbb = JRD_get_thread_data();

	dsql_nod* node = FB_NEW_RPT(*tdbb->getDefaultPool(), count) dsql_nod;
	node->nod_type = type;
	node->nod_flags = flag;
	node->nod_line = (USHORT) lex.lines_bk;
	node->nod_column = (USHORT) (lex.last_token_bk - lex.line_start_bk + 1);
	node->nod_count = count;
	dsql_nod** p = node->nod_arg;
	va_list	ptr;
	va_start (ptr, count);

	while (--count >= 0)
		*p++ = va_arg (ptr, dsql_nod*);

	va_end(ptr);
	return node;
}


#ifdef NOT_USED_OR_REPLACED
static bool short_int(dsql_nod* string, SLONG* long_value, SSHORT range)
{
/*************************************
 *
 *	s h o r t _ i n t
 *
 *************************************
 *
 * Functional description
 *	is the string a valid representation
 *	of a positive short int?
 *
 *************************************/

	if (((dsql_str*) string)->str_length > 5) {
		return false;
	}

	for (UCHAR* p = (UCHAR*)((dsql_str*) string)->str_data; true; p++)
	{
		if (!(classes(*p) & CHR_DIGIT)) {
			return false;
		}
	}

	// there are 5 or fewer digits, it's value may still be greater than 32767...

	SCHAR buf[10];
	buf[0] = ((dsql_str*) string)->str_data[0];
	buf[1] = ((dsql_str*) string)->str_data[1];
	buf[2] = ((dsql_str*) string)->str_data[2];
	buf[3] = ((dsql_str*) string)->str_data[3];
	buf[4] = ((dsql_str*) string)->str_data[4];
	buf[5] = '\0';

	*long_value = atoi (buf);

	bool return_value;

	switch (range)
	{
		case POSITIVE:
			return_value = *long_value > SHRT_POS_MAX;
			break;
		case NEGATIVE:
			return_value = *long_value > SHRT_NEG_MAX;
			break;
		case UNSIGNED:
			return_value = *long_value > SHRT_UNSIGNED_MAX;
			break;
	}
	return !return_value;
}
#endif

static void stack_nodes (dsql_nod* node, DsqlNodStack& stack)
{
/**************************************
 *
 *	s t a c k _ n o d e s
 *
 **************************************
 *
 * Functional description
 *	Assist in turning a tree of misc nodes into a clean list.
 *
 **************************************/
	if (node->nod_type != nod_list)
	{
		stack.push(node);
		return;
	}

	/* To take care of cases where long lists of nodes are in a chain
	   of list nodes with exactly one entry, this algorithm will look
	   for a pattern of repeated list nodes with two entries, the first
	   being a list node and the second being a non-list node.   Such
	   a list will be reverse linked, and then re-reversed, stacking the
	   non-list nodes in the process.   The purpose of this is to avoid
	   massive recursion of this function. */

	dsql_nod* start_chain = node;
	dsql_nod* end_chain = NULL;
	dsql_nod* curr_node = node;
	dsql_nod* next_node = node->nod_arg[0];
	while ( curr_node->nod_count == 2 &&
			curr_node->nod_arg[0]->nod_type == nod_list &&
			curr_node->nod_arg[1]->nod_type != nod_list &&
			next_node->nod_arg[0]->nod_type == nod_list &&
			next_node->nod_arg[1]->nod_type != nod_list)
	{

		// pattern was found so reverse the links and go to next node

		dsql_nod* save_link = next_node->nod_arg[0];
		next_node->nod_arg[0] = curr_node;
		curr_node = next_node;
		next_node = save_link;
		end_chain = curr_node;
	}

	// see if any chain was found

	if (end_chain)
	{

		// first, handle the rest of the nodes
		// note that next_node still points to the first non-pattern node

		stack_nodes (next_node, stack);

		// stack the non-list nodes and reverse the chain on the way back

		curr_node = end_chain;
		while (true)
		{
			stack.push(curr_node->nod_arg[1]);
			if (curr_node == start_chain)
				break;
			dsql_nod* save_link = curr_node->nod_arg[0];
			curr_node->nod_arg[0] = next_node;
			next_node = curr_node;
			curr_node = save_link;
		}
		return;
	}

	dsql_nod** ptr = node->nod_arg;
	for (const dsql_nod* const* const end = ptr + node->nod_count; ptr < end; ptr++)
		stack_nodes (*ptr, stack);
}

static MetaName toName(dsql_nod* node)
{
	if (!node)
		return "";

	dsql_str* str = (dsql_str*) node;

	if (str->str_length > MAX_SQL_IDENTIFIER_LEN)
		yyabandon(-104, isc_dyn_name_longer);

	return MetaName(str->str_data);
}

static string toString(dsql_str* node)
{
	return string(node->str_data);
}

int Parser::yylex()
{
	lex.prev_keyword = yylexAux();
	return lex.prev_keyword;
}

int Parser::yylexAux()
{
/**************************************
 *
 *	y y l e x A u x
 *
 **************************************
 *
 * Functional description: lexer.
 *
 **************************************/
	UCHAR tok_class;
	char string[MAX_TOKEN_LEN];
	SSHORT c;

	// Find end of white space and skip comments

	for (;;)
	{
		if (lex.ptr >= lex.end)
			return -1;

		c = *lex.ptr++;

		// Process comments

		if (c == '\n')
		{
			lex.lines++;
			lex.line_start = lex.ptr;
			continue;
		}

		if (c == '-' && lex.ptr < lex.end && *lex.ptr == '-')
		{
			// single-line

			lex.ptr++;
			while (lex.ptr < lex.end)
			{
				if ((c = *lex.ptr++) == '\n')
				{
					lex.lines++;
					lex.line_start = lex.ptr; // + 1; // CVC: +1 left out.
					break;
				}
			}
			if (lex.ptr >= lex.end)
				return -1;

			continue;
		}
		else if (c == '/' && lex.ptr < lex.end && *lex.ptr == '*')
		{
			// multi-line

			const TEXT& start_block = lex.ptr[-1];
			lex.ptr++;
			while (lex.ptr < lex.end)
			{
				if ((c = *lex.ptr++) == '*')
				{
					if (*lex.ptr == '/')
						break;
				}
				if (c == '\n')
				{
					lex.lines++;
					lex.line_start = lex.ptr; // + 1; // CVC: +1 left out.

				}
			}
			if (lex.ptr >= lex.end)
			{
				// I need this to report the correct beginning of the block,
				// since it's not a token really.
				lex.last_token = &start_block;
				yyerror("unterminated block comment");
				return -1;
			}
			lex.ptr++;
			continue;
		}

		tok_class = classes(c);

		if (!(tok_class & CHR_WHITE))
			break;
	}

	// Depending on tok_class of token, parse token

	lex.last_token = lex.ptr - 1;

	if (tok_class & CHR_INTRODUCER)
	{
		// The Introducer (_) is skipped, all other idents are copied
		// to become the name of the character set.
		char* p = string;
		for (; lex.ptr < lex.end && classes(*lex.ptr) & CHR_IDENT; lex.ptr++)
		{
			if (lex.ptr >= lex.end)
				return -1;

			check_copy_incr(p, UPPER7(*lex.ptr), string);
		}

		check_bound(p, string);
		*p = 0;

		// make a string value to hold the name, the name
		// is resolved in pass1_constant

		yylval.textPtr = MAKE_string(string, p - string)->str_data;

		return INTRODUCER;
	}

	// parse a quoted string, being sure to look for double quotes

	if (tok_class & CHR_QUOTE)
	{
		StrMark mark;
		mark.pos = lex.last_token - lex.start;

		char* buffer = string;
		size_t buffer_len = sizeof (string);
		const char* buffer_end = buffer + buffer_len - 1;
		char* p;
		for (p = buffer; ; ++p)
		{
			if (lex.ptr >= lex.end)
			{
				if (buffer != string)
					gds__free (buffer);
				yyerror("unterminated string");
				return -1;
			}
			// Care about multi-line constants and identifiers
			if (*lex.ptr == '\n')
			{
				lex.lines++;
				lex.line_start = lex.ptr + 1;
			}
			// *lex.ptr is quote - if next != quote we're at the end
			if ((*lex.ptr == c) && ((++lex.ptr == lex.end) || (*lex.ptr != c)))
				break;
			if (p > buffer_end)
			{
				char* const new_buffer = (char*) gds__alloc (2 * buffer_len);
				// FREE: at outer block
				if (!new_buffer)		// NOMEM:
				{
					if (buffer != string)
						gds__free (buffer);
					return -1;
				}
				memcpy (new_buffer, buffer, buffer_len);
				if (buffer != string)
					gds__free (buffer);
				buffer = new_buffer;
				p = buffer + buffer_len;
				buffer_len = 2 * buffer_len;
				buffer_end = buffer + buffer_len - 1;
			}
			*p = *lex.ptr++;
		}
		if (c == '"')
		{
			stmt_ambiguous = true;
			// string delimited by double quotes could be
			// either a string constant or a SQL delimited
			// identifier, therefore marks the SQL statement as ambiguous

			if (client_dialect == SQL_DIALECT_V6_TRANSITION)
			{
				if (buffer != string)
					gds__free (buffer);
				yyabandon (-104, isc_invalid_string_constant);
			}
			else if (client_dialect >= SQL_DIALECT_V6)
			{
				if (p - buffer >= MAX_TOKEN_LEN)
				{
					if (buffer != string)
						gds__free (buffer);
					yyabandon(-104, isc_token_too_long);
				}
				else if (p - buffer > MAX_SQL_IDENTIFIER_LEN)
				{
					if (buffer != string)
						gds__free (buffer);
					yyabandon(-104, isc_dyn_name_longer);
				}
				else if (p - buffer == 0)
				{
					if (buffer != string)
						gds__free (buffer);
					yyabandon(-104, isc_dyn_zero_len_id);
				}

				thread_db* tdbb = JRD_get_thread_data();
				Attachment* attachment = tdbb->getAttachment();
				MetaName name(attachment->nameToMetaCharSet(tdbb, MetaName(buffer, p - buffer)));

				yylval.legacyStr = MAKE_string(name.c_str(), name.length());

				dsql_str* delimited_id_str = yylval.legacyStr;
				delimited_id_str->type = dsql_str::TYPE_DELIMITED;
				if (buffer != string)
					gds__free (buffer);

				return SYMBOL;
			}
		}
		yylval.legacyStr = MAKE_string(buffer, p - buffer);
		if (buffer != string)
			gds__free (buffer);

		mark.length = lex.ptr - lex.last_token;
		mark.str = yylval.legacyStr;
		strMarks.put(mark.str, mark);

		return STRING;
	}

	/*
	 * Check for a numeric constant, which starts either with a digit or with
	 * a decimal point followed by a digit.
	 *
	 * This code recognizes the following token types:
	 *
	 * NUMBER: string of digits which fits into a 32-bit integer
	 *
	 * NUMBER64BIT: string of digits whose value might fit into an SINT64,
	 *   depending on whether or not there is a preceding '-', which is to
	 *   say that "9223372036854775808" is accepted here.
	 *
	 * SCALEDINT: string of digits and a single '.', where the digits
	 *   represent a value which might fit into an SINT64, depending on
	 *   whether or not there is a preceding '-'.
	 *
	 * FLOAT: string of digits with an optional '.', and followed by an "e"
	 *   or "E" and an optionally-signed exponent.
	 *
	 * NOTE: we swallow leading or trailing blanks, but we do NOT accept
	 *   embedded blanks:
	 *
	 * Another note: c is the first character which need to be considered,
	 *   ptr points to the next character.
	 */

	fb_assert(lex.ptr <= lex.end);

	// Hexadecimal string constant.  This is treated the same as a
	// string constant, but is defined as: X'bbbb'
	//
	// Where the X is a literal 'x' or 'X' character, followed
	// by a set of nibble values in single quotes.  The nibble
	// can be 0-9, a-f, or A-F, and is converted from the hex.
	// The number of nibbles should be even.
	//
	// The resulting value is stored in a string descriptor and
	// returned to the parser as a string.  This can be stored
	// in a character or binary item.
	if ((c == 'x' || c == 'X') && lex.ptr < lex.end && *lex.ptr == '\'')
	{
		bool hexerror = false;

		// Remember where we start from, to rescan later.
		// Also we'll need to know the length of the buffer.

		const char* hexstring = ++lex.ptr;
		int charlen = 0;

		// Time to scan the string. Make sure the characters are legal,
		// and find out how long the hex digit string is.

		for (;;)
		{
			if (lex.ptr >= lex.end)	// Unexpected EOS
			{
				hexerror = true;
				break;
			}

			c = *lex.ptr;

			if (c == '\'')			// Trailing quote, done
			{
				++lex.ptr;			// Skip the quote
				break;
			}

			if (!(classes(c) & CHR_HEX))	// Illegal character
			{
				hexerror = true;
				break;
			}

			++charlen;	// Okay, just count 'em
			++lex.ptr;	// and advance...
		}

		hexerror = hexerror || (charlen & 1);	// IS_ODD(charlen)

		// If we made it this far with no error, then convert the string.
		if (!hexerror)
		{
			// Figure out the length of the actual resulting hex string.
			// Allocate a second temporary buffer for it.

			Firebird::string temp;

			// Re-scan over the hex string we got earlier, converting
			// adjacent bytes into nibble values.  Every other nibble,
			// write the saved byte to the temp space.  At the end of
			// this, the temp.space area will contain the binary
			// representation of the hex constant.

			UCHAR byte = 0;
			for (int i = 0; i < charlen; i++)
			{
				c = UPPER7(hexstring[i]);

				// Now convert the character to a nibble

				if (c >= 'A')
					c = (c - 'A') + 10;
				else
					c = (c - '0');

				if (i & 1) // nibble?
				{
					byte = (byte << 4) + (UCHAR) c;
					temp.append(1, (char) byte);
				}
				else
					byte = c;
			}

			dsql_str* string = MAKE_string(temp.c_str(), temp.length());
			string->type = dsql_str::TYPE_HEXA;
			string->str_charset = "BINARY";
			yylval.legacyStr = string;

			return STRING;
		}  // if (!hexerror)...

		// If we got here, there was a parsing error.  Set the
		// position back to where it was before we messed with
		// it.  Then fall through to the next thing we might parse.

		c = *lex.last_token;
		lex.ptr = lex.last_token + 1;
	}

	if ((c == 'q' || c == 'Q') && lex.ptr + 3 < lex.end && *lex.ptr == '\'')
	{
		StrMark mark;
		mark.pos = lex.last_token - lex.start;

		char endChar = *++lex.ptr;
		switch (endChar)
		{
			case '{':
				endChar = '}';
				break;
			case '(':
				endChar = ')';
				break;
			case '[':
				endChar = ']';
				break;
			case '<':
				endChar = '>';
				break;
		}

		while (++lex.ptr + 1 < lex.end)
		{
			if (*lex.ptr == endChar && *++lex.ptr == '\'')
			{
				yylval.legacyStr = MAKE_string(lex.last_token + 3, lex.ptr - lex.last_token - 4);
				yylval.legacyStr->type = dsql_str::TYPE_ALTERNATE;
				lex.ptr++;

				mark.length = lex.ptr - lex.last_token;
				mark.str = yylval.legacyStr;
				strMarks.put(mark.str, mark);

				return STRING;
			}
		}

		// If we got here, there was a parsing error.  Set the
		// position back to where it was before we messed with
		// it.  Then fall through to the next thing we might parse.

		c = *lex.last_token;
		lex.ptr = lex.last_token + 1;
	}

	// Hexadecimal numeric constants - 0xBBBBBB
	//
	// where the '0' and the 'X' (or 'x') are literal, followed
	// by a set of nibbles, using 0-9, a-f, or A-F.  Odd numbers
	// of nibbles assume a leading '0'.  The result is converted
	// to an integer, and the result returned to the caller.  The
	// token is identified as a NUMBER if it's a 32-bit or less
	// value, or a NUMBER64INT if it requires a 64-bit number.
	if (c == '0' && lex.ptr + 1 < lex.end && (*lex.ptr == 'x' || *lex.ptr == 'X') &&
		(classes(lex.ptr[1]) & CHR_HEX))
	{
		bool hexerror = false;

		// Remember where we start from, to rescan later.
		// Also we'll need to know the length of the buffer.

		++lex.ptr;  // Skip the 'X' and point to the first digit
		const char* hexstring = lex.ptr;
		int charlen = 0;

		// Time to scan the string. Make sure the characters are legal,
		// and find out how long the hex digit string is.

		for (;;)
		{
			if (lex.ptr >= lex.end)			// Unexpected EOS
			{
				hexerror = true;
				break;
			}

			c = *lex.ptr;

			if (!(classes(c) & CHR_HEX))	// End of digit string
				break;

			++charlen;			// Okay, just count 'em
			++lex.ptr;			// and advance...

			if (charlen > 16)	// Too many digits...
			{
				hexerror = true;
				break;
			}
		}

		// we have a valid hex token. Now give it back, either as
		// an NUMBER or NUMBER64BIT.
		if (!hexerror)
		{
			// if charlen > 8 (something like FFFF FFFF 0, w/o the spaces)
			// then we have to return a NUMBER64BIT. We'll make a string
			// node here, and let make.cpp worry about converting the
			// string to a number and building the node later.
			if (charlen > 8)
			{
				char cbuff[32];
				cbuff[0] = 'X';
				strncpy(&cbuff[1], hexstring, charlen);
				cbuff[charlen + 1] = '\0';

				char* p = &cbuff[1];

				while (*p != '\0')
				{
					if ((*p >= 'a') && (*p <= 'f'))
						*p = UPPER(*p);
					p++;
				}

				yylval.legacyNode = (dsql_nod*) MAKE_string(cbuff, strlen(cbuff));
				return NUMBER64BIT;
			}
			else
			{
				// we have an integer value. we'll return NUMBER.
				// but we have to make a number value to be compatible
				// with existing code.

				// See if the string length is odd.  If so,
				// we'll assume a leading zero.  Then figure out the length
				// of the actual resulting hex string.  Allocate a second
				// temporary buffer for it.

				bool nibble = (charlen & 1);  // IS_ODD(temp.length)

				// Re-scan over the hex string we got earlier, converting
				// adjacent bytes into nibble values.  Every other nibble,
				// write the saved byte to the temp space.  At the end of
				// this, the temp.space area will contain the binary
				// representation of the hex constant.

				UCHAR byte = 0;
				SINT64 value = 0;

				for (int i = 0; i < charlen; i++)
				{
					c = UPPER(hexstring[i]);

					// Now convert the character to a nibble

					if (c >= 'A')
						c = (c - 'A') + 10;
					else
						c = (c - '0');

					if (nibble)
					{
						byte = (byte << 4) + (UCHAR) c;
						nibble = false;
						value = (value << 8) + byte;
					}
					else
					{
						byte = c;
						nibble = true;
					}
				}

				yylval.int32Val = (SLONG) value;
				return NUMBER;
			} // integer value
		}  // if (!hexerror)...

		// If we got here, there was a parsing error.  Set the
		// position back to where it was before we messed with
		// it.  Then fall through to the next thing we might parse.

		c = *lex.last_token;
		lex.ptr = lex.last_token + 1;
	} // headecimal numeric constants

	if ((tok_class & CHR_DIGIT) ||
		((c == '.') && (lex.ptr < lex.end) && (classes(*lex.ptr) & CHR_DIGIT)))
	{
		// The following variables are used to recognize kinds of numbers.

		bool have_error	 = false;	// syntax error or value too large
		bool have_digit	 = false;	// we've seen a digit
		bool have_decimal   = false;	// we've seen a '.'
		bool have_exp	   = false;	// digit ... [eE]
		bool have_exp_sign  = false; // digit ... [eE] {+-]
		bool have_exp_digit = false; // digit ... [eE] ... digit
		FB_UINT64 number		= 0;
		FB_UINT64 limit_by_10	= MAX_SINT64 / 10;

		for (--lex.ptr; lex.ptr < lex.end; lex.ptr++)
		{
			c = *lex.ptr;
			if (have_exp_digit && (! (classes(c) & CHR_DIGIT)))
				// First non-digit after exponent and digit terminates the token.
				break;

			if (have_exp_sign && (! (classes(c) & CHR_DIGIT)))
			{
				// only digits can be accepted after "1E-"
				have_error = true;
				break;
			}

			if (have_exp)
			{
				// We've seen e or E, but nothing beyond that.
				if ( ('-' == c) || ('+' == c) )
					have_exp_sign = true;
				else if ( classes(c) & CHR_DIGIT )
					// We have a digit: we haven't seen a sign yet, but it's too late now.
					have_exp_digit = have_exp_sign  = true;
				else
				{
					// end of the token
					have_error = true;
					break;
				}
			}
			else if ('.' == c)
			{
				if (!have_decimal)
					have_decimal = true;
				else
				{
					have_error = true;
					break;
				}
			}
			else if (classes(c) & CHR_DIGIT)
			{
				// Before computing the next value, make sure there will be no overflow.

				have_digit = true;

				if (number >= limit_by_10)
				{
					// possibility of an overflow
					if ((number > limit_by_10) || (c > '8'))
					{
						have_error = true;
						break;
					}
				}
				number = number * 10 + (c - '0');
			}
			else if ( (('E' == c) || ('e' == c)) && have_digit )
				have_exp = true;
			else
				// Unexpected character: this is the end of the number.
				break;
		}

		// We're done scanning the characters: now return the right kind
		// of number token, if any fits the bill.

		if (!have_error)
		{
			fb_assert(have_digit);

			if (have_exp_digit)
			{
				yylval.legacyNode = (dsql_nod*) MAKE_string(lex.last_token, lex.ptr - lex.last_token);
				lex.last_token_bk = lex.last_token;
				lex.line_start_bk = lex.line_start;
				lex.lines_bk = lex.lines;

				return FLOAT_NUMBER;
			}

			if (!have_exp)
			{

				// We should return some kind (scaled-) integer type
				// except perhaps in dialect 1.

				if (!have_decimal && (number <= MAX_SLONG))
				{
					yylval.int32Val = (SLONG) number;
					//printf ("parse.y %p %d\n", yylval.legacyNode, number);
					return NUMBER;
				}
				else
				{
					/* We have either a decimal point with no exponent
					   or a string of digits whose value exceeds MAX_SLONG:
					   the returned type depends on the client dialect,
					   so warn of the difference if the client dialect is
					   SQL_DIALECT_V6_TRANSITION.
					*/

					if (SQL_DIALECT_V6_TRANSITION == client_dialect)
					{
						/* Issue a warning about the ambiguity of the numeric
						 * numeric literal.  There are multiple calls because
						 * the message text exceeds the 119-character limit
						 * of our message database.
						 */
						ERRD_post_warning(Arg::Warning(isc_dsql_warning_number_ambiguous) <<
										  Arg::Str(Firebird::string(lex.last_token, lex.ptr - lex.last_token)));
						ERRD_post_warning(Arg::Warning(isc_dsql_warning_number_ambiguous1));
					}

					yylval.legacyNode = (dsql_nod*) MAKE_string(lex.last_token, lex.ptr - lex.last_token);

					lex.last_token_bk = lex.last_token;
					lex.line_start_bk = lex.line_start;
					lex.lines_bk = lex.lines;

					if (client_dialect < SQL_DIALECT_V6_TRANSITION)
						return FLOAT_NUMBER;

					if (have_decimal)
						return SCALEDINT;

					return NUMBER64BIT;
				}
			} // else if (!have_exp)
		} // if (!have_error)

		// we got some kind of error or overflow, so don't recognize this
		// as a number: just pass it through to the next part of the lexer.
	}

	// Restore the status quo ante, before we started our unsuccessful
	// attempt to recognize a number.
	lex.ptr = lex.last_token;
	c = *lex.ptr++;
	// We never touched tok_class, so it doesn't need to be restored.

	// end of number-recognition code


	if (tok_class & CHR_LETTER)
	{
		char* p = string;
		check_copy_incr(p, UPPER (c), string);
		for (; lex.ptr < lex.end && classes(*lex.ptr) & CHR_IDENT; lex.ptr++)
		{
			if (lex.ptr >= lex.end)
				return -1;
			check_copy_incr(p, UPPER (*lex.ptr), string);
		}

		check_bound(p, string);
		*p = 0;
		dsql_sym* sym =
			HSHD_lookup (NULL, string, (SSHORT)(p - string), SYM_keyword, parser_version);
		if (sym && (sym->sym_keyword != COMMENT || lex.prev_keyword == -1))
		{
			yylval.legacyNode = (dsql_nod*) sym->sym_object;
			lex.last_token_bk = lex.last_token;
			lex.line_start_bk = lex.line_start;
			lex.lines_bk = lex.lines;
			return sym->sym_keyword;
		}

		if (p - string > MAX_SQL_IDENTIFIER_LEN)
			yyabandon(-104, isc_dyn_name_longer);

		yylval.legacyNode = (dsql_nod*) MAKE_string(string, p - string);
		lex.last_token_bk = lex.last_token;
		lex.line_start_bk = lex.line_start;
		lex.lines_bk = lex.lines;
		return SYMBOL;
	}

	// Must be punctuation -- test for double character punctuation

	if (lex.last_token + 1 < lex.end)
	{
		dsql_sym* sym =
			HSHD_lookup (NULL, lex.last_token, (SSHORT) 2, SYM_keyword, (USHORT) parser_version);
		if (sym)
		{
			++lex.ptr;
			return sym->sym_keyword;
		}
	}

	// Single character punctuation are simply passed on

	return (UCHAR) c;
}


void Parser::yyerror_detailed(const TEXT* /*error_string*/, int yychar, YYSTYPE&, YYPOSN&)
{
/**************************************
 *
 *	y y e r r o r _ d e t a i l e d
 *
 **************************************
 *
 * Functional description
 *	Print a syntax error.
 *
 **************************************/
	const TEXT* line_start = lex.line_start;
	SLONG lines = lex.lines;
	if (lex.last_token < lex.line_start)
	{
		line_start = lex.line_start_bk;
		lines--;
	}

	if (yychar < 1)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  // Unexpected end of command
				  Arg::Gds(isc_command_end_err2) << Arg::Num(lines) <<
													Arg::Num(lex.last_token - line_start + 1));
	}
	else
	{
		ERRD_post (Arg::Gds(isc_sqlerr) << Arg::Num(-104) <<
				  // Token unknown - line %d, column %d
				  Arg::Gds(isc_dsql_token_unk_err) << Arg::Num(lines) <<
				  									  Arg::Num(lex.last_token - line_start + 1) << // CVC: +1
				  // Show the token
				  Arg::Gds(isc_random) << Arg::Str(string(lex.last_token, lex.ptr - lex.last_token)));
	}
}


// The argument passed to this function is ignored. Therefore, messages like
// "syntax error" and "yacc stack overflow" are never seen.
void Parser::yyerror(const TEXT* error_string)
{
	YYSTYPE errt_value;
	YYPOSN errt_posn = -1;
	yyerror_detailed(error_string, -1, errt_value, errt_posn);
}


static void yyabandon (SLONG sql_code, ISC_STATUS error_symbol)
{
/**************************************
 *
 *	y y a b a n d o n
 *
 **************************************
 *
 * Functional description
 *	Abandon the parsing outputting the supplied string
 *
 **************************************/

	ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(sql_code) <<
			  Arg::Gds(error_symbol));
}
