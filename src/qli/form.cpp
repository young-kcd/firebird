/*
 *	PROGRAM:	JRD Command Oriented Query Language
 *	MODULE:		form.c
 *	DESCRIPTION:	Form interface
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

#include "firebird.h"
#include "../qli/dtr.h"
#include "../qli/exe.h"
#include "../qli/form.h"
#include "../jrd/gds.h"
#include "../pyxis/phase1.h"
#include "../pyxis/pyxis_proto.h"
#include "../pyxis/phase2.h"
#include "../pyxis/phase3.h"
#include "../pyxis/save_proto.h"
#include "../qli/all_proto.h"
#include "../qli/err_proto.h"
#include "../qli/eval_proto.h"
#include "../qli/exe_proto.h"
#include "../qli/form_proto.h"
#include "../qli/hsh_proto.h"
#include "../qli/proc_proto.h"
#include "../qli/mov_proto.h"

#include <stdio.h>

extern "C" int PYXIS_edit(WIN , OBJ , int *, int *);

extern TEXT *QLI_error;
extern USHORT QLI_prompt_count, QLI_reprompt;
extern USHORT sw_forms;

static void clear_fields(FRM);
static FRM register_form(DBB, int *, TEXT *);
static void set_form(OBJ);
static void update_form(FRM, TEXT *);

static int *pyxis_window;
static int *current_form;


FRM FORM_default_form( DBB database, TEXT * string)
{
/**************************************
 *
 *	F O R M _ d e f a u l t _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Generate a default for for a relation.
 *
 **************************************/
	int *handle, *fields, *transaction;
	FRM form;

	transaction = (int*) PRO_transaction(database, FALSE);

	if (!
		(fields = (int*)
		 PYXIS_relation_fields((SLONG**) &database->dbb_handle, (SLONG**) &transaction,
							   string))) return NULL;

	handle = (int*) PYXIS_relation_form((WIN) pyxis_window, (OBJ) fields);
	pyxis__delete((OBJ*) &fields);

	if (!handle)
		return NULL;

	return register_form(database, handle, string);
}


void FORM_display( NOD node)
{
/**************************************
 *
 *	F O R M _ d i s p l a y
 *
 **************************************
 *
 * Functional description
 *	Given a FORM_FOR node, display an empty form.
 *
 **************************************/
	FRM form;
	FFL field;

/* Initialize all fields to known state */

	sw_forms = TRUE;
	form = (FRM) node->nod_arg[e_ffr_form];

	for (field = form->frm_fields; field; field = field->ffl_next)
		PYXIS_set_field_options((OBJ) field->ffl_handle, 0, PYXIS_reset,
								PYXIS_clear);

	EXEC_execute(node->nod_arg[e_ffr_statement]);
}


void FORM_edit( DBB database, TEXT * name)
{
/**************************************
 *
 *	F O R M _ e d i t
 *
 **************************************
 *
 * Functional description
 *	Edit a form.
 *
 **************************************/
	FRM form;
	OBJ transaction, handle, fields, label;

	sw_forms = TRUE;
	transaction = (OBJ) PRO_transaction(database, FALSE);

/* Get an existing form, make a form out of a relation name, or
   build one from scratch */

	if (form = FORM_lookup_form(database, name))
		handle = (OBJ) form->frm_handle;
	else if (fields =
			 PYXIS_relation_fields((SLONG**) &database->dbb_handle, (SLONG**) &transaction, name)) {
		handle = PYXIS_relation_form((WIN) pyxis_window, (OBJ) fields);
		pyxis__delete((OBJ*) &fields);
	}
	else {
		handle = PYXIS_create_object(0, (enum att_n)0);
		label = PYXIS_create_object(name, att_literal_string);
		PYXIS_put_attribute(handle, att_label, attype_object, label);
	}

	PYXIS_set_field_options(handle, "all", PYXIS_reset, PYXIS_clear);
	set_form(handle);

	if (PYXIS_edit((WIN) pyxis_window, handle, (int*) &database->dbb_handle, (int*) &transaction)) {
		PYXIS_store_form((SLONG**) &database->dbb_handle, (SLONG**) &transaction, name, handle);
		PRO_commit(database);
	}
}


