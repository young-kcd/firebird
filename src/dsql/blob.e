/*
 *	PROGRAM:	InterBase layered support library
 *	MODULE:		blob.e
 *	DESCRIPTION:	Dynamic blob support
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

#include "../jrd/common.h"
#include <stdarg.h>
#include "../include/jrd/gds.h"
#include "../jrd/intl.h"
#include "../jrd/constants.h"
#include "../dsql/blob_proto.h"

DATABASE DB = STATIC "yachts.lnk";

static STATUS copy_status(STATUS *, STATUS *);
static STATUS error(STATUS *, SSHORT, ...);
static void get_name(UCHAR *, UCHAR *);


void API_ROUTINE isc_blob_default_desc(
									   ISC_BLOB_DESC * desc,
									   UCHAR * relation_name,
									   UCHAR * field_name)
{
/**************************************
 *
 *	i s c _ b l o b _ d e f a u l t _ d e s c
 *
 **************************************
 *
 * Functional description
 *
 *	This function will set the default
 *	values in the blob_descriptor.
 *
 **************************************/

	desc->blob_desc_subtype = BLOB_text;
	desc->blob_desc_charset = CS_dynamic;
	desc->blob_desc_segment_size = 80;
	get_name(field_name, desc->blob_desc_field_name);
	get_name(relation_name, desc->blob_desc_relation_name);
}


STATUS API_ROUTINE isc_blob_gen_bpb(
									STATUS * status,
									ISC_BLOB_DESC * to_desc,
									ISC_BLOB_DESC * from_desc,
USHORT bpb_buffer_length, UCHAR * bpb_buffer, USHORT * bpb_length)
{
/**************************************
 *
 *	i s c _ b l o b _ g e n _ b p b
 *
 **************************************
 *
 * Functional description
 *
 *  	This function will generate a bpb
 *	given a to_desc and a from_desc 
 *	which contain the subtype and
 *	character set information.
 *
 **************************************/
	UCHAR *p;

	if (bpb_buffer_length < 17)
		return error(status, 3, (STATUS) gds__random,
					 (STATUS) gds_arg_string,
					 (STATUS) "BPB buffer too small");

	p = bpb_buffer;
	*p++ = isc_bpb_version1;
	*p++ = isc_bpb_target_type;
	*p++ = 2;
	*p++ = (UCHAR)to_desc->blob_desc_subtype;
	*p++ = (UCHAR)(to_desc->blob_desc_subtype >> 8);
	*p++ = isc_bpb_source_type;
	*p++ = 2;
	*p++ = (UCHAR)from_desc->blob_desc_subtype;
	*p++ = (UCHAR)(from_desc->blob_desc_subtype >> 8);
	*p++ = isc_bpb_target_interp;
	*p++ = 2;
	*p++ = (UCHAR)to_desc->blob_desc_charset;
	*p++ = (UCHAR)(to_desc->blob_desc_charset >> 8);
	*p++ = isc_bpb_source_interp;
	*p++ = 2;
	*p++ = (UCHAR)from_desc->blob_desc_charset;
	*p++ = (UCHAR)(from_desc->blob_desc_charset >> 8);

	*bpb_length = p - bpb_buffer;

	return error(status, 1, (STATUS) SUCCESS);
}


STATUS API_ROUTINE isc_blob_lookup_desc(STATUS * status,
										void **db_handle,
										void **trans_handle,
										UCHAR * relation_name,
										UCHAR * field_name,
										ISC_BLOB_DESC * desc, UCHAR * global)
{
/***********************************************
 *
 *	i s c _ b l o b _ l o o k u p _ d e s c
 *
 ***********************************************
 *
 * Functional description
 *
 *	This routine will lookup the subtype,
 *	character set and segment size information
 *	from the metadata, given a relation name
 *	and column name.  it will fill in the information
 *	in the BLOB_DESC.	
 *
 ***********************************************/
	SSHORT flag;

	if (DB && DB != *db_handle)
		RELEASE_REQUESTS;

	DB = *db_handle;
	gds__trans = *trans_handle;
	get_name(field_name, desc->blob_desc_field_name);
	get_name(relation_name, desc->blob_desc_relation_name);

	flag = FALSE;

	FOR X IN RDB$RELATION_FIELDS CROSS Y IN RDB$FIELDS
		WITH X.RDB$FIELD_SOURCE EQ Y.RDB$FIELD_NAME AND
		X.RDB$RELATION_NAME EQ desc->blob_desc_relation_name AND
		X.RDB$FIELD_NAME EQ desc->blob_desc_field_name flag = TRUE;

	desc->blob_desc_subtype = Y.RDB$FIELD_SUB_TYPE;
	desc->blob_desc_charset = Y.RDB$CHARACTER_SET_ID;
	desc->blob_desc_segment_size = Y.RDB$SEGMENT_LENGTH;

	if (global)
		get_name((UCHAR *) Y.RDB$FIELD_NAME, global);
	END_FOR ON_ERROR return copy_status(gds__status, status);
	END_ERROR;

	if (!flag)
		return error(status, 5, (STATUS) gds__fldnotdef,
					 (STATUS) gds_arg_string,
					 (STATUS) desc->blob_desc_field_name,
					 (STATUS) gds_arg_string,
					 (STATUS) desc->blob_desc_relation_name);

	return error(status, 1, (STATUS) SUCCESS);
}


STATUS API_ROUTINE isc_blob_set_desc(STATUS * status,
									 UCHAR * relation_name,
									 UCHAR * field_name,
									 SSHORT subtype,
									 SSHORT charset,
									 SSHORT segment_size,
									 ISC_BLOB_DESC * desc)
{
/**************************************
 *
 *	i s c _ b l o b _ s e t _ d e s c
 *
 **************************************
 *
 * Functional description
 *
 *	This routine will set the subtype
 *	and character set information in the
 *	BLOB_DESC based on the information
 *	specifically passed in by the user.
 *
 **************************************/

	get_name(field_name, desc->blob_desc_field_name);
	get_name(relation_name, desc->blob_desc_relation_name);

	desc->blob_desc_subtype = subtype;
	desc->blob_desc_charset = charset;
	desc->blob_desc_segment_size = segment_size;

	return error(status, 1, (STATUS) SUCCESS);
}


static STATUS copy_status( STATUS * from, STATUS * to)
{
/**************************************
 *
 *	c o p y _ s t a t u s
 *
 **************************************
 *
 * Functional description
 *	Copy a status vector.
 *
 **************************************/
	STATUS status, *end;

	status = from[1];

	for (end = from + 20; from < end;)
		*to++ = *from++;

	return status;
}


static STATUS error( STATUS * status, SSHORT count, ...)
{
/**************************************
 *
 *	e r r o r	
 *
 **************************************
 *
 * Functional description
 *	Stuff a status vector.
 *
 **************************************/
	STATUS *stat;
	va_list ptr;

	VA_START(ptr, count);
	stat = status;
	*stat++ = gds_arg_gds;

	for (; count; --count)
		*stat++ = (STATUS) va_arg(ptr, STATUS);

	*stat = gds_arg_end;

	return status[1];
}


static void get_name( UCHAR * from, UCHAR * to)
{
/**************************************
 *
 *	g e t _ n a m e 
 *
 **************************************
 *
 * Functional description
 *	Copy null or blank terminated name.
 *
 **************************************/

	while (*from && *from != ' ')
		*to++ = *from++;

	*to = 0;
}
