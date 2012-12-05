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
 * Adriano dos Santos Fernandes
 */

#include "firebird.h"
#include <ctype.h>
#include <string.h>
#include "../dsql/dsql.h"
#include "../dsql/Nodes.h"
#include "../dsql/ExprNodes.h"
#include "../jrd/ibase.h"
#include "../jrd/intl.h"
#include "../jrd/constants.h"
#include "../jrd/align.h"
#include "../dsql/errd_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/metd_proto.h"
#include "../dsql/utld_proto.h"
#include "../dsql/DSqlDataTypeUtil.h"
#include "../jrd/DataTypeUtil.h"
#include "../jrd/RecordSourceNodes.h"
#include "../jrd/jrd.h"
#include "../jrd/ods.h"
#include "../jrd/ini.h"
#include "../common/dsc_proto.h"
#include "../common/cvt.h"
#include "../jrd/thread_proto.h"
#include "../yvalve/why_proto.h"
#include "../common/config/config.h"
#include "../common/StatusArg.h"

using namespace Jrd;
using namespace Firebird;


LiteralNode* MAKE_const_slong(SLONG value)
{
	thread_db* tdbb = JRD_get_thread_data();

	SLONG* valuePtr = FB_NEW(*tdbb->getDefaultPool()) SLONG(value);

	LiteralNode* literal = FB_NEW(*tdbb->getDefaultPool()) LiteralNode(*tdbb->getDefaultPool());
	literal->litDesc.dsc_dtype = dtype_long;
	literal->litDesc.dsc_length = sizeof(SLONG);
	literal->litDesc.dsc_scale = 0;
	literal->litDesc.dsc_sub_type = 0;
	literal->litDesc.dsc_address = reinterpret_cast<UCHAR*>(valuePtr);

	return literal;
}


/**

 	MAKE_constant

    @brief	Make a constant node.


    @param constant
    @param numeric_flag

 **/
