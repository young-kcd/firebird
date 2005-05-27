/*
 *	PROGRAM:	JRD access method
 *	MODULE:		dsc.h
 *	DESCRIPTION:	Definitions associated with descriptors
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
 * 2002.04.16  Paul Beach - HP10 Define changed from -4 to (-4) to make it
 *             compatible with the HP Compiler
 */

#ifndef JRD_DSC_H
#define JRD_DSC_H

#include "../jrd/dsc_pub.h"

/* Descriptor format */

/* WARNING !
   This run-time structure is stored in RDB$FORMATS.RDB$DESCRIPTORS.
   Any modification to this structure is tantamount to an ODS change.
   See MET_format() and make_format() in MET.EPP for enlightenment.
*/

typedef struct dsc
{
	dsc()
	:	dsc_dtype(0),
		dsc_scale(0),
		dsc_length(0),
		dsc_sub_type(0),
		dsc_flags(0),
		dsc_address(0)
	{}

	UCHAR	dsc_dtype;
	SCHAR	dsc_scale;
	USHORT	dsc_length;
	SSHORT	dsc_sub_type;
	USHORT	dsc_flags;
	UCHAR*	dsc_address; // Used either as offset in a message or as a pointer
#ifdef __cplusplus
	SSHORT dsc_blob_ttype() const { return dsc_scale | (dsc_flags & 0xFF00);}
	SSHORT& dsc_ttype() { return dsc_sub_type;}
	SSHORT dsc_ttype() const { return dsc_sub_type;}
#endif
} DSC;

struct OdsDesc
{
	UCHAR	dsc_dtype;
	SCHAR	dsc_scale;
	USHORT	dsc_length;
	SSHORT	dsc_sub_type;
	USHORT	dsc_flags;
	ULONG	dsc_offset;
};


inline SSHORT DSC_GET_CHARSET(const dsc* desc) {
	return (desc->dsc_sub_type & 0x00FF);
}
//#define DSC_GET_CHARSET(dsc)	(((dsc)->dsc_sub_type) & 0x00FF)

inline SSHORT DSC_GET_COLLATE(const dsc* desc) {
	return (desc->dsc_sub_type >> 8);
}
//#define DSC_GET_COLLATE(dsc)	(((dsc)->dsc_sub_type) >> 8)

typedef struct alt_dsc {
	SLONG dsc_combined_type;
	SSHORT dsc_sub_type;
	USHORT dsc_flags;			/* Not currently used */
} ALT_DSC;

inline bool DSC_EQUIV(const dsc* d1, const dsc* d2) {
	return ((((ALT_DSC*) d1)->dsc_combined_type == ((ALT_DSC*) d2)->dsc_combined_type) && 
		((DSC_GET_CHARSET (d1) == DSC_GET_CHARSET (d2)) || d1->dsc_dtype > dtype_varying));
}
//#define DSC_EQUIV(d1,d2) ((((ALT_DSC*) d1)->dsc_combined_type == ((ALT_DSC*) d2)->dsc_combined_type) && 
//			  ((DSC_GET_CHARSET (d1) == DSC_GET_CHARSET (d2)) || d1->dsc_dtype > dtype_any_text))

/* In DSC_*_result tables, DTYPE_CANNOT means that the two operands
   cannot participate together in the requested operation. */

const UCHAR DTYPE_CANNOT	= 127;

/* Historical alias definition */
const UCHAR dtype_date		= dtype_timestamp;

const UCHAR dtype_aligned	= dtype_varying;
const UCHAR dtype_any_text	= dtype_varying;
const UCHAR dtype_min_comp	= dtype_packed;
const UCHAR dtype_max_comp	= dtype_d_float;

/* NOTE: For types <= dtype_any_text the dsc_sub_type field defines
   the text type */

inline USHORT TEXT_LEN(const dsc* desc) {
	return ((desc->dsc_dtype == dtype_text) ? desc->dsc_length 
		: (desc->dsc_dtype == dtype_cstring) ? desc->dsc_length - 1 : desc->dsc_length - sizeof(USHORT));
}
//#define TEXT_LEN(d)   ((d->dsc_dtype == dtype_text) ? d->dsc_length : (d->dsc_dtype == dtype_cstring) ? d->dsc_length - 1 : d->dsc_length - sizeof(USHORT))


/* Text Sub types, distinct from character sets & collations */

