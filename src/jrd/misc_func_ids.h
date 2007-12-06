/*
 *	PROGRAM:	Miscellaneous internal functions support (JRD layer)
 *	MODULE:		misc_func_ids.h
 *	DESCRIPTION:	Constants.
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

#ifndef _JRD_MISC_FUNC_IDS_H_
#define _JRD_MISC_FUNC_IDS_H_

enum internal_info_id
{
	internal_unknown = 0,
	internal_connection_id = 1,
	internal_transaction_id = 2,
	internal_gdscode = 3,
	internal_sqlcode = 4,
	internal_rows_affected = 5,
	internal_trigger_action = 6,
	max_internal_id
};

#endif // _JRD_MISC_FUNC_IDS_H_
