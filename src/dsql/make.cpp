/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		make.c
 *	DESCRIPTION:	Routines to make various blocks.
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
 * 2001.11.21 Claudio Valderrama: Finally solved the mystery of DSQL
 * not recognizing when a UDF returns NULL. This fixes SF bug #484399.
 * See case nod_udf in MAKE_desc().
 * 2001.02.23 Claudio Valderrama: Fix SF bug #518350 with substring()
 * and text blobs containing charsets other than ASCII/NONE/BINARY.
 * 2002.07.30 Arno Brinkman: 
 *   COALESCE, CASE support added
 *   procedure MAKE_desc_from_list added 
 * 2003.01.25 Dmitry Yemanov: Fixed problem with concatenation which
 *   trashed RDB$FIELD_LENGTH in the system tables. This change may
 *   potentially interfere with the one made by Claudio one year ago.
 */
 
//This MUST be before any other includes
#ifdef DARWIN
#define _STLP_CCTYPE
#endif

#include "firebird.h"
#include <ctype.h>
#include <string.h>
#include "../dsql/dsql.h"
#include "../dsql/node.h"
#include "../dsql/sym.h"
#include "../jrd/gds.h"
#include "../jrd/intl.h"
#include "../jrd/constants.h"
#include "../jrd/align.h"
#include "../dsql/alld_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/hsh_proto.h"
#include "../dsql/make_proto.h"
#include "../jrd/thd_proto.h"
#include "../jrd/dsc_proto.h"
#include "../jrd/cvt_proto.h"
#include "../common/config/config.h"


ASSERT_FILENAME					/* declare things assert() needs */
ASSERT_BLKCHK_MSG
/* InterBase provides transparent conversion from string to date in
 * contexts where it makes sense.  This macro checks a descriptor to
 * see if it is something that *could* represent a date value
 */
#define COULD_BE_DATE(d)	(DTYPE_IS_DATE((d).dsc_dtype) || ((d).dsc_dtype <= dtype_any_text))
/* One of d1,d2 is time, the other is date */
#define IS_DATE_AND_TIME(d1,d2)	\
  ((((d1).dsc_dtype==dtype_sql_time)&&((d2).dsc_dtype==dtype_sql_date)) || \
   (((d2).dsc_dtype==dtype_sql_time)&&((d1).dsc_dtype==dtype_sql_date)))


/**
  
 	MAKE_constant
  
    @brief	Make a constant node.
 

    @param constant
    @param numeric_flag

 **/
DSQL_NOD MAKE_constant(STR constant, int numeric_flag)
{
	DSQL_NOD node;
	TSQL tdsql;

	tdsql = GET_THREAD_DATA;

	node = FB_NEW_RPT(*tdsql->tsql_default,
						(numeric_flag == CONSTANT_TIMESTAMP ||
						  numeric_flag == CONSTANT_SINT64) ? 2 : 1) dsql_nod;
	node->nod_type = nod_constant;

	if (numeric_flag == CONSTANT_SLONG) {
		node->nod_desc.dsc_dtype = dtype_long;
		node->nod_desc.dsc_length = sizeof(SLONG);
		node->nod_desc.dsc_scale = 0;
		node->nod_desc.dsc_sub_type = 0;
		node->nod_desc.dsc_address = (UCHAR *) node->nod_arg;
		node->nod_arg[0] = (DSQL_NOD) constant;
	}
	else if (numeric_flag == CONSTANT_DOUBLE) {
		DEV_BLKCHK(constant, dsql_type_str);

		/* This is a numeric value which is transported to the engine as
		 * a string.  The engine will convert it. Use dtype_double so that
		 the engine can distinguish it from an actual string.
		 Note: Due to the size of dsc_scale we are limited to numeric
		 constants of less than 256 bytes.  
		 */
		node->nod_desc.dsc_dtype = dtype_double;
		node->nod_desc.dsc_scale = static_cast < char >(constant->str_length);	/* Scale has no use for double */
		node->nod_desc.dsc_sub_type = 0;
		node->nod_desc.dsc_length = sizeof(double);
		node->nod_desc.dsc_address = reinterpret_cast<UCHAR*>(constant->str_data);
		node->nod_desc.dsc_ttype = ttype_ascii;
		node->nod_arg[0] = (DSQL_NOD) constant;
	}
	else if (numeric_flag == CONSTANT_SINT64) {
		/* We convert the string to an int64.  We treat the two adjacent
		   32-bit words node->nod_arg[0] and node->nod_arg[1] as a
		   64-bit integer: if we ever port to a platform which requires
		   8-byte alignment of int64 data, we will have to force 8-byte
		   alignment of node->nod_arg, which is now only guaranteed
		   4-byte alignment.    -- ChrisJ 1999-02-20 */

		UINT64 value = 0;
		const char* p = constant->str_data;

		node->nod_desc.dsc_dtype = dtype_int64;
		node->nod_desc.dsc_length = sizeof(SINT64);
		node->nod_desc.dsc_scale = 0;
		node->nod_desc.dsc_sub_type = 0;
		node->nod_desc.dsc_address = (UCHAR *) node->nod_arg;

		/* Now convert the string to an int64.  We can omit testing for
		   overflow, because we would never have gotten here if yylex
		   hadn't recognized the string as a valid 64-bit integer value.
		   We *might* have "9223372936854775808", which works an an int64
		   only if preceded by a '-', but that issue is handled in GEN_expr,
		   and need not be addressed here. */

		while (isdigit(*p))
			value = 10 * value + (*(p++) - '0');
		if (*p++ == '.') {
			while (isdigit(*p)) {
				value = 10 * value + (*p++ - '0');
				node->nod_desc.dsc_scale--;
			}
		}

		*(UINT64 *) (node->nod_desc.dsc_address) = value;

	}
	else if (numeric_flag == CONSTANT_DATE ||
			 numeric_flag == CONSTANT_TIME ||
			 numeric_flag == CONSTANT_TIMESTAMP) {
		DSC tmp;

		/* Setup the constant's descriptor */

		switch (numeric_flag) {
		case CONSTANT_DATE:
			node->nod_desc.dsc_dtype = dtype_sql_date;
			break;
		case CONSTANT_TIME:
			node->nod_desc.dsc_dtype = dtype_sql_time;
			break;
		case CONSTANT_TIMESTAMP:
			node->nod_desc.dsc_dtype = dtype_timestamp;
			break;
		};
		node->nod_desc.dsc_sub_type = 0;
		node->nod_desc.dsc_scale = 0;
		node->nod_desc.dsc_length = type_lengths[node->nod_desc.dsc_dtype];
		node->nod_desc.dsc_address = (UCHAR *) node->nod_arg;

		/* Set up a descriptor to point to the string */

		tmp.dsc_dtype = dtype_text;
		tmp.dsc_scale = 0;
		tmp.dsc_flags = 0;
		tmp.dsc_ttype = ttype_ascii;
		tmp.dsc_length = static_cast<USHORT>(constant->str_length);
		tmp.dsc_address = reinterpret_cast<UCHAR*>(constant->str_data);

		/* Now invoke the string_to_date/time/timestamp routines */

		CVT_move(&tmp, &node->nod_desc,
				 reinterpret_cast<void (*)()> (ERRD_post));
	}
	else {
		assert(numeric_flag == CONSTANT_STRING);
		DEV_BLKCHK(constant, dsql_type_str);

		node->nod_desc.dsc_dtype = dtype_text;
		node->nod_desc.dsc_sub_type = 0;
		node->nod_desc.dsc_scale = 0;
		node->nod_desc.dsc_length =
			static_cast<USHORT>(constant->str_length);
		node->nod_desc.dsc_address = reinterpret_cast<UCHAR*>(constant->str_data);
		node->nod_desc.dsc_ttype = ttype_dynamic;
		/* carry a pointer to the constant to resolve character set in pass1 */
		node->nod_arg[0] = (DSQL_NOD) constant;
	}

	return node;
}


