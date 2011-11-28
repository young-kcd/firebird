/*
 *	PROGRAM:	JRD Data Definition Utility
 *	MODULE:		dyn.h
 *	DESCRIPTION:	Dynamic data definition local data
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

#ifndef	JRD_DYN_H
#define JRD_DYN_H

#include "../common/classes/MsgPrint.h"
#include "../common/classes/MetaName.h"
#include "../common/classes/array.h"
#include "../common/classes/fb_string.h"
#include "../common/dsc.h"

const char* const ALL_PRIVILEGES = "SIUDR";	// all applicable grant/revoke privileges
const char* const ALL_PROC_PRIVILEGES = "X";	// all applicable grant/revoke privileges for a procedure
const int DYN_MSG_FAC		= 8;


#define GET_STRING(from, to)	DYN_get_string(from, to, sizeof(to))

namespace Jrd {

class jrd_tra;
class thread_db;

class Global
{
public:
	Global(jrd_tra* t, const Firebird::string& aSqlText)
		: gbl_transaction(t),
		  sqlText(aSqlText)
	{ }

	jrd_tra* const gbl_transaction;
	Firebird::string sqlText;
};

class dyn_fld
{
public:
	dsc dyn_dsc;
	bool dyn_null_flag;
	USHORT dyn_dtype;
	USHORT dyn_precision;
	USHORT dyn_charlen;
	SSHORT dyn_collation;
	SSHORT dyn_charset;
	SSHORT dyn_sub_type;
	Firebird::MetaName dyn_fld_source;
	Firebird::MetaName dyn_rel_name;
	Firebird::MetaName dyn_fld_name;
    USHORT dyn_charbytelen; // Used to check modify operations on string types.
    const UCHAR* dyn_default_src;
    const UCHAR* dyn_default_val;
    bool dyn_drop_default;
    const UCHAR* dyn_computed_src;
    const UCHAR* dyn_computed_val;
    bool dyn_drop_computed;
public:
	explicit dyn_fld(MemoryPool& p)
		: dyn_null_flag(false), dyn_dtype(0), dyn_precision(0), dyn_charlen(0),
		dyn_collation(0), dyn_charset(0), dyn_sub_type(0), dyn_fld_source(p), dyn_rel_name(p),
		dyn_fld_name(p), dyn_charbytelen(0),
		dyn_default_src(0), dyn_default_val(0), dyn_drop_default(false),
		dyn_computed_src(0), dyn_computed_val(0), dyn_drop_computed(false)
	{ }

	dyn_fld()
		: dyn_null_flag(false), dyn_dtype(0), dyn_precision(0), dyn_charlen(0),
		dyn_collation(0), dyn_charset(0), dyn_charbytelen(0),
		dyn_default_src(0), dyn_default_val(0), dyn_drop_default(false),
		dyn_computed_src(0), dyn_computed_val(0), dyn_drop_computed(false)
	{ }
};

} //namespace Jrd

void	DYN_error(bool, USHORT, const MsgFormat::SafeArg& sarg = MsgFormat::SafeArg());
void	DYN_error_punt(bool, USHORT, const MsgFormat::SafeArg& arg);
void	DYN_error_punt(bool, USHORT, const char* str);
void	DYN_error_punt(bool, USHORT);
void	DYN_execute(Jrd::Global*, const UCHAR**, const Firebird::MetaName*, Firebird::MetaName*,
	Firebird::MetaName*, Firebird::MetaName*, Firebird::MetaName*);
SLONG	DYN_get_number(const UCHAR**);
USHORT	DYN_get_string(const UCHAR**, Firebird::MetaName&, size_t);
USHORT	DYN_get_string(const UCHAR**, Firebird::PathName&, size_t);
USHORT	DYN_get_string(const UCHAR**, Firebird::string&, size_t);
USHORT	DYN_get_string(const UCHAR**, Firebird::UCharBuffer&, size_t);
USHORT	DYN_get_string(const UCHAR**, TEXT*, size_t);

bool	DYN_is_it_sql_role(Jrd::jrd_tra*, const Firebird::MetaName&, Firebird::MetaName&, Jrd::thread_db*);

void	DYN_unsupported_verb();

#endif // JRD_DYN_H
