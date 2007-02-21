/*
 *	PROGRAM:	Miscellaneous internal functions support (DSQL layer)
 *	MODULE:		misc_func.h
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

#ifndef _DSQL_MISC_FUNC_H_
#define _DSQL_MISC_FUNC_H_

#include "../jrd/misc_func_ids.h"

class InternalInfo {
private:
	struct InfoAttr {
		char *alias_name;
		unsigned short req_mask;
	};
	static const InfoAttr attr_array[max_internal_id];
public:
	static char *getAlias(internal_info_id);
	static USHORT getMask(internal_info_id);
};

#endif // _DSQL_MISC_FUNC_H_