void FORM_fini(void)
{
/**************************************
 *
 *	F O R M _ f i n i
 *
 **************************************
 *
 * Functional description
 *	Shut down forms interface.
 *
 **************************************/

	if (pyxis_window)
		pyxis__delete_window((SLONG*) &pyxis_window);
}


void FORM_finish( DBB database)
{
/**************************************
 *
 *	F O R M _ f i n i s h
 *
 **************************************
 *
 * Functional description
 *	Release forms in preparation for finishing a relation.
 *
 **************************************/
	FRM form;
	FFL field;
	SYM symbol;

	while (form = database->dbb_forms) {
		database->dbb_forms = form->frm_next;
		pyxis__delete((OBJ*)&form->frm_handle);
		if (symbol = form->frm_symbol) {
			HSH_remove(symbol);
			ALLQ_release((FRB)symbol);
		}
		while (field = form->frm_fields) {
			form->frm_fields = field->ffl_next;
			if (symbol = field->ffl_symbol)
				ALLQ_release((FRB)symbol);
			ALLQ_release((FRB)field);
		}
		ALLQ_release((FRB)form);
	}
}


int FORM_get_blob( NOD value, PAR parameter)
{
/**************************************
 *
 *	 F O R M _ g e t _ b l o b
 *
 **************************************
 *
 * Functional description
 *	Copy blob from form to database.
 *
 **************************************/
	MSG message;
	REQ request;
	DBB dbb;
	DSC *desc;
	FFL field;
	OBJ blob, item;
	TEXT bpb[20], buffer[4096], *p, *data;
	STATUS status_vector[20];
	USHORT length, n, bpb_length;

	field = (FFL) value->nod_arg[e_ffl_field];

/* Find the sending and receiving requests.  If they are the same,
   a simple assignment will suffice. */

	message = parameter->par_message;
	request = message->msg_request;
	dbb = request->req_database;

/* We've got a blob copy on our hands. */

	blob = NULL;
	desc = EVAL_parameter(parameter);

/* Format blob parameter block */

	p = bpb;
	*p++ = gds_bpb_version1;
	*p++ = gds_bpb_source_type;
	*p++ = 1;
	*p++ = 1;
	*p++ = gds_bpb_target_type;
	*p++ = 2;
	*p++ = desc->dsc_sub_type;
	*p++ = desc->dsc_sub_type >> 8;
	bpb_length = p - bpb;

	if (gds__create_blob2(status_vector,
						  GDS_REF(dbb->dbb_handle),
						  GDS_REF(dbb->dbb_transaction),
						  (void**)GDS_REF(blob),
						  (GDS__QUAD*)GDS_VAL(desc->dsc_address),
						  bpb_length,
						  bpb)) ERRQ_database_error(dbb, status_vector);

	for (n = 1;; n++) {
		if (!(item = PYXIS_find_index((OBJ)field->ffl_handle, n)))
			break;
		if (!(data = (TEXT*)PYXIS_get_attribute_value(item, att_update_data)) &&
			!(data = (TEXT*)PYXIS_get_attribute_value(item, att_data)))
			data = "";
		do {
			for (p = buffer; *data && p < &buffer[sizeof(buffer) - 2];)
				*p++ = *data++;
			if (p == buffer || (!*data && p[-1] != '\n'))
				*p++ = '\n';
			*p = 0;
			length = p - buffer;
			if (gds__put_segment(status_vector,
								 (void**)GDS_REF(blob), length, buffer)) break;
		} while (*data);
		if (status_vector[1])
			break;
	}

	if (gds__close_blob(status_vector, (void**)GDS_REF(blob)))
		ERRQ_database_error(dbb, status_vector);

	return TRUE;
}