ValueExprNode* MAKE_constant(dsql_str* constant, dsql_constant_type numeric_flag)
{
	thread_db* tdbb = JRD_get_thread_data();

	LiteralNode* literal = FB_NEW(*tdbb->getDefaultPool()) LiteralNode(*tdbb->getDefaultPool());

	switch (numeric_flag)
	{
	case CONSTANT_DOUBLE:
		DEV_BLKCHK(constant, dsql_type_str);

		// This is a numeric value which is transported to the engine as
		// a string.  The engine will convert it. Use dtype_double so that
		// the engine can distinguish it from an actual string.
		// Note: Due to the size of dsc_scale we are limited to numeric
		// constants of less than 256 bytes.

		literal->litDesc.dsc_dtype = dtype_double;
		// Scale has no use for double
		literal->litDesc.dsc_scale = static_cast<signed char>(constant->str_length);
		literal->litDesc.dsc_sub_type = 0;
		literal->litDesc.dsc_length = sizeof(double);
		literal->litDesc.dsc_address = (UCHAR*) constant->str_data;
		literal->litDesc.dsc_ttype() = ttype_ascii;
		literal->dsqlStr = constant;
		break;

	case CONSTANT_SINT64:
		{
			// We convert the string to an int64.

			literal->litDesc.dsc_dtype = dtype_int64;
			literal->litDesc.dsc_length = sizeof(SINT64);
			literal->litDesc.dsc_scale = 0;
			literal->litDesc.dsc_sub_type = 0;
			literal->litDesc.dsc_address = (UCHAR*) FB_NEW(*tdbb->getDefaultPool()) SINT64;

			// Now convert the string to an int64.  We can omit testing for
			// overflow, because we would never have gotten here if yylex
			// hadn't recognized the string as a valid 64-bit integer value.
			// We *might* have "9223372936854775808", which works an an int64
			// only if preceded by a '-', but that issue is handled in GEN_expr,
			// and need not be addressed here.

			// Recent change to support hex numeric constants means the input
			// string now can be X8000000000000000, for example.
			// Hex constants coming through this code are guaranteed to be
			// valid - they start with X and contains only 0-9, A-F.
			// And, they will fit in a SINT64 without overflow.

			SINT64 value = 0;
			const UCHAR* p = reinterpret_cast<const UCHAR*>(constant->str_data);

			if (*p == 'X')
			{
				// oh no, a hex string!
				++p; // skip the 'X' part.
				UCHAR byte = 0;
				bool nibble = ((strlen(constant->str_data) - 1) & 1);
				SSHORT c;

				// hex string is already upper-cased
				while (isdigit(*p) || ((*p >= 'A') && (*p <= 'F')))
				{
					// Now convert the character to a nibble
					if (*p >= 'A')
						c = (*p - 'A') + 10;
					else
						c = (*p - '0');

					if (nibble)
					{
						byte = (byte << 4) + (UCHAR) c;
						nibble = false;
						value = (value << 8) + byte;
					}
					else
					{
						byte = c;
						nibble = true;
					}

					++p;
				}

				// if value is negative, then GEN_constant (from dsql/gen.cpp)
				// is going to want 2 nodes: NegateNode (to hold the minus)
				// and LiteralNode as a child to hold the value.
				if (value < 0)
				{
					value = -value;
					*(SINT64*) literal->litDesc.dsc_address = value;
					return FB_NEW(*tdbb->getDefaultPool()) NegateNode(*tdbb->getDefaultPool(), literal);
				}
				else
					*(SINT64*) literal->litDesc.dsc_address = value;
			} // hex constant
			else
			{
				// Good old-fashioned base-10 number from the lexer.
				// We convert the string to an int64.

				while (isdigit(*p))
					value = 10 * value + (*(p++) - '0');

				if (*p++ == '.')
				{
					while (isdigit(*p))
					{
						value = 10 * value + (*p++ - '0');
						literal->litDesc.dsc_scale--;
					}
				}

				*(FB_UINT64*) (literal->litDesc.dsc_address) = value;
			}
		}
		break;

	case CONSTANT_DATE:
	case CONSTANT_TIME:
	case CONSTANT_TIMESTAMP:
		{
			// Setup the constant's descriptor

			switch (numeric_flag)
			{
			case CONSTANT_DATE:
				literal->litDesc.dsc_dtype = dtype_sql_date;
				break;
			case CONSTANT_TIME:
				literal->litDesc.dsc_dtype = dtype_sql_time;
				break;
			case CONSTANT_TIMESTAMP:
				literal->litDesc.dsc_dtype = dtype_timestamp;
				break;
			}
			literal->litDesc.dsc_sub_type = 0;
			literal->litDesc.dsc_scale = 0;
			literal->litDesc.dsc_length = type_lengths[literal->litDesc.dsc_dtype];
			literal->litDesc.dsc_address =
				FB_NEW(*tdbb->getDefaultPool()) UCHAR[literal->litDesc.dsc_length];

			// Set up a descriptor to point to the string

			dsc tmp;
			tmp.dsc_dtype = dtype_text;
			tmp.dsc_scale = 0;
			tmp.dsc_flags = 0;
			tmp.dsc_ttype() = ttype_ascii;
			tmp.dsc_length = static_cast<USHORT>(constant->str_length);
			tmp.dsc_address = (UCHAR*) constant->str_data;

			// Now invoke the string_to_date/time/timestamp routines

			CVT_move(&tmp, &literal->litDesc, ERRD_post);
			break;
		}

	case CONSTANT_BOOLEAN:
		DEV_BLKCHK(constant, dsql_type_str);

		literal->litDesc.makeBoolean((UCHAR*) constant->str_data);
		literal->dsqlStr = constant;
		break;

	default:
		fb_assert(numeric_flag == CONSTANT_STRING);
		DEV_BLKCHK(constant, dsql_type_str);

		literal->litDesc.dsc_dtype = dtype_text;
		literal->litDesc.dsc_sub_type = 0;
		literal->litDesc.dsc_scale = 0;
		literal->litDesc.dsc_length = static_cast<USHORT>(constant->str_length);
		literal->litDesc.dsc_address = (UCHAR*) constant->str_data;
		literal->litDesc.dsc_ttype() = ttype_dynamic;
		// carry a pointer to the constant to resolve character set in pass1
		literal->dsqlStr = constant;
		break;
	}

	return literal;
}


