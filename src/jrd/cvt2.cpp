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
 *
 * 2001.6.18 Claudio Valderrama: Implement comparison on blobs and blobs against
 * other datatypes by request from Ann Harrison.
 */

#include "firebird.h"
#include <string.h>
#include "../jrd/jrd.h"
#include "../jrd/val.h"
#include "../jrd/quad.h"
#include "gen/codes.h"
#include "../jrd/intl.h"
#include "../jrd/gdsassert.h"
#include "../jrd/all_proto.h"
#include "../jrd/cvt_proto.h"
#include "../jrd/cvt2_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/intl_classes.h"
#include "../jrd/gds_proto.h"
/* CVC: I needed them here. */
#include "../jrd/jrd.h"
#include "../jrd/blb_proto.h"
#include "../jrd/constants.h"
#include "../jrd/tra.h"
#include "../jrd/req.h"

#ifdef VMS
extern double MTH$CVT_D_G(), MTH$CVT_G_D();
#endif

#ifndef WORDS_BIGENDIAN
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
static const BYTE compare_priority[] = { dtype_null,	/* dtype_null through dtype_varying  */
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

#pragma FB_COMPILER_MESSAGE("Fix this! Ugly function pointer cast!")
typedef void (*pfn_cvt_private_cludge) (int, int);
typedef void (*pfn_cvt_private_cludge2) (int, int, ...);


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
	TDBB tdbb = NULL;

	// AB: Maybe we need a other error-message, but at least throw 
	// a message when 1 or both input paramters are empty.
	if (!arg1 || !arg2) {
		BUGCHECK(189);	// msg 189 comparison not supported for specified data types.
	}

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

/* Handle heterogeneous compares */

	if (compare_priority[arg1->dsc_dtype] < compare_priority[arg2->dsc_dtype])
		return -CVT2_compare(arg2, arg1, err);

/* At this point, the type of arg1 is guaranteed to be "greater than" arg2,
   in the sense that it is the preferred type for comparing the two. */

	switch (arg1->dsc_dtype)
	{
		SLONG date[2];

	case dtype_timestamp:
	{
		DSC desc;
		MOVE_CLEAR(&desc, sizeof(desc));
		desc.dsc_dtype = dtype_timestamp;
		desc.dsc_length = sizeof(date);
		desc.dsc_address = (UCHAR *) date;
		CVT_move(arg2, &desc, err);
		return CVT2_compare(arg1, &desc, err);
	}

	case dtype_sql_time:
	{
		DSC desc;
		MOVE_CLEAR(&desc, sizeof(desc));
		desc.dsc_dtype = dtype_sql_time;
		desc.dsc_length = sizeof(date[0]);
		desc.dsc_address = (UCHAR *) date;
		CVT_move(arg2, &desc, err);
		return CVT2_compare(arg1, &desc, err);
	}

	case dtype_sql_date:
	{
		DSC desc;
		MOVE_CLEAR(&desc, sizeof(desc));
		desc.dsc_dtype = dtype_sql_date;
		desc.dsc_length = sizeof(date[0]);
		desc.dsc_address = (UCHAR *) date;
		CVT_move(arg2, &desc, err);
		return CVT2_compare(arg1, &desc, err);
	}

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
		return CVT2_blob_compare(arg1, arg2, err);

	case dtype_array:
		reinterpret_cast < void (*) (...) > (*err) (gds_wish_list,
													gds_arg_gds,
													gds_blobnotsup,
													gds_arg_string, "compare",
													0);
		break;

	default:
		BUGCHECK(189);			/* msg 189 comparison not supported for specified data types */
		break;
	}
	return 0;
}


