/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		sym.h
 *	DESCRIPTION:	Definitions for symbol table
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

#ifndef _DSQL_SYM_H_
#define _DSQL_SYM_H_

#include "../jrd/common.h"

/* possible symbol types */

typedef enum sym_type {
	SYM_statement,
	SYM_cursor,
	SYM_keyword,
	SYM_context,
	SYM_relation,
	SYM_field,
	SYM_stream,
	SYM_udf,
	SYM_procedure,
	SYM_intlsym,
	SYM_eof
} SYM_TYPE;

/* symbol block */

class sym : public pool_alloc_rpt<UCHAR, dsql_type_sym> {
public:
	void *sym_dbb;				/* generic DB structure handle */
	TEXT *sym_string;			/* address of asciz string */
	USHORT sym_length;			/* length of string (exc. term.) */
	SYM_TYPE sym_type;			/* symbol type */
	USHORT sym_keyword;			/* keyword number, if keyword */
	void *sym_object;			/* general pointer to object */
	struct sym *sym_collision;	/* collision pointer */
	struct sym *sym_homonym;	/* homonym pointer */
	TEXT sym_name[2];			/* space for name, if necessary */
	USHORT sym_version;			/* dialect version the symbol was introduced */
};
typedef sym *SYM;

#endif /* _DSQL_SYM_H_ */