/**

 	MAKE_str_constant

    @brief	Make a constant node when the
       character set ID is already known.


    @param constant
    @param character_set

 **/
LiteralNode* MAKE_str_constant(dsql_str* constant, SSHORT character_set)
{
	thread_db* tdbb = JRD_get_thread_data();

	DEV_BLKCHK(constant, dsql_type_str);

	LiteralNode* literal = FB_NEW(*tdbb->getDefaultPool()) LiteralNode(*tdbb->getDefaultPool());
	literal->litDesc.dsc_dtype = dtype_text;
	literal->litDesc.dsc_sub_type = 0;
	literal->litDesc.dsc_scale = 0;
	literal->litDesc.dsc_length = static_cast<USHORT>(constant->str_length);
	literal->litDesc.dsc_address = (UCHAR*) constant->str_data;
	literal->litDesc.dsc_ttype() = character_set;

	literal->dsqlStr = constant;

	return literal;
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


// Make a descriptor from input node.
void MAKE_desc(DsqlCompilerScratch* dsqlScratch, dsc* desc, ValueExprNode* node)
{
	DEV_BLKCHK(node, dsql_type_nod);

	// If we already know the datatype, don't worry about anything.
	if (node->nodDesc.dsc_dtype)
	{
		*desc = node->nodDesc;
		return;
	}

	node->make(dsqlScratch, desc);
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

	desc->clear();
	desc->dsc_dtype = static_cast<UCHAR>(field->dtype);
	desc->dsc_scale = static_cast<SCHAR>(field->scale);
	desc->dsc_sub_type = field->subType;
	desc->dsc_length = field->length;
	desc->dsc_flags = (field->flags & FLD_nullable) ? DSC_nullable : 0;

	if (desc->isText() || desc->isBlob())
	{
		desc->setTextType(INTL_CS_COLL_TO_TTYPE(
			field->charSetId, field->collationId));
	}
}


/**

 	MAKE_desc_from_list

    @brief	Make a descriptor from a list of values
    according to the sql-standard.


    @param desc
    @param node
	@param expression_name

 **/
void MAKE_desc_from_list(DsqlCompilerScratch* dsqlScratch, dsc* desc, ValueListNode* node,
	const TEXT* expression_name)
{
	NestConst<ValueExprNode>* p = node->items.begin();
	NestConst<ValueExprNode>* end = node->items.end();

	Array<const dsc*> args;

	while (p != end)
	{
		MAKE_desc(dsqlScratch, &(*p)->nodDesc, *p);
		args.add(&(*p)->nodDesc);
		++p;
	}

	DSqlDataTypeUtil(dsqlScratch).makeFromList(desc, expression_name, args.getCount(), args.begin());
}


/**

 	MAKE_field

    @brief	Make up a field node.


    @param context
    @param field
    @param indices

 **/
FieldNode* MAKE_field(dsql_ctx* context, dsql_fld* field, ValueListNode* indices)
{
	DEV_BLKCHK(context, dsql_type_ctx);
	DEV_BLKCHK(field, dsql_type_fld);

	thread_db* const tdbb = JRD_get_thread_data();
	FieldNode* const node = FB_NEW(*tdbb->getDefaultPool()) FieldNode(
		*tdbb->getDefaultPool(), context, field, indices);

	if (field->dimensions)
	{
		if (indices)
		{
			MAKE_desc_from_field(&node->nodDesc, field);
			node->nodDesc.dsc_dtype = static_cast<UCHAR>(field->elementDtype);
			node->nodDesc.dsc_length = field->elementLength;

			// node->nodDesc.dsc_scale = field->scale;
			// node->nodDesc.dsc_sub_type = field->subType;
		}
		else
		{
			node->nodDesc.dsc_dtype = dtype_array;
			node->nodDesc.dsc_length = sizeof(ISC_QUAD);
			node->nodDesc.dsc_scale = static_cast<SCHAR>(field->scale);
			node->nodDesc.dsc_sub_type = field->subType;
		}
	}
	else
	{
		if (indices)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-607) <<
					  Arg::Gds(isc_dsql_only_can_subscript_array) << Arg::Str(field->fld_name));
		}

		MAKE_desc_from_field(&node->nodDesc, field);
	}

	if ((field->flags & FLD_nullable) || (context->ctx_flags & CTX_outer_join))
		node->nodDesc.dsc_flags |= DSC_nullable;

	// UNICODE_FSS_HACK
	// check if the field is a system domain and the type is CHAR/VARCHAR CHARACTER SET UNICODE_FSS
	if ((field->flags & FLD_system) && node->nodDesc.dsc_dtype <= dtype_varying &&
		INTL_GET_CHARSET(&node->nodDesc) == CS_METADATA)
	{
		USHORT adjust = 0;

		if (node->nodDesc.dsc_dtype == dtype_varying)
			adjust = sizeof(USHORT);
		else if (node->nodDesc.dsc_dtype == dtype_cstring)
			adjust = 1;

		node->nodDesc.dsc_length -= adjust;
		node->nodDesc.dsc_length *= 3;
		node->nodDesc.dsc_length += adjust;
	}

	return node;
}


