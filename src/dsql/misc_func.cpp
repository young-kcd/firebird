/*
 *	PROGRAM:	Miscellaneous internal functions support (DSQL layer)
 *	MODULE:		misc_func.cpp
 *	DESCRIPTION:	DSQL helper.
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
 * Contributor(s): ______________________________________
 *
 * 2002.09.20 Dmitry Yemanov: Created all this stuff
 */

#include "firebird.h"
#include "../dsql/dsql.h"
#include "../dsql/misc_func.h"

const InternalInfo::InfoAttr InternalInfo::attr_array[max_internal_id] = {
	{"<UNKNOWN>", 0},
	{"CURRENT_CONNECTION", 0},
	{"CURRENT_TRANSACTION", 0},
	{"GDSCODE", REQ_procedure},
	{"SQLCODE", REQ_procedure},
	{"ROW_COUNT", REQ_procedure},
	{"INSERTING/UPDATING/DELETING", REQ_trigger}
};

char *InternalInfo::getAlias(internal_info_id info_id)
{
	return attr_array[info_id].alias_name;
}

USHORT InternalInfo::getMask(internal_info_id info_id)
{
	return attr_array[info_id].req_mask;
}
