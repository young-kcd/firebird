#ifndef JRD_Y_HANDLE_H
#define JRD_Y_HANDLE_H

/*
 *  The contents of this file are subject to the Mozilla Public
 *  License Version 1.1 (the "License"); you may not use this file
 *  except in compliance with the License. You may obtain a copy of
 *  the License at http://www.mozilla.org/MPL/
 *  Alternatively, the contents of this file may be used under the
 *  terms of the GNU General Public License Version 2 or later (the
 *  "GPL"), in which case the provisions of the GPL are applicable
 *  instead of those above. You may obtain a copy of the Licence at
 *  http://www.gnu.org/copyleft/gpl.html
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Relevant for more details.
 *
 *  This file was created by members of the firebird development team.
 *  All individual contributions remain the Copyright (C) of those
 *  individuals.  Contributors to this file are either listed here or
 *  can be obtained from a CVS history command.
 *
 *  All rights reserved.
 *
 *  Contributor(s):
 *       Alex Peshkoff <peshkoff@hotmail.ru> *
 *
 */

/*
 * This definitions placed into separate file 
 * to avoid multiple definition of struct why_hndl in why.cpp
 * and dsql.cpp
 *
 * This file (or y_ref.h) must be included BEFORE ibase.h !
 */

union any_handle {
	struct why_hndl* h_why;
	class dsql_req* h_dsql;
	class att* h_dbb;
	class jrd_tra* h_tra;
};

typedef struct why_hndl
{
	UCHAR				type;
	UCHAR				flags;
	USHORT				implementation;
	union any_handle	handle;
	struct why_hndl*	parent;
	struct why_hndl*	next;
	union {
		struct why_hndl*requests;
		struct dasup*	das;
	};
	struct why_hndl*	statements;
	struct why_hndl*	blobs;
	struct why_hndl**	user_handle;
	struct clean*		cleanup;
	TEXT*				db_path;
} *WHY_HNDL, *WHY_REQ, *WHY_DBB, *WHY_TRA, 
  *WHY_BLB, *WHY_ATT, *WHY_STMT, *WHY_SVC;

#define HANDLE_invalid		0
#define HANDLE_database		1
#define HANDLE_transaction	2
#define HANDLE_request		3
#define HANDLE_blob			4
#define HANDLE_statement	5
#define HANDLE_service		6

#define HANDLE_TRANSACTION_limbo	1
#define HANDLE_BLOB_filter			2	/* Blob is locally filtered */
#define	HANDLE_STATEMENT_local		4	/* Process DSQL statement locally */

#include "../jrd/y_ref.h"

#endif // JRD_Y_HANDLE_H
