/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		pwd.e
 *	DESCRIPTION:	User information database access
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
 * 2003.02.02 Dmitry Yemanov: Implemented cached security database connection
 */

#include "firebird.h"
#include <string.h>
#include <stdlib.h>
#include "../jrd/gds.h"
#include "../jrd/jrd.h"
#include "../jrd/jrd_pwd.h"
#include "../jrd/enc_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/sch_proto.h"
#include "../jrd/thd_proto.h"

#ifdef SUPERSERVER
const bool SecurityDatabase::is_cached = true;
#else
const bool SecurityDatabase::is_cached = false;
#endif

/* BLR to search database for user name record */

const UCHAR SecurityDatabase::PWD_REQUEST[256] = {
	blr_version5,
	blr_begin,
	blr_message, 1, 4, 0,
	blr_long, 0,
	blr_long, 0,
	blr_short, 0,
	blr_text, 34, 0,
	blr_message, 0, 1, 0,
	blr_cstring, 129, 0,
	blr_receive, 0,
	blr_begin,
	blr_for,
	blr_rse, 1,
	blr_relation, 5, 'U', 'S', 'E', 'R', 'S', 0,
	blr_first,
	blr_literal, blr_short, 0, 1, 0,
	blr_boolean,
	blr_eql,
	blr_field, 0, 9, 'U', 'S', 'E', 'R', '_', 'N', 'A', 'M', 'E',
	blr_parameter, 0, 0, 0,
	blr_end,
	blr_send, 1,
	blr_begin,
	blr_assignment,
	blr_field, 0, 3, 'G', 'I', 'D',
	blr_parameter, 1, 0, 0,
	blr_assignment,
	blr_field, 0, 3, 'U', 'I', 'D',
	blr_parameter, 1, 1, 0,
	blr_assignment,
	blr_literal, blr_short, 0, 1, 0,
	blr_parameter, 1, 2, 0,
	blr_assignment,
	blr_field, 0, 6, 'P', 'A', 'S', 'S', 'W', 'D',
	blr_parameter, 1, 3, 0,
	blr_end,
	blr_send, 1,
	blr_assignment,
	blr_literal, blr_short, 0, 0, 0,
	blr_parameter, 1, 2, 0,
	blr_end,
	blr_end,
	blr_eoc
};

/* Transaction parameter buffer */

const UCHAR SecurityDatabase::TPB[4] = {
	isc_tpb_version1,
	isc_tpb_read,
	isc_tpb_concurrency,
	isc_tpb_wait
};

/* Static instance of the database */

SecurityDatabase SecurityDatabase::instance;


/******************************************************************************
 *
 *	Constructor & destructor
 */

SecurityDatabase::SecurityDatabase()
	: lookup_db(0), lookup_req(0), counter(0) 
{
	THD_MUTEX_INIT(&mutex);
}

SecurityDatabase::~SecurityDatabase()
{
	THD_MUTEX_DESTROY(&mutex);
}

/******************************************************************************
 *
 *	Private interface
 */

void SecurityDatabase::lock()
{
	THREAD_EXIT;
	THD_MUTEX_LOCK(&mutex);
	THREAD_ENTER;
}

void SecurityDatabase::unlock()
{
	THD_MUTEX_UNLOCK(&mutex);
}

void SecurityDatabase::fini()
{
	counter -= (is_cached) ? 1 : 0;
#ifndef EMBEDDED
	if (counter == 1 && lookup_db)
	{
		THREAD_EXIT;
		isc_detach_database(status, &lookup_db);
		THREAD_ENTER;
	}
#endif
}

void SecurityDatabase::init()
{
	counter += (is_cached) ? 1 : 0;
}

bool SecurityDatabase::lookup_user(TEXT * user_name, int *uid, int *gid, TEXT * pwd)
{
	bool notfound = true;	// user found flag
	TEXT uname[129];		// user name buffer
	user_record user;		// user record

	/* Start by clearing the output data */

	if (uid)
		*uid = 0;
	if (gid)
		*gid = 0;
	if (pwd)
		*pwd = '\0';

	strncpy(uname, user_name, 129);

	lock();

	/* Attach database and compile request */

	if (!prepare())
	{
		if (lookup_db)
		{
			isc_detach_database(status, &lookup_db);
		}
		THREAD_ENTER;
		unlock();
		ERR_post(gds_psw_attach, 0);
	}

	/* Lookup */

	isc_tr_handle lookup_trans = 0;

	if (isc_start_transaction(status, &lookup_trans, 1, &lookup_db, sizeof(TPB), TPB))
	{
		THREAD_ENTER;
		unlock();
		ERR_post(gds_psw_start_trans, 0);
	}

	if (!isc_start_and_send(status, &lookup_req, &lookup_trans, 0, sizeof(uname), uname, 0))
	{
		while (1)
		{
			isc_receive(status, &lookup_req, 1, sizeof(user), &user, 0);
			if (!user.flag || status[1])
				break;
			notfound = false;
			if (uid)
				*uid = user.uid;
			if (gid)
				*gid = user.gid;
			if (pwd)
				strncpy(pwd, user.password, 32);
		}
	}

	isc_rollback_transaction(status, &lookup_trans);

	if (!is_cached)
	{
		isc_detach_database(status, &lookup_db);
	}
	THREAD_ENTER;
	unlock();

	return notfound;
}

