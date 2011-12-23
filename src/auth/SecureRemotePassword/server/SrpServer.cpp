/*
 *	PROGRAM:		Firebird authentication.
 *	MODULE:			SrpServer.cpp
 *	DESCRIPTION:	SPR authentication plugin.
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

#include "../auth/SecureRemotePassword/client/SrpClient.h"
#include "../auth/SecureRemotePassword/srp.h"
#include "../common/classes/ImplementHelper.h"
#include "../common/classes/ClumpletWriter.h"
#include "../auth/SecureRemotePassword/Message.h"

using namespace Firebird;

namespace {

const unsigned int INIT_KEY = ((~0) - 1);
unsigned int secDbKey = INIT_KEY;

const unsigned int SZ_LOGIN = 31;

MakeUpgradeInfo<> upInfo;

}


namespace Auth {

class SrpServer : public StdPlugin<IServer, FB_AUTH_CLIENT_VERSION>
{
public:
	explicit SrpServer(IPluginConfig* par)
		: server(NULL), data(getPool()), account(getPool()),
		  clientPubKey(getPool()), serverPubKey(getPool()),
		  verifier(getPool()), salt(getPool()), sessionKey(getPool()),
		  config(par->getFirebirdConf()), secDbName(NULL)
	{
		config->release();
	}

	// IServer implementation
	Result FB_CARG authenticate(IStatus* status, IServerBlock* sBlock, IWriter* writerInterface);
	Result FB_CARG getSessionKey(Firebird::IStatus* status,
								 const unsigned char** key, unsigned int* keyLen);
    int FB_CARG release();

private:
	RemotePassword* server;
	string data;
	string account;
	string clientPubKey, serverPubKey;
	UCharBuffer verifier;
	string salt;
	UCharBuffer sessionKey;
	RefPtr<IFirebirdConf> config;
	const char *secDbName;
};

Result SrpServer::authenticate(IStatus* status, IServerBlock* sb, IWriter* writerInterface)
{
	RefPtr<IAttachment> att;
	RefPtr<ITransaction> tra;
	RefPtr<IStatement> stmt;

	try
	{
		if (!server)
		{
			HANDSHAKE_DEBUG(fprintf(stderr, "Srv SRP1\n"));

			if (!sb->getLogin())
			{
				return AUTH_CONTINUE;
			}

			account = sb->getLogin();
			account.upper();

			unsigned int length;
			const unsigned char* val = sb->getData(&length);
			clientPubKey.assign(val, length);

			if (!clientPubKey.hasData())
			{
				return AUTH_CONTINUE;
			}

			// read salt and verifier from database
			// obviously we need something like attachments cache here
			if (secDbKey == INIT_KEY)
			{
				secDbKey = config->getKey("SecurityDatabase");
			}

			secDbName = config->asString(secDbKey);
			if (!(secDbName && secDbName[0]))
			{
				(Arg::Gds(isc_random) << "Error getting security database name").raise();
			}

			ClumpletWriter dpb(ClumpletReader::dpbList, MAX_DPB_SIZE);
			dpb.insertByte(isc_dpb_sec_attach, TRUE);
			const char* str = "SYSDBA";
			dpb.insertString(isc_dpb_user_name, str, strlen(str));

			RefPtr<IProvider> p(MasterInterfacePtr()->getDispatcher());
			p->release();

			att = p->attachDatabase(status, secDbName, dpb.getBufferLength(), dpb.getBuffer());
			if (!status->isSuccess())
			{
				status_exception::raise(status->get());
			}
			HANDSHAKE_DEBUG(fprintf(stderr, "Srv SRP1: attached sec db %s\n", secDbName));

			const UCHAR tpb[] =
			{
				isc_tpb_version1,
				isc_tpb_read,
				isc_tpb_read_committed,
				isc_tpb_rec_version,
				isc_tpb_wait
			};
			tra = att->startTransaction(status, sizeof(tpb), tpb);
			if (!status->isSuccess())
			{
				status_exception::raise(status->get());
			}
			HANDSHAKE_DEBUG(fprintf(stderr, "Srv SRP1: started transaction\n"));

			stmt = att->allocateStatement(status);
			if (!status->isSuccess())
			{
				status_exception::raise(status->get());
			}

			const char* sql = "SELECT PLG$VERIFIER, PLG$SALT FROM PLG$SRP WHERE PLG$USER_NAME = ?";
			stmt->prepare(status, tra, 0, sql, 3, 0);
			if (!status->isSuccess())
			{
				const ISC_STATUS* v = status->get();
				while (v[0] == isc_arg_gds)
				{
					if (v[1] == isc_dsql_relation_err)
					{
						Arg::Gds(isc_missing_data_structures).raise();
					}
					do
					{
						v += 2;
					} while (v[0] != isc_arg_warning && v[0] != isc_arg_gds && v[0] != isc_arg_end);
				}
				status_exception::raise(status->get());
			}

			Message par;
			Field <VarChar<SZ_LOGIN> > login(par);
			login() = account.c_str();
			login.null() = 0;
			HANDSHAKE_DEBUG(fprintf(stderr, "Srv SRP1: Ready to run statement with login '%s'\n", account.c_str()));

			Message dat;
			Field <VarChar<RemotePassword::SRP_VERIFIER_SIZE> > verify(dat);
			Field <VarChar<RemotePassword::SRP_SALT_SIZE> > slt(dat);
			dat.ready();

			stmt->execute(status, tra, 0, &par, &dat);
			if (!status->isSuccess())
			{
				status_exception::raise(status->get());
			}
			HANDSHAKE_DEBUG(fprintf(stderr, "Srv SRP1: Executed statement\n"));

			stmt->free(status, DSQL_drop);
			if (!status->isSuccess())
			{
				status_exception::raise(status->get());
			}
			stmt = NULL;

			tra->rollback(status);
			if (!status->isSuccess())
			{
				status_exception::raise(status->get());
			}
			tra = NULL;

			att->detach(status);		// It will close statement
			if (!status->isSuccess())
			{
				status_exception::raise(status->get());
			}
			att = NULL;

			server = new RemotePassword;

			verifier.assign(reinterpret_cast<UCHAR*>(verify().data), RemotePassword::SRP_VERIFIER_SIZE);
			dumpIt("Srv: verifier", verifier);
			UCharBuffer s;
			s.assign(reinterpret_cast<UCHAR*>(slt().data), RemotePassword::SRP_SALT_SIZE);
			BigInteger(s).getText(salt);
			dumpIt("Srv: salt", salt);
			server->genServerKey(serverPubKey, verifier);

			// Ready to prepare data for client and calculate session key
			data = "";
			fb_assert(salt.length() <= RemotePassword::SRP_SALT_SIZE * 2);
			data += char(salt.length());
			data += char(salt.length() >> 8);
			data.append(salt);
			fb_assert(serverPubKey.length() <= RemotePassword::SRP_KEY_SIZE * 2);
			data += char(serverPubKey.length());
			data += char(serverPubKey.length() >> 8);
			data.append(serverPubKey);
			dumpIt("Srv: serverPubKey", serverPubKey);
			dumpIt("Srv: data", data);
			sb->putData(data.length(), data.c_str());

			// Will be used to produce ISessionKey
			dumpIt("Srv: clientPubKey", clientPubKey);
			server->serverSessionKey(sessionKey, clientPubKey.c_str(), verifier);
			dumpIt("Srv: sessionKey", sessionKey);

			return AUTH_MORE_DATA;
		}

		unsigned int length;
		const unsigned char* val = sb->getData(&length);
		HANDSHAKE_DEBUG(fprintf(stderr, "Srv SRP2, data length is %d\n", length));
		string proof;
		proof.assign(val, length);
		BigInteger clientProof(proof.c_str());
		BigInteger serverProof = server->clientProof(account.c_str(), salt.c_str(), sessionKey);
		if (clientProof == serverProof)
		{
			MasterInterfacePtr()->upgradeInterface(writerInterface, FB_AUTH_WRITER_VERSION, upInfo);
			writerInterface->add(account.c_str());
			writerInterface->setAttribute(AuthReader::AUTH_SECURE_DB, secDbName);
			return AUTH_SUCCESS;
		}
	}
	catch (const Exception& ex)
	{
		if (stmt)
		{
			stmt->free(status, DSQL_drop);
		}
		if (tra)
		{
			tra->rollback(status);
		}
		if (att)
		{
			att->detach(status);
		}

		status->init();
		ex.stuffException(status);
	}

	return AUTH_FAILED;
}

Result SrpServer::getSessionKey(Firebird::IStatus*,
								const unsigned char** key, unsigned int* keyLen)
{
	if (!sessionKey.hasData())
	{
		return AUTH_MORE_DATA;
	}

	*key = sessionKey.begin();
	*keyLen = sessionKey.getCount();

	return AUTH_SUCCESS;
}

int SrpServer::release()
{
	if (--refCounter == 0)
	{
		delete this;
		return 0;
	}
	return 1;
}

namespace
{
	SimpleFactory<SrpServer> factory;
}

void registerSrpServer(IPluginManager* iPlugin)
{
	iPlugin->registerPluginFactory(PluginType::AuthServer, RemotePassword::plugName, &factory);
}

} // namespace Auth

