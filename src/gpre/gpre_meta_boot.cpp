/*
 * tab=4
 *____________________________________________________________
 *  
 *		PROGRAM:	C preprocessor
 *		MODULE:		gpre_meta.e
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
 *	$Id: gpre_meta_boot.cpp,v 1.10 2002-11-30 17:40:24 hippoman Exp $
 */

#include "firebird.h"
#include <string.h>
#include "../jrd/gds.h"
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

#define MAX_USER_LENGTH		33
#define MAX_PASSWORD_LENGTH	33

extern enum lang_t sw_language;
extern USHORT sw_cstring;
extern DBB isc_databases;

static CONST UCHAR blr_bpb[] = { isc_bpb_version1,
	isc_bpb_source_type, 1, BLOB_blr,
	isc_bpb_target_type, 1, BLOB_blr
};

#ifdef SCROLLABLE_CURSORS
static SCHAR db_version_info[] = { gds__info_base_level };
#endif

static SLONG array_size(GPRE_FLD);
static void get_array(DBB, TEXT *, GPRE_FLD);
static int get_intl_char_subtype(SSHORT *, UCHAR *, USHORT, DBB);
static int resolve_charset_and_collation(SSHORT *, UCHAR *, UCHAR *);
static int symbol_length(TEXT *);
static int upcase(TEXT *, TEXT *);


/*____________________________________________________________
 *  
 *		Lookup a field by name in a context.
 *		If found, return field block.  If not, return NULL.
 */  

