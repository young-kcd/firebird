/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		save.e
 *	DESCRIPTION:	Form dump/load code.
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

#include "../jrd/gds.h"
#include "../pyxis/pyxis.h"

#define HANDLES		10
#define PYXIS_RELATION	"PYXIS$FORMS"
#define BUF_LEN       256

extern PIC PICSTR_analyze();
extern OBJ PYXIS_create_object(), PYXIS_clone(), PYXIS_get_attribute_value();
extern ATT PYXIS_get_attribute(), PYXIS_put_attribute(), PYXIS_find_object();

DATABASE DB = STATIC "yachts.lnk";

static SCHAR ddl[] = {
#include "../pyxis/form_ddl.h"
	, 0
};

static ATT_N special_attributes[] = {
	att_display_x,
	att_display_y,
	/*
	   att_scroll_x,
	   att_scroll_y,
	 */
	att_width,
	att_height,
	att_border,
	att_index,
	att_inactive,
	att_reverse_video,
	att_bold,
	att_underline,
	att_box,
	att_blank,
	att_insert_right,
	att_updatable_flag,
	att_wakeup_flag,
	att_reverse_for_update,
	att_any
};



PYXIS_define_forms_relation(dbb)
	 SLONG **dbb;
{
/**************************************
 *
 *	P Y X I S _ d e f i n e _ f o r m s _ r e l a t i o n
 *
 **************************************
 *
 * Functional description
 *	Check for existence of PYXIS$FORMS relation.  If it doesn't
 *	exist, create it.
 *
 **************************************/
	SLONG *transaction, *handle, success;
	STATUS status_vector[20];

	DB = *dbb;
	transaction = handle = NULL;
	success = FALSE;

	START_TRANSACTION transaction READ_WRITE;

	FOR(REQUEST_HANDLE handle TRANSACTION_HANDLE transaction)
		X IN RDB$RELATIONS WITH X.RDB$RELATION_NAME EQ "PYXIS$FORMS"
			success = TRUE;
	END_FOR;

	gds__release_request(status_vector, GDS_REF(handle));

	if (!success && gds__ddl(status_vector,
							 GDS_REF(DB),
							 GDS_REF(transaction), sizeof(ddl), ddl)) {
		gds__print_status(status_vector);
		success = FALSE;
		ROLLBACK transaction;
		return FALSE;
	}

	COMMIT transaction;

	return success;
}


OBJ PYXIS_delete_form(dbb, transaction, form_name)
	 SLONG **dbb, **transaction;
	 TEXT *form_name;
{
/**************************************
 *
 *	P Y X I S _ d e l e t e _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Delete a form from a database.
 *
 **************************************/
	int *handle;

	DB = *dbb;
	gds__trans = *transaction;
	handle = NULL;

	FOR(REQUEST_HANDLE handle)
		X IN PYXIS$FORMS WITH X.PYXIS$FORM_NAME EQ form_name 
        ERASE X;
	END_FOR;

	gds__release_request(gds__status, GDS_REF(handle));

	return NULL;
}


PYXIS_dump(object, ptr)
	 OBJ object;
	 TEXT **ptr;
{
/**************************************
 *
 *	P Y X I S _ d u m p
 *
 **************************************
 *
 * Functional description
 *	Dump an object into linear form.  Assume space is large
 *	enough.
 *
 **************************************/
	ATT attribute;
	ATT_N *attr_ptr, name;
	SLONG value;
	TEXT *p, c;

	p = *ptr;

/* Start by handling ordinary attributes */

	for (attribute = object->obj_attributes; attribute;
		 attribute = attribute->att_next) {
		if (attribute->att_type == attype_other)
			continue;
		sprintf(p, "%d", attribute->att_name);
		while (*p)
			p++;
		switch (attribute->att_type) {
		case attype_string:
			c = (attribute == object->obj_display_attribute) ? 'D' : 'S';
			sprintf(p, "%c`%s`", c, attribute->att_value);
			while (*p)
				p++;
			break;

		case attype_numeric:
			sprintf(p, "N%d.", attribute->att_value);
			while (*p)
				p++;
			break;

		case attype_object:
			*p++ = 'O';
			PYXIS_dump(attribute->att_value, &p);
			break;

		default:
			BUGCHECK("DUMP:Unknown attribute type");
		}
	}

/* Next, handle special attributes */

	for (attr_ptr = special_attributes; *attr_ptr != att_any; attr_ptr++)
		if (value = GET_VALUE(object, *attr_ptr)) {
			sprintf(p, "%dN%d.", *attr_ptr, value);
			while (*p)
				p++;
		}

/* Finish up */

	*p++ = 'E';
	*ptr = p;
}


