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
#include "../dsql/node.h"
#include "../dsql/Nodes.h"
#include "../dsql/ExprNodes.h"
#include "../jrd/ibase.h"
#include "../jrd/intl.h"
#include "../jrd/constants.h"
#include "../jrd/align.h"
#include "../dsql/errd_proto.h"
#include "../dsql/hsh_proto.h"
#include "../dsql/make_proto.h"
#include "../dsql/metd_proto.h"
#include "../dsql/utld_proto.h"
#include "../dsql/DSqlDataTypeUtil.h"
#include "../jrd/DataTypeUtil.h"
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
using namespace Dsql;
using namespace Firebird;


static const char* DB_KEY_NAME = "DB_KEY";


dsql_nod* MAKE_const_slong(SLONG value)
{
	thread_db* tdbb = JRD_get_thread_data();

	SLONG* valuePtr = FB_NEW(*tdbb->getDefaultPool()) SLONG(value);

	LiteralNode* literal = FB_NEW(*tdbb->getDefaultPool()) LiteralNode(*tdbb->getDefaultPool());
	literal->litDesc.dsc_dtype = dtype_long;
	literal->litDesc.dsc_length = sizeof(SLONG);
	literal->litDesc.dsc_scale = 0;
	literal->litDesc.dsc_sub_type = 0;
	literal->litDesc.dsc_address = reinterpret_cast<UCHAR*>(valuePtr);

	dsql_nod* node = MAKE_node(nod_class_exprnode, 1);
	node->nod_flags = 0;
	node->nod_arg[0] = reinterpret_cast<dsql_nod*>(literal);

	return node;
}


/**

 	MAKE_constant

    @brief	Make a constant node.


    @param constant
    @param numeric_flag

 **/