GPRE_FLD MET_context_field( GPRE_CTX context, char *string)
{
	SYM symbol;
	GPRE_PRC procedure;
	GPRE_FLD field;
	SCHAR name[NAME_SIZE];
	SSHORT length;

	if (context->ctx_relation) {
		return (MET_field(context->ctx_relation, string));
	}

	if (!context->ctx_procedure) {
		return NULL;
	}

	strcpy(name, string);
	procedure = context->ctx_procedure;

/*  At this point the procedure should have been scanned, so all
 *  its fields are in the symbol table.
 */

	field = NULL;
	for (symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym) {
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

BOOLEAN MET_database(DBB dbb, BOOLEAN print_version)
{
	SCHAR dpb[MAX_PASSWORD_LENGTH + MAX_USER_LENGTH + 5], *d, *p;
	SCHAR buffer[16], *data;
	SSHORT l;
	static CONST UCHAR sql_version_info[] = { isc_info_base_level,
		isc_info_ods_version,
		isc_info_db_sql_dialect,
		isc_info_end
	};
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
	UCHAR sql_buffer[sizeof(sql_version_info) * (1 + 2 + 4)];
	UCHAR *ptr;


#ifndef REQUESTER
	if (sw_language == lang_internal) {
		JRDMET_init(dbb);
		return TRUE;
	}
#endif
	assert(0);
	return FALSE;
}


/*____________________________________________________________
 *  
 *		Lookup a domain by name.
 *		Initialize the size of the field.
 */  

USHORT MET_domain_lookup(GPRE_REQ request, GPRE_FLD field, char *string)
{
	SYM symbol;
	DBB dbb;
	SCHAR name[NAME_SIZE];
	GPRE_FLD d_field;
	SSHORT length;
	SSHORT found = FALSE;

	strcpy(name, string);

/*  Lookup domain.  If we find it in the hash table, and it is not the
 *  field we a currently looking at, use it.  Else look it up from the
 *  database.
 */

	for (symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
		if ((symbol->sym_type == SYM_field) &&
			((d_field = (GPRE_FLD) symbol->sym_object) && (d_field != field))) {
			field->fld_length = d_field->fld_length;
			field->fld_scale = d_field->fld_scale;
			field->fld_sub_type = d_field->fld_sub_type;
			field->fld_dtype = d_field->fld_dtype;
			field->fld_ttype = d_field->fld_ttype;
			field->fld_charset_id = d_field->fld_charset_id;
			field->fld_collate_id = d_field->fld_collate_id;
			field->fld_char_length = d_field->fld_char_length;
			return TRUE;
		}

	if (!request)
		return FALSE;

	assert(0);
	return FALSE;
}


/*____________________________________________________________
 *  
 *		Gets the default value for a domain of an existing table
 */  

BOOLEAN MET_get_domain_default(DBB dbb,
							   TEXT * domain_name,
							   TEXT * buffer, USHORT buff_length)
{
	SLONG *DB, *gds__rans;
	SCHAR name[NAME_SIZE];
	SSHORT length;
	BOOLEAN has_default;
	ISC_STATUS status_vect[ISC_STATUS_LENGTH];
	isc_blob_handle blob_handle = NULL;
	ISC_QUAD *blob_id;
	TEXT *ptr_in_buffer;
	STATUS stat;


	strcpy(name, domain_name);
	has_default = FALSE;

	assert(0);
	return FALSE;
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

BOOLEAN MET_get_column_default(GPRE_REL relation,
							   TEXT * column_name,
							   TEXT * buffer, USHORT buff_length)
{
	DBB dbb;
	SLONG *DB, *gds__trans;
	SCHAR name[NAME_SIZE];
	SSHORT length;
	BOOLEAN has_default;
	ISC_STATUS status_vect[ISC_STATUS_LENGTH];
	isc_blob_handle blob_handle = NULL;
	ISC_QUAD *blob_id;
	TEXT *ptr_in_buffer;
	STATUS stat;

	strcpy(name, column_name);
	has_default = FALSE;
	assert(0);
	return FALSE;
}


/*____________________________________________________________
 *  
 *		Lookup the fields for the primary key
 *		index on a relation, returning a list
 *		of the fields.
 */  

LLS MET_get_primary_key(DBB dbb, TEXT * relation_name)
{
	LLS fields = NULL, *ptr_fields;
	SLONG *DB, *gds__trans;
	SCHAR name[NAME_SIZE];
	STR field_name;
	TEXT *tmp;

	strcpy(name, relation_name);

	if (dbb == NULL)
		return NULL;

	if ((dbb->dbb_handle == NULL) && !MET_database(dbb, FALSE))
		CPR_exit(FINI_ERROR);

	assert(dbb->dbb_transaction == NULL);
	gds__trans = NULL;
	assert(0);
	return FALSE;
}


/*____________________________________________________________
 *  
 *		Lookup a field by name in a relation.
 *		If found, return field block.  If not, return NULL.
 */  

GPRE_FLD MET_field(GPRE_REL relation, char *string)
{
	SYM symbol;
	GPRE_FLD field;
	DBB dbb;
	SCHAR name[NAME_SIZE];
	SSHORT length;

	strcpy(name, string);
	length = strlen(name);

/*  Lookup field.  If we find it, nifty.  If not, look it up in the
 *  database.
 */

	for (symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_keyword &&
			symbol->sym_keyword == (int) KW_DBKEY) return relation->rel_dbkey;
		else if (symbol->sym_type == SYM_field &&
				 (field = (GPRE_FLD) symbol->sym_object) &&
				 field->fld_relation == relation) return field;

	if (sw_language == lang_internal)
		return NULL;
	assert(0);
	return NULL;
}


/*____________________________________________________________
 *  
 *     Return a list of the fields in a relation
 */  

GPRE_NOD MET_fields(GPRE_CTX context)
{
	DBB dbb;
	GPRE_FLD field;
	LLS stack;
	GPRE_NOD node, field_node;
	REF reference;
	GPRE_PRC procedure;
	GPRE_REL relation;
	TEXT *p;
	int count;

	if (procedure = context->ctx_procedure) {
		node = MAKE_NODE(nod_list, procedure->prc_out_count);
		count = 0;
		for (field = procedure->prc_outputs; field; field = field->fld_next) {
			reference = (REF) ALLOC(REF_LEN);
			reference->ref_field = field;
			reference->ref_context = context;
			field_node = MSC_unary(nod_field, (GPRE_NOD)reference);
			node->nod_arg[field->fld_position] = field_node;
			count++;
		}
		return node;
	}

	relation = context->ctx_relation;
	if (relation->rel_meta) {
		for (count = 0, field = relation->rel_fields; field;
			 field = field->fld_next) count++;
		node = MAKE_NODE(nod_list, count);
		count = 0;
		for (field = relation->rel_fields; field; field = field->fld_next) {
			reference = (REF) ALLOC(REF_LEN);
			reference->ref_field = field;
			reference->ref_context = context;
			field_node = MSC_unary(nod_field, (GPRE_NOD)reference);
			node->nod_arg[field->fld_position] = field_node;
			count++;
		}
		return node;
	}

	if (sw_language == lang_internal)
		return NULL;
	assert(0);
	return NULL;
}


/*____________________________________________________________
 *  
 *		Shutdown all attached databases.
 */  

void MET_fini( DBB end)
{
	DBB dbb;

	return;
}


/*____________________________________________________________
 *  
 *		Lookup a generator by name.
 *		If found, return string. If not, return NULL.
 */  

SCHAR *MET_generator(TEXT * string, DBB dbb)
{
	SYM symbol;
	SCHAR *gen_name = NULL;
	SCHAR name[NAME_SIZE];
	SSHORT length;

	strcpy(name, string);

	for (symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
		if ((symbol->sym_type == SYM_generator) &&
			(dbb == (DBB) (symbol->sym_object))) return symbol->sym_string;

	return NULL;
}


/*____________________________________________________________
 *  
 *		Compute internal datatype and length based on system relation field values.
 */  

USHORT MET_get_dtype(USHORT blr_dtype, USHORT sub_type, USHORT * length)
{
	USHORT l, dtype;

	l = *length;

	switch (blr_dtype) {
	case blr_varying:
	case blr_text:
		dtype = dtype_text;
		if (sw_cstring && !SUBTYPE_ALLOWS_NULLS(sub_type)) {
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
		l = sizeof(GDS__QUAD);
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
		l = sizeof(GDS__QUAD);
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

GPRE_PRC MET_get_procedure(DBB dbb, TEXT * string, TEXT * owner_name)
{
	SYM symbol;
	GPRE_FLD *fld_list, field;
	GPRE_PRC procedure;
	USHORT length, type, count;
	SCHAR name[NAME_SIZE], owner[NAME_SIZE];

	strcpy(name, string);
	strcpy(owner, owner_name);
	procedure = NULL;

	for (symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_procedure &&
			(procedure = (GPRE_PRC) symbol->sym_object) &&
			procedure->prc_database == dbb &&
			(!owner[0] ||
			 (procedure->prc_owner
			  && !strcmp(owner, procedure->prc_owner->sym_string)))) break;

	if (!procedure)
		return NULL;

	if (procedure->prc_flags & PRC_scanned)
		return procedure;

	assert(0);
	return NULL;
}


/*____________________________________________________________
 *  
 *		Lookup a relation (represented by a token) in a database.
 *		Return a relation block (if name is found) or NULL.
 */  

GPRE_REL MET_get_relation(DBB dbb, TEXT * string, TEXT * owner_name)
{
	SYM symbol;
	GPRE_REL relation;
	SCHAR name[NAME_SIZE], owner[NAME_SIZE];

	strcpy(name, string);
	strcpy(owner, owner_name);

	for (symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_relation &&
			(relation = (GPRE_REL) symbol->sym_object) &&
			relation->rel_database == dbb &&
			(!owner[0] ||
			 (relation->rel_owner
			  && !strcmp(owner,
						 relation->rel_owner->sym_string)))) return relation;

	return NULL;
}


/*____________________________________________________________
 *  
 */  

INTLSYM MET_get_text_subtype(SSHORT ttype)
{
	INTLSYM p;

	for (p = text_subtypes; p; p = p->intlsym_next)
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

UDF MET_get_udf(DBB dbb, TEXT * string)
{
	SYM symbol;
	GPRE_FLD field;
	UDF udf;
	USHORT length, count;
	SCHAR name[NAME_SIZE];

	strcpy(name, string);
	udf = NULL;
	for (symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_udf &&
			(udf = (UDF) symbol->sym_object) && udf->udf_database == dbb)
			break;
	if (!udf)
		return NULL;

	assert(0);
	return NULL;
}


/*____________________________________________________________
 *  
 *		Return TRUE if the passed view_name represents a 
 *		view with the passed relation as a base table 
 *		(the relation could be an alias).
 */  

GPRE_REL MET_get_view_relation(GPRE_REQ request,
						  char *view_name,
						  char *relation_or_alias, USHORT level)
{
	DBB dbb;
	TEXT *p;
	GPRE_REL relation;

	assert(0);
	return NULL;
}


/*____________________________________________________________
 *  
 *		Lookup an index for a database.
 *		Return an index block (if name is found) or NULL.
 */  

IND MET_index(DBB dbb, TEXT * string)
{
	SYM symbol;
	IND index;
	SCHAR name[NAME_SIZE];
	SSHORT length;

	strcpy(name, string);
	length = strlen(name);

	for (symbol = HSH_lookup(name); symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_index &&
			(index = (IND) symbol->sym_object) &&
			index->ind_relation->rel_database == dbb)
			return index;

	if (sw_language == lang_internal)
		return NULL;
	assert(0);
	return NULL;
}


/*____________________________________________________________
 *  
 *		Load all of the relation names
 *       and user defined function names
 *       into the symbol (hash) table.
 */  

void MET_load_hash_table( DBB dbb)
{
	GPRE_REL relation;
	GPRE_PRC procedure;
	SYM symbol;
	GPRE_FLD dbkey;
	UDF udf;
	TEXT *p;
	int *handle, *handle2;
	USHORT post_v3_flag;
	SLONG length;
	INTLSYM iname;

/*  If this is an internal ISC access method invocation, don't do any of this
 *  stuff
 */

	if (sw_language == lang_internal)
		return;
	assert(0);
	return;
}


/*____________________________________________________________
 *  
 *		Make a field symbol.
 */  

GPRE_FLD MET_make_field(SCHAR * name,
				   SSHORT dtype, SSHORT length, BOOLEAN insert_flag)
{
	GPRE_FLD field;
	SYM symbol;

	field = (GPRE_FLD) ALLOC(FLD_LEN);
	field->fld_length = length;
	field->fld_dtype = dtype;
	field->fld_symbol = symbol =
		MSC_symbol(SYM_field, name, strlen(name), (GPRE_CTX)field);
	if (insert_flag)
		HSH_insert(symbol);

	return field;
}


/*____________________________________________________________
 *  
 *		Make an index symbol.
 */  

IND MET_make_index(SCHAR * name)
{
	IND index;

	index = (IND) ALLOC(IND_LEN);
	index->ind_symbol = MSC_symbol(SYM_index, name, strlen(name), (GPRE_CTX)index);

	return index;
}


/*____________________________________________________________
 *  
 *		Make an relation symbol.
 */  

GPRE_REL MET_make_relation(SCHAR * name)
{
	GPRE_REL relation;

	relation = (GPRE_REL) ALLOC(REL_LEN);
	relation->rel_symbol =
		MSC_symbol(SYM_relation, name, strlen(name), (GPRE_CTX)relation);

	return relation;
}


/*____________________________________________________________
 *  
 *		Lookup a type name for a field.
 */  

BOOLEAN MET_type(GPRE_FLD field, TEXT * string, SSHORT * ptr)
{
	GPRE_REL relation;
	DBB dbb;
	SYM symbol;
	TYP type;
	UCHAR buffer[32];			/* BASED ON RDB$TYPES.RDB$TYPE_NAME */
	UCHAR *p;

	for (symbol = HSH_lookup(string); symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_type &&
			(type = (TYP) symbol->sym_object) &&
			(!type->typ_field || type->typ_field == field)) {
			*ptr = type->typ_value;
			return TRUE;
		}

	assert(0);
	return FALSE;
}


/*____________________________________________________________
 *  
 *		Lookup an index for a database.
 *  
 *  Return: TRUE if the trigger exists
 *		   FALSE otherwise
 */  

BOOLEAN MET_trigger_exists(DBB dbb, TEXT * trigger_name)
{
	SCHAR name[NAME_SIZE];
	SSHORT length;

	strcpy(name, trigger_name);

	assert(0);
	return FALSE;
}


/*____________________________________________________________
 *  
 *		Compute and return the size of the array.
 */  

static SLONG array_size( GPRE_FLD field)
{
	ARY array_block;
	DIM dimension;
	SLONG count;

	array_block = field->fld_array_info;
	count = field->fld_array->fld_length;
	for (dimension = array_block->ary_dimension; dimension;
		 dimension = dimension->dim_next) count =
			count * (dimension->dim_upper - dimension->dim_lower + 1);

	return count;
}


/*____________________________________________________________
 *  
 *		See if field is array.
 */  

static void get_array( DBB dbb, TEXT * field_name, GPRE_FLD field)
{
	GPRE_FLD sub_field;
	ARY array_block;
	DIM dimension_block, last_dimension_block;

	assert(0);
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
 *		1 if no errors (and *id is set).
 *		0 if the name could not be resolved.
 */  

static int get_intl_char_subtype(
								 SSHORT * id,
								 UCHAR * name, USHORT length, DBB dbb)
{
	UCHAR buffer[32];			/* BASED ON RDB$COLLATION_NAME */
	UCHAR *p;
	UCHAR *period = NULL;
	UCHAR *end_name;
	int found_it = 0;

	assert(id != NULL);
	assert(name != NULL);
	assert(dbb != NULL);

	assert(0);
	return (0);
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
 *		1 if no errors (and *id is set).
 *		0 if either name not found.
 *		  or if names found, but the collation isn't for the specified
 *		  character set.
 */  

static int resolve_charset_and_collation(
										 SSHORT * id,
										 UCHAR * charset, UCHAR * collation)
{
	int found = 0;
	int *request = NULL;

	assert(id != NULL);

	assert(0);
	return (0);
}


/*____________________________________________________________
 *  
 *		Compute significant length of symbol.
 */  

static int symbol_length( TEXT * string)
{
	TEXT *p;
	int len;

	len = strlen(string);

	p = string + (len - 1);

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

static int upcase( TEXT * from, TEXT * to)
{
	TEXT *p, *end, c;

	p = to;
	end = to + NAME_SIZE;

	while (p < end && (c = *from++)) {
		*p++ = UPPER(c);
#ifdef JPN_SJIS
		/* Do not upcase second byte of a sjis kanji character */

		if (SJIS1(c) && p < end && (c = *from++))
			*p++ = c;
#endif
	}

	*p = 0;

	return p - to;
}

STATUS API_ROUTINE isc_print_blr(SCHAR * blr,
          void (*callback) (), void *callback_argument, SSHORT language)
{
        return gds__print_blr((UCHAR *) blr, callback,
                                                  (SCHAR *) callback_argument, language);
}

extern "C" void DLL_EXPORT          CVT_move (struct dsc * a, struct dsc * b, FPTR_VOID c)
{  
    assert(0);
    /* Not available in boot_gpre */
}

extern "C" {
void DLL_EXPORT ERR_bugcheck(int number)
{
}
BOOLEAN DLL_EXPORT ERR_post(STATUS status, ...)
{
	return TRUE;
}

}
