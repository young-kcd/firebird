/*
 * tab=4
 *____________________________________________________________
 *  
 *		PROGRAM:	C preprocessor
 *		MODULE:		gpre_meta_boot.cpp
 *		DESCRIPTION:	Meta data interface to system
 *  
 *  The contents of this file are subject to the Interbase Public
 *  License Version 1.0 (the "License"); you may not use this file
 *  except in compliance with the License. You may obtain a copy
 *  of the License at http://www.Inprise.com/IPL.html
 *  
 *  Software distributed under the License is distributed on an
 *  "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 *  or implied. See the License for the specific language governing
 *  rights and limitations under the License.
 *  
 *  The Original Code was created by Inprise Corporation
 *  and its predecessors. Portions created by Inprise Corporation are
 *  Copyright (C) Inprise Corporation.
 *  
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *  
 *
 *____________________________________________________________
 *
 *	$Id: gpre_meta_boot.cpp,v 1.33 2003-11-08 16:31:40 brodsom Exp $
 */

#include "firebird.h"
#include <string.h>
#include "../jrd/y_ref.h"
#include "../jrd/ibase.h"
#include "../gpre/gpre.h"
#include "../jrd/license.h"
#include "../gpre/parse.h"
#include "../jrd/intl.h"
#include "../gpre/gpre_proto.h"
#include "../gpre/hsh_proto.h"
#include "../gpre/jrdme_proto.h"
#include "../gpre/gpre_meta.h"
#include "../gpre/msc_proto.h"
#include "../gpre/par_proto.h"
#include "../jrd/constants.h"
#include "../jrd/gds_proto.h"

//const int MAX_USER_LENGTH		= 33;
//const int MAX_PASSWORD_LENGTH	= 33;

extern enum lang_t sw_language;
extern bool sw_cstring;
extern DBB isc_databases;

static const UCHAR blr_bpb[] = { isc_bpb_version1,
	isc_bpb_source_type, 1, BLOB_blr,
	isc_bpb_target_type, 1, BLOB_blr
};

#ifdef SCROLLABLE_CURSORS
static SCHAR db_version_info[] = { isc_info_base_level };
#endif
#ifdef NOT_USED_OR_REPLACED
static SLONG array_size(GPRE_FLD);
static void get_array(DBB, const TEXT*, GPRE_FLD);
static bool get_intl_char_subtype(SSHORT*, const UCHAR*, USHORT, DBB);
static bool resolve_charset_and_collation(SSHORT*, const UCHAR*, const UCHAR*);
static int symbol_length(const TEXT*);
static int upcase(const TEXT*, TEXT*);
#endif

/*____________________________________________________________
 *  
 *		Lookup a field by name in a context.
 *		If found, return field block.  If not, return NULL.
 */  

GPRE_FLD MET_context_field( GPRE_CTX context, const char* string)
{
	SCHAR name[NAME_SIZE];

	if (context->ctx_relation) {
		return (MET_field(context->ctx_relation, string));
	}

	if (!context->ctx_procedure) {
		return NULL;
	}

	strcpy(name, string);
	GPRE_PRC procedure = context->ctx_procedure;

/*  At this point the procedure should have been scanned, so all
 *  its fields are in the symbol table.
 */

	GPRE_FLD field = NULL;
	for (SYM symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym) {
		if (symbol->sym_type == SYM_field &&
			(field = (GPRE_FLD) symbol->sym_object) &&
			field->fld_procedure == procedure)
		{
			return field;
		}
	}

	return field;
}


/*____________________________________________________________
 *  
 *		Initialize meta data access to database.  If the
 *		database can't be opened, return FALSE.
 */  

bool MET_database(DBB db,
				  bool print_version)
{
	/* 
	   ** Each info item requested will return 
	   **
	   **     1 byte for the info item tag
	   **     2 bytes for the length of the information that follows
	   **     1 to 4 bytes of integer information
	   **
	   ** isc_info_end will not have a 2-byte length - which gives us
	   ** some padding in the buffer.
	 */

#ifndef REQUESTER
	if (sw_language == lang_internal) {
		JRDMET_init(db);
		return true;
	}
#endif
	fb_assert(0);
	return false;
}


/*____________________________________________________________
 *  
 *		Lookup a domain by name.
 *		Initialize the size of the field.
 */  

