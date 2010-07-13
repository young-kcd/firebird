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
 *  The Original Code was created by Alex Peshkov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2008 Alex Peshkov <alexpeshkoff@users.sf.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../common/classes/ClumpletWriter.h"
#include "../jrd/UserManagement.h"
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/jrd_pwd.h"
#include "../jrd/tra.h"
#include "../jrd/msg_encode.h"
#include "../utilities/gsec/gsec.h"
#include "../utilities/gsec/secur_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/ini.h"
#include "gen/ids.h"

using namespace Jrd;
using namespace Firebird;


bool UsersTableScan::retrieveRecord(thread_db* tdbb, jrd_rel* relation,
									FB_UINT64 position, Record* record) const
{
	jrd_tra* const transaction = tdbb->getTransaction();
	return transaction->getUserManagement()->getList(tdbb, relation)->fetch(position, record);
}


UserManagement::UserManagement(jrd_tra* tra)
	: DataDump(*tra->tra_pool), database(0), transaction(0), buffer(0),
	  threadDbb(NULL), realUser(NULL), commands(*tra->tra_pool)
{
	Attachment* att = tra->tra_attachment;
	if (!att || !att->att_user)
	{
		(Arg::Gds(isc_random) << "Unknown user name for given transaction").raise();
	}
	if (!att->att_user->locksmith())
	{
		realUser = att->att_user->usr_user_name.c_str();
	}

	char securityDatabaseName[MAXPATHLEN];
	Auth::SecurityDatabase::getPath(securityDatabaseName);

	ClumpletWriter dpb(ClumpletReader::dpbList, MAX_DPB_SIZE);
	dpb.insertByte(isc_dpb_gsec_attach, TRUE);
	dpb.insertString(isc_dpb_trusted_auth, SYSDBA_USER_NAME);

	ISC_STATUS_ARRAY status;
	if (isc_attach_database(status, 0, securityDatabaseName, &database,
							dpb.getBufferLength(), reinterpret_cast<const char*>(dpb.getBuffer())))
	{
		status_exception::raise(status);
	}

	if (isc_start_transaction(status, &transaction, 1, &database, 0, NULL))
	{
		status_exception::raise(status);
	}
}

UserManagement::~UserManagement()
{
	for (ULONG i = 0; i < commands.getCount(); ++i)
	{
		delete commands[i];
	}
	commands.clear();

	ISC_STATUS_ARRAY status;
	if (transaction)
	{
		// Rollback transaction in security database ...
		if (isc_rollback_transaction(status, &transaction))
		{
			status_exception::raise(status);
		}
	}

	if (database)
	{
		if (isc_detach_database(status, &database))
		{
			status_exception::raise(status);
		}
	}

	delete buffer;
}

void UserManagement::commit()
{
	ISC_STATUS_ARRAY status;
	if (transaction)
	{
		// Commit transaction in security database
		if (isc_commit_transaction(status, &transaction))
		{
			status_exception::raise(status);
		}
		transaction = 0;
	}
}

USHORT UserManagement::put(internal_user_data* userData)
{
	const size_t ret = commands.getCount();
	if (ret > MAX_USHORT)
	{
		status_exception::raise(Arg::Gds(isc_random) << "Too many user management DDL per transaction)");
	}
	commands.push(userData);
	return ret;
}

void UserManagement::checkSecurityResult(int errcode, ISC_STATUS* status, const char* userName)
{
	if (!errcode)
	{
	    return;
	}

	Arg::StatusVector tmp;
	tmp << Arg::Gds(ENCODE_ISC_MSG(errcode, GSEC_MSG_FAC));
	if (errcode == GsecMsg22)
	{
		tmp << userName;
	}
	tmp.append(Arg::StatusVector(&status[0]));

	tmp.raise();
}

void UserManagement::execute(USHORT id)
{
	if (!transaction || !commands[id])
	{
		// Already executed
		return;
	}

	if (id >= commands.getCount())
	{
		status_exception::raise(Arg::Gds(isc_random) << "Wrong job id passed to UserManagement::execute()");
	}

#ifdef EMBEDDED
	// this restriction for embedded is temporarty and will gone when new build system will be introduced
	status_exception::raise(Arg::Gds(isc_random) << "User management not supported in embedded library");
#else
	ISC_STATUS_ARRAY status;
	int errcode = (!commands[id]->user_name_entered) ? GsecMsg18 :
		SECURITY_exec_line(status, realUser, database, transaction, commands[id], NULL, NULL);

	checkSecurityResult(errcode, status, commands[id]->user_name);

	delete commands[id];
	commands[id] = NULL;
#endif
}

void UserManagement::display(void* arg, const internal_user_data* u, bool/*firstTime*/)
{
	UserManagement* instance = static_cast<UserManagement*>(arg);
	fb_assert(instance && instance->buffer);

	instance->display(u);
}

void UserManagement::display(const internal_user_data* u)
{
	Record* record = buffer->getTempRecord();
	clearRecord(record);

	int attachment_charset = ttype_none;

	if (u->user_name_entered)
		putField(threadDbb, record,
				 DumpField(f_sec_user_name, VALUE_STRING, strlen(u->user_name), u->user_name),
				 attachment_charset);
	if (u->group_name_entered)
		putField(threadDbb, record,
				 DumpField(f_sec_group_name, VALUE_STRING, strlen(u->group_name), u->group_name),
				 attachment_charset);
	if (u->uid_entered)
		putField(threadDbb, record,
				 DumpField(f_sec_uid, VALUE_INTEGER, sizeof(int), &u->uid),
				 attachment_charset);
	if (u->gid_entered)
		putField(threadDbb, record,
				 DumpField(f_sec_gid, VALUE_INTEGER, sizeof(int), &u->gid),
				 attachment_charset);
	if (u->first_name_entered)
		putField(threadDbb, record,
				 DumpField(f_sec_first_name, VALUE_STRING, strlen(u->first_name), u->first_name),
				 attachment_charset);
	if (u->middle_name_entered)
		putField(threadDbb, record,
				 DumpField(f_sec_middle_name, VALUE_STRING, strlen(u->middle_name), u->middle_name),
				 attachment_charset);
	if (u->last_name_entered)
		putField(threadDbb, record,
				 DumpField(f_sec_last_name, VALUE_STRING, strlen(u->last_name), u->last_name),
				 attachment_charset);

	buffer->store(record);
}

RecordBuffer* UserManagement::getList(thread_db* tdbb, jrd_rel* relation)
{
#ifdef EMBEDDED
	// this restriction for embedded is temporarty and will gone when new build system will be introduced
	status_exception::raise(Arg::Gds(isc_random) << "User management not supported in embedded library");
	return 0; // keep the compiler happy
#else
	if (!buffer)
	{
		try
		{
			threadDbb = tdbb;

			fb_assert(relation && relation->rel_id == rel_sec_users);

			MET_scan_relation(threadDbb, relation);

			Format* const format = MET_current(threadDbb, relation);
			fb_assert(format);

			MemoryPool* const pool = threadDbb->getTransaction()->tra_pool;
			fb_assert(pool);

			buffer = FB_NEW(*pool) RecordBuffer(*pool, format);
			fb_assert(buffer);

			ISC_STATUS_ARRAY status;
			internal_user_data u;
			u.operation = DIS_OPER;
			int errcode = SECURITY_exec_line(status, realUser, database, transaction, &u, display, this);
			checkSecurityResult(errcode, status, u.user_name);
		}
		catch (const Exception&)
		{
			delete buffer;
			buffer = NULL;
			throw;
		}
	}

	return buffer;
#endif
}
