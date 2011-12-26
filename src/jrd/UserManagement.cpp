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
#include "../common/common.h"
#include "../jrd/jrd.h"
#include "../jrd/tra.h"
#include "../jrd/msg_encode.h"
#include "../utilities/gsec/gsec.h"
#include "../common/security.h"
#include "../jrd/met_proto.h"
#include "../jrd/ini.h"
#include "gen/ids.h"

using namespace Jrd;
using namespace Firebird;

static MakeUpgradeInfo<> upInfo;

const Format* UsersTableScan::getFormat(thread_db* tdbb, jrd_rel* relation) const
{
	jrd_tra* const transaction = tdbb->getTransaction();
	return transaction->getUserManagement()->getList(tdbb, relation)->getFormat();
}


bool UsersTableScan::retrieveRecord(thread_db* tdbb, jrd_rel* relation,
									FB_UINT64 position, Record* record) const
{
	jrd_tra* const transaction = tdbb->getTransaction();
	return transaction->getUserManagement()->getList(tdbb, relation)->fetch(position, record);
}


UserManagement::UserManagement(jrd_tra* tra)
	: DataDump(*tra->tra_pool),
	  buffer(0),
	  threadDbb(NULL), commands(*tra->tra_pool),
	  display(this),
	  manager(NULL)
{
	Attachment* att = tra->tra_attachment;
	if (!att || !att->att_user)
	{
		(Arg::Gds(isc_random) << "Unknown user name for given transaction").raise();
	}

	fb_assert(att->att_database && att->att_database->dbb_config.hasData());
	Auth::Get getPlugin(att->att_database->dbb_config);
	manager = getPlugin.plugin();
	fb_assert(manager);
	manager->addRef();

	class UserIdInfo : public AutoIface<Auth::ILogonInfo, FB_AUTH_LOGON_INFO_VERSION>
	{
	public:
		explicit UserIdInfo(const Attachment* pAtt)
			: att(pAtt)
		{ }

		// ILogonInfo implementation
		const char* FB_CARG name()
		{
			return att->att_user->usr_user_name.c_str();
		}

		const char* FB_CARG role()
		{
			return att->att_user->usr_sql_role_name.c_str();
		}

		int FB_CARG forceAdmin()
		{
			return ((att->att_user->usr_flags & USR_trole) &&
					att->att_user->usr_sql_role_name == ADMIN_ROLE) ? 1 : 0;
		}

		const char* FB_CARG networkProtocol()
		{
			return att->att_network_protocol.c_str();
		}

		const char* FB_CARG remoteAddress()
		{
			return att->att_remote_address.c_str();
		}

		unsigned int FB_CARG authBlock(const unsigned char** bytes)
		{
			const Auth::UserData::AuthenticationBlock& aBlock = att->att_user->usr_auth_block;
			*bytes = aBlock.getCount() ? aBlock.begin() : NULL;
			return aBlock.getCount();
		}

	private:
		const Attachment* att;
	};

	LocalStatus status;
	UserIdInfo idInfo(att);
	manager->start(&status, &idInfo);

	if (!status.isSuccess())
	{
		status_exception::raise(status.get());
	}
}

UserManagement::~UserManagement()
{
	for (ULONG i = 0; i < commands.getCount(); ++i)
	{
		delete commands[i];
	}
	commands.clear();

	delete buffer;

	if (manager)
	{
		LocalStatus status;
		manager->rollback(&status);
		PluginManagerInterfacePtr()->releasePlugin(manager);
		manager = NULL;

		if (!status.isSuccess())
		{
			status_exception::raise(status.get());
		}
	}
}

void UserManagement::commit()
{
	if (manager)
	{
		LocalStatus status;
		manager->commit(&status);

		if (!status.isSuccess())
		{
			status_exception::raise(status.get());
		}

		PluginManagerInterfacePtr()->releasePlugin(manager);
		manager = NULL;
	}
}

