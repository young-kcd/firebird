/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		sdl.h
 *	DESCRIPTION:	Interface definitions for array handler
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

#ifndef JRD_SDL_H
#define JRD_SDL_H

typedef struct sdl_info {
	USHORT sdl_info_fid;
	USHORT sdl_info_rid;
	TEXT sdl_info_field[32];
	TEXT sdl_info_relation[32];
	DSC sdl_info_element;
	USHORT sdl_info_dimensions;
	SLONG sdl_info_lower[16];
	SLONG sdl_info_upper[16];
} *SDL_INFO;

typedef struct slice {
	DSC slice_desc;
	BLOB_PTR *slice_end;
	BLOB_PTR *slice_high_water;
	BLOB_PTR *slice_base;
	USHORT slice_element_length;
	USHORT slice_direction;
	SLONG slice_count;
} *SLICE;

#endif /* JRD_SDL_H */