bool SecurityDatabase::prepare()
{
	TEXT user_info_name[MAXPATHLEN];
	IHNDL ihandle;
	SCHAR* dpb;
	SCHAR dpb_buffer[256];
	SSHORT dpb_len;

	/* dimitr: access to the class members in this routine should be synchronized
			   when fine grained locking will be implemented for the SS architecture */

	if (lookup_db)
	{
		THREAD_EXIT;
		return true;
	}

	/* Register as internal database handle */

	for (ihandle = internal_db_handles; ihandle; ihandle = ihandle->ihndl_next)
	{
		if (ihandle->ihndl_object == NULL)
		{
			ihandle->ihndl_object = &lookup_db;
			break;
		}
	}

	if (!ihandle)
	{
		ihandle = (IHNDL) gds__alloc ((SLONG) sizeof(struct ihndl));
		ihandle->ihndl_object = &lookup_db;
		ihandle->ihndl_next = internal_db_handles;
		internal_db_handles = ihandle;
	}

	THREAD_EXIT;

	lookup_db = lookup_req = 0;

	/* initialize the data base's name */

	getPath(user_info_name);

	/* Perhaps build up a dpb */

	dpb = dpb_buffer;

	*dpb++ = gds_dpb_version1;

	// insert username
	static const char szAuthenticator[] = "authenticator";
	const size_t nAuthNameLen = strlen(szAuthenticator);
	*dpb++ = gds_dpb_user_name;
	*dpb++ = nAuthNameLen;
	memcpy(dpb, szAuthenticator, nAuthNameLen);
	dpb += nAuthNameLen;

	// insert password
	static const char szPassword[] = "none";
	const size_t nPswdLen = strlen(szPassword);
	*dpb++ = gds_dpb_password;
	*dpb++ = nPswdLen;
	memcpy(dpb, szPassword, nPswdLen);
	dpb += nPswdLen;

	*dpb++ = isc_dpb_sec_attach;	// Attachment is for the security database
	*dpb++ = 1;						// Parameter value length
	*dpb++ = TRUE;					// Parameter value

	dpb_len = dpb - dpb_buffer;

	isc_attach_database(status, 0, user_info_name, &lookup_db, dpb_len, dpb_buffer);

	if (status[1] == gds_login)
	{
		/* we may be going against a V3 database which does not
		 * understand this combination */

		isc_attach_database(status, 0, user_info_name, &lookup_db, 0, 0);
	}

	if (status[1])
	{
		return false;
	}

	assert(ihandle->ihndl_object == &lookup_db);
	ihandle->ihndl_object = NULL;

	isc_compile_request(status, &lookup_db, &lookup_req, sizeof(PWD_REQUEST),
						reinterpret_cast<char*>(const_cast<UCHAR*>(PWD_REQUEST)));

	if (status[1])
	{
		return false;
	}

	return true;
}

/******************************************************************************
 *
 *	Public interface
 */

void SecurityDatabase::getPath(TEXT* path_buffer)
{
	gds__prefix(path_buffer, USER_INFO_NAME);
}

void SecurityDatabase::initialize()
{
	instance.init();
}

void SecurityDatabase::shutdown()
{
	instance.fini();
}

void SecurityDatabase::verifyUser(TEXT* name,
								  TEXT* user_name,
								  TEXT* password,
								  TEXT* password_enc,
								  int* uid,
								  int* gid,
								  int* node_id)
{
	bool notfound;
	TEXT *p, *q, pw1[33], pw2[33], pwt[33];

	if (user_name)
	{
		for (p = name, q = user_name; *q; q++, p++)
		{
			*p = UPPER7(*q);
		}
		*p = 0;
	}

	/* Look up the user name in the userinfo database and use the parameters
	   found there.  This means that another database must be accessed, and
	   that means the current context must be saved and restored. */

#ifdef EMBEDDED
	return;
#else
	notfound = instance.lookup_user(name, uid, gid, pw1);
#endif

	/* Punt if the user has specified neither a raw nor an encrypted password,
	   or if the user has specified both a raw and an encrypted password, 
	   or if the user name specified was not in the password database
	   (or if there was no password database - it's still not found) */

	if ((!password && !password_enc) ||
		(password && password_enc) ||
		notfound)
	{
		ERR_post(gds_login, 0);
	}

	if (password) {
		strcpy(pwt, ENC_crypt(password, PASSWORD_SALT));
		password_enc = pwt + 2;
	}
	strcpy(pw2, ENC_crypt(password_enc, PASSWORD_SALT));
	if (strncmp(pw1, pw2 + 2, 11)) {
		ERR_post(gds_login, 0);
	}

	*node_id = 0;
}
