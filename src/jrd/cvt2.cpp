/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		cvt2.c
 *	DESCRIPTION:	Data mover and converter and comparator, etc.
 *			Routines used ONLY within engine.
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

#include <string.h>
#include "../jrd/jrd.h"
#include "../jrd/val.h"
#include "../jrd/quad.h"
#include "../jrd/codes.h"
#include "../jrd/intl.h"
#include "../jrd/gdsassert.h"
#include "../jrd/all_proto.h"
#include "../jrd/cvt_proto.h"
#include "../jrd/cvt2_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/intlobj.h"
#include "../jrd/gds_proto.h"

#ifdef VMS
extern double MTH$CVT_D_G(), MTH$CVT_G_D();
#endif

#ifdef VAX
#define LOW_WORD	0
#define HIGH_WORD	1
#else
#define LOW_WORD	1
#define HIGH_WORD	0
#endif

/* The original order of dsc_type values corresponded to the priority
   of conversion (that is, always convert the lesser to the greater
   type.)  Introduction of dtype_int64 breaks that assumption: its
   position on the scale should be between dtype_long and dtype_real, but
   the types are integers, and dtype_quad occupies the only available
   place.  Renumbering all the higher-numbered types would be a major
   ODS change and a fundamental discomfort
   
   This table permits us to put the entries in the right order for
   comparison purpose, even though isc_int64 had to get number 19, which
   is otherwise too high.

   This table is used in CVT2_compare, is indexed by dsc_dtype, and
   returns the relative priority of types for use when different types
   are compared.
   */
static CONST BYTE compare_priority[] = { dtype_null,	/* dtype_null through dtype_varying  */
	dtype_text,					/* have their natural values stored  */
	dtype_cstring,				/* in the table.                     */
	dtype_varying,
	0, 0,						/* dtypes and 4, 5 are unused.       */
	dtype_packed,				/* packed through long also have     */
	dtype_byte,					/* their natural values in the table */
	dtype_short,
	dtype_long,
	dtype_quad + 1,				/* quad through array all move up    */
	dtype_real + 1,				/* by one to make room for int64     */
	dtype_double + 1,			/* at its proper place in the table. */
	dtype_d_float + 1,
	dtype_sql_date + 1,
	dtype_sql_time + 1,
	dtype_timestamp + 1,
	dtype_blob + 1,
	dtype_array + 1,
	dtype_long + 1
};								/* int64 goes right after long       */



