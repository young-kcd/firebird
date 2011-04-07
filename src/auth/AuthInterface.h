/*
 *	PROGRAM:		Firebird authentication
 *	MODULE:			AuthInterface.h
 *	DESCRIPTION:	Interfaces, used by authentication plugins
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
 *  Copyright (c) 2010 Alex Peshkov <peshkoff at mail.ru>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 *
 *
 */

#ifndef FB_AUTH_INTERFACE
#define FB_AUTH_INTERFACE

#include "FirebirdPluginApi.h"

namespace Firebird {
class IStatus;
}

namespace Auth {

enum Result {AUTH_SUCCESS, AUTH_CONTINUE, AUTH_FAILED, AUTH_MORE_DATA};

class WriterInterface : public Firebird::IDisposable
{
public:
	virtual void FB_CARG reset() = 0;
	virtual void FB_CARG add(const char* user, const char* method, const char* details) = 0;
};

class DpbInterface : public Firebird::IDisposable
{
public:
	virtual int FB_CARG find(UCHAR tag) = 0;
	virtual void FB_CARG add(UCHAR tag, const void* bytes, unsigned int count) = 0;
	virtual void FB_CARG drop() = 0;
};

class Server : public Firebird::IPluginBase
{
public:
	virtual Result FB_CARG startAuthentication(Firebird::IStatus* status, bool isService, const char* dbName,
									   const unsigned char* dpb, unsigned int dpbSize,
									   WriterInterface* writerInterface) = 0;
	virtual Result FB_CARG contAuthentication(Firebird::IStatus* status, WriterInterface* writerInterface,
									  const unsigned char* data, unsigned int size) = 0;
	virtual void FB_CARG getData(const unsigned char** data, unsigned short* dataSize) = 0;
};
#define FB_AUTH_SERVER_VERSION (FB_PLUGIN_VERSION + 3)

class Client : public Firebird::IPluginBase
{
public:
	virtual Result FB_CARG startAuthentication(Firebird::IStatus* status, bool isService, const char* dbName, DpbInterface* dpb) = 0;
	virtual Result FB_CARG contAuthentication(Firebird::IStatus* status, const unsigned char* data, unsigned int size) = 0;
	virtual void FB_CARG getData(const unsigned char** data, unsigned short* dataSize) = 0;
};
#define FB_AUTH_CLIENT_VERSION (FB_PLUGIN_VERSION + 3)

class UserField : public Firebird::IDisposable
{
public:
	virtual int FB_CARG entered() = 0;
	virtual int FB_CARG specified() = 0;
	virtual void FB_CARG setEntered(int newValue) = 0;
};

class CharUserField : public UserField
{
public:
	virtual const char* FB_CARG get() = 0;
	virtual void FB_CARG set(const char* newValue) = 0;
};

class IntUserField : public UserField
{
public:
	virtual int FB_CARG get() = 0;
	virtual void FB_CARG set(int newValue) = 0;
};

class User : public Firebird::IDisposable
{
public:
	virtual int FB_CARG operation() = 0;

	virtual CharUserField* FB_CARG userName() = 0;
	virtual CharUserField* FB_CARG password() = 0;

	virtual CharUserField* FB_CARG firstName() = 0;
	virtual CharUserField* FB_CARG lastName() = 0;
	virtual CharUserField* FB_CARG middleName() = 0;
	virtual CharUserField* FB_CARG groupName() = 0;

	virtual IntUserField* FB_CARG uid() = 0;
	virtual IntUserField* FB_CARG gid() = 0;
	virtual IntUserField* FB_CARG admin() = 0;

	virtual void FB_CARG clear() = 0;
};

class ListUsers : public Firebird::IDisposable
{
public:
	virtual void FB_CARG list(User* user) = 0;
};

class LogonInfo : public Firebird::IDisposable
{
public:
	virtual const char* FB_CARG name() = 0;
	virtual const char* FB_CARG role() = 0;
	virtual int FB_CARG forceAdmin() = 0;
	virtual const char* FB_CARG networkProtocol() = 0;
	virtual const char* FB_CARG remoteAddress() = 0;
};

class Management : public Firebird::IPluginBase
{
public:
	virtual void FB_CARG start(Firebird::IStatus* status, LogonInfo* logonInfo) = 0;
	virtual int FB_CARG execute(Firebird::IStatus* status, User* user, ListUsers* callback) = 0;
	virtual void FB_CARG commit(Firebird::IStatus* status) = 0;
	virtual void FB_CARG rollback(Firebird::IStatus* status) = 0;
};
#define FB_AUTH_MANAGE_VERSION (FB_PLUGIN_VERSION + 4)

} // namespace Auth


#endif // FB_AUTH_INTERFACE
