/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		ini.h
 *	DESCRIPTION:	Declarations for metadata initialization
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

/* Note that this file is used by jrdmet.c in gpre 
   as well as by ini.e in JRD.  Make sure that any
   changes are compatible in both places. */

#include "../jrd/intl.h"
#include "../intl/country_codes.h"
#include "../intl/charsets.h"
#include "../jrd/obj.h"

/* Define name ids */

#define NAME(name, id) id,

typedef enum nam_i { nam_MIN,
#include "../jrd/names.h"
nam_MAX} name_ids;

#undef NAME

/* Define name strings */

#define NAME(name, id) name,

static const TEXT* const names[] =
{
	0,
#include "../jrd/names.h"
};
#undef NAME


/* Pick up global ids */


#define FIELD(type,name,dtype,length,sub_type,ods,dflt_blr)	type,
typedef enum gflds {
#include "../jrd/fields.h"
gfld_MAX} GFLDS;
#undef FIELD

/* Pick up actual global fields */

#ifndef GPRE
#define FIELD(type,name,dtype,length,sub_type,ods,dflt_blr)	{ (UCHAR) type, (UCHAR) name, dtype, length, sub_type, ods, dflt_blr, sizeof (dflt_blr) },
#else
#define FIELD(type,name,dtype,length,sub_type,ods,dflt_blr)	{ (UCHAR) type, (UCHAR) name, dtype, length, sub_type, ods, NULL, 0 },
#endif

typedef struct gfld
{
	UCHAR gfld_type;
	UCHAR gfld_name;
	UCHAR gfld_dtype;
	UCHAR gfld_length;
	UCHAR gfld_sub_type;
	UCHAR gfld_minor;
	const UCHAR *gfld_dflt_blr;
	USHORT gfld_dflt_len;
} GFLD;

static const struct gfld gfields[] = {
#include "../jrd/fields.h"
	{ 0, 0, dtype_null, 0, 0, 0, NULL, 0 }
};
#undef FIELD


/* Pick up relation ids */

#define RELATION(name, id, ods) id,
#define FIELD(symbol, name, id, update, ods, upd_id, upd_ods)
#define END_RELATION
typedef enum rids {
#include "../jrd/relations.h"
rel_MAX} RIDS;
#undef RELATION
#undef FIELD
#undef END_RELATION

/* Pick up relations themselves */

#define RELATION(name, id, ods)	(UCHAR) name,(UCHAR) id, ods,
#define FIELD(symbol, name, id, update, ods, upd_id, upd_ods)\
				(UCHAR) name, (UCHAR) id, update, ods, (UCHAR) upd_id, upd_ods,
#define END_RELATION		0,

#define RFLD_R_NAME	0
#define RFLD_R_ID	1
#define RFLD_R_MINOR	2
#define RFLD_RPT	3

#define RFLD_F_NAME	0
#define RFLD_F_ID	1
#define RFLD_F_UPDATE	2
#define RFLD_F_MINOR	3
#define RFLD_F_UPD_ID	4
#define RFLD_F_UPD_MINOR 5
#define RFLD_F_LENGTH	6

static const UCHAR relfields[] =
{
#include "../jrd/relations.h"
	0
};

#undef RELATION
#undef FIELD
#undef END_RELATION


/* obtain field types */

typedef struct rtyp {
	const TEXT *rtyp_name;
	SSHORT rtyp_value;
	UCHAR rtyp_field;
} RTYP;

#define TYPE(text, type, field)	{ text, type, (UCHAR) field },

#define CHARSET(name, cs_id, coll_id, bytes, num, s1, s2) \
				{(name), (cs_id),   (UCHAR) nam_charset_name},
#define CSALIAS(name, cs_id)	{(name), (cs_id),   (UCHAR) nam_charset_name},
#define COLLATION(name, cc_id, cs_id, coll_id, sym)
#define END_CHARSET

static const RTYP types[] = {
#include "../jrd/types.h"
#include "../jrd/intlnames.h"
	{NULL, 0, 0}
};

#undef TYPE
#undef CHARSET
#undef CSALIAS
#undef COLLATION
#undef END_CHARSET


#ifndef GPRE
/* obtain international names */

typedef struct initcharset {
	const UCHAR *init_charset_name;
	SSHORT init_charset_id;
	USHORT init_charset_bytes_per_char;
} CS_TYPE;

#define CHARSET(name, cs_id, coll_id, bytes, num, s1, s2) \
	{ (const UCHAR *) (name), (cs_id), (bytes)},
#define CSALIAS(name, cs_id)
#define COLLATION(name, cc_id, cs_id, coll_id, sym)
#define END_CHARSET

static const CS_TYPE cs_types[] = {
#include "../jrd/intlnames.h"
	{NULL, 0, 0}
};
#undef CHARSET
#undef CSALIAS
#undef COLLATION
#undef END_CHARSET

typedef struct initcollation {
	const UCHAR *init_collation_name;
	SSHORT init_collation_charset;
	SSHORT init_collation_id;
} COLL_TYPE;

#define CHARSET(name, cs_id, coll_id, bytes, num, s1, s2) \
	{ (const UCHAR *) (name), (cs_id), (coll_id) },
#define CSALIAS(name, cs_id)
#define COLLATION(name, cc_id, cs_id, coll_id, sym) \
	{ (const UCHAR *) (name), (cs_id), (coll_id) },
#define END_CHARSET

static const COLL_TYPE coll_types[] = {
#include "../jrd/intlnames.h"
	{NULL, 0, 0}
};
#undef CHARSET
#undef CSALIAS
#undef COLLATION
#undef END_CHARSET
#endif
