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

#ifndef JRD_INTL_PROTO_H
#define JRD_INTL_PROTO_H

#include "../jrd/intl_classes.h"

CHARSET_ID	INTL_charset(TDBB, USHORT, FPTR_VOID);
int			INTL_compare(TDBB, struct dsc *, struct dsc *, FPTR_VOID);
USHORT		INTL_convert_bytes(TDBB, CHARSET_ID, UCHAR *, USHORT, CHARSET_ID, 
								BYTE *, USHORT, FPTR_VOID);
CsConvert*	INTL_convert_lookup(TDBB, CHARSET_ID, CHARSET_ID);
int			INTL_convert_string(struct dsc *, struct dsc *, FPTR_VOID);
int			INTL_data(struct dsc *);
int			INTL_data_or_binary(struct dsc *);
int			INTL_defined_type(TDBB, ISC_STATUS *, SSHORT);
//SSHORT	INTL_fss_mbtowc(struct texttype **, unsigned short *,
//										 unsigned char *, USHORT);
//USHORT	INTL_fss_to_unicode(CsConvert *, USHORT *,
//											 USHORT, unsigned char *, USHORT,
//											 SSHORT *, USHORT *);
unsigned short	INTL_getch(TDBB, TextType**, SSHORT,
											UCHAR **, USHORT *);
void		INTL_init(TDBB);
USHORT		INTL_key_length(TDBB, USHORT, USHORT);
CharSet*	INTL_charset_lookup(TDBB tdbb, SSHORT parm1, ISC_STATUS * status);
TextType*	INTL_texttype_lookup(TDBB tdbb, SSHORT parm1, FPTR_VOID err, 
								ISC_STATUS * status);
//void*		INTL_obj_lookup(TDBB, USHORT, SSHORT, FPTR_VOID, ISC_STATUS *);
void		INTL_pad_spaces(TDBB, struct dsc *, UCHAR *, USHORT);
USHORT		INTL_string_to_key(TDBB, USHORT, struct dsc *, struct dsc *, USHORT);
int			INTL_str_to_upper(TDBB, struct dsc *);
//USHORT	INTL_unicode_to_fss(CsConvert *, unsigned char *, USHORT,
//								USHORT *, USHORT, SSHORT *, USHORT *);
UCHAR		INTL_upper(TDBB, USHORT, UCHAR);

#endif // JRD_INTL_PROTO_H