/**
  
 	MAKE_str-constant
  
    @brief	Make a constant node when the 
       character set ID is already known.
 

    @param constant
    @param character_set

 **/
DSQL_NOD MAKE_str_constant(STR constant, SSHORT character_set)
{
	DSQL_NOD node;
	TSQL tdsql;

	tdsql = GET_THREAD_DATA;

	node = FB_NEW_RPT(*tdsql->tsql_default, 1) dsql_nod;
	node->nod_type = nod_constant;

	DEV_BLKCHK(constant, dsql_type_str);

	node->nod_desc.dsc_dtype = dtype_text;
	node->nod_desc.dsc_sub_type = 0;
	node->nod_desc.dsc_scale = 0;
	node->nod_desc.dsc_length = static_cast<USHORT>(constant->str_length);
	node->nod_desc.dsc_address = reinterpret_cast<UCHAR*>(constant->str_data);
	node->nod_desc.dsc_ttype = character_set;
/* carry a pointer to the constant to resolve character set in pass1 */
	node->nod_arg[0] = (DSQL_NOD) constant;

	return node;
}


/**
  
 	MAKE_cstring
  
    @brief	Make a string node for a string whose
 	length is not known, but is null-terminated.
 

    @param str

 **/
STR MAKE_cstring(const char* str)
{

	return MAKE_string(str, strlen(str));
}


/**
  
 	MAKE_desc
  
    @brief	Make a descriptor from input node.
 

    @param desc
    @param node

 **/