dsql_nod* MAKE_constant(dsql_str* constant, dsql_constant_type numeric_flag)
{
	thread_db* tdbb = JRD_get_thread_data();

	LiteralNode* literal = FB_NEW(*tdbb->getDefaultPool()) LiteralNode(*tdbb->getDefaultPool());

	dsql_nod* node = MAKE_node(nod_class_exprnode, 1);
	node->nod_flags = 0;
	node->nod_arg[0] = reinterpret_cast<dsql_nod*>(literal);

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
				*p++; // skip the 'X' part.
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

					*p++;
				}

				// if value is negative, then GEN_constant (from dsql/gen.cpp)
				// is going to want 2 nodes: NegateNode (to hold the minus)
				// and LiteralNode as a child to hold the value.
				if (value < 0)
				{
					value = -value;
					*(SINT64*) literal->litDesc.dsc_address = value;

					NegateNode* negateNode = FB_NEW(*tdbb->getDefaultPool()) NegateNode(
						*tdbb->getDefaultPool(), node);

					node = MAKE_node(nod_class_exprnode, 1);
					node->nod_arg[0] = reinterpret_cast<dsql_nod*>(negateNode);
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

	dsql_nod* node = MAKE_node(nod_class_exprnode, 1);
	node->nod_flags = 0;
	node->nod_arg[0] = reinterpret_cast<dsql_nod*>(literal);

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

 **/
void MAKE_desc(DsqlCompilerScratch* dsqlScratch, dsc* desc, dsql_nod* node)
{
	dsc desc1, desc2, desc3;
	dsql_map* map;
	dsql_ctx* context;
	dsql_rel* relation;

	DEV_BLKCHK(node, dsql_type_nod);

	// If we already know the datatype, don't worry about anything.

	if (node->nod_desc.dsc_dtype)
	{
		*desc = node->nod_desc;
		return;
	}

	switch (node->nod_type)
	{
	case nod_class_exprnode:
		{
			ValueExprNode* exprNode = reinterpret_cast<ValueExprNode*>(node->nod_arg[0]);
			exprNode->make(dsqlScratch, node, desc);
		}
		return;

	case nod_map:
		map = (dsql_map*) node->nod_arg[e_map_map];
		context = (dsql_ctx*) node->nod_arg[e_map_context];
		MAKE_desc(dsqlScratch, desc, map->map_node);

		// ASF: We should mark nod_agg_count as nullable when it's in an outer join - CORE-2660.
		if (context->ctx_flags & CTX_outer_join)
			desc->dsc_flags |= DSC_nullable;

		return;

	case nod_derived_field:
		MAKE_desc(dsqlScratch, desc, node->nod_arg[e_derived_field_value]);
		return;

	case nod_simple_case:
		MAKE_desc_from_list(dsqlScratch, &desc1, node->nod_arg[e_simple_case_results], "CASE");
		*desc = desc1;
		return;

	case nod_searched_case:
		MAKE_desc_from_list(dsqlScratch, &desc1, node->nod_arg[e_searched_case_results], "CASE");
		*desc = desc1;
		return;

	case nod_coalesce:
		MAKE_desc_from_list(dsqlScratch, &desc1, node->nod_arg[0], "COALESCE");
		*desc = desc1;
		return;

#ifdef DEV_BUILD
	case nod_collate:
		ERRD_bugcheck("Not expecting nod_collate in dsql/MAKE_desc");
		return;
#endif

	/*
	case nod_count:
		desc->dsc_dtype = dtype_long;
		desc->dsc_sub_type = 0;
		desc->dsc_scale = 0;
		desc->dsc_length = sizeof(SLONG);
		desc->dsc_flags = 0;
		return;
	*/

	case nod_alias:
		MAKE_desc(dsqlScratch, desc, node->nod_arg[e_alias_value]);
		return;

	case nod_dbkey:
		// Fix for bug 10072 check that the target is a relation
		context = (dsql_ctx*) node->nod_arg[0]->nod_arg[0];
		relation = context->ctx_relation;
		if (relation != 0)
		{
			desc->dsc_dtype = dtype_text;
			if (relation->rel_flags & REL_creating)
				desc->dsc_length = 8;
			else
				desc->dsc_length = relation->rel_dbkey_length;
			desc->dsc_flags = DSC_nullable;
			desc->dsc_ttype() = ttype_binary;
		}
		else
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-607) <<
					  Arg::Gds(isc_dsql_dbkey_from_non_table));
		}
		return;

	case nod_limit:
	case nod_rows:
		if (dsqlScratch->clientDialect <= SQL_DIALECT_V5)
		{
			desc->dsc_dtype = dtype_long;
			desc->dsc_length = sizeof (SLONG);
		}
		else
		{
			desc->dsc_dtype = dtype_int64;
			desc->dsc_length = sizeof (SINT64);
		}
		desc->dsc_sub_type = 0;
		desc->dsc_scale = 0;
		desc->dsc_flags = 0; // Can first/skip accept NULL in the future?
		return;

	case nod_field:
		if (node->nod_desc.dsc_dtype)
		{
			*desc = node->nod_desc;
		}
		else
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-203) <<
					  Arg::Gds(isc_dsql_field_ref));
		}
		return;

	case nod_hidden_var:
		MAKE_desc(dsqlScratch, desc, node->nod_arg[e_hidden_var_expr]);
		return;

	case nod_select_expr:	// this should come from pass1_any call to set_parameter_type
		node = node->nod_arg[e_sel_query_spec];
		fb_assert(node->nod_type == nod_query_spec);
		node = node->nod_arg[e_qry_list];
		fb_assert(node->nod_type == nod_list && node->nod_count > 0);
		MAKE_desc(dsqlScratch, desc, node->nod_arg[0]);
		return;

	default:
		fb_assert(false);			// unexpected dsql_nod type

	case nod_dom_value:		// computed value not used
		// By the time we get here, any nod_dom_value node should have had
		// its descriptor set to the type of the domain being created, or
		// to the type of the existing domain to which a CHECK constraint
		// is being added.

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

	if (desc->isText() || desc->isBlob())
	{
		desc->setTextType(INTL_CS_COLL_TO_TTYPE(
			field->fld_character_set_id, field->fld_collation_id));
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
void MAKE_desc_from_list(DsqlCompilerScratch* dsqlScratch, dsc* desc, dsql_nod* node,
	const TEXT* expression_name)
{
	Array<const dsc*> args;

	fb_assert(node->nod_type == nod_list);

	for (dsql_nod** p = node->nod_arg; p < node->nod_arg + node->nod_count; ++p)
	{
		MAKE_desc(dsqlScratch, &(*p)->nod_desc, *p);
		args.add(&(*p)->nod_desc);
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
dsql_nod* MAKE_field(dsql_ctx* context, dsql_fld* field, dsql_nod* indices)
{
	DEV_BLKCHK(context, dsql_type_ctx);
	DEV_BLKCHK(field, dsql_type_fld);
	DEV_BLKCHK(indices, dsql_type_nod);

	dsql_nod* node = MAKE_node(nod_field, e_fld_count);
	node->nod_arg[e_fld_context] = (dsql_nod*) context;
	node->nod_arg[e_fld_field] = (dsql_nod*) field;
	if (field->fld_dimensions)
	{
		if (indices)
		{
			node->nod_arg[e_fld_indices] = indices;
			MAKE_desc_from_field(&node->nod_desc, field);
			node->nod_desc.dsc_dtype = static_cast<UCHAR>(field->fld_element_dtype);
			node->nod_desc.dsc_length = field->fld_element_length;

			// node->nod_desc.dsc_scale = field->fld_scale;
			// node->nod_desc.dsc_sub_type = field->fld_sub_type;

		}
		else
		{
			node->nod_desc.dsc_dtype = dtype_array;
			node->nod_desc.dsc_length = sizeof(ISC_QUAD);
			node->nod_desc.dsc_scale = static_cast<SCHAR>(field->fld_scale);
			node->nod_desc.dsc_sub_type = field->fld_sub_type;
		}
	}
	else
	{
		if (indices)
		{
			ERRD_post(Arg::Gds(isc_sqlerr) << Arg::Num(-607) <<
					  Arg::Gds(isc_dsql_only_can_subscript_array) << Arg::Str(field->fld_name));
		}

		MAKE_desc_from_field(&node->nod_desc, field);
	}

	if ((field->fld_flags & FLD_nullable) || (context->ctx_flags & CTX_outer_join))
	{
		node->nod_desc.dsc_flags |= DSC_nullable;
	}

	// UNICODE_FSS_HACK
	// check if the field is a system domain and the type is CHAR/VARCHAR CHARACTER SET UNICODE_FSS
	if ((field->fld_flags & FLD_system) && node->nod_desc.dsc_dtype <= dtype_varying &&
		INTL_GET_CHARSET(&node->nod_desc) == CS_METADATA)
	{
		USHORT adjust = 0;

		if (node->nod_desc.dsc_dtype == dtype_varying)
			adjust = sizeof(USHORT);
		else if (node->nod_desc.dsc_dtype == dtype_cstring)
			adjust = 1;

		node->nod_desc.dsc_length -= adjust;
		node->nod_desc.dsc_length *= 3;
		node->nod_desc.dsc_length += adjust;
	}

	return node;
}


/**

 	MAKE_field_name

    @brief	Make up a field name node.


    @param field_name

 **/
dsql_nod* MAKE_field_name(const char* field_name)
{
    dsql_nod* const field_node = MAKE_node(nod_field_name, (int) e_fln_count);
    field_node->nod_arg[e_fln_name] = (dsql_nod*) MAKE_cstring(field_name);
	return field_node;
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
	thread_db* tdbb = JRD_get_thread_data();

	dsql_nod* node = FB_NEW_RPT(*tdbb->getDefaultPool(), count) dsql_nod;
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
	@param node

 **/
dsql_par* MAKE_parameter(dsql_msg* message, bool sqlda_flag, bool null_flag,
	USHORT sqlda_index, const dsql_nod* node)
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

	thread_db* tdbb = JRD_get_thread_data();

	dsql_sym* symbol = FB_NEW_RPT(*tdbb->getDefaultPool(), length) dsql_sym;
	symbol->sym_type = type;
	symbol->sym_object = object;
	symbol->sym_dbb = database;
	symbol->sym_length = length;
	TEXT* p = symbol->sym_name;
	symbol->sym_string = p;

	if (length)
		memcpy(p, name, length);

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
	thread_db* tdbb = JRD_get_thread_data();

	dsql_str* string = FB_NEW_RPT(*tdbb->getDefaultPool(), length) dsql_str;
	string->str_charset = charset;
	string->str_length  = length;
	memcpy(string->str_data, strvar, length);

	return string;
}


// Make a variable node.
VariableNode* MAKE_variable(dsql_fld* field, const TEXT* name, const dsql_var_type type,
	USHORT msg_number, USHORT item_number, USHORT local_number)
{
	DEV_BLKCHK(field, dsql_type_fld);

	thread_db* tdbb = JRD_get_thread_data();

	VariableNode* node = FB_NEW(*tdbb->getDefaultPool()) VariableNode(*tdbb->getDefaultPool());
	node->dsqlVar = FB_NEW_RPT(*tdbb->getDefaultPool(), strlen(name)) dsql_var;
	node->dsqlVar->var_msg_number = msg_number;
	node->dsqlVar->var_msg_item = item_number;
	node->dsqlVar->var_variable_number = local_number;
	node->dsqlVar->var_field = field;
	strcpy(node->dsqlVar->var_name, name);
	node->dsqlVar->var_type = type;

	if (field)
		MAKE_desc_from_field(&node->varDesc, field);

	return node;
}


/**

	MAKE_parameter_names

	@brief  Determine relation/column/alias names (if appropriate)
			and store them in the given parameter.

	@param parameter
	@param item

**/
void MAKE_parameter_names(dsql_par* parameter, const dsql_nod* item)
{
	const dsql_fld* field;
	const dsql_ctx* context = NULL;
	const dsql_str* string;
	const dsql_nod* alias;

	fb_assert(parameter && item);

	const char* name_alias = NULL;

	switch (item->nod_type)
	{
	case nod_class_exprnode:
		reinterpret_cast<ValueExprNode*>(item->nod_arg[0])->setParameterName(parameter);
		break;
	case nod_field:
		field = (dsql_fld*) item->nod_arg[e_fld_field];
		name_alias = field->fld_name.c_str();
		context = (dsql_ctx*) item->nod_arg[e_fld_context];
		break;
	case nod_dbkey:
		parameter->par_name = parameter->par_alias = DB_KEY_NAME;
		context = (dsql_ctx*) item->nod_arg[0]->nod_arg[0];
		break;
	case nod_alias:
		string = (dsql_str*) item->nod_arg[e_alias_alias];
		parameter->par_alias = string->str_data;
		alias = item->nod_arg[e_alias_value];
		if (alias->nod_type == nod_field)
		{
			field = (dsql_fld*) alias->nod_arg[e_fld_field];
			parameter->par_name = field->fld_name.c_str();
			context = (dsql_ctx*) alias->nod_arg[e_fld_context];
		}
		else if (alias->nod_type == nod_dbkey)
		{
			parameter->par_name = DB_KEY_NAME;
			context = (dsql_ctx*) alias->nod_arg[0]->nod_arg[0];
		}
		break;
	case nod_derived_field:
		string = (dsql_str*) item->nod_arg[e_derived_field_name];
		parameter->par_alias = string->str_data;
		alias = item->nod_arg[e_derived_field_value];
		if (alias->nod_type == nod_field)
		{
			field = (dsql_fld*) alias->nod_arg[e_fld_field];
			parameter->par_name = field->fld_name.c_str();
			context = (dsql_ctx*) alias->nod_arg[e_fld_context];
		}
		else if (alias->nod_type == nod_dbkey)
		{
			parameter->par_name = DB_KEY_NAME;
			context = (dsql_ctx*) alias->nod_arg[0]->nod_arg[0];
		}
		break;
	case nod_map:
		{
			const dsql_map* map = (dsql_map*) item->nod_arg[e_map_map];
			const dsql_nod* map_node = map->map_node;
			while (map_node->nod_type == nod_map)
			{
				// skip all the nod_map nodes
				map = (dsql_map*) map_node->nod_arg[e_map_map];
				map_node = map->map_node;
			}

			switch (map_node->nod_type)
			{
			case nod_field:
				field = (dsql_fld*) map_node->nod_arg[e_fld_field];
				name_alias = field->fld_name.c_str();
				context = (dsql_ctx*) map_node->nod_arg[e_fld_context];
				break;
			case nod_alias:
				string = (dsql_str*) map_node->nod_arg[e_alias_alias];
				parameter->par_alias = string->str_data;
				alias = map_node->nod_arg[e_alias_value];
				if (alias->nod_type == nod_field)
				{
					field = (dsql_fld*) alias->nod_arg[e_fld_field];
					parameter->par_name = field->fld_name.c_str();
					context = (dsql_ctx*) alias->nod_arg[e_fld_context];
				}
				break;
			case nod_derived_field:
				string = (dsql_str*) map_node->nod_arg[e_derived_field_name];
				parameter->par_alias = string->str_data;
				alias = map_node->nod_arg[e_derived_field_value];
				if (alias->nod_type == nod_field)
				{
					field = (dsql_fld*) alias->nod_arg[e_fld_field];
					parameter->par_name = field->fld_name.c_str();
					context = (dsql_ctx*) alias->nod_arg[e_fld_context];
				}
				break;
			case nod_dbkey:
				name_alias = DB_KEY_NAME;
				break;

			case nod_class_exprnode:
				{
					const AggNode* aggNode;
					const LiteralNode* literalNode;

					if ((aggNode = ExprNode::as<AggNode>(map_node)))
						aggNode->setParameterName(parameter);
					else if ((literalNode = ExprNode::as<LiteralNode>(map_node)))
						literalNode->setParameterName(parameter);
				}
				break;
			} // switch(map_node->nod_type)
			break;
		} // case nod_map
	case nod_searched_case:
	case nod_simple_case:
		name_alias = "CASE";
		break;
	case nod_coalesce:
		name_alias = "COALESCE";
		break;
	}

	if (name_alias)
		parameter->par_name = parameter->par_alias = name_alias;

	if (context)
	{
		if (context->ctx_relation)
		{
			parameter->par_rel_name = context->ctx_relation->rel_name.c_str();
			parameter->par_owner_name = context->ctx_relation->rel_owner.c_str();
		}
		else if (context->ctx_procedure)
		{
			parameter->par_rel_name = context->ctx_procedure->prc_name.identifier.c_str();
			parameter->par_owner_name = context->ctx_procedure->prc_owner.c_str();
		}

		parameter->par_rel_alias = context->ctx_alias;
	}
}
