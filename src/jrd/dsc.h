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

#ifdef __cplusplus
extern "C" {
#endif


/* Descriptor format */

/* WARNING !
   This run-time structure is stored in RDB$FORMATS.RDB$DESCRIPTORS.
   Any modification to this structure is tantamount to an ODS change.
   See MET_format() and make_format() in MET.E for enlightenment.
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
	UCHAR*	dsc_address;
} DSC;

/* values for dsc_flags */
/* Note: DSC_null is only reliably set for local variables
   (blr_variable) */
#define DSC_null		1
#define DSC_no_subtype		2	/* dsc has no sub type specified */
#define DSC_nullable  		4	/* not stored. instead, is derived
								   from metadata primarily to flag
								   SQLDA (in DSQL)               */

/* Overload text typing information into the dsc_sub_type field.
   See intl.h for definitions of text types */

#ifndef dsc_ttype
#define dsc_ttype	dsc_sub_type
#endif

#define DSC_GET_CHARSET(dsc)	(((dsc)->dsc_ttype) & 0x00FF)
#define DSC_GET_COLLATE(dsc)	(((dsc)->dsc_ttype) >> 8)


typedef struct alt_dsc {
	SLONG dsc_combined_type;
	SSHORT dsc_sub_type;
	USHORT dsc_flags;			/* Not currently used */
} ALT_DSC;



#define DSC_EQUIV(d1,d2) ((((ALT_DSC*) d1)->dsc_combined_type == ((ALT_DSC*) d2)->dsc_combined_type) && \
			  ((DSC_GET_CHARSET (d1) == DSC_GET_CHARSET (d2)) || d1->dsc_dtype > dtype_any_text))



/* Data types */

/* WARNING: if you add another manifest constant to this group, then you
 * must add another entry to the array compare_priority in jrd/cvt2.c.
 */

/* Note that dtype_null actually means that we do not yet know the
   dtype for this descriptor.  A nice cleanup item would be to globally
   change it to dtype_unknown.  --chrisj 1999-02-17 */

#define dtype_null	0
#define dtype_text	1
#define dtype_cstring	2
#define dtype_varying	3

#define dtype_packed	6
#define dtype_byte	7
#define dtype_short	8
#define dtype_long	9
#define dtype_quad	10
#define dtype_real	11
#define dtype_double	12
#define dtype_d_float	13
#define dtype_sql_date	14
#define dtype_sql_time	15
#define dtype_timestamp	16
#define dtype_blob	17
#define dtype_array	18
#define dtype_int64     19

#define DTYPE_TYPE_MAX	20

/* In DSC_*_result tables, DTYPE_CANNOT means that the two operands
   cannot participate together in the requested operation. */

#define DTYPE_CANNOT   127

/* Historical alias definition */
#define dtype_date	dtype_timestamp

#define dtype_aligned	dtype_varying
#define dtype_any_text	dtype_varying
#define dtype_min_comp	dtype_packed
#define dtype_max_comp	dtype_d_float

/* NOTE: For types <= dtype_any_text the dsc_sub_type field defines
   the text type */

#define TEXT_LEN(d)   ((d->dsc_dtype == dtype_text) ? d->dsc_length : (d->dsc_dtype == dtype_cstring) ? d->dsc_length - 1 : d->dsc_length - sizeof(USHORT))


/* Text Sub types, distinct from character sets & collations */

#define dsc_text_type_none	0	/* Normal text */
#define	dsc_text_type_fixed	1	/* strings can contain null bytes */
#define dsc_text_type_metadata	3	/* string represents system metadata */


/* Exact numeric subtypes: with ODS >= 10, these apply when dtype
   is short, long, or quad. */

#define dsc_num_type_none       0	/* defined as SMALLINT or INTEGER */
#define dsc_num_type_numeric    1	/* defined as NUMERIC(n,m)        */
#define dsc_num_type_decimal    2	/* defined as DECIMAL(n,m)        */


/* Date type information */

#define DTYPE_IS_TEXT(d)	(((d) >= dtype_text) && ((d) <= dtype_varying))
#define DTYPE_IS_DATE(t)	(((t) >= dtype_sql_date) && ((t) <= dtype_timestamp))

/* DTYPE_IS_BLOB includes both BLOB and ARRAY since array's are implemented over blobs. */
#define DTYPE_IS_BLOB(d)        (((d) == dtype_blob) || ((d) == dtype_array))

/* Exact numeric? */

#define DTYPE_IS_EXACT(d)       (((d) == dtype_int64) || \
				 ((d) == dtype_long)  || \
				 ((d) == dtype_short))

#ifdef VMS
#define DTYPE_IS_APPROX(d)       (((d) == dtype_double) || \
				 ((d) == dtype_real)  || \
				 ((d) == dtype_d_float))
#else
#define DTYPE_IS_APPROX(d)       (((d) == dtype_double) || \
				  ((d) == dtype_real))
#endif

#define DTYPE_IS_NUMERIC(d)	((((d) >= dtype_byte) && \
				  ((d) <= dtype_d_float)) || \
				 ((d)  == dtype_int64))

/* Macros defining what operations are legal on data types */
#define DTYPE_CAN_NEGATE(d)	DTYPE_IS_NUMERIC(d)
#define DTYPE_CAN_AVERAGE(d)	DTYPE_IS_NUMERIC(d)
#define DTYPE_CAN_DIVIDE(d)	DTYPE_IS_NUMERIC(d)
#define DTYPE_CAN_MULTIPLY(d)	DTYPE_IS_NUMERIC(d)


#define ISC_TIME_SECONDS_PRECISION		10000L
#define ISC_TIME_SECONDS_PRECISION_SCALE	(-4)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* JRD_DSC_H */
