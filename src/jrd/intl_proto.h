/*
 *	PROGRAM:	JRD Access method
 *	MODULE:		intl_proto.h
 *	DESCRIPTION:	Prototype Header file for intl.cpp
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

namespace Jrd {
	struct thread_db;
}

CHARSET_ID	INTL_charset(Jrd::thread_db*, USHORT, FPTR_ERROR);
int			INTL_compare(Jrd::thread_db*, const struct dsc*, const struct dsc*, FPTR_ERROR);
USHORT		INTL_convert_bytes(Jrd::thread_db*, CHARSET_ID, UCHAR*, USHORT, CHARSET_ID,
								const BYTE*, USHORT, FPTR_ERROR);
Jrd::CsConvert	INTL_convert_lookup(Jrd::thread_db*, CHARSET_ID, CHARSET_ID);
int			INTL_convert_string(struct dsc*, const struct dsc*, FPTR_ERROR);
int			INTL_data(const struct dsc*);
int			INTL_data_or_binary(const struct dsc*);
int			INTL_defined_type(Jrd::thread_db*, ISC_STATUS*, SSHORT);
UCS2_CHAR	INTL_getch(Jrd::thread_db*, Jrd::TextType*, SSHORT, const UCHAR**, USHORT*);
void		INTL_init(Jrd::thread_db*);
USHORT		INTL_key_length(Jrd::thread_db*, USHORT, USHORT);
Jrd::CharSet	INTL_charset_lookup(Jrd::thread_db* tdbb, SSHORT parm1, ISC_STATUS* status);
Jrd::TextType	INTL_texttype_lookup(Jrd::thread_db* tdbb, SSHORT parm1, FPTR_ERROR err,
								ISC_STATUS* status);
void		INTL_pad_spaces(Jrd::thread_db*, struct dsc*, UCHAR*, USHORT);
USHORT		INTL_string_to_key(Jrd::thread_db*, USHORT, const struct dsc*, struct dsc*, bool);
int			INTL_str_to_upper(Jrd::thread_db*, struct dsc*);
UCHAR		INTL_upper(Jrd::thread_db*, USHORT, UCHAR);

// Built-in charsets interface
FPTR_SHORT INTL_builtin_lookup(USHORT, SSHORT, SSHORT);
SSHORT INTL_builtin_nc_mbtowc(TEXTTYPE obj,
							  UCS2_CHAR* wc, const UCHAR* ptr, USHORT count);
SSHORT INTL_builtin_mb_mbtowc(TEXTTYPE obj,
							  UCS2_CHAR* wc, const UCHAR* ptr, USHORT count);
SSHORT INTL_builtin_wc_mbtowc(TEXTTYPE obj,
							  UCS2_CHAR* wc, const UCHAR* ptr, USHORT count);

#endif // JRD_INTL_PROTO_H