void MAKE_desc( DSC * desc, DSQL_NOD node)
{
	DSC desc1, desc2;
	USHORT dtype, dtype1, dtype2;
	MAP map;
	DSQL_CTX context;
	DSQL_REL relation;
	UDF udf;
	DSQL_FLD field;

	DEV_BLKCHK(node, dsql_type_nod);

/* If we already know the datatype, don't worry about anything */

	if (node->nod_desc.dsc_dtype) {
		*desc = node->nod_desc;
		return;
	}

	switch (node->nod_type) {
	case nod_agg_count:
/* count2
    case nod_agg_distinct:
*/
		desc->dsc_dtype = dtype_long;
		desc->dsc_length = sizeof(SLONG);
		desc->dsc_sub_type = 0;
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
		return;

	case nod_map:
		map = (MAP) node->nod_arg[e_map_map];
		MAKE_desc(desc, map->map_node);
		return;

	case nod_agg_min:
	case nod_agg_max:
		MAKE_desc(desc, node->nod_arg[0]);
		desc->dsc_flags = DSC_nullable;
		return;

	case nod_agg_average:
		MAKE_desc(desc, node->nod_arg[0]);
		desc->dsc_flags = DSC_nullable;
		dtype = desc->dsc_dtype;
		if (!DTYPE_CAN_AVERAGE(dtype))
			ERRD_post(gds_expression_eval_err, 0);
		return;

	case nod_agg_average2:
		MAKE_desc(desc, node->nod_arg[0]);
		desc->dsc_flags = DSC_nullable;
		dtype = desc->dsc_dtype;
		if (!DTYPE_CAN_AVERAGE(dtype))
			ERRD_post(gds_expression_eval_err, 0);
		if (DTYPE_IS_EXACT(dtype)) {
			desc->dsc_dtype = dtype_int64;
			desc->dsc_length = sizeof(SINT64);
			node->nod_flags |= NOD_COMP_DIALECT;
		}
		else {
			desc->dsc_dtype = dtype_double;
			desc->dsc_length = sizeof(double);
		}
		return;

	case nod_agg_total:
		MAKE_desc(desc, node->nod_arg[0]);
		if (desc->dsc_dtype == dtype_short) {
			desc->dsc_dtype = dtype_long;
			desc->dsc_length = sizeof(SLONG);
		}
		else if (desc->dsc_dtype == dtype_int64) {
			desc->dsc_dtype = dtype_double;
			desc->dsc_length = sizeof(double);
		}
		desc->dsc_flags = DSC_nullable;
		return;

	case nod_agg_total2:
		MAKE_desc(desc, node->nod_arg[0]);
		dtype = desc->dsc_dtype;
		if (DTYPE_IS_EXACT(dtype)) {
			desc->dsc_dtype = dtype_int64;
			desc->dsc_length = sizeof(SINT64);
			node->nod_flags |= NOD_COMP_DIALECT;
		}
		else {
			desc->dsc_dtype = dtype_double;
			desc->dsc_length = sizeof(double);
		}
		desc->dsc_flags = DSC_nullable;
		return;

	case nod_concatenate:
		{
		MAKE_desc(&desc1, node->nod_arg[0]);
		MAKE_desc(&desc2, node->nod_arg[1]);
		desc->dsc_scale = 0;
		desc->dsc_dtype = dtype_varying;
		if (desc1.dsc_dtype <= dtype_any_text)
			desc->dsc_ttype = desc1.dsc_ttype;
		else
			desc->dsc_ttype = ttype_ascii;
		ULONG length = sizeof(USHORT) +
			DSC_string_length(&desc1) + DSC_string_length(&desc2);
		if (length > MAX_SSHORT) {
			length = MAX_SSHORT;
			/* dimitr: should we post a warning about truncated descriptor length?
			ERRD_post_warning (gds_sqlwarn,
							   gds_arg_gds, gds_imp_exc, 
							   gds_arg_gds, gds_blktoobig,
							   0);
			*/
		}
		desc->dsc_length = length;
		desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;
		}
		return;

	case nod_upcase:
    case nod_substr:
		MAKE_desc(&desc1, node->nod_arg[0]);
		if (desc1.dsc_dtype <= dtype_any_text) {
			*desc = desc1;
			return;
		}
		desc->dsc_dtype = dtype_varying;
		desc->dsc_scale = 0;

        /* Beware that JRD treats substring() always as returning CHAR
           instead	of VARCHAR for historical reasons. */
        if (node->nod_type == nod_substr && desc1.dsc_dtype == dtype_blob) {
            SLONG len = 0;
            DSQL_NOD for_node = node->nod_arg [e_substr_length];
            assert (for_node->nod_desc.dsc_dtype == dtype_long);
            /* Migrate the charset from the blob to the string. */
            desc->dsc_ttype = desc1.dsc_scale;
            len = *(SLONG *) for_node->nod_desc.dsc_address;
            /* For now, our substring() doesn't handle MBCS blobs,
               neither at the DSQL layer nor at the JRD layer. */
            if (len <= MAX_COLUMN_SIZE - (SLONG) sizeof (USHORT) && len >= 0) {
                desc->dsc_length = sizeof (USHORT) + len;
            }
            else {
                ERRD_post (gds_sqlerr, gds_arg_number, (SLONG) -204,
                           gds_arg_gds, gds_dsql_datatype_err,
                           gds_arg_gds, gds_imp_exc, 
                           gds_arg_gds, gds_field_name,
                           gds_arg_string, "substring()", /* field->fld_name,*/
                           0);
            }
		}
        else {
            desc->dsc_ttype = ttype_ascii;
            desc->dsc_length = sizeof (USHORT) + DSC_string_length (&desc1);
		}
        desc->dsc_flags = desc1.dsc_flags & DSC_nullable;
        return;

	case nod_cast:
		field = (DSQL_FLD) node->nod_arg[e_cast_target];
		MAKE_desc_from_field(desc, field);
		MAKE_desc(&desc1, node->nod_arg[e_cast_source]);
		desc->dsc_flags = desc1.dsc_flags & DSC_nullable;
		return;

	case nod_simple_case:
		MAKE_desc_from_list(&desc1, node->nod_arg[e_simple_case_results], "CASE");
		*desc = desc1;
		return;

	case nod_searched_case:
		MAKE_desc_from_list(&desc1, node->nod_arg[e_searched_case_results], "CASE");
		*desc = desc1;
		return;

    case nod_coalesce:
		MAKE_desc_from_list(&desc1, node->nod_arg[0], "COALESCE");
		*desc = desc1;
		return;

#ifdef DEV_BUILD
	case nod_collate:
		ERRD_bugcheck("Not expecting nod_collate in dsql/MAKE_desc");
		return;
#endif

	case nod_add:
	case nod_subtract:
		MAKE_desc(&desc1, node->nod_arg[0]);
		MAKE_desc(&desc2, node->nod_arg[1]);
		dtype1 = desc1.dsc_dtype;
		dtype2 = desc2.dsc_dtype;

		if (dtype_int64 == dtype1)
			dtype1 = dtype_double;
		if (dtype_int64 == dtype2)
			dtype2 = dtype_double;

		dtype = MAX(dtype1, dtype2);

		if (DTYPE_IS_BLOB(dtype))
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 607,
					  gds_arg_gds, gds_dsql_no_blob_array, 0);

		desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;
		switch (dtype) {
		case dtype_sql_time:
		case dtype_sql_date:
			/* Forbid <date/time> +- <string> */
			if (IS_DTYPE_ANY_TEXT(desc1.dsc_dtype) ||
				IS_DTYPE_ANY_TEXT(desc2.dsc_dtype))
					ERRD_post(gds_expression_eval_err, 0);

		case dtype_timestamp:

			/* Allow <timestamp> +- <string> (historical) */
			if (COULD_BE_DATE(desc1) && COULD_BE_DATE(desc2)) {
				if (node->nod_type == nod_subtract) {
					/* <any date> - <any date> */

					/* Legal permutations are:
					   <timestamp> - <timestamp>
					   <timestamp> - <date>
					   <date> - <date>
					   <date> - <timestamp>
					   <time> - <time>
					   <timestamp> - <string>
					   <string> - <timestamp> 
					   <string> - <string>   */

					if (IS_DTYPE_ANY_TEXT(dtype1))
						dtype = dtype_timestamp;
					else if (IS_DTYPE_ANY_TEXT(dtype2))
						dtype = dtype_timestamp;
					else if (dtype1 == dtype2)
						dtype = dtype1;
					else if ((dtype1 == dtype_timestamp) &&
							 (dtype2 == dtype_sql_date))
							dtype = dtype_timestamp;
					else if ((dtype2 == dtype_timestamp) &&
							 (dtype1 == dtype_sql_date))
							dtype = dtype_timestamp;
					else
						ERRD_post(gds_expression_eval_err, 0);

					if (dtype == dtype_sql_date) {
						desc->dsc_dtype = dtype_long;
						desc->dsc_length = sizeof(SLONG);
						desc->dsc_scale = 0;
					}
					else if (dtype == dtype_sql_time) {
						desc->dsc_dtype = dtype_long;
						desc->dsc_length = sizeof(SLONG);
						desc->dsc_scale = ISC_TIME_SECONDS_PRECISION_SCALE;
					}
					else {
						assert(dtype == dtype_timestamp);
						desc->dsc_dtype = dtype_double;
						desc->dsc_length = sizeof(double);
						desc->dsc_scale = 0;
					}
				}
				else if (IS_DATE_AND_TIME(desc1, desc2)) {
					/* <date> + <time> */
					/* <time> + <date> */
					desc->dsc_dtype = dtype_timestamp;
					desc->dsc_length = type_lengths[dtype_timestamp];
					desc->dsc_scale = 0;
				}
				else
					/* <date> + <date> */
					ERRD_post(gds_expression_eval_err, 0);
			}
			else if (DTYPE_IS_DATE(desc1.dsc_dtype) ||
					 /* <date> +/- <non-date> */
					 (node->nod_type == nod_add))
				/* <non-date> + <date> */
			{
				desc->dsc_dtype = desc1.dsc_dtype;
				if (!DTYPE_IS_DATE(desc->dsc_dtype))
					desc->dsc_dtype = desc2.dsc_dtype;
				assert(DTYPE_IS_DATE(desc->dsc_dtype));
				desc->dsc_length = type_lengths[desc->dsc_dtype];
				desc->dsc_scale = 0;
			}
			else {
				/* <non-date> - <date> */
				assert(node->nod_type == nod_subtract);
				ERRD_post(gds_expression_eval_err, 0);
			}
			return;

		case dtype_varying:
		case dtype_cstring:
		case dtype_text:
		case dtype_double:
		case dtype_real:
			desc->dsc_dtype = dtype_double;
			desc->dsc_sub_type = 0;
			desc->dsc_scale = 0;
			desc->dsc_length = sizeof(double);
			return;

		default:
			desc->dsc_dtype = dtype_long;
			desc->dsc_sub_type = 0;
			desc->dsc_length = sizeof(SLONG);
			desc->dsc_scale = MIN(NUMERIC_SCALE(desc1), NUMERIC_SCALE(desc2));
			break;
		}
		return;

	case nod_add2:
	case nod_subtract2:
		MAKE_desc(&desc1, node->nod_arg[0]);
		MAKE_desc(&desc2, node->nod_arg[1]);
		dtype1 = desc1.dsc_dtype;
		dtype2 = desc2.dsc_dtype;

		/* Arrays and blobs can never partipate in addition/subtraction */
		if (DTYPE_IS_BLOB(desc1.dsc_dtype) || DTYPE_IS_BLOB(desc2.dsc_dtype))
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 607,
					  gds_arg_gds, gds_dsql_no_blob_array, 0);

		/* In Dialect 2 or 3, strings can never partipate in addition / sub 
		   (Use a specific cast instead) */
		if (IS_DTYPE_ANY_TEXT(desc1.dsc_dtype) ||
			IS_DTYPE_ANY_TEXT(desc2.dsc_dtype))
				ERRD_post(gds_expression_eval_err, 0);

		/* Determine the TYPE of arithmetic to perform, store it
		   in dtype.  Note:  this is different from the result of
		   the operation, as <timestamp>-<timestamp> uses
		   <timestamp> arithmetic, but returns a <double> */
		if (DTYPE_IS_EXACT(desc1.dsc_dtype)
			&& DTYPE_IS_EXACT(desc2.dsc_dtype)) dtype = dtype_int64;
		else if (DTYPE_IS_NUMERIC(desc1.dsc_dtype)
				 && DTYPE_IS_NUMERIC(desc2.dsc_dtype)) {
			assert(DTYPE_IS_APPROX(desc1.dsc_dtype) ||
				   DTYPE_IS_APPROX(desc2.dsc_dtype));
			dtype = dtype_double;
		}
		else {
			/* mixed numeric and non-numeric: */

			assert(DTYPE_IS_DATE(dtype1) || DTYPE_IS_DATE(dtype2));

			/* The MAX(dtype) rule doesn't apply with dtype_int64 */

			if (dtype_int64 == dtype1)
				dtype1 = dtype_double;
			if (dtype_int64 == dtype2)
				dtype2 = dtype_double;

			dtype = MAX(dtype1, dtype2);
		}

		desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;
		switch (dtype) {
		case dtype_sql_time:
		case dtype_sql_date:
		case dtype_timestamp:

			if ((DTYPE_IS_DATE(dtype1) || (dtype1 == dtype_null)) &&
				(DTYPE_IS_DATE(dtype2) || (dtype2 == dtype_null))) {
				if (node->nod_type == nod_subtract2) {
					/* <any date> - <any date> */
					/* Legal permutations are:
					   <timestamp> - <timestamp>
					   <timestamp> - <date>
					   <date> - <date>
					   <date> - <timestamp>
					   <time> - <time> */

					if (dtype1 == dtype2)
						dtype = dtype1;
					else if ((dtype1 == dtype_timestamp) &&
							 (dtype2 == dtype_sql_date))
							dtype = dtype_timestamp;
					else if ((dtype2 == dtype_timestamp) &&
							 (dtype1 == dtype_sql_date))
							dtype = dtype_timestamp;
					else
						ERRD_post(gds_expression_eval_err, 0);

					if (dtype == dtype_sql_date) {
						desc->dsc_dtype = dtype_long;
						desc->dsc_length = sizeof(SLONG);
						desc->dsc_scale = 0;
					}
					else if (dtype == dtype_sql_time) {
						desc->dsc_dtype = dtype_long;
						desc->dsc_length = sizeof(SLONG);
						desc->dsc_scale = ISC_TIME_SECONDS_PRECISION_SCALE;
					}
					else {
						assert(dtype == dtype_timestamp);
						desc->dsc_dtype = dtype_int64;
						desc->dsc_length = sizeof(SINT64);
						desc->dsc_scale = -9;
					}
				}
				else if (IS_DATE_AND_TIME(desc1, desc2)) {
					/* <date> + <time> */
					/* <time> + <date> */
					desc->dsc_dtype = dtype_timestamp;
					desc->dsc_length = type_lengths[dtype_timestamp];
					desc->dsc_scale = 0;
				}
				else
					/* <date> + <date> */
					ERRD_post(gds_expression_eval_err, 0);
			}
			else if (DTYPE_IS_DATE(desc1.dsc_dtype) ||
					 /* <date> +/- <non-date> */
					 (node->nod_type == nod_add2))
				/* <non-date> + <date> */
			{
				desc->dsc_dtype = desc1.dsc_dtype;
				if (!DTYPE_IS_DATE(desc->dsc_dtype))
					desc->dsc_dtype = desc2.dsc_dtype;
				assert(DTYPE_IS_DATE(desc->dsc_dtype));
				desc->dsc_length = type_lengths[desc->dsc_dtype];
				desc->dsc_scale = 0;
			}
			else {
				/* <non-date> - <date> */
				assert(node->nod_type == nod_subtract2);
				ERRD_post(gds_expression_eval_err, 0);
			}
			return;

		case dtype_varying:
		case dtype_cstring:
		case dtype_text:
		case dtype_double:
		case dtype_real:
			desc->dsc_dtype = dtype_double;
			desc->dsc_sub_type = 0;
			desc->dsc_scale = 0;
			desc->dsc_length = sizeof(double);
			return;

		case dtype_short:
		case dtype_long:
		case dtype_int64:
			desc->dsc_dtype = dtype_int64;
			desc->dsc_sub_type = 0;
			desc->dsc_length = sizeof(SINT64);

			/* The result type is int64 because both operands are
			   exact numeric: hence we don't need the NUMERIC_SCALE
			   macro here. */
			assert(DTYPE_IS_EXACT(desc1.dsc_dtype));
			assert(DTYPE_IS_EXACT(desc2.dsc_dtype));

			desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
			node->nod_flags |= NOD_COMP_DIALECT;
			break;

		default:
			/* a type which cannot participate in an add or subtract */
			ERRD_post(gds_expression_eval_err, 0);
		}
		return;

	case nod_multiply:
		MAKE_desc(&desc1, node->nod_arg[0]);
		MAKE_desc(&desc2, node->nod_arg[1]);
		dtype = DSC_multiply_blr4_result[desc1.dsc_dtype][desc2.dsc_dtype];

		if (dtype_null == dtype)
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 607,
					  gds_arg_gds, gds_dsql_no_blob_array, 0);

		desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;
		switch (dtype) {
		case dtype_double:
			desc->dsc_dtype = dtype_double;
			desc->dsc_sub_type = 0;
			desc->dsc_scale = 0;
			desc->dsc_length = sizeof(double);
			break;

		case dtype_long:
			desc->dsc_dtype = dtype_long;
			desc->dsc_sub_type = 0;
			desc->dsc_length = sizeof(SLONG);
			desc->dsc_scale = NUMERIC_SCALE(desc1) + NUMERIC_SCALE(desc2);
			break;

		default:
			ERRD_post(gds_expression_eval_err, 0);
		}
		return;

	case nod_multiply2:
		MAKE_desc(&desc1, node->nod_arg[0]);
		MAKE_desc(&desc2, node->nod_arg[1]);
		dtype = DSC_multiply_result[desc1.dsc_dtype][desc2.dsc_dtype];

		desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;
		switch (dtype) {
		case dtype_double:
			desc->dsc_dtype = dtype_double;
			desc->dsc_sub_type = 0;
			desc->dsc_scale = 0;
			desc->dsc_length = sizeof(double);
			break;

		case dtype_int64:
			desc->dsc_dtype = dtype_int64;
			desc->dsc_sub_type = 0;
			desc->dsc_length = sizeof(SINT64);
			desc->dsc_scale = NUMERIC_SCALE(desc1) + NUMERIC_SCALE(desc2);
			node->nod_flags |= NOD_COMP_DIALECT;
			break;

		default:
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 607,
					  gds_arg_gds, gds_dsql_no_blob_array, 0);
			break;
		}
		return;

	case nod_count:
		desc->dsc_dtype = dtype_long;
		desc->dsc_sub_type = 0;
		desc->dsc_scale = 0;
		desc->dsc_length = sizeof(SLONG);
		desc->dsc_flags = 0;
		return;

	case nod_divide:
		MAKE_desc(&desc1, node->nod_arg[0]);
		MAKE_desc(&desc2, node->nod_arg[1]);

		dtype1 = desc1.dsc_dtype;
		if (dtype_int64 == dtype1)
			dtype1 = dtype_double;

		dtype2 = desc2.dsc_dtype;
		if (dtype_int64 == dtype2)
			dtype2 = dtype_double;

		dtype = MAX(dtype1, dtype2);

		if (!DTYPE_CAN_DIVIDE(dtype))
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 607,
					  gds_arg_gds, gds_dsql_no_blob_array, 0);
		desc->dsc_dtype = dtype_double;
		desc->dsc_length = sizeof(double);
		desc->dsc_scale = 0;
		desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;
		return;

	case nod_divide2:
		MAKE_desc(&desc1, node->nod_arg[0]);
		MAKE_desc(&desc2, node->nod_arg[1]);
		dtype = DSC_multiply_result[desc1.dsc_dtype][desc2.dsc_dtype];
		desc->dsc_dtype = static_cast<UCHAR>(dtype);

		desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;
		switch (dtype) {
		case dtype_int64:
			desc->dsc_length = sizeof(SINT64);
			desc->dsc_scale = NUMERIC_SCALE(desc1) + NUMERIC_SCALE(desc2);
			node->nod_flags |= NOD_COMP_DIALECT;
			break;

		case dtype_double:
			desc->dsc_length = sizeof(double);
			desc->dsc_scale = 0;
			break;

		default:
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 607,
					  gds_arg_gds, gds_dsql_no_blob_array, 0);
			break;
		}

		return;

	case nod_negate:
		MAKE_desc(desc, node->nod_arg[0]);
		if (!DTYPE_CAN_NEGATE(desc->dsc_dtype))
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 607,
					  gds_arg_gds, gds_dsql_no_blob_array, 0);
		return;

	case nod_alias:
		MAKE_desc(desc, node->nod_arg[e_alias_value]);
		return;

	case nod_dbkey:
		/* Fix for bug 10072 check that the target is a relation */
		context = (DSQL_CTX) node->nod_arg[0]->nod_arg[0];
        relation = context->ctx_relation;
		if (relation != 0) {
			desc->dsc_dtype = dtype_text;
			desc->dsc_length = relation->rel_dbkey_length;
			desc->dsc_flags = 0;
			desc->dsc_ttype = ttype_binary;
		}
		else {
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 607,
					  gds_arg_gds, isc_dsql_dbkey_from_non_table, 0);
		}
		return;

	case nod_udf:
		udf = (UDF) node->nod_arg[0];
		desc->dsc_dtype = static_cast<UCHAR>(udf->udf_dtype);
		desc->dsc_length = udf->udf_length;
		desc->dsc_scale = static_cast<SCHAR>(udf->udf_scale);
        /* CVC: Setting flags to zero obviously impeded DSQL to acknowledge
           the fact that any UDF can return NULL simply returning a NULL 
           pointer. */
        desc->dsc_flags = DSC_nullable;
		
		if (desc->dsc_dtype <= dtype_any_text) {		
			desc->dsc_ttype = udf->udf_character_set_id;
		}
		else {
			desc->dsc_ttype = udf->udf_sub_type;
		}
		return;

	case nod_gen_id:
		desc->dsc_dtype = dtype_long;
		desc->dsc_sub_type = 0;
		desc->dsc_scale = 0;
		desc->dsc_length = sizeof(SLONG);
		desc->dsc_flags = node->nod_arg[e_gen_id_value]->nod_desc.dsc_flags;
		return;

	case nod_gen_id2:
		desc->dsc_dtype = dtype_int64;
		desc->dsc_sub_type = 0;
		desc->dsc_scale = 0;
		desc->dsc_length = sizeof(SINT64);
		desc->dsc_flags = node->nod_arg[e_gen_id_value]->nod_desc.dsc_flags;
		node->nod_flags |= NOD_COMP_DIALECT;
		return;

    case nod_limit:
        if (node->nod_desc.dsc_scale <= SQL_DIALECT_V5) {
            desc->dsc_dtype = dtype_long; 
            desc->dsc_length = sizeof (SLONG);
        }
        else {
            desc->dsc_dtype = dtype_int64; 
            desc->dsc_length = sizeof (SINT64);
        }
        desc->dsc_sub_type = 0;
        desc->dsc_scale = 0;
        desc->dsc_flags = 0; /* Can first/skip accept NULL in the future? */
        return;

	case nod_field:
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 203,
				  gds_arg_gds, gds_dsql_field_ref, 0);
		return;

	case nod_user_name:
    case nod_current_role:
		desc->dsc_dtype = dtype_varying;
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
		desc->dsc_ttype = ttype_dynamic;
		desc->dsc_length = USERNAME_LENGTH + sizeof(USHORT);
		return;

	case nod_internal_info:
		desc->dsc_dtype = dtype_long;
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
		desc->dsc_length = sizeof(SLONG);
		return;

	case nod_current_time:
		desc->dsc_dtype = dtype_sql_time;
		desc->dsc_sub_type = 0;
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
		desc->dsc_length = type_lengths[desc->dsc_dtype];
		return;

	case nod_current_date:
		desc->dsc_dtype = dtype_sql_date;
		desc->dsc_sub_type = 0;
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
		desc->dsc_length = type_lengths[desc->dsc_dtype];
		return;

	case nod_current_timestamp:
		desc->dsc_dtype = dtype_timestamp;
		desc->dsc_sub_type = 0;
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
		desc->dsc_length = type_lengths[desc->dsc_dtype];
		return;

	case nod_extract:
		MAKE_desc(&desc1, node->nod_arg[e_extract_value]);
		desc->dsc_sub_type = 0;
		desc->dsc_scale = 0;
		desc->dsc_flags = (desc1.dsc_flags & DSC_nullable);
		if (*(ULONG *) node->nod_arg[e_extract_part]->nod_desc.dsc_address
			== blr_extract_second) {
			/* QUADDATE - maybe this should be DECIMAL(6,4) */
			desc->dsc_dtype = dtype_long;
			desc->dsc_scale = ISC_TIME_SECONDS_PRECISION_SCALE;
			desc->dsc_length = sizeof(ULONG);
			return;
		}
		desc->dsc_dtype = dtype_short;
		desc->dsc_length = sizeof(SSHORT);
		return;

	case nod_parameter:
		/* We don't actually know the datatype of a parameter -
		   we have to guess it based on the context that the 
		   parameter appears in. (This is done is pass1.c::set_parameter_type())
		   However, a parameter can appear as part of an expression.
		   As MAKE_desc is used for both determination of parameter
		   types and for expression type checking, we just continue. */
		return;

	case nod_null:
		/* This occurs when SQL statement specifies a literal NULL, eg:
		 *  SELECT NULL FROM TABLE1;
		 * As we don't have a <dtype_null, SQL_NULL> datatype pairing,
		 * we don't know how to map this NULL to a host-language
		 * datatype.  Therefore we now describe it as a 
		 * CHAR(1) CHARACTER SET NONE type.
		 * No value will ever be sent back, as the value of the select
		 * will be NULL - this is only for purposes of DESCRIBING
		 * the statement.  Note that this mapping could be done in dsql.c
		 * as part of the DESCRIBE statement - but I suspect other areas
		 * of the code would break if this is declared dtype_null.
		 */
		desc->dsc_dtype = dtype_text;
		desc->dsc_length = 1;
		desc->dsc_scale = 0;
		desc->dsc_ttype = 0;
		desc->dsc_flags = DSC_nullable;
		return;

	case nod_via:
		MAKE_desc(desc, node->nod_arg[e_via_value_1]);
	/**
	    Set the descriptor flag as nullable. The
	    select expression may or may not return 
	    this row based on the WHERE clause. Setting this
	    flag warns the client to expect null values.
	    (bug 10379)
	**/
		desc->dsc_flags |= DSC_nullable;
		return;

	default:
		ASSERT_FAIL;			/* unexpected dsql_nod type */

	case nod_dom_value:		/* computed value not used */
		/* By the time we get here, any nod_dom_value node should have had
		   * its descriptor set to the type of the domain being created, or
		   * to the type of the existing domain to which a CHECK constraint
		   * is being added.
		 */
		assert(node->nod_desc.dsc_dtype != dtype_null);
		if (desc != &node->nod_desc)
			*desc = node->nod_desc;
		return;
	}
}


