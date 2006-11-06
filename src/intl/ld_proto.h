/*
 *	PROGRAM:	InterBase International support
 *	MODULE:		ld_proto.h
 *	DESCRIPTION:	Prototype header file for ld.c & ld2.c
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

#ifndef _INTL_LD_PROTO_H_
#define _INTL_LD_PROTO_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEV_BUILD
void FB_DLL_EXPORT LD_assert(const SCHAR*, int);
#endif

INTL_BOOL FB_DLL_EXPORT LD_lookup_charset(charset* cs, const ASCII* name);
INTL_BOOL FB_DLL_EXPORT LD_lookup_texttype(texttype* tt, const ASCII* texttype_name, const ASCII* charset_name,
										   USHORT attributes, const UCHAR* specific_attributes,
										   ULONG specific_attributes_length, INTL_BOOL ignore_attributes);

INTL_BOOL FB_DLL_EXPORT LD2_lookup_charset(charset* cs, const ASCII* name);
INTL_BOOL FB_DLL_EXPORT LD2_lookup_texttype(texttype* tt, const ASCII* texttype_name, const ASCII* charset_name,
											USHORT attributes, const UCHAR* specific_attributes,
											ULONG specific_attributes_length, INTL_BOOL ignore_attributes);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _INTL_LD_PROTO_H_ */