SSHORT CVT2_compare(DSC * arg1, DSC * arg2, FPTR_VOID err)
{
/**************************************
 *
 *	C V T 2 _ c o m p a r e
 *
 **************************************
 *
 * Functional description
 *	Compare two descriptors.  Return (-1, 0, 1) if a<b, a=b, or a>b.
 *
 **************************************/
	UCHAR *p1, *p2;
	USHORT length, length2;
	SSHORT fill;
	USHORT t1, t2;
	CHARSET_ID charset1, charset2;
	TDBB tdbb = NULL_TDBB;

/* Handle the simple (matched) ones first */

	if (arg1->dsc_dtype == arg2->dsc_dtype &&
		arg1->dsc_scale == arg2->dsc_scale) {
		p1 = arg1->dsc_address;
		p2 = arg2->dsc_address;

		switch (arg1->dsc_dtype) {
		case dtype_short:
			if (*(SSHORT *) p1 == *(SSHORT *) p2)
				return 0;
			if (*(SSHORT *) p1 > *(SSHORT *) p2)
				return 1;
			return -1;

		case dtype_sql_time:
			if (*(ULONG *) p1 == *(ULONG *) p2)
				return 0;
			if (*(ULONG *) p1 > *(ULONG *) p2)
				return 1;
			return -1;

		case dtype_long:
		case dtype_sql_date:
			if (*(SLONG *) p1 == *(SLONG *) p2)
				return 0;
			if (*(SLONG *) p1 > *(SLONG *) p2)
				return 1;
			return -1;

		case dtype_quad:
			return QUAD_COMPARE(*(SQUAD *) p1, *(SQUAD *) p2);

		case dtype_int64:
			if (*(SINT64 *) p1 == *(SINT64 *) p2)
				return 0;
			if (*(SINT64 *) p1 > *(SINT64 *) p2)
				return 1;
			return -1;

		case dtype_timestamp:
			if (((SLONG *) p1)[0] > ((SLONG *) p2)[0])
				return 1;
			if (((SLONG *) p1)[0] < ((SLONG *) p2)[0])
				return -1;
			if (((ULONG *) p1)[1] > ((ULONG *) p2)[1])
				return 1;
			if (((ULONG *) p1)[1] < ((ULONG *) p2)[1])
				return -1;
			return 0;

		case dtype_real:
			if (*(float *) p1 == *(float *) p2)
				return 0;
			if (*(float *) p1 > *(float *) p2)
				return 1;
			return -1;

		case DEFAULT_DOUBLE:
			if (*(double *) p1 == *(double *) p2)
				return 0;
			if (*(double *) p1 > *(double *) p2)
				return 1;
			return -1;

#ifdef VMS
		case SPECIAL_DOUBLE:
			if (*(double *) p1 == *(double *) p2)
				return 0;
			if (CNVT_TO_DFLT((double *) p1) > CNVT_TO_DFLT((double *) p2))
				return 1;
			return -1;
#endif

		case dtype_text:
			/* 
			 * For the sake of optimization, we call INTL_compare
			 * only when we cannot just do byte-by-byte compare, as is
			 * done in the foll. code.
			 * We can do a local compare here, if 
			 *    (a) one of the arguments is charset ttype_binary
			 * OR (b) both of the arguments are char set ttype_none
			 * OR (c) both of the arguments are char set ttype_ascii
			 * If any argument is ttype_dynamic, we must see the
			 * charset of the attachment.
			 */
			SET_TDBB(tdbb);
			if (INTL_TTYPE(arg1) == ttype_dynamic)
				charset1 = INTL_charset(tdbb, INTL_TTYPE(arg1), err);
			else
				charset1 = INTL_TTYPE(arg1);

			if (INTL_TTYPE(arg2) == ttype_dynamic)
				charset2 = INTL_charset(tdbb, INTL_TTYPE(arg2), err);
			else
				charset2 = INTL_TTYPE(arg2);

			if ((IS_INTL_DATA(arg1) || IS_INTL_DATA(arg2)) &&
				(charset1 != ttype_binary) &&
				(charset2 != ttype_binary) &&
				((charset1 != ttype_ascii) ||
				 (charset2 != ttype_ascii)) &&
				((charset1 != ttype_none) || (charset2 != ttype_none))
				)
				return INTL_compare(tdbb, arg1, arg2, err);

			if (arg1->dsc_length >= arg2->dsc_length) {
				length = arg2->dsc_length;
				if (length)
					do
						if (*p1++ != *p2++)
							if (p1[-1] > p2[-1])
								return 1;
							else
								return -1;
					while (--length);
				length = arg1->dsc_length - arg2->dsc_length;
				if (length)
					do
						if (*p1++ != ' ')
							if (p1[-1] > ' ')
								return 1;
							else
								return -1;
					while (--length);
				return 0;
			}
			length = arg1->dsc_length;
			if (length)
				do
					if (*p1++ != *p2++)
						if (p1[-1] > p2[-1])
							return 1;
						else
							return -1;
				while (--length);
			length = arg2->dsc_length - arg1->dsc_length;
			do
				if (*p2++ != ' ')
					if (' ' > p2[-1])
						return 1;
					else
						return -1;
			while (--length);
			return 0;

		case dtype_varying:
		case dtype_cstring:
		case dtype_array:
		case dtype_blob:
			/* Special processing below */
			break;

		default:
			/* the two arguments have identical dtype and scale, but the
			   dtype is not one of your defined types! */
			assert(FALSE);
			break;

		}						/* switch on dtype */
	}							/* if dtypes and scales are equal */

/* Handle mixed string comparisons */

	if (arg1->dsc_dtype <= dtype_varying && arg2->dsc_dtype <= dtype_varying) {
		/*
		 * For the sake of optimization, we call INTL_compare
		 * only when we cannot just do byte-by-byte compare.
		 * We can do a local compare here, if
		 *    (a) one of the arguments is charset ttype_binary
		 * OR (b) both of the arguments are char set ttype_none
		 * OR (c) both of the arguments are char set ttype_ascii
		 * If any argument is ttype_dynamic, we must see the
		 * charset of the attachment.
		 */

		SET_TDBB(tdbb);
		if (INTL_TTYPE(arg1) == ttype_dynamic)
			charset1 = INTL_charset(tdbb, INTL_TTYPE(arg1), err);
		else
			charset1 = INTL_TTYPE(arg1);

		if (INTL_TTYPE(arg2) == ttype_dynamic)
			charset2 = INTL_charset(tdbb, INTL_TTYPE(arg2), err);
		else
			charset2 = INTL_TTYPE(arg2);

		if ((IS_INTL_DATA(arg1) || IS_INTL_DATA(arg2)) &&
			(charset1 != ttype_binary) &&
			(charset2 != ttype_binary) &&
			((charset1 != ttype_ascii) ||
			 (charset2 != ttype_ascii)) &&
			((charset1 != ttype_none) || (charset2 != ttype_none))
			)
			return INTL_compare(tdbb, arg1, arg2, err);

		length = CVT_get_string_ptr(arg1, &t1, &p1, NULL, 0, err);
		length2 = CVT_get_string_ptr(arg2, &t2, &p2, NULL, 0, err);

		fill = length - length2;
		if (length >= length2) {
			if (length2)
				do
					if (*p1++ != *p2++)
						if (p1[-1] > p2[-1])
							return 1;
						else
							return -1;
				while (--length2);
			if (fill > 0)
				do
					if (*p1++ != ' ')
						if (p1[-1] > ' ')
							return 1;
						else
							return -1;
				while (--fill);
			return 0;
		}
		if (length)
			do
				if (*p1++ != *p2++)
					if (p1[-1] > p2[-1])
						return 1;
					else
						return -1;
			while (--length);
		do
			if (*p2++ != ' ')
				if (' ' > p2[-1])
					return 1;
				else
					return -1;
		while (++fill);
		return 0;
	}

/* Handle hetergeneous compares */

	if (compare_priority[arg1->dsc_dtype] < compare_priority[arg2->dsc_dtype])
		return -CVT2_compare(arg2, arg1, err);

/* At this point, the type of arg1 is guaranteed to be "greater than" arg2,
   in the sense that it is the preferred type for comparing the two. */

	switch (arg1->dsc_dtype) {
		DSC desc;
		SLONG date[2];

	case dtype_timestamp:
		MOVE_CLEAR(&desc, sizeof(desc));
		desc.dsc_dtype = dtype_timestamp;
		desc.dsc_length = sizeof(date);
		desc.dsc_address = (UCHAR *) date;
		CVT_move(arg2, &desc, err);
		return CVT2_compare(arg1, &desc, err);

	case dtype_sql_time:
		MOVE_CLEAR(&desc, sizeof(desc));
		desc.dsc_dtype = dtype_sql_time;
		desc.dsc_length = sizeof(date[0]);
		desc.dsc_address = (UCHAR *) date;
		CVT_move(arg2, &desc, err);
		return CVT2_compare(arg1, &desc, err);

	case dtype_sql_date:
		MOVE_CLEAR(&desc, sizeof(desc));
		desc.dsc_dtype = dtype_sql_date;
		desc.dsc_length = sizeof(date[0]);
		desc.dsc_address = (UCHAR *) date;
		CVT_move(arg2, &desc, err);
		return CVT2_compare(arg1, &desc, err);

	case dtype_short:
		{
			SSHORT scale;
			SLONG temp1, temp2;

			if (arg2->dsc_dtype > dtype_varying)
				scale = MIN(arg1->dsc_scale, arg2->dsc_scale);
			else
				scale = arg1->dsc_scale;
			temp1 = CVT_get_long(arg1, scale, err);
			temp2 = CVT_get_long(arg2, scale, err);
			if (temp1 == temp2)
				return 0;
			if (temp1 > temp2)
				return 1;
			return -1;
		}

	case dtype_long:
		/* Since longs may overflow when scaled, use int64 instead */
	case dtype_int64:
		{
			SSHORT scale;
			SINT64 temp1, temp2;
			if (arg2->dsc_dtype > dtype_varying)
				scale = MIN(arg1->dsc_scale, arg2->dsc_scale);
			else
				scale = arg1->dsc_scale;
			temp1 = CVT_get_int64(arg1, scale, err);
			temp2 = CVT_get_int64(arg2, scale, err);
			if (temp1 == temp2)
				return 0;
			if (temp1 > temp2)
				return 1;
			return -1;
		}

	case dtype_quad:
		{
			SSHORT scale;
			SQUAD temp1, temp2;

			if (arg2->dsc_dtype > dtype_varying)
				scale = MIN(arg1->dsc_scale, arg2->dsc_scale);
			else
				scale = arg1->dsc_scale;
			temp1 = CVT_get_quad(arg1, scale, err);
			temp2 = CVT_get_quad(arg2, scale, err);
			return QUAD_COMPARE(temp1, temp2);
		}

	case dtype_real:
		{
			float temp1, temp2;

			temp1 = (float) CVT_get_double(arg1, err);
			temp2 = (float) CVT_get_double(arg2, err);
			if (temp1 == temp2)
				return 0;
			if (temp1 > temp2)
				return 1;
			return -1;
		}

	case dtype_double:
#ifdef VMS
	case dtype_d_float:
#endif
		{
			double temp1, temp2;

			temp1 = CVT_get_double(arg1, err);
			temp2 = CVT_get_double(arg2, err);
			if (temp1 == temp2)
				return 0;
			if (temp1 > temp2)
				return 1;
			return -1;
		}

	case dtype_blob:
	case dtype_array:
		reinterpret_cast < void (*) (...) > (*err) (gds_wish_list,
													gds_arg_gds,
													gds__blobnotsup,
													gds_arg_string, "compare",
													0);
		break;

	default:
		BUGCHECK(189);			/* msg 189 comparison not supported for specified data types */
		break;
	}
	return 0;
}