bool MET_domain_lookup(GPRE_REQ request,
					   GPRE_FLD field, 
					   const char* string)
{
	SCHAR name[NAME_SIZE];

	strcpy(name, string);

/*  Lookup domain.  If we find it in the hash table, and it is not the
 *  field we a currently looking at, use it.  Else look it up from the
 *  database.
 */

	for (SYM symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
	{
		GPRE_FLD d_field;
		if ((symbol->sym_type == SYM_field) &&
			((d_field = (GPRE_FLD) symbol->sym_object) && (d_field != field))) 
		{
			field->fld_length = d_field->fld_length;
			field->fld_scale = d_field->fld_scale;
			field->fld_sub_type = d_field->fld_sub_type;
			field->fld_dtype = d_field->fld_dtype;
			field->fld_ttype = d_field->fld_ttype;
			field->fld_charset_id = d_field->fld_charset_id;
			field->fld_collate_id = d_field->fld_collate_id;
			field->fld_char_length = d_field->fld_char_length;
			return true;
		}
	}

	if (!request)
		return false;

	fb_assert(0);
	return false;
}


/*____________________________________________________________
 *  
 *		Gets the default value for a domain of an existing table
 */  

bool MET_get_domain_default(DBB db,
							const TEXT* domain_name,
							TEXT* buffer,
							USHORT buff_length)
{
	fb_assert(0);
	return false;
}


/*____________________________________________________________
 *  
 *		Gets the default value for a column of an existing table.
 *		Will check the default for the column of the table, if that is
 *		not present, will check for the default of the relevant domain
 *  
 *		The default blr is returned in buffer. The blr is of the form
 *		blr_version4 blr_literal ..... blr_eoc
 *  
 *		Reads the system tables RDB$FIELDS and RDB$RELATION_FIELDS.
 */  

bool MET_get_column_default(const gpre_rel* relation,
							const TEXT* column_name,
							TEXT* buffer,
							USHORT buff_length)
{
	fb_assert(0);
	return false;
}


/*____________________________________________________________
 *  
 *		Lookup the fields for the primary key
 *		index on a relation, returning a list
 *		of the fields.
 */  

LLS MET_get_primary_key(DBB db, const TEXT* relation_name)
{
	SCHAR name[NAME_SIZE];

	strcpy(name, relation_name);

	if (db == NULL)
		return NULL;

	if ((db->dbb_handle == NULL) && !MET_database(db, FALSE))
		CPR_exit(FINI_ERROR);

	fb_assert(db->dbb_transaction == NULL);
	fb_assert(0);
	return 0;
}


/*____________________________________________________________
 *  
 *		Lookup a field by name in a relation.
 *		If found, return field block.  If not, return NULL.
 */  

GPRE_FLD MET_field(GPRE_REL relation, const char* string)
{
	GPRE_FLD field;
	SCHAR name[NAME_SIZE];

	strcpy(name, string);
	//const SSHORT length = strlen(name);

/*  Lookup field.  If we find it, nifty.  If not, look it up in the
 *  database.
 */

	for (SYM symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_keyword &&
			symbol->sym_keyword == (int) KW_DBKEY)
		{
			return relation->rel_dbkey;
		}
		else if (symbol->sym_type == SYM_field &&
				 (field = (GPRE_FLD) symbol->sym_object) &&
				 field->fld_relation == relation)
		{
			return field;
		}

	if (sw_language == lang_internal)
		return NULL;
	fb_assert(0);
	return NULL;
}


/*____________________________________________________________
 *  
 *     Return a list of the fields in a relation
 */  

GPRE_NOD MET_fields(GPRE_CTX context)
{
	GPRE_FLD field;
	GPRE_NOD node, field_node;
	REF reference;

	GPRE_PRC procedure = context->ctx_procedure;
	if (procedure) {
		node = MSC_node(nod_list, procedure->prc_out_count);
		//int count = 0;
		for (field = procedure->prc_outputs; field; field = field->fld_next) {
			reference = (REF) MSC_alloc(REF_LEN);
			reference->ref_field = field;
			reference->ref_context = context;
			field_node = MSC_unary(nod_field, (GPRE_NOD)reference);
			node->nod_arg[field->fld_position] = field_node;
			//count++;
		}
		return node;
	}

	GPRE_REL relation = context->ctx_relation;
	if (relation->rel_meta) {
		int count = 0;
		for (field = relation->rel_fields; field; field = field->fld_next) {
			count++;
		}
		node = MSC_node(nod_list, count);
		//count = 0;
		for (field = relation->rel_fields; field; field = field->fld_next) {
			reference = (REF) MSC_alloc(REF_LEN);
			reference->ref_field = field;
			reference->ref_context = context;
			field_node = MSC_unary(nod_field, (GPRE_NOD)reference);
			node->nod_arg[field->fld_position] = field_node;
			//count++;
		}
		return node;
	}

	if (sw_language == lang_internal)
		return NULL;
	fb_assert(0);
	return NULL;
}


/*____________________________________________________________
 *  
 *		Shutdown all attached databases.
 */  

void MET_fini( DBB end)
{
	return;
}


/*____________________________________________________________
 *  
 *		Lookup a generator by name.
 *		If found, return string. If not, return NULL.
 */  

const SCHAR* MET_generator(const TEXT* string, DBB db)
{
	SCHAR name[NAME_SIZE];

	strcpy(name, string);

	for (SYM symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
		if ((symbol->sym_type == SYM_generator) &&
			(db == (DBB) (symbol->sym_object))) 
		{
			return symbol->sym_string;
		}

	return NULL;
}


/*____________________________________________________________
 *  
 *		Compute internal datatype and length based on system relation field values.
 */  

USHORT MET_get_dtype(USHORT blr_dtype, USHORT sub_type, USHORT* length)
{
	USHORT dtype;

	USHORT l = *length;

	switch (blr_dtype) {
	case blr_varying:
	case blr_text:
		dtype = dtype_text;
		if (sw_cstring && sub_type != dsc_text_type_fixed) {
			++l;
			dtype = dtype_cstring;
		}
		break;

	case blr_cstring:
		dtype = dtype_cstring;
		++l;
		break;

	case blr_short:
		dtype = dtype_short;
		l = sizeof(SSHORT);
		break;

	case blr_long:
		dtype = dtype_long;
		l = sizeof(SLONG);
		break;

	case blr_quad:
		dtype = dtype_quad;
		l = sizeof(GDS_QUAD);
		break;

	case blr_float:
		dtype = dtype_real;
		l = sizeof(float);
		break;

	case blr_double:
		dtype = dtype_double;
		l = sizeof(double);
		break;

	case blr_blob:
		dtype = dtype_blob;
		l = sizeof(GDS_QUAD);
		break;

/** Begin sql date/time/timestamp **/
	case blr_sql_date:
		dtype = dtype_sql_date;
		l = sizeof(ISC_DATE);
		break;

	case blr_sql_time:
		dtype = dtype_sql_time;
		l = sizeof(ISC_TIME);
		break;

	case blr_timestamp:
		dtype = dtype_timestamp;
		l = sizeof(ISC_TIMESTAMP);
		break;
/** Begin sql date/time/timestamp **/

	case blr_int64:
		dtype = dtype_int64;
		l = sizeof(ISC_INT64);
		break;

	default:
		CPR_error("datatype not supported");
	}

	*length = l;

	return dtype;
}


/*____________________________________________________________
 *  
 *		Lookup a procedure (represented by a token) in a database.
 *		Return a procedure block (if name is found) or NULL.
 *  
 *		This function has been cloned into MET_get_udf
 */  

GPRE_PRC MET_get_procedure(DBB db, const TEXT* string, const TEXT* owner_name)
{
	SCHAR name[NAME_SIZE], owner[NAME_SIZE];

	strcpy(name, string);
	strcpy(owner, owner_name);
	GPRE_PRC procedure = NULL;

	for (SYM symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_procedure &&
			(procedure = (GPRE_PRC) symbol->sym_object) &&
			procedure->prc_database == db &&
			(!owner[0] ||
			 (procedure->prc_owner
			  && !strcmp(owner, procedure->prc_owner->sym_string)))) 
		{
			break;
		}

	if (!procedure)
		return NULL;

	if (procedure->prc_flags & PRC_scanned)
		return procedure;

	fb_assert(0);
	return NULL;
}


/*____________________________________________________________
 *  
 *		Lookup a relation (represented by a token) in a database.
 *		Return a relation block (if name is found) or NULL.
 */  

GPRE_REL MET_get_relation(DBB db, const TEXT* string, const TEXT* owner_name)
{
	GPRE_REL relation;
	SCHAR name[NAME_SIZE], owner[NAME_SIZE];
	
	strcpy(name, string);
	strcpy(owner, owner_name);

	for (SYM symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_relation &&
			(relation = (GPRE_REL) symbol->sym_object) &&
			relation->rel_database == db &&
			(!owner[0] ||
			 (relation->rel_owner
			  && !strcmp(owner,
						 relation->rel_owner->sym_string))))
		{
			return relation;
		}

	return NULL;
}


/*____________________________________________________________
 *  
 */  

INTLSYM MET_get_text_subtype(SSHORT ttype)
{
	for (INTLSYM p = text_subtypes; p; p = p->intlsym_next)
		if (p->intlsym_ttype == ttype)
			return p;

	return NULL;
}


/*____________________________________________________________
 *  
 *		Lookup a udf (represented by a token) in a database.
 *		Return a udf block (if name is found) or NULL.
 *  
 *		This function was cloned from MET_get_procedure
 */  

UDF MET_get_udf(DBB db, const TEXT* string)
{
	SCHAR name[NAME_SIZE];

	strcpy(name, string);
	UDF udf_val = NULL;
	for (SYM symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_udf &&
			(udf_val = (UDF) symbol->sym_object) && udf_val->udf_database == db)
		{
			break;
		}

	if (!udf_val)
		return NULL;

	fb_assert(0);
	return NULL;
}


/*____________________________________________________________
 *  
 *		Return relation if the passed view_name represents a 
 *		view with the passed relation as a base table 
 *		(the relation could be an alias).
 */  

GPRE_REL MET_get_view_relation(GPRE_REQ request,
						  const char* view_name,
						  const char* relation_or_alias, USHORT level)
{
	fb_assert(0);
	return NULL;
}


/*____________________________________________________________
 *  
 *		Lookup an index for a database.
 *		Return an index block (if name is found) or NULL.
 */  

IND MET_index(DBB db, const TEXT* string)
{
	IND index;
	SCHAR name[NAME_SIZE];

	strcpy(name, string);
	//const SSHORT length = strlen(name);

	for (SYM symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_index &&
			(index = (IND) symbol->sym_object) &&
			index->ind_relation->rel_database == db)
		{
			return index;
		}

	if (sw_language == lang_internal)
		return NULL;
	fb_assert(0);
	return NULL;
}


/*____________________________________________________________
 *  
 *		Load all of the relation names
 *       and user defined function names
 *       into the symbol (hash) table.
 */  

void MET_load_hash_table( DBB db)
{
/*  If this is an internal ISC access method invocation, don't do any of this
 *  stuff
 */

	if (sw_language == lang_internal)
		return;
	fb_assert(0);
	return;
}


/*____________________________________________________________
 *  
 *		Make a field symbol.
 */  

GPRE_FLD MET_make_field(const SCHAR* name,
						SSHORT dtype,
						SSHORT length,
						bool insert_flag)
{
	GPRE_FLD field = (GPRE_FLD) MSC_alloc(FLD_LEN);
	field->fld_length = length;
	field->fld_dtype = dtype;
	SYM symbol = MSC_symbol(SYM_field, name, strlen(name), (GPRE_CTX)field);
	field->fld_symbol = symbol;

	if (insert_flag)
		HSH_insert(symbol);

	return field;
}


/*____________________________________________________________
 *  
 *		Make an index symbol.
 */  

IND MET_make_index(const SCHAR* name)
{
	IND index = (IND) MSC_alloc(IND_LEN);
	index->ind_symbol = MSC_symbol(SYM_index, name, strlen(name), (GPRE_CTX)index);

	return index;
}


/*____________________________________________________________
 *  
 *		Make an relation symbol.
 */  

GPRE_REL MET_make_relation(const SCHAR* name)
{
	GPRE_REL relation = (GPRE_REL) MSC_alloc(REL_LEN);
	relation->rel_symbol =
		MSC_symbol(SYM_relation, name, strlen(name), (GPRE_CTX)relation);

	return relation;
}


/*____________________________________________________________
 *  
 *		Lookup a type name for a field.
 */  

bool MET_type(GPRE_FLD field,
			  const TEXT* string,
			  SSHORT* ptr)
{
	TYP type;

	for (SYM symbol = HSH_lookup(string); symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_type &&
			(type = (TYP) symbol->sym_object) &&
			(!type->typ_field || type->typ_field == field))
		{
			*ptr = type->typ_value;
			return true;
		}

	fb_assert(0);
	return false;
}


/*____________________________________________________________
 *  
 *		Lookup an index for a database.
 *  
 *  Return: true if the trigger exists
 *		   false otherwise
 */  

bool MET_trigger_exists(DBB db,
						const TEXT* trigger_name)
{
	//SCHAR name[NAME_SIZE];

	//strcpy(name, trigger_name);

	fb_assert(0);
	return false;
}

#ifdef NOT_USED_OR_REPLACED
/*____________________________________________________________
 *  
 *		Compute and return the size of the array.
 */  

static SLONG array_size( GPRE_FLD field)
{
	ARY array_block = field->fld_array_info;
	SLONG count = field->fld_array->fld_length;
	for (DIM dimension = array_block->ary_dimension; dimension;
		 dimension = dimension->dim_next) 
	{
		count =
			count * (dimension->dim_upper - dimension->dim_lower + 1);
	}

	return count;
}


/*____________________________________________________________
 *  
 *		See if field is array.
 */  

static void get_array( DBB db, const TEXT* field_name, GPRE_FLD field)
{
	fb_assert(0);
	return;
}


/*____________________________________________________________
 *  
 *		Character types can be specified as either:
 *		   b) A POSIX style locale name "<collation>.<characterset>"
 *		   or
 *		   c) A simple <characterset> name (using default collation)
 *		   d) A simple <collation> name    (use charset for collation)
 *  
 *		Given an ASCII7 string which could be any of the above, try to
 *		resolve the name in the order b, c, d.
 *		b) is only tried iff the name contains a period.
 *		(in which case c) and d) are not tried).
 *  
 *  Return:
 *		true if no errors (and *id is set).
 *		false if the name could not be resolved.
 */  

static bool get_intl_char_subtype(SSHORT* id,
								 const UCHAR* name,
								 USHORT length,
								 DBB db)
{
	fb_assert(id != NULL);
	fb_assert(name != NULL);
	fb_assert(db != NULL);

	fb_assert(0);
	return false;
}


/*____________________________________________________________
 *  
 *		Given ASCII7 name of charset & collation
 *		resolve the specification to a ttype (id) that implements
 *		it.
 *  
 *  Inputs:
 *		(charset) 
 *			ASCII7z name of characterset.
 *			NULL (implying unspecified) means use the character set
 *		        for defined for (collation).
 *  
 *		(collation)
 *			ASCII7z name of collation.
 *			NULL means use the default collation for (charset).
 *  
 *  Outputs:
 *		(*id)	
 *			Set to subtype specified by this name.
 *  
 *  Return:
 *		true if no errors (and *id is set).
 *		false if either name not found.
 *		  or if names found, but the collation isn't for the specified
 *		  character set.
 */  

static bool resolve_charset_and_collation(
										 SSHORT* id,
										 const UCHAR* charset, const UCHAR* collation)
{
	fb_assert(id != NULL);

	fb_assert(0);
	return (false);
}


/*____________________________________________________________
 *  
 *		Compute significant length of symbol.
 */  

static int symbol_length(const TEXT* string)
{
	const size_t len = strlen(string);

	const TEXT* p = string + (len - 1);

	for (; p >= string && *p == ' '; p--);
	if (p < string)
		return 0;
	++p;
	return p - string;
}


/*____________________________________________________________
 *  
 *		Upcase a string into another string.  Return
 *		length of string.
 */

static int upcase(const TEXT* from, TEXT* to)
{
	TEXT c;

	TEXT* p = to;
	TEXT* const end = to + NAME_SIZE;

	while (p < end && (c = *from++)) {
		*p++ = UPPER(c);
	}

	*p = 0;

	return p - to;
}
#endif // NOT_USED_OR_REPLACED

ISC_STATUS API_ROUTINE isc_print_blr(SCHAR* blr,
          void (*callback) (), void* callback_argument, SSHORT language)
{
        return gds__print_blr((UCHAR*) blr, 
							  reinterpret_cast<FPTR_PRINT_CALLBACK>(callback),
                              callback_argument, language);
}

extern "C" {

void CVT_move (const dsc* a, dsc* b, FPTR_VOID c)
{  
    fb_assert(0);
    /* Not available in boot_gpre */
}

void ERR_bugcheck(int number)
{
}

void ERR_post(ISC_STATUS status, ...)
{
}

} // extern "C"

