/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		flu_proto.h
 *	DESCRIPTION:	Prototype header file for flu.cpp, functions.cpp,
 *			builtin.cpp and qatest.cpp
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

#ifndef JRD_FLU_PROTO_H
#define JRD_FLU_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

mod*	FLU_lookup_module(TEXT*);
void	FLU_unregister_module(mod*);
// int (*ISC_lookup_entrypoint(TEXT*, TEXT*, const TEXT*, bool)) (void);
// int (*FUNCTIONS_entrypoint(TEXT*, TEXT*)) (void);
// int (*BUILTIN_entrypoint(TEXT*, TEXT*)) (void);
FPTR_INT ISC_lookup_entrypoint(TEXT*, TEXT*, const TEXT*, bool);
FPTR_INT BUILTIN_entrypoint(const TEXT*, const TEXT*);

/*
   This shouldn't be in a production server,
   If we should ever need this again, 
   this should be enabled *ONLY* for
   the debug version.
   FSG 18.Dez.2000
   
*/

/*int		       QATEST_entrypoint (ULONG *, void *, void *, void *);
*/

#ifdef __cplusplus
} /* extern "C" */
#endif

FPTR_INT FUNCTIONS_entrypoint(const char*, const char*);

#endif // JRD_FLU_PROTO_H