/**
  
 	MAKE_desc_from_field
  
    @brief	Compute a DSC from a field's description information.
 

    @param desc
    @param field

 **/
void MAKE_desc_from_field( DSC * desc, DSQL_FLD field)
{

	DEV_BLKCHK(field, dsql_type_fld);

	desc->dsc_dtype = static_cast<UCHAR>(field->fld_dtype);
	desc->dsc_scale = static_cast<SCHAR>(field->fld_scale);
	desc->dsc_sub_type = field->fld_sub_type;
	desc->dsc_length = field->fld_length;
	desc->dsc_flags = (field->fld_flags & FLD_nullable) ? DSC_nullable : 0;
	if (desc->dsc_dtype <= dtype_any_text) {
		INTL_ASSIGN_DSC(desc, field->fld_character_set_id,
						field->fld_collation_id);
	}
	else if (desc->dsc_dtype == dtype_blob)
		desc->dsc_scale = static_cast<SCHAR>(field->fld_character_set_id);
}


/**
  
 	MAKE_desc_from_list
  
    @brief	Make a descriptor from a list of values 
    according to the sql-standard.
 

    @param desc
    @param node
	@param expression_name

 **/
void MAKE_desc_from_list(DSC * desc, DSQL_NOD node, const TEXT* expression_name)
{
	//-------------------------------------------------------------------------- 
	//  [Arno Brinkman] 2003-08-23
	//  
	//  This function is made to determine a output descriptor from a give list
	//  of expressions according to the latest SQL-standard that was available.
	//  (ISO/ANSI SQL:200n WG3:DRS-013 H2-2002-358 August, 2002) 
	//  
	//  If any datatype has a character type then :
	//  - the output will always be a character type except unconvertable types.
	//    (dtype_text, dtype_cstring, dtype_varying, dtype_blob sub_type TEXT)
	//  !!  Currently engine cannot convert string to BLOB therefor BLOB isn't allowed. !!
	//  - first character-set and collation are used as output descriptor.
	//  - if all types have datatype CHAR then output should be CHAR else 
	//    VARCHAR and with the maximum length used from the given list.
	//  
	//  If all of the datatypes are EXACT numeric then the output descriptor 
	//  shall be EXACT numeric with the maximum scale and the maximum precision 
	//  used. (dtype_byte, dtype_short, dtype_long, dtype_int64)
	//  
	//  If any of the datatypes is APPROXIMATE numeric then each datatype in the
	//  list shall be numeric else a error is thrown and the output descriptor 
	//  shall be APPROXIMATE numeric. (dtype_real, dtype_double, dtype_d_float)
	//  
	//  If any of the datatypes is a datetime type then each datatype in the
	//  list shall be the same datetime type else a error is thrown.
	//  numeric. (dtype_sql_date, dtype_sql_time, dtype_timestamp)
	//  
	//  If any of the datatypes is a BLOB datatype then :
	//  - all types should be a BLOB else throw error.
	//  - all types should have the same sub_type else throw error.
	//  - when TEXT type then use first character-set and collation as output
	//    descriptor.
	//  (dtype_blob)
	//  
	//--------------------------------------------------------------------------

    // Initialize values.
	UCHAR max_dtype = 0;
	SCHAR max_scale = 0;
	USHORT max_length = 0, max_dtype_length = 0, maxtextlength = 0;
	SSHORT max_sub_type = 0, first_sub_type, ttype = ttype_ascii; // default type if all nodes are nod_null.
	bool firstarg = true, all_same_sub_type = true;
	bool all_numeric = true, any_numeric = false, any_approx = false;
	bool all_text = true, any_text = false, any_varying = false;
	bool all_date = true, all_time = true, all_timestamp = true, any_datetime = false;
	bool all_blob = true, any_blob = false, any_text_blob = false;

	// Walk through arguments list.
	DSQL_NOD *arg, *end, tnod, err_node = NULL;
	DSC	desc1;
	arg = node->nod_arg;
	for (end = arg + node->nod_count; arg < end; arg++) {
		// ignore NULL and parameter value from walking
		tnod = *arg;
		if (tnod->nod_type == nod_null || tnod->nod_type == nod_parameter) {
			continue;
		}

		// Get the descriptor from current node.
		MAKE_desc(&desc1, *arg);

		// Check if we support this datatype.
		if (!(DTYPE_IS_TEXT(desc1.dsc_dtype) || DTYPE_IS_NUMERIC(desc1.dsc_dtype) ||
				DTYPE_IS_DATE(desc1.dsc_dtype) || DTYPE_IS_BLOB(desc1.dsc_dtype)))
		{
			// ERROR !!!!
			// Unknown datetype 
			ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 804,
				gds_arg_gds, gds_dsql_datatype_err, 0);
		}

		// Initialize some values if this is the first time.
		if (firstarg) {
			max_scale = desc1.dsc_scale;
			max_length = max_dtype_length = desc1.dsc_length;
			max_sub_type = first_sub_type = desc1.dsc_sub_type;
			max_dtype = desc1.dsc_dtype;
			firstarg = false;
		}

		// numeric datatypes :
		if (DTYPE_IS_NUMERIC(desc1.dsc_dtype)) {
			any_numeric = true;
			// Is there any approximate numeric?
			if (DTYPE_IS_APPROX(desc1.dsc_dtype)) {
				any_approx = true;
			}
		}
		else {
			all_numeric = false;
		}

		// Get the max scale and length (precision)
		// scale is negative so check less than < !
		if (desc1.dsc_scale < max_scale) {
			max_scale = desc1.dsc_scale;
		}
		if (desc1.dsc_length > max_length) {
			max_length = desc1.dsc_length;
		}
		// Get max dtype and sub_type
		if (desc1.dsc_dtype > max_dtype) {
			max_dtype = desc1.dsc_dtype;
			max_dtype_length = desc1.dsc_length;
		}
		if (desc1.dsc_sub_type > max_sub_type) {
			max_sub_type = desc1.dsc_sub_type;
		}
		if (desc1.dsc_sub_type != first_sub_type) {
			all_same_sub_type = false;
		}

		// Is this a text type?
		if (DTYPE_IS_TEXT(desc1.dsc_dtype))// ||
				//((desc1.dsc_dtype == dtype_blob) && 
					//(desc1.dsc_sub_type == 1))) // SUB_TYPE 1 = TEXT				
				//
				// AB: We should support TEXT BLOB in the near future, but
				// currently we can't
				//
		{ 
			DSC *ptr;
			ptr = &desc1;
			USHORT cnvlength = TEXT_LEN(ptr);
			if (cnvlength > maxtextlength) {
				maxtextlength = cnvlength;
			}
			if (desc1.dsc_dtype == dtype_varying) {
				any_varying = true;
			}

			// Pick first characterset-collate from args-list  
			// 
			// Is there an better way to determine the         
			// characterset / collate from the list ?          
			// Maybe first according SQL-standard which has an order 
			// UTF32 -> UTF16 -> UTF8 then by a Firebird specified order
			//
			// At least give any first charset other then ASCII/NONE precedence
			if (!any_text) {
				ttype = desc1.dsc_ttype;
			}
			else {
				if ((ttype == ttype_none) || (ttype == ttype_ascii)) {
					ttype = desc1.dsc_ttype;
				}
			}
			any_text = true;
		}
		else {
			// Get max needed-length for not text types suchs as int64,timestamp etc..
			USHORT cnvlength = DSC_convert_to_text_length(desc1.dsc_dtype);
			if (cnvlength > maxtextlength) {
				maxtextlength = cnvlength;
			}
			all_text = false;
		}

		if (DTYPE_IS_DATE(desc1.dsc_dtype)) {
			any_datetime = true;
			if (desc1.dsc_dtype == dtype_sql_date) {
				all_time = false;
				all_timestamp = false;
			} 
			else if (desc1.dsc_dtype == dtype_sql_time) { 
				all_date = false;
				all_timestamp = false;
			}
			else if (desc1.dsc_dtype == dtype_timestamp) {
				all_date = false;
				all_time = false;
			}
		}
		else {
			all_date = false;
			all_time = false;
			all_timestamp = false;
		}

		if (desc1.dsc_dtype == dtype_blob) {
			// When there was already another datatype raise immediately exception
			if (!all_blob || !all_same_sub_type) {
				err_node = tnod;
				break;
			}

			any_blob = true;
			if (desc1.dsc_sub_type == 1) {
				// TEXT BLOB
				if (!any_text_blob) {
					// Save first characterset and collation
					ttype = desc1.dsc_scale;
				}
				any_text_blob = true;
			}
		}
		else {
			all_blob = false;
		}
	}

	// If we haven't had a type at all then all values are NULL and/or parameter nodes.
	if (firstarg) {
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 804,
			gds_arg_gds, gds_dsql_datatype_err, 0);
		// Dynamic SQL Error SQL error code = -804 Data type unknown
	}

	if (err_node) {
		//TEXT error_info[45];
		//sprintf(error_info, "%s at line %d, column %d.", expression_name,
		//	(int) err_node->nod_line, (int) err_node->nod_column);
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
			gds_arg_gds, gds_dsql_datatypes_not_comparable,
			gds_arg_string, "",
			gds_arg_string, expression_name, 0);
		// "Datatypes %sare not comparable in expression %s"
	}

	desc->dsc_flags = DSC_nullable;
	// If all of the arguments are from type text use a text type.
	// Firebird behaves a little bit different than standard here, because
	// any datatype (except BLOB) can be converted to a character-type we
	// allow to use numeric and datetime types together with a 
	// character-type, but output will always be varying !
	if (all_text || (any_text && (any_numeric || any_datetime))) {
		if (any_varying || (any_text && (any_numeric || any_datetime))) {
			desc->dsc_dtype = dtype_varying;
			maxtextlength += sizeof(USHORT);
		}
		else {
			desc->dsc_dtype = dtype_text;
		}
		desc->dsc_ttype = ttype;  // same as dsc_subtype
		desc->dsc_length = maxtextlength;
		desc->dsc_scale = 0;
		return;
	}
	else if (all_numeric) {
		// If all of the arguments are a numeric datatype.
		if (any_approx) {
			if (max_length <= type_lengths[dtype_real]) {
				desc->dsc_dtype = dtype_real;
				desc->dsc_length = type_lengths[desc->dsc_dtype];
			}
			else {
				desc->dsc_dtype = dtype_double;
				desc->dsc_length = type_lengths[desc->dsc_dtype];
			}
			desc->dsc_scale = 0;
			desc->dsc_sub_type = 0;
		}
		else {
			desc->dsc_dtype = max_dtype;
			desc->dsc_length = max_dtype_length;
			desc->dsc_sub_type = max_sub_type;
			desc->dsc_scale = max_scale;
		}
		return;
	}
	else if (all_date || all_time || all_timestamp) {
		// If all of the arguments are the same datetime datattype.
		desc->dsc_dtype  = max_dtype;
		desc->dsc_length = max_dtype_length;
		desc->dsc_scale = 0;
		desc->dsc_sub_type = 0;
		return;
	}
	else if (all_blob && all_same_sub_type) {
		// If all of the arguments are the same BLOB datattype.
		desc->dsc_dtype  = max_dtype;
		desc->dsc_sub_type = max_sub_type;
		if (max_sub_type == 1) {
			// TEXT BLOB
			desc->dsc_scale = ttype;
		}
		else {
			desc->dsc_scale = max_scale;
		}
		desc->dsc_length = max_length;
		return;
	}
	else {
		// We couldn't do anything with this list, mostly because the
		// datatypes aren't comparable.
		// Let's try to give a usefull error message.
		ERRD_post(gds_sqlerr, gds_arg_number, (SLONG) - 104,
			gds_arg_gds, gds_dsql_datatypes_not_comparable,
			gds_arg_string, "",
			gds_arg_string, expression_name, 0);
		// "Datatypes %sare not comparable in expression %s"
	}
}


