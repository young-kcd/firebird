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

#include "../dsql/misc_func.h"

char* InternalInfo::alias_array[max_internal_id] = {
	"<UNKNOWN>",
	"CONNECTION_ID",
	"TRANSACTION_ID",
	"GDSCODE",
	"SQLCODE",
	"ROWS_AFFECTED"
};

char *InternalInfo::getAlias(internal_info_id info_id)
{
	return alias_array[info_id];
}
