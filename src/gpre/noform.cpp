//____________________________________________________________
//  
//		PROGRAM:	Language Preprocessor
//		MODULE:		noform.cpp
//		DESCRIPTION:	Porting module for no forms support
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
//  
//
//____________________________________________________________
//
//	$Id: noform.cpp,v 1.7 2003-09-05 14:55:59 brodsom Exp $
//

#ifdef PYXIS

#include "firebird.h"
#include "../gpre/gpre.h"
#include "../gpre/form.h"
#include "../gpre/form_proto.h"

typedef int *HANDLE;


//____________________________________________________________
//  
//		Lookup field in form.
//  

GPRE_FLD FORM_lookup_field(FORM form,
						   HANDLE object,
						   char *string)
{

	return 0;
}


//____________________________________________________________
//  
//		Lookup form.  This may be an instance already in
//		use.
//  

FORM FORM_lookup_form(DBB db,
					  UCHAR * string)
{

	return 0;
}


//____________________________________________________________
//  
//		Lookup sub-form of a given form.
//  

FORM FORM_lookup_subform(FORM parent,
						 GPRE_FLD field)
{

	return NULL;
}

#endif
