//____________________________________________________________
//  
//		PROGRAM:	Language Preprocessor
//		MODULE:		form_trn.cpp
//		DESCRIPTION:	Form manager dtype translator
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
//  $Id: form_trn.cpp,v 1.5 2003-02-28 01:08:44 brodsom Exp $
//  Revision 1.3  2000/11/29 11:11:56  fsg
//  Added some more PYXIS datatypes to form_trn.c
//  
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
//	$Id: form_trn.cpp,v 1.5 2003-02-28 01:08:44 brodsom Exp $
//
#ifdef PYXIS

#include "firebird.h"
#include "../jrd/common.h"
#include "../jrd/gds.h"

//  
//The datatypes from dsc.h 
//can't be used with 
//PYXIS forms 
//FSG 26.Nov.2000
//#include "../jrd/dsc.h"
//  
//  
#include "../gpre/form__proto.h"



//  
//I have now figured out most of the
//PYXIS datatypes 
//  
//FSG 29.Nov.2000
//  

#define ptype_null      0
#define ptype_text      1
#define ptype_cstring   2
#define ptype_varying   3
#define ptype_short     4
#define ptype_long      5
#define ptype_quad      6
#define ptype_real      7
#define ptype_double    8
#define ptype_timestamp 9
#define ptype_blob      10


//  FIX ME the following are  
//         from dsc.h and are
//         probably wrong
//         but I don't believe
//         that these are used by
//         PYXIS
//  FSG 29.Nov.2000        
//  
#define ptype_d_float   13
#define ptype_sql_date  14
#define ptype_sql_time  15
#define ptype_array     18
#define ptype_int64     19




extern USHORT MET_get_dtype(USHORT, USHORT, USHORT *);


//____________________________________________________________
//  
//		Takes a pyxis dtype and transforms
//		it into a standard InterBase dtype.
//  

USHORT FORM_TRN_dtype( USHORT pyxis_dtype)
{
	USHORT gpre_dtype, length;

	switch (pyxis_dtype) {
	case ptype_text:
		gpre_dtype = MET_get_dtype(blr_text, 0, &length);
		break;

	case ptype_cstring:
		gpre_dtype = MET_get_dtype(blr_cstring, 0, &length);
		break;

	case ptype_varying:
		gpre_dtype = MET_get_dtype(blr_varying, 0, &length);
		break;

	case ptype_short:
		gpre_dtype = MET_get_dtype(blr_short, 0, &length);
		break;

	case ptype_long:
		gpre_dtype = MET_get_dtype(blr_long, 0, &length);
		break;

	case ptype_quad:
		gpre_dtype = MET_get_dtype(blr_quad, 0, &length);
		break;

	case ptype_real:
		gpre_dtype = MET_get_dtype(blr_float, 0, &length);
		break;

	case ptype_double:
		gpre_dtype = MET_get_dtype(blr_double, 0, &length);
		break;

		/* dtype_sql_date & dtype_sql_time not supported in pyxis */
	case ptype_sql_date:
	case ptype_sql_time:
	case ptype_int64:
		/* Arrays aren't supported afaik
		   FSG 29.Nov.2000 */
	case ptype_array:
		gpre_dtype = ptype_null;
		break;

	case ptype_timestamp:
		gpre_dtype = MET_get_dtype(blr_timestamp, 0, &length);
		break;

	case ptype_blob:
		gpre_dtype = MET_get_dtype(blr_blob, 0, &length);
		break;

	case ptype_d_float:
		gpre_dtype = MET_get_dtype(blr_d_float, 0, &length);
		break;

	}

	return gpre_dtype;
}

#endif
