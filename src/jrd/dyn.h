/*
 *	PROGRAM:	JRD Data Definition Utility
 *	MODULE:		dyn.h
 *	DESCRIPTION:	Dynamic data definition local data
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

#ifndef	JRD_DYN_H
#define JRD_DYN_H

#define ALL_PRIVILEGES		"SIUDR"
		/* all applicable grant/revoke privileges */
#define ALL_PROC_PRIVILEGES	"X"
		/* all applicable grant/revoke privileges for a procedure */
#define DYN_MSG_FAC		8
#define STUFF_COUNT		4
#define MAX_KEY			256
#define TEXT_BLOB_LENGTH	512

#define PRIMARY_KEY		"PRIMARY KEY"
#define FOREIGN_KEY		"FOREIGN KEY"
#define UNIQUE_CNSTRT		"UNIQUE"
#define CHECK_CNSTRT		"CHECK"
#define NOT_NULL_CNSTRT		"NOT NULL"

#define GET_STRING(from,to)	DYN_get_string ((TEXT**)from, (TEXT*)to, sizeof (to), TRUE)

#define GET_STRING_2(from,to)	DYN_get_string ((TEXT**)from, (TEXT*)to, sizeof (to), FALSE)

#define PROC_NAME_SIZE		32
#define FLD_SRC_LEN		32
#define FLD_NAME_LEN		32
#define REL_NAME_LEN		32


typedef struct gbl
{
	JRD_TRA gbl_transaction;
} *GBL;

typedef struct dyn_fld {
	DSC dyn_dsc;
	BOOLEAN dyn_null_flag;
	USHORT dyn_dtype;
	USHORT dyn_precision;
	USHORT dyn_charlen;
	SSHORT dyn_collation;
	SSHORT dyn_charset;
	TEXT dyn_fld_source[FLD_SRC_LEN];
	TEXT dyn_rel_name[REL_NAME_LEN];
	TEXT dyn_fld_name[FLD_NAME_LEN];
    USHORT dyn_charbytelen; /* Used to check modify operations on string types. */

} *DYN_FLD;

void	DYN_error(USHORT, USHORT, const TEXT*, const TEXT*, const TEXT*,
				const TEXT*, const TEXT*);
void	DYN_error_punt(USHORT, USHORT, const TEXT*, const TEXT*,
				const TEXT*, const TEXT*, const TEXT*);
void	DYN_execute(GBL, UCHAR **, TEXT *, TEXT *, TEXT *, TEXT *,
						TEXT *);
SLONG	DYN_get_number(UCHAR **);
USHORT	DYN_get_string(TEXT **, TEXT *, USHORT, USHORT);
void	DYN_get_string2(TEXT **, TEXT *, USHORT);
// This function doesn't need to be exported currently.
bool	DYN_is_it_sql_role(GBL, const TEXT*, TEXT*, TDBB);
USHORT	DYN_put_blr_blob(GBL, UCHAR **, GDS__QUAD *);
USHORT	DYN_put_blr_blob2(GBL, UCHAR **, GDS__QUAD *);
USHORT	DYN_put_text_blob(GBL, UCHAR **, GDS__QUAD *);
USHORT	DYN_put_text_blob2(GBL, UCHAR **, GDS__QUAD *);
void	DYN_rundown_request(BLK, SSHORT);
USHORT	DYN_skip_attribute(UCHAR **);
USHORT	DYN_skip_attribute2(UCHAR **);
void	DYN_unsupported_verb(void);

#endif // JRD_DYN_H

