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

#include "firebird/Plugin.h"

namespace Firebird {
class IStatus;
}

namespace Auth {

enum Result {AUTH_SUCCESS, AUTH_CONTINUE, AUTH_FAILED, AUTH_MORE_DATA};

class IWriter : public Firebird::IVersioned
{
public:
	virtual void FB_CARG reset() = 0;
	virtual void FB_CARG add(const char* name) = 0;
	virtual void FB_CARG setAttribute(unsigned char tag, const char* value) = 0;
};
#define FB_AUTH_WRITER_VERSION (FB_VERSIONED_VERSION + 3)

class IClumplets : public Firebird::IVersioned
{
public:
	virtual int FB_CARG find(UCHAR tag) = 0;
	virtual void FB_CARG add(UCHAR tag, const void* bytes, unsigned int count) = 0;
	virtual void FB_CARG drop() = 0;
	virtual const unsigned char* FB_CARG get(unsigned int* cntPtr) = 0;
};
#define FB_AUTH_DPB_READER_VERSION (FB_VERSIONED_VERSION + 4)

// This struct defines auth-related tags (including legacy ones) in parameter blocks
struct AuthTags
{
	UCHAR authBlock, trustedAuth, trustedRole;
	UCHAR service;	// non-zero if we work with service connection
};

class IServer : public Firebird::IPluginBase
{
public:
	virtual Result FB_CARG startAuthentication(Firebird::IStatus* status, const AuthTags* tags, IClumplets* dpb,
											   IWriter* writerInterface) = 0;
	virtual Result FB_CARG contAuthentication(Firebird::IStatus* status, const unsigned char* data, 
											  unsigned int size, IWriter* writerInterface) = 0;
	virtual void FB_CARG getData(const unsigned char** data, unsigned short* dataSize) = 0;
};
#define FB_AUTH_SERVER_VERSION (FB_PLUGIN_VERSION + 3)

class IClient : public Firebird::IPluginBase
{
public:
	virtual Result FB_CARG startAuthentication(Firebird::IStatus* status, const AuthTags* tags, IClumplets* dpb) = 0;
	virtual Result FB_CARG contAuthentication(Firebird::IStatus* status, const unsigned char* data, unsigned int size) = 0;
	virtual void FB_CARG getData(const unsigned char** data, unsigned short* dataSize) = 0;
};
#define FB_AUTH_CLIENT_VERSION (FB_PLUGIN_VERSION + 3)

class IUserField : public Firebird::IVersioned
{
public:
	virtual int FB_CARG entered() = 0;
	virtual int FB_CARG specified() = 0;
	virtual void FB_CARG setEntered(int newValue) = 0;
};
#define FB_AUTH_FIELD_VERSION (FB_VERSIONED_VERSION + 3)

class ICharUserField : public IUserField
{
public:
	virtual const char* FB_CARG get() = 0;
	virtual void FB_CARG set(const char* newValue) = 0;
};
#define FB_AUTH_CHAR_FIELD_VERSION (FB_AUTH_FIELD_VERSION + 2)

class IIntUserField : public IUserField
{
public:
	virtual int FB_CARG get() = 0;
	virtual void FB_CARG set(int newValue) = 0;
};
#define FB_AUTH_INT_FIELD_VERSION (FB_AUTH_FIELD_VERSION + 2)

class IUser : public Firebird::IVersioned
{
public:
	virtual int FB_CARG operation() = 0;

	virtual ICharUserField* FB_CARG userName() = 0;
	virtual ICharUserField* FB_CARG password() = 0;

	virtual ICharUserField* FB_CARG firstName() = 0;
	virtual ICharUserField* FB_CARG lastName() = 0;
	virtual ICharUserField* FB_CARG middleName() = 0;
	virtual ICharUserField* FB_CARG groupName() = 0;

	virtual IIntUserField* FB_CARG uid() = 0;
	virtual IIntUserField* FB_CARG gid() = 0;
	virtual IIntUserField* FB_CARG admin() = 0;

	virtual void FB_CARG clear() = 0;
};
#define FB_AUTH_USER_VERSION (FB_VERSIONED_VERSION + 11)

class IListUsers : public Firebird::IVersioned
{
public:
	virtual void FB_CARG list(IUser* user) = 0;
};
#define FB_AUTH_LIST_USERS_VERSION (FB_VERSIONED_VERSION + 1)

class ILogonInfo : public Firebird::IVersioned
{
public:
	virtual const char* FB_CARG name() = 0;
	virtual const char* FB_CARG role() = 0;
	virtual int FB_CARG forceAdmin() = 0;
	virtual const char* FB_CARG networkProtocol() = 0;
	virtual const char* FB_CARG remoteAddress() = 0;
	virtual unsigned int FB_CARG authBlock(const unsigned char** bytes) = 0;
};
#define FB_AUTH_LOGON_INFO_VERSION (FB_VERSIONED_VERSION + 6)

class IManagement : public Firebird::IPluginBase
{
public:
	virtual void FB_CARG start(Firebird::IStatus* status, ILogonInfo* logonInfo) = 0;
	virtual int FB_CARG execute(Firebird::IStatus* status, IUser* user, IListUsers* callback) = 0;
	virtual void FB_CARG commit(Firebird::IStatus* status) = 0;
	virtual void FB_CARG rollback(Firebird::IStatus* status) = 0;
};
#define FB_AUTH_MANAGE_VERSION (FB_PLUGIN_VERSION + 4)

} // namespace Auth


#endif // FB_AUTH_INTERFACE