/**
  
 	MAKE_field
  
    @brief	Make up a field node.
 

    @param context
    @param field
    @param indices

 **/
DSQL_NOD MAKE_field(DSQL_CTX context, DSQL_FLD field, DSQL_NOD indices)
{
	DSQL_NOD node;

	DEV_BLKCHK(context, dsql_type_ctx);
	DEV_BLKCHK(field, dsql_type_fld);
	DEV_BLKCHK(indices, dsql_type_nod);

	node = MAKE_node(nod_field, e_fld_count);
	node->nod_arg[e_fld_context] = (DSQL_NOD) context;
	node->nod_arg[e_fld_field] = (DSQL_NOD) field;
	if (field->fld_dimensions) {
		if (indices) {
			node->nod_arg[e_fld_indices] = indices;
			MAKE_desc_from_field(&node->nod_desc, field);
			node->nod_desc.dsc_dtype =
				static_cast<UCHAR>(field->fld_element_dtype);
			node->nod_desc.dsc_length = field->fld_element_length;
			/*
			   node->nod_desc.dsc_scale = field->fld_scale;
			   node->nod_desc.dsc_sub_type = field->fld_sub_type;
			 */
		}
		else {
			node->nod_desc.dsc_dtype = dtype_array;
			node->nod_desc.dsc_length = sizeof(GDS__QUAD);
			node->nod_desc.dsc_scale =
				static_cast<SCHAR>(field->fld_scale);
			node->nod_desc.dsc_sub_type = field->fld_sub_type;
		}
	}
	else {
		assert(!indices);
		MAKE_desc_from_field(&node->nod_desc, field);
	}

	if ((field->fld_flags & FLD_nullable) ||
		(context->ctx_flags & CTX_outer_join)) {
		node->nod_desc.dsc_flags = DSC_nullable;
	}

	return node;
}


