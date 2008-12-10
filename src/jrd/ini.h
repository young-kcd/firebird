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

/* Note that this file is used by jrdmet.cpp in gpre
   as well as by ini.epp in JRD.  Make sure that any
   changes are compatible in both places. */

#include "../jrd/intlobj_new.h"
#include "../jrd/intl.h"
#include "../intl/country_codes.h"
#include "../intl/charsets.h"
#include "../jrd/obj.h"
#include "../jrd/dflt.h"

//******************************
// names.h
//******************************

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

//******************************
// fields.h
//******************************
const UCHAR BLOB_SIZE		= 8;
const UCHAR TIMESTAMP_SIZE	= 8;

/* Pick up global ids */


#define FIELD(type,name,dtype,length,sub_type,ods,dflt_blr)	type,
enum gflds {
#include "../jrd/fields.h"
gfld_MAX};
#undef FIELD

typedef gflds GFLDS;

/* Pick up actual global fields */

#ifndef GPRE
#define FIELD(type,name,dtype,length,sub_type,ods,dflt_blr)	{ (UCHAR) type, (UCHAR) name, dtype, length, sub_type, ods, dflt_blr, sizeof (dflt_blr) },
#else
#define FIELD(type,name,dtype,length,sub_type,ods,dflt_blr)	{ (UCHAR) type, (UCHAR) name, dtype, length, sub_type, ods, NULL, 0 },
#endif

struct gfld
{
	UCHAR gfld_type;
	UCHAR gfld_name;
	UCHAR gfld_dtype;
	USHORT gfld_length;
	UCHAR gfld_sub_type;	// mismatch; dsc2.h uses SSHORT.
	UCHAR gfld_minor;
	const UCHAR *gfld_dflt_blr;
	USHORT gfld_dflt_len;
};

static const struct gfld gfields[] = {
#include "../jrd/fields.h"
	{ 0, 0, dtype_unknown, 0, 0, 0, NULL, 0 }
};
#undef FIELD

//******************************
// relations.h
//******************************

/* Pick up relation ids */

#define RELATION(name, id, ods) id,
#define FIELD(symbol, name, id, update, ods, upd_id, upd_ods)
#define END_RELATION
enum rids {
#include "../jrd/relations.h"
rel_MAX};
#undef RELATION
#undef FIELD
#undef END_RELATION

typedef rids RIDS;

/* Pick up relations themselves */

#define RELATION(name, id, ods)	(UCHAR) name,(UCHAR) id, ods,
#define FIELD(symbol, name, id, update, ods, upd_id, upd_ods)\
				(UCHAR) name, (UCHAR) id, update, ods, (UCHAR) upd_id, upd_ods,
#define END_RELATION		0,

const int RFLD_R_NAME	= 0;
const int RFLD_R_ID		= 1;
const int RFLD_R_MINOR	= 2;
const int RFLD_RPT		= 3;

const int RFLD_F_NAME	= 0;
const int RFLD_F_ID		= 1;
const int RFLD_F_UPDATE	= 2;
const int RFLD_F_MINOR	= 3;
const int RFLD_F_UPD_ID	= 4;
const int RFLD_F_UPD_MINOR	= 5;
const int RFLD_F_LENGTH	= 6;

static const UCHAR relfields[] =
{
#include "../jrd/relations.h"
	0
};

#undef RELATION
#undef FIELD
#undef END_RELATION

//******************************
// types.h and intlnames.h
//******************************

/* obtain field types */

struct rtyp {
	const TEXT* rtyp_name;
	SSHORT rtyp_value;
	UCHAR rtyp_field;
};

#define TYPE(text, type, field)	{ text, type, (UCHAR) field },

#define CHARSET(name, cs_id, coll_id, bytes, num, s1, s2, attr) \
				{(name), (cs_id),   (UCHAR) nam_charset_name},
#define CSALIAS(name, cs_id)	{(name), (cs_id),   (UCHAR) nam_charset_name},
#define COLLATION(name, cc_id, cs_id, coll_id, sym, attr)
#define END_CHARSET

static const rtyp types[] = {
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

struct initcharset {
	const UCHAR* init_charset_name;
	SSHORT init_charset_id;
	USHORT init_charset_bytes_per_char;
};

typedef initcharset CS_TYPE;

#define CHARSET(name, cs_id, coll_id, bytes, num, s1, s2, attr) \
	{ (const UCHAR *) (name), (cs_id), (bytes)},
#define CSALIAS(name, cs_id)
#define COLLATION(name, cc_id, cs_id, coll_id, sym, attr)
#define END_CHARSET

static const CS_TYPE cs_types[] = {
#include "../jrd/intlnames.h"
	{NULL, 0, 0}
};
#undef CHARSET
#undef CSALIAS
#undef COLLATION
#undef END_CHARSET

struct initcollation {
	const UCHAR *init_collation_name;
	SSHORT init_collation_charset;
	SSHORT init_collation_id;
	USHORT init_collation_attributes;
};

typedef initcollation COLL_TYPE;

#define CHARSET(name, cs_id, coll_id, bytes, num, s1, s2, attr) \
	{ (const UCHAR *) (name), (cs_id), (coll_id), (attr) },
#define CSALIAS(name, cs_id)
#define COLLATION(name, cc_id, cs_id, coll_id, sym, attr) \
	{ (const UCHAR *) (name), (cs_id), (coll_id), (attr) },
#define END_CHARSET

static const COLL_TYPE coll_types[] = {
#include "../jrd/intlnames.h"
	{NULL, 0, 0, 0}
};
#undef CHARSET
#undef CSALIAS
#undef COLLATION
#undef END_CHARSET
#endif

