/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		jrd_pwd.h
 *	DESCRIPTION:	User information database name
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
 *
 * 2002.10.29 Sean Leyne - Removed obsolete "Netware" port
 * 2003.02.02 Dmitry Yemanov: Implemented cached security database connection
 */

#ifndef JRD_PWD_H
#define JRD_PWD_H

#include "../jrd/ibase.h"
#include "../jrd/thd.h"

#define MAX_PASSWORD_ENC_LENGTH 12
#define PASSWORD_SALT  "9z"

class SecurityDatabase
{
	typedef struct {
		SLONG gid;
		SLONG uid;
		SSHORT flag;
		SCHAR password[34];
	} user_record;

public:

	static void getPath(TEXT*);
	static void initialize();
	static void shutdown();
	static void verifyUser(TEXT*, TEXT*, TEXT*, TEXT*, int*, int*, int*);

	~SecurityDatabase();

private:

	static const UCHAR PWD_REQUEST[256];
	static const UCHAR TPB[4];

	MUTX_T mutex;

	ISC_STATUS_ARRAY status;

	isc_db_handle lookup_db;
	isc_req_handle lookup_req;

	static const bool is_cached;

	int counter;

	void lock();
	void unlock();

	void fini();
	void init();
	bool lookup_user(TEXT*, int*, int*, TEXT*);
	bool prepare();

	static SecurityDatabase& instance();

	SecurityDatabase();
};

#ifdef VMS
#define USER_INFO_NAME	"[sysmgr]security.fdb"
#else
#define USER_INFO_NAME	"security.fdb"
#endif

#endif /* JRD_PWD_H */
