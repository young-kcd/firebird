/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		make.cpp
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
#include "../jrd/ibase.h"
#include "../jrd/intl.h"
#include "../jrd/constants.h"
#include "../jrd/align.h"
#include "../dsql/alld_proto.h"
#include "../dsql/errd_proto.h"
#include "../dsql/hsh_proto.h"
#include "../dsql/make_proto.h"
#include "../jrd/thd.h"
#include "../jrd/dsc_proto.h"
#include "../jrd/cvt_proto.h"
#include "../common/config/config.h"


/* InterBase provides transparent conversion from string to date in
 * contexts where it makes sense.  This macro checks a descriptor to
 * see if it is something that *could* represent a date value
 */
static inline bool could_be_date(const dsc& d)
{
	return DTYPE_IS_DATE(d.dsc_dtype) || (d.dsc_dtype <= dtype_any_text);
}


// One of d1, d2 is time, the other is date 
static inline bool is_date_and_time(const dsc& d1, const dsc& d2)
{
	return ((d1.dsc_dtype == dtype_sql_time) && (d2.dsc_dtype == dtype_sql_date)) ||
	((d2.dsc_dtype == dtype_sql_time) && (d1.dsc_dtype == dtype_sql_date));
}

static void make_null(dsc* const desc);
static void make_placeholder_null(dsc* const desc);


/**
  
 	MAKE_constant
  
    @brief	Make a constant node.
 

    @param constant
    @param numeric_flag

 **/
dsql_nod* MAKE_constant(dsql_str* constant, dsql_constant_type numeric_flag)
{
	tsql* tdsql = DSQL_get_thread_data();

	dsql_nod* node = FB_NEW_RPT(*tdsql->getDefaultPool(),
						(numeric_flag == CONSTANT_TIMESTAMP ||
						  numeric_flag == CONSTANT_SINT64) ? 2 : 1) dsql_nod;
	node->nod_type = nod_constant;

	switch (numeric_flag)
	{
	case CONSTANT_SLONG:
		node->nod_desc.dsc_dtype = dtype_long;
		node->nod_desc.dsc_length = sizeof(SLONG);
		node->nod_desc.dsc_scale = 0;
		node->nod_desc.dsc_sub_type = 0;
		node->nod_desc.dsc_address = (UCHAR*) node->nod_arg;
		node->nod_arg[0] = (dsql_nod*) constant;
		break;

	case CONSTANT_DOUBLE:
		DEV_BLKCHK(constant, dsql_type_str);

		/* This is a numeric value which is transported to the engine as
		 * a string.  The engine will convert it. Use dtype_double so that
		 the engine can distinguish it from an actual string.
		 Note: Due to the size of dsc_scale we are limited to numeric
		 constants of less than 256 bytes.  
		 */
		node->nod_desc.dsc_dtype = dtype_double;
		// Scale has no use for double
		node->nod_desc.dsc_scale = static_cast<signed char>(constant->str_length);
		node->nod_desc.dsc_sub_type = 0;
		node->nod_desc.dsc_length = sizeof(double);
		node->nod_desc.dsc_address = (UCHAR*) constant->str_data;
		node->nod_desc.dsc_ttype() = ttype_ascii;
		node->nod_arg[0] = (dsql_nod*) constant;
		break;

	case CONSTANT_SINT64:
		{
			/* We convert the string to an int64.  We treat the two adjacent
			   32-bit words node->nod_arg[0] and node->nod_arg[1] as a
			   64-bit integer: if we ever port to a platform which requires
			   8-byte alignment of int64 data, we will have to force 8-byte
			   alignment of node->nod_arg, which is now only guaranteed
			   4-byte alignment.    -- ChrisJ 1999-02-20 */

			node->nod_desc.dsc_dtype = dtype_int64;
			node->nod_desc.dsc_length = sizeof(SINT64);
			node->nod_desc.dsc_scale = 0;
			node->nod_desc.dsc_sub_type = 0;
			node->nod_desc.dsc_address = (UCHAR*) node->nod_arg;

			/* Now convert the string to an int64.  We can omit testing for
			   overflow, because we would never have gotten here if yylex
			   hadn't recognized the string as a valid 64-bit integer value.
			   We *might* have "9223372936854775808", which works an an int64
			   only if preceded by a '-', but that issue is handled in GEN_expr,
			   and need not be addressed here. */

			UINT64 value = 0;
			const char* p = constant->str_data;

			while (isdigit(*p))
				value = 10 * value + (*(p++) - '0');
			if (*p++ == '.') {
				while (isdigit(*p)) {
					value = 10 * value + (*p++ - '0');
					node->nod_desc.dsc_scale--;
				}
			}

			*(UINT64 *) (node->nod_desc.dsc_address) = value;
			break;
		}

	case CONSTANT_DATE:
	case CONSTANT_TIME:
	case CONSTANT_TIMESTAMP:
		{
			// Setup the constant's descriptor 

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
			}
			node->nod_desc.dsc_sub_type = 0;
			node->nod_desc.dsc_scale = 0;
			node->nod_desc.dsc_length = type_lengths[node->nod_desc.dsc_dtype];
			node->nod_desc.dsc_address = (UCHAR*) node->nod_arg;

			// Set up a descriptor to point to the string 

			dsc tmp;
			tmp.dsc_dtype = dtype_text;
			tmp.dsc_scale = 0;
			tmp.dsc_flags = 0;
			tmp.dsc_ttype() = ttype_ascii;
			tmp.dsc_length = static_cast<USHORT>(constant->str_length);
			tmp.dsc_address = (UCHAR*) constant->str_data;

			// Now invoke the string_to_date/time/timestamp routines 

			CVT_move(&tmp, &node->nod_desc, ERRD_post);
			break;
		}

	default:
		fb_assert(numeric_flag == CONSTANT_STRING);
		DEV_BLKCHK(constant, dsql_type_str);

		node->nod_desc.dsc_dtype = dtype_text;
		node->nod_desc.dsc_sub_type = 0;
		node->nod_desc.dsc_scale = 0;
		node->nod_desc.dsc_length =
			static_cast<USHORT>(constant->str_length);
		node->nod_desc.dsc_address = (UCHAR*) constant->str_data;
		node->nod_desc.dsc_ttype() = ttype_dynamic;
		// carry a pointer to the constant to resolve character set in pass1 
		node->nod_arg[0] = (dsql_nod*) constant;
		break;
	}

	return node;
}


