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

/* SQLDA dialects */

#define DIALECT_sqlda	0
#define DIALECT_xsqlda	1

/* Definitions for DSQL free_statement routine */

#define DSQL_close	1
#define DSQL_drop	2

#ifndef JRD_IBASE_H

/* Dynamic SQL definitions */

#ifndef INCLUDE_FB_TYPES_H
#include "../include/fb_types.h"
#endif

typedef struct
{
	SSHORT	sqltype;
	SSHORT	sqllen;
	SCHAR*	sqldata;
	SSHORT*	sqlind;
	SSHORT	sqlname_length;
	SCHAR	sqlname[30];
} SQLVAR;

typedef struct
{
	SCHAR	sqldaid[8];
	SLONG	sqldabc;
	SSHORT	sqln;
	SSHORT	sqld;
	SQLVAR	sqlvar[1];
} SQLDA;

#define SQLDA_LENGTH(n)		(sizeof (SQLDA) + (n-1) * sizeof (SQLVAR))

/* Declare the extended SQLDA */

#ifndef FB_SQLDA

typedef struct
{
	SSHORT	sqltype;			/* datatype of field */
	SSHORT	sqlscale;			/* scale factor */
	SSHORT	sqlsubtype;			/* datatype subtype - currently BLOBs only */
	SSHORT	sqllen;				/* length of data area */
	SCHAR*	sqldata;			/* address of data */
	SSHORT*	sqlind;				/* address of indicator variable */
	SSHORT	sqlname_length;		/* length of sqlname field */
	SCHAR	sqlname[32];		/* name of field, name length + space for NULL */
	SSHORT	relname_length;		/* length of relation name */
	SCHAR	relname[32];		/* field's relation name + space for NULL */
	SSHORT	ownname_length;		/* length of owner name */
	SCHAR	ownname[32];		/* relation's owner name + space for NULL */
	SSHORT	aliasname_length;	/* length of alias name */
	SCHAR	aliasname[32];		/* relation's alias name + space for NULL */
} XSQLVAR;

#define SQLDA_VERSION1		1

typedef struct
{
	SSHORT	version;				/* version of this XSQLDA */
	SCHAR	sqldaid[8];			/* XSQLDA name field */
	SLONG	sqldabc;				/* length in bytes of SQLDA */
	SSHORT	sqln;				/* number of fields allocated */
	SSHORT	sqld;				/* actual number of fields */
	XSQLVAR	sqlvar[1];			/* first field address */
} XSQLDA;

#define XSQLDA_LENGTH(n)	(sizeof (XSQLDA) + (n-1) * sizeof (XSQLVAR))

#define FB_SQLDA
#endif


#define SQL_TEXT                           452
#define SQL_VARYING                        448
#define SQL_SHORT                          500
#define SQL_LONG                           496
#define SQL_FLOAT                          482
#define SQL_DOUBLE                         480
#define SQL_D_FLOAT                        530
#define SQL_TIMESTAMP                      510
#define SQL_BLOB                           520
#define SQL_ARRAY                          540
#define SQL_QUAD                           550
#define SQL_TYPE_TIME			   560
#define SQL_TYPE_DATE                      570
#define SQL_INT64                          580

/* Historical alias for pre v6 code */
#define SQL_DATE                           SQL_TIMESTAMP

#endif /* JRD_IBASE_H */

/* Structure to support conversion of SQLDA's to messages */

typedef struct dasup
{
	struct dasup_clause
	{
		SCHAR*	dasup_blr;
		SCHAR*	dasup_msg;
		USHORT	dasup_blr_length;
		USHORT	dasup_blr_buf_len;
		USHORT	dasup_msg_buf_len;
	} dasup_clauses[2];

	USHORT	dasup_dialect;		/* Dialect associated with statement */
	USHORT	dasup_stmt_type;	/* Type of statement */

} *DASUP;

#define DASUP_CLAUSE_select	0
#define DASUP_CLAUSE_bind	1

#endif // DSQL_SQLDA_H

