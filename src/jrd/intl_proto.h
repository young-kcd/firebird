/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		intl_proto.h
 *	DESCRIPTION:	Prototype Header file for intl.c 
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

#ifndef _JRD_INTL_PROTO_H_
#define _JRD_INTL_PROTO_H_

#include "../jrd/intl_classes.h"

extern CHARSET_ID DLL_EXPORT INTL_charset(TDBB, USHORT, FPTR_VOID);
extern int DLL_EXPORT INTL_compare(TDBB, struct dsc *, struct dsc *,
								   FPTR_VOID);
extern USHORT DLL_EXPORT INTL_convert_bytes(TDBB, CHARSET_ID, UCHAR *, USHORT,
											CHARSET_ID, BYTE *, USHORT,
											FPTR_VOID);
extern CsConvert *DLL_EXPORT INTL_convert_lookup(TDBB, CHARSET_ID, CHARSET_ID);
extern int DLL_EXPORT INTL_convert_string(struct dsc *, struct dsc *,
										  FPTR_VOID);
extern int DLL_EXPORT INTL_data(struct dsc *);
extern int DLL_EXPORT INTL_data_or_binary(struct dsc *);
extern int DLL_EXPORT INTL_defined_type(TDBB, ISC_STATUS *, SSHORT);
//extern SSHORT DLL_EXPORT INTL_fss_mbtowc(struct texttype **, unsigned short *,
//										 unsigned char *, USHORT);
//extern USHORT DLL_EXPORT INTL_fss_to_unicode(CsConvert *, USHORT *,
//											 USHORT, unsigned char *, USHORT,
//											 SSHORT *, USHORT *);
extern unsigned short DLL_EXPORT INTL_getch(TDBB, TextType**, SSHORT,
											UCHAR **, USHORT *);
extern void DLL_EXPORT INTL_init(TDBB);
extern USHORT DLL_EXPORT INTL_key_length(TDBB, USHORT, USHORT);
extern CharSet *DLL_EXPORT INTL_charset_lookup(TDBB tdbb, SSHORT parm1,
												ISC_STATUS * status);
extern TextType *DLL_EXPORT INTL_texttype_lookup(
								 TDBB tdbb,
								 SSHORT parm1, FPTR_VOID err, ISC_STATUS * status);
//extern void *DLL_EXPORT INTL_obj_lookup(TDBB, USHORT, SSHORT, FPTR_VOID,
//										ISC_STATUS *);
extern void DLL_EXPORT INTL_pad_spaces(TDBB, struct dsc *, UCHAR *, USHORT);
extern USHORT DLL_EXPORT INTL_string_to_key(TDBB, USHORT, struct dsc *,
											struct dsc *, USHORT);
extern int DLL_EXPORT INTL_str_to_upper(TDBB, struct dsc *);
//extern USHORT DLL_EXPORT INTL_unicode_to_fss(CsConvert *,
//											 unsigned char *, USHORT,
//											 USHORT *, USHORT, SSHORT *,
//											 USHORT *);
extern UCHAR DLL_EXPORT INTL_upper(TDBB, USHORT, UCHAR);

#endif /* _JRD_INTL_PROTO_H_ */
