//____________________________________________________________
//  
//		PROGRAM:	JRD Access Method
//		MODULE:		jrdmet.cpp
//		DESCRIPTION:	Non-database meta data for internal JRD stuff
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
//  TMN (Mike Nordell) 11.APR.2001 - Reduce compiler warnings
//  There however is still a bunch of constness errors in this file
//  
//
//____________________________________________________________
//
//	$Id: jrdmet.cpp,v 1.9 2003-09-06 00:52:10 brodsom Exp $
//

#include "firebird.h"
#include "../jrd/gds.h"
#include "../jrd/common.h"
#include "../jrd/ods.h"

#include "../gpre/gpre.h"
#include "../jrd/constants.h"
#define GPRE
#include "../jrd/ini.h"
#undef GPRE
#include "../gpre/hsh_proto.h"
#include "../gpre/jrdme_proto.h"


//____________________________________________________________
//  
//		Initialize in memory meta data.
//  

void JRDMET_init( DBB db)
{
	GPRE_REL relation;
	SYM symbol;
	GPRE_FLD field;
	TYP type;
	const RTYP *rtype;
	const UCHAR *relfld;
	const UCHAR *fld;
	int n;
	gfld* gfield;

	relfld = relfields;

	while (relfld[RFLD_R_NAME]) {
		relation = (GPRE_REL) ALLOC(REL_LEN);
		relation->rel_database = db;
		relation->rel_next = db->dbb_relations;
		relation->rel_id = relfld[RFLD_R_ID];
		db->dbb_relations = relation;
		relation->rel_symbol = symbol = (SYM) ALLOC(SYM_LEN);
		symbol->sym_type = SYM_relation;
		symbol->sym_object = (GPRE_CTX) relation;

#pragma FB_COMPILER_MESSAGE("FIXFIX! const_cast")

		symbol->sym_string = const_cast < char *>(names[relfld[RFLD_R_NAME]]);
		HSH_insert(symbol);

		for (n = 0, fld = relfld + RFLD_RPT; fld[RFLD_F_NAME];
			 n++, fld += RFLD_F_LENGTH) {
			gfield = const_cast < gfld * >((fld[RFLD_F_UPD_MINOR]) ?
										   &gfields[fld[RFLD_F_UPD_ID]] :
										   &gfields[fld[RFLD_F_ID]]);
			field = (GPRE_FLD) ALLOC(FLD_LEN);
			relation->rel_fields = field;
			field->fld_relation = relation;
			field->fld_next = relation->rel_fields;
			field->fld_id = n;
			field->fld_length = gfield->gfld_length;
			field->fld_dtype = gfield->gfld_dtype;
			field->fld_sub_type = gfield->gfld_sub_type;
			if (field->fld_dtype == dtype_varying ||
				field->fld_dtype == dtype_text) {
				field->fld_dtype = dtype_cstring;
				field->fld_flags |= FLD_text;
				++field->fld_length;
				if (gfield->gfld_sub_type == dsc_text_type_metadata) {
					field->fld_flags |= FLD_charset;
					field->fld_charset_id = CS_METADATA;
					field->fld_collate_id = COLLATE_NONE;
					field->fld_ttype = ttype_metadata;
				}
				else {
					field->fld_flags |= FLD_charset;
					field->fld_charset_id = CS_NONE;
					field->fld_collate_id = COLLATE_NONE;
					field->fld_ttype = ttype_none;
				}
			}
			else if (field->fld_dtype == dtype_blob) {
				field->fld_dtype = dtype_blob;
				field->fld_flags |= FLD_blob;
				if (gfield->gfld_sub_type == BLOB_text)
					field->fld_charset_id = CS_METADATA;
			}

			field->fld_symbol = symbol = (SYM) ALLOC(SYM_LEN);
			symbol->sym_type = SYM_field;
			symbol->sym_object = (GPRE_CTX) field;
			symbol->sym_string =
				const_cast < char *>(names[fld[RFLD_F_NAME]]);
			HSH_insert(symbol);

			field->fld_global = symbol = (SYM) ALLOC(SYM_LEN);
			symbol->sym_type = SYM_field;
			symbol->sym_object = (GPRE_CTX) field;
			symbol->sym_string =
				const_cast < char *>(names[gfield->gfld_name]);
		}
		relfld = fld + 1;
	}

	for (rtype = types; rtype->rtyp_name; rtype++) {
		type = (TYP) ALLOC(TYP_LEN);
		type->typ_symbol = symbol = (SYM) ALLOC(SYM_LEN);
		type->typ_value = rtype->rtyp_value;
		symbol->sym_type = SYM_type;
		symbol->sym_object = (GPRE_CTX) type;
		symbol->sym_string = const_cast < char *>(rtype->rtyp_name);
		HSH_insert(symbol);
	}
}

