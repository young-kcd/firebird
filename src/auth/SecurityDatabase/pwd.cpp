/*
 *	PROGRAM:	JRD Access Method
 *	MODULE:		pwd.cpp
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
#include <time.h>
#include "../common/common.h"
#include "../jrd/ibase.h"
#include "../jrd/jrd.h"
#include "../auth/SecurityDatabase/jrd_pwd.h"
#include "../common/enc_proto.h"
#include "../jrd/err_proto.h"
#include "../yvalve/gds_proto.h"
#include "../common/isc_proto.h"
#include "../jrd/thread_proto.h"
#include "../jrd/jrd_proto.h"
#include "../jrd/scl.h"
#include "../common/config/config.h"
#include "../common/classes/objects_array.h"
#include "../common/classes/init.h"
#include "../common/classes/ImplementHelper.h"

using namespace Firebird;

namespace {

// register plugin
char name[] = "LEGACY_AUTH";
PluginHelper<Auth::SecurityDatabaseServer, Firebird::Plugin::AuthServer, 200, name> server;

// temporal implementation of timer

GlobalPtr<Mutex> timerMutex;
FPTR_VOID_PTR toRun = 0;
unsigned int cnt = 0;

int active = 0;

int stopTimer(const int, const int mask, void*)
{
	switch(mask)
	{
	case fb_shut_preproviders:
		active = 2;
		break;
	case fb_shut_finish:
		while (active == 2)
		{
			THREAD_SLEEP(10);
		}
		break;
	}

	return 0;
}

THREAD_ENTRY_DECLARE threadTimer(THREAD_ENTRY_PARAM)
{
	while (active == 1)
	{
		{	// scope
			MutexLockGuard g(timerMutex);
			if (cnt == 0)
			{
				if (toRun)
				{
					toRun(0);
					toRun = 0;
				}
			}
			else
			{
				--cnt;
			}
		}

		THREAD_SLEEP(100);
	}

	active = 3;
	return 0;
}

int fb_alloc_timer()
{
	if (! active)
	{
		active = 1;
		Thread::start(threadTimer, 0, 0);
		fb_shutdown_callback(0, stopTimer, fb_shut_preproviders | fb_shut_finish, 0);
	}

	return 1;
}

void fb_thread_timer(int, int delay, FPTR_VOID_PTR function, void*)
{
	MutexLockGuard g(timerMutex);

	cnt = delay / 100;
	if (! cnt)
	{
		cnt = 1;
	}

	toRun = function;
}

// BLR to search database for user name record

const UCHAR PWD_REQUEST[] =
{
	blr_version5,
	blr_begin,
	blr_message, 1, 4, 0,
	blr_long, 0,
	blr_long, 0,
	blr_short, 0,
	blr_text, BLR_WORD(Auth::MAX_PASSWORD_LENGTH + 2),
	blr_message, 0, 1, 0,
	blr_cstring, 129, 0,
	blr_receive, 0,
	blr_begin,
	blr_for,
	blr_rse, 1,
	blr_relation, 9, 'P', 'L', 'G', '$', 'U', 'S', 'E', 'R', 'S', 0,
	blr_first,
	blr_literal, blr_short, 0, 1, 0,
	blr_boolean,
	blr_eql,
	blr_field, 0, 13, 'P', 'L', 'G', '$', 'U', 'S', 'E', 'R', '_', 'N', 'A', 'M', 'E',
	blr_parameter, 0, 0, 0,
	blr_end,
	blr_send, 1,
	blr_begin,
	blr_assignment,
	blr_field, 0, 7, 'P', 'L', 'G', '$', 'G', 'I', 'D',
	blr_parameter, 1, 0, 0,
	blr_assignment,
	blr_field, 0, 7, 'P', 'L', 'G', '$', 'U', 'I', 'D',
	blr_parameter, 1, 1, 0,
	blr_assignment,
	blr_literal, blr_short, 0, 1, 0,
	blr_parameter, 1, 2, 0,
	blr_assignment,
	blr_field, 0, 10, 'P', 'L', 'G', '$', 'P', 'A', 'S', 'S', 'W', 'D',
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

// Returns data in the following format

struct user_record
{
	SLONG gid;
	SLONG uid;
	SSHORT flag;
	SCHAR password[Auth::MAX_PASSWORD_LENGTH + 2];
};

// Transaction parameter buffer

const UCHAR TPB[4] =
{
	isc_tpb_version1,
	isc_tpb_read,
	isc_tpb_concurrency,
	isc_tpb_wait
};

} // anonymous

namespace Auth {

// Static instance of the database

SecurityDatabase SecurityDatabase::instance;

/******************************************************************************
 *
 *	Private interface
 */

