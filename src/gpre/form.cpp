//____________________________________________________________
//  
//		PROGRAM:	Language Preprocessor
//		MODULE:		form.cpp
//		DESCRIPTION:	Form manager stuff
//  
//  The contents of this file are subject to the Interbase Public
//  License Version 1.0 (the "License"); you may not use this file
//  except in compliance with the License. You may obtain a copy
//  of the License at http://www.Inprise.com/IPL.html
//  
//  Software distributed under the License is distributed on an
//  "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
//  or implied. See the License for the specific language governing
//  rights and limitations under the License.
//  
//  The Original Code was created by Inprise Corporation
//  and its predecessors. Portions created by Inprise Corporation are
//  Copyright (C) Inprise Corporation.
//  
//  All Rights Reserved.
//  Contributor(s): ______________________________________.
//  $Id: form.cpp,v 1.6 2002-11-17 00:04:18 hippoman Exp $
//  Revision 1.2  2000/11/27 09:26:13  fsg
//  Fixed bugs in gpre to handle PYXIS forms
//  and allow edit.e and fred.e to go through
//  gpre without errors (and correct result).
//  
//  This is a partial fix until all
//  PYXIS datatypes are adjusted in frm_trn.c
//  
//  removed some compiler warnings too
//  
//  
//
//____________________________________________________________
//
//	$Id: form.cpp,v 1.6 2002-11-17 00:04:18 hippoman Exp $
//

#include "firebird.h"
#include "../gpre/gpre.h"
#include "../gpre/form.h"
#include "../pyxis/phase1.h"
#include "../gpre/form_proto.h"
#include "../gpre/form__proto.h"
#include "../gpre/hsh_proto.h"
#include "../gpre/msc_proto.h"

typedef int *HANDLE;

extern "C" HANDLE PYXIS_find_field(HANDLE , TEXT *);
extern "C" HANDLE PYXIS_get_attribute_value(HANDLE , ATT_N );
extern int pyxis__load_form(STATUS *, SLONG *, SLONG *, SLONG *, SSHORT *,
                                   SCHAR *);

extern USHORT sw_cstring;

#define GET_VALUE	PYXIS_get_attribute_value


//____________________________________________________________
//  
//		Lookup field in form.
//  

FLD FORM_lookup_field( FORM form, HANDLE object, char *string)
{
	SYM symbol;
	FLD field;
	UCHAR name[32], *p;
	HANDLE handle;
	USHORT l;

	for (p = name; (*p = UPPER(*string)); string++, p++);

//  See if we already know about field 

	for (field = form->form_fields; field; field = field->fld_next) {
		symbol = field->fld_symbol;
		if (!strcmp((char*)name, symbol->sym_string))
			return field;
	}

//  Haven't seen it before -- look it up on form 

	if (!(handle = PYXIS_find_field(object, (TEXT*) name)))
		return NULL;
//  Make field block and decrypt data type 

	field = (FLD) ALLOC(sizeof(struct fld));
	field->fld_next = form->form_fields;
	form->form_fields = field;
	field->fld_handle = handle;
	field->fld_prototype = (int*) GET_VALUE(handle, att_prototype);
	field->fld_dtype = FORM_TRN_dtype((USHORT) GET_VALUE(handle, att_dtype));
	field->fld_length = (int) GET_VALUE(handle, att_length);
	field->fld_scale = (int) GET_VALUE(handle, att_scale);

	if (field->fld_dtype == dtype_varying) {
		field->fld_dtype = dtype_text;
		field->fld_flags |= FLD_text;
		field->fld_length -= 2;
	}

	if (sw_cstring &&
		(field->fld_dtype == dtype_text ||
		 field->fld_dtype == dtype_varying)) {
		field->fld_dtype = dtype_cstring;
		++field->fld_length;
	}

//  Make up symbol block 

	l = p - name;
	field->fld_symbol = symbol = MSC_symbol(SYM_form_field, (TEXT*) name, l, 0);

	return field;
}


//____________________________________________________________
//  
//		Lookup form.  This may be an instance already in
//		use.
//  

FORM FORM_lookup_form(DBB dbb, UCHAR * string)
{
	SYM symbol;
	FORM form;
	USHORT l;
	STATUS status[ISC_STATUS_LENGTH];
	UCHAR name[32], *p;

	if (!dbb)
		return NULL;

	for (p = name; (*p = UPPER(*string)); string++, p++);

//  See if we already know about form 

	l = p - name;

	for (symbol = HSH_lookup((SCHAR*) name); symbol; symbol = symbol->sym_homonym)
		if (symbol->sym_type == SYM_form &&
			(form = (FORM) symbol->sym_object) && form->form_dbb == dbb)
			return form;

//  Load form 

	form = (FORM) ALLOC(sizeof(struct form));
	pyxis__load_form(status,
					 (SLONG*)&dbb->dbb_handle, (SLONG*)&dbb->dbb_transaction,
					 (SLONG*)&form->form_object, 0, (SCHAR*)name);

	if (!form->form_object)
		return NULL;
// *PYXIS_print(form->form_object,0);
	form->form_dbb = dbb;
	form->form_next = dbb->dbb_forms;
	dbb->dbb_forms = form;

//  Make up form block, etc 

	form->form_name = symbol = MSC_symbol(SYM_form, (TEXT*) name, l, (GPRE_CTX) form);
	HSH_insert(symbol);

	return form;
}


//____________________________________________________________
//  
//		Lookup sub-form of a given form.
//  

FORM FORM_lookup_subform(FORM parent, FLD field)
{
	FORM form;

//  Allocate form block 

	form = (FORM) ALLOC(sizeof(struct form));
	form->form_dbb = parent->form_dbb;
	form->form_field = field;
	form->form_name = field->fld_symbol;

	if (!
		(form->form_object =
		 PYXIS_get_attribute_value(field->fld_handle,
								   att_prototype))) return NULL;

	field->fld_dtype = dtype_short;
	field->fld_length = sizeof(SSHORT);
	field->fld_flags |= FLD_repeating;

	return form;
}