/**
  
 	MAKE_str_constant
  
    @brief	Make a constant node when the 
       character set ID is already known.
 

    @param constant
    @param character_set

 **/
dsql_nod* MAKE_str_constant(dsql_str* constant, SSHORT character_set)
{
	tsql* tdsql = DSQL_get_thread_data();

	dsql_nod* node = FB_NEW_RPT(*tdsql->getDefaultPool(), 1) dsql_nod;
	node->nod_type = nod_constant;

	DEV_BLKCHK(constant, dsql_type_str);

	node->nod_desc.dsc_dtype = dtype_text;
	node->nod_desc.dsc_sub_type = 0;
	node->nod_desc.dsc_scale = 0;
	node->nod_desc.dsc_length = static_cast<USHORT>(constant->str_length);
	node->nod_desc.dsc_address = (UCHAR*) constant->str_data;
	node->nod_desc.dsc_ttype() = character_set;
// carry a pointer to the constant to resolve character set in pass1 
	node->nod_arg[0] = (dsql_nod*) constant;

	return node;
}


/**
  
 	MAKE_cstring
  
    @brief	Make a string node for a string whose
 	length is not known, but is null-terminated.
 

    @param str

 **/
dsql_str* MAKE_cstring(const char* str)
{

	return MAKE_string(str, strlen(str));
}


/**
  
 	MAKE_desc
  
    @brief	Make a descriptor from input node.
	This function can modify node->nod_flags to add NOD_COMP_DIALECT

    @param desc
    @param node
	@param null_replacement

 **/