const SSHORT dsc_text_type_none		= 0;	/* Normal text */
const SSHORT dsc_text_type_fixed	= 1;	/* strings can contain null bytes */
const SSHORT dsc_text_type_metadata	= 3;	/* string represents system metadata */


/* Exact numeric subtypes: with ODS >= 10, these apply when dtype
   is short, long, or quad. */

const SSHORT dsc_num_type_none		= 0;	/* defined as SMALLINT or INTEGER */
const SSHORT dsc_num_type_numeric	= 1;	/* defined as NUMERIC(n,m)        */
const SSHORT dsc_num_type_decimal	= 2;	/* defined as DECIMAL(n,m)        */


/* Date type information */

inline bool DTYPE_IS_TEXT(UCHAR d) {
	return (((d) >= dtype_text) && ((d) <= dtype_varying));
}
//#define DTYPE_IS_DATE(d)	(((d) >= dtype_text) && ((d) <= dtype_varying))

inline bool DTYPE_IS_DATE(UCHAR t) {
	return (((t) >= dtype_sql_date) && ((t) <= dtype_timestamp));
}
//#define DTYPE_IS_DATE(t)	(((t) >= dtype_sql_date) && ((t) <= dtype_timestamp))

/* DTYPE_IS_BLOB includes both BLOB and ARRAY since array's are implemented over blobs. */
inline bool DTYPE_IS_BLOB(UCHAR d) {
	return (((d) == dtype_blob) || ((d) == dtype_array));
}

//#define DTYPE_IS_BLOB(d)        (((d) == dtype_blob) || ((d) == dtype_array))

/* Exact numeric? */
inline bool DTYPE_IS_EXACT(UCHAR d) {
	return (((d) == dtype_int64) || ((d) == dtype_long) || ((d) == dtype_short));
}

//#define DTYPE_IS_EXACT(d)       (((d) == dtype_int64) || 
//				 ((d) == dtype_long)  || 
//				 ((d) == dtype_short))

#ifdef VMS
inline bool DTYPE_IS_APPROX(UCHAR d) {
	return (((d) == dtype_double) || ((d) == dtype_real) || ((d) == dtype_d_float));
}
//#define DTYPE_IS_APPROX(d)       (((d) == dtype_double) || 
//				 ((d) == dtype_real)  || 
//				 ((d) == dtype_d_float))
#else
inline bool DTYPE_IS_APPROX(UCHAR d) {
	return (((d) == dtype_double) || ((d) == dtype_real));
}
//#define DTYPE_IS_APPROX(d)       (((d) == dtype_double) || 
//				  ((d) == dtype_real))
#endif


inline bool DTYPE_IS_NUMERIC(UCHAR d) {
	return ((((d) >= dtype_byte) && ((d) <= dtype_d_float)) || ((d)  == dtype_int64));
}
//#define DTYPE_IS_NUMERIC(d)	((((d) >= dtype_byte) && 
//				  ((d) <= dtype_d_float)) || 
//				 ((d)  == dtype_int64))

inline SCHAR NUMERIC_SCALE(const dsc desc) {
	return ((DTYPE_IS_TEXT((desc).dsc_dtype)) ? 0 : (desc).dsc_scale);
}

//#define NUMERIC_SCALE(desc)	((DTYPE_IS_TEXT((desc).dsc_dtype)) ? 0 : (desc).dsc_scale)

/* Macros defining what operations are legal on data types */
inline bool DTYPE_CAN_NEGATE(UCHAR d) {
	return DTYPE_IS_NUMERIC(d);
}
//#define DTYPE_CAN_NEGATE(d)	DTYPE_IS_NUMERIC(d)
inline bool DTYPE_CAN_AVERAGE(UCHAR d) {
	return DTYPE_IS_NUMERIC(d);
}
//#define DTYPE_CAN_AVERAGE(d)	DTYPE_IS_NUMERIC(d)
inline bool DTYPE_CAN_DIVIDE(UCHAR d) {
	return DTYPE_IS_NUMERIC(d);
}
//#define DTYPE_CAN_DIVIDE(d)	DTYPE_IS_NUMERIC(d)
inline bool DTYPE_CAN_MULTIPLY(UCHAR d) {
	return DTYPE_IS_NUMERIC(d);
}
//#define DTYPE_CAN_MULTIPLY(d)	DTYPE_IS_NUMERIC(d)


#endif /* JRD_DSC_H */