DSC *FORM_get_field(NOD node)
{
/**************************************
 *
 *	F O R M _ g e t _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Get a value off a form.
 *
 **************************************/
	FFL field;
	FRM form;
	STR string;
	USHORT l;
	TEXT *p, *q;

	field = (FFL) node->nod_arg[e_ffl_field];
	form = field->ffl_form;

/* Do some fancy stuff for a re-prompt */

	if (QLI_reprompt) {
		clear_fields(form);
		field = (FFL) node->nod_arg[e_ffl_field];
		PYXIS_set_field_options((OBJ)field->ffl_handle, 0, PYXIS_updatable,
								PYXIS_updatable);
		p = QLI_error;
		QLI_error = NULL;
		update_form(form, p);
	}

	++QLI_prompt_count;

/* Determine if there is data and where it came from */

	switch (PYXIS_get_updated((OBJ) field->ffl_handle, NULL, &p)) {
	case PYXIS_user_data:
		break;
	case PYXIS_prog_data:
		node->nod_desc.dsc_missing = DSC_initial;
		return &node->nod_desc;
	case PYXIS_missing_data:
		node->nod_desc.dsc_missing = DSC_missing;
		node->nod_desc.dsc_length = 0;
		return &node->nod_desc;
	case PYXIS_no_field:
		return NULL;
	}

/* We got some data, so set up a descriptor */

	node->nod_desc.dsc_address = (UCHAR *) p;
	node->nod_desc.dsc_dtype = dtype_text;
	node->nod_desc.dsc_length = l = strlen(p);
	node->nod_desc.dsc_scale = 0;
	node->nod_desc.dsc_missing = FALSE;

/* Unless some de-editting is required, we're done */

	if (field->ffl_dtype < dtype_short || field->ffl_dtype > dtype_double)
		return &node->nod_desc;

/* Sigh.  We need to de-edit the field.  Start by finding somewhere to put string */

	if ((string = (STR) node->nod_arg[e_ffl_string]) &&
		l >= string->str_length) {
		ALLQ_release((FRB)string);
		string = NULL;
	}

	if (!string) {
		string = (STR) ALLOCDV(type_str, l);
		node->nod_arg[e_ffl_string] = (NOD) string;
		string->str_length = l;
	}

/* Get rid of non-essential characters */

	for (q = string->str_data; *p; p++)
		if (*p != '$' && *p != ',')
			*q++ = *p;

	node->nod_desc.dsc_address = (UCHAR *) string->str_data;
	node->nod_desc.dsc_length = q - string->str_data;

	return &node->nod_desc;
}


FRM FORM_lookup_form(DBB database, TEXT * string)
{
/**************************************
 *
 *	F O R M _ l o o k u p _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Lookup a form by name.
 *
 **************************************/
	int *handle, *transaction;
	STATUS status[20];
	FRM form;
	SYM symbol;
	USHORT l, width, height;

/* Check to see if form is already defined */

	l = strlen(string);

	for (symbol = HSH_lookup(string, l); symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_form &&
			(form = (FRM) symbol->sym_object) &&
			form->frm_database == database) return form;

/* Get set up for form lookup */

	if (!pyxis_window) {
		width = 0;
		height = 40;
		pyxis__create_window((WIN*)&pyxis_window, 0, 0, &width, &height);
		if (!pyxis_window) {
			ERRQ_print_error(459, NULL, NULL, NULL, NULL, NULL);	/* Msg459 Unable to create form window */
			return NULL;
		}
		sw_forms = TRUE;
	}

/* Make sure PYXIS$FORMS exists in the database */

	if (!(database->dbb_flags & DBB_form_init)) {
		PYXIS_define_forms_relation((SLONG**) &database->dbb_handle);
		database->dbb_flags |= DBB_form_init;
	}

	transaction = (int*) PRO_transaction(database, FALSE);
	pyxis__load_form(status, (SLONG*) &database->dbb_handle, (SLONG*) &transaction,
					 (SLONG*) &handle, 0, string);

	if (!handle)
		return NULL;

	return register_form(database, handle, string);
}


FFL FORM_lookup_field(FRM form, TEXT * string)
{
/**************************************
 *
 *	F O R M _ l o o k u p _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Lookup field name in form.  If we can't find it, return NULL.
 *
 **************************************/
	FFL field;

	for (field = form->frm_fields; field; field = field->ffl_next)
		if (!strcmp(string, field->ffl_name))
			return field;

	return NULL;
}


