/*
 *	PROGRAM:	PYXIS Form Package
 *	MODULE:		phase2.h
 *	DESCRIPTION:	Form/field level interface definitions
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

#ifndef _PYXIS_PHASE2_H_
#define _PYXIS_PHASE2_H_

/* Attribute identification */

#define PYXIS_no_field			0       /* Field not found */
#define PYXIS_prog_data			1	/* Program supplied data */
#define PYXIS_user_data			2	/* User supplied data    */
#define PYXIS_missing_data		3	/* Neither user nor program supplied data */
#define PYXIS_any_data			3       /* OBSOLETE: left in for compatibility */

/* Options for field attribute calls */

#define PYXIS_clear			0

#define PYXIS_fill_displayed		1
#define PYXIS_data_displayed		2
#define PYXIS_update_displayed		4

#define PYXIS_data_present		8
#define PYXIS_update_present		16

#define PYXIS_repeating_items		32 

#define PYXIS_wakeup			64
#define PYXIS_updatable			256

/*
#define PYXIS_wakeup_string		128 
#define PYXIS_upcase			512
#define PYXIS_must_fill			1024
#define PYXIS_reset_scroll		2048
*/

/* Convenient attribute group mask */

#define PYXIS_reset		(-1 &~ PYXIS_fill_displayed)

#endif /* _PYXIS_PHASE2_H_ */
