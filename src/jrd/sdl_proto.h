/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sdl_proto.h
 *	DESCRIPTION:	Prototype header file for sdl.c
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

#ifndef JRD_SDL_PROTO_H
#define JRD_SDL_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

extern SLONG DLL_EXPORT SDL_compute_subscript(ISC_STATUS *, struct ads *, USHORT,
											  SLONG *);
extern ISC_STATUS API_ROUTINE SDL_info(ISC_STATUS *, UCHAR *, struct sdl_info *,
								   SLONG *);
extern UCHAR *DLL_EXPORT SDL_prepare_slice(UCHAR *, USHORT);
extern int DLL_EXPORT SDL_walk(ISC_STATUS *, UCHAR *, USHORT, UCHAR *,
							   struct ads *, SLONG *, FPTR_VOID,
							   struct slice *);

#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* JRD_SDL_PROTO_H */