int FORM_put_field( NOD from, NOD node)
{
/**************************************
 *
 *	F O R M _ p u t _ f i e l d
 *
 **************************************
 *
 * Functional description
 *	Write data to a form.
 *
 **************************************/
	DSC *desc, desc2;
	FFL field;
	TEXT buffer[1024], *p;
	USHORT length, index;
	int *blob;
	STATUS status_vector[20], status;

	desc = EVAL_value(from);
	field = (FFL) node->nod_arg[e_ffl_field];

	if (desc->dsc_missing & DSC_missing)
		PYXIS_set_field_options((OBJ) field->ffl_handle, 0,
								PYXIS_data_present | PYXIS_update_present,
								PYXIS_clear);
	else if (desc->dsc_dtype != dtype_blob)
		PYXIS_put_desc((OBJ) field->ffl_handle, desc);
	else {
		blob = (int*) EXEC_open_blob(from);
		for (index = 1;; index++) {
			if ((status = gds__get_segment(status_vector,
										   (void**) GDS_REF(blob),
										   GDS_REF(length),
										   sizeof(buffer),
										   buffer)) && status != gds_segment)
				break;
			p = buffer + length;
			*p = 0;
			if (length && *--p == '\n')
				*p = 0;
			PYXIS_put_segment((OBJ) field->ffl_handle, buffer, index);
		}
		gds__close_blob(status_vector, (void**) GDS_REF(blob));
	}

	return TRUE;
}


void FORM_reset(void)
{
/**************************************
 *
 *	F O R M _ r e s e t
 *
 **************************************
 *
 * Functional description
 *	Reset after execution of a request involving forms.
 *
 **************************************/

	sw_forms = FALSE;
	pyxis__suspend_window((SLONG*) &pyxis_window);
}


void FORM_update( NOD node)
{
/**************************************
 *
 *	F O R M _ u p d a t e
 *
 **************************************
 *
 * Functional description
 *	Execute "form update".
 *
 **************************************/
	NOD list;
	FRM form;
	FFL *ptr, *end, field;
	DSC *desc;
	USHORT l;
	TEXT *prompt, temp[256], *p, *q;
	int *tag;

	form = (FRM) node->nod_arg[e_fup_form];

/* Start by marking all fields as non-updatable */

	clear_fields(form);


/* Mark given fields as updatable */

	if (list = node->nod_arg[e_fup_fields])
		for (ptr = (FFL *) list->nod_arg, end = ptr + list->nod_count;
			 ptr < end; ptr++) {
			PYXIS_set_field_options((OBJ) (*ptr)->ffl_handle, 0, PYXIS_updatable,
									PYXIS_updatable);

			/* Make sure that the current form field is reset (bug 3148) */

			PYXIS_delete_named_attribute((OBJ) (*ptr)->ffl_handle,
										 att_current_index);
		}

	sprintf(temp, "%s to continue, %s to stop",
			PYXIS_get_keyname((WIN) pyxis_window, PYXIS_KEY_ENTER),
			PYXIS_get_keyname((WIN) pyxis_window, PYXIS_KEY_PF1));
	prompt = temp;

	if (node->nod_arg[2]) {
		desc = EVAL_value(node->nod_arg[e_fup_tag]);
		if (!(desc->dsc_missing & DSC_missing)) {
			p = temp;
			if (l = MOVQ_get_string(desc, &q, (vary*) temp, sizeof(temp)))
				do
					*p++ = *q++;
				while (--l);
			*p = 0;
			prompt = temp;
		}
	}

	update_form(form, prompt);
}


static void clear_fields( FRM form)
{
/**************************************
 *
 *	c l e a r _ f i e l d s
 *
 **************************************
 *
 * Functional description
 *	Reset various fields in form.
 *
 **************************************/
	FFL field;

	for (field = form->frm_fields; field; field = field->ffl_next)
		PYXIS_set_field_options((OBJ) field->ffl_handle, 0,
								PYXIS_updatable, PYXIS_clear);
}