/**
  
 	MAKE_list
  
    @brief	Make a list node from a linked list stack of things.
 

    @param stack

 **/
DSQL_NOD MAKE_list(DLLS stack)
{
	DLLS temp;
	USHORT count;
	DSQL_NOD node, *ptr;

	DEV_BLKCHK(stack, dsql_type_lls);

	for (temp = stack, count = 0; temp; temp = temp->lls_next)
		++count;

	node = MAKE_node(nod_list, count);
	ptr = node->nod_arg + count;

	while (stack)
		*--ptr = (DSQL_NOD) LLS_POP(&stack);

	return node;
}


/**
  
 	MAKE_node
  
    @brief	Make a node of given type.
 

    @param type
    @param count

 **/
DSQL_NOD MAKE_node(NOD_TYPE type, int count)
{
	DSQL_NOD node;
	TSQL tdsql;

	tdsql = GET_THREAD_DATA;

	node = FB_NEW_RPT(*tdsql->tsql_default, count) dsql_nod;
	node->nod_type = type;
	node->nod_count = count;

	return node;
}


/**
  
 	MAKE_parameter
  
    @brief	Generate a parameter block for a message.  If requested,
 	set up for a null flag as well.
 

    @param message
    @param sqlda_flag
    @param null_flag
    @param sqlda_index

 **/