void SecurityDatabase::fini()
{
	isc_db_handle tmp = 0;

	try
	{
		MutexLockGuard guard(mutex);

		if (lookup_req)
		{
			isc_release_request(status, &lookup_req);
			checkStatus("isc_release_request");
		}

		tmp = lookup_db;
		lookup_db = 0;
	}
	catch (const Firebird::Exception&)
	{
		if (tmp)
		{
			isc_detach_database(status, &tmp);
		}
	}

	if (tmp)
	{
		isc_detach_database(status, &tmp);
		checkStatus("isc_detach_database");
	}
}

void SecurityDatabase::init()
{
	if (! timer)
	{
		timer = fb_alloc_timer();
		getPath(user_info_name);
	}
}

bool SecurityDatabase::lookup_user(const char* user_name, char* pwd)
{
	bool found = false;		// user found flag
	char uname[129];		// user name buffer
	user_record user;		// user record

	// Start by clearing the output data

	if (pwd)
		*pwd = '\0';

	strncpy(uname, user_name, sizeof uname);
	uname[sizeof uname - 1] = 0;

	MutexLockGuard guard(mutex);

	// Attach database and compile request

	prepare();

	// Lookup

	isc_tr_handle lookup_trans = 0;

	isc_start_transaction(status, &lookup_trans, 1, &lookup_db, sizeof(TPB), TPB);
	checkStatus("isc_start_transaction", isc_psw_start_trans);

	isc_start_and_send(status, &lookup_req, &lookup_trans, 0, sizeof(uname), uname, 0);
	checkStatus("isc_start_and_send");

	while (true)
	{
		isc_receive(status, &lookup_req, 1, sizeof(user), &user, 0);
		checkStatus("isc_receive");

		if (!user.flag || status[1])
			break;

		found = true;

		if (pwd)
		{
			strncpy(pwd, user.password, MAX_PASSWORD_LENGTH);
			pwd[MAX_PASSWORD_LENGTH] = 0;
		}
	}

	isc_rollback_transaction(status, &lookup_trans);
	checkStatus("isc_rollback_transaction");

	fb_thread_timer(timer, 10000, shutdown, 0);

	return found;
}

void SecurityDatabase::prepare()
{
	if (lookup_db)
	{
		return;
	}

	init();

	lookup_db = lookup_req = 0;

	// Perhaps build up a dpb
	ClumpletWriter dpb(ClumpletReader::Tagged, MAX_DPB_SIZE, isc_dpb_version1);

	// Attachment is for the security database
	dpb.insertByte(isc_dpb_sec_attach, TRUE);

	// Attach as SYSDBA
	dpb.insertString(isc_dpb_trusted_auth, SYSDBA_USER_NAME, strlen(SYSDBA_USER_NAME));

	isc_db_handle tempHandle = 0;
	isc_attach_database(status, 0, user_info_name, &tempHandle,
		dpb.getBufferLength(), reinterpret_cast<const char*>(dpb.getBuffer()));
	checkStatus("isc_attach_database", isc_psw_attach);
	lookup_db = tempHandle;

	isc_compile_request(status, &lookup_db, &lookup_req, sizeof(PWD_REQUEST),
		reinterpret_cast<const char*>(PWD_REQUEST));
	if (status[1])
	{
		ISC_STATUS_ARRAY localStatus;
		// ignore status returned in order to keep first error
		isc_detach_database(localStatus, &lookup_db);
	}

	checkStatus("isc_compile_request", isc_psw_attach);
}

/******************************************************************************
 *
 *	Public interface
 */

