/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		loiter.h
 *	DESCRIPTION:	prototypes for callback functions in 
 *			the lock manager. 
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

#ifndef _JRD_LOITER_H_
#define _JRD_LOITER_H_

#ifndef __WINDOWS_H
#include <windows.h>
#endif

BOOLEAN LOITER_loiter(void);
/*
 * future additions:
 * BOOLEAN LOITER_register_callback (BOOLEAN (*wait_callback)(void *),
 *				     void *param);
 * BOOLEAN LOITER_unregister_callback (BOOLEAN (*wait_callback)(void *),
 *			       	       void *param);
*/

#endif
