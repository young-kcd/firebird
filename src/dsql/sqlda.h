/*
 *	PROGRAM:	C preprocessor
 *	MODULE:		sqlda.h
 *	DESCRIPTION:	DSQL definitions for non-DSQL modules
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

#ifndef DSQL_SQLDA_H
#define DSQL_SQLDA_H

// SQLDA dialects 

const USHORT DIALECT_sqlda	= 0;
const USHORT DIALECT_xsqlda	= 1;

struct SQLVAR
{
	SSHORT	sqltype;
	SSHORT	sqllen;
	SCHAR*	sqldata;
	SSHORT*	sqlind;
	SSHORT	sqlname_length;
	SCHAR	sqlname[30];
};

struct SQLDA
{
	SCHAR	sqldaid[8];
	SLONG	sqldabc;
	SSHORT	sqln;
	SSHORT	sqld;
	SQLVAR	sqlvar[1];
};

#define SQLDA_LENGTH(n)		(sizeof (SQLDA) + (n - 1) * sizeof (SQLVAR))

// Structure to support conversion of SQLDA's to messages 

struct sqlda_sup
{
	struct dasup_clause
	{
		SCHAR*	dasup_blr;
		SCHAR*	dasup_msg;
		USHORT	dasup_blr_length;
		USHORT	dasup_blr_buf_len;
		USHORT	dasup_msg_buf_len;
	} dasup_clauses[2];

	USHORT	dasup_dialect;		// Dialect associated with statement 
	USHORT	dasup_stmt_type;	// Type of statement 

};

// enum would be troblesome here
const USHORT DASUP_CLAUSE_select	= 0;
const USHORT DASUP_CLAUSE_bind		= 1;

#include "../dsql/sqlda_pub.h"

#endif // DSQL_SQLDA_H