PYXIS_dump_length(object)
	 OBJ object;
{
/**************************************
 *
 *	P Y X I S _ d u m p _ l e n g t h
 *
 **************************************
 *
 * Functional description
 *	Compute length of object when dumped.
 *
 **************************************/
	ATT attribute;
	ATT_N *attr_ptr;
	TEXT buffer[32];
	USHORT n;
	SLONG length;
	SLONG value;

	length = 1;

	for (attribute = object->obj_attributes; attribute;
		 attribute = attribute->att_next) {
		if (attribute->att_type == attype_other)
			continue;
		sprintf(buffer, "%d", attribute->att_name);
		length += strlen(buffer);
		switch (attribute->att_type) {
		case attype_string:
			length += strlen(attribute->att_value) + 3;
			break;

		case attype_numeric:
			sprintf(buffer, "N%d.", attribute->att_value);
			length += strlen(buffer);
			break;

		case attype_object:
			length += 1 + PYXIS_dump_length(attribute->att_value);
			break;

		default:
			BUGCHECK("Unknown attribute type");
		}
	}

/* Next, handle special attributes */

	for (attr_ptr = special_attributes; *attr_ptr != att_any; attr_ptr++)
		if (value = GET_VALUE(object, *attr_ptr)) {
			sprintf(buffer, "%dN%d.", *attr_ptr, value);
			length += strlen(buffer);
		}

	return length;
}


PYXIS_field_defaults(field)
	 OBJ field;
{
/**************************************
 *
 *	P Y X I S _ f i e l d _ d e f a u l t s
 *
 **************************************
 *
 * Functional description
 *	Supply defaults for missing field characteristics.
 *
 **************************************/
	ATT attribute;
	OBJ prototype;
	USHORT length, display_length;
	TEXT *p, buffer[256], c, *edit_string;
	PIC picture;
	DSC desc;
	STR string;

	PYXIS_delete_named_attribute(field, att_inactive);
	picture = NULL;
	desc.dsc_length = desc.dsc_dtype = desc.dsc_scale = 0;

/* If we have a picture string, analyze it */

	if (edit_string = GET_STRING(field, att_edit_string)) {
		picture = PICSTR_analyze(edit_string, NULL);
		switch (picture->pic_type) {
		case pic_alpha:
		case pic_text:
			desc.dsc_dtype = dtype_text;
			desc.dsc_length = picture->pic_length;
			break;

		case pic_numeric:
			desc.dsc_dtype = dtype_long;
			desc.dsc_scale = -picture->pic_fractions;
			break;

		case pic_date:
			desc.dsc_dtype = dtype_text;
			break;

		case pic_float:
			c = 'F';
			desc.dsc_dtype = dtype_double;	/* No path to this stmt. */
			break;
		}
	}

/* Pick up data type info, if supplied directly */

	if (GET_ATTRIBUTE(field, att_dtype))
		desc.dsc_dtype = GET_VALUE(field, att_dtype);

	if (GET_ATTRIBUTE(field, att_length))
		desc.dsc_length = GET_VALUE(field, att_length);

	if (GET_ATTRIBUTE(field, att_scale))
		desc.dsc_scale = GET_VALUE(field, att_scale);

/* Cleanup datatype info, as appropriate */

	switch (desc.dsc_dtype) {
	case dtype_varying:
/*
	desc.dsc_length += 2;
*/
		break;

	case dtype_short:
		desc.dsc_length = 2;
		break;

	case dtype_long:
		desc.dsc_length = 4;
		break;

	case dtype_real:
		desc.dsc_length = 4;
		break;

	case dtype_double:
#ifdef VMS
	case dtype_d_float:
#endif
		desc.dsc_length = 8;
		break;

	case dtype_timestamp:
		desc.dsc_length = 8;
		break;

	case dtype_sql_date:
		desc.dsc_length = 4;
		break;

	case dtype_sql_time:
		desc.dsc_length = 4;
		break;

	case dtype_blob:
		if (!(display_length = GET_VALUE(field, att_segment_length)))
			display_length = 40;
		REPLACE_ATTRIBUTE(field, att_width, attype_numeric,
						  display_length + 2);
		REPLACE_ATTRIBUTE(field, att_height, attype_numeric, 6);
		if (!GET_VALUE(field, att_prototype)) {
			PYXIS_box(field);
			prototype = PYXIS_create_object(NULL_PTR, 0);
			REPLACE_ATTRIBUTE(prototype, att_width, attype_numeric,
							  display_length);
			REPLACE_ATTRIBUTE(prototype, att_reverse_for_update,
							  attype_numeric, TRUE);
			REPLACE_ATTRIBUTE(field, att_prototype, attype_object, prototype);
		}
		return TRUE;

	case dtype_text:
	default:
		desc.dsc_dtype = dtype_text;
		desc.dsc_scale = 0;
		if (!desc.dsc_length &&
			!(desc.dsc_length = GET_VALUE(field, att_width)))
				desc.dsc_length = 10;
		break;
	}

	REPLACE_ATTRIBUTE(field, att_dtype, attype_numeric, desc.dsc_dtype);
	REPLACE_ATTRIBUTE(field, att_length, attype_numeric, desc.dsc_length);
	if (desc.dsc_scale)
		REPLACE_ATTRIBUTE(field, att_scale, attype_numeric, desc.dsc_scale);

	if (!edit_string) {
		edit_string = (TEXT *) PICSTR_default_edit_string(&desc, buffer);
		REPLACE_ATTRIBUTE(field, att_edit_string, attype_string, edit_string);
		picture = PICSTR_analyze(edit_string, NULL);
	}

	REPLACE_ATTRIBUTE(field, att_width, attype_numeric, picture->pic_length);

	if (!(attribute = GET_ATTRIBUTE(field, att_fill_string))) {
		switch (picture->pic_type) {
		case pic_alpha:
			c = 'X';
			break;
		case pic_numeric:
			c = '9';
			break;
		case pic_date:
			c = 'D';
			break;
		case pic_float:
			c = 'F';
			break;
		case pic_text:
			c = 'T';
			break;
		}

		/* Make up fill string, possibly laboriously */

		display_length = picture->pic_length;
		string = NULL;

		if (display_length < 255)
			p = buffer;
		else {
			string = (STR) ALLOCDV(type_str, display_length);
			p = string->str_data;
		}

		do
			*p++ = c;
		while (--display_length);
		*p = 0;

		if (string) {
			attribute =
				PUT_ATTRIBUTE(field, att_fill_string, attype_string,
							  string->str_data);
			PYXIS_release(string);
		}
		else
			attribute =
				PUT_ATTRIBUTE(field, att_fill_string, attype_string, buffer);
	}

	PYXIS_set_display_attribute(field, attribute);

	if (picture->pic_type == pic_numeric || picture->pic_type == pic_float)
		REPLACE_ATTRIBUTE(field, att_insert_right, attype_numeric, TRUE);

	return TRUE;
}


