/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		err_proto.h
 *	DESCRIPTION:	Prototype header file for err.cpp
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

#ifndef JRD_ERR_PROTO_H
#define JRD_ERR_PROTO_H

#include "../common/classes/fb_string.h"
#include "../common/classes/MetaName.h"

#ifndef REQUESTER

namespace Jrd {

/* Index error types */

enum idx_e {
	idx_e_ok = 0,
	idx_e_duplicate,
	idx_e_keytoobig,
	idx_e_conversion,
	idx_e_foreign_target_doesnt_exist,
	idx_e_foreign_references_present
};

typedef idx_e IDX_E;

class jrd_rel;
} //namespace Jrd

bool	ERR_post_warning(ISC_STATUS, ...);
void	ERR_assert(const TEXT*, int);
void	ERR_bugcheck(int, const TEXT* = NULL, int = 0);
void	ERR_bugcheck_msg(const TEXT*);
void	ERR_corrupt(int);
void	ERR_duplicate_error(Jrd::idx_e, const Jrd::jrd_rel*, USHORT);
void	ERR_error(int);
void	ERR_error_msg(const TEXT*);
void	ERR_post(ISC_STATUS, ...);
void	ERR_post_nothrow(ISC_STATUS, ...);
void	ERR_punt(void);
void	ERR_warning(ISC_STATUS, ...);
void	ERR_log(int, int, const TEXT*);

#endif /* REQUESTER */

const TEXT*		ERR_cstring(const TEXT*);
inline const TEXT*		ERR_cstring(const Firebird::string& in_string)
{
	return ERR_cstring(in_string.c_str());
}
inline const TEXT*		ERR_cstring(const Firebird::MetaName& in_string)
{
	return ERR_cstring(in_string.c_str());
}

// AP: used ?

const TEXT*		ERR_string(const TEXT*, int);

#endif /* JRD_ERR_PROTO_H */

