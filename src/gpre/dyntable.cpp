//____________________________________________________________
//  
//		PROGRAM:	BLR Pretty Printer
//		MODULE:		dyntable.cpp
//		DESCRIPTION:	Dynamic DDL to internal conversion table generator
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
//	$Id: dyntable.cpp,v 1.6 2003-11-08 16:31:40 brodsom Exp $
//

#include "firebird.h"
#include "../jrd/ib_stdio.h"
#include "../jrd/y_ref.h"
#include "../jrd/ibase.h"

#define NODE(dyn) dyn, "dyn",

struct dyn {
	SSHORT dyn_value;
	SCHAR *dyn_string;
} dyn_table[] = {
	NODE(gds__dyn_begin)
		NODE(gds__dyn_end)
		NODE(gds__dyn_if)
		NODE(gds__dyn_def_database)
		NODE(gds__dyn_def_global_fld)
		NODE(gds__dyn_def_local_fld)
		NODE(gds__dyn_def_idx)
		NODE(gds__dyn_def_rel)
		NODE(gds__dyn_def_view)
		NODE(gds__dyn_def_sql_fld)
		NODE(gds__dyn_def_trigger)
		NODE(gds__dyn_mod_rel)
		NODE(gds__dyn_mod_global_fld)
		NODE(gds__dyn_mod_local_fld)
		NODE(gds__dyn_mod_view)
		NODE(gds__dyn_delete_database)
		NODE(gds__dyn_delete_rel)
		NODE(gds__dyn_delete_global_fld)
		NODE(gds__dyn_delete_local_fld)
		NODE(gds__dyn_delete_idx)
		NODE(gds__dyn_rel_defined)
		NODE(gds__dyn_global_fld_defined)
		NODE(gds__dyn_local_fld_defined)
		NODE(gds__dyn_index_defined)
		NODE(gds__dyn_not)
		NODE(gds__dyn_and)
		NODE(gds__dyn_or)
		NODE(gds__dyn_rel_name)
		NODE(gds__dyn_fld_name)
		NODE(gds__dyn_idx_name)
		NODE(gds__dyn_description)
		NODE(gds__dyn_security_class)
		NODE(gds__dyn_system_flag)
		NODE(gds__dyn_update_flag)

		NODE(gds__dyn_view_blr)
		NODE(gds__dyn_view_source)
		NODE(gds__dyn_view_relation)
		NODE(gds__dyn_view_context)
		NODE(gds__dyn_view_context_name)

		NODE(gds__dyn_rel_dbkey_length)
		NODE(gds__dyn_rel_store_trig)
		NODE(gds__dyn_rel_modify_trig)
		NODE(gds__dyn_rel_erase_trig)
		NODE(gds__dyn_rel_store_trig_source)
		NODE(gds__dyn_rel_modify_trig_source)
		NODE(gds__dyn_rel_erase_trig_source)
		NODE(gds__dyn_rel_sql_protection)
		NODE(gds__dyn_fld_type)
		NODE(gds__dyn_fld_length)
		NODE(gds__dyn_fld_scale)
		NODE(gds__dyn_fld_sub_type)
		NODE(gds__dyn_fld_segment_length)
		NODE(gds__dyn_fld_query_header)
		NODE(gds__dyn_fld_edit_string)
		NODE(gds__dyn_fld_validation_blr)
		NODE(gds__dyn_fld_validate_source)
		NODE(gds__dyn_fld_computed_blr)
		NODE(gds__dyn_fld_computed_source)
		NODE(gds__dyn_fld_missing_value)
		NODE(gds__dyn_fld_default_value)
		NODE(gds__dyn_fld_query_name)
		NODE(gds__dyn_fld_source)
		NODE(gds__dyn_fld_base_fld)
		NODE(gds__dyn_fld_position)
		NODE(gds__dyn_fld_update_flag)
		NODE(gds__dyn_idx_unique)
		NODE(gds__dyn_idx_active)
		NODE(gds__dyn_trg_type)
		NODE(gds__dyn_trg_blr)
		NODE(gds__dyn_trg_source)
0, 0};
int *table[256];


//____________________________________________________________
//  
//		Spit out a conversion table.
//  

main()
{
	dyn* item;
	int max, *stuff, dyn;
	SCHAR *table[256];

	for (max = 0; max < 256; max++)
		table[max] = NULL;

	max = 0;
	for (item = dyn_table; item->dyn_string; item++) {
		if (table[item->dyn_value])
			ib_fprintf(ib_stderr, "%s (%d) is duplicate\n",
					   item->dyn_string, item->dyn_value);
		table[item->dyn_value] = item->dyn_string;
		if (item->dyn_value > max)
			max = item->dyn_value;
	}

	for (dyn = 0; dyn <= max; dyn++)
		if (table[dyn])
			ib_printf("    \"%s\",\n", table[dyn]);
		else
			ib_printf("    NULL,\n");
}