void MAKE_desc(dsc* desc, dsql_nod* node, dsql_nod* null_replacement)
{
	dsc desc1, desc2;
	USHORT dtype, dtype1, dtype2;
	dsql_map* map;
	dsql_ctx* context;
	dsql_rel* relation;
	dsql_fld* field;

	DEV_BLKCHK(node, dsql_type_nod);

	// If we already know the datatype, don't worry about anything.
	//
	// dimitr: But let's re-evaluate descriptors for expression arguments
	//		   (when a NULL replacement node is provided) to always
	//		   choose the correct resulting datatype. Example:
	//		   NULLIF(NULL, 0) => CHAR(1), but
	//		   1 + NULLIF(NULL, 0) => INT
	//		   This is required because of MAKE_desc() being called
	//		   from custom pass1 handlers for some node types and thus
	//		   causing an incorrect datatype (determined without
	//		   context) to be cached in nod_desc.

	if (node->nod_desc.dsc_dtype && !null_replacement)
	{
		*desc = node->nod_desc;
		return;
	}

	switch (node->nod_type) {
	case nod_constant:
	case nod_variable:
		*desc = node->nod_desc;
		return;

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
		map = (dsql_map*) node->nod_arg[e_map_map];
		MAKE_desc(desc, map->map_node, null_replacement);
		return;

	case nod_agg_min:
	case nod_agg_max:
		MAKE_desc(desc, node->nod_arg[0], null_replacement);
		desc->dsc_flags = DSC_nullable;
		return;

	case nod_agg_average:
		MAKE_desc(desc, node->nod_arg[0], null_replacement);
		desc->dsc_flags = DSC_nullable;
		dtype = desc->dsc_dtype;
		if (!DTYPE_CAN_AVERAGE(dtype))
			ERRD_post(isc_expression_eval_err, 0);
		return;

	case nod_agg_average2:
		MAKE_desc(desc, node->nod_arg[0], null_replacement);
		desc->dsc_flags = DSC_nullable;
		dtype = desc->dsc_dtype;
		if (!DTYPE_CAN_AVERAGE(dtype))
			ERRD_post(isc_expression_eval_err, 0);
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
		MAKE_desc(desc, node->nod_arg[0], null_replacement);
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
		MAKE_desc(desc, node->nod_arg[0], null_replacement);
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
			MAKE_desc(&desc1, node->nod_arg[0], node->nod_arg[1]);
			MAKE_desc(&desc2, node->nod_arg[1], node->nod_arg[0]);

			desc->dsc_scale = 0;
			desc->dsc_dtype = dtype_varying;

			if (node->nod_arg[0]->nod_type == nod_null &&
				node->nod_arg[1]->nod_type == nod_null)
			{
				// NULL || NULL = NULL of VARCHAR(1) CHARACTER SET NONE
				desc->dsc_ttype() = 0;
				desc->dsc_length = sizeof(USHORT) + 1;
				desc->dsc_flags |= DSC_nullable;
				return;
			}

			if (desc1.dsc_dtype <= dtype_any_text)
				desc->dsc_ttype() = desc1.dsc_ttype();
			else
				desc->dsc_ttype() = ttype_ascii;

			ULONG length =
				((node->nod_arg[0]->nod_type == nod_null) ? 0 : DSC_string_length(&desc1)) +
				((node->nod_arg[1]->nod_type == nod_null) ? 0 : DSC_string_length(&desc2));

			if (length > MAX_COLUMN_SIZE - sizeof(USHORT))
			{
				length = MAX_COLUMN_SIZE - sizeof(USHORT);
			}

			desc->dsc_length = length + sizeof(USHORT);
			desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;
		}
		return;

	case nod_derived_field:
		MAKE_desc(desc, node->nod_arg[e_derived_field_value], null_replacement);
		return;

	case nod_upcase:
    case nod_substr:
		MAKE_desc(&desc1, node->nod_arg[0], null_replacement);
		if (desc1.dsc_dtype <= dtype_any_text) {
			*desc = desc1;
			return;
		}
		desc->dsc_dtype = dtype_varying;
		desc->dsc_scale = 0;

		/* Beware that JRD treats substring() always as returning CHAR
		instead	of VARCHAR for historical reasons. */
		if (node->nod_type == nod_substr && desc1.dsc_dtype == dtype_blob)
		{
			dsql_nod* for_node = node->nod_arg[e_substr_length];
			// Migrate the charset from the blob to the string. 
			desc->dsc_ttype() = desc1.dsc_scale;
			// Set maximum possible length
			SLONG length = MAX_COLUMN_SIZE - sizeof(USHORT);
			if (for_node->nod_type == nod_constant &&
				for_node->nod_desc.dsc_dtype == dtype_long)
			{
				// We have a constant passed as length, so
				// use the real length
				length = *(SLONG *) for_node->nod_desc.dsc_address;
				if (length < 0 || length > MAX_COLUMN_SIZE - sizeof(USHORT))
				{
					length = MAX_COLUMN_SIZE - sizeof(USHORT);
				}
			}
			/* For now, our substring() doesn't handle MBCS blobs,
			neither at the DSQL layer nor at the JRD layer. */
			desc->dsc_length = sizeof(USHORT) + length;
		}
		else
		{
			desc->dsc_ttype() = ttype_ascii;
			desc->dsc_length = sizeof(USHORT) + DSC_string_length(&desc1);
		}
		desc->dsc_flags = desc1.dsc_flags & DSC_nullable;
		return;

	case nod_cast:
		field = (dsql_fld*) node->nod_arg[e_cast_target];
		MAKE_desc_from_field(desc, field);
		MAKE_desc(&desc1, node->nod_arg[e_cast_source], NULL);
		desc->dsc_flags = desc1.dsc_flags & DSC_nullable;
		return;

	case nod_simple_case:
		MAKE_desc_from_list(&desc1, node->nod_arg[e_simple_case_results],
							null_replacement, "CASE");
		*desc = desc1;
		return;

	case nod_searched_case:
		MAKE_desc_from_list(&desc1, node->nod_arg[e_searched_case_results],
							null_replacement, "CASE");
		*desc = desc1;
		return;

	case nod_coalesce:
		MAKE_desc_from_list(&desc1, node->nod_arg[0],
							null_replacement, "COALESCE");
		*desc = desc1;
		return;

#ifdef DEV_BUILD
	case nod_collate:
		ERRD_bugcheck("Not expecting nod_collate in dsql/MAKE_desc");
		return;
#endif

	case nod_add:
	case nod_subtract:
		MAKE_desc(&desc1, node->nod_arg[0], node->nod_arg[1]);
		MAKE_desc(&desc2, node->nod_arg[1], node->nod_arg[0]);

		if (node->nod_arg[0]->nod_type == nod_null &&
			node->nod_arg[1]->nod_type == nod_null)
		{
			// NULL + NULL = NULL of INT
			make_null(desc);
			return;
		}

		dtype1 = desc1.dsc_dtype;
		dtype2 = desc2.dsc_dtype;

		if (dtype_int64 == dtype1)
			dtype1 = dtype_double;
		if (dtype_int64 == dtype2)
			dtype2 = dtype_double;

		dtype = MAX(dtype1, dtype2);

		if (DTYPE_IS_BLOB(dtype)) {
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 607,
					  isc_arg_gds, isc_dsql_no_blob_array, 0);
		}

		desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;
		switch (dtype) {
		case dtype_sql_time:
		case dtype_sql_date:
			/* Forbid <date/time> +- <string> */
			if (DTYPE_IS_TEXT(desc1.dsc_dtype) ||
				DTYPE_IS_TEXT(desc2.dsc_dtype))
			{
				ERRD_post(isc_expression_eval_err, 0);
			}

		case dtype_timestamp:

			/* Allow <timestamp> +- <string> (historical) */
			if (could_be_date(desc1) && could_be_date(desc2)) {
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

					if (DTYPE_IS_TEXT(dtype1))
						dtype = dtype_timestamp;
					else if (DTYPE_IS_TEXT(dtype2))
						dtype = dtype_timestamp;
					else if (dtype1 == dtype2)
						dtype = dtype1;
					else if ((dtype1 == dtype_timestamp) &&
							 (dtype2 == dtype_sql_date))
					{
						dtype = dtype_timestamp;
					}
					else if ((dtype2 == dtype_timestamp) &&
							 (dtype1 == dtype_sql_date))
					{
						dtype = dtype_timestamp;
					}
					else {
						ERRD_post(isc_expression_eval_err, 0);
					}

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
						fb_assert(dtype == dtype_timestamp);
						desc->dsc_dtype = dtype_double;
						desc->dsc_length = sizeof(double);
						desc->dsc_scale = 0;
					}
				}
				else if (is_date_and_time(desc1, desc2)) {
					/* <date> + <time> */
					/* <time> + <date> */
					desc->dsc_dtype = dtype_timestamp;
					desc->dsc_length = type_lengths[dtype_timestamp];
					desc->dsc_scale = 0;
				}
				else {
					/* <date> + <date> */
					ERRD_post(isc_expression_eval_err, 0);
				}
			}
			else if (DTYPE_IS_DATE(desc1.dsc_dtype) ||
					 /* <date> +/- <non-date> */
					 (node->nod_type == nod_add))
				/* <non-date> + <date> */
			{
				desc->dsc_dtype = desc1.dsc_dtype;
				if (!DTYPE_IS_DATE(desc->dsc_dtype))
					desc->dsc_dtype = desc2.dsc_dtype;
				fb_assert(DTYPE_IS_DATE(desc->dsc_dtype));
				desc->dsc_length = type_lengths[desc->dsc_dtype];
				desc->dsc_scale = 0;
			}
			else {
				/* <non-date> - <date> */
				fb_assert(node->nod_type == nod_subtract);
				ERRD_post(isc_expression_eval_err, 0);
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
		MAKE_desc(&desc1, node->nod_arg[0], node->nod_arg[1]);
		MAKE_desc(&desc2, node->nod_arg[1], node->nod_arg[0]);

		if (node->nod_arg[0]->nod_type == nod_null &&
			node->nod_arg[1]->nod_type == nod_null)
		{
			// NULL + NULL = NULL of INT
			make_null(desc);
			return;
		}

		dtype1 = desc1.dsc_dtype;
		dtype2 = desc2.dsc_dtype;

		// Arrays and blobs can never partipate in addition/subtraction 
		if (DTYPE_IS_BLOB(desc1.dsc_dtype) || DTYPE_IS_BLOB(desc2.dsc_dtype)) {
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 607,
					  isc_arg_gds, isc_dsql_no_blob_array, 0);
		}

		/* In Dialect 2 or 3, strings can never partipate in addition / sub 
		   (Use a specific cast instead) */
		if (DTYPE_IS_TEXT(desc1.dsc_dtype) ||
			DTYPE_IS_TEXT(desc2.dsc_dtype))
		{
			ERRD_post(isc_expression_eval_err, 0);
		}

		/* Determine the TYPE of arithmetic to perform, store it
		   in dtype.  Note:  this is different from the result of
		   the operation, as <timestamp>-<timestamp> uses
		   <timestamp> arithmetic, but returns a <double> */
		if (DTYPE_IS_EXACT(desc1.dsc_dtype)
			&& DTYPE_IS_EXACT(desc2.dsc_dtype)) dtype = dtype_int64;
		else if (DTYPE_IS_NUMERIC(desc1.dsc_dtype)
				 && DTYPE_IS_NUMERIC(desc2.dsc_dtype))
		{
			fb_assert(DTYPE_IS_APPROX(desc1.dsc_dtype) ||
				   DTYPE_IS_APPROX(desc2.dsc_dtype));
			dtype = dtype_double;
		}
		else {
			// mixed numeric and non-numeric: 

			fb_assert(DTYPE_IS_DATE(dtype1) || DTYPE_IS_DATE(dtype2));

			// The MAX(dtype) rule doesn't apply with dtype_int64 

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

			if ((DTYPE_IS_DATE(dtype1) || (dtype1 == dtype_unknown)) &&
				(DTYPE_IS_DATE(dtype2) || (dtype2 == dtype_unknown)))
			{
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
					{
							dtype = dtype_timestamp;
					}
					else if ((dtype2 == dtype_timestamp) &&
							 (dtype1 == dtype_sql_date))
					{
							dtype = dtype_timestamp;
					}
					else {
						ERRD_post(isc_expression_eval_err, 0);
					}

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
						fb_assert(dtype == dtype_timestamp);
						desc->dsc_dtype = dtype_int64;
						desc->dsc_length = sizeof(SINT64);
						desc->dsc_scale = -9;
					}
				}
				else if (is_date_and_time(desc1, desc2)) {
					/* <date> + <time> */
					/* <time> + <date> */
					desc->dsc_dtype = dtype_timestamp;
					desc->dsc_length = type_lengths[dtype_timestamp];
					desc->dsc_scale = 0;
				}
				else {
					/* <date> + <date> */
					ERRD_post(isc_expression_eval_err, 0);
				}
			}
			else if (DTYPE_IS_DATE(desc1.dsc_dtype) ||
					 /* <date> +/- <non-date> */
					 (node->nod_type == nod_add2))
				/* <non-date> + <date> */
			{
				desc->dsc_dtype = desc1.dsc_dtype;
				if (!DTYPE_IS_DATE(desc->dsc_dtype))
					desc->dsc_dtype = desc2.dsc_dtype;
				fb_assert(DTYPE_IS_DATE(desc->dsc_dtype));
				desc->dsc_length = type_lengths[desc->dsc_dtype];
				desc->dsc_scale = 0;
			}
			else {
				/* <non-date> - <date> */
				fb_assert(node->nod_type == nod_subtract2);
				ERRD_post(isc_expression_eval_err, 0);
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
			fb_assert(DTYPE_IS_EXACT(desc1.dsc_dtype));
			fb_assert(DTYPE_IS_EXACT(desc2.dsc_dtype));

			desc->dsc_scale = MIN(desc1.dsc_scale, desc2.dsc_scale);
			node->nod_flags |= NOD_COMP_DIALECT;
			break;

		default:
			// a type which cannot participate in an add or subtract 
			ERRD_post(isc_expression_eval_err, 0);
		}
		return;

	case nod_multiply:
		MAKE_desc(&desc1, node->nod_arg[0], node->nod_arg[1]);
		MAKE_desc(&desc2, node->nod_arg[1], node->nod_arg[0]);

		if (node->nod_arg[0]->nod_type == nod_null &&
			node->nod_arg[1]->nod_type == nod_null)
		{
			// NULL * NULL = NULL of INT
			make_null(desc);
			return;
		}

		dtype = DSC_multiply_blr4_result[desc1.dsc_dtype][desc2.dsc_dtype];

		if (dtype_unknown == dtype) {
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 607,
					  isc_arg_gds, isc_dsql_no_blob_array, 0);
		}

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
			ERRD_post(isc_expression_eval_err, 0);
		}
		return;

	case nod_multiply2:
		MAKE_desc(&desc1, node->nod_arg[0], node->nod_arg[1]);
		MAKE_desc(&desc2, node->nod_arg[1], node->nod_arg[0]);

		if (node->nod_arg[0]->nod_type == nod_null &&
			node->nod_arg[1]->nod_type == nod_null)
		{
			// NULL * NULL = NULL of INT
			make_null(desc);
			return;
		}

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
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 607,
					  isc_arg_gds, isc_dsql_no_blob_array, 0);
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
		MAKE_desc(&desc1, node->nod_arg[0], node->nod_arg[1]);
		MAKE_desc(&desc2, node->nod_arg[1], node->nod_arg[0]);

		if (node->nod_arg[0]->nod_type == nod_null &&
			node->nod_arg[1]->nod_type == nod_null)
		{
			// NULL / NULL = NULL of INT
			make_null(desc);
			return;
		}

		dtype1 = desc1.dsc_dtype;
		if (dtype_int64 == dtype1)
			dtype1 = dtype_double;

		dtype2 = desc2.dsc_dtype;
		if (dtype_int64 == dtype2)
			dtype2 = dtype_double;

		dtype = MAX(dtype1, dtype2);

		if (!DTYPE_CAN_DIVIDE(dtype)) {
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 607,
					  isc_arg_gds, isc_dsql_no_blob_array, 0);
		}
		desc->dsc_dtype = dtype_double;
		desc->dsc_length = sizeof(double);
		desc->dsc_scale = 0;
		desc->dsc_flags = (desc1.dsc_flags | desc2.dsc_flags) & DSC_nullable;
		return;

	case nod_divide2:
		MAKE_desc(&desc1, node->nod_arg[0], node->nod_arg[1]);
		MAKE_desc(&desc2, node->nod_arg[1], node->nod_arg[0]);

		if (node->nod_arg[0]->nod_type == nod_null &&
			node->nod_arg[1]->nod_type == nod_null)
		{
			// NULL / NULL = NULL of INT
			make_null(desc);
			return;
		}

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
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 607,
					  isc_arg_gds, isc_dsql_no_blob_array, 0);
			break;
		}

		return;

	case nod_negate:
		MAKE_desc(desc, node->nod_arg[0], null_replacement);

		if (node->nod_arg[0]->nod_type == nod_null)
		{
			// -NULL = NULL of INT
			make_null(desc);
			return;
		}

		if (!DTYPE_CAN_NEGATE(desc->dsc_dtype)) {
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 607,
					  isc_arg_gds, isc_dsql_no_blob_array, 0);
		}
		return;

	case nod_alias:
		MAKE_desc(desc, node->nod_arg[e_alias_value], null_replacement);
		return;

	case nod_dbkey:
		// Fix for bug 10072 check that the target is a relation 
		context = (dsql_ctx*) node->nod_arg[0]->nod_arg[0];
		relation = context->ctx_relation;
		if (relation != 0) {
			desc->dsc_dtype = dtype_text;
			desc->dsc_length = relation->rel_dbkey_length;
			desc->dsc_flags = 0;
			desc->dsc_ttype() = ttype_binary;
		}
		else {
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 607,
					  isc_arg_gds, isc_dsql_dbkey_from_non_table, 0);
		}
		return;

	case nod_udf:
		{
			const dsql_udf* userFunc = (dsql_udf*) node->nod_arg[0];
			desc->dsc_dtype = static_cast<UCHAR>(userFunc->udf_dtype);
			desc->dsc_length = userFunc->udf_length;
			desc->dsc_scale = static_cast<SCHAR>(userFunc->udf_scale);
			/* CVC: Setting flags to zero obviously impeded DSQL to acknowledge
			the fact that any UDF can return NULL simply returning a NULL
			pointer. */
			desc->dsc_flags = DSC_nullable;
			
			if (desc->dsc_dtype <= dtype_any_text) {		
				desc->dsc_ttype() = userFunc->udf_character_set_id;
			}
			else {
				desc->dsc_ttype() = userFunc->udf_sub_type;
			}
			return;
		}

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
	case nod_rows:
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
		if (node->nod_desc.dsc_dtype)
		{
			*desc = node->nod_desc;
		}
		else
		{
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 203,
					  isc_arg_gds, isc_dsql_field_ref, 0);
		}
		return;

	case nod_user_name:
	case nod_current_role:
		desc->dsc_dtype = dtype_varying;
		desc->dsc_scale = 0;
		desc->dsc_flags = 0;
		desc->dsc_ttype() = ttype_dynamic;
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
		MAKE_desc(&desc1, node->nod_arg[e_extract_value], NULL);
		desc->dsc_sub_type = 0;
		desc->dsc_scale = 0;
		desc->dsc_flags = (desc1.dsc_flags & DSC_nullable);
		if (*(ULONG *) node->nod_arg[e_extract_part]->nod_desc.dsc_address
			== blr_extract_second)
		{
			// QUADDATE - maybe this should be DECIMAL(6,4) 
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
		if (node->nod_desc.dsc_dtype)
		{
			*desc = node->nod_desc;
		}
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
		 * the statement.  Note that this mapping could be done in dsql.cpp
		 * as part of the DESCRIBE statement - but I suspect other areas
		 * of the code would break if this is declared dtype_unknown.
		 */
		if (null_replacement)
		{
			MAKE_desc(desc, null_replacement, NULL);
			desc->dsc_flags |= DSC_nullable;
		}
		else
		{
			make_placeholder_null(desc);
		}
		return;

	case nod_via:
		MAKE_desc(desc, node->nod_arg[e_via_value_1], null_replacement);
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
		fb_assert(false);			// unexpected dsql_nod type 

	case nod_dom_value:		// computed value not used 
		/* By the time we get here, any nod_dom_value node should have had
		   * its descriptor set to the type of the domain being created, or
		   * to the type of the existing domain to which a CHECK constraint
		   * is being added.
		 */
		fb_assert(node->nod_desc.dsc_dtype != dtype_unknown);
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
void MAKE_desc_from_field(dsc* desc, const dsql_fld* field)
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
void MAKE_desc_from_list(dsc* desc, dsql_nod* node,
						 dsql_nod* null_replacement,
						 const TEXT* expression_name)
{
	//-------------------------------------------------------------------------- 
	//  [Arno Brinkman] 2003-08-23
	//  
	//  This function is made to determine a output descriptor from a given list
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
	USHORT max_length = 0, max_dtype_length = 0, maxtextlength = 0, max_significant_digits = 0;
	SSHORT max_sub_type = 0, first_sub_type, ttype = ttype_ascii; // default type if all nodes are nod_null.
	SSHORT max_numeric_sub_type = 0;
	bool firstarg = true, all_same_sub_type = true, all_equal = true, all_nulls = true;
	bool all_numeric = true, any_numeric = false, any_approx = false, any_float = false;
	bool all_text = true, any_text = false, any_varying = false;
	bool all_date = true, all_time = true, all_timestamp = true, any_datetime = false;
	bool all_blob = true, any_blob = false, any_text_blob = false;

	// Walk through arguments list.
	const dsql_nod* err_node = NULL;
	dsql_nod** arg = node->nod_arg;
	for (dsql_nod** end = arg + node->nod_count; arg < end; arg++) {
		dsql_nod* tnod = *arg;
		// do we have only literal NULLs?
		if (tnod->nod_type != nod_null) {
			all_nulls = false;
		}
		// ignore NULL and parameter value from walking
		if (tnod->nod_type == nod_null || tnod->nod_type == nod_parameter) {
			continue;
		}

		// Get the descriptor from current node.
		dsc desc1;
		MAKE_desc(&desc1, tnod, NULL);

		// Check if we support this datatype.
		if (!(DTYPE_IS_TEXT(desc1.dsc_dtype) || DTYPE_IS_NUMERIC(desc1.dsc_dtype) ||
				DTYPE_IS_DATE(desc1.dsc_dtype) || DTYPE_IS_BLOB(desc1.dsc_dtype)))
		{
			// ERROR !!!!
			// Unknown datatype
			ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 804,
				isc_arg_gds, isc_dsql_datatype_err, 0);
		}

		// Initialize some values if this is the first time.
		if (firstarg) {
			max_scale = desc1.dsc_scale;
			max_length = max_dtype_length = desc1.dsc_length;
			max_sub_type = first_sub_type = desc1.dsc_sub_type;
			max_dtype = desc1.dsc_dtype;
			firstarg = false;
		}
		else {
			if (all_equal) {
				all_equal = 
					(max_dtype == desc1.dsc_dtype) &&
					(max_scale == desc1.dsc_scale) && 
					(max_length == desc1.dsc_length) && 
					(max_sub_type == desc1.dsc_sub_type);
			}
		}

		// numeric datatypes :
		if (DTYPE_IS_NUMERIC(desc1.dsc_dtype)) {
			any_numeric = true;
			// Is there any approximate numeric?
			if (DTYPE_IS_APPROX(desc1.dsc_dtype)) {
				any_approx = true;
				// Dialect 1 NUMERIC and DECIMAL are stored as sub-types 
				// 1 and 2 from float types dtype_real, dtype_double
				if (!any_float) {
					any_float = (desc1.dsc_sub_type == 0);
				}
			}
			//
			if (desc1.dsc_sub_type > max_numeric_sub_type) {
				max_numeric_sub_type = desc1.dsc_sub_type;
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
		// Get max significant bits
		if (type_significant_bits[desc1.dsc_dtype] > max_significant_digits) {
			max_significant_digits = type_significant_bits[desc1.dsc_dtype];
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
			const dsc* ptr = &desc1;
			const USHORT cnvlength = TEXT_LEN(ptr);
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
				ttype = desc1.dsc_ttype();
			}
			else {
				if ((ttype == ttype_none) || (ttype == ttype_ascii)) {
					ttype = desc1.dsc_ttype();
				}
			}
			any_text = true;
		}
		else {
			// Get max needed-length for not text types suchs as int64,timestamp etc..
			const USHORT cnvlength = DSC_convert_to_text_length(desc1.dsc_dtype);
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
			if (desc1.dsc_sub_type = 1) {
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

	// If we have literal NULLs only, let the result be either
	// CHAR(1) CHARACTER SET NONE or the context-provided datatype
	if (all_nulls)
	{
		if (null_replacement)
		{
			MAKE_desc(desc, null_replacement, NULL);
		}
		else
		{
			make_placeholder_null(desc);
		}
		return;
	}

	// If we haven't had a type at all then all values are NULL and/or parameter nodes.
	if (firstarg) {
		ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 804,
			isc_arg_gds, isc_dsql_datatype_err, 0);
		// Dynamic SQL Error SQL error code = -804 Data type unknown
	}

	if (err_node) {
		//TEXT error_info[45];
		//sprintf(error_info, "%s at line %d, column %d.", expression_name,
		//	(int) err_node->nod_line, (int) err_node->nod_column);
		ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 104,
			isc_arg_gds, isc_dsql_datatypes_not_comparable,
			isc_arg_string, "",
			isc_arg_string, expression_name, 0);
		// "Datatypes %sare not comparable in expression %s"
	}

	desc->dsc_flags = DSC_nullable;

	// If all the datatypes we've seen are exactly the same, we're done
	if (all_equal) {
		desc->dsc_dtype = max_dtype;
		desc->dsc_length = max_length;
		desc->dsc_scale = max_scale;
		desc->dsc_sub_type = max_sub_type;
		return;
	}

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
		desc->dsc_ttype() = ttype;  // same as dsc_subtype
		desc->dsc_length = maxtextlength;
		desc->dsc_scale = 0;
		return;
	}
	else if (all_numeric) {
		// If all of the arguments are a numeric datatype.
		if (any_approx) {
			if (max_significant_digits <= type_significant_bits[dtype_real]) {
				desc->dsc_dtype = dtype_real;
				desc->dsc_length = type_lengths[desc->dsc_dtype];
			}
			else {
				desc->dsc_dtype = dtype_double;
				desc->dsc_length = type_lengths[desc->dsc_dtype];
			}
			if (any_float) {
				desc->dsc_scale = 0;
				desc->dsc_sub_type = 0;
			}
			else {
				desc->dsc_scale = max_scale;
				desc->dsc_sub_type = max_numeric_sub_type;
			}
		}
		else {
			desc->dsc_dtype = max_dtype;
			desc->dsc_length = max_dtype_length;
			desc->dsc_sub_type = max_numeric_sub_type;
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
		if (max_sub_type = 1) {
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
		ERRD_post(isc_sqlerr, isc_arg_number, (SLONG) - 104,
			isc_arg_gds, isc_dsql_datatypes_not_comparable,
			isc_arg_string, "",
			isc_arg_string, expression_name, 0);
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
dsql_nod* MAKE_field(dsql_ctx* context, dsql_fld* field, dsql_nod* indices)
{
	DEV_BLKCHK(context, dsql_type_ctx);
	DEV_BLKCHK(field, dsql_type_fld);
	DEV_BLKCHK(indices, dsql_type_nod);

	dsql_nod* node = MAKE_node(nod_field, e_fld_count);
	node->nod_arg[e_fld_context] = (dsql_nod*) context;
	node->nod_arg[e_fld_field] = (dsql_nod*) field;
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
			node->nod_desc.dsc_length = sizeof(ISC_QUAD);
			node->nod_desc.dsc_scale =
				static_cast<SCHAR>(field->fld_scale);
			node->nod_desc.dsc_sub_type = field->fld_sub_type;
		}
	}
	else {
		fb_assert(!indices);
		MAKE_desc_from_field(&node->nod_desc, field);
	}

	if ((field->fld_flags & FLD_nullable) ||
		(context->ctx_flags & CTX_outer_join))
	{
		node->nod_desc.dsc_flags = DSC_nullable;
	}

	return node;
}


/**
  
 	MAKE_list
  
    @brief	Make a list node from a linked list stack of things.
 

    @param stack

 **/
dsql_nod* MAKE_list(DsqlNodStack& stack)
{
	USHORT count = stack.getCount();
	dsql_nod* node = MAKE_node(nod_list, count);
	dsql_nod** ptr = node->nod_arg + count;

	while (stack.hasData())
	{
		*--ptr = stack.pop();
	}

	return node;
}


/**
  
 	MAKE_node
  
    @brief	Make a node of given type.
 

    @param type
    @param count

 **/
dsql_nod* MAKE_node(NOD_TYPE type, int count)
{
	tsql* tdsql = DSQL_get_thread_data();

	dsql_nod* node = FB_NEW_RPT(*tdsql->getDefaultPool(), count) dsql_nod;
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
dsql_par* MAKE_parameter(dsql_msg* message, bool sqlda_flag, bool null_flag,
	USHORT sqlda_index)
{
	DEV_BLKCHK(message, dsql_type_msg);
	
	if (sqlda_flag && sqlda_index && (sqlda_index <= message->msg_index) && 
		!Config::getOldParameterOrdering()) 
	{
		// This parameter possibly already here. Look for it
		for (dsql_par* temp = message->msg_parameters; temp; temp = temp->par_next) {
			if (temp->par_index == sqlda_index)
				return temp;
		}
	}

	tsql* tdsql = DSQL_get_thread_data();

	dsql_par* parameter = FB_NEW(*tdsql->getDefaultPool()) dsql_par;
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
	parameter->par_rel_alias = NULL;

// If the parameter is used declared, set SQLDA index 
	if (sqlda_flag) {
		if (sqlda_index && !Config::getOldParameterOrdering()) {
			parameter->par_index = sqlda_index;
			if (message->msg_index < sqlda_index)
				message->msg_index = sqlda_index;
		}
		else {
			parameter->par_index = ++message->msg_index;
		}
	}
		
// If a null handing has been requested, set up a null flag 

	if (null_flag) {
		dsql_par* null = MAKE_parameter(message, false, false, 0);
		parameter->par_null = null;
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
dsql_str* MAKE_string(const char* str, int length)
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
dsql_sym* MAKE_symbol(dsql_dbb* database,
				const TEXT* name, USHORT length, SYM_TYPE type, dsql_req* object)
{
	DEV_BLKCHK(database, dsql_type_dbb);
	DEV_BLKCHK(object, dsql_type_req);
	fb_assert(name);
	fb_assert(length > 0);

	tsql* tdsql = DSQL_get_thread_data();

	dsql_sym* symbol = FB_NEW_RPT(*tdsql->getDefaultPool(), length) dsql_sym;
	symbol->sym_type = type;
	symbol->sym_object = (BLK) object;
	symbol->sym_dbb = database;
	symbol->sym_length = length;
	TEXT* p = symbol->sym_name;
	symbol->sym_string = p;

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
dsql_str* MAKE_tagged_string(const char* strvar, size_t length, const char* charset)
{
	tsql* tdsql = DSQL_get_thread_data();

	dsql_str* string = FB_NEW_RPT(*tdsql->getDefaultPool(), length) dsql_str;
	string->str_charset = charset;
	string->str_length  = length;
	memcpy(string->str_data, strvar, length);

	return string;
}


/**
  
 	MAKE_trigger_type
  
    @brief	Make a trigger type
 

    @param prefix_node
    @param suffix_node

 **/
dsql_nod* MAKE_trigger_type(dsql_nod* prefix_node, dsql_nod* suffix_node)
{
	const long prefix = (long) prefix_node->nod_arg[0];
	const long suffix = (long) suffix_node->nod_arg[0];
	delete prefix_node;
	delete suffix_node;
	return MAKE_constant((dsql_str*) (prefix + suffix - 1), CONSTANT_SLONG);
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
dsql_nod* MAKE_variable(dsql_fld* field,
				  const TEXT* name,
				  USHORT type,
				  USHORT msg_number, USHORT item_number, USHORT local_number)
{
	DEV_BLKCHK(field, dsql_type_fld);

	tsql* tdsql = DSQL_get_thread_data();

	dsql_var* variable = FB_NEW_RPT(*tdsql->getDefaultPool(), strlen(name)) dsql_var;
	dsql_nod* node = MAKE_node(nod_variable, e_var_count);
	node->nod_arg[e_var_variable] = (dsql_nod*) variable;
	variable->var_msg_number = msg_number;
	variable->var_msg_item = item_number;
	variable->var_variable_number = local_number;
	variable->var_field = field;
	strcpy(variable->var_name, name);
	variable->var_flags = type;
	MAKE_desc_from_field(&node->nod_desc, field);

	return node;
}

/**

	make_null
	
	@brief  Prepare a descriptor to signal SQL NULL
	
	@param desc
	
**/
static void make_null(dsc* const desc)
{
	desc->dsc_dtype = dtype_long;
	desc->dsc_sub_type = 0;
	desc->dsc_length = sizeof(SLONG);
	desc->dsc_scale = 0;
	desc->dsc_flags |= DSC_nullable;
}


/**

	make_placeholder_null

	@brief  Prepare a descriptor to signal SQL NULL with a char(1) when
	    the type is unknown. The descriptor will be a placeholder only.

	@param desc

**/
static void make_placeholder_null(dsc* const desc)
{
	desc->dsc_dtype = dtype_text;
	desc->dsc_length = 1;
	desc->dsc_scale = 0;
	desc->dsc_ttype() = 0;
	desc->dsc_flags = DSC_nullable;
}

