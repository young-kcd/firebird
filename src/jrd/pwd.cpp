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
 */
/*
$Id: pwd.cpp,v 1.2 2001-07-12 05:46:05 bellardo Exp $
*/

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../include/jrd/gds.h"
#include "../jrd/jrd.h"
#include "../jrd/pwd.h"
#include "../jrd/enc_proto.h"
#include "../jrd/err_proto.h"
#include "../jrd/gds_proto.h"
#include "../jrd/pwd_proto.h"
#include "../jrd/sch_proto.h"

/* blr to search database for user name record */

static const UCHAR pwd_request[] = {
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

typedef struct
{
	SLONG	gid;
	SLONG	uid;
	SSHORT	flag;
	SCHAR	password[34];
} user_record;

static CONST char tpb[4] =
{
	isc_tpb_version1,
	isc_tpb_write,
	isc_tpb_concurrency,
	isc_tpb_wait
};


extern "C" {


static BOOLEAN lookup_user(TEXT*, int*, int*, TEXT*);
static BOOLEAN open_user_db(isc_db_handle*, SLONG*);


void PWD_get_user_dbpath(TEXT* path_buffer)
{
/**************************************
 *
 *      P W D _ g e t _ u s e r _ d b p a t h
 *
 **************************************
 *
 * Functional description:
 *      Return the path to the user database.  This
 *      Centralizes the knowledge of where the db is
 *      so gsec, lookup_user, etc. can be consistent.
 *
 **************************************/

#pragma FB_COMPILER_MESSAGE("TMN: What wierdness is this?")
#if (defined VMS || defined WIN_NT || defined LINUX || defined FREEBSD || defined NETBSD || defined SUPERSERVER || defined AIX_PPC || defined DARWIN )
	gds__prefix(path_buffer, USER_INFO_NAME);
#else
	strcpy(path_buffer, USER_INFO_NAME);
#endif
}


void PWD_verify_user(TEXT*	name,
					 TEXT*	user_name,
					 TEXT*	password,
					 TEXT*	password_enc,
					 int*	uid,
					 int*	gid,
					 int*	node_id)
{
/**************************************
 *
 *      P W D _ v e r i f y _ u s e r
 *
 **************************************
 *
 * Functional description:
 *      Verify a user in the security database.
 *
 **************************************/
	TEXT *p, *q, pw1[33], pw2[33], pwt[33];
	BOOLEAN notfound;

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

	notfound = lookup_user(name, uid, gid, pw1);

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


static BOOLEAN lookup_user(TEXT * user_name, int *uid, int *gid, TEXT * pwd)
{
/**************************************
 *
 *      l o o k u p _ u s e r
 *
 **************************************
 *
 * Functional description:
 *      Look the name up in the local userinfo
 *      database.   If it is found, fill in the
 *      user and group ids, as found in the
 *      database for that user.   Also return
 *      the encrypted password.
 *
 **************************************/
	BOOLEAN			notfound;		/* user found flag */
	isc_db_handle	uinfo;			/* database handle */
	SLONG*			lookup_trans;	/* default transaction handle */
	STATUS			status[20];		/* status vector */
	SLONG*			lookup_req;		/* request handle */
	TEXT			uname[129];		/* user name buffer */
	user_record		user;			/* user record */

/* Start by clearing the output data */

	if (uid)
		*uid = 0;
	if (gid)
		*gid = 0;
	if (pwd)
		*pwd = '\0';
	notfound = TRUE;
	strncpy(uname, user_name, 129);

/* get local copies of handles, so this depends on no static variables */

/* the database is opened here, but this could also be done when the
   first database is opened */

	if (open_user_db(&uinfo, status))
	{
		THREAD_ENTER;
		ERR_post(gds_psw_attach, 0);
	}

	lookup_req = NULL;
	lookup_trans = NULL;

	if (isc_start_transaction(	status,
								reinterpret_cast<void**>(&lookup_trans),
								(short) 1,
								&uinfo,
								(short)sizeof(tpb),
								tpb))
	{
		isc_detach_database(status, reinterpret_cast<void**>(&uinfo));
		THREAD_ENTER;
		ERR_post(gds_psw_start_trans, 0);
	}

	if (!isc_compile_request(	status,
								reinterpret_cast<void**>(&uinfo),
								reinterpret_cast<void**>(&lookup_req),
								(short)sizeof(pwd_request),
								reinterpret_cast<char*>(const_cast<UCHAR*>(pwd_request))))
	{
		if (!isc_start_and_send(status,
								reinterpret_cast<void**>(&lookup_req),
								reinterpret_cast<void**>(&lookup_trans),
								(short) 0,
								(short) sizeof(uname),
								uname, (short) 0))
		{
			while (1)
			{
				isc_receive(status,
							reinterpret_cast<void**>(&lookup_req),
							(short) 1,
							(short) sizeof(user),
							&user,
							(short) 0);
				if (!user.flag || status[1])
					break;
				notfound = FALSE;
				if (uid)
					*uid = user.uid;
				if (gid)
					*gid = user.gid;
				if (pwd)
					strncpy(pwd, user.password, 32);
			}
		}
	}

/* the database is closed and the request is released here, but this
   could be postponed until all databases are closed */

	isc_rollback_transaction(status,
							 reinterpret_cast < void **>(&lookup_trans));
	isc_detach_database(status, reinterpret_cast < void **>(&uinfo));
	THREAD_ENTER;

	return notfound;
}


static BOOLEAN open_user_db(isc_db_handle* uihandle, SLONG* status)
{
/**************************************
 *
 *      o p e n _ u s e r _ d b
 *
 **************************************
 *
 * Functional description:
 *      Open the user information database
 *      and return the handle.
 *
 *	TMN: FIX!!! Reverse return values (TRUE should be success!!!)
 *
 *      returns FALSE if successfully opened
 *              TRUE if not
 *
 **************************************/
	TEXT			user_info_name[MAX_PATH_LENGTH];
	isc_db_handle	uinfo;			/* database handle */
	IHNDL			ihandle;
	SCHAR*			p;
	SCHAR*			dpb;
	SCHAR			dpb_buffer[256];
	SSHORT			dpb_len;

	/* Register as internal database handle */

	for (ihandle = internal_db_handles; ihandle; ihandle = ihandle->ihndl_next)
	{
		if (ihandle->ihndl_object == NULL)
		{
			ihandle->ihndl_object = &uinfo;
			break;
		}
	}

	if (!ihandle)
	{
		ihandle = (IHNDL) gds__alloc ((SLONG) sizeof (struct ihndl));
		ihandle->ihndl_object = &uinfo;
		ihandle->ihndl_next = internal_db_handles;
		internal_db_handles = ihandle;
	}

	THREAD_EXIT;

/* initialize the data base's name */

	uinfo = NULL;

	PWD_get_user_dbpath(user_info_name);

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

	*dpb++ = isc_dpb_sec_attach;	/* Attachment is for the security database */
	*dpb++ = 1;					/* Parameter value length */
	*dpb++ = TRUE;				/* Parameter value */

	dpb_len = dpb - dpb_buffer;

	isc_attach_database(status,
						0,
						user_info_name,
						reinterpret_cast<void**>(&uinfo),
						dpb_len,
						dpb_buffer);

	if (status[1] == gds_login)
	{
		/* we may be going against a V3 database which does not
		 * understand this combination.
		 */

		isc_attach_database(status,
							0,
							user_info_name,
							reinterpret_cast<void**>(&uinfo),
							0,
							0);
	}

	*uihandle = uinfo;

	assert(ihandle->ihndl_object == &uinfo);
	ihandle->ihndl_object = NULL;

	if (status[1])
	{
		// Is this reversed logic or what?!
		// Return TRUE to tell we did NOT open the DB.
		return TRUE;
	}

	return FALSE;
}


}	// extern "C"