SSHORT CVT2_blob_compare(DSC * arg1, DSC * arg2, FPTR_VOID err)
{
/**************************************
 *
 *	C V T 2 _ b l o b _ c o m p a r e
 *
 **************************************
 *
 * Functional description
 *	Compare two blobs.  Return (-1, 0, 1) if a<b, a=b, or a>b.
 *  Alternatively, it will try to compare a blob against a string;
 *	in this case, the string should be the second argument.
 * CVC: Ann Harrison asked for this function to make comparisons more
 * complete in the engine.
 *
 **************************************/
	/* CHARSET_ID charset1, charset2; */
	TDBB tdbb = NULL;
	SSHORT l1, l2;
	USHORT ttype1, ttype2;
	SSHORT ret_val = 0;
	TextType *obj1 = 0, *obj2 = 0;
	DSC desc1, desc2;
	BOOLEAN bin_cmp = FALSE, both_are_text = FALSE;

	SET_TDBB(tdbb);

/* DEV_BLKCHK (node, type_nod); */

	if (arg1->dsc_dtype != dtype_blob)
		reinterpret_cast < pfn_cvt_private_cludge2 >
			(err) (gds_wish_list, gds_arg_gds, gds_datnotsup, 0);

	/* Is arg2 a blob? */
	if (arg2->dsc_dtype == dtype_blob)
	{
		BLB	blob1, blob2;
		UCHAR buffer1[BUFFER_LARGE], buffer2[BUFFER_LARGE];
	
	    /* Same blob id address? */
		if (arg1->dsc_address == arg2->dsc_address)
			return 0;
		else
		{
			/* Second test for blob id, checking relation and slot. */
			BID bid1 = (BID) arg1->dsc_address, bid2 = (BID) arg2->dsc_address;
			if (bid1->bid_relation_id == bid2->bid_relation_id &&
				(!bid1->bid_relation_id && bid1->bid_stuff.bid_blob == bid1->bid_stuff.bid_blob ||
				bid1->bid_relation_id && bid1->bid_stuff.bid_number == bid2->bid_stuff.bid_number))
				return 0;
		}
	
		if (arg1->dsc_sub_type == BLOB_text)
			ttype1 = arg1->dsc_scale;       /* Load blob character set */
		else
			ttype1 = ttype_none;
		if (arg2->dsc_sub_type == BLOB_text)
			ttype2 = arg2->dsc_scale;       /* Load blob character set */
		else
			ttype2 = ttype_none;

	    desc1.dsc_dtype = dtype_text;
		desc2.dsc_dtype = dtype_text;
		desc1.dsc_address = buffer1;
		desc2.dsc_address = buffer2;
		INTL_ASSIGN_TTYPE(&desc1, ttype1);
		INTL_ASSIGN_TTYPE(&desc2, ttype2);

	    blob1 = BLB_open(tdbb, tdbb->tdbb_request->req_transaction, (BID) arg1->dsc_address);
		blob2 = BLB_open(tdbb, tdbb->tdbb_request->req_transaction, (BID) arg2->dsc_address);

		/* Can we have a lightweight, binary comparison? */
		bin_cmp = (arg1->dsc_sub_type != BLOB_text || arg2->dsc_sub_type != BLOB_text);
		if (!bin_cmp)
		{
			both_are_text = TRUE;
			if (ttype1 == ttype_dynamic || ttype2 == ttype_dynamic)
			{
				obj1 = INTL_texttype_lookup(tdbb, ttype1, err, NULL);
				obj2 = INTL_texttype_lookup(tdbb, ttype2, err, NULL);
				ttype1 = obj1->getType();
				ttype2 = obj2->getType();
			}
			bin_cmp = (ttype1 == ttype2 || ttype1 == ttype_none || ttype1 == ttype_ascii
				|| ttype2 == ttype_none || ttype2 == ttype_ascii);
		}

		/* I'm sorry but if we can't make a binary comparison, it doesn't make sense to
		compare different charsets by pulling fixed chunks of the blobs and compare those
		slices, due to different sizes of the charsets. I do the last effort by comparing
		the charsets regarding the max bytes per character: if they use one byte, I can
		proceed with the comparison, knowing that I will at least get same number of
		characters per chunk from both blobs. */
		if (!bin_cmp && (blob1->blb_length > BUFFER_LARGE || blob2->blb_length > BUFFER_LARGE))
		{
			if (!obj1)
			{
				obj1 = INTL_texttype_lookup(tdbb, ttype1, err, NULL);
				obj2 = INTL_texttype_lookup(tdbb, ttype2, err, NULL);
			}
			assert(obj1);
			assert(obj2);
			if (obj1->getBytesPerChar() != 1 || obj2->getBytesPerChar() != 1)
				reinterpret_cast < pfn_cvt_private_cludge2 >
					(err) (gds_wish_list, gds_arg_gds, gds_datnotsup, 0);
	    }

		while (!(blob1->blb_flags & BLB_eof) && !(blob2->blb_flags & BLB_eof))
	    {
			l1 = BLB_get_segment(tdbb, blob1, buffer1, BUFFER_LARGE);
			l2 = BLB_get_segment(tdbb, blob2, buffer2, BUFFER_LARGE);
			if (bin_cmp)
			{
				SSHORT safemin, common_top = MIN(l1, l2);
				for (safemin = 0; safemin < common_top && !ret_val; ++safemin)
				{
					if (buffer1 [safemin] != buffer2[safemin])
						ret_val = buffer1[safemin] < buffer2[safemin] ? -1 : 1;
				}
				/* This is the case when we hit eof on both blobs but with different number
				of bytes read and there's still no definitive comparison.
				This is not safe with MBCS for now. */
				if (!ret_val)
				{
					UCHAR blank_char = both_are_text ? '\x20' : '\x0';
					assert (safemin == common_top);
					if (l1 < l2)
					{
						for (safemin = l1; safemin < l2 && !ret_val; ++safemin)
							if (buffer2[safemin] != blank_char)
								ret_val = buffer2[safemin] > blank_char ? -1: 1;
					}
					else if (l1 > l2)
					{
						for (safemin = l2; safemin < l1 && !ret_val; ++safemin)
							if (buffer1[safemin] != blank_char)
								ret_val = buffer1[safemin] > blank_char ? 1 : -1;
					}
					assert(ret_val || l1 == l2);
				}
			}
			else 
			{
				desc1.dsc_length = l1;
				desc2.dsc_length = l2;
				ret_val = INTL_compare(tdbb, &desc1, &desc2, err);
			}
			if (ret_val)
				break;
		}
		/* This is the case when both blobs still seem to be equivalent but one of them
		has not hit eof yet.
		This is not safe with MBCS for now. */
		if (!ret_val)
		{
			BOOLEAN eof1 = (blob1->blb_flags & BLB_eof == BLB_eof);
			BOOLEAN eof2 = (blob2->blb_flags & BLB_eof == BLB_eof);
			UCHAR blank_char = both_are_text ? '\x20' : '\x0';
			if (eof1 && !eof2)
			{
				if (bin_cmp)
				{
					SSHORT safemin;
					while (!(blob2->blb_flags & BLB_eof) && !ret_val)
					{
						l2 = BLB_get_segment(tdbb, blob2, buffer2, BUFFER_LARGE);
						for (safemin = 0; safemin < l2 && !ret_val; ++safemin)
							if (buffer2[safemin] != blank_char)
								ret_val = buffer2[safemin] > blank_char ? -1: 1;
					}
				}
				else
				{
					desc1.dsc_length = 0;
					desc2.dsc_length = l2;
					ret_val = INTL_compare(tdbb, &desc1, &desc2, err);
				}
			}
			else if (!eof1 && eof2)
			{
				if (bin_cmp)
				{
					SSHORT safemin;
					while (!(blob1->blb_flags & BLB_eof) && !ret_val)
					{
						l1 = BLB_get_segment(tdbb, blob1, buffer1, BUFFER_LARGE);
						for (safemin = 0; safemin < l1 && !ret_val; ++safemin)
							if (buffer1[safemin] != blank_char)
								ret_val = buffer1[safemin] > blank_char ? 1 : -1;
					}
				}
				else
				{
					desc1.dsc_length = l1;
					desc2.dsc_length = 0;
					ret_val = INTL_compare(tdbb, &desc1, &desc2, err);
				}
			}
		}
		BLB_close(tdbb, blob1);
		BLB_close(tdbb, blob2);
	}
	/* We do not accept arrays for now. Maybe ADS in the future. */
	else if (arg2->dsc_dtype == dtype_array)
		reinterpret_cast < pfn_cvt_private_cludge2 >
		 (err) (gds_wish_list, gds_arg_gds, gds_datnotsup, 0);
	/* The second parameter should be a string. */
	else
	{
		BLB blob1;
		UCHAR buffer1[BUFFER_LARGE];
		STR temp_str = 0;
		UCHAR *dbuf = 0;
	
		if (arg1->dsc_sub_type == BLOB_text)
			ttype1 = arg1->dsc_scale;       /* Load blob character set */
		else
			ttype1 = ttype_none;
		if (arg2->dsc_dtype <= dtype_varying)
			ttype2 = arg2->dsc_ttype;
		else
			ttype2 = ttype_none;

		desc1.dsc_dtype = dtype_text;
		/* CVC: Cannot be there since we need dbuf pointing to valid address first.
		Moved after the #if/#endif shown some lines below.
		desc1.dsc_address = dbuf; */
		INTL_ASSIGN_TTYPE(&desc1, ttype1);

		/* Can we have a lightweight, binary comparison?*/
		bin_cmp = (arg1->dsc_sub_type != BLOB_text || arg2->dsc_dtype > dtype_varying);
		if (!bin_cmp)
		{
			if (arg1->dsc_sub_type == BLOB_text)
			{
				obj1 = INTL_texttype_lookup(tdbb, ttype1, err, NULL);
				assert(obj1);
				ttype1 = obj1->getType();
				if (ttype1 == ttype_none || ttype1 == ttype_ascii)
					bin_cmp = TRUE;
			}
			if (arg2->dsc_dtype <= dtype_varying)
			{
				obj2 = INTL_texttype_lookup(tdbb, ttype2, err, NULL);
				assert(obj2);
				ttype2 = obj2->getType();
				if (ttype2 == ttype_none || ttype2 == ttype_ascii)
					bin_cmp = TRUE;
			}
			if (ttype1 == ttype2)
				bin_cmp = TRUE;
		}

		/* I will stop execution here until I can complete this function. */
		if (!bin_cmp)
			reinterpret_cast < pfn_cvt_private_cludge2 >
			 (err) (gds_wish_list, gds_arg_gds, gds_datnotsup, 0);

		if (arg2->dsc_length > BUFFER_LARGE)
		{
			temp_str = FB_NEW_RPT(*tdbb->tdbb_default, sizeof(UCHAR) * arg2->dsc_length) str();
			dbuf = temp_str->str_data;
	    }
		else
			dbuf = buffer1;

		desc1.dsc_address = dbuf;
		blob1 = BLB_open(tdbb, tdbb->tdbb_request->req_transaction, (BID) arg1->dsc_address);
	    l1 = BLB_get_segment(tdbb, blob1, dbuf, arg2->dsc_length);
		desc1.dsc_length = l1;
	    ret_val = CVT2_compare(&desc1, arg2, err);
		BLB_close(tdbb, blob1);

		/*  do a block deallocation of local variables */
		if (temp_str)
			delete temp_str;
	}
	return ret_val;
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
 *	Get a name (max length 31, NULL terminated) from a descriptor.
 *
 **************************************/
	USHORT length;
	VARY_STR(32) temp;			/* 31 bytes + 1 NULL */
	const char* p;

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
		from_buf = reinterpret_cast<UCHAR*>(varying->vary_string);
		from_len =
			MIN(varying->vary_length, (USHORT) (desc->dsc_length - sizeof(SSHORT)));
		from_interp = INTL_TTYPE(desc);
	}

	if (desc->dsc_dtype <= dtype_any_text) {
		USHORT cs1, cs2;
		TDBB tdbb = NULL;

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
				*ptr = FB_NEW_RPT(*tdbb->tdbb_default, needed_len) str();
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
	*address = reinterpret_cast<UCHAR*>(temp->vary_string);

	return temp->vary_length;
}