USHORT UserManagement::put(Auth::UserData* userData)
{
	const size_t ret = commands.getCount();
	if (ret > MAX_USHORT)
	{
		status_exception::raise(Arg::Gds(isc_random) << "Too many user management DDL per transaction)");
	}
	commands.push(userData);
	return ret;
}

void UserManagement::checkSecurityResult(int errcode, Firebird::IStatus* status, const char* userName, Auth::IUser* user)
{
	if (!errcode)
	{
	    return;
	}
	errcode = Auth::setGsecCode(errcode, user);

	Arg::StatusVector tmp;
	tmp << Arg::Gds(ENCODE_ISC_MSG(errcode, GSEC_MSG_FAC));
	if (errcode == GsecMsg22)
	{
		tmp << userName;
	}
	tmp.append(Arg::StatusVector(status->get()));

	tmp.raise();
}

void UserManagement::execute(USHORT id)
{
	if (!(manager && commands[id]))
	{
		// Already executed
		return;
	}

	if (id >= commands.getCount())
	{
		status_exception::raise(Arg::Gds(isc_random) << "Wrong job id passed to UserManagement::execute()");
	}

	LocalStatus status;
	int errcode = manager->execute(&status, commands[id], NULL);
	checkSecurityResult(errcode, &status, commands[id]->userName()->get(), commands[id]);

	delete commands[id];
	commands[id] = NULL;
}

void UserManagement::Display::list(Auth::IUser* u)
{
	MasterInterfacePtr()->upgradeInterface(u, FB_AUTH_USER_VERSION, upInfo);
	userManagement->list(u);
}

void UserManagement::list(Auth::IUser* u)
{
	Record* record = buffer->getTempRecord();
	clearRecord(record);

	int attachment_charset = ttype_none;

	if (u->userName()->entered())
	{
		putField(threadDbb, record,
				 DumpField(f_sec_user_name, VALUE_STRING, strlen(u->userName()->get()), u->userName()->get()),
				 attachment_charset);
	}

	if (u->groupName()->entered())
	{
		putField(threadDbb, record,
				 DumpField(f_sec_group_name, VALUE_STRING, strlen(u->groupName()->get()), u->groupName()->get()),
				 attachment_charset);
	}

	if (u->uid()->entered())
	{
		int tmp = u->uid()->get();
		putField(threadDbb, record,
				 DumpField(f_sec_uid, VALUE_INTEGER, sizeof(int), &tmp),
				 attachment_charset);
	}

	if (u->gid()->entered())
	{
		int tmp = u->gid()->get();
		putField(threadDbb, record,
				 DumpField(f_sec_gid, VALUE_INTEGER, sizeof(int), &tmp),
				 attachment_charset);
	}

	if (u->firstName()->entered())
	{
		putField(threadDbb, record,
				 DumpField(f_sec_first_name, VALUE_STRING, strlen(u->firstName()->get()), u->firstName()->get()),
				 attachment_charset);
	}

	if (u->middleName()->entered())
	{
		putField(threadDbb, record,
				 DumpField(f_sec_middle_name, VALUE_STRING, strlen(u->middleName()->get()), u->middleName()->get()),
				 attachment_charset);
	}

	if (u->lastName()->entered())
	{
		putField(threadDbb, record,
				 DumpField(f_sec_last_name, VALUE_STRING, strlen(u->lastName()->get()), u->lastName()->get()),
				 attachment_charset);
	}

	buffer->store(record);
}

RecordBuffer* UserManagement::getList(thread_db* tdbb, jrd_rel* relation)
{
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

			LocalStatus status;
			Auth::StackUserData u;
			u.op = DIS_OPER;
			int errcode = manager->execute(&status, &u, &display);
			checkSecurityResult(errcode, &status, "Unknown", &u);
		}
		catch (const Exception&)
		{
			delete buffer;
			buffer = NULL;
			throw;
		}
	}

	return buffer;
}