/**

 	MAKE_field_name

    @brief	Make up a field name node.


    @param field_name

 **/
FieldNode* MAKE_field_name(const char* field_name)
{
	thread_db* tdbb = JRD_get_thread_data();
	FieldNode* fieldNode = FB_NEW(*tdbb->getDefaultPool()) FieldNode(*tdbb->getDefaultPool());
	fieldNode->dsqlName = field_name;
	return fieldNode;
}


/**

 	MAKE_parameter

    @brief	Generate a parameter block for a message.  If requested,
 	set up for a null flag as well.


    @param message
    @param sqlda_flag
    @param null_flag
    @param sqlda_index
	@param node

 **/
dsql_par* MAKE_parameter(dsql_msg* message, bool sqlda_flag, bool null_flag,
	USHORT sqlda_index, const ValueExprNode* node)
{
	if (!message)
	{
		ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-901) <<
				  Arg::Gds(isc_badmsgnum));
	}

	if (sqlda_flag && sqlda_index && sqlda_index <= message->msg_index)
	{
		// This parameter is possibly already here. Look for it.
		for (size_t i = 0; i < message->msg_parameters.getCount(); ++i)
		{
			dsql_par* temp = message->msg_parameters[i];

			if (temp->par_index == sqlda_index)
				return temp;
		}
	}

	thread_db* tdbb = JRD_get_thread_data();

	dsql_par* parameter = FB_NEW(*tdbb->getDefaultPool()) dsql_par(*tdbb->getDefaultPool());
	parameter->par_message = message;
	message->msg_parameters.insert(0, parameter);
	parameter->par_parameter = message->msg_parameter++;

	parameter->par_rel_name = NULL;
	parameter->par_owner_name = NULL;
	parameter->par_rel_alias = NULL;

	if (node)
		MAKE_parameter_names(parameter, node);

	// If the parameter is used declared, set SQLDA index
	if (sqlda_flag)
	{
		if (sqlda_index)
		{
			parameter->par_index = sqlda_index;
			if (message->msg_index < sqlda_index)
				message->msg_index = sqlda_index;
		}
		else {
			parameter->par_index = ++message->msg_index;
		}
	}

	// If a null handing has been requested, set up a null flag

	if (null_flag)
	{
		dsql_par* null = MAKE_parameter(message, false, false, 0, NULL);
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
	fb_assert(length >= 0);
	return MAKE_tagged_string(str, length, NULL);
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
	thread_db* tdbb = JRD_get_thread_data();

	dsql_str* string = FB_NEW_RPT(*tdbb->getDefaultPool(), length) dsql_str;
	string->str_charset = charset;
	string->str_length  = length;
	memcpy(string->str_data, strvar, length);

	return string;
}


/**

	MAKE_parameter_names

	@brief  Determine relation/column/alias names (if appropriate)
			and store them in the given parameter.

	@param parameter
	@param item

**/
void MAKE_parameter_names(dsql_par* parameter, const ValueExprNode* item)
{
	fb_assert(parameter && item);
	item->setParameterName(parameter);
}
