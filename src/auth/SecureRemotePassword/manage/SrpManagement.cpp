/*
 *	PROGRAM:		Firebird authentication
 *	MODULE:			SrpManagement.cpp
 *	DESCRIPTION:	Manages security database with SRP
 *
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
 *  Copyright (c) 2011 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"

#include "../common/classes/ImplementHelper.h"
#include "../common/classes/ClumpletWriter.h"
#include "../common/StatusHolder.h"
#include "firebird/Auth.h"
#include "../auth/SecureRemotePassword/srp.h"
#include "../jrd/constants.h"
#include "../utilities/gsec/gsec.h"
#include "../auth/SecureRemotePassword/Message.h"
#include "../common/classes/auto.h"

namespace {

Firebird::MakeUpgradeInfo<> upInfo;
const unsigned int INIT_KEY = ((~0) - 1);
unsigned int secDbKey = INIT_KEY;

const unsigned int SZ_LOGIN = 31;
const unsigned int SZ_NAME = 31;
typedef Field<Varying> Varfield;
typedef Field<Text> Name;

void setField(Name& to, Auth::ICharUserField* from)
{
	if (from->entered())
	{
		to = from->get();
	}
	else
	{
		to.null = FB_TRUE;
	}
}


void allocField(Auth::ICharUserField* value, Firebird::string& update, const char* name)
{
	if (value->entered() || value->specified())
	{
		update += ' ';
		update += name;
		update += "=?,";
	}
}

void allocField(Firebird::AutoPtr<Name>& field, Message& up, Auth::ICharUserField* value, const char* name)
{
	if (value->entered() || value->specified())
	{
		field = new Name(up);
	}
}

void assignField(Firebird::AutoPtr<Name>& field, Auth::ICharUserField* name)
{
	if (field.hasData())
	{
		if (name->entered())
		{
			*field = name->get();
			field->null = FB_FALSE;
		}
		else
		{
			fb_assert(name->specified());
			field->null = FB_TRUE;
		}
	}
}

void listField(Auth::ICharUserField* to, Name& from)
{
	to->setEntered(from.null ? 0 : 1);
	if (!from.null)
	{
		to->set(from);
	}
}

}

namespace Auth {

class SrpManagement : public Firebird::StdPlugin<IManagement, FB_AUTH_MANAGE_VERSION>
{
public:
	explicit SrpManagement(Firebird::IPluginConfig* par)
		: config(par->getFirebirdConf()), upCount(0), delCount(0)
	{
		config->release();
	}

	void prepareDataStructures()
	{
		const char* script[] = {
			"CREATE TABLE PLG$SRP (PLG$USER_NAME SEC$USER_NAME NOT NULL PRIMARY KEY, "
			"PLG$VERIFIER VARCHAR(128) CHARACTER SET OCTETS NOT NULL, "
			"PLG$SALT VARCHAR(32) CHARACTER SET OCTETS NOT NULL, "
			"PLG$COMMENT RDB$DESCRIPTION, PLG$FIRST SEC$NAME_PART, "
			"PLG$MIDDLE SEC$NAME_PART, PLG$LAST SEC$NAME_PART)"
			,
			"CREATE VIEW PLG$SRP_VIEW AS "
			"SELECT PLG$USER_NAME, PLG$VERIFIER, PLG$SALT, PLG$COMMENT, PLG$FIRST, PLG$MIDDLE, PLG$LAST "
			"FROM PLG$SRP WHERE CURRENT_USER = 'SYSDBA' OR CURRENT_ROLE = 'RDB$ADMIN' OR CURRENT_USER = PLG$SRP.PLG$USER_NAME"
			,
			"GRANT ALL ON PLG$SRP to VIEW PLG$SRP_VIEW"
			,
			"GRANT SELECT ON PLG$SRP_VIEW to PUBLIC"
			,
			"GRANT UPDATE(PLG$VERIFIER, PLG$SALT, PLG$FIRST, PLG$MIDDLE, PLG$LAST) ON PLG$SRP_VIEW TO PUBLIC"
			,
			NULL
		};

		Firebird::LocalStatus s;
		Firebird::RefPtr<Firebird::ITransaction> ddlTran(att->startTransaction(&s, 0, NULL));

		try
		{
			for (const char** sql = script; *sql; ++sql)
			{
				att->execute(&s, ddlTran, 0, *sql, 3, NULL, NULL, NULL, NULL);
				check(&s);
			}

			ddlTran->commit(&s);
			check(&s);
		}
		catch (const Firebird::Exception&)
		{
			if (ddlTran)
			{
				ddlTran->rollback(&s);
			}
			throw;
		}
	}

	// IManagement implementation
	void FB_CARG start(Firebird::IStatus* status, ILogonInfo* logonInfo)
	{
		try
		{
			Firebird::MasterInterfacePtr()->upgradeInterface(logonInfo, FB_AUTH_LOGON_INFO_VERSION, upInfo);

			status->init();

			if (att)
			{
				(Firebird::Arg::Gds(isc_random) << "Database is already attached in SRP").raise();
			}

			if (secDbKey == INIT_KEY)
			{
				secDbKey = config->getKey("SecurityDatabase");
			}
			const char* secDbName = config->asString(secDbKey);

			if (!(secDbName && secDbName[0]))
			{
				(Firebird::Arg::Gds(isc_random) << "Error getting security database name").raise();
			}

			Firebird::ClumpletWriter dpb(Firebird::ClumpletReader::dpbList, MAX_DPB_SIZE);
			dpb.insertByte(isc_dpb_gsec_attach, TRUE);

			const unsigned char* authBlock;
			unsigned int authBlockSize = logonInfo->authBlock(&authBlock);

			if (authBlockSize)
				dpb.insertBytes(isc_dpb_auth_block, authBlock, authBlockSize);
			else
			{
				const char* str = logonInfo->name();
				if (str && str[0])
					dpb.insertString(isc_dpb_trusted_auth, str, strlen(str));

				str = logonInfo->role();

				if (str && str[0])
					dpb.insertString(isc_dpb_sql_role_name, str, strlen(str));
				else if (logonInfo->forceAdmin())
					dpb.insertString(isc_dpb_trusted_role, ADMIN_ROLE, strlen(ADMIN_ROLE));
			}

			Firebird::DispatcherPtr p;
			att = p->attachDatabase(status, secDbName, dpb.getBufferLength(), dpb.getBuffer());
			check(status);

			tra = att->startTransaction(status, 0, NULL);
			check(status);
		}
		catch (const Firebird::Exception& ex)
		{
			ex.stuffException(status);

			if (att)
			{
				// detach from database
				Firebird::LocalStatus lStatus;
				att->detach(&lStatus);
				att = NULL;
			}
		}
	}

	int FB_CARG execute(Firebird::IStatus* status, IUser* user, IListUsers* callback)
	{
		try
		{
			if (callback)
			{
				Firebird::MasterInterfacePtr()->upgradeInterface(callback, FB_AUTH_LIST_USERS_VERSION, upInfo);
			}
			Firebird::MasterInterfacePtr()->upgradeInterface(user, FB_AUTH_USER_VERSION, upInfo);

			status->init();

			fb_assert(att);
			fb_assert(tra);

			switch(user->operation())
			{
			case MAP_DROP_OPER:
			case MAP_SET_OPER:
				{
					Firebird::string sql;
					sql.printf("ALTER ROLE RDB$ADMIN %s AUTO ADMIN MAPPING",
						user->operation() == MAP_SET_OPER ? "SET" : "DROP");
					att->execute(status, tra, sql.length(), sql.c_str(), 3, NULL, NULL, NULL, NULL);
					check(status);
				}
				break;

			case ADD_OPER:
				{
					const char* insert =
						"INSERT INTO plg$srp_view(PLG$USER_NAME, PLG$VERIFIER, PLG$SALT, PLG$FIRST, PLG$MIDDLE, PLG$LAST) "
						"VALUES(?, ?, ?, ?, ?, ?)";

					Firebird::IStatement* stmt = NULL;
					try
					{
						for (unsigned repeat = 0; ; ++repeat)
						{
							stmt = att->prepare(status, tra, 0, insert, 3, Firebird::IStatement::PREPARE_PREFETCH_METADATA);
							if (status->isSuccess())
							{
								break;
							}
							else if (repeat > 0)
							{
								Firebird::status_exception::raise(status->get());
							}

							const ISC_STATUS* v = status->get();
							while (v[0] == isc_arg_gds)
							{
								if (v[1] == isc_dsql_relation_err)
								{
									prepareDataStructures();
									tra->commit(status);
									check(status);
									tra = att->startTransaction(status, 0, NULL);
									check(status);
									break;
								}

								do
								{
									v += 2;
								} while (v[0] != isc_arg_warning && v[0] != isc_arg_gds && v[0] != isc_arg_end);
							}
						}

						fb_assert(stmt);

						Meta im(stmt, false);
						Message add(im);
						Name login(add);
						Varfield verifier(add), slt(add);
						Name first(add), middle(add), last(add);

						setField(login, user->userName());
						setField(first, user->firstName());
						setField(middle, user->middleName());
						setField(last, user->lastName());

#if SRP_DEBUG > 1
						Firebird::BigInteger salt("02E268803000000079A478A700000002D1A6979000000026E1601C000000054F");
#else
						Firebird::BigInteger salt;
						salt.random(RemotePassword::SRP_SALT_SIZE);
#endif
						Firebird::UCharBuffer s;
						salt.getBytes(s);
						slt.set(s.getCount(), s.begin());

						dumpIt("salt", s);
#if SRP_DEBUG > 0
						fprintf(stderr, ">%s< >%s<\n", user->userName()->get(), user->password()->get());
#endif
						Firebird::string s1;
						salt.getText(s1);
						server.computeVerifier(user->userName()->get(), s1, user->password()->get()).getBytes(s);
						dumpIt("verifier", s);
						verifier.set(s.getCount(), s.begin());

						stmt->execute(status, tra, add.metadata, add.buffer, NULL, NULL);
						check(status);

						stmt->free(status);
						check(status);
					}
					catch (const Firebird::Exception&)
					{
						if (stmt)
						{
							stmt->release();
						}
						throw;
					}
				}
				break;

			case MOD_OPER:
				{
					Firebird::string update = "UPDATE plg$srp_view SET ";

					Firebird::AutoPtr<Varfield> verifier, slt;
					if (user->password()->entered())
					{
						update += "PLG$VERIFIER=?,PLG$SALT=?,";
					}

					Firebird::AutoPtr<Name> first, middle, last;
					allocField(user->firstName(), update, "PLG$FIRST");
					allocField(user->middleName(), update, "PLG$MIDDLE");
					allocField(user->lastName(), update, "PLG$LAST");

					if (update[update.length() - 1] != ',')
					{
						return 0;
					}
					update.rtrim(",");
					update += " WHERE PLG$USER_NAME=?";

					Firebird::IStatement* stmt = NULL;
					try
					{
						stmt = att->prepare(status, tra, 0, update.c_str(), 3, Firebird::IStatement::PREPARE_PREFETCH_METADATA);
						check(status);

						Meta im(stmt, false);
						Message up(im);

						if (user->password()->entered())
						{
							verifier = new Varfield(up);
							slt = new Varfield(up);
#if SRP_DEBUG > 1
							Firebird::BigInteger salt("02E268803000000079A478A700000002D1A6979000000026E1601C000000054F");
#else
							Firebird::BigInteger salt;
							salt.random(RemotePassword::SRP_SALT_SIZE);
#endif
							Firebird::UCharBuffer s;
							salt.getBytes(s);
							slt->set(s.getCount(), s.begin());

							dumpIt("salt", s);
#if SRP_DEBUG > 0
							fprintf(stderr, ">%s< >%s<\n", user->userName()->get(), user->password()->get());
#endif
							Firebird::string s1;
							salt.getText(s1);
							server.computeVerifier(user->userName()->get(), s1, user->password()->get()).getBytes(s);
							dumpIt("verifier", s);
							verifier->set(s.getCount(), s.begin());
						}

						allocField(first, up, user->firstName(), "PLG$FIRST");
						allocField(middle, up, user->middleName(), "PLG$MIDDLE");
						allocField(last, up, user->lastName(), "PLG$LAST");

						Name login(up);

						assignField(first, user->firstName());
						assignField(middle, user->middleName());
						assignField(last, user->lastName());
						setField(login, user->userName());

						stmt->execute(status, tra, up.metadata, up.buffer, NULL, NULL);
						check(status);

						if (!checkCount(status, &upCount, isc_info_update_count))
						{
							return -1;
						}

						stmt->free(status);
						check(status);
					}
					catch (const Firebird::Exception&)
					{
						if (stmt)
						{
							stmt->release();
						}
						throw;
					}
				}
				break;

			case DEL_OPER:
				{
					const char* del = "DELETE FROM plg$srp_view WHERE PLG$USER_NAME=?";
					Firebird::IStatement* stmt = NULL;
					try
					{
						stmt = att->prepare(status, tra, 0, del, 3, Firebird::IStatement::PREPARE_PREFETCH_METADATA);
						check(status);

						Meta im(stmt, false);
						Message dl(im);
						Name login(dl);
						setField(login, user->userName());

						stmt->execute(status, tra, dl.metadata, dl.buffer, NULL, NULL);
						check(status);

						if (!checkCount(status, &delCount, isc_info_delete_count))
						{
							return -1;
						}

						stmt->free(status);
						check(status);
					}
					catch (const Firebird::Exception&)
					{
						if (stmt)
						{
							stmt->release();
						}
						throw;
					}
				}
				break;

			case OLD_DIS_OPER:
			case DIS_OPER:
				{
					user->uid()->setEntered(0);
					user->gid()->setEntered(0);
					user->groupName()->setEntered(0);
					user->password()->setEntered(0);

					Firebird::string disp =	"SELECT PLG$USER_NAME, PLG$FIRST, PLG$MIDDLE, PLG$LAST, "
											"	CASE WHEN RDB$RELATION_NAME IS NULL THEN 0 ELSE 1 END "
											"FROM PLG$SRP_VIEW LEFT JOIN RDB$USER_PRIVILEGES "
											"	ON PLG$SRP_VIEW.PLG$USER_NAME = RDB$USER_PRIVILEGES.RDB$USER "
											"		AND RDB$RELATION_NAME = 'RDB$ADMIN' "
											"		AND RDB$PRIVILEGE = 'M' ";
					if (user->userName()->entered())
					{
						disp += " WHERE PLG$USER_NAME = ?";
					}

					Firebird::IStatement* stmt = NULL;
					Firebird::IResultSet* rs = NULL;
					try
					{
						stmt = att->prepare(status, tra, 0, disp.c_str(), 3,
							Firebird::IStatement::PREPARE_PREFETCH_METADATA);
						check(status);

						Meta om(stmt, true);
						Message di(om);
						Name login(di);
						Name first(di), middle(di), last(di);
						Field<SLONG> admin(di);

						Firebird::AutoPtr<Message> par;
						if (user->userName()->entered())
						{
							Meta im(stmt, false);
							par = new Message(im);
							Name login(*par);
							setField(login, user->userName());
						}

						rs = stmt->openCursor(status, tra, (par ? par->metadata : NULL),
							(par ? par->buffer : NULL), om);
						check(status);

						while (rs->fetch(status, di.buffer))
						{
							check(status);

							listField(user->userName(), login);
							listField(user->firstName(), first);
							listField(user->middleName(), middle);
							listField(user->lastName(), last);
							user->admin()->set(admin);

							callback->list(user);
						}
						check(status);

						stmt->free(status);
						check(status);
					}
					catch (const Firebird::Exception&)
					{
						if (stmt)
						{
							stmt->release();
						}
						throw;
					}
				}
				break;

			default:
				return -1;
			}
		}
		catch (const Firebird::Exception& ex)
		{
			ex.stuffException(status);
			return -1;
		}

		return 0;
	}

	void FB_CARG commit(Firebird::IStatus* status)
	{
		if (tra)
		{
			tra->commit(status);
			if (status->isSuccess())
			{
				tra = NULL;
			}
		}
	}

	void FB_CARG rollback(Firebird::IStatus* status)
	{
		if (tra)
		{
			tra->rollback(status);
			if (status->isSuccess())
			{
				tra = NULL;
			}
		}
	}

	int FB_CARG release()
	{
		if (--refCounter == 0)
		{
			Firebird::LocalStatus status;
			rollback(&status);
			if (att)
			{
				att->detach(&status);
				if (status.isSuccess())
				{
					att = NULL;
				}
			}

			if (tra)
			{
				tra->release();
			}

			if (att)
			{
				att->release();
			}

			delete this;
			return 0;
		}

		return 1;
	}

private:
	Firebird::RefPtr<Firebird::IFirebirdConf> config;
	Firebird::RefPtr<Firebird::IAttachment> att;
	Firebird::RefPtr<Firebird::ITransaction> tra;
	RemotePassword server;
	int upCount, delCount;

	bool checkCount(Firebird::IStatus* status, int* count, UCHAR item)
	{
		unsigned char buffer[100];
		att->getInfo(status, 1, &item, sizeof(buffer), buffer);
		check(status);

		if (gds__vax_integer(buffer + 1, 2) != 6)
		{
			return false;
		}

		int newCount = gds__vax_integer(buffer + 5, 4);
		int oldCount = *count;
		*count = newCount;
		return newCount == oldCount + 1;
	}

	void check(Firebird::IStatus* status)
	{
		if (!status->isSuccess())
		{
			Firebird::status_exception::raise(status->get());
		}
	}
};

// register plugin
static Firebird::SimpleFactory<Auth::SrpManagement> factory;

} // namespace Auth

extern "C" void FB_PLUGIN_ENTRY_POINT(Firebird::IMaster* master)
{
	Firebird::CachedMasterInterface::set(master);
	Firebird::PluginManagerInterfacePtr()->registerPluginFactory(Firebird::PluginType::AuthUserManagement, Auth::RemotePassword::plugName, &Auth::factory);
	Firebird::myModule->registerMe();
}