void CVT2_get_name(DSC * desc, TEXT * string, FPTR_VOID err)
{
/**************************************
 *
 *	C V T _ g e t _ n a m e
 *
 **************************************
 *
 * Functional description
 *	Get a name (max length 31, blank terminated) from a descriptor.
 *
 **************************************/
	USHORT length;
	VARY_STR(32) temp;			/* 31 bytes + 1 NULL */
	UTEXT *p;

	length = CVT_make_string(desc, ttype_metadata, &p,
							 (VARY *) & temp, sizeof(temp), err);
	for (; length && *p != ' '; --length)
		*string++ = *p++;

	*string = 0;
}


USHORT CVT2_make_string2(DSC * desc,
						 USHORT to_interp,
						 UCHAR ** address,
						 VARY * temp, USHORT length, STR * ptr, FPTR_VOID err)
{
/**************************************
 *
 *	C V T _ m a k e _ s t r i n g 2
 *
 **************************************
 *
 * Functional description
 *
 *     Convert the data from the desc to a string in the specified interp.
 *     The pointer to this string is returned in address.
 *
 *     Should the string not fit in the specified temporary buffer, 
 *     memory is allocated to hold the conversion.
 *
 *     It is the caller's responsibility to free the buffer.
 *     
 *
 **************************************/
	VARY *varying;
	DSC temp_desc;
	UCHAR *from_buf;
	USHORT from_len;
	USHORT from_interp;

	assert(desc != NULL);
	assert(address != NULL);
	assert(err != NULL);
	assert((((temp != NULL) && (length > 0))
			|| ((INTL_TTYPE(desc) <= dtype_any_text)
				&& (INTL_TTYPE(desc) == to_interp))) || (ptr != NULL));
	assert((ptr == NULL) || (*ptr == NULL));

	if (desc->dsc_dtype == dtype_text) {
		from_buf = desc->dsc_address;
		from_len = desc->dsc_length;
		from_interp = INTL_TTYPE(desc);
	}

	else if (desc->dsc_dtype == dtype_cstring) {
		from_buf = desc->dsc_address;
		from_len = MIN(strlen((char *) desc->dsc_address), \
					   (unsigned) (desc->dsc_length - 1));
		from_interp = INTL_TTYPE(desc);
	}

	else if (desc->dsc_dtype == dtype_varying) {
		varying = (VARY *) desc->dsc_address;
		from_buf = varying->vary_string;
		from_len =
			MIN(varying->vary_length, desc->dsc_length - sizeof(SSHORT));
		from_interp = INTL_TTYPE(desc);
	}

	if (desc->dsc_dtype <= dtype_any_text) {
		USHORT cs1, cs2;
		TDBB tdbb = NULL_TDBB;

		if (to_interp == from_interp) {
			*address = from_buf;
			return from_len;
		}

		tdbb = GET_THREAD_DATA;
		cs1 = INTL_charset(tdbb, to_interp, err);
		cs2 = INTL_charset(tdbb, from_interp, err);
		if (cs1 == cs2) {
			*address = from_buf;
			return from_len;
		}
		else {
			USHORT needed_len;
			UCHAR *tempptr;
			needed_len = INTL_convert_bytes(tdbb, cs1, NULL, 0,
											cs2, from_buf, from_len, err);
			tempptr = (UCHAR *) temp;
			if (needed_len > length) {
				*ptr = (STR) ALLOCDV(type_str, needed_len);
				(*ptr)->str_length = needed_len;
				tempptr = (*ptr)->str_data;
				length = needed_len;
			}
			length = INTL_convert_bytes(tdbb, cs1, tempptr, length,
										cs2, from_buf, from_len, err);
			*address = (UCHAR *) tempptr;
			return length;
		}
	}

/* Not string data, then  -- convert value to varying string. */

	MOVE_CLEAR(&temp_desc, sizeof(temp_desc));
	temp_desc.dsc_length = length;
	temp_desc.dsc_address = (UCHAR *) temp;
	INTL_ASSIGN_TTYPE(&temp_desc, to_interp);
	temp_desc.dsc_dtype = dtype_varying;
	CVT_move(desc, &temp_desc, err);
	*address = temp->vary_string;

	return temp->vary_length;
}