OBJ PYXIS_forms(dbb, transaction)
	 SLONG **dbb, **transaction;
{
/**************************************
 *
 *	P Y X I S _ f o r m s
 *
 **************************************
 *
 * Functional description
 *	Get menu all of forms, etc.
 *
 **************************************/
	OBJ menu, entree;
	int *handle;

	DB = *dbb;
	gds__trans = *transaction;
	menu = PYXIS_create_object(NULL_PTR, 0);
	handle = NULL;

	FOR(REQUEST_HANDLE handle)
		X IN PYXIS$FORMS SORTED BY X.PYXIS$FORM_NAME
			zap_string(X.PYXIS$FORM_NAME);
		PYXIS_create_entree(menu, X.PYXIS$FORM_NAME, 0, NULL_PTR);
	END_FOR;

	gds__release_request(gds__status, GDS_REF(handle));

	return menu;
}


OBJ PYXIS_load(ptr)
	 TEXT **ptr;
{
/**************************************
 *
 *	P Y X I S _ l o a d
 *
 **************************************
 *
 * Functional description
 *	Load an object from linear representation.
 *
 **************************************/
	OBJ object;
	ATT attribute;
	ATT_T type;
	SLONG value, name, n, negate, buf_size;
	TEXT *p, c, verb, *s, *string, *new_buf, *buf_end, buffer[1024];

	p = *ptr;
	object = PYXIS_create_object(NULL_PTR, 0);

	for (;;) {
		name = 0;
		while (*p >= '0' && *p <= '9')
			name = name * 10 + *p++ - '0';
		switch (verb = *p++) {
		case 'D':
		case 'S':
			c = *p++;
			string = buffer;
			buf_size = sizeof(buffer);
			buf_end = string + buf_size;
			s = string;
			while (*p != c) {
				*s++ = *p++;
				if (s == buf_end) {
					new_buf = (TEXT *) gds__alloc(2 * buf_size);
					memcpy(new_buf, string, buf_size);
					if (string != buffer)
						gds__free(string);
					string = new_buf;
					s = string + buf_size;
					buf_size = 2 * buf_size;
					buf_end = string + buf_size;
				}
			}
			*s = 0;
			attribute = PUT_ATTRIBUTE(object, name, attype_string, string);
			if (verb == 'D')
				PYXIS_set_display_attribute(object, attribute);
			p++;
			if (string != buffer)
				gds__free(string);
			break;

		case 'N':
			type = attype_numeric;
			negate = ((*p == '-') ? (++p, TRUE) : (FALSE));
			n = 0;
			while ((c = *p++) != '.')
				n = n * 10 + c - '0';
			if (negate)
				n = -n;
			PUT_ATTRIBUTE(object, name, attype_numeric, n);
			break;

		case 'O':
			PUT_ATTRIBUTE(object, name, attype_object, PYXIS_load(&p));
			break;

		case 'E':
			*ptr = p;
			return object;

		default:
			BUGCHECK("Bad form definition");
		}
	}
}


