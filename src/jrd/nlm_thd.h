/*
 *	PROGRAM:	JRD access method
 *	MODULE:		nlm_thd.h
 *	DESCRIPTION:	NLM Thread support definitions
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
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 *
 */

/* This include file turns certain V4 threading constructs on for NLM. */

#define NLM_INIT
#define NLM_GLOBAL_MUTEX_LOCK
#define NLM_GLOBAL_MUTEX_UNLOCK
#define NLM_MUTEX_INIT(mutx)
#define NLM_MUTEX_LOCK(mutx)
#define NLM_MUTEX_UNLOCK(mutx)
#define NLM_MUTEX_DESTROY(mutx)
#define NLM_JRD_MUTEX_LOCK(mutx)
#define NLM_JRD_MUTEX_UNLOCK(mutx)
