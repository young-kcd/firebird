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
#include "../qli/form_proto.h"


FRM FORM_default_form(DBB database, TEXT * string)
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

	return NULL;
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
	int *transaction, *handle, *fields, *label;

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
	FRM form;
	int *blob, *item;
	TEXT buffer[4096], *p, *data;
	STATUS status_vector[ISC_STATUS_LENGTH];
	USHORT length, n;

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

	return NULL;
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
	STATUS status[ISC_STATUS_LENGTH];
	FRM form;
	SYM symbol;
	USHORT l, width, height;

	return NULL;
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
	STATUS status_vector[ISC_STATUS_LENGTH];

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

}