OBJ PYXIS_load_form(status_vector, dbb, transaction,
					form_handle, form_name_length, form_name)
	 int *status_vector;
	 SLONG **dbb, **transaction;
	 OBJ *form_handle;
	 USHORT *form_name_length;
	 TEXT *form_name;
{
/**************************************
 *
 *	P Y X I S _ l o a d _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Load a form from a database.  
 *	If the form doesn't exist, return NULL.
 *
 **************************************/
	USHORT buf_len, out_size, length;
	TEXT *ptr, buffer[4096], *buff, *p;
	STATUS code;
	SLONG temp_vector[20], handle, seg_count, max_seg;
	SLONG size;

	DB = *dbb;
	gds__trans = *transaction;
	*form_handle = NULL;
	handle = NULL;
	ptr = NULL;

/* Unless name is zero terminated, copy and terminate form name */

	if (form_name_length && (length = *form_name_length)) {
		for (p = buffer; length; --length)
			*p++ = *form_name++;
		*p = 0;
		form_name = buffer;
	}

	buff = buffer;
	buf_len = sizeof(buffer);

/* Lookup form */

	FOR(REQUEST_HANDLE handle)
		X IN PYXIS$FORMS WITH X.PYXIS$FORM_NAME EQ form_name 
        OPEN_BLOB B IN X.PYXIS$FORM
		ON_ERROR
			break;
		END_ERROR;

		if (gds__blob_size(&B, &size, &seg_count, &max_seg))
			if (size > sizeof(buffer))
				buff = ptr = (TEXT *) gds__alloc(size + 2);

		while (size > 0) {
			buf_len = MIN(buf_len, size);
			if (code = gds__get_segment(gds__status,
										GDS_REF(B),
										GDS_REF(out_size),
										buf_len, GDS_VAL(buff)))
			 {

				/* segment and segstr_eof codes are benign; 
				   otherwise, we have an internal error */

				if (code == gds__segstr_eof)
					break;
				if (code != gds__segment)
					BUGCHECK("Unable to load form");
			}

			buff += out_size;
			size -= out_size;
		}
		CLOSE_BLOB B;
		if (!(buff = ptr))
			buff = buffer;
		*form_handle = PYXIS_load(&buff);
		break;
	END_FOR
	ON_ERROR
	END_ERROR;

	gds__release_request(temp_vector, GDS_REF(handle));

/* If something failed and there isn't a status vector, 
   blow away user */

	if (!status_vector && !*form_handle) {
		if (gds__status[1])
			gds__print_status(gds__status);
		else {
			sprintf(buffer, "form %s not found\n", form_name);
			gds__put_error(buffer);
		}
		exit(FINI_ERROR);
	}

	if (ptr)
		gds__free(ptr);

/* Copy status vector */

	if (status_vector)
		MOVP_fast(gds__status, status_vector, 20 * sizeof(STATUS));

	return (OBJ) gds__status[1];
}


