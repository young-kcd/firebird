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
#include "../jrd/smp_impl.h"
#include "../jrd/thd.h"
#include "../jrd/os/sha.h"
#include <time.h>

const size_t MAX_PASSWORD_ENC_LENGTH = 12;	// passed by remote protocol
const size_t MAX_PASSWORD_LENGTH = 64;		// used to store passwords internally
static const char* PASSWORD_SALT  = "9z";	// for old ENC_crypt()
const size_t SALT_LENGTH = 12;				// measured after base64 coding

class SecurityDatabase
{
	struct user_record {
		SLONG gid;
		SLONG uid;
		SSHORT flag;
		SCHAR password[MAX_PASSWORD_LENGTH + 2];
	};

public:

	static void getPath(TEXT* path_buffer)
	{
		static const char* USER_INFO_NAME =
#ifdef VMS
					"[sysmgr]security.fdb";
#else
					"security.fdb";
#endif

		gds__prefix(path_buffer, USER_INFO_NAME);
	}

	static void initialize();
	static void shutdown();
	static void verifyUser(TEXT*, const TEXT*, const TEXT*, const TEXT*,
		int*, int*, int*);

	static void hash(Firebird::string& h, 
					 const TEXT* userName, 
					 const TEXT* passwd)
	{
		Firebird::string salt;
		Jrd::CryptSupport::get().random(salt, SALT_LENGTH);
		hash(h, userName, passwd, salt);
	}

	static void hash(Firebird::string& h, 
					 const TEXT* userName, 
					 const TEXT* passwd,
					 const Firebird::string& oldHash)
	{
		Firebird::string salt(oldHash);
		salt.resize(SALT_LENGTH, '=');
		Firebird::string allData(salt);
		allData += userName;
		allData += passwd;
		Jrd::CryptSupport::get().hash(h, allData);
		h = salt + h;
	}

private:

	static const UCHAR PWD_REQUEST[256];
	static const UCHAR TPB[4];

	V4Mutex mutex;

	ISC_STATUS_ARRAY status;

	isc_db_handle lookup_db;
	isc_req_handle lookup_req;

	static const bool is_cached;

	int counter;

	void fini();
	void init();
	bool lookup_user(TEXT*, int*, int*, TEXT*);
	bool prepare();

	static SecurityDatabase instance;

	SecurityDatabase() 
	{
		lookup_db = 0;
	}
};

#endif /* JRD_PWD_H */
