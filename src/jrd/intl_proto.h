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
	class thread_db;
}

struct dsc;
struct SubtypeInfo;

CHARSET_ID	INTL_charset(Jrd::thread_db*, USHORT, FPTR_ERROR);
int			INTL_compare(Jrd::thread_db*, const dsc*, const dsc*, FPTR_ERROR);
ULONG		INTL_convert_bytes(Jrd::thread_db*, CHARSET_ID, UCHAR*, ULONG, CHARSET_ID,
								const BYTE*, ULONG, FPTR_ERROR);
Jrd::CsConvert	INTL_convert_lookup(Jrd::thread_db*, CHARSET_ID, CHARSET_ID);
int			INTL_convert_string(dsc*, const dsc*, FPTR_ERROR);
int			INTL_data(const dsc*);
int			INTL_data_or_binary(const dsc*);
bool		INTL_defined_type(Jrd::thread_db*, ISC_STATUS*, SSHORT);
void		INTL_init(Jrd::thread_db*);
USHORT		INTL_key_length(Jrd::thread_db*, USHORT, USHORT);
Jrd::CharSet*	INTL_charset_lookup(Jrd::thread_db* tdbb, SSHORT parm1, ISC_STATUS* status);
Jrd::TextType*	INTL_texttype_lookup(Jrd::thread_db* tdbb, SSHORT parm1, FPTR_ERROR err,
								ISC_STATUS* status);
bool		INTL_texttype_validate(Jrd::thread_db*, const SubtypeInfo*);
void		INTL_pad_spaces(Jrd::thread_db*, dsc*, UCHAR*, ULONG);
USHORT		INTL_string_to_key(Jrd::thread_db*, USHORT, const dsc*, dsc*, USHORT);
int			INTL_str_to_upper(Jrd::thread_db*, dsc*);
int			INTL_str_to_lower(Jrd::thread_db*, dsc*);

// Built-in charsets/texttypes interface
INTL_BOOL INTL_builtin_lookup_charset(charset* cs, const ASCII* charset_name);
INTL_BOOL INTL_builtin_lookup_texttype(texttype* tt, const ASCII* texttype_name, const ASCII* charset_name,
									   USHORT attributes, const UCHAR* specific_attributes,
									   ULONG specific_attributes_length, INTL_BOOL ignore_attributes);

#endif // JRD_INTL_PROTO_H