OBJ PYXIS_relation_fields(dbb, transaction, relation_name)
	 SLONG **dbb, **transaction;
	 TEXT *relation_name;
{
/**************************************
 *
 *	P Y X I S _ r e l a t i o n _ f i e l d s
 *
 **************************************
 *
 * Functional description
 *	Create an object containing fields for relation.  This is
 *	NOT formatted as a form.
 *
 **************************************/
	ATT attribute;
	OBJ form, field;
	USHORT length, display_length, segment_length;
	TEXT *p, c, *edit_string;
	int *handle;
	DSC desc;

	DB = *dbb;
	gds__trans = *transaction;
	form = PYXIS_create_object(NULL_PTR, 0);
	handle = NULL;

	FOR(REQUEST_HANDLE handle)
		RFR IN RDB$RELATION_FIELDS CROSS FLD IN RDB$FIELDS WITH
			RFR.RDB$RELATION_NAME EQ relation_name AND
			RFR.RDB$FIELD_SOURCE EQ FLD.RDB$FIELD_NAME
			SORTED BY RFR.RDB$FIELD_POSITION
			/* Create field itself */
        zap_string(RFR.RDB$FIELD_NAME);
		field = PYXIS_create_object(RFR.RDB$FIELD_NAME, att_field_name);

		desc.dsc_scale = FLD.RDB$FIELD_SCALE;
		desc.dsc_length = FLD.RDB$FIELD_LENGTH;
		switch (FLD.RDB$FIELD_TYPE) {
		case blr_text:
			desc.dsc_dtype = dtype_text;
			break;

		case blr_varying:
			desc.dsc_dtype = dtype_varying;
			desc.dsc_length += 2;
			break;

		case blr_short:
			desc.dsc_dtype = dtype_short;
			desc.dsc_length = 2;
			break;

		case blr_long:
			desc.dsc_dtype = dtype_long;
			desc.dsc_length = 4;
			break;

		case blr_float:
			desc.dsc_dtype = dtype_real;
			desc.dsc_length = 4;
			break;

		case blr_double:
#ifndef VMS
		case blr_d_float:
#endif
			desc.dsc_dtype = dtype_double;
			desc.dsc_length = 8;
			break;

#ifdef VMS
		case blr_d_float:
			desc.dsc_dtype = dtype_d_float;
			desc.dsc_length = 8;
			break;
#endif

		case blr_timestamp:
			desc.dsc_dtype = dtype_timestamp;
			desc.dsc_length = 8;
			break;

		case blr_sql_date:
			desc.dsc_dtype = dtype_sql_date;
			desc.dsc_length = 4;
			break;

		case blr_sql_time:
			desc.dsc_dtype = dtype_sql_time;
			desc.dsc_length = 4;
			break;

		case blr_blob:
			desc.dsc_dtype = dtype_blob;
			desc.dsc_length = 8;
			if (segment_length = FLD.RDB$SEGMENT_LENGTH)
				PUT_ATTRIBUTE(field, att_segment_length, attype_numeric,
							  segment_length);
			break;
		}

		PYXIS_put_attribute(field, att_dtype, attype_numeric, desc.dsc_dtype);
		PYXIS_put_attribute(field, att_length, attype_numeric,
							desc.dsc_length);
		if (desc.dsc_scale)
			PYXIS_put_attribute(field, att_scale, attype_numeric,
								desc.dsc_scale);

		if (desc.dsc_dtype != dtype_blob) {
			edit_string = RFR.RDB$EDIT_STRING;
			if (!*edit_string || *edit_string == ' ')
				edit_string = FLD.RDB$EDIT_STRING;
			if (*edit_string && *edit_string != ' ')
				PUT_ATTRIBUTE(field, att_edit_string, attype_string,
							  edit_string);
		}
		PUT_ATTRIBUTE(field, att_inactive, attype_numeric, TRUE);
		PYXIS_create_entree(form, RFR.RDB$FIELD_NAME, attype_object, field);
	END_FOR;

	gds__release_request(gds__status, GDS_REF(handle));

	return form;
}



OBJ PYXIS_relation_form(window, fields)
	 WIN window;
	 OBJ fields;
{
/**************************************
 *
 *	P Y X I S _ r e l a t i o n _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Create a form from the fields of a relation.
 *
 **************************************/
	ATT attribute;
	OBJ form, field, label;
	TEXT *name;

	form = PYXIS_create_object(NULL_PTR, 0);

	for (attribute = NULL;
		 attribute = PYXIS_find_object(fields, attribute, att_entree, TRUE);)
		if (field = GET_OBJECT((OBJ) attribute->att_value, att_entree_value)) {
			field = PYXIS_clone(field);
			if (GET_VALUE(field, att_dtype) != dtype_blob &&
				GET_VALUE(field, att_datatype) != blr_blob)
				REPLACE_ATTRIBUTE(field, att_reverse_for_update,
								  attype_numeric, TRUE);
			PYXIS_field_defaults(field);
			name = GET_STRING(field, att_field_name);
			label = PYXIS_create_object(name, att_literal_string);
			PUT_ATTRIBUTE(form, att_label, attype_object, label);
			PUT_ATTRIBUTE(form, att_field, attype_object, field);
		}

	PYXIS_format_form(form, window->win_width, window->win_height);

	return form;
}


