#ifndef JRD_Y_HANDLE_H
#define JRD_Y_HANDLE_H

/*
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Alexander Peshkoff
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Alexander Peshkoff <peshkoff@mail.ru>
 *  and all contributors signed below.
 *
 *  All rights reserved.
 *
 *  Contributor(s): ______________________________
 *
 */

/*
 * These definitions placed into separate file
 * to avoid multiple definition of struct why_hndl in why.cpp
 * and dsql.cpp
 *
 */

namespace Jrd {
	class Attachment;
	class jrd_tra;
	class jrd_req;
}

union any_handle {
	struct why_hndl* h_why;
	class dsql_req* h_dsql;
	Jrd::Attachment* h_dbb;
	Jrd::jrd_tra* h_tra;
};

struct why_hndl
{
	UCHAR				type;
	UCHAR				flags;
	USHORT				implementation;
	FB_API_HANDLE		public_handle;
	union any_handle	handle;
	struct why_hndl*	parent;
	struct why_hndl*	next;
	union {
		struct why_hndl*	requests;
		struct sqlda_sup*	das;
	};
	struct why_hndl*	statements;
	struct why_hndl*	blobs;
	FB_API_HANDLE*		user_handle;
	struct clean*		cleanup;
	TEXT*				db_path;
};

typedef why_hndl *WHY_HNDL;
typedef why_hndl *WHY_REQ;
typedef why_hndl *WHY_DBB;
typedef why_hndl *WHY_TRA;
typedef why_hndl *WHY_BLB;
typedef why_hndl *WHY_ATT;
typedef why_hndl *WHY_STMT;
typedef why_hndl *WHY_SVC;

const int HANDLE_invalid		= 0;
const int HANDLE_database		= 1;
const int HANDLE_transaction	= 2;
const int HANDLE_request		= 3;
const int HANDLE_blob			= 4;
const int HANDLE_statement		= 5;
const int HANDLE_service		= 6;

const int HANDLE_TRANSACTION_limbo	= 1;
const int HANDLE_BLOB_filter		= 2;	/* Blob is locally filtered */
const int HANDLE_STATEMENT_local	= 4;	/* Process DSQL statement locally */

#endif // JRD_Y_HANDLE_H