PAR MAKE_parameter(DSQL_MSG message, USHORT sqlda_flag, USHORT null_flag, USHORT sqlda_index)
{
	PAR parameter, null;
	TSQL tdsql;

	DEV_BLKCHK(message, dsql_type_msg);
	
	if (sqlda_flag && sqlda_index && (sqlda_index <= message->msg_index) && 
		!Config::getOldParameterOrdering()) 
	{
		// This parameter possibly already here. Look for it
		for (PAR temp = message->msg_parameters; temp; temp = temp->par_next) {
			if (temp->par_index == sqlda_index)
				return temp;
		}
	}

	tdsql = GET_THREAD_DATA;

	parameter = FB_NEW(*tdsql->tsql_default) par;
	parameter->par_message = message;
    parameter->par_next = message->msg_parameters;
	if (parameter->par_next != 0)
		parameter->par_next->par_ordered = parameter;
	else
		message->msg_par_ordered = parameter;
	message->msg_parameters = parameter;
	parameter->par_parameter = message->msg_parameter++;
	parameter->par_rel_name = NULL;
	parameter->par_owner_name = NULL;

/* If the parameter is used declared, set SQLDA index */
	if (sqlda_flag) {
		if (sqlda_index && !Config::getOldParameterOrdering()) {
			parameter->par_index = sqlda_index;
			if (message->msg_index < sqlda_index) message->msg_index = sqlda_index;
		} else {
			parameter->par_index = ++message->msg_index;
		}
	}
		
/* If a null handing has been requested, set up a null flag */

	if (null_flag) {
		parameter->par_null = null = MAKE_parameter(message, FALSE, FALSE, 0);
		null->par_desc.dsc_dtype = dtype_short;
		null->par_desc.dsc_scale = 0;
		null->par_desc.dsc_length = sizeof(SSHORT);
	}

	return parameter;
}