OBJ PYXIS_relations(dbb, transaction)
	 SLONG **dbb, **transaction;
{
/**************************************
 *
 *	P Y X I S _ r e l a t i o n s
 *
 **************************************
 *
 * Functional description
 *	Get menu all of relations, etc.
 *
 **************************************/
	OBJ menu, entree;
	int *handle;

	DB = *dbb;
	gds__trans = *transaction;
	menu = PYXIS_create_object(NULL_PTR, 0);
	handle = NULL;

	FOR(REQUEST_HANDLE handle)
		X IN RDB$RELATIONS WITH X.RDB$SYSTEM_FLAG ==
			0 SORTED BY X.RDB$RELATION_NAME 
        zap_string(X.RDB$RELATION_NAME);
		PYXIS_create_entree(menu, X.RDB$RELATION_NAME, 0, NULL_PTR);
	END_FOR;

	gds__release_request(gds__status, GDS_REF(handle));

	return menu;
}



PYXIS_store_form(dbb, transaction, form_name, form)
	 SLONG **dbb, **transaction;
	 TEXT *form_name;
	 OBJ form;
{
/**************************************
 *
 *	P Y X I S _ s t o r e _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Store a form in a database.  If the form already exists, replace it.
 *
 **************************************/
	SLONG size;
	TEXT *ptr, buffer[4096], *buff, *p;
	int *handle;
	USHORT seg_len;

	DB = *dbb;
	gds__trans = *transaction;
	handle = NULL;
	ptr = NULL_PTR;
	seg_len = sizeof(buffer);

/* If local buffer is big enough use it; otherwise allocate buffer big enough */

	size = PYXIS_dump_length(form);
	buff = buffer;

	if (size > sizeof(buffer))
		buff = ptr = (TEXT *) gds__alloc(size);

/* Linearize the form */

	p = buff;
	PYXIS_dump(form, &p);
	size = p - buff;

/* If form exists, modify it */

	FOR(REQUEST_HANDLE handle)
		X IN PYXIS$FORMS WITH X.PYXIS$FORM_NAME EQ form_name
		MODIFY X USING
			CREATE_BLOB B IN X.PYXIS$FORM;
			while (size > 0) {
				seg_len = MIN(seg_len, size);
				if (gds__put_segment(gds__status,
									 GDS_REF(B),
									 seg_len, GDS_VAL(buff))) break;
				buff += seg_len;
				size -= seg_len;
			}
			if (gds__status[1])
				gds__print_status(gds__status);
			CLOSE_BLOB B;
		END_MODIFY;
		if (ptr)
			gds__free(ptr);
		gds__release_request(gds__status, GDS_REF(handle));
		return SUCCESS;			/* gpre generates a gds__send call after this which
								   can never be reached; but it's ok  */
	END_FOR;

	gds__release_request(gds__status, GDS_REF(handle));

/* Form doesn't exist -- store a new record */

	STORE(REQUEST_HANDLE handle)
		X IN PYXIS$FORMS 

        strcpy(X.PYXIS$FORM_NAME, form_name);
		CREATE_BLOB B IN X.PYXIS$FORM;
		while (size > 0) {
			seg_len = MIN(seg_len, size);
			if (gds__put_segment(gds__status,
								 GDS_REF(B), seg_len, GDS_VAL(buff))) break;
			buff += seg_len;
			size -= seg_len;
		}
		if (gds__status[1])
			gds__print_status(gds__status);
		CLOSE_BLOB B;
	END_STORE;

	gds__release_request(gds__status, GDS_REF(handle));

	if (ptr)
		gds__free(ptr);
}


static zap_string(string)
	 TEXT *string;
{
/**************************************
 *
 *	z a p _ s t r i n g
 *
 **************************************
 *
 * Functional description
 *	Zap trailing blanks in a string.
 *
 **************************************/

	while (*string && *string != ' ')
		string++;

	*string = 0;
}
