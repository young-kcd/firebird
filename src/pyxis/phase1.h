/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		phase1.h
 *	DESCRIPTION:	General header module
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

#ifndef _PYXIS_PHASE1_H_
#define _PYXIS_PHASE1_H_

/* Reset levels */

#define PYXIS_reset_update		4
#define PYXIS_reset_data		2
#define PYXIS_reset_control		1

/* Define attributes */

#define ATT(name)	name,
typedef enum att_n {
    att_any = 0,
#include "../pyxis/attributes.h"
    att_max
} ATT_N;
#undef ATT

typedef enum att_t {
    attype_numeric = 1,		/* numeric value */
    attype_string,		/* string address */
    attype_object,		/* an object */
    attype_other                /* used for att_picture */
} ATT_T;

#endif /* _PYXIS_PHASE1_H_ */
