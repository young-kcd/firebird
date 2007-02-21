/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		flu.h
 *	DESCRIPTION:	Function lookup definitions
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
 * 2002.10.28 Sean Leyne - Completed removal of obsolete "HP700" port
 *
 */

#ifndef _JRD_FLU_H_
#define _JRD_FLU_H_

/* RITTER - changed HP10 to HPUX in the line below */
#ifdef HPUX
#define MODULE_HANDLE
#include <dl.h>
#include <shl.h>
typedef shl_t HMOD;
#endif

#ifdef SOLARIS
#define MODULE_HANDLE
#include <dlfcn.h>
typedef void *HMOD;
#endif

#ifdef WIN_NT
#define MODULE_HANDLE
#include <windows.h>
typedef HMODULE HMOD;
#endif

#ifndef MODULE_HANDLE
#define MODULE_HANDLE
typedef void *HMOD;
#endif

/* External function module descriptor */

typedef struct mod {
	HMOD mod_handle;			/* Handle to search for entrypoints */
	struct mod *mod_next;		/* Linked list of open modules */
	SLONG mod_use_count;		/* Databases interested in module */
	USHORT mod_length;			/* Module name length */
	TEXT mod_name[1];			/* Module name */
} *MOD;

#endif /* _JRD_FLU_H_ */
