/*
 *	PROGRAM:	
 *	MODULE:		DataTypeUtil.cpp
 *	DESCRIPTION:	Data Type Utility functions
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2006 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/common.h"
#include "../jrd/DataTypeUtil.h"
#include "../jrd/SysFunction.h"
#include "../jrd/align.h"
#include "../jrd/dsc.h"
#include "../jrd/ibase.h"
#include "../jrd/intl.h"
#include "../jrd/dsc_proto.h"
#include "../jrd/intl_proto.h"
#include "../jrd/gdsassert.h"

using namespace Firebird;


SSHORT DataTypeUtilBase::getResultBlobSubType(const dsc* value1, const dsc* value2)
{	
	SSHORT subType1 = value1->getBlobSubType();
	SSHORT subType2 = value2->getBlobSubType();

	if (subType2 == isc_blob_untyped)	// binary
		return subType2;

	return subType1;
}


USHORT DataTypeUtilBase::getResultTextType(const dsc* value1, const dsc* value2)
{
	USHORT cs1 = value1->getCharSet();
	USHORT cs2 = value2->getCharSet();

	USHORT ttype1 = value1->getTextType();
	USHORT ttype2 = value2->getTextType();

	if (cs1 == CS_NONE || cs2 == CS_BINARY)
		return ttype2;

	if (cs1 == CS_ASCII && cs2 != CS_NONE)
		return ttype2;

	return ttype1;
}


void DataTypeUtilBase::makeFromList(dsc* result, const char* expressionName, int argsCount, const dsc** args)
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
	bool nullable = false;
	bool err = false;

	// Walk through arguments list.
	for (const dsc** p = args; p < args + argsCount; ++p)
	{
		const dsc* arg = *p;

		// do we have only literal NULLs?
		if (!arg->isNull())
			all_nulls = false;

		// ignore NULL and parameter value from walking
		if (arg->isNull() || arg->isUnknown())
		{
			nullable = true;
			continue;
		}

		// Get the descriptor from current node.
		if (!nullable)
			nullable = arg->isNullable();

		// Check if we support this datatype.
		if (!(DTYPE_IS_TEXT(arg->dsc_dtype) || DTYPE_IS_NUMERIC(arg->dsc_dtype) ||
				DTYPE_IS_DATE(arg->dsc_dtype) || DTYPE_IS_BLOB(arg->dsc_dtype)))
		{
			// ERROR !!!!
			// Unknown datatype
			status_exception::raise(
				isc_sqlerr, isc_arg_number, (SLONG) - 804,
				isc_arg_gds, isc_dsql_datatype_err, isc_arg_end);
		}

		// Initialize some values if this is the first time.
		if (firstarg) {
			max_scale = arg->dsc_scale;
			max_length = max_dtype_length = arg->dsc_length;
			max_sub_type = first_sub_type = arg->dsc_sub_type;
			max_dtype = arg->dsc_dtype;
			firstarg = false;
		}
		else {
			if (all_equal) {
				all_equal = 
					(max_dtype == arg->dsc_dtype) &&
					(max_scale == arg->dsc_scale) && 
					(max_length == arg->dsc_length) && 
					(max_sub_type == arg->dsc_sub_type);
			}
		}

		// numeric datatypes :
		if (DTYPE_IS_NUMERIC(arg->dsc_dtype)) {
			any_numeric = true;
			// Is there any approximate numeric?
			if (DTYPE_IS_APPROX(arg->dsc_dtype)) {
				any_approx = true;
				// Dialect 1 NUMERIC and DECIMAL are stored as sub-types 
				// 1 and 2 from float types dtype_real, dtype_double
				if (!any_float) {
					any_float = (arg->dsc_sub_type == 0);
				}
			}
			//
			if (arg->dsc_sub_type > max_numeric_sub_type) {
				max_numeric_sub_type = arg->dsc_sub_type;
			}
		}
		else {
			all_numeric = false;
		}

		// Get the max scale and length (precision)
		// scale is negative so check less than < !
		if (arg->dsc_scale < max_scale) {
			max_scale = arg->dsc_scale;
		}
		if (arg->dsc_length > max_length) {
			max_length = arg->dsc_length;
		}
		// Get max significant bits
		if (type_significant_bits[arg->dsc_dtype] > max_significant_digits) {
			max_significant_digits = type_significant_bits[arg->dsc_dtype];
		}
		// Get max dtype and sub_type
		if (arg->dsc_dtype > max_dtype) {
			max_dtype = arg->dsc_dtype;
			max_dtype_length = arg->dsc_length;
		}
		if (arg->dsc_sub_type > max_sub_type) {
			max_sub_type = arg->dsc_sub_type;
		}
		if (arg->dsc_sub_type != first_sub_type) {
			all_same_sub_type = false;
		}

		// Is this a text type?
		if (DTYPE_IS_TEXT(arg->dsc_dtype) ||
			(arg->dsc_dtype == dtype_blob && arg->dsc_sub_type == isc_blob_text))
		{ 
			const USHORT cnvlength = TEXT_LEN(arg);
			if (cnvlength > maxtextlength) {
				maxtextlength = cnvlength;
			}
			if (arg->dsc_dtype == dtype_varying) {
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
				ttype = arg->getTextType();
			}
			else {
				if ((ttype == ttype_none) || (ttype == ttype_ascii)) {
					ttype = arg->getTextType();
				}
			}
			any_text = true;
		}
		else {
			// Get max needed-length for not text types suchs as int64,timestamp etc..
			const USHORT cnvlength = DSC_convert_to_text_length(arg->dsc_dtype);
			if (cnvlength > maxtextlength) {
				maxtextlength = cnvlength;
			}
			all_text = false;
		}

		if (DTYPE_IS_DATE(arg->dsc_dtype)) {
			any_datetime = true;
			if (arg->dsc_dtype == dtype_sql_date) {
				all_time = false;
				all_timestamp = false;
			} 
			else if (arg->dsc_dtype == dtype_sql_time) { 
				all_date = false;
				all_timestamp = false;
			}
			else if (arg->dsc_dtype == dtype_timestamp) {
				all_date = false;
				all_time = false;
			}
		}
		else {
			all_date = false;
			all_time = false;
			all_timestamp = false;
		}

		if (arg->dsc_dtype == dtype_blob)
		{
			// When there was already another datatype raise immediately exception
			if (!(any_text && arg->dsc_sub_type == isc_blob_text) && (!all_blob || !all_same_sub_type))
			{
				err = true;
				break;
			}

			any_blob = true;
			if (arg->dsc_sub_type == 1) {
				// TEXT BLOB
				if (!any_text_blob) {
					// Save first characterset and collation
					ttype = arg->getTextType();
				}
				any_text_blob = true;
			}
		}
		else {
			all_blob = false;
		}
	}

	// If any type is nullable then the general descriptor is nullable
	if (nullable) {
		result->dsc_flags |= DSC_nullable;
	}
	else {
		result->dsc_flags &= ~DSC_nullable;
	}

	// If we have literal NULLs only, let the result be either
	// CHAR(1) CHARACTER SET NONE
	if (all_nulls)
	{
		result->makeNullString();
		return;
	}

	// If we haven't had a type at all then all values are NULL and/or parameter nodes.
	if (firstarg) {
		status_exception::raise(
			isc_sqlerr, isc_arg_number, (SLONG) - 804,
			isc_arg_gds, isc_dsql_datatype_err, isc_arg_end);
		// Dynamic SQL Error SQL error code = -804 Data type unknown
	}

	if (err) {
		status_exception::raise(
			isc_sqlerr, isc_arg_number, (SLONG) - 104,
			isc_arg_gds, isc_dsql_datatypes_not_comparable,
			isc_arg_string, "",
			isc_arg_string, expressionName, isc_arg_end);
		// "Datatypes %sare not comparable in expression %s"
	}

	// If all the datatypes we've seen are exactly the same, we're done
	if (all_equal) {
		result->dsc_dtype = max_dtype;
		result->dsc_length = max_length;
		result->dsc_scale = max_scale;
		result->dsc_sub_type = max_sub_type;
		return;
	}

	// If all of the arguments are from type text use a text type.
	// Firebird behaves a little bit different than standard here, because
	// any datatype (except BLOB) can be converted to a character-type we
	// allow to use numeric and datetime types together with a 
	// character-type, but output will always be varying !
	if (all_text || (any_text && (any_numeric || any_datetime))) {
		if (any_text_blob)
		{
			result->dsc_dtype = dtype_blob;
			result->dsc_length = sizeof(ISC_QUAD);
			result->setBlobSubType(isc_blob_text);
			result->setTextType(ttype);
		}
		else
		{
			if (any_varying || (any_text && (any_numeric || any_datetime)))
			{
				result->dsc_dtype = dtype_varying;
				maxtextlength += sizeof(USHORT);
			}
			else
				result->dsc_dtype = dtype_text;

			result->dsc_ttype() = ttype;  // same as dsc_subtype
			result->dsc_length = maxtextlength;
			result->dsc_scale = 0;
		}
		return;
	}
	else if (all_numeric) {
		// If all of the arguments are a numeric datatype.
		if (any_approx) {
			if (max_significant_digits <= type_significant_bits[dtype_real]) {
				result->dsc_dtype = dtype_real;
				result->dsc_length = type_lengths[result->dsc_dtype];
			}
			else {
				result->dsc_dtype = dtype_double;
				result->dsc_length = type_lengths[result->dsc_dtype];
			}
			if (any_float) {
				result->dsc_scale = 0;
				result->dsc_sub_type = 0;
			}
			else {
				result->dsc_scale = max_scale;
				result->dsc_sub_type = max_numeric_sub_type;
			}
		}
		else {
			result->dsc_dtype = max_dtype;
			result->dsc_length = max_dtype_length;
			result->dsc_sub_type = max_numeric_sub_type;
			result->dsc_scale = max_scale;
		}
		return;
	}
	else if (all_date || all_time || all_timestamp) {
		// If all of the arguments are the same datetime datattype.
		result->dsc_dtype  = max_dtype;
		result->dsc_length = max_dtype_length;
		result->dsc_scale = 0;
		result->dsc_sub_type = 0;
		return;
	}
	else if (all_blob && all_same_sub_type) {
		// If all of the arguments are the same BLOB datattype.
		result->dsc_dtype  = max_dtype;
		result->dsc_sub_type = max_sub_type;
		if (max_sub_type == isc_blob_text) {
			// TEXT BLOB
			result->dsc_scale = ttype;
			result->dsc_flags |= ttype & 0xFF00;	// collation
		}
		else {
			result->dsc_scale = max_scale;
		}
		result->dsc_length = max_length;
		return;
	}
	else {
		// We couldn't do anything with this list, mostly because the
		// datatypes aren't comparable.
		// Let's try to give a usefull error message.
		status_exception::raise(
			isc_sqlerr, isc_arg_number, (SLONG) - 104,
			isc_arg_gds, isc_dsql_datatypes_not_comparable,
			isc_arg_string, "",
			isc_arg_string, expressionName, isc_arg_end);
		// "Datatypes %sare not comparable in expression %s"
	}
}


ULONG DataTypeUtilBase::convertLength(ULONG len, USHORT srcCharSet, USHORT dstCharSet)
{
	if (dstCharSet == CS_NONE || dstCharSet == CS_BINARY)
		return len;

	return (len / maxBytesPerChar(srcCharSet)) * maxBytesPerChar(dstCharSet);
}


ULONG DataTypeUtilBase::convertLength(const dsc* src, const dsc* dst)
{
	fb_assert(dst->isText());
	return convertLength(src->getStringLength(), src->getCharSet(), dst->getCharSet());
}


ULONG DataTypeUtilBase::fixLength(const dsc* desc, ULONG length)
{
	const UCHAR bpc = maxBytesPerChar(desc->getCharSet());

	USHORT overhead = 0;
	if (desc->dsc_dtype == dtype_varying)
		overhead = sizeof(USHORT);
	else if (desc->dsc_dtype == dtype_cstring)
		overhead = sizeof(UCHAR);

	return MIN(((MAX_COLUMN_SIZE - overhead) / bpc) * bpc, length);
}


void DataTypeUtilBase::makeConcatenate(dsc* result, const dsc* value1, const dsc* value2)
{
	result->clear();

	if (value1->isNull() && value2->isNull())
	{
		result->makeNullString();
		return;
	}

	if (value1->isBlob() || value2->isBlob())
	{
		result->dsc_dtype = dtype_blob;
		result->dsc_length = sizeof(ISC_QUAD);
		result->setBlobSubType(getResultBlobSubType(value1, value2));
		result->setTextType(getResultTextType(value1, value2));
	}
	else
	{
		result->dsc_dtype = dtype_varying;
		result->setTextType(getResultTextType(value1, value2));

		ULONG length = fixLength(result,
			convertLength(value1, result) + convertLength(value2, result));
		result->dsc_length = length + sizeof(USHORT);
	}

	result->dsc_flags = (value1->dsc_flags | value2->dsc_flags) & DSC_nullable;
}


void DataTypeUtilBase::makeSubstr(dsc* result, const dsc* value, const dsc* offset, const dsc* length)
{
	result->clear();

	if (value->isNull())
	{
		result->makeNullString();
		return;
	}

	if (value->isBlob())
	{
		result->dsc_dtype = dtype_blob;
		result->dsc_length = sizeof(ISC_QUAD);
		result->setBlobSubType(value->getBlobSubType());
	}
	else
	{
		// Beware that JRD treats substring() always as returning CHAR
		// instead of VARCHAR for historical reasons.
		result->dsc_dtype = dtype_varying;
	}

	result->setTextType(value->getTextType());
	result->setNullable(value->isNullable() || offset->isNullable() || length->isNullable());

	if (result->isText())
		result->dsc_length = fixLength(result, convertLength(value, result)) + sizeof(USHORT);
}


void DataTypeUtilBase::makeSysFunction(dsc* result, const char* name, int argsCount, const dsc** args)
{
	const SysFunction* function = SysFunction::lookup(name);

	if (function)
	{
		function->checkArgsMismatch(argsCount);
		function->makeFunc(this, function, result, argsCount, args);
	}
}


namespace Jrd {

UCHAR DataTypeUtil::maxBytesPerChar(UCHAR charSet)
{
	return INTL_charset_lookup(tdbb, charSet)->maxBytesPerChar();
}

USHORT DataTypeUtil::getDialect() const
{
	return (tdbb->getDatabase()->dbb_flags & DBB_DB_SQL_dialect_3) ? 3 : 1;
}

}	// namespace Jrd