Result SecurityDatabase::verify(WriterInterface* authBlock,
								ClumpletReader& originalDpb)
{
	static AmCache useNative = AM_UNKNOWN;

	if (useNative == AM_UNKNOWN)
	{
		// We use PathName for string comparison using platform filename comparison
		// rules (case-sensitive or case-insensitive).
		const PathName authMethod(Config::getAuthMethod());
		useNative = (authMethod == AmNative || authMethod == AmMixed) ? AM_ENABLED : AM_DISABLED;
	}

	if (useNative == AM_DISABLED)
	{
		return AUTH_CONTINUE;
	}

	string login, password, passwordEnc;

	for (originalDpb.rewind(); !originalDpb.isEof(); originalDpb.moveNext())
	{
		switch (originalDpb.getClumpTag())
		{
		case isc_dpb_user_name:
			originalDpb.getString(login);
			break;
		case isc_dpb_password:
			originalDpb.getString(password);
			break;
		case isc_dpb_password_enc:
			originalDpb.getString(passwordEnc);
			break;
		}
	}

	if (login.hasData() && (password.hasData() || passwordEnc.hasData()))
	{
		login.upper();

		// Look up the user name in the userinfo database and use the parameters
		// found there. This means that another database must be accessed, and
		// that means the current context must be saved and restored.

		char pw1[MAX_PASSWORD_LENGTH + 1];
		if (!instance.lookup_user(login.c_str(), pw1))
		{
			return AUTH_FAILED;
		}
		pw1[MAX_PASSWORD_LENGTH] = 0;
		string storedHash(pw1, MAX_PASSWORD_LENGTH);
		storedHash.rtrim();

		if (!passwordEnc.hasData())
		{
			char pwt[MAX_PASSWORD_LENGTH + 2];
			ENC_crypt(pwt, sizeof pwt, password.c_str(), PASSWORD_SALT);
			passwordEnc.assign(&pwt[2]);
		}

		string newHash;
		hash(newHash, login, passwordEnc, storedHash);
		if (newHash != storedHash)
		{
			bool legacyHash = Config::getLegacyHash();
			if (legacyHash)
			{
				newHash.resize(MAX_PASSWORD_LENGTH + 2);
				ENC_crypt(newHash.begin(), newHash.length(), passwordEnc.c_str(), PASSWORD_SALT);
				newHash.recalculate_length();
				newHash.erase(0, 2);
				legacyHash = newHash == storedHash;
			}
			if (!legacyHash)
			{
				return AUTH_FAILED;
			}
		}

		authBlock->add(login.c_str(), "SecDB", instance.user_info_name);
		return AUTH_SUCCESS;
	}

	return AUTH_CONTINUE;
}

void SecurityDatabase::checkStatus(const char* callName, ISC_STATUS userError)
{
	if (status[1] == 0)
	{
		return;
	}

	string message;
	message.printf("Error in %s() API call when working with security database", callName);
	iscLogStatus(message.c_str(), status);

#ifdef DEV_BUILD
	// throw original status error
	status_exception::raise(status);
#else
	// showing real problems with security database to users is not good idea
	// from security POV - therefore some generic message is used
	Arg::Gds(userError).raise();
#endif
}

void SecurityDatabase::shutdown(void*)
{
	instance.fini();
}

ServerInstance* SecurityDatabaseServer::instance()
{
	return Firebird::interfaceAlloc<SecurityDatabaseServerInstance>();
}

Result SecurityDatabaseServerInstance::startAuthentication(Firebird::Status* status,
											  bool isService, const char*,
											  const unsigned char* dpb, unsigned int dpbSize,
											  WriterInterface* writerInterface)
{
	try
	{
		ClumpletReader rdr(isService ? ClumpletReader::spbList : ClumpletReader::dpbList, dpb, dpbSize);
		return SecurityDatabase::verify(writerInterface, rdr);
	}
	catch (const Firebird::Exception& ex)
	{
		ex.stuffException(status);
		return AUTH_FAILED;
	}
}

Result SecurityDatabaseServerInstance::contAuthentication(Firebird::Status*,
											  WriterInterface* /*writerInterface*/,
											  const unsigned char* /*data*/, unsigned int /*size*/)
{
	return AUTH_FAILED;
}

void SecurityDatabaseServerInstance::getData(const unsigned char** data, unsigned short* dataSize)
{
	*data = NULL;
	*dataSize = 0;
}

void SecurityDatabaseServerInstance::release()
{
	interfaceFree(this);
}

} // namespace Auth