/**
  
 	MAKE_string
  
    @brief	Generalized routine for making a string block.
 

    @param str
    @param length

 **/
STR MAKE_string(const char* str, int length)
{

	return MAKE_tagged_string(str, length, NULL);
}


/**
  
 	MAKE_symbol
  
    @brief	Make a symbol for an object and insert symbol into hash table.
 

    @param database
    @param name
    @param length
    @param type
    @param object

 **/
SYM MAKE_symbol(DBB database,
				const TEXT * name, USHORT length, SYM_TYPE type, DSQL_REQ object)
{
	SYM symbol;
	TEXT *p;
	TSQL tdsql;

	DEV_BLKCHK(database, dsql_type_dbb);
	DEV_BLKCHK(object, dsql_type_req);
	assert(name);
	assert(length > 0);

	tdsql = GET_THREAD_DATA;

	symbol = FB_NEW_RPT(*tdsql->tsql_default, length) sym;
	symbol->sym_type = type;
	symbol->sym_object = (BLK) object;
	symbol->sym_dbb = database;
	symbol->sym_length = length;
	symbol->sym_string = p = symbol->sym_name;

	if (length)
		MOVE_FAST(name, p, length);

	HSHD_insert(symbol);

	return symbol;
}


/**
  
 	MAKE_tagged_string
  
    @brief	Generalized routine for making a string block.
 	Which is tagged with a character set descriptor.
 

    @param str_
    @param length
    @param charset

 **/
STR MAKE_tagged_string(const char* str_, size_t length, const char* charset)
{


	TSQL tdsql = GET_THREAD_DATA;

	STR string = FB_NEW_RPT(*tdsql->tsql_default, length) str;
	string->str_charset = charset;
	string->str_length  = length;
	memcpy(string->str_data, str_, length);

	return string;
}


/**
  
 	MAKE_trigger_type
  
    @brief	Make a trigger type
 

    @param prefix_node
    @param suffix_node

 **/
DSQL_NOD MAKE_trigger_type(DSQL_NOD prefix_node, DSQL_NOD suffix_node)
{

	long prefix = (long) prefix_node->nod_arg[0];
	long suffix = (long) suffix_node->nod_arg[0];
	delete prefix_node;
	delete suffix_node;
	return MAKE_constant((STR) (prefix + suffix - 1), CONSTANT_SLONG);
}


/**
  
 	MAKE_variable
  
    @brief	Make up a field node.
 

    @param field
    @param name
    @param type
    @param msg_number
    @param item_number
    @param local_number

 **/
DSQL_NOD MAKE_variable(DSQL_FLD field,
				  const TEXT * name,
				  USHORT type,
				  USHORT msg_number, USHORT item_number, USHORT local_number)
{
	DSQL_NOD node;
	VAR var_;
	TSQL tdsql;

	DEV_BLKCHK(field, dsql_type_fld);

	tdsql = GET_THREAD_DATA;

	var_ = FB_NEW_RPT(*tdsql->tsql_default, strlen(name)) var;
	node = MAKE_node(nod_variable, e_var_count);
	node->nod_arg[e_var_variable] = (DSQL_NOD) var_;
	var_->var_msg_number = msg_number;
	var_->var_msg_item = item_number;
	var_->var_variable_number = local_number;
	var_->var_field = field;
	strcpy(var_->var_name, name);
	var_->var_flags = type;
	MAKE_desc_from_field(&node->nod_desc, field);

	return node;
}