static FRM register_form( DBB database, int *handle, TEXT * string)
{
/**************************************
 *
 *	r e g i s t e r _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Register newly fetched or created form with QLI
 *	data structures.
 *
 **************************************/
	int *attribute, *object;
	FRM form;
	FFL field;
	SYM symbol;
	TEXT *p;
	USHORT l;

	l = strlen(string);
	PRO_transaction(database, FALSE);

/* Allocate form block with enough space to hold name.  Upcase and copy name */

	form = (FRM) ALLOCPV(type_frm, l);
	form->frm_database = database;
	form->frm_handle = handle;
	strcpy(form->frm_name, string);
	form->frm_next = database->dbb_forms;
	database->dbb_forms = form;

	form->frm_symbol = symbol = (SYM) ALLOCPV(type_sym, 0);
	symbol->sym_type = SYM_form;
	symbol->sym_string = form->frm_name;
	symbol->sym_length = l;
	symbol->sym_object = (BLK) form;
	HSH_insert(symbol);

/* Pick up field names */

	for (attribute = NULL;
		 attribute = (int*)
		 PYXIS_find_object((OBJ) form->frm_handle, (ATT) attribute, att_field, TRUE);) {
		object = (int*) PYXIS_get_value((ATT) attribute);
		if (p = (TEXT*) PYXIS_get_attribute_value((OBJ) object, att_field_name)) {
			l = strlen(p);
			field = (FFL) ALLOCPV(type_ffl, l);
			field->ffl_handle = object;
			field->ffl_form = form;
			strcpy(field->ffl_name, p);
			field->ffl_next = form->frm_fields;
			form->frm_fields = field;
			field->ffl_symbol = symbol = (SYM) ALLOCPV(type_sym, 0);
			symbol->sym_string = field->ffl_name;
			symbol->sym_length = l;
			symbol->sym_object = (BLK) field;
			field->ffl_dtype =
				(int) PYXIS_get_attribute_value((OBJ) field->ffl_handle, att_dtype);
			field->ffl_scale =
				(int) PYXIS_get_attribute_value((OBJ) field->ffl_handle, att_scale);
			field->ffl_length =
				(int) PYXIS_get_attribute_value((OBJ) field->ffl_handle,
												att_length);
		}
	}

	return form;
}


static void set_form( OBJ form)
{
/**************************************
 *
 *	s e t _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Push form, is form has not been pushed.
 *
 **************************************/

	if (form == (OBJ) current_form)
		return;

	pyxis__pop_window((WIN*) &pyxis_window);
	current_form = (int*) form;

	if (form)
		PYXIS_push_form((WIN) pyxis_window, form, TRUE);
}


static void update_form( FRM form, TEXT * string)
{
/**************************************
 *
 *	u p d a t e _ f o r m
 *
 **************************************
 *
 * Functional description
 *	Assuming fields have been set up, drive form.
 *
 **************************************/
	USHORT c;
	int *tag;

	set_form((OBJ) form->frm_handle);

/* Set up tag, if given */

	if (string) {
		tag = (int*) PYXIS_create_object(string, att_literal_string);
		PYXIS_push_tag((WIN) pyxis_window, (OBJ) tag);
	}

/* Make sure that the current form field is reset (bug 3148) */

	PYXIS_delete_named_attribute((OBJ) form->frm_handle, att_current_index);

/* Drive form until abort */

	for (;;) {
		PYXIS_update((WIN) pyxis_window, (OBJ) form->frm_handle, 0, &c);
		if (!c || c == PYXIS_KEY_ENTER || c == PYXIS_KEY_PF1)
			break;
	}

/* Cleanup tag, if appropriate */

	if (string) {
		pyxis__pop_window((WIN*) &pyxis_window);
		pyxis__delete((OBJ*) &tag);
	}

/* Clean exit on ENTER, otherwise abort */

	if (c == PYXIS_KEY_ENTER)
		return;

	FORM_reset();
	EXEC_abort();
}
